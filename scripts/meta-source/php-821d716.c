#define __COMPILE_0
#define __COMPILE_1
#define __COMPILE_2
#define __COMPILE_3
#define __COMPILE_4
#define __COMPILE_5
#define __COMPILE_6
#define __COMPILE_7
#define __COMPILE_8
#define __COMPILE_9
#define __COMPILE_10
#define __COMPILE_11
#define __COMPILE_12
#define __COMPILE_13
#define __COMPILE_14
#define __COMPILE_15
#define __COMPILE_16
#define __COMPILE_17
#define __COMPILE_18
#define __COMPILE_19
#define __COMPILE_20
#define __COMPILE_21
#define __COMPILE_22
#define __COMPILE_23
#define __COMPILE_24
#define __COMPILE_25
#define __COMPILE_26
#define __COMPILE_28
#define __COMPILE_29
#define __COMPILE_30
#define __COMPILE_31
#define __COMPILE_32
#define __COMPILE_33
#define __COMPILE_34
#define __COMPILE_35
#define __COMPILE_36
#define __COMPILE_37
#define __COMPILE_38
#define __COMPILE_39
#define __COMPILE_40
#define __COMPILE_41
#define __COMPILE_42
#define __COMPILE_43
#define __COMPILE_44
#define __COMPILE_45
#define __COMPILE_46
#define __COMPILE_47
#define __COMPILE_48
#define __COMPILE_49
#define __COMPILE_50
#define __COMPILE_51
#define __COMPILE_52
#define __COMPILE_53
#define __COMPILE_54
#define __COMPILE_56
#define __COMPILE_57
#define __COMPILE_58
#define __COMPILE_59
#define __COMPILE_60
#define __COMPILE_61
#define __COMPILE_62
#define __COMPILE_63
#define __COMPILE_64
#define __COMPILE_65
#define __COMPILE_66
#define __COMPILE_67
#define __COMPILE_68
#define __COMPILE_69
#define __COMPILE_70
#define __COMPILE_71
#define __COMPILE_72
#define __COMPILE_73
#define __COMPILE_74
#define __COMPILE_75
#define __COMPILE_76
#define __COMPILE_77
#define __COMPILE_78
#define __COMPILE_80
#define __COMPILE_82
#define __COMPILE_83
#define __COMPILE_85
#define __COMPILE_87
#define __COMPILE_89
#define __COMPILE_91
#define __COMPILE_92
#define __COMPILE_94
#define __COMPILE_95
#define __COMPILE_96
#define __COMPILE_97
#define __COMPILE_98
#define __COMPILE_99
#define __COMPILE_100
#define __COMPILE_101
#define __COMPILE_102
#define __COMPILE_103
#define __COMPILE_104
#define __COMPILE_105
#define __COMPILE_106
#define __COMPILE_107
#define __COMPILE_108
#define __COMPILE_109
#define __COMPILE_110
#define __COMPILE_111
#define __COMPILE_112
#define __COMPILE_113
#define __COMPILE_114
#define __COMPILE_115
#define __COMPILE_116
#define __COMPILE_117
#define __COMPILE_118
#define __COMPILE_119
#define __COMPILE_120
#define __COMPILE_121
#define __COMPILE_122
#define __COMPILE_123
#define __COMPILE_124
#define __COMPILE_125
#define __COMPILE_126
#define __COMPILE_127
#define __COMPILE_128
#define __COMPILE_129
#define __COMPILE_130
#define __COMPILE_131
#define __COMPILE_132
#define __COMPILE_133
#define __COMPILE_134
#define __COMPILE_135
#define __COMPILE_136
#define __COMPILE_137
#define __COMPILE_138
#define __COMPILE_139
#define __COMPILE_145
// compile_fin
int __get_mutant(); int __is_neg(const char *location,char *lid,int count, ...); int __abst_hole(); char *__stat_write_init(const char *func_name);void __write_stat(char *str,const char *var_name,void *var_addr,int size); void __stat_file_close(const char *func_name,char *str);long long __mutate(const long long,const char *,const char *); int __trident_choice(char* lid, char* typestr,int* rvals, char** rvals_ids, int rvals_size,int** lvals, char** lvals_ids, int lvals_size);int __trident_output(char* id, char* typestr, int value);float fabs_trident(float a);int __choose(const char *);void *__var_select(unsigned int var_count,void *vars[]);void *__var_select_2(unsigned int var_count,void *vars[]);long long __const_select(unsigned int const_count, ...);
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

	//prophet generated patch
	{
	int __choose0 = __choose("__SWITCH0");
	if (__choose0 == 0)
	{}
	#ifdef __COMPILE_0
	else if (__choose0 == 1)
	{
	//AddIfStmtKind
	if (scope && !instanceof_function(scope, func->common.scope)) {
	    zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
	    scope = ((void *)0);
	}
	}
	#endif
	#ifdef __COMPILE_1
	else if (__choose0 == 2)
	{
	//AddIfStmtKind
	if (scope && this_ptr && (func->common.fn_flags & 1) == 0 && !instanceof_function(zend_get_class_entry(&(*this_ptr)), closure->func.common.scope)) {
	    zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
	    scope = ((void *)0);
	    this_ptr = ((void *)0);
	}
	}
	#endif
	#ifdef __COMPILE_2
	else if (__choose0 == 3)
	{
	//AddIfStmtKind
	if (this_ptr && (closure->func.common.fn_flags & 1) == 0) {
	    closure->this_ptr = this_ptr;
	    zval_addref_p(this_ptr);
	} else {
	    closure->func.common.fn_flags |= 1;
	    closure->this_ptr = ((void *)0);
	}
	}
	#endif
	#ifdef __COMPILE_3
	else if (__choose0 == 4)
	{
	//AddInitKind
	memset(&closure->debug_info, 0, sizeof (*(&closure->debug_info)));
	}
	#endif
	#ifdef __COMPILE_4
	else if (__choose0 == 5)
	{
	//AddInitKind
	memset(&closure->func, 0, sizeof (*(&closure->func)));
	}
	#endif
	#ifdef __COMPILE_5
	else if (__choose0 == 6)
	{
	//AddInitKind
	memset(&closure->func.common, 0, sizeof (*(&closure->func.common)));
	}
	#endif
	#ifdef __COMPILE_6
	else if (__choose0 == 7)
	{
	//AddInitKind
	memset(&closure->func.common.arg_info, 0, sizeof (*(&closure->func.common.arg_info)));
	}
	#endif
	#ifdef __COMPILE_7
	else if (__choose0 == 8)
	{
	//AddInitKind
	memset(&closure->func.common.fn_flags, 0, sizeof (*(&closure->func.common.fn_flags)));
	}
	#endif
	#ifdef __COMPILE_8
	else if (__choose0 == 9)
	{
	//AddInitKind
	memset(&closure->func.common.function_name, 0, sizeof (*(&closure->func.common.function_name)));
	}
	#endif
	#ifdef __COMPILE_9
	else if (__choose0 == 10)
	{
	//AddInitKind
	memset(&closure->func.common.num_args, 0, sizeof (*(&closure->func.common.num_args)));
	}
	#endif
	#ifdef __COMPILE_10
	else if (__choose0 == 11)
	{
	//AddInitKind
	memset(&closure->func.common.prototype, 0, sizeof (*(&closure->func.common.prototype)));
	}
	#endif
	#ifdef __COMPILE_11
	else if (__choose0 == 12)
	{
	//AddInitKind
	memset(&closure->func.common.required_num_args, 0, sizeof (*(&closure->func.common.required_num_args)));
	}
	#endif
	#ifdef __COMPILE_12
	else if (__choose0 == 13)
	{
	//AddInitKind
	memset(&closure->func.common.scope, 0, sizeof (*(&closure->func.common.scope)));
	}
	#endif
	#ifdef __COMPILE_13
	else if (__choose0 == 14)
	{
	//AddInitKind
	memset(&closure->func.common.type, 0, sizeof (*(&closure->func.common.type)));
	}
	#endif
	#ifdef __COMPILE_14
	else if (__choose0 == 15)
	{
	//AddInitKind
	memset(&closure->func.internal_function, 0, sizeof (*(&closure->func.internal_function)));
	}
	#endif
	#ifdef __COMPILE_15
	else if (__choose0 == 16)
	{
	//AddInitKind
	memset(&closure->func.op_array, 0, sizeof (*(&closure->func.op_array)));
	}
	#endif
	#ifdef __COMPILE_16
	else if (__choose0 == 17)
	{
	//AddInitKind
	memset(&closure->func.type, 0, sizeof (*(&closure->func.type)));
	}
	#endif
	#ifdef __COMPILE_17
	else if (__choose0 == 18)
	{
	//AddInitKind
	memset(&closure->std, 0, sizeof (*(&closure->std)));
	}
	#endif
	#ifdef __COMPILE_18
	else if (__choose0 == 19)
	{
	//AddInitKind
	memset(&closure->this_ptr, 0, sizeof (*(&closure->this_ptr)));
	}
	#endif
	#ifdef __COMPILE_19
	else if (__choose0 == 20)
	{
	//AddInitKind
	memset(closure, 0, sizeof (*(closure)));
	}
	#endif
	#ifdef __COMPILE_20
	else if (__choose0 == 21)
	{
	//AddInitKind
	memset(closure->debug_info, 0, sizeof (*(closure->debug_info)));
	}
	#endif
	#ifdef __COMPILE_21
	else if (__choose0 == 22)
	{
	//AddInitKind
	memset(closure->func.common.arg_info, 0, sizeof (*(closure->func.common.arg_info)));
	}
	#endif
	#ifdef __COMPILE_22
	else if (__choose0 == 23)
	{
	//AddInitKind
	memset(closure->func.common.function_name, 0, sizeof (*(closure->func.common.function_name)));
	}
	#endif
	#ifdef __COMPILE_23
	else if (__choose0 == 24)
	{
	//AddInitKind
	memset(closure->func.common.prototype, 0, sizeof (*(closure->func.common.prototype)));
	}
	#endif
	#ifdef __COMPILE_24
	else if (__choose0 == 25)
	{
	//AddInitKind
	memset(closure->func.common.scope, 0, sizeof (*(closure->func.common.scope)));
	}
	#endif
	#ifdef __COMPILE_25
	else if (__choose0 == 26)
	{
	//AddInitKind
	memset(closure->this_ptr, 0, sizeof (*(closure->this_ptr)));
	}
	#endif
	#ifdef __COMPILE_26
	else if (__choose0 == 27)
	{
	//AddStmtAndReplaceAtomKind
	(*(closure)->func.op_array.refcount)++;
	}
	#endif
	#ifdef __COMPILE_27
	else if (__choose0 == 28)
	{
	//AddStmtAndReplaceAtomKind
	(*closure && !(1 << 5)->func.op_array.refcount)++;
	}
	#endif
	#ifdef __COMPILE_28
	else if (__choose0 == 29)
	{
	//AddStmtAndReplaceAtomKind
	_array_init(((res)), 0);
	}
	#endif
	#ifdef __COMPILE_29
	else if (__choose0 == 30)
	{
	//AddStmtAndReplaceAtomKind
	_array_init(((this_ptr)), 0);
	}
	#endif
	#ifdef __COMPILE_30
	else if (__choose0 == 31)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((closure)));
	}
	#endif
	#ifdef __COMPILE_31
	else if (__choose0 == 32)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((closure)->debug_info));
	}
	#endif
	#ifdef __COMPILE_32
	else if (__choose0 == 33)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((func)));
	}
	#endif
	#ifdef __COMPILE_33
	else if (__choose0 == 34)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((func)->internal_function.function_name));
	}
	#endif
	#ifdef __COMPILE_34
	else if (__choose0 == 35)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((res)));
	}
	#endif
	#ifdef __COMPILE_35
	else if (__choose0 == 36)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((this_ptr)));
	}
	#endif
	#ifdef __COMPILE_36
	else if (__choose0 == 37)
	{
	//AddStmtAndReplaceAtomKind
	_object_init_ex(((res)), (zend_ce_closure));
	}
	#endif
	#ifdef __COMPILE_37
	else if (__choose0 == 38)
	{
	//AddStmtAndReplaceAtomKind
	_object_init_ex(((this_ptr)), (zend_ce_closure));
	}
	#endif
	#ifdef __COMPILE_38
	else if (__choose0 == 39)
	{
	//AddStmtAndReplaceAtomKind
	_object_init_ex((res && !(1 << 5)), (zend_ce_closure));
	}
	#endif
	#ifdef __COMPILE_39
	else if (__choose0 == 40)
	{
	//AddStmtAndReplaceAtomKind
	_object_init_ex((res), ((scope)));
	}
	#endif
	#ifdef __COMPILE_40
	else if (__choose0 == 41)
	{
	//AddStmtAndReplaceAtomKind
	_object_init_ex((res), (zend_ce_closure && !(1 << 5)));
	}
	#endif
	#ifdef __COMPILE_41
	else if (__choose0 == 42)
	{
	//AddStmtAndReplaceAtomKind
	_zend_hash_init(((closure)->debug_info), (1), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	}
	#endif
	#ifdef __COMPILE_42
	else if (__choose0 == 43)
	{
	//AddStmtAndReplaceAtomKind
	_zval_copy_ctor(((res)));
	}
	#endif
	#ifdef __COMPILE_43
	else if (__choose0 == 44)
	{
	//AddStmtAndReplaceAtomKind
	_zval_copy_ctor(((this_ptr)));
	}
	#endif
	#ifdef __COMPILE_44
	else if (__choose0 == 45)
	{
	//AddStmtAndReplaceAtomKind
	_zval_copy_ctor_func((res));
	}
	#endif
	#ifdef __COMPILE_45
	else if (__choose0 == 46)
	{
	//AddStmtAndReplaceAtomKind
	_zval_copy_ctor_func((this_ptr));
	}
	#endif
	#ifdef __COMPILE_46
	else if (__choose0 == 47)
	{
	//AddStmtAndReplaceAtomKind
	_zval_dtor(((res)));
	}
	#endif
	#ifdef __COMPILE_47
	else if (__choose0 == 48)
	{
	//AddStmtAndReplaceAtomKind
	_zval_dtor(((this_ptr)));
	}
	#endif
	#ifdef __COMPILE_48
	else if (__choose0 == 49)
	{
	//AddStmtAndReplaceAtomKind
	_zval_dtor_func((res));
	}
	#endif
	#ifdef __COMPILE_49
	else if (__choose0 == 50)
	{
	//AddStmtAndReplaceAtomKind
	_zval_dtor_func((this_ptr));
	}
	#endif
	#ifdef __COMPILE_50
	else if (__choose0 == 51)
	{
	//AddStmtAndReplaceAtomKind
	_zval_ptr_dtor((&(closure)->this_ptr));
	}
	#endif
	#ifdef __COMPILE_51
	else if (__choose0 == 52)
	{
	//AddStmtAndReplaceAtomKind
	_zval_ptr_dtor((&(res)));
	}
	#endif
	#ifdef __COMPILE_52
	else if (__choose0 == 53)
	{
	//AddStmtAndReplaceAtomKind
	_zval_ptr_dtor((&(this_ptr)));
	}
	#endif
	#ifdef __COMPILE_53
	else if (__choose0 == 54)
	{
	//AddStmtAndReplaceAtomKind
	closure = (closure);
	}
	#endif
	#ifdef __COMPILE_54
	else if (__choose0 == 55)
	{
	//AddStmtAndReplaceAtomKind
	closure->func = *(func);
	}
	#endif
	#ifdef __COMPILE_55
	else if (__choose0 == 56)
	{
	//AddStmtAndReplaceAtomKind
	closure->func = *func && !(1 << 5);
	}
	#endif
	#ifdef __COMPILE_56
	else if (__choose0 == 57)
	{
	//AddStmtAndReplaceAtomKind
	closure->func.common.scope = (scope);
	}
	#endif
	#ifdef __COMPILE_57
	else if (__choose0 == 58)
	{
	//AddStmtAndReplaceAtomKind
	closure->func.common.scope = scope && !(1 << 5);
	}
	#endif
	#ifdef __COMPILE_58
	else if (__choose0 == 59)
	{
	//AddStmtAndReplaceAtomKind
	closure->this_ptr = (res);
	}
	#endif
	#ifdef __COMPILE_59
	else if (__choose0 == 60)
	{
	//AddStmtAndReplaceAtomKind
	closure->this_ptr = (this_ptr);
	}
	#endif
	#ifdef __COMPILE_60
	else if (__choose0 == 61)
	{
	//AddStmtAndReplaceAtomKind
	closure->this_ptr = this_ptr && !(1 << 5);
	}
	#endif
	#ifdef __COMPILE_61
	else if (__choose0 == 62)
	{
	//AddStmtAndReplaceAtomKind
	convert_to_boolean((res));
	}
	#endif
	#ifdef __COMPILE_62
	else if (__choose0 == 63)
	{
	//AddStmtAndReplaceAtomKind
	convert_to_boolean((this_ptr));
	}
	#endif
	#ifdef __COMPILE_63
	else if (__choose0 == 64)
	{
	//AddStmtAndReplaceAtomKind
	destroy_op_array(&(closure)->func.op_array);
	}
	#endif
	#ifdef __COMPILE_64
	else if (__choose0 == 65)
	{
	//AddStmtAndReplaceAtomKind
	gc_remove_zval_from_buffer((res));
	}
	#endif
	#ifdef __COMPILE_65
	else if (__choose0 == 66)
	{
	//AddStmtAndReplaceAtomKind
	gc_remove_zval_from_buffer((this_ptr));
	}
	#endif
	#ifdef __COMPILE_66
	else if (__choose0 == 67)
	{
	//AddStmtAndReplaceAtomKind
	gc_zval_check_possible_root(((res)));
	}
	#endif
	#ifdef __COMPILE_67
	else if (__choose0 == 68)
	{
	//AddStmtAndReplaceAtomKind
	gc_zval_check_possible_root(((this_ptr)));
	}
	#endif
	#ifdef __COMPILE_68
	else if (__choose0 == 69)
	{
	//AddStmtAndReplaceAtomKind
	gc_zval_possible_root((res));
	}
	#endif
	#ifdef __COMPILE_69
	else if (__choose0 == 70)
	{
	//AddStmtAndReplaceAtomKind
	gc_zval_possible_root((this_ptr));
	}
	#endif
	#ifdef __COMPILE_70
	else if (__choose0 == 71)
	{
	//AddStmtAndReplaceAtomKind
	i_zval_ptr_dtor((res));
	}
	#endif
	#ifdef __COMPILE_71
	else if (__choose0 == 72)
	{
	//AddStmtAndReplaceAtomKind
	i_zval_ptr_dtor((this_ptr));
	}
	#endif
	#ifdef __COMPILE_72
	else if (__choose0 == 73)
	{
	//AddStmtAndReplaceAtomKind
	memcpy(&closure_handlers && !(1 << 5), zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	}
	#endif
	#ifdef __COMPILE_73
	else if (__choose0 == 74)
	{
	//AddStmtAndReplaceAtomKind
	memset((closure), 0, sizeof(zend_closure));
	}
	#endif
	#ifdef __COMPILE_74
	else if (__choose0 == 75)
	{
	//AddStmtAndReplaceAtomKind
	scope = (scope);
	}
	#endif
	#ifdef __COMPILE_75
	else if (__choose0 == 76)
	{
	//AddStmtAndReplaceAtomKind
	this_ptr = (res);
	}
	#endif
	#ifdef __COMPILE_76
	else if (__choose0 == 77)
	{
	//AddStmtAndReplaceAtomKind
	this_ptr = (this_ptr);
	}
	#endif
	#ifdef __COMPILE_77
	else if (__choose0 == 78)
	{
	//AddStmtAndReplaceAtomKind
	zend_ce_closure = (scope);
	}
	#endif
	#ifdef __COMPILE_78
	else if (__choose0 == 79)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", (func)->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
	}
	#endif
	#ifdef __COMPILE_79
	else if (__choose0 == 80)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func && !(1 << 5)->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
	}
	#endif
	#ifdef __COMPILE_80
	else if (__choose0 == 81)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, (func)->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
	}
	#endif
	#ifdef __COMPILE_81
	else if (__choose0 == 82)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func && !(1 << 5)->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
	}
	#endif
	#ifdef __COMPILE_82
	else if (__choose0 == 83)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(res)))->name);
	}
	#endif
	#ifdef __COMPILE_83
	else if (__choose0 == 84)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*(this_ptr)))->name);
	}
	#endif
	#ifdef __COMPILE_84
	else if (__choose0 == 85)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr && !(1 << 5)))->name);
	}
	#endif
	#ifdef __COMPILE_85
	else if (__choose0 == 86)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", (func)->common.scope->name, func->common.function_name, scope->name);
	}
	#endif
	#ifdef __COMPILE_86
	else if (__choose0 == 87)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func && !(1 << 5)->common.scope->name, func->common.function_name, scope->name);
	}
	#endif
	#ifdef __COMPILE_87
	else if (__choose0 == 88)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, (func)->common.function_name, scope->name);
	}
	#endif
	#ifdef __COMPILE_88
	else if (__choose0 == 89)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func && !(1 << 5)->common.function_name, scope->name);
	}
	#endif
	#ifdef __COMPILE_89
	else if (__choose0 == 90)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, (scope)->name);
	}
	#endif
	#ifdef __COMPILE_90
	else if (__choose0 == 91)
	{
	//AddStmtAndReplaceAtomKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope && !(1 << 5)->name);
	}
	#endif
	#ifdef __COMPILE_91
	else if (__choose0 == 92)
	{
	//AddStmtAndReplaceAtomKind
	zend_hash_destroy((closure)->debug_info);
	}
	#endif
	#ifdef __COMPILE_92
	else if (__choose0 == 93)
	{
	//AddStmtAndReplaceAtomKind
	zend_object_std_dtor(&(closure)->std);
	}
	#endif
	#ifdef __COMPILE_93
	else if (__choose0 == 94)
	{
	//AddStmtAndReplaceAtomKind
	zval_addref_p(&((executor_globals && !(1 << 5).uninitialized_zval)));
	}
	#endif
	#ifdef __COMPILE_94
	else if (__choose0 == 95)
	{
	//AddStmtAndReplaceAtomKind
	zval_addref_p((closure)->this_ptr);
	}
	#endif
	#ifdef __COMPILE_95
	else if (__choose0 == 96)
	{
	//AddStmtAndReplaceAtomKind
	zval_addref_p((res));
	}
	#endif
	#ifdef __COMPILE_96
	else if (__choose0 == 97)
	{
	//AddStmtAndReplaceAtomKind
	zval_addref_p((this_ptr));
	}
	#endif
	#ifdef __COMPILE_97
	else if (__choose0 == 98)
	{
	//AddStmtAndReplaceAtomKind
	zval_addref_p(this_ptr && !(1 << 5));
	}
	#endif
	#ifdef __COMPILE_98
	else if (__choose0 == 99)
	{
	//AddStmtAndReplaceAtomKind
	zval_unset_isref_p((res));
	}
	#endif
	#ifdef __COMPILE_99
	else if (__choose0 == 100)
	{
	//AddStmtAndReplaceAtomKind
	zval_unset_isref_p((this_ptr));
	}
	#endif
	#ifdef __COMPILE_100
	else if (__choose0 == 101)
	{
	//AddStmtKind
	(*closure->func.op_array.refcount)++;
	}
	#endif
	#ifdef __COMPILE_101
	else if (__choose0 == 102)
	{
	//AddStmtKind
	(closure->func.op_array.static_variables) = (HashTable *)_emalloc((sizeof(HashTable)));
	}
	#endif
	#ifdef __COMPILE_102
	else if (__choose0 == 103)
	{
	//AddStmtKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	}
	#endif
	#ifdef __COMPILE_103
	else if (__choose0 == 104)
	{
	//AddStmtKind
	_object_init_ex((res), (zend_ce_closure));
	}
	#endif
	#ifdef __COMPILE_104
	else if (__choose0 == 105)
	{
	//AddStmtKind
	closure = (zend_closure *)zend_object_store_get_object(res);
	}
	#endif
	#ifdef __COMPILE_105
	else if (__choose0 == 106)
	{
	//AddStmtKind
	closure->func = *func;
	}
	#endif
	#ifdef __COMPILE_106
	else if (__choose0 == 107)
	{
	//AddStmtKind
	closure->func.common.scope = scope;
	}
	#endif
	#ifdef __COMPILE_107
	else if (__choose0 == 108)
	{
	//AddStmtKind
	closure->func.op_array.run_time_cache = ((void *)0);
	}
	#endif
	#ifdef __COMPILE_108
	else if (__choose0 == 109)
	{
	//AddStmtKind
	closure->this_ptr = ((void *)0);
	}
	#endif
	#ifdef __COMPILE_109
	else if (__choose0 == 110)
	{
	//AddStmtKind
	closure->this_ptr = this_ptr;
	}
	#endif
	#ifdef __COMPILE_110
	else if (__choose0 == 111)
	{
	//AddStmtKind
	closure_handlers.clone_obj = zend_closure_clone;
	}
	#endif
	#ifdef __COMPILE_111
	else if (__choose0 == 112)
	{
	//AddStmtKind
	closure_handlers.compare_objects = zend_closure_compare_objects;
	}
	#endif
	#ifdef __COMPILE_112
	else if (__choose0 == 113)
	{
	//AddStmtKind
	closure_handlers.get_closure = zend_closure_get_closure;
	}
	#endif
	#ifdef __COMPILE_113
	else if (__choose0 == 114)
	{
	//AddStmtKind
	closure_handlers.get_constructor = zend_closure_get_constructor;
	}
	#endif
	#ifdef __COMPILE_114
	else if (__choose0 == 115)
	{
	//AddStmtKind
	closure_handlers.get_debug_info = zend_closure_get_debug_info;
	}
	#endif
	#ifdef __COMPILE_115
	else if (__choose0 == 116)
	{
	//AddStmtKind
	closure_handlers.get_method = zend_closure_get_method;
	}
	#endif
	#ifdef __COMPILE_116
	else if (__choose0 == 117)
	{
	//AddStmtKind
	closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
	}
	#endif
	#ifdef __COMPILE_117
	else if (__choose0 == 118)
	{
	//AddStmtKind
	closure_handlers.has_property = zend_closure_has_property;
	}
	#endif
	#ifdef __COMPILE_118
	else if (__choose0 == 119)
	{
	//AddStmtKind
	closure_handlers.read_property = zend_closure_read_property;
	}
	#endif
	#ifdef __COMPILE_119
	else if (__choose0 == 120)
	{
	//AddStmtKind
	closure_handlers.unset_property = zend_closure_unset_property;
	}
	#endif
	#ifdef __COMPILE_120
	else if (__choose0 == 121)
	{
	//AddStmtKind
	closure_handlers.write_property = zend_closure_write_property;
	}
	#endif
	#ifdef __COMPILE_121
	else if (__choose0 == 122)
	{
	//AddStmtKind
	exit(1);
	}
	#endif
	#ifdef __COMPILE_122
	else if (__choose0 == 123)
	{
	//AddStmtKind
	memcpy(&closure_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	}
	#endif
	#ifdef __COMPILE_123
	else if (__choose0 == 124)
	{
	//AddStmtKind
	scope = ((void *)0);
	}
	#endif
	#ifdef __COMPILE_124
	else if (__choose0 == 125)
	{
	//AddStmtKind
	this_ptr = ((void *)0);
	}
	#endif
	#ifdef __COMPILE_125
	else if (__choose0 == 126)
	{
	//AddStmtKind
	zend_ce_closure->create_object = zend_closure_new;
	}
	#endif
	#ifdef __COMPILE_126
	else if (__choose0 == 127)
	{
	//AddStmtKind
	zend_ce_closure->serialize = zend_class_serialize_deny;
	}
	#endif
	#ifdef __COMPILE_127
	else if (__choose0 == 128)
	{
	//AddStmtKind
	zend_ce_closure->unserialize = zend_class_unserialize_deny;
	}
	#endif
	#ifdef __COMPILE_128
	else if (__choose0 == 129)
	{
	//AddStmtKind
	zend_error((1 << 0L), "Cannot destroy active lambda function");
	}
	#endif
	#ifdef __COMPILE_129
	else if (__choose0 == 130)
	{
	//AddStmtKind
	zend_error((1 << 12L), "Cannot get arguments for calling closure");
	}
	#endif
	#ifdef __COMPILE_130
	else if (__choose0 == 131)
	{
	//AddStmtKind
	zend_error((1 << 12L), "Closure object cannot have properties");
	}
	#endif
	#ifdef __COMPILE_131
	else if (__choose0 == 132)
	{
	//AddStmtKind
	zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
	}
	#endif
	#ifdef __COMPILE_132
	else if (__choose0 == 133)
	{
	//AddStmtKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to object of class %s", func->common.scope->name, func->common.function_name, zend_get_class_entry(&(*this_ptr))->name);
	}
	#endif
	#ifdef __COMPILE_133
	else if (__choose0 == 134)
	{
	//AddStmtKind
	zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s", func->common.scope->name, func->common.function_name, scope->name);
	}
	#endif
	#ifdef __COMPILE_134
	else if (__choose0 == 135)
	{
	//AddStmtKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	}
	#endif
	#ifdef __COMPILE_135
	else if (__choose0 == 136)
	{
	//AddStmtKind
	zval_addref_p(&((executor_globals.uninitialized_zval)));
	}
	#endif
	#ifdef __COMPILE_136
	else if (__choose0 == 137)
	{
	//AddStmtKind
	zval_addref_p(this_ptr);
	}
	#endif
	#ifdef __COMPILE_137
	else if (__choose0 == 138)
	{
	//IfExitKind
	if (__is_neg("0-138", "L410", 9, &(func), sizeof (func), &(closure),
				 sizeof (closure), &(res), sizeof (res), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure),
				 sizeof (zend_ce_closure), &(scope), sizeof (scope), &(this_ptr), sizeof (this_ptr), &(closure->this_ptr),
				 sizeof (closure->this_ptr), &(closure->debug_info), sizeof (closure->debug_info)))
	    return;
	}
	#endif
	int __choose1 = __choose("__SWITCH1");
	if (__choose1 == 0)
	{
	closure->func = *func;
	
	}
	#ifdef __COMPILE_138
	else if (__choose1 == 1)
	{
	//GuardKind
	if (!__is_neg("1-1", "L410", 9, &(func), sizeof (func), &(closure), sizeof (closure), &(res), sizeof (res), &(zend_ce_closure), sizeof (zend_ce_closure), &(zend_ce_closure), sizeof (zend_ce_closure), &(scope), sizeof (scope), &(this_ptr), sizeof (this_ptr), &(closure->this_ptr), sizeof (closure->this_ptr), &(closure->debug_info), sizeof (closure->debug_info)))
	    closure->func = *func;
	
	}
	#endif
	#ifdef __COMPILE_139
	else if (__choose1 == 2)
	{
	//MSVExtRemoveStmtKind
	{
	}
	
	}
	#endif
	#ifdef __COMPILE_140
	else if (__choose1 == 3)
	{
	//ReplaceKind
	closure->func = *(&closure);
	
	}
	#endif
	#ifdef __COMPILE_141
	else if (__choose1 == 4)
	{
	//ReplaceKind
	closure->func = *(&func);
	
	}
	#endif
	#ifdef __COMPILE_142
	else if (__choose1 == 5)
	{
	//ReplaceKind
	closure->func = *(&res);
	
	}
	#endif
	#ifdef __COMPILE_143
	else if (__choose1 == 6)
	{
	//ReplaceKind
	closure->func = *(&scope);
	
	}
	#endif
	#ifdef __COMPILE_144
	else if (__choose1 == 7)
	{
	//ReplaceKind
	closure->func = *(&this_ptr);
	
	}
	#endif
	#ifdef __COMPILE_145
	else if (__choose1 == 8)
	{
	//ReplaceKind
	closure->func = *(func);
	
	}
	#endif
	#ifdef __COMPILE_146
	else if (__choose1 == 9)
	{
	//ReplaceKind
	closure->func = *func && !(1 << 5);
	
	}
	#endif
	}

	if (closure->func.type == ZEND_USER_FUNCTION) {
		if (closure->func.op_array.static_variables) {
			HashTable *static_variables = closure->func.op_array.static_variables;

			ALLOC_HASHTABLE(closure->func.op_array.static_variables);
			zend_hash_init(closure->func.op_array.static_variables, zend_hash_num_elements(static_variables), NULL, ZVAL_PTR_DTOR, 0);
			zend_hash_apply_with_arguments(static_variables TSRMLS_CC, (apply_func_args_t)zval_copy_static_var, 1, closure->func.op_array.static_variables);
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
