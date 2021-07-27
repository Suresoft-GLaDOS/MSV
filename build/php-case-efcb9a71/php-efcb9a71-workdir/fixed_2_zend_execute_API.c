int __get_mutant(); int __is_neg(const char *location,int count, ...); int __abst_hole(); int __choose(const char *);void* memset(void*, int, unsigned long); 
/*
   +----------------------------------------------------------------------+
   | Zend Engine                                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2011 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
   | Authors: Andi Gutmans <andi@zend.com>                                |
   |          Zeev Suraski <zeev@zend.com>                                |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include <stdio.h>
#include <signal.h>

#include "zend.h"
#include "zend_compile.h"
#include "zend_execute.h"
#include "zend_API.h"
#include "zend_ptr_stack.h"
#include "zend_constants.h"
#include "zend_extensions.h"
#include "zend_exceptions.h"
#include "zend_closures.h"
#include "zend_vm.h"
#include "zend_float.h"
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

ZEND_API void (*zend_execute)(zend_op_array *op_array TSRMLS_DC);
ZEND_API void (*zend_execute_internal)(zend_execute_data *execute_data_ptr, int return_value_used TSRMLS_DC);

/* true globals */
ZEND_API const zend_fcall_info empty_fcall_info = { 0, NULL, NULL, NULL, NULL, 0, NULL, NULL, 0 };
ZEND_API const zend_fcall_info_cache empty_fcall_info_cache = { 0, NULL, NULL, NULL, NULL };

#ifdef ZEND_WIN32
#include <process.h>
static WNDCLASS wc;
static HWND timeout_window;
static HANDLE timeout_thread_event;
static HANDLE timeout_thread_handle;
static DWORD timeout_thread_id;
static int timeout_thread_initialized=0;
#endif

#if 0&&ZEND_DEBUG
static void (*original_sigsegv_handler)(int);
static void zend_handle_sigsegv(int dummy) /* {{{ */
{
	fflush(stdout);
	fflush(stderr);
	if (original_sigsegv_handler == zend_handle_sigsegv) {
		signal(SIGSEGV, original_sigsegv_handler);
	} else {
		signal(SIGSEGV, SIG_DFL);
	}
	{
		TSRMLS_FETCH();

		fprintf(stderr, "SIGSEGV caught on opcode %d on opline %d of %s() at %s:%d\n\n",
				active_opline->opcode,
				active_opline-EG(active_op_array)->opcodes,
				get_active_function_name(TSRMLS_C),
				zend_get_executed_filename(TSRMLS_C),
				zend_get_executed_lineno(TSRMLS_C));
/* See http://support.microsoft.com/kb/190351 */
#ifdef PHP_WIN32
		fflush(stderr);
#endif
	}
	if (original_sigsegv_handler!=zend_handle_sigsegv) {
		original_sigsegv_handler(dummy);
	}
}
/* }}} */
#endif

static void zend_extension_activator(zend_extension *extension TSRMLS_DC) /* {{{ */
{
	if (extension->activate) {
		extension->activate();
	}
}
/* }}} */

static void zend_extension_deactivator(zend_extension *extension TSRMLS_DC) /* {{{ */
{
	if (extension->deactivate) {
		extension->deactivate();
	}
}
/* }}} */

static int clean_non_persistent_function(zend_function *function TSRMLS_DC) /* {{{ */
{
	return (function->type == ZEND_INTERNAL_FUNCTION) ? ZEND_HASH_APPLY_STOP : ZEND_HASH_APPLY_REMOVE;
}
/* }}} */

static int clean_non_persistent_function_full(zend_function *function TSRMLS_DC) /* {{{ */
{
	return (function->type != ZEND_INTERNAL_FUNCTION);
}
/* }}} */

static int clean_non_persistent_class(zend_class_entry **ce TSRMLS_DC) /* {{{ */
{
	return ((*ce)->type == ZEND_INTERNAL_CLASS) ? ZEND_HASH_APPLY_STOP : ZEND_HASH_APPLY_REMOVE;
}
/* }}} */

static int clean_non_persistent_class_full(zend_class_entry **ce TSRMLS_DC) /* {{{ */
{
	return ((*ce)->type != ZEND_INTERNAL_CLASS);
}
/* }}} */

void init_executor(TSRMLS_D) /* {{{ */
{
	zend_init_fpu(TSRMLS_C);

	INIT_ZVAL(EG(uninitialized_zval));
	/* trick to make uninitialized_zval never be modified, passed by ref, etc. */
	Z_ADDREF(EG(uninitialized_zval));
	INIT_ZVAL(EG(error_zval));
	EG(uninitialized_zval_ptr)=&EG(uninitialized_zval);
	EG(error_zval_ptr)=&EG(error_zval);
	zend_ptr_stack_init(&EG(arg_types_stack));
/* destroys stack frame, therefore makes core dumps worthless */
#if 0&&ZEND_DEBUG
	original_sigsegv_handler = signal(SIGSEGV, zend_handle_sigsegv);
#endif
	EG(return_value_ptr_ptr) = NULL;

	EG(symtable_cache_ptr) = EG(symtable_cache) - 1;
	EG(symtable_cache_limit) = EG(symtable_cache) + SYMTABLE_CACHE_SIZE - 1;
	EG(no_extensions) = 0;

	EG(function_table) = CG(function_table);
	EG(class_table) = CG(class_table);

	EG(in_execution) = 0;
	EG(in_autoload) = NULL;
	EG(autoload_func) = NULL;
	EG(error_handling) = EH_NORMAL;

	zend_vm_stack_init(TSRMLS_C);
	zend_vm_stack_push((void *) NULL TSRMLS_CC);

	zend_hash_init(&EG(symbol_table), 50, NULL, ZVAL_PTR_DTOR, 0);
	EG(active_symbol_table) = &EG(symbol_table);

	zend_llist_apply(&zend_extensions, (llist_apply_func_t) zend_extension_activator TSRMLS_CC);
	EG(opline_ptr) = NULL;

	zend_hash_init(&EG(included_files), 5, NULL, NULL, 0);

	EG(ticks_count) = 0;

	EG(user_error_handler) = NULL;

	EG(current_execute_data) = NULL;

	zend_stack_init(&EG(user_error_handlers_error_reporting));
	zend_ptr_stack_init(&EG(user_error_handlers));
	zend_ptr_stack_init(&EG(user_exception_handlers));

	zend_objects_store_init(&EG(objects_store), 1024);

	EG(full_tables_cleanup) = 0;
#ifdef ZEND_WIN32
	EG(timed_out) = 0;
#endif

	EG(exception) = NULL;
	EG(prev_exception) = NULL;

	EG(scope) = NULL;
	EG(called_scope) = NULL;

	EG(This) = NULL;

	EG(active_op_array) = NULL;

	EG(active) = 1;
	EG(start_op) = NULL;
}
/* }}} */

static int zval_call_destructor(zval **zv TSRMLS_DC) /* {{{ */
{
	if (Z_TYPE_PP(zv) == IS_OBJECT && Z_REFCOUNT_PP(zv) == 1) {
		return ZEND_HASH_APPLY_REMOVE;
	} else {
		return ZEND_HASH_APPLY_KEEP;
	}
}
/* }}} */

void shutdown_destructors(TSRMLS_D) /* {{{ */
{
	zend_try {
		int symbols;
		do {
			symbols = zend_hash_num_elements(&EG(symbol_table));
			zend_hash_reverse_apply(&EG(symbol_table), (apply_func_t) zval_call_destructor TSRMLS_CC);
		} while (symbols != zend_hash_num_elements(&EG(symbol_table)));
		zend_objects_store_call_destructors(&EG(objects_store) TSRMLS_CC);
	} zend_catch {
		/* if we couldn't destruct cleanly, mark all objects as destructed anyway */
		zend_objects_store_mark_destructed(&EG(objects_store) TSRMLS_CC);
	} zend_end_try();
}
/* }}} */

void shutdown_executor(TSRMLS_D) /* {{{ */
{
	zend_try {

/* Removed because this can not be safely done, e.g. in this situation:
   Object 1 creates object 2
   Object 3 holds reference to object 2.
   Now when 1 and 2 are destroyed, 3 can still access 2 in its destructor, with
   very problematic results */
/* 		zend_objects_store_call_destructors(&EG(objects_store) TSRMLS_CC); */

/* Moved after symbol table cleaners, because  some of the cleaners can call
   destructors, which would use EG(symtable_cache_ptr) and thus leave leaks */
/*		while (EG(symtable_cache_ptr)>=EG(symtable_cache)) {
			zend_hash_destroy(*EG(symtable_cache_ptr));
			efree(*EG(symtable_cache_ptr));
			EG(symtable_cache_ptr)--;
		}
*/
		zend_llist_apply(&zend_extensions, (llist_apply_func_t) zend_extension_deactivator TSRMLS_CC);
		zend_hash_graceful_reverse_destroy(&EG(symbol_table));
	} zend_end_try();

	zend_try {
		zval *zeh;
		/* remove error handlers before destroying classes and functions,
		 * so that if handler used some class, crash would not happen */
		if (EG(user_error_handler)) {
			zeh = EG(user_error_handler);
			EG(user_error_handler) = NULL;
			zval_dtor(zeh);
			FREE_ZVAL(zeh);
		}

		if (EG(user_exception_handler)) {
			zeh = EG(user_exception_handler);
			EG(user_exception_handler) = NULL;
			zval_dtor(zeh);
			FREE_ZVAL(zeh);
		}

		zend_stack_destroy(&EG(user_error_handlers_error_reporting));
		zend_stack_init(&EG(user_error_handlers_error_reporting));
		zend_ptr_stack_clean(&EG(user_error_handlers), ZVAL_DESTRUCTOR, 1);
		zend_ptr_stack_clean(&EG(user_exception_handlers), ZVAL_DESTRUCTOR, 1);
	} zend_end_try();

	zend_try {
		/* Cleanup static data for functions and arrays.
		 * We need a separate cleanup stage because of the following problem:
		 * Suppose we destroy class X, which destroys the class's function table,
		 * and in the function table we have function foo() that has static $bar.
		 * Now if an object of class X is assigned to $bar, its destructor will be
		 * called and will fail since X's function table is in mid-destruction.
		 * So we want first of all to clean up all data and then move to tables destruction.
		 * Note that only run-time accessed data need to be cleaned up, pre-defined data can
		 * not contain objects and thus are not probelmatic */
		if (EG(full_tables_cleanup)) {
			zend_hash_apply(EG(function_table), (apply_func_t) zend_cleanup_function_data_full TSRMLS_CC);
			zend_hash_apply(EG(class_table), (apply_func_t) zend_cleanup_class_data TSRMLS_CC);
		} else {
			zend_hash_reverse_apply(EG(function_table), (apply_func_t) zend_cleanup_function_data TSRMLS_CC);
			zend_hash_reverse_apply(EG(class_table), (apply_func_t) zend_cleanup_user_class_data TSRMLS_CC);
			zend_cleanup_internal_classes(TSRMLS_C);
		}

		zend_vm_stack_destroy(TSRMLS_C);

		zend_objects_store_free_object_storage(&EG(objects_store) TSRMLS_CC);

		/* Destroy all op arrays */
		if (EG(full_tables_cleanup)) {
			zend_hash_apply(EG(function_table), (apply_func_t) clean_non_persistent_function_full TSRMLS_CC);
			zend_hash_apply(EG(class_table), (apply_func_t) clean_non_persistent_class_full TSRMLS_CC);
		} else {
			zend_hash_reverse_apply(EG(function_table), (apply_func_t) clean_non_persistent_function TSRMLS_CC);
			zend_hash_reverse_apply(EG(class_table), (apply_func_t) clean_non_persistent_class TSRMLS_CC);
		}

		while (EG(symtable_cache_ptr)>=EG(symtable_cache)) {
			zend_hash_destroy(*EG(symtable_cache_ptr));
			FREE_HASHTABLE(*EG(symtable_cache_ptr));
			EG(symtable_cache_ptr)--;
		}
	} zend_end_try();

	zend_try {
		clean_non_persistent_constants(TSRMLS_C);
	} zend_end_try();

	zend_try {
#if 0&&ZEND_DEBUG
	signal(SIGSEGV, original_sigsegv_handler);
#endif

		zend_hash_destroy(&EG(included_files));

		zend_ptr_stack_destroy(&EG(arg_types_stack));
		zend_stack_destroy(&EG(user_error_handlers_error_reporting));
		zend_ptr_stack_destroy(&EG(user_error_handlers));
		zend_ptr_stack_destroy(&EG(user_exception_handlers));
		zend_objects_store_destroy(&EG(objects_store));
		if (EG(in_autoload)) {
			zend_hash_destroy(EG(in_autoload));
			FREE_HASHTABLE(EG(in_autoload));
		}
	} zend_end_try();

	zend_shutdown_fpu(TSRMLS_C);

	EG(active) = 0;
}
/* }}} */

/* return class name and "::" or "". */
ZEND_API char *get_active_class_name(char **space TSRMLS_DC) /* {{{ */
{
	if (!zend_is_executing(TSRMLS_C)) {
		if (space) {
			*space = "";
		}
		return "";
	}
	switch (EG(current_execute_data)->function_state.function->type) {
		case ZEND_USER_FUNCTION:
		case ZEND_INTERNAL_FUNCTION:
		{
			zend_class_entry *ce = EG(current_execute_data)->function_state.function->common.scope;

			if (space) {
				*space = ce ? "::" : "";
			}
			return ce ? ce->name : "";
		}
		default:
			if (space) {
				*space = "";
			}
			return "";
	}
}
/* }}} */

ZEND_API char *get_active_function_name(TSRMLS_D) /* {{{ */
{
	if (!zend_is_executing(TSRMLS_C)) {
		return NULL;
	}
	switch (EG(current_execute_data)->function_state.function->type) {
		case ZEND_USER_FUNCTION: {
				char *function_name = ((zend_op_array *) EG(current_execute_data)->function_state.function)->function_name;

				if (function_name) {
					return function_name;
				} else {
					return "main";
				}
			}
			break;
		case ZEND_INTERNAL_FUNCTION:
			return ((zend_internal_function *) EG(current_execute_data)->function_state.function)->function_name;
			break;
		default:
			return NULL;
	}
}
/* }}} */

ZEND_API char *zend_get_executed_filename(TSRMLS_D) /* {{{ */
{
	if (EG(active_op_array)) {
		return EG(active_op_array)->filename;
	} else {
		return "[no active file]";
	}
}
/* }}} */

ZEND_API uint zend_get_executed_lineno(TSRMLS_D) /* {{{ */
{
	if(EG(exception) && EG(opline_ptr) && active_opline->opcode == ZEND_HANDLE_EXCEPTION && 
		active_opline->lineno == 0 && EG(opline_before_exception)) {
		return EG(opline_before_exception)->lineno;
	}
	if (EG(opline_ptr)) {
		return active_opline->lineno;
	} else {
		return 0;
	}
}
/* }}} */

ZEND_API zend_bool zend_is_executing(TSRMLS_D) /* {{{ */
{
	return EG(in_execution);
}
/* }}} */

ZEND_API void _zval_ptr_dtor(zval **zval_ptr ZEND_FILE_LINE_DC) /* {{{ */
{
#if DEBUG_ZEND>=2
	printf("Reducing refcount for %x (%x): %d->%d\n", *zval_ptr, zval_ptr, Z_REFCOUNT_PP(zval_ptr), Z_REFCOUNT_PP(zval_ptr) - 1);
#endif
	Z_DELREF_PP(zval_ptr);
	if (Z_REFCOUNT_PP(zval_ptr) == 0) {
		TSRMLS_FETCH();

		if (*zval_ptr != &EG(uninitialized_zval)) {
			GC_REMOVE_ZVAL_FROM_BUFFER(*zval_ptr);
			zval_dtor(*zval_ptr);
			efree_rel(*zval_ptr);
		}
	} else {
		TSRMLS_FETCH();

		if (Z_REFCOUNT_PP(zval_ptr) == 1) {
			Z_UNSET_ISREF_PP(zval_ptr);
		}

		GC_ZVAL_CHECK_POSSIBLE_ROOT(*zval_ptr);
	}
}
/* }}} */

ZEND_API void _zval_internal_ptr_dtor(zval **zval_ptr ZEND_FILE_LINE_DC) /* {{{ */
{
#if DEBUG_ZEND>=2
	printf("Reducing refcount for %x (%x): %d->%d\n", *zval_ptr, zval_ptr, Z_REFCOUNT_PP(zval_ptr), Z_REFCOUNT_PP(zval_ptr) - 1);
#endif
	Z_DELREF_PP(zval_ptr);
	if (Z_REFCOUNT_PP(zval_ptr) == 0) {
		zval_internal_dtor(*zval_ptr);
		free(*zval_ptr);
	} else if (Z_REFCOUNT_PP(zval_ptr) == 1) {
		Z_UNSET_ISREF_PP(zval_ptr);
	}
}
/* }}} */

ZEND_API int zend_is_true(zval *op) /* {{{ */
{
	return i_zend_is_true(op);
}
/* }}} */

#include "../TSRM/tsrm_strtok_r.h"

#define IS_VISITED_CONSTANT			IS_CONSTANT_INDEX
#define IS_CONSTANT_VISITED(p)		(Z_TYPE_P(p) & IS_VISITED_CONSTANT)
#define Z_REAL_TYPE_P(p)			(Z_TYPE_P(p) & ~IS_VISITED_CONSTANT)
#define MARK_CONSTANT_VISITED(p)	Z_TYPE_P(p) |= IS_VISITED_CONSTANT

static void zval_deep_copy(zval **p)
{
	zval *value;

	ALLOC_ZVAL(value);
	*value = **p;
	Z_TYPE_P(value) &= ~IS_CONSTANT_INDEX;
	zval_copy_ctor(value);
	Z_TYPE_P(value) = Z_TYPE_PP(p);
	INIT_PZVAL(value);
	*p = value;
}

ZEND_API int zval_update_constant_ex(zval **pp, void *arg, zend_class_entry *scope TSRMLS_DC) /* {{{ */
{
	zval *p = *pp;
	zend_bool inline_change = (zend_bool) (zend_uintptr_t) arg;
	zval const_value;
	char *colon;

	if (IS_CONSTANT_VISITED(p)) {
		zend_error(E_ERROR, "Cannot declare self-referencing constant '%s'", Z_STRVAL_P(p));
	} else if ((Z_TYPE_P(p) & IS_CONSTANT_TYPE_MASK) == IS_CONSTANT) {
		int refcount;
		zend_uchar is_ref;

		SEPARATE_ZVAL_IF_NOT_REF(pp);
		p = *pp;

		MARK_CONSTANT_VISITED(p);

		refcount = Z_REFCOUNT_P(p);
		is_ref = Z_ISREF_P(p);

		if (!zend_get_constant_ex(p->value.str.val, p->value.str.len, &const_value, scope, Z_REAL_TYPE_P(p) TSRMLS_CC)) {
			char *actual = Z_STRVAL_P(p);

			if ((colon = zend_memrchr(Z_STRVAL_P(p), ':', Z_STRLEN_P(p)))) {
				zend_error(E_ERROR, "Undefined class constant '%s'", Z_STRVAL_P(p));
				Z_STRLEN_P(p) -= ((colon - Z_STRVAL_P(p)) + 1);
				if (inline_change) {
					colon = estrndup(colon, Z_STRLEN_P(p));
					str_efree(Z_STRVAL_P(p));
					Z_STRVAL_P(p) = colon;
				} else {
					Z_STRVAL_P(p) = colon + 1;
				}
			} else {
				char *save = actual, *slash;
				int actual_len = Z_STRLEN_P(p);
				if ((Z_TYPE_P(p) & IS_CONSTANT_UNQUALIFIED) && (slash = (char *)zend_memrchr(actual, '\\', actual_len))) {
					actual = slash + 1;
					actual_len -= (actual - Z_STRVAL_P(p));
					if (inline_change) {
						actual = estrndup(actual, actual_len);
						Z_STRVAL_P(p) = actual;
						Z_STRLEN_P(p) = actual_len;
					}
				}
				if (actual[0] == '\\') {
					if (inline_change) {
						memmove(Z_STRVAL_P(p), Z_STRVAL_P(p)+1, Z_STRLEN_P(p));
						--Z_STRLEN_P(p);
					} else {
						++actual;
					}
					--actual_len;
				}
				if ((Z_TYPE_P(p) & IS_CONSTANT_UNQUALIFIED) == 0) {
					int fix_save = 0;
					if (save[0] == '\\') {
						save++;
						fix_save = 1;
					}
					zend_error(E_ERROR, "Undefined constant '%s'", save);
					if (fix_save) {
						save--;
					}
					if (inline_change && !IS_INTERNED(save)) {
						efree(save);
					}
					save = NULL;
				}
				if (inline_change && save && save != actual && !IS_INTERNED(save)) {
					efree(save);
				}
				zend_error(E_NOTICE, "Use of undefined constant %s - assumed '%s'",  actual,  actual);
				p->type = IS_STRING;
				if (!inline_change) {
					Z_STRVAL_P(p) = actual;
					Z_STRLEN_P(p) = actual_len;
					zval_copy_ctor(p);
				}
			}
		} else {
			if (inline_change) {
				STR_FREE(Z_STRVAL_P(p));
			}
			*p = const_value;
		}

		Z_SET_REFCOUNT_P(p, refcount);
		Z_SET_ISREF_TO_P(p, is_ref);
	} else if (Z_TYPE_P(p) == IS_CONSTANT_ARRAY) {
		zval **element, *new_val;
		char *str_index;
		uint str_index_len;
		ulong num_index;
		int ret;

		SEPARATE_ZVAL_IF_NOT_REF(pp);
		p = *pp;
		Z_TYPE_P(p) = IS_ARRAY;

		if (!inline_change) {
			zval *tmp;
			HashTable *tmp_ht = NULL;

			ALLOC_HASHTABLE(tmp_ht);
			zend_hash_init(tmp_ht, zend_hash_num_elements(Z_ARRVAL_P(p)), NULL, ZVAL_PTR_DTOR, 0);
			zend_hash_copy(tmp_ht, Z_ARRVAL_P(p), (copy_ctor_func_t) zval_deep_copy, (void *) &tmp, sizeof(zval *));
			Z_ARRVAL_P(p) = tmp_ht;
		} 

		/* First go over the array and see if there are any constant indices */
		zend_hash_internal_pointer_reset(Z_ARRVAL_P(p));
		while (zend_hash_get_current_data(Z_ARRVAL_P(p), (void **) &element) == SUCCESS) {
			if (!(Z_TYPE_PP(element) & IS_CONSTANT_INDEX)) {
				zend_hash_move_forward(Z_ARRVAL_P(p));
				continue;
			}
			Z_TYPE_PP(element) &= ~IS_CONSTANT_INDEX;
			if (zend_hash_get_current_key_ex(Z_ARRVAL_P(p), &str_index, &str_index_len, &num_index, 0, NULL) != HASH_KEY_IS_STRING) {
				zend_hash_move_forward(Z_ARRVAL_P(p));
				continue;
			}
			if (!zend_get_constant_ex(str_index, str_index_len - 3, &const_value, scope, str_index[str_index_len - 2] TSRMLS_CC)) {
				char *actual, *save = str_index;
				if ((colon = zend_memrchr(str_index, ':', str_index_len - 3))) {
					zend_error(E_ERROR, "Undefined class constant '%s'", str_index);
					str_index_len -= ((colon - str_index) + 1);
					str_index = colon;
				} else {
					if (str_index[str_index_len - 2] & IS_CONSTANT_UNQUALIFIED) {
						if ((actual = (char *)zend_memrchr(str_index, '\\', str_index_len - 3))) {
							actual++;
							str_index_len -= (actual - str_index);
							str_index = actual;
						}
					}
					if (str_index[0] == '\\') {
						++str_index;
						--str_index_len;
					}
					if (save[0] == '\\') {
						++save;
					}
					if ((str_index[str_index_len - 2] & IS_CONSTANT_UNQUALIFIED) == 0) {
						zend_error(E_ERROR, "Undefined constant '%s'", save);
					}
					zend_error(E_NOTICE, "Use of undefined constant %s - assumed '%s'",	str_index, str_index);
				}
				ZVAL_STRINGL(&const_value, str_index, str_index_len-3, 1);
			}

			if (Z_REFCOUNT_PP(element) > 1) {
				ALLOC_ZVAL(new_val);
				*new_val = **element;
				zval_copy_ctor(new_val);
				Z_SET_REFCOUNT_P(new_val, 1);
				Z_UNSET_ISREF_P(new_val);

				/* preserve this bit for inheritance */
				Z_TYPE_PP(element) |= IS_CONSTANT_INDEX;
				zval_ptr_dtor(element);
				*element = new_val;
			}

			switch (Z_TYPE(const_value)) {
				case IS_STRING:
					ret = zend_symtable_update_current_key(Z_ARRVAL_P(p), Z_STRVAL(const_value), Z_STRLEN(const_value) + 1, HASH_UPDATE_KEY_IF_BEFORE);
					break;
				case IS_BOOL:
				case IS_LONG:
					ret = zend_hash_update_current_key_ex(Z_ARRVAL_P(p), HASH_KEY_IS_LONG, NULL, 0, Z_LVAL(const_value), HASH_UPDATE_KEY_IF_BEFORE, NULL);
					break;
				case IS_DOUBLE:
					ret = zend_hash_update_current_key_ex(Z_ARRVAL_P(p), HASH_KEY_IS_LONG, NULL, 0, zend_dval_to_lval(Z_DVAL(const_value)), HASH_UPDATE_KEY_IF_BEFORE, NULL);
					break;
				case IS_NULL:
					ret = zend_hash_update_current_key_ex(Z_ARRVAL_P(p), HASH_KEY_IS_STRING, "", 1, 0, HASH_UPDATE_KEY_IF_BEFORE, NULL);
					break;
				default:
					ret = SUCCESS;
					break;
			}
			if (ret == SUCCESS) {
				zend_hash_move_forward(Z_ARRVAL_P(p));
			}
			zval_dtor(&const_value);
		}
		zend_hash_apply_with_argument(Z_ARRVAL_P(p), (apply_func_arg_t) zval_update_constant_inline_change, (void *) scope TSRMLS_CC);
		zend_hash_internal_pointer_reset(Z_ARRVAL_P(p));
	}
	return 0;
}
/* }}} */

ZEND_API int zval_update_constant_inline_change(zval **pp, void *scope TSRMLS_DC) /* {{{ */
{
	return zval_update_constant_ex(pp, (void*)1, scope TSRMLS_CC);
}
/* }}} */

ZEND_API int zval_update_constant_no_inline_change(zval **pp, void *scope TSRMLS_DC) /* {{{ */
{
	return zval_update_constant_ex(pp, (void*)0, scope TSRMLS_CC);
}
/* }}} */

ZEND_API int zval_update_constant(zval **pp, void *arg TSRMLS_DC) /* {{{ */
{
	return zval_update_constant_ex(pp, arg, NULL TSRMLS_CC);
}
/* }}} */

int call_user_function(HashTable *function_table, zval **object_pp, zval *function_name, zval *retval_ptr, zend_uint param_count, zval *params[] TSRMLS_DC) /* {{{ */
{
	zval ***params_array;
	zend_uint i;
	int ex_retval;
	zval *local_retval_ptr = NULL;

	if (param_count) {
		params_array = (zval ***) emalloc(sizeof(zval **)*param_count);
		for (i=0; i<param_count; i++) {
			params_array[i] = &params[i];
		}
	} else {
		params_array = NULL;
	}
	ex_retval = call_user_function_ex(function_table, object_pp, function_name, &local_retval_ptr, param_count, params_array, 1, NULL TSRMLS_CC);
	if (local_retval_ptr) {
		COPY_PZVAL_TO_ZVAL(*retval_ptr, local_retval_ptr);
	} else {
		INIT_ZVAL(*retval_ptr);
	}
	if (params_array) {
		efree(params_array);
	}
	return ex_retval;
}
/* }}} */

int call_user_function_ex(HashTable *function_table, zval **object_pp, zval *function_name, zval **retval_ptr_ptr, zend_uint param_count, zval **params[], int no_separation, HashTable *symbol_table TSRMLS_DC) /* {{{ */
{
	zend_fcall_info fci;

	fci.size = sizeof(fci);
	fci.function_table = function_table;
	fci.object_ptr = object_pp ? *object_pp : NULL;
	fci.function_name = function_name;
	fci.retval_ptr_ptr = retval_ptr_ptr;
	fci.param_count = param_count;
	fci.params = params;
	fci.no_separation = (zend_bool) no_separation;
	fci.symbol_table = symbol_table;

	return zend_call_function(&fci, NULL TSRMLS_CC);
}
/* }}} */

int zend_call_function(zend_fcall_info *fci, zend_fcall_info_cache *fci_cache TSRMLS_DC) /* {{{ */
{
	zend_uint i;
	zval **original_return_value;
	HashTable *calling_symbol_table;
	zend_op_array *original_op_array;
	zend_op **original_opline_ptr;
	zend_class_entry *current_scope;
	zend_class_entry *current_called_scope;
	zend_class_entry *calling_scope = NULL;
	zend_class_entry *called_scope = NULL;
	zval *current_this;
	zend_execute_data execute_data;

	*fci->retval_ptr_ptr = NULL;

	if (!EG(active)) {
		return FAILURE; /* executor is already inactive */
	}

	if (EG(exception)) {
		return FAILURE; /* we would result in an instable executor otherwise */
	}

	switch (fci->size) {
		case sizeof(zend_fcall_info):
			break; /* nothing to do currently */
		default:
			zend_error(E_ERROR, "Corrupted fcall_info provided to zend_call_function()");
			break;
	}

	/* Initialize execute_data */
	if (EG(current_execute_data)) {
		execute_data = *EG(current_execute_data);
		EX(op_array) = NULL;
		EX(opline) = NULL;
		EX(object) = NULL;
	} else {
		/* This only happens when we're called outside any execute()'s
		 * It shouldn't be strictly necessary to NULL execute_data out,
		 * but it may make bugs easier to spot
		 */
		memset(&execute_data, 0, sizeof(zend_execute_data));
	}

	if (!fci_cache || !fci_cache->initialized) {
		zend_fcall_info_cache fci_cache_local;
		char *callable_name;
		char *error = NULL;

		if (!fci_cache) {
			fci_cache = &fci_cache_local;
		}

		if (!zend_is_callable_ex(fci->function_name, fci->object_ptr, IS_CALLABLE_CHECK_SILENT, &callable_name, NULL, fci_cache, &error TSRMLS_CC)) {
			if (error) {
				zend_error(E_WARNING, "Invalid callback %s, %s", callable_name, error);
				efree(error);
			}
			if (callable_name) {
				efree(callable_name);
			}
			return FAILURE;
		} else if (error) {
			/* Capitalize the first latter of the error message */
			if (error[0] >= 'a' && error[0] <= 'z') {
				error[0] += ('A' - 'a');
			}
			zend_error(E_STRICT, "%s", error);
			efree(error);
		}
		efree(callable_name);
	}

	EX(function_state).function = fci_cache->function_handler;
	calling_scope = fci_cache->calling_scope;
	called_scope = fci_cache->called_scope;
	fci->object_ptr = fci_cache->object_ptr;
	EX(object) = fci->object_ptr;
	if (fci->object_ptr && Z_TYPE_P(fci->object_ptr) == IS_OBJECT &&
	    (!EG(objects_store).object_buckets || !EG(objects_store).object_buckets[Z_OBJ_HANDLE_P(fci->object_ptr)].valid)) {
		return FAILURE;
	}

	if (EX(function_state).function->common.fn_flags & (ZEND_ACC_ABSTRACT|ZEND_ACC_DEPRECATED)) {
		if (EX(function_state).function->common.fn_flags & ZEND_ACC_ABSTRACT) {
			zend_error_noreturn(E_ERROR, "Cannot call abstract method %s::%s()", EX(function_state).function->common.scope->name, EX(function_state).function->common.function_name);
		}
		if (EX(function_state).function->common.fn_flags & ZEND_ACC_DEPRECATED) {
 			zend_error(E_DEPRECATED, "Function %s%s%s() is deprecated",
				EX(function_state).function->common.scope ? EX(function_state).function->common.scope->name : "",
				EX(function_state).function->common.scope ? "::" : "",
				EX(function_state).function->common.function_name);
		}
	}

	ZEND_VM_STACK_GROW_IF_NEEDED(fci->param_count + 1);

	for (i=0; i<fci->param_count; i++) {
		zval *param;

		if (ARG_SHOULD_BE_SENT_BY_REF(EX(function_state).function, i + 1)) {
			if (!PZVAL_IS_REF(*fci->params[i]) && Z_REFCOUNT_PP(fci->params[i]) > 1) {
				zval *new_zval;

				if (fci->no_separation &&
				    !ARG_MAY_BE_SENT_BY_REF(EX(function_state).function, i + 1)) {
					if(i) {
						/* hack to clean up the stack */
						zend_vm_stack_push_nocheck((void *) (zend_uintptr_t)i TSRMLS_CC);
						zend_vm_stack_clear_multiple(TSRMLS_C);
					}

					zend_error(E_WARNING, "Parameter %d to %s%s%s() expected to be a reference, value given",
						i+1,
						EX(function_state).function->common.scope ? EX(function_state).function->common.scope->name : "",
						EX(function_state).function->common.scope ? "::" : "",
						EX(function_state).function->common.function_name);
					return FAILURE;
				}

				ALLOC_ZVAL(new_zval);
				*new_zval = **fci->params[i];
				zval_copy_ctor(new_zval);
				Z_SET_REFCOUNT_P(new_zval, 1);
				Z_DELREF_PP(fci->params[i]);
				*fci->params[i] = new_zval;
			}
			Z_ADDREF_PP(fci->params[i]);
			Z_SET_ISREF_PP(fci->params[i]);
			param = *fci->params[i];
		} else if (PZVAL_IS_REF(*fci->params[i]) &&
		           /* don't separate references for __call */
		           (EX(function_state).function->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER) == 0 ) {
			ALLOC_ZVAL(param);
			*param = **(fci->params[i]);
			INIT_PZVAL(param);
			zval_copy_ctor(param);
		} else if (*fci->params[i] != &EG(uninitialized_zval)) {
			Z_ADDREF_PP(fci->params[i]);
			param = *fci->params[i];
		} else {
			ALLOC_ZVAL(param);
			*param = **(fci->params[i]);
			INIT_PZVAL(param);
		}
		zend_vm_stack_push_nocheck(param TSRMLS_CC);
	}

	EX(function_state).arguments = zend_vm_stack_top(TSRMLS_C);
	zend_vm_stack_push_nocheck((void*)(zend_uintptr_t)fci->param_count TSRMLS_CC);

	current_scope = EG(scope);
	EG(scope) = calling_scope;

	current_this = EG(This);

	current_called_scope = EG(called_scope);
	if (called_scope) {
		EG(called_scope) = called_scope;
	} else if (EX(function_state).function->type != ZEND_INTERNAL_FUNCTION) {
		EG(called_scope) = NULL;
	}

	if (fci->object_ptr) {
		if ((EX(function_state).function->common.fn_flags & ZEND_ACC_STATIC)) {
			EG(This) = NULL;
		} else {
			EG(This) = fci->object_ptr;

			if (!PZVAL_IS_REF(EG(This))) {
				Z_ADDREF_P(EG(This)); /* For $this pointer */
			} else {
				zval *this_ptr;

				ALLOC_ZVAL(this_ptr);
				*this_ptr = *EG(This);
				INIT_PZVAL(this_ptr);
				zval_copy_ctor(this_ptr);
				EG(This) = this_ptr;
			}
		}
	} else {
		EG(This) = NULL;
	}

	EX(prev_execute_data) = EG(current_execute_data);
	EG(current_execute_data) = &execute_data;

	if (EX(function_state).function->type == ZEND_USER_FUNCTION) {
		calling_symbol_table = EG(active_symbol_table);
		EG(scope) = EX(function_state).function->common.scope;
		if (fci->symbol_table) {
			EG(active_symbol_table) = fci->symbol_table;
		} else {
			EG(active_symbol_table) = NULL;
		}

		original_return_value = EG(return_value_ptr_ptr);
		original_op_array = EG(active_op_array);
		EG(return_value_ptr_ptr) = fci->retval_ptr_ptr;
		EG(active_op_array) = (zend_op_array *) EX(function_state).function;
		original_opline_ptr = EG(opline_ptr);
		zend_execute(EG(active_op_array) TSRMLS_CC);
		if (!fci->symbol_table && EG(active_symbol_table)) {
			if (EG(symtable_cache_ptr)>=EG(symtable_cache_limit)) {
				zend_hash_destroy(EG(active_symbol_table));
				FREE_HASHTABLE(EG(active_symbol_table));
			} else {
				/* clean before putting into the cache, since clean
				   could call dtors, which could use cached hash */
				zend_hash_clean(EG(active_symbol_table));
				*(++EG(symtable_cache_ptr)) = EG(active_symbol_table);
			}
		}
		EG(active_symbol_table) = calling_symbol_table;
		EG(active_op_array) = original_op_array;
		EG(return_value_ptr_ptr)=original_return_value;
		EG(opline_ptr) = original_opline_ptr;
	} else if (EX(function_state).function->type == ZEND_INTERNAL_FUNCTION) {
		int call_via_handler = (EX(function_state).function->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER) != 0;
		ALLOC_INIT_ZVAL(*fci->retval_ptr_ptr);
		if (EX(function_state).function->common.scope) {
			EG(scope) = EX(function_state).function->common.scope;
		}
		((zend_internal_function *) EX(function_state).function)->handler(fci->param_count, *fci->retval_ptr_ptr, fci->retval_ptr_ptr, fci->object_ptr, 1 TSRMLS_CC);
		/*  We shouldn't fix bad extensions here,
			because it can break proper ones (Bug #34045)
		if (!EX(function_state).function->common.return_reference)
		{
			INIT_PZVAL(*fci->retval_ptr_ptr);
		}*/
		if (EG(exception) && fci->retval_ptr_ptr) {
			zval_ptr_dtor(fci->retval_ptr_ptr);
			*fci->retval_ptr_ptr = NULL;
		}

		if (call_via_handler) {
			/* We must re-initialize function again */
			fci_cache->initialized = 0;
		}
	} else { /* ZEND_OVERLOADED_FUNCTION */
		ALLOC_INIT_ZVAL(*fci->retval_ptr_ptr);

		/* Not sure what should be done here if it's a static method */
		if (fci->object_ptr) {
			Z_OBJ_HT_P(fci->object_ptr)->call_method(EX(function_state).function->common.function_name, fci->param_count, *fci->retval_ptr_ptr, fci->retval_ptr_ptr, fci->object_ptr, 1 TSRMLS_CC);
		} else {
			zend_error_noreturn(E_ERROR, "Cannot call overloaded function for non-object");
		}

		if (EX(function_state).function->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY) {
			efree(EX(function_state).function->common.function_name);
		}
		efree(EX(function_state).function);

		if (EG(exception) && fci->retval_ptr_ptr) {
			zval_ptr_dtor(fci->retval_ptr_ptr);
			*fci->retval_ptr_ptr = NULL;
		}
	}
	zend_vm_stack_clear_multiple(TSRMLS_C);

	if (EG(This)) {
		zval_ptr_dtor(&EG(This));
	}
	EG(called_scope) = current_called_scope;
	EG(scope) = current_scope;
	EG(This) = current_this;
	EG(current_execute_data) = EX(prev_execute_data);

	if (EG(exception)) {
		zend_throw_exception_internal(NULL TSRMLS_CC);
	}
	return SUCCESS;
}
/* }}} */

ZEND_API int zend_lookup_class_ex(const char *name, int name_length, const zend_literal *key, int use_autoload, zend_class_entry ***ce TSRMLS_DC) /* {{{ */
{
	zval **args[1];
	zval autoload_function;
	zval *class_name_ptr;
	zval *retval_ptr = NULL;
	int retval, lc_length;
	char *lc_name;
	char *lc_free;
	zend_fcall_info fcall_info;
	zend_fcall_info_cache fcall_cache;
	char dummy = 1;
	ulong hash;
	ALLOCA_FLAG(use_heap)

	if (key) {
		lc_name = Z_STRVAL(key->constant);
		lc_length = Z_STRLEN(key->constant) + 1;
		hash = key->hash_value;
	} else {
		if (name == NULL || !name_length) {
			return FAILURE;
		}

		lc_free = lc_name = do_alloca(name_length + 1, use_heap);
		zend_str_tolower_copy(lc_name, name, name_length);
		lc_length = name_length + 1;

		if (lc_name[0] == '\\') {
			lc_name += 1;
			lc_length -= 1;
		}

		hash = zend_inline_hash_func(lc_name, lc_length);
	}

	if (zend_hash_quick_find(EG(class_table), lc_name, lc_length, hash, (void **) ce) == SUCCESS) {
		if (!key) {
			free_alloca(lc_free, use_heap);
		}
		return SUCCESS;
	}

	/* The compiler is not-reentrant. Make sure we __autoload() only during run-time
	 * (doesn't impact fuctionality of __autoload()
	*/
	if (!use_autoload || zend_is_compiling(TSRMLS_C)) {
		if (!key) {
			free_alloca(lc_free, use_heap);
		}
		return FAILURE;
	}

	if (EG(in_autoload) == NULL) {
		ALLOC_HASHTABLE(EG(in_autoload));
		zend_hash_init(EG(in_autoload), 0, NULL, NULL, 0);
	}

	if (zend_hash_quick_add(EG(in_autoload), lc_name, lc_length, hash, (void**)&dummy, sizeof(char), NULL) == FAILURE) {
		if (!key) {
			free_alloca(lc_free, use_heap);
		}
		return FAILURE;
	}

	ZVAL_STRINGL(&autoload_function, ZEND_AUTOLOAD_FUNC_NAME, sizeof(ZEND_AUTOLOAD_FUNC_NAME) - 1, 0);

	ALLOC_ZVAL(class_name_ptr);
	INIT_PZVAL(class_name_ptr);
	if (name[0] == '\\') {
		ZVAL_STRINGL(class_name_ptr, name+1, name_length-1, 1);
	} else {
		ZVAL_STRINGL(class_name_ptr, name, name_length, 1);
	}

	args[0] = &class_name_ptr;

	fcall_info.size = sizeof(fcall_info);
	fcall_info.function_table = EG(function_table);
	fcall_info.function_name = &autoload_function;
	fcall_info.symbol_table = NULL;
	fcall_info.retval_ptr_ptr = &retval_ptr;
	fcall_info.param_count = 1;
	fcall_info.params = args;
	fcall_info.object_ptr = NULL;
	fcall_info.no_separation = 1;

	fcall_cache.initialized = EG(autoload_func) ? 1 : 0;
	fcall_cache.function_handler = EG(autoload_func);
	fcall_cache.calling_scope = NULL;
	fcall_cache.called_scope = NULL;
	fcall_cache.object_ptr = NULL;

	zend_exception_save(TSRMLS_C);
	retval = zend_call_function(&fcall_info, &fcall_cache TSRMLS_CC);
	zend_exception_restore(TSRMLS_C);

	EG(autoload_func) = fcall_cache.function_handler;

	zval_ptr_dtor(&class_name_ptr);

	zend_hash_quick_del(EG(in_autoload), lc_name, lc_length, hash);

	if (retval_ptr) {
		zval_ptr_dtor(&retval_ptr);
	}

	if (retval == SUCCESS) {
		retval = zend_hash_quick_find(EG(class_table), lc_name, lc_length, hash, (void **) ce);
	}
	if (!key) {
		free_alloca(lc_free, use_heap);
	}
	return retval;
}
/* }}} */

ZEND_API int zend_lookup_class(const char *name, int name_length, zend_class_entry ***ce TSRMLS_DC) /* {{{ */
{
	return zend_lookup_class_ex(name, name_length, NULL, 1, ce TSRMLS_CC);
}
/* }}} */

ZEND_API int zend_eval_stringl(char *str, int str_len, zval *retval_ptr, char *string_name TSRMLS_DC) /* {{{ */
{
	zval pv;
	zend_op_array *new_op_array;
	zend_op_array *original_active_op_array = EG(active_op_array);
	zend_uint original_compiler_options;
	int retval;

	if (retval_ptr) {
		Z_STRLEN(pv) = str_len + sizeof("return ;") - 1;
		Z_STRVAL(pv) = emalloc(Z_STRLEN(pv) + 1);
		memcpy(Z_STRVAL(pv), "return ", sizeof("return ") - 1);
		memcpy(Z_STRVAL(pv) + sizeof("return ") - 1, str, str_len);
		Z_STRVAL(pv)[Z_STRLEN(pv) - 1] = ';';
		Z_STRVAL(pv)[Z_STRLEN(pv)] = '\0';
	} else {
		Z_STRLEN(pv) = str_len;
		Z_STRVAL(pv) = str;
	}
	Z_TYPE(pv) = IS_STRING;

	/*printf("Evaluating '%s'\n", pv.value.str.val);*/

	original_compiler_options = CG(compiler_options);
	CG(compiler_options) = ZEND_COMPILE_DEFAULT_FOR_EVAL;
	new_op_array = zend_compile_string(&pv, string_name TSRMLS_CC);
	CG(compiler_options) = original_compiler_options;

	if (new_op_array) {
		zval *local_retval_ptr=NULL;
		zval **original_return_value_ptr_ptr = EG(return_value_ptr_ptr);
		zend_op **original_opline_ptr = EG(opline_ptr);
		int orig_interactive = CG(interactive);

		EG(return_value_ptr_ptr) = &local_retval_ptr;
		EG(active_op_array) = new_op_array;
		EG(no_extensions)=1;
		if (!EG(active_symbol_table)) {
			zend_rebuild_symbol_table(TSRMLS_C);
		}
		CG(interactive) = 0;

		zend_execute(new_op_array TSRMLS_CC);

		CG(interactive) = orig_interactive;
		if (local_retval_ptr) {
			if (retval_ptr) {
				COPY_PZVAL_TO_ZVAL(*retval_ptr, local_retval_ptr);
			} else {
				zval_ptr_dtor(&local_retval_ptr);
			}
		} else {
			if (retval_ptr) {
				INIT_ZVAL(*retval_ptr);
			}
		}

		EG(no_extensions)=0;
		EG(opline_ptr) = original_opline_ptr;
		EG(active_op_array) = original_active_op_array;
		destroy_op_array(new_op_array TSRMLS_CC);
		efree(new_op_array);
		EG(return_value_ptr_ptr) = original_return_value_ptr_ptr;
		retval = SUCCESS;
	} else {
		retval = FAILURE;
	}
	if (retval_ptr) {
		zval_dtor(&pv);
	}
	return retval;
}
/* }}} */

ZEND_API int zend_eval_string(char *str, zval *retval_ptr, char *string_name TSRMLS_DC) /* {{{ */
{
	return zend_eval_stringl(str, strlen(str), retval_ptr, string_name TSRMLS_CC);
}
/* }}} */

ZEND_API int zend_eval_stringl_ex(char *str, int str_len, zval *retval_ptr, char *string_name, int handle_exceptions TSRMLS_DC) /* {{{ */
{
	int result;

	result = zend_eval_stringl(str, str_len, retval_ptr, string_name TSRMLS_CC);
	if (handle_exceptions && EG(exception)) {
		zend_exception_error(EG(exception), E_ERROR TSRMLS_CC);
		result = FAILURE;
	}
	return result;
}
/* }}} */

ZEND_API int zend_eval_string_ex(char *str, zval *retval_ptr, char *string_name, int handle_exceptions TSRMLS_DC) /* {{{ */
{
	return zend_eval_stringl_ex(str, strlen(str), retval_ptr, string_name, handle_exceptions TSRMLS_CC);
}
/* }}} */

void execute_new_code(TSRMLS_D) /* {{{ */
{
	zend_op *opline, *end;
	zend_op *ret_opline;
	int orig_interactive;

	if (!(CG(active_op_array)->fn_flags & ZEND_ACC_INTERACTIVE)
		|| CG(context).backpatch_count>0
		|| CG(active_op_array)->function_name
		|| CG(active_op_array)->type!=ZEND_USER_FUNCTION) {
		return;
	}

	ret_opline = get_next_op(CG(active_op_array) TSRMLS_CC);
	ret_opline->opcode = ZEND_RETURN;
	ret_opline->op1_type = IS_CONST;
	ret_opline->op1.constant = zend_add_literal(CG(active_op_array), &EG(uninitialized_zval) TSRMLS_CC);
	SET_UNUSED(ret_opline->op2);

	if (!EG(start_op)) {
		EG(start_op) = CG(active_op_array)->opcodes;
	}

	opline=EG(start_op);
	end=CG(active_op_array)->opcodes+CG(active_op_array)->last;

	while (opline<end) {
		if (opline->op1_type == IS_CONST) {
			opline->op1.zv = &CG(active_op_array)->literals[opline->op1.constant].constant;
		}
		if (opline->op2_type == IS_CONST) {
			opline->op2.zv = &CG(active_op_array)->literals[opline->op2.constant].constant;
		}
		switch (opline->opcode) {
			case ZEND_GOTO:
				if (Z_TYPE_P(opline->op2.zv) != IS_LONG) {
					zend_resolve_goto_label(CG(active_op_array), opline, 1 TSRMLS_CC);
				}
				/* break omitted intentionally */
			case ZEND_JMP:
				opline->op1.jmp_addr = &CG(active_op_array)->opcodes[opline->op1.opline_num];
				break;
			case ZEND_JMPZ:
			case ZEND_JMPNZ:
			case ZEND_JMPZ_EX:
			case ZEND_JMPNZ_EX:
			case ZEND_JMP_SET:
				opline->op2.jmp_addr = &CG(active_op_array)->opcodes[opline->op2.opline_num];
				break;
		}
		ZEND_VM_SET_OPCODE_HANDLER(opline);
		opline++;
	}
	
	zend_release_labels(TSRMLS_C);
	
	EG(return_value_ptr_ptr) = NULL;
	EG(active_op_array) = CG(active_op_array);
	orig_interactive = CG(interactive);
	CG(interactive) = 0;
	zend_execute(CG(active_op_array) TSRMLS_CC);
	CG(interactive) = orig_interactive;

	if (EG(exception)) {
		zend_exception_error(EG(exception), E_ERROR TSRMLS_CC);
	}

	CG(active_op_array)->last -= 1;	/* get rid of that ZEND_RETURN */
	EG(start_op) = CG(active_op_array)->opcodes+CG(active_op_array)->last;
}
/* }}} */

ZEND_API void zend_timeout(int dummy) /* {{{ */
{
	TSRMLS_FETCH();

	if (zend_on_timeout) {
		zend_on_timeout(EG(timeout_seconds) TSRMLS_CC);
	}

	zend_error(E_ERROR, "Maximum execution time of %d second%s exceeded", EG(timeout_seconds), EG(timeout_seconds) == 1 ? "" : "s");
}
/* }}} */

#ifdef ZEND_WIN32
static LRESULT CALLBACK zend_timeout_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) /* {{{ */
{
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_REGISTER_ZEND_TIMEOUT:
			/* wParam is the thread id pointer, lParam is the timeout amount in seconds */
			if (lParam == 0) {
				KillTimer(timeout_window, wParam);
			} else {
#ifdef ZTS
				void ***tsrm_ls;
#endif
				SetTimer(timeout_window, wParam, lParam*1000, NULL);
#ifdef ZTS
				tsrm_ls = ts_resource_ex(0, &wParam);
				if (!tsrm_ls) {
					/* shouldn't normally happen */
					break;
				}
#endif
				EG(timed_out) = 0;
			}
			break;
		case WM_UNREGISTER_ZEND_TIMEOUT:
			/* wParam is the thread id pointer */
			KillTimer(timeout_window, wParam);
			break;
		case WM_TIMER: {
#ifdef ZTS
				void ***tsrm_ls;

				tsrm_ls = ts_resource_ex(0, &wParam);
				if (!tsrm_ls) {
					/* Thread died before receiving its timeout? */
					break;
				}
#endif
				KillTimer(timeout_window, wParam);
				EG(timed_out) = 1;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/* }}} */

static unsigned __stdcall timeout_thread_proc(void *pArgs) /* {{{ */
{
	MSG message;

	wc.style=0;
	wc.lpfnWndProc = zend_timeout_WndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=NULL;
	wc.hIcon=NULL;
	wc.hCursor=NULL;
	wc.hbrBackground=(HBRUSH)(COLOR_BACKGROUND + 5);
	wc.lpszMenuName=NULL;
	wc.lpszClassName = "Zend Timeout Window";
	if (!RegisterClass(&wc)) {
		return -1;
	}
	timeout_window = CreateWindow(wc.lpszClassName, wc.lpszClassName, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
	SetEvent(timeout_thread_event);
	while (GetMessage(&message, NULL, 0, 0)) {
		SendMessage(timeout_window, message.message, message.wParam, message.lParam);
		if (message.message == WM_QUIT) {
			break;
		}
	}
	DestroyWindow(timeout_window);
	UnregisterClass(wc.lpszClassName, NULL);
	SetEvent(timeout_thread_handle);
	return 0;
}
/* }}} */

void zend_init_timeout_thread(void) /* {{{ */
{
	timeout_thread_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	timeout_thread_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
	_beginthreadex(NULL, 0, timeout_thread_proc, NULL, 0, &timeout_thread_id);
	WaitForSingleObject(timeout_thread_event, INFINITE);
}
/* }}} */

void zend_shutdown_timeout_thread(void) /* {{{ */
{
	if (!timeout_thread_initialized) {
		return;
	}
	PostThreadMessage(timeout_thread_id, WM_QUIT, 0, 0);

	/* Wait for thread termination */
	WaitForSingleObject(timeout_thread_handle, 5000);
	CloseHandle(timeout_thread_handle);
	timeout_thread_initialized = 0;
}
/* }}} */

#endif

/* This one doesn't exists on QNX */
#ifndef SIGPROF
#define SIGPROF 27
#endif

void zend_set_timeout(long seconds, int reset_signals) /* {{{ */
{
	TSRMLS_FETCH();

	EG(timeout_seconds) = seconds;

#ifdef ZEND_WIN32
	if(!seconds) {
		return;
	}
	if (timeout_thread_initialized == 0 && InterlockedIncrement(&timeout_thread_initialized) == 1) {
		/* We start up this process-wide thread here and not in zend_startup(), because if Zend
		 * is initialized inside a DllMain(), you're not supposed to start threads from it.
		 */
		zend_init_timeout_thread();
	}
	PostThreadMessage(timeout_thread_id, WM_REGISTER_ZEND_TIMEOUT, (WPARAM) GetCurrentThreadId(), (LPARAM) seconds);
#else
#	ifdef HAVE_SETITIMER
	{
		struct itimerval t_r;		/* timeout requested */
		sigset_t sigset;

		if(seconds) {
			t_r.it_value.tv_sec = seconds;
			t_r.it_value.tv_usec = t_r.it_interval.tv_sec = t_r.it_interval.tv_usec = 0;

#	ifdef __CYGWIN__
			setitimer(ITIMER_REAL, &t_r, NULL);
		}
		if(reset_signals) {
			signal(SIGALRM, zend_timeout);
			sigemptyset(&sigset);
			sigaddset(&sigset, SIGALRM);
		}
#	else
			setitimer(ITIMER_PROF, &t_r, NULL);
		}
		if(reset_signals) {
			signal(SIGPROF, zend_timeout);
			sigemptyset(&sigset);
			sigaddset(&sigset, SIGPROF);
		}
#	endif
		if(reset_signals) {
			sigprocmask(SIG_UNBLOCK, &sigset, NULL);
		}
	}
#	endif
#endif
}
/* }}} */

void zend_unset_timeout(TSRMLS_D) /* {{{ */
{
#ifdef ZEND_WIN32
	if(timeout_thread_initialized) {
		PostThreadMessage(timeout_thread_id, WM_UNREGISTER_ZEND_TIMEOUT, (WPARAM) GetCurrentThreadId(), (LPARAM) 0);
	}
#else
#	ifdef HAVE_SETITIMER
	if (EG(timeout_seconds)) {
		struct itimerval no_timeout;

		no_timeout.it_value.tv_sec = no_timeout.it_value.tv_usec = no_timeout.it_interval.tv_sec = no_timeout.it_interval.tv_usec = 0;

#ifdef __CYGWIN__
		setitimer(ITIMER_REAL, &no_timeout, NULL);
#else
		setitimer(ITIMER_PROF, &no_timeout, NULL);
#endif
	}
#	endif
#endif
}
/* }}} */

zend_class_entry *zend_fetch_class(const char *class_name, uint class_name_len, int fetch_type TSRMLS_DC) /* {{{ */
{
	zend_class_entry **pce;
	int use_autoload = (fetch_type & ZEND_FETCH_CLASS_NO_AUTOLOAD) == 0;
	int silent       = (fetch_type & ZEND_FETCH_CLASS_SILENT) != 0;

	fetch_type &= ZEND_FETCH_CLASS_MASK;

check_fetch_type:
	switch (fetch_type) {
		case ZEND_FETCH_CLASS_SELF:
			if (!EG(scope)) {
				zend_error(E_ERROR, "Cannot access self:: when no class scope is active");
			}
			return EG(scope);
		case ZEND_FETCH_CLASS_PARENT:
			if (!EG(scope)) {
				zend_error(E_ERROR, "Cannot access parent:: when no class scope is active");
			}
			if (!EG(scope)->parent) {
				zend_error(E_ERROR, "Cannot access parent:: when current class scope has no parent");
			}
			return EG(scope)->parent;
		case ZEND_FETCH_CLASS_STATIC:
			if (!EG(called_scope)) {
				zend_error(E_ERROR, "Cannot access static:: when no class scope is active");
			}
			return EG(called_scope);
		case ZEND_FETCH_CLASS_AUTO: {
				fetch_type = zend_get_class_fetch_type(class_name, class_name_len);
				if (fetch_type!=ZEND_FETCH_CLASS_DEFAULT) {
					goto check_fetch_type;
				}
			}
			break;
	}

	if (zend_lookup_class_ex(class_name, class_name_len, NULL, use_autoload, &pce TSRMLS_CC) == FAILURE) {
		if (use_autoload) {
			if (!silent && !EG(exception)) {
				if (fetch_type == ZEND_FETCH_CLASS_INTERFACE) {
					zend_error(E_ERROR, "Interface '%s' not found", class_name);
				} else if (fetch_type == ZEND_FETCH_CLASS_TRAIT) {
                	zend_error(E_ERROR, "Trait '%s' not found", class_name);
                } else {
					zend_error(E_ERROR, "Class '%s' not found", class_name);
				}	
			}
		}
		return NULL;
	}
	return *pce;
}
/* }}} */

zend_class_entry *zend_fetch_class_by_name(const char *class_name, uint class_name_len, const zend_literal *key, int fetch_type TSRMLS_DC) /* {{{ */
{
	zend_class_entry **pce;
	int use_autoload = (fetch_type & ZEND_FETCH_CLASS_NO_AUTOLOAD) == 0;

	if (zend_lookup_class_ex(class_name, class_name_len, key, use_autoload, &pce TSRMLS_CC) == FAILURE) {
		if (use_autoload) {
			if ((fetch_type & ZEND_FETCH_CLASS_SILENT) == 0 && !EG(exception)) {
				if ((fetch_type & ZEND_FETCH_CLASS_MASK) == ZEND_FETCH_CLASS_INTERFACE) {
					zend_error(E_ERROR, "Interface '%s' not found", class_name);
				} else if ((fetch_type & ZEND_FETCH_CLASS_MASK) == ZEND_FETCH_CLASS_TRAIT) {
					zend_error(E_ERROR, "Trait '%s' not found", class_name);
				} else {
					zend_error(E_ERROR, "Class '%s' not found", class_name);
				}	
			}
		}
		return NULL;
	}
	return *pce;
}
/* }}} */

#define MAX_ABSTRACT_INFO_CNT 3
#define MAX_ABSTRACT_INFO_FMT "%s%s%s%s"
#define DISPLAY_ABSTRACT_FN(idx) \
	ai.afn[idx] ? ZEND_FN_SCOPE_NAME(ai.afn[idx]) : "", \
	ai.afn[idx] ? "::" : "", \
	ai.afn[idx] ? ai.afn[idx]->common.function_name : "", \
	ai.afn[idx] && ai.afn[idx + 1] ? ", " : (ai.afn[idx] && ai.cnt > MAX_ABSTRACT_INFO_CNT ? ", ..." : "")

typedef struct _zend_abstract_info {
	zend_function *afn[MAX_ABSTRACT_INFO_CNT + 1];
	int cnt;
	int ctor;
} zend_abstract_info;

static int zend_verify_abstract_class_function(zend_function *fn, zend_abstract_info *ai TSRMLS_DC) /* {{{ */
{
	if (fn->common.fn_flags & ZEND_ACC_ABSTRACT) {
		if (ai->cnt < MAX_ABSTRACT_INFO_CNT) {
			ai->afn[ai->cnt] = fn;
		}
		if (fn->common.fn_flags & ZEND_ACC_CTOR) {
			if (!ai->ctor) {
				ai->cnt++;
				ai->ctor = 1;
			} else {
				ai->afn[ai->cnt] = NULL;
			}
		} else {
			ai->cnt++;
		}
	}
	return 0;
}
/* }}} */

void zend_verify_abstract_class(zend_class_entry *ce TSRMLS_DC) /* {{{ */
{
	zend_abstract_info ai;

	if ((ce->ce_flags & ZEND_ACC_IMPLICIT_ABSTRACT_CLASS) && !(ce->ce_flags & ZEND_ACC_EXPLICIT_ABSTRACT_CLASS)) {
		memset(&ai, 0, sizeof(ai));

		zend_hash_apply_with_argument(&ce->function_table, (apply_func_arg_t) zend_verify_abstract_class_function, &ai TSRMLS_CC);

		if (ai.cnt) {
			zend_error(E_ERROR, "Class %s contains %d abstract method%s and must therefore be declared abstract or implement the remaining methods (" MAX_ABSTRACT_INFO_FMT MAX_ABSTRACT_INFO_FMT MAX_ABSTRACT_INFO_FMT ")",
				ce->name, ai.cnt,
				ai.cnt > 1 ? "s" : "",
				DISPLAY_ABSTRACT_FN(0),
				DISPLAY_ABSTRACT_FN(1),
				DISPLAY_ABSTRACT_FN(2)
				);
		}
	}
}
/* }}} */

ZEND_API void zend_reset_all_cv(HashTable *symbol_table TSRMLS_DC) /* {{{ */
{
	zend_execute_data *ex;
	int i;

	for (ex = EG(current_execute_data); ex; ex = ex->prev_execute_data) {
		if (ex->op_array && ex->symbol_table == symbol_table) {
			for (i = 0; i < ex->op_array->last_var; i++) {
				ex->CVs[i] = NULL;
			}
		}
	}
}
/* }}} */

ZEND_API void zend_delete_variable(zend_execute_data *ex, HashTable *ht, char *name, int name_len, ulong hash_value TSRMLS_DC) /* {{{ */
{
	if (zend_hash_quick_del(ht, name, name_len, hash_value) == SUCCESS) {
		name_len--;
		while (ex && ex->symbol_table == ht) {
			int i;

			if (ex->op_array) {
				for (i = 0; i < ex->op_array->last_var; i++) {
					if (ex->op_array->vars[i].hash_value == hash_value &&
						ex->op_array->vars[i].name_len == name_len &&
						!memcmp(ex->op_array->vars[i].name, name, name_len)) {
						ex->CVs[i] = NULL;
						break;
					}
				}
			}
			ex = ex->prev_execute_data;
		}
	}
}
/* }}} */

ZEND_API int zend_delete_global_variable_ex(char *name, int name_len, ulong hash_value TSRMLS_DC) /* {{{ */
{
	zend_execute_data *ex;

	if (zend_hash_quick_exists(&EG(symbol_table), name, name_len + 1, hash_value)) {
		for (ex = EG(current_execute_data); ex; ex = ex->prev_execute_data) {
			if (ex->op_array && ex->symbol_table == &EG(symbol_table)) {
				int i;
				for (i = 0; i < ex->op_array->last_var; i++) {
					if (ex->op_array->vars[i].hash_value == hash_value &&
						ex->op_array->vars[i].name_len == name_len &&
						!memcmp(ex->op_array->vars[i].name, name, name_len)
					) {
						ex->CVs[i] = NULL;
						break;
					}
				}
			}
		}
		return zend_hash_quick_del(&EG(symbol_table), name, name_len + 1, hash_value);
	}
	return FAILURE;
}
/* }}} */

ZEND_API int zend_delete_global_variable(char *name, int name_len TSRMLS_DC) /* {{{ */
{
	return zend_delete_global_variable_ex(name, name_len, zend_inline_hash_func(name, name_len + 1) TSRMLS_CC);
}
/* }}} */

ZEND_API void zend_rebuild_symbol_table(TSRMLS_D) /* {{{ */
{
	zend_uint i;
	zend_execute_data *ex;

	if (!EG(active_symbol_table)) {
		
		/* Search for last called user function */
		ex = EG(current_execute_data);
		while (ex && !ex->op_array) {
			ex = ex->prev_execute_data;
		}
		if (ex && ex->symbol_table) {
			EG(active_symbol_table) = ex->symbol_table;
			return;
		}

		if (ex && ex->op_array) {
			//prophet generated patch
			{
			switch(__choose("__SWITCH62"))
			{
			case 0: 
			break;
			#ifdef COMPILE_7398
			case 1: {
			//AddAndReplaceKind
			((executor_globals.active_symbol_table)) = (HashTable *)_emalloc((sizeof(HashTable)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7399
			case 2: {
			//AddAndReplaceKind
			((executor_globals.in_autoload)) = (HashTable *)_emalloc((sizeof(HashTable)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7400
			case 3: {
			//AddAndReplaceKind
			(compiler_globals.compiler_options) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7401
			case 4: {
			//AddAndReplaceKind
			(compiler_globals.interactive) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7402
			case 5: {
			//AddAndReplaceKind
			(ex) = (executor_globals.current_execute_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7403
			case 6: {
			//AddAndReplaceKind
			(ex) = ex->prev_execute_data;
			
			break;
			}
			#endif
			#ifdef COMPILE_7404
			case 7: {
			//AddAndReplaceKind
			(ex)->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7405
			case 8: {
			//AddAndReplaceKind
			(ex)->symbol_table = (executor_globals.active_symbol_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7406
			case 9: {
			//AddAndReplaceKind
			(executor_globals.This) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7407
			case 10: {
			//AddAndReplaceKind
			(executor_globals.active) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7408
			case 11: {
			//AddAndReplaceKind
			(executor_globals.active) = 1;
			
			break;
			}
			#endif
			#ifdef COMPILE_7409
			case 12: {
			//AddAndReplaceKind
			(executor_globals.active_op_array) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7410
			case 13: {
			//AddAndReplaceKind
			(executor_globals.active_op_array) = (compiler_globals.active_op_array);
			
			break;
			}
			#endif
			#ifdef COMPILE_7411
			case 14: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = &(executor_globals.symbol_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7412
			case 15: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7413
			case 16: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = (ex)->symbol_table;
			
			break;
			}
			#endif
			#ifdef COMPILE_7414
			case 17: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
			
			break;
			}
			#endif
			#ifdef COMPILE_7415
			case 18: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = ex->symbol_table;
			
			break;
			}
			#endif
			#ifdef COMPILE_7416
			case 19: {
			//AddAndReplaceKind
			(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
			
			break;
			}
			#endif
			#ifdef COMPILE_7417
			case 20: {
			//AddAndReplaceKind
			(executor_globals.autoload_func) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7418
			case 21: {
			//AddAndReplaceKind
			(executor_globals.called_scope) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7419
			case 22: {
			//AddAndReplaceKind
			(executor_globals.class_table) = (compiler_globals.class_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7420
			case 23: {
			//AddAndReplaceKind
			(executor_globals.current_execute_data) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7421
			case 24: {
			//AddAndReplaceKind
			(executor_globals.error_handling) = (EH_NORMAL);
			
			break;
			}
			#endif
			#ifdef COMPILE_7422
			case 25: {
			//AddAndReplaceKind
			(executor_globals.error_handling) = (EH_SUPPRESS);
			
			break;
			}
			#endif
			#ifdef COMPILE_7423
			case 26: {
			//AddAndReplaceKind
			(executor_globals.error_handling) = (EH_THROW);
			
			break;
			}
			#endif
			#ifdef COMPILE_7424
			case 27: {
			//AddAndReplaceKind
			(executor_globals.error_handling) = EH_NORMAL;
			
			break;
			}
			#endif
			#ifdef COMPILE_7425
			case 28: {
			//AddAndReplaceKind
			(executor_globals.error_zval) = zval_used_for_init;
			
			break;
			}
			#endif
			#ifdef COMPILE_7426
			case 29: {
			//AddAndReplaceKind
			(executor_globals.error_zval_ptr) = &(executor_globals.error_zval);
			
			break;
			}
			#endif
			#ifdef COMPILE_7427
			case 30: {
			//AddAndReplaceKind
			(executor_globals.exception) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7428
			case 31: {
			//AddAndReplaceKind
			(executor_globals.full_tables_cleanup) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7429
			case 32: {
			//AddAndReplaceKind
			(executor_globals.function_table) = (compiler_globals.function_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7430
			case 33: {
			//AddAndReplaceKind
			(executor_globals.in_autoload) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7431
			case 34: {
			//AddAndReplaceKind
			(executor_globals.in_execution) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7432
			case 35: {
			//AddAndReplaceKind
			(executor_globals.no_extensions) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7433
			case 36: {
			//AddAndReplaceKind
			(executor_globals.no_extensions) = 1;
			
			break;
			}
			#endif
			#ifdef COMPILE_7434
			case 37: {
			//AddAndReplaceKind
			(executor_globals.opline_ptr) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7435
			case 38: {
			//AddAndReplaceKind
			(executor_globals.prev_exception) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7436
			case 39: {
			//AddAndReplaceKind
			(executor_globals.return_value_ptr_ptr) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7437
			case 40: {
			//AddAndReplaceKind
			(executor_globals.scope) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7438
			case 41: {
			//AddAndReplaceKind
			(executor_globals.start_op) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7439
			case 42: {
			//AddAndReplaceKind
			(executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes + (compiler_globals.active_op_array)->last;
			
			break;
			}
			#endif
			#ifdef COMPILE_7440
			case 43: {
			//AddAndReplaceKind
			(executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes;
			
			break;
			}
			#endif
			#ifdef COMPILE_7441
			case 44: {
			//AddAndReplaceKind
			(executor_globals.symtable_cache_limit) = (executor_globals.symtable_cache) + 32 - 1;
			
			break;
			}
			#endif
			#ifdef COMPILE_7442
			case 45: {
			//AddAndReplaceKind
			(executor_globals.symtable_cache_ptr) = (executor_globals.symtable_cache) - 1;
			
			break;
			}
			#endif
			#ifdef COMPILE_7443
			case 46: {
			//AddAndReplaceKind
			(executor_globals.symtable_cache_ptr)--;
			
			break;
			}
			#endif
			#ifdef COMPILE_7444
			case 47: {
			//AddAndReplaceKind
			(executor_globals.ticks_count) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7445
			case 48: {
			//AddAndReplaceKind
			(executor_globals.uninitialized_zval) = zval_used_for_init;
			
			break;
			}
			#endif
			#ifdef COMPILE_7446
			case 49: {
			//AddAndReplaceKind
			(executor_globals.uninitialized_zval_ptr) = &(executor_globals.uninitialized_zval);
			
			break;
			}
			#endif
			#ifdef COMPILE_7447
			case 50: {
			//AddAndReplaceKind
			(executor_globals.user_error_handler) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7448
			case 51: {
			//AddAndReplaceKind
			(executor_globals.user_exception_handler) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7449
			case 52: {
			//AddAndReplaceKind
			*(++(executor_globals.symtable_cache_ptr)) = (executor_globals.active_symbol_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7450
			case 53: {
			//AddAndReplaceKind
			*(ex)->CVs[ex->op_array->this_var] = (executor_globals.This);
			
			break;
			}
			#endif
			#ifdef COMPILE_7451
			case 54: {
			//AddAndReplaceKind
			*ex->CVs[(ex)->op_array->this_var] = (executor_globals.This);
			
			break;
			}
			#endif
			#ifdef COMPILE_7452
			case 55: {
			//AddAndReplaceKind
			*ex->CVs[ex->op_array->this_var] = (executor_globals.This);
			
			break;
			}
			#endif
			#ifdef COMPILE_7453
			case 56: {
			//AddAndReplaceKind
			_efree(((executor_globals.active_symbol_table)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7454
			case 57: {
			//AddAndReplaceKind
			_efree(((executor_globals.in_autoload)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7455
			case 58: {
			//AddAndReplaceKind
			_efree((*(executor_globals.symtable_cache_ptr)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7456
			case 59: {
			//AddAndReplaceKind
			_zend_hash_init((&(executor_globals.included_files)), (5), (((void *)0)), (((void *)0)), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7457
			case 60: {
			//AddAndReplaceKind
			_zend_hash_init((&(executor_globals.symbol_table)), (50), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7458
			case 61: {
			//AddAndReplaceKind
			_zend_hash_init(((executor_globals.active_symbol_table)), ((ex)->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7459
			case 62: {
			//AddAndReplaceKind
			_zend_hash_init(((executor_globals.active_symbol_table)), (ex->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7460
			case 63: {
			//AddAndReplaceKind
			_zend_hash_init(((executor_globals.in_autoload)), (0), (((void *)0)), (((void *)0)), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7461
			case 64: {
			//AddAndReplaceKind
			_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (ex)->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7462
			case 65: {
			//AddAndReplaceKind
			_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, (ex)->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7463
			case 66: {
			//AddAndReplaceKind
			_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, (ex)->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7464
			case 67: {
			//AddAndReplaceKind
			_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7465
			case 68: {
			//AddAndReplaceKind
			_zval_ptr_dtor((&(executor_globals.This)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7466
			case 69: {
			//AddAndReplaceKind
			clean_non_persistent_constants();
			
			break;
			}
			#endif
			#ifdef COMPILE_7467
			case 70: {
			//AddAndReplaceKind
			ex = (ex)->prev_execute_data;
			
			break;
			}
			#endif
			#ifdef COMPILE_7468
			case 71: {
			//AddAndReplaceKind
			ex = (executor_globals.current_execute_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7469
			case 72: {
			//AddAndReplaceKind
			ex = ex->prev_execute_data;
			
			break;
			}
			#endif
			#ifdef COMPILE_7470
			case 73: {
			//AddAndReplaceKind
			ex->CVs[(ex)->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7471
			case 74: {
			//AddAndReplaceKind
			ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + (ex)->op_array->last_var + ex->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7472
			case 75: {
			//AddAndReplaceKind
			ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + (ex)->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7473
			case 76: {
			//AddAndReplaceKind
			ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7474
			case 77: {
			//AddAndReplaceKind
			ex->symbol_table = (executor_globals.active_symbol_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7475
			case 78: {
			//AddAndReplaceKind
			exit(1);
			
			break;
			}
			#endif
			#ifdef COMPILE_7476
			case 79: {
			//AddAndReplaceKind
			if (!((compiler_globals.active_op_array)->fn_flags & 16) || (compiler_globals.context).backpatch_count > 0 || (compiler_globals.active_op_array)->function_name || (compiler_globals.active_op_array)->type != 2) {
			    return;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7477
			case 80: {
			//AddAndReplaceKind
			if (!(executor_globals.active)) {
			    return -1;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7478
			case 81: {
			//AddAndReplaceKind
			if (!(executor_globals.active_symbol_table)) {
			    zend_rebuild_symbol_table();
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7479
			case 82: {
			//AddAndReplaceKind
			if (!(executor_globals.called_scope)) {
			    zend_error((1 << 0L), "Cannot access static:: when no class scope is active");
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7480
			case 83: {
			//AddAndReplaceKind
			if (!(executor_globals.scope)) {
			    zend_error((1 << 0L), "Cannot access parent:: when no class scope is active");
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7481
			case 84: {
			//AddAndReplaceKind
			if (!(executor_globals.scope)) {
			    zend_error((1 << 0L), "Cannot access self:: when no class scope is active");
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7482
			case 85: {
			//AddAndReplaceKind
			if (!(executor_globals.scope)->parent) {
			    zend_error((1 << 0L), "Cannot access parent:: when current class scope has no parent");
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7483
			case 86: {
			//AddAndReplaceKind
			if (!(executor_globals.start_op)) {
			    (executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7484
			case 87: {
			//AddAndReplaceKind
			if (!zend_is_executing()) {
			    return ((void *)0);
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7485
			case 88: {
			//AddAndReplaceKind
			if ((executor_globals.This)) {
			    _zval_ptr_dtor((&(executor_globals.This)));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7486
			case 89: {
			//AddAndReplaceKind
			if ((executor_globals.active_op_array)) {
			    return (executor_globals.active_op_array)->filename;
			} else {
			    return "[no active file]";
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7487
			case 90: {
			//AddAndReplaceKind
			if ((executor_globals.exception) && (executor_globals.opline_ptr) && (*(executor_globals.opline_ptr))->opcode == 149 && (*(executor_globals.opline_ptr))->lineno == 0 && (executor_globals.opline_before_exception)) {
			    return (executor_globals.opline_before_exception)->lineno;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7488
			case 91: {
			//AddAndReplaceKind
			if ((executor_globals.exception)) {
			    return -1;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7489
			case 92: {
			//AddAndReplaceKind
			if ((executor_globals.exception)) {
			    zend_exception_error((executor_globals.exception), (1 << 0L));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7490
			case 93: {
			//AddAndReplaceKind
			if ((executor_globals.exception)) {
			    zend_throw_exception_internal(((void *)0));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7491
			case 94: {
			//AddAndReplaceKind
			if ((executor_globals.full_tables_cleanup)) {
			    zend_hash_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function_full);
			    zend_hash_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class_full);
			} else {
			    zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function);
			    zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class);
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7492
			case 95: {
			//AddAndReplaceKind
			if ((executor_globals.full_tables_cleanup)) {
			    zend_hash_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data_full);
			    zend_hash_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_class_data);
			} else {
			    zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data);
			    zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_user_class_data);
			    zend_cleanup_internal_classes();
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7493
			case 96: {
			//AddAndReplaceKind
			if ((executor_globals.in_autoload) == ((void *)0)) {
			    ((executor_globals.in_autoload)) = (HashTable *)_emalloc((sizeof(HashTable)));
			    _zend_hash_init(((executor_globals.in_autoload)), (0), (((void *)0)), (((void *)0)), (0));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7494
			case 97: {
			//AddAndReplaceKind
			if ((executor_globals.in_autoload)) {
			    zend_hash_destroy((executor_globals.in_autoload));
			    _efree(((executor_globals.in_autoload)));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7495
			case 98: {
			//AddAndReplaceKind
			if ((executor_globals.opline_ptr)) {
			    return (*(executor_globals.opline_ptr))->lineno;
			} else {
			    return 0;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7496
			case 99: {
			//AddAndReplaceKind
			if ((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache)) {
			    (executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
			} else {
			    ((executor_globals.active_symbol_table)) = (HashTable *)_emalloc((sizeof(HashTable)));
			    _zend_hash_init(((executor_globals.active_symbol_table)), (ex->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7497
			case 100: {
			//AddAndReplaceKind
			if ((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache_limit)) {
			    zend_hash_destroy((executor_globals.active_symbol_table));
			    _efree(((executor_globals.active_symbol_table)));
			} else {
			    zend_hash_clean((executor_globals.active_symbol_table));
			    *(++(executor_globals.symtable_cache_ptr)) = (executor_globals.active_symbol_table);
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7498
			case 101: {
			//AddAndReplaceKind
			if (ex && ex->symbol_table) {
			    (executor_globals.active_symbol_table) = ex->symbol_table;
			    return;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7499
			case 102: {
			//AddAndReplaceKind
			if (ex->CVs[i]) {
			    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7500
			case 103: {
			//AddAndReplaceKind
			if (ex->op_array->this_var != -1 && !ex->CVs[ex->op_array->this_var] && (executor_globals.This)) {
			    ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
			    *ex->CVs[ex->op_array->this_var] = (executor_globals.This);
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7501
			case 104: {
			//AddAndReplaceKind
			if (zend_on_timeout) {
			    zend_on_timeout((executor_globals.timeout_seconds));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7502
			case 105: {
			//AddAndReplaceKind
			signal(27, zend_timeout);
			
			break;
			}
			#endif
			#ifdef COMPILE_7503
			case 106: {
			//AddAndReplaceKind
			zend_cleanup_internal_classes();
			
			break;
			}
			#endif
			#ifdef COMPILE_7504
			case 107: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Cannot access parent:: when current class scope has no parent");
			
			break;
			}
			#endif
			#ifdef COMPILE_7505
			case 108: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Cannot access parent:: when no class scope is active");
			
			break;
			}
			#endif
			#ifdef COMPILE_7506
			case 109: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Cannot access self:: when no class scope is active");
			
			break;
			}
			#endif
			#ifdef COMPILE_7507
			case 110: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Cannot access static:: when no class scope is active");
			
			break;
			}
			#endif
			#ifdef COMPILE_7508
			case 111: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Maximum execution time of %d second%s exceeded", (executor_globals.timeout_seconds), (executor_globals.timeout_seconds) == 1 ? "" : "s");
			
			break;
			}
			#endif
			#ifdef COMPILE_7509
			case 112: {
			//AddAndReplaceKind
			zend_error((1 << 3L), "A non well formed numeric value encountered");
			
			break;
			}
			#endif
			#ifdef COMPILE_7510
			case 113: {
			//AddAndReplaceKind
			zend_error_noreturn((1 << 0L), "Cannot call overloaded function for non-object");
			
			break;
			}
			#endif
			#ifdef COMPILE_7511
			case 114: {
			//AddAndReplaceKind
			zend_exception_error((executor_globals.exception), (1 << 0L));
			
			break;
			}
			#endif
			#ifdef COMPILE_7512
			case 115: {
			//AddAndReplaceKind
			zend_exception_restore();
			
			break;
			}
			#endif
			#ifdef COMPILE_7513
			case 116: {
			//AddAndReplaceKind
			zend_exception_save();
			
			break;
			}
			#endif
			#ifdef COMPILE_7514
			case 117: {
			//AddAndReplaceKind
			zend_execute((compiler_globals.active_op_array));
			
			break;
			}
			#endif
			#ifdef COMPILE_7515
			case 118: {
			//AddAndReplaceKind
			zend_execute((executor_globals.active_op_array));
			
			break;
			}
			#endif
			#ifdef COMPILE_7516
			case 119: {
			//AddAndReplaceKind
			zend_hash_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class_full);
			
			break;
			}
			#endif
			#ifdef COMPILE_7517
			case 120: {
			//AddAndReplaceKind
			zend_hash_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_class_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7518
			case 121: {
			//AddAndReplaceKind
			zend_hash_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function_full);
			
			break;
			}
			#endif
			#ifdef COMPILE_7519
			case 122: {
			//AddAndReplaceKind
			zend_hash_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data_full);
			
			break;
			}
			#endif
			#ifdef COMPILE_7520
			case 123: {
			//AddAndReplaceKind
			zend_hash_clean((executor_globals.active_symbol_table));
			
			break;
			}
			#endif
			#ifdef COMPILE_7521
			case 124: {
			//AddAndReplaceKind
			zend_hash_destroy(&(executor_globals.included_files));
			
			break;
			}
			#endif
			#ifdef COMPILE_7522
			case 125: {
			//AddAndReplaceKind
			zend_hash_destroy((executor_globals.active_symbol_table));
			
			break;
			}
			#endif
			#ifdef COMPILE_7523
			case 126: {
			//AddAndReplaceKind
			zend_hash_destroy((executor_globals.in_autoload));
			
			break;
			}
			#endif
			#ifdef COMPILE_7524
			case 127: {
			//AddAndReplaceKind
			zend_hash_destroy(*(executor_globals.symtable_cache_ptr));
			
			break;
			}
			#endif
			#ifdef COMPILE_7525
			case 128: {
			//AddAndReplaceKind
			zend_hash_graceful_reverse_destroy(&(executor_globals.symbol_table));
			
			break;
			}
			#endif
			#ifdef COMPILE_7526
			case 129: {
			//AddAndReplaceKind
			zend_hash_reverse_apply(&(executor_globals.symbol_table), (apply_func_t)zval_call_destructor);
			
			break;
			}
			#endif
			#ifdef COMPILE_7527
			case 130: {
			//AddAndReplaceKind
			zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class);
			
			break;
			}
			#endif
			#ifdef COMPILE_7528
			case 131: {
			//AddAndReplaceKind
			zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_user_class_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7529
			case 132: {
			//AddAndReplaceKind
			zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function);
			
			break;
			}
			#endif
			#ifdef COMPILE_7530
			case 133: {
			//AddAndReplaceKind
			zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7531
			case 134: {
			//AddAndReplaceKind
			zend_init_fpu();
			
			break;
			}
			#endif
			#ifdef COMPILE_7532
			case 135: {
			//AddAndReplaceKind
			zend_llist_apply(&zend_extensions, (llist_apply_func_t)zend_extension_activator);
			
			break;
			}
			#endif
			#ifdef COMPILE_7533
			case 136: {
			//AddAndReplaceKind
			zend_llist_apply(&zend_extensions, (llist_apply_func_t)zend_extension_deactivator);
			
			break;
			}
			#endif
			#ifdef COMPILE_7534
			case 137: {
			//AddAndReplaceKind
			zend_objects_store_call_destructors(&(executor_globals.objects_store));
			
			break;
			}
			#endif
			#ifdef COMPILE_7535
			case 138: {
			//AddAndReplaceKind
			zend_objects_store_destroy(&(executor_globals.objects_store));
			
			break;
			}
			#endif
			#ifdef COMPILE_7536
			case 139: {
			//AddAndReplaceKind
			zend_objects_store_free_object_storage(&(executor_globals.objects_store));
			
			break;
			}
			#endif
			#ifdef COMPILE_7537
			case 140: {
			//AddAndReplaceKind
			zend_objects_store_init(&(executor_globals.objects_store), 1024);
			
			break;
			}
			#endif
			#ifdef COMPILE_7538
			case 141: {
			//AddAndReplaceKind
			zend_objects_store_mark_destructed(&(executor_globals.objects_store));
			
			break;
			}
			#endif
			#ifdef COMPILE_7539
			case 142: {
			//AddAndReplaceKind
			zend_on_timeout((executor_globals.timeout_seconds));
			
			break;
			}
			#endif
			#ifdef COMPILE_7540
			case 143: {
			//AddAndReplaceKind
			zend_ptr_stack_clean(&(executor_globals.user_error_handlers), (void (*)(void *))_zval_dtor_wrapper, 1);
			
			break;
			}
			#endif
			#ifdef COMPILE_7541
			case 144: {
			//AddAndReplaceKind
			zend_ptr_stack_clean(&(executor_globals.user_exception_handlers), (void (*)(void *))_zval_dtor_wrapper, 1);
			
			break;
			}
			#endif
			#ifdef COMPILE_7542
			case 145: {
			//AddAndReplaceKind
			zend_ptr_stack_destroy(&(executor_globals.arg_types_stack));
			
			break;
			}
			#endif
			#ifdef COMPILE_7543
			case 146: {
			//AddAndReplaceKind
			zend_ptr_stack_destroy(&(executor_globals.user_error_handlers));
			
			break;
			}
			#endif
			#ifdef COMPILE_7544
			case 147: {
			//AddAndReplaceKind
			zend_ptr_stack_destroy(&(executor_globals.user_exception_handlers));
			
			break;
			}
			#endif
			#ifdef COMPILE_7545
			case 148: {
			//AddAndReplaceKind
			zend_ptr_stack_init(&(executor_globals.arg_types_stack));
			
			break;
			}
			#endif
			#ifdef COMPILE_7546
			case 149: {
			//AddAndReplaceKind
			zend_ptr_stack_init(&(executor_globals.user_error_handlers));
			
			break;
			}
			#endif
			#ifdef COMPILE_7547
			case 150: {
			//AddAndReplaceKind
			zend_ptr_stack_init(&(executor_globals.user_exception_handlers));
			
			break;
			}
			#endif
			#ifdef COMPILE_7548
			case 151: {
			//AddAndReplaceKind
			zend_rebuild_symbol_table();
			
			break;
			}
			#endif
			#ifdef COMPILE_7549
			case 152: {
			//AddAndReplaceKind
			zend_release_labels();
			
			break;
			}
			#endif
			#ifdef COMPILE_7550
			case 153: {
			//AddAndReplaceKind
			zend_shutdown_fpu();
			
			break;
			}
			#endif
			#ifdef COMPILE_7551
			case 154: {
			//AddAndReplaceKind
			zend_stack_destroy(&(executor_globals.user_error_handlers_error_reporting));
			
			break;
			}
			#endif
			#ifdef COMPILE_7552
			case 155: {
			//AddAndReplaceKind
			zend_stack_init(&(executor_globals.user_error_handlers_error_reporting));
			
			break;
			}
			#endif
			#ifdef COMPILE_7553
			case 156: {
			//AddAndReplaceKind
			zend_throw_exception_internal(((void *)0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7554
			case 157: {
			//AddAndReplaceKind
			zend_vm_stack_clear_multiple();
			
			break;
			}
			#endif
			#ifdef COMPILE_7555
			case 158: {
			//AddAndReplaceKind
			zend_vm_stack_destroy();
			
			break;
			}
			#endif
			#ifdef COMPILE_7556
			case 159: {
			//AddAndReplaceKind
			zend_vm_stack_init();
			
			break;
			}
			#endif
			#ifdef COMPILE_7557
			case 160: {
			//AddAndReplaceKind
			zend_vm_stack_push((void *)((void *)0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7558
			case 161: {
			//AddAndReplaceKind
			zval_addref_p(&((executor_globals.uninitialized_zval)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7559
			case 162: {
			//AddAndReplaceKind
			zval_addref_p((executor_globals.This));
			
			break;
			}
			#endif
			}
			{
			long long __temp63=((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache)) ;
			switch(__choose("__SWITCH63"))
			{
			case 0: {
			
			break;
			}
			#ifdef COMPILE_7560
			case 1: {
			__temp63= (((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache)) && !__is_neg("63-1", 138, &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(ex), sizeof (ex), &(ex->op_array), sizeof (ex->op_array), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->last_var), sizeof (ex->op_array->last_var), &(ex->symbol_table), sizeof (ex->symbol_table), &(ex->prev_execute_data), sizeof (ex->prev_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(ex->op_array->this_var), sizeof (ex->op_array->this_var), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(ex->CVs), sizeof (ex->CVs), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.This), sizeof (executor_globals.This), &(i), sizeof (i), &(ex->op_array->vars), sizeof (ex->op_array->vars), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(ex->op_array->type), sizeof (ex->op_array->type), &(ex->op_array->function_name), sizeof (ex->op_array->function_name), &(ex->op_array->scope), sizeof (ex->op_array->scope), &(ex->op_array->fn_flags), sizeof (ex->op_array->fn_flags), &(ex->op_array->prototype), sizeof (ex->op_array->prototype), &(ex->op_array->num_args), sizeof (ex->op_array->num_args), &(ex->op_array->required_num_args), sizeof (ex->op_array->required_num_args), &(ex->op_array->arg_info), sizeof (ex->op_array->arg_info), &(ex->op_array->refcount), sizeof (ex->op_array->refcount), &(ex->op_array->opcodes), sizeof (ex->op_array->opcodes), &(ex->op_array->last), sizeof (ex->op_array->last), &(ex->op_array->T), sizeof (ex->op_array->T), &(ex->op_array->brk_cont_array), sizeof (ex->op_array->brk_cont_array), &(ex->op_array->last_brk_cont), sizeof (ex->op_array->last_brk_cont), &(ex->op_array->try_catch_array), sizeof (ex->op_array->try_catch_array), &(ex->op_array->last_try_catch), sizeof (ex->op_array->last_try_catch), &(ex->op_array->static_variables), sizeof (ex->op_array->static_variables), &(ex->op_array->filename), sizeof (ex->op_array->filename), &(ex->op_array->line_start), sizeof (ex->op_array->line_start), &(ex->op_array->line_end), sizeof (ex->op_array->line_end), &(ex->op_array->doc_comment), sizeof (ex->op_array->doc_comment), &(ex->op_array->doc_comment_len), sizeof (ex->op_array->doc_comment_len), &(ex->op_array->early_binding), sizeof (ex->op_array->early_binding), &(ex->op_array->literals), sizeof (ex->op_array->literals), &(ex->op_array->last_literal), sizeof (ex->op_array->last_literal), &(ex->op_array->run_time_cache), sizeof (ex->op_array->run_time_cache), &(ex->op_array->last_cache_slot), sizeof (ex->op_array->last_cache_slot), &(ex->opline), sizeof (ex->opline), &(ex->fbc), sizeof (ex->fbc), &(ex->called_scope), sizeof (ex->called_scope), &(ex->object), sizeof (ex->object), &(ex->Ts), sizeof (ex->Ts), &(ex->old_error_reporting), sizeof (ex->old_error_reporting), &(ex->nested), sizeof (ex->nested), &(ex->original_return_value), sizeof (ex->original_return_value), &(ex->current_scope), sizeof (ex->current_scope), &(ex->current_called_scope), sizeof (ex->current_called_scope), &(ex->current_this), sizeof (ex->current_this), &(ex->current_object), sizeof (ex->current_object), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)));
			
			break;
			}
			#endif
			}
			switch(__choose("__SWITCH66"))
			{
			case 0: {
			if (__temp63) {
							/*printf("Cache hit!  Reusing %x\n", symtable_cache[symtable_cache_ptr]);*/
							{
			switch(__choose("__SWITCH64"))
			{
			case 0: 
			break;
			#ifdef COMPILE_7561
			case 1: {
			//AddAndReplaceKind
			((executor_globals.active_symbol_table)) = (HashTable *)_emalloc((sizeof(HashTable)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7562
			case 2: {
			//AddAndReplaceKind
			((executor_globals.in_autoload)) = (HashTable *)_emalloc((sizeof(HashTable)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7563
			case 3: {
			//AddAndReplaceKind
			(compiler_globals.compiler_options) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7564
			case 4: {
			//AddAndReplaceKind
			(compiler_globals.interactive) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7565
			case 5: {
			//AddAndReplaceKind
			(ex) = (executor_globals.current_execute_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7566
			case 6: {
			//AddAndReplaceKind
			(ex) = ex->prev_execute_data;
			
			break;
			}
			#endif
			#ifdef COMPILE_7567
			case 7: {
			//AddAndReplaceKind
			(ex)->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7568
			case 8: {
			//AddAndReplaceKind
			(ex)->symbol_table = (executor_globals.active_symbol_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7569
			case 9: {
			//AddAndReplaceKind
			(executor_globals.This) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7570
			case 10: {
			//AddAndReplaceKind
			(executor_globals.active) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7571
			case 11: {
			//AddAndReplaceKind
			(executor_globals.active) = 1;
			
			break;
			}
			#endif
			#ifdef COMPILE_7572
			case 12: {
			//AddAndReplaceKind
			(executor_globals.active_op_array) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7573
			case 13: {
			//AddAndReplaceKind
			(executor_globals.active_op_array) = (compiler_globals.active_op_array);
			
			break;
			}
			#endif
			#ifdef COMPILE_7574
			case 14: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = &(executor_globals.symbol_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7575
			case 15: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7576
			case 16: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = (ex)->symbol_table;
			
			break;
			}
			#endif
			#ifdef COMPILE_7577
			case 17: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
			
			break;
			}
			#endif
			#ifdef COMPILE_7578
			case 18: {
			//AddAndReplaceKind
			(executor_globals.active_symbol_table) = ex->symbol_table;
			
			break;
			}
			#endif
			#ifdef COMPILE_7579
			case 19: {
			//AddAndReplaceKind
			(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
			
			break;
			}
			#endif
			#ifdef COMPILE_7580
			case 20: {
			//AddAndReplaceKind
			(executor_globals.autoload_func) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7581
			case 21: {
			//AddAndReplaceKind
			(executor_globals.called_scope) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7582
			case 22: {
			//AddAndReplaceKind
			(executor_globals.class_table) = (compiler_globals.class_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7583
			case 23: {
			//AddAndReplaceKind
			(executor_globals.current_execute_data) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7584
			case 24: {
			//AddAndReplaceKind
			(executor_globals.error_handling) = (EH_NORMAL);
			
			break;
			}
			#endif
			#ifdef COMPILE_7585
			case 25: {
			//AddAndReplaceKind
			(executor_globals.error_handling) = (EH_SUPPRESS);
			
			break;
			}
			#endif
			#ifdef COMPILE_7586
			case 26: {
			//AddAndReplaceKind
			(executor_globals.error_handling) = (EH_THROW);
			
			break;
			}
			#endif
			#ifdef COMPILE_7587
			case 27: {
			//AddAndReplaceKind
			(executor_globals.error_handling) = EH_NORMAL;
			
			break;
			}
			#endif
			#ifdef COMPILE_7588
			case 28: {
			//AddAndReplaceKind
			(executor_globals.error_zval) = zval_used_for_init;
			
			break;
			}
			#endif
			#ifdef COMPILE_7589
			case 29: {
			//AddAndReplaceKind
			(executor_globals.error_zval_ptr) = &(executor_globals.error_zval);
			
			break;
			}
			#endif
			#ifdef COMPILE_7590
			case 30: {
			//AddAndReplaceKind
			(executor_globals.exception) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7591
			case 31: {
			//AddAndReplaceKind
			(executor_globals.full_tables_cleanup) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7592
			case 32: {
			//AddAndReplaceKind
			(executor_globals.function_table) = (compiler_globals.function_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7593
			case 33: {
			//AddAndReplaceKind
			(executor_globals.in_autoload) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7594
			case 34: {
			//AddAndReplaceKind
			(executor_globals.in_execution) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7595
			case 35: {
			//AddAndReplaceKind
			(executor_globals.no_extensions) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7596
			case 36: {
			//AddAndReplaceKind
			(executor_globals.no_extensions) = 1;
			
			break;
			}
			#endif
			#ifdef COMPILE_7597
			case 37: {
			//AddAndReplaceKind
			(executor_globals.opline_ptr) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7598
			case 38: {
			//AddAndReplaceKind
			(executor_globals.prev_exception) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7599
			case 39: {
			//AddAndReplaceKind
			(executor_globals.return_value_ptr_ptr) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7600
			case 40: {
			//AddAndReplaceKind
			(executor_globals.scope) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7601
			case 41: {
			//AddAndReplaceKind
			(executor_globals.start_op) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7602
			case 42: {
			//AddAndReplaceKind
			(executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes + (compiler_globals.active_op_array)->last;
			
			break;
			}
			#endif
			#ifdef COMPILE_7603
			case 43: {
			//AddAndReplaceKind
			(executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes;
			
			break;
			}
			#endif
			#ifdef COMPILE_7604
			case 44: {
			//AddAndReplaceKind
			(executor_globals.symtable_cache_limit) = (executor_globals.symtable_cache) + 32 - 1;
			
			break;
			}
			#endif
			#ifdef COMPILE_7605
			case 45: {
			//AddAndReplaceKind
			(executor_globals.symtable_cache_ptr) = (executor_globals.symtable_cache) - 1;
			
			break;
			}
			#endif
			#ifdef COMPILE_7606
			case 46: {
			//AddAndReplaceKind
			(executor_globals.symtable_cache_ptr)--;
			
			break;
			}
			#endif
			#ifdef COMPILE_7607
			case 47: {
			//AddAndReplaceKind
			(executor_globals.ticks_count) = 0;
			
			break;
			}
			#endif
			#ifdef COMPILE_7608
			case 48: {
			//AddAndReplaceKind
			(executor_globals.uninitialized_zval) = zval_used_for_init;
			
			break;
			}
			#endif
			#ifdef COMPILE_7609
			case 49: {
			//AddAndReplaceKind
			(executor_globals.uninitialized_zval_ptr) = &(executor_globals.uninitialized_zval);
			
			break;
			}
			#endif
			#ifdef COMPILE_7610
			case 50: {
			//AddAndReplaceKind
			(executor_globals.user_error_handler) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7611
			case 51: {
			//AddAndReplaceKind
			(executor_globals.user_exception_handler) = ((void *)0);
			
			break;
			}
			#endif
			#ifdef COMPILE_7612
			case 52: {
			//AddAndReplaceKind
			*(++(executor_globals.symtable_cache_ptr)) = (executor_globals.active_symbol_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7613
			case 53: {
			//AddAndReplaceKind
			*(ex)->CVs[ex->op_array->this_var] = (executor_globals.This);
			
			break;
			}
			#endif
			#ifdef COMPILE_7614
			case 54: {
			//AddAndReplaceKind
			*ex->CVs[(ex)->op_array->this_var] = (executor_globals.This);
			
			break;
			}
			#endif
			#ifdef COMPILE_7615
			case 55: {
			//AddAndReplaceKind
			*ex->CVs[ex->op_array->this_var] = (executor_globals.This);
			
			break;
			}
			#endif
			#ifdef COMPILE_7616
			case 56: {
			//AddAndReplaceKind
			_efree(((executor_globals.active_symbol_table)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7617
			case 57: {
			//AddAndReplaceKind
			_efree(((executor_globals.in_autoload)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7618
			case 58: {
			//AddAndReplaceKind
			_efree((*(executor_globals.symtable_cache_ptr)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7619
			case 59: {
			//AddAndReplaceKind
			_zend_hash_init((&(executor_globals.included_files)), (5), (((void *)0)), (((void *)0)), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7620
			case 60: {
			//AddAndReplaceKind
			_zend_hash_init((&(executor_globals.symbol_table)), (50), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7621
			case 61: {
			//AddAndReplaceKind
			_zend_hash_init(((executor_globals.active_symbol_table)), ((ex)->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7622
			case 62: {
			//AddAndReplaceKind
			_zend_hash_init(((executor_globals.active_symbol_table)), (ex->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7623
			case 63: {
			//AddAndReplaceKind
			_zend_hash_init(((executor_globals.in_autoload)), (0), (((void *)0)), (((void *)0)), (0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7624
			case 64: {
			//AddAndReplaceKind
			_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (ex)->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7625
			case 65: {
			//AddAndReplaceKind
			_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, (ex)->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7626
			case 66: {
			//AddAndReplaceKind
			_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, (ex)->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7627
			case 67: {
			//AddAndReplaceKind
			_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7628
			case 68: {
			//AddAndReplaceKind
			_zval_ptr_dtor((&(executor_globals.This)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7629
			case 69: {
			//AddAndReplaceKind
			clean_non_persistent_constants();
			
			break;
			}
			#endif
			#ifdef COMPILE_7630
			case 70: {
			//AddAndReplaceKind
			ex = (ex)->prev_execute_data;
			
			break;
			}
			#endif
			#ifdef COMPILE_7631
			case 71: {
			//AddAndReplaceKind
			ex = (executor_globals.current_execute_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7632
			case 72: {
			//AddAndReplaceKind
			ex = ex->prev_execute_data;
			
			break;
			}
			#endif
			#ifdef COMPILE_7633
			case 73: {
			//AddAndReplaceKind
			ex->CVs[(ex)->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7634
			case 74: {
			//AddAndReplaceKind
			ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + (ex)->op_array->last_var + ex->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7635
			case 75: {
			//AddAndReplaceKind
			ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + (ex)->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7636
			case 76: {
			//AddAndReplaceKind
			ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
			
			break;
			}
			#endif
			#ifdef COMPILE_7637
			case 77: {
			//AddAndReplaceKind
			ex->symbol_table = (executor_globals.active_symbol_table);
			
			break;
			}
			#endif
			#ifdef COMPILE_7638
			case 78: {
			//AddAndReplaceKind
			exit(1);
			
			break;
			}
			#endif
			#ifdef COMPILE_7639
			case 79: {
			//AddAndReplaceKind
			if (!((compiler_globals.active_op_array)->fn_flags & 16) || (compiler_globals.context).backpatch_count > 0 || (compiler_globals.active_op_array)->function_name || (compiler_globals.active_op_array)->type != 2) {
			    return;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7640
			case 80: {
			//AddAndReplaceKind
			if (!(executor_globals.active)) {
			    return -1;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7641
			case 81: {
			//AddAndReplaceKind
			if (!(executor_globals.active_symbol_table)) {
			    zend_rebuild_symbol_table();
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7642
			case 82: {
			//AddAndReplaceKind
			if (!(executor_globals.called_scope)) {
			    zend_error((1 << 0L), "Cannot access static:: when no class scope is active");
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7643
			case 83: {
			//AddAndReplaceKind
			if (!(executor_globals.scope)) {
			    zend_error((1 << 0L), "Cannot access parent:: when no class scope is active");
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7644
			case 84: {
			//AddAndReplaceKind
			if (!(executor_globals.scope)) {
			    zend_error((1 << 0L), "Cannot access self:: when no class scope is active");
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7645
			case 85: {
			//AddAndReplaceKind
			if (!(executor_globals.scope)->parent) {
			    zend_error((1 << 0L), "Cannot access parent:: when current class scope has no parent");
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7646
			case 86: {
			//AddAndReplaceKind
			if (!(executor_globals.start_op)) {
			    (executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7647
			case 87: {
			//AddAndReplaceKind
			if (!zend_is_executing()) {
			    return ((void *)0);
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7648
			case 88: {
			//AddAndReplaceKind
			if ((executor_globals.This)) {
			    _zval_ptr_dtor((&(executor_globals.This)));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7649
			case 89: {
			//AddAndReplaceKind
			if ((executor_globals.active_op_array)) {
			    return (executor_globals.active_op_array)->filename;
			} else {
			    return "[no active file]";
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7650
			case 90: {
			//AddAndReplaceKind
			if ((executor_globals.exception) && (executor_globals.opline_ptr) && (*(executor_globals.opline_ptr))->opcode == 149 && (*(executor_globals.opline_ptr))->lineno == 0 && (executor_globals.opline_before_exception)) {
			    return (executor_globals.opline_before_exception)->lineno;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7651
			case 91: {
			//AddAndReplaceKind
			if ((executor_globals.exception)) {
			    return -1;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7652
			case 92: {
			//AddAndReplaceKind
			if ((executor_globals.exception)) {
			    zend_exception_error((executor_globals.exception), (1 << 0L));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7653
			case 93: {
			//AddAndReplaceKind
			if ((executor_globals.exception)) {
			    zend_throw_exception_internal(((void *)0));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7654
			case 94: {
			//AddAndReplaceKind
			if ((executor_globals.full_tables_cleanup)) {
			    zend_hash_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function_full);
			    zend_hash_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class_full);
			} else {
			    zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function);
			    zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class);
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7655
			case 95: {
			//AddAndReplaceKind
			if ((executor_globals.full_tables_cleanup)) {
			    zend_hash_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data_full);
			    zend_hash_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_class_data);
			} else {
			    zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data);
			    zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_user_class_data);
			    zend_cleanup_internal_classes();
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7656
			case 96: {
			//AddAndReplaceKind
			if ((executor_globals.in_autoload) == ((void *)0)) {
			    ((executor_globals.in_autoload)) = (HashTable *)_emalloc((sizeof(HashTable)));
			    _zend_hash_init(((executor_globals.in_autoload)), (0), (((void *)0)), (((void *)0)), (0));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7657
			case 97: {
			//AddAndReplaceKind
			if ((executor_globals.in_autoload)) {
			    zend_hash_destroy((executor_globals.in_autoload));
			    _efree(((executor_globals.in_autoload)));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7658
			case 98: {
			//AddAndReplaceKind
			if ((executor_globals.opline_ptr)) {
			    return (*(executor_globals.opline_ptr))->lineno;
			} else {
			    return 0;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7659
			case 99: {
			//AddAndReplaceKind
			if ((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache)) {
			    (executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
			} else {
			    ((executor_globals.active_symbol_table)) = (HashTable *)_emalloc((sizeof(HashTable)));
			    _zend_hash_init(((executor_globals.active_symbol_table)), (ex->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7660
			case 100: {
			//AddAndReplaceKind
			if ((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache_limit)) {
			    zend_hash_destroy((executor_globals.active_symbol_table));
			    _efree(((executor_globals.active_symbol_table)));
			} else {
			    zend_hash_clean((executor_globals.active_symbol_table));
			    *(++(executor_globals.symtable_cache_ptr)) = (executor_globals.active_symbol_table);
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7661
			case 101: {
			//AddAndReplaceKind
			if (ex && ex->symbol_table) {
			    (executor_globals.active_symbol_table) = ex->symbol_table;
			    return;
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7662
			case 102: {
			//AddAndReplaceKind
			if (ex->CVs[i]) {
			    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7663
			case 103: {
			//AddAndReplaceKind
			if (ex->op_array->this_var != -1 && !ex->CVs[ex->op_array->this_var] && (executor_globals.This)) {
			    ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
			    *ex->CVs[ex->op_array->this_var] = (executor_globals.This);
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7664
			case 104: {
			//AddAndReplaceKind
			if (zend_on_timeout) {
			    zend_on_timeout((executor_globals.timeout_seconds));
			}
			
			break;
			}
			#endif
			#ifdef COMPILE_7665
			case 105: {
			//AddAndReplaceKind
			signal(27, zend_timeout);
			
			break;
			}
			#endif
			#ifdef COMPILE_7666
			case 106: {
			//AddAndReplaceKind
			zend_cleanup_internal_classes();
			
			break;
			}
			#endif
			#ifdef COMPILE_7667
			case 107: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Cannot access parent:: when current class scope has no parent");
			
			break;
			}
			#endif
			#ifdef COMPILE_7668
			case 108: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Cannot access parent:: when no class scope is active");
			
			break;
			}
			#endif
			#ifdef COMPILE_7669
			case 109: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Cannot access self:: when no class scope is active");
			
			break;
			}
			#endif
			#ifdef COMPILE_7670
			case 110: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Cannot access static:: when no class scope is active");
			
			break;
			}
			#endif
			#ifdef COMPILE_7671
			case 111: {
			//AddAndReplaceKind
			zend_error((1 << 0L), "Maximum execution time of %d second%s exceeded", (executor_globals.timeout_seconds), (executor_globals.timeout_seconds) == 1 ? "" : "s");
			
			break;
			}
			#endif
			#ifdef COMPILE_7672
			case 112: {
			//AddAndReplaceKind
			zend_error((1 << 3L), "A non well formed numeric value encountered");
			
			break;
			}
			#endif
			#ifdef COMPILE_7673
			case 113: {
			//AddAndReplaceKind
			zend_error_noreturn((1 << 0L), "Cannot call overloaded function for non-object");
			
			break;
			}
			#endif
			#ifdef COMPILE_7674
			case 114: {
			//AddAndReplaceKind
			zend_exception_error((executor_globals.exception), (1 << 0L));
			
			break;
			}
			#endif
			#ifdef COMPILE_7675
			case 115: {
			//AddAndReplaceKind
			zend_exception_restore();
			
			break;
			}
			#endif
			#ifdef COMPILE_7676
			case 116: {
			//AddAndReplaceKind
			zend_exception_save();
			
			break;
			}
			#endif
			#ifdef COMPILE_7677
			case 117: {
			//AddAndReplaceKind
			zend_execute((compiler_globals.active_op_array));
			
			break;
			}
			#endif
			#ifdef COMPILE_7678
			case 118: {
			//AddAndReplaceKind
			zend_execute((executor_globals.active_op_array));
			
			break;
			}
			#endif
			#ifdef COMPILE_7679
			case 119: {
			//AddAndReplaceKind
			zend_hash_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class_full);
			
			break;
			}
			#endif
			#ifdef COMPILE_7680
			case 120: {
			//AddAndReplaceKind
			zend_hash_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_class_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7681
			case 121: {
			//AddAndReplaceKind
			zend_hash_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function_full);
			
			break;
			}
			#endif
			#ifdef COMPILE_7682
			case 122: {
			//AddAndReplaceKind
			zend_hash_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data_full);
			
			break;
			}
			#endif
			#ifdef COMPILE_7683
			case 123: {
			//AddAndReplaceKind
			zend_hash_clean((executor_globals.active_symbol_table));
			
			break;
			}
			#endif
			#ifdef COMPILE_7684
			case 124: {
			//AddAndReplaceKind
			zend_hash_destroy(&(executor_globals.included_files));
			
			break;
			}
			#endif
			#ifdef COMPILE_7685
			case 125: {
			//AddAndReplaceKind
			zend_hash_destroy((executor_globals.active_symbol_table));
			
			break;
			}
			#endif
			#ifdef COMPILE_7686
			case 126: {
			//AddAndReplaceKind
			zend_hash_destroy((executor_globals.in_autoload));
			
			break;
			}
			#endif
			#ifdef COMPILE_7687
			case 127: {
			//AddAndReplaceKind
			zend_hash_destroy(*(executor_globals.symtable_cache_ptr));
			
			break;
			}
			#endif
			#ifdef COMPILE_7688
			case 128: {
			//AddAndReplaceKind
			zend_hash_graceful_reverse_destroy(&(executor_globals.symbol_table));
			
			break;
			}
			#endif
			#ifdef COMPILE_7689
			case 129: {
			//AddAndReplaceKind
			zend_hash_reverse_apply(&(executor_globals.symbol_table), (apply_func_t)zval_call_destructor);
			
			break;
			}
			#endif
			#ifdef COMPILE_7690
			case 130: {
			//AddAndReplaceKind
			zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class);
			
			break;
			}
			#endif
			#ifdef COMPILE_7691
			case 131: {
			//AddAndReplaceKind
			zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_user_class_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7692
			case 132: {
			//AddAndReplaceKind
			zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function);
			
			break;
			}
			#endif
			#ifdef COMPILE_7693
			case 133: {
			//AddAndReplaceKind
			zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data);
			
			break;
			}
			#endif
			#ifdef COMPILE_7694
			case 134: {
			//AddAndReplaceKind
			zend_init_fpu();
			
			break;
			}
			#endif
			#ifdef COMPILE_7695
			case 135: {
			//AddAndReplaceKind
			zend_llist_apply(&zend_extensions, (llist_apply_func_t)zend_extension_activator);
			
			break;
			}
			#endif
			#ifdef COMPILE_7696
			case 136: {
			//AddAndReplaceKind
			zend_llist_apply(&zend_extensions, (llist_apply_func_t)zend_extension_deactivator);
			
			break;
			}
			#endif
			#ifdef COMPILE_7697
			case 137: {
			//AddAndReplaceKind
			zend_objects_store_call_destructors(&(executor_globals.objects_store));
			
			break;
			}
			#endif
			#ifdef COMPILE_7698
			case 138: {
			//AddAndReplaceKind
			zend_objects_store_destroy(&(executor_globals.objects_store));
			
			break;
			}
			#endif
			#ifdef COMPILE_7699
			case 139: {
			//AddAndReplaceKind
			zend_objects_store_free_object_storage(&(executor_globals.objects_store));
			
			break;
			}
			#endif
			#ifdef COMPILE_7700
			case 140: {
			//AddAndReplaceKind
			zend_objects_store_init(&(executor_globals.objects_store), 1024);
			
			break;
			}
			#endif
			#ifdef COMPILE_7701
			case 141: {
			//AddAndReplaceKind
			zend_objects_store_mark_destructed(&(executor_globals.objects_store));
			
			break;
			}
			#endif
			#ifdef COMPILE_7702
			case 142: {
			//AddAndReplaceKind
			zend_on_timeout((executor_globals.timeout_seconds));
			
			break;
			}
			#endif
			#ifdef COMPILE_7703
			case 143: {
			//AddAndReplaceKind
			zend_ptr_stack_clean(&(executor_globals.user_error_handlers), (void (*)(void *))_zval_dtor_wrapper, 1);
			
			break;
			}
			#endif
			#ifdef COMPILE_7704
			case 144: {
			//AddAndReplaceKind
			zend_ptr_stack_clean(&(executor_globals.user_exception_handlers), (void (*)(void *))_zval_dtor_wrapper, 1);
			
			break;
			}
			#endif
			#ifdef COMPILE_7705
			case 145: {
			//AddAndReplaceKind
			zend_ptr_stack_destroy(&(executor_globals.arg_types_stack));
			
			break;
			}
			#endif
			#ifdef COMPILE_7706
			case 146: {
			//AddAndReplaceKind
			zend_ptr_stack_destroy(&(executor_globals.user_error_handlers));
			
			break;
			}
			#endif
			#ifdef COMPILE_7707
			case 147: {
			//AddAndReplaceKind
			zend_ptr_stack_destroy(&(executor_globals.user_exception_handlers));
			
			break;
			}
			#endif
			#ifdef COMPILE_7708
			case 148: {
			//AddAndReplaceKind
			zend_ptr_stack_init(&(executor_globals.arg_types_stack));
			
			break;
			}
			#endif
			#ifdef COMPILE_7709
			case 149: {
			//AddAndReplaceKind
			zend_ptr_stack_init(&(executor_globals.user_error_handlers));
			
			break;
			}
			#endif
			#ifdef COMPILE_7710
			case 150: {
			//AddAndReplaceKind
			zend_ptr_stack_init(&(executor_globals.user_exception_handlers));
			
			break;
			}
			#endif
			#ifdef COMPILE_7711
			case 151: {
			//AddAndReplaceKind
			zend_rebuild_symbol_table();
			
			break;
			}
			#endif
			#ifdef COMPILE_7712
			case 152: {
			//AddAndReplaceKind
			zend_release_labels();
			
			break;
			}
			#endif
			#ifdef COMPILE_7713
			case 153: {
			//AddAndReplaceKind
			zend_shutdown_fpu();
			
			break;
			}
			#endif
			#ifdef COMPILE_7714
			case 154: {
			//AddAndReplaceKind
			zend_stack_destroy(&(executor_globals.user_error_handlers_error_reporting));
			
			break;
			}
			#endif
			#ifdef COMPILE_7715
			case 155: {
			//AddAndReplaceKind
			zend_stack_init(&(executor_globals.user_error_handlers_error_reporting));
			
			break;
			}
			#endif
			#ifdef COMPILE_7716
			case 156: {
			//AddAndReplaceKind
			zend_throw_exception_internal(((void *)0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7717
			case 157: {
			//AddAndReplaceKind
			zend_vm_stack_clear_multiple();
			
			break;
			}
			#endif
			#ifdef COMPILE_7718
			case 158: {
			//AddAndReplaceKind
			zend_vm_stack_destroy();
			
			break;
			}
			#endif
			#ifdef COMPILE_7719
			case 159: {
			//AddAndReplaceKind
			zend_vm_stack_init();
			
			break;
			}
			#endif
			#ifdef COMPILE_7720
			case 160: {
			//AddAndReplaceKind
			zend_vm_stack_push((void *)((void *)0));
			
			break;
			}
			#endif
			#ifdef COMPILE_7721
			case 161: {
			//AddAndReplaceKind
			zval_addref_p(&((executor_globals.uninitialized_zval)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7722
			case 162: {
			//AddAndReplaceKind
			zval_addref_p((executor_globals.This));
			
			break;
			}
			#endif
			#ifdef COMPILE_7723
			case 163: {
			//AddInitKind
			memset(&executor_globals, 0, sizeof (*(&executor_globals)));
			
			break;
			}
			#endif
			#ifdef COMPILE_7724
			case 164: {
			//IfExitKind
			if (__is_neg("64-164", 92, &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex), sizeof (ex), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(i), sizeof (i), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
			    return;
			
			break;
			}
			#endif
			}
			switch(__choose("__SWITCH65"))
			{
			case 0: {
			EG(active_symbol_table) = *(EG(symtable_cache_ptr)--);
			
			break;
			}
			#ifdef COMPILE_7725
			case 1: {
			//GuardKind
			if (!__is_neg("65-1", 92, &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex), sizeof (ex), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(i), sizeof (i), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
			    (executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
			
			break;
			}
			#endif
			}
			}
						} else {
							ALLOC_HASHTABLE(EG(active_symbol_table));
							zend_hash_init(EG(active_symbol_table), ex->op_array->last_var, NULL, ZVAL_PTR_DTOR, 0);
							/*printf("Cache miss!  Initialized %x\n", EG(active_symbol_table));*/
						}
			
			break;
			}
			#ifdef COMPILE_7726
			case 1: {
			//GuardKind
			if (!__is_neg("66-1", 92, &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex), sizeof (ex), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(i), sizeof (i), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
			    (executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
			
			break;
			}
			#endif
			}
			}
			}
			ex->symbol_table = EG(active_symbol_table);

			if (ex->op_array->this_var != -1 &&
			    !ex->CVs[ex->op_array->this_var] &&
			    EG(This)) {
				ex->CVs[ex->op_array->this_var] = (zval**)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				*ex->CVs[ex->op_array->this_var] = EG(This);
 			}
			for (i = 0; i < ex->op_array->last_var; i++) {
				//prophet generated patch
				{
				switch(__choose("__SWITCH67"))
				{
				case 0: 
				break;
				#ifdef COMPILE_7727
				case 1: {
				//AddAndReplaceKind
				((executor_globals.active_symbol_table)) = (HashTable *)_emalloc((sizeof(HashTable)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7728
				case 2: {
				//AddAndReplaceKind
				((executor_globals.in_autoload)) = (HashTable *)_emalloc((sizeof(HashTable)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7729
				case 3: {
				//AddAndReplaceKind
				(compiler_globals.compiler_options) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7730
				case 4: {
				//AddAndReplaceKind
				(compiler_globals.interactive) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7731
				case 5: {
				//AddAndReplaceKind
				(ex) = (executor_globals.current_execute_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_7732
				case 6: {
				//AddAndReplaceKind
				(ex) = ex->prev_execute_data;
				
				break;
				}
				#endif
				#ifdef COMPILE_7733
				case 7: {
				//AddAndReplaceKind
				(ex)->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7734
				case 8: {
				//AddAndReplaceKind
				(ex)->symbol_table = (executor_globals.active_symbol_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7735
				case 9: {
				//AddAndReplaceKind
				(executor_globals.This) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7736
				case 10: {
				//AddAndReplaceKind
				(executor_globals.active) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7737
				case 11: {
				//AddAndReplaceKind
				(executor_globals.active) = 1;
				
				break;
				}
				#endif
				#ifdef COMPILE_7738
				case 12: {
				//AddAndReplaceKind
				(executor_globals.active_op_array) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7739
				case 13: {
				//AddAndReplaceKind
				(executor_globals.active_op_array) = (compiler_globals.active_op_array);
				
				break;
				}
				#endif
				#ifdef COMPILE_7740
				case 14: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = &(executor_globals.symbol_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7741
				case 15: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7742
				case 16: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = (ex)->symbol_table;
				
				break;
				}
				#endif
				#ifdef COMPILE_7743
				case 17: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
				
				break;
				}
				#endif
				#ifdef COMPILE_7744
				case 18: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = ex->symbol_table;
				
				break;
				}
				#endif
				#ifdef COMPILE_7745
				case 19: {
				//AddAndReplaceKind
				(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
				
				break;
				}
				#endif
				#ifdef COMPILE_7746
				case 20: {
				//AddAndReplaceKind
				(executor_globals.autoload_func) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7747
				case 21: {
				//AddAndReplaceKind
				(executor_globals.called_scope) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7748
				case 22: {
				//AddAndReplaceKind
				(executor_globals.class_table) = (compiler_globals.class_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7749
				case 23: {
				//AddAndReplaceKind
				(executor_globals.current_execute_data) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7750
				case 24: {
				//AddAndReplaceKind
				(executor_globals.error_handling) = (EH_NORMAL);
				
				break;
				}
				#endif
				#ifdef COMPILE_7751
				case 25: {
				//AddAndReplaceKind
				(executor_globals.error_handling) = (EH_SUPPRESS);
				
				break;
				}
				#endif
				#ifdef COMPILE_7752
				case 26: {
				//AddAndReplaceKind
				(executor_globals.error_handling) = (EH_THROW);
				
				break;
				}
				#endif
				#ifdef COMPILE_7753
				case 27: {
				//AddAndReplaceKind
				(executor_globals.error_handling) = EH_NORMAL;
				
				break;
				}
				#endif
				#ifdef COMPILE_7754
				case 28: {
				//AddAndReplaceKind
				(executor_globals.error_zval) = zval_used_for_init;
				
				break;
				}
				#endif
				#ifdef COMPILE_7755
				case 29: {
				//AddAndReplaceKind
				(executor_globals.error_zval_ptr) = &(executor_globals.error_zval);
				
				break;
				}
				#endif
				#ifdef COMPILE_7756
				case 30: {
				//AddAndReplaceKind
				(executor_globals.exception) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7757
				case 31: {
				//AddAndReplaceKind
				(executor_globals.full_tables_cleanup) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7758
				case 32: {
				//AddAndReplaceKind
				(executor_globals.function_table) = (compiler_globals.function_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7759
				case 33: {
				//AddAndReplaceKind
				(executor_globals.in_autoload) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7760
				case 34: {
				//AddAndReplaceKind
				(executor_globals.in_execution) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7761
				case 35: {
				//AddAndReplaceKind
				(executor_globals.no_extensions) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7762
				case 36: {
				//AddAndReplaceKind
				(executor_globals.no_extensions) = 1;
				
				break;
				}
				#endif
				#ifdef COMPILE_7763
				case 37: {
				//AddAndReplaceKind
				(executor_globals.opline_ptr) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7764
				case 38: {
				//AddAndReplaceKind
				(executor_globals.prev_exception) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7765
				case 39: {
				//AddAndReplaceKind
				(executor_globals.return_value_ptr_ptr) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7766
				case 40: {
				//AddAndReplaceKind
				(executor_globals.scope) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7767
				case 41: {
				//AddAndReplaceKind
				(executor_globals.start_op) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7768
				case 42: {
				//AddAndReplaceKind
				(executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes + (compiler_globals.active_op_array)->last;
				
				break;
				}
				#endif
				#ifdef COMPILE_7769
				case 43: {
				//AddAndReplaceKind
				(executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes;
				
				break;
				}
				#endif
				#ifdef COMPILE_7770
				case 44: {
				//AddAndReplaceKind
				(executor_globals.symtable_cache_limit) = (executor_globals.symtable_cache) + 32 - 1;
				
				break;
				}
				#endif
				#ifdef COMPILE_7771
				case 45: {
				//AddAndReplaceKind
				(executor_globals.symtable_cache_ptr) = (executor_globals.symtable_cache) - 1;
				
				break;
				}
				#endif
				#ifdef COMPILE_7772
				case 46: {
				//AddAndReplaceKind
				(executor_globals.symtable_cache_ptr)--;
				
				break;
				}
				#endif
				#ifdef COMPILE_7773
				case 47: {
				//AddAndReplaceKind
				(executor_globals.ticks_count) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7774
				case 48: {
				//AddAndReplaceKind
				(executor_globals.uninitialized_zval) = zval_used_for_init;
				
				break;
				}
				#endif
				#ifdef COMPILE_7775
				case 49: {
				//AddAndReplaceKind
				(executor_globals.uninitialized_zval_ptr) = &(executor_globals.uninitialized_zval);
				
				break;
				}
				#endif
				#ifdef COMPILE_7776
				case 50: {
				//AddAndReplaceKind
				(executor_globals.user_error_handler) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7777
				case 51: {
				//AddAndReplaceKind
				(executor_globals.user_exception_handler) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7778
				case 52: {
				//AddAndReplaceKind
				*(++(executor_globals.symtable_cache_ptr)) = (executor_globals.active_symbol_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7779
				case 53: {
				//AddAndReplaceKind
				*(ex)->CVs[ex->op_array->this_var] = (executor_globals.This);
				
				break;
				}
				#endif
				#ifdef COMPILE_7780
				case 54: {
				//AddAndReplaceKind
				*ex->CVs[(ex)->op_array->this_var] = (executor_globals.This);
				
				break;
				}
				#endif
				#ifdef COMPILE_7781
				case 55: {
				//AddAndReplaceKind
				*ex->CVs[ex->op_array->this_var] = (executor_globals.This);
				
				break;
				}
				#endif
				#ifdef COMPILE_7782
				case 56: {
				//AddAndReplaceKind
				_efree(((executor_globals.active_symbol_table)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7783
				case 57: {
				//AddAndReplaceKind
				_efree(((executor_globals.in_autoload)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7784
				case 58: {
				//AddAndReplaceKind
				_efree((*(executor_globals.symtable_cache_ptr)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7785
				case 59: {
				//AddAndReplaceKind
				_zend_hash_init((&(executor_globals.included_files)), (5), (((void *)0)), (((void *)0)), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7786
				case 60: {
				//AddAndReplaceKind
				_zend_hash_init((&(executor_globals.symbol_table)), (50), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7787
				case 61: {
				//AddAndReplaceKind
				_zend_hash_init(((executor_globals.active_symbol_table)), ((ex)->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7788
				case 62: {
				//AddAndReplaceKind
				_zend_hash_init(((executor_globals.active_symbol_table)), (ex->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7789
				case 63: {
				//AddAndReplaceKind
				_zend_hash_init(((executor_globals.in_autoload)), (0), (((void *)0)), (((void *)0)), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7790
				case 64: {
				//AddAndReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (ex)->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7791
				case 65: {
				//AddAndReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, (ex)->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7792
				case 66: {
				//AddAndReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, (ex)->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7793
				case 67: {
				//AddAndReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7794
				case 68: {
				//AddAndReplaceKind
				_zval_ptr_dtor((&(executor_globals.This)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7795
				case 69: {
				//AddAndReplaceKind
				clean_non_persistent_constants();
				
				break;
				}
				#endif
				#ifdef COMPILE_7796
				case 70: {
				//AddAndReplaceKind
				ex = (ex)->prev_execute_data;
				
				break;
				}
				#endif
				#ifdef COMPILE_7797
				case 71: {
				//AddAndReplaceKind
				ex = (executor_globals.current_execute_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_7798
				case 72: {
				//AddAndReplaceKind
				ex = ex->prev_execute_data;
				
				break;
				}
				#endif
				#ifdef COMPILE_7799
				case 73: {
				//AddAndReplaceKind
				ex->CVs[(ex)->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7800
				case 74: {
				//AddAndReplaceKind
				ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + (ex)->op_array->last_var + ex->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7801
				case 75: {
				//AddAndReplaceKind
				ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + (ex)->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7802
				case 76: {
				//AddAndReplaceKind
				ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7803
				case 77: {
				//AddAndReplaceKind
				ex->symbol_table = (executor_globals.active_symbol_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7804
				case 78: {
				//AddAndReplaceKind
				exit(1);
				
				break;
				}
				#endif
				#ifdef COMPILE_7805
				case 79: {
				//AddAndReplaceKind
				if (!((compiler_globals.active_op_array)->fn_flags & 16) || (compiler_globals.context).backpatch_count > 0 || (compiler_globals.active_op_array)->function_name || (compiler_globals.active_op_array)->type != 2) {
				    return;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7806
				case 80: {
				//AddAndReplaceKind
				if (!(executor_globals.active)) {
				    return -1;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7807
				case 81: {
				//AddAndReplaceKind
				if (!(executor_globals.active_symbol_table)) {
				    zend_rebuild_symbol_table();
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7808
				case 82: {
				//AddAndReplaceKind
				if (!(executor_globals.called_scope)) {
				    zend_error((1 << 0L), "Cannot access static:: when no class scope is active");
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7809
				case 83: {
				//AddAndReplaceKind
				if (!(executor_globals.scope)) {
				    zend_error((1 << 0L), "Cannot access parent:: when no class scope is active");
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7810
				case 84: {
				//AddAndReplaceKind
				if (!(executor_globals.scope)) {
				    zend_error((1 << 0L), "Cannot access self:: when no class scope is active");
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7811
				case 85: {
				//AddAndReplaceKind
				if (!(executor_globals.scope)->parent) {
				    zend_error((1 << 0L), "Cannot access parent:: when current class scope has no parent");
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7812
				case 86: {
				//AddAndReplaceKind
				if (!(executor_globals.start_op)) {
				    (executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7813
				case 87: {
				//AddAndReplaceKind
				if (!zend_is_executing()) {
				    return ((void *)0);
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7814
				case 88: {
				//AddAndReplaceKind
				if ((executor_globals.This)) {
				    _zval_ptr_dtor((&(executor_globals.This)));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7815
				case 89: {
				//AddAndReplaceKind
				if ((executor_globals.active_op_array)) {
				    return (executor_globals.active_op_array)->filename;
				} else {
				    return "[no active file]";
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7816
				case 90: {
				//AddAndReplaceKind
				if ((executor_globals.exception) && (executor_globals.opline_ptr) && (*(executor_globals.opline_ptr))->opcode == 149 && (*(executor_globals.opline_ptr))->lineno == 0 && (executor_globals.opline_before_exception)) {
				    return (executor_globals.opline_before_exception)->lineno;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7817
				case 91: {
				//AddAndReplaceKind
				if ((executor_globals.exception)) {
				    return -1;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7818
				case 92: {
				//AddAndReplaceKind
				if ((executor_globals.exception)) {
				    zend_exception_error((executor_globals.exception), (1 << 0L));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7819
				case 93: {
				//AddAndReplaceKind
				if ((executor_globals.exception)) {
				    zend_throw_exception_internal(((void *)0));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7820
				case 94: {
				//AddAndReplaceKind
				if ((executor_globals.full_tables_cleanup)) {
				    zend_hash_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function_full);
				    zend_hash_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class_full);
				} else {
				    zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function);
				    zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class);
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7821
				case 95: {
				//AddAndReplaceKind
				if ((executor_globals.full_tables_cleanup)) {
				    zend_hash_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data_full);
				    zend_hash_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_class_data);
				} else {
				    zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data);
				    zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_user_class_data);
				    zend_cleanup_internal_classes();
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7822
				case 96: {
				//AddAndReplaceKind
				if ((executor_globals.in_autoload) == ((void *)0)) {
				    ((executor_globals.in_autoload)) = (HashTable *)_emalloc((sizeof(HashTable)));
				    _zend_hash_init(((executor_globals.in_autoload)), (0), (((void *)0)), (((void *)0)), (0));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7823
				case 97: {
				//AddAndReplaceKind
				if ((executor_globals.in_autoload)) {
				    zend_hash_destroy((executor_globals.in_autoload));
				    _efree(((executor_globals.in_autoload)));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7824
				case 98: {
				//AddAndReplaceKind
				if ((executor_globals.opline_ptr)) {
				    return (*(executor_globals.opline_ptr))->lineno;
				} else {
				    return 0;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7825
				case 99: {
				//AddAndReplaceKind
				if ((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache)) {
				    (executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
				} else {
				    ((executor_globals.active_symbol_table)) = (HashTable *)_emalloc((sizeof(HashTable)));
				    _zend_hash_init(((executor_globals.active_symbol_table)), (ex->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7826
				case 100: {
				//AddAndReplaceKind
				if ((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache_limit)) {
				    zend_hash_destroy((executor_globals.active_symbol_table));
				    _efree(((executor_globals.active_symbol_table)));
				} else {
				    zend_hash_clean((executor_globals.active_symbol_table));
				    *(++(executor_globals.symtable_cache_ptr)) = (executor_globals.active_symbol_table);
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7827
				case 101: {
				//AddAndReplaceKind
				if (ex && ex->symbol_table) {
				    (executor_globals.active_symbol_table) = ex->symbol_table;
				    return;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7828
				case 102: {
				//AddAndReplaceKind
				if (ex->CVs[i]) {
				    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7829
				case 103: {
				//AddAndReplaceKind
				if (ex->op_array->this_var != -1 && !ex->CVs[ex->op_array->this_var] && (executor_globals.This)) {
				    ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				    *ex->CVs[ex->op_array->this_var] = (executor_globals.This);
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7830
				case 104: {
				//AddAndReplaceKind
				if (zend_on_timeout) {
				    zend_on_timeout((executor_globals.timeout_seconds));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7831
				case 105: {
				//AddAndReplaceKind
				signal(27, zend_timeout);
				
				break;
				}
				#endif
				#ifdef COMPILE_7832
				case 106: {
				//AddAndReplaceKind
				zend_cleanup_internal_classes();
				
				break;
				}
				#endif
				#ifdef COMPILE_7833
				case 107: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Cannot access parent:: when current class scope has no parent");
				
				break;
				}
				#endif
				#ifdef COMPILE_7834
				case 108: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Cannot access parent:: when no class scope is active");
				
				break;
				}
				#endif
				#ifdef COMPILE_7835
				case 109: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Cannot access self:: when no class scope is active");
				
				break;
				}
				#endif
				#ifdef COMPILE_7836
				case 110: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Cannot access static:: when no class scope is active");
				
				break;
				}
				#endif
				#ifdef COMPILE_7837
				case 111: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Maximum execution time of %d second%s exceeded", (executor_globals.timeout_seconds), (executor_globals.timeout_seconds) == 1 ? "" : "s");
				
				break;
				}
				#endif
				#ifdef COMPILE_7838
				case 112: {
				//AddAndReplaceKind
				zend_error((1 << 3L), "A non well formed numeric value encountered");
				
				break;
				}
				#endif
				#ifdef COMPILE_7839
				case 113: {
				//AddAndReplaceKind
				zend_error_noreturn((1 << 0L), "Cannot call overloaded function for non-object");
				
				break;
				}
				#endif
				#ifdef COMPILE_7840
				case 114: {
				//AddAndReplaceKind
				zend_exception_error((executor_globals.exception), (1 << 0L));
				
				break;
				}
				#endif
				#ifdef COMPILE_7841
				case 115: {
				//AddAndReplaceKind
				zend_exception_restore();
				
				break;
				}
				#endif
				#ifdef COMPILE_7842
				case 116: {
				//AddAndReplaceKind
				zend_exception_save();
				
				break;
				}
				#endif
				#ifdef COMPILE_7843
				case 117: {
				//AddAndReplaceKind
				zend_execute((compiler_globals.active_op_array));
				
				break;
				}
				#endif
				#ifdef COMPILE_7844
				case 118: {
				//AddAndReplaceKind
				zend_execute((executor_globals.active_op_array));
				
				break;
				}
				#endif
				#ifdef COMPILE_7845
				case 119: {
				//AddAndReplaceKind
				zend_hash_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class_full);
				
				break;
				}
				#endif
				#ifdef COMPILE_7846
				case 120: {
				//AddAndReplaceKind
				zend_hash_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_class_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_7847
				case 121: {
				//AddAndReplaceKind
				zend_hash_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function_full);
				
				break;
				}
				#endif
				#ifdef COMPILE_7848
				case 122: {
				//AddAndReplaceKind
				zend_hash_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data_full);
				
				break;
				}
				#endif
				#ifdef COMPILE_7849
				case 123: {
				//AddAndReplaceKind
				zend_hash_clean((executor_globals.active_symbol_table));
				
				break;
				}
				#endif
				#ifdef COMPILE_7850
				case 124: {
				//AddAndReplaceKind
				zend_hash_destroy(&(executor_globals.included_files));
				
				break;
				}
				#endif
				#ifdef COMPILE_7851
				case 125: {
				//AddAndReplaceKind
				zend_hash_destroy((executor_globals.active_symbol_table));
				
				break;
				}
				#endif
				#ifdef COMPILE_7852
				case 126: {
				//AddAndReplaceKind
				zend_hash_destroy((executor_globals.in_autoload));
				
				break;
				}
				#endif
				#ifdef COMPILE_7853
				case 127: {
				//AddAndReplaceKind
				zend_hash_destroy(*(executor_globals.symtable_cache_ptr));
				
				break;
				}
				#endif
				#ifdef COMPILE_7854
				case 128: {
				//AddAndReplaceKind
				zend_hash_graceful_reverse_destroy(&(executor_globals.symbol_table));
				
				break;
				}
				#endif
				#ifdef COMPILE_7855
				case 129: {
				//AddAndReplaceKind
				zend_hash_reverse_apply(&(executor_globals.symbol_table), (apply_func_t)zval_call_destructor);
				
				break;
				}
				#endif
				#ifdef COMPILE_7856
				case 130: {
				//AddAndReplaceKind
				zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class);
				
				break;
				}
				#endif
				#ifdef COMPILE_7857
				case 131: {
				//AddAndReplaceKind
				zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_user_class_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_7858
				case 132: {
				//AddAndReplaceKind
				zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function);
				
				break;
				}
				#endif
				#ifdef COMPILE_7859
				case 133: {
				//AddAndReplaceKind
				zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_7860
				case 134: {
				//AddAndReplaceKind
				zend_init_fpu();
				
				break;
				}
				#endif
				#ifdef COMPILE_7861
				case 135: {
				//AddAndReplaceKind
				zend_llist_apply(&zend_extensions, (llist_apply_func_t)zend_extension_activator);
				
				break;
				}
				#endif
				#ifdef COMPILE_7862
				case 136: {
				//AddAndReplaceKind
				zend_llist_apply(&zend_extensions, (llist_apply_func_t)zend_extension_deactivator);
				
				break;
				}
				#endif
				#ifdef COMPILE_7863
				case 137: {
				//AddAndReplaceKind
				zend_objects_store_call_destructors(&(executor_globals.objects_store));
				
				break;
				}
				#endif
				#ifdef COMPILE_7864
				case 138: {
				//AddAndReplaceKind
				zend_objects_store_destroy(&(executor_globals.objects_store));
				
				break;
				}
				#endif
				#ifdef COMPILE_7865
				case 139: {
				//AddAndReplaceKind
				zend_objects_store_free_object_storage(&(executor_globals.objects_store));
				
				break;
				}
				#endif
				#ifdef COMPILE_7866
				case 140: {
				//AddAndReplaceKind
				zend_objects_store_init(&(executor_globals.objects_store), 1024);
				
				break;
				}
				#endif
				#ifdef COMPILE_7867
				case 141: {
				//AddAndReplaceKind
				zend_objects_store_mark_destructed(&(executor_globals.objects_store));
				
				break;
				}
				#endif
				#ifdef COMPILE_7868
				case 142: {
				//AddAndReplaceKind
				zend_on_timeout((executor_globals.timeout_seconds));
				
				break;
				}
				#endif
				#ifdef COMPILE_7869
				case 143: {
				//AddAndReplaceKind
				zend_ptr_stack_clean(&(executor_globals.user_error_handlers), (void (*)(void *))_zval_dtor_wrapper, 1);
				
				break;
				}
				#endif
				#ifdef COMPILE_7870
				case 144: {
				//AddAndReplaceKind
				zend_ptr_stack_clean(&(executor_globals.user_exception_handlers), (void (*)(void *))_zval_dtor_wrapper, 1);
				
				break;
				}
				#endif
				#ifdef COMPILE_7871
				case 145: {
				//AddAndReplaceKind
				zend_ptr_stack_destroy(&(executor_globals.arg_types_stack));
				
				break;
				}
				#endif
				#ifdef COMPILE_7872
				case 146: {
				//AddAndReplaceKind
				zend_ptr_stack_destroy(&(executor_globals.user_error_handlers));
				
				break;
				}
				#endif
				#ifdef COMPILE_7873
				case 147: {
				//AddAndReplaceKind
				zend_ptr_stack_destroy(&(executor_globals.user_exception_handlers));
				
				break;
				}
				#endif
				#ifdef COMPILE_7874
				case 148: {
				//AddAndReplaceKind
				zend_ptr_stack_init(&(executor_globals.arg_types_stack));
				
				break;
				}
				#endif
				#ifdef COMPILE_7875
				case 149: {
				//AddAndReplaceKind
				zend_ptr_stack_init(&(executor_globals.user_error_handlers));
				
				break;
				}
				#endif
				#ifdef COMPILE_7876
				case 150: {
				//AddAndReplaceKind
				zend_ptr_stack_init(&(executor_globals.user_exception_handlers));
				
				break;
				}
				#endif
				#ifdef COMPILE_7877
				case 151: {
				//AddAndReplaceKind
				zend_rebuild_symbol_table();
				
				break;
				}
				#endif
				#ifdef COMPILE_7878
				case 152: {
				//AddAndReplaceKind
				zend_release_labels();
				
				break;
				}
				#endif
				#ifdef COMPILE_7879
				case 153: {
				//AddAndReplaceKind
				zend_shutdown_fpu();
				
				break;
				}
				#endif
				#ifdef COMPILE_7880
				case 154: {
				//AddAndReplaceKind
				zend_stack_destroy(&(executor_globals.user_error_handlers_error_reporting));
				
				break;
				}
				#endif
				#ifdef COMPILE_7881
				case 155: {
				//AddAndReplaceKind
				zend_stack_init(&(executor_globals.user_error_handlers_error_reporting));
				
				break;
				}
				#endif
				#ifdef COMPILE_7882
				case 156: {
				//AddAndReplaceKind
				zend_throw_exception_internal(((void *)0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7883
				case 157: {
				//AddAndReplaceKind
				zend_vm_stack_clear_multiple();
				
				break;
				}
				#endif
				#ifdef COMPILE_7884
				case 158: {
				//AddAndReplaceKind
				zend_vm_stack_destroy();
				
				break;
				}
				#endif
				#ifdef COMPILE_7885
				case 159: {
				//AddAndReplaceKind
				zend_vm_stack_init();
				
				break;
				}
				#endif
				#ifdef COMPILE_7886
				case 160: {
				//AddAndReplaceKind
				zend_vm_stack_push((void *)((void *)0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7887
				case 161: {
				//AddAndReplaceKind
				zval_addref_p(&((executor_globals.uninitialized_zval)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7888
				case 162: {
				//AddAndReplaceKind
				zval_addref_p((executor_globals.This));
				
				break;
				}
				#endif
				}
				{
				long long __temp68=(ex->CVs[i]) ;
				switch(__choose("__SWITCH68"))
				{
				case 0: {
				
				break;
				}
				#ifdef COMPILE_7889
				case 1: {
				__temp68= ((ex->CVs[i]) && !__is_neg("68-1", 141, &(i), sizeof (i), &(ex), sizeof (ex), &(ex->CVs), sizeof (ex->CVs), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->vars), sizeof (ex->op_array->vars), &(ex->op_array->last_var), sizeof (ex->op_array->last_var), &(ex->op_array->vars[i].name), sizeof (ex->op_array->vars[i].name), &(ex->op_array->vars[i].name_len), sizeof (ex->op_array->vars[i].name_len), &(ex->op_array->vars[i].hash_value), sizeof (ex->op_array->vars[i].hash_value), &(ex->op_array), sizeof (ex->op_array), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.This), sizeof (executor_globals.This), &(ex->op_array->this_var), sizeof (ex->op_array->this_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->symbol_table), sizeof (ex->symbol_table), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(ex->prev_execute_data), sizeof (ex->prev_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(ex->op_array->type), sizeof (ex->op_array->type), &(ex->op_array->function_name), sizeof (ex->op_array->function_name), &(ex->op_array->scope), sizeof (ex->op_array->scope), &(ex->op_array->fn_flags), sizeof (ex->op_array->fn_flags), &(ex->op_array->prototype), sizeof (ex->op_array->prototype), &(ex->op_array->num_args), sizeof (ex->op_array->num_args), &(ex->op_array->required_num_args), sizeof (ex->op_array->required_num_args), &(ex->op_array->arg_info), sizeof (ex->op_array->arg_info), &(ex->op_array->refcount), sizeof (ex->op_array->refcount), &(ex->op_array->opcodes), sizeof (ex->op_array->opcodes), &(ex->op_array->last), sizeof (ex->op_array->last), &(ex->op_array->T), sizeof (ex->op_array->T), &(ex->op_array->brk_cont_array), sizeof (ex->op_array->brk_cont_array), &(ex->op_array->last_brk_cont), sizeof (ex->op_array->last_brk_cont), &(ex->op_array->try_catch_array), sizeof (ex->op_array->try_catch_array), &(ex->op_array->last_try_catch), sizeof (ex->op_array->last_try_catch), &(ex->op_array->static_variables), sizeof (ex->op_array->static_variables), &(ex->op_array->filename), sizeof (ex->op_array->filename), &(ex->op_array->line_start), sizeof (ex->op_array->line_start), &(ex->op_array->line_end), sizeof (ex->op_array->line_end), &(ex->op_array->doc_comment), sizeof (ex->op_array->doc_comment), &(ex->op_array->doc_comment_len), sizeof (ex->op_array->doc_comment_len), &(ex->op_array->early_binding), sizeof (ex->op_array->early_binding), &(ex->op_array->literals), sizeof (ex->op_array->literals), &(ex->op_array->last_literal), sizeof (ex->op_array->last_literal), &(ex->op_array->run_time_cache), sizeof (ex->op_array->run_time_cache), &(ex->op_array->last_cache_slot), sizeof (ex->op_array->last_cache_slot), &(ex->opline), sizeof (ex->opline), &(ex->fbc), sizeof (ex->fbc), &(ex->called_scope), sizeof (ex->called_scope), &(ex->object), sizeof (ex->object), &(ex->Ts), sizeof (ex->Ts), &(ex->old_error_reporting), sizeof (ex->old_error_reporting), &(ex->nested), sizeof (ex->nested), &(ex->original_return_value), sizeof (ex->original_return_value), &(ex->current_scope), sizeof (ex->current_scope), &(ex->current_called_scope), sizeof (ex->current_called_scope), &(ex->current_this), sizeof (ex->current_this), &(ex->current_object), sizeof (ex->current_object), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)));
				
				break;
				}
				#endif
				}
				switch(__choose("__SWITCH71"))
				{
				case 0: {
				if (__temp68) {
									{
				switch(__choose("__SWITCH69"))
				{
				case 0: 
				break;
				#ifdef COMPILE_7890
				case 1: {
				//AddAndReplaceKind
				((executor_globals.active_symbol_table)) = (HashTable *)_emalloc((sizeof(HashTable)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7891
				case 2: {
				//AddAndReplaceKind
				((executor_globals.in_autoload)) = (HashTable *)_emalloc((sizeof(HashTable)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7892
				case 3: {
				//AddAndReplaceKind
				(compiler_globals.compiler_options) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7893
				case 4: {
				//AddAndReplaceKind
				(compiler_globals.interactive) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7894
				case 5: {
				//AddAndReplaceKind
				(ex) = (executor_globals.current_execute_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_7895
				case 6: {
				//AddAndReplaceKind
				(ex) = ex->prev_execute_data;
				
				break;
				}
				#endif
				#ifdef COMPILE_7896
				case 7: {
				//AddAndReplaceKind
				(ex)->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7897
				case 8: {
				//AddAndReplaceKind
				(ex)->symbol_table = (executor_globals.active_symbol_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7898
				case 9: {
				//AddAndReplaceKind
				(executor_globals.This) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7899
				case 10: {
				//AddAndReplaceKind
				(executor_globals.active) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7900
				case 11: {
				//AddAndReplaceKind
				(executor_globals.active) = 1;
				
				break;
				}
				#endif
				#ifdef COMPILE_7901
				case 12: {
				//AddAndReplaceKind
				(executor_globals.active_op_array) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7902
				case 13: {
				//AddAndReplaceKind
				(executor_globals.active_op_array) = (compiler_globals.active_op_array);
				
				break;
				}
				#endif
				#ifdef COMPILE_7903
				case 14: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = &(executor_globals.symbol_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7904
				case 15: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7905
				case 16: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = (ex)->symbol_table;
				
				break;
				}
				#endif
				#ifdef COMPILE_7906
				case 17: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
				
				break;
				}
				#endif
				#ifdef COMPILE_7907
				case 18: {
				//AddAndReplaceKind
				(executor_globals.active_symbol_table) = ex->symbol_table;
				
				break;
				}
				#endif
				#ifdef COMPILE_7908
				case 19: {
				//AddAndReplaceKind
				(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
				
				break;
				}
				#endif
				#ifdef COMPILE_7909
				case 20: {
				//AddAndReplaceKind
				(executor_globals.autoload_func) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7910
				case 21: {
				//AddAndReplaceKind
				(executor_globals.called_scope) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7911
				case 22: {
				//AddAndReplaceKind
				(executor_globals.class_table) = (compiler_globals.class_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7912
				case 23: {
				//AddAndReplaceKind
				(executor_globals.current_execute_data) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7913
				case 24: {
				//AddAndReplaceKind
				(executor_globals.error_handling) = (EH_NORMAL);
				
				break;
				}
				#endif
				#ifdef COMPILE_7914
				case 25: {
				//AddAndReplaceKind
				(executor_globals.error_handling) = (EH_SUPPRESS);
				
				break;
				}
				#endif
				#ifdef COMPILE_7915
				case 26: {
				//AddAndReplaceKind
				(executor_globals.error_handling) = (EH_THROW);
				
				break;
				}
				#endif
				#ifdef COMPILE_7916
				case 27: {
				//AddAndReplaceKind
				(executor_globals.error_handling) = EH_NORMAL;
				
				break;
				}
				#endif
				#ifdef COMPILE_7917
				case 28: {
				//AddAndReplaceKind
				(executor_globals.error_zval) = zval_used_for_init;
				
				break;
				}
				#endif
				#ifdef COMPILE_7918
				case 29: {
				//AddAndReplaceKind
				(executor_globals.error_zval_ptr) = &(executor_globals.error_zval);
				
				break;
				}
				#endif
				#ifdef COMPILE_7919
				case 30: {
				//AddAndReplaceKind
				(executor_globals.exception) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7920
				case 31: {
				//AddAndReplaceKind
				(executor_globals.full_tables_cleanup) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7921
				case 32: {
				//AddAndReplaceKind
				(executor_globals.function_table) = (compiler_globals.function_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7922
				case 33: {
				//AddAndReplaceKind
				(executor_globals.in_autoload) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7923
				case 34: {
				//AddAndReplaceKind
				(executor_globals.in_execution) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7924
				case 35: {
				//AddAndReplaceKind
				(executor_globals.no_extensions) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7925
				case 36: {
				//AddAndReplaceKind
				(executor_globals.no_extensions) = 1;
				
				break;
				}
				#endif
				#ifdef COMPILE_7926
				case 37: {
				//AddAndReplaceKind
				(executor_globals.opline_ptr) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7927
				case 38: {
				//AddAndReplaceKind
				(executor_globals.prev_exception) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7928
				case 39: {
				//AddAndReplaceKind
				(executor_globals.return_value_ptr_ptr) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7929
				case 40: {
				//AddAndReplaceKind
				(executor_globals.scope) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7930
				case 41: {
				//AddAndReplaceKind
				(executor_globals.start_op) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7931
				case 42: {
				//AddAndReplaceKind
				(executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes + (compiler_globals.active_op_array)->last;
				
				break;
				}
				#endif
				#ifdef COMPILE_7932
				case 43: {
				//AddAndReplaceKind
				(executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes;
				
				break;
				}
				#endif
				#ifdef COMPILE_7933
				case 44: {
				//AddAndReplaceKind
				(executor_globals.symtable_cache_limit) = (executor_globals.symtable_cache) + 32 - 1;
				
				break;
				}
				#endif
				#ifdef COMPILE_7934
				case 45: {
				//AddAndReplaceKind
				(executor_globals.symtable_cache_ptr) = (executor_globals.symtable_cache) - 1;
				
				break;
				}
				#endif
				#ifdef COMPILE_7935
				case 46: {
				//AddAndReplaceKind
				(executor_globals.symtable_cache_ptr)--;
				
				break;
				}
				#endif
				#ifdef COMPILE_7936
				case 47: {
				//AddAndReplaceKind
				(executor_globals.ticks_count) = 0;
				
				break;
				}
				#endif
				#ifdef COMPILE_7937
				case 48: {
				//AddAndReplaceKind
				(executor_globals.uninitialized_zval) = zval_used_for_init;
				
				break;
				}
				#endif
				#ifdef COMPILE_7938
				case 49: {
				//AddAndReplaceKind
				(executor_globals.uninitialized_zval_ptr) = &(executor_globals.uninitialized_zval);
				
				break;
				}
				#endif
				#ifdef COMPILE_7939
				case 50: {
				//AddAndReplaceKind
				(executor_globals.user_error_handler) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7940
				case 51: {
				//AddAndReplaceKind
				(executor_globals.user_exception_handler) = ((void *)0);
				
				break;
				}
				#endif
				#ifdef COMPILE_7941
				case 52: {
				//AddAndReplaceKind
				*(++(executor_globals.symtable_cache_ptr)) = (executor_globals.active_symbol_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7942
				case 53: {
				//AddAndReplaceKind
				*(ex)->CVs[ex->op_array->this_var] = (executor_globals.This);
				
				break;
				}
				#endif
				#ifdef COMPILE_7943
				case 54: {
				//AddAndReplaceKind
				*ex->CVs[(ex)->op_array->this_var] = (executor_globals.This);
				
				break;
				}
				#endif
				#ifdef COMPILE_7944
				case 55: {
				//AddAndReplaceKind
				*ex->CVs[ex->op_array->this_var] = (executor_globals.This);
				
				break;
				}
				#endif
				#ifdef COMPILE_7945
				case 56: {
				//AddAndReplaceKind
				_efree(((executor_globals.active_symbol_table)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7946
				case 57: {
				//AddAndReplaceKind
				_efree(((executor_globals.in_autoload)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7947
				case 58: {
				//AddAndReplaceKind
				_efree((*(executor_globals.symtable_cache_ptr)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7948
				case 59: {
				//AddAndReplaceKind
				_zend_hash_init((&(executor_globals.included_files)), (5), (((void *)0)), (((void *)0)), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7949
				case 60: {
				//AddAndReplaceKind
				_zend_hash_init((&(executor_globals.symbol_table)), (50), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7950
				case 61: {
				//AddAndReplaceKind
				_zend_hash_init(((executor_globals.active_symbol_table)), ((ex)->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7951
				case 62: {
				//AddAndReplaceKind
				_zend_hash_init(((executor_globals.active_symbol_table)), (ex->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7952
				case 63: {
				//AddAndReplaceKind
				_zend_hash_init(((executor_globals.in_autoload)), (0), (((void *)0)), (((void *)0)), (0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7953
				case 64: {
				//AddAndReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (ex)->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7954
				case 65: {
				//AddAndReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, (ex)->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7955
				case 66: {
				//AddAndReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, (ex)->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7956
				case 67: {
				//AddAndReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_7957
				case 68: {
				//AddAndReplaceKind
				_zval_ptr_dtor((&(executor_globals.This)));
				
				break;
				}
				#endif
				#ifdef COMPILE_7958
				case 69: {
				//AddAndReplaceKind
				clean_non_persistent_constants();
				
				break;
				}
				#endif
				#ifdef COMPILE_7959
				case 70: {
				//AddAndReplaceKind
				ex = (ex)->prev_execute_data;
				
				break;
				}
				#endif
				#ifdef COMPILE_7960
				case 71: {
				//AddAndReplaceKind
				ex = (executor_globals.current_execute_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_7961
				case 72: {
				//AddAndReplaceKind
				ex = ex->prev_execute_data;
				
				break;
				}
				#endif
				#ifdef COMPILE_7962
				case 73: {
				//AddAndReplaceKind
				ex->CVs[(ex)->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7963
				case 74: {
				//AddAndReplaceKind
				ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + (ex)->op_array->last_var + ex->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7964
				case 75: {
				//AddAndReplaceKind
				ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + (ex)->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7965
				case 76: {
				//AddAndReplaceKind
				ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				
				break;
				}
				#endif
				#ifdef COMPILE_7966
				case 77: {
				//AddAndReplaceKind
				ex->symbol_table = (executor_globals.active_symbol_table);
				
				break;
				}
				#endif
				#ifdef COMPILE_7967
				case 78: {
				//AddAndReplaceKind
				exit(1);
				
				break;
				}
				#endif
				#ifdef COMPILE_7968
				case 79: {
				//AddAndReplaceKind
				if (!((compiler_globals.active_op_array)->fn_flags & 16) || (compiler_globals.context).backpatch_count > 0 || (compiler_globals.active_op_array)->function_name || (compiler_globals.active_op_array)->type != 2) {
				    return;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7969
				case 80: {
				//AddAndReplaceKind
				if (!(executor_globals.active)) {
				    return -1;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7970
				case 81: {
				//AddAndReplaceKind
				if (!(executor_globals.active_symbol_table)) {
				    zend_rebuild_symbol_table();
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7971
				case 82: {
				//AddAndReplaceKind
				if (!(executor_globals.called_scope)) {
				    zend_error((1 << 0L), "Cannot access static:: when no class scope is active");
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7972
				case 83: {
				//AddAndReplaceKind
				if (!(executor_globals.scope)) {
				    zend_error((1 << 0L), "Cannot access parent:: when no class scope is active");
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7973
				case 84: {
				//AddAndReplaceKind
				if (!(executor_globals.scope)) {
				    zend_error((1 << 0L), "Cannot access self:: when no class scope is active");
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7974
				case 85: {
				//AddAndReplaceKind
				if (!(executor_globals.scope)->parent) {
				    zend_error((1 << 0L), "Cannot access parent:: when current class scope has no parent");
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7975
				case 86: {
				//AddAndReplaceKind
				if (!(executor_globals.start_op)) {
				    (executor_globals.start_op) = (compiler_globals.active_op_array)->opcodes;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7976
				case 87: {
				//AddAndReplaceKind
				if (!zend_is_executing()) {
				    return ((void *)0);
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7977
				case 88: {
				//AddAndReplaceKind
				if ((executor_globals.This)) {
				    _zval_ptr_dtor((&(executor_globals.This)));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7978
				case 89: {
				//AddAndReplaceKind
				if ((executor_globals.active_op_array)) {
				    return (executor_globals.active_op_array)->filename;
				} else {
				    return "[no active file]";
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7979
				case 90: {
				//AddAndReplaceKind
				if ((executor_globals.exception) && (executor_globals.opline_ptr) && (*(executor_globals.opline_ptr))->opcode == 149 && (*(executor_globals.opline_ptr))->lineno == 0 && (executor_globals.opline_before_exception)) {
				    return (executor_globals.opline_before_exception)->lineno;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7980
				case 91: {
				//AddAndReplaceKind
				if ((executor_globals.exception)) {
				    return -1;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7981
				case 92: {
				//AddAndReplaceKind
				if ((executor_globals.exception)) {
				    zend_exception_error((executor_globals.exception), (1 << 0L));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7982
				case 93: {
				//AddAndReplaceKind
				if ((executor_globals.exception)) {
				    zend_throw_exception_internal(((void *)0));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7983
				case 94: {
				//AddAndReplaceKind
				if ((executor_globals.full_tables_cleanup)) {
				    zend_hash_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function_full);
				    zend_hash_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class_full);
				} else {
				    zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function);
				    zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class);
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7984
				case 95: {
				//AddAndReplaceKind
				if ((executor_globals.full_tables_cleanup)) {
				    zend_hash_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data_full);
				    zend_hash_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_class_data);
				} else {
				    zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data);
				    zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_user_class_data);
				    zend_cleanup_internal_classes();
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7985
				case 96: {
				//AddAndReplaceKind
				if ((executor_globals.in_autoload) == ((void *)0)) {
				    ((executor_globals.in_autoload)) = (HashTable *)_emalloc((sizeof(HashTable)));
				    _zend_hash_init(((executor_globals.in_autoload)), (0), (((void *)0)), (((void *)0)), (0));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7986
				case 97: {
				//AddAndReplaceKind
				if ((executor_globals.in_autoload)) {
				    zend_hash_destroy((executor_globals.in_autoload));
				    _efree(((executor_globals.in_autoload)));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7987
				case 98: {
				//AddAndReplaceKind
				if ((executor_globals.opline_ptr)) {
				    return (*(executor_globals.opline_ptr))->lineno;
				} else {
				    return 0;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7988
				case 99: {
				//AddAndReplaceKind
				if ((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache)) {
				    (executor_globals.active_symbol_table) = *((executor_globals.symtable_cache_ptr)--);
				} else {
				    ((executor_globals.active_symbol_table)) = (HashTable *)_emalloc((sizeof(HashTable)));
				    _zend_hash_init(((executor_globals.active_symbol_table)), (ex->op_array->last_var), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7989
				case 100: {
				//AddAndReplaceKind
				if ((executor_globals.symtable_cache_ptr) >= (executor_globals.symtable_cache_limit)) {
				    zend_hash_destroy((executor_globals.active_symbol_table));
				    _efree(((executor_globals.active_symbol_table)));
				} else {
				    zend_hash_clean((executor_globals.active_symbol_table));
				    *(++(executor_globals.symtable_cache_ptr)) = (executor_globals.active_symbol_table);
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7990
				case 101: {
				//AddAndReplaceKind
				if (ex && ex->symbol_table) {
				    (executor_globals.active_symbol_table) = ex->symbol_table;
				    return;
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7991
				case 102: {
				//AddAndReplaceKind
				if (ex->CVs[i]) {
				    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7992
				case 103: {
				//AddAndReplaceKind
				if (ex->op_array->this_var != -1 && !ex->CVs[ex->op_array->this_var] && (executor_globals.This)) {
				    ex->CVs[ex->op_array->this_var] = (zval **)ex->CVs + ex->op_array->last_var + ex->op_array->this_var;
				    *ex->CVs[ex->op_array->this_var] = (executor_globals.This);
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7993
				case 104: {
				//AddAndReplaceKind
				if (zend_on_timeout) {
				    zend_on_timeout((executor_globals.timeout_seconds));
				}
				
				break;
				}
				#endif
				#ifdef COMPILE_7994
				case 105: {
				//AddAndReplaceKind
				signal(27, zend_timeout);
				
				break;
				}
				#endif
				#ifdef COMPILE_7995
				case 106: {
				//AddAndReplaceKind
				zend_cleanup_internal_classes();
				
				break;
				}
				#endif
				#ifdef COMPILE_7996
				case 107: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Cannot access parent:: when current class scope has no parent");
				
				break;
				}
				#endif
				#ifdef COMPILE_7997
				case 108: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Cannot access parent:: when no class scope is active");
				
				break;
				}
				#endif
				#ifdef COMPILE_7998
				case 109: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Cannot access self:: when no class scope is active");
				
				break;
				}
				#endif
				#ifdef COMPILE_7999
				case 110: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Cannot access static:: when no class scope is active");
				
				break;
				}
				#endif
				#ifdef COMPILE_8000
				case 111: {
				//AddAndReplaceKind
				zend_error((1 << 0L), "Maximum execution time of %d second%s exceeded", (executor_globals.timeout_seconds), (executor_globals.timeout_seconds) == 1 ? "" : "s");
				
				break;
				}
				#endif
				#ifdef COMPILE_8001
				case 112: {
				//AddAndReplaceKind
				zend_error((1 << 3L), "A non well formed numeric value encountered");
				
				break;
				}
				#endif
				#ifdef COMPILE_8002
				case 113: {
				//AddAndReplaceKind
				zend_error_noreturn((1 << 0L), "Cannot call overloaded function for non-object");
				
				break;
				}
				#endif
				#ifdef COMPILE_8003
				case 114: {
				//AddAndReplaceKind
				zend_exception_error((executor_globals.exception), (1 << 0L));
				
				break;
				}
				#endif
				#ifdef COMPILE_8004
				case 115: {
				//AddAndReplaceKind
				zend_exception_restore();
				
				break;
				}
				#endif
				#ifdef COMPILE_8005
				case 116: {
				//AddAndReplaceKind
				zend_exception_save();
				
				break;
				}
				#endif
				#ifdef COMPILE_8006
				case 117: {
				//AddAndReplaceKind
				zend_execute((compiler_globals.active_op_array));
				
				break;
				}
				#endif
				#ifdef COMPILE_8007
				case 118: {
				//AddAndReplaceKind
				zend_execute((executor_globals.active_op_array));
				
				break;
				}
				#endif
				#ifdef COMPILE_8008
				case 119: {
				//AddAndReplaceKind
				zend_hash_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class_full);
				
				break;
				}
				#endif
				#ifdef COMPILE_8009
				case 120: {
				//AddAndReplaceKind
				zend_hash_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_class_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_8010
				case 121: {
				//AddAndReplaceKind
				zend_hash_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function_full);
				
				break;
				}
				#endif
				#ifdef COMPILE_8011
				case 122: {
				//AddAndReplaceKind
				zend_hash_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data_full);
				
				break;
				}
				#endif
				#ifdef COMPILE_8012
				case 123: {
				//AddAndReplaceKind
				zend_hash_clean((executor_globals.active_symbol_table));
				
				break;
				}
				#endif
				#ifdef COMPILE_8013
				case 124: {
				//AddAndReplaceKind
				zend_hash_destroy(&(executor_globals.included_files));
				
				break;
				}
				#endif
				#ifdef COMPILE_8014
				case 125: {
				//AddAndReplaceKind
				zend_hash_destroy((executor_globals.active_symbol_table));
				
				break;
				}
				#endif
				#ifdef COMPILE_8015
				case 126: {
				//AddAndReplaceKind
				zend_hash_destroy((executor_globals.in_autoload));
				
				break;
				}
				#endif
				#ifdef COMPILE_8016
				case 127: {
				//AddAndReplaceKind
				zend_hash_destroy(*(executor_globals.symtable_cache_ptr));
				
				break;
				}
				#endif
				#ifdef COMPILE_8017
				case 128: {
				//AddAndReplaceKind
				zend_hash_graceful_reverse_destroy(&(executor_globals.symbol_table));
				
				break;
				}
				#endif
				#ifdef COMPILE_8018
				case 129: {
				//AddAndReplaceKind
				zend_hash_reverse_apply(&(executor_globals.symbol_table), (apply_func_t)zval_call_destructor);
				
				break;
				}
				#endif
				#ifdef COMPILE_8019
				case 130: {
				//AddAndReplaceKind
				zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)clean_non_persistent_class);
				
				break;
				}
				#endif
				#ifdef COMPILE_8020
				case 131: {
				//AddAndReplaceKind
				zend_hash_reverse_apply((executor_globals.class_table), (apply_func_t)zend_cleanup_user_class_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_8021
				case 132: {
				//AddAndReplaceKind
				zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)clean_non_persistent_function);
				
				break;
				}
				#endif
				#ifdef COMPILE_8022
				case 133: {
				//AddAndReplaceKind
				zend_hash_reverse_apply((executor_globals.function_table), (apply_func_t)zend_cleanup_function_data);
				
				break;
				}
				#endif
				#ifdef COMPILE_8023
				case 134: {
				//AddAndReplaceKind
				zend_init_fpu();
				
				break;
				}
				#endif
				#ifdef COMPILE_8024
				case 135: {
				//AddAndReplaceKind
				zend_llist_apply(&zend_extensions, (llist_apply_func_t)zend_extension_activator);
				
				break;
				}
				#endif
				#ifdef COMPILE_8025
				case 136: {
				//AddAndReplaceKind
				zend_llist_apply(&zend_extensions, (llist_apply_func_t)zend_extension_deactivator);
				
				break;
				}
				#endif
				#ifdef COMPILE_8026
				case 137: {
				//AddAndReplaceKind
				zend_objects_store_call_destructors(&(executor_globals.objects_store));
				
				break;
				}
				#endif
				#ifdef COMPILE_8027
				case 138: {
				//AddAndReplaceKind
				zend_objects_store_destroy(&(executor_globals.objects_store));
				
				break;
				}
				#endif
				#ifdef COMPILE_8028
				case 139: {
				//AddAndReplaceKind
				zend_objects_store_free_object_storage(&(executor_globals.objects_store));
				
				break;
				}
				#endif
				#ifdef COMPILE_8029
				case 140: {
				//AddAndReplaceKind
				zend_objects_store_init(&(executor_globals.objects_store), 1024);
				
				break;
				}
				#endif
				#ifdef COMPILE_8030
				case 141: {
				//AddAndReplaceKind
				zend_objects_store_mark_destructed(&(executor_globals.objects_store));
				
				break;
				}
				#endif
				#ifdef COMPILE_8031
				case 142: {
				//AddAndReplaceKind
				zend_on_timeout((executor_globals.timeout_seconds));
				
				break;
				}
				#endif
				#ifdef COMPILE_8032
				case 143: {
				//AddAndReplaceKind
				zend_ptr_stack_clean(&(executor_globals.user_error_handlers), (void (*)(void *))_zval_dtor_wrapper, 1);
				
				break;
				}
				#endif
				#ifdef COMPILE_8033
				case 144: {
				//AddAndReplaceKind
				zend_ptr_stack_clean(&(executor_globals.user_exception_handlers), (void (*)(void *))_zval_dtor_wrapper, 1);
				
				break;
				}
				#endif
				#ifdef COMPILE_8034
				case 145: {
				//AddAndReplaceKind
				zend_ptr_stack_destroy(&(executor_globals.arg_types_stack));
				
				break;
				}
				#endif
				#ifdef COMPILE_8035
				case 146: {
				//AddAndReplaceKind
				zend_ptr_stack_destroy(&(executor_globals.user_error_handlers));
				
				break;
				}
				#endif
				#ifdef COMPILE_8036
				case 147: {
				//AddAndReplaceKind
				zend_ptr_stack_destroy(&(executor_globals.user_exception_handlers));
				
				break;
				}
				#endif
				#ifdef COMPILE_8037
				case 148: {
				//AddAndReplaceKind
				zend_ptr_stack_init(&(executor_globals.arg_types_stack));
				
				break;
				}
				#endif
				#ifdef COMPILE_8038
				case 149: {
				//AddAndReplaceKind
				zend_ptr_stack_init(&(executor_globals.user_error_handlers));
				
				break;
				}
				#endif
				#ifdef COMPILE_8039
				case 150: {
				//AddAndReplaceKind
				zend_ptr_stack_init(&(executor_globals.user_exception_handlers));
				
				break;
				}
				#endif
				#ifdef COMPILE_8040
				case 151: {
				//AddAndReplaceKind
				zend_rebuild_symbol_table();
				
				break;
				}
				#endif
				#ifdef COMPILE_8041
				case 152: {
				//AddAndReplaceKind
				zend_release_labels();
				
				break;
				}
				#endif
				#ifdef COMPILE_8042
				case 153: {
				//AddAndReplaceKind
				zend_shutdown_fpu();
				
				break;
				}
				#endif
				#ifdef COMPILE_8043
				case 154: {
				//AddAndReplaceKind
				zend_stack_destroy(&(executor_globals.user_error_handlers_error_reporting));
				
				break;
				}
				#endif
				#ifdef COMPILE_8044
				case 155: {
				//AddAndReplaceKind
				zend_stack_init(&(executor_globals.user_error_handlers_error_reporting));
				
				break;
				}
				#endif
				#ifdef COMPILE_8045
				case 156: {
				//AddAndReplaceKind
				zend_throw_exception_internal(((void *)0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8046
				case 157: {
				//AddAndReplaceKind
				zend_vm_stack_clear_multiple();
				
				break;
				}
				#endif
				#ifdef COMPILE_8047
				case 158: {
				//AddAndReplaceKind
				zend_vm_stack_destroy();
				
				break;
				}
				#endif
				#ifdef COMPILE_8048
				case 159: {
				//AddAndReplaceKind
				zend_vm_stack_init();
				
				break;
				}
				#endif
				#ifdef COMPILE_8049
				case 160: {
				//AddAndReplaceKind
				zend_vm_stack_push((void *)((void *)0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8050
				case 161: {
				//AddAndReplaceKind
				zval_addref_p(&((executor_globals.uninitialized_zval)));
				
				break;
				}
				#endif
				#ifdef COMPILE_8051
				case 162: {
				//AddAndReplaceKind
				zval_addref_p((executor_globals.This));
				
				break;
				}
				#endif
				#ifdef COMPILE_8052
				case 163: {
				//AddInitKind
				memset(&ex->op_array->vars[i], 0, sizeof (*(&ex->op_array->vars[i])));
				
				break;
				}
				#endif
				#ifdef COMPILE_8053
				case 164: {
				//AddInitKind
				memset(&executor_globals, 0, sizeof (*(&executor_globals)));
				
				break;
				}
				#endif
				#ifdef COMPILE_8054
				case 165: {
				//AddInitKind
				memset(ex, 0, sizeof (*(ex)));
				
				break;
				}
				#endif
				#ifdef COMPILE_8055
				case 166: {
				//AddInitKind
				memset(ex->op_array, 0, sizeof (*(ex->op_array)));
				
				break;
				}
				#endif
				#ifdef COMPILE_8056
				case 167: {
				//IfExitKind
				if (__is_neg("69-167", 141, &(i), sizeof (i), &(ex), sizeof (ex), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->vars), sizeof (ex->op_array->vars), &(ex->op_array->vars[i].name), sizeof (ex->op_array->vars[i].name), &(ex->op_array->vars[i].name_len), sizeof (ex->op_array->vars[i].name_len), &(ex->op_array->vars[i].hash_value), sizeof (ex->op_array->vars[i].hash_value), &(ex->op_array), sizeof (ex->op_array), &(ex->CVs), sizeof (ex->CVs), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->last_var), sizeof (ex->op_array->last_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->op_array->this_var), sizeof (ex->op_array->this_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->symbol_table), sizeof (ex->symbol_table), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(ex->prev_execute_data), sizeof (ex->prev_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(ex->op_array->type), sizeof (ex->op_array->type), &(ex->op_array->function_name), sizeof (ex->op_array->function_name), &(ex->op_array->scope), sizeof (ex->op_array->scope), &(ex->op_array->fn_flags), sizeof (ex->op_array->fn_flags), &(ex->op_array->prototype), sizeof (ex->op_array->prototype), &(ex->op_array->num_args), sizeof (ex->op_array->num_args), &(ex->op_array->required_num_args), sizeof (ex->op_array->required_num_args), &(ex->op_array->arg_info), sizeof (ex->op_array->arg_info), &(ex->op_array->refcount), sizeof (ex->op_array->refcount), &(ex->op_array->opcodes), sizeof (ex->op_array->opcodes), &(ex->op_array->last), sizeof (ex->op_array->last), &(ex->op_array->T), sizeof (ex->op_array->T), &(ex->op_array->brk_cont_array), sizeof (ex->op_array->brk_cont_array), &(ex->op_array->last_brk_cont), sizeof (ex->op_array->last_brk_cont), &(ex->op_array->try_catch_array), sizeof (ex->op_array->try_catch_array), &(ex->op_array->last_try_catch), sizeof (ex->op_array->last_try_catch), &(ex->op_array->static_variables), sizeof (ex->op_array->static_variables), &(ex->op_array->filename), sizeof (ex->op_array->filename), &(ex->op_array->line_start), sizeof (ex->op_array->line_start), &(ex->op_array->line_end), sizeof (ex->op_array->line_end), &(ex->op_array->doc_comment), sizeof (ex->op_array->doc_comment), &(ex->op_array->doc_comment_len), sizeof (ex->op_array->doc_comment_len), &(ex->op_array->early_binding), sizeof (ex->op_array->early_binding), &(ex->op_array->literals), sizeof (ex->op_array->literals), &(ex->op_array->last_literal), sizeof (ex->op_array->last_literal), &(ex->op_array->run_time_cache), sizeof (ex->op_array->run_time_cache), &(ex->op_array->last_cache_slot), sizeof (ex->op_array->last_cache_slot), &(ex->opline), sizeof (ex->opline), &(ex->fbc), sizeof (ex->fbc), &(ex->called_scope), sizeof (ex->called_scope), &(ex->object), sizeof (ex->object), &(ex->Ts), sizeof (ex->Ts), &(ex->old_error_reporting), sizeof (ex->old_error_reporting), &(ex->nested), sizeof (ex->nested), &(ex->original_return_value), sizeof (ex->original_return_value), &(ex->current_scope), sizeof (ex->current_scope), &(ex->current_called_scope), sizeof (ex->current_called_scope), &(ex->current_this), sizeof (ex->current_this), &(ex->current_object), sizeof (ex->current_object), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
				    break;
				
				break;
				}
				#endif
				#ifdef COMPILE_8057
				case 168: {
				//IfExitKind
				if (__is_neg("69-168", 141, &(i), sizeof (i), &(ex), sizeof (ex), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->vars), sizeof (ex->op_array->vars), &(ex->op_array->vars[i].name), sizeof (ex->op_array->vars[i].name), &(ex->op_array->vars[i].name_len), sizeof (ex->op_array->vars[i].name_len), &(ex->op_array->vars[i].hash_value), sizeof (ex->op_array->vars[i].hash_value), &(ex->op_array), sizeof (ex->op_array), &(ex->CVs), sizeof (ex->CVs), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->last_var), sizeof (ex->op_array->last_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->op_array->this_var), sizeof (ex->op_array->this_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->symbol_table), sizeof (ex->symbol_table), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(ex->prev_execute_data), sizeof (ex->prev_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(ex->op_array->type), sizeof (ex->op_array->type), &(ex->op_array->function_name), sizeof (ex->op_array->function_name), &(ex->op_array->scope), sizeof (ex->op_array->scope), &(ex->op_array->fn_flags), sizeof (ex->op_array->fn_flags), &(ex->op_array->prototype), sizeof (ex->op_array->prototype), &(ex->op_array->num_args), sizeof (ex->op_array->num_args), &(ex->op_array->required_num_args), sizeof (ex->op_array->required_num_args), &(ex->op_array->arg_info), sizeof (ex->op_array->arg_info), &(ex->op_array->refcount), sizeof (ex->op_array->refcount), &(ex->op_array->opcodes), sizeof (ex->op_array->opcodes), &(ex->op_array->last), sizeof (ex->op_array->last), &(ex->op_array->T), sizeof (ex->op_array->T), &(ex->op_array->brk_cont_array), sizeof (ex->op_array->brk_cont_array), &(ex->op_array->last_brk_cont), sizeof (ex->op_array->last_brk_cont), &(ex->op_array->try_catch_array), sizeof (ex->op_array->try_catch_array), &(ex->op_array->last_try_catch), sizeof (ex->op_array->last_try_catch), &(ex->op_array->static_variables), sizeof (ex->op_array->static_variables), &(ex->op_array->filename), sizeof (ex->op_array->filename), &(ex->op_array->line_start), sizeof (ex->op_array->line_start), &(ex->op_array->line_end), sizeof (ex->op_array->line_end), &(ex->op_array->doc_comment), sizeof (ex->op_array->doc_comment), &(ex->op_array->doc_comment_len), sizeof (ex->op_array->doc_comment_len), &(ex->op_array->early_binding), sizeof (ex->op_array->early_binding), &(ex->op_array->literals), sizeof (ex->op_array->literals), &(ex->op_array->last_literal), sizeof (ex->op_array->last_literal), &(ex->op_array->run_time_cache), sizeof (ex->op_array->run_time_cache), &(ex->op_array->last_cache_slot), sizeof (ex->op_array->last_cache_slot), &(ex->opline), sizeof (ex->opline), &(ex->fbc), sizeof (ex->fbc), &(ex->called_scope), sizeof (ex->called_scope), &(ex->object), sizeof (ex->object), &(ex->Ts), sizeof (ex->Ts), &(ex->old_error_reporting), sizeof (ex->old_error_reporting), &(ex->nested), sizeof (ex->nested), &(ex->original_return_value), sizeof (ex->original_return_value), &(ex->current_scope), sizeof (ex->current_scope), &(ex->current_called_scope), sizeof (ex->current_called_scope), &(ex->current_this), sizeof (ex->current_this), &(ex->current_object), sizeof (ex->current_object), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
				    return;
				
				break;
				}
				#endif
				}
				switch(__choose("__SWITCH70"))
				{
				case 0: {
				zend_hash_quick_update(EG(active_symbol_table),
										ex->op_array->vars[i].name,
										ex->op_array->vars[i].name_len + 1,
										ex->op_array->vars[i].hash_value,
										(void**)ex->CVs[i],
										sizeof(zval*),
										(void**)&ex->CVs[i]);
				
				break;
				}
				#ifdef COMPILE_8058
				case 1: {
				//AddAndReplaceKind
				fprintf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8059
				case 2: {
				//AddAndReplaceKind
				fscanf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8060
				case 3: {
				//AddAndReplaceKind
				printf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8061
				case 4: {
				//AddAndReplaceKind
				scanf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8062
				case 5: {
				//AddAndReplaceKind
				sprintf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8063
				case 6: {
				//AddAndReplaceKind
				sscanf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8064
				case 7: {
				//GuardKind
				if (!__is_neg("70-7", 141, &(i), sizeof (i), &(ex), sizeof (ex), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->vars), sizeof (ex->op_array->vars), &(ex->op_array->vars[i].name), sizeof (ex->op_array->vars[i].name), &(ex->op_array->vars[i].name_len), sizeof (ex->op_array->vars[i].name_len), &(ex->op_array->vars[i].hash_value), sizeof (ex->op_array->vars[i].hash_value), &(ex->op_array), sizeof (ex->op_array), &(ex->CVs), sizeof (ex->CVs), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->last_var), sizeof (ex->op_array->last_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->op_array->this_var), sizeof (ex->op_array->this_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->symbol_table), sizeof (ex->symbol_table), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(ex->prev_execute_data), sizeof (ex->prev_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(ex->op_array->type), sizeof (ex->op_array->type), &(ex->op_array->function_name), sizeof (ex->op_array->function_name), &(ex->op_array->scope), sizeof (ex->op_array->scope), &(ex->op_array->fn_flags), sizeof (ex->op_array->fn_flags), &(ex->op_array->prototype), sizeof (ex->op_array->prototype), &(ex->op_array->num_args), sizeof (ex->op_array->num_args), &(ex->op_array->required_num_args), sizeof (ex->op_array->required_num_args), &(ex->op_array->arg_info), sizeof (ex->op_array->arg_info), &(ex->op_array->refcount), sizeof (ex->op_array->refcount), &(ex->op_array->opcodes), sizeof (ex->op_array->opcodes), &(ex->op_array->last), sizeof (ex->op_array->last), &(ex->op_array->T), sizeof (ex->op_array->T), &(ex->op_array->brk_cont_array), sizeof (ex->op_array->brk_cont_array), &(ex->op_array->last_brk_cont), sizeof (ex->op_array->last_brk_cont), &(ex->op_array->try_catch_array), sizeof (ex->op_array->try_catch_array), &(ex->op_array->last_try_catch), sizeof (ex->op_array->last_try_catch), &(ex->op_array->static_variables), sizeof (ex->op_array->static_variables), &(ex->op_array->filename), sizeof (ex->op_array->filename), &(ex->op_array->line_start), sizeof (ex->op_array->line_start), &(ex->op_array->line_end), sizeof (ex->op_array->line_end), &(ex->op_array->doc_comment), sizeof (ex->op_array->doc_comment), &(ex->op_array->doc_comment_len), sizeof (ex->op_array->doc_comment_len), &(ex->op_array->early_binding), sizeof (ex->op_array->early_binding), &(ex->op_array->literals), sizeof (ex->op_array->literals), &(ex->op_array->last_literal), sizeof (ex->op_array->last_literal), &(ex->op_array->run_time_cache), sizeof (ex->op_array->run_time_cache), &(ex->op_array->last_cache_slot), sizeof (ex->op_array->last_cache_slot), &(ex->opline), sizeof (ex->opline), &(ex->fbc), sizeof (ex->fbc), &(ex->called_scope), sizeof (ex->called_scope), &(ex->object), sizeof (ex->object), &(ex->Ts), sizeof (ex->Ts), &(ex->old_error_reporting), sizeof (ex->old_error_reporting), &(ex->nested), sizeof (ex->nested), &(ex->original_return_value), sizeof (ex->original_return_value), &(ex->current_scope), sizeof (ex->current_scope), &(ex->current_called_scope), sizeof (ex->current_called_scope), &(ex->current_this), sizeof (ex->current_this), &(ex->current_object), sizeof (ex->current_object), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
				    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8065
				case 8: {
				//ReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (ex)->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8066
				case 9: {
				//ReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, (ex)->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8067
				case 10: {
				//ReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, (ex)->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8068
				case 11: {
				//ReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				}
				}
								}
				
				break;
				}
				#ifdef COMPILE_8069
				case 1: {
				//AddAndReplaceKind
				fprintf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8070
				case 2: {
				//AddAndReplaceKind
				fscanf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8071
				case 3: {
				//AddAndReplaceKind
				printf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8072
				case 4: {
				//AddAndReplaceKind
				scanf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8073
				case 5: {
				//AddAndReplaceKind
				sprintf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8074
				case 6: {
				//AddAndReplaceKind
				sscanf((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8075
				case 7: {
				//GuardKind
				if (!__is_neg("71-7", 141, &(i), sizeof (i), &(ex), sizeof (ex), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->vars), sizeof (ex->op_array->vars), &(ex->op_array->vars[i].name), sizeof (ex->op_array->vars[i].name), &(ex->op_array->vars[i].name_len), sizeof (ex->op_array->vars[i].name_len), &(ex->op_array->vars[i].hash_value), sizeof (ex->op_array->vars[i].hash_value), &(ex->op_array), sizeof (ex->op_array), &(ex->CVs), sizeof (ex->CVs), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(ex->op_array->last_var), sizeof (ex->op_array->last_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->op_array->this_var), sizeof (ex->op_array->this_var), &(executor_globals.This), sizeof (executor_globals.This), &(ex->symbol_table), sizeof (ex->symbol_table), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(ex->prev_execute_data), sizeof (ex->prev_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(ex->op_array->type), sizeof (ex->op_array->type), &(ex->op_array->function_name), sizeof (ex->op_array->function_name), &(ex->op_array->scope), sizeof (ex->op_array->scope), &(ex->op_array->fn_flags), sizeof (ex->op_array->fn_flags), &(ex->op_array->prototype), sizeof (ex->op_array->prototype), &(ex->op_array->num_args), sizeof (ex->op_array->num_args), &(ex->op_array->required_num_args), sizeof (ex->op_array->required_num_args), &(ex->op_array->arg_info), sizeof (ex->op_array->arg_info), &(ex->op_array->refcount), sizeof (ex->op_array->refcount), &(ex->op_array->opcodes), sizeof (ex->op_array->opcodes), &(ex->op_array->last), sizeof (ex->op_array->last), &(ex->op_array->T), sizeof (ex->op_array->T), &(ex->op_array->brk_cont_array), sizeof (ex->op_array->brk_cont_array), &(ex->op_array->last_brk_cont), sizeof (ex->op_array->last_brk_cont), &(ex->op_array->try_catch_array), sizeof (ex->op_array->try_catch_array), &(ex->op_array->last_try_catch), sizeof (ex->op_array->last_try_catch), &(ex->op_array->static_variables), sizeof (ex->op_array->static_variables), &(ex->op_array->filename), sizeof (ex->op_array->filename), &(ex->op_array->line_start), sizeof (ex->op_array->line_start), &(ex->op_array->line_end), sizeof (ex->op_array->line_end), &(ex->op_array->doc_comment), sizeof (ex->op_array->doc_comment), &(ex->op_array->doc_comment_len), sizeof (ex->op_array->doc_comment_len), &(ex->op_array->early_binding), sizeof (ex->op_array->early_binding), &(ex->op_array->literals), sizeof (ex->op_array->literals), &(ex->op_array->last_literal), sizeof (ex->op_array->last_literal), &(ex->op_array->run_time_cache), sizeof (ex->op_array->run_time_cache), &(ex->op_array->last_cache_slot), sizeof (ex->op_array->last_cache_slot), &(ex->opline), sizeof (ex->opline), &(ex->fbc), sizeof (ex->fbc), &(ex->called_scope), sizeof (ex->called_scope), &(ex->object), sizeof (ex->object), &(ex->Ts), sizeof (ex->Ts), &(ex->old_error_reporting), sizeof (ex->old_error_reporting), &(ex->nested), sizeof (ex->nested), &(ex->original_return_value), sizeof (ex->original_return_value), &(ex->current_scope), sizeof (ex->current_scope), &(ex->current_called_scope), sizeof (ex->current_called_scope), &(ex->current_this), sizeof (ex->current_this), &(ex->current_object), sizeof (ex->current_object), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
				    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8076
				case 8: {
				//ReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (ex)->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8077
				case 9: {
				//ReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, (ex)->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8078
				case 10: {
				//ReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, (ex)->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				#ifdef COMPILE_8079
				case 11: {
				//ReplaceKind
				_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), ex->op_array->vars[i].name, ex->op_array->vars[i].name_len + 1, ex->op_array->vars[i].hash_value, (void **)ex->CVs[i], sizeof(zval *), (void **)&ex->CVs[i], (1 << 0));
				
				break;
				}
				#endif
				}
				}
				}
			}
		}
	}
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
