int __get_mutant();
int __is_neg(const char *, int, const int *, int, const char *, int,
             const void **, int, const double *, int, ...);
int __abst_hole();
int __choose(char *);
void *memset(void *, int, unsigned long);
/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2011 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Andi Gutmans <andi@zend.com>                                |
   |          Zeev Suraski <zeev@zend.com>                                |
   +----------------------------------------------------------------------+
 */

/* $Id$ */

#include "ext/session/php_session.h"
#include "ext/standard/info.h"
#include "ext/standard/php_dns.h"
#include "ext/standard/php_uuencode.h"
#include "php.h"
#include "php_getopt.h"
#include "php_globals.h"
#include "php_http.h"
#include "php_incomplete_class.h"
#include "php_ini.h"
#include "php_main.h"
#include "php_math.h"
#include "php_standard.h"
#include "php_streams.h"
#include "zend_operators.h"

#ifdef PHP_WIN32
#include "win32/php_win32_globals.h"
#include "win32/time.h"
#endif

typedef struct yy_buffer_state *YY_BUFFER_STATE;

#include "zend.h"
#include "zend_ini_scanner.h"
#include "zend_language_scanner.h"
#include <zend_language_parser.h>

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef PHP_WIN32
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef NETWARE
#include <netinet/in.h>
#endif

#ifndef PHP_WIN32
#include <netdb.h>
#else
#include "win32/inet.h"
#endif

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#if HAVE_LOCALE_H
#include <locale.h>
#endif

#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#if HAVE_SYS_LOADAVG_H
#include <sys/loadavg.h>
#endif

#ifdef PHP_WIN32
#include "win32/unistd.h"
#endif

#ifndef INADDR_NONE
#define INADDR_NONE ((unsigned long int)-1)
#endif

#include "SAPI.h"
#include "php_globals.h"
#include "php_ticks.h"
#include "zend_globals.h"

#ifdef ZTS
PHPAPI int basic_globals_id;
#else
PHPAPI php_basic_globals basic_globals;
#endif

#include "php_fopen_wrappers.h"
#include "streamsfuncs.h"

static zend_class_entry *incomplete_class_entry = NULL;

typedef struct _php_shutdown_function_entry {
  zval **arguments;
  int arg_count;
} php_shutdown_function_entry;

typedef struct _user_tick_function_entry {
  zval **arguments;
  int arg_count;
  int calling;
} user_tick_function_entry;

/* some prototypes for local functions */
static void user_shutdown_function_dtor(
    php_shutdown_function_entry *shutdown_function_entry);
static void
user_tick_function_dtor(user_tick_function_entry *tick_function_entry);

#undef sprintf

/* {{{ arginfo */
/* {{{ main/main.c */
ZEND_BEGIN_ARG_INFO(arginfo_set_time_limit, 0)
ZEND_ARG_INFO(0, seconds)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ main/sapi.c */
ZEND_BEGIN_ARG_INFO(arginfo_header_register_callback, 0)
ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ main/output.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_ob_start, 0, 0, 0)
ZEND_ARG_INFO(0, user_function)
ZEND_ARG_INFO(0, chunk_size)
ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_flush, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_clean, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_end_flush, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_end_clean, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_get_flush, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_get_clean, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_get_contents, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_get_level, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_get_length, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ob_list_handlers, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ob_get_status, 0, 0, 0)
ZEND_ARG_INFO(0, full_status)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ob_implicit_flush, 0, 0, 0)
ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_output_reset_rewrite_vars, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_output_add_rewrite_var, 0)
ZEND_ARG_INFO(0, name)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ main/streams/userspace.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_wrapper_register, 0, 0, 2)
ZEND_ARG_INFO(0, protocol)
ZEND_ARG_INFO(0, classname)
ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_wrapper_unregister, 0)
ZEND_ARG_INFO(0, protocol)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_wrapper_restore, 0)
ZEND_ARG_INFO(0, protocol)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ array.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_krsort, 0, 0, 1)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, sort_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ksort, 0, 0, 1)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, sort_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_count, 0, 0, 1)
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_natsort, 0)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_natcasesort, 0)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_asort, 0, 0, 1)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, sort_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_arsort, 0, 0, 1)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, sort_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sort, 0, 0, 1)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, sort_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_rsort, 0, 0, 1)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, sort_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_usort, 0)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, cmp_function)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_uasort, 0)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, cmp_function)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_uksort, 0)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, cmp_function)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_end, 0)
ZEND_ARG_INFO(1, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_prev, 0)
ZEND_ARG_INFO(1, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_next, 0)
ZEND_ARG_INFO(1, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_reset, 0)
ZEND_ARG_INFO(1, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_current, ZEND_SEND_PREFER_REF)
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_key, ZEND_SEND_PREFER_REF)
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_min, 0, 0, 1)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, arg2)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_max, 0, 0, 1)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, arg2)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_walk, 0, 0, 2)
ZEND_ARG_INFO(1, input) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, funcname)
ZEND_ARG_INFO(0, userdata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_walk_recursive, 0, 0, 2)
ZEND_ARG_INFO(1, input) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, funcname)
ZEND_ARG_INFO(0, userdata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_in_array, 0, 0, 2)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, haystack) /* ARRAY_INFO(0, haystack, 0) */
ZEND_ARG_INFO(0, strict)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_search, 0, 0, 2)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, haystack) /* ARRAY_INFO(0, haystack, 0) */
ZEND_ARG_INFO(0, strict)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_extract, 0, 0, 1)
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, extract_type)
ZEND_ARG_INFO(0, prefix)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_compact, 0, 0, 1)
ZEND_ARG_INFO(0, var_names)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_fill, 0)
ZEND_ARG_INFO(0, start_key)
ZEND_ARG_INFO(0, num)
ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_fill_keys, 0)
ZEND_ARG_INFO(0, keys) /* ARRAY_INFO(0, keys, 0) */
ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_range, 0, 0, 2)
ZEND_ARG_INFO(0, low)
ZEND_ARG_INFO(0, high)
ZEND_ARG_INFO(0, step)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_shuffle, 0)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_push, 0, 0, 2)
ZEND_ARG_INFO(1, stack) /* ARRAY_INFO(1, stack, 0) */
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_pop, 0)
ZEND_ARG_INFO(1, stack) /* ARRAY_INFO(1, stack, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_shift, 0)
ZEND_ARG_INFO(1, stack) /* ARRAY_INFO(1, stack, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_unshift, 0, 0, 2)
ZEND_ARG_INFO(1, stack) /* ARRAY_INFO(1, stack, 0) */
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_splice, 0, 0, 2)
ZEND_ARG_INFO(1, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, offset)
ZEND_ARG_INFO(0, length)
ZEND_ARG_INFO(0, replacement) /* ARRAY_INFO(0, arg, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_slice, 0, 0, 2)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(1, arg, 0) */
ZEND_ARG_INFO(0, offset)
ZEND_ARG_INFO(0, length)
ZEND_ARG_INFO(0, preserve_keys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_merge, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, ..., 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_merge_recursive, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_replace, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, ..., 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_replace_recursive, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_keys, 0, 0, 1)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, search_value)
ZEND_ARG_INFO(0, strict)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_values, 0)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_count_values, 0)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_reverse, 0, 0, 1)
ZEND_ARG_INFO(0, input) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, preserve_keys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_pad, 0)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, pad_size)
ZEND_ARG_INFO(0, pad_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_flip, 0)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_change_key_case, 0, 0, 1)
ZEND_ARG_INFO(0, input) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, case)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_unique, 0)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_intersect_key, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, ..., 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_intersect_ukey, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_key_compare_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_intersect, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, ..., 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_uintersect, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_data_compare_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_intersect_assoc, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, ..., 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_uintersect_assoc, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_data_compare_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_intersect_uassoc, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_key_compare_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_uintersect_uassoc, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_data_compare_func)
ZEND_ARG_INFO(0, callback_key_compare_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_diff_key, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, ..., 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_diff_ukey, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_key_comp_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_diff, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, ..., 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_udiff, 0)
ZEND_ARG_INFO(0, arr1)
ZEND_ARG_INFO(0, arr2)
ZEND_ARG_INFO(0, callback_data_comp_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_diff_assoc, 0, 0, 2)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, ...)  /* ARRAY_INFO(0, ..., 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_diff_uassoc, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_data_comp_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_udiff_assoc, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_key_comp_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_udiff_uassoc, 0)
ZEND_ARG_INFO(0, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(0, arr2) /* ARRAY_INFO(0, arg2, 0) */
ZEND_ARG_INFO(0, callback_data_comp_func)
ZEND_ARG_INFO(0, callback_key_comp_func)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_multisort, ZEND_SEND_PREFER_REF, 0, 1)
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, arr1) /* ARRAY_INFO(0, arg1, 0) */
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, SORT_ASC_or_SORT_DESC)
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, SORT_REGULAR_or_SORT_NUMERIC_or_SORT_STRING)
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, arr2)
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, SORT_ASC_or_SORT_DESC)
ZEND_ARG_INFO(ZEND_SEND_PREFER_REF, SORT_REGULAR_or_SORT_NUMERIC_or_SORT_STRING)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_rand, 0, 0, 1)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, num_req)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_sum, 0)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_product, 0)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_reduce, 0, 0, 2)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, callback)
ZEND_ARG_INFO(0, initial)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_filter, 0, 0, 1)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_map, 0, 0, 2)
ZEND_ARG_INFO(0, callback)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_key_exists, 0)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(0, search)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_chunk, 0, 0, 2)
ZEND_ARG_INFO(0, arg) /* ARRAY_INFO(0, arg, 0) */
ZEND_ARG_INFO(0, size)
ZEND_ARG_INFO(0, preserve_keys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_array_combine, 0)
ZEND_ARG_INFO(0, keys)   /* ARRAY_INFO(0, keys, 0) */
ZEND_ARG_INFO(0, values) /* ARRAY_INFO(0, values, 0) */
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ basic_functions.c */
ZEND_BEGIN_ARG_INFO(arginfo_get_magic_quotes_gpc, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_magic_quotes_runtime, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_set_magic_quotes_runtime, 0, 0, 1)
ZEND_ARG_INFO(0, new_setting)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_constant, 0)
ZEND_ARG_INFO(0, const_name)
ZEND_END_ARG_INFO()

#ifdef HAVE_INET_NTOP
ZEND_BEGIN_ARG_INFO(arginfo_inet_ntop, 0)
ZEND_ARG_INFO(0, in_addr)
ZEND_END_ARG_INFO()
#endif

#ifdef HAVE_INET_PTON
ZEND_BEGIN_ARG_INFO(arginfo_inet_pton, 0)
ZEND_ARG_INFO(0, ip_address)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO(arginfo_ip2long, 0)
ZEND_ARG_INFO(0, ip_address)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_long2ip, 0)
ZEND_ARG_INFO(0, proper_address)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_getenv, 0)
ZEND_ARG_INFO(0, varname)
ZEND_END_ARG_INFO()

#ifdef HAVE_PUTENV
ZEND_BEGIN_ARG_INFO(arginfo_putenv, 0)
ZEND_ARG_INFO(0, setting)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_getopt, 0, 0, 1)
ZEND_ARG_INFO(0, options)
ZEND_ARG_INFO(0, opts) /* ARRAY_INFO(0, opts, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_flush, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_sleep, 0)
ZEND_ARG_INFO(0, seconds)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_usleep, 0)
ZEND_ARG_INFO(0, micro_seconds)
ZEND_END_ARG_INFO()

#if HAVE_NANOSLEEP
ZEND_BEGIN_ARG_INFO(arginfo_time_nanosleep, 0)
ZEND_ARG_INFO(0, seconds)
ZEND_ARG_INFO(0, nanoseconds)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_time_sleep_until, 0)
ZEND_ARG_INFO(0, timestamp)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO(arginfo_get_current_user, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_cfg_var, 0)
ZEND_ARG_INFO(0, option_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_error_log, 0, 0, 1)
ZEND_ARG_INFO(0, message)
ZEND_ARG_INFO(0, message_type)
ZEND_ARG_INFO(0, destination)
ZEND_ARG_INFO(0, extra_headers)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_error_get_last, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_call_user_func, 0, 0, 1)
ZEND_ARG_INFO(0, function_name)
ZEND_ARG_INFO(0, parmeter)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_call_user_func_array, 0, 0, 2)
ZEND_ARG_INFO(0, function_name)
ZEND_ARG_INFO(0, parameters) /* ARRAY_INFO(0, parameters, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_call_user_method, 0, 0, 2)
ZEND_ARG_INFO(0, method_name)
ZEND_ARG_INFO(1, object)
ZEND_ARG_INFO(0, parameter)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_call_user_method_array, 0)
ZEND_ARG_INFO(0, method_name)
ZEND_ARG_INFO(1, object)
ZEND_ARG_INFO(0, params) /* ARRAY_INFO(0, params, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_forward_static_call, 0, 0, 1)
ZEND_ARG_INFO(0, function_name)
ZEND_ARG_INFO(0, parameter)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_forward_static_call_array, 0, 0, 2)
ZEND_ARG_INFO(0, function_name)
ZEND_ARG_INFO(0, parameters) /* ARRAY_INFO(0, parameters, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_register_shutdown_function, 0)
ZEND_ARG_INFO(0, function_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_highlight_file, 0, 0, 1)
ZEND_ARG_INFO(0, file_name)
ZEND_ARG_INFO(0, return )
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_strip_whitespace, 0)
ZEND_ARG_INFO(0, file_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_highlight_string, 0, 0, 1)
ZEND_ARG_INFO(0, string)
ZEND_ARG_INFO(0, return )
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ini_get, 0)
ZEND_ARG_INFO(0, varname)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ini_get_all, 0, 0, 0)
ZEND_ARG_INFO(0, extension)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ini_set, 0)
ZEND_ARG_INFO(0, varname)
ZEND_ARG_INFO(0, newvalue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ini_restore, 0)
ZEND_ARG_INFO(0, varname)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_set_include_path, 0)
ZEND_ARG_INFO(0, new_include_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_get_include_path, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_restore_include_path, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_print_r, 0, 0, 1)
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, return )
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_connection_aborted, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_connection_status, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ignore_user_abort, 0, 0, 0)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

#if HAVE_GETSERVBYNAME
ZEND_BEGIN_ARG_INFO(arginfo_getservbyname, 0)
ZEND_ARG_INFO(0, service)
ZEND_ARG_INFO(0, protocol)
ZEND_END_ARG_INFO()
#endif

#if HAVE_GETSERVBYPORT
ZEND_BEGIN_ARG_INFO(arginfo_getservbyport, 0)
ZEND_ARG_INFO(0, port)
ZEND_ARG_INFO(0, protocol)
ZEND_END_ARG_INFO()
#endif

#if HAVE_GETPROTOBYNAME
ZEND_BEGIN_ARG_INFO(arginfo_getprotobyname, 0)
ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()
#endif

#if HAVE_GETPROTOBYNUMBER
ZEND_BEGIN_ARG_INFO(arginfo_getprotobynumber, 0)
ZEND_ARG_INFO(0, proto)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_register_tick_function, 0, 0, 1)
ZEND_ARG_INFO(0, function_name)
ZEND_ARG_INFO(0, arg)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_unregister_tick_function, 0)
ZEND_ARG_INFO(0, function_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_uploaded_file, 0)
ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_move_uploaded_file, 0)
ZEND_ARG_INFO(0, path)
ZEND_ARG_INFO(0, new_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parse_ini_file, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, process_sections)
ZEND_ARG_INFO(0, scanner_mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parse_ini_string, 0, 0, 1)
ZEND_ARG_INFO(0, ini_string)
ZEND_ARG_INFO(0, process_sections)
ZEND_ARG_INFO(0, scanner_mode)
ZEND_END_ARG_INFO()

#if ZEND_DEBUG
ZEND_BEGIN_ARG_INFO(arginfo_config_get_hash, 0)
ZEND_END_ARG_INFO()
#endif

#ifdef HAVE_GETLOADAVG
ZEND_BEGIN_ARG_INFO(arginfo_sys_getloadavg, 0)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ assert.c */
ZEND_BEGIN_ARG_INFO(arginfo_assert, 0)
ZEND_ARG_INFO(0, assertion)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_assert_options, 0, 0, 1)
ZEND_ARG_INFO(0, what)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ base64.c */
ZEND_BEGIN_ARG_INFO(arginfo_base64_encode, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_base64_decode, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, strict)
ZEND_END_ARG_INFO()

/* }}} */
/* {{{ browscap.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_get_browser, 0, 0, 0)
ZEND_ARG_INFO(0, browser_name)
ZEND_ARG_INFO(0, return_array)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ crc32.c */
ZEND_BEGIN_ARG_INFO(arginfo_crc32, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

/* }}} */
/* {{{ crypt.c */
#if HAVE_CRYPT
ZEND_BEGIN_ARG_INFO_EX(arginfo_crypt, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, salt)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ cyr_convert.c */
ZEND_BEGIN_ARG_INFO(arginfo_convert_cyr_string, 0)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, from)
ZEND_ARG_INFO(0, to)
ZEND_END_ARG_INFO()

/* }}} */
/* {{{ datetime.c */
#if HAVE_STRPTIME
ZEND_BEGIN_ARG_INFO(arginfo_strptime, 0)
ZEND_ARG_INFO(0, timestamp)
ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ dir.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_opendir, 0, 0, 1)
ZEND_ARG_INFO(0, path)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dir, 0, 0, 1)
ZEND_ARG_INFO(0, directory)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_closedir, 0, 0, 0)
ZEND_ARG_INFO(0, dir_handle)
ZEND_END_ARG_INFO()

#if defined(HAVE_CHROOT) && !defined(ZTS) && ENABLE_CHROOT_FUNC
ZEND_BEGIN_ARG_INFO(arginfo_chroot, 0)
ZEND_ARG_INFO(0, directory)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO(arginfo_chdir, 0)
ZEND_ARG_INFO(0, directory)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_getcwd, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_rewinddir, 0, 0, 0)
ZEND_ARG_INFO(0, dir_handle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_readdir, 0, 0, 0)
ZEND_ARG_INFO(0, dir_handle)
ZEND_END_ARG_INFO()

#ifdef HAVE_GLOB
ZEND_BEGIN_ARG_INFO_EX(arginfo_glob, 0, 0, 1)
ZEND_ARG_INFO(0, pattern)
ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_scandir, 0, 0, 1)
ZEND_ARG_INFO(0, dir)
ZEND_ARG_INFO(0, sorting_order)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ arginfo ext/standard/dl.c */
ZEND_BEGIN_ARG_INFO(arginfo_dl, 0)
ZEND_ARG_INFO(0, extension_filename)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ dns.c */
ZEND_BEGIN_ARG_INFO(arginfo_gethostbyaddr, 0)
ZEND_ARG_INFO(0, ip_address)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_gethostbyname, 0)
ZEND_ARG_INFO(0, hostname)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_gethostbynamel, 0)
ZEND_ARG_INFO(0, hostname)
ZEND_END_ARG_INFO()

#ifdef HAVE_GETHOSTNAME
ZEND_BEGIN_ARG_INFO(arginfo_gethostname, 0)
ZEND_END_ARG_INFO()
#endif

#if defined(PHP_WIN32) ||                                                      \
    (HAVE_DNS_SEARCH_FUNC && !(defined(__BEOS__) || defined(NETWARE)))
ZEND_BEGIN_ARG_INFO_EX(arginfo_dns_check_record, 0, 0, 1)
ZEND_ARG_INFO(0, host)
ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

#if defined(PHP_WIN32) || HAVE_FULL_DNS_FUNCS
ZEND_BEGIN_ARG_INFO_EX(arginfo_dns_get_record, 0, 0, 1)
ZEND_ARG_INFO(0, hostname)
ZEND_ARG_INFO(0, type)
ZEND_ARG_ARRAY_INFO(1, authns, 1)
ZEND_ARG_ARRAY_INFO(1, addtl, 1)
ZEND_ARG_INFO(0, raw)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dns_get_mx, 0, 0, 2)
ZEND_ARG_INFO(0, hostname)
ZEND_ARG_INFO(1, mxhosts) /* ARRAY_INFO(1, mxhosts, 1) */
ZEND_ARG_INFO(1, weight)  /* ARRAY_INFO(1, weight, 1) */
ZEND_END_ARG_INFO()
#endif

#endif /* defined(PHP_WIN32) || (HAVE_DNS_SEARCH_FUNC && !(defined(__BEOS__)   \
          || defined(NETWARE))) */
/* }}} */

/* {{{ exec.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_exec, 0, 0, 1)
ZEND_ARG_INFO(0, command)
ZEND_ARG_INFO(1, output) /* ARRAY_INFO(1, output, 1) */
ZEND_ARG_INFO(1, return_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_system, 0, 0, 1)
ZEND_ARG_INFO(0, command)
ZEND_ARG_INFO(1, return_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_passthru, 0, 0, 1)
ZEND_ARG_INFO(0, command)
ZEND_ARG_INFO(1, return_value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_escapeshellcmd, 0)
ZEND_ARG_INFO(0, command)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_escapeshellarg, 0)
ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_shell_exec, 0)
ZEND_ARG_INFO(0, cmd)
ZEND_END_ARG_INFO()

#ifdef HAVE_NICE
ZEND_BEGIN_ARG_INFO(arginfo_proc_nice, 0)
ZEND_ARG_INFO(0, priority)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ file.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_flock, 0, 0, 2)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, operation)
ZEND_ARG_INFO(1, wouldblock)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_get_meta_tags, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, use_include_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_file_get_contents, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, flags)
ZEND_ARG_INFO(0, context)
ZEND_ARG_INFO(0, offset)
ZEND_ARG_INFO(0, maxlen)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_file_put_contents, 0, 0, 2)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, data)
ZEND_ARG_INFO(0, flags)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_file, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, flags)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_tempnam, 0)
ZEND_ARG_INFO(0, dir)
ZEND_ARG_INFO(0, prefix)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_tmpfile, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fopen, 0, 0, 2)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, mode)
ZEND_ARG_INFO(0, use_include_path)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fclose, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_popen, 0)
ZEND_ARG_INFO(0, command)
ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pclose, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_feof, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fgets, 0, 0, 1)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fgetc, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fgetss, 0, 0, 1)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, length)
ZEND_ARG_INFO(0, allowable_tags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fscanf, 1, 0, 2)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(1, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fwrite, 0, 0, 2)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fflush, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_rewind, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ftell, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fseek, 0, 0, 2)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, offset)
ZEND_ARG_INFO(0, whence)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mkdir, 0, 0, 1)
ZEND_ARG_INFO(0, pathname)
ZEND_ARG_INFO(0, mode)
ZEND_ARG_INFO(0, recursive)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_rmdir, 0, 0, 1)
ZEND_ARG_INFO(0, dirname)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_readfile, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, flags)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_umask, 0, 0, 0)
ZEND_ARG_INFO(0, mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fpassthru, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_rename, 0, 0, 2)
ZEND_ARG_INFO(0, old_name)
ZEND_ARG_INFO(0, new_name)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_unlink, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ftruncate, 0)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, size)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fstat, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_copy, 0)
ZEND_ARG_INFO(0, source_file)
ZEND_ARG_INFO(0, destination_file)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fread, 0)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fputcsv, 0, 0, 2)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, fields) /* ARRAY_INFO(0, fields, 1) */
ZEND_ARG_INFO(0, delimiter)
ZEND_ARG_INFO(0, enclosure)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fgetcsv, 0, 0, 1)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, length)
ZEND_ARG_INFO(0, delimiter)
ZEND_ARG_INFO(0, enclosure)
ZEND_ARG_INFO(0, escape)
ZEND_END_ARG_INFO()

#if (!defined(__BEOS__) && !defined(NETWARE) && HAVE_REALPATH) || defined(ZTS)
ZEND_BEGIN_ARG_INFO(arginfo_realpath, 0)
ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()
#endif

#ifdef HAVE_FNMATCH
ZEND_BEGIN_ARG_INFO_EX(arginfo_fnmatch, 0, 0, 2)
ZEND_ARG_INFO(0, pattern)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO(arginfo_sys_get_temp_dir, 0)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ filestat.c */
ZEND_BEGIN_ARG_INFO(arginfo_disk_total_space, 0)
ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_disk_free_space, 0)
ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

#ifndef NETWARE
ZEND_BEGIN_ARG_INFO(arginfo_chgrp, 0)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, group)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_chown, 0)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, user)
ZEND_END_ARG_INFO()
#endif

#if HAVE_LCHOWN
ZEND_BEGIN_ARG_INFO(arginfo_lchgrp, 0)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, group)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_lchown, 0)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, user)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO(arginfo_chmod, 0)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

#if HAVE_UTIME
ZEND_BEGIN_ARG_INFO_EX(arginfo_touch, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, time)
ZEND_ARG_INFO(0, atime)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_clearstatcache, 0, 0, 0)
ZEND_ARG_INFO(0, clear_realpath_cache)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_realpath_cache_size, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_realpath_cache_get, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fileperms, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fileinode, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_filesize, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fileowner, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_filegroup, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fileatime, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_filemtime, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_filectime, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_filetype, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_writable, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_readable, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_executable, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_file, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_dir, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_link, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_file_exists, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_lstat, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stat, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ formatted_print.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_sprintf, 0, 0, 2)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_vsprintf, 0)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, args) /* ARRAY_INFO(0, args, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_printf, 0, 0, 1)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_vprintf, 0)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, args) /* ARRAY_INFO(0, args, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fprintf, 0, 0, 2)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_vfprintf, 0)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, args) /* ARRAY_INFO(0, args, 1) */
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ fsock.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_fsockopen, 0, 0, 2)
ZEND_ARG_INFO(0, hostname)
ZEND_ARG_INFO(0, port)
ZEND_ARG_INFO(1, errno)
ZEND_ARG_INFO(1, errstr)
ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pfsockopen, 0, 0, 2)
ZEND_ARG_INFO(0, hostname)
ZEND_ARG_INFO(0, port)
ZEND_ARG_INFO(1, errno)
ZEND_ARG_INFO(1, errstr)
ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ ftok.c */
#if HAVE_FTOK
ZEND_BEGIN_ARG_INFO(arginfo_ftok, 0)
ZEND_ARG_INFO(0, pathname)
ZEND_ARG_INFO(0, proj)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ head.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_header, 0, 0, 1)
ZEND_ARG_INFO(0, header)
ZEND_ARG_INFO(0, replace)
ZEND_ARG_INFO(0, http_response_code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_header_remove, 0, 0, 0)
ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_setcookie, 0, 0, 1)
ZEND_ARG_INFO(0, name)
ZEND_ARG_INFO(0, value)
ZEND_ARG_INFO(0, expires)
ZEND_ARG_INFO(0, path)
ZEND_ARG_INFO(0, domain)
ZEND_ARG_INFO(0, secure)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_setrawcookie, 0, 0, 1)
ZEND_ARG_INFO(0, name)
ZEND_ARG_INFO(0, value)
ZEND_ARG_INFO(0, expires)
ZEND_ARG_INFO(0, path)
ZEND_ARG_INFO(0, domain)
ZEND_ARG_INFO(0, secure)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_headers_sent, 0, 0, 0)
ZEND_ARG_INFO(1, file)
ZEND_ARG_INFO(1, line)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_headers_list, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_http_response_code, 0)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ html.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_htmlspecialchars, 0, 0, 1)
ZEND_ARG_INFO(0, string)
ZEND_ARG_INFO(0, quote_style)
ZEND_ARG_INFO(0, charset)
ZEND_ARG_INFO(0, double_encode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_htmlspecialchars_decode, 0, 0, 1)
ZEND_ARG_INFO(0, string)
ZEND_ARG_INFO(0, quote_style)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_html_entity_decode, 0, 0, 1)
ZEND_ARG_INFO(0, string)
ZEND_ARG_INFO(0, quote_style)
ZEND_ARG_INFO(0, charset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_htmlentities, 0, 0, 1)
ZEND_ARG_INFO(0, string)
ZEND_ARG_INFO(0, quote_style)
ZEND_ARG_INFO(0, charset)
ZEND_ARG_INFO(0, double_encode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_get_html_translation_table, 0, 0, 0)
ZEND_ARG_INFO(0, table)
ZEND_ARG_INFO(0, quote_style)
ZEND_END_ARG_INFO()

/* }}} */
/* {{{ http.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_http_build_query, 0, 0, 1)
ZEND_ARG_INFO(0, formdata)
ZEND_ARG_INFO(0, prefix)
ZEND_ARG_INFO(0, arg_separator)
ZEND_ARG_INFO(0, enc_type)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ image.c */
ZEND_BEGIN_ARG_INFO(arginfo_image_type_to_mime_type, 0)
ZEND_ARG_INFO(0, imagetype)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_image_type_to_extension, 0, 0, 1)
ZEND_ARG_INFO(0, imagetype)
ZEND_ARG_INFO(0, include_dot)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getimagesize, 0, 0, 1)
ZEND_ARG_INFO(0, imagefile)
ZEND_ARG_INFO(1, info) /* ARRAY_INFO(1, info, 1) */
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ info.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_phpinfo, 0, 0, 0)
ZEND_ARG_INFO(0, what)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phpversion, 0, 0, 0)
ZEND_ARG_INFO(0, extension)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phpcredits, 0, 0, 0)
ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_logo_guid, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_real_logo_guid, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_egg_logo_guid, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_zend_logo_guid, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_sapi_name, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_uname, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_ini_scanned_files, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_ini_loaded_file, 0)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ iptc.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_iptcembed, 0, 0, 2)
ZEND_ARG_INFO(0, iptcdata)
ZEND_ARG_INFO(0, jpeg_file_name)
ZEND_ARG_INFO(0, spool)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_iptcparse, 0)
ZEND_ARG_INFO(0, iptcdata)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ lcg.c */
ZEND_BEGIN_ARG_INFO(arginfo_lcg_value, 0)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ levenshtein.c */
ZEND_BEGIN_ARG_INFO(arginfo_levenshtein, 0)
ZEND_ARG_INFO(0, str1)
ZEND_ARG_INFO(0, str2)
ZEND_ARG_INFO(0, cost_ins)
ZEND_ARG_INFO(0, cost_rep)
ZEND_ARG_INFO(0, cost_del)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ link.c */
#if defined(HAVE_SYMLINK) || defined(PHP_WIN32)
ZEND_BEGIN_ARG_INFO(arginfo_readlink, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_linkinfo, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_symlink, 0)
ZEND_ARG_INFO(0, target)
ZEND_ARG_INFO(0, link)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_link, 0)
ZEND_ARG_INFO(0, target)
ZEND_ARG_INFO(0, link)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ mail.c */
ZEND_BEGIN_ARG_INFO(arginfo_ezmlm_hash, 0)
ZEND_ARG_INFO(0, addr)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mail, 0, 0, 3)
ZEND_ARG_INFO(0, to)
ZEND_ARG_INFO(0, subject)
ZEND_ARG_INFO(0, message)
ZEND_ARG_INFO(0, additional_headers)
ZEND_ARG_INFO(0, additional_parameters)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ math.c */
ZEND_BEGIN_ARG_INFO(arginfo_abs, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ceil, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_floor, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_round, 0, 0, 1)
ZEND_ARG_INFO(0, number)
ZEND_ARG_INFO(0, precision)
ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_sin, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_cos, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_tan, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_asin, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_acos, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_atan, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_atan2, 0)
ZEND_ARG_INFO(0, y)
ZEND_ARG_INFO(0, x)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_sinh, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_cosh, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_tanh, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_asinh, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_acosh, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_atanh, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pi, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_finite, 0)
ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_infinite, 0)
ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_nan, 0)
ZEND_ARG_INFO(0, val)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_pow, 0)
ZEND_ARG_INFO(0, base)
ZEND_ARG_INFO(0, exponent)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_exp, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_expm1, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_log1p, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_log, 0, 0, 1)
ZEND_ARG_INFO(0, number)
ZEND_ARG_INFO(0, base)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_log10, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_sqrt, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_hypot, 0)
ZEND_ARG_INFO(0, num1)
ZEND_ARG_INFO(0, num2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_deg2rad, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_rad2deg, 0)
ZEND_ARG_INFO(0, number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_bindec, 0)
ZEND_ARG_INFO(0, binary_number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_hexdec, 0)
ZEND_ARG_INFO(0, hexadecimal_number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_octdec, 0)
ZEND_ARG_INFO(0, octal_number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_decbin, 0)
ZEND_ARG_INFO(0, decimal_number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_decoct, 0)
ZEND_ARG_INFO(0, decimal_number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dechex, 0)
ZEND_ARG_INFO(0, decimal_number)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_base_convert, 0)
ZEND_ARG_INFO(0, number)
ZEND_ARG_INFO(0, frombase)
ZEND_ARG_INFO(0, tobase)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_number_format, 0, 0, 1)
ZEND_ARG_INFO(0, number)
ZEND_ARG_INFO(0, num_decimal_places)
ZEND_ARG_INFO(0, dec_seperator)
ZEND_ARG_INFO(0, thousands_seperator)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_fmod, 0)
ZEND_ARG_INFO(0, x)
ZEND_ARG_INFO(0, y)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ md5.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_md5, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, raw_output)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_md5_file, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, raw_output)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ metaphone.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_metaphone, 0, 0, 1)
ZEND_ARG_INFO(0, text)
ZEND_ARG_INFO(0, phones)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ microtime.c */
#ifdef HAVE_GETTIMEOFDAY
ZEND_BEGIN_ARG_INFO_EX(arginfo_microtime, 0, 0, 0)
ZEND_ARG_INFO(0, get_as_float)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_gettimeofday, 0, 0, 0)
ZEND_ARG_INFO(0, get_as_float)
ZEND_END_ARG_INFO()
#endif

#ifdef HAVE_GETRUSAGE
ZEND_BEGIN_ARG_INFO_EX(arginfo_getrusage, 0, 0, 0)
ZEND_ARG_INFO(0, who)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ pack.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_pack, 0, 0, 2)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, arg1)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_unpack, 0)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, input)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ pageinfo.c */
ZEND_BEGIN_ARG_INFO(arginfo_getmyuid, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_getmygid, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_getmypid, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_getmyinode, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_getlastmod, 0)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ proc_open.c */
#ifdef PHP_CAN_SUPPORT_PROC_OPEN
ZEND_BEGIN_ARG_INFO_EX(arginfo_proc_terminate, 0, 0, 1)
ZEND_ARG_INFO(0, process)
ZEND_ARG_INFO(0, signal)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_proc_close, 0)
ZEND_ARG_INFO(0, process)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_proc_get_status, 0)
ZEND_ARG_INFO(0, process)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_proc_open, 0, 0, 3)
ZEND_ARG_INFO(0, command)
ZEND_ARG_INFO(0, descriptorspec) /* ARRAY_INFO(0, descriptorspec, 1) */
ZEND_ARG_INFO(1, pipes)          /* ARRAY_INFO(1, pipes, 1) */
ZEND_ARG_INFO(0, cwd)
ZEND_ARG_INFO(0, env)           /* ARRAY_INFO(0, env, 1) */
ZEND_ARG_INFO(0, other_options) /* ARRAY_INFO(0, other_options, 1) */
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ quot_print.c */
ZEND_BEGIN_ARG_INFO(arginfo_quoted_printable_decode, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ quot_print.c */
ZEND_BEGIN_ARG_INFO(arginfo_quoted_printable_encode, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ rand.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_srand, 0, 0, 0)
ZEND_ARG_INFO(0, seed)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mt_srand, 0, 0, 0)
ZEND_ARG_INFO(0, seed)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_rand, 0, 0, 0)
ZEND_ARG_INFO(0, min)
ZEND_ARG_INFO(0, max)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_mt_rand, 0, 0, 0)
ZEND_ARG_INFO(0, min)
ZEND_ARG_INFO(0, max)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_getrandmax, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_mt_getrandmax, 0)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ sha1.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_sha1, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, raw_output)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sha1_file, 0, 0, 1)
ZEND_ARG_INFO(0, filename)
ZEND_ARG_INFO(0, raw_output)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ soundex.c */
ZEND_BEGIN_ARG_INFO(arginfo_soundex, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ streamsfuncs.c */
#if HAVE_SOCKETPAIR
ZEND_BEGIN_ARG_INFO(arginfo_stream_socket_pair, 0)
ZEND_ARG_INFO(0, domain)
ZEND_ARG_INFO(0, type)
ZEND_ARG_INFO(0, protocol)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_socket_client, 0, 0, 1)
ZEND_ARG_INFO(0, remoteaddress)
ZEND_ARG_INFO(1, errcode)
ZEND_ARG_INFO(1, errstring)
ZEND_ARG_INFO(0, timeout)
ZEND_ARG_INFO(0, flags)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_socket_server, 0, 0, 1)
ZEND_ARG_INFO(0, localaddress)
ZEND_ARG_INFO(1, errcode)
ZEND_ARG_INFO(1, errstring)
ZEND_ARG_INFO(0, flags)
ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_socket_accept, 0, 0, 1)
ZEND_ARG_INFO(0, serverstream)
ZEND_ARG_INFO(0, timeout)
ZEND_ARG_INFO(1, peername)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_socket_get_name, 0)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, want_peer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_socket_sendto, 0, 0, 2)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, data)
ZEND_ARG_INFO(0, flags)
ZEND_ARG_INFO(0, target_addr)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_socket_recvfrom, 0, 0, 2)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, amount)
ZEND_ARG_INFO(0, flags)
ZEND_ARG_INFO(1, remote_addr)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_get_contents, 0, 0, 1)
ZEND_ARG_INFO(0, source)
ZEND_ARG_INFO(0, maxlen)
ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_copy_to_stream, 0, 0, 2)
ZEND_ARG_INFO(0, source)
ZEND_ARG_INFO(0, dest)
ZEND_ARG_INFO(0, maxlen)
ZEND_ARG_INFO(0, pos)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_get_meta_data, 0)
ZEND_ARG_INFO(0, fp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_get_transports, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_get_wrappers, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_resolve_include_path, 0)
ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_is_local, 0)
ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_supports_lock, 0, 0, 1)
ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_select, 0, 0, 4)
ZEND_ARG_INFO(1, read_streams)   /* ARRAY_INFO(1, read_streams, 1) */
ZEND_ARG_INFO(1, write_streams)  /* ARRAY_INFO(1, write_streams, 1) */
ZEND_ARG_INFO(1, except_streams) /* ARRAY_INFO(1, except_streams, 1) */
ZEND_ARG_INFO(0, tv_sec)
ZEND_ARG_INFO(0, tv_usec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_context_get_options, 0)
ZEND_ARG_INFO(0, stream_or_context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_context_set_option, 0)
ZEND_ARG_INFO(0, stream_or_context)
ZEND_ARG_INFO(0, wrappername)
ZEND_ARG_INFO(0, optionname)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_context_set_params, 0)
ZEND_ARG_INFO(0, stream_or_context)
ZEND_ARG_INFO(0, options) /* ARRAY_INFO(0, options, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_context_get_params, 0, ZEND_RETURN_VALUE,
                       1)
ZEND_ARG_INFO(0, stream_or_context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_context_get_default, 0, 0, 0)
ZEND_ARG_INFO(0, options) /* ARRAY_INFO(0, options, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_context_set_default, 0)
ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_context_create, 0, 0, 0)
ZEND_ARG_INFO(0, options) /* ARRAY_INFO(0, options, 1) */
ZEND_ARG_INFO(0, params)  /* ARRAY_INFO(0, params, 1) */
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_filter_prepend, 0, 0, 2)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, filtername)
ZEND_ARG_INFO(0, read_write)
ZEND_ARG_INFO(0, filterparams)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_filter_append, 0, 0, 2)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, filtername)
ZEND_ARG_INFO(0, read_write)
ZEND_ARG_INFO(0, filterparams)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_filter_remove, 0)
ZEND_ARG_INFO(0, stream_filter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_get_line, 0, 0, 2)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, maxlen)
ZEND_ARG_INFO(0, ending)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_set_blocking, 0)
ZEND_ARG_INFO(0, socket)
ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

#if HAVE_SYS_TIME_H || defined(PHP_WIN32)
ZEND_BEGIN_ARG_INFO(arginfo_stream_set_timeout, 0)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, seconds)
ZEND_ARG_INFO(0, microseconds)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO(arginfo_stream_set_read_buffer, 0)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_set_write_buffer, 0)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_set_chunk_size, 0)
ZEND_ARG_INFO(0, fp)
ZEND_ARG_INFO(0, chunk_size)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stream_socket_enable_crypto, 0, 0, 2)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, enable)
ZEND_ARG_INFO(0, cryptokind)
ZEND_ARG_INFO(0, sessionstream)
ZEND_END_ARG_INFO()

#ifdef HAVE_SHUTDOWN
ZEND_BEGIN_ARG_INFO(arginfo_stream_socket_shutdown, 0)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, how)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ string.c */
ZEND_BEGIN_ARG_INFO(arginfo_bin2hex, 0)
ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strspn, 0, 0, 2)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, mask)
ZEND_ARG_INFO(0, start)
ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strcspn, 0, 0, 2)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, mask)
ZEND_ARG_INFO(0, start)
ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

#if HAVE_NL_LANGINFO
ZEND_BEGIN_ARG_INFO(arginfo_nl_langinfo, 0)
ZEND_ARG_INFO(0, item)
ZEND_END_ARG_INFO()
#endif

#ifdef HAVE_STRCOLL
ZEND_BEGIN_ARG_INFO(arginfo_strcoll, 0)
ZEND_ARG_INFO(0, str1)
ZEND_ARG_INFO(0, str2)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_trim, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, character_mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_rtrim, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, character_mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ltrim, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, character_mask)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_wordwrap, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, width)
ZEND_ARG_INFO(0, break)
ZEND_ARG_INFO(0, cut)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_explode, 0, 0, 2)
ZEND_ARG_INFO(0, separator)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_implode, 0)
ZEND_ARG_INFO(0, glue)
ZEND_ARG_INFO(0, pieces)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_strtok, 0)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, token)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_strtoupper, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_strtolower, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_basename, 0, 0, 1)
ZEND_ARG_INFO(0, path)
ZEND_ARG_INFO(0, suffix)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_dirname, 0)
ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pathinfo, 0, 0, 1)
ZEND_ARG_INFO(0, path)
ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stristr, 0, 0, 2)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strstr, 0, 0, 2)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, part)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strpos, 0, 0, 2)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stripos, 0, 0, 2)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strrpos, 0, 0, 2)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strripos, 0, 0, 2)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_strrchr, 0)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, needle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_chunk_split, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, chunklen)
ZEND_ARG_INFO(0, ending)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_substr, 0, 0, 2)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, start)
ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_substr_replace, 0, 0, 3)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, replace)
ZEND_ARG_INFO(0, start)
ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_quotemeta, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ord, 0)
ZEND_ARG_INFO(0, character)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_chr, 0)
ZEND_ARG_INFO(0, codepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ucfirst, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_lcfirst, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_ucwords, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strtr, 0, 0, 2)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, from)
ZEND_ARG_INFO(0, to)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_strrev, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_similar_text, 0, 0, 2)
ZEND_ARG_INFO(0, str1)
ZEND_ARG_INFO(0, str2)
ZEND_ARG_INFO(1, percent)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_addcslashes, 0)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, charlist)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_addslashes, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stripcslashes, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stripslashes, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_str_replace, 0, 0, 3)
ZEND_ARG_INFO(0, search)
ZEND_ARG_INFO(0, replace)
ZEND_ARG_INFO(0, subject)
ZEND_ARG_INFO(1, replace_count)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_str_ireplace, 0, 0, 3)
ZEND_ARG_INFO(0, search)
ZEND_ARG_INFO(0, replace)
ZEND_ARG_INFO(0, subject)
ZEND_ARG_INFO(1, replace_count)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hebrev, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, max_chars_per_line)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hebrevc, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, max_chars_per_line)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_nl2br, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, is_xhtml)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strip_tags, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, allowable_tags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_setlocale, 0, 0, 2)
ZEND_ARG_INFO(0, category)
ZEND_ARG_INFO(0, locale)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parse_str, 0, 0, 1)
ZEND_ARG_INFO(0, encoded_string)
ZEND_ARG_INFO(1, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_str_getcsv, 0, 0, 1)
ZEND_ARG_INFO(0, string)
ZEND_ARG_INFO(0, delimiter)
ZEND_ARG_INFO(0, enclosure)
ZEND_ARG_INFO(0, escape)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_str_repeat, 0)
ZEND_ARG_INFO(0, input)
ZEND_ARG_INFO(0, mult)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_count_chars, 0, 0, 1)
ZEND_ARG_INFO(0, input)
ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_strnatcmp, 0)
ZEND_ARG_INFO(0, s1)
ZEND_ARG_INFO(0, s2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_localeconv, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_strnatcasecmp, 0)
ZEND_ARG_INFO(0, s1)
ZEND_ARG_INFO(0, s2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_substr_count, 0, 0, 2)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, needle)
ZEND_ARG_INFO(0, offset)
ZEND_ARG_INFO(0, length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_str_pad, 0, 0, 2)
ZEND_ARG_INFO(0, input)
ZEND_ARG_INFO(0, pad_length)
ZEND_ARG_INFO(0, pad_string)
ZEND_ARG_INFO(0, pad_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sscanf, 1, 0, 2)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(1, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_str_rot13, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_str_shuffle, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_str_word_count, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, charlist)
ZEND_END_ARG_INFO()

#ifdef HAVE_STRFMON
ZEND_BEGIN_ARG_INFO(arginfo_money_format, 0)
ZEND_ARG_INFO(0, format)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_str_split, 0, 0, 1)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, split_length)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_strpbrk, 0, 0, 1)
ZEND_ARG_INFO(0, haystack)
ZEND_ARG_INFO(0, char_list)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_substr_compare, 0, 0, 3)
ZEND_ARG_INFO(0, main_str)
ZEND_ARG_INFO(0, str)
ZEND_ARG_INFO(0, offset)
ZEND_ARG_INFO(0, length)
ZEND_ARG_INFO(0, case_sensitivity)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ syslog.c */
#ifdef HAVE_SYSLOG_H
ZEND_BEGIN_ARG_INFO(arginfo_openlog, 0)
ZEND_ARG_INFO(0, ident)
ZEND_ARG_INFO(0, option)
ZEND_ARG_INFO(0, facility)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_closelog, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_syslog, 0)
ZEND_ARG_INFO(0, priority)
ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ type.c */
ZEND_BEGIN_ARG_INFO(arginfo_gettype, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_settype, 0)
ZEND_ARG_INFO(1, var)
ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_intval, 0, 0, 1)
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, base)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_floatval, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_strval, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_null, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_resource, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_bool, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_long, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_float, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_string, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_array, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_object, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_numeric, 0)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_is_scalar, 0)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_is_callable, 0, 0, 1)
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, syntax_only)
ZEND_ARG_INFO(1, callable_name)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ uniqid.c */
#ifdef HAVE_GETTIMEOFDAY
ZEND_BEGIN_ARG_INFO_EX(arginfo_uniqid, 0, 0, 0)
ZEND_ARG_INFO(0, prefix)
ZEND_ARG_INFO(0, more_entropy)
ZEND_END_ARG_INFO()
#endif
/* }}} */
/* {{{ url.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_parse_url, 0, 0, 1)
ZEND_ARG_INFO(0, url)
ZEND_ARG_INFO(0, component)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_urlencode, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_urldecode, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_rawurlencode, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_rawurldecode, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_get_headers, 0, 0, 1)
ZEND_ARG_INFO(0, url)
ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ user_filters.c */
ZEND_BEGIN_ARG_INFO(arginfo_stream_bucket_make_writeable, 0)
ZEND_ARG_INFO(0, brigade)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_bucket_prepend, 0)
ZEND_ARG_INFO(0, brigade)
ZEND_ARG_INFO(0, bucket)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_bucket_append, 0)
ZEND_ARG_INFO(0, brigade)
ZEND_ARG_INFO(0, bucket)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_bucket_new, 0)
ZEND_ARG_INFO(0, stream)
ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_get_filters, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stream_filter_register, 0)
ZEND_ARG_INFO(0, filtername)
ZEND_ARG_INFO(0, classname)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ uuencode.c */
ZEND_BEGIN_ARG_INFO(arginfo_convert_uuencode, 0)
ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_convert_uudecode, 0)
ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ var.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_var_dump, 0, 0, 1)
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_debug_zval_dump, 0, 0, 1)
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_var_export, 0, 0, 1)
ZEND_ARG_INFO(0, var)
ZEND_ARG_INFO(0, return )
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_serialize, 0)
ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_unserialize, 0)
ZEND_ARG_INFO(0, variable_representation)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_memory_get_usage, 0, 0, 0)
ZEND_ARG_INFO(0, real_usage)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_memory_get_peak_usage, 0, 0, 0)
ZEND_ARG_INFO(0, real_usage)
ZEND_END_ARG_INFO()
/* }}} */
/* {{{ versioning.c */
ZEND_BEGIN_ARG_INFO_EX(arginfo_version_compare, 0, 0, 2)
ZEND_ARG_INFO(0, ver1)
ZEND_ARG_INFO(0, ver2)
ZEND_ARG_INFO(0, oper)
ZEND_END_ARG_INFO()
/* }}} */
/* }}} */

const zend_function_entry basic_functions
    [] = {/* {{{ */
          PHP_FE(constant, arginfo_constant) PHP_FE(
              bin2hex,
              arginfo_bin2hex) PHP_FE(sleep,
                                      arginfo_sleep) PHP_FE(usleep,
                                                            arginfo_usleep)
#if HAVE_NANOSLEEP
              PHP_FE(time_nanosleep, arginfo_time_nanosleep) PHP_FE(
                  time_sleep_until, arginfo_time_sleep_until)
#endif

#if HAVE_STRPTIME
                  PHP_FE(strptime, arginfo_strptime)
#endif

                      PHP_FE(flush, arginfo_flush) PHP_FE(wordwrap, arginfo_wordwrap) PHP_FE(
                          htmlspecialchars,
                          arginfo_htmlspecialchars) PHP_FE(htmlentities,
                                                           arginfo_htmlentities)
                          PHP_FE(html_entity_decode, arginfo_html_entity_decode) PHP_FE(
                              htmlspecialchars_decode,
                              arginfo_htmlspecialchars_decode) PHP_FE(get_html_translation_table,
                                                                      arginfo_get_html_translation_table)
                              PHP_FE(sha1, arginfo_sha1) PHP_FE(
                                  sha1_file,
                                  arginfo_sha1_file) PHP_NAMED_FE(md5,
                                                                  php_if_md5,
                                                                  arginfo_md5)
                                  PHP_NAMED_FE(
                                      md5_file, php_if_md5_file,
                                      arginfo_md5_file) PHP_NAMED_FE(crc32,
                                                                     php_if_crc32,
                                                                     arginfo_crc32)

                                      PHP_FE(iptcparse, arginfo_iptcparse) PHP_FE(
                                          iptcembed,
                                          arginfo_iptcembed) PHP_FE(getimagesize, arginfo_getimagesize)
                                          PHP_FE(getimagesizefromstring, arginfo_getimagesize) PHP_FE(
                                              image_type_to_mime_type,
                                              arginfo_image_type_to_mime_type)
                                              PHP_FE(
                                                  image_type_to_extension,
                                                  arginfo_image_type_to_extension)

                                                  PHP_FE(phpinfo, arginfo_phpinfo) PHP_FE(
                                                      phpversion,
                                                      arginfo_phpversion) PHP_FE(phpcredits, arginfo_phpcredits)
                                                      PHP_FE(php_logo_guid, arginfo_php_logo_guid) PHP_FE(
                                                          php_real_logo_guid,
                                                          arginfo_php_real_logo_guid)
                                                          PHP_FE(
                                                              php_egg_logo_guid,
                                                              arginfo_php_egg_logo_guid)
                                                              PHP_FE(zend_logo_guid, arginfo_zend_logo_guid) PHP_FE(
                                                                  php_sapi_name,
                                                                  arginfo_php_sapi_name)
                                                                  PHP_FE(
                                                                      php_uname,
                                                                      arginfo_php_uname)
                                                                      PHP_FE(
                                                                          php_ini_scanned_files,
                                                                          arginfo_php_ini_scanned_files)
                                                                          PHP_FE(
                                                                              php_ini_loaded_file,
                                                                              arginfo_php_ini_loaded_file)

                                                                              PHP_FE(
                                                                                  strnatcmp,
                                                                                  arginfo_strnatcmp)
                                                                                  PHP_FE(
                                                                                      strnatcasecmp,
                                                                                      arginfo_strnatcasecmp)
                                                                                      PHP_FE(
                                                                                          substr_count,
                                                                                          arginfo_substr_count)
                                                                                          PHP_FE(strspn, arginfo_strspn)
                                                                                              PHP_FE(strcspn, arginfo_strcspn) PHP_FE(strtok, arginfo_strtok)
                                                                                                  PHP_FE(
                                                                                                      strtoupper, arginfo_strtoupper)
                                                                                                      PHP_FE(strtolower, arginfo_strtolower)
                                                                                                          PHP_FE(
                                                                                                              strpos, arginfo_strpos)
                                                                                                              PHP_FE(stripos, arginfo_stripos) PHP_FE(strrpos, arginfo_strrpos) PHP_FE(strripos, arginfo_strripos) PHP_FE(strrev, arginfo_strrev) PHP_FE(hebrev, arginfo_hebrev) PHP_FE(hebrevc, arginfo_hebrevc) PHP_FE(nl2br, arginfo_nl2br) PHP_FE(basename, arginfo_basename)
                                                                                                                  PHP_FE(dirname, arginfo_dirname) PHP_FE(pathinfo, arginfo_pathinfo) PHP_FE(stripslashes, arginfo_stripslashes) PHP_FE(stripcslashes, arginfo_stripcslashes) PHP_FE(strstr, arginfo_strstr) PHP_FE(stristr, arginfo_stristr) PHP_FE(strrchr, arginfo_strrchr) PHP_FE(str_shuffle, arginfo_str_shuffle) PHP_FE(str_word_count, arginfo_str_word_count) PHP_FE(
                                                                                                                      str_split, arginfo_str_split) PHP_FE(strpbrk,
                                                                                                                                                           arginfo_strpbrk) PHP_FE(substr_compare,
                                                                                                                                                                                   arginfo_substr_compare)

#ifdef HAVE_STRCOLL
                                                                                                                      PHP_FE(
                                                                                                                          strcoll, arginfo_strcoll)
#endif

#ifdef HAVE_STRFMON
                                                                                                                          PHP_FE(
                                                                                                                              money_format,
                                                                                                                              arginfo_money_format)
#endif

                                                                                                                              PHP_FE(
                                                                                                                                  substr, arginfo_substr) PHP_FE(substr_replace, arginfo_substr_replace) PHP_FE(quotemeta,
                                                                                                                                                                                                                arginfo_quotemeta) PHP_FE(ucfirst,
                                                                                                                                                                                                                                          arginfo_ucfirst)
                                                                                                                                  PHP_FE(lcfirst, arginfo_lcfirst) PHP_FE(ucwords, arginfo_ucwords) PHP_FE(strtr, arginfo_strtr) PHP_FE(addslashes, arginfo_addslashes) PHP_FE(addcslashes, arginfo_addcslashes) PHP_FE(rtrim, arginfo_rtrim) PHP_FE(str_replace, arginfo_str_replace) PHP_FE(str_ireplace, arginfo_str_ireplace)
                                                                                                                                      PHP_FE(str_repeat, arginfo_str_repeat)
                                                                                                                                          PHP_FE(count_chars, arginfo_count_chars) PHP_FE(chunk_split, arginfo_chunk_split) PHP_FE(trim,
                                                                                                                                                                                                                                   arginfo_trim) PHP_FE(ltrim,
                                                                                                                                                                                                                                                        arginfo_ltrim) PHP_FE(strip_tags,
                                                                                                                                                                                                                                                                              arginfo_strip_tags)
                                                                                                                                              PHP_FE(
                                                                                                                                                  similar_text,
                                                                                                                                                  arginfo_similar_text)
                                                                                                                                                  PHP_FE(explode, arginfo_explode) PHP_FE(implode, arginfo_implode) PHP_FALIAS(join, implode, arginfo_implode)
                                                                                                                                                      PHP_FE(setlocale, arginfo_setlocale) PHP_FE(localeconv,
                                                                                                                                                                                                  arginfo_localeconv)

#if HAVE_NL_LANGINFO
                                                                                                                                                          PHP_FE(
                                                                                                                                                              nl_langinfo, arginfo_nl_langinfo)
#endif

                                                                                                                                                              PHP_FE(soundex, arginfo_soundex) PHP_FE(levenshtein,
                                                                                                                                                                                                      arginfo_levenshtein) PHP_FE(chr, arginfo_chr)
                                                                                                                                                                  PHP_FE(
                                                                                                                                                                      ord, arginfo_ord) PHP_FE(parse_str, arginfo_parse_str)
                                                                                                                                                                      PHP_FE(str_getcsv, arginfo_str_getcsv) PHP_FE(str_pad, arginfo_str_pad) PHP_FALIAS(chop,
                                                                                                                                                                                                                                                         rtrim,
                                                                                                                                                                                                                                                         arginfo_rtrim)
                                                                                                                                                                          PHP_FALIAS(strchr, strstr, arginfo_strstr)
                                                                                                                                                                              PHP_NAMED_FE(sprintf, PHP_FN(user_sprintf), arginfo_sprintf) PHP_NAMED_FE(printf,
                                                                                                                                                                                                                                                        PHP_FN(user_printf), arginfo_printf)
                                                                                                                                                                                  PHP_FE(vprintf, arginfo_vprintf) PHP_FE(
                                                                                                                                                                                      vsprintf,
                                                                                                                                                                                      arginfo_vsprintf)
                                                                                                                                                                                      PHP_FE(fprintf, arginfo_fprintf) PHP_FE(vfprintf,
                                                                                                                                                                                                                              arginfo_vfprintf) PHP_FE(sscanf,
                                                                                                                                                                                                                                                       arginfo_sscanf)
                                                                                                                                                                                          PHP_FE(fscanf, arginfo_fscanf)
                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                  parse_url,
                                                                                                                                                                                                  arginfo_parse_url)
                                                                                                                                                                                                  PHP_FE(urlencode, arginfo_urlencode) PHP_FE(urldecode, arginfo_urldecode) PHP_FE(rawurlencode,
                                                                                                                                                                                                                                                                                   arginfo_rawurlencode)
                                                                                                                                                                                                      PHP_FE(rawurldecode,
                                                                                                                                                                                                             arginfo_rawurldecode)
                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                              http_build_query, arginfo_http_build_query)

#if defined(HAVE_SYMLINK) || defined(PHP_WIN32)
                                                                                                                                                                                                              PHP_FE(readlink, arginfo_readlink) PHP_FE(linkinfo, arginfo_linkinfo) PHP_FE(symlink, arginfo_symlink) PHP_FE(
                                                                                                                                                                                                                  link,
                                                                                                                                                                                                                  arginfo_link)
#endif

                                                                                                                                                                                                                  PHP_FE(unlink,
                                                                                                                                                                                                                         arginfo_unlink) PHP_FE(exec,
                                                                                                                                                                                                                                                arginfo_exec) PHP_FE(system,
                                                                                                                                                                                                                                                                     arginfo_system)
                                                                                                                                                                                                                      PHP_FE(escapeshellcmd, arginfo_escapeshellcmd) PHP_FE(
                                                                                                                                                                                                                          escapeshellarg, arginfo_escapeshellarg)
                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                              passthru, arginfo_passthru)
                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                  shell_exec, arginfo_shell_exec)
#ifdef PHP_CAN_SUPPORT_PROC_OPEN
                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                      proc_open, arginfo_proc_open) PHP_FE(proc_close, arginfo_proc_close) PHP_FE(proc_terminate,
                                                                                                                                                                                                                                                                                                                  arginfo_proc_terminate) PHP_FE(proc_get_status,
                                                                                                                                                                                                                                                                                                                                                 arginfo_proc_get_status)
#endif

#ifdef HAVE_NICE
                                                                                                                                                                                                                                      PHP_FE(
                                                                                                                                                                                                                                          proc_nice,
                                                                                                                                                                                                                                          arginfo_proc_nice)
#endif

                                                                                                                                                                                                                                          PHP_FE(rand, arginfo_rand) PHP_FE(srand, arginfo_srand)
                                                                                                                                                                                                                                              PHP_FE(getrandmax, arginfo_getrandmax)
                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                      mt_rand,
                                                                                                                                                                                                                                                      arginfo_mt_rand)
                                                                                                                                                                                                                                                      PHP_FE(mt_srand, arginfo_mt_srand) PHP_FE(mt_getrandmax, arginfo_mt_getrandmax)

#if HAVE_GETSERVBYNAME
                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                              getservbyname,
                                                                                                                                                                                                                                                              arginfo_getservbyname)
#endif

#if HAVE_GETSERVBYPORT
                                                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                                                  getservbyport,
                                                                                                                                                                                                                                                                  arginfo_getservbyport)
#endif

#if HAVE_GETPROTOBYNAME
                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                      getprotobyname,
                                                                                                                                                                                                                                                                      arginfo_getprotobyname)
#endif

#if HAVE_GETPROTOBYNUMBER
                                                                                                                                                                                                                                                                      PHP_FE(
                                                                                                                                                                                                                                                                          getprotobynumber,
                                                                                                                                                                                                                                                                          arginfo_getprotobynumber)
#endif

                                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                                              getmyuid, arginfo_getmyuid) PHP_FE(getmygid, arginfo_getmygid) PHP_FE(getmypid,
                                                                                                                                                                                                                                                                                                                                                    arginfo_getmypid) PHP_FE(getmyinode,
                                                                                                                                                                                                                                                                                                                                                                             arginfo_getmyinode) PHP_FE(getlastmod, arginfo_getlastmod)

                                                                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                                                                  base64_decode,
                                                                                                                                                                                                                                                                                  arginfo_base64_decode) PHP_FE(base64_encode,
                                                                                                                                                                                                                                                                                                                arginfo_base64_encode)

                                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                                      convert_uuencode,
                                                                                                                                                                                                                                                                                      arginfo_convert_uuencode) PHP_FE(convert_uudecode,
                                                                                                                                                                                                                                                                                                                       arginfo_convert_uudecode)

                                                                                                                                                                                                                                                                                      PHP_FE(abs, arginfo_abs) PHP_FE(ceil, arginfo_ceil) PHP_FE(floor, arginfo_floor) PHP_FE(round, arginfo_round) PHP_FE(sin, arginfo_sin) PHP_FE(cos, arginfo_cos) PHP_FE(tan, arginfo_tan) PHP_FE(asin, arginfo_asin) PHP_FE(acos, arginfo_acos) PHP_FE(atan, arginfo_atan) PHP_FE(atanh, arginfo_atanh) PHP_FE(atan2, arginfo_atan2) PHP_FE(sinh, arginfo_sinh) PHP_FE(cosh, arginfo_cosh) PHP_FE(tanh, arginfo_tanh) PHP_FE(asinh, arginfo_asinh) PHP_FE(acosh, arginfo_acosh) PHP_FE(expm1, arginfo_expm1) PHP_FE(log1p, arginfo_log1p) PHP_FE(pi,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      arginfo_pi) PHP_FE(is_finite, arginfo_is_finite) PHP_FE(is_nan, arginfo_is_nan) PHP_FE(is_infinite, arginfo_is_infinite) PHP_FE(pow,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      arginfo_pow) PHP_FE(exp, arginfo_exp) PHP_FE(log, arginfo_log) PHP_FE(log10, arginfo_log10) PHP_FE(sqrt, arginfo_sqrt) PHP_FE(hypot, arginfo_hypot) PHP_FE(deg2rad, arginfo_deg2rad) PHP_FE(rad2deg, arginfo_rad2deg) PHP_FE(bindec,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   arginfo_bindec) PHP_FE(hexdec,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          arginfo_hexdec) PHP_FE(octdec, arginfo_octdec) PHP_FE(decbin, arginfo_decbin) PHP_FE(decoct,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               arginfo_decoct)
                                                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                                                              dechex,
                                                                                                                                                                                                                                                                                              arginfo_dechex) PHP_FE(base_convert, arginfo_base_convert)
                                                                                                                                                                                                                                                                                              PHP_FE(number_format, arginfo_number_format) PHP_FE(fmod, arginfo_fmod)
#ifdef HAVE_INET_NTOP
                                                                                                                                                                                                                                                                                                  PHP_RAW_NAMED_FE(inet_ntop, php_inet_ntop, arginfo_inet_ntop)
#endif
#ifdef HAVE_INET_PTON
                                                                                                                                                                                                                                                                                                      PHP_RAW_NAMED_FE(
                                                                                                                                                                                                                                                                                                          inet_pton,
                                                                                                                                                                                                                                                                                                          php_inet_pton,
                                                                                                                                                                                                                                                                                                          arginfo_inet_pton)
#endif
                                                                                                                                                                                                                                                                                                          PHP_FE(ip2long, arginfo_ip2long) PHP_FE(long2ip, arginfo_long2ip)

                                                                                                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                                                                                                  getenv,
                                                                                                                                                                                                                                                                                                                  arginfo_getenv)
#ifdef HAVE_PUTENV
                                                                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                                                                      putenv,
                                                                                                                                                                                                                                                                                                                      arginfo_putenv)
#endif

                                                                                                                                                                                                                                                                                                                      PHP_FE(
                                                                                                                                                                                                                                                                                                                          getopt,
                                                                                                                                                                                                                                                                                                                          arginfo_getopt)

#ifdef HAVE_GETLOADAVG
                                                                                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                                                                                              sys_getloadavg,
                                                                                                                                                                                                                                                                                                                              arginfo_sys_getloadavg)
#endif
#ifdef HAVE_GETTIMEOFDAY
                                                                                                                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                                                                                                                  microtime,
                                                                                                                                                                                                                                                                                                                                  arginfo_microtime) PHP_FE(gettimeofday,
                                                                                                                                                                                                                                                                                                                                                            arginfo_gettimeofday)
#endif

#ifdef HAVE_GETRUSAGE
                                                                                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                                                                                      getrusage,
                                                                                                                                                                                                                                                                                                                                      arginfo_getrusage)
#endif

#ifdef HAVE_GETTIMEOFDAY
                                                                                                                                                                                                                                                                                                                                      PHP_FE(
                                                                                                                                                                                                                                                                                                                                          uniqid,
                                                                                                                                                                                                                                                                                                                                          arginfo_uniqid)
#endif

                                                                                                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                                                                                                              quoted_printable_decode,
                                                                                                                                                                                                                                                                                                                                              arginfo_quoted_printable_decode)
                                                                                                                                                                                                                                                                                                                                              PHP_FE(quoted_printable_encode,
                                                                                                                                                                                                                                                                                                                                                     arginfo_quoted_printable_encode) PHP_FE(convert_cyr_string, arginfo_convert_cyr_string)
                                                                                                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                                                                                                      get_current_user,
                                                                                                                                                                                                                                                                                                                                                      arginfo_get_current_user)
                                                                                                                                                                                                                                                                                                                                                      PHP_FE(
                                                                                                                                                                                                                                                                                                                                                          set_time_limit,
                                                                                                                                                                                                                                                                                                                                                          arginfo_set_time_limit)
                                                                                                                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                                                                                                                              header_register_callback,
                                                                                                                                                                                                                                                                                                                                                              arginfo_header_register_callback)
                                                                                                                                                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                  get_cfg_var, arginfo_get_cfg_var)

                                                                                                                                                                                                                                                                                                                                                                  PHP_DEP_FALIAS(magic_quotes_runtime, set_magic_quotes_runtime, arginfo_set_magic_quotes_runtime)
                                                                                                                                                                                                                                                                                                                                                                      PHP_DEP_FE(set_magic_quotes_runtime, arginfo_set_magic_quotes_runtime)
                                                                                                                                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                              get_magic_quotes_gpc, arginfo_get_magic_quotes_gpc)
                                                                                                                                                                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                  get_magic_quotes_runtime, arginfo_get_magic_quotes_runtime)

                                                                                                                                                                                                                                                                                                                                                                                  PHP_FE(error_log, arginfo_error_log) PHP_FE(error_get_last, arginfo_error_get_last) PHP_FE(call_user_func, arginfo_call_user_func) PHP_FE(call_user_func_array, arginfo_call_user_func_array)
                                                                                                                                                                                                                                                                                                                                                                                      PHP_DEP_FE(call_user_method, arginfo_call_user_method)
                                                                                                                                                                                                                                                                                                                                                                                          PHP_DEP_FE(call_user_method_array, arginfo_call_user_method_array)
                                                                                                                                                                                                                                                                                                                                                                                              PHP_FE(forward_static_call, arginfo_forward_static_call) PHP_FE(forward_static_call_array, arginfo_forward_static_call_array) PHP_FE(serialize, arginfo_serialize) PHP_FE(unserialize, arginfo_unserialize)

                                                                                                                                                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                      var_dump,
                                                                                                                                                                                                                                                                                                                                                                                                      arginfo_var_dump)
                                                                                                                                                                                                                                                                                                                                                                                                      PHP_FE(var_export, arginfo_var_export)
                                                                                                                                                                                                                                                                                                                                                                                                          PHP_FE(debug_zval_dump, arginfo_debug_zval_dump) PHP_FE(print_r, arginfo_print_r) PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                              memory_get_usage,
                                                                                                                                                                                                                                                                                                                                                                                                              arginfo_memory_get_usage)
                                                                                                                                                                                                                                                                                                                                                                                                              PHP_FE(memory_get_peak_usage, arginfo_memory_get_peak_usage)

                                                                                                                                                                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                      register_shutdown_function, arginfo_register_shutdown_function) PHP_FE(register_tick_function, arginfo_register_tick_function)
                                                                                                                                                                                                                                                                                                                                                                                                                      PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                          unregister_tick_function,
                                                                                                                                                                                                                                                                                                                                                                                                                          arginfo_unregister_tick_function)

                                                                                                                                                                                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                              highlight_file, arginfo_highlight_file) PHP_FALIAS(show_source, highlight_file, arginfo_highlight_file)
                                                                                                                                                                                                                                                                                                                                                                                                                              PHP_FE(highlight_string, arginfo_highlight_string)
                                                                                                                                                                                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                                      php_strip_whitespace,
                                                                                                                                                                                                                                                                                                                                                                                                                                      arginfo_php_strip_whitespace)

                                                                                                                                                                                                                                                                                                                                                                                                                                      PHP_FE(ini_get, arginfo_ini_get) PHP_FE(ini_get_all, arginfo_ini_get_all) PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                                          ini_set, arginfo_ini_set)
                                                                                                                                                                                                                                                                                                                                                                                                                                          PHP_FALIAS(ini_alter, ini_set, arginfo_ini_set)
                                                                                                                                                                                                                                                                                                                                                                                                                                              PHP_FE(ini_restore, arginfo_ini_restore) PHP_FE(get_include_path, arginfo_get_include_path)
                                                                                                                                                                                                                                                                                                                                                                                                                                                  PHP_FE(set_include_path, arginfo_set_include_path)
                                                                                                                                                                                                                                                                                                                                                                                                                                                      PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                                                          restore_include_path,
                                                                                                                                                                                                                                                                                                                                                                                                                                                          arginfo_restore_include_path)

                                                                                                                                                                                                                                                                                                                                                                                                                                                          PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                                                              setcookie,
                                                                                                                                                                                                                                                                                                                                                                                                                                                              arginfo_setcookie) PHP_FE(setrawcookie, arginfo_setrawcookie)
                                                                                                                                                                                                                                                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                                                                  header, arginfo_header)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                  PHP_FE(header_remove, arginfo_header_remove) PHP_FE(headers_sent, arginfo_headers_sent) PHP_FE(headers_list, arginfo_headers_list)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                      PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                                                                          http_response_code, arginfo_http_response_code)

                                                                                                                                                                                                                                                                                                                                                                                                                                                                          PHP_FE(connection_aborted, arginfo_connection_aborted) PHP_FE(connection_status, arginfo_connection_status) PHP_FE(ignore_user_abort, arginfo_ignore_user_abort) PHP_FE(parse_ini_file,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  arginfo_parse_ini_file) PHP_FE(parse_ini_string, arginfo_parse_ini_string)
#if ZEND_DEBUG
                                                                                                                                                                                                                                                                                                                                                                                                                                                                              PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  config_get_hash, arginfo_config_get_hash)
#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  PHP_FE(
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      is_uploaded_file,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      arginfo_is_uploaded_file) PHP_FE(move_uploaded_file, arginfo_move_uploaded_file)

          /* functions from dns.c */
          PHP_FE(gethostbyaddr, arginfo_gethostbyaddr)
              PHP_FE(gethostbyname, arginfo_gethostbyname) PHP_FE(
                  gethostbynamel, arginfo_gethostbynamel)

#ifdef HAVE_GETHOSTNAME
                  PHP_FE(gethostname, arginfo_gethostname)
#endif

#if defined(PHP_WIN32) ||                                                      \
    (HAVE_DNS_SEARCH_FUNC && !(defined(__BEOS__) || defined(NETWARE)))

                      PHP_FE(dns_check_record, arginfo_dns_check_record)
                          PHP_FALIAS(checkdnsrr, dns_check_record,
                                     arginfo_dns_check_record)

#if defined(PHP_WIN32) || HAVE_FULL_DNS_FUNCS
                              PHP_FE(dns_get_mx, arginfo_dns_get_mx) PHP_FALIAS(
                                  getmxrr, dns_get_mx,
                                  arginfo_dns_get_mx)
                                  PHP_FE(dns_get_record, arginfo_dns_get_record)
#endif
#endif

          /* functions from type.c */
          PHP_FE(intval, arginfo_intval) PHP_FE(floatval, arginfo_floatval) PHP_FALIAS(
              doubleval,
              floatval,
              arginfo_floatval) PHP_FE(strval,
                                       arginfo_strval) PHP_FE(gettype,
                                                              arginfo_gettype)
              PHP_FE(settype, arginfo_settype) PHP_FE(
                  is_null,
                  arginfo_is_null) PHP_FE(is_resource, arginfo_is_resource)
                  PHP_FE(is_bool, arginfo_is_bool) PHP_FE(
                      is_long,
                      arginfo_is_long) PHP_FE(is_float, arginfo_is_float)
                      PHP_FALIAS(is_int, is_long, arginfo_is_long) PHP_FALIAS(
                          is_integer,
                          is_long, arginfo_is_long)
                          PHP_FALIAS(is_double, is_float, arginfo_is_float)
                              PHP_FALIAS(
                                  is_real, is_float,
                                  arginfo_is_float) PHP_FE(is_numeric,
                                                           arginfo_is_numeric)
                                  PHP_FE(is_string, arginfo_is_string) PHP_FE(
                                      is_array,
                                      arginfo_is_array)
                                      PHP_FE(is_object, arginfo_is_object) PHP_FE(
                                          is_scalar,
                                          arginfo_is_scalar)
                                          PHP_FE(is_callable, arginfo_is_callable)

          /* functions from file.c */
          PHP_FE(pclose, arginfo_pclose) PHP_FE(popen, arginfo_popen) PHP_FE(
              readfile, arginfo_readfile) PHP_FE(rewind, arginfo_rewind)
              PHP_FE(rmdir, arginfo_rmdir) PHP_FE(umask, arginfo_umask)
                  PHP_FE(fclose, arginfo_fclose) PHP_FE(feof, arginfo_feof)
                      PHP_FE(fgetc, arginfo_fgetc) PHP_FE(fgets, arginfo_fgets)
                          PHP_FE(fgetss, arginfo_fgetss)
                              PHP_FE(fread, arginfo_fread) PHP_NAMED_FE(fopen, php_if_fopen, arginfo_fopen) PHP_FE(fpassthru,
                                                                                                                   arginfo_fpassthru) PHP_NAMED_FE(ftruncate, php_if_ftruncate, arginfo_ftruncate) PHP_NAMED_FE(fstat, php_if_fstat, arginfo_fstat) PHP_FE(fseek,
                                                                                                                                                                                                                                                           arginfo_fseek) PHP_FE(ftell, arginfo_ftell) PHP_FE(fflush,
                                                                                                                                                                                                                                                                                                              arginfo_fflush) PHP_FE(fwrite, arginfo_fwrite) PHP_FALIAS(fputs, fwrite, arginfo_fwrite) PHP_FE(mkdir, arginfo_mkdir) PHP_FE(rename, arginfo_rename) PHP_FE(copy,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                          arginfo_copy) PHP_FE(tempnam, arginfo_tempnam) PHP_NAMED_FE(tmpfile,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      php_if_tmpfile,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      arginfo_tmpfile) PHP_FE(file,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              arginfo_file) PHP_FE(file_get_contents,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   arginfo_file_get_contents) PHP_FE(file_put_contents, arginfo_file_put_contents) PHP_FE(stream_select,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          arginfo_stream_select) PHP_FE(stream_context_create,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        arginfo_stream_context_create) PHP_FE(stream_context_set_params,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              arginfo_stream_context_set_params) PHP_FE(stream_context_get_params, arginfo_stream_context_get_params) PHP_FE(stream_context_set_option, arginfo_stream_context_set_option) PHP_FE(stream_context_get_options, arginfo_stream_context_get_options) PHP_FE(stream_context_get_default, arginfo_stream_context_get_default) PHP_FE(stream_context_set_default, arginfo_stream_context_set_default) PHP_FE(stream_filter_prepend,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       arginfo_stream_filter_prepend) PHP_FE(stream_filter_append, arginfo_stream_filter_append) PHP_FE(stream_filter_remove, arginfo_stream_filter_remove) PHP_FE(stream_socket_client,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   arginfo_stream_socket_client) PHP_FE(stream_socket_server, arginfo_stream_socket_server) PHP_FE(stream_socket_accept, arginfo_stream_socket_accept) PHP_FE(stream_socket_get_name,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              arginfo_stream_socket_get_name) PHP_FE(stream_socket_recvfrom, arginfo_stream_socket_recvfrom) PHP_FE(stream_socket_sendto,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    arginfo_stream_socket_sendto) PHP_FE(stream_socket_enable_crypto,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         arginfo_stream_socket_enable_crypto)
#ifdef HAVE_SHUTDOWN
                                  PHP_FE(stream_socket_shutdown,
                                         arginfo_stream_socket_shutdown)
#endif
#if HAVE_SOCKETPAIR
                                      PHP_FE(stream_socket_pair,
                                             arginfo_stream_socket_pair)
#endif
                                          PHP_FE(stream_copy_to_stream, arginfo_stream_copy_to_stream) PHP_FE(
                                              stream_get_contents,
                                              arginfo_stream_get_contents) PHP_FE(stream_supports_lock, arginfo_stream_supports_lock)
                                              PHP_FE(fgetcsv, arginfo_fgetcsv) PHP_FE(fputcsv, arginfo_fputcsv) PHP_FE(
                                                  flock,
                                                  arginfo_flock) PHP_FE(get_meta_tags,
                                                                        arginfo_get_meta_tags)
                                                  PHP_FE(stream_set_read_buffer, arginfo_stream_set_read_buffer) PHP_FE(
                                                      stream_set_write_buffer,
                                                      arginfo_stream_set_write_buffer)
                                                      PHP_FALIAS(set_file_buffer, stream_set_write_buffer, arginfo_stream_set_write_buffer) PHP_FE(
                                                          stream_set_chunk_size,
                                                          arginfo_stream_set_chunk_size)

                                                          PHP_DEP_FALIAS(
                                                              set_socket_blocking,
                                                              stream_set_blocking,
                                                              arginfo_stream_set_blocking)
                                                              PHP_FE(stream_set_blocking, arginfo_stream_set_blocking) PHP_FALIAS(
                                                                  socket_set_blocking,
                                                                  stream_set_blocking,
                                                                  arginfo_stream_set_blocking)

                                                                  PHP_FE(stream_get_meta_data, arginfo_stream_get_meta_data) PHP_FE(stream_get_line,
                                                                                                                                    arginfo_stream_get_line) PHP_FE(stream_wrapper_register, arginfo_stream_wrapper_register) PHP_FALIAS(stream_register_wrapper, stream_wrapper_register, arginfo_stream_wrapper_register) PHP_FE(stream_wrapper_unregister,
                                                                                                                                                                                                                                                                                                                                   arginfo_stream_wrapper_unregister) PHP_FE(stream_wrapper_restore, arginfo_stream_wrapper_restore) PHP_FE(stream_get_wrappers,
                                                                                                                                                                                                                                                                                                                                                                                                                                            arginfo_stream_get_wrappers) PHP_FE(stream_get_transports,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                arginfo_stream_get_transports) PHP_FE(stream_resolve_include_path,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      arginfo_stream_resolve_include_path)
                                                                      PHP_FE(stream_is_local, arginfo_stream_is_local) PHP_FE(
                                                                          get_headers,
                                                                          arginfo_get_headers)

#if HAVE_SYS_TIME_H || defined(PHP_WIN32)
                                                                          PHP_FE(
                                                                              stream_set_timeout,
                                                                              arginfo_stream_set_timeout)
                                                                              PHP_FALIAS(
                                                                                  socket_set_timeout,
                                                                                  stream_set_timeout,
                                                                                  arginfo_stream_set_timeout)
#endif

                                                                                  PHP_FALIAS(
                                                                                      socket_get_status,
                                                                                      stream_get_meta_data,
                                                                                      arginfo_stream_get_meta_data)

#if (!defined(__BEOS__) && !defined(NETWARE) && HAVE_REALPATH) || defined(ZTS)
                                                                                      PHP_FE(
                                                                                          realpath,
                                                                                          arginfo_realpath)
#endif

#ifdef HAVE_FNMATCH
                                                                                          PHP_FE(
                                                                                              fnmatch,
                                                                                              arginfo_fnmatch)
#endif

          /* functions from fsock.c */
          PHP_FE(fsockopen, arginfo_fsockopen)
              PHP_FE(pfsockopen, arginfo_pfsockopen)

          /* functions from pack.c */
          PHP_FE(pack, arginfo_pack) PHP_FE(unpack, arginfo_unpack)

          /* functions from browscap.c */
          PHP_FE(get_browser, arginfo_get_browser)

#if HAVE_CRYPT
          /* functions from crypt.c */
          PHP_FE(crypt, arginfo_crypt)
#endif

          /* functions from dir.c */
          PHP_FE(opendir, arginfo_opendir) PHP_FE(closedir, arginfo_closedir)
              PHP_FE(chdir, arginfo_chdir)

#if defined(HAVE_CHROOT) && !defined(ZTS) && ENABLE_CHROOT_FUNC
                  PHP_FE(chroot, arginfo_chroot)
#endif

                      PHP_FE(getcwd, arginfo_getcwd) PHP_FE(rewinddir,
                                                            arginfo_rewinddir)
                          PHP_NAMED_FE(readdir, php_if_readdir, arginfo_readdir)
                              PHP_FALIAS(dir, getdir, arginfo_dir)
                                  PHP_FE(scandir, arginfo_scandir)
#ifdef HAVE_GLOB
                                      PHP_FE(glob, arginfo_glob)
#endif
          /* functions from filestat.c */
          PHP_FE(fileatime, arginfo_fileatime) PHP_FE(
              filectime,
              arginfo_filectime) PHP_FE(filegroup,
                                        arginfo_filegroup) PHP_FE(fileinode,
                                                                  arginfo_fileinode)
              PHP_FE(filemtime, arginfo_filemtime) PHP_FE(
                  fileowner,
                  arginfo_fileowner) PHP_FE(fileperms, arginfo_fileperms)
                  PHP_FE(filesize, arginfo_filesize) PHP_FE(
                      filetype, arginfo_filetype) PHP_FE(file_exists,
                                                         arginfo_file_exists)
                      PHP_FE(is_writable, arginfo_is_writable) PHP_FALIAS(
                          is_writeable, is_writable,
                          arginfo_is_writable) PHP_FE(is_readable, arginfo_is_readable)
                          PHP_FE(is_executable, arginfo_is_executable) PHP_FE(
                              is_file, arginfo_is_file) PHP_FE(is_dir,
                                                               arginfo_is_dir)
                              PHP_FE(is_link, arginfo_is_link) PHP_NAMED_FE(
                                  stat, php_if_stat,
                                  arginfo_stat) PHP_NAMED_FE(lstat, php_if_lstat, arginfo_lstat)
#ifndef NETWARE
                                  PHP_FE(chown, arginfo_chown) PHP_FE(
                                      chgrp, arginfo_chgrp)
#endif
#if HAVE_LCHOWN
                                      PHP_FE(lchown, arginfo_lchown)
#endif
#if HAVE_LCHOWN
                                          PHP_FE(lchgrp, arginfo_lchgrp)
#endif
                                              PHP_FE(chmod, arginfo_chmod)
#if HAVE_UTIME
                                                  PHP_FE(touch, arginfo_touch)
#endif
                                                      PHP_FE(
                                                          clearstatcache,
                                                          arginfo_clearstatcache)
                                                          PHP_FE(
                                                              disk_total_space,
                                                              arginfo_disk_total_space)
                                                              PHP_FE(
                                                                  disk_free_space,
                                                                  arginfo_disk_free_space)
                                                                  PHP_FALIAS(
                                                                      diskfreespace,
                                                                      disk_free_space,
                                                                      arginfo_disk_free_space)
                                                                      PHP_FE(
                                                                          realpath_cache_size,
                                                                          arginfo_realpath_cache_size)
                                                                          PHP_FE(
                                                                              realpath_cache_get,
                                                                              arginfo_realpath_cache_get)

          /* functions from mail.c */
          PHP_FE(mail, arginfo_mail) PHP_FE(ezmlm_hash, arginfo_ezmlm_hash)

/* functions from syslog.c */
#ifdef HAVE_SYSLOG_H
              PHP_FE(openlog, arginfo_openlog) PHP_FE(syslog, arginfo_syslog)
                  PHP_FE(closelog, arginfo_closelog)
#endif

          /* functions from lcg.c */
          PHP_FE(lcg_value, arginfo_lcg_value)

          /* functions from metaphone.c */
          PHP_FE(metaphone, arginfo_metaphone)

          /* functions from output.c */
          PHP_FE(ob_start, arginfo_ob_start) PHP_FE(ob_flush, arginfo_ob_flush)
              PHP_FE(ob_clean, arginfo_ob_clean) PHP_FE(ob_end_flush,
                                                        arginfo_ob_end_flush)
                  PHP_FE(ob_end_clean, arginfo_ob_end_clean)
                      PHP_FE(ob_get_flush, arginfo_ob_get_flush)
                          PHP_FE(ob_get_clean, arginfo_ob_get_clean) PHP_FE(
                              ob_get_length, arginfo_ob_get_length)
                              PHP_FE(ob_get_level, arginfo_ob_get_level)
                                  PHP_FE(ob_get_status, arginfo_ob_get_status)
                                      PHP_FE(ob_get_contents,
                                             arginfo_ob_get_contents)
                                          PHP_FE(ob_implicit_flush,
                                                 arginfo_ob_implicit_flush)
                                              PHP_FE(ob_list_handlers,
                                                     arginfo_ob_list_handlers)

          /* functions from array.c */
          PHP_FE(ksort, arginfo_ksort) PHP_FE(krsort, arginfo_krsort) PHP_FE(
              natsort, arginfo_natsort) PHP_FE(natcasesort, arginfo_natcasesort)
              PHP_FE(asort, arginfo_asort) PHP_FE(arsort, arginfo_arsort)
                  PHP_FE(sort, arginfo_sort) PHP_FE(rsort, arginfo_rsort)
                      PHP_FE(usort, arginfo_usort)
                          PHP_FE(uasort,
                                 arginfo_uasort) PHP_FE(uksort,
                                                        arginfo_uksort) PHP_FE(shuffle, arginfo_shuffle) PHP_FE(array_walk, arginfo_array_walk) PHP_FE(array_walk_recursive, arginfo_array_walk_recursive) PHP_FE(count, arginfo_count) PHP_FE(end,
                                                                                                                                                                                                                                               arginfo_end) PHP_FE(prev,
                                                                                                                                                                                                                                                                   arginfo_prev) PHP_FE(next, arginfo_next) PHP_FE(reset,
                                                                                                                                                                                                                                                                                                                   arginfo_reset) PHP_FE(current,
                                                                                                                                                                                                                                                                                                                                         arginfo_current) PHP_FE(key,
                                                                                                                                                                                                                                                                                                                                                                 arginfo_key) PHP_FE(min,
                                                                                                                                                                                                                                                                                                                                                                                     arginfo_min) PHP_FE(max,
                                                                                                                                                                                                                                                                                                                                                                                                         arginfo_max) PHP_FE(in_array,
                                                                                                                                                                                                                                                                                                                                                                                                                             arginfo_in_array) PHP_FE(array_search, arginfo_array_search) PHP_FE(extract, arginfo_extract) PHP_FE(compact, arginfo_compact) PHP_FE(array_fill, arginfo_array_fill) PHP_FE(array_fill_keys, arginfo_array_fill_keys) PHP_FE(range, arginfo_range) PHP_FE(array_multisort, arginfo_array_multisort) PHP_FE(array_push, arginfo_array_push) PHP_FE(array_pop, arginfo_array_pop) PHP_FE(array_shift, arginfo_array_shift) PHP_FE(array_unshift, arginfo_array_unshift) PHP_FE(array_splice, arginfo_array_splice) PHP_FE(array_slice,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      arginfo_array_slice) PHP_FE(array_merge,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  arginfo_array_merge) PHP_FE(array_merge_recursive,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              arginfo_array_merge_recursive) PHP_FE(array_replace,
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    arginfo_array_replace)
                              PHP_FE(array_replace_recursive, arginfo_array_replace_recursive) PHP_FE(array_keys, arginfo_array_keys) PHP_FE(
                                  array_values,
                                  arginfo_array_values) PHP_FE(array_count_values,
                                                               arginfo_array_count_values) PHP_FE(array_reverse,
                                                                                                  arginfo_array_reverse)
                                  PHP_FE(array_reduce, arginfo_array_reduce) PHP_FE(
                                      array_pad,
                                      arginfo_array_pad) PHP_FE(array_flip,
                                                                arginfo_array_flip) PHP_FE(array_change_key_case,
                                                                                           arginfo_array_change_key_case)
                                      PHP_FE(array_rand, arginfo_array_rand) PHP_FE(
                                          array_unique,
                                          arginfo_array_unique) PHP_FE(array_intersect, arginfo_array_intersect)
                                          PHP_FE(array_intersect_key, arginfo_array_intersect_key) PHP_FE(
                                              array_intersect_ukey,
                                              arginfo_array_intersect_ukey) PHP_FE(array_uintersect,
                                                                                   arginfo_array_uintersect)
                                              PHP_FE(array_intersect_assoc, arginfo_array_intersect_assoc) PHP_FE(
                                                  array_uintersect_assoc,
                                                  arginfo_array_uintersect_assoc)
                                                  PHP_FE(array_intersect_uassoc, arginfo_array_intersect_uassoc) PHP_FE(
                                                      array_uintersect_uassoc,
                                                      arginfo_array_uintersect_uassoc)
                                                      PHP_FE(array_diff, arginfo_array_diff) PHP_FE(
                                                          array_diff_key,
                                                          arginfo_array_diff_key) PHP_FE(array_diff_ukey,
                                                                                         arginfo_array_diff_ukey)
                                                          PHP_FE(array_udiff, arginfo_array_udiff) PHP_FE(
                                                              array_diff_assoc,
                                                              arginfo_array_diff_assoc) PHP_FE(array_udiff_assoc,
                                                                                               arginfo_array_udiff_assoc)
                                                              PHP_FE(array_diff_uassoc, arginfo_array_diff_uassoc) PHP_FE(
                                                                  array_udiff_uassoc,
                                                                  arginfo_array_udiff_uassoc)
                                                                  PHP_FE(array_sum, arginfo_array_sum) PHP_FE(
                                                                      array_product,
                                                                      arginfo_array_product)
                                                                      PHP_FE(
                                                                          array_filter,
                                                                          arginfo_array_filter)
                                                                          PHP_FE(
                                                                              array_map,
                                                                              arginfo_array_map)
                                                                              PHP_FE(
                                                                                  array_chunk,
                                                                                  arginfo_array_chunk)
                                                                                  PHP_FE(
                                                                                      array_combine,
                                                                                      arginfo_array_combine)
                                                                                      PHP_FE(
                                                                                          array_key_exists,
                                                                                          arginfo_array_key_exists)

          /* aliases from array.c */
          PHP_FALIAS(pos, current, arginfo_current) PHP_FALIAS(sizeof, count,
                                                               arginfo_count)
              PHP_FALIAS(key_exists, array_key_exists, arginfo_array_key_exists)

          /* functions from assert.c */
          PHP_FE(assert, arginfo_assert)
              PHP_FE(assert_options, arginfo_assert_options)

          /* functions from versioning.c */
          PHP_FE(version_compare, arginfo_version_compare)

/* functions from ftok.c*/
#if HAVE_FTOK
              PHP_FE(ftok, arginfo_ftok)
#endif

                  PHP_FE(str_rot13, arginfo_str_rot13) PHP_FE(
                      stream_get_filters, arginfo_stream_get_filters)
                      PHP_FE(stream_filter_register,
                             arginfo_stream_filter_register)
                          PHP_FE(stream_bucket_make_writeable,
                                 arginfo_stream_bucket_make_writeable)
                              PHP_FE(stream_bucket_prepend,
                                     arginfo_stream_bucket_prepend)
                                  PHP_FE(stream_bucket_append,
                                         arginfo_stream_bucket_append)
                                      PHP_FE(stream_bucket_new,
                                             arginfo_stream_bucket_new)

                                          PHP_FE(output_add_rewrite_var,
                                                 arginfo_output_add_rewrite_var)
                                              PHP_FE(
                                                  output_reset_rewrite_vars,
                                                  arginfo_output_reset_rewrite_vars)

                                                  PHP_FE(
                                                      sys_get_temp_dir,
                                                      arginfo_sys_get_temp_dir)

                                                      {NULL, NULL, NULL}};
/* }}} */

static const zend_module_dep standard_deps[] = {/* {{{ */
                                                ZEND_MOD_OPTIONAL("session"){
                                                    NULL, NULL, NULL}};
/* }}} */

zend_module_entry basic_functions_module =
    {/* {{{ */
     STANDARD_MODULE_HEADER_EX,
     NULL,
     standard_deps,
     "standard",           /* extension name */
     basic_functions,      /* function list */
     PHP_MINIT(basic),     /* process startup */
     PHP_MSHUTDOWN(basic), /* process shutdown */
     PHP_RINIT(basic),     /* request startup */
     PHP_RSHUTDOWN(basic), /* request shutdown */
     PHP_MINFO(basic),     /* extension info */
     PHP_VERSION,          /* extension version */
     STANDARD_MODULE_PROPERTIES};
/* }}} */

#if defined(HAVE_PUTENV)
static void php_putenv_destructor(putenv_entry *pe) /* {{{ */
{
  if (pe->previous_value) {
#if _MSC_VER >= 1300
    /* VS.Net has a bug in putenv() when setting a variable that
     * is already set; if the SetEnvironmentVariable() API call
     * fails, the Crt will double free() a string.
     * We try to avoid this by setting our own value first */
    SetEnvironmentVariable(pe->key, "bugbug");
#endif
    putenv(pe->previous_value);
#if defined(PHP_WIN32)
    efree(pe->previous_value);
#endif
  } else {
#if HAVE_UNSETENV
    unsetenv(pe->key);
#elif defined(PHP_WIN32)
    SetEnvironmentVariable(pe->key, NULL);
#else
    char **env;

    for (env = environ; env != NULL && *env != NULL; env++) {
      if (!strncmp(*env, pe->key, pe->key_len) &&
          (*env)[pe->key_len] == '=') { /* found it */
        *env = "";
        break;
      }
    }
#endif
  }
#ifdef HAVE_TZSET
  /* don't forget to reset the various libc globals that
   * we might have changed by an earlier call to tzset(). */
  if (!strncmp(pe->key, "TZ", pe->key_len)) {
    tzset();
  }
#endif

  efree(pe->putenv_string);
  efree(pe->key);
}
/* }}} */
#endif

static void
basic_globals_ctor(php_basic_globals *basic_globals_p TSRMLS_DC) /* {{{ */
{
  BG(rand_is_seeded) = 0;
  BG(mt_rand_is_seeded) = 0;
  BG(umask) = -1;
  BG(next) = NULL;
  BG(left) = -1;
  BG(user_tick_functions) = NULL;
  BG(user_filter_map) = NULL;

  memset(&BG(serialize), 0, sizeof(BG(serialize)));
  memset(&BG(unserialize), 0, sizeof(BG(unserialize)));

  memset(&BG(url_adapt_state_ex), 0, sizeof(BG(url_adapt_state_ex)));

#if defined(_REENTRANT) && defined(HAVE_MBRLEN) && defined(HAVE_MBSTATE_T)
  memset(&BG(mblen_state), 0, sizeof(BG(mblen_state)));
#endif

  BG(incomplete_class) = incomplete_class_entry;
  BG(page_uid) = -1;
  BG(page_gid) = -1;
}
/* }}} */

static void
basic_globals_dtor(php_basic_globals *basic_globals_p TSRMLS_DC) /* {{{ */
{
  if (BG(url_adapt_state_ex).tags) {
    zend_hash_destroy(BG(url_adapt_state_ex).tags);
    free(BG(url_adapt_state_ex).tags);
  }
}
/* }}} */

#define PHP_DOUBLE_INFINITY_HIGH 0x7ff00000
#define PHP_DOUBLE_QUIET_NAN_HIGH 0xfff80000

PHPAPI double php_get_nan(void) /* {{{ */
{
#if HAVE_HUGE_VAL_NAN
  return HUGE_VAL + -HUGE_VAL;
#elif defined(__i386__) || defined(_X86_) || defined(ALPHA) ||                 \
    defined(_ALPHA) || defined(__alpha)
  double val = 0.0;
  ((php_uint32 *)&val)[1] = PHP_DOUBLE_QUIET_NAN_HIGH;
  ((php_uint32 *)&val)[0] = 0;
  return val;
#elif HAVE_ATOF_ACCEPTS_NAN
  return atof("NAN");
#else
  return 0.0 / 0.0;
#endif
}
/* }}} */

PHPAPI double php_get_inf(void) /* {{{ */
{
#if HAVE_HUGE_VAL_INF
  return HUGE_VAL;
#elif defined(__i386__) || defined(_X86_) || defined(ALPHA) ||                 \
    defined(_ALPHA) || defined(__alpha)
  double val = 0.0;
  ((php_uint32 *)&val)[1] = PHP_DOUBLE_INFINITY_HIGH;
  ((php_uint32 *)&val)[0] = 0;
  return val;
#elif HAVE_ATOF_ACCEPTS_INF
  return atof("INF");
#else
  return 1.0 / 0.0;
#endif
}
/* }}} */

PHP_MINIT_FUNCTION(basic) /* {{{ */
{
#ifdef ZTS
  ts_allocate_id(&basic_globals_id, sizeof(php_basic_globals),
                 (ts_allocate_ctor)basic_globals_ctor,
                 (ts_allocate_dtor)basic_globals_dtor);
#ifdef PHP_WIN32
  ts_allocate_id(&php_win32_core_globals_id, sizeof(php_win32_core_globals),
                 (ts_allocate_ctor)php_win32_core_globals_ctor,
                 (ts_allocate_dtor)php_win32_core_globals_dtor);
#endif
#else
  basic_globals_ctor(&basic_globals TSRMLS_CC);
#ifdef PHP_WIN32
  php_win32_core_globals_ctor(&the_php_win32_core_globals TSRMLS_CC);
#endif
#endif

  BG(incomplete_class) = incomplete_class_entry =
      php_create_incomplete_class(TSRMLS_C);

  REGISTER_LONG_CONSTANT("CONNECTION_ABORTED", PHP_CONNECTION_ABORTED,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("CONNECTION_NORMAL", PHP_CONNECTION_NORMAL,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("CONNECTION_TIMEOUT", PHP_CONNECTION_TIMEOUT,
                         CONST_CS | CONST_PERSISTENT);

  REGISTER_LONG_CONSTANT("INI_USER", ZEND_INI_USER,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("INI_PERDIR", ZEND_INI_PERDIR,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("INI_SYSTEM", ZEND_INI_SYSTEM,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("INI_ALL", ZEND_INI_ALL, CONST_CS | CONST_PERSISTENT);

  REGISTER_LONG_CONSTANT("INI_SCANNER_NORMAL", ZEND_INI_SCANNER_NORMAL,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("INI_SCANNER_RAW", ZEND_INI_SCANNER_RAW,
                         CONST_CS | CONST_PERSISTENT);

  REGISTER_LONG_CONSTANT("PHP_URL_SCHEME", PHP_URL_SCHEME,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_URL_HOST", PHP_URL_HOST,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_URL_PORT", PHP_URL_PORT,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_URL_USER", PHP_URL_USER,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_URL_PASS", PHP_URL_PASS,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_URL_PATH", PHP_URL_PATH,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_URL_QUERY", PHP_URL_QUERY,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_URL_FRAGMENT", PHP_URL_FRAGMENT,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_QUERY_RFC1738", PHP_QUERY_RFC1738,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_QUERY_RFC3986", PHP_QUERY_RFC3986,
                         CONST_CS | CONST_PERSISTENT);

#define REGISTER_MATH_CONSTANT(x)                                              \
  REGISTER_DOUBLE_CONSTANT(#x, x, CONST_CS | CONST_PERSISTENT)
  REGISTER_MATH_CONSTANT(M_E);
  REGISTER_MATH_CONSTANT(M_LOG2E);
  REGISTER_MATH_CONSTANT(M_LOG10E);
  REGISTER_MATH_CONSTANT(M_LN2);
  REGISTER_MATH_CONSTANT(M_LN10);
  REGISTER_MATH_CONSTANT(M_PI);
  REGISTER_MATH_CONSTANT(M_PI_2);
  REGISTER_MATH_CONSTANT(M_PI_4);
  REGISTER_MATH_CONSTANT(M_1_PI);
  REGISTER_MATH_CONSTANT(M_2_PI);
  REGISTER_MATH_CONSTANT(M_SQRTPI);
  REGISTER_MATH_CONSTANT(M_2_SQRTPI);
  REGISTER_MATH_CONSTANT(M_LNPI);
  REGISTER_MATH_CONSTANT(M_EULER);
  REGISTER_MATH_CONSTANT(M_SQRT2);
  REGISTER_MATH_CONSTANT(M_SQRT1_2);
  REGISTER_MATH_CONSTANT(M_SQRT3);
  REGISTER_DOUBLE_CONSTANT("INF", php_get_inf(), CONST_CS | CONST_PERSISTENT);
  REGISTER_DOUBLE_CONSTANT("NAN", php_get_nan(), CONST_CS | CONST_PERSISTENT);

  REGISTER_LONG_CONSTANT("PHP_ROUND_HALF_UP", PHP_ROUND_HALF_UP,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_ROUND_HALF_DOWN", PHP_ROUND_HALF_DOWN,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_ROUND_HALF_EVEN", PHP_ROUND_HALF_EVEN,
                         CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("PHP_ROUND_HALF_ODD", PHP_ROUND_HALF_ODD,
                         CONST_CS | CONST_PERSISTENT);

#if ENABLE_TEST_CLASS
  test_class_startup();
#endif

  register_phpinfo_constants(INIT_FUNC_ARGS_PASSTHRU);
  register_html_constants(INIT_FUNC_ARGS_PASSTHRU);
  register_string_constants(INIT_FUNC_ARGS_PASSTHRU);

  PHP_MINIT(file)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_MINIT(pack)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_MINIT(browscap)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_MINIT(standard_filters)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_MINIT(user_filters)(INIT_FUNC_ARGS_PASSTHRU);

#if defined(HAVE_LOCALECONV) && defined(ZTS)
  PHP_MINIT(localeconv)(INIT_FUNC_ARGS_PASSTHRU);
#endif

#if defined(HAVE_NL_LANGINFO)
  PHP_MINIT(nl_langinfo)(INIT_FUNC_ARGS_PASSTHRU);
#endif

#if HAVE_CRYPT
  PHP_MINIT(crypt)(INIT_FUNC_ARGS_PASSTHRU);
#endif

  PHP_MINIT(lcg)(INIT_FUNC_ARGS_PASSTHRU);

  PHP_MINIT(dir)(INIT_FUNC_ARGS_PASSTHRU);
#ifdef HAVE_SYSLOG_H
  PHP_MINIT(syslog)(INIT_FUNC_ARGS_PASSTHRU);
#endif
  PHP_MINIT(array)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_MINIT(assert)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_MINIT(url_scanner_ex)(INIT_FUNC_ARGS_PASSTHRU);
#ifdef PHP_CAN_SUPPORT_PROC_OPEN
  PHP_MINIT(proc_open)(INIT_FUNC_ARGS_PASSTHRU);
#endif

  PHP_MINIT(user_streams)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_MINIT(imagetypes)(INIT_FUNC_ARGS_PASSTHRU);

  php_register_url_stream_wrapper("php", &php_stream_php_wrapper TSRMLS_CC);
  php_register_url_stream_wrapper("file", &php_plain_files_wrapper TSRMLS_CC);
#ifdef HAVE_GLOB
  php_register_url_stream_wrapper("glob", &php_glob_stream_wrapper TSRMLS_CC);
#endif
  php_register_url_stream_wrapper("data",
                                  &php_stream_rfc2397_wrapper TSRMLS_CC);
#ifndef PHP_CURL_URL_WRAPPERS
  php_register_url_stream_wrapper("http", &php_stream_http_wrapper TSRMLS_CC);
  php_register_url_stream_wrapper("ftp", &php_stream_ftp_wrapper TSRMLS_CC);
#endif

#if defined(PHP_WIN32) ||                                                      \
    (HAVE_DNS_SEARCH_FUNC && !(defined(__BEOS__) || defined(NETWARE)))
#if defined(PHP_WIN32) || HAVE_FULL_DNS_FUNCS
  PHP_MINIT(dns)(INIT_FUNC_ARGS_PASSTHRU);
#endif
#endif

  return SUCCESS;
}
/* }}} */

PHP_MSHUTDOWN_FUNCTION(basic) /* {{{ */
{
#ifdef HAVE_SYSLOG_H
  PHP_MSHUTDOWN(syslog)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#endif
#ifdef ZTS
  ts_free_id(basic_globals_id);
#ifdef PHP_WIN32
  ts_free_id(php_win32_core_globals_id);
#endif
#else
  basic_globals_dtor(&basic_globals TSRMLS_CC);
#ifdef PHP_WIN32
  php_win32_core_globals_dtor(&the_php_win32_core_globals TSRMLS_CC);
#endif
#endif

  php_unregister_url_stream_wrapper("php" TSRMLS_CC);
#ifndef PHP_CURL_URL_WRAPPERS
  php_unregister_url_stream_wrapper("http" TSRMLS_CC);
  php_unregister_url_stream_wrapper("ftp" TSRMLS_CC);
#endif

  PHP_MSHUTDOWN(browscap)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
  PHP_MSHUTDOWN(array)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
  PHP_MSHUTDOWN(assert)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
  PHP_MSHUTDOWN(url_scanner_ex)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
  PHP_MSHUTDOWN(file)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
  PHP_MSHUTDOWN(standard_filters)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#if defined(HAVE_LOCALECONV) && defined(ZTS)
  PHP_MSHUTDOWN(localeconv)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#endif
#if HAVE_CRYPT
  PHP_MSHUTDOWN(crypt)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#endif

  return SUCCESS;
}
/* }}} */

PHP_RINIT_FUNCTION(basic) /* {{{ */
{
  memset(BG(strtok_table), 0, 256);
  BG(strtok_string) = NULL;
  BG(strtok_zval) = NULL;
  BG(strtok_last) = NULL;
  BG(locale_string) = NULL;
  BG(array_walk_fci) = empty_fcall_info;
  BG(array_walk_fci_cache) = empty_fcall_info_cache;
  BG(user_compare_fci) = empty_fcall_info;
  BG(user_compare_fci_cache) = empty_fcall_info_cache;
  BG(page_uid) = -1;
  BG(page_gid) = -1;
  BG(page_inode) = -1;
  BG(page_mtime) = -1;
#ifdef HAVE_PUTENV
  if (zend_hash_init(&BG(putenv_ht), 1, NULL,
                     (void (*)(void *))php_putenv_destructor, 0) == FAILURE) {
    return FAILURE;
  }
#endif
  BG(user_shutdown_function_names) = NULL;

  PHP_RINIT(filestat)(INIT_FUNC_ARGS_PASSTHRU);
#ifdef HAVE_SYSLOG_H
  PHP_RINIT(syslog)(INIT_FUNC_ARGS_PASSTHRU);
#endif
  PHP_RINIT(dir)(INIT_FUNC_ARGS_PASSTHRU);
  PHP_RINIT(url_scanner_ex)(INIT_FUNC_ARGS_PASSTHRU);

  /* Setup default context */
  FG(default_context) = NULL;

  /* Default to global wrappers only */
  FG(stream_wrappers) = NULL;

  /* Default to global filters only */
  FG(stream_filters) = NULL;

  return SUCCESS;
}
/* }}} */

PHP_RSHUTDOWN_FUNCTION(basic) /* {{{ */
{
  if (BG(strtok_zval)) {
    zval_ptr_dtor(&BG(strtok_zval));
  }
  BG(strtok_string) = NULL;
  BG(strtok_zval) = NULL;
#ifdef HAVE_PUTENV
  zend_hash_destroy(&BG(putenv_ht));
#endif

  if (BG(umask) != -1) {
    umask(BG(umask));
  }

  /* Check if locale was changed and change it back
   * to the value in startup environment */
  if (BG(locale_string) != NULL) {
    setlocale(LC_ALL, "C");
    setlocale(LC_CTYPE, "");
    zend_update_current_locale();
  }
  STR_FREE(BG(locale_string));
  BG(locale_string) = NULL;

  /* FG(stream_wrappers) and FG(stream_filters) are destroyed
   * during php_request_shutdown() */

  PHP_RSHUTDOWN(filestat)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#ifdef HAVE_SYSLOG_H
#ifdef PHP_WIN32
  PHP_RSHUTDOWN(syslog)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#endif
#endif
  PHP_RSHUTDOWN(assert)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
  PHP_RSHUTDOWN(url_scanner_ex)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
  PHP_RSHUTDOWN(streams)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#ifdef PHP_WIN32
  PHP_RSHUTDOWN(win32_core_globals)(SHUTDOWN_FUNC_ARGS_PASSTHRU);
#endif

  if (BG(user_tick_functions)) {
    zend_llist_destroy(BG(user_tick_functions));
    efree(BG(user_tick_functions));
    BG(user_tick_functions) = NULL;
  }

  PHP_RSHUTDOWN(user_filters)(SHUTDOWN_FUNC_ARGS_PASSTHRU);

  BG(page_uid) = -1;
  BG(page_gid) = -1;
  return SUCCESS;
}
/* }}} */

PHP_MINFO_FUNCTION(basic) /* {{{ */
{
  php_info_print_table_start();
  PHP_MINFO(dl)(ZEND_MODULE_INFO_FUNC_ARGS_PASSTHRU);
  PHP_MINFO(mail)(ZEND_MODULE_INFO_FUNC_ARGS_PASSTHRU);
  php_info_print_table_end();
  PHP_MINFO(assert)(ZEND_MODULE_INFO_FUNC_ARGS_PASSTHRU);
}
/* }}} */

/* {{{ proto mixed constant(string const_name)
   Given the name of a constant this function will return the constant's
   associated value */
PHP_FUNCTION(constant) {
  char *const_name;
  int const_name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &const_name,
                            &const_name_len) == FAILURE) {
    return;
  }

  if (!zend_get_constant_ex(const_name, const_name_len, return_value, NULL,
                            ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Couldn't find constant %s",
                     const_name);
    RETURN_NULL();
  }
}
/* }}} */

#ifdef HAVE_INET_NTOP
/* {{{ proto string inet_ntop(string in_addr)
   Converts a packed inet address to a human readable IP address string */
PHP_NAMED_FUNCTION(php_inet_ntop) {
  char *address;
  int address_len, af = AF_INET;
  char buffer[40];

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address,
                            &address_len) == FAILURE) {
    RETURN_FALSE;
  }

#ifdef HAVE_IPV6
  if (address_len == 16) {
    af = AF_INET6;
  } else
#endif
      if (address_len != 4) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid in_addr value");
    RETURN_FALSE;
  }

  if (!inet_ntop(af, address, buffer, sizeof(buffer))) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "An unknown error occured");
    RETURN_FALSE;
  }

  RETURN_STRING(buffer, 1);
}
/* }}} */
#endif /* HAVE_INET_NTOP */

#ifdef HAVE_INET_PTON
/* {{{ proto string inet_pton(string ip_address)
   Converts a human readable IP address to a packed binary string */
PHP_NAMED_FUNCTION(php_inet_pton) {
  int ret, af = AF_INET;
  char *address;
  int address_len;
  char buffer[17];

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &address,
                            &address_len) == FAILURE) {
    RETURN_FALSE;
  }

  memset(buffer, 0, sizeof(buffer));

#ifdef HAVE_IPV6
  if (strchr(address, ':')) {
    af = AF_INET6;
  } else
#endif
      if (!strchr(address, '.')) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unrecognized address %s",
                     address);
    RETURN_FALSE;
  }

  ret = inet_pton(af, address, buffer);

  if (ret <= 0) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unrecognized address %s",
                     address);
    RETURN_FALSE;
  }

  RETURN_STRINGL(buffer, af == AF_INET ? 4 : 16, 1);
}
/* }}} */
#endif /* HAVE_INET_PTON */

/* {{{ proto int ip2long(string ip_address)
   Converts a string containing an (IPv4) Internet Protocol dotted address into
   a proper address */
PHP_FUNCTION(ip2long) {
  char *addr;
  int addr_len;
#ifdef HAVE_INET_PTON
  struct in_addr ip;
#else
  unsigned long int ip;
#endif

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &addr, &addr_len) ==
      FAILURE) {
    return;
  }

#ifdef HAVE_INET_PTON
  if (addr_len == 0 || inet_pton(AF_INET, addr, &ip) != 1) {
    RETURN_FALSE;
  }
  RETURN_LONG(ntohl(ip.s_addr));
#else
  if (addr_len == 0 || (ip = inet_addr(addr)) == INADDR_NONE) {
    /* The only special case when we should return -1 ourselves,
     * because inet_addr() considers it wrong. We return 0xFFFFFFFF and
     * not -1 or ~0 because of 32/64bit issues. */
    if (addr_len == sizeof("255.255.255.255") - 1 &&
        !memcmp(addr, "255.255.255.255", sizeof("255.255.255.255") - 1)) {
      RETURN_LONG(0xFFFFFFFF);
    }
    RETURN_FALSE;
  }
  RETURN_LONG(ntohl(ip));
#endif
}
/* }}} */

/* {{{ proto string long2ip(int proper_address)
   Converts an (IPv4) Internet network address into a string in Internet
   standard dotted format */
PHP_FUNCTION(long2ip) {
  /* "It's a long but it's not, PHP ints are signed */
  char *ip;
  int ip_len;
  unsigned long n;
  struct in_addr myaddr;
#ifdef HAVE_INET_PTON
  char str[40];
#endif

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &ip, &ip_len) ==
      FAILURE) {
    return;
  }

  n = strtoul(ip, NULL, 0);

  myaddr.s_addr = htonl(n);
#ifdef HAVE_INET_PTON
  if (inet_ntop(AF_INET, &myaddr, str, sizeof(str))) {
    RETURN_STRING(str, 1);
  } else {
    RETURN_FALSE;
  }
#else
  RETURN_STRING(inet_ntoa(myaddr), 1);
#endif
}
/* }}} */

/********************
 * System Functions *
 ********************/

/* {{{ proto string getenv(string varname)
   Get the value of an environment variable */
PHP_FUNCTION(getenv) {
  char *ptr, *str;
  int str_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len) ==
      FAILURE) {
    RETURN_FALSE;
  }

  /* SAPI method returns an emalloc()'d string */
  ptr = sapi_getenv(str, str_len TSRMLS_CC);
  if (ptr) {
    RETURN_STRING(ptr, 0);
  }
#ifdef PHP_WIN32
  {
    char dummybuf;
    int size;

    SetLastError(0);
    /*If the given bugger is not large enough to hold the data, the return value
    is the buffer size,  in characters, required to hold the string and its
    terminating null character. We use this return value to alloc the final
    buffer. */
    size = GetEnvironmentVariableA(str, &dummybuf, 0);
    if (GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
      /* The environment variable doesn't exist. */
      RETURN_FALSE;
    }

    if (size == 0) {
      /* env exists, but it is empty */
      RETURN_EMPTY_STRING();
    }

    ptr = emalloc(size);
    size = GetEnvironmentVariableA(str, ptr, size);
    RETURN_STRING(ptr, 0);
  }
#else
  /* system method returns a const */
  ptr = getenv(str);
  if (ptr) {
    RETURN_STRING(ptr, 1);
  }
#endif
  RETURN_FALSE;
}
/* }}} */

#ifdef HAVE_PUTENV
/* {{{ proto bool putenv(string setting)
   Set the value of an environment variable */
PHP_FUNCTION(putenv) {
  char *setting;
  int setting_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &setting,
                            &setting_len) == FAILURE) {
    return;
  }

  if (setting_len) {
    char *p, **env;
    putenv_entry pe;
#ifdef PHP_WIN32
    char *value = NULL;
    int equals = 0;
    int error_code;
#endif

    pe.putenv_string = estrndup(setting, setting_len);
    pe.key = estrndup(setting, setting_len);
    if ((p = strchr(pe.key, '='))) { /* nullify the '=' if there is one */
      *p = '\0';
#ifdef PHP_WIN32
      equals = 1;
#endif
    }

    pe.key_len = strlen(pe.key);
#ifdef PHP_WIN32
    if (equals) {
      if (pe.key_len < setting_len - 1) {
        value = p + 1;
      } else {
        /* empty string*/
        value = p;
      }
    }
#endif

    zend_hash_del(&BG(putenv_ht), pe.key, pe.key_len + 1);

    /* find previous value */
    pe.previous_value = NULL;
    for (env = environ; env != NULL && *env != NULL; env++) {
      if (!strncmp(*env, pe.key, pe.key_len) &&
          (*env)[pe.key_len] == '=') { /* found it */
#if defined(PHP_WIN32)
        /* must copy previous value because MSVCRT's putenv can free the string
         * without notice */
        pe.previous_value = estrdup(*env);
#else
        pe.previous_value = *env;
#endif
        break;
      }
    }

#if HAVE_UNSETENV
    if (!p) { /* no '=' means we want to unset it */
      unsetenv(pe.putenv_string);
    }
    if (!p || putenv(pe.putenv_string) == 0) { /* success */
#else
#ifndef PHP_WIN32
    if (putenv(pe.putenv_string) == 0) { /* success */
#else
    error_code = SetEnvironmentVariable(pe.key, value);
#if _MSC_VER < 1500
    /* Yet another VC6 bug, unset may return env not found */
    if (error_code != 0 ||
        (error_code == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND)) {
#else
    if (error_code != 0) { /* success */
#endif
#endif
#endif
      zend_hash_add(&BG(putenv_ht), pe.key, pe.key_len + 1, (void **)&pe,
                    sizeof(putenv_entry), NULL);
#ifdef HAVE_TZSET
      if (!strncmp(pe.key, "TZ", pe.key_len)) {
        tzset();
      }
#endif
      RETURN_TRUE;
    } else {
      efree(pe.putenv_string);
      efree(pe.key);
      RETURN_FALSE;
    }
  }

  php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid parameter syntax");
  RETURN_FALSE;
}
/* }}} */
#endif

/* {{{ free_argv()
   Free the memory allocated to an argv array. */
static void free_argv(char **argv, int argc) {
  int i;

  if (argv) {
    for (i = 0; i < argc; i++) {
      if (argv[i]) {
        efree(argv[i]);
      }
    }
    efree(argv);
  }
}
/* }}} */

/* {{{ free_longopts()
   Free the memory allocated to an longopt array. */
static void free_longopts(opt_struct *longopts) {
  opt_struct *p;

  if (longopts) {
    for (p = longopts; p && p->opt_char != '-'; p++) {
      if (p->opt_name != NULL) {
        efree((char *)(p->opt_name));
      }
    }
  }
}
/* }}} */

/* {{{ parse_opts()
   Convert the typical getopt input characters to the php_getopt struct array */
static int parse_opts(char *opts, opt_struct **result) {
  opt_struct *paras = NULL;
  unsigned int i, count = 0;

  for (i = 0; i < strlen(opts); i++) {
    if ((opts[i] >= 48 && opts[i] <= 57) || (opts[i] >= 65 && opts[i] <= 90) ||
        (opts[i] >= 97 && opts[i] <= 122)) {
      count++;
    }
  }

  paras = safe_emalloc(sizeof(opt_struct), count, 0);
  memset(paras, 0, sizeof(opt_struct) * count);
  *result = paras;
  while ((*opts >= 48 && *opts <= 57) || /* 0 - 9 */
         (*opts >= 65 && *opts <= 90) || /* A - Z */
         (*opts >= 97 && *opts <= 122)   /* a - z */
  ) {
    paras->opt_char = *opts;
    paras->need_param = (*(++opts) == ':') ? 1 : 0;
    paras->opt_name = NULL;
    if (paras->need_param == 1) {
      opts++;
      if (*opts == ':') {
        paras->need_param++;
        opts++;
      }
    }
    paras++;
  }
  return count;
}
/* }}} */

/* {{{ proto array getopt(string options [, array longopts])
   Get options from the command line argument list */
PHP_FUNCTION(getopt) {
  char *options = NULL, **argv = NULL;
  char opt[2] = {'\0'};
  char *optname;
  int argc = 0, options_len = 0, len, o;
  char *php_optarg = NULL;
  int php_optind = 1;
  zval *val, **args = NULL, *p_longopts = NULL;
  int optname_len = 0;
  opt_struct *opts, *orig_opts;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|a", &options,
                            &options_len, &p_longopts) == FAILURE) {
    RETURN_FALSE;
  }

  /* Get argv from the global symbol table. We calculate argc ourselves
   * in order to be on the safe side, even though it is also available
   * from the symbol table. */
  if ((zend_hash_find(HASH_OF(PG(http_globals)[TRACK_VARS_SERVER]), "argv",
                      sizeof("argv"), (void **)&args) != FAILURE ||
       zend_hash_find(&EG(symbol_table), "argv", sizeof("argv"),
                      (void **)&args) != FAILURE) &&
      Z_TYPE_PP(args) == IS_ARRAY) {
    int pos = 0;
    zval **entry;

    argc = zend_hash_num_elements(Z_ARRVAL_PP(args));

    /* Attempt to allocate enough memory to hold all of the arguments
     * and a trailing NULL */
    argv = (char **)safe_emalloc(sizeof(char *), (argc + 1), 0);

    /* Reset the array indexes. */
    zend_hash_internal_pointer_reset(Z_ARRVAL_PP(args));

    /* Iterate over the hash to construct the argv array. */
    while (zend_hash_get_current_data(Z_ARRVAL_PP(args), (void **)&entry) ==
           SUCCESS) {
      zval arg, *arg_ptr = *entry;

      if (Z_TYPE_PP(entry) != IS_STRING) {
        arg = **entry;
        zval_copy_ctor(&arg);
        convert_to_string(&arg);
        arg_ptr = &arg;
      }

      argv[pos++] = estrdup(Z_STRVAL_P(arg_ptr));

      if (arg_ptr != *entry) {
        zval_dtor(&arg);
      }

      zend_hash_move_forward(Z_ARRVAL_PP(args));
    }

    /* The C Standard requires argv[argc] to be NULL - this might
     * keep some getopt implementations happy. */
    argv[argc] = NULL;
  } else {
    /* Return false if we can't find argv. */
    RETURN_FALSE;
  }

  len = parse_opts(options, &opts);

  if (p_longopts) {
    int count;
    zval **entry;

    count = zend_hash_num_elements(Z_ARRVAL_P(p_longopts));

    /* the first <len> slots are filled by the one short ops
     * we now extend our array and jump to the new added structs */
    opts = (opt_struct *)erealloc(opts, sizeof(opt_struct) * (len + count + 1));
    orig_opts = opts;
    opts += len;

    memset(opts, 0, count * sizeof(opt_struct));

    if (!opts) {
      RETURN_FALSE;
    }

    /* Reset the array indexes. */
    zend_hash_internal_pointer_reset(Z_ARRVAL_P(p_longopts));

    /* Iterate over the hash to construct the argv array. */
    while (zend_hash_get_current_data(Z_ARRVAL_P(p_longopts),
                                      (void **)&entry) == SUCCESS) {
      zval arg, *arg_ptr = *entry;

      if (Z_TYPE_PP(entry) != IS_STRING) {
        arg = **entry;
        zval_copy_ctor(&arg);
        convert_to_string(&arg);
        arg_ptr = &arg;
      }

      opts->need_param = 0;
      opts->opt_name = estrdup(Z_STRVAL_P(arg_ptr));
      len = strlen(opts->opt_name);
      if ((len > 0) && (opts->opt_name[len - 1] == ':')) {
        opts->need_param++;
        opts->opt_name[len - 1] = '\0';
        if ((len > 1) && (opts->opt_name[len - 2] == ':')) {
          opts->need_param++;
          opts->opt_name[len - 2] = '\0';
        }
      }
      opts->opt_char = 0;
      opts++;

      if (arg_ptr != *entry) {
        zval_dtor(&arg);
      }

      zend_hash_move_forward(Z_ARRVAL_P(p_longopts));
    }
  } else {
    opts = (opt_struct *)erealloc(opts, sizeof(opt_struct) * (len + 1));
    orig_opts = opts;
    opts += len;
  }

  /* php_getopt want to identify the last param */
  opts->opt_char = '-';
  opts->need_param = 0;
  opts->opt_name = NULL;

  /* Initialize the return value as an array. */
  array_init(return_value);

  /* after our pointer arithmetic jump back to the first element */
  opts = orig_opts;

  while ((o = php_getopt(argc, argv, opts, &php_optarg, &php_optind, 0, 1)) !=
         -1) {
    /* Skip unknown arguments. */
    if (o == '?') {
      continue;
    }

    /* Prepare the option character and the argument string. */
    if (o == 0) {
      optname = opts[php_optidx].opt_name;
    } else {
      if (o == 1) {
        o = '-';
      }
      opt[0] = o;
      optname = opt;
    }

    MAKE_STD_ZVAL(val);
    if (php_optarg != NULL) {
      /* keep the arg as binary, since the encoding is not known */
      ZVAL_STRING(val, php_optarg, 1);
    } else {
      ZVAL_FALSE(val);
    }

    /* Add this option / argument pair to the result hash. */
    optname_len = strlen(optname);
    if (!(optname_len > 1 && optname[0] == '0') &&
        is_numeric_string(optname, optname_len, NULL, NULL, 0) == IS_LONG) {
      /* numeric string */
      int optname_int = atoi(optname);
      if (zend_hash_index_find(HASH_OF(return_value), optname_int,
                               (void **)&args) != FAILURE) {
        if (Z_TYPE_PP(args) != IS_ARRAY) {
          convert_to_array_ex(args);
        }
        zend_hash_next_index_insert(HASH_OF(*args), (void *)&val,
                                    sizeof(zval *), NULL);
      } else {
        zend_hash_index_update(HASH_OF(return_value), optname_int, &val,
                               sizeof(zval *), NULL);
      }
    } else {
      /* other strings */
      if (zend_hash_find(HASH_OF(return_value), optname, strlen(optname) + 1,
                         (void **)&args) != FAILURE) {
        if (Z_TYPE_PP(args) != IS_ARRAY) {
          convert_to_array_ex(args);
        }
        zend_hash_next_index_insert(HASH_OF(*args), (void *)&val,
                                    sizeof(zval *), NULL);
      } else {
        zend_hash_add(HASH_OF(return_value), optname, strlen(optname) + 1,
                      (void *)&val, sizeof(zval *), NULL);
      }
    }

    php_optarg = NULL;
  }

  free_longopts(orig_opts);
  efree(orig_opts);
  free_argv(argv, argc);
}
/* }}} */

/* {{{ proto void flush(void)
   Flush the output buffer */
PHP_FUNCTION(flush) { sapi_flush(TSRMLS_C); }
/* }}} */

/* {{{ proto void sleep(int seconds)
   Delay for a given number of seconds */
PHP_FUNCTION(sleep) {
  long num;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &num) == FAILURE) {
    RETURN_FALSE;
  }
  if (num < 0) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING,
                     "Number of seconds must be greater than or equal to 0");
    RETURN_FALSE;
  }
#ifdef PHP_SLEEP_NON_VOID
  RETURN_LONG(php_sleep(num));
#else
  php_sleep(num);
#endif
}
/* }}} */

/* {{{ proto void usleep(int micro_seconds)
   Delay for a given number of micro seconds */
PHP_FUNCTION(usleep) {
#if HAVE_USLEEP
  long num;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &num) == FAILURE) {
    return;
  }
  if (num < 0) {
    php_error_docref(
        NULL TSRMLS_CC, E_WARNING,
        "Number of microseconds must be greater than or equal to 0");
    RETURN_FALSE;
  }
  usleep(num);
#endif
}
/* }}} */

#if HAVE_NANOSLEEP
/* {{{ proto mixed time_nanosleep(long seconds, long nanoseconds)
   Delay for a number of seconds and nano seconds */
PHP_FUNCTION(time_nanosleep) {
  long tv_sec, tv_nsec;
  struct timespec php_req, php_rem;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &tv_sec,
                            &tv_nsec) == FAILURE) {
    return;
  }

  php_req.tv_sec = (time_t)tv_sec;
  php_req.tv_nsec = tv_nsec;
  if (!nanosleep(&php_req, &php_rem)) {
    RETURN_TRUE;
  } else if (errno == EINTR) {
    array_init(return_value);
    add_assoc_long_ex(return_value, "seconds", sizeof("seconds"),
                      php_rem.tv_sec);
    add_assoc_long_ex(return_value, "nanoseconds", sizeof("nanoseconds"),
                      php_rem.tv_nsec);
    return;
  } else if (errno == EINVAL) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING,
                     "nanoseconds was not in the range 0 to 999 999 999 or "
                     "seconds was negative");
  }

  RETURN_FALSE;
}
/* }}} */

/* {{{ proto mixed time_sleep_until(float timestamp)
   Make the script sleep until the specified time */
PHP_FUNCTION(time_sleep_until) {
  double d_ts, c_ts;
  struct timeval tm;
  struct timespec php_req, php_rem;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &d_ts) == FAILURE) {
    return;
  }

  if (gettimeofday((struct timeval *)&tm, NULL) != 0) {
    RETURN_FALSE;
  }

  c_ts = (double)(d_ts - tm.tv_sec - tm.tv_usec / 1000000.00);
  if (c_ts < 0) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING,
                     "Sleep until to time is less than current time");
    RETURN_FALSE;
  }

  php_req.tv_sec = (time_t)c_ts;
  if (php_req.tv_sec > c_ts) { /* rounding up occurred */
    php_req.tv_sec--;
  }
  /* 1sec = 1000000000 nanoseconds */
  php_req.tv_nsec = (long)((c_ts - php_req.tv_sec) * 1000000000.00);

  while (nanosleep(&php_req, &php_rem)) {
    if (errno == EINTR) {
      php_req.tv_sec = php_rem.tv_sec;
      php_req.tv_nsec = php_rem.tv_nsec;
    } else {
      RETURN_FALSE;
    }
  }

  RETURN_TRUE;
}
/* }}} */
#endif

/* {{{ proto string get_current_user(void)
   Get the name of the owner of the current PHP script */
PHP_FUNCTION(get_current_user) {
  if (zend_parse_parameters_none() == FAILURE) {
    return;
  }

  RETURN_STRING(php_get_current_user(TSRMLS_C), 1);
}
/* }}} */

/* {{{ add_config_entry_cb
 */
static int add_config_entry_cb(zval *entry TSRMLS_DC, int num_args,
                               va_list args, zend_hash_key *hash_key) {
  zval *retval = (zval *)va_arg(args, zval *);
  zval *tmp;

  if (Z_TYPE_P(entry) == IS_STRING) {
    if (hash_key->nKeyLength > 0) {
      add_assoc_stringl_ex(retval, hash_key->arKey, hash_key->nKeyLength,
                           Z_STRVAL_P(entry), Z_STRLEN_P(entry), 1);
    } else {
      add_index_stringl(retval, hash_key->h, Z_STRVAL_P(entry),
                        Z_STRLEN_P(entry), 1);
    }
  } else if (Z_TYPE_P(entry) == IS_ARRAY) {
    MAKE_STD_ZVAL(tmp);
    array_init(tmp);
    zend_hash_apply_with_arguments(Z_ARRVAL_P(entry) TSRMLS_CC,
                                   (apply_func_args_t)add_config_entry_cb, 1,
                                   tmp);
    add_assoc_zval_ex(retval, hash_key->arKey, hash_key->nKeyLength, tmp);
  }
  return 0;
}
/* }}} */

/* {{{ proto mixed get_cfg_var(string option_name)
   Get the value of a PHP configuration option */
PHP_FUNCTION(get_cfg_var) {
  char *varname;
  int varname_len;
  zval *retval;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &varname,
                            &varname_len) == FAILURE) {
    return;
  }

  retval = cfg_get_entry(varname, varname_len + 1);

  if (retval) {
    if (Z_TYPE_P(retval) == IS_ARRAY) {
      array_init(return_value);
      zend_hash_apply_with_arguments(Z_ARRVAL_P(retval) TSRMLS_CC,
                                     (apply_func_args_t)add_config_entry_cb, 1,
                                     return_value);
      return;
    } else {
      RETURN_STRING(Z_STRVAL_P(retval), 1);
    }
  } else {
    RETURN_FALSE;
  }
}
/* }}} */

/* {{{ proto bool set_magic_quotes_runtime(int new_setting)
   Set the current active configuration setting of magic_quotes_runtime and
   return previous */
PHP_FUNCTION(set_magic_quotes_runtime) {
  zend_bool new_setting;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &new_setting) ==
      FAILURE) {
    return;
  }

  if (zend_alter_ini_entry_ex("magic_quotes_runtime",
                              sizeof("magic_quotes_runtime"),
                              new_setting ? "1" : "0", 1, PHP_INI_USER,
                              PHP_INI_STAGE_RUNTIME, 0 TSRMLS_CC) == FAILURE) {
    RETURN_FALSE;
  }
  RETURN_TRUE;
}
/* }}} */

/* {{{ proto int get_magic_quotes_runtime(void)
   Get the current active configuration setting of magic_quotes_runtime */
PHP_FUNCTION(get_magic_quotes_runtime) {
  RETURN_LONG(PG(magic_quotes_runtime));
}
/* }}} */

/* {{{ proto int get_magic_quotes_gpc(void)
   Get the current active configuration setting of magic_quotes_gpc */
PHP_FUNCTION(get_magic_quotes_gpc) { RETURN_LONG(PG(magic_quotes_gpc)); }
/* }}} */

/*
        1st arg = error message
        2nd arg = error option
        3rd arg = optional parameters (email address or tcp address)
        4th arg = used for additional headers if email

error options:
        0 = send to php_error_log (uses syslog or file depending on ini setting)
        1 = send via email to 3rd parameter 4th option = additional headers
        2 = send via tcp/ip to 3rd parameter (name or ip:port)
        3 = save to file in 3rd parameter
        4 = send to SAPI logger directly
*/

/* {{{ proto bool error_log(string message [, int message_type [, string
   destination [, string extra_headers]]]) Send an error message somewhere */
PHP_FUNCTION(error_log) {
  char *message, *opt = NULL, *headers = NULL;
  int message_len, opt_len = 0, headers_len = 0;
  int opt_err = 0, argc = ZEND_NUM_ARGS();
  long erropt = 0;

  if (zend_parse_parameters(argc TSRMLS_CC, "s|lss", &message, &message_len,
                            &erropt, &opt, &opt_len, &headers,
                            &headers_len) == FAILURE) {
    return;
  }

  if (argc > 1) {
    opt_err = erropt;
  }

  if (_php_error_log_ex(opt_err, message, message_len, opt,
                        headers TSRMLS_CC) == FAILURE) {
    RETURN_FALSE;
  }

  RETURN_TRUE;
}
/* }}} */

/* For BC (not binary-safe!) */
PHPAPI int _php_error_log(int opt_err, char *message, char *opt,
                          char *headers TSRMLS_DC) /* {{{ */
{
  return _php_error_log_ex(opt_err, message,
                           (opt_err == 3) ? strlen(message) : 0, opt,
                           headers TSRMLS_CC);
}
/* }}} */

PHPAPI int _php_error_log_ex(int opt_err, char *message, int message_len,
                             char *opt, char *headers TSRMLS_DC) /* {{{ */
{
  php_stream *stream = NULL;

  switch (opt_err) {
  case 1: /*send an email */
    if (!php_mail(opt, "PHP error_log message", message, headers,
                  NULL TSRMLS_CC)) {
      return FAILURE;
    }
    break;

  case 2: /*send to an address */
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "TCP/IP option not available!");
    return FAILURE;
    break;

  case 3: /*save to a file */
    stream =
        php_stream_open_wrapper(opt, "a", IGNORE_URL_WIN | REPORT_ERRORS, NULL);
    if (!stream) {
      return FAILURE;
    }
    php_stream_write(stream, message, message_len);
    php_stream_close(stream);
    break;

  case 4: /* send to SAPI */
    if (sapi_module.log_message) {
      sapi_module.log_message(message TSRMLS_CC);
    } else {
      return FAILURE;
    }
    break;

  default:
    php_log_err(message TSRMLS_CC);
    break;
  }
  return SUCCESS;
}
/* }}} */

/* {{{ proto array error_get_last()
   Get the last occurred error as associative array. Returns NULL if there
   hasn't been an error yet. */
PHP_FUNCTION(error_get_last) {
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
    return;
  }

  if (PG(last_error_message)) {
    array_init(return_value);
    add_assoc_long_ex(return_value, "type", sizeof("type"),
                      PG(last_error_type));
    add_assoc_string_ex(return_value, "message", sizeof("message"),
                        PG(last_error_message), 1);
    add_assoc_string_ex(return_value, "file", sizeof("file"),
                        PG(last_error_file) ? PG(last_error_file) : "-", 1);
    add_assoc_long_ex(return_value, "line", sizeof("line"),
                      PG(last_error_lineno));
  }
}
/* }}} */

/* {{{ proto mixed call_user_func(mixed function_name [, mixed parmeter] [,
   mixed ...]) Call a user function which is the first parameter */
PHP_FUNCTION(call_user_func) {
  zval *retval_ptr = NULL;
  zend_fcall_info fci;
  zend_fcall_info_cache fci_cache;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f*", &fci, &fci_cache,
                            &fci.params, &fci.param_count) == FAILURE) {
    return;
  }

  fci.retval_ptr_ptr = &retval_ptr;

  if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS &&
      fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
    COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
  }

  if (fci.params) {
    efree(fci.params);
  }
}
/* }}} */

/* {{{ proto mixed call_user_func_array(string function_name, array parameters)
   Call a user function which is the first parameter with the arguments
   contained in array */
PHP_FUNCTION(call_user_func_array) {
  zval *params, *retval_ptr = NULL;
  zend_fcall_info fci;
  zend_fcall_info_cache fci_cache;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "fa/", &fci, &fci_cache,
                            &params) == FAILURE) {
    return;
  }

  zend_fcall_info_args(&fci, params TSRMLS_CC);
  fci.retval_ptr_ptr = &retval_ptr;

  if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS &&
      fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
    COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
  }

  zend_fcall_info_args_clear(&fci, 1);
}
/* }}} */

/* {{{ proto mixed call_user_method(string method_name, mixed object [, mixed
   parameter] [, mixed ...]) Call a user method on a specific object or class */
PHP_FUNCTION(call_user_method) {
  zval ***params = NULL;
  int n_params = 0;
  zval *retval_ptr;
  zval *callback, *object;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/z*", &callback,
                            &object, &params, &n_params) == FAILURE) {
    return;
  }

  if (Z_TYPE_P(object) != IS_OBJECT && Z_TYPE_P(object) != IS_STRING) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING,
                     "Second argument is not an object or class name");
    efree(params);
    RETURN_FALSE;
  }

  convert_to_string(callback);

  if (call_user_function_ex(EG(function_table), &object, callback, &retval_ptr,
                            n_params, params, 0, NULL TSRMLS_CC) == SUCCESS) {
    if (retval_ptr) {
      COPY_PZVAL_TO_ZVAL(*return_value, retval_ptr);
    }
  } else {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call %s()",
                     Z_STRVAL_P(callback));
  }
  if (n_params) {
    efree(params);
  }
}
/* }}} */

/* {{{ proto mixed call_user_method_array(string method_name, mixed object,
   array params) Call a user method on a specific object or class using a
   parameter array */
PHP_FUNCTION(call_user_method_array) {
  zval *params, ***method_args = NULL, *retval_ptr;
  zval *callback, *object;
  HashTable *params_ar;
  int num_elems, element = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/zA/", &callback,
                            &object, &params) == FAILURE) {
    return;
  }

  if (Z_TYPE_P(object) != IS_OBJECT && Z_TYPE_P(object) != IS_STRING) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING,
                     "Second argument is not an object or class name");
    RETURN_FALSE;
  }

  convert_to_string(callback);

  params_ar = HASH_OF(params);
  num_elems = zend_hash_num_elements(params_ar);
  method_args = (zval ***)safe_emalloc(sizeof(zval **), num_elems, 0);

  for (zend_hash_internal_pointer_reset(params_ar);
       zend_hash_get_current_data(params_ar,
                                  (void **)&(method_args[element])) == SUCCESS;
       zend_hash_move_forward(params_ar)) {
    element++;
  }

  if (call_user_function_ex(EG(function_table), &object, callback, &retval_ptr,
                            num_elems, method_args, 0,
                            NULL TSRMLS_CC) == SUCCESS) {
    if (retval_ptr) {
      COPY_PZVAL_TO_ZVAL(*return_value, retval_ptr);
    }
  } else {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call %s()",
                     Z_STRVAL_P(callback));
  }

  efree(method_args);
}
/* }}} */

/* {{{ proto mixed forward_static_call(mixed function_name [, mixed parmeter] [,
   mixed ...]) U Call a user function which is the first parameter */
PHP_FUNCTION(forward_static_call) {
  zval *retval_ptr = NULL;
  zend_fcall_info fci;
  zend_fcall_info_cache fci_cache;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f*", &fci, &fci_cache,
                            &fci.params, &fci.param_count) == FAILURE) {
    return;
  }

  if (!EG(active_op_array)->scope) {
    zend_error(
        E_ERROR,
        "Cannot call forward_static_call() when no class scope is active");
  }

  fci.retval_ptr_ptr = &retval_ptr;

  if (EG(called_scope) &&
      instanceof_function(EG(called_scope),
                          fci_cache.calling_scope TSRMLS_CC)) {
    fci_cache.called_scope = EG(called_scope);
  }

  if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS &&
      fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
    COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
  }

  if (fci.params) {
    efree(fci.params);
  }
}
/* }}} */

/* {{{ proto mixed call_user_func_array(string function_name, array parameters)
   U Call a user function which is the first parameter with the arguments
   contained in array */
PHP_FUNCTION(forward_static_call_array) {
  zval *params, *retval_ptr = NULL;
  zend_fcall_info fci;
  zend_fcall_info_cache fci_cache;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "fa/", &fci, &fci_cache,
                            &params) == FAILURE) {
    return;
  }

  zend_fcall_info_args(&fci, params TSRMLS_CC);
  fci.retval_ptr_ptr = &retval_ptr;

  if (EG(called_scope) &&
      instanceof_function(EG(called_scope),
                          fci_cache.calling_scope TSRMLS_CC)) {
    fci_cache.called_scope = EG(called_scope);
  }

  if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS &&
      fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
    COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
  }

  zend_fcall_info_args_clear(&fci, 1);
}
/* }}} */

void user_shutdown_function_dtor(
    php_shutdown_function_entry *shutdown_function_entry) /* {{{ */
{
  int i;

  for (i = 0; i < shutdown_function_entry->arg_count; i++) {
    zval_ptr_dtor(&shutdown_function_entry->arguments[i]);
  }
  efree(shutdown_function_entry->arguments);
}
/* }}} */

void user_tick_function_dtor(
    user_tick_function_entry *tick_function_entry) /* {{{ */
{
  int i;

  for (i = 0; i < tick_function_entry->arg_count; i++) {
    zval_ptr_dtor(&tick_function_entry->arguments[i]);
  }
  efree(tick_function_entry->arguments);
}
/* }}} */

static int user_shutdown_function_call(
    php_shutdown_function_entry *shutdown_function_entry TSRMLS_DC) /* {{{ */
{
  zval retval;
  char *function_name;

  if (!zend_is_callable(shutdown_function_entry->arguments[0], 0,
                        &function_name TSRMLS_CC)) {
    php_error(E_WARNING,
              "(Registered shutdown functions) Unable to call %s() - function "
              "does not exist",
              function_name);
    if (function_name) {
      efree(function_name);
    }
    return 0;
  }
  if (function_name) {
    efree(function_name);
  }

  if (call_user_function(
          EG(function_table), NULL, shutdown_function_entry->arguments[0],
          &retval, shutdown_function_entry->arg_count - 1,
          shutdown_function_entry->arguments + 1 TSRMLS_CC) == SUCCESS) {
    zval_dtor(&retval);
  }
  return 0;
}
/* }}} */

static void
user_tick_function_call(user_tick_function_entry *tick_fe TSRMLS_DC) /* {{{ */
{
  zval retval;
  zval *function = tick_fe->arguments[0];

  /* Prevent reentrant calls to the same user ticks function */
  if (!tick_fe->calling) {
    tick_fe->calling = 1;

    if (call_user_function(EG(function_table), NULL, function, &retval,
                           tick_fe->arg_count - 1,
                           tick_fe->arguments + 1 TSRMLS_CC) == SUCCESS) {
      zval_dtor(&retval);

    } else {
      zval **obj, **method;

      if (Z_TYPE_P(function) == IS_STRING) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
                         "Unable to call %s() - function does not exist",
                         Z_STRVAL_P(function));
      } else if (Z_TYPE_P(function) == IS_ARRAY &&
                 zend_hash_index_find(Z_ARRVAL_P(function), 0, (void **)&obj) ==
                     SUCCESS &&
                 zend_hash_index_find(Z_ARRVAL_P(function), 1,
                                      (void **)&method) == SUCCESS &&
                 Z_TYPE_PP(obj) == IS_OBJECT &&
                 Z_TYPE_PP(method) == IS_STRING) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
                         "Unable to call %s::%s() - function does not exist",
                         Z_OBJCE_PP(obj)->name, Z_STRVAL_PP(method));
      } else {
        php_error_docref(NULL TSRMLS_CC, E_WARNING,
                         "Unable to call tick function");
      }
    }

    tick_fe->calling = 0;
  }
}
/* }}} */

static void run_user_tick_functions(int tick_count) /* {{{ */
{
  TSRMLS_FETCH();

  zend_llist_apply(BG(user_tick_functions),
                   (llist_apply_func_t)user_tick_function_call TSRMLS_CC);
}
/* }}} */

static int
user_tick_function_compare(user_tick_function_entry *tick_fe1,
                           user_tick_function_entry *tick_fe2) /* {{{ */
{
  zval *func1 = tick_fe1->arguments[0];
  zval *func2 = tick_fe2->arguments[0];
  int ret;
  TSRMLS_FETCH();

  if (Z_TYPE_P(func1) == IS_STRING && Z_TYPE_P(func2) == IS_STRING) {
    ret = (zend_binary_zval_strcmp(func1, func2) == 0);
  } else if (Z_TYPE_P(func1) == IS_ARRAY && Z_TYPE_P(func2) == IS_ARRAY) {
    zval result;
    zend_compare_arrays(&result, func1, func2 TSRMLS_CC);
    ret = (Z_LVAL(result) == 0);
  } else if (Z_TYPE_P(func1) == IS_OBJECT && Z_TYPE_P(func2) == IS_OBJECT) {
    zval result;
    zend_compare_objects(&result, func1, func2 TSRMLS_CC);
    ret = (Z_LVAL(result) == 0);
  } else {
    ret = 0;
  }

  if (ret && tick_fe1->calling) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING,
                     "Unable to delete tick function executed at the moment");
    return 0;
  }
  return ret;
}
/* }}} */

void php_call_shutdown_functions(TSRMLS_D) /* {{{ */
{
  if (BG(user_shutdown_function_names)) {
    zend_try {
      zend_hash_apply(BG(user_shutdown_function_names),
                      (apply_func_t)user_shutdown_function_call TSRMLS_CC);
    }
    zend_end_try();
    php_free_shutdown_functions(TSRMLS_C);
  }
}
/* }}} */

void php_free_shutdown_functions(TSRMLS_D) /* {{{ */
{
  if (BG(user_shutdown_function_names))
    zend_try {
      zend_hash_destroy(BG(user_shutdown_function_names));
      FREE_HASHTABLE(BG(user_shutdown_function_names));
      BG(user_shutdown_function_names) = NULL;
    }
  zend_end_try();
}
/* }}} */

/* {{{ proto void register_shutdown_function(string function_name) U
   Register a user-level function to be called on request termination */
PHP_FUNCTION(register_shutdown_function) {
  php_shutdown_function_entry shutdown_function_entry;
  char *function_name = NULL;
  int i;

  shutdown_function_entry.arg_count = ZEND_NUM_ARGS();

  if (shutdown_function_entry.arg_count < 1) {
    WRONG_PARAM_COUNT;
  }

  shutdown_function_entry.arguments = (zval **)safe_emalloc(
      sizeof(zval *), shutdown_function_entry.arg_count, 0);

  if (zend_get_parameters_array(ht, shutdown_function_entry.arg_count,
                                shutdown_function_entry.arguments) == FAILURE) {
    efree(shutdown_function_entry.arguments);
    RETURN_FALSE;
  }

  /* Prevent entering of anything but valid callback (syntax check only!) */
  if (!zend_is_callable(shutdown_function_entry.arguments[0], 0,
                        &function_name TSRMLS_CC)) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING,
                     "Invalid shutdown callback '%s' passed", function_name);
    efree(shutdown_function_entry.arguments);
    RETVAL_FALSE;
  } else {
    if (!BG(user_shutdown_function_names)) {
      ALLOC_HASHTABLE(BG(user_shutdown_function_names));
      zend_hash_init(BG(user_shutdown_function_names), 0, NULL,
                     (void (*)(void *))user_shutdown_function_dtor, 0);
    }

    for (i = 0; i < shutdown_function_entry.arg_count; i++) {
      Z_ADDREF_P(shutdown_function_entry.arguments[i]);
    }
    zend_hash_next_index_insert(BG(user_shutdown_function_names),
                                &shutdown_function_entry,
                                sizeof(php_shutdown_function_entry), NULL);
  }
  if (function_name) {
    efree(function_name);
  }
}
/* }}} */

ZEND_API void php_get_highlight_struct(
    zend_syntax_highlighter_ini *syntax_highlighter_ini) /* {{{ */
{
  syntax_highlighter_ini->highlight_comment = INI_STR("highlight.comment");
  syntax_highlighter_ini->highlight_default = INI_STR("highlight.default");
  syntax_highlighter_ini->highlight_html = INI_STR("highlight.html");
  syntax_highlighter_ini->highlight_keyword = INI_STR("highlight.keyword");
  syntax_highlighter_ini->highlight_string = INI_STR("highlight.string");
}
/* }}} */

/* {{{ proto bool highlight_file(string file_name [, bool return] )
   Syntax highlight a source file */
PHP_FUNCTION(highlight_file) {
  char *filename;
  int filename_len, ret;
  zend_syntax_highlighter_ini syntax_highlighter_ini;
  zend_bool i = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &filename,
                            &filename_len, &i) == FAILURE) {
    RETURN_FALSE;
  }

  if (php_check_open_basedir(filename TSRMLS_CC)) {
    RETURN_FALSE;
  }

  if (i) {
    php_output_start_default(TSRMLS_C);
  }

  php_get_highlight_struct(&syntax_highlighter_ini);

  ret = highlight_file(filename, &syntax_highlighter_ini TSRMLS_CC);

  if (ret == FAILURE) {
    if (i) {
      php_output_end(TSRMLS_C);
    }
    RETURN_FALSE;
  }

  if (i) {
    php_output_get_contents(return_value TSRMLS_CC);
    php_output_discard(TSRMLS_C);
  } else {
    RETURN_TRUE;
  }
}
/* }}} */

/* {{{ proto string php_strip_whitespace(string file_name)
   Return source with stripped comments and whitespace */
PHP_FUNCTION(php_strip_whitespace) {
  char *filename;
  int filename_len;
  zend_lex_state original_lex_state;
  zend_file_handle file_handle = {0};

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename,
                            &filename_len) == FAILURE) {
    RETURN_FALSE;
  }

  php_output_start_default(TSRMLS_C);

  file_handle.type = ZEND_HANDLE_FILENAME;
  file_handle.filename = filename;
  file_handle.free_filename = 0;
  file_handle.opened_path = NULL;
  zend_save_lexical_state(&original_lex_state TSRMLS_CC);
  if (open_file_for_scanning(&file_handle TSRMLS_CC) == FAILURE) {
    zend_restore_lexical_state(&original_lex_state TSRMLS_CC);
    php_output_end(TSRMLS_C);
    RETURN_EMPTY_STRING();
  }

  zend_strip(TSRMLS_C);

  zend_destroy_file_handle(&file_handle TSRMLS_CC);
  zend_restore_lexical_state(&original_lex_state TSRMLS_CC);

  php_output_get_contents(return_value TSRMLS_CC);
  php_output_discard(TSRMLS_C);
}
/* }}} */

/* {{{ proto bool highlight_string(string string [, bool return] )
   Syntax highlight a string or optionally return it */
PHP_FUNCTION(highlight_string) {
  zval **expr;
  zend_syntax_highlighter_ini syntax_highlighter_ini;
  char *hicompiled_string_description;
  zend_bool i = 0;
  int old_error_reporting = EG(error_reporting);

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Z|b", &expr, &i) ==
      FAILURE) {
    RETURN_FALSE;
  }
  convert_to_string_ex(expr);

  if (i) {
    php_output_start_default(TSRMLS_C);
  }

  EG(error_reporting) = E_ERROR;

  php_get_highlight_struct(&syntax_highlighter_ini);

  hicompiled_string_description =
      zend_make_compiled_string_description("highlighted code" TSRMLS_CC);

  if (highlight_string(*expr, &syntax_highlighter_ini,
                       hicompiled_string_description TSRMLS_CC) == FAILURE) {
    efree(hicompiled_string_description);
    EG(error_reporting) = old_error_reporting;
    if (i) {
      php_output_end(TSRMLS_C);
    }
    RETURN_FALSE;
  }
  efree(hicompiled_string_description);

  EG(error_reporting) = old_error_reporting;

  if (i) {
    php_output_get_contents(return_value TSRMLS_CC);
    php_output_discard(TSRMLS_C);
  } else {
    RETURN_TRUE;
  }
}
/* }}} */

/* {{{ proto string ini_get(string varname)
   Get a configuration option */
PHP_FUNCTION(ini_get) {
  char *varname, *str;
  int varname_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &varname,
                            &varname_len) == FAILURE) {
    return;
  }

  str = zend_ini_string(varname, varname_len + 1, 0);

  if (!str) {
    RETURN_FALSE;
  }

  RETURN_STRING(str, 1);
}
/* }}} */

static int php_ini_get_option(zend_ini_entry *ini_entry TSRMLS_DC, int num_args,
                              va_list args, zend_hash_key *hash_key) /* {{{ */
{
  zval *ini_array = va_arg(args, zval *);
  int module_number = va_arg(args, int);
  int details = va_arg(args, int);
  zval *option;

  if (module_number != 0 && ini_entry->module_number != module_number) {
    return 0;
  }

  if (hash_key->nKeyLength == 0 || hash_key->arKey[0] != 0) {
    if (details) {
      MAKE_STD_ZVAL(option);
      array_init(option);

      if (ini_entry->orig_value) {
        add_assoc_stringl(option, "global_value", ini_entry->orig_value,
                          ini_entry->orig_value_length, 1);
      } else if (ini_entry->value) {
        add_assoc_stringl(option, "global_value", ini_entry->value,
                          ini_entry->value_length, 1);
      } else {
        add_assoc_null(option, "global_value");
      }

      if (ini_entry->value) {
        add_assoc_stringl(option, "local_value", ini_entry->value,
                          ini_entry->value_length, 1);
      } else {
        add_assoc_null(option, "local_value");
      }

      add_assoc_long(option, "access", ini_entry->modifiable);

      add_assoc_zval_ex(ini_array, ini_entry->name, ini_entry->name_length,
                        option);
    } else {
      if (ini_entry->value) {
        add_assoc_stringl(ini_array, ini_entry->name, ini_entry->value,
                          ini_entry->value_length, 1);
      } else {
        add_assoc_null(ini_array, ini_entry->name);
      }
    }
  }
  return 0;
}
/* }}} */

/* {{{ proto array ini_get_all([string extension[, bool details = true]])
   Get all configuration options */
PHP_FUNCTION(ini_get_all) {
  char *extname = NULL;
  int extname_len = 0, extnumber = 0;
  zend_module_entry *module;
  zend_bool details = 1;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s!b", &extname,
                            &extname_len, &details) == FAILURE) {
    return;
  }

  zend_ini_sort_entries(TSRMLS_C);

  if (extname) {
    if (zend_hash_find(&module_registry, extname, extname_len + 1,
                       (void **)&module) == FAILURE) {
      php_error_docref(NULL TSRMLS_CC, E_WARNING,
                       "Unable to find extension '%s'", extname);
      RETURN_FALSE;
    }
    extnumber = module->module_number;
  }

  array_init(return_value);
  zend_hash_apply_with_arguments(EG(ini_directives) TSRMLS_CC,
                                 (apply_func_args_t)php_ini_get_option, 2,
                                 return_value, extnumber, details);
}
/* }}} */

static int php_ini_check_path(char *option_name, int option_len,
                              char *new_option_name,
                              int new_option_len) /* {{{ */
{
  if (option_len != (new_option_len - 1)) {
    return 0;
  }

  return !strncmp(option_name, new_option_name, option_len);
}
/* }}} */

/* {{{ proto string ini_set(string varname, string newvalue)
   Set a configuration option, returns false on error and the old value of the
   configuration option on success */
PHP_FUNCTION(ini_set) {
  char *varname, *new_value;
  int varname_len, new_value_len;
  char *old_value;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &varname,
                            &varname_len, &new_value,
                            &new_value_len) == FAILURE) {
    return;
  }

  old_value = zend_ini_string(varname, varname_len + 1, 0);

  /* copy to return here, because alter might free it! */
  if (old_value) {
    RETVAL_STRING(old_value, 1);
  } else {
    RETVAL_FALSE;
  }

#define _CHECK_PATH(var, var_len, ini)                                         \
  php_ini_check_path(var, var_len, ini, sizeof(ini))
  /* open basedir check */
  if (PG(open_basedir)) {
    if (_CHECK_PATH(varname, varname_len, "error_log") ||
        _CHECK_PATH(varname, varname_len, "java.class.path") ||
        _CHECK_PATH(varname, varname_len, "java.home") ||
        _CHECK_PATH(varname, varname_len, "mail.log") ||
        _CHECK_PATH(varname, varname_len, "java.library.path") ||
        _CHECK_PATH(varname, varname_len, "vpopmail.directory")) {
      if (php_check_open_basedir(new_value TSRMLS_CC)) {
        zval_dtor(return_value);
        RETURN_FALSE;
      }
    }
  }

  if (zend_alter_ini_entry_ex(varname, varname_len + 1, new_value,
                              new_value_len, PHP_INI_USER,
                              PHP_INI_STAGE_RUNTIME, 0 TSRMLS_CC) == FAILURE) {
    zval_dtor(return_value);
    RETURN_FALSE;
  }
}
/* }}} */

/* {{{ proto void ini_restore(string varname)
   Restore the value of a configuration option specified by varname */
PHP_FUNCTION(ini_restore) {
  char *varname;
  int varname_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &varname,
                            &varname_len) == FAILURE) {
    return;
  }

  zend_restore_ini_entry(varname, varname_len + 1, PHP_INI_STAGE_RUNTIME);
}
/* }}} */

/* {{{ proto string set_include_path(string new_include_path)
   Sets the include_path configuration option */
PHP_FUNCTION(set_include_path) {
  char *new_value;
  int new_value_len;
  char *old_value;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &new_value,
                            &new_value_len) == FAILURE) {
    return;
  }

  old_value = zend_ini_string("include_path", sizeof("include_path"), 0);
  /* copy to return here, because alter might free it! */
  if (old_value) {
    RETVAL_STRING(old_value, 1);
  } else {
    RETVAL_FALSE;
  }

  if (zend_alter_ini_entry_ex("include_path", sizeof("include_path"), new_value,
                              new_value_len, PHP_INI_USER,
                              PHP_INI_STAGE_RUNTIME, 0 TSRMLS_CC) == FAILURE) {
    zval_dtor(return_value);
    RETURN_FALSE;
  }
}
/* }}} */

/* {{{ proto string get_include_path()
   Get the current include_path configuration option */
PHP_FUNCTION(get_include_path) {
  char *str;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
    return;
  }

  str = zend_ini_string("include_path", sizeof("include_path"), 0);

  if (str == NULL) {
    RETURN_FALSE;
  }

  RETURN_STRING(str, 1);
}
/* }}} */

/* {{{ proto void restore_include_path()
   Restore the value of the include_path configuration option */
PHP_FUNCTION(restore_include_path) {
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
    return;
  }
  zend_restore_ini_entry("include_path", sizeof("include_path"),
                         PHP_INI_STAGE_RUNTIME);
}
/* }}} */

/* {{{ proto mixed print_r(mixed var [, bool return])
   Prints out or returns information about the specified variable */
PHP_FUNCTION(print_r) {
  zval *var;
  zend_bool do_return = 0;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|b", &var,
                            &do_return) == FAILURE) {
    RETURN_FALSE;
  }

  if (do_return) {
    php_output_start_default(TSRMLS_C);
  }

  zend_print_zval_r(var, 0 TSRMLS_CC);

  if (do_return) {
    php_output_get_contents(return_value TSRMLS_CC);
    php_output_discard(TSRMLS_C);
  } else {
    RETURN_TRUE;
  }
}
/* }}} */

/* {{{ proto int connection_aborted(void)
   Returns true if client disconnected */
PHP_FUNCTION(connection_aborted) {
  RETURN_LONG(PG(connection_status) & PHP_CONNECTION_ABORTED);
}
/* }}} */

/* {{{ proto int connection_status(void)
   Returns the connection status bitfield */
PHP_FUNCTION(connection_status) { RETURN_LONG(PG(connection_status)); }
/* }}} */

/* {{{ proto int ignore_user_abort([string value])
   Set whether we want to ignore a user abort event or not */
PHP_FUNCTION(ignore_user_abort) {
  char *arg = NULL;
  int arg_len = 0;
  int old_setting;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &arg, &arg_len) ==
      FAILURE) {
    return;
  }

  old_setting = PG(ignore_user_abort);

  if (arg) {
    zend_alter_ini_entry_ex("ignore_user_abort", sizeof("ignore_user_abort"),
                            arg, arg_len, PHP_INI_USER, PHP_INI_STAGE_RUNTIME,
                            0 TSRMLS_CC);
  }

  RETURN_LONG(old_setting);
}
/* }}} */

#if HAVE_GETSERVBYNAME
/* {{{ proto int getservbyname(string service, string protocol)
   Returns port associated with service. Protocol must be "tcp" or "udp" */
PHP_FUNCTION(getservbyname) {
  char *name, *proto;
  int name_len, proto_len;
  struct servent *serv;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &name, &name_len,
                            &proto, &proto_len) == FAILURE) {
    return;
  }

/* empty string behaves like NULL on windows implementation of
   getservbyname. Let be portable instead. */
#ifdef PHP_WIN32
  if (proto_len == 0) {
    RETURN_FALSE;
  }
#endif

  serv = getservbyname(name, proto);

  if (serv == NULL) {
    RETURN_FALSE;
  }

  RETURN_LONG(ntohs(serv->s_port));
}
/* }}} */
#endif

#if HAVE_GETSERVBYPORT
/* {{{ proto string getservbyport(int port, string protocol)
   Returns service name associated with port. Protocol must be "tcp" or "udp" */
PHP_FUNCTION(getservbyport) {
  char *proto;
  int proto_len;
  long port;
  struct servent *serv;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &port, &proto,
                            &proto_len) == FAILURE) {
    return;
  }

  serv = getservbyport(htons((unsigned short)port), proto);

  if (serv == NULL) {
    RETURN_FALSE;
  }

  RETURN_STRING(serv->s_name, 1);
}
/* }}} */
#endif

#if HAVE_GETPROTOBYNAME
/* {{{ proto int getprotobyname(string name)
   Returns protocol number associated with name as per /etc/protocols */
PHP_FUNCTION(getprotobyname) {
  char *name;
  int name_len;
  struct protoent *ent;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) ==
      FAILURE) {
    return;
  }

  ent = getprotobyname(name);

  if (ent == NULL) {
    RETURN_FALSE;
  }

  RETURN_LONG(ent->p_proto);
}
/* }}} */
#endif

#if HAVE_GETPROTOBYNUMBER
/* {{{ proto string getprotobynumber(int proto)
   Returns protocol name associated with protocol number proto */
PHP_FUNCTION(getprotobynumber) {
  long proto;
  struct protoent *ent;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &proto) ==
      FAILURE) {
    return;
  }

  ent = getprotobynumber(proto);

  if (ent == NULL) {
    RETURN_FALSE;
  }

  RETURN_STRING(ent->p_name, 1);
}
/* }}} */
#endif

/* {{{ proto bool register_tick_function(string function_name [, mixed arg [,
   mixed ... ]]) Registers a tick callback function */
PHP_FUNCTION(register_tick_function) {
  user_tick_function_entry tick_fe;
  int i;
  char *function_name = NULL;

  tick_fe.calling = 0;
  tick_fe.arg_count = ZEND_NUM_ARGS();

  if (tick_fe.arg_count < 1) {
    WRONG_PARAM_COUNT;
  }

  tick_fe.arguments =
      (zval **)safe_emalloc(sizeof(zval *), tick_fe.arg_count, 0);

  if (zend_get_parameters_array(ht, tick_fe.arg_count, tick_fe.arguments) ==
      FAILURE) {
    efree(tick_fe.arguments);
    RETURN_FALSE;
  }

  if (!zend_is_callable(tick_fe.arguments[0], 0, &function_name TSRMLS_CC)) {
    efree(tick_fe.arguments);
    php_error_docref(NULL TSRMLS_CC, E_WARNING,
                     "Invalid tick callback '%s' passed", function_name);
    efree(function_name);
    RETURN_FALSE;
  } else if (function_name) {
    efree(function_name);
  }

  if (Z_TYPE_P(tick_fe.arguments[0]) != IS_ARRAY &&
      Z_TYPE_P(tick_fe.arguments[0]) != IS_OBJECT) {
    convert_to_string_ex(&tick_fe.arguments[0]);
  }

  if (!BG(user_tick_functions)) {
    BG(user_tick_functions) = (zend_llist *)emalloc(sizeof(zend_llist));
    zend_llist_init(BG(user_tick_functions), sizeof(user_tick_function_entry),
                    (llist_dtor_func_t)user_tick_function_dtor, 0);
    php_add_tick_function(run_user_tick_functions);
  }

  for (i = 0; i < tick_fe.arg_count; i++) {
    Z_ADDREF_P(tick_fe.arguments[i]);
  }

  zend_llist_add_element(BG(user_tick_functions), &tick_fe);

  RETURN_TRUE;
}
/* }}} */

/* {{{ proto void unregister_tick_function(string function_name)
   Unregisters a tick callback function */
PHP_FUNCTION(unregister_tick_function) {
  zval *function;
  user_tick_function_entry tick_fe;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/", &function) ==
      FAILURE) {
    return;
  }

  if (!BG(user_tick_functions)) {
    return;
  }

  if (Z_TYPE_P(function) != IS_ARRAY) {
    convert_to_string(function);
  }

  tick_fe.arguments = (zval **)emalloc(sizeof(zval *));
  tick_fe.arguments[0] = function;
  tick_fe.arg_count = 1;
  zend_llist_del_element(BG(user_tick_functions), &tick_fe,
                         (int (*)(void *, void *))user_tick_function_compare);
  efree(tick_fe.arguments);
}
/* }}} */

/* {{{ proto bool is_uploaded_file(string path)
   Check if file was created by rfc1867 upload */
PHP_FUNCTION(is_uploaded_file) {
  char *path;
  int path_len;

  if (!SG(rfc1867_uploaded_files)) {
    RETURN_FALSE;
  }

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) ==
      FAILURE) {
    return;
  }

  if (zend_hash_exists(SG(rfc1867_uploaded_files), path, path_len + 1)) {
    RETURN_TRUE;
  } else {
    RETURN_FALSE;
  }
}
/* }}} */

/* {{{ proto bool move_uploaded_file(string path, string new_path)
   Move a file if and only if it was created by an upload */
PHP_FUNCTION(move_uploaded_file) {
  char *path, *new_path;
  int path_len, new_path_len;
  zend_bool successful = 0;

#ifndef PHP_WIN32
  int oldmask;
  int ret;
#endif

  if (!SG(rfc1867_uploaded_files)) {
    RETURN_FALSE;
  }

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &path, &path_len,
                            &new_path, &new_path_len) == FAILURE) {
    return;
  }

  if (!zend_hash_exists(SG(rfc1867_uploaded_files), path, path_len + 1)) {
    RETURN_FALSE;
  }

  if (php_check_open_basedir(new_path TSRMLS_CC)) {
    RETURN_FALSE;
  }

  VCWD_UNLINK(new_path);
  if (VCWD_RENAME(path, new_path) == 0) {
    successful = 1;
#ifndef PHP_WIN32
    oldmask = umask(077);
    umask(oldmask);

    ret = VCWD_CHMOD(new_path, 0666 & ~oldmask);

    if (ret == -1) {
      php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s", strerror(errno));
    }
#endif
  } else if (php_copy_file_ex(path, new_path,
                              STREAM_DISABLE_OPEN_BASEDIR TSRMLS_CC) ==
             SUCCESS) {
    VCWD_UNLINK(path);
    successful = 1;
  }

  if (successful) {
    zend_hash_del(SG(rfc1867_uploaded_files), path, path_len + 1);
  } else {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to move '%s' to '%s'",
                     path, new_path);
  }

  RETURN_BOOL(successful);
}
/* }}} */

/* {{{ php_simple_ini_parser_cb
 */
static void php_simple_ini_parser_cb(zval *arg1, zval *arg2, zval *arg3,
                                     int callback_type, zval *arr TSRMLS_DC) {
  zval *element;

  switch (callback_type) {

  case ZEND_INI_PARSER_ENTRY:
    if (!arg2) {
      /* bare string - nothing to do */
      break;
    }
    ALLOC_ZVAL(element);
    MAKE_COPY_ZVAL(&arg2, element);
    zend_symtable_update(Z_ARRVAL_P(arr), Z_STRVAL_P(arg1),
                         Z_STRLEN_P(arg1) + 1, &element, sizeof(zval *), NULL);
    break;

  case ZEND_INI_PARSER_POP_ENTRY: {
    zval *hash, **find_hash;

    if (!arg2) {
      /* bare string - nothing to do */
      break;
    }

    if (!(Z_STRLEN_P(arg1) > 1 && Z_STRVAL_P(arg1)[0] == '0') &&
        is_numeric_string(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1), NULL, NULL, 0) ==
            IS_LONG) {
      ulong key = (ulong)zend_atol(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1));
      // prophet generated patch
      switch (__choose("__ID85")) {
      case 0: {
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#ifdef COMPILE_15739
      case 1: {
        ((arg1))->is_ref__gc = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15740
      case 2: {
        ((arg1))->refcount__gc = 1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15741
      case 3: {
        ((arg2))->is_ref__gc = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15742
      case 4: {
        ((arg2))->refcount__gc = 1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15743
      case 5: {
        ((arg3))->is_ref__gc = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15744
      case 6: {
        ((arg3))->refcount__gc = 1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15745
      case 7: {
        ((arr))->is_ref__gc = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15746
      case 8: {
        ((arr))->refcount__gc = 1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15747
      case 9: {
        ((basic_globals.active_ini_file_section))->is_ref__gc = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15748
      case 10: {
        ((basic_globals.active_ini_file_section))->refcount__gc = 1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15749
      case 11: {
        ((basic_globals.user_shutdown_function_names)) =
            (HashTable *)_emalloc((sizeof(HashTable)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15750
      case 12: {
        ((element))->is_ref__gc = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15751
      case 13: {
        ((element))->refcount__gc = 1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15752
      case 14: {
        ((hash))->is_ref__gc = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15753
      case 15: {
        ((hash))->refcount__gc = 1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15754
      case 16: {
        (arg1) = *find_hash;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15755
      case 17: {
        (arg2) = *find_hash;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15756
      case 18: {
        (arg3) = *find_hash;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15757
      case 19: {
        (arr) = *find_hash;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15758
      case 20: {
        (basic_globals.active_ini_file_section) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15759
      case 21: {
        (basic_globals.array_walk_fci) = empty_fcall_info;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15760
      case 22: {
        (basic_globals.array_walk_fci_cache) = empty_fcall_info_cache;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15761
      case 23: {
        (basic_globals.incomplete_class) = incomplete_class_entry;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15762
      case 24: {
        (basic_globals.incomplete_class) = php_create_incomplete_class();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15763
      case 25: {
        (basic_globals.left) = -1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15764
      case 26: {
        (basic_globals.locale_string) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15765
      case 27: {
        (basic_globals.mt_rand_is_seeded) = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15766
      case 28: {
        (basic_globals.next) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15767
      case 29: {
        (basic_globals.page_gid) = -1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15768
      case 30: {
        (basic_globals.page_inode) = -1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15769
      case 31: {
        (basic_globals.page_mtime) = -1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15770
      case 32: {
        (basic_globals.page_uid) = -1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15771
      case 33: {
        (basic_globals.rand_is_seeded) = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15772
      case 34: {
        (basic_globals.strtok_last) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15773
      case 35: {
        (basic_globals.strtok_string) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15774
      case 36: {
        (basic_globals.strtok_zval) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15775
      case 37: {
        (basic_globals.umask) = -1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15776
      case 38: {
        (basic_globals.user_compare_fci) = empty_fcall_info;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15777
      case 39: {
        (basic_globals.user_compare_fci_cache) = empty_fcall_info_cache;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15778
      case 40: {
        (basic_globals.user_filter_map) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15779
      case 41: {
        (basic_globals.user_shutdown_function_names) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15780
      case 42: {
        (basic_globals.user_tick_functions) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15781
      case 43: {
        (basic_globals.user_tick_functions) =
            (zend_llist *)_emalloc((sizeof(zend_llist)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15782
      case 44: {
        (element) = *find_hash;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15783
      case 45: {
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15784
      case 46: {
        (executor_globals.error_reporting) = (1 << 0L);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15785
      case 47: {
        (file_globals.default_context) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15786
      case 48: {
        (file_globals.stream_filters) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15787
      case 49: {
        (file_globals.stream_wrappers) = ((void *)0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15788
      case 50: {
        (hash) = *find_hash;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15789
      case 51: {
        (hash)->is_ref__gc = 0;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15790
      case 52: {
        (hash)->refcount__gc = 1;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15791
      case 53: {
        _array_init(((arg1)), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15792
      case 54: {
        _array_init(((arg2)), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15793
      case 55: {
        _array_init(((arg3)), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15794
      case 56: {
        _array_init(((arr)), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15795
      case 57: {
        _array_init(((basic_globals.active_ini_file_section)), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15796
      case 58: {
        _array_init(((element)), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15797
      case 59: {
        _array_init(((hash)), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15798
      case 60: {
        _array_init((hash), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15799
      case 61: {
        _convert_to_string(((arg1)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15800
      case 62: {
        _convert_to_string(((arg2)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15801
      case 63: {
        _convert_to_string(((arg3)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15802
      case 64: {
        _convert_to_string(((arr)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15803
      case 65: {
        _convert_to_string(((element)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15804
      case 66: {
        _convert_to_string(((hash)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15805
      case 67: {
        _convert_to_string((*(find_hash)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15806
      case 68: {
        _efree(((*(arg1)).value.ht));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15807
      case 69: {
        _efree(((*(arg2)).value.ht));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15808
      case 70: {
        _efree(((*(arg3)).value.ht));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15809
      case 71: {
        _efree(((*(arr)).value.ht));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15810
      case 72: {
        _efree(((*(element)).value.ht));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15811
      case 73: {
        _efree(((*(hash)).value.ht));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15812
      case 74: {
        _efree(((arg1)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15813
      case 75: {
        _efree(((arg2)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15814
      case 76: {
        _efree(((arg3)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15815
      case 77: {
        _efree(((arr)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15816
      case 78: {
        _efree(((basic_globals.locale_string)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15817
      case 79: {
        _efree(((basic_globals.user_shutdown_function_names)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15818
      case 80: {
        _efree(((basic_globals.user_tick_functions)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15819
      case 81: {
        _efree(((element)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15820
      case 82: {
        _efree(((hash)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15821
      case 83: {
        _zend_hash_add_or_update((*(arg1)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15822
      case 84: {
        _zend_hash_add_or_update((*(arg2)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15823
      case 85: {
        _zend_hash_add_or_update((*(arg3)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15824
      case 86: {
        _zend_hash_add_or_update((*(arr)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15825
      case 87: {
        _zend_hash_add_or_update((*(element)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15826
      case 88: {
        _zend_hash_add_or_update((*(hash)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15827
      case 89: {
        _zend_hash_add_or_update((*arr).value.ht, (*(arg1)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15828
      case 90: {
        _zend_hash_add_or_update((*arr).value.ht, (*(arg2)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15829
      case 91: {
        _zend_hash_add_or_update((*arr).value.ht, (*(arg3)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15830
      case 92: {
        _zend_hash_add_or_update((*arr).value.ht, (*(arr)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15831
      case 93: {
        _zend_hash_add_or_update((*arr).value.ht, (*(element)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15832
      case 94: {
        _zend_hash_add_or_update((*arr).value.ht, (*(hash)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15833
      case 95: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(arg1)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15834
      case 96: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(arg2)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15835
      case 97: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(arg3)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15836
      case 98: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(arr)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15837
      case 99: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(element)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15838
      case 100: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(hash)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15839
      case 101: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(arg1),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15840
      case 102: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(arg2),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15841
      case 103: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(arg3),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15842
      case 104: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(arr),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15843
      case 105: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(element),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15844
      case 106: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(hash),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15845
      case 107: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15846
      case 108: {
        _zend_hash_index_update_or_next_insert((*(arg1)).value.ht, key, &hash,
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15847
      case 109: {
        _zend_hash_index_update_or_next_insert((*(arg2)).value.ht, key, &hash,
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15848
      case 110: {
        _zend_hash_index_update_or_next_insert((*(arg3)).value.ht, key, &hash,
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15849
      case 111: {
        _zend_hash_index_update_or_next_insert((*(arr)).value.ht, key, &hash,
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15850
      case 112: {
        _zend_hash_index_update_or_next_insert((*(element)).value.ht, key,
                                               &hash, sizeof(zval *),
                                               ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15851
      case 113: {
        _zend_hash_index_update_or_next_insert((*(hash)).value.ht, key, &hash,
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15852
      case 114: {
        _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &(arg1),
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15853
      case 115: {
        _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &(arg2),
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15854
      case 116: {
        _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &(arg3),
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15855
      case 117: {
        _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &(arr),
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15856
      case 118: {
        _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &(element),
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15857
      case 119: {
        _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &(hash),
                                               sizeof(zval *), ((void *)0),
                                               (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15858
      case 120: {
        _zend_hash_index_update_or_next_insert(
            (*arr).value.ht, key, &hash, sizeof(zval *), ((void *)0), (1 << 0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15859
      case 121: {
        _zend_hash_init(((basic_globals.user_shutdown_function_names)), (0),
                        (((void *)0)),
                        ((void (*)(void *))user_shutdown_function_dtor), (0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15860
      case 122: {
        _zval_copy_ctor((&(*(arg1))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15861
      case 123: {
        _zval_copy_ctor((&(*(arg2))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15862
      case 124: {
        _zval_copy_ctor((&(*(arg3))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15863
      case 125: {
        _zval_copy_ctor((&(*(arr))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15864
      case 126: {
        _zval_copy_ctor((&(*(element))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15865
      case 127: {
        _zval_copy_ctor((&(*(hash))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15866
      case 128: {
        _zval_copy_ctor((((arg1))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15867
      case 129: {
        _zval_copy_ctor((((arg2))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15868
      case 130: {
        _zval_copy_ctor((((arg3))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15869
      case 131: {
        _zval_copy_ctor((((arr))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15870
      case 132: {
        _zval_copy_ctor((((element))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15871
      case 133: {
        _zval_copy_ctor((((hash))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15872
      case 134: {
        _zval_copy_ctor(((element)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15873
      case 135: {
        _zval_copy_ctor_func((arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15874
      case 136: {
        _zval_copy_ctor_func((arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15875
      case 137: {
        _zval_copy_ctor_func((arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15876
      case 138: {
        _zval_copy_ctor_func((arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15877
      case 139: {
        _zval_copy_ctor_func((element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15878
      case 140: {
        _zval_copy_ctor_func((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15879
      case 141: {
        _zval_dtor(((arg1)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15880
      case 142: {
        _zval_dtor(((arg2)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15881
      case 143: {
        _zval_dtor(((arg3)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15882
      case 144: {
        _zval_dtor(((arr)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15883
      case 145: {
        _zval_dtor(((element)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15884
      case 146: {
        _zval_dtor(((hash)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15885
      case 147: {
        _zval_dtor((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15886
      case 148: {
        _zval_dtor_func((arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15887
      case 149: {
        _zval_dtor_func((arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15888
      case 150: {
        _zval_dtor_func((arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15889
      case 151: {
        _zval_dtor_func((arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15890
      case 152: {
        _zval_dtor_func((element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15891
      case 153: {
        _zval_dtor_func((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15892
      case 154: {
        _zval_ptr_dtor((&(arg1)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15893
      case 155: {
        _zval_ptr_dtor((&(arg2)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15894
      case 156: {
        _zval_ptr_dtor((&(arg3)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15895
      case 157: {
        _zval_ptr_dtor((&(arr)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15896
      case 158: {
        _zval_ptr_dtor((&(basic_globals.strtok_zval)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15897
      case 159: {
        _zval_ptr_dtor((&(element)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15898
      case 160: {
        _zval_ptr_dtor((&(hash)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15899
      case 161: {
        add_assoc_long_ex((arg1), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15900
      case 162: {
        add_assoc_long_ex((arg1), "type", sizeof("type"),
                          (core_globals.last_error_type));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15901
      case 163: {
        add_assoc_long_ex((arg2), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15902
      case 164: {
        add_assoc_long_ex((arg2), "type", sizeof("type"),
                          (core_globals.last_error_type));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15903
      case 165: {
        add_assoc_long_ex((arg3), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15904
      case 166: {
        add_assoc_long_ex((arg3), "type", sizeof("type"),
                          (core_globals.last_error_type));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15905
      case 167: {
        add_assoc_long_ex((arr), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15906
      case 168: {
        add_assoc_long_ex((arr), "type", sizeof("type"),
                          (core_globals.last_error_type));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15907
      case 169: {
        add_assoc_long_ex((element), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15908
      case 170: {
        add_assoc_long_ex((element), "type", sizeof("type"),
                          (core_globals.last_error_type));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15909
      case 171: {
        add_assoc_long_ex((hash), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15910
      case 172: {
        add_assoc_long_ex((hash), "type", sizeof("type"),
                          (core_globals.last_error_type));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15911
      case 173: {
        add_assoc_null_ex((arg1), "global_value", strlen("global_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15912
      case 174: {
        add_assoc_null_ex((arg1), "local_value", strlen("local_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15913
      case 175: {
        add_assoc_null_ex((arg2), "global_value", strlen("global_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15914
      case 176: {
        add_assoc_null_ex((arg2), "local_value", strlen("local_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15915
      case 177: {
        add_assoc_null_ex((arg3), "global_value", strlen("global_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15916
      case 178: {
        add_assoc_null_ex((arg3), "local_value", strlen("local_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15917
      case 179: {
        add_assoc_null_ex((arr), "global_value", strlen("global_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15918
      case 180: {
        add_assoc_null_ex((arr), "local_value", strlen("local_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15919
      case 181: {
        add_assoc_null_ex((element), "global_value",
                          strlen("global_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15920
      case 182: {
        add_assoc_null_ex((element), "local_value", strlen("local_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15921
      case 183: {
        add_assoc_null_ex((hash), "global_value", strlen("global_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15922
      case 184: {
        add_assoc_null_ex((hash), "local_value", strlen("local_value") + 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15923
      case 185: {
        add_assoc_string_ex((arg1), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15924
      case 186: {
        add_assoc_string_ex((arg1), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15925
      case 187: {
        add_assoc_string_ex((arg2), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15926
      case 188: {
        add_assoc_string_ex((arg2), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15927
      case 189: {
        add_assoc_string_ex((arg3), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15928
      case 190: {
        add_assoc_string_ex((arg3), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15929
      case 191: {
        add_assoc_string_ex((arr), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15930
      case 192: {
        add_assoc_string_ex((arr), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15931
      case 193: {
        add_assoc_string_ex((element), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15932
      case 194: {
        add_assoc_string_ex((element), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15933
      case 195: {
        add_assoc_string_ex((hash), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15934
      case 196: {
        add_assoc_string_ex((hash), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15935
      case 197: {
        add_assoc_zval_ex((arg1), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15936
      case 198: {
        add_assoc_zval_ex((arg2), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15937
      case 199: {
        add_assoc_zval_ex((arg3), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15938
      case 200: {
        add_assoc_zval_ex((arr), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15939
      case 201: {
        add_assoc_zval_ex((element), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15940
      case 202: {
        add_assoc_zval_ex((hash), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15941
      case 203: {
        add_assoc_zval_ex(hash, (*(arg1)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15942
      case 204: {
        add_assoc_zval_ex(hash, (*(arg2)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15943
      case 205: {
        add_assoc_zval_ex(hash, (*(arg3)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15944
      case 206: {
        add_assoc_zval_ex(hash, (*(arr)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15945
      case 207: {
        add_assoc_zval_ex(hash, (*(element)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15946
      case 208: {
        add_assoc_zval_ex(hash, (*(hash)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15947
      case 209: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(arg1)).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15948
      case 210: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(arg2)).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15949
      case 211: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(arg3)).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15950
      case 212: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(arr)).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15951
      case 213: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(element)).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15952
      case 214: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(hash)).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15953
      case 215: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15954
      case 216: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15955
      case 217: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15956
      case 218: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15957
      case 219: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15958
      case 220: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15959
      case 221: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15960
      case 222: {
        add_next_index_zval((arg1), element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15961
      case 223: {
        add_next_index_zval((arg2), element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15962
      case 224: {
        add_next_index_zval((arg3), element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15963
      case 225: {
        add_next_index_zval((arr), element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15964
      case 226: {
        add_next_index_zval((element), element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15965
      case 227: {
        add_next_index_zval((hash), element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15966
      case 228: {
        add_next_index_zval(hash, (arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15967
      case 229: {
        add_next_index_zval(hash, (arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15968
      case 230: {
        add_next_index_zval(hash, (arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15969
      case 231: {
        add_next_index_zval(hash, (arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15970
      case 232: {
        add_next_index_zval(hash, (element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15971
      case 233: {
        add_next_index_zval(hash, (hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15972
      case 234: {
        add_next_index_zval(hash, element);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15973
      case 235: {
        basic_globals_ctor(&basic_globals);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15974
      case 236: {
        basic_globals_dtor(&basic_globals);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15975
      case 237: {
        convert_to_array(*(find_hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15976
      case 238: {
        convert_to_boolean((arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15977
      case 239: {
        convert_to_boolean((arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15978
      case 240: {
        convert_to_boolean((arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15979
      case 241: {
        convert_to_boolean((arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15980
      case 242: {
        convert_to_boolean((element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15981
      case 243: {
        convert_to_boolean((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15982
      case 244: {
        exit(1);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15983
      case 245: {
        free((basic_globals.url_adapt_state_ex).tags);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15984
      case 246: {
        gc_remove_zval_from_buffer((arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15985
      case 247: {
        gc_remove_zval_from_buffer((arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15986
      case 248: {
        gc_remove_zval_from_buffer((arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15987
      case 249: {
        gc_remove_zval_from_buffer((arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15988
      case 250: {
        gc_remove_zval_from_buffer((element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15989
      case 251: {
        gc_remove_zval_from_buffer((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15990
      case 252: {
        gc_zval_check_possible_root(((arg1)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15991
      case 253: {
        gc_zval_check_possible_root(((arg2)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15992
      case 254: {
        gc_zval_check_possible_root(((arg3)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15993
      case 255: {
        gc_zval_check_possible_root(((arr)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15994
      case 256: {
        gc_zval_check_possible_root(((element)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15995
      case 257: {
        gc_zval_check_possible_root(((hash)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15996
      case 258: {
        gc_zval_possible_root((arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15997
      case 259: {
        gc_zval_possible_root((arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15998
      case 260: {
        gc_zval_possible_root((arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_15999
      case 261: {
        gc_zval_possible_root((arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16000
      case 262: {
        gc_zval_possible_root((element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16001
      case 263: {
        gc_zval_possible_root((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16002
      case 264: {
        hash = *(find_hash);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16003
      case 265: {
        hash = *find_hash;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16004
      case 266: {
        i_zval_ptr_dtor((arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16005
      case 267: {
        i_zval_ptr_dtor((arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16006
      case 268: {
        i_zval_ptr_dtor((arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16007
      case 269: {
        i_zval_ptr_dtor((arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16008
      case 270: {
        i_zval_ptr_dtor((element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16009
      case 271: {
        i_zval_ptr_dtor((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16010
      case 272: {
        if (!(basic_globals.user_shutdown_function_names)) {
          ((basic_globals.user_shutdown_function_names)) =
              (HashTable *)_emalloc((sizeof(HashTable)));
          _zend_hash_init(((basic_globals.user_shutdown_function_names)), (0),
                          (((void *)0)),
                          ((void (*)(void *))user_shutdown_function_dtor), (0));
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16011
      case 273: {
        if (!(basic_globals.user_tick_functions)) {
          (basic_globals.user_tick_functions) =
              (zend_llist *)_emalloc((sizeof(zend_llist)));
          zend_llist_init((basic_globals.user_tick_functions),
                          sizeof(user_tick_function_entry),
                          (llist_dtor_func_t)user_tick_function_dtor, 0);
          php_add_tick_function(run_user_tick_functions);
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16012
      case 274: {
        if (!(basic_globals.user_tick_functions)) {
          return;
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16013
      case 275: {
        if (!(executor_globals.active_op_array)->scope) {
          zend_error((1 << 0L), "Cannot call forward_static_call() when no "
                                "class scope is active");
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16014
      case 276: {
        if (!__is_neg(
                "85-276", 1, (int *){callback_type}, 0, 0, 7,
                (void **){arr, find_hash, arg1, hash, arg2, element, arg3}, 0,
                0, 9, "arr", "find_hash", "key", "arg1", "hash", "arg2",
                "element", "callback_type", "arg3") &&
            (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
             -1)) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16015
      case 277: {
        if (!__is_neg(
                "85-277", 1, (int *){callback_type}, 0, 0, 7,
                (void **){arr, find_hash, arg1, hash, arg2, element, arg3}, 0,
                0, 9, "arr", "find_hash", "key", "arg1", "hash", "arg2",
                "element", "callback_type", "arg3"))
          if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
              -1) {
            do {
              (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
              ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
            } while (0);
            (hash)->refcount__gc = 1;
            (hash)->is_ref__gc = 0;
            ;
            _array_init((hash), 0);
            _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                   sizeof(zval *), ((void *)0),
                                                   (1 << 0));
          } else {
            hash = *find_hash;
          }

        break;
      }
#endif
#ifdef COMPILE_16016
      case 278: {
        if ((*__errno_location()) == 22) {
          php_error_docref0(((void *)0), (1 << 1L),
                            "nanoseconds was not in the range 0 to 999 999 999 "
                            "or seconds was negative");
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16017
      case 279: {
        if ((basic_globals.locale_string) &&
            !((((basic_globals.locale_string)) >=
               (compiler_globals.interned_strings_start)) &&
              (((basic_globals.locale_string)) <
               (compiler_globals.interned_strings_end)))) {
          _efree(((basic_globals.locale_string)));
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16018
      case 280: {
        if ((basic_globals.strtok_zval)) {
          _zval_ptr_dtor((&(basic_globals.strtok_zval)));
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16019
      case 281: {
        if ((basic_globals.umask) != -1) {
          umask((basic_globals.umask));
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16020
      case 282: {
        if ((basic_globals.url_adapt_state_ex).tags) {
          zend_hash_destroy((basic_globals.url_adapt_state_ex).tags);
          free((basic_globals.url_adapt_state_ex).tags);
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16021
      case 283: {
        if ((basic_globals.user_tick_functions)) {
          zend_llist_destroy((basic_globals.user_tick_functions));
          _efree(((basic_globals.user_tick_functions)));
          (basic_globals.user_tick_functions) = ((void *)0);
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16022
      case 284: {
        if ((zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
             -1) &&
            !__is_neg(
                "85-284", 1, (int *){callback_type}, 0, 0, 7,
                (void **){arr, find_hash, arg1, hash, arg2, element, arg3}, 0,
                0, 9, "arr", "find_hash", "key", "arg1", "hash", "arg2",
                "element", "callback_type", "arg3")) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16023
      case 285: {
        if ((zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
             -1) ||
            __is_neg("85-285", 1, (int *){callback_type}, 0, 0, 7,
                     (void **){arr, find_hash, arg1, hash, arg2, element, arg3},
                     0, 0, 9, "arr", "find_hash", "key", "arg1", "hash", "arg2",
                     "element", "callback_type", "arg3")) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16024
      case 286: {
        if (__is_neg("85-286", 1, (int *){callback_type}, 0, 0, 7,
                     (void **){arr, find_hash, arg1, hash, arg2, element, arg3},
                     0, 0, 9, "arr", "find_hash", "key", "arg1", "hash", "arg2",
                     "element", "callback_type", "arg3"))
          return;
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16025
      case 287: {
        if (_zend_hash_init((&(basic_globals.putenv_ht)), (1), (((void *)0)),
                            ((void (*)(void *))php_putenv_destructor),
                            (0)) == -1) {
          return -1;
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16026
      case 288: {
        if (arg3 && (*arg3).value.str.len > 0) {
          add_assoc_zval_ex(hash, (*arg3).value.str.val,
                            (*arg3).value.str.len + 1, element);
        } else {
          add_next_index_zval(hash, element);
        }
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16027
      case 289: {
        incomplete_class_entry = php_create_incomplete_class();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16028
      case 290: {
        memset(&((zval_gc_info *)(hash))->u, 0,
               sizeof(*(&((zval_gc_info *)(hash))->u)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16029
      case 291: {
        memset(&(*arr), 0, sizeof(*(&(*arr))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16030
      case 292: {
        memset(&(*arr).value, 0, sizeof(*(&(*arr).value)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16031
      case 293: {
        memset(&(basic_globals.serialize), 0,
               sizeof((basic_globals.serialize)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16032
      case 294: {
        memset(&(basic_globals.unserialize), 0,
               sizeof((basic_globals.unserialize)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16033
      case 295: {
        memset(&(basic_globals.url_adapt_state_ex), 0,
               sizeof((basic_globals.url_adapt_state_ex)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16034
      case 296: {
        memset(((zval_gc_info *)(hash)), 0,
               sizeof(*(((zval_gc_info *)(hash)))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16035
      case 297: {
        memset((basic_globals.strtok_table), 0, 256);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16036
      case 298: {
        memset((hash), 0, sizeof(*((hash))));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16037
      case 299: {
        php_add_tick_function(run_user_tick_functions);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16038
      case 300: {
        php_error_docref0(((void *)0), (1 << 1L), "%s",
                          strerror((*__errno_location())));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16039
      case 301: {
        php_error_docref0(((void *)0), (1 << 1L), "An unknown error occured");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16040
      case 302: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted GLOBALS variable overwrite");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16041
      case 303: {
        php_error_docref0(((void *)0), (1 << 1L), "Filename cannot be empty!");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16042
      case 304: {
        php_error_docref0(((void *)0), (1 << 1L), "Invalid in_addr value");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16043
      case 305: {
        php_error_docref0(((void *)0), (1 << 1L), "Invalid parameter syntax");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16044
      case 306: {
        php_error_docref0(
            ((void *)0), (1 << 1L),
            "Number of microseconds must be greater than or equal to 0");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16045
      case 307: {
        php_error_docref0(
            ((void *)0), (1 << 1L),
            "Number of seconds must be greater than or equal to 0");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16046
      case 308: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Second argument is not an object or class name");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16047
      case 309: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Sleep until to time is less than current time");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16048
      case 310: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(arg1)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16049
      case 311: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(arg2)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16050
      case 312: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(arg3)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16051
      case 313: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(arr)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16052
      case 314: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(element)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16053
      case 315: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(hash)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16054
      case 316: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(arg1)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16055
      case 317: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(arg2)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16056
      case 318: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(arg3)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16057
      case 319: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(arr)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16058
      case 320: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(element)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16059
      case 321: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(hash)).value.str.val);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16060
      case 322: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call tick function");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16061
      case 323: {
        php_error_docref0(
            ((void *)0), (1 << 1L),
            "Unable to delete tick function executed at the moment");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16062
      case 324: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "nanoseconds was not in the range 0 to 999 999 999 "
                          "or seconds was negative");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16063
      case 325: {
        php_free_shutdown_functions();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16064
      case 326: {
        php_info_print_table_end();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16065
      case 327: {
        php_info_print_table_start();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16066
      case 328: {
        php_output_discard();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16067
      case 329: {
        php_output_end();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16068
      case 330: {
        php_output_get_contents((arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16069
      case 331: {
        php_output_get_contents((arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16070
      case 332: {
        php_output_get_contents((arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16071
      case 333: {
        php_output_get_contents((arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16072
      case 334: {
        php_output_get_contents((element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16073
      case 335: {
        php_output_get_contents((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16074
      case 336: {
        php_output_start_default();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16075
      case 337: {
        php_register_url_stream_wrapper("data", &php_stream_rfc2397_wrapper);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16076
      case 338: {
        php_register_url_stream_wrapper("file", &php_plain_files_wrapper);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16077
      case 339: {
        php_register_url_stream_wrapper("ftp", &php_stream_ftp_wrapper);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16078
      case 340: {
        php_register_url_stream_wrapper("glob", &php_glob_stream_wrapper);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16079
      case 341: {
        php_register_url_stream_wrapper("http", &php_stream_http_wrapper);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16080
      case 342: {
        php_register_url_stream_wrapper("php", &php_stream_php_wrapper);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16081
      case 343: {
        php_unregister_url_stream_wrapper("ftp");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16082
      case 344: {
        php_unregister_url_stream_wrapper("http");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16083
      case 345: {
        php_unregister_url_stream_wrapper("php");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16084
      case 346: {
        sapi_flush();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16085
      case 347: {
        setlocale(0, "");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16086
      case 348: {
        setlocale(6, "C");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16087
      case 349: {
        tzset();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16088
      case 350: {
        umask((basic_globals.umask));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16089
      case 351: {
        zend_error(
            (1 << 0L),
            "Cannot call forward_static_call() when no class scope is active");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16090
      case 352: {
        zend_error((1 << 3L), "A non well formed numeric value encountered");
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16091
      case 353: {
        zend_hash_apply((basic_globals.user_shutdown_function_names),
                        (apply_func_t)user_shutdown_function_call);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16092
      case 354: {
        zend_hash_destroy(&(basic_globals.putenv_ht));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16093
      case 355: {
        zend_hash_destroy((*(arg1)).value.ht);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16094
      case 356: {
        zend_hash_destroy((*(arg2)).value.ht);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16095
      case 357: {
        zend_hash_destroy((*(arg3)).value.ht);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16096
      case 358: {
        zend_hash_destroy((*(arr)).value.ht);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16097
      case 359: {
        zend_hash_destroy((*(element)).value.ht);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16098
      case 360: {
        zend_hash_destroy((*(hash)).value.ht);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16099
      case 361: {
        zend_hash_destroy((basic_globals.url_adapt_state_ex).tags);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16100
      case 362: {
        zend_hash_destroy((basic_globals.user_shutdown_function_names));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16101
      case 363: {
        zend_hash_internal_pointer_reset_ex((*(arg1)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16102
      case 364: {
        zend_hash_internal_pointer_reset_ex((*(arg2)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16103
      case 365: {
        zend_hash_internal_pointer_reset_ex((*(arg3)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16104
      case 366: {
        zend_hash_internal_pointer_reset_ex((*(arr)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16105
      case 367: {
        zend_hash_internal_pointer_reset_ex((*(element)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16106
      case 368: {
        zend_hash_internal_pointer_reset_ex((*(hash)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16107
      case 369: {
        zend_hash_internal_pointer_reset_ex((**(find_hash)).value.ht,
                                            ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16108
      case 370: {
        zend_hash_move_forward_ex((*(arg1)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16109
      case 371: {
        zend_hash_move_forward_ex((*(arg2)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16110
      case 372: {
        zend_hash_move_forward_ex((*(arg3)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16111
      case 373: {
        zend_hash_move_forward_ex((*(arr)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16112
      case 374: {
        zend_hash_move_forward_ex((*(element)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16113
      case 375: {
        zend_hash_move_forward_ex((*(hash)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16114
      case 376: {
        zend_hash_move_forward_ex((**(find_hash)).value.ht, ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16115
      case 377: {
        zend_ini_sort_entries();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16116
      case 378: {
        zend_llist_apply((basic_globals.user_tick_functions),
                         (llist_apply_func_t)user_tick_function_call);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16117
      case 379: {
        zend_llist_destroy((basic_globals.user_tick_functions));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16118
      case 380: {
        zend_llist_init((basic_globals.user_tick_functions),
                        sizeof(user_tick_function_entry),
                        (llist_dtor_func_t)user_tick_function_dtor, 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16119
      case 381: {
        zend_print_zval_r((arg1), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16120
      case 382: {
        zend_print_zval_r((arg2), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16121
      case 383: {
        zend_print_zval_r((arg3), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16122
      case 384: {
        zend_print_zval_r((arr), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16123
      case 385: {
        zend_print_zval_r((element), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16124
      case 386: {
        zend_print_zval_r((hash), 0);
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16125
      case 387: {
        zend_restore_ini_entry("include_path", sizeof("include_path"),
                               (1 << 4));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16126
      case 388: {
        zend_strip();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16127
      case 389: {
        zend_symtable_update((*(arg1)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16128
      case 390: {
        zend_symtable_update((*(arg2)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16129
      case 391: {
        zend_symtable_update((*(arg3)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16130
      case 392: {
        zend_symtable_update((*(arr)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16131
      case 393: {
        zend_symtable_update((*(element)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16132
      case 394: {
        zend_symtable_update((*(hash)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16133
      case 395: {
        zend_symtable_update((*arr).value.ht, (*(arg1)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16134
      case 396: {
        zend_symtable_update((*arr).value.ht, (*(arg2)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16135
      case 397: {
        zend_symtable_update((*arr).value.ht, (*(arg3)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16136
      case 398: {
        zend_symtable_update((*arr).value.ht, (*(arr)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16137
      case 399: {
        zend_symtable_update((*arr).value.ht, (*(element)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16138
      case 400: {
        zend_symtable_update((*arr).value.ht, (*(hash)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16139
      case 401: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(arg1)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16140
      case 402: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(arg2)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16141
      case 403: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(arg3)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16142
      case 404: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(arr)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16143
      case 405: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(element)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16144
      case 406: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(hash)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16145
      case 407: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(arg1), sizeof(zval *),
                             ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16146
      case 408: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(arg2), sizeof(zval *),
                             ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16147
      case 409: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(arg3), sizeof(zval *),
                             ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16148
      case 410: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(arr), sizeof(zval *),
                             ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16149
      case 411: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(element),
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16150
      case 412: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(hash), sizeof(zval *),
                             ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16151
      case 413: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16152
      case 414: {
        zend_wrong_param_count();
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16153
      case 415: {
        zval_delref_p(((arg1)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16154
      case 416: {
        zval_delref_p(((arg2)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16155
      case 417: {
        zval_delref_p(((arg3)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16156
      case 418: {
        zval_delref_p(((arr)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16157
      case 419: {
        zval_delref_p(((element)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16158
      case 420: {
        zval_delref_p(((hash)));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16159
      case 421: {
        zval_unset_isref_p((arg1));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16160
      case 422: {
        zval_unset_isref_p((arg2));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16161
      case 423: {
        zval_unset_isref_p((arg3));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16162
      case 424: {
        zval_unset_isref_p((arr));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16163
      case 425: {
        zval_unset_isref_p((element));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
#ifdef COMPILE_16164
      case 426: {
        zval_unset_isref_p((hash));
        if (zend_hash_index_find((*arr).value.ht, key, (void **)&find_hash) ==
            -1) {
          do {
            (hash) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(hash))->u.buffered = ((void *)0);
          } while (0);
          (hash)->refcount__gc = 1;
          (hash)->is_ref__gc = 0;
          ;
          _array_init((hash), 0);
          _zend_hash_index_update_or_next_insert((*arr).value.ht, key, &hash,
                                                 sizeof(zval *), ((void *)0),
                                                 (1 << 0));
        } else {
          hash = *find_hash;
        }

        break;
      }
#endif
      }
    } else {
      if (zend_hash_find(Z_ARRVAL_P(arr), Z_STRVAL_P(arg1),
                         Z_STRLEN_P(arg1) + 1,
                         (void **)&find_hash) == FAILURE) {
        ALLOC_ZVAL(hash);
        INIT_PZVAL(hash);
        array_init(hash);

        zend_hash_update(Z_ARRVAL_P(arr), Z_STRVAL_P(arg1),
                         Z_STRLEN_P(arg1) + 1, &hash, sizeof(zval *), NULL);
      } else {
        hash = *find_hash;
      }
    }

    if (Z_TYPE_P(hash) != IS_ARRAY) {
      zval_dtor(hash);
      INIT_PZVAL(hash);
      // prophet generated patch
      switch (__choose("__ID86")) {
      case 0: {
        _array_init((hash), 0);

        break;
      }
#ifdef COMPILE_16165
      case 1: {
        ((arg1))->is_ref__gc = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16166
      case 2: {
        ((arg1))->refcount__gc = 1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16167
      case 3: {
        ((arg2))->is_ref__gc = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16168
      case 4: {
        ((arg2))->refcount__gc = 1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16169
      case 5: {
        ((arg3))->is_ref__gc = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16170
      case 6: {
        ((arg3))->refcount__gc = 1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16171
      case 7: {
        ((arr))->is_ref__gc = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16172
      case 8: {
        ((arr))->refcount__gc = 1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16173
      case 9: {
        ((basic_globals.active_ini_file_section))->is_ref__gc = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16174
      case 10: {
        ((basic_globals.active_ini_file_section))->refcount__gc = 1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16175
      case 11: {
        ((basic_globals.user_shutdown_function_names)) =
            (HashTable *)_emalloc((sizeof(HashTable)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16176
      case 12: {
        ((element))->is_ref__gc = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16177
      case 13: {
        ((element))->refcount__gc = 1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16178
      case 14: {
        ((hash))->is_ref__gc = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16179
      case 15: {
        ((hash))->refcount__gc = 1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16180
      case 16: {
        (arg1) = *find_hash;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16181
      case 17: {
        (arg2) = *find_hash;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16182
      case 18: {
        (arg3) = *find_hash;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16183
      case 19: {
        (arr) = *find_hash;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16184
      case 20: {
        (basic_globals.active_ini_file_section) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16185
      case 21: {
        (basic_globals.array_walk_fci) = empty_fcall_info;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16186
      case 22: {
        (basic_globals.array_walk_fci_cache) = empty_fcall_info_cache;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16187
      case 23: {
        (basic_globals.incomplete_class) = incomplete_class_entry;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16188
      case 24: {
        (basic_globals.incomplete_class) = php_create_incomplete_class();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16189
      case 25: {
        (basic_globals.left) = -1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16190
      case 26: {
        (basic_globals.locale_string) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16191
      case 27: {
        (basic_globals.mt_rand_is_seeded) = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16192
      case 28: {
        (basic_globals.next) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16193
      case 29: {
        (basic_globals.page_gid) = -1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16194
      case 30: {
        (basic_globals.page_inode) = -1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16195
      case 31: {
        (basic_globals.page_mtime) = -1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16196
      case 32: {
        (basic_globals.page_uid) = -1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16197
      case 33: {
        (basic_globals.rand_is_seeded) = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16198
      case 34: {
        (basic_globals.strtok_last) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16199
      case 35: {
        (basic_globals.strtok_string) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16200
      case 36: {
        (basic_globals.strtok_zval) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16201
      case 37: {
        (basic_globals.umask) = -1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16202
      case 38: {
        (basic_globals.user_compare_fci) = empty_fcall_info;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16203
      case 39: {
        (basic_globals.user_compare_fci_cache) = empty_fcall_info_cache;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16204
      case 40: {
        (basic_globals.user_filter_map) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16205
      case 41: {
        (basic_globals.user_shutdown_function_names) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16206
      case 42: {
        (basic_globals.user_tick_functions) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16207
      case 43: {
        (basic_globals.user_tick_functions) =
            (zend_llist *)_emalloc((sizeof(zend_llist)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16208
      case 44: {
        (element) = *find_hash;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16209
      case 45: {
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16210
      case 46: {
        (executor_globals.error_reporting) = (1 << 0L);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16211
      case 47: {
        (file_globals.default_context) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16212
      case 48: {
        (file_globals.stream_filters) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16213
      case 49: {
        (file_globals.stream_wrappers) = ((void *)0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16214
      case 50: {
        (hash) = *find_hash;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16215
      case 51: {
        (hash)->is_ref__gc = 0;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16216
      case 52: {
        (hash)->refcount__gc = 1;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16217
      case 53: {
        Debug((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16218
      case 54: {
        PHP_MD5Final((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16219
      case 55: {
        PHP_SHA1Final((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16220
      case 56: {
        __cmsg_nxthdr((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16221
      case 57: {
        __overflow((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16222
      case 58: {
        __sigsetjmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16223
      case 59: {
        __stpcpy((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16224
      case 60: {
        __zend_realloc((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16225
      case 61: {
        _array_init(((arg1)), 0);

        break;
      }
#endif
#ifdef COMPILE_16226
      case 62: {
        _array_init(((arg1)), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16227
      case 63: {
        _array_init(((arg2)), 0);

        break;
      }
#endif
#ifdef COMPILE_16228
      case 64: {
        _array_init(((arg2)), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16229
      case 65: {
        _array_init(((arg3)), 0);

        break;
      }
#endif
#ifdef COMPILE_16230
      case 66: {
        _array_init(((arg3)), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16231
      case 67: {
        _array_init(((arr)), 0);

        break;
      }
#endif
#ifdef COMPILE_16232
      case 68: {
        _array_init(((arr)), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16233
      case 69: {
        _array_init(((basic_globals.active_ini_file_section)), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16234
      case 70: {
        _array_init(((element)), 0);

        break;
      }
#endif
#ifdef COMPILE_16235
      case 71: {
        _array_init(((element)), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16236
      case 72: {
        _array_init(((hash)), 0);

        break;
      }
#endif
#ifdef COMPILE_16237
      case 73: {
        _array_init(((hash)), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16238
      case 74: {
        _array_init((hash), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16239
      case 75: {
        _convert_to_string(((arg1)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16240
      case 76: {
        _convert_to_string(((arg2)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16241
      case 77: {
        _convert_to_string(((arg3)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16242
      case 78: {
        _convert_to_string(((arr)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16243
      case 79: {
        _convert_to_string(((element)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16244
      case 80: {
        _convert_to_string(((hash)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16245
      case 81: {
        _convert_to_string((*(find_hash)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16246
      case 82: {
        _efree(((*(arg1)).value.ht));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16247
      case 83: {
        _efree(((*(arg2)).value.ht));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16248
      case 84: {
        _efree(((*(arg3)).value.ht));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16249
      case 85: {
        _efree(((*(arr)).value.ht));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16250
      case 86: {
        _efree(((*(element)).value.ht));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16251
      case 87: {
        _efree(((*(hash)).value.ht));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16252
      case 88: {
        _efree(((arg1)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16253
      case 89: {
        _efree(((arg2)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16254
      case 90: {
        _efree(((arg3)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16255
      case 91: {
        _efree(((arr)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16256
      case 92: {
        _efree(((basic_globals.locale_string)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16257
      case 93: {
        _efree(((basic_globals.user_shutdown_function_names)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16258
      case 94: {
        _efree(((basic_globals.user_tick_functions)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16259
      case 95: {
        _efree(((element)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16260
      case 96: {
        _efree(((hash)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16261
      case 97: {
        _estrndup((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16262
      case 98: {
        _longjmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16263
      case 99: {
        _object_init_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16264
      case 100: {
        _php_glob_stream_get_count((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16265
      case 101: {
        _php_math_basetolong((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16266
      case 102: {
        _php_math_longtobase((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16267
      case 103: {
        _php_math_zvaltobase((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16268
      case 104: {
        _php_stream_filter_append((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16269
      case 105: {
        _php_stream_filter_flush((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16270
      case 106: {
        _php_stream_filter_prepend((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16271
      case 107: {
        _php_stream_flush((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16272
      case 108: {
        _php_stream_fopen_from_file((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16273
      case 109: {
        _php_stream_fopen_from_pipe((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16274
      case 110: {
        _php_stream_free((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16275
      case 111: {
        _php_stream_free_enclosed((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16276
      case 112: {
        _php_stream_memory_get_buffer((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16277
      case 113: {
        _php_stream_mmap_unmap_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16278
      case 114: {
        _php_stream_printf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16279
      case 115: {
        _php_stream_putc((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16280
      case 116: {
        _php_stream_puts((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16281
      case 117: {
        _php_stream_readdir((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16282
      case 118: {
        _php_stream_stat((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16283
      case 119: {
        _php_stream_truncate_set_size((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16284
      case 120: {
        _zend_bailout((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16285
      case 121: {
        _zend_hash_add_or_update((*(arg1)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16286
      case 122: {
        _zend_hash_add_or_update((*(arg2)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16287
      case 123: {
        _zend_hash_add_or_update((*(arg3)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16288
      case 124: {
        _zend_hash_add_or_update((*(arr)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16289
      case 125: {
        _zend_hash_add_or_update((*(element)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16290
      case 126: {
        _zend_hash_add_or_update((*(hash)).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16291
      case 127: {
        _zend_hash_add_or_update((*arr).value.ht, (*(arg1)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16292
      case 128: {
        _zend_hash_add_or_update((*arr).value.ht, (*(arg2)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16293
      case 129: {
        _zend_hash_add_or_update((*arr).value.ht, (*(arg3)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16294
      case 130: {
        _zend_hash_add_or_update((*arr).value.ht, (*(arr)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16295
      case 131: {
        _zend_hash_add_or_update((*arr).value.ht, (*(element)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16296
      case 132: {
        _zend_hash_add_or_update((*arr).value.ht, (*(hash)).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16297
      case 133: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(arg1)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16298
      case 134: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(arg2)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16299
      case 135: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(arg3)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16300
      case 136: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(arr)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16301
      case 137: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(element)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16302
      case 138: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*(hash)).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16303
      case 139: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(arg1),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16304
      case 140: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(arg2),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16305
      case 141: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(arg3),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16306
      case 142: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(arr),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16307
      case 143: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(element),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16308
      case 144: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &(hash),
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16309
      case 145: {
        _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                                 (*arg1).value.str.len + 1, &hash,
                                 sizeof(zval *), ((void *)0), (1 << 0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16310
      case 146: {
        _zend_hash_init(((basic_globals.user_shutdown_function_names)), (0),
                        (((void *)0)),
                        ((void (*)(void *))user_shutdown_function_dtor), (0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16311
      case 147: {
        _zend_mm_alloc((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16312
      case 148: {
        _zend_mm_block_size((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16313
      case 149: {
        _zend_mm_free((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16314
      case 150: {
        _zval_copy_ctor((&(*(arg1))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16315
      case 151: {
        _zval_copy_ctor((&(*(arg2))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16316
      case 152: {
        _zval_copy_ctor((&(*(arg3))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16317
      case 153: {
        _zval_copy_ctor((&(*(arr))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16318
      case 154: {
        _zval_copy_ctor((&(*(element))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16319
      case 155: {
        _zval_copy_ctor((&(*(hash))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16320
      case 156: {
        _zval_copy_ctor((((arg1))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16321
      case 157: {
        _zval_copy_ctor((((arg2))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16322
      case 158: {
        _zval_copy_ctor((((arg3))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16323
      case 159: {
        _zval_copy_ctor((((arr))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16324
      case 160: {
        _zval_copy_ctor((((element))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16325
      case 161: {
        _zval_copy_ctor((((hash))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16326
      case 162: {
        _zval_copy_ctor(((element)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16327
      case 163: {
        _zval_copy_ctor_func((arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16328
      case 164: {
        _zval_copy_ctor_func((arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16329
      case 165: {
        _zval_copy_ctor_func((arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16330
      case 166: {
        _zval_copy_ctor_func((arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16331
      case 167: {
        _zval_copy_ctor_func((element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16332
      case 168: {
        _zval_copy_ctor_func((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16333
      case 169: {
        _zval_dtor(((arg1)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16334
      case 170: {
        _zval_dtor(((arg2)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16335
      case 171: {
        _zval_dtor(((arg3)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16336
      case 172: {
        _zval_dtor(((arr)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16337
      case 173: {
        _zval_dtor(((element)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16338
      case 174: {
        _zval_dtor(((hash)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16339
      case 175: {
        _zval_dtor((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16340
      case 176: {
        _zval_dtor_func((arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16341
      case 177: {
        _zval_dtor_func((arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16342
      case 178: {
        _zval_dtor_func((arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16343
      case 179: {
        _zval_dtor_func((arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16344
      case 180: {
        _zval_dtor_func((element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16345
      case 181: {
        _zval_dtor_func((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16346
      case 182: {
        _zval_ptr_dtor((&(arg1)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16347
      case 183: {
        _zval_ptr_dtor((&(arg2)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16348
      case 184: {
        _zval_ptr_dtor((&(arg3)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16349
      case 185: {
        _zval_ptr_dtor((&(arr)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16350
      case 186: {
        _zval_ptr_dtor((&(basic_globals.strtok_zval)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16351
      case 187: {
        _zval_ptr_dtor((&(element)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16352
      case 188: {
        _zval_ptr_dtor((&(hash)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16353
      case 189: {
        access((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16354
      case 190: {
        add_assoc_long_ex((arg1), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16355
      case 191: {
        add_assoc_long_ex((arg1), "type", sizeof("type"),
                          (core_globals.last_error_type));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16356
      case 192: {
        add_assoc_long_ex((arg2), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16357
      case 193: {
        add_assoc_long_ex((arg2), "type", sizeof("type"),
                          (core_globals.last_error_type));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16358
      case 194: {
        add_assoc_long_ex((arg3), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16359
      case 195: {
        add_assoc_long_ex((arg3), "type", sizeof("type"),
                          (core_globals.last_error_type));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16360
      case 196: {
        add_assoc_long_ex((arr), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16361
      case 197: {
        add_assoc_long_ex((arr), "type", sizeof("type"),
                          (core_globals.last_error_type));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16362
      case 198: {
        add_assoc_long_ex((element), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16363
      case 199: {
        add_assoc_long_ex((element), "type", sizeof("type"),
                          (core_globals.last_error_type));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16364
      case 200: {
        add_assoc_long_ex((hash), "line", sizeof("line"),
                          (core_globals.last_error_lineno));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16365
      case 201: {
        add_assoc_long_ex((hash), "type", sizeof("type"),
                          (core_globals.last_error_type));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16366
      case 202: {
        add_assoc_null_ex((arg1), "global_value", strlen("global_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16367
      case 203: {
        add_assoc_null_ex((arg1), "local_value", strlen("local_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16368
      case 204: {
        add_assoc_null_ex((arg2), "global_value", strlen("global_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16369
      case 205: {
        add_assoc_null_ex((arg2), "local_value", strlen("local_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16370
      case 206: {
        add_assoc_null_ex((arg3), "global_value", strlen("global_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16371
      case 207: {
        add_assoc_null_ex((arg3), "local_value", strlen("local_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16372
      case 208: {
        add_assoc_null_ex((arr), "global_value", strlen("global_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16373
      case 209: {
        add_assoc_null_ex((arr), "local_value", strlen("local_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16374
      case 210: {
        add_assoc_null_ex((element), "global_value",
                          strlen("global_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16375
      case 211: {
        add_assoc_null_ex((element), "local_value", strlen("local_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16376
      case 212: {
        add_assoc_null_ex((hash), "global_value", strlen("global_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16377
      case 213: {
        add_assoc_null_ex((hash), "local_value", strlen("local_value") + 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16378
      case 214: {
        add_assoc_string_ex((arg1), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16379
      case 215: {
        add_assoc_string_ex((arg1), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16380
      case 216: {
        add_assoc_string_ex((arg2), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16381
      case 217: {
        add_assoc_string_ex((arg2), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16382
      case 218: {
        add_assoc_string_ex((arg3), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16383
      case 219: {
        add_assoc_string_ex((arg3), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16384
      case 220: {
        add_assoc_string_ex((arr), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16385
      case 221: {
        add_assoc_string_ex((arr), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16386
      case 222: {
        add_assoc_string_ex((element), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16387
      case 223: {
        add_assoc_string_ex((element), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16388
      case 224: {
        add_assoc_string_ex((hash), "file", sizeof("file"),
                            (core_globals.last_error_file)
                                ? (core_globals.last_error_file)
                                : "-",
                            1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16389
      case 225: {
        add_assoc_string_ex((hash), "message", sizeof("message"),
                            (core_globals.last_error_message), 1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16390
      case 226: {
        add_assoc_zval_ex((arg1), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16391
      case 227: {
        add_assoc_zval_ex((arg2), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16392
      case 228: {
        add_assoc_zval_ex((arg3), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16393
      case 229: {
        add_assoc_zval_ex((arr), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16394
      case 230: {
        add_assoc_zval_ex((element), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16395
      case 231: {
        add_assoc_zval_ex((hash), (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16396
      case 232: {
        add_assoc_zval_ex(hash, (*(arg1)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16397
      case 233: {
        add_assoc_zval_ex(hash, (*(arg2)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16398
      case 234: {
        add_assoc_zval_ex(hash, (*(arg3)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16399
      case 235: {
        add_assoc_zval_ex(hash, (*(arr)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16400
      case 236: {
        add_assoc_zval_ex(hash, (*(element)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16401
      case 237: {
        add_assoc_zval_ex(hash, (*(hash)).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16402
      case 238: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(arg1)).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16403
      case 239: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(arg2)).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16404
      case 240: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(arg3)).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16405
      case 241: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(arr)).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16406
      case 242: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(element)).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16407
      case 243: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*(hash)).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16408
      case 244: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16409
      case 245: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16410
      case 246: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16411
      case 247: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16412
      case 248: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16413
      case 249: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, (hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16414
      case 250: {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16415
      case 251: {
        add_index_null((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16416
      case 252: {
        add_next_index_bool((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16417
      case 253: {
        add_next_index_long((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16418
      case 254: {
        add_next_index_resource((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16419
      case 255: {
        add_next_index_zval((arg1), element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16420
      case 256: {
        add_next_index_zval((arg2), element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16421
      case 257: {
        add_next_index_zval((arg3), element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16422
      case 258: {
        add_next_index_zval((arr), element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16423
      case 259: {
        add_next_index_zval((element), element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16424
      case 260: {
        add_next_index_zval((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16425
      case 261: {
        add_next_index_zval((hash), element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16426
      case 262: {
        add_next_index_zval(hash, (arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16427
      case 263: {
        add_next_index_zval(hash, (arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16428
      case 264: {
        add_next_index_zval(hash, (arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16429
      case 265: {
        add_next_index_zval(hash, (arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16430
      case 266: {
        add_next_index_zval(hash, (element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16431
      case 267: {
        add_next_index_zval(hash, (hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16432
      case 268: {
        add_next_index_zval(hash, element);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16433
      case 269: {
        add_trait_alias((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16434
      case 270: {
        adjtime((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16435
      case 271: {
        alphasort((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16436
      case 272: {
        ap_php_asprintf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16437
      case 273: {
        asctime_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16438
      case 274: {
        basic_globals_ctor(&basic_globals);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16439
      case 275: {
        basic_globals_dtor(&basic_globals);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16440
      case 276: {
        bitwise_not_function((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16441
      case 277: {
        boolean_not_function((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16442
      case 278: {
        bzero((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16443
      case 279: {
        cfg_get_double((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16444
      case 280: {
        cfg_get_entry((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16445
      case 281: {
        cfg_get_long((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16446
      case 282: {
        cfg_get_string((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16447
      case 283: {
        chmod((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16448
      case 284: {
        compile_file((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16449
      case 285: {
        compile_string((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16450
      case 286: {
        convert_to_array(*(find_hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16451
      case 287: {
        convert_to_boolean((arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16452
      case 288: {
        convert_to_boolean((arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16453
      case 289: {
        convert_to_boolean((arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16454
      case 290: {
        convert_to_boolean((arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16455
      case 291: {
        convert_to_boolean((element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16456
      case 292: {
        convert_to_boolean((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16457
      case 293: {
        convert_to_long_base((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16458
      case 294: {
        ctime_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16459
      case 295: {
        display_link_numbers((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16460
      case 296: {
        dlopen((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16461
      case 297: {
        dlsym((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16462
      case 298: {
        drand48_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16463
      case 299: {
        execl((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16464
      case 300: {
        execle((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16465
      case 301: {
        execlp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16466
      case 302: {
        execute_internal((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16467
      case 303: {
        execv((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16468
      case 304: {
        execvp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16469
      case 305: {
        exit(1);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16470
      case 306: {
        expand_filepath((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16471
      case 307: {
        explicit_bzero((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16472
      case 308: {
        fgetpos((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16473
      case 309: {
        fopen((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16474
      case 310: {
        fprintf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16475
      case 311: {
        fputs((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16476
      case 312: {
        fputws((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16477
      case 313: {
        free((basic_globals.url_adapt_state_ex).tags);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16478
      case 314: {
        free_argv((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16479
      case 315: {
        fscanf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16480
      case 316: {
        fsetpos((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16481
      case 317: {
        fwide((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16482
      case 318: {
        fwprintf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16483
      case 319: {
        fwscanf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16484
      case 320: {
        gc_remove_zval_from_buffer((arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16485
      case 321: {
        gc_remove_zval_from_buffer((arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16486
      case 322: {
        gc_remove_zval_from_buffer((arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16487
      case 323: {
        gc_remove_zval_from_buffer((arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16488
      case 324: {
        gc_remove_zval_from_buffer((element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16489
      case 325: {
        gc_remove_zval_from_buffer((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16490
      case 326: {
        gc_zval_check_possible_root(((arg1)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16491
      case 327: {
        gc_zval_check_possible_root(((arg2)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16492
      case 328: {
        gc_zval_check_possible_root(((arg3)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16493
      case 329: {
        gc_zval_check_possible_root(((arr)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16494
      case 330: {
        gc_zval_check_possible_root(((element)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16495
      case 331: {
        gc_zval_check_possible_root(((hash)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16496
      case 332: {
        gc_zval_possible_root((arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16497
      case 333: {
        gc_zval_possible_root((arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16498
      case 334: {
        gc_zval_possible_root((arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16499
      case 335: {
        gc_zval_possible_root((arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16500
      case 336: {
        gc_zval_possible_root((element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16501
      case 337: {
        gc_zval_possible_root((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16502
      case 338: {
        getcwd((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16503
      case 339: {
        getdomainname((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16504
      case 340: {
        getentropy((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16505
      case 341: {
        gethostbyname2((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16506
      case 342: {
        gethostname((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16507
      case 343: {
        getloadavg((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16508
      case 344: {
        getlogin_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16509
      case 345: {
        getservbyname((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16510
      case 346: {
        gettimeofday((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16511
      case 347: {
        gmtime_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16512
      case 348: {
        hash = *(find_hash);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16513
      case 349: {
        hash = *find_hash;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16514
      case 350: {
        highlight_file((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16515
      case 351: {
        i_zval_ptr_dtor((arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16516
      case 352: {
        i_zval_ptr_dtor((arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16517
      case 353: {
        i_zval_ptr_dtor((arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16518
      case 354: {
        i_zval_ptr_dtor((arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16519
      case 355: {
        i_zval_ptr_dtor((element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16520
      case 356: {
        i_zval_ptr_dtor((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16521
      case 357: {
        if (!(basic_globals.user_shutdown_function_names)) {
          ((basic_globals.user_shutdown_function_names)) =
              (HashTable *)_emalloc((sizeof(HashTable)));
          _zend_hash_init(((basic_globals.user_shutdown_function_names)), (0),
                          (((void *)0)),
                          ((void (*)(void *))user_shutdown_function_dtor), (0));
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16522
      case 358: {
        if (!(basic_globals.user_tick_functions)) {
          (basic_globals.user_tick_functions) =
              (zend_llist *)_emalloc((sizeof(zend_llist)));
          zend_llist_init((basic_globals.user_tick_functions),
                          sizeof(user_tick_function_entry),
                          (llist_dtor_func_t)user_tick_function_dtor, 0);
          php_add_tick_function(run_user_tick_functions);
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16523
      case 359: {
        if (!(basic_globals.user_tick_functions)) {
          return;
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16524
      case 360: {
        if (!(executor_globals.active_op_array)->scope) {
          zend_error((1 << 0L), "Cannot call forward_static_call() when no "
                                "class scope is active");
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16525
      case 361: {
        if (!__is_neg(
                "86-361", 1, (int *){callback_type}, 0, 0, 7,
                (void **){hash, element, arg2, arg3, find_hash, arg1, arr}, 0,
                0, 8, "hash", "element", "arg2", "arg3", "find_hash", "arg1",
                "arr", "callback_type"))
          _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16526
      case 362: {
        if ((*__errno_location()) == 22) {
          php_error_docref0(((void *)0), (1 << 1L),
                            "nanoseconds was not in the range 0 to 999 999 999 "
                            "or seconds was negative");
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16527
      case 363: {
        if ((basic_globals.locale_string) &&
            !((((basic_globals.locale_string)) >=
               (compiler_globals.interned_strings_start)) &&
              (((basic_globals.locale_string)) <
               (compiler_globals.interned_strings_end)))) {
          _efree(((basic_globals.locale_string)));
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16528
      case 364: {
        if ((basic_globals.strtok_zval)) {
          _zval_ptr_dtor((&(basic_globals.strtok_zval)));
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16529
      case 365: {
        if ((basic_globals.umask) != -1) {
          umask((basic_globals.umask));
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16530
      case 366: {
        if ((basic_globals.url_adapt_state_ex).tags) {
          zend_hash_destroy((basic_globals.url_adapt_state_ex).tags);
          free((basic_globals.url_adapt_state_ex).tags);
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16531
      case 367: {
        if ((basic_globals.user_tick_functions)) {
          zend_llist_destroy((basic_globals.user_tick_functions));
          _efree(((basic_globals.user_tick_functions)));
          (basic_globals.user_tick_functions) = ((void *)0);
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16532
      case 368: {
        if (__is_neg("86-368", 1, (int *){callback_type}, 0, 0, 7,
                     (void **){hash, element, arg2, arg3, find_hash, arg1, arr},
                     0, 0, 8, "hash", "element", "arg2", "arg3", "find_hash",
                     "arg1", "arr", "callback_type"))
          return;
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16533
      case 369: {
        if (_zend_hash_init((&(basic_globals.putenv_ht)), (1), (((void *)0)),
                            ((void (*)(void *))php_putenv_destructor),
                            (0)) == -1) {
          return -1;
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16534
      case 370: {
        if (arg3 && (*arg3).value.str.len > 0) {
          add_assoc_zval_ex(hash, (*arg3).value.str.val,
                            (*arg3).value.str.len + 1, element);
        } else {
          add_next_index_zval(hash, element);
        }
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16535
      case 371: {
        incomplete_class_entry = php_create_incomplete_class();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16536
      case 372: {
        index((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16537
      case 373: {
        inet_aton((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16538
      case 374: {
        init_trait_alias_list((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16539
      case 375: {
        instanceof_function((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16540
      case 376: {
        lchmod((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16541
      case 377: {
        lcong48_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16542
      case 378: {
        link((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16543
      case 379: {
        localtime_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16544
      case 380: {
        longjmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16545
      case 381: {
        lrand48_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16546
      case 382: {
        lstat((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16547
      case 383: {
        lutimes((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16548
      case 384: {
        make_digest((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16549
      case 385: {
        make_sha1_digest((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16550
      case 386: {
        mblen((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16551
      case 387: {
        memset(&(basic_globals.serialize), 0,
               sizeof((basic_globals.serialize)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16552
      case 388: {
        memset(&(basic_globals.unserialize), 0,
               sizeof((basic_globals.unserialize)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16553
      case 389: {
        memset(&(basic_globals.url_adapt_state_ex), 0,
               sizeof((basic_globals.url_adapt_state_ex)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16554
      case 390: {
        memset((basic_globals.strtok_table), 0, 256);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16555
      case 391: {
        memset((hash), 0, sizeof(*((hash))));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16556
      case 392: {
        mkdir((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16557
      case 393: {
        mkfifo((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16558
      case 394: {
        mkstemps((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16559
      case 395: {
        mlock((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16560
      case 396: {
        mrand48_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16561
      case 397: {
        munlock((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16562
      case 398: {
        munmap((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16563
      case 399: {
        nanosleep((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16564
      case 400: {
        object_properties_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16565
      case 401: {
        on_exit((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16566
      case 402: {
        open_memstream((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16567
      case 403: {
        open_wmemstream((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16568
      case 404: {
        parse_opts((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16569
      case 405: {
        pathconf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16570
      case 406: {
        php_add_session_var((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16571
      case 407: {
        php_add_tick_function(run_user_tick_functions);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16572
      case 408: {
        php_array_replace_recursive((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16573
      case 409: {
        php_check_open_basedir_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16574
      case 410: {
        php_check_specific_open_basedir((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16575
      case 411: {
        php_copy_file((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16576
      case 412: {
        php_create_empty_class((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16577
      case 413: {
        php_debug_zval_dump((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16578
      case 414: {
        php_dirname((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16579
      case 415: {
        php_error_docref0(((void *)0), (1 << 1L), "%s",
                          strerror((*__errno_location())));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16580
      case 416: {
        php_error_docref0(((void *)0), (1 << 1L), "An unknown error occured");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16581
      case 417: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted GLOBALS variable overwrite");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16582
      case 418: {
        php_error_docref0(((void *)0), (1 << 1L), "Filename cannot be empty!");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16583
      case 419: {
        php_error_docref0(((void *)0), (1 << 1L), "Invalid in_addr value");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16584
      case 420: {
        php_error_docref0(((void *)0), (1 << 1L), "Invalid parameter syntax");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16585
      case 421: {
        php_error_docref0(
            ((void *)0), (1 << 1L),
            "Number of microseconds must be greater than or equal to 0");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16586
      case 422: {
        php_error_docref0(
            ((void *)0), (1 << 1L),
            "Number of seconds must be greater than or equal to 0");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16587
      case 423: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Second argument is not an object or class name");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16588
      case 424: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Sleep until to time is less than current time");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16589
      case 425: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(arg1)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16590
      case 426: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(arg2)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16591
      case 427: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(arg3)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16592
      case 428: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(arr)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16593
      case 429: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(element)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16594
      case 430: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call %s() - function does not exist",
                          (*(hash)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16595
      case 431: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(arg1)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16596
      case 432: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(arg2)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16597
      case 433: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(arg3)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16598
      case 434: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(arr)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16599
      case 435: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(element)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16600
      case 436: {
        php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                          (*(hash)).value.str.val);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16601
      case 437: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Unable to call tick function");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16602
      case 438: {
        php_error_docref0(
            ((void *)0), (1 << 1L),
            "Unable to delete tick function executed at the moment");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16603
      case 439: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "nanoseconds was not in the range 0 to 999 999 999 "
                          "or seconds was negative");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16604
      case 440: {
        php_execute_simple_script((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16605
      case 441: {
        php_free_shutdown_functions();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16606
      case 442: {
        php_getimagetype((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16607
      case 443: {
        php_hash_fetch_ops((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16608
      case 444: {
        php_hash_register_algo((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16609
      case 445: {
        php_html_puts((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16610
      case 446: {
        php_info_html_esc_write((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16611
      case 447: {
        php_info_print_table_end();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16612
      case 448: {
        php_info_print_table_start();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16613
      case 449: {
        php_ini_activate_per_dir_config((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16614
      case 450: {
        php_ini_activate_per_host_config((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16615
      case 451: {
        php_lookup_class_name((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16616
      case 452: {
        php_mkdir((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16617
      case 453: {
        php_multisort_compare((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16618
      case 454: {
        php_output_discard();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16619
      case 455: {
        php_output_end();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16620
      case 456: {
        php_output_get_contents((arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16621
      case 457: {
        php_output_get_contents((arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16622
      case 458: {
        php_output_get_contents((arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16623
      case 459: {
        php_output_get_contents((arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16624
      case 460: {
        php_output_get_contents((element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16625
      case 461: {
        php_output_get_contents((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16626
      case 462: {
        php_output_handler_alias((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16627
      case 463: {
        php_output_handler_started((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16628
      case 464: {
        php_output_start_default();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16629
      case 465: {
        php_output_write((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16630
      case 466: {
        php_output_write_unbuffered((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16631
      case 467: {
        php_printf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16632
      case 468: {
        php_raw_url_decode((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16633
      case 469: {
        php_register_extensions((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16634
      case 470: {
        php_register_pre_request_shutdown((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16635
      case 471: {
        php_register_url_stream_wrapper("data", &php_stream_rfc2397_wrapper);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16636
      case 472: {
        php_register_url_stream_wrapper("file", &php_plain_files_wrapper);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16637
      case 473: {
        php_register_url_stream_wrapper("ftp", &php_stream_ftp_wrapper);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16638
      case 474: {
        php_register_url_stream_wrapper("glob", &php_glob_stream_wrapper);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16639
      case 475: {
        php_register_url_stream_wrapper("http", &php_stream_http_wrapper);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16640
      case 476: {
        php_register_url_stream_wrapper("php", &php_stream_php_wrapper);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16641
      case 477: {
        php_register_url_stream_wrapper((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16642
      case 478: {
        php_register_url_stream_wrapper_volatile((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16643
      case 479: {
        php_session_create_id((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16644
      case 480: {
        php_sprintf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16645
      case 481: {
        php_stream_bucket_append((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16646
      case 482: {
        php_stream_bucket_prepend((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16647
      case 483: {
        php_stream_context_del_link((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16648
      case 484: {
        php_stream_context_set((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16649
      case 485: {
        php_stream_dirent_alphasort((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16650
      case 486: {
        php_stream_dirent_alphasortr((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16651
      case 487: {
        php_stream_encloses((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16652
      case 488: {
        php_stream_filter_append_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16653
      case 489: {
        php_stream_filter_prepend_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16654
      case 490: {
        php_stream_filter_register_factory((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16655
      case 491: {
        php_stream_filter_register_factory_volatile((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16656
      case 492: {
        php_stream_filter_remove((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16657
      case 493: {
        php_stream_from_persistent_id((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16658
      case 494: {
        php_stream_xport_crypto_enable((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16659
      case 495: {
        php_stream_xport_register((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16660
      case 496: {
        php_stream_xport_shutdown((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16661
      case 497: {
        php_stripcslashes((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16662
      case 498: {
        php_stripslashes((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16663
      case 499: {
        php_strtolower((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16664
      case 500: {
        php_strtoupper((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16665
      case 501: {
        php_unregister_url_stream_wrapper("ftp");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16666
      case 502: {
        php_unregister_url_stream_wrapper("http");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16667
      case 503: {
        php_unregister_url_stream_wrapper("php");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16668
      case 504: {
        php_url_decode((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16669
      case 505: {
        php_url_parse_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16670
      case 506: {
        php_var_dump((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16671
      case 507: {
        php_var_export((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16672
      case 508: {
        php_version_compare((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16673
      case 509: {
        php_write((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16674
      case 510: {
        popen((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16675
      case 511: {
        print_op_array((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16676
      case 512: {
        printf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16677
      case 513: {
        psiginfo((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16678
      case 514: {
        putpwent((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16679
      case 515: {
        random_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16680
      case 516: {
        realloc((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16681
      case 517: {
        realpath((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16682
      case 518: {
        realpath_cache_del((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16683
      case 519: {
        rename((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16684
      case 520: {
        rindex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16685
      case 521: {
        rresvport_af((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16686
      case 522: {
        sapi_apply_default_charset((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16687
      case 523: {
        sapi_flush();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16688
      case 524: {
        sapi_getenv((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16689
      case 525: {
        sapi_register_input_filter((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16690
      case 526: {
        scanf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16691
      case 527: {
        seed48_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16692
      case 528: {
        seekdir((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16693
      case 529: {
        setbuf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16694
      case 530: {
        setdomainname((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16695
      case 531: {
        sethostname((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16696
      case 532: {
        setlocale(0, "");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16697
      case 533: {
        setlocale(6, "C");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16698
      case 534: {
        setstate_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16699
      case 535: {
        settimeofday((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16700
      case 536: {
        sigaddset((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16701
      case 537: {
        sigaltstack((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16702
      case 538: {
        sigdelset((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16703
      case 539: {
        sigismember((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16704
      case 540: {
        siglongjmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16705
      case 541: {
        sigstack((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16706
      case 542: {
        sigwait((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16707
      case 543: {
        sigwaitinfo((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16708
      case 544: {
        smart_str_print_long((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16709
      case 545: {
        smart_str_print_unsigned((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16710
      case 546: {
        sprintf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16711
      case 547: {
        sscanf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16712
      case 548: {
        stat((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16713
      case 549: {
        stpcpy((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16714
      case 550: {
        strcasecmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16715
      case 551: {
        strcat((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16716
      case 552: {
        strchr((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16717
      case 553: {
        strcmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16718
      case 554: {
        strcoll((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16719
      case 555: {
        strcpy((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16720
      case 556: {
        strcspn((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16721
      case 557: {
        strndup((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16722
      case 558: {
        strnlen((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16723
      case 559: {
        strpbrk((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16724
      case 560: {
        strrchr((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16725
      case 561: {
        strsep((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16726
      case 562: {
        strspn((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16727
      case 563: {
        strstr((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16728
      case 564: {
        strtod((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16729
      case 565: {
        strtof((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16730
      case 566: {
        strtok((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16731
      case 567: {
        strtold((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16732
      case 568: {
        swscanf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16733
      case 569: {
        symlink((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16734
      case 570: {
        tempnam((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16735
      case 571: {
        timer_gettime((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16736
      case 572: {
        timespec_get((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16737
      case 573: {
        truncate((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16738
      case 574: {
        tsrm_realpath((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16739
      case 575: {
        tzset();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16740
      case 576: {
        umask((basic_globals.umask));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16741
      case 577: {
        user_tick_function_compare((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16742
      case 578: {
        utime((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16743
      case 579: {
        utimes((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16744
      case 580: {
        var_push_dtor((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16745
      case 581: {
        virtual_access((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16746
      case 582: {
        virtual_chdir_file((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16747
      case 583: {
        virtual_chmod((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16748
      case 584: {
        virtual_creat((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16749
      case 585: {
        virtual_filepath((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16750
      case 586: {
        virtual_fopen((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16751
      case 587: {
        virtual_getcwd((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16752
      case 588: {
        virtual_lstat((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16753
      case 589: {
        virtual_mkdir((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16754
      case 590: {
        virtual_open((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16755
      case 591: {
        virtual_popen((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16756
      case 592: {
        virtual_realpath((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16757
      case 593: {
        virtual_rename((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16758
      case 594: {
        virtual_stat((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16759
      case 595: {
        virtual_utime((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16760
      case 596: {
        vprintf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16761
      case 597: {
        vscanf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16762
      case 598: {
        vwprintf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16763
      case 599: {
        vwscanf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16764
      case 600: {
        wcpcpy((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16765
      case 601: {
        wcscasecmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16766
      case 602: {
        wcscat((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16767
      case 603: {
        wcschr((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16768
      case 604: {
        wcscmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16769
      case 605: {
        wcscoll((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16770
      case 606: {
        wcscpy((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16771
      case 607: {
        wcscspn((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16772
      case 608: {
        wcsnlen((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16773
      case 609: {
        wcspbrk((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16774
      case 610: {
        wcsrchr((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16775
      case 611: {
        wcsspn((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16776
      case 612: {
        wcsstr((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16777
      case 613: {
        wcstod((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16778
      case 614: {
        wcstof((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16779
      case 615: {
        wcstold((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16780
      case 616: {
        wctomb((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16781
      case 617: {
        wprintf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16782
      case 618: {
        wscanf((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16783
      case 619: {
        zend_add_literal((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16784
      case 620: {
        zend_add_to_list((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16785
      case 621: {
        zend_atoi((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16786
      case 622: {
        zend_atol((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16787
      case 623: {
        zend_binary_zval_strcasecmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16788
      case 624: {
        zend_binary_zval_strcmp((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16789
      case 625: {
        zend_call_function((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16790
      case 626: {
        zend_check_protected((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16791
      case 627: {
        zend_class_implements((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16792
      case 628: {
        zend_compare_file_handles((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16793
      case 629: {
        zend_copy_constants((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16794
      case 630: {
        zend_delete_global_variable((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16795
      case 631: {
        zend_dirname((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16796
      case 632: {
        zend_disable_class((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16797
      case 633: {
        zend_disable_function((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16798
      case 634: {
        zend_do_begin_class_member_function_call((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16799
      case 635: {
        zend_do_begin_dynamic_function_call((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16800
      case 636: {
        zend_do_begin_function_call((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16801
      case 637: {
        zend_do_begin_namespace((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16802
      case 638: {
        zend_do_begin_new_object((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16803
      case 639: {
        zend_do_begin_qm_op((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16804
      case 640: {
        zend_do_boolean_and_begin((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16805
      case 641: {
        zend_do_boolean_or_begin((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16806
      case 642: {
        zend_do_case_after_statement((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16807
      case 643: {
        zend_do_clone((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16808
      case 644: {
        zend_do_declare_class_constant((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16809
      case 645: {
        zend_do_declare_constant((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16810
      case 646: {
        zend_do_declare_stmt((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16811
      case 647: {
        zend_do_default_before_statement((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16812
      case 648: {
        zend_do_end_class_declaration((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16813
      case 649: {
        zend_do_exit((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16814
      case 650: {
        zend_do_fetch_class((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16815
      case 651: {
        zend_do_fetch_lexical_variable((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16816
      case 652: {
        zend_do_fetch_static_member((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16817
      case 653: {
        zend_do_for_before_statement((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16818
      case 654: {
        zend_do_for_cond((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16819
      case 655: {
        zend_do_foreach_end((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16820
      case 656: {
        zend_do_if_after_statement((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16821
      case 657: {
        zend_do_if_cond((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16822
      case 658: {
        zend_do_implement_interface((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16823
      case 659: {
        zend_do_implement_trait((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16824
      case 660: {
        zend_do_inherit_interfaces((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16825
      case 661: {
        zend_do_inheritance((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16826
      case 662: {
        zend_do_list_end((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16827
      case 663: {
        zend_do_mark_last_catch((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16828
      case 664: {
        zend_do_print((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16829
      case 665: {
        zend_do_return((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16830
      case 666: {
        zend_do_shell_exec((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16831
      case 667: {
        zend_do_verify_access_types((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16832
      case 668: {
        zend_do_while_cond((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16833
      case 669: {
        zend_do_while_end((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16834
      case 670: {
        zend_error(
            (1 << 0L),
            "Cannot call forward_static_call() when no class scope is active");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16835
      case 671: {
        zend_error((1 << 3L), "A non well formed numeric value encountered");
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16836
      case 672: {
        zend_fcall_info_argn((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16837
      case 673: {
        zend_fcall_info_args((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16838
      case 674: {
        zend_fcall_info_args_clear((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16839
      case 675: {
        zend_get_class_fetch_type((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16840
      case 676: {
        zend_get_compiled_variable_value((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16841
      case 677: {
        zend_get_hash_value((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16842
      case 678: {
        zend_hash_apply((basic_globals.user_shutdown_function_names),
                        (apply_func_t)user_shutdown_function_call);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16843
      case 679: {
        zend_hash_apply((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16844
      case 680: {
        zend_hash_destroy(&(basic_globals.putenv_ht));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16845
      case 681: {
        zend_hash_destroy((*(arg1)).value.ht);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16846
      case 682: {
        zend_hash_destroy((*(arg2)).value.ht);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16847
      case 683: {
        zend_hash_destroy((*(arg3)).value.ht);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16848
      case 684: {
        zend_hash_destroy((*(arr)).value.ht);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16849
      case 685: {
        zend_hash_destroy((*(element)).value.ht);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16850
      case 686: {
        zend_hash_destroy((*(hash)).value.ht);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16851
      case 687: {
        zend_hash_destroy((basic_globals.url_adapt_state_ex).tags);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16852
      case 688: {
        zend_hash_destroy((basic_globals.user_shutdown_function_names));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16853
      case 689: {
        zend_hash_func((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16854
      case 690: {
        zend_hash_get_current_key_type_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16855
      case 691: {
        zend_hash_get_pointer((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16856
      case 692: {
        zend_hash_index_exists((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16857
      case 693: {
        zend_hash_internal_pointer_end_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16858
      case 694: {
        zend_hash_internal_pointer_reset_ex((*(arg1)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16859
      case 695: {
        zend_hash_internal_pointer_reset_ex((*(arg2)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16860
      case 696: {
        zend_hash_internal_pointer_reset_ex((*(arg3)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16861
      case 697: {
        zend_hash_internal_pointer_reset_ex((*(arr)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16862
      case 698: {
        zend_hash_internal_pointer_reset_ex((*(element)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16863
      case 699: {
        zend_hash_internal_pointer_reset_ex((*(hash)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16864
      case 700: {
        zend_hash_internal_pointer_reset_ex((**(find_hash)).value.ht,
                                            ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16865
      case 701: {
        zend_hash_internal_pointer_reset_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16866
      case 702: {
        zend_hash_move_backwards_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16867
      case 703: {
        zend_hash_move_forward_ex((*(arg1)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16868
      case 704: {
        zend_hash_move_forward_ex((*(arg2)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16869
      case 705: {
        zend_hash_move_forward_ex((*(arg3)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16870
      case 706: {
        zend_hash_move_forward_ex((*(arr)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16871
      case 707: {
        zend_hash_move_forward_ex((*(element)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16872
      case 708: {
        zend_hash_move_forward_ex((*(hash)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16873
      case 709: {
        zend_hash_move_forward_ex((**(find_hash)).value.ht, ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16874
      case 710: {
        zend_hash_move_forward_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16875
      case 711: {
        zend_hash_reverse_apply((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16876
      case 712: {
        zend_hash_set_pointer((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16877
      case 713: {
        zend_hex_strtod((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16878
      case 714: {
        zend_html_puts((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16879
      case 715: {
        zend_ini_boolean_displayer_cb((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16880
      case 716: {
        zend_ini_color_displayer_cb((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16881
      case 717: {
        zend_ini_open_file_for_scanning((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16882
      case 718: {
        zend_ini_prepare_string_for_scanning((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16883
      case 719: {
        zend_ini_sort_entries();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16884
      case 720: {
        zend_init_list((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16885
      case 721: {
        zend_initialize_class_data((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16886
      case 722: {
        zend_inline_hash_func((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16887
      case 723: {
        zend_is_auto_global((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16888
      case 724: {
        zend_iterator_unwrap((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16889
      case 725: {
        zend_list_insert((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16890
      case 726: {
        zend_llist_add_element((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16891
      case 727: {
        zend_llist_apply((basic_globals.user_tick_functions),
                         (llist_apply_func_t)user_tick_function_call);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16892
      case 728: {
        zend_llist_apply((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16893
      case 729: {
        zend_llist_apply_with_del((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16894
      case 730: {
        zend_llist_copy((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16895
      case 731: {
        zend_llist_destroy((basic_globals.user_tick_functions));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16896
      case 732: {
        zend_llist_get_first_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16897
      case 733: {
        zend_llist_get_last_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16898
      case 734: {
        zend_llist_get_next_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16899
      case 735: {
        zend_llist_get_prev_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16900
      case 736: {
        zend_llist_init((basic_globals.user_tick_functions),
                        sizeof(user_tick_function_entry),
                        (llist_dtor_func_t)user_tick_function_dtor, 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16901
      case 737: {
        zend_llist_prepend_element((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16902
      case 738: {
        zend_llist_sort((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16903
      case 739: {
        zend_make_callable((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16904
      case 740: {
        zend_multibyte_set_script_encoding((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16905
      case 741: {
        zend_multibyte_set_script_encoding_by_string((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16906
      case 742: {
        zend_multibyte_yyinput_again((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16907
      case 743: {
        zend_object_create_proxy((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16908
      case 744: {
        zend_object_std_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16909
      case 745: {
        zend_object_store_set_object((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16910
      case 746: {
        zend_objects_destroy_object((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16911
      case 747: {
        zend_objects_new((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16912
      case 748: {
        zend_objects_store_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16913
      case 749: {
        zend_oct_strtod((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16914
      case 750: {
        zend_prepare_string_for_scanning((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16915
      case 751: {
        zend_print_zval((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16916
      case 752: {
        zend_print_zval_r((arg1), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16917
      case 753: {
        zend_print_zval_r((arg2), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16918
      case 754: {
        zend_print_zval_r((arg3), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16919
      case 755: {
        zend_print_zval_r((arr), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16920
      case 756: {
        zend_print_zval_r((element), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16921
      case 757: {
        zend_print_zval_r((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16922
      case 758: {
        zend_print_zval_r((hash), 0);
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16923
      case 759: {
        zend_ptr_stack_apply((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16924
      case 760: {
        zend_ptr_stack_init_ex((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16925
      case 761: {
        zend_ptr_stack_n_pop((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16926
      case 762: {
        zend_ptr_stack_n_push((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16927
      case 763: {
        zend_ptr_stack_push((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16928
      case 764: {
        zend_quick_get_constant((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16929
      case 765: {
        zend_register_ini_entries((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16930
      case 766: {
        zend_resolve_non_class_name((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16931
      case 767: {
        zend_restore_ini_entry("include_path", sizeof("include_path"),
                               (1 << 4));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16932
      case 768: {
        zend_stack_top((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16933
      case 769: {
        zend_startup((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16934
      case 770: {
        zend_std_get_debug_info((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16935
      case 771: {
        zend_str_tolower((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16936
      case 772: {
        zend_str_tolower_dup((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16937
      case 773: {
        zend_stream_open((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16938
      case 774: {
        zend_string_to_double((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16939
      case 775: {
        zend_strip();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16940
      case 776: {
        zend_strndup((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16941
      case 777: {
        zend_strtod((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16942
      case 778: {
        zend_symtable_update((*(arg1)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16943
      case 779: {
        zend_symtable_update((*(arg2)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16944
      case 780: {
        zend_symtable_update((*(arg3)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16945
      case 781: {
        zend_symtable_update((*(arr)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16946
      case 782: {
        zend_symtable_update((*(element)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16947
      case 783: {
        zend_symtable_update((*(hash)).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16948
      case 784: {
        zend_symtable_update((*arr).value.ht, (*(arg1)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16949
      case 785: {
        zend_symtable_update((*arr).value.ht, (*(arg2)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16950
      case 786: {
        zend_symtable_update((*arr).value.ht, (*(arg3)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16951
      case 787: {
        zend_symtable_update((*arr).value.ht, (*(arr)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16952
      case 788: {
        zend_symtable_update((*arr).value.ht, (*(element)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16953
      case 789: {
        zend_symtable_update((*arr).value.ht, (*(hash)).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16954
      case 790: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(arg1)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16955
      case 791: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(arg2)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16956
      case 792: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(arg3)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16957
      case 793: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(arr)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16958
      case 794: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(element)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16959
      case 795: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*(hash)).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16960
      case 796: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(arg1), sizeof(zval *),
                             ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16961
      case 797: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(arg2), sizeof(zval *),
                             ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16962
      case 798: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(arg3), sizeof(zval *),
                             ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16963
      case 799: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(arr), sizeof(zval *),
                             ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16964
      case 800: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(element),
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16965
      case 801: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &(hash), sizeof(zval *),
                             ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16966
      case 802: {
        zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                             (*arg1).value.str.len + 1, &element,
                             sizeof(zval *), ((void *)0));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16967
      case 803: {
        zend_ts_hash_apply((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16968
      case 804: {
        zend_ts_hash_func((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16969
      case 805: {
        zend_ts_hash_index_exists((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16970
      case 806: {
        zend_ts_hash_reverse_apply((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16971
      case 807: {
        zend_wrong_param_count();
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16972
      case 808: {
        zval_delref_p(((arg1)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16973
      case 809: {
        zval_delref_p(((arg2)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16974
      case 810: {
        zval_delref_p(((arg3)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16975
      case 811: {
        zval_delref_p(((arr)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16976
      case 812: {
        zval_delref_p(((element)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16977
      case 813: {
        zval_delref_p(((hash)));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16978
      case 814: {
        zval_set_isref_to_p((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16979
      case 815: {
        zval_set_refcount_p((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16980
      case 816: {
        zval_unset_isref_p((arg1));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16981
      case 817: {
        zval_unset_isref_p((arg2));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16982
      case 818: {
        zval_unset_isref_p((arg3));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16983
      case 819: {
        zval_unset_isref_p((arr));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16984
      case 820: {
        zval_unset_isref_p((element));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16985
      case 821: {
        zval_unset_isref_p((hash));
        _array_init((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16986
      case 822: {
        zval_update_constant((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16987
      case 823: {
        zval_update_constant_inline_change((hash), 0);

        break;
      }
#endif
#ifdef COMPILE_16988
      case 824: {
        zval_update_constant_no_inline_change((hash), 0);

        break;
      }
#endif
      }
    }

    ALLOC_ZVAL(element);
    // prophet generated patch
    switch (__choose("__ID87")) {
    case 0: {
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#ifdef COMPILE_16989
    case 1: {
      ((arg1))->is_ref__gc = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16990
    case 2: {
      ((arg1))->refcount__gc = 1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16991
    case 3: {
      ((arg2))->is_ref__gc = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16992
    case 4: {
      ((arg2))->refcount__gc = 1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16993
    case 5: {
      ((arg3))->is_ref__gc = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16994
    case 6: {
      ((arg3))->refcount__gc = 1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16995
    case 7: {
      ((arr))->is_ref__gc = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16996
    case 8: {
      ((arr))->refcount__gc = 1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16997
    case 9: {
      ((basic_globals.active_ini_file_section))->is_ref__gc = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16998
    case 10: {
      ((basic_globals.active_ini_file_section))->refcount__gc = 1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_16999
    case 11: {
      ((basic_globals.user_shutdown_function_names)) =
          (HashTable *)_emalloc((sizeof(HashTable)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17000
    case 12: {
      ((element))->is_ref__gc = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17001
    case 13: {
      ((element))->refcount__gc = 1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17002
    case 14: {
      ((hash))->is_ref__gc = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17003
    case 15: {
      ((hash))->refcount__gc = 1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17004
    case 16: {
      (arg1) = *find_hash;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17005
    case 17: {
      (arg2) = *find_hash;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17006
    case 18: {
      (arg3) = *find_hash;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17007
    case 19: {
      (arr) = *find_hash;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17008
    case 20: {
      (basic_globals.active_ini_file_section) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17009
    case 21: {
      (basic_globals.array_walk_fci) = empty_fcall_info;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17010
    case 22: {
      (basic_globals.array_walk_fci_cache) = empty_fcall_info_cache;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17011
    case 23: {
      (basic_globals.incomplete_class) = incomplete_class_entry;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17012
    case 24: {
      (basic_globals.incomplete_class) = php_create_incomplete_class();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17013
    case 25: {
      (basic_globals.left) = -1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17014
    case 26: {
      (basic_globals.locale_string) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17015
    case 27: {
      (basic_globals.mt_rand_is_seeded) = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17016
    case 28: {
      (basic_globals.next) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17017
    case 29: {
      (basic_globals.page_gid) = -1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17018
    case 30: {
      (basic_globals.page_inode) = -1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17019
    case 31: {
      (basic_globals.page_mtime) = -1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17020
    case 32: {
      (basic_globals.page_uid) = -1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17021
    case 33: {
      (basic_globals.rand_is_seeded) = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17022
    case 34: {
      (basic_globals.strtok_last) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17023
    case 35: {
      (basic_globals.strtok_string) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17024
    case 36: {
      (basic_globals.strtok_zval) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17025
    case 37: {
      (basic_globals.umask) = -1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17026
    case 38: {
      (basic_globals.user_compare_fci) = empty_fcall_info;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17027
    case 39: {
      (basic_globals.user_compare_fci_cache) = empty_fcall_info_cache;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17028
    case 40: {
      (basic_globals.user_filter_map) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17029
    case 41: {
      (basic_globals.user_shutdown_function_names) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17030
    case 42: {
      (basic_globals.user_tick_functions) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17031
    case 43: {
      (basic_globals.user_tick_functions) =
          (zend_llist *)_emalloc((sizeof(zend_llist)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17032
    case 44: {
      (element) = *find_hash;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17033
    case 45: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17034
    case 46: {
      (executor_globals.error_reporting) = (1 << 0L);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17035
    case 47: {
      (file_globals.default_context) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17036
    case 48: {
      (file_globals.stream_filters) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17037
    case 49: {
      (file_globals.stream_wrappers) = ((void *)0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17038
    case 50: {
      (hash) = *find_hash;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17039
    case 51: {
      (hash)->is_ref__gc = 0;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17040
    case 52: {
      (hash)->refcount__gc = 1;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17041
    case 53: {
      _array_init(((arg1)), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17042
    case 54: {
      _array_init(((arg2)), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17043
    case 55: {
      _array_init(((arg3)), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17044
    case 56: {
      _array_init(((arr)), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17045
    case 57: {
      _array_init(((basic_globals.active_ini_file_section)), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17046
    case 58: {
      _array_init(((element)), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17047
    case 59: {
      _array_init(((hash)), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17048
    case 60: {
      _array_init((hash), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17049
    case 61: {
      _convert_to_string(((arg1)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17050
    case 62: {
      _convert_to_string(((arg2)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17051
    case 63: {
      _convert_to_string(((arg3)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17052
    case 64: {
      _convert_to_string(((arr)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17053
    case 65: {
      _convert_to_string(((element)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17054
    case 66: {
      _convert_to_string(((hash)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17055
    case 67: {
      _convert_to_string((*(find_hash)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17056
    case 68: {
      _efree(((*(arg1)).value.ht));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17057
    case 69: {
      _efree(((*(arg2)).value.ht));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17058
    case 70: {
      _efree(((*(arg3)).value.ht));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17059
    case 71: {
      _efree(((*(arr)).value.ht));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17060
    case 72: {
      _efree(((*(element)).value.ht));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17061
    case 73: {
      _efree(((*(hash)).value.ht));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17062
    case 74: {
      _efree(((arg1)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17063
    case 75: {
      _efree(((arg2)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17064
    case 76: {
      _efree(((arg3)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17065
    case 77: {
      _efree(((arr)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17066
    case 78: {
      _efree(((basic_globals.locale_string)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17067
    case 79: {
      _efree(((basic_globals.user_shutdown_function_names)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17068
    case 80: {
      _efree(((basic_globals.user_tick_functions)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17069
    case 81: {
      _efree(((element)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17070
    case 82: {
      _efree(((hash)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17071
    case 83: {
      _zend_hash_add_or_update((*(arg1)).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17072
    case 84: {
      _zend_hash_add_or_update((*(arg2)).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17073
    case 85: {
      _zend_hash_add_or_update((*(arg3)).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17074
    case 86: {
      _zend_hash_add_or_update((*(arr)).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17075
    case 87: {
      _zend_hash_add_or_update((*(element)).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17076
    case 88: {
      _zend_hash_add_or_update((*(hash)).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17077
    case 89: {
      _zend_hash_add_or_update((*arr).value.ht, (*(arg1)).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17078
    case 90: {
      _zend_hash_add_or_update((*arr).value.ht, (*(arg2)).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17079
    case 91: {
      _zend_hash_add_or_update((*arr).value.ht, (*(arg3)).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17080
    case 92: {
      _zend_hash_add_or_update((*arr).value.ht, (*(arr)).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17081
    case 93: {
      _zend_hash_add_or_update((*arr).value.ht, (*(element)).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17082
    case 94: {
      _zend_hash_add_or_update((*arr).value.ht, (*(hash)).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17083
    case 95: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*(arg1)).value.str.len + 1, &hash,
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17084
    case 96: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*(arg2)).value.str.len + 1, &hash,
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17085
    case 97: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*(arg3)).value.str.len + 1, &hash,
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17086
    case 98: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*(arr)).value.str.len + 1, &hash,
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17087
    case 99: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*(element)).value.str.len + 1, &hash,
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17088
    case 100: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*(hash)).value.str.len + 1, &hash,
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17089
    case 101: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &(arg1),
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17090
    case 102: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &(arg2),
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17091
    case 103: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &(arg3),
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17092
    case 104: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &(arr),
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17093
    case 105: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &(element),
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17094
    case 106: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &(hash),
                               sizeof(zval *), ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17095
    case 107: {
      _zend_hash_add_or_update((*arr).value.ht, (*arg1).value.str.val,
                               (*arg1).value.str.len + 1, &hash, sizeof(zval *),
                               ((void *)0), (1 << 0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17096
    case 108: {
      _zend_hash_init(((basic_globals.user_shutdown_function_names)), (0),
                      (((void *)0)),
                      ((void (*)(void *))user_shutdown_function_dtor), (0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17097
    case 109: {
      _zval_copy_ctor((&(*(arg1))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17098
    case 110: {
      _zval_copy_ctor((&(*(arg2))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17099
    case 111: {
      _zval_copy_ctor((&(*(arg3))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17100
    case 112: {
      _zval_copy_ctor((&(*(arr))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17101
    case 113: {
      _zval_copy_ctor((&(*(element))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17102
    case 114: {
      _zval_copy_ctor((&(*(hash))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17103
    case 115: {
      _zval_copy_ctor((((arg1))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17104
    case 116: {
      _zval_copy_ctor((((arg2))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17105
    case 117: {
      _zval_copy_ctor((((arg3))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17106
    case 118: {
      _zval_copy_ctor((((arr))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17107
    case 119: {
      _zval_copy_ctor((((element))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17108
    case 120: {
      _zval_copy_ctor((((hash))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17109
    case 121: {
      _zval_copy_ctor(((element)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17110
    case 122: {
      _zval_copy_ctor_func((arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17111
    case 123: {
      _zval_copy_ctor_func((arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17112
    case 124: {
      _zval_copy_ctor_func((arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17113
    case 125: {
      _zval_copy_ctor_func((arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17114
    case 126: {
      _zval_copy_ctor_func((element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17115
    case 127: {
      _zval_copy_ctor_func((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17116
    case 128: {
      _zval_dtor(((arg1)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17117
    case 129: {
      _zval_dtor(((arg2)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17118
    case 130: {
      _zval_dtor(((arg3)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17119
    case 131: {
      _zval_dtor(((arr)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17120
    case 132: {
      _zval_dtor(((element)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17121
    case 133: {
      _zval_dtor(((hash)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17122
    case 134: {
      _zval_dtor((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17123
    case 135: {
      _zval_dtor_func((arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17124
    case 136: {
      _zval_dtor_func((arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17125
    case 137: {
      _zval_dtor_func((arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17126
    case 138: {
      _zval_dtor_func((arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17127
    case 139: {
      _zval_dtor_func((element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17128
    case 140: {
      _zval_dtor_func((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17129
    case 141: {
      _zval_ptr_dtor((&(arg1)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17130
    case 142: {
      _zval_ptr_dtor((&(arg2)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17131
    case 143: {
      _zval_ptr_dtor((&(arg3)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17132
    case 144: {
      _zval_ptr_dtor((&(arr)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17133
    case 145: {
      _zval_ptr_dtor((&(basic_globals.strtok_zval)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17134
    case 146: {
      _zval_ptr_dtor((&(element)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17135
    case 147: {
      _zval_ptr_dtor((&(hash)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17136
    case 148: {
      add_assoc_long_ex((arg1), "line", sizeof("line"),
                        (core_globals.last_error_lineno));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17137
    case 149: {
      add_assoc_long_ex((arg1), "type", sizeof("type"),
                        (core_globals.last_error_type));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17138
    case 150: {
      add_assoc_long_ex((arg2), "line", sizeof("line"),
                        (core_globals.last_error_lineno));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17139
    case 151: {
      add_assoc_long_ex((arg2), "type", sizeof("type"),
                        (core_globals.last_error_type));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17140
    case 152: {
      add_assoc_long_ex((arg3), "line", sizeof("line"),
                        (core_globals.last_error_lineno));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17141
    case 153: {
      add_assoc_long_ex((arg3), "type", sizeof("type"),
                        (core_globals.last_error_type));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17142
    case 154: {
      add_assoc_long_ex((arr), "line", sizeof("line"),
                        (core_globals.last_error_lineno));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17143
    case 155: {
      add_assoc_long_ex((arr), "type", sizeof("type"),
                        (core_globals.last_error_type));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17144
    case 156: {
      add_assoc_long_ex((element), "line", sizeof("line"),
                        (core_globals.last_error_lineno));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17145
    case 157: {
      add_assoc_long_ex((element), "type", sizeof("type"),
                        (core_globals.last_error_type));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17146
    case 158: {
      add_assoc_long_ex((hash), "line", sizeof("line"),
                        (core_globals.last_error_lineno));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17147
    case 159: {
      add_assoc_long_ex((hash), "type", sizeof("type"),
                        (core_globals.last_error_type));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17148
    case 160: {
      add_assoc_null_ex((arg1), "global_value", strlen("global_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17149
    case 161: {
      add_assoc_null_ex((arg1), "local_value", strlen("local_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17150
    case 162: {
      add_assoc_null_ex((arg2), "global_value", strlen("global_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17151
    case 163: {
      add_assoc_null_ex((arg2), "local_value", strlen("local_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17152
    case 164: {
      add_assoc_null_ex((arg3), "global_value", strlen("global_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17153
    case 165: {
      add_assoc_null_ex((arg3), "local_value", strlen("local_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17154
    case 166: {
      add_assoc_null_ex((arr), "global_value", strlen("global_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17155
    case 167: {
      add_assoc_null_ex((arr), "local_value", strlen("local_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17156
    case 168: {
      add_assoc_null_ex((element), "global_value", strlen("global_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17157
    case 169: {
      add_assoc_null_ex((element), "local_value", strlen("local_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17158
    case 170: {
      add_assoc_null_ex((hash), "global_value", strlen("global_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17159
    case 171: {
      add_assoc_null_ex((hash), "local_value", strlen("local_value") + 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17160
    case 172: {
      add_assoc_string_ex(
          (arg1), "file", sizeof("file"),
          (core_globals.last_error_file) ? (core_globals.last_error_file) : "-",
          1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17161
    case 173: {
      add_assoc_string_ex((arg1), "message", sizeof("message"),
                          (core_globals.last_error_message), 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17162
    case 174: {
      add_assoc_string_ex(
          (arg2), "file", sizeof("file"),
          (core_globals.last_error_file) ? (core_globals.last_error_file) : "-",
          1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17163
    case 175: {
      add_assoc_string_ex((arg2), "message", sizeof("message"),
                          (core_globals.last_error_message), 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17164
    case 176: {
      add_assoc_string_ex(
          (arg3), "file", sizeof("file"),
          (core_globals.last_error_file) ? (core_globals.last_error_file) : "-",
          1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17165
    case 177: {
      add_assoc_string_ex((arg3), "message", sizeof("message"),
                          (core_globals.last_error_message), 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17166
    case 178: {
      add_assoc_string_ex(
          (arr), "file", sizeof("file"),
          (core_globals.last_error_file) ? (core_globals.last_error_file) : "-",
          1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17167
    case 179: {
      add_assoc_string_ex((arr), "message", sizeof("message"),
                          (core_globals.last_error_message), 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17168
    case 180: {
      add_assoc_string_ex(
          (element), "file", sizeof("file"),
          (core_globals.last_error_file) ? (core_globals.last_error_file) : "-",
          1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17169
    case 181: {
      add_assoc_string_ex((element), "message", sizeof("message"),
                          (core_globals.last_error_message), 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17170
    case 182: {
      add_assoc_string_ex(
          (hash), "file", sizeof("file"),
          (core_globals.last_error_file) ? (core_globals.last_error_file) : "-",
          1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17171
    case 183: {
      add_assoc_string_ex((hash), "message", sizeof("message"),
                          (core_globals.last_error_message), 1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17172
    case 184: {
      add_assoc_zval_ex((arg1), (*arg3).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17173
    case 185: {
      add_assoc_zval_ex((arg2), (*arg3).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17174
    case 186: {
      add_assoc_zval_ex((arg3), (*arg3).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17175
    case 187: {
      add_assoc_zval_ex((arr), (*arg3).value.str.val, (*arg3).value.str.len + 1,
                        element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17176
    case 188: {
      add_assoc_zval_ex((element), (*arg3).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17177
    case 189: {
      add_assoc_zval_ex((hash), (*arg3).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17178
    case 190: {
      add_assoc_zval_ex(hash, (*(arg1)).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17179
    case 191: {
      add_assoc_zval_ex(hash, (*(arg2)).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17180
    case 192: {
      add_assoc_zval_ex(hash, (*(arg3)).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17181
    case 193: {
      add_assoc_zval_ex(hash, (*(arr)).value.str.val, (*arg3).value.str.len + 1,
                        element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17182
    case 194: {
      add_assoc_zval_ex(hash, (*(element)).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17183
    case 195: {
      add_assoc_zval_ex(hash, (*(hash)).value.str.val,
                        (*arg3).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17184
    case 196: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val,
                        (*(arg1)).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17185
    case 197: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val,
                        (*(arg2)).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17186
    case 198: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val,
                        (*(arg3)).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17187
    case 199: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val, (*(arr)).value.str.len + 1,
                        element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17188
    case 200: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val,
                        (*(element)).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17189
    case 201: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val,
                        (*(hash)).value.str.len + 1, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17190
    case 202: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val, (*arg3).value.str.len + 1,
                        (arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17191
    case 203: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val, (*arg3).value.str.len + 1,
                        (arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17192
    case 204: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val, (*arg3).value.str.len + 1,
                        (arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17193
    case 205: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val, (*arg3).value.str.len + 1,
                        (arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17194
    case 206: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val, (*arg3).value.str.len + 1,
                        (element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17195
    case 207: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val, (*arg3).value.str.len + 1,
                        (hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17196
    case 208: {
      add_assoc_zval_ex(hash, (*arg3).value.str.val, (*arg3).value.str.len + 1,
                        element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17197
    case 209: {
      add_next_index_zval((arg1), element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17198
    case 210: {
      add_next_index_zval((arg2), element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17199
    case 211: {
      add_next_index_zval((arg3), element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17200
    case 212: {
      add_next_index_zval((arr), element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17201
    case 213: {
      add_next_index_zval((element), element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17202
    case 214: {
      add_next_index_zval((hash), element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17203
    case 215: {
      add_next_index_zval(hash, (arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17204
    case 216: {
      add_next_index_zval(hash, (arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17205
    case 217: {
      add_next_index_zval(hash, (arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17206
    case 218: {
      add_next_index_zval(hash, (arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17207
    case 219: {
      add_next_index_zval(hash, (element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17208
    case 220: {
      add_next_index_zval(hash, (hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17209
    case 221: {
      add_next_index_zval(hash, element);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17210
    case 222: {
      basic_globals_ctor(&basic_globals);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17211
    case 223: {
      basic_globals_dtor(&basic_globals);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17212
    case 224: {
      convert_to_array(*(find_hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17213
    case 225: {
      convert_to_boolean((arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17214
    case 226: {
      convert_to_boolean((arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17215
    case 227: {
      convert_to_boolean((arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17216
    case 228: {
      convert_to_boolean((arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17217
    case 229: {
      convert_to_boolean((element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17218
    case 230: {
      convert_to_boolean((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17219
    case 231: {
      exit(1);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17220
    case 232: {
      free((basic_globals.url_adapt_state_ex).tags);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17221
    case 233: {
      gc_remove_zval_from_buffer((arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17222
    case 234: {
      gc_remove_zval_from_buffer((arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17223
    case 235: {
      gc_remove_zval_from_buffer((arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17224
    case 236: {
      gc_remove_zval_from_buffer((arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17225
    case 237: {
      gc_remove_zval_from_buffer((element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17226
    case 238: {
      gc_remove_zval_from_buffer((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17227
    case 239: {
      gc_zval_check_possible_root(((arg1)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17228
    case 240: {
      gc_zval_check_possible_root(((arg2)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17229
    case 241: {
      gc_zval_check_possible_root(((arg3)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17230
    case 242: {
      gc_zval_check_possible_root(((arr)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17231
    case 243: {
      gc_zval_check_possible_root(((element)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17232
    case 244: {
      gc_zval_check_possible_root(((hash)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17233
    case 245: {
      gc_zval_possible_root((arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17234
    case 246: {
      gc_zval_possible_root((arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17235
    case 247: {
      gc_zval_possible_root((arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17236
    case 248: {
      gc_zval_possible_root((arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17237
    case 249: {
      gc_zval_possible_root((element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17238
    case 250: {
      gc_zval_possible_root((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17239
    case 251: {
      hash = *(find_hash);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17240
    case 252: {
      hash = *find_hash;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17241
    case 253: {
      i_zval_ptr_dtor((arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17242
    case 254: {
      i_zval_ptr_dtor((arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17243
    case 255: {
      i_zval_ptr_dtor((arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17244
    case 256: {
      i_zval_ptr_dtor((arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17245
    case 257: {
      i_zval_ptr_dtor((element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17246
    case 258: {
      i_zval_ptr_dtor((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17247
    case 259: {
      if (!(basic_globals.user_shutdown_function_names)) {
        ((basic_globals.user_shutdown_function_names)) =
            (HashTable *)_emalloc((sizeof(HashTable)));
        _zend_hash_init(((basic_globals.user_shutdown_function_names)), (0),
                        (((void *)0)),
                        ((void (*)(void *))user_shutdown_function_dtor), (0));
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17248
    case 260: {
      if (!(basic_globals.user_tick_functions)) {
        (basic_globals.user_tick_functions) =
            (zend_llist *)_emalloc((sizeof(zend_llist)));
        zend_llist_init((basic_globals.user_tick_functions),
                        sizeof(user_tick_function_entry),
                        (llist_dtor_func_t)user_tick_function_dtor, 0);
        php_add_tick_function(run_user_tick_functions);
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17249
    case 261: {
      if (!(basic_globals.user_tick_functions)) {
        return;
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17250
    case 262: {
      if (!(executor_globals.active_op_array)->scope) {
        zend_error(
            (1 << 0L),
            "Cannot call forward_static_call() when no class scope is active");
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17251
    case 263: {
      if (!__is_neg("87-263", 1, (int *){callback_type}, 0, 0, 7,
                    (void **){arg2, element, arg3, hash, find_hash, arg1, arr},
                    0, 0, 8, "arg2", "element", "arg3", "hash", "find_hash",
                    "arg1", "arr", "callback_type"))
        do {
          do {
            (element)->value = (*(&arg2))->value;
            (*element).type = (**(&arg2)).type;
          } while (0);
          zval_set_refcount_p(element, 1);
          zval_unset_isref_p(element);
        } while (0);

      break;
    }
#endif
#ifdef COMPILE_17252
    case 264: {
      if ((*__errno_location()) == 22) {
        php_error_docref0(((void *)0), (1 << 1L),
                          "nanoseconds was not in the range 0 to 999 999 999 "
                          "or seconds was negative");
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17253
    case 265: {
      if ((basic_globals.locale_string) &&
          !((((basic_globals.locale_string)) >=
             (compiler_globals.interned_strings_start)) &&
            (((basic_globals.locale_string)) <
             (compiler_globals.interned_strings_end)))) {
        _efree(((basic_globals.locale_string)));
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17254
    case 266: {
      if ((basic_globals.strtok_zval)) {
        _zval_ptr_dtor((&(basic_globals.strtok_zval)));
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17255
    case 267: {
      if ((basic_globals.umask) != -1) {
        umask((basic_globals.umask));
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17256
    case 268: {
      if ((basic_globals.url_adapt_state_ex).tags) {
        zend_hash_destroy((basic_globals.url_adapt_state_ex).tags);
        free((basic_globals.url_adapt_state_ex).tags);
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17257
    case 269: {
      if ((basic_globals.user_tick_functions)) {
        zend_llist_destroy((basic_globals.user_tick_functions));
        _efree(((basic_globals.user_tick_functions)));
        (basic_globals.user_tick_functions) = ((void *)0);
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17258
    case 270: {
      if (__is_neg("87-270", 1, (int *){callback_type}, 0, 0, 7,
                   (void **){arg2, element, arg3, hash, find_hash, arg1, arr},
                   0, 0, 8, "arg2", "element", "arg3", "hash", "find_hash",
                   "arg1", "arr", "callback_type"))
        return;
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17259
    case 271: {
      if (_zend_hash_init((&(basic_globals.putenv_ht)), (1), (((void *)0)),
                          ((void (*)(void *))php_putenv_destructor),
                          (0)) == -1) {
        return -1;
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17260
    case 272: {
      if (arg3 && (*arg3).value.str.len > 0) {
        add_assoc_zval_ex(hash, (*arg3).value.str.val,
                          (*arg3).value.str.len + 1, element);
      } else {
        add_next_index_zval(hash, element);
      }
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17261
    case 273: {
      incomplete_class_entry = php_create_incomplete_class();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17262
    case 274: {
      memset(&(**(&arg2)), 0, sizeof(*(&(**(&arg2)))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17263
    case 275: {
      memset(&(*element), 0, sizeof(*(&(*element))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17264
    case 276: {
      memset(&(basic_globals.serialize), 0, sizeof((basic_globals.serialize)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17265
    case 277: {
      memset(&(basic_globals.unserialize), 0,
             sizeof((basic_globals.unserialize)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17266
    case 278: {
      memset(&(basic_globals.url_adapt_state_ex), 0,
             sizeof((basic_globals.url_adapt_state_ex)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17267
    case 279: {
      memset((*(&arg2)), 0, sizeof(*((*(&arg2)))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17268
    case 280: {
      memset((basic_globals.strtok_table), 0, 256);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17269
    case 281: {
      memset((element), 0, sizeof(*((element))));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17270
    case 282: {
      php_add_tick_function(run_user_tick_functions);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17271
    case 283: {
      php_error_docref0(((void *)0), (1 << 1L), "%s",
                        strerror((*__errno_location())));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17272
    case 284: {
      php_error_docref0(((void *)0), (1 << 1L), "An unknown error occured");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17273
    case 285: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Attempted GLOBALS variable overwrite");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17274
    case 286: {
      php_error_docref0(((void *)0), (1 << 1L), "Filename cannot be empty!");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17275
    case 287: {
      php_error_docref0(((void *)0), (1 << 1L), "Invalid in_addr value");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17276
    case 288: {
      php_error_docref0(((void *)0), (1 << 1L), "Invalid parameter syntax");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17277
    case 289: {
      php_error_docref0(
          ((void *)0), (1 << 1L),
          "Number of microseconds must be greater than or equal to 0");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17278
    case 290: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Number of seconds must be greater than or equal to 0");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17279
    case 291: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Second argument is not an object or class name");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17280
    case 292: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Sleep until to time is less than current time");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17281
    case 293: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Unable to call %s() - function does not exist",
                        (*(arg1)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17282
    case 294: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Unable to call %s() - function does not exist",
                        (*(arg2)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17283
    case 295: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Unable to call %s() - function does not exist",
                        (*(arg3)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17284
    case 296: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Unable to call %s() - function does not exist",
                        (*(arr)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17285
    case 297: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Unable to call %s() - function does not exist",
                        (*(element)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17286
    case 298: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "Unable to call %s() - function does not exist",
                        (*(hash)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17287
    case 299: {
      php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                        (*(arg1)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17288
    case 300: {
      php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                        (*(arg2)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17289
    case 301: {
      php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                        (*(arg3)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17290
    case 302: {
      php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                        (*(arr)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17291
    case 303: {
      php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                        (*(element)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17292
    case 304: {
      php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                        (*(hash)).value.str.val);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17293
    case 305: {
      php_error_docref0(((void *)0), (1 << 1L), "Unable to call tick function");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17294
    case 306: {
      php_error_docref0(
          ((void *)0), (1 << 1L),
          "Unable to delete tick function executed at the moment");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17295
    case 307: {
      php_error_docref0(((void *)0), (1 << 1L),
                        "nanoseconds was not in the range 0 to 999 999 999 or "
                        "seconds was negative");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17296
    case 308: {
      php_free_shutdown_functions();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17297
    case 309: {
      php_info_print_table_end();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17298
    case 310: {
      php_info_print_table_start();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17299
    case 311: {
      php_output_discard();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17300
    case 312: {
      php_output_end();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17301
    case 313: {
      php_output_get_contents((arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17302
    case 314: {
      php_output_get_contents((arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17303
    case 315: {
      php_output_get_contents((arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17304
    case 316: {
      php_output_get_contents((arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17305
    case 317: {
      php_output_get_contents((element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17306
    case 318: {
      php_output_get_contents((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17307
    case 319: {
      php_output_start_default();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17308
    case 320: {
      php_register_url_stream_wrapper("data", &php_stream_rfc2397_wrapper);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17309
    case 321: {
      php_register_url_stream_wrapper("file", &php_plain_files_wrapper);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17310
    case 322: {
      php_register_url_stream_wrapper("ftp", &php_stream_ftp_wrapper);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17311
    case 323: {
      php_register_url_stream_wrapper("glob", &php_glob_stream_wrapper);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17312
    case 324: {
      php_register_url_stream_wrapper("http", &php_stream_http_wrapper);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17313
    case 325: {
      php_register_url_stream_wrapper("php", &php_stream_php_wrapper);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17314
    case 326: {
      php_unregister_url_stream_wrapper("ftp");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17315
    case 327: {
      php_unregister_url_stream_wrapper("http");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17316
    case 328: {
      php_unregister_url_stream_wrapper("php");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17317
    case 329: {
      sapi_flush();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17318
    case 330: {
      setlocale(0, "");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17319
    case 331: {
      setlocale(6, "C");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17320
    case 332: {
      tzset();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17321
    case 333: {
      umask((basic_globals.umask));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17322
    case 334: {
      zend_error(
          (1 << 0L),
          "Cannot call forward_static_call() when no class scope is active");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17323
    case 335: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17324
    case 336: {
      zend_hash_apply((basic_globals.user_shutdown_function_names),
                      (apply_func_t)user_shutdown_function_call);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17325
    case 337: {
      zend_hash_destroy(&(basic_globals.putenv_ht));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17326
    case 338: {
      zend_hash_destroy((*(arg1)).value.ht);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17327
    case 339: {
      zend_hash_destroy((*(arg2)).value.ht);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17328
    case 340: {
      zend_hash_destroy((*(arg3)).value.ht);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17329
    case 341: {
      zend_hash_destroy((*(arr)).value.ht);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17330
    case 342: {
      zend_hash_destroy((*(element)).value.ht);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17331
    case 343: {
      zend_hash_destroy((*(hash)).value.ht);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17332
    case 344: {
      zend_hash_destroy((basic_globals.url_adapt_state_ex).tags);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17333
    case 345: {
      zend_hash_destroy((basic_globals.user_shutdown_function_names));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17334
    case 346: {
      zend_hash_internal_pointer_reset_ex((*(arg1)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17335
    case 347: {
      zend_hash_internal_pointer_reset_ex((*(arg2)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17336
    case 348: {
      zend_hash_internal_pointer_reset_ex((*(arg3)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17337
    case 349: {
      zend_hash_internal_pointer_reset_ex((*(arr)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17338
    case 350: {
      zend_hash_internal_pointer_reset_ex((*(element)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17339
    case 351: {
      zend_hash_internal_pointer_reset_ex((*(hash)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17340
    case 352: {
      zend_hash_internal_pointer_reset_ex((**(find_hash)).value.ht,
                                          ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17341
    case 353: {
      zend_hash_move_forward_ex((*(arg1)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17342
    case 354: {
      zend_hash_move_forward_ex((*(arg2)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17343
    case 355: {
      zend_hash_move_forward_ex((*(arg3)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17344
    case 356: {
      zend_hash_move_forward_ex((*(arr)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17345
    case 357: {
      zend_hash_move_forward_ex((*(element)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17346
    case 358: {
      zend_hash_move_forward_ex((*(hash)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17347
    case 359: {
      zend_hash_move_forward_ex((**(find_hash)).value.ht, ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17348
    case 360: {
      zend_ini_sort_entries();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17349
    case 361: {
      zend_llist_apply((basic_globals.user_tick_functions),
                       (llist_apply_func_t)user_tick_function_call);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17350
    case 362: {
      zend_llist_destroy((basic_globals.user_tick_functions));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17351
    case 363: {
      zend_llist_init((basic_globals.user_tick_functions),
                      sizeof(user_tick_function_entry),
                      (llist_dtor_func_t)user_tick_function_dtor, 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17352
    case 364: {
      zend_print_zval_r((arg1), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17353
    case 365: {
      zend_print_zval_r((arg2), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17354
    case 366: {
      zend_print_zval_r((arg3), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17355
    case 367: {
      zend_print_zval_r((arr), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17356
    case 368: {
      zend_print_zval_r((element), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17357
    case 369: {
      zend_print_zval_r((hash), 0);
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17358
    case 370: {
      zend_restore_ini_entry("include_path", sizeof("include_path"), (1 << 4));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17359
    case 371: {
      zend_strip();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17360
    case 372: {
      zend_symtable_update((*(arg1)).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17361
    case 373: {
      zend_symtable_update((*(arg2)).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17362
    case 374: {
      zend_symtable_update((*(arg3)).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17363
    case 375: {
      zend_symtable_update((*(arr)).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17364
    case 376: {
      zend_symtable_update((*(element)).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17365
    case 377: {
      zend_symtable_update((*(hash)).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17366
    case 378: {
      zend_symtable_update((*arr).value.ht, (*(arg1)).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17367
    case 379: {
      zend_symtable_update((*arr).value.ht, (*(arg2)).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17368
    case 380: {
      zend_symtable_update((*arr).value.ht, (*(arg3)).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17369
    case 381: {
      zend_symtable_update((*arr).value.ht, (*(arr)).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17370
    case 382: {
      zend_symtable_update((*arr).value.ht, (*(element)).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17371
    case 383: {
      zend_symtable_update((*arr).value.ht, (*(hash)).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17372
    case 384: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*(arg1)).value.str.len + 1, &element,
                           sizeof(zval *), ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17373
    case 385: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*(arg2)).value.str.len + 1, &element,
                           sizeof(zval *), ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17374
    case 386: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*(arg3)).value.str.len + 1, &element,
                           sizeof(zval *), ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17375
    case 387: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*(arr)).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17376
    case 388: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*(element)).value.str.len + 1, &element,
                           sizeof(zval *), ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17377
    case 389: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*(hash)).value.str.len + 1, &element,
                           sizeof(zval *), ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17378
    case 390: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &(arg1), sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17379
    case 391: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &(arg2), sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17380
    case 392: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &(arg3), sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17381
    case 393: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &(arr), sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17382
    case 394: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &(element),
                           sizeof(zval *), ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17383
    case 395: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &(hash), sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17384
    case 396: {
      zend_symtable_update((*arr).value.ht, (*arg1).value.str.val,
                           (*arg1).value.str.len + 1, &element, sizeof(zval *),
                           ((void *)0));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17385
    case 397: {
      zend_wrong_param_count();
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17386
    case 398: {
      zval_delref_p(((arg1)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17387
    case 399: {
      zval_delref_p(((arg2)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17388
    case 400: {
      zval_delref_p(((arg3)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17389
    case 401: {
      zval_delref_p(((arr)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17390
    case 402: {
      zval_delref_p(((element)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17391
    case 403: {
      zval_delref_p(((hash)));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17392
    case 404: {
      zval_unset_isref_p((arg1));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17393
    case 405: {
      zval_unset_isref_p((arg2));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17394
    case 406: {
      zval_unset_isref_p((arg3));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17395
    case 407: {
      zval_unset_isref_p((arr));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17396
    case 408: {
      zval_unset_isref_p((element));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_17397
    case 409: {
      zval_unset_isref_p((hash));
      do {
        do {
          (element)->value = (*(&arg2))->value;
          (*element).type = (**(&arg2)).type;
        } while (0);
        zval_set_refcount_p(element, 1);
        zval_unset_isref_p(element);
      } while (0);

      break;
    }
#endif
    }

    if (arg3 && Z_STRLEN_P(arg3) > 0) {
      add_assoc_zval_ex(hash, Z_STRVAL_P(arg3), Z_STRLEN_P(arg3) + 1, element);
    } else {
      add_next_index_zval(hash, element);
    }
  } break;

  case ZEND_INI_PARSER_SECTION:
    break;
  }
}
/* }}} */

/* {{{ php_ini_parser_cb_with_sections
 */
static void php_ini_parser_cb_with_sections(zval *arg1, zval *arg2, zval *arg3,
                                            int callback_type,
                                            zval *arr TSRMLS_DC) {
  if (callback_type == ZEND_INI_PARSER_SECTION) {
    MAKE_STD_ZVAL(BG(active_ini_file_section));
    array_init(BG(active_ini_file_section));
    zend_symtable_update(Z_ARRVAL_P(arr), Z_STRVAL_P(arg1),
                         Z_STRLEN_P(arg1) + 1, &BG(active_ini_file_section),
                         sizeof(zval *), NULL);
  } else if (arg2) {
    zval *active_arr;

    if (BG(active_ini_file_section)) {
      active_arr = BG(active_ini_file_section);
    } else {
      active_arr = arr;
    }

    php_simple_ini_parser_cb(arg1, arg2, arg3, callback_type,
                             active_arr TSRMLS_CC);
  }
}
/* }}} */

/* {{{ proto array parse_ini_file(string filename [, bool process_sections [,
   int scanner_mode]]) Parse configuration file */
PHP_FUNCTION(parse_ini_file) {
  char *filename = NULL;
  int filename_len = 0;
  zend_bool process_sections = 0;
  long scanner_mode = ZEND_INI_SCANNER_NORMAL;
  zend_file_handle fh;
  zend_ini_parser_cb_t ini_parser_cb;

  // prophet generated patch
  switch (__choose("__ID88")) {
  case 0: {
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#ifdef COMPILE_17398
  case 1: {
    ((basic_globals.active_ini_file_section))->is_ref__gc = 0;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17399
  case 2: {
    ((basic_globals.active_ini_file_section))->refcount__gc = 1;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17400
  case 3: {
    ((basic_globals.user_shutdown_function_names)) =
        (HashTable *)_emalloc((sizeof(HashTable)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17401
  case 4: {
    (basic_globals.active_ini_file_section) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17402
  case 5: {
    (basic_globals.array_walk_fci) = empty_fcall_info;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17403
  case 6: {
    (basic_globals.array_walk_fci_cache) = empty_fcall_info_cache;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17404
  case 7: {
    (basic_globals.incomplete_class) = incomplete_class_entry;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17405
  case 8: {
    (basic_globals.incomplete_class) = php_create_incomplete_class();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17406
  case 9: {
    (basic_globals.left) = -1;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17407
  case 10: {
    (basic_globals.locale_string) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17408
  case 11: {
    (basic_globals.mt_rand_is_seeded) = 0;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17409
  case 12: {
    (basic_globals.next) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17410
  case 13: {
    (basic_globals.page_gid) = -1;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17411
  case 14: {
    (basic_globals.page_inode) = -1;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17412
  case 15: {
    (basic_globals.page_mtime) = -1;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17413
  case 16: {
    (basic_globals.page_uid) = -1;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17414
  case 17: {
    (basic_globals.rand_is_seeded) = 0;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17415
  case 18: {
    (basic_globals.strtok_last) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17416
  case 19: {
    (basic_globals.strtok_string) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17417
  case 20: {
    (basic_globals.strtok_zval) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17418
  case 21: {
    (basic_globals.umask) = -1;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17419
  case 22: {
    (basic_globals.user_compare_fci) = empty_fcall_info;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17420
  case 23: {
    (basic_globals.user_compare_fci_cache) = empty_fcall_info_cache;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17421
  case 24: {
    (basic_globals.user_filter_map) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17422
  case 25: {
    (basic_globals.user_shutdown_function_names) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17423
  case 26: {
    (basic_globals.user_tick_functions) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17424
  case 27: {
    (basic_globals.user_tick_functions) =
        (zend_llist *)_emalloc((sizeof(zend_llist)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17425
  case 28: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17426
  case 29: {
    (executor_globals.error_reporting) = (1 << 0L);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17427
  case 30: {
    (file_globals.default_context) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17428
  case 31: {
    (file_globals.stream_filters) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17429
  case 32: {
    (file_globals.stream_wrappers) = ((void *)0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17430
  case 33: {
    (filename)++;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17431
  case 34: {
    (ini_parser_cb) = (zend_ini_parser_cb_t)php_ini_parser_cb_with_sections;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17432
  case 35: {
    (ini_parser_cb) = (zend_ini_parser_cb_t)php_simple_ini_parser_cb;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17433
  case 36: {
    _array_init(((basic_globals.active_ini_file_section)), 0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17434
  case 37: {
    _array_init(((return_value)), 0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17435
  case 38: {
    _array_init(((this_ptr)), 0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17436
  case 39: {
    _array_init((return_value), 0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17437
  case 40: {
    _convert_to_string(((return_value)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17438
  case 41: {
    _convert_to_string(((this_ptr)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17439
  case 42: {
    _convert_to_string((*(return_value_ptr)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17440
  case 43: {
    _efree(((*(return_value)).value.ht));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17441
  case 44: {
    _efree(((*(this_ptr)).value.ht));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17442
  case 45: {
    _efree(((*return_value).value.ht));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17443
  case 46: {
    _efree(((basic_globals.locale_string)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17444
  case 47: {
    _efree(((basic_globals.user_shutdown_function_names)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17445
  case 48: {
    _efree(((basic_globals.user_tick_functions)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17446
  case 49: {
    _efree(((filename)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17447
  case 50: {
    _efree(((return_value)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17448
  case 51: {
    _efree(((this_ptr)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17449
  case 52: {
    _zend_hash_init(((basic_globals.user_shutdown_function_names)), (0),
                    (((void *)0)),
                    ((void (*)(void *))user_shutdown_function_dtor), (0));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17450
  case 53: {
    _zval_copy_ctor((&(*(return_value))));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17451
  case 54: {
    _zval_copy_ctor((&(*(this_ptr))));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17452
  case 55: {
    _zval_copy_ctor((((return_value))));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17453
  case 56: {
    _zval_copy_ctor((((this_ptr))));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17454
  case 57: {
    _zval_copy_ctor_func((return_value));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17455
  case 58: {
    _zval_copy_ctor_func((this_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17456
  case 59: {
    _zval_dtor(((return_value)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17457
  case 60: {
    _zval_dtor(((this_ptr)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17458
  case 61: {
    _zval_dtor_func((return_value));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17459
  case 62: {
    _zval_dtor_func((this_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17460
  case 63: {
    _zval_ptr_dtor((&(basic_globals.strtok_zval)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17461
  case 64: {
    _zval_ptr_dtor((&(return_value)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17462
  case 65: {
    _zval_ptr_dtor((&(this_ptr)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17463
  case 66: {
    add_assoc_long_ex((return_value), "line", sizeof("line"),
                      (core_globals.last_error_lineno));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17464
  case 67: {
    add_assoc_long_ex((return_value), "type", sizeof("type"),
                      (core_globals.last_error_type));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17465
  case 68: {
    add_assoc_long_ex((this_ptr), "line", sizeof("line"),
                      (core_globals.last_error_lineno));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17466
  case 69: {
    add_assoc_long_ex((this_ptr), "type", sizeof("type"),
                      (core_globals.last_error_type));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17467
  case 70: {
    add_assoc_null_ex((return_value), "global_value",
                      strlen("global_value") + 1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17468
  case 71: {
    add_assoc_null_ex((return_value), "local_value", strlen("local_value") + 1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17469
  case 72: {
    add_assoc_null_ex((this_ptr), "global_value", strlen("global_value") + 1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17470
  case 73: {
    add_assoc_null_ex((this_ptr), "local_value", strlen("local_value") + 1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17471
  case 74: {
    add_assoc_string_ex(
        (return_value), "file", sizeof("file"),
        (core_globals.last_error_file) ? (core_globals.last_error_file) : "-",
        1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17472
  case 75: {
    add_assoc_string_ex((return_value), "message", sizeof("message"),
                        (core_globals.last_error_message), 1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17473
  case 76: {
    add_assoc_string_ex(
        (this_ptr), "file", sizeof("file"),
        (core_globals.last_error_file) ? (core_globals.last_error_file) : "-",
        1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17474
  case 77: {
    add_assoc_string_ex((this_ptr), "message", sizeof("message"),
                        (core_globals.last_error_message), 1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17475
  case 78: {
    basic_globals_ctor(&basic_globals);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17476
  case 79: {
    basic_globals_dtor(&basic_globals);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17477
  case 80: {
    convert_to_array(*(return_value_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17478
  case 81: {
    convert_to_boolean((return_value));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17479
  case 82: {
    convert_to_boolean((this_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17480
  case 83: {
    exit(1);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17481
  case 84: {
    fh.filename = (filename);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17482
  case 85: {
    fh.filename = filename;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17483
  case 86: {
    fh.type = (ZEND_HANDLE_FD);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17484
  case 87: {
    fh.type = (ZEND_HANDLE_FILENAME);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17485
  case 88: {
    fh.type = (ZEND_HANDLE_FP);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17486
  case 89: {
    fh.type = (ZEND_HANDLE_MAPPED);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17487
  case 90: {
    fh.type = (ZEND_HANDLE_STREAM);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17488
  case 91: {
    fh.type = ZEND_HANDLE_FILENAME;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17489
  case 92: {
    free((basic_globals.url_adapt_state_ex).tags);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17490
  case 93: {
    gc_remove_zval_from_buffer((return_value));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17491
  case 94: {
    gc_remove_zval_from_buffer((this_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17492
  case 95: {
    gc_zval_check_possible_root(((return_value)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17493
  case 96: {
    gc_zval_check_possible_root(((this_ptr)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17494
  case 97: {
    gc_zval_possible_root((return_value));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17495
  case 98: {
    gc_zval_possible_root((this_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17496
  case 99: {
    i_zval_ptr_dtor((return_value));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17497
  case 100: {
    i_zval_ptr_dtor((this_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17498
  case 101: {
    if (!(basic_globals.user_shutdown_function_names)) {
      ((basic_globals.user_shutdown_function_names)) =
          (HashTable *)_emalloc((sizeof(HashTable)));
      _zend_hash_init(((basic_globals.user_shutdown_function_names)), (0),
                      (((void *)0)),
                      ((void (*)(void *))user_shutdown_function_dtor), (0));
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17499
  case 102: {
    if (!(basic_globals.user_tick_functions)) {
      (basic_globals.user_tick_functions) =
          (zend_llist *)_emalloc((sizeof(zend_llist)));
      zend_llist_init((basic_globals.user_tick_functions),
                      sizeof(user_tick_function_entry),
                      (llist_dtor_func_t)user_tick_function_dtor, 0);
      php_add_tick_function(run_user_tick_functions);
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17500
  case 103: {
    if (!(basic_globals.user_tick_functions)) {
      return;
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17501
  case 104: {
    if (!(executor_globals.active_op_array)->scope) {
      zend_error(
          (1 << 0L),
          "Cannot call forward_static_call() when no class scope is active");
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17502
  case 105: {
    if (!__is_neg("88-105", 2, (int *){ht, filename_len}, 0, 0, 3,
                  (void **){filename, return_value, ini_parser_cb}, 0, 0, 7,
                  "ht", "filename", "filename_len", "process_sections",
                  "scanner_mode", "return_value", "ini_parser_cb") &&
        (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                               &process_sections, &scanner_mode) == -1)) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17503
  case 106: {
    if (!__is_neg("88-106", 2, (int *){ht, filename_len}, 0, 0, 3,
                  (void **){filename, return_value, ini_parser_cb}, 0, 0, 7,
                  "ht", "filename", "filename_len", "process_sections",
                  "scanner_mode", "return_value", "ini_parser_cb"))
      if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                                &process_sections, &scanner_mode) == -1) {
        {
          do {
            zval *__z = (return_value);
            (*__z).value.lval = ((0) != 0);
            (*__z).type = 3;
          } while (0);
          return;
        };
      }

    break;
  }
#endif
#ifdef COMPILE_17504
  case 107: {
    if ((*__errno_location()) == 22) {
      php_error_docref0(((void *)0), (1 << 1L),
                        "nanoseconds was not in the range 0 to 999 999 999 or "
                        "seconds was negative");
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17505
  case 108: {
    if ((basic_globals.locale_string) &&
        !((((basic_globals.locale_string)) >=
           (compiler_globals.interned_strings_start)) &&
          (((basic_globals.locale_string)) <
           (compiler_globals.interned_strings_end)))) {
      _efree(((basic_globals.locale_string)));
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17506
  case 109: {
    if ((basic_globals.strtok_zval)) {
      _zval_ptr_dtor((&(basic_globals.strtok_zval)));
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17507
  case 110: {
    if ((basic_globals.umask) != -1) {
      umask((basic_globals.umask));
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17508
  case 111: {
    if ((basic_globals.url_adapt_state_ex).tags) {
      zend_hash_destroy((basic_globals.url_adapt_state_ex).tags);
      free((basic_globals.url_adapt_state_ex).tags);
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17509
  case 112: {
    if ((basic_globals.user_tick_functions)) {
      zend_llist_destroy((basic_globals.user_tick_functions));
      _efree(((basic_globals.user_tick_functions)));
      (basic_globals.user_tick_functions) = ((void *)0);
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17510
  case 113: {
    if ((zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                               &process_sections, &scanner_mode) == -1) &&
        !__is_neg("88-113", 2, (int *){ht, filename_len}, 0, 0, 3,
                  (void **){filename, return_value, ini_parser_cb}, 0, 0, 7,
                  "ht", "filename", "filename_len", "process_sections",
                  "scanner_mode", "return_value", "ini_parser_cb")) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17511
  case 114: {
    if ((zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                               &process_sections, &scanner_mode) == -1) ||
        __is_neg("88-114", 2, (int *){ht, filename_len}, 0, 0, 3,
                 (void **){filename, return_value, ini_parser_cb}, 0, 0, 7,
                 "ht", "filename", "filename_len", "process_sections",
                 "scanner_mode", "return_value", "ini_parser_cb")) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17512
  case 115: {
    if (__is_neg("88-115", 2, (int *){ht, filename_len}, 0, 0, 3,
                 (void **){filename, return_value, ini_parser_cb}, 0, 0, 7,
                 "ht", "filename", "filename_len", "process_sections",
                 "scanner_mode", "return_value", "ini_parser_cb"))
      return;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17513
  case 116: {
    if (_zend_hash_init((&(basic_globals.putenv_ht)), (1), (((void *)0)),
                        ((void (*)(void *))php_putenv_destructor), (0)) == -1) {
      return -1;
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17514
  case 117: {
    if (process_sections) {
      (basic_globals.active_ini_file_section) = ((void *)0);
      ini_parser_cb = (zend_ini_parser_cb_t)php_ini_parser_cb_with_sections;
    } else {
      ini_parser_cb = (zend_ini_parser_cb_t)php_simple_ini_parser_cb;
    }
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17515
  case 118: {
    incomplete_class_entry = php_create_incomplete_class();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17516
  case 119: {
    ini_parser_cb = (zend_ini_parser_cb_t)php_ini_parser_cb_with_sections;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17517
  case 120: {
    ini_parser_cb = (zend_ini_parser_cb_t)php_simple_ini_parser_cb;
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17518
  case 121: {
    memset(&(basic_globals.serialize), 0, sizeof((basic_globals.serialize)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17519
  case 122: {
    memset(&(basic_globals.unserialize), 0,
           sizeof((basic_globals.unserialize)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17520
  case 123: {
    memset(&(basic_globals.url_adapt_state_ex), 0,
           sizeof((basic_globals.url_adapt_state_ex)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17521
  case 124: {
    memset(&fh, 0, sizeof(fh));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17522
  case 125: {
    memset((basic_globals.strtok_table), 0, 256);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17523
  case 126: {
    php_add_tick_function(run_user_tick_functions);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17524
  case 127: {
    php_error_docref0(((void *)0), (1 << 1L), "%s",
                      strerror((*__errno_location())));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17525
  case 128: {
    php_error_docref0(((void *)0), (1 << 1L), "An unknown error occured");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17526
  case 129: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Attempted GLOBALS variable overwrite");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17527
  case 130: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Attempted long input array (%s) overwrite", (filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17528
  case 131: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Attempted super-global (%s) variable overwrite",
                      (filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17529
  case 132: {
    php_error_docref0(((void *)0), (1 << 1L), "Couldn't find constant %s",
                      (filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17530
  case 133: {
    php_error_docref0(((void *)0), (1 << 1L), "Filename cannot be empty!");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17531
  case 134: {
    php_error_docref0(((void *)0), (1 << 1L), "Invalid in_addr value");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17532
  case 135: {
    php_error_docref0(((void *)0), (1 << 1L), "Invalid parameter syntax");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17533
  case 136: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Invalid shutdown callback '%s' passed", (filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17534
  case 137: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Invalid tick callback '%s' passed", (filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17535
  case 138: {
    php_error_docref0(
        ((void *)0), (1 << 1L),
        "Number of microseconds must be greater than or equal to 0");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17536
  case 139: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Number of seconds must be greater than or equal to 0");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17537
  case 140: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Second argument is not an object or class name");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17538
  case 141: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Sleep until to time is less than current time");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17539
  case 142: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Unable to call %s() - function does not exist",
                      (*(return_value)).value.str.val);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17540
  case 143: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Unable to call %s() - function does not exist",
                      (*(this_ptr)).value.str.val);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17541
  case 144: {
    php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                      (*(return_value)).value.str.val);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17542
  case 145: {
    php_error_docref0(((void *)0), (1 << 1L), "Unable to call %s()",
                      (*(this_ptr)).value.str.val);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17543
  case 146: {
    php_error_docref0(((void *)0), (1 << 1L), "Unable to call tick function");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17544
  case 147: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "Unable to delete tick function executed at the moment");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17545
  case 148: {
    php_error_docref0(((void *)0), (1 << 1L), "Unable to find extension '%s'",
                      (filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17546
  case 149: {
    php_error_docref0(((void *)0), (1 << 1L), "Unrecognized address %s",
                      (filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17547
  case 150: {
    php_error_docref0(((void *)0), (1 << 1L),
                      "nanoseconds was not in the range 0 to 999 999 999 or "
                      "seconds was negative");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17548
  case 151: {
    php_free_shutdown_functions();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17549
  case 152: {
    php_info_print_table_end();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17550
  case 153: {
    php_info_print_table_start();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17551
  case 154: {
    php_output_discard();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17552
  case 155: {
    php_output_end();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17553
  case 156: {
    php_output_get_contents((return_value));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17554
  case 157: {
    php_output_get_contents((this_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17555
  case 158: {
    php_output_start_default();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17556
  case 159: {
    php_register_url_stream_wrapper("data", &php_stream_rfc2397_wrapper);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17557
  case 160: {
    php_register_url_stream_wrapper("file", &php_plain_files_wrapper);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17558
  case 161: {
    php_register_url_stream_wrapper("ftp", &php_stream_ftp_wrapper);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17559
  case 162: {
    php_register_url_stream_wrapper("glob", &php_glob_stream_wrapper);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17560
  case 163: {
    php_register_url_stream_wrapper("http", &php_stream_http_wrapper);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17561
  case 164: {
    php_register_url_stream_wrapper("php", &php_stream_php_wrapper);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17562
  case 165: {
    php_unregister_url_stream_wrapper("ftp");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17563
  case 166: {
    php_unregister_url_stream_wrapper("http");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17564
  case 167: {
    php_unregister_url_stream_wrapper("php");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17565
  case 168: {
    sapi_flush();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17566
  case 169: {
    sapi_module.log_message((filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17567
  case 170: {
    setlocale(0, "");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17568
  case 171: {
    setlocale(6, "C");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17569
  case 172: {
    tzset();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17570
  case 173: {
    umask((basic_globals.umask));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17571
  case 174: {
    unlink((filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17572
  case 175: {
    zend_error(
        (1 << 0L),
        "Cannot call forward_static_call() when no class scope is active");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17573
  case 176: {
    zend_error((1 << 1L),
               "(Registered shutdown functions) Unable to call %s() - function "
               "does not exist",
               (filename));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17574
  case 177: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17575
  case 178: {
    zend_hash_apply((basic_globals.user_shutdown_function_names),
                    (apply_func_t)user_shutdown_function_call);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17576
  case 179: {
    zend_hash_destroy(&(basic_globals.putenv_ht));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17577
  case 180: {
    zend_hash_destroy((*(return_value)).value.ht);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17578
  case 181: {
    zend_hash_destroy((*(this_ptr)).value.ht);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17579
  case 182: {
    zend_hash_destroy((*return_value).value.ht);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17580
  case 183: {
    zend_hash_destroy((basic_globals.url_adapt_state_ex).tags);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17581
  case 184: {
    zend_hash_destroy((basic_globals.user_shutdown_function_names));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17582
  case 185: {
    zend_hash_internal_pointer_reset_ex((*(return_value)).value.ht,
                                        ((void *)0));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17583
  case 186: {
    zend_hash_internal_pointer_reset_ex((*(this_ptr)).value.ht, ((void *)0));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17584
  case 187: {
    zend_hash_internal_pointer_reset_ex((**(return_value_ptr)).value.ht,
                                        ((void *)0));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17585
  case 188: {
    zend_hash_move_forward_ex((*(return_value)).value.ht, ((void *)0));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17586
  case 189: {
    zend_hash_move_forward_ex((*(this_ptr)).value.ht, ((void *)0));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17587
  case 190: {
    zend_hash_move_forward_ex((**(return_value_ptr)).value.ht, ((void *)0));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17588
  case 191: {
    zend_ini_sort_entries();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17589
  case 192: {
    zend_llist_apply((basic_globals.user_tick_functions),
                     (llist_apply_func_t)user_tick_function_call);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17590
  case 193: {
    zend_llist_destroy((basic_globals.user_tick_functions));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17591
  case 194: {
    zend_llist_init((basic_globals.user_tick_functions),
                    sizeof(user_tick_function_entry),
                    (llist_dtor_func_t)user_tick_function_dtor, 0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17592
  case 195: {
    zend_print_zval_r((return_value), 0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17593
  case 196: {
    zend_print_zval_r((this_ptr), 0);
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17594
  case 197: {
    zend_restore_ini_entry("include_path", sizeof("include_path"), (1 << 4));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17595
  case 198: {
    zend_strip();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17596
  case 199: {
    zend_wrong_param_count();
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17597
  case 200: {
    zval_delref_p(((return_value)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17598
  case 201: {
    zval_delref_p(((this_ptr)));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17599
  case 202: {
    zval_unset_isref_p((return_value));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
#ifdef COMPILE_17600
  case 203: {
    zval_unset_isref_p((this_ptr));
    if (zend_parse_parameters((ht), "s|bl", &filename, &filename_len,
                              &process_sections, &scanner_mode) == -1) {
      {
        do {
          zval *__z = (return_value);
          (*__z).value.lval = ((0) != 0);
          (*__z).type = 3;
        } while (0);
        return;
      };
    }

    break;
  }
#endif
  }

  if (filename_len == 0) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "Filename cannot be empty!");
    RETURN_FALSE;
  }

  /* Set callback function */
  if (process_sections) {
    BG(active_ini_file_section) = NULL;
    ini_parser_cb = (zend_ini_parser_cb_t)php_ini_parser_cb_with_sections;
  } else {
    ini_parser_cb = (zend_ini_parser_cb_t)php_simple_ini_parser_cb;
  }

  /* Setup filehandle */
  memset(&fh, 0, sizeof(fh));
  fh.filename = filename;
  fh.type = ZEND_HANDLE_FILENAME;

  array_init(return_value);
  if (zend_parse_ini_file(&fh, 0, scanner_mode, ini_parser_cb,
                          return_value TSRMLS_CC) == FAILURE) {
    zend_hash_destroy(Z_ARRVAL_P(return_value));
    efree(Z_ARRVAL_P(return_value));
    RETURN_FALSE;
  }
}
/* }}} */

/* {{{ proto array parse_ini_string(string ini_string [, bool process_sections
   [, int scanner_mode]]) Parse configuration string */
PHP_FUNCTION(parse_ini_string) {
  char *string = NULL, *str = NULL;
  int str_len = 0;
  zend_bool process_sections = 0;
  long scanner_mode = ZEND_INI_SCANNER_NORMAL;
  zend_ini_parser_cb_t ini_parser_cb;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|bl", &str, &str_len,
                            &process_sections, &scanner_mode) == FAILURE) {
    RETURN_FALSE;
  }

  /* Set callback function */
  if (process_sections) {
    BG(active_ini_file_section) = NULL;
    ini_parser_cb = (zend_ini_parser_cb_t)php_ini_parser_cb_with_sections;
  } else {
    ini_parser_cb = (zend_ini_parser_cb_t)php_simple_ini_parser_cb;
  }

  /* Setup string */
  string = (char *)emalloc(str_len + ZEND_MMAP_AHEAD);
  memcpy(string, str, str_len);
  memset(string + str_len, 0, ZEND_MMAP_AHEAD);

  array_init(return_value);
  if (zend_parse_ini_string(string, 0, scanner_mode, ini_parser_cb,
                            return_value TSRMLS_CC) == FAILURE) {
    zend_hash_destroy(Z_ARRVAL_P(return_value));
    efree(Z_ARRVAL_P(return_value));
    RETVAL_FALSE;
  }
  efree(string);
}
/* }}} */

#if ZEND_DEBUG
/* This function returns an array of ALL valid ini options with values and
 *  is not the same as ini_get_all() which returns only registered ini options.
 * Only useful for devs to debug php.ini scanner/parser! */
PHP_FUNCTION(config_get_hash) /* {{{ */
{
  HashTable *hash = php_ini_get_configuration_hash();

  array_init(return_value);
  zend_hash_apply_with_arguments(
      hash TSRMLS_CC, (apply_func_args_t)add_config_entry_cb, 1, return_value);
}
/* }}} */
#endif

#ifdef HAVE_GETLOADAVG
/* {{{ proto array sys_getloadavg()
 */
PHP_FUNCTION(sys_getloadavg) {
  double load[3];

  if (zend_parse_parameters_none() == FAILURE) {
    return;
  }

  if (getloadavg(load, 3) == -1) {
    RETURN_FALSE;
  } else {
    array_init(return_value);
    add_index_double(return_value, 0, load[0]);
    add_index_double(return_value, 1, load[1]);
    add_index_double(return_value, 2, load[2]);
  }
}
/* }}} */
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */
