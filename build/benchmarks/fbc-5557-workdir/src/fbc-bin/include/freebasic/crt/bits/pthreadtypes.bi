#include "crt/long.bi"

#IFNDEF _BITS_PTHREADTYPES_H
#DEFINE _BITS_PTHREADTYPES_H 1
#DEFINE __SIZEOF_PTHREAD_ATTR_T 36
#DEFINE __SIZEOF_PTHREAD_MUTEX_T 24
#DEFINE __SIZEOF_PTHREAD_MUTEXATTR_T 4
#DEFINE __SIZEOF_PTHREAD_COND_T 48
#DEFINE __SIZEOF_PTHREAD_COND_COMPAT_T 12
#DEFINE __SIZEOF_PTHREAD_CONDATTR_T 4
#DEFINE __SIZEOF_PTHREAD_RWLOCK_T 32
#DEFINE __SIZEOF_PTHREAD_RWLOCKATTR_T 8
#DEFINE __SIZEOF_PTHREAD_BARRIER_T 20
#DEFINE __SIZEOF_PTHREAD_BARRIERATTR_T 4

TYPE pthread_t AS CULONG

UNION pthread_attr_t
  AS ZSTRING*__SIZEOF_PTHREAD_ATTR_T __size
  AS CLONG __align
END UNION

TYPE __pthread_internal_slist
  AS __pthread_internal_slist PTR __next
END TYPE

TYPE AS __pthread_internal_slist __pthread_slist_t

TYPE pthread_mutex_t___pthread_mutex_s
  AS LONG __lock
  AS ULONG __count
  AS LONG __owner
  AS LONG __kind
  AS ULONG __nusers
END TYPE

UNION pthread_mutex_t
  AS pthread_mutex_t___pthread_mutex_s __data
  AS ZSTRING*__SIZEOF_PTHREAD_MUTEX_T __size
  AS CLONG __align
END UNION

UNION pthread_mutexattr_t
  AS ZSTRING*__SIZEOF_PTHREAD_MUTEXATTR_T __size
  AS LONG __align
END UNION

TYPE pthread_cond_t___data
  AS LONG __lock
  AS ULONG __futex
  AS ULONGINT __total_seq
  AS ULONGINT __wakeup_seq
  AS ULONGINT __woken_seq
  AS ANY PTR __mutex
  AS ULONG __nwaiters
  AS ULONG __broadcast_seq
END TYPE

UNION pthread_cond_t
  AS pthread_cond_t___data __data
  AS ZSTRING*__SIZEOF_PTHREAD_COND_T __size
  AS LONGINT __align
END UNION

UNION pthread_condattr_t
  AS ZSTRING*__SIZEOF_PTHREAD_CONDATTR_T __size
  AS LONG __align
END UNION

TYPE pthread_key_t AS ULONG
TYPE pthread_once_t AS LONG

#IF DEFINED (__USE_UNIX98) OR DEFINED (__USE_XOPEN2K)

TYPE pthread_rwlock_t___data
  AS LONG __lock
  AS ULONG __nr_readers
  AS ULONG __readers_wakeup
  AS ULONG __writer_wakeup
  AS ULONG __nr_readers_queued
  AS ULONG __nr_writers_queued
  AS UBYTE __flags
  AS UBYTE __shared
  AS UBYTE __pad1
  AS UBYTE __pad2
  AS LONG __writer
END TYPE

UNION pthread_rwlock_t
  AS pthread_rwlock_t___data __data
  AS ZSTRING*__SIZEOF_PTHREAD_RWLOCK_T __size
  AS CLONG __align
END UNION

UNION pthread_rwlockattr_t
  AS ZSTRING*__SIZEOF_PTHREAD_RWLOCKATTR_T __size
  AS CLONG __align
END UNION

#ENDIF ' DEFINED __USE_U...

#IFDEF __USE_XOPEN2K

TYPE pthread_spinlock_t AS LONG

UNION pthread_barrier_t
  AS ZSTRING*__SIZEOF_PTHREAD_BARRIER_T __size
  AS CLONG __align
END UNION

UNION pthread_barrierattr_t
  AS ZSTRING*__SIZEOF_PTHREAD_BARRIERATTR_T __size
  AS LONG __align
END UNION

#ENDIF ' __USE_XOPEN2K

#DEFINE __cleanup_fct_attribute __attribute__ ((__regparm__ (1)))
#ENDIF ' _BITS_PTHREADTYPES_H
