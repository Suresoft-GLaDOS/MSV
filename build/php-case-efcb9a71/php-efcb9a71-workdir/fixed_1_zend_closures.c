int __get_mutant();
int __is_neg(const char *, int, const int *, int, const char *, int,
             const void **, int, const double *, int, ...);
int __abst_hole();
int __choose(char *);
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
#include "zend_globals.h"
#include "zend_interfaces.h"
#include "zend_objects.h"
#include "zend_objects_API.h"

#define ZEND_CLOSURE_PRINT_NAME "Closure object"

#define ZEND_CLOSURE_PROPERTY_ERROR()                                          \
  zend_error(E_RECOVERABLE_ERROR, "Closure object cannot have properties")

typedef struct _zend_closure {
  zend_object std;
  zend_function func;
  zval *this_ptr;
  HashTable *debug_info;
} zend_closure;

/* non-static since it needs to be referenced */
ZEND_API zend_class_entry *zend_ce_closure;
static zend_object_handlers closure_handlers;

ZEND_METHOD(Closure, __invoke) /* {{{ */
{
  zend_function *func = EG(current_execute_data)->function_state.function;
  zval ***arguments;
  zval *closure_result_ptr = NULL;

  arguments = emalloc(sizeof(zval **) * ZEND_NUM_ARGS());
  if (zend_get_parameters_array_ex(ZEND_NUM_ARGS(), arguments) == FAILURE) {
    efree(arguments);
    zend_error(E_RECOVERABLE_ERROR, "Cannot get arguments for calling closure");
    RETVAL_FALSE;
  } else if (call_user_function_ex(CG(function_table), NULL, this_ptr,
                                   &closure_result_ptr, ZEND_NUM_ARGS(),
                                   arguments, 1, NULL TSRMLS_CC) == FAILURE) {
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
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(getThis() TSRMLS_CC);

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o!", &newthis) ==
      FAILURE) {
    RETURN_NULL();
  }

  zend_create_closure(return_value, &closure->func,
                      newthis ? Z_OBJCE_P(newthis) : NULL, newthis TSRMLS_CC);
}
/* }}} */

/* {{{ proto Closure Closure::bind(object $to, Closure $old)
   Create a closure to with binding to another object */
ZEND_METHOD(Closure, bind) /* {{{ */
{
  zval *newthis, *zclosure;
  zend_closure *closure;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o!O", &newthis,
                            &zclosure, zend_ce_closure) == FAILURE) {
    RETURN_NULL();
  }

  closure = (zend_closure *)zend_object_store_get_object(zclosure TSRMLS_CC);

  zend_create_closure(return_value, &closure->func,
                      newthis ? Z_OBJCE_P(newthis) : NULL, newthis TSRMLS_CC);
}
/* }}} */

static zend_function *
zend_closure_get_constructor(zval *object TSRMLS_DC) /* {{{ */
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

ZEND_API zend_function *
zend_get_closure_invoke_method(zval *obj TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(obj TSRMLS_CC);
  zend_function *invoke = (zend_function *)emalloc(sizeof(zend_function));

  invoke->common = closure->func.common;
  invoke->type = ZEND_INTERNAL_FUNCTION;
  invoke->internal_function.fn_flags =
      ZEND_ACC_PUBLIC | ZEND_ACC_CALL_VIA_HANDLER |
      (closure->func.common.fn_flags & ZEND_ACC_RETURN_REFERENCE);
  invoke->internal_function.handler = ZEND_MN(Closure___invoke);
  invoke->internal_function.module = 0;
  invoke->internal_function.scope = zend_ce_closure;
  invoke->internal_function.function_name =
      estrndup(ZEND_INVOKE_FUNC_NAME, sizeof(ZEND_INVOKE_FUNC_NAME) - 1);
  return invoke;
}
/* }}} */

ZEND_API const zend_function *
zend_get_closure_method_def(zval *obj TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(obj TSRMLS_CC);
  return &closure->func;
}
/* }}} */

ZEND_API zval *zend_get_closure_this_ptr(zval *obj TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(obj TSRMLS_CC);
  return closure->this_ptr;
}
/* }}} */

static zend_function *
zend_closure_get_method(zval **object_ptr, char *method_name, int method_len,
                        const zend_literal *key TSRMLS_DC) /* {{{ */
{
  char *lc_name;
  ALLOCA_FLAG(use_heap)

  lc_name = do_alloca(method_len + 1, use_heap);
  zend_str_tolower_copy(lc_name, method_name, method_len);
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    // prophet generated patch
    switch (__choose("__ID1")) {
    case 0: {
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#ifdef COMPILE_186
    case 1: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_187
    case 2: {
      (lc_name) =
          ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
               ? _emalloc((method_len + 1))
               : __builtin_alloca(method_len + 1));
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_188
    case 3: {
      (lc_name)++;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_189
    case 4: {
      (method_name) =
          ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
               ? _emalloc((method_len + 1))
               : __builtin_alloca(method_len + 1));
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_190
    case 5: {
      (method_name)++;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_191
    case 6: {
      _efree(((lc_name)));
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_192
    case 7: {
      _efree(((method_name)));
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_193
    case 8: {
      closure_handlers.clone_obj = zend_closure_clone;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_194
    case 9: {
      closure_handlers.compare_objects = zend_closure_compare_objects;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_195
    case 10: {
      closure_handlers.get_closure = zend_closure_get_closure;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_196
    case 11: {
      closure_handlers.get_constructor = zend_closure_get_constructor;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_197
    case 12: {
      closure_handlers.get_debug_info = zend_closure_get_debug_info;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_198
    case 13: {
      closure_handlers.get_method = zend_closure_get_method;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_199
    case 14: {
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_200
    case 15: {
      closure_handlers.has_property = zend_closure_has_property;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_201
    case 16: {
      closure_handlers.read_property = zend_closure_read_property;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_202
    case 17: {
      closure_handlers.unset_property = zend_closure_unset_property;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_203
    case 18: {
      closure_handlers.write_property = zend_closure_write_property;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_204
    case 19: {
      exit(1);
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_205
    case 20: {
      if (!__is_neg("1-20", 1, (int *){method_len}, 0, 0, 4,
                    (void **){lc_name, object_ptr, method_name, key}, 0, 0, 6,
                    "lc_name", "use_heap", "object_ptr", "method_len",
                    "method_name", "key"))
        do {
          if (__builtin_expect(use_heap, 0))
            _efree((lc_name));
        } while (0);

      break;
    }
#endif
#ifdef COMPILE_206
    case 21: {
      if (__is_neg("1-21", 1, (int *){method_len}, 0, 0, 4,
                   (void **){lc_name, object_ptr, key, method_name}, 0, 0, 6,
                   "lc_name", "use_heap", "object_ptr", "method_len", "key",
                   "method_name"))
        return 0;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_207
    case 22: {
      lc_name =
          ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
               ? _emalloc((method_len + 1))
               : __builtin_alloca(method_len + 1));
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_208
    case 23: {
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_209
    case 24: {
      zend_ce_closure->create_object = zend_closure_new;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_210
    case 25: {
      zend_ce_closure->serialize = zend_class_serialize_deny;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_211
    case 26: {
      zend_ce_closure->unserialize = zend_class_unserialize_deny;
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_212
    case 27: {
      zend_error((1 << 0L), "Cannot destroy active lambda function");
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_213
    case 28: {
      zend_error((1 << 12L), "Cannot get arguments for calling closure");
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_214
    case 29: {
      zend_error((1 << 12L), "Closure object cannot have properties");
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_215
    case 30: {
      zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_216
    case 31: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_217
    case 32: {
      zend_str_tolower_copy((lc_name), method_name, method_len);
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_218
    case 33: {
      zend_str_tolower_copy((method_name), method_name, method_len);
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_219
    case 34: {
      zend_str_tolower_copy(lc_name, (lc_name), method_len);
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_220
    case 35: {
      zend_str_tolower_copy(lc_name, (method_name), method_len);
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_221
    case 36: {
      zend_str_tolower_copy(lc_name, method_name, method_len);
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
#ifdef COMPILE_222
    case 37: {
      zval_addref_p(&((executor_globals.uninitialized_zval)));
      do {
        if (__builtin_expect(use_heap, 0))
          _efree((lc_name));
      } while (0);

      break;
    }
#endif
    }
    return zend_get_closure_invoke_method(*object_ptr TSRMLS_CC);
  }
  free_alloca(lc_name, use_heap);
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key TSRMLS_CC);
}
/* }}} */

static zval *
zend_closure_read_property(zval *object, zval *member, int type,
                           const zend_literal *key TSRMLS_DC) /* {{{ */
{
  ZEND_CLOSURE_PROPERTY_ERROR();
  Z_ADDREF(EG(uninitialized_zval));
  return &EG(uninitialized_zval);
}
/* }}} */

static void
zend_closure_write_property(zval *object, zval *member, zval *value,
                            const zend_literal *key TSRMLS_DC) /* {{{ */
{
  ZEND_CLOSURE_PROPERTY_ERROR();
}
/* }}} */

static zval **
zend_closure_get_property_ptr_ptr(zval *object, zval *member,
                                  const zend_literal *key TSRMLS_DC) /* {{{ */
{
  ZEND_CLOSURE_PROPERTY_ERROR();
  return NULL;
}
/* }}} */

static int
zend_closure_has_property(zval *object, zval *member, int has_set_exists,
                          const zend_literal *key TSRMLS_DC) /* {{{ */
{
  if (has_set_exists != 2) {
    ZEND_CLOSURE_PROPERTY_ERROR();
  }
  return 0;
}
/* }}} */

static void
zend_closure_unset_property(zval *object, zval *member,
                            const zend_literal *key TSRMLS_DC) /* {{{ */
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

static zend_object_value
zend_closure_new(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
  zend_closure *closure;
  zend_object_value object;

  closure = emalloc(sizeof(zend_closure));
  memset(closure, 0, sizeof(zend_closure));

  zend_object_std_init(&closure->std, class_type TSRMLS_CC);

  object.handle = zend_objects_store_put(
      closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
      (zend_objects_free_object_storage_t)zend_closure_free_storage,
      NULL TSRMLS_CC);
  object.handlers = &closure_handlers;

  return object;
}
/* }}} */

static zend_object_value zend_closure_clone(zval *zobject TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(zobject TSRMLS_CC);
  zval result;

  zend_create_closure(&result, &closure->func, closure->func.common.scope,
                      closure->this_ptr TSRMLS_CC);
  return Z_OBJVAL(result);
}
/* }}} */

int zend_closure_get_closure(zval *obj, zend_class_entry **ce_ptr,
                             zend_function **fptr_ptr,
                             zval **zobj_ptr TSRMLS_DC) /* {{{ */
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

static HashTable *zend_closure_get_debug_info(zval *object,
                                              int *is_temp TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(object TSRMLS_CC);
  zval *val;
  struct _zend_arg_info *arg_info = closure->func.common.arg_info;

  *is_temp = 0;

  if (closure->debug_info == NULL) {
    ALLOC_HASHTABLE(closure->debug_info);
    zend_hash_init(closure->debug_info, 1, NULL, ZVAL_PTR_DTOR, 0);
  }
  if (closure->debug_info->nApplyCount == 0) {
    if (closure->func.type == ZEND_USER_FUNCTION &&
        closure->func.op_array.static_variables) {
      HashTable *static_variables = closure->func.op_array.static_variables;
      MAKE_STD_ZVAL(val);
      array_init(val);
      zend_hash_copy(Z_ARRVAL_P(val), static_variables,
                     (copy_ctor_func_t)zval_add_ref, NULL, sizeof(zval *));
      zend_hash_update(closure->debug_info, "static", sizeof("static"),
                       (void *)&val, sizeof(zval *), NULL);
    }

    if (closure->this_ptr) {
      Z_ADDREF_P(closure->this_ptr);
      zend_symtable_update(closure->debug_info, "this", sizeof("this"),
                           (void *)&closure->this_ptr, sizeof(zval *), NULL);
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
          name_len =
              zend_spprintf(&name, 0, "%s$param%d",
                            arg_info->pass_by_reference ? "&" : "", i + 1);
        }
        info_len = zend_spprintf(&info, 0, "%s",
                                 i >= required ? "<optional>" : "<required>");
        add_assoc_stringl_ex(val, name, name_len + 1, info, info_len, 0);
        efree(name);
        arg_info++;
      }
      zend_hash_update(closure->debug_info, "parameter", sizeof("parameter"),
                       (void *)&val, sizeof(zval *), NULL);
    }
  }

  return closure->debug_info;
}
/* }}} */

/* {{{ proto Closure::__construct()
   Private constructor preventing instantiation */
ZEND_METHOD(Closure, __construct) {
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
            ZEND_ME(Closure, bind, arginfo_closure_bind,
                    ZEND_ACC_PUBLIC | ZEND_ACC_STATIC){NULL, NULL, NULL}};

void zend_register_closure_ce(TSRMLS_D) /* {{{ */
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, "Closure", closure_functions);
  zend_ce_closure = zend_register_internal_class(&ce TSRMLS_CC);
  zend_ce_closure->ce_flags |= ZEND_ACC_FINAL_CLASS;
  zend_ce_closure->create_object = zend_closure_new;
  zend_ce_closure->serialize = zend_class_serialize_deny;
  zend_ce_closure->unserialize = zend_class_unserialize_deny;

  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
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

ZEND_API void zend_create_closure(zval *res, zend_function *func,
                                  zend_class_entry *scope,
                                  zval *this_ptr TSRMLS_DC) /* {{{ */
{
  zend_closure *closure;

  object_init_ex(res, zend_ce_closure);

  closure = (zend_closure *)zend_object_store_get_object(res TSRMLS_CC);

  closure->func = *func;

  if (closure->func.type == ZEND_USER_FUNCTION) {
    if (closure->func.op_array.static_variables) {
      HashTable *static_variables = closure->func.op_array.static_variables;

      ALLOC_HASHTABLE(closure->func.op_array.static_variables);
      zend_hash_init(closure->func.op_array.static_variables,
                     zend_hash_num_elements(static_variables), NULL,
                     ZVAL_PTR_DTOR, 0);
      zend_hash_apply_with_arguments(static_variables TSRMLS_CC,
                                     (apply_func_args_t)zval_copy_static_var, 1,
                                     closure->func.op_array.static_variables);
    }
    closure->func.op_array.run_time_cache = NULL;
    (*closure->func.op_array.refcount)++;
  } else {
    /* verify that we aren't binding internal function to a wrong scope */
    if (func->common.scope != NULL) {
      if (scope && !instanceof_function(scope, func->common.scope TSRMLS_CC)) {
        zend_error(E_WARNING, "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, func->common.function_name,
                   scope->name);
        scope = NULL;
      }
      if (scope && this_ptr && (func->common.fn_flags & ZEND_ACC_STATIC) == 0 &&
          !instanceof_function(Z_OBJCE_P(this_ptr),
                               closure->func.common.scope TSRMLS_CC)) {
        zend_error(E_WARNING,
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   Z_OBJCE_P(this_ptr)->name);
        scope = NULL;
        this_ptr = NULL;
      }
    } else {
      /* if it's a free function, we won't set scope & this since they're
       * meaningless */
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
// prophet generated patch
switch (__choose("__ID2")) {
case 0: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#ifdef COMPILE_223
case 1: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_224
case 2: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    (lc_name) =
        ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
             ? _emalloc((method_len + 1))
             : __builtin_alloca(method_len + 1));
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_225
case 3: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    (lc_name)++;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_226
case 4: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    (method_name) =
        ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
             ? _emalloc((method_len + 1))
             : __builtin_alloca(method_len + 1));
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_227
case 5: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    (method_name)++;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_228
case 6: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    _efree(((lc_name)));
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_229
case 7: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    _efree(((method_name)));
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_230
case 8: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.clone_obj = zend_closure_clone;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_231
case 9: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.compare_objects = zend_closure_compare_objects;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_232
case 10: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.get_closure = zend_closure_get_closure;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_233
case 11: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.get_constructor = zend_closure_get_constructor;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_234
case 12: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_235
case 13: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.get_method = zend_closure_get_method;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_236
case 14: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_237
case 15: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.has_property = zend_closure_has_property;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_238
case 16: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.read_property = zend_closure_read_property;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_239
case 17: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.unset_property = zend_closure_unset_property;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_240
case 18: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    closure_handlers.write_property = zend_closure_write_property;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_241
case 19: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    exit(1);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_242
case 20: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    if (!__is_neg("2-20", 1, (int *){method_len}, 0, 0, 4,
                  (void **){object_ptr, lc_name, method_name, key}, 0, 0, 6,
                  "object_ptr", "lc_name", "use_heap", "method_name",
                  "method_len", "key"))
      return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_243
case 21: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    if (__is_neg("2-21", 1, (int *){method_len}, 0, 0, 4,
                 (void **){object_ptr, lc_name, method_name, key}, 0, 0, 6,
                 "object_ptr", "lc_name", "use_heap", "method_name",
                 "method_len", "key"))
      return 0;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_244
case 22: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    lc_name =
        ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
             ? _emalloc((method_len + 1))
             : __builtin_alloca(method_len + 1));
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_245
case 23: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_246
case 24: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_ce_closure->create_object = zend_closure_new;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_247
case 25: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_ce_closure->serialize = zend_class_serialize_deny;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_248
case 26: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_249
case 27: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_250
case 28: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_251
case 29: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_error((1 << 12L), "Closure object cannot have properties");
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_252
case 30: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_253
case 31: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_254
case 32: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_str_tolower_copy((lc_name), method_name, method_len);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_255
case 33: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_str_tolower_copy((method_name), method_name, method_len);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_256
case 34: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_str_tolower_copy(lc_name, (lc_name), method_len);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_257
case 35: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_str_tolower_copy(lc_name, (method_name), method_len);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_258
case 36: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zend_str_tolower_copy(lc_name, method_name, method_len);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_259
case 37: {
  if ((method_len == sizeof(ZEND_INVOKE_FUNC_NAME) - 1) &&
      memcmp(lc_name, ZEND_INVOKE_FUNC_NAME,
             sizeof(ZEND_INVOKE_FUNC_NAME) - 1) == 0) {
    free_alloca(lc_name, use_heap);
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_260
case 38: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_261
case 39: {
  (lc_name) =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_262
case 40: {
  (lc_name)++;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_263
case 41: {
  (method_name) =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_264
case 42: {
  (method_name)++;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_265
case 43: {
  _efree(((lc_name)));
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_266
case 44: {
  _efree(((method_name)));
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_267
case 45: {
  closure_handlers.clone_obj = zend_closure_clone;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_268
case 46: {
  closure_handlers.compare_objects = zend_closure_compare_objects;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_269
case 47: {
  closure_handlers.get_closure = zend_closure_get_closure;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_270
case 48: {
  closure_handlers.get_constructor = zend_closure_get_constructor;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_271
case 49: {
  closure_handlers.get_debug_info = zend_closure_get_debug_info;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_272
case 50: {
  closure_handlers.get_method = zend_closure_get_method;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_273
case 51: {
  closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_274
case 52: {
  closure_handlers.has_property = zend_closure_has_property;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_275
case 53: {
  closure_handlers.read_property = zend_closure_read_property;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_276
case 54: {
  closure_handlers.unset_property = zend_closure_unset_property;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_277
case 55: {
  closure_handlers.write_property = zend_closure_write_property;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_278
case 56: {
  exit(1);
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_279
case 57: {
  if (((method_len == sizeof("__invoke") - 1) &&
       memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) &&
      !__is_neg("2-57", 1, (int *){method_len}, 0, 0, 4,
                (void **){lc_name, method_name, object_ptr, key}, 0, 0, 6,
                "lc_name", "method_len", "method_name", "use_heap",
                "object_ptr", "key")) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_280
case 58: {
  lc_name =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_281
case 59: {
  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_282
case 60: {
  zend_ce_closure->create_object = zend_closure_new;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_283
case 61: {
  zend_ce_closure->serialize = zend_class_serialize_deny;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_284
case 62: {
  zend_ce_closure->unserialize = zend_class_unserialize_deny;
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_285
case 63: {
  zend_error((1 << 0L), "Cannot destroy active lambda function");
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_286
case 64: {
  zend_error((1 << 12L), "Cannot get arguments for calling closure");
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_287
case 65: {
  zend_error((1 << 12L), "Closure object cannot have properties");
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_288
case 66: {
  zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_289
case 67: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_290
case 68: {
  zend_str_tolower_copy((lc_name), method_name, method_len);
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_291
case 69: {
  zend_str_tolower_copy((method_name), method_name, method_len);
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_292
case 70: {
  zend_str_tolower_copy(lc_name, (lc_name), method_len);
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_293
case 71: {
  zend_str_tolower_copy(lc_name, (method_name), method_len);
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_294
case 72: {
  zend_str_tolower_copy(lc_name, method_name, method_len);
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
#ifdef COMPILE_295
case 73: {
  zval_addref_p(&((executor_globals.uninitialized_zval)));
  if ((method_len == sizeof("__invoke") - 1) &&
      memcmp(lc_name, "__invoke", sizeof("__invoke") - 1) == 0) {
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);
    return zend_get_closure_invoke_method(*object_ptr);
  }

  break;
}
#endif
}
// prophet generated patch
switch (__choose("__ID3")) {
case 0: {
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#ifdef COMPILE_296
case 1: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_297
case 2: {
  (lc_name) =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_298
case 3: {
  (lc_name)++;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_299
case 4: {
  (method_name) =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_300
case 5: {
  (method_name)++;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_301
case 6: {
  _efree(((lc_name)));
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_302
case 7: {
  _efree(((method_name)));
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_303
case 8: {
  closure_handlers.clone_obj = zend_closure_clone;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_304
case 9: {
  closure_handlers.compare_objects = zend_closure_compare_objects;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_305
case 10: {
  closure_handlers.get_closure = zend_closure_get_closure;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_306
case 11: {
  closure_handlers.get_constructor = zend_closure_get_constructor;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_307
case 12: {
  closure_handlers.get_debug_info = zend_closure_get_debug_info;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_308
case 13: {
  closure_handlers.get_method = zend_closure_get_method;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_309
case 14: {
  closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_310
case 15: {
  closure_handlers.has_property = zend_closure_has_property;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_311
case 16: {
  closure_handlers.read_property = zend_closure_read_property;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_312
case 17: {
  closure_handlers.unset_property = zend_closure_unset_property;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_313
case 18: {
  closure_handlers.write_property = zend_closure_write_property;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_314
case 19: {
  exit(1);
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_315
case 20: {
  if (!__is_neg("3-20", 1, (int *){method_len}, 0, 0, 4,
                (void **){lc_name, object_ptr, method_name, key}, 0, 0, 6,
                "lc_name", "use_heap", "object_ptr", "method_name",
                "method_len", "key"))
    do {
      if (__builtin_expect(use_heap, 0))
        _efree((lc_name));
    } while (0);

  break;
}
#endif
#ifdef COMPILE_316
case 21: {
  if (__is_neg("3-21", 1, (int *){method_len}, 0, 0, 4,
               (void **){lc_name, object_ptr, method_name, key}, 0, 0, 6,
               "lc_name", "use_heap", "object_ptr", "method_name", "method_len",
               "key"))
    return 0;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_317
case 22: {
  lc_name =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_318
case 23: {
  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_319
case 24: {
  zend_ce_closure->create_object = zend_closure_new;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_320
case 25: {
  zend_ce_closure->serialize = zend_class_serialize_deny;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_321
case 26: {
  zend_ce_closure->unserialize = zend_class_unserialize_deny;
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_322
case 27: {
  zend_error((1 << 0L), "Cannot destroy active lambda function");
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_323
case 28: {
  zend_error((1 << 12L), "Cannot get arguments for calling closure");
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_324
case 29: {
  zend_error((1 << 12L), "Closure object cannot have properties");
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_325
case 30: {
  zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_326
case 31: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_327
case 32: {
  zend_str_tolower_copy((lc_name), method_name, method_len);
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_328
case 33: {
  zend_str_tolower_copy((method_name), method_name, method_len);
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_329
case 34: {
  zend_str_tolower_copy(lc_name, (lc_name), method_len);
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_330
case 35: {
  zend_str_tolower_copy(lc_name, (method_name), method_len);
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_331
case 36: {
  zend_str_tolower_copy(lc_name, method_name, method_len);
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
#ifdef COMPILE_332
case 37: {
  zval_addref_p(&((executor_globals.uninitialized_zval)));
  do {
    if (__builtin_expect(use_heap, 0))
      _efree((lc_name));
  } while (0);

  break;
}
#endif
}
// prophet generated patch
switch (__choose("__ID4")) {
case 0: {
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#ifdef COMPILE_333
case 1: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_334
case 2: {
  (lc_name) =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_335
case 3: {
  (lc_name)++;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_336
case 4: {
  (method_name) =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_337
case 5: {
  (method_name)++;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_338
case 6: {
  _efree(((lc_name)));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_339
case 7: {
  _efree(((method_name)));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_340
case 8: {
  closure_handlers.clone_obj = zend_closure_clone;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_341
case 9: {
  closure_handlers.compare_objects = zend_closure_compare_objects;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_342
case 10: {
  closure_handlers.get_closure = zend_closure_get_closure;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_343
case 11: {
  closure_handlers.get_constructor = zend_closure_get_constructor;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_344
case 12: {
  closure_handlers.get_debug_info = zend_closure_get_debug_info;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_345
case 13: {
  closure_handlers.get_method = zend_closure_get_method;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_346
case 14: {
  closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_347
case 15: {
  closure_handlers.has_property = zend_closure_has_property;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_348
case 16: {
  closure_handlers.read_property = zend_closure_read_property;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_349
case 17: {
  closure_handlers.unset_property = zend_closure_unset_property;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_350
case 18: {
  closure_handlers.write_property = zend_closure_write_property;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_351
case 19: {
  exit(1);
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_352
case 20: {
  if (!__is_neg("4-20", 1, (int *){method_len}, 0, 0, 4,
                (void **){object_ptr, method_name, key, lc_name}, 0, 0, 6,
                "object_ptr", "method_name", "method_len", "key", "lc_name",
                "use_heap"))
    return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                          key);

  break;
}
#endif
#ifdef COMPILE_353
case 21: {
  if (__is_neg("4-21", 1, (int *){method_len}, 0, 0, 4,
               (void **){object_ptr, method_name, key, lc_name}, 0, 0, 6,
               "object_ptr", "method_name", "method_len", "key", "lc_name",
               "use_heap"))
    return 0;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_354
case 22: {
  lc_name =
      ((use_heap = (__builtin_expect((method_len + 1) > ((32 * 1024)), 0)))
           ? _emalloc((method_len + 1))
           : __builtin_alloca(method_len + 1));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_355
case 23: {
  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_356
case 24: {
  memset(&std_object_handlers, 0, sizeof(*(&std_object_handlers)));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_357
case 25: {
  zend_ce_closure->create_object = zend_closure_new;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_358
case 26: {
  zend_ce_closure->serialize = zend_class_serialize_deny;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_359
case 27: {
  zend_ce_closure->unserialize = zend_class_unserialize_deny;
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_360
case 28: {
  zend_error((1 << 0L), "Cannot destroy active lambda function");
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_361
case 29: {
  zend_error((1 << 12L), "Cannot get arguments for calling closure");
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_362
case 30: {
  zend_error((1 << 12L), "Closure object cannot have properties");
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_363
case 31: {
  zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_364
case 32: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_365
case 33: {
  zend_str_tolower_copy((lc_name), method_name, method_len);
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_366
case 34: {
  zend_str_tolower_copy((method_name), method_name, method_len);
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_367
case 35: {
  zend_str_tolower_copy(lc_name, (lc_name), method_len);
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_368
case 36: {
  zend_str_tolower_copy(lc_name, (method_name), method_len);
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_369
case 37: {
  zend_str_tolower_copy(lc_name, method_name, method_len);
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
#ifdef COMPILE_370
case 38: {
  zval_addref_p(&((executor_globals.uninitialized_zval)));
  return std_object_handlers.get_method(object_ptr, method_name, method_len,
                                        key);

  break;
}
#endif
}
}
/* }}} */

static zval *
zend_closure_read_property(zval *object, zval *member, int type,
                           const zend_literal *key TSRMLS_DC) /* {{{ */
{
  // prophet generated patch
  switch (__choose("__ID5")) {
  case 0: {
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#ifdef COMPILE_371
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_372
  case 2: {
    _IO_putc((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_373
  case 3: {
    _array_init(((member)), 0);
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_374
  case 4: {
    _array_init(((object)), 0);
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_375
  case 5: {
    _efree(((member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_376
  case 6: {
    _efree(((object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_377
  case 7: {
    _object_init_ex(((member)), (zend_ce_closure));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_378
  case 8: {
    _object_init_ex(((object)), (zend_ce_closure));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_379
  case 9: {
    _zend_get_parameters_array_ex((1 << 12L),
                                  "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_380
  case 10: {
    _zend_list_find((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_381
  case 11: {
    _zval_copy_ctor(((member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_382
  case 12: {
    _zval_copy_ctor(((object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_383
  case 13: {
    _zval_copy_ctor_func((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_384
  case 14: {
    _zval_copy_ctor_func((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_385
  case 15: {
    _zval_dtor(((member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_386
  case 16: {
    _zval_dtor(((object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_387
  case 17: {
    _zval_dtor_func((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_388
  case 18: {
    _zval_dtor_func((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_389
  case 19: {
    _zval_ptr_dtor((&(member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_390
  case 20: {
    _zval_ptr_dtor((&(object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_391
  case 21: {
    closure_handlers.clone_obj = zend_closure_clone;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_392
  case 22: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_393
  case 23: {
    closure_handlers.get_closure = zend_closure_get_closure;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_394
  case 24: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_395
  case 25: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_396
  case 26: {
    closure_handlers.get_method = zend_closure_get_method;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_397
  case 27: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_398
  case 28: {
    closure_handlers.has_property = zend_closure_has_property;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_399
  case 29: {
    closure_handlers.read_property = zend_closure_read_property;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_400
  case 30: {
    closure_handlers.unset_property = zend_closure_unset_property;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_401
  case 31: {
    closure_handlers.write_property = zend_closure_write_property;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_402
  case 32: {
    compile_filename((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_403
  case 33: {
    convert_to_boolean((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_404
  case 34: {
    convert_to_boolean((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_405
  case 35: {
    dprintf((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_406
  case 36: {
    exit(1);
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_407
  case 37: {
    fdopen((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_408
  case 38: {
    fputc((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_409
  case 39: {
    fputc_unlocked((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_410
  case 40: {
    gc_remove_zval_from_buffer((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_411
  case 41: {
    gc_remove_zval_from_buffer((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_412
  case 42: {
    gc_zval_check_possible_root(((member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_413
  case 43: {
    gc_zval_check_possible_root(((object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_414
  case 44: {
    gc_zval_possible_root((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_415
  case 45: {
    gc_zval_possible_root((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_416
  case 46: {
    i_zval_ptr_dtor((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_417
  case 47: {
    i_zval_ptr_dtor((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_418
  case 48: {
    if (!__is_neg("5-48", 0, 0, 0, 0, 0, 0, 0, 0, 0))
      zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_419
  case 49: {
    if (__is_neg("5-49", 0, 0, 0, 0, 0, 0, 0, 0, 0))
      return 0;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_420
  case 50: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_421
  case 51: {
    multi_convert_to_double_ex((1 << 12L),
                               "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_422
  case 52: {
    multi_convert_to_long_ex((1 << 12L),
                             "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_423
  case 53: {
    multi_convert_to_string_ex((1 << 12L),
                               "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_424
  case 54: {
    putc((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_425
  case 55: {
    putc_unlocked((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_426
  case 56: {
    putw((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_427
  case 57: {
    strerror_l((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_428
  case 58: {
    ungetc((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_429
  case 59: {
    zend_ce_closure->create_object = zend_closure_new;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_430
  case 60: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_431
  case 61: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_432
  case 62: {
    zend_copy_parameters_array((1 << 12L),
                               "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_433
  case 63: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_434
  case 64: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_435
  case 65: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_436
  case 66: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_437
  case 67: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_438
  case 68: {
    zend_error_noreturn((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_439
  case 69: {
    zend_get_parameters_ex((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_440
  case 70: {
    zend_parse_parameters((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_441
  case 71: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_442
  case 72: {
    zval_addref_p((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_443
  case 73: {
    zval_addref_p((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_444
  case 74: {
    zval_unset_isref_p((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_445
  case 75: {
    zval_unset_isref_p((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID6")) {
  case 0: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#ifdef COMPILE_446
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_447
  case 2: {
    _array_init(((member)), 0);
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_448
  case 3: {
    _array_init(((object)), 0);
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_449
  case 4: {
    _convert_to_string(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_450
  case 5: {
    _efree(((member)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_451
  case 6: {
    _efree(((object)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_452
  case 7: {
    _object_init(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_453
  case 8: {
    _object_init_ex(((member)), (zend_ce_closure));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_454
  case 9: {
    _object_init_ex(((object)), (zend_ce_closure));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_455
  case 10: {
    _zval_copy_ctor(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_456
  case 11: {
    _zval_copy_ctor(((member)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_457
  case 12: {
    _zval_copy_ctor(((object)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_458
  case 13: {
    _zval_copy_ctor_func(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_459
  case 14: {
    _zval_copy_ctor_func((member));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_460
  case 15: {
    _zval_copy_ctor_func((object));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_461
  case 16: {
    _zval_dtor(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_462
  case 17: {
    _zval_dtor(((member)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_463
  case 18: {
    _zval_dtor(((object)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_464
  case 19: {
    _zval_dtor_func(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_465
  case 20: {
    _zval_dtor_func((member));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_466
  case 21: {
    _zval_dtor_func((object));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_467
  case 22: {
    _zval_dtor_wrapper(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_468
  case 23: {
    _zval_internal_dtor(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_469
  case 24: {
    _zval_ptr_dtor((&(member)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_470
  case 25: {
    _zval_ptr_dtor((&(object)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_471
  case 26: {
    add_next_index_null(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_472
  case 27: {
    closure_handlers.clone_obj = zend_closure_clone;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_473
  case 28: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_474
  case 29: {
    closure_handlers.get_closure = zend_closure_get_closure;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_475
  case 30: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_476
  case 31: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_477
  case 32: {
    closure_handlers.get_method = zend_closure_get_method;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_478
  case 33: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_479
  case 34: {
    closure_handlers.has_property = zend_closure_has_property;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_480
  case 35: {
    closure_handlers.read_property = zend_closure_read_property;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_481
  case 36: {
    closure_handlers.unset_property = zend_closure_unset_property;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_482
  case 37: {
    closure_handlers.write_property = zend_closure_write_property;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_483
  case 38: {
    convert_scalar_to_number(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_484
  case 39: {
    convert_to_array(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_485
  case 40: {
    convert_to_boolean(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_486
  case 41: {
    convert_to_boolean((member));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_487
  case 42: {
    convert_to_boolean((object));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_488
  case 43: {
    convert_to_double(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_489
  case 44: {
    convert_to_long(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_490
  case 45: {
    convert_to_null(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_491
  case 46: {
    convert_to_object(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_492
  case 47: {
    decrement_function(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_493
  case 48: {
    exit(1);
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_494
  case 49: {
    gc_remove_zval_from_buffer(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_495
  case 50: {
    gc_remove_zval_from_buffer((member));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_496
  case 51: {
    gc_remove_zval_from_buffer((object));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_497
  case 52: {
    gc_zobj_possible_root(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_498
  case 53: {
    gc_zval_check_possible_root(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_499
  case 54: {
    gc_zval_check_possible_root(((member)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_500
  case 55: {
    gc_zval_check_possible_root(((object)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_501
  case 56: {
    gc_zval_possible_root(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_502
  case 57: {
    gc_zval_possible_root((member));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_503
  case 58: {
    gc_zval_possible_root((object));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_504
  case 59: {
    i_zend_is_true(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_505
  case 60: {
    i_zval_ptr_dtor(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_506
  case 61: {
    i_zval_ptr_dtor((member));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_507
  case 62: {
    i_zval_ptr_dtor((object));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_508
  case 63: {
    if (!__is_neg("6-63", 0, 0, 0, 0, 0, 0, 0, 0, 0))
      zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_509
  case 64: {
    if (__is_neg("6-64", 0, 0, 0, 0, 0, 0, 0, 0, 0))
      return 0;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_510
  case 65: {
    increment_function(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_511
  case 66: {
    lex_scan(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_512
  case 67: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_513
  case 68: {
    memset(&executor_globals, 0, sizeof(*(&executor_globals)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_514
  case 69: {
    memset(&executor_globals.uninitialized_zval, 0,
           sizeof(*(&executor_globals.uninitialized_zval)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_515
  case 70: {
    safe_free_zval_ptr_rel(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_516
  case 71: {
    zend_ce_closure->create_object = zend_closure_new;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_517
  case 72: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_518
  case 73: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_519
  case 74: {
    zend_closure_clone(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_520
  case 75: {
    zend_closure_get_constructor(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_521
  case 76: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_522
  case 77: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_523
  case 78: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_524
  case 79: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_525
  case 80: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_526
  case 81: {
    zend_get_closure_invoke_method(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_527
  case 82: {
    zend_get_closure_method_def(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_528
  case 83: {
    zend_get_closure_this_ptr(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_529
  case 84: {
    zend_is_true(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_530
  case 85: {
    zend_locale_sprintf_double(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_531
  case 86: {
    zend_object_store_ctor_failed(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_532
  case 87: {
    zend_objects_clone_obj(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_533
  case 88: {
    zend_objects_store_add_ref(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_534
  case 89: {
    zend_objects_store_clone_obj(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_535
  case 90: {
    zend_objects_store_del_ref(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_536
  case 91: {
    zend_objects_store_get_refcount(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_537
  case 92: {
    zend_print_flat_zval_r(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_538
  case 93: {
    zend_print_variable(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_539
  case 94: {
    zend_std_get_constructor(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_540
  case 95: {
    zend_std_get_properties(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_541
  case 96: {
    zend_throw_exception_internal(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_542
  case 97: {
    zend_throw_exception_object(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_543
  case 98: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_544
  case 99: {
    zval_addref_p((member));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_545
  case 100: {
    zval_addref_p((object));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_546
  case 101: {
    zval_delref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_547
  case 102: {
    zval_is_true(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_548
  case 103: {
    zval_isref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_549
  case 104: {
    zval_refcount_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_550
  case 105: {
    zval_set_isref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_551
  case 106: {
    zval_unset_isref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_552
  case 107: {
    zval_unset_isref_p((member));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
#ifdef COMPILE_553
  case 108: {
    zval_unset_isref_p((object));
    zval_addref_p(&((executor_globals.uninitialized_zval)));

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID7")) {
  case 0: {
    return &(executor_globals.uninitialized_zval);

    break;
  }
#ifdef COMPILE_554
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_555
  case 2: {
    _array_init(((member)), 0);
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_556
  case 3: {
    _array_init(((object)), 0);
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_557
  case 4: {
    _efree(((member)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_558
  case 5: {
    _efree(((object)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_559
  case 6: {
    _object_init_ex(((member)), (zend_ce_closure));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_560
  case 7: {
    _object_init_ex(((object)), (zend_ce_closure));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_561
  case 8: {
    _zval_copy_ctor(((member)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_562
  case 9: {
    _zval_copy_ctor(((object)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_563
  case 10: {
    _zval_copy_ctor_func((member));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_564
  case 11: {
    _zval_copy_ctor_func((object));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_565
  case 12: {
    _zval_dtor(((member)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_566
  case 13: {
    _zval_dtor(((object)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_567
  case 14: {
    _zval_dtor_func((member));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_568
  case 15: {
    _zval_dtor_func((object));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_569
  case 16: {
    _zval_ptr_dtor((&(member)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_570
  case 17: {
    _zval_ptr_dtor((&(object)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_571
  case 18: {
    closure_handlers.clone_obj = zend_closure_clone;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_572
  case 19: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_573
  case 20: {
    closure_handlers.get_closure = zend_closure_get_closure;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_574
  case 21: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_575
  case 22: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_576
  case 23: {
    closure_handlers.get_method = zend_closure_get_method;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_577
  case 24: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_578
  case 25: {
    closure_handlers.has_property = zend_closure_has_property;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_579
  case 26: {
    closure_handlers.read_property = zend_closure_read_property;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_580
  case 27: {
    closure_handlers.unset_property = zend_closure_unset_property;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_581
  case 28: {
    closure_handlers.write_property = zend_closure_write_property;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_582
  case 29: {
    convert_to_boolean((member));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_583
  case 30: {
    convert_to_boolean((object));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_584
  case 31: {
    exit(1);
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_585
  case 32: {
    gc_remove_zval_from_buffer((member));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_586
  case 33: {
    gc_remove_zval_from_buffer((object));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_587
  case 34: {
    gc_zval_check_possible_root(((member)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_588
  case 35: {
    gc_zval_check_possible_root(((object)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_589
  case 36: {
    gc_zval_possible_root((member));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_590
  case 37: {
    gc_zval_possible_root((object));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_591
  case 38: {
    i_zval_ptr_dtor((member));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_592
  case 39: {
    i_zval_ptr_dtor((object));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_593
  case 40: {
    if (!__is_neg("7-40", 0, 0, 0, 0, 0, 0, 0, 0, 0))
      return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_594
  case 41: {
    if (__is_neg("7-41", 0, 0, 0, 0, 0, 0, 0, 0, 0))
      return 0;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_595
  case 42: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_596
  case 43: {
    memset(&executor_globals, 0, sizeof(*(&executor_globals)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_597
  case 44: {
    memset(&executor_globals.uninitialized_zval, 0,
           sizeof(*(&executor_globals.uninitialized_zval)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_598
  case 45: {
    zend_ce_closure->create_object = zend_closure_new;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_599
  case 46: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_600
  case 47: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_601
  case 48: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_602
  case 49: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_603
  case 50: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_604
  case 51: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_605
  case 52: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_606
  case 53: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_607
  case 54: {
    zval_addref_p((member));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_608
  case 55: {
    zval_addref_p((object));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_609
  case 56: {
    zval_unset_isref_p((member));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
#ifdef COMPILE_610
  case 57: {
    zval_unset_isref_p((object));
    return &(executor_globals.uninitialized_zval);

    break;
  }
#endif
  }
}
/* }}} */

static void
zend_closure_write_property(zval *object, zval *member, zval *value,
                            const zend_literal *key TSRMLS_DC) /* {{{ */
{
  ZEND_CLOSURE_PROPERTY_ERROR();
}
/* }}} */

static zval **
zend_closure_get_property_ptr_ptr(zval *object, zval *member,
                                  const zend_literal *key TSRMLS_DC) /* {{{ */
{
  ZEND_CLOSURE_PROPERTY_ERROR();
  return NULL;
}
/* }}} */

static int
zend_closure_has_property(zval *object, zval *member, int has_set_exists,
                          const zend_literal *key TSRMLS_DC) /* {{{ */
{
  // prophet generated patch
  switch (__choose("__ID8")) {
  case 0: {
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#ifdef COMPILE_611
  case 1: {
    if (has_set_exists != 2) {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_612
  case 2: {
    if (has_set_exists != 2) {
      _IO_putc((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_613
  case 3: {
    if (has_set_exists != 2) {
      _array_init(((member)), 0);
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_614
  case 4: {
    if (has_set_exists != 2) {
      _array_init(((object)), 0);
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_615
  case 5: {
    if (has_set_exists != 2) {
      _efree(((member)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_616
  case 6: {
    if (has_set_exists != 2) {
      _efree(((object)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_617
  case 7: {
    if (has_set_exists != 2) {
      _object_init_ex(((member)), (zend_ce_closure));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_618
  case 8: {
    if (has_set_exists != 2) {
      _object_init_ex(((object)), (zend_ce_closure));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_619
  case 9: {
    if (has_set_exists != 2) {
      _zend_get_parameters_array_ex((1 << 12L),
                                    "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_620
  case 10: {
    if (has_set_exists != 2) {
      _zend_list_find((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_621
  case 11: {
    if (has_set_exists != 2) {
      _zval_copy_ctor(((member)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_622
  case 12: {
    if (has_set_exists != 2) {
      _zval_copy_ctor(((object)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_623
  case 13: {
    if (has_set_exists != 2) {
      _zval_copy_ctor_func((member));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_624
  case 14: {
    if (has_set_exists != 2) {
      _zval_copy_ctor_func((object));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_625
  case 15: {
    if (has_set_exists != 2) {
      _zval_dtor(((member)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_626
  case 16: {
    if (has_set_exists != 2) {
      _zval_dtor(((object)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_627
  case 17: {
    if (has_set_exists != 2) {
      _zval_dtor_func((member));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_628
  case 18: {
    if (has_set_exists != 2) {
      _zval_dtor_func((object));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_629
  case 19: {
    if (has_set_exists != 2) {
      _zval_ptr_dtor((&(member)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_630
  case 20: {
    if (has_set_exists != 2) {
      _zval_ptr_dtor((&(object)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_631
  case 21: {
    if (has_set_exists != 2) {
      closure_handlers.clone_obj = zend_closure_clone;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_632
  case 22: {
    if (has_set_exists != 2) {
      closure_handlers.compare_objects = zend_closure_compare_objects;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_633
  case 23: {
    if (has_set_exists != 2) {
      closure_handlers.get_closure = zend_closure_get_closure;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_634
  case 24: {
    if (has_set_exists != 2) {
      closure_handlers.get_constructor = zend_closure_get_constructor;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_635
  case 25: {
    if (has_set_exists != 2) {
      closure_handlers.get_debug_info = zend_closure_get_debug_info;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_636
  case 26: {
    if (has_set_exists != 2) {
      closure_handlers.get_method = zend_closure_get_method;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_637
  case 27: {
    if (has_set_exists != 2) {
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_638
  case 28: {
    if (has_set_exists != 2) {
      closure_handlers.has_property = zend_closure_has_property;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_639
  case 29: {
    if (has_set_exists != 2) {
      closure_handlers.read_property = zend_closure_read_property;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_640
  case 30: {
    if (has_set_exists != 2) {
      closure_handlers.unset_property = zend_closure_unset_property;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_641
  case 31: {
    if (has_set_exists != 2) {
      closure_handlers.write_property = zend_closure_write_property;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_642
  case 32: {
    if (has_set_exists != 2) {
      compile_filename((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_643
  case 33: {
    if (has_set_exists != 2) {
      convert_to_boolean((member));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_644
  case 34: {
    if (has_set_exists != 2) {
      convert_to_boolean((object));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_645
  case 35: {
    if (has_set_exists != 2) {
      dprintf((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_646
  case 36: {
    if (has_set_exists != 2) {
      exit(1);
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_647
  case 37: {
    if (has_set_exists != 2) {
      fdopen((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_648
  case 38: {
    if (has_set_exists != 2) {
      fputc((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_649
  case 39: {
    if (has_set_exists != 2) {
      fputc_unlocked((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_650
  case 40: {
    if (has_set_exists != 2) {
      gc_remove_zval_from_buffer((member));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_651
  case 41: {
    if (has_set_exists != 2) {
      gc_remove_zval_from_buffer((object));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_652
  case 42: {
    if (has_set_exists != 2) {
      gc_zval_check_possible_root(((member)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_653
  case 43: {
    if (has_set_exists != 2) {
      gc_zval_check_possible_root(((object)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_654
  case 44: {
    if (has_set_exists != 2) {
      gc_zval_possible_root((member));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_655
  case 45: {
    if (has_set_exists != 2) {
      gc_zval_possible_root((object));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_656
  case 46: {
    if (has_set_exists != 2) {
      i_zval_ptr_dtor((member));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_657
  case 47: {
    if (has_set_exists != 2) {
      i_zval_ptr_dtor((object));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_658
  case 48: {
    if (has_set_exists != 2) {
      if (!__is_neg("8-48", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                    "has_set_exists"))
        zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_659
  case 49: {
    if (has_set_exists != 2) {
      if (__is_neg("8-49", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                   "has_set_exists"))
        return 0;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_660
  case 50: {
    if (has_set_exists != 2) {
      if (__is_neg("8-50", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                   "has_set_exists"))
        return 12;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_661
  case 51: {
    if (has_set_exists != 2) {
      if (__is_neg("8-51", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                   "has_set_exists"))
        return 1;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_662
  case 52: {
    if (has_set_exists != 2) {
      if (__is_neg("8-52", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                   "has_set_exists"))
        return 2;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_663
  case 53: {
    if (has_set_exists != 2) {
      if (has_set_exists != 2) {
        zend_error((1 << 12L), "Closure object cannot have properties");
      }
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_664
  case 54: {
    if (has_set_exists != 2) {
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_665
  case 55: {
    if (has_set_exists != 2) {
      multi_convert_to_double_ex((1 << 12L),
                                 "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_666
  case 56: {
    if (has_set_exists != 2) {
      multi_convert_to_long_ex((1 << 12L),
                               "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_667
  case 57: {
    if (has_set_exists != 2) {
      multi_convert_to_string_ex((1 << 12L),
                                 "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_668
  case 58: {
    if (has_set_exists != 2) {
      putc((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_669
  case 59: {
    if (has_set_exists != 2) {
      putc_unlocked((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_670
  case 60: {
    if (has_set_exists != 2) {
      putw((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_671
  case 61: {
    if (has_set_exists != 2) {
      strerror_l((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_672
  case 62: {
    if (has_set_exists != 2) {
      ungetc((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_673
  case 63: {
    if (has_set_exists != 2) {
      zend_ce_closure->create_object = zend_closure_new;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_674
  case 64: {
    if (has_set_exists != 2) {
      zend_ce_closure->serialize = zend_class_serialize_deny;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_675
  case 65: {
    if (has_set_exists != 2) {
      zend_ce_closure->unserialize = zend_class_unserialize_deny;
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_676
  case 66: {
    if (has_set_exists != 2) {
      zend_copy_parameters_array((1 << 12L),
                                 "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_677
  case 67: {
    if (has_set_exists != 2) {
      zend_error((1 << 0L), "Cannot destroy active lambda function");
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_678
  case 68: {
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Cannot get arguments for calling closure");
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_679
  case 69: {
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_680
  case 70: {
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_681
  case 71: {
    if (has_set_exists != 2) {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_682
  case 72: {
    if (has_set_exists != 2) {
      zend_error_noreturn((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_683
  case 73: {
    if (has_set_exists != 2) {
      zend_get_parameters_ex((1 << 12L),
                             "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_684
  case 74: {
    if (has_set_exists != 2) {
      zend_parse_parameters((1 << 12L),
                            "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_685
  case 75: {
    if (has_set_exists != 2) {
      zval_addref_p(&((executor_globals.uninitialized_zval)));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_686
  case 76: {
    if (has_set_exists != 2) {
      zval_addref_p((member));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_687
  case 77: {
    if (has_set_exists != 2) {
      zval_addref_p((object));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_688
  case 78: {
    if (has_set_exists != 2) {
      zval_unset_isref_p((member));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_689
  case 79: {
    if (has_set_exists != 2) {
      zval_unset_isref_p((object));
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_690
  case 80: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_691
  case 81: {
    _array_init(((member)), 0);
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_692
  case 82: {
    _array_init(((object)), 0);
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_693
  case 83: {
    _efree(((member)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_694
  case 84: {
    _efree(((object)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_695
  case 85: {
    _object_init_ex(((member)), (zend_ce_closure));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_696
  case 86: {
    _object_init_ex(((object)), (zend_ce_closure));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_697
  case 87: {
    _zval_copy_ctor(((member)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_698
  case 88: {
    _zval_copy_ctor(((object)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_699
  case 89: {
    _zval_copy_ctor_func((member));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_700
  case 90: {
    _zval_copy_ctor_func((object));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_701
  case 91: {
    _zval_dtor(((member)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_702
  case 92: {
    _zval_dtor(((object)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_703
  case 93: {
    _zval_dtor_func((member));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_704
  case 94: {
    _zval_dtor_func((object));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_705
  case 95: {
    _zval_ptr_dtor((&(member)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_706
  case 96: {
    _zval_ptr_dtor((&(object)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_707
  case 97: {
    closure_handlers.clone_obj = zend_closure_clone;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_708
  case 98: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_709
  case 99: {
    closure_handlers.get_closure = zend_closure_get_closure;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_710
  case 100: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_711
  case 101: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_712
  case 102: {
    closure_handlers.get_method = zend_closure_get_method;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_713
  case 103: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_714
  case 104: {
    closure_handlers.has_property = zend_closure_has_property;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_715
  case 105: {
    closure_handlers.read_property = zend_closure_read_property;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_716
  case 106: {
    closure_handlers.unset_property = zend_closure_unset_property;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_717
  case 107: {
    closure_handlers.write_property = zend_closure_write_property;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_718
  case 108: {
    convert_to_boolean((member));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_719
  case 109: {
    convert_to_boolean((object));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_720
  case 110: {
    exit(1);
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_721
  case 111: {
    gc_remove_zval_from_buffer((member));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_722
  case 112: {
    gc_remove_zval_from_buffer((object));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_723
  case 113: {
    gc_zval_check_possible_root(((member)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_724
  case 114: {
    gc_zval_check_possible_root(((object)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_725
  case 115: {
    gc_zval_possible_root((member));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_726
  case 116: {
    gc_zval_possible_root((object));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_727
  case 117: {
    i_zval_ptr_dtor((member));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_728
  case 118: {
    i_zval_ptr_dtor((object));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_729
  case 119: {
    if ((has_set_exists != 2) &&
        !__is_neg("8-119", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                  "has_set_exists")) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_730
  case 120: {
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_731
  case 121: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_732
  case 122: {
    zend_ce_closure->create_object = zend_closure_new;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_733
  case 123: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_734
  case 124: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_735
  case 125: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_736
  case 126: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_737
  case 127: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_738
  case 128: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_739
  case 129: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_740
  case 130: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_741
  case 131: {
    zval_addref_p((member));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_742
  case 132: {
    zval_addref_p((object));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_743
  case 133: {
    zval_unset_isref_p((member));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
#ifdef COMPILE_744
  case 134: {
    zval_unset_isref_p((object));
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID9")) {
  case 0: {
    return 0;

    break;
  }
#ifdef COMPILE_745
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_746
  case 2: {
    _array_init(((member)), 0);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_747
  case 3: {
    _array_init(((object)), 0);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_748
  case 4: {
    _efree(((member)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_749
  case 5: {
    _efree(((object)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_750
  case 6: {
    _object_init_ex(((member)), (zend_ce_closure));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_751
  case 7: {
    _object_init_ex(((object)), (zend_ce_closure));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_752
  case 8: {
    _zval_copy_ctor(((member)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_753
  case 9: {
    _zval_copy_ctor(((object)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_754
  case 10: {
    _zval_copy_ctor_func((member));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_755
  case 11: {
    _zval_copy_ctor_func((object));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_756
  case 12: {
    _zval_dtor(((member)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_757
  case 13: {
    _zval_dtor(((object)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_758
  case 14: {
    _zval_dtor_func((member));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_759
  case 15: {
    _zval_dtor_func((object));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_760
  case 16: {
    _zval_ptr_dtor((&(member)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_761
  case 17: {
    _zval_ptr_dtor((&(object)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_762
  case 18: {
    closure_handlers.clone_obj = zend_closure_clone;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_763
  case 19: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_764
  case 20: {
    closure_handlers.get_closure = zend_closure_get_closure;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_765
  case 21: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_766
  case 22: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_767
  case 23: {
    closure_handlers.get_method = zend_closure_get_method;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_768
  case 24: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_769
  case 25: {
    closure_handlers.has_property = zend_closure_has_property;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_770
  case 26: {
    closure_handlers.read_property = zend_closure_read_property;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_771
  case 27: {
    closure_handlers.unset_property = zend_closure_unset_property;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_772
  case 28: {
    closure_handlers.write_property = zend_closure_write_property;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_773
  case 29: {
    convert_to_boolean((member));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_774
  case 30: {
    convert_to_boolean((object));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_775
  case 31: {
    exit(1);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_776
  case 32: {
    gc_remove_zval_from_buffer((member));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_777
  case 33: {
    gc_remove_zval_from_buffer((object));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_778
  case 34: {
    gc_zval_check_possible_root(((member)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_779
  case 35: {
    gc_zval_check_possible_root(((object)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_780
  case 36: {
    gc_zval_possible_root((member));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_781
  case 37: {
    gc_zval_possible_root((object));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_782
  case 38: {
    i_zval_ptr_dtor((member));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_783
  case 39: {
    i_zval_ptr_dtor((object));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_784
  case 40: {
    if (!__is_neg("9-40", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                  "has_set_exists"))
      return 0;

    break;
  }
#endif
#ifdef COMPILE_785
  case 41: {
    if (__is_neg("9-41", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                 "has_set_exists"))
      return 0;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_786
  case 42: {
    if (__is_neg("9-42", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                 "has_set_exists"))
      return 12;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_787
  case 43: {
    if (__is_neg("9-43", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                 "has_set_exists"))
      return 1;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_788
  case 44: {
    if (__is_neg("9-44", 1, (int *){has_set_exists}, 0, 0, 0, 0, 0, 0, 1,
                 "has_set_exists"))
      return 2;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_789
  case 45: {
    if (has_set_exists != 2) {
      zend_error((1 << 12L), "Closure object cannot have properties");
    }
    return 0;

    break;
  }
#endif
#ifdef COMPILE_790
  case 46: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_791
  case 47: {
    zend_ce_closure->create_object = zend_closure_new;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_792
  case 48: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_793
  case 49: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_794
  case 50: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_795
  case 51: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_796
  case 52: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_797
  case 53: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_798
  case 54: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_799
  case 55: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_800
  case 56: {
    zval_addref_p((member));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_801
  case 57: {
    zval_addref_p((object));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_802
  case 58: {
    zval_unset_isref_p((member));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_803
  case 59: {
    zval_unset_isref_p((object));
    return 0;

    break;
  }
#endif
  }
}
/* }}} */

static void
zend_closure_unset_property(zval *object, zval *member,
                            const zend_literal *key TSRMLS_DC) /* {{{ */
{
  // prophet generated patch
  switch (__choose("__ID10")) {
  case 0: {
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#ifdef COMPILE_804
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_805
  case 2: {
    _IO_putc((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_806
  case 3: {
    _array_init(((member)), 0);
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_807
  case 4: {
    _array_init(((object)), 0);
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_808
  case 5: {
    _efree(((member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_809
  case 6: {
    _efree(((object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_810
  case 7: {
    _object_init_ex(((member)), (zend_ce_closure));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_811
  case 8: {
    _object_init_ex(((object)), (zend_ce_closure));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_812
  case 9: {
    _zend_get_parameters_array_ex((1 << 12L),
                                  "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_813
  case 10: {
    _zend_list_find((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_814
  case 11: {
    _zval_copy_ctor(((member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_815
  case 12: {
    _zval_copy_ctor(((object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_816
  case 13: {
    _zval_copy_ctor_func((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_817
  case 14: {
    _zval_copy_ctor_func((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_818
  case 15: {
    _zval_dtor(((member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_819
  case 16: {
    _zval_dtor(((object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_820
  case 17: {
    _zval_dtor_func((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_821
  case 18: {
    _zval_dtor_func((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_822
  case 19: {
    _zval_ptr_dtor((&(member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_823
  case 20: {
    _zval_ptr_dtor((&(object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_824
  case 21: {
    closure_handlers.clone_obj = zend_closure_clone;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_825
  case 22: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_826
  case 23: {
    closure_handlers.get_closure = zend_closure_get_closure;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_827
  case 24: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_828
  case 25: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_829
  case 26: {
    closure_handlers.get_method = zend_closure_get_method;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_830
  case 27: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_831
  case 28: {
    closure_handlers.has_property = zend_closure_has_property;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_832
  case 29: {
    closure_handlers.read_property = zend_closure_read_property;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_833
  case 30: {
    closure_handlers.unset_property = zend_closure_unset_property;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_834
  case 31: {
    closure_handlers.write_property = zend_closure_write_property;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_835
  case 32: {
    compile_filename((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_836
  case 33: {
    convert_to_boolean((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_837
  case 34: {
    convert_to_boolean((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_838
  case 35: {
    dprintf((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_839
  case 36: {
    exit(1);
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_840
  case 37: {
    fdopen((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_841
  case 38: {
    fputc((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_842
  case 39: {
    fputc_unlocked((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_843
  case 40: {
    gc_remove_zval_from_buffer((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_844
  case 41: {
    gc_remove_zval_from_buffer((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_845
  case 42: {
    gc_zval_check_possible_root(((member)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_846
  case 43: {
    gc_zval_check_possible_root(((object)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_847
  case 44: {
    gc_zval_possible_root((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_848
  case 45: {
    gc_zval_possible_root((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_849
  case 46: {
    i_zval_ptr_dtor((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_850
  case 47: {
    i_zval_ptr_dtor((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_851
  case 48: {
    if (!__is_neg("10-48", 0, 0, 0, 0, 0, 0, 0, 0, 0))
      zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_852
  case 49: {
    if (__is_neg("10-49", 0, 0, 0, 0, 0, 0, 0, 0, 0))
      return;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_853
  case 50: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_854
  case 51: {
    multi_convert_to_double_ex((1 << 12L),
                               "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_855
  case 52: {
    multi_convert_to_long_ex((1 << 12L),
                             "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_856
  case 53: {
    multi_convert_to_string_ex((1 << 12L),
                               "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_857
  case 54: {
    putc((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_858
  case 55: {
    putc_unlocked((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_859
  case 56: {
    putw((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_860
  case 57: {
    strerror_l((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_861
  case 58: {
    ungetc((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_862
  case 59: {
    zend_ce_closure->create_object = zend_closure_new;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_863
  case 60: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_864
  case 61: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_865
  case 62: {
    zend_copy_parameters_array((1 << 12L),
                               "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_866
  case 63: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_867
  case 64: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_868
  case 65: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_869
  case 66: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_870
  case 67: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_871
  case 68: {
    zend_error_noreturn((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_872
  case 69: {
    zend_get_parameters_ex((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_873
  case 70: {
    zend_parse_parameters((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_874
  case 71: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_875
  case 72: {
    zval_addref_p((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_876
  case 73: {
    zval_addref_p((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_877
  case 74: {
    zval_unset_isref_p((member));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
#ifdef COMPILE_878
  case 75: {
    zval_unset_isref_p((object));
    zend_error((1 << 12L), "Closure object cannot have properties");

    break;
  }
#endif
  }
}
/* }}} */

static void zend_closure_free_storage(void *object TSRMLS_DC) /* {{{ */
{
  zend_closure *closure = (zend_closure *)object;

  // prophet generated patch
  switch (__choose("__ID11")) {
  case 0: {
    zend_object_std_dtor(&closure->std);

    break;
  }
#ifdef COMPILE_879
  case 1: {
    (*(closure)->func.op_array.refcount)++;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_880
  case 2: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_881
  case 3: {
    *((executor_globals.argument_stack)->top++) = (object);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_882
  case 4: {
    _efree(((closure)));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_883
  case 5: {
    _efree(((closure)->debug_info));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_884
  case 6: {
    _efree((closure));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_885
  case 7: {
    _efree((closure->debug_info));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_886
  case 8: {
    _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_887
  case 9: {
    _zval_ptr_dtor((&(closure)->this_ptr));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_888
  case 10: {
    _zval_ptr_dtor((&closure->this_ptr));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_889
  case 11: {
    closure_handlers.clone_obj = zend_closure_clone;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_890
  case 12: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_891
  case 13: {
    closure_handlers.get_closure = zend_closure_get_closure;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_892
  case 14: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_893
  case 15: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_894
  case 16: {
    closure_handlers.get_method = zend_closure_get_method;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_895
  case 17: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_896
  case 18: {
    closure_handlers.has_property = zend_closure_has_property;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_897
  case 19: {
    closure_handlers.read_property = zend_closure_read_property;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_898
  case 20: {
    closure_handlers.unset_property = zend_closure_unset_property;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_899
  case 21: {
    closure_handlers.write_property = zend_closure_write_property;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_900
  case 22: {
    destroy_op_array(&(closure)->func.op_array);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_901
  case 23: {
    destroy_op_array(&closure->func.op_array);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_902
  case 24: {
    exit(1);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_903
  case 25: {
    if (!__is_neg("11-25", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                  "closure", "object"))
      zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_904
  case 26: {
    if (__is_neg("11-26", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                 "closure", "object"))
      return;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_905
  case 27: {
    if (closure->debug_info != ((void *)0)) {
      zend_hash_destroy(closure->debug_info);
      _efree((closure->debug_info));
    }
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_906
  case 28: {
    if (closure->this_ptr) {
      _zval_ptr_dtor((&closure->this_ptr));
    }
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_907
  case 29: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_908
  case 30: {
    memset(&closure->std, 0, sizeof(*(&closure->std)));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_909
  case 31: {
    memset((closure), 0, sizeof(zend_closure));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_910
  case 32: {
    memset(closure, 0, sizeof(*(closure)));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_911
  case 33: {
    rebuild_object_properties(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_912
  case 34: {
    zend_ce_closure->create_object = zend_closure_new;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_913
  case 35: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_914
  case 36: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_915
  case 37: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_916
  case 38: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_917
  case 39: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_918
  case 40: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_919
  case 41: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_920
  case 42: {
    zend_hash_destroy((closure)->debug_info);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_921
  case 43: {
    zend_hash_destroy(closure->debug_info);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_922
  case 44: {
    zend_object_std_dtor(&(closure)->std);

    break;
  }
#endif
#ifdef COMPILE_923
  case 45: {
    zend_object_std_dtor(&(closure)->std);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_924
  case 46: {
    zend_object_std_dtor(&closure->std);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_925
  case 47: {
    zend_objects_free_object_storage(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_926
  case 48: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
#ifdef COMPILE_927
  case 49: {
    zval_addref_p((closure)->this_ptr);
    zend_object_std_dtor(&closure->std);

    break;
  }
#endif
  }

  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      // prophet generated patch
      switch (__choose("__ID12")) {
      case 0: {
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#ifdef COMPILE_928
      case 1: {
        if (ex->op_array == &closure->func.op_array) {
          (*(closure)->func.op_array.refcount)++;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_929
      case 2: {
        if (ex->op_array == &closure->func.op_array) {
          (ex) = ex->prev_execute_data;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_930
      case 3: {
        if (ex->op_array == &closure->func.op_array) {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_931
      case 4: {
        if (ex->op_array == &closure->func.op_array) {
          *((executor_globals.argument_stack)->top++) = (object);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_932
      case 5: {
        if (ex->op_array == &closure->func.op_array) {
          _IO_putc((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_933
      case 6: {
        if (ex->op_array == &closure->func.op_array) {
          _efree(((closure)));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_934
      case 7: {
        if (ex->op_array == &closure->func.op_array) {
          _efree(((closure)->debug_info));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_935
      case 8: {
        if (ex->op_array == &closure->func.op_array) {
          _efree((closure));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_936
      case 9: {
        if (ex->op_array == &closure->func.op_array) {
          _efree((closure->debug_info));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_937
      case 10: {
        if (ex->op_array == &closure->func.op_array) {
          _zend_get_parameters_array_ex(
              (1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_938
      case 11: {
        if (ex->op_array == &closure->func.op_array) {
          _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                          ((void (*)(void *))_zval_ptr_dtor), (0));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_939
      case 12: {
        if (ex->op_array == &closure->func.op_array) {
          _zend_list_find((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_940
      case 13: {
        if (ex->op_array == &closure->func.op_array) {
          _zval_ptr_dtor((&(closure)->this_ptr));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_941
      case 14: {
        if (ex->op_array == &closure->func.op_array) {
          _zval_ptr_dtor((&closure->this_ptr));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_942
      case 15: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.clone_obj = zend_closure_clone;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_943
      case 16: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.compare_objects = zend_closure_compare_objects;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_944
      case 17: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.get_closure = zend_closure_get_closure;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_945
      case 18: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.get_constructor = zend_closure_get_constructor;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_946
      case 19: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.get_debug_info = zend_closure_get_debug_info;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_947
      case 20: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.get_method = zend_closure_get_method;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_948
      case 21: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.get_property_ptr_ptr =
              zend_closure_get_property_ptr_ptr;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_949
      case 22: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.has_property = zend_closure_has_property;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_950
      case 23: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.read_property = zend_closure_read_property;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_951
      case 24: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.unset_property = zend_closure_unset_property;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_952
      case 25: {
        if (ex->op_array == &closure->func.op_array) {
          closure_handlers.write_property = zend_closure_write_property;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_953
      case 26: {
        if (ex->op_array == &closure->func.op_array) {
          compile_filename((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_954
      case 27: {
        if (ex->op_array == &closure->func.op_array) {
          destroy_op_array(&(closure)->func.op_array);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_955
      case 28: {
        if (ex->op_array == &closure->func.op_array) {
          destroy_op_array(&closure->func.op_array);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_956
      case 29: {
        if (ex->op_array == &closure->func.op_array) {
          dprintf((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_957
      case 30: {
        if (ex->op_array == &closure->func.op_array) {
          ex = (ex)->prev_execute_data;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_958
      case 31: {
        if (ex->op_array == &closure->func.op_array) {
          ex = ex->prev_execute_data;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_959
      case 32: {
        if (ex->op_array == &closure->func.op_array) {
          exit(1);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_960
      case 33: {
        if (ex->op_array == &closure->func.op_array) {
          fdopen((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_961
      case 34: {
        if (ex->op_array == &closure->func.op_array) {
          fputc((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_962
      case 35: {
        if (ex->op_array == &closure->func.op_array) {
          fputc_unlocked((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_963
      case 36: {
        if (ex->op_array == &closure->func.op_array) {
          if (!__is_neg("12-36", 0, 0, 0, 0, 3, (void **){closure, ex, object},
                        0, 0, 3, "closure", "ex", "object"))
            zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_964
      case 37: {
        if (ex->op_array == &closure->func.op_array) {
          if (__is_neg("12-37", 0, 0, 0, 0, 3, (void **){closure, ex, object},
                       0, 0, 3, "closure", "ex", "object"))
            break;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_965
      case 38: {
        if (ex->op_array == &closure->func.op_array) {
          if (__is_neg("12-38", 0, 0, 0, 0, 3, (void **){closure, ex, object},
                       0, 0, 3, "closure", "ex", "object"))
            return;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_966
      case 39: {
        if (ex->op_array == &closure->func.op_array) {
          if (closure->debug_info != ((void *)0)) {
            zend_hash_destroy(closure->debug_info);
            _efree((closure->debug_info));
          }
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_967
      case 40: {
        if (ex->op_array == &closure->func.op_array) {
          if (closure->this_ptr) {
            _zval_ptr_dtor((&closure->this_ptr));
          }
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_968
      case 41: {
        if (ex->op_array == &closure->func.op_array) {
          if (ex->op_array == &closure->func.op_array) {
            zend_error((1 << 0L), "Cannot destroy active lambda function");
          }
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_969
      case 42: {
        if (ex->op_array == &closure->func.op_array) {
          memcpy(&closure_handlers, zend_get_std_object_handlers(),
                 sizeof(zend_object_handlers));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_970
      case 43: {
        if (ex->op_array == &closure->func.op_array) {
          memset((closure), 0, sizeof(zend_closure));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_971
      case 44: {
        if (ex->op_array == &closure->func.op_array) {
          multi_convert_to_double_ex((1 << 0L),
                                     "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_972
      case 45: {
        if (ex->op_array == &closure->func.op_array) {
          multi_convert_to_long_ex((1 << 0L),
                                   "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_973
      case 46: {
        if (ex->op_array == &closure->func.op_array) {
          multi_convert_to_string_ex((1 << 0L),
                                     "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_974
      case 47: {
        if (ex->op_array == &closure->func.op_array) {
          putc((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_975
      case 48: {
        if (ex->op_array == &closure->func.op_array) {
          putc_unlocked((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_976
      case 49: {
        if (ex->op_array == &closure->func.op_array) {
          putw((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_977
      case 50: {
        if (ex->op_array == &closure->func.op_array) {
          strerror_l((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_978
      case 51: {
        if (ex->op_array == &closure->func.op_array) {
          ungetc((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_979
      case 52: {
        if (ex->op_array == &closure->func.op_array) {
          zend_ce_closure->create_object = zend_closure_new;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_980
      case 53: {
        if (ex->op_array == &closure->func.op_array) {
          zend_ce_closure->serialize = zend_class_serialize_deny;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_981
      case 54: {
        if (ex->op_array == &closure->func.op_array) {
          zend_ce_closure->unserialize = zend_class_unserialize_deny;
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_982
      case 55: {
        if (ex->op_array == &closure->func.op_array) {
          zend_copy_parameters_array((1 << 0L),
                                     "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_983
      case 56: {
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_984
      case 57: {
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 12L), "Cannot get arguments for calling closure");
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_985
      case 58: {
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 12L), "Closure object cannot have properties");
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_986
      case 59: {
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_987
      case 60: {
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_988
      case 61: {
        if (ex->op_array == &closure->func.op_array) {
          zend_error_noreturn((1 << 0L),
                              "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_989
      case 62: {
        if (ex->op_array == &closure->func.op_array) {
          zend_get_parameters_ex((1 << 0L),
                                 "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_990
      case 63: {
        if (ex->op_array == &closure->func.op_array) {
          zend_hash_destroy((closure)->debug_info);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_991
      case 64: {
        if (ex->op_array == &closure->func.op_array) {
          zend_hash_destroy(closure->debug_info);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_992
      case 65: {
        if (ex->op_array == &closure->func.op_array) {
          zend_object_std_dtor(&(closure)->std);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_993
      case 66: {
        if (ex->op_array == &closure->func.op_array) {
          zend_object_std_dtor(&closure->std);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_994
      case 67: {
        if (ex->op_array == &closure->func.op_array) {
          zend_parse_parameters((1 << 0L),
                                "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_995
      case 68: {
        if (ex->op_array == &closure->func.op_array) {
          zval_addref_p(&((executor_globals.uninitialized_zval)));
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_996
      case 69: {
        if (ex->op_array == &closure->func.op_array) {
          zval_addref_p((closure)->this_ptr);
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_997
      case 70: {
        (*(closure)->func.op_array.refcount)++;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_998
      case 71: {
        (ex) = ex->prev_execute_data;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_999
      case 72: {
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1000
      case 73: {
        *((executor_globals.argument_stack)->top++) = (object);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1001
      case 74: {
        _efree(((closure)));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1002
      case 75: {
        _efree(((closure)->debug_info));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1003
      case 76: {
        _efree((closure));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1004
      case 77: {
        _efree((closure->debug_info));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1005
      case 78: {
        _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                        ((void (*)(void *))_zval_ptr_dtor), (0));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1006
      case 79: {
        _zval_ptr_dtor((&(closure)->this_ptr));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1007
      case 80: {
        _zval_ptr_dtor((&closure->this_ptr));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1008
      case 81: {
        closure_handlers.clone_obj = zend_closure_clone;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1009
      case 82: {
        closure_handlers.compare_objects = zend_closure_compare_objects;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1010
      case 83: {
        closure_handlers.get_closure = zend_closure_get_closure;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1011
      case 84: {
        closure_handlers.get_constructor = zend_closure_get_constructor;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1012
      case 85: {
        closure_handlers.get_debug_info = zend_closure_get_debug_info;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1013
      case 86: {
        closure_handlers.get_method = zend_closure_get_method;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1014
      case 87: {
        closure_handlers.get_property_ptr_ptr =
            zend_closure_get_property_ptr_ptr;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1015
      case 88: {
        closure_handlers.has_property = zend_closure_has_property;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1016
      case 89: {
        closure_handlers.read_property = zend_closure_read_property;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1017
      case 90: {
        closure_handlers.unset_property = zend_closure_unset_property;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1018
      case 91: {
        closure_handlers.write_property = zend_closure_write_property;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1019
      case 92: {
        destroy_op_array(&(closure)->func.op_array);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1020
      case 93: {
        destroy_op_array(&closure->func.op_array);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1021
      case 94: {
        ex = (ex)->prev_execute_data;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1022
      case 95: {
        ex = ex->prev_execute_data;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1023
      case 96: {
        exit(1);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1024
      case 97: {
        if (!__is_neg("12-97", 0, 0, 0, 0, 3, (void **){closure, ex, object}, 0,
                      0, 3, "closure", "ex", "object") &&
            (ex->op_array == &closure->func.op_array)) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1025
      case 98: {
        if (!__is_neg("12-98", 0, 0, 0, 0, 3, (void **){closure, ex, object}, 0,
                      0, 3, "closure", "ex", "object"))
          if (ex->op_array == &closure->func.op_array) {
            zend_error((1 << 0L), "Cannot destroy active lambda function");
          }

        break;
      }
#endif
#ifdef COMPILE_1026
      case 99: {
        if ((ex->op_array == &closure->func.op_array) &&
            !__is_neg("12-99", 0, 0, 0, 0, 3, (void **){closure, ex, object}, 0,
                      0, 3, "closure", "ex", "object")) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1027
      case 100: {
        if ((ex->op_array == &closure->func.op_array) ||
            __is_neg("12-100", 0, 0, 0, 0, 3, (void **){closure, ex, object}, 0,
                     0, 3, "closure", "ex", "object")) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1028
      case 101: {
        if (__is_neg("12-101", 0, 0, 0, 0, 3, (void **){closure, ex, object}, 0,
                     0, 3, "closure", "ex", "object"))
          break;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1029
      case 102: {
        if (__is_neg("12-102", 0, 0, 0, 0, 3, (void **){closure, ex, object}, 0,
                     0, 3, "closure", "ex", "object"))
          return;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1030
      case 103: {
        if (closure->debug_info != ((void *)0)) {
          zend_hash_destroy(closure->debug_info);
          _efree((closure->debug_info));
        }
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1031
      case 104: {
        if (closure->this_ptr) {
          _zval_ptr_dtor((&closure->this_ptr));
        }
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1032
      case 105: {
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1033
      case 106: {
        memcpy(&closure_handlers, zend_get_std_object_handlers(),
               sizeof(zend_object_handlers));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1034
      case 107: {
        memset(&closure->func, 0, sizeof(*(&closure->func)));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1035
      case 108: {
        memset(&closure->func.op_array, 0, sizeof(*(&closure->func.op_array)));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1036
      case 109: {
        memset((closure), 0, sizeof(zend_closure));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1037
      case 110: {
        memset(closure, 0, sizeof(*(closure)));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1038
      case 111: {
        memset(ex, 0, sizeof(*(ex)));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1039
      case 112: {
        zend_ce_closure->create_object = zend_closure_new;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1040
      case 113: {
        zend_ce_closure->serialize = zend_class_serialize_deny;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1041
      case 114: {
        zend_ce_closure->unserialize = zend_class_unserialize_deny;
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1042
      case 115: {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1043
      case 116: {
        zend_error((1 << 12L), "Cannot get arguments for calling closure");
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1044
      case 117: {
        zend_error((1 << 12L), "Closure object cannot have properties");
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1045
      case 118: {
        zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1046
      case 119: {
        zend_error((1 << 3L), "A non well formed numeric value encountered");
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1047
      case 120: {
        zend_hash_destroy((closure)->debug_info);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1048
      case 121: {
        zend_hash_destroy(closure->debug_info);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1049
      case 122: {
        zend_object_std_dtor(&(closure)->std);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1050
      case 123: {
        zend_object_std_dtor(&closure->std);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1051
      case 124: {
        zval_addref_p(&((executor_globals.uninitialized_zval)));
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
#ifdef COMPILE_1052
      case 125: {
        zval_addref_p((closure)->this_ptr);
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }

        break;
      }
#endif
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

static zend_object_value
zend_closure_new(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
  zend_closure *closure;
  zend_object_value object;

  closure = emalloc(sizeof(zend_closure));
  memset(closure, 0, sizeof(zend_closure));

  zend_object_std_init(&closure->std, class_type TSRMLS_CC);

  object.handle = zend_objects_store_put(
      closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
      (zend_objects_free_object_storage_t)zend_closure_free_storage,
      NULL TSRMLS_CC);
  object.handlers = &closure_handlers;

  return object;
}
/* }}} */

static zend_object_value zend_closure_clone(zval *zobject TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(zobject TSRMLS_CC);
  zval result;

  zend_create_closure(&result, &closure->func, closure->func.common.scope,
                      closure->this_ptr TSRMLS_CC);
  return Z_OBJVAL(result);
}
/* }}} */

int zend_closure_get_closure(zval *obj, zend_class_entry **ce_ptr,
                             zend_function **fptr_ptr,
                             zval **zobj_ptr TSRMLS_DC) /* {{{ */
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

static HashTable *zend_closure_get_debug_info(zval *object,
                                              int *is_temp TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(object TSRMLS_CC);
  zval *val;
  struct _zend_arg_info *arg_info = closure->func.common.arg_info;

  *is_temp = 0;

  if (closure->debug_info == NULL) {
    ALLOC_HASHTABLE(closure->debug_info);
    zend_hash_init(closure->debug_info, 1, NULL, ZVAL_PTR_DTOR, 0);
  }
  if (closure->debug_info->nApplyCount == 0) {
    if (closure->func.type == ZEND_USER_FUNCTION &&
        closure->func.op_array.static_variables) {
      HashTable *static_variables = closure->func.op_array.static_variables;
      MAKE_STD_ZVAL(val);
      array_init(val);
      zend_hash_copy(Z_ARRVAL_P(val), static_variables,
                     (copy_ctor_func_t)zval_add_ref, NULL, sizeof(zval *));
      zend_hash_update(closure->debug_info, "static", sizeof("static"),
                       (void *)&val, sizeof(zval *), NULL);
    }

    if (closure->this_ptr) {
      Z_ADDREF_P(closure->this_ptr);
      zend_symtable_update(closure->debug_info, "this", sizeof("this"),
                           (void *)&closure->this_ptr, sizeof(zval *), NULL);
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
          name_len =
              zend_spprintf(&name, 0, "%s$param%d",
                            arg_info->pass_by_reference ? "&" : "", i + 1);
        }
        info_len = zend_spprintf(&info, 0, "%s",
                                 i >= required ? "<optional>" : "<required>");
        add_assoc_stringl_ex(val, name, name_len + 1, info, info_len, 0);
        efree(name);
        arg_info++;
      }
      zend_hash_update(closure->debug_info, "parameter", sizeof("parameter"),
                       (void *)&val, sizeof(zval *), NULL);
    }
  }

  return closure->debug_info;
}
/* }}} */

/* {{{ proto Closure::__construct()
   Private constructor preventing instantiation */
ZEND_METHOD(Closure, __construct) {
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
            ZEND_ME(Closure, bind, arginfo_closure_bind,
                    ZEND_ACC_PUBLIC | ZEND_ACC_STATIC){NULL, NULL, NULL}};

void zend_register_closure_ce(TSRMLS_D) /* {{{ */
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, "Closure", closure_functions);
  zend_ce_closure = zend_register_internal_class(&ce TSRMLS_CC);
  zend_ce_closure->ce_flags |= ZEND_ACC_FINAL_CLASS;
  zend_ce_closure->create_object = zend_closure_new;
  zend_ce_closure->serialize = zend_class_serialize_deny;
  zend_ce_closure->unserialize = zend_class_unserialize_deny;

  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
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

ZEND_API void zend_create_closure(zval *res, zend_function *func,
                                  zend_class_entry *scope,
                                  zval *this_ptr TSRMLS_DC) /* {{{ */
{
  zend_closure *closure;

  object_init_ex(res, zend_ce_closure);

  closure = (zend_closure *)zend_object_store_get_object(res TSRMLS_CC);

  closure->func = *func;

  if (closure->func.type == ZEND_USER_FUNCTION) {
    if (closure->func.op_array.static_variables) {
      HashTable *static_variables = closure->func.op_array.static_variables;

      ALLOC_HASHTABLE(closure->func.op_array.static_variables);
      zend_hash_init(closure->func.op_array.static_variables,
                     zend_hash_num_elements(static_variables), NULL,
                     ZVAL_PTR_DTOR, 0);
      zend_hash_apply_with_arguments(static_variables TSRMLS_CC,
                                     (apply_func_args_t)zval_copy_static_var, 1,
                                     closure->func.op_array.static_variables);
    }
    closure->func.op_array.run_time_cache = NULL;
    (*closure->func.op_array.refcount)++;
  } else {
    /* verify that we aren't binding internal function to a wrong scope */
    if (func->common.scope != NULL) {
      if (scope && !instanceof_function(scope, func->common.scope TSRMLS_CC)) {
        zend_error(E_WARNING, "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, func->common.function_name,
                   scope->name);
        scope = NULL;
      }
      if (scope && this_ptr && (func->common.fn_flags & ZEND_ACC_STATIC) == 0 &&
          !instanceof_function(Z_OBJCE_P(this_ptr),
                               closure->func.common.scope TSRMLS_CC)) {
        zend_error(E_WARNING,
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   Z_OBJCE_P(this_ptr)->name);
        scope = NULL;
        this_ptr = NULL;
      }
    } else {
      /* if it's a free function, we won't set scope & this since they're
       * meaningless */
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
// prophet generated patch
switch (__choose("__ID13")) {
case 0: {
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

  break;
}
#ifdef COMPILE_1053
case 1: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      (*(closure)->func.op_array.refcount)++;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1054
case 2: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      (ex) = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1055
case 3: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      (ex) = ex->prev_execute_data;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1056
case 4: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1057
case 5: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      *((executor_globals.argument_stack)->top++) = (object);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1058
case 6: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      _efree(((closure)));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1059
case 7: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      _efree(((closure)->debug_info));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1060
case 8: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      _efree((closure));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1061
case 9: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      _efree((closure->debug_info));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1062
case 10: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1063
case 11: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      _zval_ptr_dtor((&(closure)->this_ptr));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1064
case 12: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      _zval_ptr_dtor((&closure->this_ptr));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1065
case 13: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.clone_obj = zend_closure_clone;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1066
case 14: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.compare_objects = zend_closure_compare_objects;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1067
case 15: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.get_closure = zend_closure_get_closure;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1068
case 16: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.get_constructor = zend_closure_get_constructor;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1069
case 17: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.get_debug_info = zend_closure_get_debug_info;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1070
case 18: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.get_method = zend_closure_get_method;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1071
case 19: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1072
case 20: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.has_property = zend_closure_has_property;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1073
case 21: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.read_property = zend_closure_read_property;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1074
case 22: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.unset_property = zend_closure_unset_property;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1075
case 23: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      closure_handlers.write_property = zend_closure_write_property;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1076
case 24: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      destroy_op_array(&(closure)->func.op_array);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1077
case 25: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      destroy_op_array(&closure->func.op_array);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1078
case 26: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      ex = (ex)->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1079
case 27: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      ex = (ex)->prev_execute_data;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1080
case 28: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1081
case 29: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      exit(1);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1082
case 30: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      if (!__is_neg("13-30", 0, 0, 0, 0, 3, (void **){ex, closure, object}, 0,
                    0, 3, "ex", "closure", "object"))
        ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1083
case 31: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      if (__is_neg("13-31", 0, 0, 0, 0, 3, (void **){ex, closure, object}, 0, 0,
                   3, "ex", "closure", "object"))
        break;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1084
case 32: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      if (__is_neg("13-32", 0, 0, 0, 0, 3, (void **){ex, closure, object}, 0, 0,
                   3, "ex", "closure", "object"))
        return;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1085
case 33: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      if (closure->debug_info != ((void *)0)) {
        zend_hash_destroy(closure->debug_info);
        _efree((closure->debug_info));
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1086
case 34: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      if (closure->this_ptr) {
        _zval_ptr_dtor((&closure->this_ptr));
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1087
case 35: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1088
case 36: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1089
case 37: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      memset((closure), 0, sizeof(zend_closure));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1090
case 38: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      memset(ex, 0, sizeof(*(ex)));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1091
case 39: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_ce_closure->create_object = zend_closure_new;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1092
case 40: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_ce_closure->serialize = zend_class_serialize_deny;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1093
case 41: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_ce_closure->unserialize = zend_class_unserialize_deny;
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1094
case 42: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_error((1 << 0L), "Cannot destroy active lambda function");
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1095
case 43: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_error((1 << 12L), "Cannot get arguments for calling closure");
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1096
case 44: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_error((1 << 12L), "Closure object cannot have properties");
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1097
case 45: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1098
case 46: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1099
case 47: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_hash_destroy((closure)->debug_info);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1100
case 48: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_hash_destroy(closure->debug_info);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1101
case 49: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_object_std_dtor(&(closure)->std);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1102
case 50: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zend_object_std_dtor(&closure->std);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1103
case 51: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zval_addref_p(&((executor_globals.uninitialized_zval)));
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1104
case 52: {
  if (closure->func.type == ZEND_USER_FUNCTION) {
    zend_execute_data *ex = EG(current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error(E_ERROR, "Cannot destroy active lambda function");
      }
      zval_addref_p((closure)->this_ptr);
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array TSRMLS_CC);
  }

  break;
}
#endif
#ifdef COMPILE_1105
case 53: {
  (*(closure)->func.op_array.refcount)++;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1106
case 54: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1107
case 55: {
  *((executor_globals.argument_stack)->top++) = (object);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1108
case 56: {
  _efree(((closure)));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1109
case 57: {
  _efree(((closure)->debug_info));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1110
case 58: {
  _efree((closure));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1111
case 59: {
  _efree((closure->debug_info));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1112
case 60: {
  _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                  ((void (*)(void *))_zval_ptr_dtor), (0));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1113
case 61: {
  _zval_ptr_dtor((&(closure)->this_ptr));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1114
case 62: {
  _zval_ptr_dtor((&closure->this_ptr));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1115
case 63: {
  closure_handlers.clone_obj = zend_closure_clone;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1116
case 64: {
  closure_handlers.compare_objects = zend_closure_compare_objects;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1117
case 65: {
  closure_handlers.get_closure = zend_closure_get_closure;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1118
case 66: {
  closure_handlers.get_constructor = zend_closure_get_constructor;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1119
case 67: {
  closure_handlers.get_debug_info = zend_closure_get_debug_info;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1120
case 68: {
  closure_handlers.get_method = zend_closure_get_method;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1121
case 69: {
  closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1122
case 70: {
  closure_handlers.has_property = zend_closure_has_property;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1123
case 71: {
  closure_handlers.read_property = zend_closure_read_property;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1124
case 72: {
  closure_handlers.unset_property = zend_closure_unset_property;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1125
case 73: {
  closure_handlers.write_property = zend_closure_write_property;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1126
case 74: {
  destroy_op_array(&(closure)->func.op_array);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1127
case 75: {
  destroy_op_array(&closure->func.op_array);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1128
case 76: {
  exit(1);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1129
case 77: {
  if (!__is_neg("13-77", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object") &&
      (closure->func.type == 2)) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1130
case 78: {
  if (!__is_neg("13-78", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object"))
    if (closure->func.type == 2) {
      zend_execute_data *ex = (executor_globals.current_execute_data);
      while (ex) {
        if (ex->op_array == &closure->func.op_array) {
          zend_error((1 << 0L), "Cannot destroy active lambda function");
        }
        ex = ex->prev_execute_data;
      }
      destroy_op_array(&closure->func.op_array);
    }

  break;
}
#endif
#ifdef COMPILE_1131
case 79: {
  if ((closure->func.type == 2) &&
      !__is_neg("13-79", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object")) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1132
case 80: {
  if ((closure->func.type == 2) ||
      __is_neg("13-80", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
               "closure", "object")) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1133
case 81: {
  if (__is_neg("13-81", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
               "closure", "object"))
    return;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1134
case 82: {
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1135
case 83: {
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1136
case 84: {
  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1137
case 85: {
  memset(&closure->func, 0, sizeof(*(&closure->func)));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1138
case 86: {
  memset(&closure->func.op_array, 0, sizeof(*(&closure->func.op_array)));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1139
case 87: {
  memset(&executor_globals, 0, sizeof(*(&executor_globals)));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1140
case 88: {
  memset((closure), 0, sizeof(zend_closure));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1141
case 89: {
  memset(closure, 0, sizeof(*(closure)));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1142
case 90: {
  zend_ce_closure->create_object = zend_closure_new;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1143
case 91: {
  zend_ce_closure->serialize = zend_class_serialize_deny;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1144
case 92: {
  zend_ce_closure->unserialize = zend_class_unserialize_deny;
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1145
case 93: {
  zend_error((1 << 0L), "Cannot destroy active lambda function");
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1146
case 94: {
  zend_error((1 << 12L), "Cannot get arguments for calling closure");
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1147
case 95: {
  zend_error((1 << 12L), "Closure object cannot have properties");
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1148
case 96: {
  zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1149
case 97: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1150
case 98: {
  zend_hash_destroy((closure)->debug_info);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1151
case 99: {
  zend_hash_destroy(closure->debug_info);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1152
case 100: {
  zend_object_std_dtor(&(closure)->std);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1153
case 101: {
  zend_object_std_dtor(&closure->std);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1154
case 102: {
  zval_addref_p(&((executor_globals.uninitialized_zval)));
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
#ifdef COMPILE_1155
case 103: {
  zval_addref_p((closure)->this_ptr);
  if (closure->func.type == 2) {
    zend_execute_data *ex = (executor_globals.current_execute_data);
    while (ex) {
      if (ex->op_array == &closure->func.op_array) {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
      }
      ex = ex->prev_execute_data;
    }
    destroy_op_array(&closure->func.op_array);
  }

  break;
}
#endif
}

if (closure->debug_info != NULL) {
  // prophet generated patch
  switch (__choose("__ID14")) {
  case 0: {
    zend_hash_destroy(closure->debug_info);

    break;
  }
#ifdef COMPILE_1156
  case 1: {
    (*(closure)->func.op_array.refcount)++;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1157
  case 2: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1158
  case 3: {
    *((executor_globals.argument_stack)->top++) = (object);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1159
  case 4: {
    _efree(((closure)));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1160
  case 5: {
    _efree(((closure)->debug_info));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1161
  case 6: {
    _efree((closure));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1162
  case 7: {
    _efree((closure->debug_info));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1163
  case 8: {
    _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1164
  case 9: {
    _zval_ptr_dtor((&(closure)->this_ptr));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1165
  case 10: {
    _zval_ptr_dtor((&closure->this_ptr));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1166
  case 11: {
    closure_handlers.clone_obj = zend_closure_clone;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1167
  case 12: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1168
  case 13: {
    closure_handlers.get_closure = zend_closure_get_closure;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1169
  case 14: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1170
  case 15: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1171
  case 16: {
    closure_handlers.get_method = zend_closure_get_method;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1172
  case 17: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1173
  case 18: {
    closure_handlers.has_property = zend_closure_has_property;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1174
  case 19: {
    closure_handlers.read_property = zend_closure_read_property;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1175
  case 20: {
    closure_handlers.unset_property = zend_closure_unset_property;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1176
  case 21: {
    closure_handlers.write_property = zend_closure_write_property;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1177
  case 22: {
    destroy_op_array(&(closure)->func.op_array);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1178
  case 23: {
    destroy_op_array(&closure->func.op_array);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1179
  case 24: {
    exit(1);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1180
  case 25: {
    if (!__is_neg("14-25", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                  "closure", "object"))
      zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1181
  case 26: {
    if (__is_neg("14-26", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                 "closure", "object"))
      return;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1182
  case 27: {
    if (closure->debug_info != ((void *)0)) {
      zend_hash_destroy(closure->debug_info);
      _efree((closure->debug_info));
    }
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1183
  case 28: {
    if (closure->this_ptr) {
      _zval_ptr_dtor((&closure->this_ptr));
    }
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1184
  case 29: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1185
  case 30: {
    memset((closure), 0, sizeof(zend_closure));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1186
  case 31: {
    memset(closure, 0, sizeof(*(closure)));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1187
  case 32: {
    zend_ce_closure->create_object = zend_closure_new;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1188
  case 33: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1189
  case 34: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1190
  case 35: {
    zend_destroy_rsrc_list(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1191
  case 36: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1192
  case 37: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1193
  case 38: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1194
  case 39: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1195
  case 40: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1196
  case 41: {
    zend_hash_clean(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1197
  case 42: {
    zend_hash_destroy((closure)->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1198
  case 43: {
    zend_hash_destroy((closure)->debug_info);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1199
  case 44: {
    zend_hash_destroy(closure->debug_info);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1200
  case 45: {
    zend_hash_graceful_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1201
  case 46: {
    zend_hash_graceful_reverse_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1202
  case 47: {
    zend_hash_rehash(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1203
  case 48: {
    zend_object_std_dtor(&(closure)->std);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1204
  case 49: {
    zend_object_std_dtor(&closure->std);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1205
  case 50: {
    zend_reset_all_cv(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1206
  case 51: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
#ifdef COMPILE_1207
  case 52: {
    zval_addref_p((closure)->this_ptr);
    zend_hash_destroy(closure->debug_info);

    break;
  }
#endif
  }
  efree(closure->debug_info);
}

if (closure->this_ptr) {
  zval_ptr_dtor(&closure->this_ptr);
}

efree(closure);
}
/* }}} */

static zend_object_value
zend_closure_new(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
  zend_closure *closure;
  zend_object_value object;

  closure = emalloc(sizeof(zend_closure));
  memset(closure, 0, sizeof(zend_closure));

  zend_object_std_init(&closure->std, class_type TSRMLS_CC);

  object.handle = zend_objects_store_put(
      closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
      (zend_objects_free_object_storage_t)zend_closure_free_storage,
      NULL TSRMLS_CC);
  object.handlers = &closure_handlers;

  return object;
}
/* }}} */

static zend_object_value zend_closure_clone(zval *zobject TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(zobject TSRMLS_CC);
  zval result;

  zend_create_closure(&result, &closure->func, closure->func.common.scope,
                      closure->this_ptr TSRMLS_CC);
  return Z_OBJVAL(result);
}
/* }}} */

int zend_closure_get_closure(zval *obj, zend_class_entry **ce_ptr,
                             zend_function **fptr_ptr,
                             zval **zobj_ptr TSRMLS_DC) /* {{{ */
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

static HashTable *zend_closure_get_debug_info(zval *object,
                                              int *is_temp TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(object TSRMLS_CC);
  zval *val;
  struct _zend_arg_info *arg_info = closure->func.common.arg_info;

  *is_temp = 0;

  if (closure->debug_info == NULL) {
    ALLOC_HASHTABLE(closure->debug_info);
    zend_hash_init(closure->debug_info, 1, NULL, ZVAL_PTR_DTOR, 0);
  }
  if (closure->debug_info->nApplyCount == 0) {
    if (closure->func.type == ZEND_USER_FUNCTION &&
        closure->func.op_array.static_variables) {
      HashTable *static_variables = closure->func.op_array.static_variables;
      MAKE_STD_ZVAL(val);
      array_init(val);
      zend_hash_copy(Z_ARRVAL_P(val), static_variables,
                     (copy_ctor_func_t)zval_add_ref, NULL, sizeof(zval *));
      zend_hash_update(closure->debug_info, "static", sizeof("static"),
                       (void *)&val, sizeof(zval *), NULL);
    }

    if (closure->this_ptr) {
      Z_ADDREF_P(closure->this_ptr);
      zend_symtable_update(closure->debug_info, "this", sizeof("this"),
                           (void *)&closure->this_ptr, sizeof(zval *), NULL);
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
          name_len =
              zend_spprintf(&name, 0, "%s$param%d",
                            arg_info->pass_by_reference ? "&" : "", i + 1);
        }
        info_len = zend_spprintf(&info, 0, "%s",
                                 i >= required ? "<optional>" : "<required>");
        add_assoc_stringl_ex(val, name, name_len + 1, info, info_len, 0);
        efree(name);
        arg_info++;
      }
      zend_hash_update(closure->debug_info, "parameter", sizeof("parameter"),
                       (void *)&val, sizeof(zval *), NULL);
    }
  }

  return closure->debug_info;
}
/* }}} */

/* {{{ proto Closure::__construct()
   Private constructor preventing instantiation */
ZEND_METHOD(Closure, __construct) {
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
            ZEND_ME(Closure, bind, arginfo_closure_bind,
                    ZEND_ACC_PUBLIC | ZEND_ACC_STATIC){NULL, NULL, NULL}};

void zend_register_closure_ce(TSRMLS_D) /* {{{ */
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, "Closure", closure_functions);
  zend_ce_closure = zend_register_internal_class(&ce TSRMLS_CC);
  zend_ce_closure->ce_flags |= ZEND_ACC_FINAL_CLASS;
  zend_ce_closure->create_object = zend_closure_new;
  zend_ce_closure->serialize = zend_class_serialize_deny;
  zend_ce_closure->unserialize = zend_class_unserialize_deny;

  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
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

ZEND_API void zend_create_closure(zval *res, zend_function *func,
                                  zend_class_entry *scope,
                                  zval *this_ptr TSRMLS_DC) /* {{{ */
{
  zend_closure *closure;

  object_init_ex(res, zend_ce_closure);

  closure = (zend_closure *)zend_object_store_get_object(res TSRMLS_CC);

  closure->func = *func;

  if (closure->func.type == ZEND_USER_FUNCTION) {
    if (closure->func.op_array.static_variables) {
      HashTable *static_variables = closure->func.op_array.static_variables;

      ALLOC_HASHTABLE(closure->func.op_array.static_variables);
      zend_hash_init(closure->func.op_array.static_variables,
                     zend_hash_num_elements(static_variables), NULL,
                     ZVAL_PTR_DTOR, 0);
      zend_hash_apply_with_arguments(static_variables TSRMLS_CC,
                                     (apply_func_args_t)zval_copy_static_var, 1,
                                     closure->func.op_array.static_variables);
    }
    closure->func.op_array.run_time_cache = NULL;
    (*closure->func.op_array.refcount)++;
  } else {
    /* verify that we aren't binding internal function to a wrong scope */
    if (func->common.scope != NULL) {
      if (scope && !instanceof_function(scope, func->common.scope TSRMLS_CC)) {
        zend_error(E_WARNING, "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, func->common.function_name,
                   scope->name);
        scope = NULL;
      }
      if (scope && this_ptr && (func->common.fn_flags & ZEND_ACC_STATIC) == 0 &&
          !instanceof_function(Z_OBJCE_P(this_ptr),
                               closure->func.common.scope TSRMLS_CC)) {
        zend_error(E_WARNING,
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   Z_OBJCE_P(this_ptr)->name);
        scope = NULL;
        this_ptr = NULL;
      }
    } else {
      /* if it's a free function, we won't set scope & this since they're
       * meaningless */
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
// prophet generated patch
switch (__choose("__ID15")) {
case 0: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#ifdef COMPILE_1208
case 1: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    (*(closure)->func.op_array.refcount)++;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1209
case 2: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1210
case 3: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    *((executor_globals.argument_stack)->top++) = (object);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1211
case 4: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _efree(((closure)));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1212
case 5: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _efree(((closure)->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1213
case 6: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _efree(((closure)->debug_info));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1214
case 7: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1215
case 8: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1216
case 9: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1217
case 10: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _zend_mem_block_size((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1218
case 11: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _zval_ptr_dtor((&(closure)->this_ptr));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1219
case 12: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    _zval_ptr_dtor((&closure->this_ptr));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1220
case 13: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.clone_obj = zend_closure_clone;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1221
case 14: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.compare_objects = zend_closure_compare_objects;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1222
case 15: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.get_closure = zend_closure_get_closure;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1223
case 16: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.get_constructor = zend_closure_get_constructor;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1224
case 17: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1225
case 18: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.get_method = zend_closure_get_method;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1226
case 19: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1227
case 20: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.has_property = zend_closure_has_property;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1228
case 21: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.read_property = zend_closure_read_property;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1229
case 22: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.unset_property = zend_closure_unset_property;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1230
case 23: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    closure_handlers.write_property = zend_closure_write_property;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1231
case 24: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    destroy_op_array(&(closure)->func.op_array);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1232
case 25: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    destroy_op_array(&closure->func.op_array);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1233
case 26: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    dlclose((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1234
case 27: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    exit(1);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1235
case 28: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    free((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1236
case 29: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    if (!__is_neg("15-29", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                  "closure", "object"))
      _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1237
case 30: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    if (__is_neg("15-30", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                 "closure", "object"))
      return;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1238
case 31: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    if (closure->debug_info != ((void *)0)) {
      zend_hash_destroy(closure->debug_info);
      _efree((closure->debug_info));
    }
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1239
case 32: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    if (closure->this_ptr) {
      _zval_ptr_dtor((&closure->this_ptr));
    }
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1240
case 33: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    list_entry_destructor((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1241
case 34: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1242
case 35: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    memset((closure), 0, sizeof(zend_closure));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1243
case 36: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    memset(closure, 0, sizeof(*(closure)));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1244
case 37: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    plist_entry_destructor((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1245
case 38: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_ce_closure->create_object = zend_closure_new;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1246
case 39: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_ce_closure->serialize = zend_class_serialize_deny;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1247
case 40: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1248
case 41: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_closure_free_storage((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1249
case 42: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1250
case 43: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1251
case 44: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_error((1 << 12L), "Closure object cannot have properties");
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1252
case 45: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1253
case 46: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1254
case 47: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_hash_destroy((closure)->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1255
case 48: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1256
case 49: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_object_std_dtor(&(closure)->std);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1257
case 50: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_object_std_dtor(&closure->std);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1258
case 51: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_vm_stack_free((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1259
case 52: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_vm_stack_free_int((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1260
case 53: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_vm_stack_push((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1261
case 54: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zend_vm_stack_push_nocheck((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1262
case 55: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1263
case 56: {
  if (closure->debug_info != NULL) {
    zend_hash_destroy(closure->debug_info);
    zval_addref_p((closure)->this_ptr);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1264
case 57: {
  (*(closure)->func.op_array.refcount)++;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1265
case 58: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1266
case 59: {
  *((executor_globals.argument_stack)->top++) = (object);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1267
case 60: {
  _efree(((closure)));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1268
case 61: {
  _efree(((closure)->debug_info));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1269
case 62: {
  _efree((closure));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1270
case 63: {
  _efree((closure->debug_info));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1271
case 64: {
  _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                  ((void (*)(void *))_zval_ptr_dtor), (0));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1272
case 65: {
  _zval_ptr_dtor((&(closure)->this_ptr));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1273
case 66: {
  _zval_ptr_dtor((&closure->this_ptr));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1274
case 67: {
  closure_handlers.clone_obj = zend_closure_clone;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1275
case 68: {
  closure_handlers.compare_objects = zend_closure_compare_objects;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1276
case 69: {
  closure_handlers.get_closure = zend_closure_get_closure;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1277
case 70: {
  closure_handlers.get_constructor = zend_closure_get_constructor;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1278
case 71: {
  closure_handlers.get_debug_info = zend_closure_get_debug_info;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1279
case 72: {
  closure_handlers.get_method = zend_closure_get_method;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1280
case 73: {
  closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1281
case 74: {
  closure_handlers.has_property = zend_closure_has_property;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1282
case 75: {
  closure_handlers.read_property = zend_closure_read_property;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1283
case 76: {
  closure_handlers.unset_property = zend_closure_unset_property;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1284
case 77: {
  closure_handlers.write_property = zend_closure_write_property;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1285
case 78: {
  destroy_op_array(&(closure)->func.op_array);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1286
case 79: {
  destroy_op_array(&closure->func.op_array);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1287
case 80: {
  exit(1);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1288
case 81: {
  if (!__is_neg("15-81", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object") &&
      (closure->debug_info != ((void *)0))) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1289
case 82: {
  if (!__is_neg("15-82", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object"))
    if (closure->debug_info != ((void *)0)) {
      zend_hash_destroy(closure->debug_info);
      _efree((closure->debug_info));
    }

  break;
}
#endif
#ifdef COMPILE_1290
case 83: {
  if ((closure->debug_info != ((void *)0)) &&
      !__is_neg("15-83", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object")) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1291
case 84: {
  if ((closure->debug_info != ((void *)0)) ||
      __is_neg("15-84", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
               "closure", "object")) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1292
case 85: {
  if (__is_neg("15-85", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
               "closure", "object"))
    return;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1293
case 86: {
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1294
case 87: {
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1295
case 88: {
  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1296
case 89: {
  memset((closure), 0, sizeof(zend_closure));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1297
case 90: {
  memset(closure, 0, sizeof(*(closure)));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1298
case 91: {
  zend_ce_closure->create_object = zend_closure_new;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1299
case 92: {
  zend_ce_closure->serialize = zend_class_serialize_deny;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1300
case 93: {
  zend_ce_closure->unserialize = zend_class_unserialize_deny;
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1301
case 94: {
  zend_error((1 << 0L), "Cannot destroy active lambda function");
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1302
case 95: {
  zend_error((1 << 12L), "Cannot get arguments for calling closure");
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1303
case 96: {
  zend_error((1 << 12L), "Closure object cannot have properties");
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1304
case 97: {
  zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1305
case 98: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1306
case 99: {
  zend_hash_destroy((closure)->debug_info);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1307
case 100: {
  zend_hash_destroy(closure->debug_info);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1308
case 101: {
  zend_object_std_dtor(&(closure)->std);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1309
case 102: {
  zend_object_std_dtor(&closure->std);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1310
case 103: {
  zval_addref_p(&((executor_globals.uninitialized_zval)));
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
#ifdef COMPILE_1311
case 104: {
  zval_addref_p((closure)->this_ptr);
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }

  break;
}
#endif
}

// prophet generated patch
switch (__choose("__ID16")) {
case 0: {
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#ifdef COMPILE_1312
case 1: {
  if (closure->this_ptr) {
    (*(closure)->func.op_array.refcount)++;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1313
case 2: {
  if (closure->this_ptr) {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1314
case 3: {
  if (closure->this_ptr) {
    *((executor_globals.argument_stack)->top++) = (object);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1315
case 4: {
  if (closure->this_ptr) {
    _efree(((closure)));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1316
case 5: {
  if (closure->this_ptr) {
    _efree(((closure)->debug_info));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1317
case 6: {
  if (closure->this_ptr) {
    _efree((closure));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1318
case 7: {
  if (closure->this_ptr) {
    _efree((closure->debug_info));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1319
case 8: {
  if (closure->this_ptr) {
    _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1320
case 9: {
  if (closure->this_ptr) {
    _zval_internal_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1321
case 10: {
  if (closure->this_ptr) {
    _zval_ptr_dtor((&(closure)->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1322
case 11: {
  if (closure->this_ptr) {
    _zval_ptr_dtor((&(closure)->this_ptr));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1323
case 12: {
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1324
case 13: {
  if (closure->this_ptr) {
    closure_handlers.clone_obj = zend_closure_clone;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1325
case 14: {
  if (closure->this_ptr) {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1326
case 15: {
  if (closure->this_ptr) {
    closure_handlers.get_closure = zend_closure_get_closure;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1327
case 16: {
  if (closure->this_ptr) {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1328
case 17: {
  if (closure->this_ptr) {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1329
case 18: {
  if (closure->this_ptr) {
    closure_handlers.get_method = zend_closure_get_method;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1330
case 19: {
  if (closure->this_ptr) {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1331
case 20: {
  if (closure->this_ptr) {
    closure_handlers.has_property = zend_closure_has_property;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1332
case 21: {
  if (closure->this_ptr) {
    closure_handlers.read_property = zend_closure_read_property;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1333
case 22: {
  if (closure->this_ptr) {
    closure_handlers.unset_property = zend_closure_unset_property;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1334
case 23: {
  if (closure->this_ptr) {
    closure_handlers.write_property = zend_closure_write_property;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1335
case 24: {
  if (closure->this_ptr) {
    destroy_op_array(&(closure)->func.op_array);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1336
case 25: {
  if (closure->this_ptr) {
    destroy_op_array(&closure->func.op_array);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1337
case 26: {
  if (closure->this_ptr) {
    exit(1);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1338
case 27: {
  if (closure->this_ptr) {
    if (!__is_neg("16-27", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                  "closure", "object"))
      _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1339
case 28: {
  if (closure->this_ptr) {
    if (__is_neg("16-28", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                 "closure", "object"))
      return;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1340
case 29: {
  if (closure->this_ptr) {
    if (closure->debug_info != ((void *)0)) {
      zend_hash_destroy(closure->debug_info);
      _efree((closure->debug_info));
    }
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1341
case 30: {
  if (closure->this_ptr) {
    if (closure->this_ptr) {
      _zval_ptr_dtor((&closure->this_ptr));
    }
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1342
case 31: {
  if (closure->this_ptr) {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1343
case 32: {
  if (closure->this_ptr) {
    memset((closure), 0, sizeof(zend_closure));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1344
case 33: {
  if (closure->this_ptr) {
    memset(closure, 0, sizeof(*(closure)));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1345
case 34: {
  if (closure->this_ptr) {
    zend_ce_closure->create_object = zend_closure_new;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1346
case 35: {
  if (closure->this_ptr) {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1347
case 36: {
  if (closure->this_ptr) {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1348
case 37: {
  if (closure->this_ptr) {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1349
case 38: {
  if (closure->this_ptr) {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1350
case 39: {
  if (closure->this_ptr) {
    zend_error((1 << 12L), "Closure object cannot have properties");
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1351
case 40: {
  if (closure->this_ptr) {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1352
case 41: {
  if (closure->this_ptr) {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1353
case 42: {
  if (closure->this_ptr) {
    zend_hash_destroy((closure)->debug_info);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1354
case 43: {
  if (closure->this_ptr) {
    zend_hash_destroy(closure->debug_info);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1355
case 44: {
  if (closure->this_ptr) {
    zend_object_std_dtor(&(closure)->std);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1356
case 45: {
  if (closure->this_ptr) {
    zend_object_std_dtor(&closure->std);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1357
case 46: {
  if (closure->this_ptr) {
    zval_add_ref((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1358
case 47: {
  if (closure->this_ptr) {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1359
case 48: {
  if (closure->this_ptr) {
    zval_addref_p((closure)->this_ptr);
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1360
case 49: {
  (*(closure)->func.op_array.refcount)++;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1361
case 50: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1362
case 51: {
  *((executor_globals.argument_stack)->top++) = (object);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1363
case 52: {
  _efree(((closure)));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1364
case 53: {
  _efree(((closure)->debug_info));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1365
case 54: {
  _efree((closure));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1366
case 55: {
  _efree((closure->debug_info));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1367
case 56: {
  _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                  ((void (*)(void *))_zval_ptr_dtor), (0));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1368
case 57: {
  _zval_ptr_dtor((&(closure)->this_ptr));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1369
case 58: {
  _zval_ptr_dtor((&closure->this_ptr));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1370
case 59: {
  closure_handlers.clone_obj = zend_closure_clone;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1371
case 60: {
  closure_handlers.compare_objects = zend_closure_compare_objects;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1372
case 61: {
  closure_handlers.get_closure = zend_closure_get_closure;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1373
case 62: {
  closure_handlers.get_constructor = zend_closure_get_constructor;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1374
case 63: {
  closure_handlers.get_debug_info = zend_closure_get_debug_info;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1375
case 64: {
  closure_handlers.get_method = zend_closure_get_method;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1376
case 65: {
  closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1377
case 66: {
  closure_handlers.has_property = zend_closure_has_property;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1378
case 67: {
  closure_handlers.read_property = zend_closure_read_property;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1379
case 68: {
  closure_handlers.unset_property = zend_closure_unset_property;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1380
case 69: {
  closure_handlers.write_property = zend_closure_write_property;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1381
case 70: {
  destroy_op_array(&(closure)->func.op_array);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1382
case 71: {
  destroy_op_array(&closure->func.op_array);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1383
case 72: {
  exit(1);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1384
case 73: {
  if (!__is_neg("16-73", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object") &&
      (closure->this_ptr)) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1385
case 74: {
  if (!__is_neg("16-74", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object"))
    if (closure->this_ptr) {
      _zval_ptr_dtor((&closure->this_ptr));
    }

  break;
}
#endif
#ifdef COMPILE_1386
case 75: {
  if ((closure->this_ptr) &&
      !__is_neg("16-75", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
                "closure", "object")) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1387
case 76: {
  if ((closure->this_ptr) ||
      __is_neg("16-76", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
               "closure", "object")) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1388
case 77: {
  if (__is_neg("16-77", 0, 0, 0, 0, 2, (void **){closure, object}, 0, 0, 2,
               "closure", "object"))
    return;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1389
case 78: {
  if (closure->debug_info != ((void *)0)) {
    zend_hash_destroy(closure->debug_info);
    _efree((closure->debug_info));
  }
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1390
case 79: {
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1391
case 80: {
  memcpy(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1392
case 81: {
  memset((closure), 0, sizeof(zend_closure));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1393
case 82: {
  memset(closure, 0, sizeof(*(closure)));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1394
case 83: {
  zend_ce_closure->create_object = zend_closure_new;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1395
case 84: {
  zend_ce_closure->serialize = zend_class_serialize_deny;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1396
case 85: {
  zend_ce_closure->unserialize = zend_class_unserialize_deny;
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1397
case 86: {
  zend_error((1 << 0L), "Cannot destroy active lambda function");
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1398
case 87: {
  zend_error((1 << 12L), "Cannot get arguments for calling closure");
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1399
case 88: {
  zend_error((1 << 12L), "Closure object cannot have properties");
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1400
case 89: {
  zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1401
case 90: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1402
case 91: {
  zend_hash_destroy((closure)->debug_info);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1403
case 92: {
  zend_hash_destroy(closure->debug_info);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1404
case 93: {
  zend_object_std_dtor(&(closure)->std);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1405
case 94: {
  zend_object_std_dtor(&closure->std);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1406
case 95: {
  zval_addref_p(&((executor_globals.uninitialized_zval)));
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
#ifdef COMPILE_1407
case 96: {
  zval_addref_p((closure)->this_ptr);
  if (closure->this_ptr) {
    _zval_ptr_dtor((&closure->this_ptr));
  }

  break;
}
#endif
}

efree(closure);
}
/* }}} */

static zend_object_value
zend_closure_new(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
  zend_closure *closure;
  zend_object_value object;

  closure = emalloc(sizeof(zend_closure));
  memset(closure, 0, sizeof(zend_closure));

  // prophet generated patch
  switch (__choose("__ID17")) {
  case 0: {
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#ifdef COMPILE_1408
  case 1: {
    (*(closure)->func.op_array.refcount)++;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1409
  case 2: {
    (class_type)->create_object = zend_closure_new;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1410
  case 3: {
    (class_type)->serialize = zend_class_serialize_deny;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1411
  case 4: {
    (class_type)->unserialize = zend_class_unserialize_deny;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1412
  case 5: {
    (closure) = _emalloc((sizeof(zend_closure)));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1413
  case 6: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1414
  case 7: {
    _efree(((closure)));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1415
  case 8: {
    _efree(((closure)->debug_info));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1416
  case 9: {
    _object_init_ex(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1417
  case 10: {
    _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1418
  case 11: {
    _zval_ptr_dtor((&(closure)->this_ptr));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1419
  case 12: {
    closure = _emalloc((sizeof(zend_closure)));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1420
  case 13: {
    closure_handlers.clone_obj = zend_closure_clone;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1421
  case 14: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1422
  case 15: {
    closure_handlers.get_closure = zend_closure_get_closure;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1423
  case 16: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1424
  case 17: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1425
  case 18: {
    closure_handlers.get_method = zend_closure_get_method;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1426
  case 19: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1427
  case 20: {
    closure_handlers.has_property = zend_closure_has_property;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1428
  case 21: {
    closure_handlers.read_property = zend_closure_read_property;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1429
  case 22: {
    closure_handlers.unset_property = zend_closure_unset_property;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1430
  case 23: {
    closure_handlers.write_property = zend_closure_write_property;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1431
  case 24: {
    destroy_op_array(&(closure)->func.op_array);
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1432
  case 25: {
    exit(1);
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1433
  case 26: {
    if (!__is_neg("17-26", 0, 0, 0, 0, 2, (void **){class_type, closure}, 0, 0,
                  2, "class_type", "closure"))
      zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1434
  case 27: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1435
  case 28: {
    memset(&closure->std, 0, sizeof(*(&closure->std)));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1436
  case 29: {
    memset((closure), 0, sizeof(zend_closure));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1437
  case 30: {
    memset(closure, 0, sizeof(*(closure)));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1438
  case 31: {
    memset(closure, 0, sizeof(zend_closure));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1439
  case 32: {
    object.handle = zend_objects_store_put(
        (closure), (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1440
  case 33: {
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1441
  case 34: {
    object.handlers = &closure_handlers;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1442
  case 35: {
    object_properties_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1443
  case 36: {
    printf(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1444
  case 37: {
    scanf(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1445
  case 38: {
    zend_ce_closure->create_object = zend_closure_new;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1446
  case 39: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1447
  case 40: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1448
  case 41: {
    zend_check_protected(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1449
  case 42: {
    zend_do_implement_interface(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1450
  case 43: {
    zend_do_implement_trait(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1451
  case 44: {
    zend_do_inheritance(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1452
  case 45: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1453
  case 46: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1454
  case 47: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1455
  case 48: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1456
  case 49: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1457
  case 50: {
    zend_hash_destroy((closure)->debug_info);
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1458
  case 51: {
    zend_object_std_dtor(&(closure)->std);
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1459
  case 52: {
    zend_object_std_init(&(closure)->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1460
  case 53: {
    zend_object_std_init(&(closure)->std, class_type);
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1461
  case 54: {
    zend_object_std_init(&closure->std, (class_type));

    break;
  }
#endif
#ifdef COMPILE_1462
  case 55: {
    zend_object_std_init(&closure->std, (class_type));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1463
  case 56: {
    zend_object_std_init(&closure->std, class_type);
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1464
  case 57: {
    zend_objects_new(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1465
  case 58: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
#ifdef COMPILE_1466
  case 59: {
    zval_addref_p((closure)->this_ptr);
    zend_object_std_init(&closure->std, class_type);

    break;
  }
#endif
  }

  // prophet generated patch
  switch (__choose("__ID18")) {
  case 0: {
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#ifdef COMPILE_1467
  case 1: {
    (*(closure)->func.op_array.refcount)++;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1468
  case 2: {
    (class_type)->create_object = zend_closure_new;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1469
  case 3: {
    (class_type)->serialize = zend_class_serialize_deny;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1470
  case 4: {
    (class_type)->unserialize = zend_class_unserialize_deny;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1471
  case 5: {
    (closure) = _emalloc((sizeof(zend_closure)));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1472
  case 6: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1473
  case 7: {
    _efree(((closure)));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1474
  case 8: {
    _efree(((closure)->debug_info));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1475
  case 9: {
    _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1476
  case 10: {
    _zval_ptr_dtor((&(closure)->this_ptr));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1477
  case 11: {
    closure = _emalloc((sizeof(zend_closure)));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1478
  case 12: {
    closure_handlers.clone_obj = zend_closure_clone;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1479
  case 13: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1480
  case 14: {
    closure_handlers.get_closure = zend_closure_get_closure;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1481
  case 15: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1482
  case 16: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1483
  case 17: {
    closure_handlers.get_method = zend_closure_get_method;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1484
  case 18: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1485
  case 19: {
    closure_handlers.has_property = zend_closure_has_property;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1486
  case 20: {
    closure_handlers.read_property = zend_closure_read_property;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1487
  case 21: {
    closure_handlers.unset_property = zend_closure_unset_property;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1488
  case 22: {
    closure_handlers.write_property = zend_closure_write_property;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1489
  case 23: {
    destroy_op_array(&(closure)->func.op_array);
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1490
  case 24: {
    exit(1);
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1491
  case 25: {
    if (!__is_neg("18-25", 0, 0, 0, 0, 2, (void **){closure, class_type}, 0, 0,
                  2, "closure", "class_type"))
      object.handle = zend_objects_store_put(
          closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
          (zend_objects_free_object_storage_t)zend_closure_free_storage,
          ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1492
  case 26: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1493
  case 27: {
    memset(&object, 0, sizeof(*(&object)));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1494
  case 28: {
    memset((closure), 0, sizeof(zend_closure));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1495
  case 29: {
    memset(closure, 0, sizeof(*(closure)));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1496
  case 30: {
    memset(closure, 0, sizeof(zend_closure));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1497
  case 31: {
    object.handle = zend_objects_store_put(
        (closure), (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1498
  case 32: {
    object.handle = zend_objects_store_put(
        (closure), (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1499
  case 33: {
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1500
  case 34: {
    object.handlers = &closure_handlers;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1501
  case 35: {
    zend_ce_closure->create_object = zend_closure_new;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1502
  case 36: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1503
  case 37: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1504
  case 38: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1505
  case 39: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1506
  case 40: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1507
  case 41: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1508
  case 42: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1509
  case 43: {
    zend_hash_destroy((closure)->debug_info);
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1510
  case 44: {
    zend_object_std_dtor(&(closure)->std);
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1511
  case 45: {
    zend_object_std_init(&(closure)->std, class_type);
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1512
  case 46: {
    zend_object_std_init(&closure->std, (class_type));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1513
  case 47: {
    zend_object_std_init(&closure->std, class_type);
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1514
  case 48: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
#ifdef COMPILE_1515
  case 49: {
    zval_addref_p((closure)->this_ptr);
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID19")) {
  case 0: {
    object.handlers = &closure_handlers;

    break;
  }
#ifdef COMPILE_1516
  case 1: {
    (*(closure)->func.op_array.refcount)++;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1517
  case 2: {
    (class_type)->create_object = zend_closure_new;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1518
  case 3: {
    (class_type)->serialize = zend_class_serialize_deny;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1519
  case 4: {
    (class_type)->unserialize = zend_class_unserialize_deny;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1520
  case 5: {
    (closure) = _emalloc((sizeof(zend_closure)));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1521
  case 6: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1522
  case 7: {
    _efree(((closure)));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1523
  case 8: {
    _efree(((closure)->debug_info));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1524
  case 9: {
    _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1525
  case 10: {
    _zval_ptr_dtor((&(closure)->this_ptr));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1526
  case 11: {
    closure = _emalloc((sizeof(zend_closure)));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1527
  case 12: {
    closure_handlers.clone_obj = zend_closure_clone;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1528
  case 13: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1529
  case 14: {
    closure_handlers.get_closure = zend_closure_get_closure;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1530
  case 15: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1531
  case 16: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1532
  case 17: {
    closure_handlers.get_method = zend_closure_get_method;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1533
  case 18: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1534
  case 19: {
    closure_handlers.has_property = zend_closure_has_property;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1535
  case 20: {
    closure_handlers.read_property = zend_closure_read_property;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1536
  case 21: {
    closure_handlers.unset_property = zend_closure_unset_property;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1537
  case 22: {
    closure_handlers.write_property = zend_closure_write_property;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1538
  case 23: {
    destroy_op_array(&(closure)->func.op_array);
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1539
  case 24: {
    exit(1);
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1540
  case 25: {
    if (!__is_neg("19-25", 0, 0, 0, 0, 2, (void **){closure, class_type}, 0, 0,
                  2, "closure", "class_type"))
      object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1541
  case 26: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1542
  case 27: {
    memset(&object, 0, sizeof(*(&object)));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1543
  case 28: {
    memset((closure), 0, sizeof(zend_closure));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1544
  case 29: {
    memset(closure, 0, sizeof(zend_closure));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1545
  case 30: {
    object.handle = zend_objects_store_put(
        (closure), (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1546
  case 31: {
    object.handle = zend_objects_store_put(
        closure, (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)zend_closure_free_storage,
        ((void *)0));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1547
  case 32: {
    object.handlers = &closure_handlers;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1548
  case 33: {
    zend_ce_closure->create_object = zend_closure_new;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1549
  case 34: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1550
  case 35: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1551
  case 36: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1552
  case 37: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1553
  case 38: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1554
  case 39: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1555
  case 40: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1556
  case 41: {
    zend_hash_destroy((closure)->debug_info);
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1557
  case 42: {
    zend_object_std_dtor(&(closure)->std);
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1558
  case 43: {
    zend_object_std_init(&(closure)->std, class_type);
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1559
  case 44: {
    zend_object_std_init(&closure->std, (class_type));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1560
  case 45: {
    zend_object_std_init(&closure->std, class_type);
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1561
  case 46: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    object.handlers = &closure_handlers;

    break;
  }
#endif
#ifdef COMPILE_1562
  case 47: {
    zval_addref_p((closure)->this_ptr);
    object.handlers = &closure_handlers;

    break;
  }
#endif
  }

  return object;
}
/* }}} */

static zend_object_value zend_closure_clone(zval *zobject TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(zobject TSRMLS_CC);
  zval result;

  zend_create_closure(&result, &closure->func, closure->func.common.scope,
                      closure->this_ptr TSRMLS_CC);
  return Z_OBJVAL(result);
}
/* }}} */

int zend_closure_get_closure(zval *obj, zend_class_entry **ce_ptr,
                             zend_function **fptr_ptr,
                             zval **zobj_ptr TSRMLS_DC) /* {{{ */
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

static HashTable *zend_closure_get_debug_info(zval *object,
                                              int *is_temp TSRMLS_DC) /* {{{ */
{
  zend_closure *closure =
      (zend_closure *)zend_object_store_get_object(object TSRMLS_CC);
  zval *val;
  struct _zend_arg_info *arg_info = closure->func.common.arg_info;

  *is_temp = 0;

  if (closure->debug_info == NULL) {
    ALLOC_HASHTABLE(closure->debug_info);
    zend_hash_init(closure->debug_info, 1, NULL, ZVAL_PTR_DTOR, 0);
  }
  if (closure->debug_info->nApplyCount == 0) {
    if (closure->func.type == ZEND_USER_FUNCTION &&
        closure->func.op_array.static_variables) {
      HashTable *static_variables = closure->func.op_array.static_variables;
      MAKE_STD_ZVAL(val);
      array_init(val);
      zend_hash_copy(Z_ARRVAL_P(val), static_variables,
                     (copy_ctor_func_t)zval_add_ref, NULL, sizeof(zval *));
      zend_hash_update(closure->debug_info, "static", sizeof("static"),
                       (void *)&val, sizeof(zval *), NULL);
    }

    if (closure->this_ptr) {
      Z_ADDREF_P(closure->this_ptr);
      zend_symtable_update(closure->debug_info, "this", sizeof("this"),
                           (void *)&closure->this_ptr, sizeof(zval *), NULL);
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
          name_len =
              zend_spprintf(&name, 0, "%s$param%d",
                            arg_info->pass_by_reference ? "&" : "", i + 1);
        }
        info_len = zend_spprintf(&info, 0, "%s",
                                 i >= required ? "<optional>" : "<required>");
        add_assoc_stringl_ex(val, name, name_len + 1, info, info_len, 0);
        efree(name);
        arg_info++;
      }
      zend_hash_update(closure->debug_info, "parameter", sizeof("parameter"),
                       (void *)&val, sizeof(zval *), NULL);
    }
  }

  return closure->debug_info;
}
/* }}} */

/* {{{ proto Closure::__construct()
   Private constructor preventing instantiation */
ZEND_METHOD(Closure, __construct) {
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
            ZEND_ME(Closure, bind, arginfo_closure_bind,
                    ZEND_ACC_PUBLIC | ZEND_ACC_STATIC){NULL, NULL, NULL}};

void zend_register_closure_ce(TSRMLS_D) /* {{{ */
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, "Closure", closure_functions);
  zend_ce_closure = zend_register_internal_class(&ce TSRMLS_CC);
  zend_ce_closure->ce_flags |= ZEND_ACC_FINAL_CLASS;
  zend_ce_closure->create_object = zend_closure_new;
  // prophet generated patch
  switch (__choose("__ID20")) {
  case 0: {
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#ifdef COMPILE_1563
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1564
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1565
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1566
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1567
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1568
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1569
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1570
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1571
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1572
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1573
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1574
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1575
  case 13: {
    exit(1);
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1576
  case 14: {
    if (!__is_neg("20-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1577
  case 15: {
    if (__is_neg("20-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1578
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1579
  case 17: {
    memset(zend_ce_closure, 0, sizeof(*(zend_ce_closure)));
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1580
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1581
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1582
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1583
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1584
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1585
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1586
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1587
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1588
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1589
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    zend_ce_closure->serialize = zend_class_serialize_deny;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID21")) {
  case 0: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#ifdef COMPILE_1590
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1591
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1592
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1593
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1594
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1595
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1596
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1597
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1598
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1599
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1600
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1601
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1602
  case 13: {
    exit(1);
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1603
  case 14: {
    if (!__is_neg("21-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1604
  case 15: {
    if (__is_neg("21-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1605
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1606
  case 17: {
    memset(zend_ce_closure, 0, sizeof(*(zend_ce_closure)));
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1607
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1608
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1609
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1610
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1611
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1612
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1613
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1614
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1615
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
#ifdef COMPILE_1616
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    zend_ce_closure->unserialize = zend_class_unserialize_deny;

    break;
  }
#endif
  }

  // prophet generated patch
  switch (__choose("__ID22")) {
  case 0: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#ifdef COMPILE_1617
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1618
  case 2: {
    bcmp(&closure_handlers, zend_get_std_object_handlers(),
         sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1619
  case 3: {
    closure_handlers.clone_obj = zend_closure_clone;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1620
  case 4: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1621
  case 5: {
    closure_handlers.get_closure = zend_closure_get_closure;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1622
  case 6: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1623
  case 7: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1624
  case 8: {
    closure_handlers.get_method = zend_closure_get_method;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1625
  case 9: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1626
  case 10: {
    closure_handlers.has_property = zend_closure_has_property;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1627
  case 11: {
    closure_handlers.read_property = zend_closure_read_property;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1628
  case 12: {
    closure_handlers.unset_property = zend_closure_unset_property;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1629
  case 13: {
    closure_handlers.write_property = zend_closure_write_property;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1630
  case 14: {
    exit(1);
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1631
  case 15: {
    if (!__is_neg("22-15", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1632
  case 16: {
    if (__is_neg("22-16", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1633
  case 17: {
    memcmp(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1634
  case 18: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1635
  case 19: {
    memmove(&closure_handlers, zend_get_std_object_handlers(),
            sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1636
  case 20: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1637
  case 21: {
    printf(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1638
  case 22: {
    scanf(&closure_handlers, zend_get_std_object_handlers(),
          sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1639
  case 23: {
    zend_ce_closure = zend_register_internal_class(&ce);
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1640
  case 24: {
    zend_ce_closure->create_object = zend_closure_new;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1641
  case 25: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1642
  case 26: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1643
  case 27: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1644
  case 28: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1645
  case 29: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1646
  case 30: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1647
  case 31: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
#ifdef COMPILE_1648
  case 32: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID23")) {
  case 0: {
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#ifdef COMPILE_1649
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1650
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1651
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1652
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1653
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1654
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1655
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1656
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1657
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1658
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1659
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1660
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1661
  case 13: {
    exit(1);
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1662
  case 14: {
    if (!__is_neg("23-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1663
  case 15: {
    if (__is_neg("23-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1664
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1665
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1666
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1667
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1668
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1669
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1670
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1671
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1672
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1673
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1674
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
#ifdef COMPILE_1675
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.get_constructor = zend_closure_get_constructor;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID24")) {
  case 0: {
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#ifdef COMPILE_1676
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1677
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1678
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1679
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1680
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1681
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1682
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1683
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1684
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1685
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1686
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1687
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1688
  case 13: {
    exit(1);
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1689
  case 14: {
    if (!__is_neg("24-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1690
  case 15: {
    if (__is_neg("24-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1691
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1692
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1693
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1694
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1695
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1696
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1697
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1698
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1699
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1700
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1701
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
#ifdef COMPILE_1702
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.get_method = zend_closure_get_method;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID25")) {
  case 0: {
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#ifdef COMPILE_1703
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1704
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1705
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1706
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1707
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1708
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1709
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1710
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1711
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1712
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1713
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1714
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1715
  case 13: {
    exit(1);
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1716
  case 14: {
    if (!__is_neg("25-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1717
  case 15: {
    if (__is_neg("25-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1718
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1719
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1720
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1721
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1722
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1723
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1724
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1725
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1726
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1727
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1728
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
#ifdef COMPILE_1729
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.write_property = zend_closure_write_property;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID26")) {
  case 0: {
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#ifdef COMPILE_1730
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1731
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1732
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1733
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1734
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1735
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1736
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1737
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1738
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1739
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1740
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1741
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1742
  case 13: {
    exit(1);
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1743
  case 14: {
    if (!__is_neg("26-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1744
  case 15: {
    if (__is_neg("26-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1745
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1746
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1747
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1748
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1749
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1750
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1751
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1752
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1753
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1754
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1755
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
#ifdef COMPILE_1756
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.read_property = zend_closure_read_property;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID27")) {
  case 0: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#ifdef COMPILE_1757
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1758
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1759
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1760
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1761
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1762
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1763
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1764
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1765
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1766
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1767
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1768
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1769
  case 13: {
    exit(1);
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1770
  case 14: {
    if (!__is_neg("27-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1771
  case 15: {
    if (__is_neg("27-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1772
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1773
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1774
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1775
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1776
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1777
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1778
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1779
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1780
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1781
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1782
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
#ifdef COMPILE_1783
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID28")) {
  case 0: {
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#ifdef COMPILE_1784
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1785
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1786
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1787
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1788
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1789
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1790
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1791
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1792
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1793
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1794
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1795
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1796
  case 13: {
    exit(1);
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1797
  case 14: {
    if (!__is_neg("28-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1798
  case 15: {
    if (__is_neg("28-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1799
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1800
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1801
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1802
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1803
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1804
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1805
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1806
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1807
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1808
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1809
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
#ifdef COMPILE_1810
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.has_property = zend_closure_has_property;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID29")) {
  case 0: {
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#ifdef COMPILE_1811
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1812
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1813
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1814
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1815
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1816
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1817
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1818
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1819
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1820
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1821
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1822
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1823
  case 13: {
    exit(1);
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1824
  case 14: {
    if (!__is_neg("29-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1825
  case 15: {
    if (__is_neg("29-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1826
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1827
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1828
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1829
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1830
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1831
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1832
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1833
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1834
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1835
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1836
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
#ifdef COMPILE_1837
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.unset_property = zend_closure_unset_property;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID30")) {
  case 0: {
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#ifdef COMPILE_1838
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1839
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1840
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1841
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1842
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1843
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1844
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1845
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1846
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1847
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1848
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1849
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1850
  case 13: {
    exit(1);
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1851
  case 14: {
    if (!__is_neg("30-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1852
  case 15: {
    if (__is_neg("30-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1853
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1854
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1855
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1856
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1857
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1858
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1859
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1860
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1861
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1862
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1863
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
#ifdef COMPILE_1864
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.compare_objects = zend_closure_compare_objects;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID31")) {
  case 0: {
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#ifdef COMPILE_1865
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1866
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1867
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1868
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1869
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1870
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1871
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1872
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1873
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1874
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1875
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1876
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1877
  case 13: {
    exit(1);
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1878
  case 14: {
    if (!__is_neg("31-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1879
  case 15: {
    if (__is_neg("31-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1880
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1881
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1882
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1883
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1884
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1885
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1886
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1887
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1888
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1889
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1890
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
#ifdef COMPILE_1891
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.clone_obj = zend_closure_clone;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID32")) {
  case 0: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#ifdef COMPILE_1892
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1893
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1894
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1895
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1896
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1897
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1898
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1899
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1900
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1901
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1902
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1903
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1904
  case 13: {
    exit(1);
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1905
  case 14: {
    if (!__is_neg("32-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1906
  case 15: {
    if (__is_neg("32-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1907
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1908
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1909
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1910
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1911
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1912
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1913
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1914
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1915
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1916
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1917
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
#ifdef COMPILE_1918
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.get_debug_info = zend_closure_get_debug_info;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID33")) {
  case 0: {
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#ifdef COMPILE_1919
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1920
  case 2: {
    closure_handlers.clone_obj = zend_closure_clone;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1921
  case 3: {
    closure_handlers.compare_objects = zend_closure_compare_objects;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1922
  case 4: {
    closure_handlers.get_closure = zend_closure_get_closure;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1923
  case 5: {
    closure_handlers.get_constructor = zend_closure_get_constructor;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1924
  case 6: {
    closure_handlers.get_debug_info = zend_closure_get_debug_info;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1925
  case 7: {
    closure_handlers.get_method = zend_closure_get_method;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1926
  case 8: {
    closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1927
  case 9: {
    closure_handlers.has_property = zend_closure_has_property;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1928
  case 10: {
    closure_handlers.read_property = zend_closure_read_property;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1929
  case 11: {
    closure_handlers.unset_property = zend_closure_unset_property;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1930
  case 12: {
    closure_handlers.write_property = zend_closure_write_property;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1931
  case 13: {
    exit(1);
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1932
  case 14: {
    if (!__is_neg("33-14", 0, 0, 0, 0, 3,
                  (void **){zend_ce_closure, zend_ce_closure,
                            zend_new_interned_string},
                  0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                  "zend_new_interned_string"))
      closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1933
  case 15: {
    if (__is_neg("33-15", 0, 0, 0, 0, 3,
                 (void **){zend_ce_closure, zend_ce_closure,
                           zend_new_interned_string},
                 0, 0, 3, "zend_ce_closure", "zend_ce_closure",
                 "zend_new_interned_string"))
      return;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1934
  case 16: {
    memcpy(&closure_handlers, zend_get_std_object_handlers(),
           sizeof(zend_object_handlers));
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1935
  case 17: {
    memset(&closure_handlers, 0, sizeof(*(&closure_handlers)));
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1936
  case 18: {
    zend_ce_closure = zend_register_internal_class(&ce);
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1937
  case 19: {
    zend_ce_closure->create_object = zend_closure_new;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1938
  case 20: {
    zend_ce_closure->serialize = zend_class_serialize_deny;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1939
  case 21: {
    zend_ce_closure->unserialize = zend_class_unserialize_deny;
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1940
  case 22: {
    zend_error((1 << 0L), "Cannot destroy active lambda function");
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1941
  case 23: {
    zend_error((1 << 12L), "Cannot get arguments for calling closure");
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1942
  case 24: {
    zend_error((1 << 12L), "Closure object cannot have properties");
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1943
  case 25: {
    zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1944
  case 26: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
#ifdef COMPILE_1945
  case 27: {
    zval_addref_p(&((executor_globals.uninitialized_zval)));
    closure_handlers.get_closure = zend_closure_get_closure;

    break;
  }
#endif
  }
}
/* }}} */

ZEND_API void zend_create_closure(zval *res, zend_function *func,
                                  zend_class_entry *scope,
                                  zval *this_ptr TSRMLS_DC) /* {{{ */
{
  zend_closure *closure;

  object_init_ex(res, zend_ce_closure);

  closure = (zend_closure *)zend_object_store_get_object(res TSRMLS_CC);

  closure->func = *func;

  if (closure->func.type == ZEND_USER_FUNCTION) {
    if (closure->func.op_array.static_variables) {
      HashTable *static_variables = closure->func.op_array.static_variables;

      ALLOC_HASHTABLE(closure->func.op_array.static_variables);
      // prophet generated patch
      switch (__choose("__ID34")) {
      case 0: {
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#ifdef COMPILE_1946
      case 1: {
        ((closure)->func.op_array.static_variables) =
            (HashTable *)_emalloc((sizeof(HashTable)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1947
      case 2: {
        (*(closure)->func.op_array.refcount)++;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1948
      case 3: {
        (*closure->func.op_array.refcount)++;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1949
      case 4: {
        (closure) = (zend_closure *)zend_object_store_get_object(res);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1950
      case 5: {
        (closure)->func = *func;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1951
      case 6: {
        (closure)->func.common.scope = scope;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1952
      case 7: {
        (closure)->func.op_array.run_time_cache = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1953
      case 8: {
        (closure)->this_ptr = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1954
      case 9: {
        (closure)->this_ptr = this_ptr;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1955
      case 10: {
        (closure->func.op_array.static_variables) =
            (HashTable *)_emalloc((sizeof(HashTable)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1956
      case 11: {
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1957
      case 12: {
        (res) = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1958
      case 13: {
        (scope) = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1959
      case 14: {
        (scope)->create_object = zend_closure_new;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1960
      case 15: {
        (scope)->serialize = zend_class_serialize_deny;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1961
      case 16: {
        (scope)->unserialize = zend_class_unserialize_deny;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1962
      case 17: {
        (this_ptr) = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1963
      case 18: {
        _array_init(((res)), 0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1964
      case 19: {
        _array_init(((this_ptr)), 0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1965
      case 20: {
        _efree(((closure)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1966
      case 21: {
        _efree(((closure)->debug_info));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1967
      case 22: {
        _efree(((func)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1968
      case 23: {
        _efree(((func)->internal_function.function_name));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1969
      case 24: {
        _efree(((res)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1970
      case 25: {
        _efree(((this_ptr)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1971
      case 26: {
        _object_init_ex(((res)), (zend_ce_closure));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1972
      case 27: {
        _object_init_ex(((this_ptr)), (zend_ce_closure));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1973
      case 28: {
        _object_init_ex((res), ((scope)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1974
      case 29: {
        _object_init_ex((res), (zend_ce_closure));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1975
      case 30: {
        _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                        ((void (*)(void *))_zval_ptr_dtor), (0));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1976
      case 31: {
        _zend_hash_init(((closure)->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1977
      case 32: {
        _zend_hash_init(((closure)->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1978
      case 33: {
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements((static_variables))),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1979
      case 34: {
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements((static_variables))),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1980
      case 35: {
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1981
      case 36: {
        _zend_ts_hash_init((closure->func.op_array.static_variables),
                           (zend_hash_num_elements(static_variables)),
                           (((void *)0)), ((void (*)(void *))_zval_ptr_dtor),
                           (0));

        break;
      }
#endif
#ifdef COMPILE_1982
      case 37: {
        _zval_copy_ctor(((res)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1983
      case 38: {
        _zval_copy_ctor(((this_ptr)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1984
      case 39: {
        _zval_copy_ctor_func((res));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1985
      case 40: {
        _zval_copy_ctor_func((this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1986
      case 41: {
        _zval_dtor(((res)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1987
      case 42: {
        _zval_dtor(((this_ptr)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1988
      case 43: {
        _zval_dtor_func((res));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1989
      case 44: {
        _zval_dtor_func((this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1990
      case 45: {
        _zval_ptr_dtor((&(closure)->this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1991
      case 46: {
        _zval_ptr_dtor((&(res)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1992
      case 47: {
        _zval_ptr_dtor((&(this_ptr)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1993
      case 48: {
        closure = (zend_closure *)zend_object_store_get_object(res);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1994
      case 49: {
        closure->func = *(func);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1995
      case 50: {
        closure->func = *func;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1996
      case 51: {
        closure->func.common.scope = (scope);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1997
      case 52: {
        closure->func.common.scope = scope;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1998
      case 53: {
        closure->func.op_array.run_time_cache = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_1999
      case 54: {
        closure->this_ptr = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2000
      case 55: {
        closure->this_ptr = (res);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2001
      case 56: {
        closure->this_ptr = (this_ptr);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2002
      case 57: {
        closure->this_ptr = this_ptr;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2003
      case 58: {
        closure_handlers.clone_obj = zend_closure_clone;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2004
      case 59: {
        closure_handlers.compare_objects = zend_closure_compare_objects;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2005
      case 60: {
        closure_handlers.get_closure = zend_closure_get_closure;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2006
      case 61: {
        closure_handlers.get_constructor = zend_closure_get_constructor;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2007
      case 62: {
        closure_handlers.get_debug_info = zend_closure_get_debug_info;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2008
      case 63: {
        closure_handlers.get_method = zend_closure_get_method;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2009
      case 64: {
        closure_handlers.get_property_ptr_ptr =
            zend_closure_get_property_ptr_ptr;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2010
      case 65: {
        closure_handlers.has_property = zend_closure_has_property;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2011
      case 66: {
        closure_handlers.read_property = zend_closure_read_property;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2012
      case 67: {
        closure_handlers.unset_property = zend_closure_unset_property;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2013
      case 68: {
        closure_handlers.write_property = zend_closure_write_property;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2014
      case 69: {
        convert_to_boolean((res));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2015
      case 70: {
        convert_to_boolean((this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2016
      case 71: {
        destroy_op_array(&(closure)->func.op_array);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2017
      case 72: {
        exit(1);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2018
      case 73: {
        gc_remove_zval_from_buffer((res));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2019
      case 74: {
        gc_remove_zval_from_buffer((this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2020
      case 75: {
        gc_zval_check_possible_root(((res)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2021
      case 76: {
        gc_zval_check_possible_root(((this_ptr)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2022
      case 77: {
        gc_zval_possible_root((res));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2023
      case 78: {
        gc_zval_possible_root((this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2024
      case 79: {
        i_zval_ptr_dtor((res));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2025
      case 80: {
        i_zval_ptr_dtor((this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2026
      case 81: {
        if (!__is_neg("34-81", 0, 0, 0, 0, 8,
                      (void **){closure, static_variables, func, scope, res,
                                zend_ce_closure, zend_ce_closure, this_ptr},
                      0, 0, 8, "closure", "static_variables", "func", "scope",
                      "res", "zend_ce_closure", "zend_ce_closure", "this_ptr"))
          _zend_hash_init((closure->func.op_array.static_variables),
                          (zend_hash_num_elements(static_variables)),
                          (((void *)0)), ((void (*)(void *))_zval_ptr_dtor),
                          (0));

        break;
      }
#endif
#ifdef COMPILE_2027
      case 82: {
        if (__is_neg("34-82", 0, 0, 0, 0, 8,
                     (void **){closure, static_variables, func, scope, res,
                               zend_ce_closure, zend_ce_closure, this_ptr},
                     0, 0, 8, "closure", "static_variables", "func", "scope",
                     "res", "zend_ce_closure", "zend_ce_closure", "this_ptr"))
          return;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2028
      case 83: {
        if (scope && !instanceof_function(scope, func->common.scope)) {
          zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                     func->common.scope->name, func->common.function_name,
                     scope->name);
          scope = ((void *)0);
        }
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2029
      case 84: {
        if (scope && this_ptr && (func->common.fn_flags & 1) == 0 &&
            !instanceof_function(zend_get_class_entry(&(*this_ptr)),
                                 closure->func.common.scope)) {
          zend_error((1 << 1L),
                     "Cannot bind function %s::%s to object of class %s",
                     func->common.scope->name, func->common.function_name,
                     zend_get_class_entry(&(*this_ptr))->name);
          scope = ((void *)0);
          this_ptr = ((void *)0);
        }
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2030
      case 85: {
        if (this_ptr && (closure->func.common.fn_flags & 1) == 0) {
          closure->this_ptr = this_ptr;
          zval_addref_p(this_ptr);
        } else {
          closure->func.common.fn_flags |= 1;
          closure->this_ptr = ((void *)0);
        }
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2031
      case 86: {
        memcpy(&closure_handlers, zend_get_std_object_handlers(),
               sizeof(zend_object_handlers));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2032
      case 87: {
        memset(&closure->func, 0, sizeof(*(&closure->func)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2033
      case 88: {
        memset(&closure->func.op_array, 0, sizeof(*(&closure->func.op_array)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2034
      case 89: {
        memset((closure), 0, sizeof(zend_closure));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2035
      case 90: {
        memset(closure, 0, sizeof(*(closure)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2036
      case 91: {
        printf((closure->func.op_array.static_variables),
               (zend_hash_num_elements(static_variables)), (((void *)0)),
               ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2037
      case 92: {
        scanf((closure->func.op_array.static_variables),
              (zend_hash_num_elements(static_variables)), (((void *)0)),
              ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2038
      case 93: {
        scope = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2039
      case 94: {
        this_ptr = ((void *)0);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2040
      case 95: {
        zend_ce_closure->create_object = zend_closure_new;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2041
      case 96: {
        zend_ce_closure->serialize = zend_class_serialize_deny;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2042
      case 97: {
        zend_ce_closure->unserialize = zend_class_unserialize_deny;
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2043
      case 98: {
        zend_error((1 << 0L), "Cannot destroy active lambda function");
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2044
      case 99: {
        zend_error((1 << 12L), "Cannot get arguments for calling closure");
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2045
      case 100: {
        zend_error((1 << 12L), "Closure object cannot have properties");
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2046
      case 101: {
        zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2047
      case 102: {
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   (func)->common.scope->name, func->common.function_name,
                   zend_get_class_entry(&(*this_ptr))->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2048
      case 103: {
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, (func)->common.function_name,
                   zend_get_class_entry(&(*this_ptr))->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2049
      case 104: {
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   zend_get_class_entry(&(*(res)))->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2050
      case 105: {
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   zend_get_class_entry(&(*(this_ptr)))->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2051
      case 106: {
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   zend_get_class_entry(&(*this_ptr))->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2052
      case 107: {
        zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                   (func)->common.scope->name, func->common.function_name,
                   scope->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2053
      case 108: {
        zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, (func)->common.function_name,
                   scope->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2054
      case 109: {
        zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, func->common.function_name,
                   (scope)->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2055
      case 110: {
        zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, func->common.function_name,
                   scope->name);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2056
      case 111: {
        zend_error((1 << 3L), "A non well formed numeric value encountered");
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2057
      case 112: {
        zend_hash_apply_with_arguments(
            (static_variables), (apply_func_args_t)zval_copy_static_var, 1,
            closure->func.op_array.static_variables);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2058
      case 113: {
        zend_hash_apply_with_arguments(
            static_variables, (apply_func_args_t)zval_copy_static_var, 1,
            (closure)->func.op_array.static_variables);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2059
      case 114: {
        zend_hash_apply_with_arguments(
            static_variables, (apply_func_args_t)zval_copy_static_var, 1,
            closure->func.op_array.static_variables);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2060
      case 115: {
        zend_hash_destroy((closure)->debug_info);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2061
      case 116: {
        zend_object_std_dtor(&(closure)->std);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2062
      case 117: {
        zval_addref_p(&((executor_globals.uninitialized_zval)));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2063
      case 118: {
        zval_addref_p((closure)->this_ptr);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2064
      case 119: {
        zval_addref_p((res));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2065
      case 120: {
        zval_addref_p((this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2066
      case 121: {
        zval_addref_p(this_ptr);
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2067
      case 122: {
        zval_unset_isref_p((res));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_2068
      case 123: {
        zval_unset_isref_p((this_ptr));
        _zend_hash_init((closure->func.op_array.static_variables),
                        (zend_hash_num_elements(static_variables)),
                        (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
      }
      zend_hash_apply_with_arguments(static_variables TSRMLS_CC,
                                     (apply_func_args_t)zval_copy_static_var, 1,
                                     closure->func.op_array.static_variables);
    }
    closure->func.op_array.run_time_cache = NULL;
    (*closure->func.op_array.refcount)++;
  } else {
    /* verify that we aren't binding internal function to a wrong scope */
    if (func->common.scope != NULL) {
      if (scope && !instanceof_function(scope, func->common.scope TSRMLS_CC)) {
        zend_error(E_WARNING, "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, func->common.function_name,
                   scope->name);
        scope = NULL;
      }
      if (scope && this_ptr && (func->common.fn_flags & ZEND_ACC_STATIC) == 0 &&
          !instanceof_function(Z_OBJCE_P(this_ptr),
                               closure->func.common.scope TSRMLS_CC)) {
        zend_error(E_WARNING,
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   Z_OBJCE_P(this_ptr)->name);
        scope = NULL;
        this_ptr = NULL;
      }
    } else {
      /* if it's a free function, we won't set scope & this since they're
       * meaningless */
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
