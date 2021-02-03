/*
 * cache-membuffer.c: in-memory caching for Subversion
 *
 * ====================================================================
 *    Licensed to the Apache Software Foundation (ASF) under one
 *    or more contributor license agreements.  See the NOTICE file
 *    distributed with this work for additional information
 *    regarding copyright ownership.  The ASF licenses this file
 *    to you under the Apache License, Version 2.0 (the
 *    "License"); you may not use this file except in compliance
 *    with the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing,
 *    software distributed under the License is distributed on an
 *    "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *    KIND, either express or implied.  See the License for the
 *    specific language governing permissions and limitations
 *    under the License.
 * ====================================================================
 */

#include <assert.h>
#include <apr_md5.h>
#include <apr_thread_rwlock.h>

#include "svn_pools.h"
#include "svn_checksum.h"
#include "svn_private_config.h"
#include "cache.h"
#include "svn_string.h"
#include "svn_sorts.h"  /* get the MIN macro */
#include "private/svn_atomic.h"
#include "private/svn_dep_compat.h"
#include "private/svn_mutex.h"
#include "private/svn_pseudo_md5.h"

/*
 * This svn_cache__t implementation actually consists of two parts:
 * a shared (per-process) singleton membuffer cache instance and shallow
 * svn_cache__t front-end instances that each use different key spaces.
 * For data management, they all forward to the singleton membuffer cache.
 *
 * A membuffer cache consists of two parts:
 *
 * 1. A linear data buffer containing cached items in a serialized
 *    representation. There may be arbitrary gaps between entries.
 *    This buffer is sub-devided into (currently two) cache levels.
 *
 * 2. A directory of cache entries. This is organized similar to CPU
 *    data caches: for every possible key, there is exactly one group
 *    of entries that may contain the header info for an item with
 *    that given key.  The result is a GROUP_SIZE+-way associative cache
 *    whose associativity can be dynamically increased.
 *
 * Only the start address of these two data parts are given as a native
 * pointer. All other references are expressed as offsets to these pointers.
 * With that design, it is relatively easy to share the same data structure
 * between different processes and / or to persist them on disk. These
 * out-of-process features have not been implemented, yet.
 *
 * Superficially, cache levels are being used as usual: insertion happens
 * into L1 and evictions will promote items to L2.  But their whole point
 * is a different one.  L1 uses a circular buffer, i.e. we have perfect
 * caching for the last N bytes where N is the size of L1.  L2 uses a more
 * elaborate scheme based on priorities and hit counts as described below.
 *
 * The data buffer usage information is implicitly given by the directory
 * entries. Every USED entry has a reference to the previous and the next
 * used dictionary entry and this double-linked list is ordered by the
 * offsets of their item data within the data buffer. So removing data,
 * for instance, is done simply by unlinking it from the chain, implicitly
 * marking the entry as well as the data buffer section previously
 * associated to it as unused.  First and last element of that chain are
 * being referenced from the respective cache level.
 *
 * Insertion can occur at only one, sliding position per cache level.  It is
 * marked by its offset in the data buffer and the index of the first used
 * entry at or behind that position.  If this gap is too small to accommodate
 * the new item, the insertion window is extended as described below. The new
 * entry will always be inserted at the bottom end of the window and since
 * the next used entry is known, properly sorted insertion is possible.
 *
 * To make the cache perform robustly in a wide range of usage scenarios,
 * L2 uses a randomized variant of LFU (see ensure_data_insertable_l2 for
 * details). Every item holds a read hit counter and there is a global read
 * hit counter. The more hits an entry has in relation to the average, the
 * more it is likely to be kept using a rand()-based condition. The test is
 * applied only to the entry following the insertion window. If it doesn't
 * get evicted, it is moved to the begin of that window and the window is
 * moved.
 *
 * Moreover, the entry's hits get halved to make that entry more likely to
 * be removed the next time the sliding insertion / removal window comes by.
 * As a result, frequently used entries are likely not to be dropped until
 * they get not used for a while. Also, even a cache thrashing situation
 * about 50% of the content survives every 50% of the cache being re-written
 * with new entries. For details on the fine-tuning involved, see the
 * comments in ensure_data_insertable_l2().
 *
 * Due to the randomized mapping of keys to entry groups, some groups may
 * overflow.  In that case, there are spare groups that can be chained to
 * an already used group to extend it.
 *
 * To limit the entry size and management overhead, not the actual item keys
 * but only their MD5-based hashes will be stored. This is reasonably safe
 * to do since users have only limited control over the full keys, even if
 * these contain folder paths. So, it is very hard to deliberately construct
 * colliding keys. Random checksum collisions can be shown to be extremely
 * unlikely.
 *
 * All access to the cached data needs to be serialized. Because we want
 * to scale well despite that bottleneck, we simply segment the cache into
 * a number of independent caches (segments). Items will be multiplexed based
 * on their hash key.
 */

/* APR's read-write lock implementation on Windows is horribly inefficient.
 * Even with very low contention a runtime overhead of 35% percent has been
 * measured for 'svn-bench null-export' over ra_serf.
 *
 * Use a simple mutex on Windows.  Because there is one mutex per segment,
 * large machines should (and usually can) be configured with large caches
 * such that read contention is kept low.  This is basically the situation
 * we had before 1.8.
 */
#ifdef WIN32
#  define USE_SIMPLE_MUTEX 1
#else
#  define USE_SIMPLE_MUTEX 0
#endif

/* For more efficient copy operations, let's align all data items properly.
 * Must be a power of 2.
 */
#define ITEM_ALIGNMENT 16

/* By default, don't create cache segments smaller than this value unless
 * the total cache size itself is smaller.
 */
#define DEFAULT_MIN_SEGMENT_SIZE APR_UINT64_C(0x2000000)

/* The minimum segment size we will allow for multi-segmented caches
 */
#define MIN_SEGMENT_SIZE APR_UINT64_C(0x10000)

/* The maximum number of segments allowed. Larger numbers reduce the size
 * of each segment, in turn reducing the max size of a cachable item.
 * Also, each segment gets its own lock object. The actual number supported
 * by the OS may therefore be lower and svn_cache__membuffer_cache_create
 * may return an error.
 */
#define MAX_SEGMENT_COUNT 0x10000

/* As of today, APR won't allocate chunks of 4GB or more. So, limit the
 * segment size to slightly below that.
 */
#define MAX_SEGMENT_SIZE APR_UINT64_C(0xffff0000)

/* We don't mark the initialization status for every group but initialize
 * a number of groups at once. That will allow for a very small init flags
 * vector that is likely to fit into the CPU caches even for fairly large
 * membuffer caches. For instance, the default of 32 means 8x32 groups per
 * byte, i.e. 8 flags/byte x 32 groups/flag x 8 entries/group x 40 index
 * bytes/entry x 8 cache bytes/index byte = 1kB init vector / 640MB cache.
 */
#define GROUP_INIT_GRANULARITY 32

/* Invalid index reference value. Equivalent to APR_UINT32_T(-1)
 */
#define NO_INDEX APR_UINT32_MAX

/* To save space in our group structure, we only use 32 bit size values
 * and, therefore, limit the size of each entry to just below 4GB.
 * Supporting larger items is not a good idea as the data transfer
 * to and from the cache would block other threads for a very long time.
 */
#define MAX_ITEM_SIZE ((apr_uint32_t)(0 - ITEM_ALIGNMENT))

/* A 16 byte key type. We use that to identify cache entries.
 * The notation as just two integer values will cause many compilers
 * to create better code.
 */
typedef apr_uint64_t entry_key_t[2];

/* The prefix passed to svn_cache__create_membuffer_cache() effectively
 * defines the type of all items stored by that cache instance. We'll take
 * the last 15 bytes + \0 as plaintext for easy identification by the dev.
 */
#define PREFIX_TAIL_LEN 16

/* Debugging / corruption detection support.
 * If you define this macro, the getter functions will performed expensive
 * checks on the item data, requested keys and entry types. If there is
 * a mismatch found in any of them when being compared with the values
 * remembered in the setter function, an error will be returned.
 */
#ifdef SVN_DEBUG_CACHE_MEMBUFFER

/* This record will be attached to any cache entry. It tracks item data
 * (content), key and type as hash values and is the baseline against which
 * the getters will compare their results to detect inconsistencies.
 */
typedef struct entry_tag_t
{
  /* MD5 checksum over the serialized item data.
   */
  unsigned char content_hash[APR_MD5_DIGESTSIZE];

  /* Hash value of the svn_cache_t instance that wrote the item
   * (i.e. a combination of type and repository)
   */
  unsigned char prefix_hash[APR_MD5_DIGESTSIZE];

  /* Note that this only covers the variable part of the key,
   * i.e. it will be different from the full key hash used for
   * cache indexing.
   */
  unsigned char key_hash[APR_MD5_DIGESTSIZE];

  /* Last letters from of the key in human readable format
   * (ends with the type identifier, e.g. "DAG")
   */
  char prefix_tail[PREFIX_TAIL_LEN];

  /* Length of the variable key part.
   */
  apr_size_t key_len;

} entry_tag_t;

/* Initialize all members of TAG except for the content hash.
 */
static svn_error_t *store_key_part(entry_tag_t *tag,
                                   entry_key_t prefix_hash,
                                   char *prefix_tail,
                                   const void *key,
                                   apr_size_t key_len,
                                   apr_pool_t *pool)
{
  svn_checksum_t *checksum;
  SVN_ERR(svn_checksum(&checksum,
                       svn_checksum_md5,
                       key,
                       key_len,
                       pool));

  memcpy(tag->prefix_hash, prefix_hash, sizeof(tag->prefix_hash));
  memcpy(tag->key_hash, checksum->digest, sizeof(tag->key_hash));
  memcpy(tag->prefix_tail, prefix_tail, sizeof(tag->prefix_tail));

  tag->key_len = key_len;

  return SVN_NO_ERROR;
}

/* Initialize the content hash member of TAG.
 */
static svn_error_t* store_content_part(entry_tag_t *tag,
                                       const char *data,
                                       apr_size_t size,
                                       apr_pool_t *pool)
{
  svn_checksum_t *checksum;
  SVN_ERR(svn_checksum(&checksum,
                       svn_checksum_md5,
                       data,
                       size,
                       pool));

  memcpy(tag->content_hash, checksum->digest, sizeof(tag->content_hash));

  return SVN_NO_ERROR;
}

/* Compare two tags and fail with an assertion upon differences.
 */
static svn_error_t* assert_equal_tags(const entry_tag_t *lhs,
                                      const entry_tag_t *rhs)
{
  SVN_ERR_ASSERT(memcmp(lhs->content_hash, rhs->content_hash,
                        sizeof(lhs->content_hash)) == 0);
  SVN_ERR_ASSERT(memcmp(lhs->prefix_hash, rhs->prefix_hash,
                        sizeof(lhs->prefix_hash)) == 0);
  SVN_ERR_ASSERT(memcmp(lhs->key_hash, rhs->key_hash,
                        sizeof(lhs->key_hash)) == 0);
  SVN_ERR_ASSERT(memcmp(lhs->prefix_tail, rhs->prefix_tail,
                        sizeof(lhs->prefix_tail)) == 0);

  SVN_ERR_ASSERT(lhs->key_len == rhs->key_len);

  return SVN_NO_ERROR;
}

/* Reoccurring code snippets.
 */

#define DEBUG_CACHE_MEMBUFFER_TAG_ARG entry_tag_t *tag,

#define DEBUG_CACHE_MEMBUFFER_TAG tag,

#define DEBUG_CACHE_MEMBUFFER_INIT_TAG(pool)                     \
  entry_tag_t _tag;                                              \
  entry_tag_t *tag = &_tag;                                      \
  if (key)                                                       \
    SVN_ERR(store_key_part(tag,                                  \
                           cache->prefix,                        \
                           cache->info_prefix,                   \
                           key,                                  \
                           cache->key_len == APR_HASH_KEY_STRING \
                               ? strlen((const char *) key)      \
                               : cache->key_len,                 \
                           pool));

#else

/* Don't generate any checks if consistency checks have not been enabled.
 */
#define DEBUG_CACHE_MEMBUFFER_TAG_ARG
#define DEBUG_CACHE_MEMBUFFER_TAG
#define DEBUG_CACHE_MEMBUFFER_INIT_TAG(pool)

#endif /* SVN_DEBUG_CACHE_MEMBUFFER */

/* Per svn_cache_t instance initialization helper.
 * Copy the last to up PREFIX_TAIL_LEN-1 chars from PREFIX to PREFIX_TAIL.
 * If the prefix has been structured by ':', only store the last element
 * (which will tell us the type).
 */
static void get_prefix_tail(const char *prefix, char *prefix_tail)
{
  apr_size_t len = strlen(prefix);
  apr_size_t to_copy = MIN(len, PREFIX_TAIL_LEN - 1);
  const char *last_colon = strrchr(prefix, ':');
  apr_size_t last_element_pos = last_colon ? 0 : last_colon - prefix + 1;

  to_copy = MIN(to_copy, len - last_element_pos);
  memset(prefix_tail, 0, PREFIX_TAIL_LEN);
  memcpy(prefix_tail, prefix + len - to_copy, to_copy);
}

/* A single dictionary entry. Since all entries will be allocated once
 * during cache creation, those entries might be either used or unused.
 * An entry is used if and only if it is contained in the doubly-linked
 * list of used entries per cache level.
 */
typedef struct entry_t
{
  /* Identifying the data item. Only valid for used entries.
   */
  entry_key_t key;

  /* The offset of the cached item's serialized data within the caches
   * DATA buffer.
   */
  apr_uint64_t offset;

  /* Size of the serialized item data. May be 0.  The MAX_ITEM_SIZE macro
   * above ensures that there will be no overflows.
   * Only valid for used entries.
   */
  apr_uint32_t size;

  /* Number of (read) hits for this entry. Will be reset upon write.
   * Only valid for used entries.
   */
  svn_atomic_t hit_count;

  /* Reference to the next used entry in the order defined by offset.
   * NO_INDEX indicates the end of the list; this entry must be referenced
   * by the caches cache_level_t.last member.  NO_INDEX also implies that
   * the data buffer is not used beyond offset+size.
   * Only valid for used entries.
   */
  apr_uint32_t next;

  /* Reference to the previous used entry in the order defined by offset.
   * NO_INDEX indicates the end of the list; this entry must be referenced
   * by the caches cache_level_t.first member.
   * Only valid for used entries.
   */
  apr_uint32_t previous;

  /* Priority of this entry.  This entry will not be replaced by lower-
   * priority items.
   */
  apr_uint32_t priority;
#ifdef SVN_DEBUG_CACHE_MEMBUFFER
  /* Remember type, content and key hashes.
   */
  entry_tag_t tag;
#endif
} entry_t;

/* Group header struct.
 */
typedef struct group_header_t
{
  /* number of entries used [0 .. USED-1] */
  apr_uint32_t used;

  /* next group in the chain or NO_INDEX for the last.
   * For recycleable unused spare groups, this points to the next
   * unused spare group */
  apr_uint32_t next;

  /* previously group in the chain or NO_INDEX for the first */
  apr_uint32_t previous;

  /* number of elements in the chain from start to here.
   * >= 1 for used groups, 0 for unused spare groups */
  apr_uint32_t chain_length;

} group_header_t;

/* The size of the group struct should be a power of two make sure it does
 * not cross memory page boundaries.  Since we already access the cache
 * randomly, having two page table lookups instead of one is bad.
 */
#define GROUP_BLOCK_SIZE 512

/* A ~10-way associative cache seems to be a good compromise between
 * performance (worst-case lookups) and efficiency-loss due to collisions.
 *
 * This value may be changed to any positive integer.
 */
#define GROUP_SIZE \
          ((GROUP_BLOCK_SIZE - sizeof(group_header_t)) / sizeof(entry_t))

/* Maximum number of groups in a chain, i.e. a cache index group can hold
 * up to GROUP_SIZE * MAX_GROUP_CHAIN_LENGTH entries.
 */
#define MAX_GROUP_CHAIN_LENGTH 8

/* We group dictionary entries to make this GROUP-SIZE-way associative.
 */
typedef struct entry_group_t
{
  /* group globals */
  group_header_t header;

  /* padding and also room for future extensions */
  char padding[GROUP_BLOCK_SIZE - sizeof(group_header_t)
               - sizeof(entry_t) * GROUP_SIZE];

  /* the actual entries */
  entry_t entries[GROUP_SIZE];

} entry_group_t;

/* Per-cache level header structure.  Instances of this are members of
 * svn_membuffer_t and will use non-overlapping sections of its DATA buffer.
 * All offset values are global / absolute to that whole buffer.
 */
typedef struct cache_level_t
{
  /* Reference to the first (defined by the order content in the data
   * buffer) dictionary entry used by any data item.
   * NO_INDEX for an empty cache.
   */
  apr_uint32_t first;

  /* Reference to the last (defined by the order content in the data
   * buffer) dictionary entry used by any data item.
   * NO_INDEX for an empty cache.
   */
  apr_uint32_t last;

  /* Reference to the first (defined by the order content in the data
   * buffer) used dictionary entry behind the insertion position
   * (current_data). If NO_INDEX, the data buffer is free starting at the
   * current_data offset.
   */
  apr_uint32_t next;


  /* First offset in the caches DATA buffer that belongs to this level.
   */
  apr_uint64_t start_offset;

  /* Size of data buffer allocated to this level in bytes. Must be > 0.
   */
  apr_uint64_t size;

  /* Offset in the data buffer where the next insertion shall occur.
   */
  apr_uint64_t current_data;

} cache_level_t;

/* The cache header structure.
 */
struct svn_membuffer_t
{
  /* Number of cache segments. Must be a power of 2.
     Please note that this structure represents only one such segment
     and that all segments must / will report the same values here. */
  apr_uint32_t segment_count;

  /* The dictionary, GROUP_SIZE * (group_count + spare_group_count)
   * entries long.  Never NULL.
   */
  entry_group_t *directory;

  /* Flag array with group_count / GROUP_INIT_GRANULARITY _bit_ elements.
   * Allows for efficiently marking groups as "not initialized".
   */
  unsigned char *group_initialized;

  /* Size of dictionary in groups. Must be > 0.
   */
  apr_uint32_t group_count;

  /* Total number of spare groups.
   */
  apr_uint32_t spare_group_count;

  /* First recycleable spare group.
   */
  apr_uint32_t first_spare_group;

  /* Maximum number of spare groups ever used.  I.e. group index
   * group_count + max_spare_used is the first unused spare group
   * if first_spare_group is NO_INDEX.
   */
  apr_uint32_t max_spare_used;

  /* Pointer to the data buffer, data_size bytes long. Never NULL.
   */
  unsigned char *data;

  /* Total number of data buffer bytes in use.
   */
  apr_uint64_t data_used;

  /* Largest entry size that we would accept.  For total cache sizes
   * less than 4TB (sic!), this is determined by the total cache size.
   */
  apr_uint64_t max_entry_size;

  /* The cache levels, organized as sub-buffers.  Since entries in the
   * DIRECTORY use offsets in DATA for addressing, a cache lookup does
   * not need to know the cache level of a specific item.  Cache levels
   * are only used to implement a hybrid insertion / eviction strategy.
   */

  /* First cache level, i.e. most insertions happen here.  Very large
   * items might get inserted directly into L2.  L1 is a strict FIFO
   * ring buffer that does not care about item priorities.  All evicted
   * items get a chance to be promoted to L2.
   */
  cache_level_t l1;

  /* Second cache level, i.e. data evicted from L1 will be added here
   * if the item is "important" enough or the L2 insertion window is large
   * enough.
   */
  cache_level_t l2;


  /* Number of used dictionary entries, i.e. number of cached items.
   * Purely statistical information that may be used for profiling only.
   * Updates are not synchronized and values may be nonsensicle on some
   * platforms.
   */
  apr_uint32_t used_entries;

  /* Total number of calls to membuffer_cache_get.
   * Purely statistical information that may be used for profiling only.
   * Updates are not synchronized and values may be nonsensicle on some
   * platforms.
   */
  apr_uint64_t total_reads;

  /* Total number of calls to membuffer_cache_set.
   * Purely statistical information that may be used for profiling only.
   * Updates are not synchronized and values may be nonsensicle on some
   * platforms.
   */
  apr_uint64_t total_writes;

  /* Total number of hits since the cache's creation.
   * Purely statistical information that may be used for profiling only.
   * Updates are not synchronized and values may be nonsensicle on some
   * platforms.
   */
  apr_uint64_t total_hits;

#if (APR_HAS_THREADS && USE_SIMPLE_MUTEX)
  /* A lock for intra-process synchronization to the cache, or NULL if
   * the cache's creator doesn't feel the cache needs to be
   * thread-safe.
   */
  svn_mutex__t *lock;
#elif (APR_HAS_THREADS && !USE_SIMPLE_MUTEX)
  /* Same for read-write lock. */
  apr_thread_rwlock_t *lock;

  /* If set, write access will wait until they get exclusive access.
   * Otherwise, they will become no-ops if the segment is currently
   * read-locked.  Only used when LOCK is an r/w lock.
   */
  svn_boolean_t allow_blocking_writes;
#endif
};

/* Align integer VALUE to the next ITEM_ALIGNMENT boundary.
 */
#define ALIGN_VALUE(value) (((value) + ITEM_ALIGNMENT-1) & -ITEM_ALIGNMENT)

/* Align POINTER value to the next ITEM_ALIGNMENT boundary.
 */
#define ALIGN_POINTER(pointer) ((void*)ALIGN_VALUE((apr_size_t)(char*)(pointer)))

/* If locking is supported for CACHE, acquire a read lock for it.
 */
static svn_error_t *
read_lock_cache(svn_membuffer_t *cache)
{
#if (APR_HAS_THREADS && USE_SIMPLE_MUTEX)
  return svn_mutex__lock(cache->lock);
#elif (APR_HAS_THREADS && !USE_SIMPLE_MUTEX)
  if (cache->lock)
  {
    apr_status_t status = apr_thread_rwlock_rdlock(cache->lock);
    if (status)
      return svn_error_wrap_apr(status, _("Can't lock cache mutex"));
  }

  return SVN_NO_ERROR;
#else
  return SVN_NO_ERROR;
#endif
}

/* If locking is supported for CACHE, acquire a write lock for it.
 * Set *SUCCESS to FALSE, if we couldn't acquire the write lock;
 * leave it untouched otherwise.
 */
static svn_error_t *
write_lock_cache(svn_membuffer_t *cache, svn_boolean_t *success)
{
#if (APR_HAS_THREADS && USE_SIMPLE_MUTEX)
  return svn_mutex__lock(cache->lock);
#elif (APR_HAS_THREADS && !USE_SIMPLE_MUTEX)
  if (cache->lock)
    {
      apr_status_t status;
      if (cache->allow_blocking_writes)
        {
          status = apr_thread_rwlock_wrlock(cache->lock);
        }
      else
        {
          status = apr_thread_rwlock_trywrlock(cache->lock);
          if (SVN_LOCK_IS_BUSY(status))
            {
              *success = FALSE;
              status = APR_SUCCESS;
            }
        }

      if (status)
        return svn_error_wrap_apr(status,
                                  _("Can't write-lock cache mutex"));
    }

  return SVN_NO_ERROR;
#else
  return SVN_NO_ERROR;
#endif
}

/* If locking is supported for CACHE, acquire an unconditional write lock
 * for it.
 */
static svn_error_t *
force_write_lock_cache(svn_membuffer_t *cache)
{
#if (APR_HAS_THREADS && USE_SIMPLE_MUTEX)
  return svn_mutex__lock(cache->lock);
#elif (APR_HAS_THREADS && !USE_SIMPLE_MUTEX)
  apr_status_t status = apr_thread_rwlock_wrlock(cache->lock);
  if (status)
    return svn_error_wrap_apr(status,
                              _("Can't write-lock cache mutex"));

  return SVN_NO_ERROR;
#else
  return SVN_NO_ERROR;
#endif
}

/* If locking is supported for CACHE, release the current lock
 * (read or write).  Return ERR upon success.
 */
static svn_error_t *
unlock_cache(svn_membuffer_t *cache, svn_error_t *err)
{
#if (APR_HAS_THREADS && USE_SIMPLE_MUTEX)
  return svn_mutex__unlock(cache->lock, err);
#elif (APR_HAS_THREADS && !USE_SIMPLE_MUTEX)
  if (cache->lock)
  {
    apr_status_t status = apr_thread_rwlock_unlock(cache->lock);
    if (err)
      return err;

    if (status)
      return svn_error_wrap_apr(status, _("Can't unlock cache mutex"));
  }

  return err;
#else
  return err;
#endif
}

/* If supported, guard the execution of EXPR with a read lock to CACHE.
 * The macro has been modeled after SVN_MUTEX__WITH_LOCK.
 */
#define WITH_READ_LOCK(cache, expr)         \
do {                                        \
  SVN_ERR(read_lock_cache(cache));          \
  SVN_ERR(unlock_cache(cache, (expr)));     \
} while (0)

/* If supported, guard the execution of EXPR with a write lock to CACHE.
 * The macro has been modeled after SVN_MUTEX__WITH_LOCK.
 *
 * The write lock process is complicated if we don't allow to wait for
 * the lock: If we didn't get the lock, we may still need to remove an
 * existing entry for the given key because that content is now stale.
 * Once we discovered such an entry, we unconditionally do a blocking
 * wait for the write lock.  In case no old content could be found, a
 * failing lock attempt is simply a no-op and we exit the macro.
 */
#define WITH_WRITE_LOCK(cache, expr)                            \
do {                                                            \
  svn_boolean_t got_lock = TRUE;                                \
  SVN_ERR(write_lock_cache(cache, &got_lock));                  \
  if (!got_lock)                                                \
    {                                                           \
      svn_boolean_t exists;                                     \
      SVN_ERR(entry_exists(cache, group_index, key, &exists));  \
      if (exists)                                               \
        SVN_ERR(force_write_lock_cache(cache));                 \
      else                                                      \
        break;                                                  \
    }                                                           \
  SVN_ERR(unlock_cache(cache, (expr)));                         \
} while (0)

/* Returns 0 if the entry group identified by GROUP_INDEX in CACHE has not
 * been initialized, yet. In that case, this group can not data. Otherwise,
 * a non-zero value is returned.
 */
static APR_INLINE unsigned char
is_group_initialized(svn_membuffer_t *cache, apr_uint32_t group_index)
{
  unsigned char flags
    = cache->group_initialized[group_index / (8 * GROUP_INIT_GRANULARITY)];
  unsigned char bit_mask
    = (unsigned char)(1 << ((group_index / GROUP_INIT_GRANULARITY) % 8));

  return flags & bit_mask;
}

/* Initializes the section of the directory in CACHE that contains
 * the entry group identified by GROUP_INDEX. */
static void
initialize_group(svn_membuffer_t *cache, apr_uint32_t group_index)
{
  unsigned char bit_mask;
  apr_uint32_t i;

  /* range of groups to initialize due to GROUP_INIT_GRANULARITY */
  apr_uint32_t first_index =
      (group_index / GROUP_INIT_GRANULARITY) * GROUP_INIT_GRANULARITY;
  apr_uint32_t last_index = first_index + GROUP_INIT_GRANULARITY;
  if (last_index > cache->group_count)
    last_index = cache->group_count;

  for (i = first_index; i < last_index; ++i)
    {
      group_header_t *header = &cache->directory[i].header;
      header->used = 0;
      header->chain_length = 1;
      header->next = NO_INDEX;
      header->previous = NO_INDEX;
    }

  /* set the "initialized" bit for these groups */
  bit_mask
    = (unsigned char)(1 << ((group_index / GROUP_INIT_GRANULARITY) % 8));
  cache->group_initialized[group_index / (8 * GROUP_INIT_GRANULARITY)]
    |= bit_mask;
}

/* Return the next available spare group from CACHE and mark it as used.
 * May return NULL.
 */
static entry_group_t *
allocate_spare_group(svn_membuffer_t *cache)
{
  entry_group_t *group = NULL;

  /* is there some ready-to-use group? */
  if (cache->first_spare_group != NO_INDEX)
    {
      group = &cache->directory[cache->first_spare_group];
      cache->first_spare_group = group->header.next;
    }

  /* any so far untouched spares available? */
  else if (cache->max_spare_used < cache->spare_group_count)
    {
      apr_uint32_t group_index = cache->group_count + cache->max_spare_used;
      ++cache->max_spare_used;

      if (!is_group_initialized(cache, group_index))
        initialize_group(cache, group_index);

      group = &cache->directory[group_index];
    }

  /* spare groups must be empty */
  assert(!group || !group->header.used);
  return group;
}

/* Mark previously allocated spare group GROUP in CACHE as "unused".
 */
static void
free_spare_group(svn_membuffer_t *cache,
                 entry_group_t *group)
{
  assert(group->header.used == 0);
  assert(group->header.previous != NO_INDEX);
  assert(group - cache->directory >= (apr_ssize_t)cache->group_count);

  /* unchain */
  cache->directory[group->header.previous].header.next = NO_INDEX;
  group->header.chain_length = 0;
  group->header.previous = NO_INDEX;

  /* add to chain of spares */
  group->header.next = cache->first_spare_group;
  cache->first_spare_group = (apr_uint32_t) (group - cache->directory);
}

/* Follow the group chain from GROUP in CACHE to its end and return the last
 * group.  May return GROUP.
 */
static entry_group_t *
last_group_in_chain(svn_membuffer_t *cache,
                    entry_group_t *group)
{
  while (group->header.next != NO_INDEX)
    group = &cache->directory[group->header.next];

  return group;
}

/* Return the CHAIN_INDEX-th element in the group chain starting from group
 * START_GROUP_INDEX in CACHE.
 */
static entry_group_t *
get_group(svn_membuffer_t *cache,
          apr_uint32_t start_group_index,
          apr_uint32_t chain_index)
{
  entry_group_t *group = &cache->directory[start_group_index];
  for (; chain_index; --chain_index)
    group = &cache->directory[group->header.next];

  return group;
}

/* Resolve a dictionary entry reference, i.e. return the entry
 * for the given IDX.
 */
static APR_INLINE entry_t *
get_entry(svn_membuffer_t *cache, apr_uint32_t idx)
{
  return &cache->directory[idx / GROUP_SIZE].entries[idx % GROUP_SIZE];
}

/* Get the entry references for the given ENTRY.
 */
static APR_INLINE apr_uint32_t
get_index(svn_membuffer_t *cache, entry_t *entry)
{
  apr_size_t group_index
    = ((char *)entry - (char *)cache->directory) / sizeof(entry_group_t);

  return (apr_uint32_t)group_index * GROUP_SIZE
       + (apr_uint32_t)(entry - cache->directory[group_index].entries);
}

/* Return the cache level of ENTRY in CACHE.
 */
static cache_level_t *
get_cache_level(svn_membuffer_t *cache, entry_t *entry)
{
  return entry->offset < cache->l1.size ? &cache->l1
                                        : &cache->l2;
}

/* Insert ENTRY to the chain of items that belong to LEVEL in CACHE.  IDX
 * is ENTRY's item index and is only given for efficiency.  The insertion
 * takes place just before LEVEL->NEXT.  *CACHE will not be modified.
 */
static void
chain_entry(svn_membuffer_t *cache,
            cache_level_t *level,
            entry_t *entry,
            apr_uint32_t idx)
{
  /* insert ENTRY before this item */
  entry_t *next = level->next == NO_INDEX
                ? NULL
                : get_entry(cache, level->next);
  assert(idx == get_index(cache, entry));

  /* update entry chain
   */
  entry->next = level->next;
  if (level->first == NO_INDEX)
    {
      /* insert as the first entry and only in the chain
       */
      entry->previous = NO_INDEX;
      level->last = idx;
      level->first = idx;
    }
  else if (next == NULL)
    {
      /* insert as the last entry in the chain.
       * Note that it cannot also be at the beginning of the chain.
       */
      entry->previous = level->last;
      get_entry(cache, level->last)->next = idx;
      level->last = idx;
    }
  else
    {
      /* insert either at the start of a non-empty list or
       * somewhere in the middle
       */
      entry->previous = next->previous;
      next->previous = idx;

      if (entry->previous != NO_INDEX)
        get_entry(cache, entry->previous)->next = idx;
      else
        level->first = idx;
    }
}

/* Remove ENTRY from the chain of items that belong to LEVEL in CACHE. IDX
 * is ENTRY's item index and is only given for efficiency.  Please note
 * that neither *CACHE nor *ENTRY will not be modified.
 */
static void
unchain_entry(svn_membuffer_t *cache,
              cache_level_t *level,
              entry_t *entry,
              apr_uint32_t idx)
{
  assert(idx == get_index(cache, entry));

  /* update
   */
  if (level->next == idx)
    level->next = entry->next;

  /* unlink it from the chain of used entries
   */
  if (entry->previous == NO_INDEX)
    level->first = entry->next;
  else
    get_entry(cache, entry->previous)->next = entry->next;

  if (entry->next == NO_INDEX)
    level->last = entry->previous;
  else
    get_entry(cache, entry->next)->previous = entry->previous;
}

/* Remove the used ENTRY from the CACHE, i.e. make it "unused".
 * In contrast to insertion, removal is possible for any entry.
 */
static void
drop_entry(svn_membuffer_t *cache, entry_t *entry)
{
  /* the group that ENTRY belongs to plus a number of useful index values
   */
  apr_uint32_t idx = get_index(cache, entry);
  apr_uint32_t group_index = idx / GROUP_SIZE;
  entry_group_t *last_group
    = last_group_in_chain(cache, &cache->directory[group_index]);
  apr_uint32_t last_in_group
    = (apr_uint32_t) ((last_group - cache->directory) * GROUP_SIZE
    + last_group->header.used - 1);

  cache_level_t *level = get_cache_level(cache, entry);

  /* update global cache usage counters
   */
  cache->used_entries--;
  cache->data_used -= entry->size;

  /* extend the insertion window, if the entry happens to border it
   */
  if (idx == level->next)
    level->next = entry->next;
  else
    if (entry->next == level->next)
      {
        /* insertion window starts right behind the entry to remove
         */
        if (entry->previous == NO_INDEX)
          {
            /* remove the first entry -> insertion may start at pos 0, now */
            level->current_data = level->start_offset;
          }
        else
          {
            /* insertion may start right behind the previous entry */
            entry_t *previous = get_entry(cache, entry->previous);
            level->current_data = ALIGN_VALUE(  previous->offset
                                              + previous->size);
          }
      }

  /* unlink it from the chain of used entries
   */
  unchain_entry(cache, level, entry, idx);

  /* Move last entry into hole (if the removed one is not the last used).
   * We need to do this since all used entries are at the beginning of
   * the group's entries array.
   */
  if (idx != last_in_group)
    {
      /* copy the last used entry to the removed entry's index
       */
      *entry = last_group->entries[last_group->header.used-1];

      /* this ENTRY may belong to a different cache level than the entry
       * we have just removed */
      level = get_cache_level(cache, entry);

      /* update foreign links to new index
       */
      if (last_in_group == level->next)
        level->next = idx;

      if (entry->previous == NO_INDEX)
        level->first = idx;
      else
        get_entry(cache, entry->previous)->next = idx;

      if (entry->next == NO_INDEX)
        level->last = idx;
      else
        get_entry(cache, entry->next)->previous = idx;
    }

  /* Update the number of used entries.
   */
  last_group->header.used--;

  /* Release the last group in the chain if it is a spare group
   */
  if (!last_group->header.used && last_group->header.previous != NO_INDEX)
    free_spare_group(cache, last_group);
}

/* Insert ENTRY into the chain of used dictionary entries. The entry's
 * offset and size members must already have been initialized. Also,
 * the offset must match the beginning of the insertion window.
 */
static void
insert_entry(svn_membuffer_t *cache, entry_t *entry)
{
  /* the group that ENTRY belongs to plus a number of useful index values
   */
  apr_uint32_t idx = get_index(cache, entry);
  apr_uint32_t group_index = idx / GROUP_SIZE;
  entry_group_t *group = &cache->directory[group_index];
  cache_level_t *level = get_cache_level(cache, entry);

  /* The entry must start at the beginning of the insertion window.
   * It must also be the first unused entry in the group.
   */
  assert(entry->offset == level->current_data);
  assert(idx == group_index * GROUP_SIZE + group->header.used);
  level->current_data = ALIGN_VALUE(entry->offset + entry->size);

  /* update usage counters
   */
  cache->used_entries++;
  cache->data_used += entry->size;
  entry->hit_count = 0;
  group->header.used++;

  /* update entry chain
   */
  chain_entry(cache, level, entry, idx);

  /* The current insertion position must never point outside our
   * data buffer.
   */
  assert(level->current_data <= level->start_offset + level->size);
}

/* Map a KEY of 16 bytes to the CACHE and group that shall contain the
 * respective item.
 */
static apr_uint32_t
get_group_index(svn_membuffer_t **cache,
                entry_key_t key)
{
  svn_membuffer_t *segment0 = *cache;

  /* select the cache segment to use. they have all the same group_count.
   * Since key may not be well-distributed, pre-fold it to a smaller but
   * "denser" ranger.  The modulus is a prime larger than the largest
   * counts. */
  *cache = &segment0[(key[1] % APR_UINT64_C(2809637) + (key[0] / 37))
                     & (segment0->segment_count - 1)];
  return (key[0] % APR_UINT64_C(5030895599)) % segment0->group_count;
}

/* Reduce the hit count of ENTRY and update the accumulated hit info
 * in CACHE accordingly.
 */
static APR_INLINE void
let_entry_age(svn_membuffer_t *cache, entry_t *entry)
{
  apr_uint32_t hits_removed = (entry->hit_count + 1) >> 1;

  if (hits_removed)
    {
      entry->hit_count -= hits_removed;
    }
  else
    {
      entry->priority /= 2;
    }
}

/* Given the GROUP_INDEX that shall contain an entry with the hash key
 * TO_FIND, find that entry in the specified group.
 *
 * If FIND_EMPTY is not set, this function will return the one used entry
 * that actually matches the hash or NULL, if no such entry exists.
 *
 * If FIND_EMPTY has been set, this function will drop the one used entry
 * that actually matches the hash (i.e. make it fit to be replaced with
 * new content), an unused entry or a forcibly removed entry (if all
 * group entries are currently in use). The entries' hash value will be
 * initialized with TO_FIND.
 */
static entry_t *
find_entry(svn_membuffer_t *cache,
           apr_uint32_t group_index,
           const apr_uint64_t to_find[2],
           svn_boolean_t find_empty)
{
  entry_group_t *group;
  entry_t *entry = NULL;
  apr_size_t i;

  /* get the group that *must* contain the entry
   */
  group = &cache->directory[group_index];

  /* If the entry group has not been initialized, yet, there is no data.
   */
  if (! is_group_initialized(cache, group_index))
    {
      if (find_empty)
        {
          initialize_group(cache, group_index);
          entry = &group->entries[0];

          /* initialize entry for the new key */
          entry->key[0] = to_find[0];
          entry->key[1] = to_find[1];
        }

      return entry;
    }

  /* try to find the matching entry
   */
  while (1)
    {
      for (i = 0; i < group->header.used; ++i)
        if (   to_find[0] == group->entries[i].key[0]
            && to_find[1] == group->entries[i].key[1])
          {
            /* found it
             */
            entry = &group->entries[i];
            if (!find_empty)
              return entry;

            /* need to empty that entry */
            drop_entry(cache, entry);
            if (group->header.used == GROUP_SIZE)
              group = last_group_in_chain(cache, group);
            else if (group->header.chain_length == 0)
              group = last_group_in_chain(cache,
                                          &cache->directory[group_index]);

            break;
          }

      /* end of chain? */
      if (group->header.next == NO_INDEX)
        break;

      /* only full groups may chain */
      assert(group->header.used == GROUP_SIZE);
      group = &cache->directory[group->header.next];
    }

  /* None found. Are we looking for a free entry?
   */
  if (find_empty)
    {
      /* There is no empty entry in the chain, try chaining a spare group.
       */
      if (   group->header.used == GROUP_SIZE
          && group->header.chain_length < MAX_GROUP_CHAIN_LENGTH)
        {
          entry_group_t *new_group = allocate_spare_group(cache);
          if (new_group)
            {
              /* chain groups
               */
              new_group->header.chain_length = group->header.chain_length + 1;
              new_group->header.previous = (apr_uint32_t) (group -
                                                           cache->directory);
              new_group->header.next = NO_INDEX;
              group->header.next = (apr_uint32_t) (new_group -
                                                   cache->directory);
              group = new_group;
            }
        }

      /* if GROUP is still filled, we need to remove a random entry */
      if (group->header.used == GROUP_SIZE)
        {
          /* every entry gets the same chance of being removed.
           * Otherwise, we free the first entry, fill it and
           * remove it again on the next occasion without considering
           * the other entries in this group.
           *
           * We hit only one random group instead of processing all
           * groups in the chain.
           */
          cache_level_t *entry_level;
          int to_remove = rand() % (GROUP_SIZE * group->header.chain_length);
          entry_group_t *to_shrink
            = get_group(cache, group_index, to_remove / GROUP_SIZE);

          entry = &to_shrink->entries[to_remove % GROUP_SIZE];
          entry_level = get_cache_level(cache, entry);
          for (i = 0; i < GROUP_SIZE; ++i)
            {
              /* keep L1 entries whenever possible */

              cache_level_t *level
                = get_cache_level(cache, &to_shrink->entries[i]);
              if (   (level != entry_level && entry_level == &cache->l1)
                  || (entry->hit_count > to_shrink->entries[i].hit_count))
                {
                  entry_level = level;
                  entry = &to_shrink->entries[i];
                }
            }

          /* for the entries that don't have been removed,
           * reduce their hit counts to put them at a relative
           * disadvantage the next time.
           */
          for (i = 0; i < GROUP_SIZE; ++i)
            if (entry != &to_shrink->entries[i])
              let_entry_age(cache, entry);

          drop_entry(cache, entry);
        }

      /* initialize entry for the new key
       */
      entry = &group->entries[group->header.used];
      entry->key[0] = to_find[0];
      entry->key[1] = to_find[1];
    }

  return entry;
}

/* Move a surviving ENTRY from just behind the insertion window to
 * its beginning and move the insertion window up accordingly.
 */
static void
move_entry(svn_membuffer_t *cache, entry_t *entry)
{
  apr_size_t size = ALIGN_VALUE(entry->size);
  cache_level_t *level = get_cache_level(cache, entry);

  /* This entry survived this cleansing run. Reset half of its
   * hit count so that its removal gets more likely in the next
   * run unless someone read / hit this entry in the meantime.
   */
  let_entry_age(cache, entry);

  /* Move the entry to the start of the empty / insertion section
   * (if it isn't there already). Size-aligned moves are legal
   * since all offsets and block sizes share this same alignment.
   * Size-aligned moves tend to be faster than non-aligned ones
   * because no "odd" bytes at the end need to special treatment.
   */
  if (entry->offset != level->current_data)
    {
      memmove(cache->data + level->current_data,
              cache->data + entry->offset,
              size);
      entry->offset = level->current_data;
    }

  /* The insertion position is now directly behind this entry.
   */
  level->current_data = entry->offset + size;
  level->next = entry->next;

  /* The current insertion position must never point outside our
   * data buffer.
   */
  assert(level->current_data <= level->start_offset + level->size);
}

/* Move ENTRY in CACHE from L1 to L2.
 */
static void
promote_entry(svn_membuffer_t *cache, entry_t *entry)
{
  apr_uint32_t idx = get_index(cache, entry);
  apr_size_t size = ALIGN_VALUE(entry->size);
  assert(get_cache_level(cache, entry) == &cache->l1);
  assert(idx == cache->l1.next);

  /* copy item from the current location in L1 to the start of L2's
   * insertion window */
  memmove(cache->data + cache->l2.current_data,
          cache->data + entry->offset,
          size);
  entry->offset = cache->l2.current_data;

  /* The insertion position is now directly behind this entry.
   */
  cache->l2.current_data += size;

  /* remove ENTRY from chain of L1 entries and put it into L2
   */
  unchain_entry(cache, &cache->l1, entry, idx);
  chain_entry(cache, &cache->l2, entry, idx);
}

/* This function implements the cache insertion / eviction strategy for L2.
 *
 * If necessary, enlarge the insertion window of CACHE->L2 until it is at
 * least TO_FIT_IN->SIZE bytes long. TO_FIT_IN->SIZE must not exceed the
 * data buffer size allocated to CACHE->L2.  IDX is the item index of
 * TO_FIT_IN and is given for performance reasons.
 *
 * Return TRUE if enough room could be found or made.  A FALSE result
 * indicates that the respective item shall not be added.
 */
static svn_boolean_t
ensure_data_insertable_l2(svn_membuffer_t *cache,
                          entry_t *to_fit_in)
{
  entry_t *entry;

  /* accumulated size of the entries that have been removed to make
   * room for the new one.
   */
  apr_size_t moved_size = 0;

  /* count the number of entries that got moved.  A single large entry
   * being moved is not enough to reject an insertion.
   */
  apr_size_t moved_count = 0;

  /* accumulated "worth" of items dropped so far */
  apr_uint64_t drop_hits = 0;

  /* estimated "worth" of the new entry */
  apr_uint64_t drop_hits_limit = (to_fit_in->hit_count + 1)
                               * (apr_uint64_t)to_fit_in->priority;

  /* This loop will eventually terminate because every cache entry
   * would get dropped eventually:
   *
   * - the incoming entry is small enough to fit into L2
   * - every iteration either frees parts of L2 or counts the moved size
   * - eventually, we either moved too many items with too much total size
   *   to accept the new entry, or made enough room in L2 for the new entry
   *
   * Low-prio items get rejected even sooner.
   */
  while (1)
    {
      /* first offset behind the insertion window
       */
      apr_uint64_t end = cache->l2.next == NO_INDEX
                       ? cache->l2.start_offset + cache->l2.size
                       : get_entry(cache, cache->l2.next)->offset;

      /* leave function as soon as the insertion window is large enough
       */
      if (end >= to_fit_in->size + cache->l2.current_data)
        return TRUE;

      /* Don't be too eager to cache data.  If a lot of data has been moved
       * around, the current item has probably a relatively low priority.
       * We must also limit the effort spent here (if even in case of faulty
       * heuristics).  Therefore, give up after some time.
       */
      if (moved_size > 4 * to_fit_in->size && moved_count > 7)
        return FALSE;

      /* if the net worth (in weighted hits) of items removed is already
       * larger than what we want to insert, reject TO_FIT_IN because it
       * still does not fit in. */
      if (drop_hits > drop_hits_limit)
        return FALSE;

      /* try to enlarge the insertion window
       */
      if (cache->l2.next == NO_INDEX)
        {
          /* We reached the end of the data buffer; restart at the beginning.
           * Due to the randomized nature of our LFU implementation, very
           * large data items may require multiple passes. Therefore, SIZE
           * should be restricted to significantly less than data_size.
           */
          cache->l2.current_data = cache->l2.start_offset;
          cache->l2.next = cache->l2.first;
        }
      else
        {
          svn_boolean_t keep;
          entry = get_entry(cache, cache->l2.next);

          if (to_fit_in->priority < SVN_CACHE__MEMBUFFER_DEFAULT_PRIORITY)
            {
              /* Low prio items can only be accepted only if the current
               * entry is of even lower prio and has fewer hits.
               */
              if (   entry->priority > to_fit_in->priority
                  || entry->hit_count > to_fit_in->hit_count)
                return FALSE;
            }

          if (entry->priority <= SVN_CACHE__MEMBUFFER_LOW_PRIORITY)
            {
              /* Be quick to remove low-prio entries - even if the incoming
               * one is low-prio as well.  This makes room for more important
               * data and replaces existing data with newly read information.
               */
              keep = FALSE;
            }
          else
            {
              /* If the existing data is the same prio as the incoming data,
               * drop the existing entry if it had seen fewer (probably 0)
               * hits than the entry coming in from L1.  In case of different
               * priorities, keep the current entry of it has higher prio.
               * The new entry may still find room by ousting other entries.
               */
              keep = to_fit_in->priority == entry->priority
                   ? entry->hit_count >= to_fit_in->hit_count
                   : entry->priority > to_fit_in->priority;
            }

          /* keepers or destroyers? */
          if (keep)
            {
              /* Moving entries around is not for free -> track costs. */
              moved_size += entry->size;
              moved_count++;

              move_entry(cache, entry);
            }
          else
            {
              /* Drop the entry from the end of the insertion window.
               * Count the "hit importance" such that we are not sacrificing
               * too much of the high-hit contents.  However, don't count
               * low-priority hits because higher prio entries will often
               * provide the same data but in a further stage of processing.
               */
              if (entry->priority > SVN_CACHE__MEMBUFFER_LOW_PRIORITY)
                drop_hits += entry->hit_count * (apr_uint64_t)entry->priority;

              drop_entry(cache, entry);
            }
        }
    }

  /* This will never be reached. But if it was, "can't insert" was the
   * right answer. */
}

/* This function implements the cache insertion / eviction strategy for L1.
 *
 * If necessary, enlarge the insertion window of CACHE->L1 by promoting
 * entries to L2 until it is at least SIZE bytes long.
 *
 * Return TRUE if enough room could be found or made.  A FALSE result
 * indicates that the respective item shall not be added because it is
 * too large.
 */
static svn_boolean_t
ensure_data_insertable_l1(svn_membuffer_t *cache, apr_size_t size)
{
  /* Guarantees that the while loop will terminate. */
  if (size > cache->l1.size)
    return FALSE;

  /* This loop will eventually terminate because every cache entry
   * would get dropped eventually.
   */
  while (1)
    {
      /* first offset behind the insertion window
       */
      apr_uint32_t entry_index = cache->l1.next;
      entry_t *entry = get_entry(cache, entry_index);
      apr_uint64_t end = cache->l1.next == NO_INDEX
                       ? cache->l1.start_offset + cache->l1.size
                       : entry->offset;

      /* leave function as soon as the insertion window is large enough
       */
      if (end >= size + cache->l1.current_data)
        return TRUE;

      /* Enlarge the insertion window
       */
      if (cache->l1.next == NO_INDEX)
        {
          /* We reached the end of the data buffer; restart at the beginning.
           * Due to the randomized nature of our LFU implementation, very
           * large data items may require multiple passes. Therefore, SIZE
           * should be restricted to significantly less than data_size.
           */
          cache->l1.current_data = cache->l1.start_offset;
          cache->l1.next = cache->l1.first;
        }
      else
        {
          /* Remove the entry from the end of insertion window and promote
           * it to L2, if it is important enough.
           */
          svn_boolean_t keep = ensure_data_insertable_l2(cache, entry);

          /* We might have touched the group that contains ENTRY. Recheck. */
          if (entry_index == cache->l1.next)
            {
              if (keep)
                promote_entry(cache, entry);
              else
                drop_entry(cache, entry);
            }
        }
    }

  /* This will never be reached. But if it was, "can't insert" was the
   * right answer. */
}

/* Mimic apr_pcalloc in APR_POOL_DEBUG mode, i.e. handle failed allocations
 * (e.g. OOM) properly: Allocate at least SIZE bytes from POOL and zero
 * the content of the allocated memory if ZERO has been set. Return NULL
 * upon failed allocations.
 *
 * Also, satisfy our buffer alignment needs for performance reasons.
 */
static void* secure_aligned_alloc(apr_pool_t *pool,
                                  apr_size_t size,
                                  svn_boolean_t zero)
{
  void* memory = apr_palloc(pool, size + ITEM_ALIGNMENT);
  if (memory != NULL)
    {
      memory = ALIGN_POINTER(memory);
      if (zero)
        memset(memory, 0, size);
    }

  return memory;
}

svn_error_t *
svn_cache__membuffer_cache_create(svn_membuffer_t **cache,
                                  apr_size_t total_size,
                                  apr_size_t directory_size,
                                  apr_size_t segment_count,
                                  svn_boolean_t thread_safe,
                                  svn_boolean_t allow_blocking_writes,
                                  apr_pool_t *pool)
{
  svn_membuffer_t *c;

  apr_uint32_t seg;
  apr_uint32_t group_count;
  apr_uint32_t main_group_count;
  apr_uint32_t spare_group_count;
  apr_uint32_t group_init_size;
  apr_uint64_t data_size;
  apr_uint64_t max_entry_size;

  /* Limit the total size (only relevant if we can address > 4GB)
   */
#if APR_SIZEOF_VOIDP > 4
  if (total_size > MAX_SEGMENT_SIZE * MAX_SEGMENT_COUNT)
    total_size = MAX_SEGMENT_SIZE * MAX_SEGMENT_COUNT;
#endif

  /* Limit the segment count
   */
  if (segment_count > MAX_SEGMENT_COUNT)
    segment_count = MAX_SEGMENT_COUNT;
  if (segment_count * MIN_SEGMENT_SIZE > total_size)
    segment_count = total_size / MIN_SEGMENT_SIZE;

  /* The segment count must be a power of two. Round it down as necessary.
   */
  while ((segment_count & (segment_count-1)) != 0)
    segment_count &= segment_count-1;

  /* if the caller hasn't provided a reasonable segment count or the above
   * limitations set it to 0, derive one from the absolute cache size
   */
  if (segment_count < 1)
    {
      /* Determine a reasonable number of cache segments. Segmentation is
       * only useful for multi-threaded / multi-core servers as it reduces
       * lock contention on these systems.
       *
       * But on these systems, we can assume that ample memory has been
       * allocated to this cache. Smaller caches should not be segmented
       * as this severely limits the maximum size of cachable items.
       *
       * Segments should not be smaller than 32MB and max. cachable item
       * size should grow as fast as segmentation.
       */

      apr_uint32_t segment_count_shift = 0;
      while (((2 * DEFAULT_MIN_SEGMENT_SIZE) << (2 * segment_count_shift))
             < total_size)
        ++segment_count_shift;

      segment_count = (apr_size_t)1 << segment_count_shift;
    }

  /* If we have an extremely large cache (>512 GB), the default segment
   * size may exceed the amount allocatable as one chunk. In that case,
   * increase segmentation until we are under the threshold.
   */
  while (   total_size / segment_count > MAX_SEGMENT_SIZE
         && segment_count < MAX_SEGMENT_COUNT)
    segment_count *= 2;

  /* allocate cache as an array of segments / cache objects */
  c = apr_palloc(pool, segment_count * sizeof(*c));

  /* Split total cache size into segments of equal size
   */
  total_size /= segment_count;
  directory_size /= segment_count;

  /* prevent pathological conditions: ensure a certain minimum cache size
   */
  if (total_size < 2 * sizeof(entry_group_t))
    total_size = 2 * sizeof(entry_group_t);

  /* adapt the dictionary size accordingly, if necessary:
   * It must hold at least one group and must not exceed the cache size.
   */
  if (directory_size > total_size - sizeof(entry_group_t))
    directory_size = total_size - sizeof(entry_group_t);
  if (directory_size < 2 * sizeof(entry_group_t))
    directory_size = 2 * sizeof(entry_group_t);

  /* limit the data size to what we can address.
   * Note that this cannot overflow since all values are of size_t.
   * Also, make it a multiple of the item placement granularity to
   * prevent subtle overflows.
   */
  data_size = ALIGN_VALUE(total_size - directory_size + 1) - ITEM_ALIGNMENT;

  /* For cache sizes > 16TB, individual cache segments will be larger
   * than 32GB allowing for >4GB entries.  But caching chunks larger
   * than 4GB are simply not supported.
   */
  max_entry_size = data_size / 8 > MAX_ITEM_SIZE
                 ? MAX_ITEM_SIZE
                 : data_size / 8;

  /* to keep the entries small, we use 32 bit indexes only
   * -> we need to ensure that no more then 4G entries exist.
   *
   * Note, that this limit could only be exceeded in a very
   * theoretical setup with about 1EB of cache.
   */
  group_count = directory_size / sizeof(entry_group_t)
                    >= (APR_UINT32_MAX / GROUP_SIZE)
              ? (APR_UINT32_MAX / GROUP_SIZE) - 1
              : (apr_uint32_t)(directory_size / sizeof(entry_group_t));

  /* set some of the index directory aside as over-flow (spare) buffers */
  spare_group_count = MAX(group_count / 4, 1);
  main_group_count = group_count - spare_group_count;
  assert(spare_group_count > 0 && main_group_count > 0);

  group_init_size = 1 + group_count / (8 * GROUP_INIT_GRANULARITY);
  for (seg = 0; seg < segment_count; ++seg)
    {
      /* allocate buffers and initialize cache members
       */
      c[seg].segment_count = (apr_uint32_t)segment_count;

      c[seg].group_count = main_group_count;
      c[seg].spare_group_count = spare_group_count;
      c[seg].first_spare_group = NO_INDEX;
      c[seg].max_spare_used = 0;

      c[seg].directory = apr_pcalloc(pool,
                                     group_count * sizeof(entry_group_t));

      /* Allocate and initialize directory entries as "not initialized",
         hence "unused" */
      c[seg].group_initialized = apr_pcalloc(pool, group_init_size);

      /* Allocate 1/4th of the data buffer to L1
       */
      c[seg].l1.first = NO_INDEX;
      c[seg].l1.last = NO_INDEX;
      c[seg].l1.next = NO_INDEX;
      c[seg].l1.start_offset = 0;
      c[seg].l1.size = ALIGN_VALUE(data_size / 4);
      c[seg].l1.current_data = 0;

      /* The remaining 3/4th will be used as L2
       */
      c[seg].l2.first = NO_INDEX;
      c[seg].l2.last = NO_INDEX;
      c[seg].l2.next = NO_INDEX;
      c[seg].l2.start_offset = c[seg].l1.size;
      c[seg].l2.size = data_size - c[seg].l1.size;
      c[seg].l2.current_data = c[seg].l2.start_offset;

      c[seg].data = secure_aligned_alloc(pool, (apr_size_t)data_size, FALSE);
      c[seg].data_used = 0;
      c[seg].max_entry_size = max_entry_size;

      c[seg].used_entries = 0;
      c[seg].total_reads = 0;
      c[seg].total_writes = 0;
      c[seg].total_hits = 0;

      /* were allocations successful?
       * If not, initialize a minimal cache structure.
       */
      if (c[seg].data == NULL || c[seg].directory == NULL)
        {
          /* We are OOM. There is no need to proceed with "half a cache".
           */
          return svn_error_wrap_apr(APR_ENOMEM, "OOM");
        }

#if (APR_HAS_THREADS && USE_SIMPLE_MUTEX)
      /* A lock for intra-process synchronization to the cache, or NULL if
       * the cache's creator doesn't feel the cache needs to be
       * thread-safe.
       */
      SVN_ERR(svn_mutex__init(&c[seg].lock, thread_safe, pool));
#elif (APR_HAS_THREADS && !USE_SIMPLE_MUTEX)
      /* Same for read-write lock. */
      c[seg].lock = NULL;
      if (thread_safe)
        {
          apr_status_t status =
              apr_thread_rwlock_create(&(c[seg].lock), pool);
          if (status)
            return svn_error_wrap_apr(status, _("Can't create cache mutex"));
        }

      /* Select the behavior of write operations.
       */
      c[seg].allow_blocking_writes = allow_blocking_writes;
#endif
    }

  /* done here
   */
  *cache = c;
  return SVN_NO_ERROR;
}

svn_error_t *
svn_cache__membuffer_clear(svn_membuffer_t *cache)
{
  apr_size_t seg;
  apr_size_t segment_count = cache->segment_count;

  /* Length of the group_initialized array in bytes.
     See also svn_cache__membuffer_cache_create(). */
  apr_size_t group_init_size
    = 1 + (cache->group_count + cache->spare_group_count)
            / (8 * GROUP_INIT_GRANULARITY);

  /* Clear segment by segment.  This implies that other thread may read
     and write to other segments after we cleared them and before the
     last segment is done.

     However, that is no different from a write request coming through
     right after we cleared all segments because dependencies between
     cache entries (recursive lookup / access locks) are not allowed.
   */
  for (seg = 0; seg < segment_count; ++seg)
    {
      /* Unconditionally acquire the write lock. */
      SVN_ERR(force_write_lock_cache(&cache[seg]));

      /* Mark all groups as "not initialized", which implies "empty". */
      cache[seg].first_spare_group = NO_INDEX;
      cache[seg].max_spare_used = 0;

      memset(cache[seg].group_initialized, 0, group_init_size);

      /* Unlink L1 contents. */
      cache[seg].l1.first = NO_INDEX;
      cache[seg].l1.last = NO_INDEX;
      cache[seg].l1.next = NO_INDEX;
      cache[seg].l1.current_data = cache[seg].l1.start_offset;

      /* Unlink L2 contents. */
      cache[seg].l2.first = NO_INDEX;
      cache[seg].l2.last = NO_INDEX;
      cache[seg].l2.next = NO_INDEX;
      cache[seg].l2.current_data = cache[seg].l2.start_offset;

      /* Reset content counters. */
      cache[seg].data_used = 0;
      cache[seg].used_entries = 0;

      /* Segment may be used again. */
      SVN_ERR(unlock_cache(&cache[seg], SVN_NO_ERROR));
    }

  /* done here */
  return SVN_NO_ERROR;
}

/* Look for the cache entry in group GROUP_INDEX of CACHE, identified
 * by the hash value TO_FIND and set *FOUND accordingly.
 *
 * Note: This function requires the caller to serialize access.
 * Don't call it directly, call entry_exists instead.
 */
static svn_error_t *
entry_exists_internal(svn_membuffer_t *cache,
                      apr_uint32_t group_index,
                      entry_key_t to_find,
                      svn_boolean_t *found)
{
  *found = find_entry(cache, group_index, to_find, FALSE) != NULL;
  return SVN_NO_ERROR;
}

/* Look for the cache entry in group GROUP_INDEX of CACHE, identified
 * by the hash value TO_FIND and set *FOUND accordingly.
 */
static svn_error_t *
entry_exists(svn_membuffer_t *cache,
             apr_uint32_t group_index,
             entry_key_t to_find,
             svn_boolean_t *found)
{
  WITH_READ_LOCK(cache,
                 entry_exists_internal(cache,
                                       group_index,
                                       to_find,
                                       found));

  return SVN_NO_ERROR;
}

/* Given the SIZE and PRIORITY of a new item, return the cache level
   (L1 or L2) in fragment CACHE that this item shall be inserted into.
   If we can't find nor make enough room for the item, return NULL.
 */
static cache_level_t *
select_level(svn_membuffer_t *cache,
             apr_size_t size,
             apr_uint32_t priority)
{
  if (cache->max_entry_size >= size)
    {
      /* Small items go into L1. */
      return ensure_data_insertable_l1(cache, size)
           ? &cache->l1
           : NULL;
    }
  else if (   cache->l2.size >= size
           && MAX_ITEM_SIZE >= size
           && priority > SVN_CACHE__MEMBUFFER_DEFAULT_PRIORITY)
    {
      /* Large but important items go into L2. */
      entry_t dummy_entry = { { 0 } };
      dummy_entry.priority = priority;
      dummy_entry.size = (apr_uint32_t) size;

      return ensure_data_insertable_l2(cache, &dummy_entry)
           ? &cache->l2
           : NULL;
    }

  /* Don't cache large, unimportant items. */
  return NULL;
}

/* Try to insert the serialized item given in BUFFER with SIZE into
 * the group GROUP_INDEX of CACHE and uniquely identify it by hash
 * value TO_FIND.
 *
 * However, there is no guarantee that it will actually be put into
 * the cache. If there is already some data associated with TO_FIND,
 * it will be removed from the cache even if the new data cannot
 * be inserted.
 *
 * Note: This function requires the caller to serialization access.
 * Don't call it directly, call membuffer_cache_get_partial instead.
 */
static svn_error_t *
membuffer_cache_set_internal(svn_membuffer_t *cache,
                             entry_key_t to_find,
                             apr_uint32_t group_index,
                             char *buffer,
                             apr_size_t size,
                             apr_uint32_t priority,
                             DEBUG_CACHE_MEMBUFFER_TAG_ARG
                             apr_pool_t *scratch_pool)
{
  cache_level_t *level;

  /* first, look for a previous entry for the given key */
  entry_t *entry = find_entry(cache, group_index, to_find, FALSE);

  /* if there is an old version of that entry and the new data fits into
   * the old spot, just re-use that space. */
  if (entry && ALIGN_VALUE(entry->size) >= size && buffer)
    {
      /* Careful! We need to cast SIZE to the full width of CACHE->DATA_USED
       * lest we run into trouble with 32 bit underflow *not* treated as a
       * negative value.
       */
      cache->data_used += (apr_uint64_t)size - entry->size;
      entry->size = (apr_uint32_t) size;
      entry->priority = priority;

#ifdef SVN_DEBUG_CACHE_MEMBUFFER

      /* Remember original content, type and key (hashes)
       */
      SVN_ERR(store_content_part(tag, buffer, size, scratch_pool));
      memcpy(&entry->tag, tag, sizeof(*tag));

#endif

      if (size)
        memcpy(cache->data + entry->offset, buffer, size);

      cache->total_writes++;
      return SVN_NO_ERROR;
    }

  /* if necessary, enlarge the insertion window.
   */
  level = buffer ? select_level(cache, size, priority) : NULL;
  if (level)
    {
      /* Remove old data for this key, if that exists.
       * Get an unused entry for the key and and initialize it with
       * the serialized item's (future) position within data buffer.
       */
      entry = find_entry(cache, group_index, to_find, TRUE);
      entry->size = (apr_uint32_t) size;
      entry->offset = level->current_data;
      entry->priority = priority;

#ifdef SVN_DEBUG_CACHE_MEMBUFFER

      /* Remember original content, type and key (hashes)
       */
      SVN_ERR(store_content_part(tag, buffer, size, scratch_pool));
      memcpy(&entry->tag, tag, sizeof(*tag));

#endif

      /* Link the entry properly.
       */
      insert_entry(cache, entry);

      /* Copy the serialized item data into the cache.
       */
      if (size)
        memcpy(cache->data + entry->offset, buffer, size);

      cache->total_writes++;
    }
  else
    {
      /* if there is already an entry for this key, drop it.
       * Since ensure_data_insertable may have removed entries from
       * ENTRY's group, re-do the lookup.
       */
      entry = find_entry(cache, group_index, to_find, FALSE);
      if (entry)
        drop_entry(cache, entry);
    }

  return SVN_NO_ERROR;
}

/* Try to insert the ITEM and use the KEY to uniquely identify it.
 * However, there is no guarantee that it will actually be put into
 * the cache. If there is already some data associated to the KEY,
 * it will be removed from the cache even if the new data cannot
 * be inserted.
 *
 * The SERIALIZER is called to transform the ITEM into a single,
 * flat data buffer. Temporary allocations may be done in POOL.
 */
static svn_error_t *
membuffer_cache_set(svn_membuffer_t *cache,
                    entry_key_t key,
                    void *item,
                    svn_cache__serialize_func_t serializer,
                    apr_uint32_t priority,
                    DEBUG_CACHE_MEMBUFFER_TAG_ARG
                    apr_pool_t *scratch_pool)
{
  apr_uint32_t group_index;
  void *buffer = NULL;
  apr_size_t size = 0;

  /* find the entry group that will hold the key.
   */
  group_index = get_group_index(&cache, key);

  /* Serialize data data.
   */
  if (item)
    SVN_ERR(serializer(&buffer, &size, item, scratch_pool));

  /* The actual cache data access needs to sync'ed
   */
  WITH_WRITE_LOCK(cache,
                  membuffer_cache_set_internal(cache,
                                               key,
                                               group_index,
                                               buffer,
                                               size,
                                               priority,
                                               DEBUG_CACHE_MEMBUFFER_TAG
                                               scratch_pool));
  return SVN_NO_ERROR;
}

/* Count a hit in ENTRY within CACHE.
 */
static void
increment_hit_counters(svn_membuffer_t *cache, entry_t *entry)
{
  /* To minimize the memory footprint of the cache index, we limit local
   * hit counters to 32 bits.  These may overflow but we don't really
   * care because at worst, ENTRY will be dropped from cache once every
   * few billion hits. */
  svn_atomic_inc(&entry->hit_count);

  /* That one is for stats only. */
  cache->total_hits++;
}

/* Look for the cache entry in group GROUP_INDEX of CACHE, identified
 * by the hash value TO_FIND. If no item has been stored for KEY,
 * *BUFFER will be NULL. Otherwise, return a copy of the serialized
 * data in *BUFFER and return its size in *ITEM_SIZE. Allocations will
 * be done in POOL.
 *
 * Note: This function requires the caller to serialization access.
 * Don't call it directly, call membuffer_cache_get_partial instead.
 */
static svn_error_t *
membuffer_cache_get_internal(svn_membuffer_t *cache,
                             apr_uint32_t group_index,
                             entry_key_t to_find,
                             char **buffer,
                             apr_size_t *item_size,
                             DEBUG_CACHE_MEMBUFFER_TAG_ARG
                             apr_pool_t *result_pool)
{
  entry_t *entry;
  apr_size_t size;

  /* The actual cache data access needs to sync'ed
   */
  entry = find_entry(cache, group_index, to_find, FALSE);
  cache->total_reads++;
  if (entry == NULL)
    {
      /* no such entry found.
       */
      *buffer = NULL;
      *item_size = 0;

      return SVN_NO_ERROR;
    }

  size = ALIGN_VALUE(entry->size);
  *buffer = ALIGN_POINTER(apr_palloc(result_pool, size + ITEM_ALIGNMENT-1));
  memcpy(*buffer, (const char*)cache->data + entry->offset, size);

#ifdef SVN_DEBUG_CACHE_MEMBUFFER

  /* Check for overlapping entries.
   */
  SVN_ERR_ASSERT(entry->next == NO_INDEX ||
                 entry->offset + size
                    <= get_entry(cache, entry->next)->offset);

  /* Compare original content, type and key (hashes)
   */
  SVN_ERR(store_content_part(tag, *buffer, entry->size, result_pool));
  SVN_ERR(assert_equal_tags(&entry->tag, tag));

#endif

  /* update hit statistics
   */
  increment_hit_counters(cache, entry);
  *item_size = entry->size;

  return SVN_NO_ERROR;
}

/* Look for the *ITEM identified by KEY. If no item has been stored
 * for KEY, *ITEM will be NULL. Otherwise, the DESERIALIZER is called
 * re-construct the proper object from the serialized data.
 * Allocations will be done in POOL.
 */
static svn_error_t *
membuffer_cache_get(svn_membuffer_t *cache,
                    entry_key_t key,
                    void **item,
                    svn_cache__deserialize_func_t deserializer,
                    DEBUG_CACHE_MEMBUFFER_TAG_ARG
                    apr_pool_t *result_pool)
{
  apr_uint32_t group_index;
  char *buffer;
  apr_size_t size;

  /* find the entry group that will hold the key.
   */
  group_index = get_group_index(&cache, key);
  WITH_READ_LOCK(cache,
                 membuffer_cache_get_internal(cache,
                                              group_index,
                                              key,
                                              &buffer,
                                              &size,
                                              DEBUG_CACHE_MEMBUFFER_TAG
                                              result_pool));

  /* re-construct the original data object from its serialized form.
   */
  if (buffer == NULL)
    {
      *item = NULL;
      return SVN_NO_ERROR;
    }

  return deserializer(item, buffer, size, result_pool);
}

/* Look for the cache entry in group GROUP_INDEX of CACHE, identified
 * by the hash value TO_FIND.  If no item has been stored for KEY, *FOUND
 * will be FALSE and TRUE otherwise.
 */
static svn_error_t *
membuffer_cache_has_key_internal(svn_membuffer_t *cache,
                                 apr_uint32_t group_index,
                                 entry_key_t to_find,
                                 svn_boolean_t *found)
{
  entry_t *entry = find_entry(cache, group_index, to_find, FALSE);
  if (entry)
    {
      /* This often be called by "block read" when most data is already
         in L2 and only a few previously evicted items are added to L1
         again.  While items in L1 are well protected for a while, L2
         items may get evicted soon.  Thus, mark all them as "hit" to give
         them a higher chance of survival. */
      increment_hit_counters(cache, entry);
      *found = TRUE;
    }
  else
    {
      *found = FALSE;
    }

  return SVN_NO_ERROR;
}

/* Look for an entry identified by KEY.  If no item has been stored
 * for KEY, *FOUND will be set to FALSE and TRUE otherwise.
 */
/* Implements svn_cache__has_key for membuffer caches.
 */
static svn_error_t *
membuffer_cache_has_key(svn_membuffer_t *cache,
                        entry_key_t key,
                        svn_boolean_t *found)
{
  /* find the entry group that will hold the key.
   */
  apr_uint32_t group_index = get_group_index(&cache, key);
  cache->total_reads++;

  WITH_READ_LOCK(cache,
                 membuffer_cache_has_key_internal(cache,
                                                  group_index,
                                                  key,
                                                  found));

  return SVN_NO_ERROR;
}

/* Look for the cache entry in group GROUP_INDEX of CACHE, identified
 * by the hash value TO_FIND. FOUND indicates whether that entry exists.
 * If not found, *ITEM will be NULL.
 *
 * Otherwise, the DESERIALIZER is called with that entry and the BATON
 * provided and will extract the desired information. The result is set
 * in *ITEM. Allocations will be done in POOL.
 *
 * Note: This function requires the caller to serialization access.
 * Don't call it directly, call membuffer_cache_get_partial instead.
 */
static svn_error_t *
membuffer_cache_get_partial_internal(svn_membuffer_t *cache,
                                     apr_uint32_t group_index,
                                     entry_key_t to_find,
                                     void **item,
                                     svn_boolean_t *found,
                                     svn_cache__partial_getter_func_t deserializer,
                                     void *baton,
                                     DEBUG_CACHE_MEMBUFFER_TAG_ARG
                                     apr_pool_t *result_pool)
{
  entry_t *entry = find_entry(cache, group_index, to_find, FALSE);
  cache->total_reads++;
  if (entry == NULL)
    {
      *item = NULL;
      *found = FALSE;

      return SVN_NO_ERROR;
    }
  else
    {
      *found = TRUE;
      increment_hit_counters(cache, entry);

#ifdef SVN_DEBUG_CACHE_MEMBUFFER

      /* Check for overlapping entries.
       */
      SVN_ERR_ASSERT(entry->next == NO_INDEX ||
                     entry->offset + entry->size
                        <= get_entry(cache, entry->next)->offset);

      /* Compare original content, type and key (hashes)
       */
      SVN_ERR(store_content_part(tag,
                                 (const char*)cache->data + entry->offset,
                                 entry->size,
                                 result_pool));
      SVN_ERR(assert_equal_tags(&entry->tag, tag));

#endif

      return deserializer(item,
                          (const char*)cache->data + entry->offset,
                          entry->size,
                          baton,
                          result_pool);
    }
}

/* Look for the cache entry identified by KEY. FOUND indicates
 * whether that entry exists. If not found, *ITEM will be NULL. Otherwise,
 * the DESERIALIZER is called with that entry and the BATON provided
 * and will extract the desired information. The result is set in *ITEM.
 * Allocations will be done in POOL.
 */
static svn_error_t *
membuffer_cache_get_partial(svn_membuffer_t *cache,
                            entry_key_t key,
                            void **item,
                            svn_boolean_t *found,
                            svn_cache__partial_getter_func_t deserializer,
                            void *baton,
                            DEBUG_CACHE_MEMBUFFER_TAG_ARG
                            apr_pool_t *result_pool)
{
  apr_uint32_t group_index = get_group_index(&cache, key);

  WITH_READ_LOCK(cache,
                 membuffer_cache_get_partial_internal
                     (cache, group_index, key, item, found,
                      deserializer, baton, DEBUG_CACHE_MEMBUFFER_TAG
                      result_pool));

  return SVN_NO_ERROR;
}

/* Look for the cache entry in group GROUP_INDEX of CACHE, identified
 * by the hash value TO_FIND. If no entry has been found, the function
 * returns without modifying the cache.
 *
 * Otherwise, FUNC is called with that entry and the BATON provided
 * and may modify the cache entry. Allocations will be done in POOL.
 *
 * Note: This function requires the caller to serialization access.
 * Don't call it directly, call membuffer_cache_set_partial instead.
 */
static svn_error_t *
membuffer_cache_set_partial_internal(svn_membuffer_t *cache,
                                     apr_uint32_t group_index,
                                     entry_key_t to_find,
                                     svn_cache__partial_setter_func_t func,
                                     void *baton,
                                     DEBUG_CACHE_MEMBUFFER_TAG_ARG
                                     apr_pool_t *scratch_pool)
{
  /* cache item lookup
   */
  entry_t *entry = find_entry(cache, group_index, to_find, FALSE);
  cache->total_reads++;

  /* this function is a no-op if the item is not in cache
   */
  if (entry != NULL)
    {
      svn_error_t *err;

      /* access the serialized cache item */
      char *data = (char*)cache->data + entry->offset;
      char *orig_data = data;
      apr_size_t size = entry->size;

      increment_hit_counters(cache, entry);
      cache->total_writes++;

#ifdef SVN_DEBUG_CACHE_MEMBUFFER

      /* Check for overlapping entries.
       */
      SVN_ERR_ASSERT(entry->next == NO_INDEX ||
                     entry->offset + size
                        <= get_entry(cache, entry->next)->offset);

      /* Compare original content, type and key (hashes)
       */
      SVN_ERR(store_content_part(tag, data, size, scratch_pool));
      SVN_ERR(assert_equal_tags(&entry->tag, tag));

#endif

      /* modify it, preferably in-situ.
       */
      err = func((void **)&data, &size, baton, scratch_pool);

      if (err)
        {
          /* Something somewhere when wrong while FUNC was modifying the
           * changed item. Thus, it might have become invalid /corrupted.
           * We better drop that.
           */
          drop_entry(cache, entry);

          return err;
        }
      else
        {
          /* if the modification caused a re-allocation, we need to remove
           * the old entry and to copy the new data back into cache.
           */
          if (data != orig_data)
            {
              /* Remove the old entry and try to make space for the new one.
               */
              drop_entry(cache, entry);
              if (   (cache->max_entry_size >= size)
                  && ensure_data_insertable_l1(cache, size))
                {
                  /* Write the new entry.
                   */
                  entry = find_entry(cache, group_index, to_find, TRUE);
                  entry->size = (apr_uint32_t) size;
                  entry->offset = cache->l1.current_data;
                  if (size)
                    memcpy(cache->data + entry->offset, data, size);

                  /* Link the entry properly.
                   */
                  insert_entry(cache, entry);
                }
            }

#ifdef SVN_DEBUG_CACHE_MEMBUFFER

          /* Remember original content, type and key (hashes)
           */
          SVN_ERR(store_content_part(tag, data, size, scratch_pool));
          memcpy(&entry->tag, tag, sizeof(*tag));

#endif
        }
    }

  return SVN_NO_ERROR;
}

/* Look for the cache entry identified by KEY. If no entry
 * has been found, the function returns without modifying the cache.
 * Otherwise, FUNC is called with that entry and the BATON provided
 * and may modify the cache entry. Allocations will be done in POOL.
 */
static svn_error_t *
membuffer_cache_set_partial(svn_membuffer_t *cache,
                            entry_key_t key,
                            svn_cache__partial_setter_func_t func,
                            void *baton,
                            DEBUG_CACHE_MEMBUFFER_TAG_ARG
                            apr_pool_t *scratch_pool)
{
  /* cache item lookup
   */
  apr_uint32_t group_index = get_group_index(&cache, key);
  WITH_WRITE_LOCK(cache,
                  membuffer_cache_set_partial_internal
                     (cache, group_index, key, func, baton,
                      DEBUG_CACHE_MEMBUFFER_TAG
                      scratch_pool));

  /* done here -> unlock the cache
   */
  return SVN_NO_ERROR;
}

/* Implement the svn_cache__t interface on top of a shared membuffer cache.
 *
 * Because membuffer caches tend to be very large, there will be rather few
 * of them (usually only one). Thus, the same instance shall be used as the
 * backend to many application-visible svn_cache__t instances. This should
 * also achieve global resource usage fairness.
 *
 * To accommodate items from multiple resources, the individual keys must be
 * unique over all sources. This is achieved by simply adding a prefix key
 * that unambiguously identifies the item's context (e.g. path to the
 * respective repository). The prefix will be set upon construction of the
 * svn_cache__t instance.
 */

/* Stores the combined key value for the given key.  It will be used by
 * combine_key() to short-circuit expensive hash calculations.
 */
typedef struct last_access_key_t
{
  /* result of key combining */
  entry_key_t combined_key;

  /* length of the key (or APR_HASH_KEY_STRING if not used) */
  apr_ssize_t key_len;

  /* the original key.  Only KEY_LEN bytes are valid.  We use uint32 for
   * better compatibility with pseudo-md5 functions. */
  apr_uint32_t key[64];
} last_access_key_t;

/* Internal cache structure (used in svn_cache__t.cache_internal) basically
 * holding the additional parameters needed to call the respective membuffer
 * functions.
 */
typedef struct svn_membuffer_cache_t
{
  /* this is where all our data will end up in
   */
  svn_membuffer_t *membuffer;

  /* use this conversion function when inserting an item into the memcache
   */
  svn_cache__serialize_func_t serializer;

  /* use this conversion function when reading an item from the memcache
   */
  svn_cache__deserialize_func_t deserializer;

  /* Prepend this byte sequence to any key passed to us.
   * This makes (very likely) our keys different from all keys used
   * by other svn_membuffer_cache_t instances.
   */
  entry_key_t prefix;

  /* The tail of the prefix string. It is being used as a developer-visible
   * ID for this cache instance.
   */
  char info_prefix[PREFIX_TAIL_LEN];

  /* length of the keys that will be passed to us through the
   * svn_cache_t interface. May be APR_HASH_KEY_STRING.
   */
  apr_ssize_t key_len;

  /* priority class for all items written through this interface */
  apr_uint32_t priority;

  /* Temporary buffer containing the hash key for the current access
   */
  entry_key_t combined_key;

  /* cache for the last key used.
   * Will be NULL for caches with short fix-sized keys.
   */
  last_access_key_t *last_access;

  /* if enabled, this will serialize the access to this instance.
   */
  svn_mutex__t *mutex;
} svn_membuffer_cache_t;

/* After an estimated ALLOCATIONS_PER_POOL_CLEAR allocations, we should
 * clear the svn_membuffer_cache_t.pool to keep memory consumption in check.
 */
#define ALLOCATIONS_PER_POOL_CLEAR 10

/* Basically calculate a hash value for KEY of length KEY_LEN, combine it
 * with the CACHE->PREFIX and write the result in CACHE->COMBINED_KEY.
 * This could replace combine_key() entirely but we actually use it only
 * when the quick path failed.
 */
static void
combine_long_key(svn_membuffer_cache_t *cache,
                 const void *key,
                 apr_ssize_t key_len)
{
  assert(cache->last_access);

  /* handle variable-length keys */
  if (key_len == APR_HASH_KEY_STRING)
    key_len = strlen((const char *) key);

  /* same key as the last time? -> short-circuit */
  if (   key_len == cache->last_access->key_len
      && memcmp(key, cache->last_access->key, key_len) == 0)
    {
      memcpy(cache->combined_key, cache->last_access->combined_key,
             sizeof(cache->combined_key));
    }
  else if (key_len >= 64)
    {
      /* relatively long key.  Use the generic, slow hash code for it */
      apr_md5((unsigned char*)cache->combined_key, key, key_len);
      cache->combined_key[0] ^= cache->prefix[0];
      cache->combined_key[1] ^= cache->prefix[1];

      /* is the key short enough to cache the result? */
      if (key_len <= sizeof(cache->last_access->key))
        {
          memcpy(cache->last_access->combined_key, cache->combined_key,
                 sizeof(cache->combined_key));
          cache->last_access->key_len = key_len;
          memcpy(cache->last_access->key, key, key_len);
        }
    }
  else
    {
      /* shorter keys use efficient hash code and *do* cache the results */
      cache->last_access->key_len = key_len;
      if (key_len < 16)
        {
          memset(cache->last_access->key, 0, 16);
          memcpy(cache->last_access->key, key, key_len);

          svn__pseudo_md5_15((apr_uint32_t *)cache->combined_key,
                             cache->last_access->key);
        }
      else if (key_len < 32)
        {
          memset(cache->last_access->key, 0, 32);
          memcpy(cache->last_access->key, key, key_len);

          svn__pseudo_md5_31((apr_uint32_t *)cache->combined_key,
                             cache->last_access->key);
        }
      else
        {
          memset(cache->last_access->key, 0, 64);
          memcpy(cache->last_access->key, key, key_len);

          svn__pseudo_md5_63((apr_uint32_t *)cache->combined_key,
                             cache->last_access->key);
        }

      cache->combined_key[0] ^= cache->prefix[0];
      cache->combined_key[1] ^= cache->prefix[1];

      memcpy(cache->last_access->combined_key, cache->combined_key,
             sizeof(cache->combined_key));
    }
}

/* Basically calculate a hash value for KEY of length KEY_LEN, combine it
 * with the CACHE->PREFIX and write the result in CACHE->COMBINED_KEY.
 */
static void
combine_key(svn_membuffer_cache_t *cache,
            const void *key,
            apr_ssize_t key_len)
{
  /* copy of *key, padded with 0 */
  apr_uint64_t data[2];

  /* short, fixed-size keys are the most common case */
  if (key_len == 16)
    {
      data[0] = ((const apr_uint64_t *)key)[0];
      data[1] = ((const apr_uint64_t *)key)[1];
    }
  else if (key_len == 8)
    {
      data[0] = ((const apr_uint64_t *)key)[0];
      data[1] = 0;
    }
  else if (key_len != APR_HASH_KEY_STRING && key_len < 16)
    {
      data[0] = 0;
      data[1] = 0;
      memcpy(data, key, key_len);
    }
  else
    {
      /* longer or variably sized keys */
      combine_long_key(cache, key, key_len);
      return;
    }

  /* scramble key DATA.  All of this must be reversible to prevent key
   * collisions.  So, we limit ourselves to xor and permutations. */
  data[1] = (data[1] << 27) | (data[1] >> 37);
  data[1] ^= data[0] & 0xffff;
  data[0] ^= data[1] & APR_UINT64_C(0xffffffffffff0000);

  /* combine with this cache's namespace */
  cache->combined_key[0] = data[0] ^ cache->prefix[0];
  cache->combined_key[1] = data[1] ^ cache->prefix[1];
}

/* Implement svn_cache__vtable_t.get (not thread-safe)
 */
static svn_error_t *
svn_membuffer_cache_get(void **value_p,
                        svn_boolean_t *found,
                        void *cache_void,
                        const void *key,
                        apr_pool_t *result_pool)
{
  svn_membuffer_cache_t *cache = cache_void;

  DEBUG_CACHE_MEMBUFFER_INIT_TAG(result_pool)

  /* special case */
  if (key == NULL)
    {
      *value_p = NULL;
      *found = FALSE;

      return SVN_NO_ERROR;
    }

  /* construct the full, i.e. globally unique, key by adding
   * this cache instances' prefix
   */
  combine_key(cache, key, cache->key_len);

  /* Look the item up. */
  SVN_ERR(membuffer_cache_get(cache->membuffer,
                              cache->combined_key,
                              value_p,
                              cache->deserializer,
                              DEBUG_CACHE_MEMBUFFER_TAG
                              result_pool));

  /* return result */
  *found = *value_p != NULL;

  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.has_key (not thread-safe)
 */
static svn_error_t *
svn_membuffer_cache_has_key(svn_boolean_t *found,
                            void *cache_void,
                            const void *key,
                            apr_pool_t *scratch_pool)
{
  svn_membuffer_cache_t *cache = cache_void;

  /* special case */
  if (key == NULL)
    {
      *found = FALSE;

      return SVN_NO_ERROR;
    }

  /* construct the full, i.e. globally unique, key by adding
   * this cache instances' prefix
   */
  combine_key(cache, key, cache->key_len);

  /* Look the item up. */
  SVN_ERR(membuffer_cache_has_key(cache->membuffer,
                                  cache->combined_key,
                                  found));

  /* return result */
  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.set (not thread-safe)
 */
static svn_error_t *
svn_membuffer_cache_set(void *cache_void,
                        const void *key,
                        void *value,
                        apr_pool_t *scratch_pool)
{
  svn_membuffer_cache_t *cache = cache_void;

  DEBUG_CACHE_MEMBUFFER_INIT_TAG(scratch_pool)

  /* special case */
  if (key == NULL)
    return SVN_NO_ERROR;

  /* construct the full, i.e. globally unique, key by adding
   * this cache instances' prefix
   */
  combine_key(cache, key, cache->key_len);

  /* (probably) add the item to the cache. But there is no real guarantee
   * that the item will actually be cached afterwards.
   */
  return membuffer_cache_set(cache->membuffer,
                             cache->combined_key,
                             value,
                             cache->serializer,
                             cache->priority,
                             DEBUG_CACHE_MEMBUFFER_TAG
                             scratch_pool);
}

/* Implement svn_cache__vtable_t.iter as "not implemented"
 */
static svn_error_t *
svn_membuffer_cache_iter(svn_boolean_t *completed,
                          void *cache_void,
                          svn_iter_apr_hash_cb_t user_cb,
                          void *user_baton,
                          apr_pool_t *scratch_pool)
{
  return svn_error_create(SVN_ERR_UNSUPPORTED_FEATURE, NULL,
                          _("Can't iterate a membuffer-based cache"));
}

/* Implement svn_cache__vtable_t.get_partial (not thread-safe)
 */
static svn_error_t *
svn_membuffer_cache_get_partial(void **value_p,
                                svn_boolean_t *found,
                                void *cache_void,
                                const void *key,
                                svn_cache__partial_getter_func_t func,
                                void *baton,
                                apr_pool_t *result_pool)
{
  svn_membuffer_cache_t *cache = cache_void;

  DEBUG_CACHE_MEMBUFFER_INIT_TAG(result_pool)

  if (key == NULL)
    {
      *value_p = NULL;
      *found = FALSE;

      return SVN_NO_ERROR;
    }

  combine_key(cache, key, cache->key_len);
  SVN_ERR(membuffer_cache_get_partial(cache->membuffer,
                                      cache->combined_key,
                                      value_p,
                                      found,
                                      func,
                                      baton,
                                      DEBUG_CACHE_MEMBUFFER_TAG
                                      result_pool));

  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.set_partial (not thread-safe)
 */
static svn_error_t *
svn_membuffer_cache_set_partial(void *cache_void,
                                const void *key,
                                svn_cache__partial_setter_func_t func,
                                void *baton,
                                apr_pool_t *scratch_pool)
{
  svn_membuffer_cache_t *cache = cache_void;

  DEBUG_CACHE_MEMBUFFER_INIT_TAG(scratch_pool)

  if (key != NULL)
    {
      combine_key(cache, key, cache->key_len);
      SVN_ERR(membuffer_cache_set_partial(cache->membuffer,
                                          cache->combined_key,
                                          func,
                                          baton,
                                          DEBUG_CACHE_MEMBUFFER_TAG
                                          scratch_pool));
    }
  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.is_cachable
 * (thread-safe even without mutex)
 */
static svn_boolean_t
svn_membuffer_cache_is_cachable(void *cache_void, apr_size_t size)
{
  /* Don't allow extremely large element sizes. Otherwise, the cache
   * might by thrashed by a few extremely large entries. And the size
   * must be small enough to be stored in a 32 bit value.
   */
  svn_membuffer_cache_t *cache = cache_void;
  return cache->priority > SVN_CACHE__MEMBUFFER_DEFAULT_PRIORITY
       ? cache->membuffer->l2.size >= size && MAX_ITEM_SIZE >= size
       : size <= cache->membuffer->max_entry_size;
}

/* Add statistics of SEGMENT to INFO.  If INCLUDE_HISTOGRAM is TRUE,
 * accumulate index bucket fill levels in INFO->HISTOGRAM.
 */
static svn_error_t *
svn_membuffer_get_segment_info(svn_membuffer_t *segment,
                               svn_cache__info_t *info,
                               svn_boolean_t include_histogram)
{
  apr_uint32_t i;

  info->data_size += segment->l1.size + segment->l2.size;
  info->used_size += segment->data_used;
  info->total_size += segment->l1.size + segment->l2.size +
      segment->group_count * GROUP_SIZE * sizeof(entry_t);

  info->used_entries += segment->used_entries;
  info->total_entries += segment->group_count * GROUP_SIZE;

  if (include_histogram)
    for (i = 0; i < segment->group_count; ++i)
      if (is_group_initialized(segment, i))
        {
          entry_group_t *chain_end
            = last_group_in_chain(segment, &segment->directory[i]);
          apr_size_t use
            = MIN(chain_end->header.used,
                  sizeof(info->histogram) / sizeof(info->histogram[0]) - 1);
          info->histogram[use]++;
        }

  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.get_info
 * (thread-safe even without mutex)
 */
static svn_error_t *
svn_membuffer_cache_get_info(void *cache_void,
                             svn_cache__info_t *info,
                             svn_boolean_t reset,
                             apr_pool_t *result_pool)
{
  svn_membuffer_cache_t *cache = cache_void;
  apr_uint32_t i;

  /* cache front-end specific data */

  info->id = apr_pstrdup(result_pool, cache->info_prefix);

  /* collect info from shared cache back-end */

  for (i = 0; i < cache->membuffer->segment_count; ++i)
    {
      svn_membuffer_t *segment = cache->membuffer + i;
      WITH_READ_LOCK(segment,
                     svn_membuffer_get_segment_info(segment, info, FALSE));
    }

  return SVN_NO_ERROR;
}


/* the v-table for membuffer-based caches (single-threaded access)
 */
static svn_cache__vtable_t membuffer_cache_vtable = {
  svn_membuffer_cache_get,
  svn_membuffer_cache_has_key,
  svn_membuffer_cache_set,
  svn_membuffer_cache_iter,
  svn_membuffer_cache_is_cachable,
  svn_membuffer_cache_get_partial,
  svn_membuffer_cache_set_partial,
  svn_membuffer_cache_get_info
};

/* Implement svn_cache__vtable_t.get and serialize all cache access.
 */
static svn_error_t *
svn_membuffer_cache_get_synced(void **value_p,
                               svn_boolean_t *found,
                               void *cache_void,
                               const void *key,
                               apr_pool_t *result_pool)
{
  svn_membuffer_cache_t *cache = cache_void;
  SVN_MUTEX__WITH_LOCK(cache->mutex,
                       svn_membuffer_cache_get(value_p,
                                               found,
                                               cache_void,
                                               key,
                                               result_pool));

  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.has_key and serialize all cache access.
 */
static svn_error_t *
svn_membuffer_cache_has_key_synced(svn_boolean_t *found,
                                   void *cache_void,
                                   const void *key,
                                   apr_pool_t *result_pool)
{
  svn_membuffer_cache_t *cache = cache_void;
  SVN_MUTEX__WITH_LOCK(cache->mutex,
                       svn_membuffer_cache_has_key(found,
                                                   cache_void,
                                                   key,
                                                   result_pool));

  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.set and serialize all cache access.
 */
static svn_error_t *
svn_membuffer_cache_set_synced(void *cache_void,
                               const void *key,
                               void *value,
                               apr_pool_t *scratch_pool)
{
  svn_membuffer_cache_t *cache = cache_void;
  SVN_MUTEX__WITH_LOCK(cache->mutex,
                       svn_membuffer_cache_set(cache_void,
                                               key,
                                               value,
                                               scratch_pool));

  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.get_partial and serialize all cache access.
 */
static svn_error_t *
svn_membuffer_cache_get_partial_synced(void **value_p,
                                       svn_boolean_t *found,
                                       void *cache_void,
                                       const void *key,
                                       svn_cache__partial_getter_func_t func,
                                       void *baton,
                                       apr_pool_t *result_pool)
{
  svn_membuffer_cache_t *cache = cache_void;
  SVN_MUTEX__WITH_LOCK(cache->mutex,
                       svn_membuffer_cache_get_partial(value_p,
                                                       found,
                                                       cache_void,
                                                       key,
                                                       func,
                                                       baton,
                                                       result_pool));

  return SVN_NO_ERROR;
}

/* Implement svn_cache__vtable_t.set_partial and serialize all cache access.
 */
static svn_error_t *
svn_membuffer_cache_set_partial_synced(void *cache_void,
                                       const void *key,
                                       svn_cache__partial_setter_func_t func,
                                       void *baton,
                                       apr_pool_t *scratch_pool)
{
  svn_membuffer_cache_t *cache = cache_void;
  SVN_MUTEX__WITH_LOCK(cache->mutex,
                       svn_membuffer_cache_set_partial(cache_void,
                                                       key,
                                                       func,
                                                       baton,
                                                       scratch_pool));

  return SVN_NO_ERROR;
}

/* the v-table for membuffer-based caches with multi-threading support)
 */
static svn_cache__vtable_t membuffer_cache_synced_vtable = {
  svn_membuffer_cache_get_synced,
  svn_membuffer_cache_has_key_synced,
  svn_membuffer_cache_set_synced,
  svn_membuffer_cache_iter,               /* no sync required */
  svn_membuffer_cache_is_cachable,        /* no sync required */
  svn_membuffer_cache_get_partial_synced,
  svn_membuffer_cache_set_partial_synced,
  svn_membuffer_cache_get_info            /* no sync required */
};

/* standard serialization function for svn_stringbuf_t items.
 * Implements svn_cache__serialize_func_t.
 */
static svn_error_t *
serialize_svn_stringbuf(void **buffer,
                        apr_size_t *buffer_size,
                        void *item,
                        apr_pool_t *result_pool)
{
  svn_stringbuf_t *value_str = item;

  *buffer = value_str->data;
  *buffer_size = value_str->len + 1;

  return SVN_NO_ERROR;
}

/* standard de-serialization function for svn_stringbuf_t items.
 * Implements svn_cache__deserialize_func_t.
 */
static svn_error_t *
deserialize_svn_stringbuf(void **item,
                          void *buffer,
                          apr_size_t buffer_size,
                          apr_pool_t *result_pool)
{
  svn_stringbuf_t *value_str = apr_palloc(result_pool, sizeof(svn_stringbuf_t));

  value_str->pool = result_pool;
  value_str->blocksize = buffer_size;
  value_str->data = buffer;
  value_str->len = buffer_size-1;
  *item = value_str;

  return SVN_NO_ERROR;
}

/* Construct a svn_cache__t object on top of a shared memcache.
 */
svn_error_t *
svn_cache__create_membuffer_cache(svn_cache__t **cache_p,
                                  svn_membuffer_t *membuffer,
                                  svn_cache__serialize_func_t serializer,
                                  svn_cache__deserialize_func_t deserializer,
                                  apr_ssize_t klen,
                                  const char *prefix,
                                  apr_uint32_t priority,
                                  svn_boolean_t thread_safe,
                                  apr_pool_t *result_pool,
                                  apr_pool_t *scratch_pool)
{
  svn_checksum_t *checksum;

  /* allocate the cache header structures
   */
  svn_cache__t *wrapper = apr_pcalloc(result_pool, sizeof(*wrapper));
  svn_membuffer_cache_t *cache = apr_palloc(result_pool, sizeof(*cache));

  /* initialize our internal cache header
   */
  cache->membuffer = membuffer;
  cache->serializer = serializer
                    ? serializer
                    : serialize_svn_stringbuf;
  cache->deserializer = deserializer
                      ? deserializer
                      : deserialize_svn_stringbuf;
  get_prefix_tail(prefix, cache->info_prefix);
  cache->priority = priority;
  cache->key_len = klen;

  SVN_ERR(svn_mutex__init(&cache->mutex, thread_safe, result_pool));

  /* for performance reasons, we don't actually store the full prefix but a
   * hash value of it
   */
  SVN_ERR(svn_checksum(&checksum,
                       svn_checksum_md5,
                       prefix,
                       strlen(prefix),
                       scratch_pool));
  memcpy(cache->prefix, checksum->digest, sizeof(cache->prefix));

  /* fix-length keys of 16 bytes or under don't need a buffer because we
   * can use a very fast key combining algorithm. */
  if ((klen == APR_HASH_KEY_STRING) ||  klen > sizeof(entry_key_t))
    {
      cache->last_access = apr_pcalloc(result_pool, sizeof(*cache->last_access));
      cache->last_access->key_len = APR_HASH_KEY_STRING;
    }
  else
    {
      cache->last_access = NULL;
    }

  /* initialize the generic cache wrapper
   */
  wrapper->vtable = thread_safe ? &membuffer_cache_synced_vtable
                                : &membuffer_cache_vtable;
  wrapper->cache_internal = cache;
  wrapper->error_handler = 0;
  wrapper->error_baton = 0;
  wrapper->pretend_empty = !!getenv("SVN_X_DOES_NOT_MARK_THE_SPOT");

  *cache_p = wrapper;
  return SVN_NO_ERROR;
}

static svn_error_t *
svn_membuffer_get_global_segment_info(svn_membuffer_t *segment,
                                      svn_cache__info_t *info)
{
  info->gets += segment->total_reads;
  info->sets += segment->total_writes;
  info->hits += segment->total_hits;

  WITH_READ_LOCK(segment,
                  svn_membuffer_get_segment_info(segment, info, TRUE));

  return SVN_NO_ERROR;
}

svn_cache__info_t *
svn_cache__membuffer_get_global_info(apr_pool_t *pool)
{
  apr_uint32_t i;

  svn_membuffer_t *membuffer = svn_cache__get_global_membuffer_cache();
  svn_cache__info_t *info = apr_pcalloc(pool, sizeof(*info));

  /* cache front-end specific data */

  info->id = "membuffer globals";

  /* collect info from shared cache back-end */

  for (i = 0; i < membuffer->segment_count; ++i)
    svn_error_clear(svn_membuffer_get_global_segment_info(membuffer + i,
                                                          info));

  return info;
}
