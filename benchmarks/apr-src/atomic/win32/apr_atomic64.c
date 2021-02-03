/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "apr.h"
#include "apr_atomic.h"
#include "apr_thread_mutex.h"

/* Of course we want the 2's compliment of the unsigned value, val */
#ifdef _MSC_VER
#pragma warning(disable: 4146)
#endif

APR_DECLARE(apr_uint64_t) apr_atomic_add64(volatile apr_uint64_t *mem, apr_uint64_t val)
{
    return InterlockedExchangeAdd64((volatile LONG64 *)mem, val);
}

APR_DECLARE(void) apr_atomic_sub64(volatile apr_uint64_t *mem, apr_uint64_t val)
{
    InterlockedExchangeAdd64((volatile LONG64 *)mem, -val);
}

APR_DECLARE(apr_uint64_t) apr_atomic_inc64(volatile apr_uint64_t *mem)
{
    /* we return old value, win64 returns new value :( */
    return InterlockedIncrement64((volatile LONG64 *)mem) - 1;
}

APR_DECLARE(int) apr_atomic_dec64(volatile apr_uint64_t *mem)
{
    return !!InterlockedDecrement64((volatile LONG64 *)mem);
}

APR_DECLARE(void) apr_atomic_set64(volatile apr_uint64_t *mem, apr_uint64_t val)
{
    InterlockedExchange64((volatile LONG64 *)mem, val);
}

APR_DECLARE(apr_uint64_t) apr_atomic_read64(volatile apr_uint64_t *mem)
{
#if defined(_M_X64)
    /* https://docs.microsoft.com/en-us/windows/win32/sync/interlocked-variable-access
     * "Simple reads and writes to properly aligned 64-bit variables are atomic
     * on 64-bit Windows."*/
    return *mem;
#else
    /* 64-bit read is not atomic on 32-bit platform: use InterlockedCompareExchange
       to perform atomic read. */
    return InterlockedCompareExchange64((volatile LONG64 *)mem, 0, 0);
#endif
}

APR_DECLARE(apr_uint64_t) apr_atomic_cas64(volatile apr_uint64_t *mem, apr_uint64_t with,
                                           apr_uint64_t cmp)
{
    return InterlockedCompareExchange64((volatile LONG64 *)mem, with, cmp);
}

APR_DECLARE(apr_uint64_t) apr_atomic_xchg64(volatile apr_uint64_t *mem, apr_uint64_t val)
{
    return InterlockedExchange64((volatile LONG64 *)mem, val);
}
