/* io-test.c --- tests for some i/o functions
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

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include <apr.h>
#include <apr_version.h>

#include "svn_pools.h"
#include "svn_string.h"
#include "private/svn_skel.h"
#include "private/svn_dep_compat.h"

#include "../svn_test.h"
#include "../svn_test_fs.h"


/* Helpers to create the test data directory. */

#define TEST_DIR_PREFIX "io-test-temp"

/* The definition for the test data files. */
struct test_file_definition_t
  {
    /* The name of the test data file. */
    const char* const name;

    /* The string needs to contain up to 5 bytes, they
     * are interpreded as:
     * - first byte
     * - filler between first and medium byte
     * - medium byte (the byte in the middle of the file)
     * - filler between medium and last byte
     * - last byte.
     * If the string is shorter than the file length,
     * the test will fail. */
    const char* const data;

    /* The size of the file actually to create. */
    const apr_off_t size;

    /* The created path of the file. Will be filled in
     * by create_test_file() */
    char* created_path;
  };

static struct test_file_definition_t test_file_definitions_template[] =
  {
    {"empty",                 "",      0},
    {"single_a",              "a",     1},
    {"single_b",              "b",     1},
    {"hundred_a",             "aaaaa", 100},
    {"hundred_b",             "bbbbb", 100},
    {"hundred_b1",            "baaaa", 100},
    {"hundred_b2",            "abaaa", 100},
    {"hundred_b3",            "aabaa", 100},
    {"hundred_b4",            "aaaba", 100},
    {"hundred_b5",            "aaaab", 100},
    {"chunk_minus_one_a",     "aaaaa", SVN__STREAM_CHUNK_SIZE - 1},
    {"chunk_minus_one_b1",    "baaaa", SVN__STREAM_CHUNK_SIZE - 1},
    {"chunk_minus_one_b2",    "abaaa", SVN__STREAM_CHUNK_SIZE - 1},
    {"chunk_minus_one_b3",    "aabaa", SVN__STREAM_CHUNK_SIZE - 1},
    {"chunk_minus_one_b4",    "aaaba", SVN__STREAM_CHUNK_SIZE - 1},
    {"chunk_minus_one_b5",    "aaaab", SVN__STREAM_CHUNK_SIZE - 1},
    {"chunk_a",               "aaaaa", SVN__STREAM_CHUNK_SIZE},
    {"chunk_b1",              "baaaa", SVN__STREAM_CHUNK_SIZE},
    {"chunk_b2",              "abaaa", SVN__STREAM_CHUNK_SIZE},
    {"chunk_b3",              "aabaa", SVN__STREAM_CHUNK_SIZE},
    {"chunk_b4",              "aaaba", SVN__STREAM_CHUNK_SIZE},
    {"chunk_b5",              "aaaab", SVN__STREAM_CHUNK_SIZE},
    {"chunk_plus_one_a",      "aaaaa", SVN__STREAM_CHUNK_SIZE + 1},
    {"chunk_plus_one_b1",     "baaaa", SVN__STREAM_CHUNK_SIZE + 1},
    {"chunk_plus_one_b2",     "abaaa", SVN__STREAM_CHUNK_SIZE + 1},
    {"chunk_plus_one_b3",     "aabaa", SVN__STREAM_CHUNK_SIZE + 1},
    {"chunk_plus_one_b4",     "aaaba", SVN__STREAM_CHUNK_SIZE + 1},
    {"chunk_plus_one_b5",     "aaaab", SVN__STREAM_CHUNK_SIZE + 1},
    {"twochunk_minus_one_a",  "aaaaa", SVN__STREAM_CHUNK_SIZE*2 - 1},
    {"twochunk_minus_one_b1", "baaaa", SVN__STREAM_CHUNK_SIZE*2 - 1},
    {"twochunk_minus_one_b2", "abaaa", SVN__STREAM_CHUNK_SIZE*2 - 1},
    {"twochunk_minus_one_b3", "aabaa", SVN__STREAM_CHUNK_SIZE*2 - 1},
    {"twochunk_minus_one_b4", "aaaba", SVN__STREAM_CHUNK_SIZE*2 - 1},
    {"twochunk_minus_one_b5", "aaaab", SVN__STREAM_CHUNK_SIZE*2 - 1},
    {"twochunk_a",            "aaaaa", SVN__STREAM_CHUNK_SIZE*2},
    {"twochunk_b1",           "baaaa", SVN__STREAM_CHUNK_SIZE*2},
    {"twochunk_b2",           "abaaa", SVN__STREAM_CHUNK_SIZE*2},
    {"twochunk_b3",           "aabaa", SVN__STREAM_CHUNK_SIZE*2},
    {"twochunk_b4",           "aaaba", SVN__STREAM_CHUNK_SIZE*2},
    {"twochunk_b5",           "aaaab", SVN__STREAM_CHUNK_SIZE*2},
    {"twochunk_plus_one_a",   "aaaaa", SVN__STREAM_CHUNK_SIZE*2 + 1},
    {"twochunk_plus_one_b1",  "baaaa", SVN__STREAM_CHUNK_SIZE*2 + 1},
    {"twochunk_plus_one_b2",  "abaaa", SVN__STREAM_CHUNK_SIZE*2 + 1},
    {"twochunk_plus_one_b3",  "aabaa", SVN__STREAM_CHUNK_SIZE*2 + 1},
    {"twochunk_plus_one_b4",  "aaaba", SVN__STREAM_CHUNK_SIZE*2 + 1},
    {"twochunk_plus_one_b5",  "aaaab", SVN__STREAM_CHUNK_SIZE*2 + 1},
    {0},
  };

/* Function to prepare a single test file */

static svn_error_t *
create_test_file(struct test_file_definition_t* definition,
                 const char *testname,
                 apr_pool_t *pool,
                 apr_pool_t *scratch_pool)
{
  apr_status_t status = 0;
  apr_file_t *file_h;
  apr_off_t midpos = definition->size / 2;
  svn_error_t *err = NULL;
  int i;
  const char *test_dir = apr_pstrcat(pool, TEST_DIR_PREFIX, testname, NULL);

  if (definition->size < 5)
    SVN_ERR_ASSERT(strlen(definition->data) >= (apr_size_t)definition->size);
  else
    SVN_ERR_ASSERT(strlen(definition->data) >= 5);


  definition->created_path = svn_dirent_join(test_dir,
                                             definition->name,
                                             pool);

  SVN_ERR(svn_io_file_open(&file_h,
                           definition->created_path,
                           (APR_WRITE | APR_CREATE | APR_EXCL | APR_BUFFERED),
                           APR_OS_DEFAULT,
                           scratch_pool));

  for (i=1; i <= definition->size; i += 1)
    {
      char c;
      if (i == 1)
        c = definition->data[0];
      else if (i < midpos)
        c = definition->data[1];
      else if (i == midpos)
        c = definition->data[2];
      else if (i < definition->size)
        c = definition->data[3];
      else
        c = definition->data[4];

      status = apr_file_putc(c, file_h);

      if (status)
        break;
    }

  if (status)
    err = svn_error_wrap_apr(status, "Can't write to file '%s'",
                              definition->name);

  return svn_error_compose_create(err,
                        svn_io_file_close(file_h, scratch_pool));
}

/* Function to prepare the whole set of on-disk files to be compared. */
static svn_error_t *
create_comparison_candidates(struct test_file_definition_t **definitions,
                             const char *testname,
                             apr_pool_t *pool)
{
  svn_node_kind_t kind;
  apr_pool_t *iterpool = svn_pool_create(pool);
  struct test_file_definition_t *candidate;
  svn_error_t *err = SVN_NO_ERROR;
  apr_size_t count = 0;
  const char *test_dir = apr_pstrcat(pool, TEST_DIR_PREFIX,
                                     testname, NULL);

  /* If there's already a directory named io-test-temp, delete it.
     Doing things this way means that repositories stick around after
     a failure for postmortem analysis, but also that tests can be
     re-run without cleaning out the repositories created by prior
     runs.  */
  SVN_ERR(svn_io_check_path(test_dir, &kind, pool));

  if (kind == svn_node_dir)
    SVN_ERR(svn_io_remove_dir2(test_dir, TRUE, NULL, NULL, pool));
  else if (kind != svn_node_none)
    return svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                             "There is already a file named '%s'",
                             test_dir);

  SVN_ERR(svn_io_dir_make(test_dir, APR_OS_DEFAULT, pool));

  svn_test_add_dir_cleanup(test_dir);

  for (candidate = test_file_definitions_template;
       candidate->name != NULL;
       candidate += 1)
    count++;

  *definitions = apr_pmemdup(pool, test_file_definitions_template,
                             (count + 1) * sizeof(**definitions));
  for (candidate = *definitions; candidate->name != NULL; candidate += 1)
    {
      svn_pool_clear(iterpool);
      err = create_test_file(candidate, testname, pool, iterpool);
      if (err)
        break;
    }

  svn_pool_destroy(iterpool);

  return err;
}


/* Functions to check the 2-way and 3-way file comparison functions.  */

/* Test 2-way file size checking */
static svn_error_t *
test_two_file_size_comparison(apr_pool_t *scratch_pool)
{
  struct test_file_definition_t *inner, *outer;
  svn_boolean_t actual;
  svn_boolean_t expected;
  svn_error_t *err = SVN_NO_ERROR;
  svn_error_t *cmp_err;
  apr_pool_t *iterpool = svn_pool_create(scratch_pool);
  struct test_file_definition_t *test_file_definitions;

  SVN_ERR(create_comparison_candidates(&test_file_definitions,
                                       "test_two_file_size_comparison",
                                       scratch_pool));

  for (outer = test_file_definitions; outer->name != NULL; outer += 1)
    {
#ifdef SVN_IO_TEST_ALL_PERMUTATIONS
      inner = test_file_definitions;
#else
      inner = outer;
#endif
      for (; inner->name != NULL; inner += 1)
        {
          svn_pool_clear(iterpool);

          expected = inner->size != outer->size;

          cmp_err = svn_io_filesizes_different_p(&actual,
                                                 inner->created_path,
                                                 outer->created_path,
                                                 iterpool);

          if (cmp_err)
            {
              err = svn_error_compose_create(err, cmp_err);
            }
          else if (expected != actual)
            {
              err = svn_error_compose_create(err,
                  svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                                   "size comparison problem: '%s' and '%s'",
                                   inner->created_path,
                                   outer->created_path));
            }
        }
    }

  svn_pool_destroy(iterpool);
  return err;
}


/* Test 2-way file content checking */
static svn_error_t *
test_two_file_content_comparison(apr_pool_t *scratch_pool)
{
  struct test_file_definition_t *inner, *outer;
  svn_boolean_t actual;
  svn_boolean_t expected;
  svn_error_t *err = SVN_NO_ERROR;
  svn_error_t *cmp_err;
  apr_pool_t *iterpool = svn_pool_create(scratch_pool);
  struct test_file_definition_t *test_file_definitions;

  SVN_ERR(create_comparison_candidates(&test_file_definitions,
                                       "test_two_file_content_comparison",
                                       scratch_pool));

  for (outer = test_file_definitions; outer->name != NULL; outer += 1)
    {
#ifdef SVN_IO_TEST_ALL_PERMUTATIONS
      inner = test_file_definitions;
#else
      inner = outer;
#endif
      for (; inner->name != NULL; inner += 1)
        {
          svn_pool_clear(iterpool);

          expected = inner->size == outer->size
            && strcmp(inner->data, outer->data) == 0;

          cmp_err = svn_io_files_contents_same_p(&actual,
                                                 inner->created_path,
                                                 outer->created_path,
                                                 iterpool);

          if (cmp_err)
            {
              err = svn_error_compose_create(err, cmp_err);
            }
          else
            {
              if (expected != actual)
                  err = svn_error_compose_create(err,
                      svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                                 "content comparison problem: '%s' and '%s'",
                                 inner->created_path,
                                 outer->created_path));
            }
        }
    }

  svn_pool_destroy(iterpool);
  return err;
}


/* Test 3-way file size checking */
static svn_error_t *
test_three_file_size_comparison(apr_pool_t *scratch_pool)
{
  struct test_file_definition_t *inner, *middle, *outer;
  svn_boolean_t actual12, actual23, actual13;
  svn_boolean_t expected12, expected23, expected13;
  svn_error_t *err = SVN_NO_ERROR;
  svn_error_t *cmp_err;
  apr_pool_t *iterpool = svn_pool_create(scratch_pool);
  struct test_file_definition_t *test_file_definitions;

  SVN_ERR(create_comparison_candidates(&test_file_definitions,
                                       "test_three_file_size_comparison",
                                       scratch_pool));

  for (outer = test_file_definitions; outer->name != NULL; outer += 1)
    {
#ifdef SVN_IO_TEST_ALL_PERMUTATIONS
      middle = test_file_definitions;
#else
      middle = outer;
#endif
      for (; middle->name != NULL; middle += 1)
        {
#ifdef SVN_IO_TEST_ALL_PERMUTATIONS
          inner = test_file_definitions;
#else
          inner = middle;
#endif
          for (; inner->name != NULL; inner += 1)
            {
              svn_pool_clear(iterpool);

              expected12 = inner->size != middle->size;
              expected23 = middle->size != outer->size;
              expected13 = inner->size != outer->size;

              cmp_err = svn_io_filesizes_three_different_p(&actual12,
                                &actual23,
                                &actual13,
                                inner->created_path,
                                middle->created_path,
                                outer->created_path,
                                iterpool);

              if (cmp_err)
                {
                  err = svn_error_compose_create(err, cmp_err);
                }
              else
                {
                  if (expected12 != actual12)
                      err = svn_error_compose_create(err,
                          svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                                     "size comparison problem: '%s' and '%s'",
                                     inner->created_path,
                                     middle->created_path));

                  if (expected23 != actual23)
                      err = svn_error_compose_create(err,
                          svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                                     "size comparison problem: '%s' and '%s'",
                                     middle->created_path,
                                     outer->created_path));

                  if (expected13 != actual13)
                      err = svn_error_compose_create(err,
                          svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                                     "size comparison problem: '%s' and '%s'",
                                     inner->created_path,
                                     outer->created_path));
                }
            }
        }
    }

  svn_pool_destroy(iterpool);

  return err;
}


/* Test 3-way file content checking */
static svn_error_t *
test_three_file_content_comparison(apr_pool_t *scratch_pool)
{
  struct test_file_definition_t *inner, *middle, *outer;
  svn_boolean_t actual12, actual23, actual13;
  svn_boolean_t expected12, expected23, expected13;
  svn_error_t *err = SVN_NO_ERROR;
  svn_error_t *cmp_err;
  apr_pool_t *iterpool = svn_pool_create(scratch_pool);
  struct test_file_definition_t *test_file_definitions;

  SVN_ERR(create_comparison_candidates(&test_file_definitions,
                                       "test_three_file_content_comparison",
                                       scratch_pool));

  for (outer = test_file_definitions; outer->name != NULL; outer += 1)
    {
#ifdef SVN_IO_TEST_ALL_PERMUTATIONS
      middle = test_file_definitions;
#else
      middle = outer;
#endif
      for (; middle->name != NULL; middle += 1)
        {
#ifdef SVN_IO_TEST_ALL_PERMUTATIONS
          inner = test_file_definitions;
#else
          inner = middle;
#endif
          for (; inner->name != NULL; inner += 1)
            {
              svn_pool_clear(iterpool);

              expected12 = outer->size == middle->size
                && strcmp(outer->data, middle->data) == 0;
              expected23 = middle->size == inner->size
                && strcmp(middle->data, inner->data) == 0;
              expected13 = outer->size == inner->size
                && strcmp(outer->data, inner->data) == 0;

              cmp_err = svn_io_files_contents_three_same_p(&actual12,
                                &actual23,
                                &actual13,
                                outer->created_path,
                                middle->created_path,
                                inner->created_path,
                                iterpool);

              if (cmp_err)
                {
                  err = svn_error_compose_create(err, cmp_err);
                }
              else
                {
                  if (expected12 != actual12)
                      err = svn_error_compose_create(err,
                          svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                                     "size comparison problem: '%s' and '%s'",
                                     inner->created_path,
                                     middle->created_path));

                  if (expected23 != actual23)
                      err = svn_error_compose_create(err,
                          svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                                     "size comparison problem: '%s' and '%s'",
                                     middle->created_path,
                                     outer->created_path));

                  if (expected13 != actual13)
                      err = svn_error_compose_create(err,
                          svn_error_createf(SVN_ERR_TEST_FAILED, NULL,
                                     "size comparison problem: '%s' and '%s'",
                                     inner->created_path,
                                     outer->created_path));
                }
            }
        }
    }

  return err;
}

static svn_error_t *
read_length_line_shouldnt_loop(apr_pool_t *pool)
{
  const char *tmp_dir;
  const char *tmp_file;
  char buffer[4];
  apr_size_t buffer_limit = sizeof(buffer);
  apr_file_t *f;

  SVN_ERR(svn_dirent_get_absolute(&tmp_dir, "read_length_tmp", pool));
  SVN_ERR(svn_io_remove_dir2(tmp_dir, TRUE, NULL, NULL, pool));
  SVN_ERR(svn_io_make_dir_recursively(tmp_dir, pool));
  svn_test_add_dir_cleanup(tmp_dir);

  SVN_ERR(svn_io_write_unique(&tmp_file, tmp_dir, "1234\r\n", 6,
                              svn_io_file_del_on_pool_cleanup, pool));

  SVN_ERR(svn_io_file_open(&f, tmp_file, APR_READ, APR_OS_DEFAULT, pool));

  SVN_TEST_ASSERT_ERROR(svn_io_read_length_line(f, buffer, &buffer_limit,
                                                pool), SVN_ERR_MALFORMED_FILE);
  SVN_TEST_ASSERT(buffer_limit == 4);

  return SVN_NO_ERROR;
}

/* Move the read pointer in FILE to absolute position OFFSET and align
 * the read buffer to multiples of BLOCK_SIZE.  BUFFERED is set only if
 * FILE actually uses a read buffer.  Use POOL for allocations.
 */
static svn_error_t *
aligned_seek(apr_file_t *file,
             apr_size_t block_size,
             apr_size_t offset,
             svn_boolean_t buffered,
             apr_pool_t *pool)
{
  apr_off_t block_start;
  apr_off_t current;

  SVN_ERR(svn_io_file_aligned_seek(file, (apr_off_t)block_size,
                                   &block_start, (apr_off_t)offset, pool));

  /* block start shall be aligned to multiples of block_size.
     If it isn't, it must be aligned to APR's default block size(pre-1.3 APR)
   */
  if (buffered)
    {
      SVN_TEST_ASSERT(block_start % block_size == 0);
      SVN_TEST_ASSERT(offset - block_start < block_size);
    }

  /* we must be at the desired offset */
  current = 0;
  SVN_ERR(svn_io_file_seek(file, APR_CUR, &current, pool));
  SVN_TEST_ASSERT(current == (apr_off_t)offset);

  return SVN_NO_ERROR;
}

/* Move the read pointer in FILE to absolute position OFFSET, align the
 * read buffer to multiples of BLOCK_SIZE and read one byte from that
 * position.  Verify that it matches the CONTENTS for that offset.
 * BUFFERED is set only if FILE actually uses a read buffer.
 * Use POOL for allocations.
 */
static svn_error_t *
aligned_read_at(apr_file_t *file,
                svn_stringbuf_t *contents,
                apr_size_t block_size,
                apr_size_t offset,
                svn_boolean_t buffered,
                apr_pool_t *pool)
{
  char c;
  SVN_ERR(aligned_seek(file, block_size, offset, buffered, pool));

  /* the data we read must match whatever we wrote there */
  SVN_ERR(svn_io_file_getc(&c, file, pool));
  SVN_TEST_ASSERT(c == contents->data[offset]);

  return SVN_NO_ERROR;
}

/* Verify that aligned seek with the given BLOCK_SIZE works for FILE.
 * CONTENTS is the data expected from FILE.  BUFFERED is set only if FILE
 * actually uses a read buffer.  Use POOL for allocations.
 */
static svn_error_t *
aligned_read(apr_file_t *file,
             svn_stringbuf_t *contents,
             apr_size_t block_size,
             svn_boolean_t buffered,
             apr_pool_t *pool)
{
  apr_size_t i;
  apr_size_t offset = 0;
  const apr_size_t prime = 78427;

  /* "random" access to different offsets */
  for (i = 0, offset = prime; i < 10; ++i, offset += prime)
    SVN_ERR(aligned_read_at(file, contents, block_size,
                            offset % contents->len, buffered, pool));

  /* we can seek to EOF */
  SVN_ERR(aligned_seek(file, contents->len, block_size, buffered, pool));

  /* reversed order access to all bytes */
  for (i = contents->len; i > 0; --i)
    SVN_ERR(aligned_read_at(file, contents, block_size, i - 1, buffered,
                            pool));

  /* forward order access to all bytes */
  for (i = 0; i < contents->len; ++i)
    SVN_ERR(aligned_read_at(file, contents, block_size, i, buffered, pool));

  return SVN_NO_ERROR;
}

static svn_error_t *
aligned_seek_test(apr_pool_t *pool)
{
  apr_size_t i;
  const char *tmp_dir;
  const char *tmp_file;
  apr_file_t *f;
  svn_stringbuf_t *contents;
  const apr_size_t file_size = 100000;

  /* create a temp folder & schedule it for automatic cleanup */

  SVN_ERR(svn_dirent_get_absolute(&tmp_dir, "aligned_seek_tmp", pool));
  SVN_ERR(svn_io_remove_dir2(tmp_dir, TRUE, NULL, NULL, pool));
  SVN_ERR(svn_io_make_dir_recursively(tmp_dir, pool));
  svn_test_add_dir_cleanup(tmp_dir);

  /* create a temp file with know contents */

  contents = svn_stringbuf_create_ensure(file_size, pool);
  for (i = 0; i < file_size; ++i)
    svn_stringbuf_appendbyte(contents, (char)rand());

  SVN_ERR(svn_io_write_unique(&tmp_file, tmp_dir, contents->data,
                              contents->len,
                              svn_io_file_del_on_pool_cleanup, pool));

  /* now, access read data with varying alignment sizes */
  SVN_ERR(svn_io_file_open(&f, tmp_file, APR_READ | APR_BUFFERED,
                           APR_OS_DEFAULT, pool));
  SVN_ERR(aligned_read(f, contents,   0x1000, TRUE, pool)); /* APR default */
  SVN_ERR(aligned_read(f, contents,   0x8000, TRUE, pool)); /* "unusual" 32K */
  SVN_ERR(aligned_read(f, contents,  0x10000, TRUE, pool)); /* FSX default */
  SVN_ERR(aligned_read(f, contents, 0x100000, TRUE, pool)); /* larger than file */
  SVN_ERR(aligned_read(f, contents,    10001, TRUE, pool)); /* odd, larger than
                                                               APR default */
  SVN_ERR(aligned_read(f, contents,     1003, TRUE, pool)); /* odd, smaller than
                                                               APR default */
  SVN_ERR(svn_io_file_close(f, pool));

  /* now, try read data with buffering disabled.
     That are a special case because APR reports a buffer size of 0. */
  SVN_ERR(svn_io_file_open(&f, tmp_file, APR_READ, APR_OS_DEFAULT, pool));
  SVN_ERR(aligned_read(f, contents,   0x1000, FALSE, pool));
  SVN_ERR(aligned_read(f, contents,   0x8000, FALSE, pool));
  SVN_ERR(aligned_read(f, contents,  0x10000, FALSE, pool));
  SVN_ERR(aligned_read(f, contents, 0x100000, FALSE, pool));
  SVN_ERR(aligned_read(f, contents,    10001, FALSE, pool));
  SVN_ERR(aligned_read(f, contents,     1003, FALSE, pool));
  SVN_ERR(svn_io_file_close(f, pool));

  return SVN_NO_ERROR;
}

static svn_error_t *
ignore_enoent(apr_pool_t *pool)
{
  const char *tmp_dir, *path;
  const svn_io_dirent2_t *dirent_p;
  apr_file_t *file;

  /* Create an empty directory. */
  SVN_ERR(svn_dirent_get_absolute(&tmp_dir, "ignore_enoent", pool));
  SVN_ERR(svn_io_remove_dir2(tmp_dir, TRUE, NULL, NULL, pool));
  SVN_ERR(svn_io_make_dir_recursively(tmp_dir, pool));
  svn_test_add_dir_cleanup(tmp_dir);

  /* Path does not exist. */
  path = svn_dirent_join(tmp_dir, "not-present", pool);
  SVN_ERR(svn_io_remove_dir2(path, TRUE, NULL, NULL, pool));
  SVN_ERR(svn_io_remove_file2(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_read_only(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_read_write(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_executable(path, TRUE, TRUE, pool));
  SVN_ERR(svn_io_set_file_executable(path, FALSE, TRUE, pool));
  SVN_ERR(svn_io_stat_dirent2(&dirent_p, path, TRUE, TRUE, pool, pool));
  SVN_ERR(svn_io_stat_dirent2(&dirent_p, path, FALSE, TRUE, pool, pool));

  /* Neither path nor parent exists. */
  path = svn_dirent_join(path, "not-present", pool);
  SVN_ERR(svn_io_remove_dir2(path, TRUE, NULL, NULL, pool));
  SVN_ERR(svn_io_remove_file2(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_read_only(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_read_write(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_executable(path, TRUE, TRUE, pool));
  SVN_ERR(svn_io_set_file_executable(path, FALSE, TRUE, pool));
  SVN_ERR(svn_io_stat_dirent2(&dirent_p, path, TRUE, TRUE, pool, pool));
  SVN_ERR(svn_io_stat_dirent2(&dirent_p, path, FALSE, TRUE, pool, pool));

  /* File does exist. */
  path = svn_dirent_join(tmp_dir, "present", pool);
  SVN_ERR(svn_io_file_open(&file, path,
                           APR_WRITE | APR_CREATE | APR_TRUNCATE,
                           APR_OS_DEFAULT,
                           pool));
  SVN_ERR(svn_io_file_close(file, pool));

  /* Path does not exist as child of file. */
  path = svn_dirent_join(path, "not-present", pool);
  SVN_ERR(svn_io_remove_dir2(path, TRUE, NULL, NULL, pool));
  SVN_ERR(svn_io_remove_file2(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_read_only(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_read_write(path, TRUE, pool));
  SVN_ERR(svn_io_set_file_executable(path, TRUE, TRUE, pool));
  SVN_ERR(svn_io_set_file_executable(path, FALSE, TRUE, pool));
  SVN_ERR(svn_io_stat_dirent2(&dirent_p, path, TRUE, TRUE, pool, pool));
  SVN_ERR(svn_io_stat_dirent2(&dirent_p, path, FALSE, TRUE, pool, pool));

  return SVN_NO_ERROR;
}


/* The test table.  */

static int max_threads = 3;

static struct svn_test_descriptor_t test_funcs[] =
  {
    SVN_TEST_NULL,
    SVN_TEST_PASS2(test_two_file_size_comparison,
                   "two file size comparison"),
    SVN_TEST_PASS2(test_two_file_content_comparison,
                   "two file content comparison"),
    SVN_TEST_PASS2(test_three_file_size_comparison,
                   "three file size comparison"),
    SVN_TEST_PASS2(test_three_file_content_comparison,
                   "three file content comparison"),
    SVN_TEST_PASS2(read_length_line_shouldnt_loop,
                   "svn_io_read_length_line() shouldn't loop"),
    SVN_TEST_PASS2(aligned_seek_test,
                   "test aligned seek"),
    SVN_TEST_PASS2(ignore_enoent,
                   "test ignore-enoent"),
    SVN_TEST_NULL
  };

SVN_TEST_MAIN
