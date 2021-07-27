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
#include "zend.h"
#include "zend_API.h"
#include "zend_globals.h"
#include "zend_constants.h"
#include "zend_list.h"


ZEND_API void _zval_dtor_func(zval *zvalue ZEND_FILE_LINE_DC)
{
	switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
		case IS_STRING:
		case IS_CONSTANT:
		case IS_CLASS:
			CHECK_ZVAL_STRING_REL(zvalue);
			STR_FREE_REL(zvalue->value.str.val);
			break;
		case IS_ARRAY:
		case IS_CONSTANT_ARRAY: {
				TSRMLS_FETCH();

				if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
					zend_hash_destroy(zvalue->value.ht);
					FREE_HASHTABLE(zvalue->value.ht);
				}
			}
			break;
		case IS_OBJECT:
			{
				TSRMLS_FETCH();

				Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);
			}
			break;
		case IS_RESOURCE:
			{
				TSRMLS_FETCH();

				/* destroy resource */
				zend_list_delete(zvalue->value.lval);
			}
			break;
		case IS_LONG:
		case IS_DOUBLE:
		case IS_BOOL:
		case IS_NULL:
		default:
			return;
			break;
	}
}


ZEND_API void _zval_internal_dtor(zval *zvalue ZEND_FILE_LINE_DC)
{
	switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
		case IS_STRING:
		case IS_CONSTANT:
			CHECK_ZVAL_STRING_REL(zvalue);
			str_free(zvalue->value.str.val);
			break;
		case IS_ARRAY:
		case IS_CONSTANT_ARRAY:
		case IS_OBJECT:
		case IS_RESOURCE:
			zend_error(E_CORE_ERROR, "Internal zval's can't be arrays, objects or resources");
			break;
		case IS_LONG:
		case IS_DOUBLE:
		case IS_BOOL:
		case IS_NULL:
		default:
			break;
	}
}


ZEND_API void zval_add_ref(zval **p)
{
	Z_ADDREF_PP(p);
}


ZEND_API void _zval_copy_ctor_func(zval *zvalue ZEND_FILE_LINE_DC)
{
	switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
		case IS_RESOURCE: {
				TSRMLS_FETCH();

				zend_list_addref(zvalue->value.lval);
			}
			break;
		case IS_BOOL:
		case IS_LONG:
		case IS_NULL:
			break;
		case IS_CONSTANT:
		case IS_STRING:
			CHECK_ZVAL_STRING_REL(zvalue);
			if (!IS_INTERNED(zvalue->value.str.val)) {
				zvalue->value.str.val = (char *) estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
			}
			break;
		case IS_ARRAY:
		case IS_CONSTANT_ARRAY: {
				zval *tmp;
				HashTable *original_ht = zvalue->value.ht;
				HashTable *tmp_ht = NULL;
				TSRMLS_FETCH();

				if (zvalue->value.ht == &EG(symbol_table)) {
					return; /* do nothing */
				}
				ALLOC_HASHTABLE_REL(tmp_ht);
				zend_hash_init(tmp_ht, zend_hash_num_elements(original_ht), NULL, ZVAL_PTR_DTOR, 0);
				zend_hash_copy(tmp_ht, original_ht, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
				zvalue->value.ht = tmp_ht;
			}
			break;
		case IS_OBJECT:
			{
				TSRMLS_FETCH();
				Z_OBJ_HT_P(zvalue)->add_ref(zvalue TSRMLS_CC);
			}
			break;
	}
}


ZEND_API int zend_print_variable(zval *var) 
{
	return zend_print_zval(var, 0);
}


ZEND_API void _zval_dtor_wrapper(zval *zvalue)
{
	TSRMLS_FETCH();

	GC_REMOVE_ZVAL_FROM_BUFFER(zvalue);
	zval_dtor(zvalue);
}


#if ZEND_DEBUG
ZEND_API void _zval_copy_ctor_wrapper(zval *zvalue)
{
	zval_copy_ctor(zvalue);
}


ZEND_API void _zval_internal_dtor_wrapper(zval *zvalue)
{
	zval_internal_dtor(zvalue);
}


ZEND_API void _zval_ptr_dtor_wrapper(zval **zval_ptr)
{
	zval_ptr_dtor(zval_ptr);
}


ZEND_API void _zval_internal_ptr_dtor_wrapper(zval **zval_ptr)
{
	zval_internal_ptr_dtor(zval_ptr);
}
#endif

ZEND_API int zval_copy_static_var(zval **p TSRMLS_DC, int num_args, va_list args, zend_hash_key *key) /* {{{ */
{
	HashTable *target = va_arg(args, HashTable*);
	zend_bool is_ref;
	zval *tmp;
  
	//prophet generated patch
	{
	switch(__choose("__SWITCH86"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9114
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9115
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9116
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9117
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9118
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9119
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9120
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9121
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9122
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9123
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9124
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9125
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9126
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9127
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9128
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9129
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9130
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9131
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9132
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9133
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9134
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9135
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9136
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9137
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9138
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9139
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9140
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9141
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9142
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9143
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9144
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9145
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9146
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9147
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9148
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9149
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9150
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9151
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9152
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9153
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9154
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9155
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9156
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9157
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9158
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9159
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9160
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9161
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9162
	case 49: {
	//AddInitKind
	memset(&((zval_gc_info *)(tmp))->u, 0, sizeof (*(&((zval_gc_info *)(tmp))->u)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9163
	case 50: {
	//AddInitKind
	memset(&(**(p)), 0, sizeof (*(&(**(p)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9164
	case 51: {
	//AddInitKind
	memset(&(**p), 0, sizeof (*(&(**p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9165
	case 52: {
	//AddInitKind
	memset(&(*tmp), 0, sizeof (*(&(*tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9166
	case 53: {
	//AddInitKind
	memset(&executor_globals, 0, sizeof (*(&executor_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9167
	case 54: {
	//AddInitKind
	memset(((zval_gc_info *)(tmp)), 0, sizeof (*(((zval_gc_info *)(tmp)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9168
	case 55: {
	//AddInitKind
	memset((*(p)), 0, sizeof (*((*(p)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9169
	case 56: {
	//AddInitKind
	memset((*p), 0, sizeof (*((*p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9170
	case 57: {
	//AddInitKind
	memset((tmp), 0, sizeof (*((tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9171
	case 58: {
	//AddInitKind
	memset(key, 0, sizeof (*(key)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9172
	case 59: {
	//IfExitKind
	if (__is_neg("86-59", 9, &(p), sizeof (p), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9173
	case 60: {
	//IfExitKind
	if (__is_neg("86-60", 9, &(p), sizeof (p), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9174
	case 61: {
	//IfExitKind
	if (__is_neg("86-61", 9, &(p), sizeof (p), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9175
	case 62: {
	//IfExitKind
	if (__is_neg("86-62", 9, &(p), sizeof (p), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9176
	case 63: {
	//IfExitKind
	if (__is_neg("86-63", 9, &(p), sizeof (p), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 64;
	
	break;
	}
	#endif
	}
	{
	long long __temp87=((**p).type & (32 | 64)) ;
	switch(__choose("__SWITCH87"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_9177
	case 1: {
	__temp87= (((**p).type & (32 | 64)) || __is_neg("87-1", 9, &(p), sizeof (p), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9178
	case 2: {
	__temp87= (((**p).type & (32 | 64)) && !__is_neg("87-2", 9, &(p), sizeof (p), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH112"))
	{
	case 0: {
	if (__temp87) {
			{
	switch(__choose("__SWITCH88"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9179
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9180
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9181
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9182
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9183
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9184
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9185
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9186
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9187
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9188
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9189
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9190
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9191
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9192
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9193
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9194
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9195
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9196
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9197
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9198
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9199
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9200
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9201
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9202
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9203
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9204
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9205
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9206
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9207
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9208
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9209
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9210
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9211
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9212
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9213
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9214
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9215
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9216
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9217
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9218
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9219
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9220
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9221
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9222
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9223
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9224
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9225
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9226
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9227
	case 49: {
	//AddInitKind
	memset(&(**p), 0, sizeof (*(&(**p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9228
	case 50: {
	//IfExitKind
	if (__is_neg("88-50", 9, &(p), sizeof (p), &(is_ref), sizeof (is_ref), &((**p).type), sizeof ((**p).type), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9229
	case 51: {
	//IfExitKind
	if (__is_neg("88-51", 9, &(p), sizeof (p), &(is_ref), sizeof (is_ref), &((**p).type), sizeof ((**p).type), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9230
	case 52: {
	//IfExitKind
	if (__is_neg("88-52", 9, &(p), sizeof (p), &(is_ref), sizeof (is_ref), &((**p).type), sizeof ((**p).type), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9231
	case 53: {
	//IfExitKind
	if (__is_neg("88-53", 9, &(p), sizeof (p), &(is_ref), sizeof (is_ref), &((**p).type), sizeof ((**p).type), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9232
	case 54: {
	//IfExitKind
	if (__is_neg("88-54", 9, &(p), sizeof (p), &(is_ref), sizeof (is_ref), &((**p).type), sizeof ((**p).type), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 64;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH89"))
	{
	case 0: {
	is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;
	
	break;
	}
	#ifdef COMPILE_9233
	case 1: {
	//GuardKind
	if (!__is_neg("89-1", 9, &(p), sizeof (p), &(is_ref), sizeof (is_ref), &((**p).type), sizeof ((**p).type), &(args), sizeof (args), &(key), sizeof (key), &(tmp), sizeof (tmp), &(target), sizeof (target), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9234
	case 2: {
	//ReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	}
	}
	    
			{
	switch(__choose("__SWITCH90"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9235
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9236
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9237
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9238
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9239
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9240
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9241
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9242
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9243
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9244
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9245
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9246
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9247
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9248
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9249
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9250
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9251
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9252
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9253
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9254
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9255
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9256
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9257
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9258
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9259
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9260
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9261
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9262
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9263
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9264
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9265
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9266
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9267
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9268
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9269
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9270
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9271
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9272
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9273
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9274
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9275
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9276
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9277
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9278
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9279
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9280
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9281
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9282
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9283
	case 49: {
	//AddInitKind
	memset(&executor_globals, 0, sizeof (*(&executor_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9284
	case 50: {
	//IfExitKind
	if (__is_neg("90-50", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9285
	case 51: {
	//IfExitKind
	if (__is_neg("90-51", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9286
	case 52: {
	//IfExitKind
	if (__is_neg("90-52", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9287
	case 53: {
	//IfExitKind
	if (__is_neg("90-53", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9288
	case 54: {
	//IfExitKind
	if (__is_neg("90-54", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 64;
	
	break;
	}
	#endif
	}
	{
	long long __temp91=(!(executor_globals.active_symbol_table)) ;
	switch(__choose("__SWITCH91"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_9289
	case 1: {
	__temp91= ((!(executor_globals.active_symbol_table)) || __is_neg("91-1", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9290
	case 2: {
	__temp91= ((!(executor_globals.active_symbol_table)) && !__is_neg("91-2", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH94"))
	{
	case 0: {
	if (__temp91) {
				{
	switch(__choose("__SWITCH92"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9291
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9292
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9293
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9294
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9295
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9296
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9297
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9298
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9299
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9300
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9301
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9302
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9303
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9304
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9305
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9306
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9307
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9308
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9309
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9310
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9311
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9312
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9313
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9314
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9315
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9316
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9317
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9318
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9319
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9320
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9321
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9322
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9323
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9324
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9325
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9326
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9327
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9328
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9329
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9330
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9331
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9332
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9333
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9334
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9335
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9336
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9337
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9338
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9339
	case 49: {
	//IfExitKind
	if (__is_neg("92-49", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(key), sizeof (key), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9340
	case 50: {
	//IfExitKind
	if (__is_neg("92-50", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(key), sizeof (key), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9341
	case 51: {
	//IfExitKind
	if (__is_neg("92-51", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(key), sizeof (key), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9342
	case 52: {
	//IfExitKind
	if (__is_neg("92-52", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(key), sizeof (key), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9343
	case 53: {
	//IfExitKind
	if (__is_neg("92-53", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(key), sizeof (key), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 64;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH93"))
	{
	case 0: {
	zend_rebuild_symbol_table(TSRMLS_C);
	
	break;
	}
	#ifdef COMPILE_9344
	case 1: {
	//GuardKind
	if (!__is_neg("93-1", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(key), sizeof (key), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	}
	}
			}
	
	break;
	}
	#ifdef COMPILE_9345
	case 1: {
	//GuardKind
	if (!__is_neg("94-1", 96, &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(p), sizeof (p), &(key), sizeof (key), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(args), sizeof (args), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	}
	}
	}
			{
	switch(__choose("__SWITCH95"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9346
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9347
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9348
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9349
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9350
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9351
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9352
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9353
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9354
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9355
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9356
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9357
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9358
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9359
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9360
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9361
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9362
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9363
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9364
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9365
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9366
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9367
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9368
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9369
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9370
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9371
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9372
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9373
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9374
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9375
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9376
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9377
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9378
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9379
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9380
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9381
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9382
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9383
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9384
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9385
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9386
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9387
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9388
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9389
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9390
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9391
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9392
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9393
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9394
	case 49: {
	//AddInitKind
	memset(&((zval_gc_info *)(tmp))->u, 0, sizeof (*(&((zval_gc_info *)(tmp))->u)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9395
	case 50: {
	//AddInitKind
	memset(&(**(p)), 0, sizeof (*(&(**(p)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9396
	case 51: {
	//AddInitKind
	memset(&(**p), 0, sizeof (*(&(**p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9397
	case 52: {
	//AddInitKind
	memset(&(*tmp), 0, sizeof (*(&(*tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9398
	case 53: {
	//AddInitKind
	memset(&executor_globals, 0, sizeof (*(&executor_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9399
	case 54: {
	//AddInitKind
	memset(((zval_gc_info *)(tmp)), 0, sizeof (*(((zval_gc_info *)(tmp)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9400
	case 55: {
	//AddInitKind
	memset((*(p)), 0, sizeof (*((*(p)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9401
	case 56: {
	//AddInitKind
	memset((*p), 0, sizeof (*((*p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9402
	case 57: {
	//AddInitKind
	memset((tmp), 0, sizeof (*((tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9403
	case 58: {
	//AddInitKind
	memset(key, 0, sizeof (*(key)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9404
	case 59: {
	//IfExitKind
	if (__is_neg("95-59", 99, &(p), sizeof (p), &(key), sizeof (key), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9405
	case 60: {
	//IfExitKind
	if (__is_neg("95-60", 99, &(p), sizeof (p), &(key), sizeof (key), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9406
	case 61: {
	//IfExitKind
	if (__is_neg("95-61", 99, &(p), sizeof (p), &(key), sizeof (key), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9407
	case 62: {
	//IfExitKind
	if (__is_neg("95-62", 99, &(p), sizeof (p), &(key), sizeof (key), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9408
	case 63: {
	//IfExitKind
	if (__is_neg("95-63", 99, &(p), sizeof (p), &(key), sizeof (key), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)))
	    return 64;
	
	break;
	}
	#endif
	}
	{
	long long __temp96=(zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, (void **)&p) == -1) ;
	switch(__choose("__SWITCH96"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_9409
	case 1: {
	__temp96= ((zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, (void **)&p) == -1) || __is_neg("96-1", 99, &(p), sizeof (p), &(key), sizeof (key), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9410
	case 2: {
	__temp96= ((zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, (void **)&p) == -1) && !__is_neg("96-2", 99, &(p), sizeof (p), &(key), sizeof (key), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(executor_globals.active_symbol_table), sizeof (executor_globals.active_symbol_table), &(is_ref), sizeof (is_ref), &(tmp), sizeof (tmp), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(executor_globals.uninitialized_zval_ptr), sizeof (executor_globals.uninitialized_zval_ptr), &(args), sizeof (args), &(target), sizeof (target), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw), &(executor_globals.return_value_ptr_ptr), sizeof (executor_globals.return_value_ptr_ptr), &(executor_globals.error_zval_ptr), sizeof (executor_globals.error_zval_ptr), &(executor_globals.symtable_cache_limit), sizeof (executor_globals.symtable_cache_limit), &(executor_globals.symtable_cache_ptr), sizeof (executor_globals.symtable_cache_ptr), &(executor_globals.opline_ptr), sizeof (executor_globals.opline_ptr), &(executor_globals.bailout), sizeof (executor_globals.bailout), &(executor_globals.error_reporting), sizeof (executor_globals.error_reporting), &(executor_globals.orig_error_reporting), sizeof (executor_globals.orig_error_reporting), &(executor_globals.exit_status), sizeof (executor_globals.exit_status), &(executor_globals.active_op_array), sizeof (executor_globals.active_op_array), &(executor_globals.function_table), sizeof (executor_globals.function_table), &(executor_globals.class_table), sizeof (executor_globals.class_table), &(executor_globals.zend_constants), sizeof (executor_globals.zend_constants), &(executor_globals.scope), sizeof (executor_globals.scope), &(executor_globals.called_scope), sizeof (executor_globals.called_scope), &(executor_globals.This), sizeof (executor_globals.This), &(executor_globals.precision), sizeof (executor_globals.precision), &(executor_globals.ticks_count), sizeof (executor_globals.ticks_count), &(executor_globals.in_execution), sizeof (executor_globals.in_execution), &(executor_globals.in_autoload), sizeof (executor_globals.in_autoload), &(executor_globals.autoload_func), sizeof (executor_globals.autoload_func), &(executor_globals.full_tables_cleanup), sizeof (executor_globals.full_tables_cleanup), &(executor_globals.no_extensions), sizeof (executor_globals.no_extensions), &(executor_globals.argument_stack), sizeof (executor_globals.argument_stack), &(executor_globals.user_error_handler_error_reporting), sizeof (executor_globals.user_error_handler_error_reporting), &(executor_globals.user_error_handler), sizeof (executor_globals.user_error_handler), &(executor_globals.user_exception_handler), sizeof (executor_globals.user_exception_handler), &(executor_globals.error_handling), sizeof (executor_globals.error_handling), &(executor_globals.exception_class), sizeof (executor_globals.exception_class), &(executor_globals.timeout_seconds), sizeof (executor_globals.timeout_seconds), &(executor_globals.lambda_count), sizeof (executor_globals.lambda_count), &(executor_globals.ini_directives), sizeof (executor_globals.ini_directives), &(executor_globals.modified_ini_directives), sizeof (executor_globals.modified_ini_directives), &(executor_globals.error_reporting_ini_entry), sizeof (executor_globals.error_reporting_ini_entry), &(executor_globals.exception), sizeof (executor_globals.exception), &(executor_globals.prev_exception), sizeof (executor_globals.prev_exception), &(executor_globals.opline_before_exception), sizeof (executor_globals.opline_before_exception), &(executor_globals.current_execute_data), sizeof (executor_globals.current_execute_data), &(executor_globals.current_module), sizeof (executor_globals.current_module), &(executor_globals.active), sizeof (executor_globals.active), &(executor_globals.start_op), sizeof (executor_globals.start_op), &(executor_globals.saved_fpu_cw_ptr), sizeof (executor_globals.saved_fpu_cw_ptr), &(executor_globals.saved_fpu_cw), sizeof (executor_globals.saved_fpu_cw)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH111"))
	{
	case 0: {
	if (__temp96) {
				if (is_ref) {        
					ALLOC_INIT_ZVAL(tmp);
					Z_SET_ISREF_P(tmp);
					zend_hash_quick_add(EG(active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval*), (void**)&p);
				} else {
					tmp = EG(uninitialized_zval_ptr);
					zend_error(E_NOTICE,"Undefined variable: %s", key->arKey);
				}
			} else {
				{
	switch(__choose("__SWITCH97"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9411
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9412
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9413
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9414
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9415
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9416
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9417
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9418
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9419
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9420
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9421
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9422
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9423
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9424
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9425
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9426
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9427
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9428
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9429
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9430
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9431
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9432
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9433
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9434
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9435
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9436
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9437
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9438
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9439
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9440
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9441
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9442
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9443
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9444
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9445
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9446
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9447
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9448
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9449
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9450
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9451
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9452
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9453
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9454
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9455
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9456
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9457
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9458
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9459
	case 49: {
	//AddInitKind
	memset(&((zval_gc_info *)(tmp))->u, 0, sizeof (*(&((zval_gc_info *)(tmp))->u)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9460
	case 50: {
	//AddInitKind
	memset(&(**(p)), 0, sizeof (*(&(**(p)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9461
	case 51: {
	//AddInitKind
	memset(&(**p), 0, sizeof (*(&(**p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9462
	case 52: {
	//AddInitKind
	memset(&(*tmp), 0, sizeof (*(&(*tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9463
	case 53: {
	//AddInitKind
	memset(((zval_gc_info *)(tmp)), 0, sizeof (*(((zval_gc_info *)(tmp)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9464
	case 54: {
	//AddInitKind
	memset((*(p)), 0, sizeof (*((*(p)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9465
	case 55: {
	//AddInitKind
	memset((*p), 0, sizeof (*((*p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9466
	case 56: {
	//AddInitKind
	memset((tmp), 0, sizeof (*((tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9467
	case 57: {
	//IfExitKind
	if (__is_neg("97-57", 12, &(is_ref), sizeof (is_ref), &(p), sizeof (p), &((**(p)).type), sizeof ((**(p)).type), &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((*(p))->refcount__gc), sizeof ((*(p))->refcount__gc), &((*(p))->type), sizeof ((*(p))->type), &((*(p))->is_ref__gc), sizeof ((*(p))->is_ref__gc), &((**(p)).refcount__gc), sizeof ((**(p)).refcount__gc), &((**(p)).is_ref__gc), sizeof ((**(p)).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9468
	case 58: {
	//IfExitKind
	if (__is_neg("97-58", 12, &(is_ref), sizeof (is_ref), &(p), sizeof (p), &((**(p)).type), sizeof ((**(p)).type), &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((*(p))->refcount__gc), sizeof ((*(p))->refcount__gc), &((*(p))->type), sizeof ((*(p))->type), &((*(p))->is_ref__gc), sizeof ((*(p))->is_ref__gc), &((**(p)).refcount__gc), sizeof ((**(p)).refcount__gc), &((**(p)).is_ref__gc), sizeof ((**(p)).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9469
	case 59: {
	//IfExitKind
	if (__is_neg("97-59", 12, &(is_ref), sizeof (is_ref), &(p), sizeof (p), &((**(p)).type), sizeof ((**(p)).type), &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((*(p))->refcount__gc), sizeof ((*(p))->refcount__gc), &((*(p))->type), sizeof ((*(p))->type), &((*(p))->is_ref__gc), sizeof ((*(p))->is_ref__gc), &((**(p)).refcount__gc), sizeof ((**(p)).refcount__gc), &((**(p)).is_ref__gc), sizeof ((**(p)).is_ref__gc)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9470
	case 60: {
	//IfExitKind
	if (__is_neg("97-60", 12, &(is_ref), sizeof (is_ref), &(p), sizeof (p), &((**(p)).type), sizeof ((**(p)).type), &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((*(p))->refcount__gc), sizeof ((*(p))->refcount__gc), &((*(p))->type), sizeof ((*(p))->type), &((*(p))->is_ref__gc), sizeof ((*(p))->is_ref__gc), &((**(p)).refcount__gc), sizeof ((**(p)).refcount__gc), &((**(p)).is_ref__gc), sizeof ((**(p)).is_ref__gc)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9471
	case 61: {
	//IfExitKind
	if (__is_neg("97-61", 12, &(is_ref), sizeof (is_ref), &(p), sizeof (p), &((**(p)).type), sizeof ((**(p)).type), &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((*(p))->refcount__gc), sizeof ((*(p))->refcount__gc), &((*(p))->type), sizeof ((*(p))->type), &((*(p))->is_ref__gc), sizeof ((*(p))->is_ref__gc), &((**(p)).refcount__gc), sizeof ((**(p)).refcount__gc), &((**(p)).is_ref__gc), sizeof ((**(p)).is_ref__gc)))
	    return 64;
	
	break;
	}
	#endif
	}
	{
	long long __temp98=(is_ref) ;
	switch(__choose("__SWITCH98"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_9472
	case 1: {
	__temp98= ((is_ref) || __is_neg("98-1", 12, &(is_ref), sizeof (is_ref), &(p), sizeof (p), &((**(p)).type), sizeof ((**(p)).type), &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((*(p))->refcount__gc), sizeof ((*(p))->refcount__gc), &((*(p))->type), sizeof ((*(p))->type), &((*(p))->is_ref__gc), sizeof ((*(p))->is_ref__gc), &((**(p)).refcount__gc), sizeof ((**(p)).refcount__gc), &((**(p)).is_ref__gc), sizeof ((**(p)).is_ref__gc)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9473
	case 2: {
	__temp98= ((is_ref) && !__is_neg("98-2", 12, &(is_ref), sizeof (is_ref), &(p), sizeof (p), &((**(p)).type), sizeof ((**(p)).type), &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((*(p))->refcount__gc), sizeof ((*(p))->refcount__gc), &((*(p))->type), sizeof ((*(p))->type), &((*(p))->is_ref__gc), sizeof ((*(p))->is_ref__gc), &((**(p)).refcount__gc), sizeof ((**(p)).refcount__gc), &((**(p)).is_ref__gc), sizeof ((**(p)).is_ref__gc)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH110"))
	{
	case 0: {
	if (__temp98) {
					SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
					tmp = *p;
				} else {
	switch(__choose("__SWITCH99"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9474
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9475
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9476
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9477
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9478
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9479
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9480
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9481
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9482
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9483
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9484
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9485
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9486
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9487
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9488
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9489
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9490
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9491
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9492
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9493
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9494
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9495
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9496
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9497
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9498
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9499
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9500
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9501
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9502
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9503
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9504
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9505
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9506
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9507
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9508
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9509
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9510
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9511
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9512
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9513
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9514
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9515
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9516
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9517
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9518
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9519
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9520
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9521
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9522
	case 49: {
	//AddInitKind
	memset(&((zval_gc_info *)(tmp))->u, 0, sizeof (*(&((zval_gc_info *)(tmp))->u)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9523
	case 50: {
	//AddInitKind
	memset(&(**p), 0, sizeof (*(&(**p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9524
	case 51: {
	//AddInitKind
	memset(&(*tmp), 0, sizeof (*(&(*tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9525
	case 52: {
	//AddInitKind
	memset(((zval_gc_info *)(tmp)), 0, sizeof (*(((zval_gc_info *)(tmp)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9526
	case 53: {
	//AddInitKind
	memset((*p), 0, sizeof (*((*p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9527
	case 54: {
	//AddInitKind
	memset((tmp), 0, sizeof (*((tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9528
	case 55: {
	//IfExitKind
	if (__is_neg("99-55", 12, &(p), sizeof (p), &(tmp), sizeof (tmp), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9529
	case 56: {
	//IfExitKind
	if (__is_neg("99-56", 12, &(p), sizeof (p), &(tmp), sizeof (tmp), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9530
	case 57: {
	//IfExitKind
	if (__is_neg("99-57", 12, &(p), sizeof (p), &(tmp), sizeof (tmp), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9531
	case 58: {
	//IfExitKind
	if (__is_neg("99-58", 12, &(p), sizeof (p), &(tmp), sizeof (tmp), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9532
	case 59: {
	//IfExitKind
	if (__is_neg("99-59", 12, &(p), sizeof (p), &(tmp), sizeof (tmp), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 64;
	
	break;
	}
	#endif
	}
	{
	long long __temp100=(zval_isref_p(*(p))) ;
	switch(__choose("__SWITCH100"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_9533
	case 1: {
	__temp100= ((zval_isref_p(*(p))) || __is_neg("100-1", 12, &(p), sizeof (p), &(tmp), sizeof (tmp), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9534
	case 2: {
	__temp100= ((zval_isref_p(*(p))) && !__is_neg("100-2", 12, &(p), sizeof (p), &(tmp), sizeof (tmp), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH109"))
	{
	case 0: {
	if (__temp100) {
					{
	switch(__choose("__SWITCH101"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9535
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9536
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9537
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9538
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9539
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9540
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9541
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9542
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9543
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9544
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9545
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9546
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9547
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9548
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9549
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9550
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9551
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9552
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9553
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9554
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9555
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9556
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9557
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9558
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9559
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9560
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9561
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9562
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9563
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9564
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9565
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9566
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9567
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9568
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9569
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9570
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9571
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9572
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9573
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9574
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9575
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9576
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9577
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9578
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9579
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9580
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9581
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9582
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9583
	case 49: {
	//AddInitKind
	memset(&((zval_gc_info *)(tmp))->u, 0, sizeof (*(&((zval_gc_info *)(tmp))->u)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9584
	case 50: {
	//AddInitKind
	memset(((zval_gc_info *)(tmp)), 0, sizeof (*(((zval_gc_info *)(tmp)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9585
	case 51: {
	//AddInitKind
	memset((tmp), 0, sizeof (*((tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9586
	case 52: {
	//IfExitKind
	if (__is_neg("101-52", 21, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9587
	case 53: {
	//IfExitKind
	if (__is_neg("101-53", 21, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9588
	case 54: {
	//IfExitKind
	if (__is_neg("101-54", 21, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9589
	case 55: {
	//IfExitKind
	if (__is_neg("101-55", 21, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9590
	case 56: {
	//IfExitKind
	if (__is_neg("101-56", 21, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 64;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH102"))
	{
	case 0: {
	ALLOC_INIT_ZVAL(tmp);
	
	break;
	}
	#ifdef COMPILE_9591
	case 1: {
	//GuardKind
	if (!__is_neg("102-1", 21, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    do {
	        (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
	        ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
	    } while (0);
	
	break;
	}
	#endif
	}
	}
					{
	switch(__choose("__SWITCH103"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9592
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9593
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9594
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9595
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9596
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9597
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9598
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9599
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9600
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9601
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9602
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9603
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9604
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9605
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9606
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9607
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9608
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9609
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9610
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9611
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9612
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9613
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9614
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9615
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9616
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9617
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9618
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9619
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9620
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9621
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9622
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9623
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9624
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9625
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9626
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9627
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9628
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9629
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9630
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9631
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9632
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9633
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9634
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9635
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9636
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9637
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9638
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9639
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9640
	case 49: {
	//AddInitKind
	memset(&(**p), 0, sizeof (*(&(**p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9641
	case 50: {
	//AddInitKind
	memset(&(*tmp), 0, sizeof (*(&(*tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9642
	case 51: {
	//AddInitKind
	memset((*p), 0, sizeof (*((*p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9643
	case 52: {
	//AddInitKind
	memset((tmp), 0, sizeof (*((tmp))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9644
	case 53: {
	//IfExitKind
	if (__is_neg("103-53", 21, &(p), sizeof (p), &(tmp), sizeof (tmp), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9645
	case 54: {
	//IfExitKind
	if (__is_neg("103-54", 21, &(p), sizeof (p), &(tmp), sizeof (tmp), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9646
	case 55: {
	//IfExitKind
	if (__is_neg("103-55", 21, &(p), sizeof (p), &(tmp), sizeof (tmp), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9647
	case 56: {
	//IfExitKind
	if (__is_neg("103-56", 21, &(p), sizeof (p), &(tmp), sizeof (tmp), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9648
	case 57: {
	//IfExitKind
	if (__is_neg("103-57", 21, &(p), sizeof (p), &(tmp), sizeof (tmp), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    return 64;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH104"))
	{
	case 0: {
	ZVAL_COPY_VALUE(tmp, *p);
	
	break;
	}
	#ifdef COMPILE_9649
	case 1: {
	//GuardKind
	if (!__is_neg("104-1", 21, &(p), sizeof (p), &(tmp), sizeof (tmp), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc), &(zval_used_for_init.refcount__gc), sizeof (zval_used_for_init.refcount__gc), &(zval_used_for_init.type), sizeof (zval_used_for_init.type), &(zval_used_for_init.is_ref__gc), sizeof (zval_used_for_init.is_ref__gc)))
	    do {
	        (tmp)->value = (*p)->value;
	        (*tmp).type = (**p).type;
	    } while (0);
	
	break;
	}
	#endif
	}
	}
					{
	switch(__choose("__SWITCH105"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9650
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9651
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9652
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9653
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9654
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9655
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9656
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9657
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9658
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9659
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9660
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9661
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9662
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9663
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9664
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9665
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9666
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9667
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9668
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9669
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9670
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9671
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9672
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9673
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9674
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9675
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9676
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9677
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9678
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9679
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9680
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9681
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9682
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9683
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9684
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9685
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9686
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9687
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9688
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9689
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9690
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9691
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9692
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9693
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9694
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9695
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9696
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9697
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9698
	case 49: {
	//IfExitKind
	if (__is_neg("105-49", 18, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9699
	case 50: {
	//IfExitKind
	if (__is_neg("105-50", 18, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9700
	case 51: {
	//IfExitKind
	if (__is_neg("105-51", 18, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9701
	case 52: {
	//IfExitKind
	if (__is_neg("105-52", 18, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9702
	case 53: {
	//IfExitKind
	if (__is_neg("105-53", 18, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    return 64;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH106"))
	{
	case 0: {
	Z_SET_REFCOUNT_P(tmp, 0);
	
	break;
	}
	#ifdef COMPILE_9703
	case 1: {
	//AddAndReplaceKind
	__overflow(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9704
	case 2: {
	//AddAndReplaceKind
	__sigsetjmp(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9705
	case 3: {
	//AddAndReplaceKind
	__stpcpy(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9706
	case 4: {
	//AddAndReplaceKind
	__zend_realloc(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9707
	case 5: {
	//AddAndReplaceKind
	_array_init(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9708
	case 6: {
	//AddAndReplaceKind
	_estrndup(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9709
	case 7: {
	//AddAndReplaceKind
	_longjmp(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9710
	case 8: {
	//AddAndReplaceKind
	_object_init_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9711
	case 9: {
	//AddAndReplaceKind
	_zend_bailout(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9712
	case 10: {
	//AddAndReplaceKind
	_zend_mm_alloc(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9713
	case 11: {
	//AddAndReplaceKind
	_zend_mm_block_size(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9714
	case 12: {
	//AddAndReplaceKind
	_zend_mm_free(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9715
	case 13: {
	//AddAndReplaceKind
	add_index_null(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9716
	case 14: {
	//AddAndReplaceKind
	add_next_index_bool(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9717
	case 15: {
	//AddAndReplaceKind
	add_next_index_long(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9718
	case 16: {
	//AddAndReplaceKind
	add_next_index_resource(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9719
	case 17: {
	//AddAndReplaceKind
	add_next_index_zval(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9720
	case 18: {
	//AddAndReplaceKind
	add_trait_alias(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9721
	case 19: {
	//AddAndReplaceKind
	bitwise_not_function(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9722
	case 20: {
	//AddAndReplaceKind
	boolean_not_function(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9723
	case 21: {
	//AddAndReplaceKind
	bzero(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9724
	case 22: {
	//AddAndReplaceKind
	compile_file(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9725
	case 23: {
	//AddAndReplaceKind
	compile_string(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9726
	case 24: {
	//AddAndReplaceKind
	convert_to_long_base(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9727
	case 25: {
	//AddAndReplaceKind
	dlopen(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9728
	case 26: {
	//AddAndReplaceKind
	dlsym(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9729
	case 27: {
	//AddAndReplaceKind
	drand48_r(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9730
	case 28: {
	//AddAndReplaceKind
	execute_internal(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9731
	case 29: {
	//AddAndReplaceKind
	explicit_bzero(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9732
	case 30: {
	//AddAndReplaceKind
	fgetpos(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9733
	case 31: {
	//AddAndReplaceKind
	fopen(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9734
	case 32: {
	//AddAndReplaceKind
	fprintf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9735
	case 33: {
	//AddAndReplaceKind
	fputs(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9736
	case 34: {
	//AddAndReplaceKind
	fscanf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9737
	case 35: {
	//AddAndReplaceKind
	fsetpos(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9738
	case 36: {
	//AddAndReplaceKind
	getloadavg(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9739
	case 37: {
	//AddAndReplaceKind
	index(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9740
	case 38: {
	//AddAndReplaceKind
	init_trait_alias_list(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9741
	case 39: {
	//AddAndReplaceKind
	instanceof_function(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9742
	case 40: {
	//AddAndReplaceKind
	lcong48_r(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9743
	case 41: {
	//AddAndReplaceKind
	longjmp(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9744
	case 42: {
	//AddAndReplaceKind
	lrand48_r(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9745
	case 43: {
	//AddAndReplaceKind
	mblen(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9746
	case 44: {
	//AddAndReplaceKind
	mkstemps(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9747
	case 45: {
	//AddAndReplaceKind
	mrand48_r(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9748
	case 46: {
	//AddAndReplaceKind
	object_properties_init(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9749
	case 47: {
	//AddAndReplaceKind
	on_exit(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9750
	case 48: {
	//AddAndReplaceKind
	open_memstream(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9751
	case 49: {
	//AddAndReplaceKind
	popen(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9752
	case 50: {
	//AddAndReplaceKind
	print_op_array(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9753
	case 51: {
	//AddAndReplaceKind
	printf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9754
	case 52: {
	//AddAndReplaceKind
	random_r(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9755
	case 53: {
	//AddAndReplaceKind
	realloc(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9756
	case 54: {
	//AddAndReplaceKind
	realpath(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9757
	case 55: {
	//AddAndReplaceKind
	rename(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9758
	case 56: {
	//AddAndReplaceKind
	rindex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9759
	case 57: {
	//AddAndReplaceKind
	scanf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9760
	case 58: {
	//AddAndReplaceKind
	seed48_r(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9761
	case 59: {
	//AddAndReplaceKind
	setbuf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9762
	case 60: {
	//AddAndReplaceKind
	setstate_r(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9763
	case 61: {
	//AddAndReplaceKind
	siglongjmp(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9764
	case 62: {
	//AddAndReplaceKind
	sprintf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9765
	case 63: {
	//AddAndReplaceKind
	sscanf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9766
	case 64: {
	//AddAndReplaceKind
	stpcpy(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9767
	case 65: {
	//AddAndReplaceKind
	strcasecmp(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9768
	case 66: {
	//AddAndReplaceKind
	strcat(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9769
	case 67: {
	//AddAndReplaceKind
	strchr(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9770
	case 68: {
	//AddAndReplaceKind
	strcmp(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9771
	case 69: {
	//AddAndReplaceKind
	strcoll(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9772
	case 70: {
	//AddAndReplaceKind
	strcpy(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9773
	case 71: {
	//AddAndReplaceKind
	strcspn(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9774
	case 72: {
	//AddAndReplaceKind
	strndup(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9775
	case 73: {
	//AddAndReplaceKind
	strnlen(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9776
	case 74: {
	//AddAndReplaceKind
	strpbrk(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9777
	case 75: {
	//AddAndReplaceKind
	strrchr(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9778
	case 76: {
	//AddAndReplaceKind
	strsep(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9779
	case 77: {
	//AddAndReplaceKind
	strspn(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9780
	case 78: {
	//AddAndReplaceKind
	strstr(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9781
	case 79: {
	//AddAndReplaceKind
	strtod(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9782
	case 80: {
	//AddAndReplaceKind
	strtof(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9783
	case 81: {
	//AddAndReplaceKind
	strtok(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9784
	case 82: {
	//AddAndReplaceKind
	strtold(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9785
	case 83: {
	//AddAndReplaceKind
	tempnam(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9786
	case 84: {
	//AddAndReplaceKind
	vprintf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9787
	case 85: {
	//AddAndReplaceKind
	vscanf(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9788
	case 86: {
	//AddAndReplaceKind
	wctomb(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9789
	case 87: {
	//AddAndReplaceKind
	zend_add_literal(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9790
	case 88: {
	//AddAndReplaceKind
	zend_add_to_list(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9791
	case 89: {
	//AddAndReplaceKind
	zend_atoi(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9792
	case 90: {
	//AddAndReplaceKind
	zend_atol(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9793
	case 91: {
	//AddAndReplaceKind
	zend_binary_zval_strcasecmp(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9794
	case 92: {
	//AddAndReplaceKind
	zend_binary_zval_strcmp(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9795
	case 93: {
	//AddAndReplaceKind
	zend_call_function(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9796
	case 94: {
	//AddAndReplaceKind
	zend_check_protected(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9797
	case 95: {
	//AddAndReplaceKind
	zend_class_implements(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9798
	case 96: {
	//AddAndReplaceKind
	zend_compare_file_handles(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9799
	case 97: {
	//AddAndReplaceKind
	zend_copy_constants(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9800
	case 98: {
	//AddAndReplaceKind
	zend_delete_global_variable(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9801
	case 99: {
	//AddAndReplaceKind
	zend_dirname(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9802
	case 100: {
	//AddAndReplaceKind
	zend_disable_class(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9803
	case 101: {
	//AddAndReplaceKind
	zend_disable_function(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9804
	case 102: {
	//AddAndReplaceKind
	zend_do_begin_class_member_function_call(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9805
	case 103: {
	//AddAndReplaceKind
	zend_do_begin_dynamic_function_call(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9806
	case 104: {
	//AddAndReplaceKind
	zend_do_begin_function_call(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9807
	case 105: {
	//AddAndReplaceKind
	zend_do_begin_namespace(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9808
	case 106: {
	//AddAndReplaceKind
	zend_do_begin_new_object(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9809
	case 107: {
	//AddAndReplaceKind
	zend_do_begin_qm_op(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9810
	case 108: {
	//AddAndReplaceKind
	zend_do_boolean_and_begin(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9811
	case 109: {
	//AddAndReplaceKind
	zend_do_boolean_or_begin(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9812
	case 110: {
	//AddAndReplaceKind
	zend_do_case_after_statement(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9813
	case 111: {
	//AddAndReplaceKind
	zend_do_clone(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9814
	case 112: {
	//AddAndReplaceKind
	zend_do_declare_class_constant(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9815
	case 113: {
	//AddAndReplaceKind
	zend_do_declare_constant(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9816
	case 114: {
	//AddAndReplaceKind
	zend_do_declare_stmt(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9817
	case 115: {
	//AddAndReplaceKind
	zend_do_default_before_statement(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9818
	case 116: {
	//AddAndReplaceKind
	zend_do_end_class_declaration(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9819
	case 117: {
	//AddAndReplaceKind
	zend_do_exit(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9820
	case 118: {
	//AddAndReplaceKind
	zend_do_fetch_class(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9821
	case 119: {
	//AddAndReplaceKind
	zend_do_fetch_lexical_variable(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9822
	case 120: {
	//AddAndReplaceKind
	zend_do_fetch_static_member(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9823
	case 121: {
	//AddAndReplaceKind
	zend_do_for_before_statement(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9824
	case 122: {
	//AddAndReplaceKind
	zend_do_for_cond(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9825
	case 123: {
	//AddAndReplaceKind
	zend_do_foreach_end(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9826
	case 124: {
	//AddAndReplaceKind
	zend_do_if_after_statement(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9827
	case 125: {
	//AddAndReplaceKind
	zend_do_if_cond(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9828
	case 126: {
	//AddAndReplaceKind
	zend_do_implement_interface(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9829
	case 127: {
	//AddAndReplaceKind
	zend_do_implement_trait(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9830
	case 128: {
	//AddAndReplaceKind
	zend_do_inherit_interfaces(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9831
	case 129: {
	//AddAndReplaceKind
	zend_do_inheritance(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9832
	case 130: {
	//AddAndReplaceKind
	zend_do_list_end(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9833
	case 131: {
	//AddAndReplaceKind
	zend_do_mark_last_catch(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9834
	case 132: {
	//AddAndReplaceKind
	zend_do_print(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9835
	case 133: {
	//AddAndReplaceKind
	zend_do_return(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9836
	case 134: {
	//AddAndReplaceKind
	zend_do_shell_exec(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9837
	case 135: {
	//AddAndReplaceKind
	zend_do_verify_access_types(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9838
	case 136: {
	//AddAndReplaceKind
	zend_do_while_cond(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9839
	case 137: {
	//AddAndReplaceKind
	zend_do_while_end(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9840
	case 138: {
	//AddAndReplaceKind
	zend_fcall_info_argn(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9841
	case 139: {
	//AddAndReplaceKind
	zend_fcall_info_args(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9842
	case 140: {
	//AddAndReplaceKind
	zend_fcall_info_args_clear(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9843
	case 141: {
	//AddAndReplaceKind
	zend_get_class_fetch_type(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9844
	case 142: {
	//AddAndReplaceKind
	zend_get_compiled_variable_value(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9845
	case 143: {
	//AddAndReplaceKind
	zend_get_hash_value(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9846
	case 144: {
	//AddAndReplaceKind
	zend_hash_apply(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9847
	case 145: {
	//AddAndReplaceKind
	zend_hash_func(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9848
	case 146: {
	//AddAndReplaceKind
	zend_hash_get_current_key_type_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9849
	case 147: {
	//AddAndReplaceKind
	zend_hash_get_pointer(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9850
	case 148: {
	//AddAndReplaceKind
	zend_hash_index_exists(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9851
	case 149: {
	//AddAndReplaceKind
	zend_hash_internal_pointer_end_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9852
	case 150: {
	//AddAndReplaceKind
	zend_hash_internal_pointer_reset_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9853
	case 151: {
	//AddAndReplaceKind
	zend_hash_move_backwards_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9854
	case 152: {
	//AddAndReplaceKind
	zend_hash_move_forward_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9855
	case 153: {
	//AddAndReplaceKind
	zend_hash_reverse_apply(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9856
	case 154: {
	//AddAndReplaceKind
	zend_hash_set_pointer(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9857
	case 155: {
	//AddAndReplaceKind
	zend_hex_strtod(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9858
	case 156: {
	//AddAndReplaceKind
	zend_init_list(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9859
	case 157: {
	//AddAndReplaceKind
	zend_initialize_class_data(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9860
	case 158: {
	//AddAndReplaceKind
	zend_inline_hash_func(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9861
	case 159: {
	//AddAndReplaceKind
	zend_is_auto_global(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9862
	case 160: {
	//AddAndReplaceKind
	zend_iterator_unwrap(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9863
	case 161: {
	//AddAndReplaceKind
	zend_list_insert(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9864
	case 162: {
	//AddAndReplaceKind
	zend_llist_add_element(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9865
	case 163: {
	//AddAndReplaceKind
	zend_llist_apply(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9866
	case 164: {
	//AddAndReplaceKind
	zend_llist_apply_with_del(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9867
	case 165: {
	//AddAndReplaceKind
	zend_llist_copy(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9868
	case 166: {
	//AddAndReplaceKind
	zend_llist_get_first_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9869
	case 167: {
	//AddAndReplaceKind
	zend_llist_get_last_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9870
	case 168: {
	//AddAndReplaceKind
	zend_llist_get_next_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9871
	case 169: {
	//AddAndReplaceKind
	zend_llist_get_prev_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9872
	case 170: {
	//AddAndReplaceKind
	zend_llist_prepend_element(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9873
	case 171: {
	//AddAndReplaceKind
	zend_llist_sort(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9874
	case 172: {
	//AddAndReplaceKind
	zend_make_callable(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9875
	case 173: {
	//AddAndReplaceKind
	zend_multibyte_set_script_encoding(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9876
	case 174: {
	//AddAndReplaceKind
	zend_multibyte_set_script_encoding_by_string(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9877
	case 175: {
	//AddAndReplaceKind
	zend_object_create_proxy(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9878
	case 176: {
	//AddAndReplaceKind
	zend_object_std_init(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9879
	case 177: {
	//AddAndReplaceKind
	zend_object_store_set_object(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9880
	case 178: {
	//AddAndReplaceKind
	zend_objects_destroy_object(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9881
	case 179: {
	//AddAndReplaceKind
	zend_objects_new(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9882
	case 180: {
	//AddAndReplaceKind
	zend_objects_store_init(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9883
	case 181: {
	//AddAndReplaceKind
	zend_oct_strtod(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9884
	case 182: {
	//AddAndReplaceKind
	zend_print_zval(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9885
	case 183: {
	//AddAndReplaceKind
	zend_print_zval_r(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9886
	case 184: {
	//AddAndReplaceKind
	zend_ptr_stack_apply(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9887
	case 185: {
	//AddAndReplaceKind
	zend_ptr_stack_init_ex(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9888
	case 186: {
	//AddAndReplaceKind
	zend_ptr_stack_n_pop(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9889
	case 187: {
	//AddAndReplaceKind
	zend_ptr_stack_n_push(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9890
	case 188: {
	//AddAndReplaceKind
	zend_ptr_stack_push(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9891
	case 189: {
	//AddAndReplaceKind
	zend_quick_get_constant(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9892
	case 190: {
	//AddAndReplaceKind
	zend_resolve_non_class_name(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9893
	case 191: {
	//AddAndReplaceKind
	zend_stack_top(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9894
	case 192: {
	//AddAndReplaceKind
	zend_startup(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9895
	case 193: {
	//AddAndReplaceKind
	zend_std_get_debug_info(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9896
	case 194: {
	//AddAndReplaceKind
	zend_str_tolower(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9897
	case 195: {
	//AddAndReplaceKind
	zend_str_tolower_dup(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9898
	case 196: {
	//AddAndReplaceKind
	zend_stream_open(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9899
	case 197: {
	//AddAndReplaceKind
	zend_string_to_double(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9900
	case 198: {
	//AddAndReplaceKind
	zend_strndup(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9901
	case 199: {
	//AddAndReplaceKind
	zend_strtod(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9902
	case 200: {
	//AddAndReplaceKind
	zend_ts_hash_apply(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9903
	case 201: {
	//AddAndReplaceKind
	zend_ts_hash_func(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9904
	case 202: {
	//AddAndReplaceKind
	zend_ts_hash_index_exists(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9905
	case 203: {
	//AddAndReplaceKind
	zend_ts_hash_reverse_apply(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9906
	case 204: {
	//AddAndReplaceKind
	zval_set_isref_to_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9907
	case 205: {
	//AddAndReplaceKind
	zval_update_constant(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9908
	case 206: {
	//AddAndReplaceKind
	zval_update_constant_inline_change(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9909
	case 207: {
	//AddAndReplaceKind
	zval_update_constant_no_inline_change(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9910
	case 208: {
	//GuardKind
	if (!__is_neg("106-208", 18, &(tmp), sizeof (tmp), &(p), sizeof (p), &((*tmp).type), sizeof ((*tmp).type), &((**p).type), sizeof ((**p).type), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args), &((tmp)->refcount__gc), sizeof ((tmp)->refcount__gc), &((tmp)->type), sizeof ((tmp)->type), &((tmp)->is_ref__gc), sizeof ((tmp)->is_ref__gc), &((*p)->refcount__gc), sizeof ((*p)->refcount__gc), &((*p)->type), sizeof ((*p)->type), &((*p)->is_ref__gc), sizeof ((*p)->is_ref__gc), &((*tmp).refcount__gc), sizeof ((*tmp).refcount__gc), &((*tmp).is_ref__gc), sizeof ((*tmp).is_ref__gc), &((**p).refcount__gc), sizeof ((**p).refcount__gc), &((**p).is_ref__gc), sizeof ((**p).is_ref__gc)))
	    zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9911
	case 209: {
	//ReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	}
	}
					{
	switch(__choose("__SWITCH107"))
	{
	case 0: 
	break;
	#ifdef COMPILE_9912
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_9913
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9914
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9915
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9916
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_9917
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9918
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_9919
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_9920
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9921
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9922
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9923
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9924
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9925
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9926
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_9927
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9928
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9929
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9930
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9931
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9932
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9933
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9934
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9935
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9936
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9937
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9938
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9939
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9940
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_9941
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_9942
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_9943
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_9944
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9945
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9946
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_9947
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_9948
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_9949
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9950
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9951
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9952
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9953
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9954
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9955
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9956
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9957
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_9958
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_9959
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9960
	case 49: {
	//IfExitKind
	if (__is_neg("107-49", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9961
	case 50: {
	//IfExitKind
	if (__is_neg("107-50", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9962
	case 51: {
	//IfExitKind
	if (__is_neg("107-51", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_9963
	case 52: {
	//IfExitKind
	if (__is_neg("107-52", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_9964
	case 53: {
	//IfExitKind
	if (__is_neg("107-53", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    return 64;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH108"))
	{
	case 0: {
	Z_UNSET_ISREF_P(tmp);
	
	break;
	}
	#ifdef COMPILE_9965
	case 1: {
	//AddAndReplaceKind
	_convert_to_string(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9966
	case 2: {
	//AddAndReplaceKind
	_object_init(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9967
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9968
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9969
	case 5: {
	//AddAndReplaceKind
	_zval_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9970
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9971
	case 7: {
	//AddAndReplaceKind
	_zval_dtor_wrapper(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9972
	case 8: {
	//AddAndReplaceKind
	_zval_internal_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9973
	case 9: {
	//AddAndReplaceKind
	add_next_index_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9974
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9975
	case 11: {
	//AddAndReplaceKind
	convert_to_array(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9976
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9977
	case 13: {
	//AddAndReplaceKind
	convert_to_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9978
	case 14: {
	//AddAndReplaceKind
	convert_to_long(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9979
	case 15: {
	//AddAndReplaceKind
	convert_to_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9980
	case 16: {
	//AddAndReplaceKind
	convert_to_object(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9981
	case 17: {
	//AddAndReplaceKind
	decrement_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9982
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9983
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9984
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9985
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9986
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9987
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9988
	case 24: {
	//AddAndReplaceKind
	increment_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9989
	case 25: {
	//AddAndReplaceKind
	lex_scan(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9990
	case 26: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9991
	case 27: {
	//AddAndReplaceKind
	zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9992
	case 28: {
	//AddAndReplaceKind
	zend_locale_sprintf_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9993
	case 29: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9994
	case 30: {
	//AddAndReplaceKind
	zend_objects_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9995
	case 31: {
	//AddAndReplaceKind
	zend_objects_store_add_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9996
	case 32: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9997
	case 33: {
	//AddAndReplaceKind
	zend_objects_store_del_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9998
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_9999
	case 35: {
	//AddAndReplaceKind
	zend_print_flat_zval_r(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10000
	case 36: {
	//AddAndReplaceKind
	zend_print_variable(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10001
	case 37: {
	//AddAndReplaceKind
	zend_std_get_constructor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10002
	case 38: {
	//AddAndReplaceKind
	zend_std_get_properties(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10003
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10004
	case 40: {
	//AddAndReplaceKind
	zval_delref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10005
	case 41: {
	//AddAndReplaceKind
	zval_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10006
	case 42: {
	//AddAndReplaceKind
	zval_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10007
	case 43: {
	//AddAndReplaceKind
	zval_refcount_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10008
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10009
	case 45: {
	//GuardKind
	if (!__is_neg("108-45", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10010
	case 46: {
	//ReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	}
	}
				} else {
					tmp = *p;
				}
	
	break;
	}
	#ifdef COMPILE_10011
	case 1: {
	//AddAndReplaceKind
	_convert_to_string(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10012
	case 2: {
	//AddAndReplaceKind
	_object_init(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10013
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10014
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10015
	case 5: {
	//AddAndReplaceKind
	_zval_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10016
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10017
	case 7: {
	//AddAndReplaceKind
	_zval_dtor_wrapper(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10018
	case 8: {
	//AddAndReplaceKind
	_zval_internal_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10019
	case 9: {
	//AddAndReplaceKind
	add_next_index_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10020
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10021
	case 11: {
	//AddAndReplaceKind
	convert_to_array(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10022
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10023
	case 13: {
	//AddAndReplaceKind
	convert_to_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10024
	case 14: {
	//AddAndReplaceKind
	convert_to_long(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10025
	case 15: {
	//AddAndReplaceKind
	convert_to_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10026
	case 16: {
	//AddAndReplaceKind
	convert_to_object(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10027
	case 17: {
	//AddAndReplaceKind
	decrement_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10028
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10029
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10030
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10031
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10032
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10033
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10034
	case 24: {
	//AddAndReplaceKind
	increment_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10035
	case 25: {
	//AddAndReplaceKind
	lex_scan(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10036
	case 26: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10037
	case 27: {
	//AddAndReplaceKind
	zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10038
	case 28: {
	//AddAndReplaceKind
	zend_locale_sprintf_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10039
	case 29: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10040
	case 30: {
	//AddAndReplaceKind
	zend_objects_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10041
	case 31: {
	//AddAndReplaceKind
	zend_objects_store_add_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10042
	case 32: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10043
	case 33: {
	//AddAndReplaceKind
	zend_objects_store_del_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10044
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10045
	case 35: {
	//AddAndReplaceKind
	zend_print_flat_zval_r(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10046
	case 36: {
	//AddAndReplaceKind
	zend_print_variable(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10047
	case 37: {
	//AddAndReplaceKind
	zend_std_get_constructor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10048
	case 38: {
	//AddAndReplaceKind
	zend_std_get_properties(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10049
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10050
	case 40: {
	//AddAndReplaceKind
	zval_delref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10051
	case 41: {
	//AddAndReplaceKind
	zval_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10052
	case 42: {
	//AddAndReplaceKind
	zval_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10053
	case 43: {
	//AddAndReplaceKind
	zval_refcount_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10054
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10055
	case 45: {
	//GuardKind
	if (!__is_neg("109-45", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10056
	case 46: {
	//ReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	}
	}
	}
	
	break;
	}
	#ifdef COMPILE_10057
	case 1: {
	//AddAndReplaceKind
	_convert_to_string(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10058
	case 2: {
	//AddAndReplaceKind
	_object_init(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10059
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10060
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10061
	case 5: {
	//AddAndReplaceKind
	_zval_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10062
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10063
	case 7: {
	//AddAndReplaceKind
	_zval_dtor_wrapper(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10064
	case 8: {
	//AddAndReplaceKind
	_zval_internal_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10065
	case 9: {
	//AddAndReplaceKind
	add_next_index_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10066
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10067
	case 11: {
	//AddAndReplaceKind
	convert_to_array(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10068
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10069
	case 13: {
	//AddAndReplaceKind
	convert_to_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10070
	case 14: {
	//AddAndReplaceKind
	convert_to_long(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10071
	case 15: {
	//AddAndReplaceKind
	convert_to_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10072
	case 16: {
	//AddAndReplaceKind
	convert_to_object(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10073
	case 17: {
	//AddAndReplaceKind
	decrement_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10074
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10075
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10076
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10077
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10078
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10079
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10080
	case 24: {
	//AddAndReplaceKind
	increment_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10081
	case 25: {
	//AddAndReplaceKind
	lex_scan(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10082
	case 26: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10083
	case 27: {
	//AddAndReplaceKind
	zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10084
	case 28: {
	//AddAndReplaceKind
	zend_locale_sprintf_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10085
	case 29: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10086
	case 30: {
	//AddAndReplaceKind
	zend_objects_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10087
	case 31: {
	//AddAndReplaceKind
	zend_objects_store_add_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10088
	case 32: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10089
	case 33: {
	//AddAndReplaceKind
	zend_objects_store_del_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10090
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10091
	case 35: {
	//AddAndReplaceKind
	zend_print_flat_zval_r(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10092
	case 36: {
	//AddAndReplaceKind
	zend_print_variable(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10093
	case 37: {
	//AddAndReplaceKind
	zend_std_get_constructor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10094
	case 38: {
	//AddAndReplaceKind
	zend_std_get_properties(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10095
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10096
	case 40: {
	//AddAndReplaceKind
	zval_delref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10097
	case 41: {
	//AddAndReplaceKind
	zval_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10098
	case 42: {
	//AddAndReplaceKind
	zval_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10099
	case 43: {
	//AddAndReplaceKind
	zval_refcount_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10100
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10101
	case 45: {
	//GuardKind
	if (!__is_neg("110-45", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10102
	case 46: {
	//ReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	}
	}
	}
			}
	
	break;
	}
	#ifdef COMPILE_10103
	case 1: {
	//AddAndReplaceKind
	_convert_to_string(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10104
	case 2: {
	//AddAndReplaceKind
	_object_init(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10105
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10106
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10107
	case 5: {
	//AddAndReplaceKind
	_zval_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10108
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10109
	case 7: {
	//AddAndReplaceKind
	_zval_dtor_wrapper(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10110
	case 8: {
	//AddAndReplaceKind
	_zval_internal_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10111
	case 9: {
	//AddAndReplaceKind
	add_next_index_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10112
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10113
	case 11: {
	//AddAndReplaceKind
	convert_to_array(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10114
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10115
	case 13: {
	//AddAndReplaceKind
	convert_to_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10116
	case 14: {
	//AddAndReplaceKind
	convert_to_long(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10117
	case 15: {
	//AddAndReplaceKind
	convert_to_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10118
	case 16: {
	//AddAndReplaceKind
	convert_to_object(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10119
	case 17: {
	//AddAndReplaceKind
	decrement_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10120
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10121
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10122
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10123
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10124
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10125
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10126
	case 24: {
	//AddAndReplaceKind
	increment_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10127
	case 25: {
	//AddAndReplaceKind
	lex_scan(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10128
	case 26: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10129
	case 27: {
	//AddAndReplaceKind
	zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10130
	case 28: {
	//AddAndReplaceKind
	zend_locale_sprintf_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10131
	case 29: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10132
	case 30: {
	//AddAndReplaceKind
	zend_objects_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10133
	case 31: {
	//AddAndReplaceKind
	zend_objects_store_add_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10134
	case 32: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10135
	case 33: {
	//AddAndReplaceKind
	zend_objects_store_del_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10136
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10137
	case 35: {
	//AddAndReplaceKind
	zend_print_flat_zval_r(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10138
	case 36: {
	//AddAndReplaceKind
	zend_print_variable(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10139
	case 37: {
	//AddAndReplaceKind
	zend_std_get_constructor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10140
	case 38: {
	//AddAndReplaceKind
	zend_std_get_properties(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10141
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10142
	case 40: {
	//AddAndReplaceKind
	zval_delref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10143
	case 41: {
	//AddAndReplaceKind
	zval_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10144
	case 42: {
	//AddAndReplaceKind
	zval_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10145
	case 43: {
	//AddAndReplaceKind
	zval_refcount_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10146
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10147
	case 45: {
	//GuardKind
	if (!__is_neg("111-45", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10148
	case 46: {
	//ReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	}
	}
	}
		} else {
			tmp = *p;
		}
	
	break;
	}
	#ifdef COMPILE_10149
	case 1: {
	//AddAndReplaceKind
	_convert_to_string(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10150
	case 2: {
	//AddAndReplaceKind
	_object_init(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10151
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10152
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10153
	case 5: {
	//AddAndReplaceKind
	_zval_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10154
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10155
	case 7: {
	//AddAndReplaceKind
	_zval_dtor_wrapper(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10156
	case 8: {
	//AddAndReplaceKind
	_zval_internal_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10157
	case 9: {
	//AddAndReplaceKind
	add_next_index_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10158
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10159
	case 11: {
	//AddAndReplaceKind
	convert_to_array(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10160
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10161
	case 13: {
	//AddAndReplaceKind
	convert_to_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10162
	case 14: {
	//AddAndReplaceKind
	convert_to_long(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10163
	case 15: {
	//AddAndReplaceKind
	convert_to_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10164
	case 16: {
	//AddAndReplaceKind
	convert_to_object(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10165
	case 17: {
	//AddAndReplaceKind
	decrement_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10166
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10167
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10168
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10169
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10170
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10171
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10172
	case 24: {
	//AddAndReplaceKind
	increment_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10173
	case 25: {
	//AddAndReplaceKind
	lex_scan(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10174
	case 26: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10175
	case 27: {
	//AddAndReplaceKind
	zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10176
	case 28: {
	//AddAndReplaceKind
	zend_locale_sprintf_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10177
	case 29: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10178
	case 30: {
	//AddAndReplaceKind
	zend_objects_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10179
	case 31: {
	//AddAndReplaceKind
	zend_objects_store_add_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10180
	case 32: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10181
	case 33: {
	//AddAndReplaceKind
	zend_objects_store_del_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10182
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10183
	case 35: {
	//AddAndReplaceKind
	zend_print_flat_zval_r(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10184
	case 36: {
	//AddAndReplaceKind
	zend_print_variable(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10185
	case 37: {
	//AddAndReplaceKind
	zend_std_get_constructor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10186
	case 38: {
	//AddAndReplaceKind
	zend_std_get_properties(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10187
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10188
	case 40: {
	//AddAndReplaceKind
	zval_delref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10189
	case 41: {
	//AddAndReplaceKind
	zval_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10190
	case 42: {
	//AddAndReplaceKind
	zval_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10191
	case 43: {
	//AddAndReplaceKind
	zval_refcount_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10192
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10193
	case 45: {
	//GuardKind
	if (!__is_neg("112-45", 6, &(tmp), sizeof (tmp), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(key), sizeof (key), &(target), sizeof (target), &(args), sizeof (args)))
	    zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10194
	case 46: {
	//ReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	}
	}
	}
	//prophet generated patch
	{
	switch(__choose("__SWITCH113"))
	{
	case 0: 
	break;
	#ifdef COMPILE_10195
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_10196
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_10197
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_10198
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_10199
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_10200
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10201
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_10202
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_10203
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10204
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10205
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10206
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10207
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10208
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_10209
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_10210
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10211
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10212
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10213
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10214
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10215
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_10216
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10217
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10218
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10219
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10220
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_10221
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_10222
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_10223
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_10224
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_10225
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_10226
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_10227
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_10228
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_10229
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_10230
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_10231
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_10232
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10233
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10234
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10235
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10236
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_10237
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10238
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10239
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_10240
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_10241
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10242
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10243
	case 49: {
	//AddInitKind
	memset(key, 0, sizeof (*(key)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10244
	case 50: {
	//IfExitKind
	if (__is_neg("113-50", 9, &(key), sizeof (key), &(target), sizeof (target), &(tmp), sizeof (tmp), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_10245
	case 51: {
	//IfExitKind
	if (__is_neg("113-51", 9, &(key), sizeof (key), &(target), sizeof (target), &(tmp), sizeof (tmp), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_10246
	case 52: {
	//IfExitKind
	if (__is_neg("113-52", 9, &(key), sizeof (key), &(target), sizeof (target), &(tmp), sizeof (tmp), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_10247
	case 53: {
	//IfExitKind
	if (__is_neg("113-53", 9, &(key), sizeof (key), &(target), sizeof (target), &(tmp), sizeof (tmp), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_10248
	case 54: {
	//IfExitKind
	if (__is_neg("113-54", 9, &(key), sizeof (key), &(target), sizeof (target), &(tmp), sizeof (tmp), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 64;
	
	break;
	}
	#endif
	}
	{
	long long __temp114=(_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) ;
	switch(__choose("__SWITCH114"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_10249
	case 1: {
	__temp114= ((_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) || __is_neg("114-1", 9, &(key), sizeof (key), &(target), sizeof (target), &(tmp), sizeof (tmp), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10250
	case 2: {
	__temp114= ((_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) && !__is_neg("114-2", 9, &(key), sizeof (key), &(target), sizeof (target), &(tmp), sizeof (tmp), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH117"))
	{
	case 0: {
	if (__temp114) {
			{
	switch(__choose("__SWITCH115"))
	{
	case 0: 
	break;
	#ifdef COMPILE_10251
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_10252
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_10253
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_10254
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_10255
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_10256
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10257
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_10258
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_10259
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10260
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10261
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10262
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10263
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10264
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_10265
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_10266
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10267
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10268
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10269
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10270
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10271
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_10272
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10273
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10274
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10275
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10276
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_10277
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_10278
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_10279
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_10280
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_10281
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_10282
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_10283
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_10284
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_10285
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_10286
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_10287
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_10288
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10289
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10290
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10291
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10292
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_10293
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10294
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10295
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_10296
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_10297
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10298
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10299
	case 49: {
	//IfExitKind
	if (__is_neg("115-49", 9, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_10300
	case 50: {
	//IfExitKind
	if (__is_neg("115-50", 9, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_10301
	case 51: {
	//IfExitKind
	if (__is_neg("115-51", 9, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_10302
	case 52: {
	//IfExitKind
	if (__is_neg("115-52", 9, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_10303
	case 53: {
	//IfExitKind
	if (__is_neg("115-53", 9, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 64;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH116"))
	{
	case 0: {
	Z_ADDREF_P(tmp);
	
	break;
	}
	#ifdef COMPILE_10304
	case 1: {
	//AddAndReplaceKind
	_convert_to_string(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10305
	case 2: {
	//AddAndReplaceKind
	_object_init(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10306
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10307
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10308
	case 5: {
	//AddAndReplaceKind
	_zval_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10309
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10310
	case 7: {
	//AddAndReplaceKind
	_zval_dtor_wrapper(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10311
	case 8: {
	//AddAndReplaceKind
	_zval_internal_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10312
	case 9: {
	//AddAndReplaceKind
	add_next_index_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10313
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10314
	case 11: {
	//AddAndReplaceKind
	convert_to_array(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10315
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10316
	case 13: {
	//AddAndReplaceKind
	convert_to_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10317
	case 14: {
	//AddAndReplaceKind
	convert_to_long(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10318
	case 15: {
	//AddAndReplaceKind
	convert_to_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10319
	case 16: {
	//AddAndReplaceKind
	convert_to_object(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10320
	case 17: {
	//AddAndReplaceKind
	decrement_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10321
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10322
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10323
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10324
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10325
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10326
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10327
	case 24: {
	//AddAndReplaceKind
	increment_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10328
	case 25: {
	//AddAndReplaceKind
	lex_scan(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10329
	case 26: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10330
	case 27: {
	//AddAndReplaceKind
	zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10331
	case 28: {
	//AddAndReplaceKind
	zend_locale_sprintf_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10332
	case 29: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10333
	case 30: {
	//AddAndReplaceKind
	zend_objects_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10334
	case 31: {
	//AddAndReplaceKind
	zend_objects_store_add_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10335
	case 32: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10336
	case 33: {
	//AddAndReplaceKind
	zend_objects_store_del_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10337
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10338
	case 35: {
	//AddAndReplaceKind
	zend_print_flat_zval_r(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10339
	case 36: {
	//AddAndReplaceKind
	zend_print_variable(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10340
	case 37: {
	//AddAndReplaceKind
	zend_std_get_constructor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10341
	case 38: {
	//AddAndReplaceKind
	zend_std_get_properties(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10342
	case 39: {
	//AddAndReplaceKind
	zval_delref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10343
	case 40: {
	//AddAndReplaceKind
	zval_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10344
	case 41: {
	//AddAndReplaceKind
	zval_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10345
	case 42: {
	//AddAndReplaceKind
	zval_refcount_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10346
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10347
	case 44: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10348
	case 45: {
	//GuardKind
	if (!__is_neg("116-45", 9, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10349
	case 46: {
	//ReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	}
	}
		}
	
	break;
	}
	#ifdef COMPILE_10350
	case 1: {
	//AddAndReplaceKind
	_convert_to_string(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10351
	case 2: {
	//AddAndReplaceKind
	_object_init(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10352
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10353
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10354
	case 5: {
	//AddAndReplaceKind
	_zval_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10355
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_func(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10356
	case 7: {
	//AddAndReplaceKind
	_zval_dtor_wrapper(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10357
	case 8: {
	//AddAndReplaceKind
	_zval_internal_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10358
	case 9: {
	//AddAndReplaceKind
	add_next_index_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10359
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10360
	case 11: {
	//AddAndReplaceKind
	convert_to_array(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10361
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10362
	case 13: {
	//AddAndReplaceKind
	convert_to_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10363
	case 14: {
	//AddAndReplaceKind
	convert_to_long(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10364
	case 15: {
	//AddAndReplaceKind
	convert_to_null(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10365
	case 16: {
	//AddAndReplaceKind
	convert_to_object(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10366
	case 17: {
	//AddAndReplaceKind
	decrement_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10367
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10368
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10369
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10370
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10371
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10372
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10373
	case 24: {
	//AddAndReplaceKind
	increment_function(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10374
	case 25: {
	//AddAndReplaceKind
	lex_scan(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10375
	case 26: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10376
	case 27: {
	//AddAndReplaceKind
	zend_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10377
	case 28: {
	//AddAndReplaceKind
	zend_locale_sprintf_double(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10378
	case 29: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10379
	case 30: {
	//AddAndReplaceKind
	zend_objects_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10380
	case 31: {
	//AddAndReplaceKind
	zend_objects_store_add_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10381
	case 32: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10382
	case 33: {
	//AddAndReplaceKind
	zend_objects_store_del_ref(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10383
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10384
	case 35: {
	//AddAndReplaceKind
	zend_print_flat_zval_r(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10385
	case 36: {
	//AddAndReplaceKind
	zend_print_variable(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10386
	case 37: {
	//AddAndReplaceKind
	zend_std_get_constructor(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10387
	case 38: {
	//AddAndReplaceKind
	zend_std_get_properties(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10388
	case 39: {
	//AddAndReplaceKind
	zval_delref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10389
	case 40: {
	//AddAndReplaceKind
	zval_is_true(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10390
	case 41: {
	//AddAndReplaceKind
	zval_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10391
	case 42: {
	//AddAndReplaceKind
	zval_refcount_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10392
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10393
	case 44: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10394
	case 45: {
	//GuardKind
	if (!__is_neg("117-45", 9, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(key->arKey), sizeof (key->arKey), &(key->nKeyLength), sizeof (key->nKeyLength), &(key->h), sizeof (key->h), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10395
	case 46: {
	//ReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	}
	}
	}
	//prophet generated patch
	{
	switch(__choose("__SWITCH118"))
	{
	case 0: 
	break;
	#ifdef COMPILE_10396
	case 1: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_10397
	case 2: {
	//AddAndReplaceKind
	(tmp) = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_10398
	case 3: {
	//AddAndReplaceKind
	(tmp) = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_10399
	case 4: {
	//AddAndReplaceKind
	*(tmp) = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_10400
	case 5: {
	//AddAndReplaceKind
	*tmp = zval_used_for_init;
	
	break;
	}
	#endif
	#ifdef COMPILE_10401
	case 6: {
	//AddAndReplaceKind
	_efree(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10402
	case 7: {
	//AddAndReplaceKind
	_efree(((tmp)->value.ht));
	
	break;
	}
	#endif
	#ifdef COMPILE_10403
	case 8: {
	//AddAndReplaceKind
	_efree(((tmp)->value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_10404
	case 9: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10405
	case 10: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10406
	case 11: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10407
	case 12: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10408
	case 13: {
	//AddAndReplaceKind
	_zend_hash_quick_add_or_update((executor_globals.active_symbol_table), key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
	
	break;
	}
	#endif
	#ifdef COMPILE_10409
	case 14: {
	//AddAndReplaceKind
	_zend_list_addref((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_10410
	case 15: {
	//AddAndReplaceKind
	_zend_list_delete((tmp)->value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_10411
	case 16: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10412
	case 17: {
	//AddAndReplaceKind
	_zval_dtor(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10413
	case 18: {
	//AddAndReplaceKind
	_zval_dtor_func((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10414
	case 19: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10415
	case 20: {
	//AddAndReplaceKind
	convert_to_boolean((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10416
	case 21: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_10417
	case 22: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10418
	case 23: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((tmp)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10419
	case 24: {
	//AddAndReplaceKind
	gc_zval_possible_root((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10420
	case 25: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10421
	case 26: {
	//AddAndReplaceKind
	if (!(executor_globals.active_symbol_table)) {
	    zend_rebuild_symbol_table();
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_10422
	case 27: {
	//AddAndReplaceKind
	if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength, key->h, &tmp, sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
	    zval_addref_p(tmp);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_10423
	case 28: {
	//AddAndReplaceKind
	is_ref = (**(p)).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_10424
	case 29: {
	//AddAndReplaceKind
	is_ref = (**p).type & 64;
	
	break;
	}
	#endif
	#ifdef COMPILE_10425
	case 30: {
	//AddAndReplaceKind
	tmp = (executor_globals.uninitialized_zval_ptr);
	
	break;
	}
	#endif
	#ifdef COMPILE_10426
	case 31: {
	//AddAndReplaceKind
	tmp = *(p);
	
	break;
	}
	#endif
	#ifdef COMPILE_10427
	case 32: {
	//AddAndReplaceKind
	tmp = *p;
	
	break;
	}
	#endif
	#ifdef COMPILE_10428
	case 33: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_10429
	case 34: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_10430
	case 35: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
	
	break;
	}
	#endif
	#ifdef COMPILE_10431
	case 36: {
	//AddAndReplaceKind
	zend_hash_destroy((tmp)->value.ht);
	
	break;
	}
	#endif
	#ifdef COMPILE_10432
	case 37: {
	//AddAndReplaceKind
	zend_rebuild_symbol_table();
	
	break;
	}
	#endif
	#ifdef COMPILE_10433
	case 38: {
	//AddAndReplaceKind
	zval_addref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10434
	case 39: {
	//AddAndReplaceKind
	zval_addref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10435
	case 40: {
	//AddAndReplaceKind
	zval_addref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10436
	case 41: {
	//AddAndReplaceKind
	zval_set_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10437
	case 42: {
	//AddAndReplaceKind
	zval_set_isref_p(*(((p))));
	
	break;
	}
	#endif
	#ifdef COMPILE_10438
	case 43: {
	//AddAndReplaceKind
	zval_set_isref_p(*((p)));
	
	break;
	}
	#endif
	#ifdef COMPILE_10439
	case 44: {
	//AddAndReplaceKind
	zval_set_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10440
	case 45: {
	//AddAndReplaceKind
	zval_set_refcount_p((tmp), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_10441
	case 46: {
	//AddAndReplaceKind
	zval_set_refcount_p(tmp, 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_10442
	case 47: {
	//AddAndReplaceKind
	zval_unset_isref_p((tmp));
	
	break;
	}
	#endif
	#ifdef COMPILE_10443
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(tmp);
	
	break;
	}
	#endif
	#ifdef COMPILE_10444
	case 49: {
	//IfExitKind
	if (__is_neg("118-49", 6, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_10445
	case 50: {
	//IfExitKind
	if (__is_neg("118-50", 6, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_10446
	case 51: {
	//IfExitKind
	if (__is_neg("118-51", 6, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 32;
	
	break;
	}
	#endif
	#ifdef COMPILE_10447
	case 52: {
	//IfExitKind
	if (__is_neg("118-52", 6, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 3;
	
	break;
	}
	#endif
	#ifdef COMPILE_10448
	case 53: {
	//IfExitKind
	if (__is_neg("118-53", 6, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 64;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH119"))
	{
	case 0: {
	return ZEND_HASH_APPLY_KEEP;
	
	break;
	}
	#ifdef COMPILE_10449
	case 1: {
	//GuardKind
	if (!__is_neg("119-1", 6, &(tmp), sizeof (tmp), &(key), sizeof (key), &(target), sizeof (target), &(p), sizeof (p), &(is_ref), sizeof (is_ref), &(args), sizeof (args)))
	    return 0;
	
	break;
	}
	#endif
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
