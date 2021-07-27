int __get_mutant(); int __is_neg(const char *location,int count, ...); int __abst_hole(); int __choose(const char *);
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
   | Authors: Christian Seiler <chris_se@gmx.net>                         |
   |          Dmitry Stogov <dmitry@zend.com>                             |
   |          Marcus Boerger <helly@php.net>                              |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "zend.h"
#include "zend_API.h"
#include "zend_closures.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_objects.h"
#include "zend_objects_API.h"
#include "zend_globals.h"

#define ZEND_CLOSURE_PRINT_NAME "Closure object"

#define ZEND_CLOSURE_PROPERTY_ERROR() \
	zend_error(E_RECOVERABLE_ERROR, "Closure object cannot have properties")

typedef struct _zend_closure {
	zend_object    std;
	zend_function  func;
	zval          *this_ptr;
	HashTable     *debug_info;
} zend_closure;

/* non-static since it needs to be referenced */
ZEND_API zend_class_entry *zend_ce_closure;
static zend_object_handlers closure_handlers;

ZEND_METHOD(Closure, __invoke) /* {{{ */
{
	zend_function *func = EG(current_execute_data)->function_state.function;
	zval ***arguments;
	zval *closure_result_ptr = NULL;

	arguments = emalloc(sizeof(zval**) * ZEND_NUM_ARGS());
	if (zend_get_parameters_array_ex(ZEND_NUM_ARGS(), arguments) == FAILURE) {
		efree(arguments);
		zend_error(E_RECOVERABLE_ERROR, "Cannot get arguments for calling closure");
		RETVAL_FALSE;
	} else if (call_user_function_ex(CG(function_table), NULL, this_ptr, &closure_result_ptr, ZEND_NUM_ARGS(), arguments, 1, NULL TSRMLS_CC) == FAILURE) {
		RETVAL_FALSE;
	} else if (closure_result_ptr) {
		if (Z_ISREF_P(closure_result_ptr) && return_value_ptr) {
			if (return_value) {
				zval_ptr_dtor(&return_value);
			}
			*return_value_ptr = closure_result_ptr;
		} else {
			RETVAL_ZVAL(closure_result_ptr, 1, 1);
		}
	}
	efree(arguments);

	/* destruct the function also, then - we have allocated it in get_method */
	efree(func->internal_function.function_name);
	efree(func);
}
/* }}} */

/* {{{ proto Closure Closure::bindTo(object $to)
   Bind a closure to another object */
ZEND_METHOD(Closure, bindTo) /* {{{ */
{
	zval *newthis;
	zend_closure *closure = (zend_closure *)zend_object_store_get_object(getThis() TSRMLS_CC);	

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o!", &newthis) == FAILURE) {
		RETURN_NULL();
	}

	zend_create_closure(return_value, &closure->func, newthis?Z_OBJCE_P(newthis):NULL, newthis TSRMLS_CC);
}
/* }}} */

/* {{{ proto Closure Closure::bind(object $to, Closure $old)
   Create a closure to with binding to another object */
ZEND_METHOD(Closure, bind) /* {{{ */
{
	zval *newthis, *zclosure;
	zend_closure *closure;	

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o!O", &newthis, &zclosure, zend_ce_closure) == FAILURE) {
		RETURN_NULL();
	}

	closure = (zend_closure *)zend_object_store_get_object(zclosure TSRMLS_CC);	

	zend_create_closure(return_value, &closure->func, newthis?Z_OBJCE_P(newthis):NULL, newthis TSRMLS_CC);
}
/* }}} */


static zend_function *zend_closure_get_constructor(zval *object TSRMLS_DC) /* {{{ */
{
	zend_error(E_RECOVERABLE_ERROR, "Instantiation of 'Closure' is not allowed");
	return NULL;
}
/* }}} */

static int zend_closure_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
	return (Z_OBJ_HANDLE_P(o1) != Z_OBJ_HANDLE_P(o2));
}
/* }}} */

ZEND_API zend_function *zend_get_closure_invoke_method(zval *obj TSRMLS_DC) /* {{{ */
{
	zend_closure *closure = (zend_closure *)zend_object_store_get_object(obj TSRMLS_CC);	
	zend_function *invoke = (zend_function*)emalloc(sizeof(zend_function));

	invoke->common = closure->func.common;
	invoke->type = ZEND_INTERNAL_FUNCTION;
	invoke->internal_function.fn_flags = ZEND_ACC_PUBLIC | ZEND_ACC_CALL_VIA_HANDLER | (closure->func.common.fn_flags & ZEND_ACC_RETURN_REFERENCE);
	invoke->internal_function.handler = ZEND_MN(Closure___invoke);
	invoke->internal_function.module = 0;
	invoke->internal_function.scope = zend_ce_closure;
	invoke->internal_function.function_name = estrndup(ZEND_INVOKE_FUNC_NAME, sizeof(ZEND_INVOKE_FUNC_NAME)-1);
	return invoke;
}
/* }}} */

ZEND_API const zend_function *zend_get_closure_method_def(zval *obj TSRMLS_DC) /* {{{ */
{
	zend_closure *closure = (zend_closure *)zend_object_store_get_object(obj TSRMLS_CC);	
	return &closure->func;
}
/* }}} */

ZEND_API zval* zend_get_closure_this_ptr(zval *obj TSRMLS_DC) /* {{{ */
{
	zend_closure *closure = (zend_closure *)zend_object_store_get_object(obj TSRMLS_CC);	
	return closure->this_ptr;
}
/* }}} */

static zend_function *zend_closure_get_method(zval **object_ptr, char *method_name, int method_len, const zend_literal *key TSRMLS_DC) /* {{{ */
{
	char *lc_name;
	ALLOCA_FLAG(use_heap)

	lc_name = do_alloca(method_len + 1, use_heap);
	zend_str_tolower_copy(lc_name, method_name, method_len);
	if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME)-1) &&
		memcmp(lc_name, ZEND_INVOKE_FUNC_NAME, sizeof(ZEND_INVOKE_FUNC_NAME)-1) == 0
	) {
		free_alloca(lc_name, use_heap);
		return zend_get_closure_invoke_method(*object_ptr TSRMLS_CC);
	}
	free_alloca(lc_name, use_heap);
	return std_object_handlers.get_method(object_ptr, method_name, method_len, key TSRMLS_CC);
}
/* }}} */

static zval *zend_closure_read_property(zval *object, zval *member, int type, const zend_literal *key TSRMLS_DC) /* {{{ */
{
	ZEND_CLOSURE_PROPERTY_ERROR();
	Z_ADDREF(EG(uninitialized_zval));
	return &EG(uninitialized_zval);
}
/* }}} */

static void zend_closure_write_property(zval *object, zval *member, zval *value, const zend_literal *key TSRMLS_DC) /* {{{ */
{
	ZEND_CLOSURE_PROPERTY_ERROR();
}
/* }}} */

static zval **zend_closure_get_property_ptr_ptr(zval *object, zval *member, const zend_literal *key TSRMLS_DC) /* {{{ */
{
	ZEND_CLOSURE_PROPERTY_ERROR();
	return NULL;
}
/* }}} */

static int zend_closure_has_property(zval *object, zval *member, int has_set_exists, const zend_literal *key TSRMLS_DC) /* {{{ */
{
	if (has_set_exists != 2) {
		ZEND_CLOSURE_PROPERTY_ERROR();
	}
	return 0;
}
/* }}} */

static void zend_closure_unset_property(zval *object, zval *member, const zend_literal *key TSRMLS_DC) /* {{{ */
{
	ZEND_CLOSURE_PROPERTY_ERROR();
}
/* }}} */

static void zend_closure_free_storage(void *object TSRMLS_DC) /* {{{ */
{
	zend_closure *closure = (zend_closure *)object;

	zend_object_std_dtor(&closure->std TSRMLS_CC);

	if (closure->func.type == ZEND_USER_FUNCTION) {
		zend_execute_data *ex = EG(current_execute_data);
		while (ex) {
			if (ex->op_array == &closure->func.op_array) {
				zend_error(E_ERROR, "Cannot destroy active lambda function");
			}
			ex = ex->prev_execute_data;
		}
		destroy_op_array(&closure->func.op_array TSRMLS_CC);
	}

	if (closure->debug_info != NULL) {
		zend_hash_destroy(closure->debug_info);
		efree(closure->debug_info);
	}

	if (closure->this_ptr) {
		zval_ptr_dtor(&closure->this_ptr);
	}

	efree(closure);
}
/* }}} */

static zend_object_value zend_closure_new(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_closure *closure;
	zend_object_value object;

	closure = emalloc(sizeof(zend_closure));
	memset(closure, 0, sizeof(zend_closure));

	zend_object_std_init(&closure->std, class_type TSRMLS_CC);

	object.handle = zend_objects_store_put(closure, (zend_objects_store_dtor_t)zend_objects_destroy_object, (zend_objects_free_object_storage_t) zend_closure_free_storage, NULL TSRMLS_CC);
	object.handlers = &closure_handlers;

	return object;
}
/* }}} */

static zend_object_value zend_closure_clone(zval *zobject TSRMLS_DC) /* {{{ */
{
	zend_closure *closure = (zend_closure *)zend_object_store_get_object(zobject TSRMLS_CC);
	zval result;

	zend_create_closure(&result, &closure->func, closure->func.common.scope, closure->this_ptr TSRMLS_CC);
	return Z_OBJVAL(result);
}
/* }}} */


int zend_closure_get_closure(zval *obj, zend_class_entry **ce_ptr, zend_function **fptr_ptr, zval **zobj_ptr TSRMLS_DC) /* {{{ */
{
	zend_closure *closure;

	if (Z_TYPE_P(obj) != IS_OBJECT) {
		return FAILURE;
	}

	closure = (zend_closure *)zend_object_store_get_object(obj TSRMLS_CC);
	*fptr_ptr = &closure->func;

	if (closure->this_ptr) {
		if (zobj_ptr) {
			*zobj_ptr = closure->this_ptr;
		}
		*ce_ptr = Z_OBJCE_P(closure->this_ptr);
	} else {
		if (zobj_ptr) {
			*zobj_ptr = NULL;
		}
		*ce_ptr = closure->func.common.scope;
	}
	return SUCCESS;
}
/* }}} */

static HashTable *zend_closure_get_debug_info(zval *object, int *is_temp TSRMLS_DC) /* {{{ */
{
	zend_closure *closure = (zend_closure *)zend_object_store_get_object(object TSRMLS_CC);
	zval *val;
	struct _zend_arg_info *arg_info = closure->func.common.arg_info;

	*is_temp = 0;

	if (closure->debug_info == NULL) {
		ALLOC_HASHTABLE(closure->debug_info);
		zend_hash_init(closure->debug_info, 1, NULL, ZVAL_PTR_DTOR, 0);
	}
	if (closure->debug_info->nApplyCount == 0) {
		if (closure->func.type == ZEND_USER_FUNCTION && closure->func.op_array.static_variables) {
			HashTable *static_variables = closure->func.op_array.static_variables;
			MAKE_STD_ZVAL(val);
			array_init(val);
			zend_hash_copy(Z_ARRVAL_P(val), static_variables, (copy_ctor_func_t)zval_add_ref, NULL, sizeof(zval*));
			zend_hash_update(closure->debug_info, "static", sizeof("static"), (void *) &val, sizeof(zval *), NULL);
		}

		if (closure->this_ptr) {
			Z_ADDREF_P(closure->this_ptr);
			zend_symtable_update(closure->debug_info, "this", sizeof("this"), (void *) &closure->this_ptr, sizeof(zval *), NULL);
		}

		if (arg_info) {
			zend_uint i, required = closure->func.common.required_num_args;

			MAKE_STD_ZVAL(val);
			array_init(val);

			for (i = 0; i < closure->func.common.num_args; i++) {
				char *name, *info;
				int name_len, info_len;
				if (arg_info->name) {
					name_len = zend_spprintf(&name, 0, "%s$%s",
									arg_info->pass_by_reference ? "&" : "",
									arg_info->name);
				} else {
					name_len = zend_spprintf(&name, 0, "%s$param%d",
									arg_info->pass_by_reference ? "&" : "",
									i + 1);
				}
				info_len = zend_spprintf(&info, 0, "%s",
								i >= required ? "<optional>" : "<required>");
				add_assoc_stringl_ex(val, name, name_len + 1, info, info_len, 0);
				efree(name);
				arg_info++;
			}
			zend_hash_update(closure->debug_info, "parameter", sizeof("parameter"), (void *) &val, sizeof(zval *), NULL);
		}
	}

	return closure->debug_info;
}
/* }}} */

/* {{{ proto Closure::__construct()
   Private constructor preventing instantiation */
ZEND_METHOD(Closure, __construct)
{
	zend_error(E_RECOVERABLE_ERROR, "Instantiation of 'Closure' is not allowed");
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_bindto, 0, 0, 0)
	ZEND_ARG_INFO(0, newthis)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_closure_bind, 0, 0, 0)
	ZEND_ARG_INFO(0, newthis)
	ZEND_ARG_INFO(0, closure)
ZEND_END_ARG_INFO()

static const zend_function_entry closure_functions[] = {
	ZEND_ME(Closure, __construct, NULL, ZEND_ACC_PRIVATE)
	ZEND_ME(Closure, bindTo, arginfo_closure_bindto, ZEND_ACC_PUBLIC)
	ZEND_ME(Closure, bind, arginfo_closure_bind, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	{NULL, NULL, NULL}
};

void zend_register_closure_ce(TSRMLS_D) /* {{{ */
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Closure", closure_functions);
	zend_ce_closure = zend_register_internal_class(&ce TSRMLS_CC);
	zend_ce_closure->ce_flags |= ZEND_ACC_FINAL_CLASS;
	zend_ce_closure->create_object = zend_closure_new;
	zend_ce_closure->serialize = zend_class_serialize_deny;
	zend_ce_closure->unserialize = zend_class_unserialize_deny;

	memcpy(&closure_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	closure_handlers.get_constructor = zend_closure_get_constructor;
	closure_handlers.get_method = zend_closure_get_method;
	closure_handlers.write_property = zend_closure_write_property;
	closure_handlers.read_property = zend_closure_read_property;
	closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
	closure_handlers.has_property = zend_closure_has_property;
	closure_handlers.unset_property = zend_closure_unset_property;
	closure_handlers.compare_objects = zend_closure_compare_objects;
	closure_handlers.clone_obj = zend_closure_clone;
	closure_handlers.get_debug_info = zend_closure_get_debug_info;
	closure_handlers.get_closure = zend_closure_get_closure;
}
/* }}} */

ZEND_API void zend_create_closure(zval *res, zend_function *func, zend_class_entry *scope, zval *this_ptr TSRMLS_DC) /* {{{ */
{
	zend_closure *closure;

	object_init_ex(res, zend_ce_closure);

	closure = (zend_closure *)zend_object_store_get_object(res TSRMLS_CC);

	closure->func = *func;

	if (closure->func.type == ZEND_USER_FUNCTION) {
		//prophet generated patch
		{
		switch(__choose("__SWITCH0"))
		{
		case 0: 
		break;
		#ifdef COMPILE_0
		case 1: {
		//AddAndReplaceKind
		((closure)->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_1
		case 2: {
		//AddAndReplaceKind
		(*(closure)->func.op_array.refcount)++;
		
		break;
		}
		#endif
		#ifdef COMPILE_2
		case 3: {
		//AddAndReplaceKind
		(*closure->func.op_array.refcount)++;
		
		break;
		}
		#endif
		#ifdef COMPILE_3
		case 4: {
		//AddAndReplaceKind
		(closure) = (zend_closure *)zend_object_store_get_object(res);
		
		break;
		}
		#endif
		#ifdef COMPILE_4
		case 5: {
		//AddAndReplaceKind
		(closure)->func = *func;
		
		break;
		}
		#endif
		#ifdef COMPILE_5
		case 6: {
		//AddAndReplaceKind
		(closure)->func.common.scope = scope;
		
		break;
		}
		#endif
		#ifdef COMPILE_6
		case 7: {
		//AddAndReplaceKind
		(closure)->func.op_array.run_time_cache = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_7
		case 8: {
		//AddAndReplaceKind
		(closure)->this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_8
		case 9: {
		//AddAndReplaceKind
		(closure)->this_ptr = this_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_9
		case 10: {
		//AddAndReplaceKind
		(closure->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_10
		case 11: {
		//AddAndReplaceKind
		(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
		
		break;
		}
		#endif
		#ifdef COMPILE_11
		case 12: {
		//AddAndReplaceKind
		(res) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_12
		case 13: {
		//AddAndReplaceKind
		(scope) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_13
		case 14: {
		//AddAndReplaceKind
		(scope)->create_object = zend_closure_new;
		
		break;
		}
		#endif
		#ifdef COMPILE_14
		case 15: {
		//AddAndReplaceKind
		(scope)->serialize = zend_class_serialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_15
		case 16: {
		//AddAndReplaceKind
		(scope)->unserialize = zend_class_unserialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_16
		case 17: {
		//AddAndReplaceKind
		(this_ptr) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_17
		case 18: {
		//AddAndReplaceKind
		_array_init(((res)), 0);
		
		break;
		}
		#endif
		#ifdef COMPILE_18
		case 19: {
		//AddAndReplaceKind
		_array_init(((this_ptr)), 0);
		
		break;
		}
		#endif
		#ifdef COMPILE_19
		case 20: {
		//AddAndReplaceKind
		_efree(((closure)));
		
		break;
		}
		#endif
		#ifdef COMPILE_20
		case 21: {
		//AddAndReplaceKind
		_efree(((closure)->debug_info));
		
		break;
		}
		#endif
		#ifdef COMPILE_21
		case 22: {
		//AddAndReplaceKind
		_efree(((func)));
		
		break;
		}
		#endif
		#ifdef COMPILE_22
		case 23: {
		//AddAndReplaceKind
		_efree(((func)->internal_function.function_name));
		
		break;
		}
		#endif
		#ifdef COMPILE_23
		case 24: {
		//AddAndReplaceKind
		_efree(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_24
		case 25: {
		//AddAndReplaceKind
		_efree(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_25
		case 26: {
		//AddAndReplaceKind
		_object_init_ex(((res)), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_26
		case 27: {
		//AddAndReplaceKind
		_object_init_ex(((this_ptr)), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_27
		case 28: {
		//AddAndReplaceKind
		_object_init_ex((res), ((scope)));
		
		break;
		}
		#endif
		#ifdef COMPILE_28
		case 29: {
		//AddAndReplaceKind
		_object_init_ex((res), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_29
		case 30: {
		//AddAndReplaceKind
		_zend_hash_init(((closure)->debug_info), (1), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_30
		case 31: {
		//AddAndReplaceKind
		_zval_copy_ctor(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_31
		case 32: {
		//AddAndReplaceKind
		_zval_copy_ctor(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_32
		case 33: {
		//AddAndReplaceKind
		_zval_copy_ctor_func((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_33
		case 34: {
		//AddAndReplaceKind
		_zval_copy_ctor_func((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_34
		case 35: {
		//AddAndReplaceKind
		_zval_dtor(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_35
		case 36: {
		//AddAndReplaceKind
		_zval_dtor(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_36
		case 37: {
		//AddAndReplaceKind
		_zval_dtor_func((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_37
		case 38: {
		//AddAndReplaceKind
		_zval_dtor_func((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_38
		case 39: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(closure)->this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_39
		case 40: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_40
		case 41: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_41
		case 42: {
		//AddAndReplaceKind
		closure = (zend_closure *)zend_object_store_get_object(res);
		
		break;
		}
		#endif
		#ifdef COMPILE_42
		case 43: {
		//AddAndReplaceKind
		closure->func = *(func);
		
		break;
		}
		#endif
		#ifdef COMPILE_43
		case 44: {
		//AddAndReplaceKind
		closure->func = *func;
		
		break;
		}
		#endif
		#ifdef COMPILE_44
		case 45: {
		//AddAndReplaceKind
		closure->func.common.scope = (scope);
		
		break;
		}
		#endif
		#ifdef COMPILE_45
		case 46: {
		//AddAndReplaceKind
		closure->func.common.scope = scope;
		
		break;
		}
		#endif
		#ifdef COMPILE_46
		case 47: {
		//AddAndReplaceKind
		closure->func.op_array.run_time_cache = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_47
		case 48: {
		//AddAndReplaceKind
		closure->this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_48
		case 49: {
		//AddAndReplaceKind
		closure->this_ptr = (res);
		
		break;
		}
		#endif
		#ifdef COMPILE_49
		case 50: {
		//AddAndReplaceKind
		closure->this_ptr = (this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_50
		case 51: {
		//AddAndReplaceKind
		closure->this_ptr = this_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_51
		case 52: {
		//AddAndReplaceKind
		closure_handlers.clone_obj = zend_closure_clone;
		
		break;
		}
		#endif
		#ifdef COMPILE_52
		case 53: {
		//AddAndReplaceKind
		closure_handlers.compare_objects = zend_closure_compare_objects;
		
		break;
		}
		#endif
		#ifdef COMPILE_53
		case 54: {
		//AddAndReplaceKind
		closure_handlers.get_closure = zend_closure_get_closure;
		
		break;
		}
		#endif
		#ifdef COMPILE_54
		case 55: {
		//AddAndReplaceKind
		closure_handlers.get_constructor = zend_closure_get_constructor;
		
		break;
		}
		#endif
		#ifdef COMPILE_55
		case 56: {
		//AddAndReplaceKind
		closure_handlers.get_debug_info = zend_closure_get_debug_info;
		
		break;
		}
		#endif
		#ifdef COMPILE_56
		case 57: {
		//AddAndReplaceKind
		closure_handlers.get_method = zend_closure_get_method;
		
		break;
		}
		#endif
		#ifdef COMPILE_57
		case 58: {
		//AddAndReplaceKind
		closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_58
		case 59: {
		//AddAndReplaceKind
		closure_handlers.has_property = zend_closure_has_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_59
		case 60: {
		//AddAndReplaceKind
		closure_handlers.read_property = zend_closure_read_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_60
		case 61: {
		//AddAndReplaceKind
		closure_handlers.unset_property = zend_closure_unset_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_61
		case 62: {
		//AddAndReplaceKind
		closure_handlers.write_property = zend_closure_write_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_62
		case 63: {
		//AddAndReplaceKind
		convert_to_boolean((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_63
		case 64: {
		//AddAndReplaceKind
		convert_to_boolean((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_64
		case 65: {
		//AddAndReplaceKind
		destroy_op_array(&(closure)->func.op_array);
		
		break;
		}
		#endif
		#ifdef COMPILE_65
		case 66: {
		//AddAndReplaceKind
		exit(1);
		
		break;
		}
		#endif
		#ifdef COMPILE_66
		case 67: {
		//AddAndReplaceKind
		gc_remove_zval_from_buffer((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_67
		case 68: {
		//AddAndReplaceKind
		gc_remove_zval_from_buffer((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_68
		case 69: {
		//AddAndReplaceKind
		gc_zval_check_possible_root(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_69
		case 70: {
		//AddAndReplaceKind
		gc_zval_check_possible_root(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_70
		case 71: {
		//AddAndReplaceKind
		gc_zval_possible_root((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_71
		case 72: {
		//AddAndReplaceKind
		gc_zval_possible_root((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_72
		case 73: {
		//AddAndReplaceKind
		i_zval_ptr_dtor((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_73
		case 74: {
		//AddAndReplaceKind
		i_zval_ptr_dtor((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_74
		case 75: {
		//AddAndReplaceKind
		if (scope && !instanceof_function(scope, func->common.scope)) {
		    zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
		    scope = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_75
		case 76: {
		//AddAndReplaceKind
		if (scope && this_ptr && (func->common.fn_flags & 1) == 0 && !instanceof_function(zend_get_class_entry(&(*this_ptr)), closure->func.common.scope)) {
		    zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		    scope = ((void *)0);
		    this_ptr = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_76
		case 77: {
		//AddAndReplaceKind
		if (this_ptr && (closure->func.common.fn_flags & 1) == 0) {
		    closure->this_ptr = this_ptr;
		    zval_addref_p(this_ptr);
		} else {
		    closure->func.common.fn_flags |= 1;
		    closure->this_ptr = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_77
		case 78: {
		//AddAndReplaceKind
		memcpy(&closure_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
		
		break;
		}
		#endif
		#ifdef COMPILE_78
		case 79: {
		//AddAndReplaceKind
		memset((closure), 0, sizeof(zend_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_79
		case 80: {
		//AddAndReplaceKind
		scope = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_80
		case 81: {
		//AddAndReplaceKind
		this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_81
		case 82: {
		//AddAndReplaceKind
		zend_ce_closure->create_object = zend_closure_new;
		
		break;
		}
		#endif
		#ifdef COMPILE_82
		case 83: {
		//AddAndReplaceKind
		zend_ce_closure->serialize = zend_class_serialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_83
		case 84: {
		//AddAndReplaceKind
		zend_ce_closure->unserialize = zend_class_unserialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_84
		case 85: {
		//AddAndReplaceKind
		zend_error((1 << 0L), "Cannot destroy active lambda function");
		
		break;
		}
		#endif
		#ifdef COMPILE_85
		case 86: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Cannot get arguments for calling closure");
		
		break;
		}
		#endif
		#ifdef COMPILE_86
		case 87: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Closure object cannot have properties");
		
		break;
		}
		#endif
		#ifdef COMPILE_87
		case 88: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
		
		break;
		}
		#endif
		#ifdef COMPILE_88
		case 89: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", (func)->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_89
		case 90: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, (func)->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_90
		case 91: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(res)))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_91
		case 92: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(this_ptr)))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_92
		case 93: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_93
		case 94: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", (func)->common.scope->name, func->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_94
		case 95: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, (func)->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_95
		case 96: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, (scope)->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_96
		case 97: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_97
		case 98: {
		//AddAndReplaceKind
		zend_error((1 << 3L), "A non well formed numeric value encountered");
		
		break;
		}
		#endif
		#ifdef COMPILE_98
		case 99: {
		//AddAndReplaceKind
		zend_hash_destroy((closure)->debug_info);
		
		break;
		}
		#endif
		#ifdef COMPILE_99
		case 100: {
		//AddAndReplaceKind
		zend_object_std_dtor(&(closure)->std);
		
		break;
		}
		#endif
		#ifdef COMPILE_100
		case 101: {
		//AddAndReplaceKind
		zval_addref_p(&((executor_globals.uninitialized_zval)));
		
		break;
		}
		#endif
		#ifdef COMPILE_101
		case 102: {
		//AddAndReplaceKind
		zval_addref_p((closure)->this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_102
		case 103: {
		//AddAndReplaceKind
		zval_addref_p((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_103
		case 104: {
		//AddAndReplaceKind
		zval_addref_p((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_104
		case 105: {
		//AddAndReplaceKind
		zval_addref_p(this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_105
		case 106: {
		//AddAndReplaceKind
		zval_unset_isref_p((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_106
		case 107: {
		//AddAndReplaceKind
		zval_unset_isref_p((this_ptr));
		
		break;
		}
		#endif
		}
		{
		long long __temp1=(closure->func.op_array.static_variables) ;
		switch(__choose("__SWITCH1"))
		{
		case 0: {
		
		break;
		}
		#ifdef COMPILE_107
		case 1: {
		__temp1= ((closure->func.op_array.static_variables) && !__is_neg("1-1", 39, &(closure), sizeof (closure), &(closure->func.op_array.static_variables), sizeof (closure->func.op_array.static_variables), &(func), sizeof (func), &(res), sizeof (res), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(closure->func.op_array.run_time_cache), sizeof (closure->func.op_array.run_time_cache), &(closure->func.op_array.refcount), sizeof (closure->func.op_array.refcount), &(scope), sizeof (scope), &(this_ptr), sizeof (this_ptr), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->func.op_array.type), sizeof (closure->func.op_array.type), &(closure->func.op_array.function_name), sizeof (closure->func.op_array.function_name), &(closure->func.op_array.scope), sizeof (closure->func.op_array.scope), &(closure->func.op_array.fn_flags), sizeof (closure->func.op_array.fn_flags), &(closure->func.op_array.prototype), sizeof (closure->func.op_array.prototype), &(closure->func.op_array.num_args), sizeof (closure->func.op_array.num_args), &(closure->func.op_array.required_num_args), sizeof (closure->func.op_array.required_num_args), &(closure->func.op_array.arg_info), sizeof (closure->func.op_array.arg_info), &(closure->func.op_array.opcodes), sizeof (closure->func.op_array.opcodes), &(closure->func.op_array.last), sizeof (closure->func.op_array.last), &(closure->func.op_array.vars), sizeof (closure->func.op_array.vars), &(closure->func.op_array.last_var), sizeof (closure->func.op_array.last_var), &(closure->func.op_array.T), sizeof (closure->func.op_array.T), &(closure->func.op_array.brk_cont_array), sizeof (closure->func.op_array.brk_cont_array), &(closure->func.op_array.last_brk_cont), sizeof (closure->func.op_array.last_brk_cont), &(closure->func.op_array.try_catch_array), sizeof (closure->func.op_array.try_catch_array), &(closure->func.op_array.last_try_catch), sizeof (closure->func.op_array.last_try_catch), &(closure->func.op_array.this_var), sizeof (closure->func.op_array.this_var), &(closure->func.op_array.filename), sizeof (closure->func.op_array.filename), &(closure->func.op_array.line_start), sizeof (closure->func.op_array.line_start), &(closure->func.op_array.line_end), sizeof (closure->func.op_array.line_end), &(closure->func.op_array.doc_comment), sizeof (closure->func.op_array.doc_comment), &(closure->func.op_array.doc_comment_len), sizeof (closure->func.op_array.doc_comment_len), &(closure->func.op_array.early_binding), sizeof (closure->func.op_array.early_binding), &(closure->func.op_array.literals), sizeof (closure->func.op_array.literals), &(closure->func.op_array.last_literal), sizeof (closure->func.op_array.last_literal), &(closure->func.op_array.last_cache_slot), sizeof (closure->func.op_array.last_cache_slot), &(closure->debug_info), sizeof (closure->debug_info)));
		
		break;
		}
		#endif
		}
		switch(__choose("__SWITCH8"))
		{
		case 0: {
		if (__temp1) {
					HashTable *static_variables = closure->func.op_array.static_variables;
		
					{
		switch(__choose("__SWITCH2"))
		{
		case 0: 
		break;
		#ifdef COMPILE_108
		case 1: {
		//AddAndReplaceKind
		((closure)->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_109
		case 2: {
		//AddAndReplaceKind
		(*(closure)->func.op_array.refcount)++;
		
		break;
		}
		#endif
		#ifdef COMPILE_110
		case 3: {
		//AddAndReplaceKind
		(*closure->func.op_array.refcount)++;
		
		break;
		}
		#endif
		#ifdef COMPILE_111
		case 4: {
		//AddAndReplaceKind
		(closure) = (zend_closure *)zend_object_store_get_object(res);
		
		break;
		}
		#endif
		#ifdef COMPILE_112
		case 5: {
		//AddAndReplaceKind
		(closure)->func = *func;
		
		break;
		}
		#endif
		#ifdef COMPILE_113
		case 6: {
		//AddAndReplaceKind
		(closure)->func.common.scope = scope;
		
		break;
		}
		#endif
		#ifdef COMPILE_114
		case 7: {
		//AddAndReplaceKind
		(closure)->func.op_array.run_time_cache = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_115
		case 8: {
		//AddAndReplaceKind
		(closure)->this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_116
		case 9: {
		//AddAndReplaceKind
		(closure)->this_ptr = this_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_117
		case 10: {
		//AddAndReplaceKind
		(closure->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_118
		case 11: {
		//AddAndReplaceKind
		(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
		
		break;
		}
		#endif
		#ifdef COMPILE_119
		case 12: {
		//AddAndReplaceKind
		(res) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_120
		case 13: {
		//AddAndReplaceKind
		(scope) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_121
		case 14: {
		//AddAndReplaceKind
		(scope)->create_object = zend_closure_new;
		
		break;
		}
		#endif
		#ifdef COMPILE_122
		case 15: {
		//AddAndReplaceKind
		(scope)->serialize = zend_class_serialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_123
		case 16: {
		//AddAndReplaceKind
		(scope)->unserialize = zend_class_unserialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_124
		case 17: {
		//AddAndReplaceKind
		(this_ptr) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_125
		case 18: {
		//AddAndReplaceKind
		_array_init(((res)), 0);
		
		break;
		}
		#endif
		#ifdef COMPILE_126
		case 19: {
		//AddAndReplaceKind
		_array_init(((this_ptr)), 0);
		
		break;
		}
		#endif
		#ifdef COMPILE_127
		case 20: {
		//AddAndReplaceKind
		_efree(((closure)));
		
		break;
		}
		#endif
		#ifdef COMPILE_128
		case 21: {
		//AddAndReplaceKind
		_efree(((closure)->debug_info));
		
		break;
		}
		#endif
		#ifdef COMPILE_129
		case 22: {
		//AddAndReplaceKind
		_efree(((func)));
		
		break;
		}
		#endif
		#ifdef COMPILE_130
		case 23: {
		//AddAndReplaceKind
		_efree(((func)->internal_function.function_name));
		
		break;
		}
		#endif
		#ifdef COMPILE_131
		case 24: {
		//AddAndReplaceKind
		_efree(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_132
		case 25: {
		//AddAndReplaceKind
		_efree(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_133
		case 26: {
		//AddAndReplaceKind
		_object_init_ex(((res)), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_134
		case 27: {
		//AddAndReplaceKind
		_object_init_ex(((this_ptr)), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_135
		case 28: {
		//AddAndReplaceKind
		_object_init_ex((res), ((scope)));
		
		break;
		}
		#endif
		#ifdef COMPILE_136
		case 29: {
		//AddAndReplaceKind
		_object_init_ex((res), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_137
		case 30: {
		//AddAndReplaceKind
		_zend_hash_init(((closure)->debug_info), (1), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_138
		case 31: {
		//AddAndReplaceKind
		_zend_hash_init(((closure)->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_139
		case 32: {
		//AddAndReplaceKind
		_zend_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements((static_variables))), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_140
		case 33: {
		//AddAndReplaceKind
		_zend_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_141
		case 34: {
		//AddAndReplaceKind
		_zval_copy_ctor(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_142
		case 35: {
		//AddAndReplaceKind
		_zval_copy_ctor(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_143
		case 36: {
		//AddAndReplaceKind
		_zval_copy_ctor_func((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_144
		case 37: {
		//AddAndReplaceKind
		_zval_copy_ctor_func((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_145
		case 38: {
		//AddAndReplaceKind
		_zval_dtor(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_146
		case 39: {
		//AddAndReplaceKind
		_zval_dtor(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_147
		case 40: {
		//AddAndReplaceKind
		_zval_dtor_func((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_148
		case 41: {
		//AddAndReplaceKind
		_zval_dtor_func((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_149
		case 42: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(closure)->this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_150
		case 43: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_151
		case 44: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_152
		case 45: {
		//AddAndReplaceKind
		closure = (zend_closure *)zend_object_store_get_object(res);
		
		break;
		}
		#endif
		#ifdef COMPILE_153
		case 46: {
		//AddAndReplaceKind
		closure->func = *(func);
		
		break;
		}
		#endif
		#ifdef COMPILE_154
		case 47: {
		//AddAndReplaceKind
		closure->func = *func;
		
		break;
		}
		#endif
		#ifdef COMPILE_155
		case 48: {
		//AddAndReplaceKind
		closure->func.common.scope = (scope);
		
		break;
		}
		#endif
		#ifdef COMPILE_156
		case 49: {
		//AddAndReplaceKind
		closure->func.common.scope = scope;
		
		break;
		}
		#endif
		#ifdef COMPILE_157
		case 50: {
		//AddAndReplaceKind
		closure->func.op_array.run_time_cache = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_158
		case 51: {
		//AddAndReplaceKind
		closure->this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_159
		case 52: {
		//AddAndReplaceKind
		closure->this_ptr = (res);
		
		break;
		}
		#endif
		#ifdef COMPILE_160
		case 53: {
		//AddAndReplaceKind
		closure->this_ptr = (this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_161
		case 54: {
		//AddAndReplaceKind
		closure->this_ptr = this_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_162
		case 55: {
		//AddAndReplaceKind
		closure_handlers.clone_obj = zend_closure_clone;
		
		break;
		}
		#endif
		#ifdef COMPILE_163
		case 56: {
		//AddAndReplaceKind
		closure_handlers.compare_objects = zend_closure_compare_objects;
		
		break;
		}
		#endif
		#ifdef COMPILE_164
		case 57: {
		//AddAndReplaceKind
		closure_handlers.get_closure = zend_closure_get_closure;
		
		break;
		}
		#endif
		#ifdef COMPILE_165
		case 58: {
		//AddAndReplaceKind
		closure_handlers.get_constructor = zend_closure_get_constructor;
		
		break;
		}
		#endif
		#ifdef COMPILE_166
		case 59: {
		//AddAndReplaceKind
		closure_handlers.get_debug_info = zend_closure_get_debug_info;
		
		break;
		}
		#endif
		#ifdef COMPILE_167
		case 60: {
		//AddAndReplaceKind
		closure_handlers.get_method = zend_closure_get_method;
		
		break;
		}
		#endif
		#ifdef COMPILE_168
		case 61: {
		//AddAndReplaceKind
		closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_169
		case 62: {
		//AddAndReplaceKind
		closure_handlers.has_property = zend_closure_has_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_170
		case 63: {
		//AddAndReplaceKind
		closure_handlers.read_property = zend_closure_read_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_171
		case 64: {
		//AddAndReplaceKind
		closure_handlers.unset_property = zend_closure_unset_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_172
		case 65: {
		//AddAndReplaceKind
		closure_handlers.write_property = zend_closure_write_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_173
		case 66: {
		//AddAndReplaceKind
		convert_to_boolean((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_174
		case 67: {
		//AddAndReplaceKind
		convert_to_boolean((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_175
		case 68: {
		//AddAndReplaceKind
		destroy_op_array(&(closure)->func.op_array);
		
		break;
		}
		#endif
		#ifdef COMPILE_176
		case 69: {
		//AddAndReplaceKind
		exit(1);
		
		break;
		}
		#endif
		#ifdef COMPILE_177
		case 70: {
		//AddAndReplaceKind
		gc_remove_zval_from_buffer((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_178
		case 71: {
		//AddAndReplaceKind
		gc_remove_zval_from_buffer((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_179
		case 72: {
		//AddAndReplaceKind
		gc_zval_check_possible_root(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_180
		case 73: {
		//AddAndReplaceKind
		gc_zval_check_possible_root(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_181
		case 74: {
		//AddAndReplaceKind
		gc_zval_possible_root((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_182
		case 75: {
		//AddAndReplaceKind
		gc_zval_possible_root((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_183
		case 76: {
		//AddAndReplaceKind
		i_zval_ptr_dtor((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_184
		case 77: {
		//AddAndReplaceKind
		i_zval_ptr_dtor((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_185
		case 78: {
		//AddAndReplaceKind
		if (scope && !instanceof_function(scope, func->common.scope)) {
		    zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
		    scope = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_186
		case 79: {
		//AddAndReplaceKind
		if (scope && this_ptr && (func->common.fn_flags & 1) == 0 && !instanceof_function(zend_get_class_entry(&(*this_ptr)), closure->func.common.scope)) {
		    zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		    scope = ((void *)0);
		    this_ptr = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_187
		case 80: {
		//AddAndReplaceKind
		if (this_ptr && (closure->func.common.fn_flags & 1) == 0) {
		    closure->this_ptr = this_ptr;
		    zval_addref_p(this_ptr);
		} else {
		    closure->func.common.fn_flags |= 1;
		    closure->this_ptr = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_188
		case 81: {
		//AddAndReplaceKind
		memcpy(&closure_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
		
		break;
		}
		#endif
		#ifdef COMPILE_189
		case 82: {
		//AddAndReplaceKind
		memset((closure), 0, sizeof(zend_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_190
		case 83: {
		//AddAndReplaceKind
		scope = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_191
		case 84: {
		//AddAndReplaceKind
		this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_192
		case 85: {
		//AddAndReplaceKind
		zend_ce_closure->create_object = zend_closure_new;
		
		break;
		}
		#endif
		#ifdef COMPILE_193
		case 86: {
		//AddAndReplaceKind
		zend_ce_closure->serialize = zend_class_serialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_194
		case 87: {
		//AddAndReplaceKind
		zend_ce_closure->unserialize = zend_class_unserialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_195
		case 88: {
		//AddAndReplaceKind
		zend_error((1 << 0L), "Cannot destroy active lambda function");
		
		break;
		}
		#endif
		#ifdef COMPILE_196
		case 89: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Cannot get arguments for calling closure");
		
		break;
		}
		#endif
		#ifdef COMPILE_197
		case 90: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Closure object cannot have properties");
		
		break;
		}
		#endif
		#ifdef COMPILE_198
		case 91: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
		
		break;
		}
		#endif
		#ifdef COMPILE_199
		case 92: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", (func)->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_200
		case 93: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, (func)->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_201
		case 94: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(res)))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_202
		case 95: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(this_ptr)))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_203
		case 96: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_204
		case 97: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", (func)->common.scope->name, func->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_205
		case 98: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, (func)->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_206
		case 99: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, (scope)->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_207
		case 100: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_208
		case 101: {
		//AddAndReplaceKind
		zend_error((1 << 3L), "A non well formed numeric value encountered");
		
		break;
		}
		#endif
		#ifdef COMPILE_209
		case 102: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments((static_variables), (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_210
		case 103: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, (closure)->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_211
		case 104: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_212
		case 105: {
		//AddAndReplaceKind
		zend_hash_destroy((closure)->debug_info);
		
		break;
		}
		#endif
		#ifdef COMPILE_213
		case 106: {
		//AddAndReplaceKind
		zend_object_std_dtor(&(closure)->std);
		
		break;
		}
		#endif
		#ifdef COMPILE_214
		case 107: {
		//AddAndReplaceKind
		zval_addref_p(&((executor_globals.uninitialized_zval)));
		
		break;
		}
		#endif
		#ifdef COMPILE_215
		case 108: {
		//AddAndReplaceKind
		zval_addref_p((closure)->this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_216
		case 109: {
		//AddAndReplaceKind
		zval_addref_p((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_217
		case 110: {
		//AddAndReplaceKind
		zval_addref_p((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_218
		case 111: {
		//AddAndReplaceKind
		zval_addref_p(this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_219
		case 112: {
		//AddAndReplaceKind
		zval_unset_isref_p((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_220
		case 113: {
		//AddAndReplaceKind
		zval_unset_isref_p((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_221
		case 114: {
		//AddInitKind
		memset(&closure->func, 0, sizeof (*(&closure->func)));
		
		break;
		}
		#endif
		#ifdef COMPILE_222
		case 115: {
		//AddInitKind
		memset(&closure->func.op_array, 0, sizeof (*(&closure->func.op_array)));
		
		break;
		}
		#endif
		#ifdef COMPILE_223
		case 116: {
		//AddInitKind
		memset(closure, 0, sizeof (*(closure)));
		
		break;
		}
		#endif
		#ifdef COMPILE_224
		case 117: {
		//IfExitKind
		if (__is_neg("2-117", 40, &(closure), sizeof (closure), &(closure->func.op_array.static_variables), sizeof (closure->func.op_array.static_variables), &(static_variables), sizeof (static_variables), &(closure->func.op_array.run_time_cache), sizeof (closure->func.op_array.run_time_cache), &(closure->func.op_array.refcount), sizeof (closure->func.op_array.refcount), &(func), sizeof (func), &(res), sizeof (res), &(scope), sizeof (scope), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(this_ptr), sizeof (this_ptr), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->func.op_array.type), sizeof (closure->func.op_array.type), &(closure->func.op_array.function_name), sizeof (closure->func.op_array.function_name), &(closure->func.op_array.scope), sizeof (closure->func.op_array.scope), &(closure->func.op_array.fn_flags), sizeof (closure->func.op_array.fn_flags), &(closure->func.op_array.prototype), sizeof (closure->func.op_array.prototype), &(closure->func.op_array.num_args), sizeof (closure->func.op_array.num_args), &(closure->func.op_array.required_num_args), sizeof (closure->func.op_array.required_num_args), &(closure->func.op_array.arg_info), sizeof (closure->func.op_array.arg_info), &(closure->func.op_array.opcodes), sizeof (closure->func.op_array.opcodes), &(closure->func.op_array.last), sizeof (closure->func.op_array.last), &(closure->func.op_array.vars), sizeof (closure->func.op_array.vars), &(closure->func.op_array.last_var), sizeof (closure->func.op_array.last_var), &(closure->func.op_array.T), sizeof (closure->func.op_array.T), &(closure->func.op_array.brk_cont_array), sizeof (closure->func.op_array.brk_cont_array), &(closure->func.op_array.last_brk_cont), sizeof (closure->func.op_array.last_brk_cont), &(closure->func.op_array.try_catch_array), sizeof (closure->func.op_array.try_catch_array), &(closure->func.op_array.last_try_catch), sizeof (closure->func.op_array.last_try_catch), &(closure->func.op_array.this_var), sizeof (closure->func.op_array.this_var), &(closure->func.op_array.filename), sizeof (closure->func.op_array.filename), &(closure->func.op_array.line_start), sizeof (closure->func.op_array.line_start), &(closure->func.op_array.line_end), sizeof (closure->func.op_array.line_end), &(closure->func.op_array.doc_comment), sizeof (closure->func.op_array.doc_comment), &(closure->func.op_array.doc_comment_len), sizeof (closure->func.op_array.doc_comment_len), &(closure->func.op_array.early_binding), sizeof (closure->func.op_array.early_binding), &(closure->func.op_array.literals), sizeof (closure->func.op_array.literals), &(closure->func.op_array.last_literal), sizeof (closure->func.op_array.last_literal), &(closure->func.op_array.last_cache_slot), sizeof (closure->func.op_array.last_cache_slot), &(closure->debug_info), sizeof (closure->debug_info)))
		    return;
		
		break;
		}
		#endif
		}
		switch(__choose("__SWITCH3"))
		{
		case 0: {
		ALLOC_HASHTABLE(closure->func.op_array.static_variables);
		
		break;
		}
		#ifdef COMPILE_225
		case 1: {
		//AddAndReplaceKind
		(closure->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_226
		case 2: {
		//GuardKind
		if (!__is_neg("3-2", 40, &(closure), sizeof (closure), &(closure->func.op_array.static_variables), sizeof (closure->func.op_array.static_variables), &(static_variables), sizeof (static_variables), &(closure->func.op_array.run_time_cache), sizeof (closure->func.op_array.run_time_cache), &(closure->func.op_array.refcount), sizeof (closure->func.op_array.refcount), &(func), sizeof (func), &(res), sizeof (res), &(scope), sizeof (scope), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(this_ptr), sizeof (this_ptr), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->func.op_array.type), sizeof (closure->func.op_array.type), &(closure->func.op_array.function_name), sizeof (closure->func.op_array.function_name), &(closure->func.op_array.scope), sizeof (closure->func.op_array.scope), &(closure->func.op_array.fn_flags), sizeof (closure->func.op_array.fn_flags), &(closure->func.op_array.prototype), sizeof (closure->func.op_array.prototype), &(closure->func.op_array.num_args), sizeof (closure->func.op_array.num_args), &(closure->func.op_array.required_num_args), sizeof (closure->func.op_array.required_num_args), &(closure->func.op_array.arg_info), sizeof (closure->func.op_array.arg_info), &(closure->func.op_array.opcodes), sizeof (closure->func.op_array.opcodes), &(closure->func.op_array.last), sizeof (closure->func.op_array.last), &(closure->func.op_array.vars), sizeof (closure->func.op_array.vars), &(closure->func.op_array.last_var), sizeof (closure->func.op_array.last_var), &(closure->func.op_array.T), sizeof (closure->func.op_array.T), &(closure->func.op_array.brk_cont_array), sizeof (closure->func.op_array.brk_cont_array), &(closure->func.op_array.last_brk_cont), sizeof (closure->func.op_array.last_brk_cont), &(closure->func.op_array.try_catch_array), sizeof (closure->func.op_array.try_catch_array), &(closure->func.op_array.last_try_catch), sizeof (closure->func.op_array.last_try_catch), &(closure->func.op_array.this_var), sizeof (closure->func.op_array.this_var), &(closure->func.op_array.filename), sizeof (closure->func.op_array.filename), &(closure->func.op_array.line_start), sizeof (closure->func.op_array.line_start), &(closure->func.op_array.line_end), sizeof (closure->func.op_array.line_end), &(closure->func.op_array.doc_comment), sizeof (closure->func.op_array.doc_comment), &(closure->func.op_array.doc_comment_len), sizeof (closure->func.op_array.doc_comment_len), &(closure->func.op_array.early_binding), sizeof (closure->func.op_array.early_binding), &(closure->func.op_array.literals), sizeof (closure->func.op_array.literals), &(closure->func.op_array.last_literal), sizeof (closure->func.op_array.last_literal), &(closure->func.op_array.last_cache_slot), sizeof (closure->func.op_array.last_cache_slot), &(closure->debug_info), sizeof (closure->debug_info)))
		    (closure->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_227
		case 3: {
		//ReplaceKind
		((closure)->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		}
		}
					{
		switch(__choose("__SWITCH4"))
		{
		case 0: 
		break;
		#ifdef COMPILE_228
		case 1: {
		//AddAndReplaceKind
		((closure)->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_229
		case 2: {
		//AddAndReplaceKind
		(*(closure)->func.op_array.refcount)++;
		
		break;
		}
		#endif
		#ifdef COMPILE_230
		case 3: {
		//AddAndReplaceKind
		(*closure->func.op_array.refcount)++;
		
		break;
		}
		#endif
		#ifdef COMPILE_231
		case 4: {
		//AddAndReplaceKind
		(closure) = (zend_closure *)zend_object_store_get_object(res);
		
		break;
		}
		#endif
		#ifdef COMPILE_232
		case 5: {
		//AddAndReplaceKind
		(closure)->func = *func;
		
		break;
		}
		#endif
		#ifdef COMPILE_233
		case 6: {
		//AddAndReplaceKind
		(closure)->func.common.scope = scope;
		
		break;
		}
		#endif
		#ifdef COMPILE_234
		case 7: {
		//AddAndReplaceKind
		(closure)->func.op_array.run_time_cache = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_235
		case 8: {
		//AddAndReplaceKind
		(closure)->this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_236
		case 9: {
		//AddAndReplaceKind
		(closure)->this_ptr = this_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_237
		case 10: {
		//AddAndReplaceKind
		(closure->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_238
		case 11: {
		//AddAndReplaceKind
		(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
		
		break;
		}
		#endif
		#ifdef COMPILE_239
		case 12: {
		//AddAndReplaceKind
		(res) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_240
		case 13: {
		//AddAndReplaceKind
		(scope) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_241
		case 14: {
		//AddAndReplaceKind
		(scope)->create_object = zend_closure_new;
		
		break;
		}
		#endif
		#ifdef COMPILE_242
		case 15: {
		//AddAndReplaceKind
		(scope)->serialize = zend_class_serialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_243
		case 16: {
		//AddAndReplaceKind
		(scope)->unserialize = zend_class_unserialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_244
		case 17: {
		//AddAndReplaceKind
		(this_ptr) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_245
		case 18: {
		//AddAndReplaceKind
		_array_init(((res)), 0);
		
		break;
		}
		#endif
		#ifdef COMPILE_246
		case 19: {
		//AddAndReplaceKind
		_array_init(((this_ptr)), 0);
		
		break;
		}
		#endif
		#ifdef COMPILE_247
		case 20: {
		//AddAndReplaceKind
		_efree(((closure)));
		
		break;
		}
		#endif
		#ifdef COMPILE_248
		case 21: {
		//AddAndReplaceKind
		_efree(((closure)->debug_info));
		
		break;
		}
		#endif
		#ifdef COMPILE_249
		case 22: {
		//AddAndReplaceKind
		_efree(((func)));
		
		break;
		}
		#endif
		#ifdef COMPILE_250
		case 23: {
		//AddAndReplaceKind
		_efree(((func)->internal_function.function_name));
		
		break;
		}
		#endif
		#ifdef COMPILE_251
		case 24: {
		//AddAndReplaceKind
		_efree(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_252
		case 25: {
		//AddAndReplaceKind
		_efree(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_253
		case 26: {
		//AddAndReplaceKind
		_object_init_ex(((res)), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_254
		case 27: {
		//AddAndReplaceKind
		_object_init_ex(((this_ptr)), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_255
		case 28: {
		//AddAndReplaceKind
		_object_init_ex((res), ((scope)));
		
		break;
		}
		#endif
		#ifdef COMPILE_256
		case 29: {
		//AddAndReplaceKind
		_object_init_ex((res), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_257
		case 30: {
		//AddAndReplaceKind
		_zend_hash_init(((closure)->debug_info), (1), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_258
		case 31: {
		//AddAndReplaceKind
		_zend_hash_init(((closure)->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_259
		case 32: {
		//AddAndReplaceKind
		_zend_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements((static_variables))), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_260
		case 33: {
		//AddAndReplaceKind
		_zend_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_261
		case 34: {
		//AddAndReplaceKind
		_zval_copy_ctor(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_262
		case 35: {
		//AddAndReplaceKind
		_zval_copy_ctor(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_263
		case 36: {
		//AddAndReplaceKind
		_zval_copy_ctor_func((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_264
		case 37: {
		//AddAndReplaceKind
		_zval_copy_ctor_func((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_265
		case 38: {
		//AddAndReplaceKind
		_zval_dtor(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_266
		case 39: {
		//AddAndReplaceKind
		_zval_dtor(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_267
		case 40: {
		//AddAndReplaceKind
		_zval_dtor_func((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_268
		case 41: {
		//AddAndReplaceKind
		_zval_dtor_func((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_269
		case 42: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(closure)->this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_270
		case 43: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_271
		case 44: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_272
		case 45: {
		//AddAndReplaceKind
		closure = (zend_closure *)zend_object_store_get_object(res);
		
		break;
		}
		#endif
		#ifdef COMPILE_273
		case 46: {
		//AddAndReplaceKind
		closure->func = *(func);
		
		break;
		}
		#endif
		#ifdef COMPILE_274
		case 47: {
		//AddAndReplaceKind
		closure->func = *func;
		
		break;
		}
		#endif
		#ifdef COMPILE_275
		case 48: {
		//AddAndReplaceKind
		closure->func.common.scope = (scope);
		
		break;
		}
		#endif
		#ifdef COMPILE_276
		case 49: {
		//AddAndReplaceKind
		closure->func.common.scope = scope;
		
		break;
		}
		#endif
		#ifdef COMPILE_277
		case 50: {
		//AddAndReplaceKind
		closure->func.op_array.run_time_cache = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_278
		case 51: {
		//AddAndReplaceKind
		closure->this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_279
		case 52: {
		//AddAndReplaceKind
		closure->this_ptr = (res);
		
		break;
		}
		#endif
		#ifdef COMPILE_280
		case 53: {
		//AddAndReplaceKind
		closure->this_ptr = (this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_281
		case 54: {
		//AddAndReplaceKind
		closure->this_ptr = this_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_282
		case 55: {
		//AddAndReplaceKind
		closure_handlers.clone_obj = zend_closure_clone;
		
		break;
		}
		#endif
		#ifdef COMPILE_283
		case 56: {
		//AddAndReplaceKind
		closure_handlers.compare_objects = zend_closure_compare_objects;
		
		break;
		}
		#endif
		#ifdef COMPILE_284
		case 57: {
		//AddAndReplaceKind
		closure_handlers.get_closure = zend_closure_get_closure;
		
		break;
		}
		#endif
		#ifdef COMPILE_285
		case 58: {
		//AddAndReplaceKind
		closure_handlers.get_constructor = zend_closure_get_constructor;
		
		break;
		}
		#endif
		#ifdef COMPILE_286
		case 59: {
		//AddAndReplaceKind
		closure_handlers.get_debug_info = zend_closure_get_debug_info;
		
		break;
		}
		#endif
		#ifdef COMPILE_287
		case 60: {
		//AddAndReplaceKind
		closure_handlers.get_method = zend_closure_get_method;
		
		break;
		}
		#endif
		#ifdef COMPILE_288
		case 61: {
		//AddAndReplaceKind
		closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_289
		case 62: {
		//AddAndReplaceKind
		closure_handlers.has_property = zend_closure_has_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_290
		case 63: {
		//AddAndReplaceKind
		closure_handlers.read_property = zend_closure_read_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_291
		case 64: {
		//AddAndReplaceKind
		closure_handlers.unset_property = zend_closure_unset_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_292
		case 65: {
		//AddAndReplaceKind
		closure_handlers.write_property = zend_closure_write_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_293
		case 66: {
		//AddAndReplaceKind
		convert_to_boolean((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_294
		case 67: {
		//AddAndReplaceKind
		convert_to_boolean((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_295
		case 68: {
		//AddAndReplaceKind
		destroy_op_array(&(closure)->func.op_array);
		
		break;
		}
		#endif
		#ifdef COMPILE_296
		case 69: {
		//AddAndReplaceKind
		exit(1);
		
		break;
		}
		#endif
		#ifdef COMPILE_297
		case 70: {
		//AddAndReplaceKind
		gc_remove_zval_from_buffer((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_298
		case 71: {
		//AddAndReplaceKind
		gc_remove_zval_from_buffer((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_299
		case 72: {
		//AddAndReplaceKind
		gc_zval_check_possible_root(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_300
		case 73: {
		//AddAndReplaceKind
		gc_zval_check_possible_root(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_301
		case 74: {
		//AddAndReplaceKind
		gc_zval_possible_root((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_302
		case 75: {
		//AddAndReplaceKind
		gc_zval_possible_root((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_303
		case 76: {
		//AddAndReplaceKind
		i_zval_ptr_dtor((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_304
		case 77: {
		//AddAndReplaceKind
		i_zval_ptr_dtor((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_305
		case 78: {
		//AddAndReplaceKind
		if (scope && !instanceof_function(scope, func->common.scope)) {
		    zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
		    scope = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_306
		case 79: {
		//AddAndReplaceKind
		if (scope && this_ptr && (func->common.fn_flags & 1) == 0 && !instanceof_function(zend_get_class_entry(&(*this_ptr)), closure->func.common.scope)) {
		    zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		    scope = ((void *)0);
		    this_ptr = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_307
		case 80: {
		//AddAndReplaceKind
		if (this_ptr && (closure->func.common.fn_flags & 1) == 0) {
		    closure->this_ptr = this_ptr;
		    zval_addref_p(this_ptr);
		} else {
		    closure->func.common.fn_flags |= 1;
		    closure->this_ptr = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_308
		case 81: {
		//AddAndReplaceKind
		memcpy(&closure_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
		
		break;
		}
		#endif
		#ifdef COMPILE_309
		case 82: {
		//AddAndReplaceKind
		memset((closure), 0, sizeof(zend_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_310
		case 83: {
		//AddAndReplaceKind
		scope = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_311
		case 84: {
		//AddAndReplaceKind
		this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_312
		case 85: {
		//AddAndReplaceKind
		zend_ce_closure->create_object = zend_closure_new;
		
		break;
		}
		#endif
		#ifdef COMPILE_313
		case 86: {
		//AddAndReplaceKind
		zend_ce_closure->serialize = zend_class_serialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_314
		case 87: {
		//AddAndReplaceKind
		zend_ce_closure->unserialize = zend_class_unserialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_315
		case 88: {
		//AddAndReplaceKind
		zend_error((1 << 0L), "Cannot destroy active lambda function");
		
		break;
		}
		#endif
		#ifdef COMPILE_316
		case 89: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Cannot get arguments for calling closure");
		
		break;
		}
		#endif
		#ifdef COMPILE_317
		case 90: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Closure object cannot have properties");
		
		break;
		}
		#endif
		#ifdef COMPILE_318
		case 91: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
		
		break;
		}
		#endif
		#ifdef COMPILE_319
		case 92: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", (func)->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_320
		case 93: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, (func)->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_321
		case 94: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(res)))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_322
		case 95: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(this_ptr)))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_323
		case 96: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_324
		case 97: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", (func)->common.scope->name, func->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_325
		case 98: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, (func)->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_326
		case 99: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, (scope)->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_327
		case 100: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_328
		case 101: {
		//AddAndReplaceKind
		zend_error((1 << 3L), "A non well formed numeric value encountered");
		
		break;
		}
		#endif
		#ifdef COMPILE_329
		case 102: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments((static_variables), (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_330
		case 103: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, (closure)->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_331
		case 104: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_332
		case 105: {
		//AddAndReplaceKind
		zend_hash_destroy((closure)->debug_info);
		
		break;
		}
		#endif
		#ifdef COMPILE_333
		case 106: {
		//AddAndReplaceKind
		zend_object_std_dtor(&(closure)->std);
		
		break;
		}
		#endif
		#ifdef COMPILE_334
		case 107: {
		//AddAndReplaceKind
		zval_addref_p(&((executor_globals.uninitialized_zval)));
		
		break;
		}
		#endif
		#ifdef COMPILE_335
		case 108: {
		//AddAndReplaceKind
		zval_addref_p((closure)->this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_336
		case 109: {
		//AddAndReplaceKind
		zval_addref_p((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_337
		case 110: {
		//AddAndReplaceKind
		zval_addref_p((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_338
		case 111: {
		//AddAndReplaceKind
		zval_addref_p(this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_339
		case 112: {
		//AddAndReplaceKind
		zval_unset_isref_p((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_340
		case 113: {
		//AddAndReplaceKind
		zval_unset_isref_p((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_341
		case 114: {
		//AddInitKind
		memset(&closure->func, 0, sizeof (*(&closure->func)));
		
		break;
		}
		#endif
		#ifdef COMPILE_342
		case 115: {
		//AddInitKind
		memset(&closure->func.op_array, 0, sizeof (*(&closure->func.op_array)));
		
		break;
		}
		#endif
		#ifdef COMPILE_343
		case 116: {
		//AddInitKind
		memset(closure, 0, sizeof (*(closure)));
		
		break;
		}
		#endif
		#ifdef COMPILE_344
		case 117: {
		//IfExitKind
		if (__is_neg("4-117", 40, &(closure), sizeof (closure), &(static_variables), sizeof (static_variables), &(closure->func.op_array.static_variables), sizeof (closure->func.op_array.static_variables), &(closure->func.op_array.run_time_cache), sizeof (closure->func.op_array.run_time_cache), &(closure->func.op_array.refcount), sizeof (closure->func.op_array.refcount), &(func), sizeof (func), &(scope), sizeof (scope), &(res), sizeof (res), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(this_ptr), sizeof (this_ptr), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->func.op_array.type), sizeof (closure->func.op_array.type), &(closure->func.op_array.function_name), sizeof (closure->func.op_array.function_name), &(closure->func.op_array.scope), sizeof (closure->func.op_array.scope), &(closure->func.op_array.fn_flags), sizeof (closure->func.op_array.fn_flags), &(closure->func.op_array.prototype), sizeof (closure->func.op_array.prototype), &(closure->func.op_array.num_args), sizeof (closure->func.op_array.num_args), &(closure->func.op_array.required_num_args), sizeof (closure->func.op_array.required_num_args), &(closure->func.op_array.arg_info), sizeof (closure->func.op_array.arg_info), &(closure->func.op_array.opcodes), sizeof (closure->func.op_array.opcodes), &(closure->func.op_array.last), sizeof (closure->func.op_array.last), &(closure->func.op_array.vars), sizeof (closure->func.op_array.vars), &(closure->func.op_array.last_var), sizeof (closure->func.op_array.last_var), &(closure->func.op_array.T), sizeof (closure->func.op_array.T), &(closure->func.op_array.brk_cont_array), sizeof (closure->func.op_array.brk_cont_array), &(closure->func.op_array.last_brk_cont), sizeof (closure->func.op_array.last_brk_cont), &(closure->func.op_array.try_catch_array), sizeof (closure->func.op_array.try_catch_array), &(closure->func.op_array.last_try_catch), sizeof (closure->func.op_array.last_try_catch), &(closure->func.op_array.this_var), sizeof (closure->func.op_array.this_var), &(closure->func.op_array.filename), sizeof (closure->func.op_array.filename), &(closure->func.op_array.line_start), sizeof (closure->func.op_array.line_start), &(closure->func.op_array.line_end), sizeof (closure->func.op_array.line_end), &(closure->func.op_array.doc_comment), sizeof (closure->func.op_array.doc_comment), &(closure->func.op_array.doc_comment_len), sizeof (closure->func.op_array.doc_comment_len), &(closure->func.op_array.early_binding), sizeof (closure->func.op_array.early_binding), &(closure->func.op_array.literals), sizeof (closure->func.op_array.literals), &(closure->func.op_array.last_literal), sizeof (closure->func.op_array.last_literal), &(closure->func.op_array.last_cache_slot), sizeof (closure->func.op_array.last_cache_slot), &(closure->debug_info), sizeof (closure->debug_info)))
		    return;
		
		break;
		}
		#endif
		}
		switch(__choose("__SWITCH5"))
		{
		case 0: {
		zend_hash_init(closure->func.op_array.static_variables, zend_hash_num_elements(static_variables), NULL, ZVAL_PTR_DTOR, 0);
		
		break;
		}
		#ifdef COMPILE_345
		case 1: {
		//AddAndReplaceKind
		_zend_ts_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_346
		case 2: {
		//AddAndReplaceKind
		printf((closure->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_347
		case 3: {
		//AddAndReplaceKind
		scanf((closure->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_348
		case 4: {
		//GuardKind
		if (!__is_neg("5-4", 40, &(closure), sizeof (closure), &(static_variables), sizeof (static_variables), &(closure->func.op_array.static_variables), sizeof (closure->func.op_array.static_variables), &(closure->func.op_array.run_time_cache), sizeof (closure->func.op_array.run_time_cache), &(closure->func.op_array.refcount), sizeof (closure->func.op_array.refcount), &(func), sizeof (func), &(scope), sizeof (scope), &(res), sizeof (res), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(this_ptr), sizeof (this_ptr), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->func.op_array.type), sizeof (closure->func.op_array.type), &(closure->func.op_array.function_name), sizeof (closure->func.op_array.function_name), &(closure->func.op_array.scope), sizeof (closure->func.op_array.scope), &(closure->func.op_array.fn_flags), sizeof (closure->func.op_array.fn_flags), &(closure->func.op_array.prototype), sizeof (closure->func.op_array.prototype), &(closure->func.op_array.num_args), sizeof (closure->func.op_array.num_args), &(closure->func.op_array.required_num_args), sizeof (closure->func.op_array.required_num_args), &(closure->func.op_array.arg_info), sizeof (closure->func.op_array.arg_info), &(closure->func.op_array.opcodes), sizeof (closure->func.op_array.opcodes), &(closure->func.op_array.last), sizeof (closure->func.op_array.last), &(closure->func.op_array.vars), sizeof (closure->func.op_array.vars), &(closure->func.op_array.last_var), sizeof (closure->func.op_array.last_var), &(closure->func.op_array.T), sizeof (closure->func.op_array.T), &(closure->func.op_array.brk_cont_array), sizeof (closure->func.op_array.brk_cont_array), &(closure->func.op_array.last_brk_cont), sizeof (closure->func.op_array.last_brk_cont), &(closure->func.op_array.try_catch_array), sizeof (closure->func.op_array.try_catch_array), &(closure->func.op_array.last_try_catch), sizeof (closure->func.op_array.last_try_catch), &(closure->func.op_array.this_var), sizeof (closure->func.op_array.this_var), &(closure->func.op_array.filename), sizeof (closure->func.op_array.filename), &(closure->func.op_array.line_start), sizeof (closure->func.op_array.line_start), &(closure->func.op_array.line_end), sizeof (closure->func.op_array.line_end), &(closure->func.op_array.doc_comment), sizeof (closure->func.op_array.doc_comment), &(closure->func.op_array.doc_comment_len), sizeof (closure->func.op_array.doc_comment_len), &(closure->func.op_array.early_binding), sizeof (closure->func.op_array.early_binding), &(closure->func.op_array.literals), sizeof (closure->func.op_array.literals), &(closure->func.op_array.last_literal), sizeof (closure->func.op_array.last_literal), &(closure->func.op_array.last_cache_slot), sizeof (closure->func.op_array.last_cache_slot), &(closure->debug_info), sizeof (closure->debug_info)))
		    _zend_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_349
		case 5: {
		//ReplaceKind
		_zend_hash_init(((closure)->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_350
		case 6: {
		//ReplaceKind
		_zend_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements((static_variables))), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		}
		}
					{
		switch(__choose("__SWITCH6"))
		{
		case 0: 
		break;
		#ifdef COMPILE_351
		case 1: {
		//AddAndReplaceKind
		((closure)->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_352
		case 2: {
		//AddAndReplaceKind
		(*(closure)->func.op_array.refcount)++;
		
		break;
		}
		#endif
		#ifdef COMPILE_353
		case 3: {
		//AddAndReplaceKind
		(*closure->func.op_array.refcount)++;
		
		break;
		}
		#endif
		#ifdef COMPILE_354
		case 4: {
		//AddAndReplaceKind
		(closure) = (zend_closure *)zend_object_store_get_object(res);
		
		break;
		}
		#endif
		#ifdef COMPILE_355
		case 5: {
		//AddAndReplaceKind
		(closure)->func = *func;
		
		break;
		}
		#endif
		#ifdef COMPILE_356
		case 6: {
		//AddAndReplaceKind
		(closure)->func.common.scope = scope;
		
		break;
		}
		#endif
		#ifdef COMPILE_357
		case 7: {
		//AddAndReplaceKind
		(closure)->func.op_array.run_time_cache = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_358
		case 8: {
		//AddAndReplaceKind
		(closure)->this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_359
		case 9: {
		//AddAndReplaceKind
		(closure)->this_ptr = this_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_360
		case 10: {
		//AddAndReplaceKind
		(closure->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
		
		break;
		}
		#endif
		#ifdef COMPILE_361
		case 11: {
		//AddAndReplaceKind
		(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
		
		break;
		}
		#endif
		#ifdef COMPILE_362
		case 12: {
		//AddAndReplaceKind
		(res) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_363
		case 13: {
		//AddAndReplaceKind
		(scope) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_364
		case 14: {
		//AddAndReplaceKind
		(scope)->create_object = zend_closure_new;
		
		break;
		}
		#endif
		#ifdef COMPILE_365
		case 15: {
		//AddAndReplaceKind
		(scope)->serialize = zend_class_serialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_366
		case 16: {
		//AddAndReplaceKind
		(scope)->unserialize = zend_class_unserialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_367
		case 17: {
		//AddAndReplaceKind
		(this_ptr) = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_368
		case 18: {
		//AddAndReplaceKind
		_array_init(((res)), 0);
		
		break;
		}
		#endif
		#ifdef COMPILE_369
		case 19: {
		//AddAndReplaceKind
		_array_init(((this_ptr)), 0);
		
		break;
		}
		#endif
		#ifdef COMPILE_370
		case 20: {
		//AddAndReplaceKind
		_efree(((closure)));
		
		break;
		}
		#endif
		#ifdef COMPILE_371
		case 21: {
		//AddAndReplaceKind
		_efree(((closure)->debug_info));
		
		break;
		}
		#endif
		#ifdef COMPILE_372
		case 22: {
		//AddAndReplaceKind
		_efree(((func)));
		
		break;
		}
		#endif
		#ifdef COMPILE_373
		case 23: {
		//AddAndReplaceKind
		_efree(((func)->internal_function.function_name));
		
		break;
		}
		#endif
		#ifdef COMPILE_374
		case 24: {
		//AddAndReplaceKind
		_efree(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_375
		case 25: {
		//AddAndReplaceKind
		_efree(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_376
		case 26: {
		//AddAndReplaceKind
		_object_init_ex(((res)), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_377
		case 27: {
		//AddAndReplaceKind
		_object_init_ex(((this_ptr)), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_378
		case 28: {
		//AddAndReplaceKind
		_object_init_ex((res), ((scope)));
		
		break;
		}
		#endif
		#ifdef COMPILE_379
		case 29: {
		//AddAndReplaceKind
		_object_init_ex((res), (zend_ce_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_380
		case 30: {
		//AddAndReplaceKind
		_zend_hash_init(((closure)->debug_info), (1), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_381
		case 31: {
		//AddAndReplaceKind
		_zend_hash_init(((closure)->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_382
		case 32: {
		//AddAndReplaceKind
		_zend_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements((static_variables))), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_383
		case 33: {
		//AddAndReplaceKind
		_zend_hash_init((closure->func.op_array.static_variables), (zend_hash_num_elements(static_variables)), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
		
		break;
		}
		#endif
		#ifdef COMPILE_384
		case 34: {
		//AddAndReplaceKind
		_zval_copy_ctor(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_385
		case 35: {
		//AddAndReplaceKind
		_zval_copy_ctor(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_386
		case 36: {
		//AddAndReplaceKind
		_zval_copy_ctor_func((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_387
		case 37: {
		//AddAndReplaceKind
		_zval_copy_ctor_func((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_388
		case 38: {
		//AddAndReplaceKind
		_zval_dtor(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_389
		case 39: {
		//AddAndReplaceKind
		_zval_dtor(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_390
		case 40: {
		//AddAndReplaceKind
		_zval_dtor_func((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_391
		case 41: {
		//AddAndReplaceKind
		_zval_dtor_func((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_392
		case 42: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(closure)->this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_393
		case 43: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_394
		case 44: {
		//AddAndReplaceKind
		_zval_ptr_dtor((&(this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_395
		case 45: {
		//AddAndReplaceKind
		closure = (zend_closure *)zend_object_store_get_object(res);
		
		break;
		}
		#endif
		#ifdef COMPILE_396
		case 46: {
		//AddAndReplaceKind
		closure->func = *(func);
		
		break;
		}
		#endif
		#ifdef COMPILE_397
		case 47: {
		//AddAndReplaceKind
		closure->func = *func;
		
		break;
		}
		#endif
		#ifdef COMPILE_398
		case 48: {
		//AddAndReplaceKind
		closure->func.common.scope = (scope);
		
		break;
		}
		#endif
		#ifdef COMPILE_399
		case 49: {
		//AddAndReplaceKind
		closure->func.common.scope = scope;
		
		break;
		}
		#endif
		#ifdef COMPILE_400
		case 50: {
		//AddAndReplaceKind
		closure->func.op_array.run_time_cache = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_401
		case 51: {
		//AddAndReplaceKind
		closure->this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_402
		case 52: {
		//AddAndReplaceKind
		closure->this_ptr = (res);
		
		break;
		}
		#endif
		#ifdef COMPILE_403
		case 53: {
		//AddAndReplaceKind
		closure->this_ptr = (this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_404
		case 54: {
		//AddAndReplaceKind
		closure->this_ptr = this_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_405
		case 55: {
		//AddAndReplaceKind
		closure_handlers.clone_obj = zend_closure_clone;
		
		break;
		}
		#endif
		#ifdef COMPILE_406
		case 56: {
		//AddAndReplaceKind
		closure_handlers.compare_objects = zend_closure_compare_objects;
		
		break;
		}
		#endif
		#ifdef COMPILE_407
		case 57: {
		//AddAndReplaceKind
		closure_handlers.get_closure = zend_closure_get_closure;
		
		break;
		}
		#endif
		#ifdef COMPILE_408
		case 58: {
		//AddAndReplaceKind
		closure_handlers.get_constructor = zend_closure_get_constructor;
		
		break;
		}
		#endif
		#ifdef COMPILE_409
		case 59: {
		//AddAndReplaceKind
		closure_handlers.get_debug_info = zend_closure_get_debug_info;
		
		break;
		}
		#endif
		#ifdef COMPILE_410
		case 60: {
		//AddAndReplaceKind
		closure_handlers.get_method = zend_closure_get_method;
		
		break;
		}
		#endif
		#ifdef COMPILE_411
		case 61: {
		//AddAndReplaceKind
		closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
		
		break;
		}
		#endif
		#ifdef COMPILE_412
		case 62: {
		//AddAndReplaceKind
		closure_handlers.has_property = zend_closure_has_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_413
		case 63: {
		//AddAndReplaceKind
		closure_handlers.read_property = zend_closure_read_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_414
		case 64: {
		//AddAndReplaceKind
		closure_handlers.unset_property = zend_closure_unset_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_415
		case 65: {
		//AddAndReplaceKind
		closure_handlers.write_property = zend_closure_write_property;
		
		break;
		}
		#endif
		#ifdef COMPILE_416
		case 66: {
		//AddAndReplaceKind
		convert_to_boolean((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_417
		case 67: {
		//AddAndReplaceKind
		convert_to_boolean((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_418
		case 68: {
		//AddAndReplaceKind
		destroy_op_array(&(closure)->func.op_array);
		
		break;
		}
		#endif
		#ifdef COMPILE_419
		case 69: {
		//AddAndReplaceKind
		exit(1);
		
		break;
		}
		#endif
		#ifdef COMPILE_420
		case 70: {
		//AddAndReplaceKind
		gc_remove_zval_from_buffer((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_421
		case 71: {
		//AddAndReplaceKind
		gc_remove_zval_from_buffer((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_422
		case 72: {
		//AddAndReplaceKind
		gc_zval_check_possible_root(((res)));
		
		break;
		}
		#endif
		#ifdef COMPILE_423
		case 73: {
		//AddAndReplaceKind
		gc_zval_check_possible_root(((this_ptr)));
		
		break;
		}
		#endif
		#ifdef COMPILE_424
		case 74: {
		//AddAndReplaceKind
		gc_zval_possible_root((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_425
		case 75: {
		//AddAndReplaceKind
		gc_zval_possible_root((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_426
		case 76: {
		//AddAndReplaceKind
		i_zval_ptr_dtor((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_427
		case 77: {
		//AddAndReplaceKind
		i_zval_ptr_dtor((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_428
		case 78: {
		//AddAndReplaceKind
		if (scope && !instanceof_function(scope, func->common.scope)) {
		    zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
		    scope = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_429
		case 79: {
		//AddAndReplaceKind
		if (scope && this_ptr && (func->common.fn_flags & 1) == 0 && !instanceof_function(zend_get_class_entry(&(*this_ptr)), closure->func.common.scope)) {
		    zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		    scope = ((void *)0);
		    this_ptr = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_430
		case 80: {
		//AddAndReplaceKind
		if (this_ptr && (closure->func.common.fn_flags & 1) == 0) {
		    closure->this_ptr = this_ptr;
		    zval_addref_p(this_ptr);
		} else {
		    closure->func.common.fn_flags |= 1;
		    closure->this_ptr = ((void *)0);
		}
		
		break;
		}
		#endif
		#ifdef COMPILE_431
		case 81: {
		//AddAndReplaceKind
		memcpy(&closure_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
		
		break;
		}
		#endif
		#ifdef COMPILE_432
		case 82: {
		//AddAndReplaceKind
		memset((closure), 0, sizeof(zend_closure));
		
		break;
		}
		#endif
		#ifdef COMPILE_433
		case 83: {
		//AddAndReplaceKind
		scope = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_434
		case 84: {
		//AddAndReplaceKind
		this_ptr = ((void *)0);
		
		break;
		}
		#endif
		#ifdef COMPILE_435
		case 85: {
		//AddAndReplaceKind
		zend_ce_closure->create_object = zend_closure_new;
		
		break;
		}
		#endif
		#ifdef COMPILE_436
		case 86: {
		//AddAndReplaceKind
		zend_ce_closure->serialize = zend_class_serialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_437
		case 87: {
		//AddAndReplaceKind
		zend_ce_closure->unserialize = zend_class_unserialize_deny;
		
		break;
		}
		#endif
		#ifdef COMPILE_438
		case 88: {
		//AddAndReplaceKind
		zend_error((1 << 0L), "Cannot destroy active lambda function");
		
		break;
		}
		#endif
		#ifdef COMPILE_439
		case 89: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Cannot get arguments for calling closure");
		
		break;
		}
		#endif
		#ifdef COMPILE_440
		case 90: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Closure object cannot have properties");
		
		break;
		}
		#endif
		#ifdef COMPILE_441
		case 91: {
		//AddAndReplaceKind
		zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
		
		break;
		}
		#endif
		#ifdef COMPILE_442
		case 92: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", (func)->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_443
		case 93: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, (func)->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_444
		case 94: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(res)))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_445
		case 95: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(this_ptr)))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_446
		case 96: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_447
		case 97: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", (func)->common.scope->name, func->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_448
		case 98: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, (func)->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_449
		case 99: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, (scope)->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_450
		case 100: {
		//AddAndReplaceKind
		zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
		
		break;
		}
		#endif
		#ifdef COMPILE_451
		case 101: {
		//AddAndReplaceKind
		zend_error((1 << 3L), "A non well formed numeric value encountered");
		
		break;
		}
		#endif
		#ifdef COMPILE_452
		case 102: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments((static_variables), (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_453
		case 103: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, (closure)->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_454
		case 104: {
		//AddAndReplaceKind
		zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_455
		case 105: {
		//AddAndReplaceKind
		zend_hash_destroy((closure)->debug_info);
		
		break;
		}
		#endif
		#ifdef COMPILE_456
		case 106: {
		//AddAndReplaceKind
		zend_object_std_dtor(&(closure)->std);
		
		break;
		}
		#endif
		#ifdef COMPILE_457
		case 107: {
		//AddAndReplaceKind
		zval_addref_p(&((executor_globals.uninitialized_zval)));
		
		break;
		}
		#endif
		#ifdef COMPILE_458
		case 108: {
		//AddAndReplaceKind
		zval_addref_p((closure)->this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_459
		case 109: {
		//AddAndReplaceKind
		zval_addref_p((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_460
		case 110: {
		//AddAndReplaceKind
		zval_addref_p((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_461
		case 111: {
		//AddAndReplaceKind
		zval_addref_p(this_ptr);
		
		break;
		}
		#endif
		#ifdef COMPILE_462
		case 112: {
		//AddAndReplaceKind
		zval_unset_isref_p((res));
		
		break;
		}
		#endif
		#ifdef COMPILE_463
		case 113: {
		//AddAndReplaceKind
		zval_unset_isref_p((this_ptr));
		
		break;
		}
		#endif
		#ifdef COMPILE_464
		case 114: {
		//AddInitKind
		memset(&closure->func, 0, sizeof (*(&closure->func)));
		
		break;
		}
		#endif
		#ifdef COMPILE_465
		case 115: {
		//AddInitKind
		memset(&closure->func.op_array, 0, sizeof (*(&closure->func.op_array)));
		
		break;
		}
		#endif
		#ifdef COMPILE_466
		case 116: {
		//AddInitKind
		memset(closure, 0, sizeof (*(closure)));
		
		break;
		}
		#endif
		#ifdef COMPILE_467
		case 117: {
		//IfExitKind
		if (__is_neg("6-117", 40, &(closure), sizeof (closure), &(static_variables), sizeof (static_variables), &(closure->func.op_array.static_variables), sizeof (closure->func.op_array.static_variables), &(closure->func.op_array.run_time_cache), sizeof (closure->func.op_array.run_time_cache), &(closure->func.op_array.refcount), sizeof (closure->func.op_array.refcount), &(func), sizeof (func), &(scope), sizeof (scope), &(res), sizeof (res), &(this_ptr), sizeof (this_ptr), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->debug_info), sizeof (closure->debug_info), &(closure->func.op_array.type), sizeof (closure->func.op_array.type), &(closure->func.op_array.function_name), sizeof (closure->func.op_array.function_name), &(closure->func.op_array.scope), sizeof (closure->func.op_array.scope), &(closure->func.op_array.fn_flags), sizeof (closure->func.op_array.fn_flags), &(closure->func.op_array.prototype), sizeof (closure->func.op_array.prototype), &(closure->func.op_array.num_args), sizeof (closure->func.op_array.num_args), &(closure->func.op_array.required_num_args), sizeof (closure->func.op_array.required_num_args), &(closure->func.op_array.arg_info), sizeof (closure->func.op_array.arg_info), &(closure->func.op_array.opcodes), sizeof (closure->func.op_array.opcodes), &(closure->func.op_array.last), sizeof (closure->func.op_array.last), &(closure->func.op_array.vars), sizeof (closure->func.op_array.vars), &(closure->func.op_array.last_var), sizeof (closure->func.op_array.last_var), &(closure->func.op_array.T), sizeof (closure->func.op_array.T), &(closure->func.op_array.brk_cont_array), sizeof (closure->func.op_array.brk_cont_array), &(closure->func.op_array.last_brk_cont), sizeof (closure->func.op_array.last_brk_cont), &(closure->func.op_array.try_catch_array), sizeof (closure->func.op_array.try_catch_array), &(closure->func.op_array.last_try_catch), sizeof (closure->func.op_array.last_try_catch), &(closure->func.op_array.this_var), sizeof (closure->func.op_array.this_var), &(closure->func.op_array.filename), sizeof (closure->func.op_array.filename), &(closure->func.op_array.line_start), sizeof (closure->func.op_array.line_start), &(closure->func.op_array.line_end), sizeof (closure->func.op_array.line_end), &(closure->func.op_array.doc_comment), sizeof (closure->func.op_array.doc_comment), &(closure->func.op_array.doc_comment_len), sizeof (closure->func.op_array.doc_comment_len), &(closure->func.op_array.early_binding), sizeof (closure->func.op_array.early_binding), &(closure->func.op_array.literals), sizeof (closure->func.op_array.literals), &(closure->func.op_array.last_literal), sizeof (closure->func.op_array.last_literal), &(closure->func.op_array.last_cache_slot), sizeof (closure->func.op_array.last_cache_slot)))
		    return;
		
		break;
		}
		#endif
		}
		switch(__choose("__SWITCH7"))
		{
		case 0: {
		zend_hash_apply_with_arguments(static_variables TSRMLS_CC, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#ifdef COMPILE_468
		case 1: {
		//AddAndReplaceKind
		printf(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_469
		case 2: {
		//AddAndReplaceKind
		scanf(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_470
		case 3: {
		//AddAndReplaceKind
		zend_ts_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_471
		case 4: {
		//GuardKind
		if (!__is_neg("7-4", 40, &(closure), sizeof (closure), &(static_variables), sizeof (static_variables), &(closure->func.op_array.static_variables), sizeof (closure->func.op_array.static_variables), &(closure->func.op_array.run_time_cache), sizeof (closure->func.op_array.run_time_cache), &(closure->func.op_array.refcount), sizeof (closure->func.op_array.refcount), &(func), sizeof (func), &(scope), sizeof (scope), &(res), sizeof (res), &(this_ptr), sizeof (this_ptr), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->func.op_array.type), sizeof (closure->func.op_array.type), &(closure->func.op_array.function_name), sizeof (closure->func.op_array.function_name), &(closure->func.op_array.scope), sizeof (closure->func.op_array.scope), &(closure->func.op_array.fn_flags), sizeof (closure->func.op_array.fn_flags), &(closure->func.op_array.prototype), sizeof (closure->func.op_array.prototype), &(closure->func.op_array.num_args), sizeof (closure->func.op_array.num_args), &(closure->func.op_array.required_num_args), sizeof (closure->func.op_array.required_num_args), &(closure->func.op_array.arg_info), sizeof (closure->func.op_array.arg_info), &(closure->func.op_array.opcodes), sizeof (closure->func.op_array.opcodes), &(closure->func.op_array.last), sizeof (closure->func.op_array.last), &(closure->func.op_array.vars), sizeof (closure->func.op_array.vars), &(closure->func.op_array.last_var), sizeof (closure->func.op_array.last_var), &(closure->func.op_array.T), sizeof (closure->func.op_array.T), &(closure->func.op_array.brk_cont_array), sizeof (closure->func.op_array.brk_cont_array), &(closure->func.op_array.last_brk_cont), sizeof (closure->func.op_array.last_brk_cont), &(closure->func.op_array.try_catch_array), sizeof (closure->func.op_array.try_catch_array), &(closure->func.op_array.last_try_catch), sizeof (closure->func.op_array.last_try_catch), &(closure->func.op_array.this_var), sizeof (closure->func.op_array.this_var), &(closure->func.op_array.filename), sizeof (closure->func.op_array.filename), &(closure->func.op_array.line_start), sizeof (closure->func.op_array.line_start), &(closure->func.op_array.line_end), sizeof (closure->func.op_array.line_end), &(closure->func.op_array.doc_comment), sizeof (closure->func.op_array.doc_comment), &(closure->func.op_array.doc_comment_len), sizeof (closure->func.op_array.doc_comment_len), &(closure->func.op_array.early_binding), sizeof (closure->func.op_array.early_binding), &(closure->func.op_array.literals), sizeof (closure->func.op_array.literals), &(closure->func.op_array.last_literal), sizeof (closure->func.op_array.last_literal), &(closure->func.op_array.last_cache_slot), sizeof (closure->func.op_array.last_cache_slot), &(closure->debug_info), sizeof (closure->debug_info)))
		    zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_472
		case 5: {
		//ReplaceKind
		zend_hash_apply_with_arguments((static_variables), (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_473
		case 6: {
		//ReplaceKind
		zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, (closure)->func.op_array.static_variables);
		
		break;
		}
		#endif
		}
		}
				}
		
		break;
		}
		#ifdef COMPILE_474
		case 1: {
		//AddAndReplaceKind
		printf(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_475
		case 2: {
		//AddAndReplaceKind
		scanf(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_476
		case 3: {
		//AddAndReplaceKind
		zend_ts_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_477
		case 4: {
		//GuardKind
		if (!__is_neg("8-4", 40, &(closure), sizeof (closure), &(static_variables), sizeof (static_variables), &(closure->func.op_array.static_variables), sizeof (closure->func.op_array.static_variables), &(closure->func.op_array.run_time_cache), sizeof (closure->func.op_array.run_time_cache), &(closure->func.op_array.refcount), sizeof (closure->func.op_array.refcount), &(func), sizeof (func), &(scope), sizeof (scope), &(res), sizeof (res), &(this_ptr), sizeof (this_ptr), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->func.op_array.type), sizeof (closure->func.op_array.type), &(closure->func.op_array.function_name), sizeof (closure->func.op_array.function_name), &(closure->func.op_array.scope), sizeof (closure->func.op_array.scope), &(closure->func.op_array.fn_flags), sizeof (closure->func.op_array.fn_flags), &(closure->func.op_array.prototype), sizeof (closure->func.op_array.prototype), &(closure->func.op_array.num_args), sizeof (closure->func.op_array.num_args), &(closure->func.op_array.required_num_args), sizeof (closure->func.op_array.required_num_args), &(closure->func.op_array.arg_info), sizeof (closure->func.op_array.arg_info), &(closure->func.op_array.opcodes), sizeof (closure->func.op_array.opcodes), &(closure->func.op_array.last), sizeof (closure->func.op_array.last), &(closure->func.op_array.vars), sizeof (closure->func.op_array.vars), &(closure->func.op_array.last_var), sizeof (closure->func.op_array.last_var), &(closure->func.op_array.T), sizeof (closure->func.op_array.T), &(closure->func.op_array.brk_cont_array), sizeof (closure->func.op_array.brk_cont_array), &(closure->func.op_array.last_brk_cont), sizeof (closure->func.op_array.last_brk_cont), &(closure->func.op_array.try_catch_array), sizeof (closure->func.op_array.try_catch_array), &(closure->func.op_array.last_try_catch), sizeof (closure->func.op_array.last_try_catch), &(closure->func.op_array.this_var), sizeof (closure->func.op_array.this_var), &(closure->func.op_array.filename), sizeof (closure->func.op_array.filename), &(closure->func.op_array.line_start), sizeof (closure->func.op_array.line_start), &(closure->func.op_array.line_end), sizeof (closure->func.op_array.line_end), &(closure->func.op_array.doc_comment), sizeof (closure->func.op_array.doc_comment), &(closure->func.op_array.doc_comment_len), sizeof (closure->func.op_array.doc_comment_len), &(closure->func.op_array.early_binding), sizeof (closure->func.op_array.early_binding), &(closure->func.op_array.literals), sizeof (closure->func.op_array.literals), &(closure->func.op_array.last_literal), sizeof (closure->func.op_array.last_literal), &(closure->func.op_array.last_cache_slot), sizeof (closure->func.op_array.last_cache_slot), &(closure->debug_info), sizeof (closure->debug_info)))
		    zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_478
		case 5: {
		//ReplaceKind
		zend_hash_apply_with_arguments((static_variables), (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
		
		break;
		}
		#endif
		#ifdef COMPILE_479
		case 6: {
		//ReplaceKind
		zend_hash_apply_with_arguments(static_variables, (apply_func_args_t)zval_copy_static_var, 1, (closure)->func.op_array.static_variables);
		
		break;
		}
		#endif
		}
		}
		}
		closure->func.op_array.run_time_cache = NULL;
		(*closure->func.op_array.refcount)++;
	} else {
		/* verify that we aren't binding internal function to a wrong scope */
		if(func->common.scope != NULL) {
			if(scope && !instanceof_function(scope, func->common.scope TSRMLS_CC)) {
				zend_error(E_WARNING, "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
				scope = NULL;
			}
			if(scope && this_ptr && (func->common.fn_flags & ZEND_ACC_STATIC) == 0 && 
					!instanceof_function(Z_OBJCE_P(this_ptr), closure->func.common.scope TSRMLS_CC)) {
				zend_error(E_WARNING, "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, Z_OBJCE_P(this_ptr)->name);
				scope = NULL;
				this_ptr = NULL;
			}
		} else {
			/* if it's a free function, we won't set scope & this since they're meaningless */
			this_ptr = NULL;
			scope = NULL;
		}
	}

	closure->func.common.scope = scope;
	if (scope) {
		closure->func.common.fn_flags |= ZEND_ACC_PUBLIC;
		if (this_ptr && (closure->func.common.fn_flags & ZEND_ACC_STATIC) == 0) {
			closure->this_ptr = this_ptr;
			Z_ADDREF_P(this_ptr);
		} else {
			closure->func.common.fn_flags |= ZEND_ACC_STATIC;
			closure->this_ptr = NULL;
		}
	} else {
		closure->this_ptr = NULL;
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
