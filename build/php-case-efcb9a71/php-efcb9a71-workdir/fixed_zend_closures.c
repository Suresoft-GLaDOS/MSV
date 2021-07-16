int __get_mutant();
int __is_neg(const char *location, int count, ...);
int __abst_hole();
int __choose(const char *);
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
    free_alloca(lc_name, use_heap);
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
      // prophet generated patch
      {
        switch (__choose("__SWITCH0")) {
        case 0:
          break;
#ifdef COMPILE_0
        case 1: {
          // AddAndReplaceKind
          (*(closure)->func.op_array.refcount)++;

          break;
        }
#endif
#ifdef COMPILE_1
        case 2: {
          // AddAndReplaceKind
          (ex) = ex->prev_execute_data;

          break;
        }
#endif
#ifdef COMPILE_2
        case 3: {
          // AddAndReplaceKind
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));

          break;
        }
#endif
#ifdef COMPILE_3
        case 4: {
          // AddAndReplaceKind
          *((executor_globals.argument_stack)->top++) = (object);

          break;
        }
#endif
#ifdef COMPILE_4
        case 5: {
          // AddAndReplaceKind
          _efree(((closure)));

          break;
        }
#endif
#ifdef COMPILE_5
        case 6: {
          // AddAndReplaceKind
          _efree(((closure)->debug_info));

          break;
        }
#endif
#ifdef COMPILE_6
        case 7: {
          // AddAndReplaceKind
          _efree((closure));

          break;
        }
#endif
#ifdef COMPILE_7
        case 8: {
          // AddAndReplaceKind
          _efree((closure->debug_info));

          break;
        }
#endif
#ifdef COMPILE_8
        case 9: {
          // AddAndReplaceKind
          _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                          ((void (*)(void *))_zval_ptr_dtor), (0));

          break;
        }
#endif
#ifdef COMPILE_9
        case 10: {
          // AddAndReplaceKind
          _zval_ptr_dtor((&(closure)->this_ptr));

          break;
        }
#endif
#ifdef COMPILE_10
        case 11: {
          // AddAndReplaceKind
          _zval_ptr_dtor((&closure->this_ptr));

          break;
        }
#endif
#ifdef COMPILE_11
        case 12: {
          // AddAndReplaceKind
          closure_handlers.clone_obj = zend_closure_clone;

          break;
        }
#endif
#ifdef COMPILE_12
        case 13: {
          // AddAndReplaceKind
          closure_handlers.compare_objects = zend_closure_compare_objects;

          break;
        }
#endif
#ifdef COMPILE_13
        case 14: {
          // AddAndReplaceKind
          closure_handlers.get_closure = zend_closure_get_closure;

          break;
        }
#endif
#ifdef COMPILE_14
        case 15: {
          // AddAndReplaceKind
          closure_handlers.get_constructor = zend_closure_get_constructor;

          break;
        }
#endif
#ifdef COMPILE_15
        case 16: {
          // AddAndReplaceKind
          closure_handlers.get_debug_info = zend_closure_get_debug_info;

          break;
        }
#endif
#ifdef COMPILE_16
        case 17: {
          // AddAndReplaceKind
          closure_handlers.get_method = zend_closure_get_method;

          break;
        }
#endif
#ifdef COMPILE_17
        case 18: {
          // AddAndReplaceKind
          closure_handlers.get_property_ptr_ptr =
              zend_closure_get_property_ptr_ptr;

          break;
        }
#endif
#ifdef COMPILE_18
        case 19: {
          // AddAndReplaceKind
          closure_handlers.has_property = zend_closure_has_property;

          break;
        }
#endif
#ifdef COMPILE_19
        case 20: {
          // AddAndReplaceKind
          closure_handlers.read_property = zend_closure_read_property;

          break;
        }
#endif
#ifdef COMPILE_20
        case 21: {
          // AddAndReplaceKind
          closure_handlers.unset_property = zend_closure_unset_property;

          break;
        }
#endif
#ifdef COMPILE_21
        case 22: {
          // AddAndReplaceKind
          closure_handlers.write_property = zend_closure_write_property;

          break;
        }
#endif
#ifdef COMPILE_22
        case 23: {
          // AddAndReplaceKind
          destroy_op_array(&(closure)->func.op_array);

          break;
        }
#endif
#ifdef COMPILE_23
        case 24: {
          // AddAndReplaceKind
          destroy_op_array(&closure->func.op_array);

          break;
        }
#endif
#ifdef COMPILE_24
        case 25: {
          // AddAndReplaceKind
          ex = (ex)->prev_execute_data;

          break;
        }
#endif
#ifdef COMPILE_25
        case 26: {
          // AddAndReplaceKind
          ex = ex->prev_execute_data;

          break;
        }
#endif
#ifdef COMPILE_26
        case 27: {
          // AddAndReplaceKind
          exit(1);

          break;
        }
#endif
#ifdef COMPILE_27
        case 28: {
          // AddAndReplaceKind
          if (closure->debug_info != ((void *)0)) {
            zend_hash_destroy(closure->debug_info);
            _efree((closure->debug_info));
          }

          break;
        }
#endif
#ifdef COMPILE_28
        case 29: {
          // AddAndReplaceKind
          if (closure->this_ptr) {
            _zval_ptr_dtor((&closure->this_ptr));
          }

          break;
        }
#endif
#ifdef COMPILE_29
        case 30: {
          // AddAndReplaceKind
          if (ex->op_array == &closure->func.op_array) {
            zend_error((1 << 0L), "Cannot destroy active lambda function");
          }

          break;
        }
#endif
#ifdef COMPILE_30
        case 31: {
          // AddAndReplaceKind
          memcpy(&closure_handlers, zend_get_std_object_handlers(),
                 sizeof(zend_object_handlers));

          break;
        }
#endif
#ifdef COMPILE_31
        case 32: {
          // AddAndReplaceKind
          memset((closure), 0, sizeof(zend_closure));

          break;
        }
#endif
#ifdef COMPILE_32
        case 33: {
          // AddAndReplaceKind
          zend_ce_closure->create_object = zend_closure_new;

          break;
        }
#endif
#ifdef COMPILE_33
        case 34: {
          // AddAndReplaceKind
          zend_ce_closure->serialize = zend_class_serialize_deny;

          break;
        }
#endif
#ifdef COMPILE_34
        case 35: {
          // AddAndReplaceKind
          zend_ce_closure->unserialize = zend_class_unserialize_deny;

          break;
        }
#endif
#ifdef COMPILE_35
        case 36: {
          // AddAndReplaceKind
          zend_error((1 << 0L), "Cannot destroy active lambda function");

          break;
        }
#endif
#ifdef COMPILE_36
        case 37: {
          // AddAndReplaceKind
          zend_error((1 << 12L), "Cannot get arguments for calling closure");

          break;
        }
#endif
#ifdef COMPILE_37
        case 38: {
          // AddAndReplaceKind
          zend_error((1 << 12L), "Closure object cannot have properties");

          break;
        }
#endif
#ifdef COMPILE_38
        case 39: {
          // AddAndReplaceKind
          zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");

          break;
        }
#endif
#ifdef COMPILE_39
        case 40: {
          // AddAndReplaceKind
          zend_error((1 << 3L), "A non well formed numeric value encountered");

          break;
        }
#endif
#ifdef COMPILE_40
        case 41: {
          // AddAndReplaceKind
          zend_hash_destroy((closure)->debug_info);

          break;
        }
#endif
#ifdef COMPILE_41
        case 42: {
          // AddAndReplaceKind
          zend_hash_destroy(closure->debug_info);

          break;
        }
#endif
#ifdef COMPILE_42
        case 43: {
          // AddAndReplaceKind
          zend_object_std_dtor(&(closure)->std);

          break;
        }
#endif
#ifdef COMPILE_43
        case 44: {
          // AddAndReplaceKind
          zend_object_std_dtor(&closure->std);

          break;
        }
#endif
#ifdef COMPILE_44
        case 45: {
          // AddAndReplaceKind
          zval_addref_p(&((executor_globals.uninitialized_zval)));

          break;
        }
#endif
#ifdef COMPILE_45
        case 46: {
          // AddAndReplaceKind
          zval_addref_p((closure)->this_ptr);

          break;
        }
#endif
#ifdef COMPILE_46
        case 47: {
          // AddInitKind
          memset(&closure->func, 0, sizeof(*(&closure->func)));

          break;
        }
#endif
#ifdef COMPILE_47
        case 48: {
          // AddInitKind
          memset(&closure->func.op_array, 0,
                 sizeof(*(&closure->func.op_array)));

          break;
        }
#endif
#ifdef COMPILE_48
        case 49: {
          // AddInitKind
          memset(closure, 0, sizeof(*(closure)));

          break;
        }
#endif
#ifdef COMPILE_49
        case 50: {
          // AddInitKind
          memset(ex, 0, sizeof(*(ex)));

          break;
        }
#endif
#ifdef COMPILE_50
        case 51: {
          // IfExitKind
          if (__is_neg(
                  "0-51", 51, &(closure), sizeof(closure), &(ex), sizeof(ex),
                  &(ex->op_array), sizeof(ex->op_array),
                  &(ex->prev_execute_data), sizeof(ex->prev_execute_data),
                  &(object), sizeof(object), &(closure->debug_info),
                  sizeof(closure->debug_info), &(closure->this_ptr),
                  sizeof(closure->this_ptr), &(ex->opline), sizeof(ex->opline),
                  &(ex->fbc), sizeof(ex->fbc), &(ex->called_scope),
                  sizeof(ex->called_scope), &(ex->object), sizeof(ex->object),
                  &(ex->Ts), sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                  &(ex->symbol_table), sizeof(ex->symbol_table),
                  &(ex->old_error_reporting), sizeof(ex->old_error_reporting),
                  &(ex->nested), sizeof(ex->nested),
                  &(ex->original_return_value),
                  sizeof(ex->original_return_value), &(ex->current_scope),
                  sizeof(ex->current_scope), &(ex->current_called_scope),
                  sizeof(ex->current_called_scope), &(ex->current_this),
                  sizeof(ex->current_this), &(ex->current_object),
                  sizeof(ex->current_object), &(closure->func.op_array.type),
                  sizeof(closure->func.op_array.type),
                  &(closure->func.op_array.function_name),
                  sizeof(closure->func.op_array.function_name),
                  &(closure->func.op_array.scope),
                  sizeof(closure->func.op_array.scope),
                  &(closure->func.op_array.fn_flags),
                  sizeof(closure->func.op_array.fn_flags),
                  &(closure->func.op_array.prototype),
                  sizeof(closure->func.op_array.prototype),
                  &(closure->func.op_array.num_args),
                  sizeof(closure->func.op_array.num_args),
                  &(closure->func.op_array.required_num_args),
                  sizeof(closure->func.op_array.required_num_args),
                  &(closure->func.op_array.arg_info),
                  sizeof(closure->func.op_array.arg_info),
                  &(closure->func.op_array.refcount),
                  sizeof(closure->func.op_array.refcount),
                  &(closure->func.op_array.opcodes),
                  sizeof(closure->func.op_array.opcodes),
                  &(closure->func.op_array.last),
                  sizeof(closure->func.op_array.last),
                  &(closure->func.op_array.vars),
                  sizeof(closure->func.op_array.vars),
                  &(closure->func.op_array.last_var),
                  sizeof(closure->func.op_array.last_var),
                  &(closure->func.op_array.T), sizeof(closure->func.op_array.T),
                  &(closure->func.op_array.brk_cont_array),
                  sizeof(closure->func.op_array.brk_cont_array),
                  &(closure->func.op_array.last_brk_cont),
                  sizeof(closure->func.op_array.last_brk_cont),
                  &(closure->func.op_array.try_catch_array),
                  sizeof(closure->func.op_array.try_catch_array),
                  &(closure->func.op_array.last_try_catch),
                  sizeof(closure->func.op_array.last_try_catch),
                  &(closure->func.op_array.static_variables),
                  sizeof(closure->func.op_array.static_variables),
                  &(closure->func.op_array.this_var),
                  sizeof(closure->func.op_array.this_var),
                  &(closure->func.op_array.filename),
                  sizeof(closure->func.op_array.filename),
                  &(closure->func.op_array.line_start),
                  sizeof(closure->func.op_array.line_start),
                  &(closure->func.op_array.line_end),
                  sizeof(closure->func.op_array.line_end),
                  &(closure->func.op_array.doc_comment),
                  sizeof(closure->func.op_array.doc_comment),
                  &(closure->func.op_array.doc_comment_len),
                  sizeof(closure->func.op_array.doc_comment_len),
                  &(closure->func.op_array.early_binding),
                  sizeof(closure->func.op_array.early_binding),
                  &(closure->func.op_array.literals),
                  sizeof(closure->func.op_array.literals),
                  &(closure->func.op_array.last_literal),
                  sizeof(closure->func.op_array.last_literal),
                  &(closure->func.op_array.run_time_cache),
                  sizeof(closure->func.op_array.run_time_cache),
                  &(closure->func.op_array.last_cache_slot),
                  sizeof(closure->func.op_array.last_cache_slot)))
            break;

          break;
        }
#endif
#ifdef COMPILE_51
        case 52: {
          // IfExitKind
          if (__is_neg(
                  "0-52", 51, &(closure), sizeof(closure), &(ex), sizeof(ex),
                  &(ex->op_array), sizeof(ex->op_array),
                  &(ex->prev_execute_data), sizeof(ex->prev_execute_data),
                  &(object), sizeof(object), &(closure->debug_info),
                  sizeof(closure->debug_info), &(closure->this_ptr),
                  sizeof(closure->this_ptr), &(ex->opline), sizeof(ex->opline),
                  &(ex->fbc), sizeof(ex->fbc), &(ex->called_scope),
                  sizeof(ex->called_scope), &(ex->object), sizeof(ex->object),
                  &(ex->Ts), sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                  &(ex->symbol_table), sizeof(ex->symbol_table),
                  &(ex->old_error_reporting), sizeof(ex->old_error_reporting),
                  &(ex->nested), sizeof(ex->nested),
                  &(ex->original_return_value),
                  sizeof(ex->original_return_value), &(ex->current_scope),
                  sizeof(ex->current_scope), &(ex->current_called_scope),
                  sizeof(ex->current_called_scope), &(ex->current_this),
                  sizeof(ex->current_this), &(ex->current_object),
                  sizeof(ex->current_object), &(closure->func.op_array.type),
                  sizeof(closure->func.op_array.type),
                  &(closure->func.op_array.function_name),
                  sizeof(closure->func.op_array.function_name),
                  &(closure->func.op_array.scope),
                  sizeof(closure->func.op_array.scope),
                  &(closure->func.op_array.fn_flags),
                  sizeof(closure->func.op_array.fn_flags),
                  &(closure->func.op_array.prototype),
                  sizeof(closure->func.op_array.prototype),
                  &(closure->func.op_array.num_args),
                  sizeof(closure->func.op_array.num_args),
                  &(closure->func.op_array.required_num_args),
                  sizeof(closure->func.op_array.required_num_args),
                  &(closure->func.op_array.arg_info),
                  sizeof(closure->func.op_array.arg_info),
                  &(closure->func.op_array.refcount),
                  sizeof(closure->func.op_array.refcount),
                  &(closure->func.op_array.opcodes),
                  sizeof(closure->func.op_array.opcodes),
                  &(closure->func.op_array.last),
                  sizeof(closure->func.op_array.last),
                  &(closure->func.op_array.vars),
                  sizeof(closure->func.op_array.vars),
                  &(closure->func.op_array.last_var),
                  sizeof(closure->func.op_array.last_var),
                  &(closure->func.op_array.T), sizeof(closure->func.op_array.T),
                  &(closure->func.op_array.brk_cont_array),
                  sizeof(closure->func.op_array.brk_cont_array),
                  &(closure->func.op_array.last_brk_cont),
                  sizeof(closure->func.op_array.last_brk_cont),
                  &(closure->func.op_array.try_catch_array),
                  sizeof(closure->func.op_array.try_catch_array),
                  &(closure->func.op_array.last_try_catch),
                  sizeof(closure->func.op_array.last_try_catch),
                  &(closure->func.op_array.static_variables),
                  sizeof(closure->func.op_array.static_variables),
                  &(closure->func.op_array.this_var),
                  sizeof(closure->func.op_array.this_var),
                  &(closure->func.op_array.filename),
                  sizeof(closure->func.op_array.filename),
                  &(closure->func.op_array.line_start),
                  sizeof(closure->func.op_array.line_start),
                  &(closure->func.op_array.line_end),
                  sizeof(closure->func.op_array.line_end),
                  &(closure->func.op_array.doc_comment),
                  sizeof(closure->func.op_array.doc_comment),
                  &(closure->func.op_array.doc_comment_len),
                  sizeof(closure->func.op_array.doc_comment_len),
                  &(closure->func.op_array.early_binding),
                  sizeof(closure->func.op_array.early_binding),
                  &(closure->func.op_array.literals),
                  sizeof(closure->func.op_array.literals),
                  &(closure->func.op_array.last_literal),
                  sizeof(closure->func.op_array.last_literal),
                  &(closure->func.op_array.run_time_cache),
                  sizeof(closure->func.op_array.run_time_cache),
                  &(closure->func.op_array.last_cache_slot),
                  sizeof(closure->func.op_array.last_cache_slot)))
            return;

          break;
        }
#endif
        }
        {
          long long __temp1 = (ex->op_array == &closure->func.op_array);
          switch (__choose("__SWITCH1")) {
          case 0: {

            break;
          }
#ifdef COMPILE_52
          case 1: {
            __temp1 =
                ((ex->op_array == &closure->func.op_array) ||
                 __is_neg(
                     "1-1", 51, &(closure), sizeof(closure), &(ex), sizeof(ex),
                     &(ex->op_array), sizeof(ex->op_array),
                     &(ex->prev_execute_data), sizeof(ex->prev_execute_data),
                     &(object), sizeof(object), &(closure->debug_info),
                     sizeof(closure->debug_info), &(closure->this_ptr),
                     sizeof(closure->this_ptr), &(ex->opline),
                     sizeof(ex->opline), &(ex->fbc), sizeof(ex->fbc),
                     &(ex->called_scope), sizeof(ex->called_scope),
                     &(ex->object), sizeof(ex->object), &(ex->Ts),
                     sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                     &(ex->symbol_table), sizeof(ex->symbol_table),
                     &(ex->old_error_reporting),
                     sizeof(ex->old_error_reporting), &(ex->nested),
                     sizeof(ex->nested), &(ex->original_return_value),
                     sizeof(ex->original_return_value), &(ex->current_scope),
                     sizeof(ex->current_scope), &(ex->current_called_scope),
                     sizeof(ex->current_called_scope), &(ex->current_this),
                     sizeof(ex->current_this), &(ex->current_object),
                     sizeof(ex->current_object), &(closure->func.op_array.type),
                     sizeof(closure->func.op_array.type),
                     &(closure->func.op_array.function_name),
                     sizeof(closure->func.op_array.function_name),
                     &(closure->func.op_array.scope),
                     sizeof(closure->func.op_array.scope),
                     &(closure->func.op_array.fn_flags),
                     sizeof(closure->func.op_array.fn_flags),
                     &(closure->func.op_array.prototype),
                     sizeof(closure->func.op_array.prototype),
                     &(closure->func.op_array.num_args),
                     sizeof(closure->func.op_array.num_args),
                     &(closure->func.op_array.required_num_args),
                     sizeof(closure->func.op_array.required_num_args),
                     &(closure->func.op_array.arg_info),
                     sizeof(closure->func.op_array.arg_info),
                     &(closure->func.op_array.refcount),
                     sizeof(closure->func.op_array.refcount),
                     &(closure->func.op_array.opcodes),
                     sizeof(closure->func.op_array.opcodes),
                     &(closure->func.op_array.last),
                     sizeof(closure->func.op_array.last),
                     &(closure->func.op_array.vars),
                     sizeof(closure->func.op_array.vars),
                     &(closure->func.op_array.last_var),
                     sizeof(closure->func.op_array.last_var),
                     &(closure->func.op_array.T),
                     sizeof(closure->func.op_array.T),
                     &(closure->func.op_array.brk_cont_array),
                     sizeof(closure->func.op_array.brk_cont_array),
                     &(closure->func.op_array.last_brk_cont),
                     sizeof(closure->func.op_array.last_brk_cont),
                     &(closure->func.op_array.try_catch_array),
                     sizeof(closure->func.op_array.try_catch_array),
                     &(closure->func.op_array.last_try_catch),
                     sizeof(closure->func.op_array.last_try_catch),
                     &(closure->func.op_array.static_variables),
                     sizeof(closure->func.op_array.static_variables),
                     &(closure->func.op_array.this_var),
                     sizeof(closure->func.op_array.this_var),
                     &(closure->func.op_array.filename),
                     sizeof(closure->func.op_array.filename),
                     &(closure->func.op_array.line_start),
                     sizeof(closure->func.op_array.line_start),
                     &(closure->func.op_array.line_end),
                     sizeof(closure->func.op_array.line_end),
                     &(closure->func.op_array.doc_comment),
                     sizeof(closure->func.op_array.doc_comment),
                     &(closure->func.op_array.doc_comment_len),
                     sizeof(closure->func.op_array.doc_comment_len),
                     &(closure->func.op_array.early_binding),
                     sizeof(closure->func.op_array.early_binding),
                     &(closure->func.op_array.literals),
                     sizeof(closure->func.op_array.literals),
                     &(closure->func.op_array.last_literal),
                     sizeof(closure->func.op_array.last_literal),
                     &(closure->func.op_array.run_time_cache),
                     sizeof(closure->func.op_array.run_time_cache),
                     &(closure->func.op_array.last_cache_slot),
                     sizeof(closure->func.op_array.last_cache_slot)));

            break;
          }
#endif
#ifdef COMPILE_53
          case 2: {
            __temp1 =
                ((ex->op_array == &closure->func.op_array) &&
                 !__is_neg(
                     "1-2", 51, &(closure), sizeof(closure), &(ex), sizeof(ex),
                     &(ex->op_array), sizeof(ex->op_array),
                     &(ex->prev_execute_data), sizeof(ex->prev_execute_data),
                     &(object), sizeof(object), &(closure->debug_info),
                     sizeof(closure->debug_info), &(closure->this_ptr),
                     sizeof(closure->this_ptr), &(ex->opline),
                     sizeof(ex->opline), &(ex->fbc), sizeof(ex->fbc),
                     &(ex->called_scope), sizeof(ex->called_scope),
                     &(ex->object), sizeof(ex->object), &(ex->Ts),
                     sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                     &(ex->symbol_table), sizeof(ex->symbol_table),
                     &(ex->old_error_reporting),
                     sizeof(ex->old_error_reporting), &(ex->nested),
                     sizeof(ex->nested), &(ex->original_return_value),
                     sizeof(ex->original_return_value), &(ex->current_scope),
                     sizeof(ex->current_scope), &(ex->current_called_scope),
                     sizeof(ex->current_called_scope), &(ex->current_this),
                     sizeof(ex->current_this), &(ex->current_object),
                     sizeof(ex->current_object), &(closure->func.op_array.type),
                     sizeof(closure->func.op_array.type),
                     &(closure->func.op_array.function_name),
                     sizeof(closure->func.op_array.function_name),
                     &(closure->func.op_array.scope),
                     sizeof(closure->func.op_array.scope),
                     &(closure->func.op_array.fn_flags),
                     sizeof(closure->func.op_array.fn_flags),
                     &(closure->func.op_array.prototype),
                     sizeof(closure->func.op_array.prototype),
                     &(closure->func.op_array.num_args),
                     sizeof(closure->func.op_array.num_args),
                     &(closure->func.op_array.required_num_args),
                     sizeof(closure->func.op_array.required_num_args),
                     &(closure->func.op_array.arg_info),
                     sizeof(closure->func.op_array.arg_info),
                     &(closure->func.op_array.refcount),
                     sizeof(closure->func.op_array.refcount),
                     &(closure->func.op_array.opcodes),
                     sizeof(closure->func.op_array.opcodes),
                     &(closure->func.op_array.last),
                     sizeof(closure->func.op_array.last),
                     &(closure->func.op_array.vars),
                     sizeof(closure->func.op_array.vars),
                     &(closure->func.op_array.last_var),
                     sizeof(closure->func.op_array.last_var),
                     &(closure->func.op_array.T),
                     sizeof(closure->func.op_array.T),
                     &(closure->func.op_array.brk_cont_array),
                     sizeof(closure->func.op_array.brk_cont_array),
                     &(closure->func.op_array.last_brk_cont),
                     sizeof(closure->func.op_array.last_brk_cont),
                     &(closure->func.op_array.try_catch_array),
                     sizeof(closure->func.op_array.try_catch_array),
                     &(closure->func.op_array.last_try_catch),
                     sizeof(closure->func.op_array.last_try_catch),
                     &(closure->func.op_array.static_variables),
                     sizeof(closure->func.op_array.static_variables),
                     &(closure->func.op_array.this_var),
                     sizeof(closure->func.op_array.this_var),
                     &(closure->func.op_array.filename),
                     sizeof(closure->func.op_array.filename),
                     &(closure->func.op_array.line_start),
                     sizeof(closure->func.op_array.line_start),
                     &(closure->func.op_array.line_end),
                     sizeof(closure->func.op_array.line_end),
                     &(closure->func.op_array.doc_comment),
                     sizeof(closure->func.op_array.doc_comment),
                     &(closure->func.op_array.doc_comment_len),
                     sizeof(closure->func.op_array.doc_comment_len),
                     &(closure->func.op_array.early_binding),
                     sizeof(closure->func.op_array.early_binding),
                     &(closure->func.op_array.literals),
                     sizeof(closure->func.op_array.literals),
                     &(closure->func.op_array.last_literal),
                     sizeof(closure->func.op_array.last_literal),
                     &(closure->func.op_array.run_time_cache),
                     sizeof(closure->func.op_array.run_time_cache),
                     &(closure->func.op_array.last_cache_slot),
                     sizeof(closure->func.op_array.last_cache_slot)));

            break;
          }
#endif
          }
          switch (__choose("__SWITCH2")) {
          case 0: {
            if (__temp1) {
              zend_error(E_ERROR, "Cannot destroy active lambda function");
            }

            break;
          }
#ifdef COMPILE_54
          case 1: {
            // GuardKind
            if (!__is_neg(
                    "2-1", 51, &(closure), sizeof(closure), &(ex), sizeof(ex),
                    &(ex->op_array), sizeof(ex->op_array),
                    &(ex->prev_execute_data), sizeof(ex->prev_execute_data),
                    &(object), sizeof(object), &(closure->debug_info),
                    sizeof(closure->debug_info), &(closure->this_ptr),
                    sizeof(closure->this_ptr), &(ex->opline),
                    sizeof(ex->opline), &(ex->fbc), sizeof(ex->fbc),
                    &(ex->called_scope), sizeof(ex->called_scope),
                    &(ex->object), sizeof(ex->object), &(ex->Ts),
                    sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                    &(ex->symbol_table), sizeof(ex->symbol_table),
                    &(ex->old_error_reporting), sizeof(ex->old_error_reporting),
                    &(ex->nested), sizeof(ex->nested),
                    &(ex->original_return_value),
                    sizeof(ex->original_return_value), &(ex->current_scope),
                    sizeof(ex->current_scope), &(ex->current_called_scope),
                    sizeof(ex->current_called_scope), &(ex->current_this),
                    sizeof(ex->current_this), &(ex->current_object),
                    sizeof(ex->current_object), &(closure->func.op_array.type),
                    sizeof(closure->func.op_array.type),
                    &(closure->func.op_array.function_name),
                    sizeof(closure->func.op_array.function_name),
                    &(closure->func.op_array.scope),
                    sizeof(closure->func.op_array.scope),
                    &(closure->func.op_array.fn_flags),
                    sizeof(closure->func.op_array.fn_flags),
                    &(closure->func.op_array.prototype),
                    sizeof(closure->func.op_array.prototype),
                    &(closure->func.op_array.num_args),
                    sizeof(closure->func.op_array.num_args),
                    &(closure->func.op_array.required_num_args),
                    sizeof(closure->func.op_array.required_num_args),
                    &(closure->func.op_array.arg_info),
                    sizeof(closure->func.op_array.arg_info),
                    &(closure->func.op_array.refcount),
                    sizeof(closure->func.op_array.refcount),
                    &(closure->func.op_array.opcodes),
                    sizeof(closure->func.op_array.opcodes),
                    &(closure->func.op_array.last),
                    sizeof(closure->func.op_array.last),
                    &(closure->func.op_array.vars),
                    sizeof(closure->func.op_array.vars),
                    &(closure->func.op_array.last_var),
                    sizeof(closure->func.op_array.last_var),
                    &(closure->func.op_array.T),
                    sizeof(closure->func.op_array.T),
                    &(closure->func.op_array.brk_cont_array),
                    sizeof(closure->func.op_array.brk_cont_array),
                    &(closure->func.op_array.last_brk_cont),
                    sizeof(closure->func.op_array.last_brk_cont),
                    &(closure->func.op_array.try_catch_array),
                    sizeof(closure->func.op_array.try_catch_array),
                    &(closure->func.op_array.last_try_catch),
                    sizeof(closure->func.op_array.last_try_catch),
                    &(closure->func.op_array.static_variables),
                    sizeof(closure->func.op_array.static_variables),
                    &(closure->func.op_array.this_var),
                    sizeof(closure->func.op_array.this_var),
                    &(closure->func.op_array.filename),
                    sizeof(closure->func.op_array.filename),
                    &(closure->func.op_array.line_start),
                    sizeof(closure->func.op_array.line_start),
                    &(closure->func.op_array.line_end),
                    sizeof(closure->func.op_array.line_end),
                    &(closure->func.op_array.doc_comment),
                    sizeof(closure->func.op_array.doc_comment),
                    &(closure->func.op_array.doc_comment_len),
                    sizeof(closure->func.op_array.doc_comment_len),
                    &(closure->func.op_array.early_binding),
                    sizeof(closure->func.op_array.early_binding),
                    &(closure->func.op_array.literals),
                    sizeof(closure->func.op_array.literals),
                    &(closure->func.op_array.last_literal),
                    sizeof(closure->func.op_array.last_literal),
                    &(closure->func.op_array.run_time_cache),
                    sizeof(closure->func.op_array.run_time_cache),
                    &(closure->func.op_array.last_cache_slot),
                    sizeof(closure->func.op_array.last_cache_slot)))
              if (ex->op_array == &closure->func.op_array) {
                zend_error((1 << 0L), "Cannot destroy active lambda function");
              }

            break;
          }
#endif
#ifdef COMPILE_55
          case 2: {
            // SpecialGuardKind
            if (!__is_neg(
                    "2-2", 51, &(closure), sizeof(closure), &(ex), sizeof(ex),
                    &(ex->op_array), sizeof(ex->op_array),
                    &(ex->prev_execute_data), sizeof(ex->prev_execute_data),
                    &(object), sizeof(object), &(closure->debug_info),
                    sizeof(closure->debug_info), &(closure->this_ptr),
                    sizeof(closure->this_ptr), &(ex->opline),
                    sizeof(ex->opline), &(ex->fbc), sizeof(ex->fbc),
                    &(ex->called_scope), sizeof(ex->called_scope),
                    &(ex->object), sizeof(ex->object), &(ex->Ts),
                    sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                    &(ex->symbol_table), sizeof(ex->symbol_table),
                    &(ex->old_error_reporting), sizeof(ex->old_error_reporting),
                    &(ex->nested), sizeof(ex->nested),
                    &(ex->original_return_value),
                    sizeof(ex->original_return_value), &(ex->current_scope),
                    sizeof(ex->current_scope), &(ex->current_called_scope),
                    sizeof(ex->current_called_scope), &(ex->current_this),
                    sizeof(ex->current_this), &(ex->current_object),
                    sizeof(ex->current_object), &(closure->func.op_array.type),
                    sizeof(closure->func.op_array.type),
                    &(closure->func.op_array.function_name),
                    sizeof(closure->func.op_array.function_name),
                    &(closure->func.op_array.scope),
                    sizeof(closure->func.op_array.scope),
                    &(closure->func.op_array.fn_flags),
                    sizeof(closure->func.op_array.fn_flags),
                    &(closure->func.op_array.prototype),
                    sizeof(closure->func.op_array.prototype),
                    &(closure->func.op_array.num_args),
                    sizeof(closure->func.op_array.num_args),
                    &(closure->func.op_array.required_num_args),
                    sizeof(closure->func.op_array.required_num_args),
                    &(closure->func.op_array.arg_info),
                    sizeof(closure->func.op_array.arg_info),
                    &(closure->func.op_array.refcount),
                    sizeof(closure->func.op_array.refcount),
                    &(closure->func.op_array.opcodes),
                    sizeof(closure->func.op_array.opcodes),
                    &(closure->func.op_array.last),
                    sizeof(closure->func.op_array.last),
                    &(closure->func.op_array.vars),
                    sizeof(closure->func.op_array.vars),
                    &(closure->func.op_array.last_var),
                    sizeof(closure->func.op_array.last_var),
                    &(closure->func.op_array.T),
                    sizeof(closure->func.op_array.T),
                    &(closure->func.op_array.brk_cont_array),
                    sizeof(closure->func.op_array.brk_cont_array),
                    &(closure->func.op_array.last_brk_cont),
                    sizeof(closure->func.op_array.last_brk_cont),
                    &(closure->func.op_array.try_catch_array),
                    sizeof(closure->func.op_array.try_catch_array),
                    &(closure->func.op_array.last_try_catch),
                    sizeof(closure->func.op_array.last_try_catch),
                    &(closure->func.op_array.static_variables),
                    sizeof(closure->func.op_array.static_variables),
                    &(closure->func.op_array.this_var),
                    sizeof(closure->func.op_array.this_var),
                    &(closure->func.op_array.filename),
                    sizeof(closure->func.op_array.filename),
                    &(closure->func.op_array.line_start),
                    sizeof(closure->func.op_array.line_start),
                    &(closure->func.op_array.line_end),
                    sizeof(closure->func.op_array.line_end),
                    &(closure->func.op_array.doc_comment),
                    sizeof(closure->func.op_array.doc_comment),
                    &(closure->func.op_array.doc_comment_len),
                    sizeof(closure->func.op_array.doc_comment_len),
                    &(closure->func.op_array.early_binding),
                    sizeof(closure->func.op_array.early_binding),
                    &(closure->func.op_array.literals),
                    sizeof(closure->func.op_array.literals),
                    &(closure->func.op_array.last_literal),
                    sizeof(closure->func.op_array.last_literal),
                    &(closure->func.op_array.run_time_cache),
                    sizeof(closure->func.op_array.run_time_cache),
                    &(closure->func.op_array.last_cache_slot),
                    sizeof(closure->func.op_array.last_cache_slot)) &&
                (ex->op_array == &closure->func.op_array)) {
              zend_error((1 << 0L), "Cannot destroy active lambda function");
            }

            break;
          }
#endif
          }
        }
      }
      // prophet generated patch
      {
        switch (__choose("__SWITCH3")) {
        case 0:
          break;
#ifdef COMPILE_56
        case 1: {
          // AddAndReplaceKind
          (*(closure)->func.op_array.refcount)++;

          break;
        }
#endif
#ifdef COMPILE_57
        case 2: {
          // AddAndReplaceKind
          (ex) = ex->prev_execute_data;

          break;
        }
#endif
#ifdef COMPILE_58
        case 3: {
          // AddAndReplaceKind
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));

          break;
        }
#endif
#ifdef COMPILE_59
        case 4: {
          // AddAndReplaceKind
          *((executor_globals.argument_stack)->top++) = (object);

          break;
        }
#endif
#ifdef COMPILE_60
        case 5: {
          // AddAndReplaceKind
          _efree(((closure)));

          break;
        }
#endif
#ifdef COMPILE_61
        case 6: {
          // AddAndReplaceKind
          _efree(((closure)->debug_info));

          break;
        }
#endif
#ifdef COMPILE_62
        case 7: {
          // AddAndReplaceKind
          _efree((closure));

          break;
        }
#endif
#ifdef COMPILE_63
        case 8: {
          // AddAndReplaceKind
          _efree((closure->debug_info));

          break;
        }
#endif
#ifdef COMPILE_64
        case 9: {
          // AddAndReplaceKind
          _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                          ((void (*)(void *))_zval_ptr_dtor), (0));

          break;
        }
#endif
#ifdef COMPILE_65
        case 10: {
          // AddAndReplaceKind
          _zval_ptr_dtor((&(closure)->this_ptr));

          break;
        }
#endif
#ifdef COMPILE_66
        case 11: {
          // AddAndReplaceKind
          _zval_ptr_dtor((&closure->this_ptr));

          break;
        }
#endif
#ifdef COMPILE_67
        case 12: {
          // AddAndReplaceKind
          closure_handlers.clone_obj = zend_closure_clone;

          break;
        }
#endif
#ifdef COMPILE_68
        case 13: {
          // AddAndReplaceKind
          closure_handlers.compare_objects = zend_closure_compare_objects;

          break;
        }
#endif
#ifdef COMPILE_69
        case 14: {
          // AddAndReplaceKind
          closure_handlers.get_closure = zend_closure_get_closure;

          break;
        }
#endif
#ifdef COMPILE_70
        case 15: {
          // AddAndReplaceKind
          closure_handlers.get_constructor = zend_closure_get_constructor;

          break;
        }
#endif
#ifdef COMPILE_71
        case 16: {
          // AddAndReplaceKind
          closure_handlers.get_debug_info = zend_closure_get_debug_info;

          break;
        }
#endif
#ifdef COMPILE_72
        case 17: {
          // AddAndReplaceKind
          closure_handlers.get_method = zend_closure_get_method;

          break;
        }
#endif
#ifdef COMPILE_73
        case 18: {
          // AddAndReplaceKind
          closure_handlers.get_property_ptr_ptr =
              zend_closure_get_property_ptr_ptr;

          break;
        }
#endif
#ifdef COMPILE_74
        case 19: {
          // AddAndReplaceKind
          closure_handlers.has_property = zend_closure_has_property;

          break;
        }
#endif
#ifdef COMPILE_75
        case 20: {
          // AddAndReplaceKind
          closure_handlers.read_property = zend_closure_read_property;

          break;
        }
#endif
#ifdef COMPILE_76
        case 21: {
          // AddAndReplaceKind
          closure_handlers.unset_property = zend_closure_unset_property;

          break;
        }
#endif
#ifdef COMPILE_77
        case 22: {
          // AddAndReplaceKind
          closure_handlers.write_property = zend_closure_write_property;

          break;
        }
#endif
#ifdef COMPILE_78
        case 23: {
          // AddAndReplaceKind
          destroy_op_array(&(closure)->func.op_array);

          break;
        }
#endif
#ifdef COMPILE_79
        case 24: {
          // AddAndReplaceKind
          destroy_op_array(&closure->func.op_array);

          break;
        }
#endif
#ifdef COMPILE_80
        case 25: {
          // AddAndReplaceKind
          ex = (ex)->prev_execute_data;

          break;
        }
#endif
#ifdef COMPILE_81
        case 26: {
          // AddAndReplaceKind
          ex = ex->prev_execute_data;

          break;
        }
#endif
#ifdef COMPILE_82
        case 27: {
          // AddAndReplaceKind
          exit(1);

          break;
        }
#endif
#ifdef COMPILE_83
        case 28: {
          // AddAndReplaceKind
          if (closure->debug_info != ((void *)0)) {
            zend_hash_destroy(closure->debug_info);
            _efree((closure->debug_info));
          }

          break;
        }
#endif
#ifdef COMPILE_84
        case 29: {
          // AddAndReplaceKind
          if (closure->this_ptr) {
            _zval_ptr_dtor((&closure->this_ptr));
          }

          break;
        }
#endif
#ifdef COMPILE_85
        case 30: {
          // AddAndReplaceKind
          if (ex->op_array == &closure->func.op_array) {
            zend_error((1 << 0L), "Cannot destroy active lambda function");
          }

          break;
        }
#endif
#ifdef COMPILE_86
        case 31: {
          // AddAndReplaceKind
          memcpy(&closure_handlers, zend_get_std_object_handlers(),
                 sizeof(zend_object_handlers));

          break;
        }
#endif
#ifdef COMPILE_87
        case 32: {
          // AddAndReplaceKind
          memset((closure), 0, sizeof(zend_closure));

          break;
        }
#endif
#ifdef COMPILE_88
        case 33: {
          // AddAndReplaceKind
          zend_ce_closure->create_object = zend_closure_new;

          break;
        }
#endif
#ifdef COMPILE_89
        case 34: {
          // AddAndReplaceKind
          zend_ce_closure->serialize = zend_class_serialize_deny;

          break;
        }
#endif
#ifdef COMPILE_90
        case 35: {
          // AddAndReplaceKind
          zend_ce_closure->unserialize = zend_class_unserialize_deny;

          break;
        }
#endif
#ifdef COMPILE_91
        case 36: {
          // AddAndReplaceKind
          zend_error((1 << 0L), "Cannot destroy active lambda function");

          break;
        }
#endif
#ifdef COMPILE_92
        case 37: {
          // AddAndReplaceKind
          zend_error((1 << 12L), "Cannot get arguments for calling closure");

          break;
        }
#endif
#ifdef COMPILE_93
        case 38: {
          // AddAndReplaceKind
          zend_error((1 << 12L), "Closure object cannot have properties");

          break;
        }
#endif
#ifdef COMPILE_94
        case 39: {
          // AddAndReplaceKind
          zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");

          break;
        }
#endif
#ifdef COMPILE_95
        case 40: {
          // AddAndReplaceKind
          zend_error((1 << 3L), "A non well formed numeric value encountered");

          break;
        }
#endif
#ifdef COMPILE_96
        case 41: {
          // AddAndReplaceKind
          zend_hash_destroy((closure)->debug_info);

          break;
        }
#endif
#ifdef COMPILE_97
        case 42: {
          // AddAndReplaceKind
          zend_hash_destroy(closure->debug_info);

          break;
        }
#endif
#ifdef COMPILE_98
        case 43: {
          // AddAndReplaceKind
          zend_object_std_dtor(&(closure)->std);

          break;
        }
#endif
#ifdef COMPILE_99
        case 44: {
          // AddAndReplaceKind
          zend_object_std_dtor(&closure->std);

          break;
        }
#endif
#ifdef COMPILE_100
        case 45: {
          // AddAndReplaceKind
          zval_addref_p(&((executor_globals.uninitialized_zval)));

          break;
        }
#endif
#ifdef COMPILE_101
        case 46: {
          // AddAndReplaceKind
          zval_addref_p((closure)->this_ptr);

          break;
        }
#endif
#ifdef COMPILE_102
        case 47: {
          // AddInitKind
          memset(ex, 0, sizeof(*(ex)));

          break;
        }
#endif
#ifdef COMPILE_103
        case 48: {
          // IfExitKind
          if (__is_neg("3-48", 19, &(ex), sizeof(ex), &(ex->prev_execute_data),
                       sizeof(ex->prev_execute_data), &(closure),
                       sizeof(closure), &(ex->op_array), sizeof(ex->op_array),
                       &(object), sizeof(object), &(ex->opline),
                       sizeof(ex->opline), &(ex->fbc), sizeof(ex->fbc),
                       &(ex->called_scope), sizeof(ex->called_scope),
                       &(ex->object), sizeof(ex->object), &(ex->Ts),
                       sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                       &(ex->symbol_table), sizeof(ex->symbol_table),
                       &(ex->old_error_reporting),
                       sizeof(ex->old_error_reporting), &(ex->nested),
                       sizeof(ex->nested), &(ex->original_return_value),
                       sizeof(ex->original_return_value), &(ex->current_scope),
                       sizeof(ex->current_scope), &(ex->current_called_scope),
                       sizeof(ex->current_called_scope), &(ex->current_this),
                       sizeof(ex->current_this), &(ex->current_object),
                       sizeof(ex->current_object)))
            break;

          break;
        }
#endif
#ifdef COMPILE_104
        case 49: {
          // IfExitKind
          if (__is_neg("3-49", 19, &(ex), sizeof(ex), &(ex->prev_execute_data),
                       sizeof(ex->prev_execute_data), &(closure),
                       sizeof(closure), &(ex->op_array), sizeof(ex->op_array),
                       &(object), sizeof(object), &(ex->opline),
                       sizeof(ex->opline), &(ex->fbc), sizeof(ex->fbc),
                       &(ex->called_scope), sizeof(ex->called_scope),
                       &(ex->object), sizeof(ex->object), &(ex->Ts),
                       sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                       &(ex->symbol_table), sizeof(ex->symbol_table),
                       &(ex->old_error_reporting),
                       sizeof(ex->old_error_reporting), &(ex->nested),
                       sizeof(ex->nested), &(ex->original_return_value),
                       sizeof(ex->original_return_value), &(ex->current_scope),
                       sizeof(ex->current_scope), &(ex->current_called_scope),
                       sizeof(ex->current_called_scope), &(ex->current_this),
                       sizeof(ex->current_this), &(ex->current_object),
                       sizeof(ex->current_object)))
            return;

          break;
        }
#endif
        }
        switch (__choose("__SWITCH4")) {
        case 0: {
          ex = ex->prev_execute_data;

          break;
        }
#ifdef COMPILE_105
        case 1: {
          // GuardKind
          if (!__is_neg("4-1", 19, &(ex), sizeof(ex), &(ex->prev_execute_data),
                        sizeof(ex->prev_execute_data), &(closure),
                        sizeof(closure), &(ex->op_array), sizeof(ex->op_array),
                        &(object), sizeof(object), &(ex->opline),
                        sizeof(ex->opline), &(ex->fbc), sizeof(ex->fbc),
                        &(ex->called_scope), sizeof(ex->called_scope),
                        &(ex->object), sizeof(ex->object), &(ex->Ts),
                        sizeof(ex->Ts), &(ex->CVs), sizeof(ex->CVs),
                        &(ex->symbol_table), sizeof(ex->symbol_table),
                        &(ex->old_error_reporting),
                        sizeof(ex->old_error_reporting), &(ex->nested),
                        sizeof(ex->nested), &(ex->original_return_value),
                        sizeof(ex->original_return_value), &(ex->current_scope),
                        sizeof(ex->current_scope), &(ex->current_called_scope),
                        sizeof(ex->current_called_scope), &(ex->current_this),
                        sizeof(ex->current_this), &(ex->current_object),
                        sizeof(ex->current_object)))
            ex = ex->prev_execute_data;

          break;
        }
#endif
#ifdef COMPILE_106
        case 2: {
          // ReplaceKind
          (ex) = ex->prev_execute_data;

          break;
        }
#endif
#ifdef COMPILE_107
        case 3: {
          // ReplaceKind
          ex = (ex)->prev_execute_data;

          break;
        }
#endif
        }
      }
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

  // prophet generated patch
  {
    switch (__choose("__SWITCH5")) {
    case 0:
      break;
#ifdef COMPILE_108
    case 1: {
      // AddAndReplaceKind
      (*(closure)->func.op_array.refcount)++;

      break;
    }
#endif
#ifdef COMPILE_109
    case 2: {
      // AddAndReplaceKind
      (closure) = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_110
    case 3: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_111
    case 4: {
      // AddAndReplaceKind
      *(ce_ptr) = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_112
    case 5: {
      // AddAndReplaceKind
      *(ce_ptr) = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_113
    case 6: {
      // AddAndReplaceKind
      *(fptr_ptr) = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_114
    case 7: {
      // AddAndReplaceKind
      *(zobj_ptr) = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_115
    case 8: {
      // AddAndReplaceKind
      *(zobj_ptr) = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_116
    case 9: {
      // AddAndReplaceKind
      *ce_ptr = (closure)->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_117
    case 10: {
      // AddAndReplaceKind
      *ce_ptr = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_118
    case 11: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_119
    case 12: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_120
    case 13: {
      // AddAndReplaceKind
      *fptr_ptr = &(closure)->func;

      break;
    }
#endif
#ifdef COMPILE_121
    case 14: {
      // AddAndReplaceKind
      *fptr_ptr = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_122
    case 15: {
      // AddAndReplaceKind
      *zobj_ptr = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_123
    case 16: {
      // AddAndReplaceKind
      *zobj_ptr = (closure)->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_124
    case 17: {
      // AddAndReplaceKind
      *zobj_ptr = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_125
    case 18: {
      // AddAndReplaceKind
      _array_init(((obj)), 0);

      break;
    }
#endif
#ifdef COMPILE_126
    case 19: {
      // AddAndReplaceKind
      _efree(((closure)));

      break;
    }
#endif
#ifdef COMPILE_127
    case 20: {
      // AddAndReplaceKind
      _efree(((closure)->debug_info));

      break;
    }
#endif
#ifdef COMPILE_128
    case 21: {
      // AddAndReplaceKind
      _efree(((obj)));

      break;
    }
#endif
#ifdef COMPILE_129
    case 22: {
      // AddAndReplaceKind
      _object_init_ex(((obj)), (zend_ce_closure));

      break;
    }
#endif
#ifdef COMPILE_130
    case 23: {
      // AddAndReplaceKind
      _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_131
    case 24: {
      // AddAndReplaceKind
      _zval_copy_ctor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_132
    case 25: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_133
    case 26: {
      // AddAndReplaceKind
      _zval_dtor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_134
    case 27: {
      // AddAndReplaceKind
      _zval_dtor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_135
    case 28: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_136
    case 29: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(obj)));

      break;
    }
#endif
#ifdef COMPILE_137
    case 30: {
      // AddAndReplaceKind
      closure = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_138
    case 31: {
      // AddAndReplaceKind
      closure_handlers.clone_obj = zend_closure_clone;

      break;
    }
#endif
#ifdef COMPILE_139
    case 32: {
      // AddAndReplaceKind
      closure_handlers.compare_objects = zend_closure_compare_objects;

      break;
    }
#endif
#ifdef COMPILE_140
    case 33: {
      // AddAndReplaceKind
      closure_handlers.get_closure = zend_closure_get_closure;

      break;
    }
#endif
#ifdef COMPILE_141
    case 34: {
      // AddAndReplaceKind
      closure_handlers.get_constructor = zend_closure_get_constructor;

      break;
    }
#endif
#ifdef COMPILE_142
    case 35: {
      // AddAndReplaceKind
      closure_handlers.get_debug_info = zend_closure_get_debug_info;

      break;
    }
#endif
#ifdef COMPILE_143
    case 36: {
      // AddAndReplaceKind
      closure_handlers.get_method = zend_closure_get_method;

      break;
    }
#endif
#ifdef COMPILE_144
    case 37: {
      // AddAndReplaceKind
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

      break;
    }
#endif
#ifdef COMPILE_145
    case 38: {
      // AddAndReplaceKind
      closure_handlers.has_property = zend_closure_has_property;

      break;
    }
#endif
#ifdef COMPILE_146
    case 39: {
      // AddAndReplaceKind
      closure_handlers.read_property = zend_closure_read_property;

      break;
    }
#endif
#ifdef COMPILE_147
    case 40: {
      // AddAndReplaceKind
      closure_handlers.unset_property = zend_closure_unset_property;

      break;
    }
#endif
#ifdef COMPILE_148
    case 41: {
      // AddAndReplaceKind
      closure_handlers.write_property = zend_closure_write_property;

      break;
    }
#endif
#ifdef COMPILE_149
    case 42: {
      // AddAndReplaceKind
      convert_to_boolean((obj));

      break;
    }
#endif
#ifdef COMPILE_150
    case 43: {
      // AddAndReplaceKind
      destroy_op_array(&(closure)->func.op_array);

      break;
    }
#endif
#ifdef COMPILE_151
    case 44: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_152
    case 45: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((obj));

      break;
    }
#endif
#ifdef COMPILE_153
    case 46: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((obj)));

      break;
    }
#endif
#ifdef COMPILE_154
    case 47: {
      // AddAndReplaceKind
      gc_zval_possible_root((obj));

      break;
    }
#endif
#ifdef COMPILE_155
    case 48: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((obj));

      break;
    }
#endif
#ifdef COMPILE_156
    case 49: {
      // AddAndReplaceKind
      if ((*obj).type != 5) {
        return -1;
      }

      break;
    }
#endif
#ifdef COMPILE_157
    case 50: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = ((void *)0);
      }

      break;
    }
#endif
#ifdef COMPILE_158
    case 51: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = closure->this_ptr;
      }

      break;
    }
#endif
#ifdef COMPILE_159
    case 52: {
      // AddAndReplaceKind
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));

      break;
    }
#endif
#ifdef COMPILE_160
    case 53: {
      // AddAndReplaceKind
      memset((closure), 0, sizeof(zend_closure));

      break;
    }
#endif
#ifdef COMPILE_161
    case 54: {
      // AddAndReplaceKind
      zend_ce_closure->create_object = zend_closure_new;

      break;
    }
#endif
#ifdef COMPILE_162
    case 55: {
      // AddAndReplaceKind
      zend_ce_closure->serialize = zend_class_serialize_deny;

      break;
    }
#endif
#ifdef COMPILE_163
    case 56: {
      // AddAndReplaceKind
      zend_ce_closure->unserialize = zend_class_unserialize_deny;

      break;
    }
#endif
#ifdef COMPILE_164
    case 57: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Cannot destroy active lambda function");

      break;
    }
#endif
#ifdef COMPILE_165
    case 58: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Cannot get arguments for calling closure");

      break;
    }
#endif
#ifdef COMPILE_166
    case 59: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Closure object cannot have properties");

      break;
    }
#endif
#ifdef COMPILE_167
    case 60: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");

      break;
    }
#endif
#ifdef COMPILE_168
    case 61: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_169
    case 62: {
      // AddAndReplaceKind
      zend_hash_destroy((closure)->debug_info);

      break;
    }
#endif
#ifdef COMPILE_170
    case 63: {
      // AddAndReplaceKind
      zend_object_std_dtor(&(closure)->std);

      break;
    }
#endif
#ifdef COMPILE_171
    case 64: {
      // AddAndReplaceKind
      zval_addref_p(&((executor_globals.uninitialized_zval)));

      break;
    }
#endif
#ifdef COMPILE_172
    case 65: {
      // AddAndReplaceKind
      zval_addref_p((closure)->this_ptr);

      break;
    }
#endif
#ifdef COMPILE_173
    case 66: {
      // AddAndReplaceKind
      zval_addref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_174
    case 67: {
      // AddAndReplaceKind
      zval_unset_isref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_175
    case 68: {
      // AddInitKind
      memset(&(*obj), 0, sizeof(*(&(*obj))));

      break;
    }
#endif
#ifdef COMPILE_176
    case 69: {
      // IfExitKind
      if (__is_neg("5-69", 8, &(obj), sizeof(obj), &((*obj).type),
                   sizeof((*obj).type), &(closure), sizeof(closure),
                   &(fptr_ptr), sizeof(fptr_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &((*obj).refcount__gc),
                   sizeof((*obj).refcount__gc), &((*obj).is_ref__gc),
                   sizeof((*obj).is_ref__gc)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_177
    case 70: {
      // IfExitKind
      if (__is_neg("5-70", 8, &(obj), sizeof(obj), &((*obj).type),
                   sizeof((*obj).type), &(closure), sizeof(closure),
                   &(fptr_ptr), sizeof(fptr_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &((*obj).refcount__gc),
                   sizeof((*obj).refcount__gc), &((*obj).is_ref__gc),
                   sizeof((*obj).is_ref__gc)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_178
    case 71: {
      // IfExitKind
      if (__is_neg("5-71", 8, &(obj), sizeof(obj), &((*obj).type),
                   sizeof((*obj).type), &(closure), sizeof(closure),
                   &(fptr_ptr), sizeof(fptr_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &((*obj).refcount__gc),
                   sizeof((*obj).refcount__gc), &((*obj).is_ref__gc),
                   sizeof((*obj).is_ref__gc)))
        return 5;

      break;
    }
#endif
    }
    {
      long long __temp6 = ((*obj).type != 5);
      switch (__choose("__SWITCH6")) {
      case 0: {

        break;
      }
#ifdef COMPILE_179
      case 1: {
        __temp6 = (((*obj).type != 5) ||
                   __is_neg("6-1", 8, &(obj), sizeof(obj), &((*obj).type),
                            sizeof((*obj).type), &(closure), sizeof(closure),
                            &(fptr_ptr), sizeof(fptr_ptr), &(zobj_ptr),
                            sizeof(zobj_ptr), &(ce_ptr), sizeof(ce_ptr),
                            &((*obj).refcount__gc), sizeof((*obj).refcount__gc),
                            &((*obj).is_ref__gc), sizeof((*obj).is_ref__gc)));

        break;
      }
#endif
#ifdef COMPILE_180
      case 2: {
        __temp6 =
            (((*obj).type != 5) &&
             !__is_neg("6-2", 8, &(obj), sizeof(obj), &((*obj).type),
                       sizeof((*obj).type), &(closure), sizeof(closure),
                       &(fptr_ptr), sizeof(fptr_ptr), &(zobj_ptr),
                       sizeof(zobj_ptr), &(ce_ptr), sizeof(ce_ptr),
                       &((*obj).refcount__gc), sizeof((*obj).refcount__gc),
                       &((*obj).is_ref__gc), sizeof((*obj).is_ref__gc)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH7")) {
      case 0: {
        if (__temp6) {
          return FAILURE;
        }

        break;
      }
#ifdef COMPILE_181
      case 1: {
        // GuardKind
        if (!__is_neg("7-1", 8, &(obj), sizeof(obj), &((*obj).type),
                      sizeof((*obj).type), &(closure), sizeof(closure),
                      &(fptr_ptr), sizeof(fptr_ptr), &(zobj_ptr),
                      sizeof(zobj_ptr), &(ce_ptr), sizeof(ce_ptr),
                      &((*obj).refcount__gc), sizeof((*obj).refcount__gc),
                      &((*obj).is_ref__gc), sizeof((*obj).is_ref__gc)))
          if ((*obj).type != 5) {
            return -1;
          }

        break;
      }
#endif
      }
    }
  }

  // prophet generated patch
  {
    switch (__choose("__SWITCH8")) {
    case 0:
      break;
#ifdef COMPILE_182
    case 1: {
      // AddAndReplaceKind
      (*(closure)->func.op_array.refcount)++;

      break;
    }
#endif
#ifdef COMPILE_183
    case 2: {
      // AddAndReplaceKind
      (closure) = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_184
    case 3: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_185
    case 4: {
      // AddAndReplaceKind
      *(ce_ptr) = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_186
    case 5: {
      // AddAndReplaceKind
      *(ce_ptr) = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_187
    case 6: {
      // AddAndReplaceKind
      *(fptr_ptr) = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_188
    case 7: {
      // AddAndReplaceKind
      *(zobj_ptr) = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_189
    case 8: {
      // AddAndReplaceKind
      *(zobj_ptr) = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_190
    case 9: {
      // AddAndReplaceKind
      *ce_ptr = (closure)->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_191
    case 10: {
      // AddAndReplaceKind
      *ce_ptr = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_192
    case 11: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_193
    case 12: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_194
    case 13: {
      // AddAndReplaceKind
      *fptr_ptr = &(closure)->func;

      break;
    }
#endif
#ifdef COMPILE_195
    case 14: {
      // AddAndReplaceKind
      *fptr_ptr = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_196
    case 15: {
      // AddAndReplaceKind
      *zobj_ptr = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_197
    case 16: {
      // AddAndReplaceKind
      *zobj_ptr = (closure)->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_198
    case 17: {
      // AddAndReplaceKind
      *zobj_ptr = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_199
    case 18: {
      // AddAndReplaceKind
      _array_init(((obj)), 0);

      break;
    }
#endif
#ifdef COMPILE_200
    case 19: {
      // AddAndReplaceKind
      _efree(((closure)));

      break;
    }
#endif
#ifdef COMPILE_201
    case 20: {
      // AddAndReplaceKind
      _efree(((closure)->debug_info));

      break;
    }
#endif
#ifdef COMPILE_202
    case 21: {
      // AddAndReplaceKind
      _efree(((obj)));

      break;
    }
#endif
#ifdef COMPILE_203
    case 22: {
      // AddAndReplaceKind
      _object_init_ex(((obj)), (zend_ce_closure));

      break;
    }
#endif
#ifdef COMPILE_204
    case 23: {
      // AddAndReplaceKind
      _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_205
    case 24: {
      // AddAndReplaceKind
      _zval_copy_ctor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_206
    case 25: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_207
    case 26: {
      // AddAndReplaceKind
      _zval_dtor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_208
    case 27: {
      // AddAndReplaceKind
      _zval_dtor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_209
    case 28: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_210
    case 29: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(obj)));

      break;
    }
#endif
#ifdef COMPILE_211
    case 30: {
      // AddAndReplaceKind
      closure = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_212
    case 31: {
      // AddAndReplaceKind
      closure_handlers.clone_obj = zend_closure_clone;

      break;
    }
#endif
#ifdef COMPILE_213
    case 32: {
      // AddAndReplaceKind
      closure_handlers.compare_objects = zend_closure_compare_objects;

      break;
    }
#endif
#ifdef COMPILE_214
    case 33: {
      // AddAndReplaceKind
      closure_handlers.get_closure = zend_closure_get_closure;

      break;
    }
#endif
#ifdef COMPILE_215
    case 34: {
      // AddAndReplaceKind
      closure_handlers.get_constructor = zend_closure_get_constructor;

      break;
    }
#endif
#ifdef COMPILE_216
    case 35: {
      // AddAndReplaceKind
      closure_handlers.get_debug_info = zend_closure_get_debug_info;

      break;
    }
#endif
#ifdef COMPILE_217
    case 36: {
      // AddAndReplaceKind
      closure_handlers.get_method = zend_closure_get_method;

      break;
    }
#endif
#ifdef COMPILE_218
    case 37: {
      // AddAndReplaceKind
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

      break;
    }
#endif
#ifdef COMPILE_219
    case 38: {
      // AddAndReplaceKind
      closure_handlers.has_property = zend_closure_has_property;

      break;
    }
#endif
#ifdef COMPILE_220
    case 39: {
      // AddAndReplaceKind
      closure_handlers.read_property = zend_closure_read_property;

      break;
    }
#endif
#ifdef COMPILE_221
    case 40: {
      // AddAndReplaceKind
      closure_handlers.unset_property = zend_closure_unset_property;

      break;
    }
#endif
#ifdef COMPILE_222
    case 41: {
      // AddAndReplaceKind
      closure_handlers.write_property = zend_closure_write_property;

      break;
    }
#endif
#ifdef COMPILE_223
    case 42: {
      // AddAndReplaceKind
      convert_to_boolean((obj));

      break;
    }
#endif
#ifdef COMPILE_224
    case 43: {
      // AddAndReplaceKind
      destroy_op_array(&(closure)->func.op_array);

      break;
    }
#endif
#ifdef COMPILE_225
    case 44: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_226
    case 45: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((obj));

      break;
    }
#endif
#ifdef COMPILE_227
    case 46: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((obj)));

      break;
    }
#endif
#ifdef COMPILE_228
    case 47: {
      // AddAndReplaceKind
      gc_zval_possible_root((obj));

      break;
    }
#endif
#ifdef COMPILE_229
    case 48: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((obj));

      break;
    }
#endif
#ifdef COMPILE_230
    case 49: {
      // AddAndReplaceKind
      if ((*obj).type != 5) {
        return -1;
      }

      break;
    }
#endif
#ifdef COMPILE_231
    case 50: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = ((void *)0);
      }

      break;
    }
#endif
#ifdef COMPILE_232
    case 51: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = closure->this_ptr;
      }

      break;
    }
#endif
#ifdef COMPILE_233
    case 52: {
      // AddAndReplaceKind
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));

      break;
    }
#endif
#ifdef COMPILE_234
    case 53: {
      // AddAndReplaceKind
      memset((closure), 0, sizeof(zend_closure));

      break;
    }
#endif
#ifdef COMPILE_235
    case 54: {
      // AddAndReplaceKind
      zend_ce_closure->create_object = zend_closure_new;

      break;
    }
#endif
#ifdef COMPILE_236
    case 55: {
      // AddAndReplaceKind
      zend_ce_closure->serialize = zend_class_serialize_deny;

      break;
    }
#endif
#ifdef COMPILE_237
    case 56: {
      // AddAndReplaceKind
      zend_ce_closure->unserialize = zend_class_unserialize_deny;

      break;
    }
#endif
#ifdef COMPILE_238
    case 57: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Cannot destroy active lambda function");

      break;
    }
#endif
#ifdef COMPILE_239
    case 58: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Cannot get arguments for calling closure");

      break;
    }
#endif
#ifdef COMPILE_240
    case 59: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Closure object cannot have properties");

      break;
    }
#endif
#ifdef COMPILE_241
    case 60: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");

      break;
    }
#endif
#ifdef COMPILE_242
    case 61: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_243
    case 62: {
      // AddAndReplaceKind
      zend_hash_destroy((closure)->debug_info);

      break;
    }
#endif
#ifdef COMPILE_244
    case 63: {
      // AddAndReplaceKind
      zend_object_std_dtor(&(closure)->std);

      break;
    }
#endif
#ifdef COMPILE_245
    case 64: {
      // AddAndReplaceKind
      zval_addref_p(&((executor_globals.uninitialized_zval)));

      break;
    }
#endif
#ifdef COMPILE_246
    case 65: {
      // AddAndReplaceKind
      zval_addref_p((closure)->this_ptr);

      break;
    }
#endif
#ifdef COMPILE_247
    case 66: {
      // AddAndReplaceKind
      zval_addref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_248
    case 67: {
      // AddAndReplaceKind
      zval_unset_isref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_249
    case 68: {
      // AddInitKind
      memset(closure, 0, sizeof(*(closure)));

      break;
    }
#endif
#ifdef COMPILE_250
    case 69: {
      // IfExitKind
      if (__is_neg("8-69", 7, &(obj), sizeof(obj), &(closure), sizeof(closure),
                   &(fptr_ptr), sizeof(fptr_ptr), &(closure->this_ptr),
                   sizeof(closure->this_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &(closure->debug_info),
                   sizeof(closure->debug_info)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_251
    case 70: {
      // IfExitKind
      if (__is_neg("8-70", 7, &(obj), sizeof(obj), &(closure), sizeof(closure),
                   &(fptr_ptr), sizeof(fptr_ptr), &(closure->this_ptr),
                   sizeof(closure->this_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &(closure->debug_info),
                   sizeof(closure->debug_info)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_252
    case 71: {
      // IfExitKind
      if (__is_neg("8-71", 7, &(obj), sizeof(obj), &(closure), sizeof(closure),
                   &(fptr_ptr), sizeof(fptr_ptr), &(closure->this_ptr),
                   sizeof(closure->this_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &(closure->debug_info),
                   sizeof(closure->debug_info)))
        return 5;

      break;
    }
#endif
    }
    switch (__choose("__SWITCH9")) {
    case 0: {
      closure = (zend_closure *)zend_object_store_get_object(obj TSRMLS_CC);

      break;
    }
#ifdef COMPILE_253
    case 1: {
      // GuardKind
      if (!__is_neg("9-1", 7, &(obj), sizeof(obj), &(closure), sizeof(closure),
                    &(fptr_ptr), sizeof(fptr_ptr), &(closure->this_ptr),
                    sizeof(closure->this_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                    &(ce_ptr), sizeof(ce_ptr), &(closure->debug_info),
                    sizeof(closure->debug_info)))
        closure = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_254
    case 2: {
      // ReplaceKind
      (closure) = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_255
    case 3: {
      // ReplaceKind
      closure = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
    }
  }
  // prophet generated patch
  {
    switch (__choose("__SWITCH10")) {
    case 0:
      break;
#ifdef COMPILE_256
    case 1: {
      // AddAndReplaceKind
      (*(closure)->func.op_array.refcount)++;

      break;
    }
#endif
#ifdef COMPILE_257
    case 2: {
      // AddAndReplaceKind
      (closure) = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_258
    case 3: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_259
    case 4: {
      // AddAndReplaceKind
      *(ce_ptr) = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_260
    case 5: {
      // AddAndReplaceKind
      *(ce_ptr) = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_261
    case 6: {
      // AddAndReplaceKind
      *(fptr_ptr) = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_262
    case 7: {
      // AddAndReplaceKind
      *(zobj_ptr) = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_263
    case 8: {
      // AddAndReplaceKind
      *(zobj_ptr) = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_264
    case 9: {
      // AddAndReplaceKind
      *ce_ptr = (closure)->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_265
    case 10: {
      // AddAndReplaceKind
      *ce_ptr = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_266
    case 11: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_267
    case 12: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_268
    case 13: {
      // AddAndReplaceKind
      *fptr_ptr = &(closure)->func;

      break;
    }
#endif
#ifdef COMPILE_269
    case 14: {
      // AddAndReplaceKind
      *fptr_ptr = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_270
    case 15: {
      // AddAndReplaceKind
      *zobj_ptr = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_271
    case 16: {
      // AddAndReplaceKind
      *zobj_ptr = (closure)->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_272
    case 17: {
      // AddAndReplaceKind
      *zobj_ptr = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_273
    case 18: {
      // AddAndReplaceKind
      _array_init(((obj)), 0);

      break;
    }
#endif
#ifdef COMPILE_274
    case 19: {
      // AddAndReplaceKind
      _efree(((closure)));

      break;
    }
#endif
#ifdef COMPILE_275
    case 20: {
      // AddAndReplaceKind
      _efree(((closure)->debug_info));

      break;
    }
#endif
#ifdef COMPILE_276
    case 21: {
      // AddAndReplaceKind
      _efree(((obj)));

      break;
    }
#endif
#ifdef COMPILE_277
    case 22: {
      // AddAndReplaceKind
      _object_init_ex(((obj)), (zend_ce_closure));

      break;
    }
#endif
#ifdef COMPILE_278
    case 23: {
      // AddAndReplaceKind
      _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_279
    case 24: {
      // AddAndReplaceKind
      _zval_copy_ctor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_280
    case 25: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_281
    case 26: {
      // AddAndReplaceKind
      _zval_dtor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_282
    case 27: {
      // AddAndReplaceKind
      _zval_dtor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_283
    case 28: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_284
    case 29: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(obj)));

      break;
    }
#endif
#ifdef COMPILE_285
    case 30: {
      // AddAndReplaceKind
      closure = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_286
    case 31: {
      // AddAndReplaceKind
      closure_handlers.clone_obj = zend_closure_clone;

      break;
    }
#endif
#ifdef COMPILE_287
    case 32: {
      // AddAndReplaceKind
      closure_handlers.compare_objects = zend_closure_compare_objects;

      break;
    }
#endif
#ifdef COMPILE_288
    case 33: {
      // AddAndReplaceKind
      closure_handlers.get_closure = zend_closure_get_closure;

      break;
    }
#endif
#ifdef COMPILE_289
    case 34: {
      // AddAndReplaceKind
      closure_handlers.get_constructor = zend_closure_get_constructor;

      break;
    }
#endif
#ifdef COMPILE_290
    case 35: {
      // AddAndReplaceKind
      closure_handlers.get_debug_info = zend_closure_get_debug_info;

      break;
    }
#endif
#ifdef COMPILE_291
    case 36: {
      // AddAndReplaceKind
      closure_handlers.get_method = zend_closure_get_method;

      break;
    }
#endif
#ifdef COMPILE_292
    case 37: {
      // AddAndReplaceKind
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

      break;
    }
#endif
#ifdef COMPILE_293
    case 38: {
      // AddAndReplaceKind
      closure_handlers.has_property = zend_closure_has_property;

      break;
    }
#endif
#ifdef COMPILE_294
    case 39: {
      // AddAndReplaceKind
      closure_handlers.read_property = zend_closure_read_property;

      break;
    }
#endif
#ifdef COMPILE_295
    case 40: {
      // AddAndReplaceKind
      closure_handlers.unset_property = zend_closure_unset_property;

      break;
    }
#endif
#ifdef COMPILE_296
    case 41: {
      // AddAndReplaceKind
      closure_handlers.write_property = zend_closure_write_property;

      break;
    }
#endif
#ifdef COMPILE_297
    case 42: {
      // AddAndReplaceKind
      convert_to_boolean((obj));

      break;
    }
#endif
#ifdef COMPILE_298
    case 43: {
      // AddAndReplaceKind
      destroy_op_array(&(closure)->func.op_array);

      break;
    }
#endif
#ifdef COMPILE_299
    case 44: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_300
    case 45: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((obj));

      break;
    }
#endif
#ifdef COMPILE_301
    case 46: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((obj)));

      break;
    }
#endif
#ifdef COMPILE_302
    case 47: {
      // AddAndReplaceKind
      gc_zval_possible_root((obj));

      break;
    }
#endif
#ifdef COMPILE_303
    case 48: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((obj));

      break;
    }
#endif
#ifdef COMPILE_304
    case 49: {
      // AddAndReplaceKind
      if ((*obj).type != 5) {
        return -1;
      }

      break;
    }
#endif
#ifdef COMPILE_305
    case 50: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = ((void *)0);
      }

      break;
    }
#endif
#ifdef COMPILE_306
    case 51: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = closure->this_ptr;
      }

      break;
    }
#endif
#ifdef COMPILE_307
    case 52: {
      // AddAndReplaceKind
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));

      break;
    }
#endif
#ifdef COMPILE_308
    case 53: {
      // AddAndReplaceKind
      memset((closure), 0, sizeof(zend_closure));

      break;
    }
#endif
#ifdef COMPILE_309
    case 54: {
      // AddAndReplaceKind
      zend_ce_closure->create_object = zend_closure_new;

      break;
    }
#endif
#ifdef COMPILE_310
    case 55: {
      // AddAndReplaceKind
      zend_ce_closure->serialize = zend_class_serialize_deny;

      break;
    }
#endif
#ifdef COMPILE_311
    case 56: {
      // AddAndReplaceKind
      zend_ce_closure->unserialize = zend_class_unserialize_deny;

      break;
    }
#endif
#ifdef COMPILE_312
    case 57: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Cannot destroy active lambda function");

      break;
    }
#endif
#ifdef COMPILE_313
    case 58: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Cannot get arguments for calling closure");

      break;
    }
#endif
#ifdef COMPILE_314
    case 59: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Closure object cannot have properties");

      break;
    }
#endif
#ifdef COMPILE_315
    case 60: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");

      break;
    }
#endif
#ifdef COMPILE_316
    case 61: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_317
    case 62: {
      // AddAndReplaceKind
      zend_hash_destroy((closure)->debug_info);

      break;
    }
#endif
#ifdef COMPILE_318
    case 63: {
      // AddAndReplaceKind
      zend_object_std_dtor(&(closure)->std);

      break;
    }
#endif
#ifdef COMPILE_319
    case 64: {
      // AddAndReplaceKind
      zval_addref_p(&((executor_globals.uninitialized_zval)));

      break;
    }
#endif
#ifdef COMPILE_320
    case 65: {
      // AddAndReplaceKind
      zval_addref_p((closure)->this_ptr);

      break;
    }
#endif
#ifdef COMPILE_321
    case 66: {
      // AddAndReplaceKind
      zval_addref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_322
    case 67: {
      // AddAndReplaceKind
      zval_unset_isref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_323
    case 68: {
      // AddInitKind
      memset(&closure->func, 0, sizeof(*(&closure->func)));

      break;
    }
#endif
#ifdef COMPILE_324
    case 69: {
      // AddInitKind
      memset(closure, 0, sizeof(*(closure)));

      break;
    }
#endif
#ifdef COMPILE_325
    case 70: {
      // IfExitKind
      if (__is_neg("10-70", 7, &(fptr_ptr), sizeof(fptr_ptr), &(closure),
                   sizeof(closure), &(obj), sizeof(obj), &(closure->this_ptr),
                   sizeof(closure->this_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &(closure->debug_info),
                   sizeof(closure->debug_info)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_326
    case 71: {
      // IfExitKind
      if (__is_neg("10-71", 7, &(fptr_ptr), sizeof(fptr_ptr), &(closure),
                   sizeof(closure), &(obj), sizeof(obj), &(closure->this_ptr),
                   sizeof(closure->this_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &(closure->debug_info),
                   sizeof(closure->debug_info)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_327
    case 72: {
      // IfExitKind
      if (__is_neg("10-72", 7, &(fptr_ptr), sizeof(fptr_ptr), &(closure),
                   sizeof(closure), &(obj), sizeof(obj), &(closure->this_ptr),
                   sizeof(closure->this_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                   &(ce_ptr), sizeof(ce_ptr), &(closure->debug_info),
                   sizeof(closure->debug_info)))
        return 5;

      break;
    }
#endif
    }
    switch (__choose("__SWITCH11")) {
    case 0: {
      *fptr_ptr = &closure->func;

      break;
    }
#ifdef COMPILE_328
    case 1: {
      // GuardKind
      if (!__is_neg("11-1", 7, &(fptr_ptr), sizeof(fptr_ptr), &(closure),
                    sizeof(closure), &(obj), sizeof(obj), &(closure->this_ptr),
                    sizeof(closure->this_ptr), &(zobj_ptr), sizeof(zobj_ptr),
                    &(ce_ptr), sizeof(ce_ptr), &(closure->debug_info),
                    sizeof(closure->debug_info)))
        *fptr_ptr = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_329
    case 2: {
      // ReplaceKind
      *(fptr_ptr) = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_330
    case 3: {
      // ReplaceKind
      *fptr_ptr = &(closure)->func;

      break;
    }
#endif
    }
  }

  // prophet generated patch
  {
    switch (__choose("__SWITCH12")) {
    case 0:
      break;
#ifdef COMPILE_331
    case 1: {
      // AddAndReplaceKind
      (*(closure)->func.op_array.refcount)++;

      break;
    }
#endif
#ifdef COMPILE_332
    case 2: {
      // AddAndReplaceKind
      (closure) = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_333
    case 3: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_334
    case 4: {
      // AddAndReplaceKind
      *(ce_ptr) = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_335
    case 5: {
      // AddAndReplaceKind
      *(ce_ptr) = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_336
    case 6: {
      // AddAndReplaceKind
      *(fptr_ptr) = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_337
    case 7: {
      // AddAndReplaceKind
      *(zobj_ptr) = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_338
    case 8: {
      // AddAndReplaceKind
      *(zobj_ptr) = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_339
    case 9: {
      // AddAndReplaceKind
      *ce_ptr = (closure)->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_340
    case 10: {
      // AddAndReplaceKind
      *ce_ptr = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_341
    case 11: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_342
    case 12: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_343
    case 13: {
      // AddAndReplaceKind
      *fptr_ptr = &(closure)->func;

      break;
    }
#endif
#ifdef COMPILE_344
    case 14: {
      // AddAndReplaceKind
      *fptr_ptr = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_345
    case 15: {
      // AddAndReplaceKind
      *zobj_ptr = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_346
    case 16: {
      // AddAndReplaceKind
      *zobj_ptr = (closure)->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_347
    case 17: {
      // AddAndReplaceKind
      *zobj_ptr = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_348
    case 18: {
      // AddAndReplaceKind
      _array_init(((obj)), 0);

      break;
    }
#endif
#ifdef COMPILE_349
    case 19: {
      // AddAndReplaceKind
      _efree(((closure)));

      break;
    }
#endif
#ifdef COMPILE_350
    case 20: {
      // AddAndReplaceKind
      _efree(((closure)->debug_info));

      break;
    }
#endif
#ifdef COMPILE_351
    case 21: {
      // AddAndReplaceKind
      _efree(((obj)));

      break;
    }
#endif
#ifdef COMPILE_352
    case 22: {
      // AddAndReplaceKind
      _object_init_ex(((obj)), (zend_ce_closure));

      break;
    }
#endif
#ifdef COMPILE_353
    case 23: {
      // AddAndReplaceKind
      _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_354
    case 24: {
      // AddAndReplaceKind
      _zval_copy_ctor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_355
    case 25: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_356
    case 26: {
      // AddAndReplaceKind
      _zval_dtor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_357
    case 27: {
      // AddAndReplaceKind
      _zval_dtor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_358
    case 28: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_359
    case 29: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(obj)));

      break;
    }
#endif
#ifdef COMPILE_360
    case 30: {
      // AddAndReplaceKind
      closure = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_361
    case 31: {
      // AddAndReplaceKind
      closure_handlers.clone_obj = zend_closure_clone;

      break;
    }
#endif
#ifdef COMPILE_362
    case 32: {
      // AddAndReplaceKind
      closure_handlers.compare_objects = zend_closure_compare_objects;

      break;
    }
#endif
#ifdef COMPILE_363
    case 33: {
      // AddAndReplaceKind
      closure_handlers.get_closure = zend_closure_get_closure;

      break;
    }
#endif
#ifdef COMPILE_364
    case 34: {
      // AddAndReplaceKind
      closure_handlers.get_constructor = zend_closure_get_constructor;

      break;
    }
#endif
#ifdef COMPILE_365
    case 35: {
      // AddAndReplaceKind
      closure_handlers.get_debug_info = zend_closure_get_debug_info;

      break;
    }
#endif
#ifdef COMPILE_366
    case 36: {
      // AddAndReplaceKind
      closure_handlers.get_method = zend_closure_get_method;

      break;
    }
#endif
#ifdef COMPILE_367
    case 37: {
      // AddAndReplaceKind
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

      break;
    }
#endif
#ifdef COMPILE_368
    case 38: {
      // AddAndReplaceKind
      closure_handlers.has_property = zend_closure_has_property;

      break;
    }
#endif
#ifdef COMPILE_369
    case 39: {
      // AddAndReplaceKind
      closure_handlers.read_property = zend_closure_read_property;

      break;
    }
#endif
#ifdef COMPILE_370
    case 40: {
      // AddAndReplaceKind
      closure_handlers.unset_property = zend_closure_unset_property;

      break;
    }
#endif
#ifdef COMPILE_371
    case 41: {
      // AddAndReplaceKind
      closure_handlers.write_property = zend_closure_write_property;

      break;
    }
#endif
#ifdef COMPILE_372
    case 42: {
      // AddAndReplaceKind
      convert_to_boolean((obj));

      break;
    }
#endif
#ifdef COMPILE_373
    case 43: {
      // AddAndReplaceKind
      destroy_op_array(&(closure)->func.op_array);

      break;
    }
#endif
#ifdef COMPILE_374
    case 44: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_375
    case 45: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((obj));

      break;
    }
#endif
#ifdef COMPILE_376
    case 46: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((obj)));

      break;
    }
#endif
#ifdef COMPILE_377
    case 47: {
      // AddAndReplaceKind
      gc_zval_possible_root((obj));

      break;
    }
#endif
#ifdef COMPILE_378
    case 48: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((obj));

      break;
    }
#endif
#ifdef COMPILE_379
    case 49: {
      // AddAndReplaceKind
      if ((*obj).type != 5) {
        return -1;
      }

      break;
    }
#endif
#ifdef COMPILE_380
    case 50: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = ((void *)0);
      }

      break;
    }
#endif
#ifdef COMPILE_381
    case 51: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = closure->this_ptr;
      }

      break;
    }
#endif
#ifdef COMPILE_382
    case 52: {
      // AddAndReplaceKind
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));

      break;
    }
#endif
#ifdef COMPILE_383
    case 53: {
      // AddAndReplaceKind
      memset((closure), 0, sizeof(zend_closure));

      break;
    }
#endif
#ifdef COMPILE_384
    case 54: {
      // AddAndReplaceKind
      zend_ce_closure->create_object = zend_closure_new;

      break;
    }
#endif
#ifdef COMPILE_385
    case 55: {
      // AddAndReplaceKind
      zend_ce_closure->serialize = zend_class_serialize_deny;

      break;
    }
#endif
#ifdef COMPILE_386
    case 56: {
      // AddAndReplaceKind
      zend_ce_closure->unserialize = zend_class_unserialize_deny;

      break;
    }
#endif
#ifdef COMPILE_387
    case 57: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Cannot destroy active lambda function");

      break;
    }
#endif
#ifdef COMPILE_388
    case 58: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Cannot get arguments for calling closure");

      break;
    }
#endif
#ifdef COMPILE_389
    case 59: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Closure object cannot have properties");

      break;
    }
#endif
#ifdef COMPILE_390
    case 60: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");

      break;
    }
#endif
#ifdef COMPILE_391
    case 61: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_392
    case 62: {
      // AddAndReplaceKind
      zend_hash_destroy((closure)->debug_info);

      break;
    }
#endif
#ifdef COMPILE_393
    case 63: {
      // AddAndReplaceKind
      zend_object_std_dtor(&(closure)->std);

      break;
    }
#endif
#ifdef COMPILE_394
    case 64: {
      // AddAndReplaceKind
      zval_addref_p(&((executor_globals.uninitialized_zval)));

      break;
    }
#endif
#ifdef COMPILE_395
    case 65: {
      // AddAndReplaceKind
      zval_addref_p((closure)->this_ptr);

      break;
    }
#endif
#ifdef COMPILE_396
    case 66: {
      // AddAndReplaceKind
      zval_addref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_397
    case 67: {
      // AddAndReplaceKind
      zval_unset_isref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_398
    case 68: {
      // AddInitKind
      memset(&closure->func, 0, sizeof(*(&closure->func)));

      break;
    }
#endif
#ifdef COMPILE_399
    case 69: {
      // AddInitKind
      memset(&closure->func.common, 0, sizeof(*(&closure->func.common)));

      break;
    }
#endif
#ifdef COMPILE_400
    case 70: {
      // AddInitKind
      memset(closure, 0, sizeof(*(closure)));

      break;
    }
#endif
#ifdef COMPILE_401
    case 71: {
      // IfExitKind
      if (__is_neg("12-71", 7, &(closure), sizeof(closure),
                   &(closure->this_ptr), sizeof(closure->this_ptr), &(zobj_ptr),
                   sizeof(zobj_ptr), &(fptr_ptr), sizeof(fptr_ptr), &(obj),
                   sizeof(obj), &(ce_ptr), sizeof(ce_ptr),
                   &(closure->debug_info), sizeof(closure->debug_info)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_402
    case 72: {
      // IfExitKind
      if (__is_neg("12-72", 7, &(closure), sizeof(closure),
                   &(closure->this_ptr), sizeof(closure->this_ptr), &(zobj_ptr),
                   sizeof(zobj_ptr), &(fptr_ptr), sizeof(fptr_ptr), &(obj),
                   sizeof(obj), &(ce_ptr), sizeof(ce_ptr),
                   &(closure->debug_info), sizeof(closure->debug_info)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_403
    case 73: {
      // IfExitKind
      if (__is_neg("12-73", 7, &(closure), sizeof(closure),
                   &(closure->this_ptr), sizeof(closure->this_ptr), &(zobj_ptr),
                   sizeof(zobj_ptr), &(fptr_ptr), sizeof(fptr_ptr), &(obj),
                   sizeof(obj), &(ce_ptr), sizeof(ce_ptr),
                   &(closure->debug_info), sizeof(closure->debug_info)))
        return 5;

      break;
    }
#endif
    }
    {
      long long __temp13 = (closure->this_ptr);
      switch (__choose("__SWITCH13")) {
      case 0: {

        break;
      }
#ifdef COMPILE_404
      case 1: {
        __temp13 = ((closure->this_ptr) ||
                    __is_neg("13-1", 7, &(closure), sizeof(closure),
                             &(closure->this_ptr), sizeof(closure->this_ptr),
                             &(zobj_ptr), sizeof(zobj_ptr), &(fptr_ptr),
                             sizeof(fptr_ptr), &(obj), sizeof(obj), &(ce_ptr),
                             sizeof(ce_ptr), &(closure->debug_info),
                             sizeof(closure->debug_info)));

        break;
      }
#endif
#ifdef COMPILE_405
      case 2: {
        __temp13 = ((closure->this_ptr) &&
                    !__is_neg("13-2", 7, &(closure), sizeof(closure),
                              &(closure->this_ptr), sizeof(closure->this_ptr),
                              &(zobj_ptr), sizeof(zobj_ptr), &(fptr_ptr),
                              sizeof(fptr_ptr), &(obj), sizeof(obj), &(ce_ptr),
                              sizeof(ce_ptr), &(closure->debug_info),
                              sizeof(closure->debug_info)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH21")) {
      case 0: {
        if (__temp13) {
          if (zobj_ptr) {
            *zobj_ptr = closure->this_ptr;
          }
          *ce_ptr = Z_OBJCE_P(closure->this_ptr);
        } else {
          {
            switch (__choose("__SWITCH14")) {
            case 0:
              break;
#ifdef COMPILE_406
            case 1: {
              // AddAndReplaceKind
              (*(closure)->func.op_array.refcount)++;

              break;
            }
#endif
#ifdef COMPILE_407
            case 2: {
              // AddAndReplaceKind
              (closure) = (zend_closure *)zend_object_store_get_object(obj);

              break;
            }
#endif
#ifdef COMPILE_408
            case 3: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_409
            case 4: {
              // AddAndReplaceKind
              *(ce_ptr) = closure->func.common.scope;

              break;
            }
#endif
#ifdef COMPILE_410
            case 5: {
              // AddAndReplaceKind
              *(ce_ptr) = zend_get_class_entry(&(*closure->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_411
            case 6: {
              // AddAndReplaceKind
              *(fptr_ptr) = &closure->func;

              break;
            }
#endif
#ifdef COMPILE_412
            case 7: {
              // AddAndReplaceKind
              *(zobj_ptr) = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_413
            case 8: {
              // AddAndReplaceKind
              *(zobj_ptr) = closure->this_ptr;

              break;
            }
#endif
#ifdef COMPILE_414
            case 9: {
              // AddAndReplaceKind
              *ce_ptr = (closure)->func.common.scope;

              break;
            }
#endif
#ifdef COMPILE_415
            case 10: {
              // AddAndReplaceKind
              *ce_ptr = closure->func.common.scope;

              break;
            }
#endif
#ifdef COMPILE_416
            case 11: {
              // AddAndReplaceKind
              *ce_ptr = zend_get_class_entry(&(*(closure)->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_417
            case 12: {
              // AddAndReplaceKind
              *ce_ptr = zend_get_class_entry(&(*closure->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_418
            case 13: {
              // AddAndReplaceKind
              *fptr_ptr = &(closure)->func;

              break;
            }
#endif
#ifdef COMPILE_419
            case 14: {
              // AddAndReplaceKind
              *fptr_ptr = &closure->func;

              break;
            }
#endif
#ifdef COMPILE_420
            case 15: {
              // AddAndReplaceKind
              *zobj_ptr = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_421
            case 16: {
              // AddAndReplaceKind
              *zobj_ptr = (closure)->this_ptr;

              break;
            }
#endif
#ifdef COMPILE_422
            case 17: {
              // AddAndReplaceKind
              *zobj_ptr = closure->this_ptr;

              break;
            }
#endif
#ifdef COMPILE_423
            case 18: {
              // AddAndReplaceKind
              _array_init(((obj)), 0);

              break;
            }
#endif
#ifdef COMPILE_424
            case 19: {
              // AddAndReplaceKind
              _efree(((closure)));

              break;
            }
#endif
#ifdef COMPILE_425
            case 20: {
              // AddAndReplaceKind
              _efree(((closure)->debug_info));

              break;
            }
#endif
#ifdef COMPILE_426
            case 21: {
              // AddAndReplaceKind
              _efree(((obj)));

              break;
            }
#endif
#ifdef COMPILE_427
            case 22: {
              // AddAndReplaceKind
              _object_init_ex(((obj)), (zend_ce_closure));

              break;
            }
#endif
#ifdef COMPILE_428
            case 23: {
              // AddAndReplaceKind
              _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_429
            case 24: {
              // AddAndReplaceKind
              _zval_copy_ctor(((obj)));

              break;
            }
#endif
#ifdef COMPILE_430
            case 25: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((obj));

              break;
            }
#endif
#ifdef COMPILE_431
            case 26: {
              // AddAndReplaceKind
              _zval_dtor(((obj)));

              break;
            }
#endif
#ifdef COMPILE_432
            case 27: {
              // AddAndReplaceKind
              _zval_dtor_func((obj));

              break;
            }
#endif
#ifdef COMPILE_433
            case 28: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(closure)->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_434
            case 29: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(obj)));

              break;
            }
#endif
#ifdef COMPILE_435
            case 30: {
              // AddAndReplaceKind
              closure = (zend_closure *)zend_object_store_get_object(obj);

              break;
            }
#endif
#ifdef COMPILE_436
            case 31: {
              // AddAndReplaceKind
              closure_handlers.clone_obj = zend_closure_clone;

              break;
            }
#endif
#ifdef COMPILE_437
            case 32: {
              // AddAndReplaceKind
              closure_handlers.compare_objects = zend_closure_compare_objects;

              break;
            }
#endif
#ifdef COMPILE_438
            case 33: {
              // AddAndReplaceKind
              closure_handlers.get_closure = zend_closure_get_closure;

              break;
            }
#endif
#ifdef COMPILE_439
            case 34: {
              // AddAndReplaceKind
              closure_handlers.get_constructor = zend_closure_get_constructor;

              break;
            }
#endif
#ifdef COMPILE_440
            case 35: {
              // AddAndReplaceKind
              closure_handlers.get_debug_info = zend_closure_get_debug_info;

              break;
            }
#endif
#ifdef COMPILE_441
            case 36: {
              // AddAndReplaceKind
              closure_handlers.get_method = zend_closure_get_method;

              break;
            }
#endif
#ifdef COMPILE_442
            case 37: {
              // AddAndReplaceKind
              closure_handlers.get_property_ptr_ptr =
                  zend_closure_get_property_ptr_ptr;

              break;
            }
#endif
#ifdef COMPILE_443
            case 38: {
              // AddAndReplaceKind
              closure_handlers.has_property = zend_closure_has_property;

              break;
            }
#endif
#ifdef COMPILE_444
            case 39: {
              // AddAndReplaceKind
              closure_handlers.read_property = zend_closure_read_property;

              break;
            }
#endif
#ifdef COMPILE_445
            case 40: {
              // AddAndReplaceKind
              closure_handlers.unset_property = zend_closure_unset_property;

              break;
            }
#endif
#ifdef COMPILE_446
            case 41: {
              // AddAndReplaceKind
              closure_handlers.write_property = zend_closure_write_property;

              break;
            }
#endif
#ifdef COMPILE_447
            case 42: {
              // AddAndReplaceKind
              convert_to_boolean((obj));

              break;
            }
#endif
#ifdef COMPILE_448
            case 43: {
              // AddAndReplaceKind
              destroy_op_array(&(closure)->func.op_array);

              break;
            }
#endif
#ifdef COMPILE_449
            case 44: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_450
            case 45: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((obj));

              break;
            }
#endif
#ifdef COMPILE_451
            case 46: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((obj)));

              break;
            }
#endif
#ifdef COMPILE_452
            case 47: {
              // AddAndReplaceKind
              gc_zval_possible_root((obj));

              break;
            }
#endif
#ifdef COMPILE_453
            case 48: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((obj));

              break;
            }
#endif
#ifdef COMPILE_454
            case 49: {
              // AddAndReplaceKind
              if ((*obj).type != 5) {
                return -1;
              }

              break;
            }
#endif
#ifdef COMPILE_455
            case 50: {
              // AddAndReplaceKind
              if (zobj_ptr) {
                *zobj_ptr = ((void *)0);
              }

              break;
            }
#endif
#ifdef COMPILE_456
            case 51: {
              // AddAndReplaceKind
              if (zobj_ptr) {
                *zobj_ptr = closure->this_ptr;
              }

              break;
            }
#endif
#ifdef COMPILE_457
            case 52: {
              // AddAndReplaceKind
              memcpy(&closure_handlers, zend_get_std_object_handlers(),
                     sizeof(zend_object_handlers));

              break;
            }
#endif
#ifdef COMPILE_458
            case 53: {
              // AddAndReplaceKind
              memset((closure), 0, sizeof(zend_closure));

              break;
            }
#endif
#ifdef COMPILE_459
            case 54: {
              // AddAndReplaceKind
              zend_ce_closure->create_object = zend_closure_new;

              break;
            }
#endif
#ifdef COMPILE_460
            case 55: {
              // AddAndReplaceKind
              zend_ce_closure->serialize = zend_class_serialize_deny;

              break;
            }
#endif
#ifdef COMPILE_461
            case 56: {
              // AddAndReplaceKind
              zend_ce_closure->unserialize = zend_class_unserialize_deny;

              break;
            }
#endif
#ifdef COMPILE_462
            case 57: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Cannot destroy active lambda function");

              break;
            }
#endif
#ifdef COMPILE_463
            case 58: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Cannot get arguments for calling closure");

              break;
            }
#endif
#ifdef COMPILE_464
            case 59: {
              // AddAndReplaceKind
              zend_error((1 << 12L), "Closure object cannot have properties");

              break;
            }
#endif
#ifdef COMPILE_465
            case 60: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Instantiation of 'Closure' is not allowed");

              break;
            }
#endif
#ifdef COMPILE_466
            case 61: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_467
            case 62: {
              // AddAndReplaceKind
              zend_hash_destroy((closure)->debug_info);

              break;
            }
#endif
#ifdef COMPILE_468
            case 63: {
              // AddAndReplaceKind
              zend_object_std_dtor(&(closure)->std);

              break;
            }
#endif
#ifdef COMPILE_469
            case 64: {
              // AddAndReplaceKind
              zval_addref_p(&((executor_globals.uninitialized_zval)));

              break;
            }
#endif
#ifdef COMPILE_470
            case 65: {
              // AddAndReplaceKind
              zval_addref_p((closure)->this_ptr);

              break;
            }
#endif
#ifdef COMPILE_471
            case 66: {
              // AddAndReplaceKind
              zval_addref_p((obj));

              break;
            }
#endif
#ifdef COMPILE_472
            case 67: {
              // AddAndReplaceKind
              zval_unset_isref_p((obj));

              break;
            }
#endif
#ifdef COMPILE_473
            case 68: {
              // IfExitKind
              if (__is_neg("14-68", 5, &(zobj_ptr), sizeof(zobj_ptr), &(ce_ptr),
                           sizeof(ce_ptr), &(closure), sizeof(closure),
                           &(fptr_ptr), sizeof(fptr_ptr), &(obj), sizeof(obj)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_474
            case 69: {
              // IfExitKind
              if (__is_neg("14-69", 5, &(zobj_ptr), sizeof(zobj_ptr), &(ce_ptr),
                           sizeof(ce_ptr), &(closure), sizeof(closure),
                           &(fptr_ptr), sizeof(fptr_ptr), &(obj), sizeof(obj)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_475
            case 70: {
              // IfExitKind
              if (__is_neg("14-70", 5, &(zobj_ptr), sizeof(zobj_ptr), &(ce_ptr),
                           sizeof(ce_ptr), &(closure), sizeof(closure),
                           &(fptr_ptr), sizeof(fptr_ptr), &(obj), sizeof(obj)))
                return 5;

              break;
            }
#endif
            }
            {
              long long __temp15 = (zobj_ptr);
              switch (__choose("__SWITCH15")) {
              case 0: {

                break;
              }
#ifdef COMPILE_476
              case 1: {
                __temp15 = ((zobj_ptr) ||
                            __is_neg("15-1", 5, &(zobj_ptr), sizeof(zobj_ptr),
                                     &(ce_ptr), sizeof(ce_ptr), &(closure),
                                     sizeof(closure), &(fptr_ptr),
                                     sizeof(fptr_ptr), &(obj), sizeof(obj)));

                break;
              }
#endif
#ifdef COMPILE_477
              case 2: {
                __temp15 = ((zobj_ptr) &&
                            !__is_neg("15-2", 5, &(zobj_ptr), sizeof(zobj_ptr),
                                      &(ce_ptr), sizeof(ce_ptr), &(closure),
                                      sizeof(closure), &(fptr_ptr),
                                      sizeof(fptr_ptr), &(obj), sizeof(obj)));

                break;
              }
#endif
              }
              switch (__choose("__SWITCH18")) {
              case 0: {
                if (__temp15) {
                  {
                    switch (__choose("__SWITCH16")) {
                    case 0:
                      break;
#ifdef COMPILE_478
                    case 1: {
                      // AddAndReplaceKind
                      (*(closure)->func.op_array.refcount)++;

                      break;
                    }
#endif
#ifdef COMPILE_479
                    case 2: {
                      // AddAndReplaceKind
                      (closure) =
                          (zend_closure *)zend_object_store_get_object(obj);

                      break;
                    }
#endif
#ifdef COMPILE_480
                    case 3: {
                      // AddAndReplaceKind
                      (executor_globals.argument_stack) =
                          zend_vm_stack_new_page(((16 * 1024) - 16));

                      break;
                    }
#endif
#ifdef COMPILE_481
                    case 4: {
                      // AddAndReplaceKind
                      *(ce_ptr) = closure->func.common.scope;

                      break;
                    }
#endif
#ifdef COMPILE_482
                    case 5: {
                      // AddAndReplaceKind
                      *(ce_ptr) = zend_get_class_entry(&(*closure->this_ptr));

                      break;
                    }
#endif
#ifdef COMPILE_483
                    case 6: {
                      // AddAndReplaceKind
                      *(fptr_ptr) = &closure->func;

                      break;
                    }
#endif
#ifdef COMPILE_484
                    case 7: {
                      // AddAndReplaceKind
                      *(zobj_ptr) = ((void *)0);

                      break;
                    }
#endif
#ifdef COMPILE_485
                    case 8: {
                      // AddAndReplaceKind
                      *(zobj_ptr) = closure->this_ptr;

                      break;
                    }
#endif
#ifdef COMPILE_486
                    case 9: {
                      // AddAndReplaceKind
                      *ce_ptr = (closure)->func.common.scope;

                      break;
                    }
#endif
#ifdef COMPILE_487
                    case 10: {
                      // AddAndReplaceKind
                      *ce_ptr = closure->func.common.scope;

                      break;
                    }
#endif
#ifdef COMPILE_488
                    case 11: {
                      // AddAndReplaceKind
                      *ce_ptr = zend_get_class_entry(&(*(closure)->this_ptr));

                      break;
                    }
#endif
#ifdef COMPILE_489
                    case 12: {
                      // AddAndReplaceKind
                      *ce_ptr = zend_get_class_entry(&(*closure->this_ptr));

                      break;
                    }
#endif
#ifdef COMPILE_490
                    case 13: {
                      // AddAndReplaceKind
                      *fptr_ptr = &(closure)->func;

                      break;
                    }
#endif
#ifdef COMPILE_491
                    case 14: {
                      // AddAndReplaceKind
                      *fptr_ptr = &closure->func;

                      break;
                    }
#endif
#ifdef COMPILE_492
                    case 15: {
                      // AddAndReplaceKind
                      *zobj_ptr = ((void *)0);

                      break;
                    }
#endif
#ifdef COMPILE_493
                    case 16: {
                      // AddAndReplaceKind
                      *zobj_ptr = (closure)->this_ptr;

                      break;
                    }
#endif
#ifdef COMPILE_494
                    case 17: {
                      // AddAndReplaceKind
                      *zobj_ptr = closure->this_ptr;

                      break;
                    }
#endif
#ifdef COMPILE_495
                    case 18: {
                      // AddAndReplaceKind
                      _array_init(((obj)), 0);

                      break;
                    }
#endif
#ifdef COMPILE_496
                    case 19: {
                      // AddAndReplaceKind
                      _efree(((closure)));

                      break;
                    }
#endif
#ifdef COMPILE_497
                    case 20: {
                      // AddAndReplaceKind
                      _efree(((closure)->debug_info));

                      break;
                    }
#endif
#ifdef COMPILE_498
                    case 21: {
                      // AddAndReplaceKind
                      _efree(((obj)));

                      break;
                    }
#endif
#ifdef COMPILE_499
                    case 22: {
                      // AddAndReplaceKind
                      _object_init_ex(((obj)), (zend_ce_closure));

                      break;
                    }
#endif
#ifdef COMPILE_500
                    case 23: {
                      // AddAndReplaceKind
                      _zend_hash_init(((closure)->debug_info), (1),
                                      (((void *)0)),
                                      ((void (*)(void *))_zval_ptr_dtor), (0));

                      break;
                    }
#endif
#ifdef COMPILE_501
                    case 24: {
                      // AddAndReplaceKind
                      _zval_copy_ctor(((obj)));

                      break;
                    }
#endif
#ifdef COMPILE_502
                    case 25: {
                      // AddAndReplaceKind
                      _zval_copy_ctor_func((obj));

                      break;
                    }
#endif
#ifdef COMPILE_503
                    case 26: {
                      // AddAndReplaceKind
                      _zval_dtor(((obj)));

                      break;
                    }
#endif
#ifdef COMPILE_504
                    case 27: {
                      // AddAndReplaceKind
                      _zval_dtor_func((obj));

                      break;
                    }
#endif
#ifdef COMPILE_505
                    case 28: {
                      // AddAndReplaceKind
                      _zval_ptr_dtor((&(closure)->this_ptr));

                      break;
                    }
#endif
#ifdef COMPILE_506
                    case 29: {
                      // AddAndReplaceKind
                      _zval_ptr_dtor((&(obj)));

                      break;
                    }
#endif
#ifdef COMPILE_507
                    case 30: {
                      // AddAndReplaceKind
                      closure =
                          (zend_closure *)zend_object_store_get_object(obj);

                      break;
                    }
#endif
#ifdef COMPILE_508
                    case 31: {
                      // AddAndReplaceKind
                      closure_handlers.clone_obj = zend_closure_clone;

                      break;
                    }
#endif
#ifdef COMPILE_509
                    case 32: {
                      // AddAndReplaceKind
                      closure_handlers.compare_objects =
                          zend_closure_compare_objects;

                      break;
                    }
#endif
#ifdef COMPILE_510
                    case 33: {
                      // AddAndReplaceKind
                      closure_handlers.get_closure = zend_closure_get_closure;

                      break;
                    }
#endif
#ifdef COMPILE_511
                    case 34: {
                      // AddAndReplaceKind
                      closure_handlers.get_constructor =
                          zend_closure_get_constructor;

                      break;
                    }
#endif
#ifdef COMPILE_512
                    case 35: {
                      // AddAndReplaceKind
                      closure_handlers.get_debug_info =
                          zend_closure_get_debug_info;

                      break;
                    }
#endif
#ifdef COMPILE_513
                    case 36: {
                      // AddAndReplaceKind
                      closure_handlers.get_method = zend_closure_get_method;

                      break;
                    }
#endif
#ifdef COMPILE_514
                    case 37: {
                      // AddAndReplaceKind
                      closure_handlers.get_property_ptr_ptr =
                          zend_closure_get_property_ptr_ptr;

                      break;
                    }
#endif
#ifdef COMPILE_515
                    case 38: {
                      // AddAndReplaceKind
                      closure_handlers.has_property = zend_closure_has_property;

                      break;
                    }
#endif
#ifdef COMPILE_516
                    case 39: {
                      // AddAndReplaceKind
                      closure_handlers.read_property =
                          zend_closure_read_property;

                      break;
                    }
#endif
#ifdef COMPILE_517
                    case 40: {
                      // AddAndReplaceKind
                      closure_handlers.unset_property =
                          zend_closure_unset_property;

                      break;
                    }
#endif
#ifdef COMPILE_518
                    case 41: {
                      // AddAndReplaceKind
                      closure_handlers.write_property =
                          zend_closure_write_property;

                      break;
                    }
#endif
#ifdef COMPILE_519
                    case 42: {
                      // AddAndReplaceKind
                      convert_to_boolean((obj));

                      break;
                    }
#endif
#ifdef COMPILE_520
                    case 43: {
                      // AddAndReplaceKind
                      destroy_op_array(&(closure)->func.op_array);

                      break;
                    }
#endif
#ifdef COMPILE_521
                    case 44: {
                      // AddAndReplaceKind
                      exit(1);

                      break;
                    }
#endif
#ifdef COMPILE_522
                    case 45: {
                      // AddAndReplaceKind
                      gc_remove_zval_from_buffer((obj));

                      break;
                    }
#endif
#ifdef COMPILE_523
                    case 46: {
                      // AddAndReplaceKind
                      gc_zval_check_possible_root(((obj)));

                      break;
                    }
#endif
#ifdef COMPILE_524
                    case 47: {
                      // AddAndReplaceKind
                      gc_zval_possible_root((obj));

                      break;
                    }
#endif
#ifdef COMPILE_525
                    case 48: {
                      // AddAndReplaceKind
                      i_zval_ptr_dtor((obj));

                      break;
                    }
#endif
#ifdef COMPILE_526
                    case 49: {
                      // AddAndReplaceKind
                      if ((*obj).type != 5) {
                        return -1;
                      }

                      break;
                    }
#endif
#ifdef COMPILE_527
                    case 50: {
                      // AddAndReplaceKind
                      if (zobj_ptr) {
                        *zobj_ptr = ((void *)0);
                      }

                      break;
                    }
#endif
#ifdef COMPILE_528
                    case 51: {
                      // AddAndReplaceKind
                      if (zobj_ptr) {
                        *zobj_ptr = closure->this_ptr;
                      }

                      break;
                    }
#endif
#ifdef COMPILE_529
                    case 52: {
                      // AddAndReplaceKind
                      memcpy(&closure_handlers, zend_get_std_object_handlers(),
                             sizeof(zend_object_handlers));

                      break;
                    }
#endif
#ifdef COMPILE_530
                    case 53: {
                      // AddAndReplaceKind
                      memset((closure), 0, sizeof(zend_closure));

                      break;
                    }
#endif
#ifdef COMPILE_531
                    case 54: {
                      // AddAndReplaceKind
                      zend_ce_closure->create_object = zend_closure_new;

                      break;
                    }
#endif
#ifdef COMPILE_532
                    case 55: {
                      // AddAndReplaceKind
                      zend_ce_closure->serialize = zend_class_serialize_deny;

                      break;
                    }
#endif
#ifdef COMPILE_533
                    case 56: {
                      // AddAndReplaceKind
                      zend_ce_closure->unserialize =
                          zend_class_unserialize_deny;

                      break;
                    }
#endif
#ifdef COMPILE_534
                    case 57: {
                      // AddAndReplaceKind
                      zend_error((1 << 0L),
                                 "Cannot destroy active lambda function");

                      break;
                    }
#endif
#ifdef COMPILE_535
                    case 58: {
                      // AddAndReplaceKind
                      zend_error((1 << 12L),
                                 "Cannot get arguments for calling closure");

                      break;
                    }
#endif
#ifdef COMPILE_536
                    case 59: {
                      // AddAndReplaceKind
                      zend_error((1 << 12L),
                                 "Closure object cannot have properties");

                      break;
                    }
#endif
#ifdef COMPILE_537
                    case 60: {
                      // AddAndReplaceKind
                      zend_error((1 << 12L),
                                 "Instantiation of 'Closure' is not allowed");

                      break;
                    }
#endif
#ifdef COMPILE_538
                    case 61: {
                      // AddAndReplaceKind
                      zend_error((1 << 3L),
                                 "A non well formed numeric value encountered");

                      break;
                    }
#endif
#ifdef COMPILE_539
                    case 62: {
                      // AddAndReplaceKind
                      zend_hash_destroy((closure)->debug_info);

                      break;
                    }
#endif
#ifdef COMPILE_540
                    case 63: {
                      // AddAndReplaceKind
                      zend_object_std_dtor(&(closure)->std);

                      break;
                    }
#endif
#ifdef COMPILE_541
                    case 64: {
                      // AddAndReplaceKind
                      zval_addref_p(&((executor_globals.uninitialized_zval)));

                      break;
                    }
#endif
#ifdef COMPILE_542
                    case 65: {
                      // AddAndReplaceKind
                      zval_addref_p((closure)->this_ptr);

                      break;
                    }
#endif
#ifdef COMPILE_543
                    case 66: {
                      // AddAndReplaceKind
                      zval_addref_p((obj));

                      break;
                    }
#endif
#ifdef COMPILE_544
                    case 67: {
                      // AddAndReplaceKind
                      zval_unset_isref_p((obj));

                      break;
                    }
#endif
#ifdef COMPILE_545
                    case 68: {
                      // IfExitKind
                      if (__is_neg("16-68", 5, &(zobj_ptr), sizeof(zobj_ptr),
                                   &(ce_ptr), sizeof(ce_ptr), &(closure),
                                   sizeof(closure), &(fptr_ptr),
                                   sizeof(fptr_ptr), &(obj), sizeof(obj)))
                        return 0;

                      break;
                    }
#endif
#ifdef COMPILE_546
                    case 69: {
                      // IfExitKind
                      if (__is_neg("16-69", 5, &(zobj_ptr), sizeof(zobj_ptr),
                                   &(ce_ptr), sizeof(ce_ptr), &(closure),
                                   sizeof(closure), &(fptr_ptr),
                                   sizeof(fptr_ptr), &(obj), sizeof(obj)))
                        return 1;

                      break;
                    }
#endif
#ifdef COMPILE_547
                    case 70: {
                      // IfExitKind
                      if (__is_neg("16-70", 5, &(zobj_ptr), sizeof(zobj_ptr),
                                   &(ce_ptr), sizeof(ce_ptr), &(closure),
                                   sizeof(closure), &(fptr_ptr),
                                   sizeof(fptr_ptr), &(obj), sizeof(obj)))
                        return 5;

                      break;
                    }
#endif
                    }
                    switch (__choose("__SWITCH17")) {
                    case 0: {
                      *zobj_ptr = NULL;

                      break;
                    }
#ifdef COMPILE_548
                    case 1: {
                      // GuardKind
                      if (!__is_neg("17-1", 5, &(zobj_ptr), sizeof(zobj_ptr),
                                    &(ce_ptr), sizeof(ce_ptr), &(closure),
                                    sizeof(closure), &(fptr_ptr),
                                    sizeof(fptr_ptr), &(obj), sizeof(obj)))
                        *zobj_ptr = ((void *)0);

                      break;
                    }
#endif
#ifdef COMPILE_549
                    case 2: {
                      // ReplaceKind
                      *(zobj_ptr) = ((void *)0);

                      break;
                    }
#endif
                    }
                  }
                }

                break;
              }
#ifdef COMPILE_550
              case 1: {
                // GuardKind
                if (!__is_neg("18-1", 5, &(zobj_ptr), sizeof(zobj_ptr),
                              &(ce_ptr), sizeof(ce_ptr), &(closure),
                              sizeof(closure), &(fptr_ptr), sizeof(fptr_ptr),
                              &(obj), sizeof(obj)))
                  *zobj_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_551
              case 2: {
                // ReplaceKind
                *(zobj_ptr) = ((void *)0);

                break;
              }
#endif
              }
            }
          }
          {
            switch (__choose("__SWITCH19")) {
            case 0:
              break;
#ifdef COMPILE_552
            case 1: {
              // AddAndReplaceKind
              (*(closure)->func.op_array.refcount)++;

              break;
            }
#endif
#ifdef COMPILE_553
            case 2: {
              // AddAndReplaceKind
              (closure) = (zend_closure *)zend_object_store_get_object(obj);

              break;
            }
#endif
#ifdef COMPILE_554
            case 3: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_555
            case 4: {
              // AddAndReplaceKind
              *(ce_ptr) = closure->func.common.scope;

              break;
            }
#endif
#ifdef COMPILE_556
            case 5: {
              // AddAndReplaceKind
              *(ce_ptr) = zend_get_class_entry(&(*closure->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_557
            case 6: {
              // AddAndReplaceKind
              *(fptr_ptr) = &closure->func;

              break;
            }
#endif
#ifdef COMPILE_558
            case 7: {
              // AddAndReplaceKind
              *(zobj_ptr) = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_559
            case 8: {
              // AddAndReplaceKind
              *(zobj_ptr) = closure->this_ptr;

              break;
            }
#endif
#ifdef COMPILE_560
            case 9: {
              // AddAndReplaceKind
              *ce_ptr = (closure)->func.common.scope;

              break;
            }
#endif
#ifdef COMPILE_561
            case 10: {
              // AddAndReplaceKind
              *ce_ptr = closure->func.common.scope;

              break;
            }
#endif
#ifdef COMPILE_562
            case 11: {
              // AddAndReplaceKind
              *ce_ptr = zend_get_class_entry(&(*(closure)->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_563
            case 12: {
              // AddAndReplaceKind
              *ce_ptr = zend_get_class_entry(&(*closure->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_564
            case 13: {
              // AddAndReplaceKind
              *fptr_ptr = &(closure)->func;

              break;
            }
#endif
#ifdef COMPILE_565
            case 14: {
              // AddAndReplaceKind
              *fptr_ptr = &closure->func;

              break;
            }
#endif
#ifdef COMPILE_566
            case 15: {
              // AddAndReplaceKind
              *zobj_ptr = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_567
            case 16: {
              // AddAndReplaceKind
              *zobj_ptr = (closure)->this_ptr;

              break;
            }
#endif
#ifdef COMPILE_568
            case 17: {
              // AddAndReplaceKind
              *zobj_ptr = closure->this_ptr;

              break;
            }
#endif
#ifdef COMPILE_569
            case 18: {
              // AddAndReplaceKind
              _array_init(((obj)), 0);

              break;
            }
#endif
#ifdef COMPILE_570
            case 19: {
              // AddAndReplaceKind
              _efree(((closure)));

              break;
            }
#endif
#ifdef COMPILE_571
            case 20: {
              // AddAndReplaceKind
              _efree(((closure)->debug_info));

              break;
            }
#endif
#ifdef COMPILE_572
            case 21: {
              // AddAndReplaceKind
              _efree(((obj)));

              break;
            }
#endif
#ifdef COMPILE_573
            case 22: {
              // AddAndReplaceKind
              _object_init_ex(((obj)), (zend_ce_closure));

              break;
            }
#endif
#ifdef COMPILE_574
            case 23: {
              // AddAndReplaceKind
              _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_575
            case 24: {
              // AddAndReplaceKind
              _zval_copy_ctor(((obj)));

              break;
            }
#endif
#ifdef COMPILE_576
            case 25: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((obj));

              break;
            }
#endif
#ifdef COMPILE_577
            case 26: {
              // AddAndReplaceKind
              _zval_dtor(((obj)));

              break;
            }
#endif
#ifdef COMPILE_578
            case 27: {
              // AddAndReplaceKind
              _zval_dtor_func((obj));

              break;
            }
#endif
#ifdef COMPILE_579
            case 28: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(closure)->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_580
            case 29: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(obj)));

              break;
            }
#endif
#ifdef COMPILE_581
            case 30: {
              // AddAndReplaceKind
              closure = (zend_closure *)zend_object_store_get_object(obj);

              break;
            }
#endif
#ifdef COMPILE_582
            case 31: {
              // AddAndReplaceKind
              closure_handlers.clone_obj = zend_closure_clone;

              break;
            }
#endif
#ifdef COMPILE_583
            case 32: {
              // AddAndReplaceKind
              closure_handlers.compare_objects = zend_closure_compare_objects;

              break;
            }
#endif
#ifdef COMPILE_584
            case 33: {
              // AddAndReplaceKind
              closure_handlers.get_closure = zend_closure_get_closure;

              break;
            }
#endif
#ifdef COMPILE_585
            case 34: {
              // AddAndReplaceKind
              closure_handlers.get_constructor = zend_closure_get_constructor;

              break;
            }
#endif
#ifdef COMPILE_586
            case 35: {
              // AddAndReplaceKind
              closure_handlers.get_debug_info = zend_closure_get_debug_info;

              break;
            }
#endif
#ifdef COMPILE_587
            case 36: {
              // AddAndReplaceKind
              closure_handlers.get_method = zend_closure_get_method;

              break;
            }
#endif
#ifdef COMPILE_588
            case 37: {
              // AddAndReplaceKind
              closure_handlers.get_property_ptr_ptr =
                  zend_closure_get_property_ptr_ptr;

              break;
            }
#endif
#ifdef COMPILE_589
            case 38: {
              // AddAndReplaceKind
              closure_handlers.has_property = zend_closure_has_property;

              break;
            }
#endif
#ifdef COMPILE_590
            case 39: {
              // AddAndReplaceKind
              closure_handlers.read_property = zend_closure_read_property;

              break;
            }
#endif
#ifdef COMPILE_591
            case 40: {
              // AddAndReplaceKind
              closure_handlers.unset_property = zend_closure_unset_property;

              break;
            }
#endif
#ifdef COMPILE_592
            case 41: {
              // AddAndReplaceKind
              closure_handlers.write_property = zend_closure_write_property;

              break;
            }
#endif
#ifdef COMPILE_593
            case 42: {
              // AddAndReplaceKind
              convert_to_boolean((obj));

              break;
            }
#endif
#ifdef COMPILE_594
            case 43: {
              // AddAndReplaceKind
              destroy_op_array(&(closure)->func.op_array);

              break;
            }
#endif
#ifdef COMPILE_595
            case 44: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_596
            case 45: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((obj));

              break;
            }
#endif
#ifdef COMPILE_597
            case 46: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((obj)));

              break;
            }
#endif
#ifdef COMPILE_598
            case 47: {
              // AddAndReplaceKind
              gc_zval_possible_root((obj));

              break;
            }
#endif
#ifdef COMPILE_599
            case 48: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((obj));

              break;
            }
#endif
#ifdef COMPILE_600
            case 49: {
              // AddAndReplaceKind
              if ((*obj).type != 5) {
                return -1;
              }

              break;
            }
#endif
#ifdef COMPILE_601
            case 50: {
              // AddAndReplaceKind
              if (zobj_ptr) {
                *zobj_ptr = ((void *)0);
              }

              break;
            }
#endif
#ifdef COMPILE_602
            case 51: {
              // AddAndReplaceKind
              if (zobj_ptr) {
                *zobj_ptr = closure->this_ptr;
              }

              break;
            }
#endif
#ifdef COMPILE_603
            case 52: {
              // AddAndReplaceKind
              memcpy(&closure_handlers, zend_get_std_object_handlers(),
                     sizeof(zend_object_handlers));

              break;
            }
#endif
#ifdef COMPILE_604
            case 53: {
              // AddAndReplaceKind
              memset((closure), 0, sizeof(zend_closure));

              break;
            }
#endif
#ifdef COMPILE_605
            case 54: {
              // AddAndReplaceKind
              zend_ce_closure->create_object = zend_closure_new;

              break;
            }
#endif
#ifdef COMPILE_606
            case 55: {
              // AddAndReplaceKind
              zend_ce_closure->serialize = zend_class_serialize_deny;

              break;
            }
#endif
#ifdef COMPILE_607
            case 56: {
              // AddAndReplaceKind
              zend_ce_closure->unserialize = zend_class_unserialize_deny;

              break;
            }
#endif
#ifdef COMPILE_608
            case 57: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Cannot destroy active lambda function");

              break;
            }
#endif
#ifdef COMPILE_609
            case 58: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Cannot get arguments for calling closure");

              break;
            }
#endif
#ifdef COMPILE_610
            case 59: {
              // AddAndReplaceKind
              zend_error((1 << 12L), "Closure object cannot have properties");

              break;
            }
#endif
#ifdef COMPILE_611
            case 60: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Instantiation of 'Closure' is not allowed");

              break;
            }
#endif
#ifdef COMPILE_612
            case 61: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_613
            case 62: {
              // AddAndReplaceKind
              zend_hash_destroy((closure)->debug_info);

              break;
            }
#endif
#ifdef COMPILE_614
            case 63: {
              // AddAndReplaceKind
              zend_object_std_dtor(&(closure)->std);

              break;
            }
#endif
#ifdef COMPILE_615
            case 64: {
              // AddAndReplaceKind
              zval_addref_p(&((executor_globals.uninitialized_zval)));

              break;
            }
#endif
#ifdef COMPILE_616
            case 65: {
              // AddAndReplaceKind
              zval_addref_p((closure)->this_ptr);

              break;
            }
#endif
#ifdef COMPILE_617
            case 66: {
              // AddAndReplaceKind
              zval_addref_p((obj));

              break;
            }
#endif
#ifdef COMPILE_618
            case 67: {
              // AddAndReplaceKind
              zval_unset_isref_p((obj));

              break;
            }
#endif
#ifdef COMPILE_619
            case 68: {
              // AddInitKind
              memset(&closure->func, 0, sizeof(*(&closure->func)));

              break;
            }
#endif
#ifdef COMPILE_620
            case 69: {
              // AddInitKind
              memset(&closure->func.common, 0,
                     sizeof(*(&closure->func.common)));

              break;
            }
#endif
#ifdef COMPILE_621
            case 70: {
              // AddInitKind
              memset(closure, 0, sizeof(*(closure)));

              break;
            }
#endif
#ifdef COMPILE_622
            case 71: {
              // IfExitKind
              if (__is_neg("19-71", 15, &(ce_ptr), sizeof(ce_ptr), &(closure),
                           sizeof(closure), &(closure->func.common.scope),
                           sizeof(closure->func.common.scope), &(zobj_ptr),
                           sizeof(zobj_ptr), &(closure->this_ptr),
                           sizeof(closure->this_ptr), &(fptr_ptr),
                           sizeof(fptr_ptr), &(obj), sizeof(obj),
                           &(closure->debug_info), sizeof(closure->debug_info),
                           &(closure->func.common.type),
                           sizeof(closure->func.common.type),
                           &(closure->func.common.function_name),
                           sizeof(closure->func.common.function_name),
                           &(closure->func.common.fn_flags),
                           sizeof(closure->func.common.fn_flags),
                           &(closure->func.common.prototype),
                           sizeof(closure->func.common.prototype),
                           &(closure->func.common.num_args),
                           sizeof(closure->func.common.num_args),
                           &(closure->func.common.required_num_args),
                           sizeof(closure->func.common.required_num_args),
                           &(closure->func.common.arg_info),
                           sizeof(closure->func.common.arg_info)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_623
            case 72: {
              // IfExitKind
              if (__is_neg("19-72", 15, &(ce_ptr), sizeof(ce_ptr), &(closure),
                           sizeof(closure), &(closure->func.common.scope),
                           sizeof(closure->func.common.scope), &(zobj_ptr),
                           sizeof(zobj_ptr), &(closure->this_ptr),
                           sizeof(closure->this_ptr), &(fptr_ptr),
                           sizeof(fptr_ptr), &(obj), sizeof(obj),
                           &(closure->debug_info), sizeof(closure->debug_info),
                           &(closure->func.common.type),
                           sizeof(closure->func.common.type),
                           &(closure->func.common.function_name),
                           sizeof(closure->func.common.function_name),
                           &(closure->func.common.fn_flags),
                           sizeof(closure->func.common.fn_flags),
                           &(closure->func.common.prototype),
                           sizeof(closure->func.common.prototype),
                           &(closure->func.common.num_args),
                           sizeof(closure->func.common.num_args),
                           &(closure->func.common.required_num_args),
                           sizeof(closure->func.common.required_num_args),
                           &(closure->func.common.arg_info),
                           sizeof(closure->func.common.arg_info)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_624
            case 73: {
              // IfExitKind
              if (__is_neg("19-73", 15, &(ce_ptr), sizeof(ce_ptr), &(closure),
                           sizeof(closure), &(closure->func.common.scope),
                           sizeof(closure->func.common.scope), &(zobj_ptr),
                           sizeof(zobj_ptr), &(closure->this_ptr),
                           sizeof(closure->this_ptr), &(fptr_ptr),
                           sizeof(fptr_ptr), &(obj), sizeof(obj),
                           &(closure->debug_info), sizeof(closure->debug_info),
                           &(closure->func.common.type),
                           sizeof(closure->func.common.type),
                           &(closure->func.common.function_name),
                           sizeof(closure->func.common.function_name),
                           &(closure->func.common.fn_flags),
                           sizeof(closure->func.common.fn_flags),
                           &(closure->func.common.prototype),
                           sizeof(closure->func.common.prototype),
                           &(closure->func.common.num_args),
                           sizeof(closure->func.common.num_args),
                           &(closure->func.common.required_num_args),
                           sizeof(closure->func.common.required_num_args),
                           &(closure->func.common.arg_info),
                           sizeof(closure->func.common.arg_info)))
                return 5;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH20")) {
            case 0: {
              *ce_ptr = closure->func.common.scope;

              break;
            }
#ifdef COMPILE_625
            case 1: {
              // GuardKind
              if (!__is_neg("20-1", 15, &(ce_ptr), sizeof(ce_ptr), &(closure),
                            sizeof(closure), &(closure->func.common.scope),
                            sizeof(closure->func.common.scope), &(zobj_ptr),
                            sizeof(zobj_ptr), &(closure->this_ptr),
                            sizeof(closure->this_ptr), &(fptr_ptr),
                            sizeof(fptr_ptr), &(obj), sizeof(obj),
                            &(closure->debug_info), sizeof(closure->debug_info),
                            &(closure->func.common.type),
                            sizeof(closure->func.common.type),
                            &(closure->func.common.function_name),
                            sizeof(closure->func.common.function_name),
                            &(closure->func.common.fn_flags),
                            sizeof(closure->func.common.fn_flags),
                            &(closure->func.common.prototype),
                            sizeof(closure->func.common.prototype),
                            &(closure->func.common.num_args),
                            sizeof(closure->func.common.num_args),
                            &(closure->func.common.required_num_args),
                            sizeof(closure->func.common.required_num_args),
                            &(closure->func.common.arg_info),
                            sizeof(closure->func.common.arg_info)))
                *ce_ptr = closure->func.common.scope;

              break;
            }
#endif
#ifdef COMPILE_626
            case 2: {
              // ReplaceKind
              *(ce_ptr) = closure->func.common.scope;

              break;
            }
#endif
#ifdef COMPILE_627
            case 3: {
              // ReplaceKind
              *ce_ptr = (closure)->func.common.scope;

              break;
            }
#endif
            }
          }
        }

        break;
      }
#ifdef COMPILE_628
      case 1: {
        // GuardKind
        if (!__is_neg("21-1", 15, &(ce_ptr), sizeof(ce_ptr), &(closure),
                      sizeof(closure), &(closure->func.common.scope),
                      sizeof(closure->func.common.scope), &(zobj_ptr),
                      sizeof(zobj_ptr), &(closure->this_ptr),
                      sizeof(closure->this_ptr), &(fptr_ptr), sizeof(fptr_ptr),
                      &(obj), sizeof(obj), &(closure->debug_info),
                      sizeof(closure->debug_info), &(closure->func.common.type),
                      sizeof(closure->func.common.type),
                      &(closure->func.common.function_name),
                      sizeof(closure->func.common.function_name),
                      &(closure->func.common.fn_flags),
                      sizeof(closure->func.common.fn_flags),
                      &(closure->func.common.prototype),
                      sizeof(closure->func.common.prototype),
                      &(closure->func.common.num_args),
                      sizeof(closure->func.common.num_args),
                      &(closure->func.common.required_num_args),
                      sizeof(closure->func.common.required_num_args),
                      &(closure->func.common.arg_info),
                      sizeof(closure->func.common.arg_info)))
          *ce_ptr = closure->func.common.scope;

        break;
      }
#endif
#ifdef COMPILE_629
      case 2: {
        // ReplaceKind
        *(ce_ptr) = closure->func.common.scope;

        break;
      }
#endif
#ifdef COMPILE_630
      case 3: {
        // ReplaceKind
        *ce_ptr = (closure)->func.common.scope;

        break;
      }
#endif
      }
    }
  }
  // prophet generated patch
  {
    switch (__choose("__SWITCH22")) {
    case 0:
      break;
#ifdef COMPILE_631
    case 1: {
      // AddAndReplaceKind
      (*(closure)->func.op_array.refcount)++;

      break;
    }
#endif
#ifdef COMPILE_632
    case 2: {
      // AddAndReplaceKind
      (closure) = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_633
    case 3: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_634
    case 4: {
      // AddAndReplaceKind
      *(ce_ptr) = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_635
    case 5: {
      // AddAndReplaceKind
      *(ce_ptr) = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_636
    case 6: {
      // AddAndReplaceKind
      *(fptr_ptr) = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_637
    case 7: {
      // AddAndReplaceKind
      *(zobj_ptr) = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_638
    case 8: {
      // AddAndReplaceKind
      *(zobj_ptr) = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_639
    case 9: {
      // AddAndReplaceKind
      *ce_ptr = (closure)->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_640
    case 10: {
      // AddAndReplaceKind
      *ce_ptr = closure->func.common.scope;

      break;
    }
#endif
#ifdef COMPILE_641
    case 11: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_642
    case 12: {
      // AddAndReplaceKind
      *ce_ptr = zend_get_class_entry(&(*closure->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_643
    case 13: {
      // AddAndReplaceKind
      *fptr_ptr = &(closure)->func;

      break;
    }
#endif
#ifdef COMPILE_644
    case 14: {
      // AddAndReplaceKind
      *fptr_ptr = &closure->func;

      break;
    }
#endif
#ifdef COMPILE_645
    case 15: {
      // AddAndReplaceKind
      *zobj_ptr = ((void *)0);

      break;
    }
#endif
#ifdef COMPILE_646
    case 16: {
      // AddAndReplaceKind
      *zobj_ptr = (closure)->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_647
    case 17: {
      // AddAndReplaceKind
      *zobj_ptr = closure->this_ptr;

      break;
    }
#endif
#ifdef COMPILE_648
    case 18: {
      // AddAndReplaceKind
      _array_init(((obj)), 0);

      break;
    }
#endif
#ifdef COMPILE_649
    case 19: {
      // AddAndReplaceKind
      _efree(((closure)));

      break;
    }
#endif
#ifdef COMPILE_650
    case 20: {
      // AddAndReplaceKind
      _efree(((closure)->debug_info));

      break;
    }
#endif
#ifdef COMPILE_651
    case 21: {
      // AddAndReplaceKind
      _efree(((obj)));

      break;
    }
#endif
#ifdef COMPILE_652
    case 22: {
      // AddAndReplaceKind
      _object_init_ex(((obj)), (zend_ce_closure));

      break;
    }
#endif
#ifdef COMPILE_653
    case 23: {
      // AddAndReplaceKind
      _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_654
    case 24: {
      // AddAndReplaceKind
      _zval_copy_ctor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_655
    case 25: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_656
    case 26: {
      // AddAndReplaceKind
      _zval_dtor(((obj)));

      break;
    }
#endif
#ifdef COMPILE_657
    case 27: {
      // AddAndReplaceKind
      _zval_dtor_func((obj));

      break;
    }
#endif
#ifdef COMPILE_658
    case 28: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(closure)->this_ptr));

      break;
    }
#endif
#ifdef COMPILE_659
    case 29: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(obj)));

      break;
    }
#endif
#ifdef COMPILE_660
    case 30: {
      // AddAndReplaceKind
      closure = (zend_closure *)zend_object_store_get_object(obj);

      break;
    }
#endif
#ifdef COMPILE_661
    case 31: {
      // AddAndReplaceKind
      closure_handlers.clone_obj = zend_closure_clone;

      break;
    }
#endif
#ifdef COMPILE_662
    case 32: {
      // AddAndReplaceKind
      closure_handlers.compare_objects = zend_closure_compare_objects;

      break;
    }
#endif
#ifdef COMPILE_663
    case 33: {
      // AddAndReplaceKind
      closure_handlers.get_closure = zend_closure_get_closure;

      break;
    }
#endif
#ifdef COMPILE_664
    case 34: {
      // AddAndReplaceKind
      closure_handlers.get_constructor = zend_closure_get_constructor;

      break;
    }
#endif
#ifdef COMPILE_665
    case 35: {
      // AddAndReplaceKind
      closure_handlers.get_debug_info = zend_closure_get_debug_info;

      break;
    }
#endif
#ifdef COMPILE_666
    case 36: {
      // AddAndReplaceKind
      closure_handlers.get_method = zend_closure_get_method;

      break;
    }
#endif
#ifdef COMPILE_667
    case 37: {
      // AddAndReplaceKind
      closure_handlers.get_property_ptr_ptr = zend_closure_get_property_ptr_ptr;

      break;
    }
#endif
#ifdef COMPILE_668
    case 38: {
      // AddAndReplaceKind
      closure_handlers.has_property = zend_closure_has_property;

      break;
    }
#endif
#ifdef COMPILE_669
    case 39: {
      // AddAndReplaceKind
      closure_handlers.read_property = zend_closure_read_property;

      break;
    }
#endif
#ifdef COMPILE_670
    case 40: {
      // AddAndReplaceKind
      closure_handlers.unset_property = zend_closure_unset_property;

      break;
    }
#endif
#ifdef COMPILE_671
    case 41: {
      // AddAndReplaceKind
      closure_handlers.write_property = zend_closure_write_property;

      break;
    }
#endif
#ifdef COMPILE_672
    case 42: {
      // AddAndReplaceKind
      convert_to_boolean((obj));

      break;
    }
#endif
#ifdef COMPILE_673
    case 43: {
      // AddAndReplaceKind
      destroy_op_array(&(closure)->func.op_array);

      break;
    }
#endif
#ifdef COMPILE_674
    case 44: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_675
    case 45: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((obj));

      break;
    }
#endif
#ifdef COMPILE_676
    case 46: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((obj)));

      break;
    }
#endif
#ifdef COMPILE_677
    case 47: {
      // AddAndReplaceKind
      gc_zval_possible_root((obj));

      break;
    }
#endif
#ifdef COMPILE_678
    case 48: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((obj));

      break;
    }
#endif
#ifdef COMPILE_679
    case 49: {
      // AddAndReplaceKind
      if ((*obj).type != 5) {
        return -1;
      }

      break;
    }
#endif
#ifdef COMPILE_680
    case 50: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = ((void *)0);
      }

      break;
    }
#endif
#ifdef COMPILE_681
    case 51: {
      // AddAndReplaceKind
      if (zobj_ptr) {
        *zobj_ptr = closure->this_ptr;
      }

      break;
    }
#endif
#ifdef COMPILE_682
    case 52: {
      // AddAndReplaceKind
      memcpy(&closure_handlers, zend_get_std_object_handlers(),
             sizeof(zend_object_handlers));

      break;
    }
#endif
#ifdef COMPILE_683
    case 53: {
      // AddAndReplaceKind
      memset((closure), 0, sizeof(zend_closure));

      break;
    }
#endif
#ifdef COMPILE_684
    case 54: {
      // AddAndReplaceKind
      zend_ce_closure->create_object = zend_closure_new;

      break;
    }
#endif
#ifdef COMPILE_685
    case 55: {
      // AddAndReplaceKind
      zend_ce_closure->serialize = zend_class_serialize_deny;

      break;
    }
#endif
#ifdef COMPILE_686
    case 56: {
      // AddAndReplaceKind
      zend_ce_closure->unserialize = zend_class_unserialize_deny;

      break;
    }
#endif
#ifdef COMPILE_687
    case 57: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Cannot destroy active lambda function");

      break;
    }
#endif
#ifdef COMPILE_688
    case 58: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Cannot get arguments for calling closure");

      break;
    }
#endif
#ifdef COMPILE_689
    case 59: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Closure object cannot have properties");

      break;
    }
#endif
#ifdef COMPILE_690
    case 60: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");

      break;
    }
#endif
#ifdef COMPILE_691
    case 61: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_692
    case 62: {
      // AddAndReplaceKind
      zend_hash_destroy((closure)->debug_info);

      break;
    }
#endif
#ifdef COMPILE_693
    case 63: {
      // AddAndReplaceKind
      zend_object_std_dtor(&(closure)->std);

      break;
    }
#endif
#ifdef COMPILE_694
    case 64: {
      // AddAndReplaceKind
      zval_addref_p(&((executor_globals.uninitialized_zval)));

      break;
    }
#endif
#ifdef COMPILE_695
    case 65: {
      // AddAndReplaceKind
      zval_addref_p((closure)->this_ptr);

      break;
    }
#endif
#ifdef COMPILE_696
    case 66: {
      // AddAndReplaceKind
      zval_addref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_697
    case 67: {
      // AddAndReplaceKind
      zval_unset_isref_p((obj));

      break;
    }
#endif
#ifdef COMPILE_698
    case 68: {
      // IfExitKind
      if (__is_neg("22-68", 5, &(ce_ptr), sizeof(ce_ptr), &(closure),
                   sizeof(closure), &(zobj_ptr), sizeof(zobj_ptr), &(fptr_ptr),
                   sizeof(fptr_ptr), &(obj), sizeof(obj)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_699
    case 69: {
      // IfExitKind
      if (__is_neg("22-69", 5, &(ce_ptr), sizeof(ce_ptr), &(closure),
                   sizeof(closure), &(zobj_ptr), sizeof(zobj_ptr), &(fptr_ptr),
                   sizeof(fptr_ptr), &(obj), sizeof(obj)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_700
    case 70: {
      // IfExitKind
      if (__is_neg("22-70", 5, &(ce_ptr), sizeof(ce_ptr), &(closure),
                   sizeof(closure), &(zobj_ptr), sizeof(zobj_ptr), &(fptr_ptr),
                   sizeof(fptr_ptr), &(obj), sizeof(obj)))
        return 5;

      break;
    }
#endif
    }
    switch (__choose("__SWITCH23")) {
    case 0: {
      return SUCCESS;

      break;
    }
#ifdef COMPILE_701
    case 1: {
      // GuardKind
      if (!__is_neg("23-1", 5, &(ce_ptr), sizeof(ce_ptr), &(closure),
                    sizeof(closure), &(zobj_ptr), sizeof(zobj_ptr), &(fptr_ptr),
                    sizeof(fptr_ptr), &(obj), sizeof(obj)))
        return 0;

      break;
    }
#endif
    }
  }
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
    // prophet generated patch
    {
      switch (__choose("__SWITCH24")) {
      case 0:
        break;
#ifdef COMPILE_702
      case 1: {
        // AddAndReplaceKind
        ((closure)->func.op_array.static_variables) =
            (HashTable *)_emalloc((sizeof(HashTable)));

        break;
      }
#endif
#ifdef COMPILE_703
      case 2: {
        // AddAndReplaceKind
        (*(closure)->func.op_array.refcount)++;

        break;
      }
#endif
#ifdef COMPILE_704
      case 3: {
        // AddAndReplaceKind
        (*closure->func.op_array.refcount)++;

        break;
      }
#endif
#ifdef COMPILE_705
      case 4: {
        // AddAndReplaceKind
        (closure) = (zend_closure *)zend_object_store_get_object(res);

        break;
      }
#endif
#ifdef COMPILE_706
      case 5: {
        // AddAndReplaceKind
        (closure)->func = *func;

        break;
      }
#endif
#ifdef COMPILE_707
      case 6: {
        // AddAndReplaceKind
        (closure)->func.common.scope = scope;

        break;
      }
#endif
#ifdef COMPILE_708
      case 7: {
        // AddAndReplaceKind
        (closure)->func.op_array.run_time_cache = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_709
      case 8: {
        // AddAndReplaceKind
        (closure)->this_ptr = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_710
      case 9: {
        // AddAndReplaceKind
        (closure)->this_ptr = this_ptr;

        break;
      }
#endif
#ifdef COMPILE_711
      case 10: {
        // AddAndReplaceKind
        (closure->func.op_array.static_variables) =
            (HashTable *)_emalloc((sizeof(HashTable)));

        break;
      }
#endif
#ifdef COMPILE_712
      case 11: {
        // AddAndReplaceKind
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));

        break;
      }
#endif
#ifdef COMPILE_713
      case 12: {
        // AddAndReplaceKind
        (res) = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_714
      case 13: {
        // AddAndReplaceKind
        (scope) = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_715
      case 14: {
        // AddAndReplaceKind
        (scope)->create_object = zend_closure_new;

        break;
      }
#endif
#ifdef COMPILE_716
      case 15: {
        // AddAndReplaceKind
        (scope)->serialize = zend_class_serialize_deny;

        break;
      }
#endif
#ifdef COMPILE_717
      case 16: {
        // AddAndReplaceKind
        (scope)->unserialize = zend_class_unserialize_deny;

        break;
      }
#endif
#ifdef COMPILE_718
      case 17: {
        // AddAndReplaceKind
        (this_ptr) = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_719
      case 18: {
        // AddAndReplaceKind
        _array_init(((res)), 0);

        break;
      }
#endif
#ifdef COMPILE_720
      case 19: {
        // AddAndReplaceKind
        _array_init(((this_ptr)), 0);

        break;
      }
#endif
#ifdef COMPILE_721
      case 20: {
        // AddAndReplaceKind
        _efree(((closure)));

        break;
      }
#endif
#ifdef COMPILE_722
      case 21: {
        // AddAndReplaceKind
        _efree(((closure)->debug_info));

        break;
      }
#endif
#ifdef COMPILE_723
      case 22: {
        // AddAndReplaceKind
        _efree(((func)));

        break;
      }
#endif
#ifdef COMPILE_724
      case 23: {
        // AddAndReplaceKind
        _efree(((func)->internal_function.function_name));

        break;
      }
#endif
#ifdef COMPILE_725
      case 24: {
        // AddAndReplaceKind
        _efree(((res)));

        break;
      }
#endif
#ifdef COMPILE_726
      case 25: {
        // AddAndReplaceKind
        _efree(((this_ptr)));

        break;
      }
#endif
#ifdef COMPILE_727
      case 26: {
        // AddAndReplaceKind
        _object_init_ex(((res)), (zend_ce_closure));

        break;
      }
#endif
#ifdef COMPILE_728
      case 27: {
        // AddAndReplaceKind
        _object_init_ex(((this_ptr)), (zend_ce_closure));

        break;
      }
#endif
#ifdef COMPILE_729
      case 28: {
        // AddAndReplaceKind
        _object_init_ex((res), ((scope)));

        break;
      }
#endif
#ifdef COMPILE_730
      case 29: {
        // AddAndReplaceKind
        _object_init_ex((res), (zend_ce_closure));

        break;
      }
#endif
#ifdef COMPILE_731
      case 30: {
        // AddAndReplaceKind
        _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                        ((void (*)(void *))_zval_ptr_dtor), (0));

        break;
      }
#endif
#ifdef COMPILE_732
      case 31: {
        // AddAndReplaceKind
        _zval_copy_ctor(((res)));

        break;
      }
#endif
#ifdef COMPILE_733
      case 32: {
        // AddAndReplaceKind
        _zval_copy_ctor(((this_ptr)));

        break;
      }
#endif
#ifdef COMPILE_734
      case 33: {
        // AddAndReplaceKind
        _zval_copy_ctor_func((res));

        break;
      }
#endif
#ifdef COMPILE_735
      case 34: {
        // AddAndReplaceKind
        _zval_copy_ctor_func((this_ptr));

        break;
      }
#endif
#ifdef COMPILE_736
      case 35: {
        // AddAndReplaceKind
        _zval_dtor(((res)));

        break;
      }
#endif
#ifdef COMPILE_737
      case 36: {
        // AddAndReplaceKind
        _zval_dtor(((this_ptr)));

        break;
      }
#endif
#ifdef COMPILE_738
      case 37: {
        // AddAndReplaceKind
        _zval_dtor_func((res));

        break;
      }
#endif
#ifdef COMPILE_739
      case 38: {
        // AddAndReplaceKind
        _zval_dtor_func((this_ptr));

        break;
      }
#endif
#ifdef COMPILE_740
      case 39: {
        // AddAndReplaceKind
        _zval_ptr_dtor((&(closure)->this_ptr));

        break;
      }
#endif
#ifdef COMPILE_741
      case 40: {
        // AddAndReplaceKind
        _zval_ptr_dtor((&(res)));

        break;
      }
#endif
#ifdef COMPILE_742
      case 41: {
        // AddAndReplaceKind
        _zval_ptr_dtor((&(this_ptr)));

        break;
      }
#endif
#ifdef COMPILE_743
      case 42: {
        // AddAndReplaceKind
        closure = (zend_closure *)zend_object_store_get_object(res);

        break;
      }
#endif
#ifdef COMPILE_744
      case 43: {
        // AddAndReplaceKind
        closure->func = *(func);

        break;
      }
#endif
#ifdef COMPILE_745
      case 44: {
        // AddAndReplaceKind
        closure->func = *func;

        break;
      }
#endif
#ifdef COMPILE_746
      case 45: {
        // AddAndReplaceKind
        closure->func.common.scope = (scope);

        break;
      }
#endif
#ifdef COMPILE_747
      case 46: {
        // AddAndReplaceKind
        closure->func.common.scope = scope;

        break;
      }
#endif
#ifdef COMPILE_748
      case 47: {
        // AddAndReplaceKind
        closure->func.op_array.run_time_cache = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_749
      case 48: {
        // AddAndReplaceKind
        closure->this_ptr = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_750
      case 49: {
        // AddAndReplaceKind
        closure->this_ptr = (res);

        break;
      }
#endif
#ifdef COMPILE_751
      case 50: {
        // AddAndReplaceKind
        closure->this_ptr = (this_ptr);

        break;
      }
#endif
#ifdef COMPILE_752
      case 51: {
        // AddAndReplaceKind
        closure->this_ptr = this_ptr;

        break;
      }
#endif
#ifdef COMPILE_753
      case 52: {
        // AddAndReplaceKind
        closure_handlers.clone_obj = zend_closure_clone;

        break;
      }
#endif
#ifdef COMPILE_754
      case 53: {
        // AddAndReplaceKind
        closure_handlers.compare_objects = zend_closure_compare_objects;

        break;
      }
#endif
#ifdef COMPILE_755
      case 54: {
        // AddAndReplaceKind
        closure_handlers.get_closure = zend_closure_get_closure;

        break;
      }
#endif
#ifdef COMPILE_756
      case 55: {
        // AddAndReplaceKind
        closure_handlers.get_constructor = zend_closure_get_constructor;

        break;
      }
#endif
#ifdef COMPILE_757
      case 56: {
        // AddAndReplaceKind
        closure_handlers.get_debug_info = zend_closure_get_debug_info;

        break;
      }
#endif
#ifdef COMPILE_758
      case 57: {
        // AddAndReplaceKind
        closure_handlers.get_method = zend_closure_get_method;

        break;
      }
#endif
#ifdef COMPILE_759
      case 58: {
        // AddAndReplaceKind
        closure_handlers.get_property_ptr_ptr =
            zend_closure_get_property_ptr_ptr;

        break;
      }
#endif
#ifdef COMPILE_760
      case 59: {
        // AddAndReplaceKind
        closure_handlers.has_property = zend_closure_has_property;

        break;
      }
#endif
#ifdef COMPILE_761
      case 60: {
        // AddAndReplaceKind
        closure_handlers.read_property = zend_closure_read_property;

        break;
      }
#endif
#ifdef COMPILE_762
      case 61: {
        // AddAndReplaceKind
        closure_handlers.unset_property = zend_closure_unset_property;

        break;
      }
#endif
#ifdef COMPILE_763
      case 62: {
        // AddAndReplaceKind
        closure_handlers.write_property = zend_closure_write_property;

        break;
      }
#endif
#ifdef COMPILE_764
      case 63: {
        // AddAndReplaceKind
        convert_to_boolean((res));

        break;
      }
#endif
#ifdef COMPILE_765
      case 64: {
        // AddAndReplaceKind
        convert_to_boolean((this_ptr));

        break;
      }
#endif
#ifdef COMPILE_766
      case 65: {
        // AddAndReplaceKind
        destroy_op_array(&(closure)->func.op_array);

        break;
      }
#endif
#ifdef COMPILE_767
      case 66: {
        // AddAndReplaceKind
        exit(1);

        break;
      }
#endif
#ifdef COMPILE_768
      case 67: {
        // AddAndReplaceKind
        gc_remove_zval_from_buffer((res));

        break;
      }
#endif
#ifdef COMPILE_769
      case 68: {
        // AddAndReplaceKind
        gc_remove_zval_from_buffer((this_ptr));

        break;
      }
#endif
#ifdef COMPILE_770
      case 69: {
        // AddAndReplaceKind
        gc_zval_check_possible_root(((res)));

        break;
      }
#endif
#ifdef COMPILE_771
      case 70: {
        // AddAndReplaceKind
        gc_zval_check_possible_root(((this_ptr)));

        break;
      }
#endif
#ifdef COMPILE_772
      case 71: {
        // AddAndReplaceKind
        gc_zval_possible_root((res));

        break;
      }
#endif
#ifdef COMPILE_773
      case 72: {
        // AddAndReplaceKind
        gc_zval_possible_root((this_ptr));

        break;
      }
#endif
#ifdef COMPILE_774
      case 73: {
        // AddAndReplaceKind
        i_zval_ptr_dtor((res));

        break;
      }
#endif
#ifdef COMPILE_775
      case 74: {
        // AddAndReplaceKind
        i_zval_ptr_dtor((this_ptr));

        break;
      }
#endif
#ifdef COMPILE_776
      case 75: {
        // AddAndReplaceKind
        if (scope && !instanceof_function(scope, func->common.scope)) {
          zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                     func->common.scope->name, func->common.function_name,
                     scope->name);
          scope = ((void *)0);
        }

        break;
      }
#endif
#ifdef COMPILE_777
      case 76: {
        // AddAndReplaceKind
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

        break;
      }
#endif
#ifdef COMPILE_778
      case 77: {
        // AddAndReplaceKind
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
#ifdef COMPILE_779
      case 78: {
        // AddAndReplaceKind
        memcpy(&closure_handlers, zend_get_std_object_handlers(),
               sizeof(zend_object_handlers));

        break;
      }
#endif
#ifdef COMPILE_780
      case 79: {
        // AddAndReplaceKind
        memset((closure), 0, sizeof(zend_closure));

        break;
      }
#endif
#ifdef COMPILE_781
      case 80: {
        // AddAndReplaceKind
        scope = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_782
      case 81: {
        // AddAndReplaceKind
        this_ptr = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_783
      case 82: {
        // AddAndReplaceKind
        zend_ce_closure->create_object = zend_closure_new;

        break;
      }
#endif
#ifdef COMPILE_784
      case 83: {
        // AddAndReplaceKind
        zend_ce_closure->serialize = zend_class_serialize_deny;

        break;
      }
#endif
#ifdef COMPILE_785
      case 84: {
        // AddAndReplaceKind
        zend_ce_closure->unserialize = zend_class_unserialize_deny;

        break;
      }
#endif
#ifdef COMPILE_786
      case 85: {
        // AddAndReplaceKind
        zend_error((1 << 0L), "Cannot destroy active lambda function");

        break;
      }
#endif
#ifdef COMPILE_787
      case 86: {
        // AddAndReplaceKind
        zend_error((1 << 12L), "Cannot get arguments for calling closure");

        break;
      }
#endif
#ifdef COMPILE_788
      case 87: {
        // AddAndReplaceKind
        zend_error((1 << 12L), "Closure object cannot have properties");

        break;
      }
#endif
#ifdef COMPILE_789
      case 88: {
        // AddAndReplaceKind
        zend_error((1 << 12L), "Instantiation of 'Closure' is not allowed");

        break;
      }
#endif
#ifdef COMPILE_790
      case 89: {
        // AddAndReplaceKind
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   (func)->common.scope->name, func->common.function_name,
                   zend_get_class_entry(&(*this_ptr))->name);

        break;
      }
#endif
#ifdef COMPILE_791
      case 90: {
        // AddAndReplaceKind
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, (func)->common.function_name,
                   zend_get_class_entry(&(*this_ptr))->name);

        break;
      }
#endif
#ifdef COMPILE_792
      case 91: {
        // AddAndReplaceKind
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   zend_get_class_entry(&(*(res)))->name);

        break;
      }
#endif
#ifdef COMPILE_793
      case 92: {
        // AddAndReplaceKind
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   zend_get_class_entry(&(*(this_ptr)))->name);

        break;
      }
#endif
#ifdef COMPILE_794
      case 93: {
        // AddAndReplaceKind
        zend_error((1 << 1L),
                   "Cannot bind function %s::%s to object of class %s",
                   func->common.scope->name, func->common.function_name,
                   zend_get_class_entry(&(*this_ptr))->name);

        break;
      }
#endif
#ifdef COMPILE_795
      case 94: {
        // AddAndReplaceKind
        zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                   (func)->common.scope->name, func->common.function_name,
                   scope->name);

        break;
      }
#endif
#ifdef COMPILE_796
      case 95: {
        // AddAndReplaceKind
        zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, (func)->common.function_name,
                   scope->name);

        break;
      }
#endif
#ifdef COMPILE_797
      case 96: {
        // AddAndReplaceKind
        zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, func->common.function_name,
                   (scope)->name);

        break;
      }
#endif
#ifdef COMPILE_798
      case 97: {
        // AddAndReplaceKind
        zend_error((1 << 1L), "Cannot bind function %s::%s to scope class %s",
                   func->common.scope->name, func->common.function_name,
                   scope->name);

        break;
      }
#endif
#ifdef COMPILE_799
      case 98: {
        // AddAndReplaceKind
        zend_error((1 << 3L), "A non well formed numeric value encountered");

        break;
      }
#endif
#ifdef COMPILE_800
      case 99: {
        // AddAndReplaceKind
        zend_hash_destroy((closure)->debug_info);

        break;
      }
#endif
#ifdef COMPILE_801
      case 100: {
        // AddAndReplaceKind
        zend_object_std_dtor(&(closure)->std);

        break;
      }
#endif
#ifdef COMPILE_802
      case 101: {
        // AddAndReplaceKind
        zval_addref_p(&((executor_globals.uninitialized_zval)));

        break;
      }
#endif
#ifdef COMPILE_803
      case 102: {
        // AddAndReplaceKind
        zval_addref_p((closure)->this_ptr);

        break;
      }
#endif
#ifdef COMPILE_804
      case 103: {
        // AddAndReplaceKind
        zval_addref_p((res));

        break;
      }
#endif
#ifdef COMPILE_805
      case 104: {
        // AddAndReplaceKind
        zval_addref_p((this_ptr));

        break;
      }
#endif
#ifdef COMPILE_806
      case 105: {
        // AddAndReplaceKind
        zval_addref_p(this_ptr);

        break;
      }
#endif
#ifdef COMPILE_807
      case 106: {
        // AddAndReplaceKind
        zval_unset_isref_p((res));

        break;
      }
#endif
#ifdef COMPILE_808
      case 107: {
        // AddAndReplaceKind
        zval_unset_isref_p((this_ptr));

        break;
      }
#endif
      }
      {
        long long __temp25 = (closure->func.op_array.static_variables);
        switch (__choose("__SWITCH25")) {
        case 0: {

          break;
        }
#ifdef COMPILE_809
        case 1: {
          __temp25 =
              ((closure->func.op_array.static_variables) &&
               !__is_neg("25-1", 39, &(closure), sizeof(closure),
                         &(closure->func.op_array.static_variables),
                         sizeof(closure->func.op_array.static_variables),
                         &(func), sizeof(func), &(res), sizeof(res),
                         &(zend_ce_closure), sizeof(zend_ce_closure),
                         &(zend_ce_closure), sizeof(zend_ce_closure),
                         &(closure->func.op_array.run_time_cache),
                         sizeof(closure->func.op_array.run_time_cache),
                         &(closure->func.op_array.refcount),
                         sizeof(closure->func.op_array.refcount), &(scope),
                         sizeof(scope), &(this_ptr), sizeof(this_ptr),
                         &(closure->this_ptr), sizeof(closure->this_ptr),
                         &(closure->func.op_array.type),
                         sizeof(closure->func.op_array.type),
                         &(closure->func.op_array.function_name),
                         sizeof(closure->func.op_array.function_name),
                         &(closure->func.op_array.scope),
                         sizeof(closure->func.op_array.scope),
                         &(closure->func.op_array.fn_flags),
                         sizeof(closure->func.op_array.fn_flags),
                         &(closure->func.op_array.prototype),
                         sizeof(closure->func.op_array.prototype),
                         &(closure->func.op_array.num_args),
                         sizeof(closure->func.op_array.num_args),
                         &(closure->func.op_array.required_num_args),
                         sizeof(closure->func.op_array.required_num_args),
                         &(closure->func.op_array.arg_info),
                         sizeof(closure->func.op_array.arg_info),
                         &(closure->func.op_array.opcodes),
                         sizeof(closure->func.op_array.opcodes),
                         &(closure->func.op_array.last),
                         sizeof(closure->func.op_array.last),
                         &(closure->func.op_array.vars),
                         sizeof(closure->func.op_array.vars),
                         &(closure->func.op_array.last_var),
                         sizeof(closure->func.op_array.last_var),
                         &(closure->func.op_array.T),
                         sizeof(closure->func.op_array.T),
                         &(closure->func.op_array.brk_cont_array),
                         sizeof(closure->func.op_array.brk_cont_array),
                         &(closure->func.op_array.last_brk_cont),
                         sizeof(closure->func.op_array.last_brk_cont),
                         &(closure->func.op_array.try_catch_array),
                         sizeof(closure->func.op_array.try_catch_array),
                         &(closure->func.op_array.last_try_catch),
                         sizeof(closure->func.op_array.last_try_catch),
                         &(closure->func.op_array.this_var),
                         sizeof(closure->func.op_array.this_var),
                         &(closure->func.op_array.filename),
                         sizeof(closure->func.op_array.filename),
                         &(closure->func.op_array.line_start),
                         sizeof(closure->func.op_array.line_start),
                         &(closure->func.op_array.line_end),
                         sizeof(closure->func.op_array.line_end),
                         &(closure->func.op_array.doc_comment),
                         sizeof(closure->func.op_array.doc_comment),
                         &(closure->func.op_array.doc_comment_len),
                         sizeof(closure->func.op_array.doc_comment_len),
                         &(closure->func.op_array.early_binding),
                         sizeof(closure->func.op_array.early_binding),
                         &(closure->func.op_array.literals),
                         sizeof(closure->func.op_array.literals),
                         &(closure->func.op_array.last_literal),
                         sizeof(closure->func.op_array.last_literal),
                         &(closure->func.op_array.last_cache_slot),
                         sizeof(closure->func.op_array.last_cache_slot),
                         &(closure->debug_info), sizeof(closure->debug_info)));

          break;
        }
#endif
        }
        switch (__choose("__SWITCH32")) {
        case 0: {
          if (__temp25) {
            HashTable *static_variables =
                closure->func.op_array.static_variables;

            {
              switch (__choose("__SWITCH26")) {
              case 0:
                break;
#ifdef COMPILE_810
              case 1: {
                // AddAndReplaceKind
                ((closure)->func.op_array.static_variables) =
                    (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
#ifdef COMPILE_811
              case 2: {
                // AddAndReplaceKind
                (*(closure)->func.op_array.refcount)++;

                break;
              }
#endif
#ifdef COMPILE_812
              case 3: {
                // AddAndReplaceKind
                (*closure->func.op_array.refcount)++;

                break;
              }
#endif
#ifdef COMPILE_813
              case 4: {
                // AddAndReplaceKind
                (closure) = (zend_closure *)zend_object_store_get_object(res);

                break;
              }
#endif
#ifdef COMPILE_814
              case 5: {
                // AddAndReplaceKind
                (closure)->func = *func;

                break;
              }
#endif
#ifdef COMPILE_815
              case 6: {
                // AddAndReplaceKind
                (closure)->func.common.scope = scope;

                break;
              }
#endif
#ifdef COMPILE_816
              case 7: {
                // AddAndReplaceKind
                (closure)->func.op_array.run_time_cache = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_817
              case 8: {
                // AddAndReplaceKind
                (closure)->this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_818
              case 9: {
                // AddAndReplaceKind
                (closure)->this_ptr = this_ptr;

                break;
              }
#endif
#ifdef COMPILE_819
              case 10: {
                // AddAndReplaceKind
                (closure->func.op_array.static_variables) =
                    (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
#ifdef COMPILE_820
              case 11: {
                // AddAndReplaceKind
                (executor_globals.argument_stack) =
                    zend_vm_stack_new_page(((16 * 1024) - 16));

                break;
              }
#endif
#ifdef COMPILE_821
              case 12: {
                // AddAndReplaceKind
                (res) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_822
              case 13: {
                // AddAndReplaceKind
                (scope) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_823
              case 14: {
                // AddAndReplaceKind
                (scope)->create_object = zend_closure_new;

                break;
              }
#endif
#ifdef COMPILE_824
              case 15: {
                // AddAndReplaceKind
                (scope)->serialize = zend_class_serialize_deny;

                break;
              }
#endif
#ifdef COMPILE_825
              case 16: {
                // AddAndReplaceKind
                (scope)->unserialize = zend_class_unserialize_deny;

                break;
              }
#endif
#ifdef COMPILE_826
              case 17: {
                // AddAndReplaceKind
                (this_ptr) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_827
              case 18: {
                // AddAndReplaceKind
                _array_init(((res)), 0);

                break;
              }
#endif
#ifdef COMPILE_828
              case 19: {
                // AddAndReplaceKind
                _array_init(((this_ptr)), 0);

                break;
              }
#endif
#ifdef COMPILE_829
              case 20: {
                // AddAndReplaceKind
                _efree(((closure)));

                break;
              }
#endif
#ifdef COMPILE_830
              case 21: {
                // AddAndReplaceKind
                _efree(((closure)->debug_info));

                break;
              }
#endif
#ifdef COMPILE_831
              case 22: {
                // AddAndReplaceKind
                _efree(((func)));

                break;
              }
#endif
#ifdef COMPILE_832
              case 23: {
                // AddAndReplaceKind
                _efree(((func)->internal_function.function_name));

                break;
              }
#endif
#ifdef COMPILE_833
              case 24: {
                // AddAndReplaceKind
                _efree(((res)));

                break;
              }
#endif
#ifdef COMPILE_834
              case 25: {
                // AddAndReplaceKind
                _efree(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_835
              case 26: {
                // AddAndReplaceKind
                _object_init_ex(((res)), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_836
              case 27: {
                // AddAndReplaceKind
                _object_init_ex(((this_ptr)), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_837
              case 28: {
                // AddAndReplaceKind
                _object_init_ex((res), ((scope)));

                break;
              }
#endif
#ifdef COMPILE_838
              case 29: {
                // AddAndReplaceKind
                _object_init_ex((res), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_839
              case 30: {
                // AddAndReplaceKind
                _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_840
              case 31: {
                // AddAndReplaceKind
                _zend_hash_init(((closure)->func.op_array.static_variables),
                                (zend_hash_num_elements(static_variables)),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_841
              case 32: {
                // AddAndReplaceKind
                _zend_hash_init((closure->func.op_array.static_variables),
                                (zend_hash_num_elements((static_variables))),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_842
              case 33: {
                // AddAndReplaceKind
                _zend_hash_init((closure->func.op_array.static_variables),
                                (zend_hash_num_elements(static_variables)),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_843
              case 34: {
                // AddAndReplaceKind
                _zval_copy_ctor(((res)));

                break;
              }
#endif
#ifdef COMPILE_844
              case 35: {
                // AddAndReplaceKind
                _zval_copy_ctor(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_845
              case 36: {
                // AddAndReplaceKind
                _zval_copy_ctor_func((res));

                break;
              }
#endif
#ifdef COMPILE_846
              case 37: {
                // AddAndReplaceKind
                _zval_copy_ctor_func((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_847
              case 38: {
                // AddAndReplaceKind
                _zval_dtor(((res)));

                break;
              }
#endif
#ifdef COMPILE_848
              case 39: {
                // AddAndReplaceKind
                _zval_dtor(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_849
              case 40: {
                // AddAndReplaceKind
                _zval_dtor_func((res));

                break;
              }
#endif
#ifdef COMPILE_850
              case 41: {
                // AddAndReplaceKind
                _zval_dtor_func((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_851
              case 42: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(closure)->this_ptr));

                break;
              }
#endif
#ifdef COMPILE_852
              case 43: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(res)));

                break;
              }
#endif
#ifdef COMPILE_853
              case 44: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_854
              case 45: {
                // AddAndReplaceKind
                closure = (zend_closure *)zend_object_store_get_object(res);

                break;
              }
#endif
#ifdef COMPILE_855
              case 46: {
                // AddAndReplaceKind
                closure->func = *(func);

                break;
              }
#endif
#ifdef COMPILE_856
              case 47: {
                // AddAndReplaceKind
                closure->func = *func;

                break;
              }
#endif
#ifdef COMPILE_857
              case 48: {
                // AddAndReplaceKind
                closure->func.common.scope = (scope);

                break;
              }
#endif
#ifdef COMPILE_858
              case 49: {
                // AddAndReplaceKind
                closure->func.common.scope = scope;

                break;
              }
#endif
#ifdef COMPILE_859
              case 50: {
                // AddAndReplaceKind
                closure->func.op_array.run_time_cache = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_860
              case 51: {
                // AddAndReplaceKind
                closure->this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_861
              case 52: {
                // AddAndReplaceKind
                closure->this_ptr = (res);

                break;
              }
#endif
#ifdef COMPILE_862
              case 53: {
                // AddAndReplaceKind
                closure->this_ptr = (this_ptr);

                break;
              }
#endif
#ifdef COMPILE_863
              case 54: {
                // AddAndReplaceKind
                closure->this_ptr = this_ptr;

                break;
              }
#endif
#ifdef COMPILE_864
              case 55: {
                // AddAndReplaceKind
                closure_handlers.clone_obj = zend_closure_clone;

                break;
              }
#endif
#ifdef COMPILE_865
              case 56: {
                // AddAndReplaceKind
                closure_handlers.compare_objects = zend_closure_compare_objects;

                break;
              }
#endif
#ifdef COMPILE_866
              case 57: {
                // AddAndReplaceKind
                closure_handlers.get_closure = zend_closure_get_closure;

                break;
              }
#endif
#ifdef COMPILE_867
              case 58: {
                // AddAndReplaceKind
                closure_handlers.get_constructor = zend_closure_get_constructor;

                break;
              }
#endif
#ifdef COMPILE_868
              case 59: {
                // AddAndReplaceKind
                closure_handlers.get_debug_info = zend_closure_get_debug_info;

                break;
              }
#endif
#ifdef COMPILE_869
              case 60: {
                // AddAndReplaceKind
                closure_handlers.get_method = zend_closure_get_method;

                break;
              }
#endif
#ifdef COMPILE_870
              case 61: {
                // AddAndReplaceKind
                closure_handlers.get_property_ptr_ptr =
                    zend_closure_get_property_ptr_ptr;

                break;
              }
#endif
#ifdef COMPILE_871
              case 62: {
                // AddAndReplaceKind
                closure_handlers.has_property = zend_closure_has_property;

                break;
              }
#endif
#ifdef COMPILE_872
              case 63: {
                // AddAndReplaceKind
                closure_handlers.read_property = zend_closure_read_property;

                break;
              }
#endif
#ifdef COMPILE_873
              case 64: {
                // AddAndReplaceKind
                closure_handlers.unset_property = zend_closure_unset_property;

                break;
              }
#endif
#ifdef COMPILE_874
              case 65: {
                // AddAndReplaceKind
                closure_handlers.write_property = zend_closure_write_property;

                break;
              }
#endif
#ifdef COMPILE_875
              case 66: {
                // AddAndReplaceKind
                convert_to_boolean((res));

                break;
              }
#endif
#ifdef COMPILE_876
              case 67: {
                // AddAndReplaceKind
                convert_to_boolean((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_877
              case 68: {
                // AddAndReplaceKind
                destroy_op_array(&(closure)->func.op_array);

                break;
              }
#endif
#ifdef COMPILE_878
              case 69: {
                // AddAndReplaceKind
                exit(1);

                break;
              }
#endif
#ifdef COMPILE_879
              case 70: {
                // AddAndReplaceKind
                gc_remove_zval_from_buffer((res));

                break;
              }
#endif
#ifdef COMPILE_880
              case 71: {
                // AddAndReplaceKind
                gc_remove_zval_from_buffer((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_881
              case 72: {
                // AddAndReplaceKind
                gc_zval_check_possible_root(((res)));

                break;
              }
#endif
#ifdef COMPILE_882
              case 73: {
                // AddAndReplaceKind
                gc_zval_check_possible_root(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_883
              case 74: {
                // AddAndReplaceKind
                gc_zval_possible_root((res));

                break;
              }
#endif
#ifdef COMPILE_884
              case 75: {
                // AddAndReplaceKind
                gc_zval_possible_root((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_885
              case 76: {
                // AddAndReplaceKind
                i_zval_ptr_dtor((res));

                break;
              }
#endif
#ifdef COMPILE_886
              case 77: {
                // AddAndReplaceKind
                i_zval_ptr_dtor((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_887
              case 78: {
                // AddAndReplaceKind
                if (scope && !instanceof_function(scope, func->common.scope)) {
                  zend_error((1 << 1L),
                             "Cannot bind function %s::%s to scope class %s",
                             func->common.scope->name,
                             func->common.function_name, scope->name);
                  scope = ((void *)0);
                }

                break;
              }
#endif
#ifdef COMPILE_888
              case 79: {
                // AddAndReplaceKind
                if (scope && this_ptr && (func->common.fn_flags & 1) == 0 &&
                    !instanceof_function(zend_get_class_entry(&(*this_ptr)),
                                         closure->func.common.scope)) {
                  zend_error(
                      (1 << 1L),
                      "Cannot bind function %s::%s to object of class %s",
                      func->common.scope->name, func->common.function_name,
                      zend_get_class_entry(&(*this_ptr))->name);
                  scope = ((void *)0);
                  this_ptr = ((void *)0);
                }

                break;
              }
#endif
#ifdef COMPILE_889
              case 80: {
                // AddAndReplaceKind
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
#ifdef COMPILE_890
              case 81: {
                // AddAndReplaceKind
                memcpy(&closure_handlers, zend_get_std_object_handlers(),
                       sizeof(zend_object_handlers));

                break;
              }
#endif
#ifdef COMPILE_891
              case 82: {
                // AddAndReplaceKind
                memset((closure), 0, sizeof(zend_closure));

                break;
              }
#endif
#ifdef COMPILE_892
              case 83: {
                // AddAndReplaceKind
                scope = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_893
              case 84: {
                // AddAndReplaceKind
                this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_894
              case 85: {
                // AddAndReplaceKind
                zend_ce_closure->create_object = zend_closure_new;

                break;
              }
#endif
#ifdef COMPILE_895
              case 86: {
                // AddAndReplaceKind
                zend_ce_closure->serialize = zend_class_serialize_deny;

                break;
              }
#endif
#ifdef COMPILE_896
              case 87: {
                // AddAndReplaceKind
                zend_ce_closure->unserialize = zend_class_unserialize_deny;

                break;
              }
#endif
#ifdef COMPILE_897
              case 88: {
                // AddAndReplaceKind
                zend_error((1 << 0L), "Cannot destroy active lambda function");

                break;
              }
#endif
#ifdef COMPILE_898
              case 89: {
                // AddAndReplaceKind
                zend_error((1 << 12L),
                           "Cannot get arguments for calling closure");

                break;
              }
#endif
#ifdef COMPILE_899
              case 90: {
                // AddAndReplaceKind
                zend_error((1 << 12L), "Closure object cannot have properties");

                break;
              }
#endif
#ifdef COMPILE_900
              case 91: {
                // AddAndReplaceKind
                zend_error((1 << 12L),
                           "Instantiation of 'Closure' is not allowed");

                break;
              }
#endif
#ifdef COMPILE_901
              case 92: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           (func)->common.scope->name,
                           func->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_902
              case 93: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name,
                           (func)->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_903
              case 94: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*(res)))->name);

                break;
              }
#endif
#ifdef COMPILE_904
              case 95: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*(this_ptr)))->name);

                break;
              }
#endif
#ifdef COMPILE_905
              case 96: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_906
              case 97: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           (func)->common.scope->name,
                           func->common.function_name, scope->name);

                break;
              }
#endif
#ifdef COMPILE_907
              case 98: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name,
                           (func)->common.function_name, scope->name);

                break;
              }
#endif
#ifdef COMPILE_908
              case 99: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name, func->common.function_name,
                           (scope)->name);

                break;
              }
#endif
#ifdef COMPILE_909
              case 100: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name, func->common.function_name,
                           scope->name);

                break;
              }
#endif
#ifdef COMPILE_910
              case 101: {
                // AddAndReplaceKind
                zend_error((1 << 3L),
                           "A non well formed numeric value encountered");

                break;
              }
#endif
#ifdef COMPILE_911
              case 102: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    (static_variables), (apply_func_args_t)zval_copy_static_var,
                    1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_912
              case 103: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    static_variables, (apply_func_args_t)zval_copy_static_var,
                    1, (closure)->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_913
              case 104: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    static_variables, (apply_func_args_t)zval_copy_static_var,
                    1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_914
              case 105: {
                // AddAndReplaceKind
                zend_hash_destroy((closure)->debug_info);

                break;
              }
#endif
#ifdef COMPILE_915
              case 106: {
                // AddAndReplaceKind
                zend_object_std_dtor(&(closure)->std);

                break;
              }
#endif
#ifdef COMPILE_916
              case 107: {
                // AddAndReplaceKind
                zval_addref_p(&((executor_globals.uninitialized_zval)));

                break;
              }
#endif
#ifdef COMPILE_917
              case 108: {
                // AddAndReplaceKind
                zval_addref_p((closure)->this_ptr);

                break;
              }
#endif
#ifdef COMPILE_918
              case 109: {
                // AddAndReplaceKind
                zval_addref_p((res));

                break;
              }
#endif
#ifdef COMPILE_919
              case 110: {
                // AddAndReplaceKind
                zval_addref_p((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_920
              case 111: {
                // AddAndReplaceKind
                zval_addref_p(this_ptr);

                break;
              }
#endif
#ifdef COMPILE_921
              case 112: {
                // AddAndReplaceKind
                zval_unset_isref_p((res));

                break;
              }
#endif
#ifdef COMPILE_922
              case 113: {
                // AddAndReplaceKind
                zval_unset_isref_p((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_923
              case 114: {
                // AddInitKind
                memset(&closure->func, 0, sizeof(*(&closure->func)));

                break;
              }
#endif
#ifdef COMPILE_924
              case 115: {
                // AddInitKind
                memset(&closure->func.op_array, 0,
                       sizeof(*(&closure->func.op_array)));

                break;
              }
#endif
#ifdef COMPILE_925
              case 116: {
                // AddInitKind
                memset(closure, 0, sizeof(*(closure)));

                break;
              }
#endif
#ifdef COMPILE_926
              case 117: {
                // IfExitKind
                if (__is_neg("26-117", 40, &(closure), sizeof(closure),
                             &(closure->func.op_array.static_variables),
                             sizeof(closure->func.op_array.static_variables),
                             &(static_variables), sizeof(static_variables),
                             &(closure->func.op_array.run_time_cache),
                             sizeof(closure->func.op_array.run_time_cache),
                             &(closure->func.op_array.refcount),
                             sizeof(closure->func.op_array.refcount), &(func),
                             sizeof(func), &(res), sizeof(res), &(scope),
                             sizeof(scope), &(zend_ce_closure),
                             sizeof(zend_ce_closure), &(zend_ce_closure),
                             sizeof(zend_ce_closure), &(this_ptr),
                             sizeof(this_ptr), &(closure->this_ptr),
                             sizeof(closure->this_ptr), &(closure->debug_info),
                             sizeof(closure->debug_info),
                             &(closure->func.op_array.type),
                             sizeof(closure->func.op_array.type),
                             &(closure->func.op_array.function_name),
                             sizeof(closure->func.op_array.function_name),
                             &(closure->func.op_array.scope),
                             sizeof(closure->func.op_array.scope),
                             &(closure->func.op_array.fn_flags),
                             sizeof(closure->func.op_array.fn_flags),
                             &(closure->func.op_array.prototype),
                             sizeof(closure->func.op_array.prototype),
                             &(closure->func.op_array.num_args),
                             sizeof(closure->func.op_array.num_args),
                             &(closure->func.op_array.required_num_args),
                             sizeof(closure->func.op_array.required_num_args),
                             &(closure->func.op_array.arg_info),
                             sizeof(closure->func.op_array.arg_info),
                             &(closure->func.op_array.opcodes),
                             sizeof(closure->func.op_array.opcodes),
                             &(closure->func.op_array.last),
                             sizeof(closure->func.op_array.last),
                             &(closure->func.op_array.vars),
                             sizeof(closure->func.op_array.vars),
                             &(closure->func.op_array.last_var),
                             sizeof(closure->func.op_array.last_var),
                             &(closure->func.op_array.T),
                             sizeof(closure->func.op_array.T),
                             &(closure->func.op_array.brk_cont_array),
                             sizeof(closure->func.op_array.brk_cont_array),
                             &(closure->func.op_array.last_brk_cont),
                             sizeof(closure->func.op_array.last_brk_cont),
                             &(closure->func.op_array.try_catch_array),
                             sizeof(closure->func.op_array.try_catch_array),
                             &(closure->func.op_array.last_try_catch),
                             sizeof(closure->func.op_array.last_try_catch),
                             &(closure->func.op_array.this_var),
                             sizeof(closure->func.op_array.this_var),
                             &(closure->func.op_array.filename),
                             sizeof(closure->func.op_array.filename),
                             &(closure->func.op_array.line_start),
                             sizeof(closure->func.op_array.line_start),
                             &(closure->func.op_array.line_end),
                             sizeof(closure->func.op_array.line_end),
                             &(closure->func.op_array.doc_comment),
                             sizeof(closure->func.op_array.doc_comment),
                             &(closure->func.op_array.doc_comment_len),
                             sizeof(closure->func.op_array.doc_comment_len),
                             &(closure->func.op_array.early_binding),
                             sizeof(closure->func.op_array.early_binding),
                             &(closure->func.op_array.literals),
                             sizeof(closure->func.op_array.literals),
                             &(closure->func.op_array.last_literal),
                             sizeof(closure->func.op_array.last_literal),
                             &(closure->func.op_array.last_cache_slot),
                             sizeof(closure->func.op_array.last_cache_slot)))
                  return;

                break;
              }
#endif
              }
              switch (__choose("__SWITCH27")) {
              case 0: {
                ALLOC_HASHTABLE(closure->func.op_array.static_variables);

                break;
              }
#ifdef COMPILE_927
              case 1: {
                // AddAndReplaceKind
                (closure->func.op_array.static_variables) =
                    (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
#ifdef COMPILE_928
              case 2: {
                // GuardKind
                if (!__is_neg(
                        "27-2", 40, &(closure), sizeof(closure),
                        &(closure->func.op_array.static_variables),
                        sizeof(closure->func.op_array.static_variables),
                        &(static_variables), sizeof(static_variables),
                        &(closure->func.op_array.run_time_cache),
                        sizeof(closure->func.op_array.run_time_cache),
                        &(closure->func.op_array.refcount),
                        sizeof(closure->func.op_array.refcount), &(func),
                        sizeof(func), &(res), sizeof(res), &(scope),
                        sizeof(scope), &(zend_ce_closure),
                        sizeof(zend_ce_closure), &(zend_ce_closure),
                        sizeof(zend_ce_closure), &(this_ptr), sizeof(this_ptr),
                        &(closure->this_ptr), sizeof(closure->this_ptr),
                        &(closure->func.op_array.type),
                        sizeof(closure->func.op_array.type),
                        &(closure->func.op_array.function_name),
                        sizeof(closure->func.op_array.function_name),
                        &(closure->func.op_array.scope),
                        sizeof(closure->func.op_array.scope),
                        &(closure->func.op_array.fn_flags),
                        sizeof(closure->func.op_array.fn_flags),
                        &(closure->func.op_array.prototype),
                        sizeof(closure->func.op_array.prototype),
                        &(closure->func.op_array.num_args),
                        sizeof(closure->func.op_array.num_args),
                        &(closure->func.op_array.required_num_args),
                        sizeof(closure->func.op_array.required_num_args),
                        &(closure->func.op_array.arg_info),
                        sizeof(closure->func.op_array.arg_info),
                        &(closure->func.op_array.opcodes),
                        sizeof(closure->func.op_array.opcodes),
                        &(closure->func.op_array.last),
                        sizeof(closure->func.op_array.last),
                        &(closure->func.op_array.vars),
                        sizeof(closure->func.op_array.vars),
                        &(closure->func.op_array.last_var),
                        sizeof(closure->func.op_array.last_var),
                        &(closure->func.op_array.T),
                        sizeof(closure->func.op_array.T),
                        &(closure->func.op_array.brk_cont_array),
                        sizeof(closure->func.op_array.brk_cont_array),
                        &(closure->func.op_array.last_brk_cont),
                        sizeof(closure->func.op_array.last_brk_cont),
                        &(closure->func.op_array.try_catch_array),
                        sizeof(closure->func.op_array.try_catch_array),
                        &(closure->func.op_array.last_try_catch),
                        sizeof(closure->func.op_array.last_try_catch),
                        &(closure->func.op_array.this_var),
                        sizeof(closure->func.op_array.this_var),
                        &(closure->func.op_array.filename),
                        sizeof(closure->func.op_array.filename),
                        &(closure->func.op_array.line_start),
                        sizeof(closure->func.op_array.line_start),
                        &(closure->func.op_array.line_end),
                        sizeof(closure->func.op_array.line_end),
                        &(closure->func.op_array.doc_comment),
                        sizeof(closure->func.op_array.doc_comment),
                        &(closure->func.op_array.doc_comment_len),
                        sizeof(closure->func.op_array.doc_comment_len),
                        &(closure->func.op_array.early_binding),
                        sizeof(closure->func.op_array.early_binding),
                        &(closure->func.op_array.literals),
                        sizeof(closure->func.op_array.literals),
                        &(closure->func.op_array.last_literal),
                        sizeof(closure->func.op_array.last_literal),
                        &(closure->func.op_array.last_cache_slot),
                        sizeof(closure->func.op_array.last_cache_slot),
                        &(closure->debug_info), sizeof(closure->debug_info)))
                  (closure->func.op_array.static_variables) =
                      (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
#ifdef COMPILE_929
              case 3: {
                // ReplaceKind
                ((closure)->func.op_array.static_variables) =
                    (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
              }
            }
            {
              switch (__choose("__SWITCH28")) {
              case 0:
                break;
#ifdef COMPILE_930
              case 1: {
                // AddAndReplaceKind
                ((closure)->func.op_array.static_variables) =
                    (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
#ifdef COMPILE_931
              case 2: {
                // AddAndReplaceKind
                (*(closure)->func.op_array.refcount)++;

                break;
              }
#endif
#ifdef COMPILE_932
              case 3: {
                // AddAndReplaceKind
                (*closure->func.op_array.refcount)++;

                break;
              }
#endif
#ifdef COMPILE_933
              case 4: {
                // AddAndReplaceKind
                (closure) = (zend_closure *)zend_object_store_get_object(res);

                break;
              }
#endif
#ifdef COMPILE_934
              case 5: {
                // AddAndReplaceKind
                (closure)->func = *func;

                break;
              }
#endif
#ifdef COMPILE_935
              case 6: {
                // AddAndReplaceKind
                (closure)->func.common.scope = scope;

                break;
              }
#endif
#ifdef COMPILE_936
              case 7: {
                // AddAndReplaceKind
                (closure)->func.op_array.run_time_cache = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_937
              case 8: {
                // AddAndReplaceKind
                (closure)->this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_938
              case 9: {
                // AddAndReplaceKind
                (closure)->this_ptr = this_ptr;

                break;
              }
#endif
#ifdef COMPILE_939
              case 10: {
                // AddAndReplaceKind
                (closure->func.op_array.static_variables) =
                    (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
#ifdef COMPILE_940
              case 11: {
                // AddAndReplaceKind
                (executor_globals.argument_stack) =
                    zend_vm_stack_new_page(((16 * 1024) - 16));

                break;
              }
#endif
#ifdef COMPILE_941
              case 12: {
                // AddAndReplaceKind
                (res) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_942
              case 13: {
                // AddAndReplaceKind
                (scope) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_943
              case 14: {
                // AddAndReplaceKind
                (scope)->create_object = zend_closure_new;

                break;
              }
#endif
#ifdef COMPILE_944
              case 15: {
                // AddAndReplaceKind
                (scope)->serialize = zend_class_serialize_deny;

                break;
              }
#endif
#ifdef COMPILE_945
              case 16: {
                // AddAndReplaceKind
                (scope)->unserialize = zend_class_unserialize_deny;

                break;
              }
#endif
#ifdef COMPILE_946
              case 17: {
                // AddAndReplaceKind
                (this_ptr) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_947
              case 18: {
                // AddAndReplaceKind
                _array_init(((res)), 0);

                break;
              }
#endif
#ifdef COMPILE_948
              case 19: {
                // AddAndReplaceKind
                _array_init(((this_ptr)), 0);

                break;
              }
#endif
#ifdef COMPILE_949
              case 20: {
                // AddAndReplaceKind
                _efree(((closure)));

                break;
              }
#endif
#ifdef COMPILE_950
              case 21: {
                // AddAndReplaceKind
                _efree(((closure)->debug_info));

                break;
              }
#endif
#ifdef COMPILE_951
              case 22: {
                // AddAndReplaceKind
                _efree(((func)));

                break;
              }
#endif
#ifdef COMPILE_952
              case 23: {
                // AddAndReplaceKind
                _efree(((func)->internal_function.function_name));

                break;
              }
#endif
#ifdef COMPILE_953
              case 24: {
                // AddAndReplaceKind
                _efree(((res)));

                break;
              }
#endif
#ifdef COMPILE_954
              case 25: {
                // AddAndReplaceKind
                _efree(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_955
              case 26: {
                // AddAndReplaceKind
                _object_init_ex(((res)), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_956
              case 27: {
                // AddAndReplaceKind
                _object_init_ex(((this_ptr)), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_957
              case 28: {
                // AddAndReplaceKind
                _object_init_ex((res), ((scope)));

                break;
              }
#endif
#ifdef COMPILE_958
              case 29: {
                // AddAndReplaceKind
                _object_init_ex((res), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_959
              case 30: {
                // AddAndReplaceKind
                _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_960
              case 31: {
                // AddAndReplaceKind
                _zend_hash_init(((closure)->func.op_array.static_variables),
                                (zend_hash_num_elements(static_variables)),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_961
              case 32: {
                // AddAndReplaceKind
                _zend_hash_init((closure->func.op_array.static_variables),
                                (zend_hash_num_elements((static_variables))),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_962
              case 33: {
                // AddAndReplaceKind
                _zend_hash_init((closure->func.op_array.static_variables),
                                (zend_hash_num_elements(static_variables)),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_963
              case 34: {
                // AddAndReplaceKind
                _zval_copy_ctor(((res)));

                break;
              }
#endif
#ifdef COMPILE_964
              case 35: {
                // AddAndReplaceKind
                _zval_copy_ctor(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_965
              case 36: {
                // AddAndReplaceKind
                _zval_copy_ctor_func((res));

                break;
              }
#endif
#ifdef COMPILE_966
              case 37: {
                // AddAndReplaceKind
                _zval_copy_ctor_func((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_967
              case 38: {
                // AddAndReplaceKind
                _zval_dtor(((res)));

                break;
              }
#endif
#ifdef COMPILE_968
              case 39: {
                // AddAndReplaceKind
                _zval_dtor(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_969
              case 40: {
                // AddAndReplaceKind
                _zval_dtor_func((res));

                break;
              }
#endif
#ifdef COMPILE_970
              case 41: {
                // AddAndReplaceKind
                _zval_dtor_func((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_971
              case 42: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(closure)->this_ptr));

                break;
              }
#endif
#ifdef COMPILE_972
              case 43: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(res)));

                break;
              }
#endif
#ifdef COMPILE_973
              case 44: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_974
              case 45: {
                // AddAndReplaceKind
                closure = (zend_closure *)zend_object_store_get_object(res);

                break;
              }
#endif
#ifdef COMPILE_975
              case 46: {
                // AddAndReplaceKind
                closure->func = *(func);

                break;
              }
#endif
#ifdef COMPILE_976
              case 47: {
                // AddAndReplaceKind
                closure->func = *func;

                break;
              }
#endif
#ifdef COMPILE_977
              case 48: {
                // AddAndReplaceKind
                closure->func.common.scope = (scope);

                break;
              }
#endif
#ifdef COMPILE_978
              case 49: {
                // AddAndReplaceKind
                closure->func.common.scope = scope;

                break;
              }
#endif
#ifdef COMPILE_979
              case 50: {
                // AddAndReplaceKind
                closure->func.op_array.run_time_cache = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_980
              case 51: {
                // AddAndReplaceKind
                closure->this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_981
              case 52: {
                // AddAndReplaceKind
                closure->this_ptr = (res);

                break;
              }
#endif
#ifdef COMPILE_982
              case 53: {
                // AddAndReplaceKind
                closure->this_ptr = (this_ptr);

                break;
              }
#endif
#ifdef COMPILE_983
              case 54: {
                // AddAndReplaceKind
                closure->this_ptr = this_ptr;

                break;
              }
#endif
#ifdef COMPILE_984
              case 55: {
                // AddAndReplaceKind
                closure_handlers.clone_obj = zend_closure_clone;

                break;
              }
#endif
#ifdef COMPILE_985
              case 56: {
                // AddAndReplaceKind
                closure_handlers.compare_objects = zend_closure_compare_objects;

                break;
              }
#endif
#ifdef COMPILE_986
              case 57: {
                // AddAndReplaceKind
                closure_handlers.get_closure = zend_closure_get_closure;

                break;
              }
#endif
#ifdef COMPILE_987
              case 58: {
                // AddAndReplaceKind
                closure_handlers.get_constructor = zend_closure_get_constructor;

                break;
              }
#endif
#ifdef COMPILE_988
              case 59: {
                // AddAndReplaceKind
                closure_handlers.get_debug_info = zend_closure_get_debug_info;

                break;
              }
#endif
#ifdef COMPILE_989
              case 60: {
                // AddAndReplaceKind
                closure_handlers.get_method = zend_closure_get_method;

                break;
              }
#endif
#ifdef COMPILE_990
              case 61: {
                // AddAndReplaceKind
                closure_handlers.get_property_ptr_ptr =
                    zend_closure_get_property_ptr_ptr;

                break;
              }
#endif
#ifdef COMPILE_991
              case 62: {
                // AddAndReplaceKind
                closure_handlers.has_property = zend_closure_has_property;

                break;
              }
#endif
#ifdef COMPILE_992
              case 63: {
                // AddAndReplaceKind
                closure_handlers.read_property = zend_closure_read_property;

                break;
              }
#endif
#ifdef COMPILE_993
              case 64: {
                // AddAndReplaceKind
                closure_handlers.unset_property = zend_closure_unset_property;

                break;
              }
#endif
#ifdef COMPILE_994
              case 65: {
                // AddAndReplaceKind
                closure_handlers.write_property = zend_closure_write_property;

                break;
              }
#endif
#ifdef COMPILE_995
              case 66: {
                // AddAndReplaceKind
                convert_to_boolean((res));

                break;
              }
#endif
#ifdef COMPILE_996
              case 67: {
                // AddAndReplaceKind
                convert_to_boolean((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_997
              case 68: {
                // AddAndReplaceKind
                destroy_op_array(&(closure)->func.op_array);

                break;
              }
#endif
#ifdef COMPILE_998
              case 69: {
                // AddAndReplaceKind
                exit(1);

                break;
              }
#endif
#ifdef COMPILE_999
              case 70: {
                // AddAndReplaceKind
                gc_remove_zval_from_buffer((res));

                break;
              }
#endif
#ifdef COMPILE_1000
              case 71: {
                // AddAndReplaceKind
                gc_remove_zval_from_buffer((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1001
              case 72: {
                // AddAndReplaceKind
                gc_zval_check_possible_root(((res)));

                break;
              }
#endif
#ifdef COMPILE_1002
              case 73: {
                // AddAndReplaceKind
                gc_zval_check_possible_root(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_1003
              case 74: {
                // AddAndReplaceKind
                gc_zval_possible_root((res));

                break;
              }
#endif
#ifdef COMPILE_1004
              case 75: {
                // AddAndReplaceKind
                gc_zval_possible_root((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1005
              case 76: {
                // AddAndReplaceKind
                i_zval_ptr_dtor((res));

                break;
              }
#endif
#ifdef COMPILE_1006
              case 77: {
                // AddAndReplaceKind
                i_zval_ptr_dtor((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1007
              case 78: {
                // AddAndReplaceKind
                if (scope && !instanceof_function(scope, func->common.scope)) {
                  zend_error((1 << 1L),
                             "Cannot bind function %s::%s to scope class %s",
                             func->common.scope->name,
                             func->common.function_name, scope->name);
                  scope = ((void *)0);
                }

                break;
              }
#endif
#ifdef COMPILE_1008
              case 79: {
                // AddAndReplaceKind
                if (scope && this_ptr && (func->common.fn_flags & 1) == 0 &&
                    !instanceof_function(zend_get_class_entry(&(*this_ptr)),
                                         closure->func.common.scope)) {
                  zend_error(
                      (1 << 1L),
                      "Cannot bind function %s::%s to object of class %s",
                      func->common.scope->name, func->common.function_name,
                      zend_get_class_entry(&(*this_ptr))->name);
                  scope = ((void *)0);
                  this_ptr = ((void *)0);
                }

                break;
              }
#endif
#ifdef COMPILE_1009
              case 80: {
                // AddAndReplaceKind
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
#ifdef COMPILE_1010
              case 81: {
                // AddAndReplaceKind
                memcpy(&closure_handlers, zend_get_std_object_handlers(),
                       sizeof(zend_object_handlers));

                break;
              }
#endif
#ifdef COMPILE_1011
              case 82: {
                // AddAndReplaceKind
                memset((closure), 0, sizeof(zend_closure));

                break;
              }
#endif
#ifdef COMPILE_1012
              case 83: {
                // AddAndReplaceKind
                scope = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1013
              case 84: {
                // AddAndReplaceKind
                this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1014
              case 85: {
                // AddAndReplaceKind
                zend_ce_closure->create_object = zend_closure_new;

                break;
              }
#endif
#ifdef COMPILE_1015
              case 86: {
                // AddAndReplaceKind
                zend_ce_closure->serialize = zend_class_serialize_deny;

                break;
              }
#endif
#ifdef COMPILE_1016
              case 87: {
                // AddAndReplaceKind
                zend_ce_closure->unserialize = zend_class_unserialize_deny;

                break;
              }
#endif
#ifdef COMPILE_1017
              case 88: {
                // AddAndReplaceKind
                zend_error((1 << 0L), "Cannot destroy active lambda function");

                break;
              }
#endif
#ifdef COMPILE_1018
              case 89: {
                // AddAndReplaceKind
                zend_error((1 << 12L),
                           "Cannot get arguments for calling closure");

                break;
              }
#endif
#ifdef COMPILE_1019
              case 90: {
                // AddAndReplaceKind
                zend_error((1 << 12L), "Closure object cannot have properties");

                break;
              }
#endif
#ifdef COMPILE_1020
              case 91: {
                // AddAndReplaceKind
                zend_error((1 << 12L),
                           "Instantiation of 'Closure' is not allowed");

                break;
              }
#endif
#ifdef COMPILE_1021
              case 92: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           (func)->common.scope->name,
                           func->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_1022
              case 93: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name,
                           (func)->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_1023
              case 94: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*(res)))->name);

                break;
              }
#endif
#ifdef COMPILE_1024
              case 95: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*(this_ptr)))->name);

                break;
              }
#endif
#ifdef COMPILE_1025
              case 96: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_1026
              case 97: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           (func)->common.scope->name,
                           func->common.function_name, scope->name);

                break;
              }
#endif
#ifdef COMPILE_1027
              case 98: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name,
                           (func)->common.function_name, scope->name);

                break;
              }
#endif
#ifdef COMPILE_1028
              case 99: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name, func->common.function_name,
                           (scope)->name);

                break;
              }
#endif
#ifdef COMPILE_1029
              case 100: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name, func->common.function_name,
                           scope->name);

                break;
              }
#endif
#ifdef COMPILE_1030
              case 101: {
                // AddAndReplaceKind
                zend_error((1 << 3L),
                           "A non well formed numeric value encountered");

                break;
              }
#endif
#ifdef COMPILE_1031
              case 102: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    (static_variables), (apply_func_args_t)zval_copy_static_var,
                    1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1032
              case 103: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    static_variables, (apply_func_args_t)zval_copy_static_var,
                    1, (closure)->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1033
              case 104: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    static_variables, (apply_func_args_t)zval_copy_static_var,
                    1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1034
              case 105: {
                // AddAndReplaceKind
                zend_hash_destroy((closure)->debug_info);

                break;
              }
#endif
#ifdef COMPILE_1035
              case 106: {
                // AddAndReplaceKind
                zend_object_std_dtor(&(closure)->std);

                break;
              }
#endif
#ifdef COMPILE_1036
              case 107: {
                // AddAndReplaceKind
                zval_addref_p(&((executor_globals.uninitialized_zval)));

                break;
              }
#endif
#ifdef COMPILE_1037
              case 108: {
                // AddAndReplaceKind
                zval_addref_p((closure)->this_ptr);

                break;
              }
#endif
#ifdef COMPILE_1038
              case 109: {
                // AddAndReplaceKind
                zval_addref_p((res));

                break;
              }
#endif
#ifdef COMPILE_1039
              case 110: {
                // AddAndReplaceKind
                zval_addref_p((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1040
              case 111: {
                // AddAndReplaceKind
                zval_addref_p(this_ptr);

                break;
              }
#endif
#ifdef COMPILE_1041
              case 112: {
                // AddAndReplaceKind
                zval_unset_isref_p((res));

                break;
              }
#endif
#ifdef COMPILE_1042
              case 113: {
                // AddAndReplaceKind
                zval_unset_isref_p((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1043
              case 114: {
                // AddInitKind
                memset(&closure->func, 0, sizeof(*(&closure->func)));

                break;
              }
#endif
#ifdef COMPILE_1044
              case 115: {
                // AddInitKind
                memset(&closure->func.op_array, 0,
                       sizeof(*(&closure->func.op_array)));

                break;
              }
#endif
#ifdef COMPILE_1045
              case 116: {
                // AddInitKind
                memset(closure, 0, sizeof(*(closure)));

                break;
              }
#endif
#ifdef COMPILE_1046
              case 117: {
                // IfExitKind
                if (__is_neg(
                        "28-117", 40, &(closure), sizeof(closure),
                        &(static_variables), sizeof(static_variables),
                        &(closure->func.op_array.static_variables),
                        sizeof(closure->func.op_array.static_variables),
                        &(closure->func.op_array.run_time_cache),
                        sizeof(closure->func.op_array.run_time_cache),
                        &(closure->func.op_array.refcount),
                        sizeof(closure->func.op_array.refcount), &(func),
                        sizeof(func), &(scope), sizeof(scope), &(res),
                        sizeof(res), &(zend_ce_closure),
                        sizeof(zend_ce_closure), &(zend_ce_closure),
                        sizeof(zend_ce_closure), &(this_ptr), sizeof(this_ptr),
                        &(closure->this_ptr), sizeof(closure->this_ptr),
                        &(closure->func.op_array.type),
                        sizeof(closure->func.op_array.type),
                        &(closure->func.op_array.function_name),
                        sizeof(closure->func.op_array.function_name),
                        &(closure->func.op_array.scope),
                        sizeof(closure->func.op_array.scope),
                        &(closure->func.op_array.fn_flags),
                        sizeof(closure->func.op_array.fn_flags),
                        &(closure->func.op_array.prototype),
                        sizeof(closure->func.op_array.prototype),
                        &(closure->func.op_array.num_args),
                        sizeof(closure->func.op_array.num_args),
                        &(closure->func.op_array.required_num_args),
                        sizeof(closure->func.op_array.required_num_args),
                        &(closure->func.op_array.arg_info),
                        sizeof(closure->func.op_array.arg_info),
                        &(closure->func.op_array.opcodes),
                        sizeof(closure->func.op_array.opcodes),
                        &(closure->func.op_array.last),
                        sizeof(closure->func.op_array.last),
                        &(closure->func.op_array.vars),
                        sizeof(closure->func.op_array.vars),
                        &(closure->func.op_array.last_var),
                        sizeof(closure->func.op_array.last_var),
                        &(closure->func.op_array.T),
                        sizeof(closure->func.op_array.T),
                        &(closure->func.op_array.brk_cont_array),
                        sizeof(closure->func.op_array.brk_cont_array),
                        &(closure->func.op_array.last_brk_cont),
                        sizeof(closure->func.op_array.last_brk_cont),
                        &(closure->func.op_array.try_catch_array),
                        sizeof(closure->func.op_array.try_catch_array),
                        &(closure->func.op_array.last_try_catch),
                        sizeof(closure->func.op_array.last_try_catch),
                        &(closure->func.op_array.this_var),
                        sizeof(closure->func.op_array.this_var),
                        &(closure->func.op_array.filename),
                        sizeof(closure->func.op_array.filename),
                        &(closure->func.op_array.line_start),
                        sizeof(closure->func.op_array.line_start),
                        &(closure->func.op_array.line_end),
                        sizeof(closure->func.op_array.line_end),
                        &(closure->func.op_array.doc_comment),
                        sizeof(closure->func.op_array.doc_comment),
                        &(closure->func.op_array.doc_comment_len),
                        sizeof(closure->func.op_array.doc_comment_len),
                        &(closure->func.op_array.early_binding),
                        sizeof(closure->func.op_array.early_binding),
                        &(closure->func.op_array.literals),
                        sizeof(closure->func.op_array.literals),
                        &(closure->func.op_array.last_literal),
                        sizeof(closure->func.op_array.last_literal),
                        &(closure->func.op_array.last_cache_slot),
                        sizeof(closure->func.op_array.last_cache_slot),
                        &(closure->debug_info), sizeof(closure->debug_info)))
                  return;

                break;
              }
#endif
              }
              switch (__choose("__SWITCH29")) {
              case 0: {
                zend_hash_init(closure->func.op_array.static_variables,
                               zend_hash_num_elements(static_variables), NULL,
                               ZVAL_PTR_DTOR, 0);

                break;
              }
#ifdef COMPILE_1047
              case 1: {
                // AddAndReplaceKind
                _zend_ts_hash_init((closure->func.op_array.static_variables),
                                   (zend_hash_num_elements(static_variables)),
                                   (((void *)0)),
                                   ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1048
              case 2: {
                // AddAndReplaceKind
                printf((closure->func.op_array.static_variables),
                       (zend_hash_num_elements(static_variables)),
                       (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1049
              case 3: {
                // AddAndReplaceKind
                scanf((closure->func.op_array.static_variables),
                      (zend_hash_num_elements(static_variables)), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1050
              case 4: {
                // GuardKind
                if (!__is_neg(
                        "29-4", 40, &(closure), sizeof(closure),
                        &(static_variables), sizeof(static_variables),
                        &(closure->func.op_array.static_variables),
                        sizeof(closure->func.op_array.static_variables),
                        &(closure->func.op_array.run_time_cache),
                        sizeof(closure->func.op_array.run_time_cache),
                        &(closure->func.op_array.refcount),
                        sizeof(closure->func.op_array.refcount), &(func),
                        sizeof(func), &(scope), sizeof(scope), &(res),
                        sizeof(res), &(zend_ce_closure),
                        sizeof(zend_ce_closure), &(zend_ce_closure),
                        sizeof(zend_ce_closure), &(this_ptr), sizeof(this_ptr),
                        &(closure->this_ptr), sizeof(closure->this_ptr),
                        &(closure->func.op_array.type),
                        sizeof(closure->func.op_array.type),
                        &(closure->func.op_array.function_name),
                        sizeof(closure->func.op_array.function_name),
                        &(closure->func.op_array.scope),
                        sizeof(closure->func.op_array.scope),
                        &(closure->func.op_array.fn_flags),
                        sizeof(closure->func.op_array.fn_flags),
                        &(closure->func.op_array.prototype),
                        sizeof(closure->func.op_array.prototype),
                        &(closure->func.op_array.num_args),
                        sizeof(closure->func.op_array.num_args),
                        &(closure->func.op_array.required_num_args),
                        sizeof(closure->func.op_array.required_num_args),
                        &(closure->func.op_array.arg_info),
                        sizeof(closure->func.op_array.arg_info),
                        &(closure->func.op_array.opcodes),
                        sizeof(closure->func.op_array.opcodes),
                        &(closure->func.op_array.last),
                        sizeof(closure->func.op_array.last),
                        &(closure->func.op_array.vars),
                        sizeof(closure->func.op_array.vars),
                        &(closure->func.op_array.last_var),
                        sizeof(closure->func.op_array.last_var),
                        &(closure->func.op_array.T),
                        sizeof(closure->func.op_array.T),
                        &(closure->func.op_array.brk_cont_array),
                        sizeof(closure->func.op_array.brk_cont_array),
                        &(closure->func.op_array.last_brk_cont),
                        sizeof(closure->func.op_array.last_brk_cont),
                        &(closure->func.op_array.try_catch_array),
                        sizeof(closure->func.op_array.try_catch_array),
                        &(closure->func.op_array.last_try_catch),
                        sizeof(closure->func.op_array.last_try_catch),
                        &(closure->func.op_array.this_var),
                        sizeof(closure->func.op_array.this_var),
                        &(closure->func.op_array.filename),
                        sizeof(closure->func.op_array.filename),
                        &(closure->func.op_array.line_start),
                        sizeof(closure->func.op_array.line_start),
                        &(closure->func.op_array.line_end),
                        sizeof(closure->func.op_array.line_end),
                        &(closure->func.op_array.doc_comment),
                        sizeof(closure->func.op_array.doc_comment),
                        &(closure->func.op_array.doc_comment_len),
                        sizeof(closure->func.op_array.doc_comment_len),
                        &(closure->func.op_array.early_binding),
                        sizeof(closure->func.op_array.early_binding),
                        &(closure->func.op_array.literals),
                        sizeof(closure->func.op_array.literals),
                        &(closure->func.op_array.last_literal),
                        sizeof(closure->func.op_array.last_literal),
                        &(closure->func.op_array.last_cache_slot),
                        sizeof(closure->func.op_array.last_cache_slot),
                        &(closure->debug_info), sizeof(closure->debug_info)))
                  _zend_hash_init((closure->func.op_array.static_variables),
                                  (zend_hash_num_elements(static_variables)),
                                  (((void *)0)),
                                  ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1051
              case 5: {
                // ReplaceKind
                _zend_hash_init(((closure)->func.op_array.static_variables),
                                (zend_hash_num_elements(static_variables)),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1052
              case 6: {
                // ReplaceKind
                _zend_hash_init((closure->func.op_array.static_variables),
                                (zend_hash_num_elements((static_variables))),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
              }
            }
            {
              switch (__choose("__SWITCH30")) {
              case 0:
                break;
#ifdef COMPILE_1053
              case 1: {
                // AddAndReplaceKind
                ((closure)->func.op_array.static_variables) =
                    (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
#ifdef COMPILE_1054
              case 2: {
                // AddAndReplaceKind
                (*(closure)->func.op_array.refcount)++;

                break;
              }
#endif
#ifdef COMPILE_1055
              case 3: {
                // AddAndReplaceKind
                (*closure->func.op_array.refcount)++;

                break;
              }
#endif
#ifdef COMPILE_1056
              case 4: {
                // AddAndReplaceKind
                (closure) = (zend_closure *)zend_object_store_get_object(res);

                break;
              }
#endif
#ifdef COMPILE_1057
              case 5: {
                // AddAndReplaceKind
                (closure)->func = *func;

                break;
              }
#endif
#ifdef COMPILE_1058
              case 6: {
                // AddAndReplaceKind
                (closure)->func.common.scope = scope;

                break;
              }
#endif
#ifdef COMPILE_1059
              case 7: {
                // AddAndReplaceKind
                (closure)->func.op_array.run_time_cache = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1060
              case 8: {
                // AddAndReplaceKind
                (closure)->this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1061
              case 9: {
                // AddAndReplaceKind
                (closure)->this_ptr = this_ptr;

                break;
              }
#endif
#ifdef COMPILE_1062
              case 10: {
                // AddAndReplaceKind
                (closure->func.op_array.static_variables) =
                    (HashTable *)_emalloc((sizeof(HashTable)));

                break;
              }
#endif
#ifdef COMPILE_1063
              case 11: {
                // AddAndReplaceKind
                (executor_globals.argument_stack) =
                    zend_vm_stack_new_page(((16 * 1024) - 16));

                break;
              }
#endif
#ifdef COMPILE_1064
              case 12: {
                // AddAndReplaceKind
                (res) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1065
              case 13: {
                // AddAndReplaceKind
                (scope) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1066
              case 14: {
                // AddAndReplaceKind
                (scope)->create_object = zend_closure_new;

                break;
              }
#endif
#ifdef COMPILE_1067
              case 15: {
                // AddAndReplaceKind
                (scope)->serialize = zend_class_serialize_deny;

                break;
              }
#endif
#ifdef COMPILE_1068
              case 16: {
                // AddAndReplaceKind
                (scope)->unserialize = zend_class_unserialize_deny;

                break;
              }
#endif
#ifdef COMPILE_1069
              case 17: {
                // AddAndReplaceKind
                (this_ptr) = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1070
              case 18: {
                // AddAndReplaceKind
                _array_init(((res)), 0);

                break;
              }
#endif
#ifdef COMPILE_1071
              case 19: {
                // AddAndReplaceKind
                _array_init(((this_ptr)), 0);

                break;
              }
#endif
#ifdef COMPILE_1072
              case 20: {
                // AddAndReplaceKind
                _efree(((closure)));

                break;
              }
#endif
#ifdef COMPILE_1073
              case 21: {
                // AddAndReplaceKind
                _efree(((closure)->debug_info));

                break;
              }
#endif
#ifdef COMPILE_1074
              case 22: {
                // AddAndReplaceKind
                _efree(((func)));

                break;
              }
#endif
#ifdef COMPILE_1075
              case 23: {
                // AddAndReplaceKind
                _efree(((func)->internal_function.function_name));

                break;
              }
#endif
#ifdef COMPILE_1076
              case 24: {
                // AddAndReplaceKind
                _efree(((res)));

                break;
              }
#endif
#ifdef COMPILE_1077
              case 25: {
                // AddAndReplaceKind
                _efree(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_1078
              case 26: {
                // AddAndReplaceKind
                _object_init_ex(((res)), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_1079
              case 27: {
                // AddAndReplaceKind
                _object_init_ex(((this_ptr)), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_1080
              case 28: {
                // AddAndReplaceKind
                _object_init_ex((res), ((scope)));

                break;
              }
#endif
#ifdef COMPILE_1081
              case 29: {
                // AddAndReplaceKind
                _object_init_ex((res), (zend_ce_closure));

                break;
              }
#endif
#ifdef COMPILE_1082
              case 30: {
                // AddAndReplaceKind
                _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1083
              case 31: {
                // AddAndReplaceKind
                _zend_hash_init(((closure)->func.op_array.static_variables),
                                (zend_hash_num_elements(static_variables)),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1084
              case 32: {
                // AddAndReplaceKind
                _zend_hash_init((closure->func.op_array.static_variables),
                                (zend_hash_num_elements((static_variables))),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1085
              case 33: {
                // AddAndReplaceKind
                _zend_hash_init((closure->func.op_array.static_variables),
                                (zend_hash_num_elements(static_variables)),
                                (((void *)0)),
                                ((void (*)(void *))_zval_ptr_dtor), (0));

                break;
              }
#endif
#ifdef COMPILE_1086
              case 34: {
                // AddAndReplaceKind
                _zval_copy_ctor(((res)));

                break;
              }
#endif
#ifdef COMPILE_1087
              case 35: {
                // AddAndReplaceKind
                _zval_copy_ctor(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_1088
              case 36: {
                // AddAndReplaceKind
                _zval_copy_ctor_func((res));

                break;
              }
#endif
#ifdef COMPILE_1089
              case 37: {
                // AddAndReplaceKind
                _zval_copy_ctor_func((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1090
              case 38: {
                // AddAndReplaceKind
                _zval_dtor(((res)));

                break;
              }
#endif
#ifdef COMPILE_1091
              case 39: {
                // AddAndReplaceKind
                _zval_dtor(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_1092
              case 40: {
                // AddAndReplaceKind
                _zval_dtor_func((res));

                break;
              }
#endif
#ifdef COMPILE_1093
              case 41: {
                // AddAndReplaceKind
                _zval_dtor_func((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1094
              case 42: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(closure)->this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1095
              case 43: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(res)));

                break;
              }
#endif
#ifdef COMPILE_1096
              case 44: {
                // AddAndReplaceKind
                _zval_ptr_dtor((&(this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_1097
              case 45: {
                // AddAndReplaceKind
                closure = (zend_closure *)zend_object_store_get_object(res);

                break;
              }
#endif
#ifdef COMPILE_1098
              case 46: {
                // AddAndReplaceKind
                closure->func = *(func);

                break;
              }
#endif
#ifdef COMPILE_1099
              case 47: {
                // AddAndReplaceKind
                closure->func = *func;

                break;
              }
#endif
#ifdef COMPILE_1100
              case 48: {
                // AddAndReplaceKind
                closure->func.common.scope = (scope);

                break;
              }
#endif
#ifdef COMPILE_1101
              case 49: {
                // AddAndReplaceKind
                closure->func.common.scope = scope;

                break;
              }
#endif
#ifdef COMPILE_1102
              case 50: {
                // AddAndReplaceKind
                closure->func.op_array.run_time_cache = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1103
              case 51: {
                // AddAndReplaceKind
                closure->this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1104
              case 52: {
                // AddAndReplaceKind
                closure->this_ptr = (res);

                break;
              }
#endif
#ifdef COMPILE_1105
              case 53: {
                // AddAndReplaceKind
                closure->this_ptr = (this_ptr);

                break;
              }
#endif
#ifdef COMPILE_1106
              case 54: {
                // AddAndReplaceKind
                closure->this_ptr = this_ptr;

                break;
              }
#endif
#ifdef COMPILE_1107
              case 55: {
                // AddAndReplaceKind
                closure_handlers.clone_obj = zend_closure_clone;

                break;
              }
#endif
#ifdef COMPILE_1108
              case 56: {
                // AddAndReplaceKind
                closure_handlers.compare_objects = zend_closure_compare_objects;

                break;
              }
#endif
#ifdef COMPILE_1109
              case 57: {
                // AddAndReplaceKind
                closure_handlers.get_closure = zend_closure_get_closure;

                break;
              }
#endif
#ifdef COMPILE_1110
              case 58: {
                // AddAndReplaceKind
                closure_handlers.get_constructor = zend_closure_get_constructor;

                break;
              }
#endif
#ifdef COMPILE_1111
              case 59: {
                // AddAndReplaceKind
                closure_handlers.get_debug_info = zend_closure_get_debug_info;

                break;
              }
#endif
#ifdef COMPILE_1112
              case 60: {
                // AddAndReplaceKind
                closure_handlers.get_method = zend_closure_get_method;

                break;
              }
#endif
#ifdef COMPILE_1113
              case 61: {
                // AddAndReplaceKind
                closure_handlers.get_property_ptr_ptr =
                    zend_closure_get_property_ptr_ptr;

                break;
              }
#endif
#ifdef COMPILE_1114
              case 62: {
                // AddAndReplaceKind
                closure_handlers.has_property = zend_closure_has_property;

                break;
              }
#endif
#ifdef COMPILE_1115
              case 63: {
                // AddAndReplaceKind
                closure_handlers.read_property = zend_closure_read_property;

                break;
              }
#endif
#ifdef COMPILE_1116
              case 64: {
                // AddAndReplaceKind
                closure_handlers.unset_property = zend_closure_unset_property;

                break;
              }
#endif
#ifdef COMPILE_1117
              case 65: {
                // AddAndReplaceKind
                closure_handlers.write_property = zend_closure_write_property;

                break;
              }
#endif
#ifdef COMPILE_1118
              case 66: {
                // AddAndReplaceKind
                convert_to_boolean((res));

                break;
              }
#endif
#ifdef COMPILE_1119
              case 67: {
                // AddAndReplaceKind
                convert_to_boolean((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1120
              case 68: {
                // AddAndReplaceKind
                destroy_op_array(&(closure)->func.op_array);

                break;
              }
#endif
#ifdef COMPILE_1121
              case 69: {
                // AddAndReplaceKind
                exit(1);

                break;
              }
#endif
#ifdef COMPILE_1122
              case 70: {
                // AddAndReplaceKind
                gc_remove_zval_from_buffer((res));

                break;
              }
#endif
#ifdef COMPILE_1123
              case 71: {
                // AddAndReplaceKind
                gc_remove_zval_from_buffer((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1124
              case 72: {
                // AddAndReplaceKind
                gc_zval_check_possible_root(((res)));

                break;
              }
#endif
#ifdef COMPILE_1125
              case 73: {
                // AddAndReplaceKind
                gc_zval_check_possible_root(((this_ptr)));

                break;
              }
#endif
#ifdef COMPILE_1126
              case 74: {
                // AddAndReplaceKind
                gc_zval_possible_root((res));

                break;
              }
#endif
#ifdef COMPILE_1127
              case 75: {
                // AddAndReplaceKind
                gc_zval_possible_root((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1128
              case 76: {
                // AddAndReplaceKind
                i_zval_ptr_dtor((res));

                break;
              }
#endif
#ifdef COMPILE_1129
              case 77: {
                // AddAndReplaceKind
                i_zval_ptr_dtor((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1130
              case 78: {
                // AddAndReplaceKind
                if (scope && !instanceof_function(scope, func->common.scope)) {
                  zend_error((1 << 1L),
                             "Cannot bind function %s::%s to scope class %s",
                             func->common.scope->name,
                             func->common.function_name, scope->name);
                  scope = ((void *)0);
                }

                break;
              }
#endif
#ifdef COMPILE_1131
              case 79: {
                // AddAndReplaceKind
                if (scope && this_ptr && (func->common.fn_flags & 1) == 0 &&
                    !instanceof_function(zend_get_class_entry(&(*this_ptr)),
                                         closure->func.common.scope)) {
                  zend_error(
                      (1 << 1L),
                      "Cannot bind function %s::%s to object of class %s",
                      func->common.scope->name, func->common.function_name,
                      zend_get_class_entry(&(*this_ptr))->name);
                  scope = ((void *)0);
                  this_ptr = ((void *)0);
                }

                break;
              }
#endif
#ifdef COMPILE_1132
              case 80: {
                // AddAndReplaceKind
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
#ifdef COMPILE_1133
              case 81: {
                // AddAndReplaceKind
                memcpy(&closure_handlers, zend_get_std_object_handlers(),
                       sizeof(zend_object_handlers));

                break;
              }
#endif
#ifdef COMPILE_1134
              case 82: {
                // AddAndReplaceKind
                memset((closure), 0, sizeof(zend_closure));

                break;
              }
#endif
#ifdef COMPILE_1135
              case 83: {
                // AddAndReplaceKind
                scope = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1136
              case 84: {
                // AddAndReplaceKind
                this_ptr = ((void *)0);

                break;
              }
#endif
#ifdef COMPILE_1137
              case 85: {
                // AddAndReplaceKind
                zend_ce_closure->create_object = zend_closure_new;

                break;
              }
#endif
#ifdef COMPILE_1138
              case 86: {
                // AddAndReplaceKind
                zend_ce_closure->serialize = zend_class_serialize_deny;

                break;
              }
#endif
#ifdef COMPILE_1139
              case 87: {
                // AddAndReplaceKind
                zend_ce_closure->unserialize = zend_class_unserialize_deny;

                break;
              }
#endif
#ifdef COMPILE_1140
              case 88: {
                // AddAndReplaceKind
                zend_error((1 << 0L), "Cannot destroy active lambda function");

                break;
              }
#endif
#ifdef COMPILE_1141
              case 89: {
                // AddAndReplaceKind
                zend_error((1 << 12L),
                           "Cannot get arguments for calling closure");

                break;
              }
#endif
#ifdef COMPILE_1142
              case 90: {
                // AddAndReplaceKind
                zend_error((1 << 12L), "Closure object cannot have properties");

                break;
              }
#endif
#ifdef COMPILE_1143
              case 91: {
                // AddAndReplaceKind
                zend_error((1 << 12L),
                           "Instantiation of 'Closure' is not allowed");

                break;
              }
#endif
#ifdef COMPILE_1144
              case 92: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           (func)->common.scope->name,
                           func->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_1145
              case 93: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name,
                           (func)->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_1146
              case 94: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*(res)))->name);

                break;
              }
#endif
#ifdef COMPILE_1147
              case 95: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*(this_ptr)))->name);

                break;
              }
#endif
#ifdef COMPILE_1148
              case 96: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to object of class %s",
                           func->common.scope->name, func->common.function_name,
                           zend_get_class_entry(&(*this_ptr))->name);

                break;
              }
#endif
#ifdef COMPILE_1149
              case 97: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           (func)->common.scope->name,
                           func->common.function_name, scope->name);

                break;
              }
#endif
#ifdef COMPILE_1150
              case 98: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name,
                           (func)->common.function_name, scope->name);

                break;
              }
#endif
#ifdef COMPILE_1151
              case 99: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name, func->common.function_name,
                           (scope)->name);

                break;
              }
#endif
#ifdef COMPILE_1152
              case 100: {
                // AddAndReplaceKind
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name, func->common.function_name,
                           scope->name);

                break;
              }
#endif
#ifdef COMPILE_1153
              case 101: {
                // AddAndReplaceKind
                zend_error((1 << 3L),
                           "A non well formed numeric value encountered");

                break;
              }
#endif
#ifdef COMPILE_1154
              case 102: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    (static_variables), (apply_func_args_t)zval_copy_static_var,
                    1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1155
              case 103: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    static_variables, (apply_func_args_t)zval_copy_static_var,
                    1, (closure)->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1156
              case 104: {
                // AddAndReplaceKind
                zend_hash_apply_with_arguments(
                    static_variables, (apply_func_args_t)zval_copy_static_var,
                    1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1157
              case 105: {
                // AddAndReplaceKind
                zend_hash_destroy((closure)->debug_info);

                break;
              }
#endif
#ifdef COMPILE_1158
              case 106: {
                // AddAndReplaceKind
                zend_object_std_dtor(&(closure)->std);

                break;
              }
#endif
#ifdef COMPILE_1159
              case 107: {
                // AddAndReplaceKind
                zval_addref_p(&((executor_globals.uninitialized_zval)));

                break;
              }
#endif
#ifdef COMPILE_1160
              case 108: {
                // AddAndReplaceKind
                zval_addref_p((closure)->this_ptr);

                break;
              }
#endif
#ifdef COMPILE_1161
              case 109: {
                // AddAndReplaceKind
                zval_addref_p((res));

                break;
              }
#endif
#ifdef COMPILE_1162
              case 110: {
                // AddAndReplaceKind
                zval_addref_p((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1163
              case 111: {
                // AddAndReplaceKind
                zval_addref_p(this_ptr);

                break;
              }
#endif
#ifdef COMPILE_1164
              case 112: {
                // AddAndReplaceKind
                zval_unset_isref_p((res));

                break;
              }
#endif
#ifdef COMPILE_1165
              case 113: {
                // AddAndReplaceKind
                zval_unset_isref_p((this_ptr));

                break;
              }
#endif
#ifdef COMPILE_1166
              case 114: {
                // AddInitKind
                memset(&closure->func, 0, sizeof(*(&closure->func)));

                break;
              }
#endif
#ifdef COMPILE_1167
              case 115: {
                // AddInitKind
                memset(&closure->func.op_array, 0,
                       sizeof(*(&closure->func.op_array)));

                break;
              }
#endif
#ifdef COMPILE_1168
              case 116: {
                // AddInitKind
                memset(closure, 0, sizeof(*(closure)));

                break;
              }
#endif
#ifdef COMPILE_1169
              case 117: {
                // IfExitKind
                if (__is_neg("30-117", 40, &(closure), sizeof(closure),
                             &(static_variables), sizeof(static_variables),
                             &(closure->func.op_array.static_variables),
                             sizeof(closure->func.op_array.static_variables),
                             &(closure->func.op_array.run_time_cache),
                             sizeof(closure->func.op_array.run_time_cache),
                             &(closure->func.op_array.refcount),
                             sizeof(closure->func.op_array.refcount), &(func),
                             sizeof(func), &(scope), sizeof(scope), &(res),
                             sizeof(res), &(this_ptr), sizeof(this_ptr),
                             &(zend_ce_closure), sizeof(zend_ce_closure),
                             &(zend_ce_closure), sizeof(zend_ce_closure),
                             &(closure->this_ptr), sizeof(closure->this_ptr),
                             &(closure->func.op_array.type),
                             sizeof(closure->func.op_array.type),
                             &(closure->func.op_array.function_name),
                             sizeof(closure->func.op_array.function_name),
                             &(closure->func.op_array.scope),
                             sizeof(closure->func.op_array.scope),
                             &(closure->func.op_array.fn_flags),
                             sizeof(closure->func.op_array.fn_flags),
                             &(closure->func.op_array.prototype),
                             sizeof(closure->func.op_array.prototype),
                             &(closure->func.op_array.num_args),
                             sizeof(closure->func.op_array.num_args),
                             &(closure->func.op_array.required_num_args),
                             sizeof(closure->func.op_array.required_num_args),
                             &(closure->func.op_array.arg_info),
                             sizeof(closure->func.op_array.arg_info),
                             &(closure->func.op_array.opcodes),
                             sizeof(closure->func.op_array.opcodes),
                             &(closure->func.op_array.last),
                             sizeof(closure->func.op_array.last),
                             &(closure->func.op_array.vars),
                             sizeof(closure->func.op_array.vars),
                             &(closure->func.op_array.last_var),
                             sizeof(closure->func.op_array.last_var),
                             &(closure->func.op_array.T),
                             sizeof(closure->func.op_array.T),
                             &(closure->func.op_array.brk_cont_array),
                             sizeof(closure->func.op_array.brk_cont_array),
                             &(closure->func.op_array.last_brk_cont),
                             sizeof(closure->func.op_array.last_brk_cont),
                             &(closure->func.op_array.try_catch_array),
                             sizeof(closure->func.op_array.try_catch_array),
                             &(closure->func.op_array.last_try_catch),
                             sizeof(closure->func.op_array.last_try_catch),
                             &(closure->func.op_array.this_var),
                             sizeof(closure->func.op_array.this_var),
                             &(closure->func.op_array.filename),
                             sizeof(closure->func.op_array.filename),
                             &(closure->func.op_array.line_start),
                             sizeof(closure->func.op_array.line_start),
                             &(closure->func.op_array.line_end),
                             sizeof(closure->func.op_array.line_end),
                             &(closure->func.op_array.doc_comment),
                             sizeof(closure->func.op_array.doc_comment),
                             &(closure->func.op_array.doc_comment_len),
                             sizeof(closure->func.op_array.doc_comment_len),
                             &(closure->func.op_array.early_binding),
                             sizeof(closure->func.op_array.early_binding),
                             &(closure->func.op_array.literals),
                             sizeof(closure->func.op_array.literals),
                             &(closure->func.op_array.last_literal),
                             sizeof(closure->func.op_array.last_literal),
                             &(closure->func.op_array.last_cache_slot),
                             sizeof(closure->func.op_array.last_cache_slot),
                             &(closure->debug_info),
                             sizeof(closure->debug_info)))
                  return;

                break;
              }
#endif
              }
              switch (__choose("__SWITCH31")) {
              case 0: {
                zend_hash_apply_with_arguments(
                    static_variables TSRMLS_CC,
                    (apply_func_args_t)zval_copy_static_var, 1,
                    closure->func.op_array.static_variables);

                break;
              }
#ifdef COMPILE_1170
              case 1: {
                // AddAndReplaceKind
                printf(static_variables,
                       (apply_func_args_t)zval_copy_static_var, 1,
                       closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1171
              case 2: {
                // AddAndReplaceKind
                scanf(static_variables, (apply_func_args_t)zval_copy_static_var,
                      1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1172
              case 3: {
                // AddAndReplaceKind
                zend_ts_hash_apply_with_arguments(
                    static_variables, (apply_func_args_t)zval_copy_static_var,
                    1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1173
              case 4: {
                // GuardKind
                if (!__is_neg("31-4", 40, &(closure), sizeof(closure),
                              &(static_variables), sizeof(static_variables),
                              &(closure->func.op_array.static_variables),
                              sizeof(closure->func.op_array.static_variables),
                              &(closure->func.op_array.run_time_cache),
                              sizeof(closure->func.op_array.run_time_cache),
                              &(closure->func.op_array.refcount),
                              sizeof(closure->func.op_array.refcount), &(func),
                              sizeof(func), &(scope), sizeof(scope), &(res),
                              sizeof(res), &(this_ptr), sizeof(this_ptr),
                              &(zend_ce_closure), sizeof(zend_ce_closure),
                              &(zend_ce_closure), sizeof(zend_ce_closure),
                              &(closure->this_ptr), sizeof(closure->this_ptr),
                              &(closure->func.op_array.type),
                              sizeof(closure->func.op_array.type),
                              &(closure->func.op_array.function_name),
                              sizeof(closure->func.op_array.function_name),
                              &(closure->func.op_array.scope),
                              sizeof(closure->func.op_array.scope),
                              &(closure->func.op_array.fn_flags),
                              sizeof(closure->func.op_array.fn_flags),
                              &(closure->func.op_array.prototype),
                              sizeof(closure->func.op_array.prototype),
                              &(closure->func.op_array.num_args),
                              sizeof(closure->func.op_array.num_args),
                              &(closure->func.op_array.required_num_args),
                              sizeof(closure->func.op_array.required_num_args),
                              &(closure->func.op_array.arg_info),
                              sizeof(closure->func.op_array.arg_info),
                              &(closure->func.op_array.opcodes),
                              sizeof(closure->func.op_array.opcodes),
                              &(closure->func.op_array.last),
                              sizeof(closure->func.op_array.last),
                              &(closure->func.op_array.vars),
                              sizeof(closure->func.op_array.vars),
                              &(closure->func.op_array.last_var),
                              sizeof(closure->func.op_array.last_var),
                              &(closure->func.op_array.T),
                              sizeof(closure->func.op_array.T),
                              &(closure->func.op_array.brk_cont_array),
                              sizeof(closure->func.op_array.brk_cont_array),
                              &(closure->func.op_array.last_brk_cont),
                              sizeof(closure->func.op_array.last_brk_cont),
                              &(closure->func.op_array.try_catch_array),
                              sizeof(closure->func.op_array.try_catch_array),
                              &(closure->func.op_array.last_try_catch),
                              sizeof(closure->func.op_array.last_try_catch),
                              &(closure->func.op_array.this_var),
                              sizeof(closure->func.op_array.this_var),
                              &(closure->func.op_array.filename),
                              sizeof(closure->func.op_array.filename),
                              &(closure->func.op_array.line_start),
                              sizeof(closure->func.op_array.line_start),
                              &(closure->func.op_array.line_end),
                              sizeof(closure->func.op_array.line_end),
                              &(closure->func.op_array.doc_comment),
                              sizeof(closure->func.op_array.doc_comment),
                              &(closure->func.op_array.doc_comment_len),
                              sizeof(closure->func.op_array.doc_comment_len),
                              &(closure->func.op_array.early_binding),
                              sizeof(closure->func.op_array.early_binding),
                              &(closure->func.op_array.literals),
                              sizeof(closure->func.op_array.literals),
                              &(closure->func.op_array.last_literal),
                              sizeof(closure->func.op_array.last_literal),
                              &(closure->func.op_array.last_cache_slot),
                              sizeof(closure->func.op_array.last_cache_slot),
                              &(closure->debug_info),
                              sizeof(closure->debug_info)))
                  zend_hash_apply_with_arguments(
                      static_variables, (apply_func_args_t)zval_copy_static_var,
                      1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1174
              case 5: {
                // ReplaceKind
                zend_hash_apply_with_arguments(
                    (static_variables), (apply_func_args_t)zval_copy_static_var,
                    1, closure->func.op_array.static_variables);

                break;
              }
#endif
#ifdef COMPILE_1175
              case 6: {
                // ReplaceKind
                zend_hash_apply_with_arguments(
                    static_variables, (apply_func_args_t)zval_copy_static_var,
                    1, (closure)->func.op_array.static_variables);

                break;
              }
#endif
              }
            }
          }

          break;
        }
#ifdef COMPILE_1176
        case 1: {
          // AddAndReplaceKind
          printf(static_variables, (apply_func_args_t)zval_copy_static_var, 1,
                 closure->func.op_array.static_variables);

          break;
        }
#endif
#ifdef COMPILE_1177
        case 2: {
          // AddAndReplaceKind
          scanf(static_variables, (apply_func_args_t)zval_copy_static_var, 1,
                closure->func.op_array.static_variables);

          break;
        }
#endif
#ifdef COMPILE_1178
        case 3: {
          // AddAndReplaceKind
          zend_ts_hash_apply_with_arguments(
              static_variables, (apply_func_args_t)zval_copy_static_var, 1,
              closure->func.op_array.static_variables);

          break;
        }
#endif
#ifdef COMPILE_1179
        case 4: {
          // GuardKind
          if (!__is_neg(
                  "32-4", 40, &(closure), sizeof(closure), &(static_variables),
                  sizeof(static_variables),
                  &(closure->func.op_array.static_variables),
                  sizeof(closure->func.op_array.static_variables),
                  &(closure->func.op_array.run_time_cache),
                  sizeof(closure->func.op_array.run_time_cache),
                  &(closure->func.op_array.refcount),
                  sizeof(closure->func.op_array.refcount), &(func),
                  sizeof(func), &(scope), sizeof(scope), &(res), sizeof(res),
                  &(this_ptr), sizeof(this_ptr), &(zend_ce_closure),
                  sizeof(zend_ce_closure), &(zend_ce_closure),
                  sizeof(zend_ce_closure), &(closure->this_ptr),
                  sizeof(closure->this_ptr), &(closure->func.op_array.type),
                  sizeof(closure->func.op_array.type),
                  &(closure->func.op_array.function_name),
                  sizeof(closure->func.op_array.function_name),
                  &(closure->func.op_array.scope),
                  sizeof(closure->func.op_array.scope),
                  &(closure->func.op_array.fn_flags),
                  sizeof(closure->func.op_array.fn_flags),
                  &(closure->func.op_array.prototype),
                  sizeof(closure->func.op_array.prototype),
                  &(closure->func.op_array.num_args),
                  sizeof(closure->func.op_array.num_args),
                  &(closure->func.op_array.required_num_args),
                  sizeof(closure->func.op_array.required_num_args),
                  &(closure->func.op_array.arg_info),
                  sizeof(closure->func.op_array.arg_info),
                  &(closure->func.op_array.opcodes),
                  sizeof(closure->func.op_array.opcodes),
                  &(closure->func.op_array.last),
                  sizeof(closure->func.op_array.last),
                  &(closure->func.op_array.vars),
                  sizeof(closure->func.op_array.vars),
                  &(closure->func.op_array.last_var),
                  sizeof(closure->func.op_array.last_var),
                  &(closure->func.op_array.T), sizeof(closure->func.op_array.T),
                  &(closure->func.op_array.brk_cont_array),
                  sizeof(closure->func.op_array.brk_cont_array),
                  &(closure->func.op_array.last_brk_cont),
                  sizeof(closure->func.op_array.last_brk_cont),
                  &(closure->func.op_array.try_catch_array),
                  sizeof(closure->func.op_array.try_catch_array),
                  &(closure->func.op_array.last_try_catch),
                  sizeof(closure->func.op_array.last_try_catch),
                  &(closure->func.op_array.this_var),
                  sizeof(closure->func.op_array.this_var),
                  &(closure->func.op_array.filename),
                  sizeof(closure->func.op_array.filename),
                  &(closure->func.op_array.line_start),
                  sizeof(closure->func.op_array.line_start),
                  &(closure->func.op_array.line_end),
                  sizeof(closure->func.op_array.line_end),
                  &(closure->func.op_array.doc_comment),
                  sizeof(closure->func.op_array.doc_comment),
                  &(closure->func.op_array.doc_comment_len),
                  sizeof(closure->func.op_array.doc_comment_len),
                  &(closure->func.op_array.early_binding),
                  sizeof(closure->func.op_array.early_binding),
                  &(closure->func.op_array.literals),
                  sizeof(closure->func.op_array.literals),
                  &(closure->func.op_array.last_literal),
                  sizeof(closure->func.op_array.last_literal),
                  &(closure->func.op_array.last_cache_slot),
                  sizeof(closure->func.op_array.last_cache_slot),
                  &(closure->debug_info), sizeof(closure->debug_info)))
            zend_hash_apply_with_arguments(
                static_variables, (apply_func_args_t)zval_copy_static_var, 1,
                closure->func.op_array.static_variables);

          break;
        }
#endif
#ifdef COMPILE_1180
        case 5: {
          // ReplaceKind
          zend_hash_apply_with_arguments(
              (static_variables), (apply_func_args_t)zval_copy_static_var, 1,
              closure->func.op_array.static_variables);

          break;
        }
#endif
#ifdef COMPILE_1181
        case 6: {
          // ReplaceKind
          zend_hash_apply_with_arguments(
              static_variables, (apply_func_args_t)zval_copy_static_var, 1,
              (closure)->func.op_array.static_variables);

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
  // prophet generated patch
  {
    {
      long long __temp33 = (scope);
      switch (__choose("__SWITCH33")) {
      case 0: {

        break;
      }
#ifdef COMPILE_1182
      case 1: {
        __temp33 =
            ((scope) ||
             __is_neg(
                 "33-1", 52, &(scope), sizeof(scope), &(closure),
                 sizeof(closure), &(closure->func.common.scope),
                 sizeof(closure->func.common.scope),
                 &(closure->func.common.fn_flags),
                 sizeof(closure->func.common.fn_flags), &(this_ptr),
                 sizeof(this_ptr), &(closure->this_ptr),
                 sizeof(closure->this_ptr), &(func), sizeof(func),
                 &(scope->name), sizeof(scope->name), &(res), sizeof(res),
                 &(zend_ce_closure), sizeof(zend_ce_closure),
                 &(zend_ce_closure), sizeof(zend_ce_closure), &(scope->type),
                 sizeof(scope->type), &(scope->name_length),
                 sizeof(scope->name_length), &(scope->parent),
                 sizeof(scope->parent), &(scope->refcount),
                 sizeof(scope->refcount), &(scope->ce_flags),
                 sizeof(scope->ce_flags), &(scope->default_properties_table),
                 sizeof(scope->default_properties_table),
                 &(scope->default_static_members_table),
                 sizeof(scope->default_static_members_table),
                 &(scope->static_members_table),
                 sizeof(scope->static_members_table),
                 &(scope->default_properties_count),
                 sizeof(scope->default_properties_count),
                 &(scope->default_static_members_count),
                 sizeof(scope->default_static_members_count),
                 &(scope->constructor), sizeof(scope->constructor),
                 &(scope->destructor), sizeof(scope->destructor),
                 &(scope->clone), sizeof(scope->clone), &(scope->__get),
                 sizeof(scope->__get), &(scope->__set), sizeof(scope->__set),
                 &(scope->__unset), sizeof(scope->__unset), &(scope->__isset),
                 sizeof(scope->__isset), &(scope->__call),
                 sizeof(scope->__call), &(scope->__callstatic),
                 sizeof(scope->__callstatic), &(scope->__tostring),
                 sizeof(scope->__tostring), &(scope->serialize_func),
                 sizeof(scope->serialize_func), &(scope->unserialize_func),
                 sizeof(scope->unserialize_func), &(scope->create_object),
                 sizeof(scope->create_object), &(scope->get_iterator),
                 sizeof(scope->get_iterator),
                 &(scope->interface_gets_implemented),
                 sizeof(scope->interface_gets_implemented),
                 &(scope->get_static_method), sizeof(scope->get_static_method),
                 &(scope->serialize), sizeof(scope->serialize),
                 &(scope->unserialize), sizeof(scope->unserialize),
                 &(scope->interfaces), sizeof(scope->interfaces),
                 &(scope->num_interfaces), sizeof(scope->num_interfaces),
                 &(scope->traits), sizeof(scope->traits), &(scope->num_traits),
                 sizeof(scope->num_traits), &(scope->trait_aliases),
                 sizeof(scope->trait_aliases), &(scope->trait_precedences),
                 sizeof(scope->trait_precedences), &(closure->func.common.type),
                 sizeof(closure->func.common.type),
                 &(closure->func.common.function_name),
                 sizeof(closure->func.common.function_name),
                 &(closure->func.common.prototype),
                 sizeof(closure->func.common.prototype),
                 &(closure->func.common.num_args),
                 sizeof(closure->func.common.num_args),
                 &(closure->func.common.required_num_args),
                 sizeof(closure->func.common.required_num_args),
                 &(closure->func.common.arg_info),
                 sizeof(closure->func.common.arg_info), &(closure->debug_info),
                 sizeof(closure->debug_info)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH36")) {
      case 0: {
        if (__temp33) {
          closure->func.common.fn_flags |= ZEND_ACC_PUBLIC;
          if (this_ptr &&
              (closure->func.common.fn_flags & ZEND_ACC_STATIC) == 0) {
            closure->this_ptr = this_ptr;
            Z_ADDREF_P(this_ptr);
          } else {
            closure->func.common.fn_flags |= ZEND_ACC_STATIC;
            closure->this_ptr = NULL;
          }
        } else {
          {
            switch (__choose("__SWITCH34")) {
            case 0:
              break;
#ifdef COMPILE_1183
            case 1: {
              // AddAndReplaceKind
              ((closure)->func.op_array.static_variables) =
                  (HashTable *)_emalloc((sizeof(HashTable)));

              break;
            }
#endif
#ifdef COMPILE_1184
            case 2: {
              // AddAndReplaceKind
              (*(closure)->func.op_array.refcount)++;

              break;
            }
#endif
#ifdef COMPILE_1185
            case 3: {
              // AddAndReplaceKind
              (*closure->func.op_array.refcount)++;

              break;
            }
#endif
#ifdef COMPILE_1186
            case 4: {
              // AddAndReplaceKind
              (closure) = (zend_closure *)zend_object_store_get_object(res);

              break;
            }
#endif
#ifdef COMPILE_1187
            case 5: {
              // AddAndReplaceKind
              (closure)->func = *func;

              break;
            }
#endif
#ifdef COMPILE_1188
            case 6: {
              // AddAndReplaceKind
              (closure)->func.common.scope = scope;

              break;
            }
#endif
#ifdef COMPILE_1189
            case 7: {
              // AddAndReplaceKind
              (closure)->func.op_array.run_time_cache = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1190
            case 8: {
              // AddAndReplaceKind
              (closure)->this_ptr = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1191
            case 9: {
              // AddAndReplaceKind
              (closure)->this_ptr = this_ptr;

              break;
            }
#endif
#ifdef COMPILE_1192
            case 10: {
              // AddAndReplaceKind
              (closure->func.op_array.static_variables) =
                  (HashTable *)_emalloc((sizeof(HashTable)));

              break;
            }
#endif
#ifdef COMPILE_1193
            case 11: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_1194
            case 12: {
              // AddAndReplaceKind
              (res) = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1195
            case 13: {
              // AddAndReplaceKind
              (scope) = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1196
            case 14: {
              // AddAndReplaceKind
              (scope)->create_object = zend_closure_new;

              break;
            }
#endif
#ifdef COMPILE_1197
            case 15: {
              // AddAndReplaceKind
              (scope)->serialize = zend_class_serialize_deny;

              break;
            }
#endif
#ifdef COMPILE_1198
            case 16: {
              // AddAndReplaceKind
              (scope)->unserialize = zend_class_unserialize_deny;

              break;
            }
#endif
#ifdef COMPILE_1199
            case 17: {
              // AddAndReplaceKind
              (this_ptr) = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1200
            case 18: {
              // AddAndReplaceKind
              _array_init(((res)), 0);

              break;
            }
#endif
#ifdef COMPILE_1201
            case 19: {
              // AddAndReplaceKind
              _array_init(((this_ptr)), 0);

              break;
            }
#endif
#ifdef COMPILE_1202
            case 20: {
              // AddAndReplaceKind
              _efree(((closure)));

              break;
            }
#endif
#ifdef COMPILE_1203
            case 21: {
              // AddAndReplaceKind
              _efree(((closure)->debug_info));

              break;
            }
#endif
#ifdef COMPILE_1204
            case 22: {
              // AddAndReplaceKind
              _efree(((func)));

              break;
            }
#endif
#ifdef COMPILE_1205
            case 23: {
              // AddAndReplaceKind
              _efree(((func)->internal_function.function_name));

              break;
            }
#endif
#ifdef COMPILE_1206
            case 24: {
              // AddAndReplaceKind
              _efree(((res)));

              break;
            }
#endif
#ifdef COMPILE_1207
            case 25: {
              // AddAndReplaceKind
              _efree(((this_ptr)));

              break;
            }
#endif
#ifdef COMPILE_1208
            case 26: {
              // AddAndReplaceKind
              _object_init_ex(((res)), (zend_ce_closure));

              break;
            }
#endif
#ifdef COMPILE_1209
            case 27: {
              // AddAndReplaceKind
              _object_init_ex(((this_ptr)), (zend_ce_closure));

              break;
            }
#endif
#ifdef COMPILE_1210
            case 28: {
              // AddAndReplaceKind
              _object_init_ex((res), ((scope)));

              break;
            }
#endif
#ifdef COMPILE_1211
            case 29: {
              // AddAndReplaceKind
              _object_init_ex((res), (zend_ce_closure));

              break;
            }
#endif
#ifdef COMPILE_1212
            case 30: {
              // AddAndReplaceKind
              _zend_hash_init(((closure)->debug_info), (1), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_1213
            case 31: {
              // AddAndReplaceKind
              _zval_copy_ctor(((res)));

              break;
            }
#endif
#ifdef COMPILE_1214
            case 32: {
              // AddAndReplaceKind
              _zval_copy_ctor(((this_ptr)));

              break;
            }
#endif
#ifdef COMPILE_1215
            case 33: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((res));

              break;
            }
#endif
#ifdef COMPILE_1216
            case 34: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1217
            case 35: {
              // AddAndReplaceKind
              _zval_dtor(((res)));

              break;
            }
#endif
#ifdef COMPILE_1218
            case 36: {
              // AddAndReplaceKind
              _zval_dtor(((this_ptr)));

              break;
            }
#endif
#ifdef COMPILE_1219
            case 37: {
              // AddAndReplaceKind
              _zval_dtor_func((res));

              break;
            }
#endif
#ifdef COMPILE_1220
            case 38: {
              // AddAndReplaceKind
              _zval_dtor_func((this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1221
            case 39: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(closure)->this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1222
            case 40: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(res)));

              break;
            }
#endif
#ifdef COMPILE_1223
            case 41: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(this_ptr)));

              break;
            }
#endif
#ifdef COMPILE_1224
            case 42: {
              // AddAndReplaceKind
              closure = (zend_closure *)zend_object_store_get_object(res);

              break;
            }
#endif
#ifdef COMPILE_1225
            case 43: {
              // AddAndReplaceKind
              closure->func = *(func);

              break;
            }
#endif
#ifdef COMPILE_1226
            case 44: {
              // AddAndReplaceKind
              closure->func = *func;

              break;
            }
#endif
#ifdef COMPILE_1227
            case 45: {
              // AddAndReplaceKind
              closure->func.common.scope = (scope);

              break;
            }
#endif
#ifdef COMPILE_1228
            case 46: {
              // AddAndReplaceKind
              closure->func.common.scope = scope;

              break;
            }
#endif
#ifdef COMPILE_1229
            case 47: {
              // AddAndReplaceKind
              closure->func.op_array.run_time_cache = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1230
            case 48: {
              // AddAndReplaceKind
              closure->this_ptr = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1231
            case 49: {
              // AddAndReplaceKind
              closure->this_ptr = (res);

              break;
            }
#endif
#ifdef COMPILE_1232
            case 50: {
              // AddAndReplaceKind
              closure->this_ptr = (this_ptr);

              break;
            }
#endif
#ifdef COMPILE_1233
            case 51: {
              // AddAndReplaceKind
              closure->this_ptr = this_ptr;

              break;
            }
#endif
#ifdef COMPILE_1234
            case 52: {
              // AddAndReplaceKind
              closure_handlers.clone_obj = zend_closure_clone;

              break;
            }
#endif
#ifdef COMPILE_1235
            case 53: {
              // AddAndReplaceKind
              closure_handlers.compare_objects = zend_closure_compare_objects;

              break;
            }
#endif
#ifdef COMPILE_1236
            case 54: {
              // AddAndReplaceKind
              closure_handlers.get_closure = zend_closure_get_closure;

              break;
            }
#endif
#ifdef COMPILE_1237
            case 55: {
              // AddAndReplaceKind
              closure_handlers.get_constructor = zend_closure_get_constructor;

              break;
            }
#endif
#ifdef COMPILE_1238
            case 56: {
              // AddAndReplaceKind
              closure_handlers.get_debug_info = zend_closure_get_debug_info;

              break;
            }
#endif
#ifdef COMPILE_1239
            case 57: {
              // AddAndReplaceKind
              closure_handlers.get_method = zend_closure_get_method;

              break;
            }
#endif
#ifdef COMPILE_1240
            case 58: {
              // AddAndReplaceKind
              closure_handlers.get_property_ptr_ptr =
                  zend_closure_get_property_ptr_ptr;

              break;
            }
#endif
#ifdef COMPILE_1241
            case 59: {
              // AddAndReplaceKind
              closure_handlers.has_property = zend_closure_has_property;

              break;
            }
#endif
#ifdef COMPILE_1242
            case 60: {
              // AddAndReplaceKind
              closure_handlers.read_property = zend_closure_read_property;

              break;
            }
#endif
#ifdef COMPILE_1243
            case 61: {
              // AddAndReplaceKind
              closure_handlers.unset_property = zend_closure_unset_property;

              break;
            }
#endif
#ifdef COMPILE_1244
            case 62: {
              // AddAndReplaceKind
              closure_handlers.write_property = zend_closure_write_property;

              break;
            }
#endif
#ifdef COMPILE_1245
            case 63: {
              // AddAndReplaceKind
              convert_to_boolean((res));

              break;
            }
#endif
#ifdef COMPILE_1246
            case 64: {
              // AddAndReplaceKind
              convert_to_boolean((this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1247
            case 65: {
              // AddAndReplaceKind
              destroy_op_array(&(closure)->func.op_array);

              break;
            }
#endif
#ifdef COMPILE_1248
            case 66: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_1249
            case 67: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((res));

              break;
            }
#endif
#ifdef COMPILE_1250
            case 68: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1251
            case 69: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((res)));

              break;
            }
#endif
#ifdef COMPILE_1252
            case 70: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((this_ptr)));

              break;
            }
#endif
#ifdef COMPILE_1253
            case 71: {
              // AddAndReplaceKind
              gc_zval_possible_root((res));

              break;
            }
#endif
#ifdef COMPILE_1254
            case 72: {
              // AddAndReplaceKind
              gc_zval_possible_root((this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1255
            case 73: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((res));

              break;
            }
#endif
#ifdef COMPILE_1256
            case 74: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1257
            case 75: {
              // AddAndReplaceKind
              if (scope && !instanceof_function(scope, func->common.scope)) {
                zend_error((1 << 1L),
                           "Cannot bind function %s::%s to scope class %s",
                           func->common.scope->name, func->common.function_name,
                           scope->name);
                scope = ((void *)0);
              }

              break;
            }
#endif
#ifdef COMPILE_1258
            case 76: {
              // AddAndReplaceKind
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

              break;
            }
#endif
#ifdef COMPILE_1259
            case 77: {
              // AddAndReplaceKind
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
#ifdef COMPILE_1260
            case 78: {
              // AddAndReplaceKind
              memcpy(&closure_handlers, zend_get_std_object_handlers(),
                     sizeof(zend_object_handlers));

              break;
            }
#endif
#ifdef COMPILE_1261
            case 79: {
              // AddAndReplaceKind
              memset((closure), 0, sizeof(zend_closure));

              break;
            }
#endif
#ifdef COMPILE_1262
            case 80: {
              // AddAndReplaceKind
              scope = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1263
            case 81: {
              // AddAndReplaceKind
              this_ptr = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1264
            case 82: {
              // AddAndReplaceKind
              zend_ce_closure->create_object = zend_closure_new;

              break;
            }
#endif
#ifdef COMPILE_1265
            case 83: {
              // AddAndReplaceKind
              zend_ce_closure->serialize = zend_class_serialize_deny;

              break;
            }
#endif
#ifdef COMPILE_1266
            case 84: {
              // AddAndReplaceKind
              zend_ce_closure->unserialize = zend_class_unserialize_deny;

              break;
            }
#endif
#ifdef COMPILE_1267
            case 85: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Cannot destroy active lambda function");

              break;
            }
#endif
#ifdef COMPILE_1268
            case 86: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Cannot get arguments for calling closure");

              break;
            }
#endif
#ifdef COMPILE_1269
            case 87: {
              // AddAndReplaceKind
              zend_error((1 << 12L), "Closure object cannot have properties");

              break;
            }
#endif
#ifdef COMPILE_1270
            case 88: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Instantiation of 'Closure' is not allowed");

              break;
            }
#endif
#ifdef COMPILE_1271
            case 89: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to object of class %s",
                         (func)->common.scope->name, func->common.function_name,
                         zend_get_class_entry(&(*this_ptr))->name);

              break;
            }
#endif
#ifdef COMPILE_1272
            case 90: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to object of class %s",
                         func->common.scope->name, (func)->common.function_name,
                         zend_get_class_entry(&(*this_ptr))->name);

              break;
            }
#endif
#ifdef COMPILE_1273
            case 91: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to object of class %s",
                         func->common.scope->name, func->common.function_name,
                         zend_get_class_entry(&(*(res)))->name);

              break;
            }
#endif
#ifdef COMPILE_1274
            case 92: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to object of class %s",
                         func->common.scope->name, func->common.function_name,
                         zend_get_class_entry(&(*(this_ptr)))->name);

              break;
            }
#endif
#ifdef COMPILE_1275
            case 93: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to object of class %s",
                         func->common.scope->name, func->common.function_name,
                         zend_get_class_entry(&(*this_ptr))->name);

              break;
            }
#endif
#ifdef COMPILE_1276
            case 94: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to scope class %s",
                         (func)->common.scope->name, func->common.function_name,
                         scope->name);

              break;
            }
#endif
#ifdef COMPILE_1277
            case 95: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to scope class %s",
                         func->common.scope->name, (func)->common.function_name,
                         scope->name);

              break;
            }
#endif
#ifdef COMPILE_1278
            case 96: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to scope class %s",
                         func->common.scope->name, func->common.function_name,
                         (scope)->name);

              break;
            }
#endif
#ifdef COMPILE_1279
            case 97: {
              // AddAndReplaceKind
              zend_error((1 << 1L),
                         "Cannot bind function %s::%s to scope class %s",
                         func->common.scope->name, func->common.function_name,
                         scope->name);

              break;
            }
#endif
#ifdef COMPILE_1280
            case 98: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_1281
            case 99: {
              // AddAndReplaceKind
              zend_hash_destroy((closure)->debug_info);

              break;
            }
#endif
#ifdef COMPILE_1282
            case 100: {
              // AddAndReplaceKind
              zend_object_std_dtor(&(closure)->std);

              break;
            }
#endif
#ifdef COMPILE_1283
            case 101: {
              // AddAndReplaceKind
              zval_addref_p(&((executor_globals.uninitialized_zval)));

              break;
            }
#endif
#ifdef COMPILE_1284
            case 102: {
              // AddAndReplaceKind
              zval_addref_p((closure)->this_ptr);

              break;
            }
#endif
#ifdef COMPILE_1285
            case 103: {
              // AddAndReplaceKind
              zval_addref_p((res));

              break;
            }
#endif
#ifdef COMPILE_1286
            case 104: {
              // AddAndReplaceKind
              zval_addref_p((this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1287
            case 105: {
              // AddAndReplaceKind
              zval_addref_p(this_ptr);

              break;
            }
#endif
#ifdef COMPILE_1288
            case 106: {
              // AddAndReplaceKind
              zval_unset_isref_p((res));

              break;
            }
#endif
#ifdef COMPILE_1289
            case 107: {
              // AddAndReplaceKind
              zval_unset_isref_p((this_ptr));

              break;
            }
#endif
#ifdef COMPILE_1290
            case 108: {
              // AddInitKind
              memset(closure, 0, sizeof(*(closure)));

              break;
            }
#endif
#ifdef COMPILE_1291
            case 109: {
              // IfExitKind
              if (__is_neg("34-109", 9, &(closure), sizeof(closure),
                           &(closure->this_ptr), sizeof(closure->this_ptr),
                           &(this_ptr), sizeof(this_ptr), &(scope),
                           sizeof(scope), &(func), sizeof(func), &(res),
                           sizeof(res), &(zend_ce_closure),
                           sizeof(zend_ce_closure), &(zend_ce_closure),
                           sizeof(zend_ce_closure), &(closure->debug_info),
                           sizeof(closure->debug_info)))
                return;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH35")) {
            case 0: {
              closure->this_ptr = NULL;

              break;
            }
#ifdef COMPILE_1292
            case 1: {
              // GuardKind
              if (!__is_neg("35-1", 9, &(closure), sizeof(closure),
                            &(closure->this_ptr), sizeof(closure->this_ptr),
                            &(this_ptr), sizeof(this_ptr), &(scope),
                            sizeof(scope), &(func), sizeof(func), &(res),
                            sizeof(res), &(zend_ce_closure),
                            sizeof(zend_ce_closure), &(zend_ce_closure),
                            sizeof(zend_ce_closure), &(closure->debug_info),
                            sizeof(closure->debug_info)))
                closure->this_ptr = ((void *)0);

              break;
            }
#endif
#ifdef COMPILE_1293
            case 2: {
              // ReplaceKind
              (closure)->this_ptr = ((void *)0);

              break;
            }
#endif
            }
          }
        }

        break;
      }
#ifdef COMPILE_1294
      case 1: {
        // GuardKind
        if (!__is_neg("36-1", 9, &(closure), sizeof(closure),
                      &(closure->this_ptr), sizeof(closure->this_ptr),
                      &(this_ptr), sizeof(this_ptr), &(scope), sizeof(scope),
                      &(func), sizeof(func), &(res), sizeof(res),
                      &(zend_ce_closure), sizeof(zend_ce_closure),
                      &(zend_ce_closure), sizeof(zend_ce_closure),
                      &(closure->debug_info), sizeof(closure->debug_info)))
          closure->this_ptr = ((void *)0);

        break;
      }
#endif
#ifdef COMPILE_1295
      case 2: {
        // ReplaceKind
        (closure)->this_ptr = ((void *)0);

        break;
      }
#endif
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
