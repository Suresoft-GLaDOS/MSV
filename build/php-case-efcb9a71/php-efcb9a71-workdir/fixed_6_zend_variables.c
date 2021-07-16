int __get_mutant();
int __is_neg(const char *location, int count, ...);
int __abst_hole();
int __choose(const char *);
void *memset(void *, int, unsigned long);
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

#include "zend.h"
#include "zend_API.h"
#include "zend_constants.h"
#include "zend_globals.h"
#include "zend_list.h"
#include <stdio.h>

ZEND_API void _zval_dtor_func(zval *zvalue ZEND_FILE_LINE_DC) {
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
  } break;
  case IS_OBJECT: {
    TSRMLS_FETCH();

    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);
  } break;
  case IS_RESOURCE: {
    TSRMLS_FETCH();

    /* destroy resource */
    zend_list_delete(zvalue->value.lval);
  } break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }
}

ZEND_API void _zval_internal_dtor(zval *zvalue ZEND_FILE_LINE_DC) {
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
    zend_error(E_CORE_ERROR,
               "Internal zval's can't be arrays, objects or resources");
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    break;
  }
}

ZEND_API void zval_add_ref(zval **p) { Z_ADDREF_PP(p); }

ZEND_API void _zval_copy_ctor_func(zval *zvalue ZEND_FILE_LINE_DC) {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_RESOURCE: {
    TSRMLS_FETCH();

    zend_list_addref(zvalue->value.lval);
  } break;
  case IS_BOOL:
  case IS_LONG:
  case IS_NULL:
    break;
  case IS_CONSTANT:
  case IS_STRING:
    CHECK_ZVAL_STRING_REL(zvalue);
    if (!IS_INTERNED(zvalue->value.str.val)) {
      zvalue->value.str.val =
          (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
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
    zend_hash_init(tmp_ht, zend_hash_num_elements(original_ht), NULL,
                   ZVAL_PTR_DTOR, 0);
    zend_hash_copy(tmp_ht, original_ht, (copy_ctor_func_t)zval_add_ref,
                   (void *)&tmp, sizeof(zval *));
    zvalue->value.ht = tmp_ht;
  } break;
  case IS_OBJECT: {
    TSRMLS_FETCH();
    Z_OBJ_HT_P(zvalue)->add_ref(zvalue TSRMLS_CC);
  } break;
  }
}

ZEND_API int zend_print_variable(zval *var) { return zend_print_zval(var, 0); }

ZEND_API void _zval_dtor_wrapper(zval *zvalue) {
  TSRMLS_FETCH();

  GC_REMOVE_ZVAL_FROM_BUFFER(zvalue);
  zval_dtor(zvalue);
}

#if ZEND_DEBUG
ZEND_API void _zval_copy_ctor_wrapper(zval *zvalue) { zval_copy_ctor(zvalue); }

ZEND_API void _zval_internal_dtor_wrapper(zval *zvalue) {
  zval_internal_dtor(zvalue);
}

ZEND_API void _zval_ptr_dtor_wrapper(zval **zval_ptr) {
  zval_ptr_dtor(zval_ptr);
}

ZEND_API void _zval_internal_ptr_dtor_wrapper(zval **zval_ptr) {
  zval_internal_ptr_dtor(zval_ptr);
}
#endif

ZEND_API int zval_copy_static_var(zval **p TSRMLS_DC, int num_args,
                                  va_list args, zend_hash_key *key) /* {{{ */
{
  HashTable *target = va_arg(args, HashTable *);
  zend_bool is_ref;
  zval *tmp;

  // prophet generated patch
  {
    switch (__choose("__SWITCH227")) {
    case 0:
      break;
#ifdef COMPILE_21390
    case 1: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_21391
    case 2: {
      // AddAndReplaceKind
      (tmp) = (executor_globals.uninitialized_zval_ptr);

      break;
    }
#endif
#ifdef COMPILE_21392
    case 3: {
      // AddAndReplaceKind
      (tmp) = *p;

      break;
    }
#endif
#ifdef COMPILE_21393
    case 4: {
      // AddAndReplaceKind
      *(tmp) = zval_used_for_init;

      break;
    }
#endif
#ifdef COMPILE_21394
    case 5: {
      // AddAndReplaceKind
      *tmp = zval_used_for_init;

      break;
    }
#endif
#ifdef COMPILE_21395
    case 6: {
      // AddAndReplaceKind
      _efree(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_21396
    case 7: {
      // AddAndReplaceKind
      _efree(((tmp)->value.ht));

      break;
    }
#endif
#ifdef COMPILE_21397
    case 8: {
      // AddAndReplaceKind
      _efree(((tmp)->value.str.val));

      break;
    }
#endif
#ifdef COMPILE_21398
    case 9: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_21399
    case 10: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_21400
    case 11: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_21401
    case 12: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_21402
    case 13: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                     key->arKey, key->nKeyLength, key->h, &tmp,
                                     sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_21403
    case 14: {
      // AddAndReplaceKind
      _zend_list_addref((tmp)->value.lval);

      break;
    }
#endif
#ifdef COMPILE_21404
    case 15: {
      // AddAndReplaceKind
      _zend_list_delete((tmp)->value.lval);

      break;
    }
#endif
#ifdef COMPILE_21405
    case 16: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((tmp));

      break;
    }
#endif
#ifdef COMPILE_21406
    case 17: {
      // AddAndReplaceKind
      _zval_dtor(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_21407
    case 18: {
      // AddAndReplaceKind
      _zval_dtor_func((tmp));

      break;
    }
#endif
#ifdef COMPILE_21408
    case 19: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(tmp)));

      break;
    }
#endif
#ifdef COMPILE_21409
    case 20: {
      // AddAndReplaceKind
      convert_to_boolean((tmp));

      break;
    }
#endif
#ifdef COMPILE_21410
    case 21: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_21411
    case 22: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((tmp));

      break;
    }
#endif
#ifdef COMPILE_21412
    case 23: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_21413
    case 24: {
      // AddAndReplaceKind
      gc_zval_possible_root((tmp));

      break;
    }
#endif
#ifdef COMPILE_21414
    case 25: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((tmp));

      break;
    }
#endif
#ifdef COMPILE_21415
    case 26: {
      // AddAndReplaceKind
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_21416
    case 27: {
      // AddAndReplaceKind
      if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                         key->h, &tmp, sizeof(zval *),
                                         ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }

      break;
    }
#endif
#ifdef COMPILE_21417
    case 28: {
      // AddAndReplaceKind
      is_ref = (**(p)).type & 64;

      break;
    }
#endif
#ifdef COMPILE_21418
    case 29: {
      // AddAndReplaceKind
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_21419
    case 30: {
      // AddAndReplaceKind
      tmp = (executor_globals.uninitialized_zval_ptr);

      break;
    }
#endif
#ifdef COMPILE_21420
    case 31: {
      // AddAndReplaceKind
      tmp = *(p);

      break;
    }
#endif
#ifdef COMPILE_21421
    case 32: {
      // AddAndReplaceKind
      tmp = *p;

      break;
    }
#endif
#ifdef COMPILE_21422
    case 33: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_21423
    case 34: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);

      break;
    }
#endif
#ifdef COMPILE_21424
    case 35: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Undefined variable: %s", key->arKey);

      break;
    }
#endif
#ifdef COMPILE_21425
    case 36: {
      // AddAndReplaceKind
      zend_hash_destroy((tmp)->value.ht);

      break;
    }
#endif
#ifdef COMPILE_21426
    case 37: {
      // AddAndReplaceKind
      zend_rebuild_symbol_table();

      break;
    }
#endif
#ifdef COMPILE_21427
    case 38: {
      // AddAndReplaceKind
      zval_addref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_21428
    case 39: {
      // AddAndReplaceKind
      zval_addref_p(*((p)));

      break;
    }
#endif
#ifdef COMPILE_21429
    case 40: {
      // AddAndReplaceKind
      zval_addref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_21430
    case 41: {
      // AddAndReplaceKind
      zval_set_isref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_21431
    case 42: {
      // AddAndReplaceKind
      zval_set_isref_p(*(((p))));

      break;
    }
#endif
#ifdef COMPILE_21432
    case 43: {
      // AddAndReplaceKind
      zval_set_isref_p(*((p)));

      break;
    }
#endif
#ifdef COMPILE_21433
    case 44: {
      // AddAndReplaceKind
      zval_set_isref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_21434
    case 45: {
      // AddAndReplaceKind
      zval_set_refcount_p((tmp), 0);

      break;
    }
#endif
#ifdef COMPILE_21435
    case 46: {
      // AddAndReplaceKind
      zval_set_refcount_p(tmp, 0);

      break;
    }
#endif
#ifdef COMPILE_21436
    case 47: {
      // AddAndReplaceKind
      zval_unset_isref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_21437
    case 48: {
      // AddAndReplaceKind
      zval_unset_isref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_21438
    case 49: {
      // AddInitKind
      memset(&((zval_gc_info *)(tmp))->u, 0,
             sizeof(*(&((zval_gc_info *)(tmp))->u)));

      break;
    }
#endif
#ifdef COMPILE_21439
    case 50: {
      // AddInitKind
      memset(&(**(p)), 0, sizeof(*(&(**(p)))));

      break;
    }
#endif
#ifdef COMPILE_21440
    case 51: {
      // AddInitKind
      memset(&(**p), 0, sizeof(*(&(**p))));

      break;
    }
#endif
#ifdef COMPILE_21441
    case 52: {
      // AddInitKind
      memset(&(*tmp), 0, sizeof(*(&(*tmp))));

      break;
    }
#endif
#ifdef COMPILE_21442
    case 53: {
      // AddInitKind
      memset(&executor_globals, 0, sizeof(*(&executor_globals)));

      break;
    }
#endif
#ifdef COMPILE_21443
    case 54: {
      // AddInitKind
      memset(((zval_gc_info *)(tmp)), 0, sizeof(*(((zval_gc_info *)(tmp)))));

      break;
    }
#endif
#ifdef COMPILE_21444
    case 55: {
      // AddInitKind
      memset((*(p)), 0, sizeof(*((*(p)))));

      break;
    }
#endif
#ifdef COMPILE_21445
    case 56: {
      // AddInitKind
      memset((*p), 0, sizeof(*((*p))));

      break;
    }
#endif
#ifdef COMPILE_21446
    case 57: {
      // AddInitKind
      memset((tmp), 0, sizeof(*((tmp))));

      break;
    }
#endif
#ifdef COMPILE_21447
    case 58: {
      // AddInitKind
      memset(key, 0, sizeof(*(key)));

      break;
    }
#endif
#ifdef COMPILE_21448
    case 59: {
      // IfExitKind
      if (__is_neg("227-59", 9, &(p), sizeof(p), &((**p).type),
                   sizeof((**p).type), &(is_ref), sizeof(is_ref), &(args),
                   sizeof(args), &(key), sizeof(key), &(tmp), sizeof(tmp),
                   &(target), sizeof(target), &((**p).refcount__gc),
                   sizeof((**p).refcount__gc), &((**p).is_ref__gc),
                   sizeof((**p).is_ref__gc)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_21449
    case 60: {
      // IfExitKind
      if (__is_neg("227-60", 9, &(p), sizeof(p), &((**p).type),
                   sizeof((**p).type), &(is_ref), sizeof(is_ref), &(args),
                   sizeof(args), &(key), sizeof(key), &(tmp), sizeof(tmp),
                   &(target), sizeof(target), &((**p).refcount__gc),
                   sizeof((**p).refcount__gc), &((**p).is_ref__gc),
                   sizeof((**p).is_ref__gc)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_21450
    case 61: {
      // IfExitKind
      if (__is_neg("227-61", 9, &(p), sizeof(p), &((**p).type),
                   sizeof((**p).type), &(is_ref), sizeof(is_ref), &(args),
                   sizeof(args), &(key), sizeof(key), &(tmp), sizeof(tmp),
                   &(target), sizeof(target), &((**p).refcount__gc),
                   sizeof((**p).refcount__gc), &((**p).is_ref__gc),
                   sizeof((**p).is_ref__gc)))
        return 32;

      break;
    }
#endif
#ifdef COMPILE_21451
    case 62: {
      // IfExitKind
      if (__is_neg("227-62", 9, &(p), sizeof(p), &((**p).type),
                   sizeof((**p).type), &(is_ref), sizeof(is_ref), &(args),
                   sizeof(args), &(key), sizeof(key), &(tmp), sizeof(tmp),
                   &(target), sizeof(target), &((**p).refcount__gc),
                   sizeof((**p).refcount__gc), &((**p).is_ref__gc),
                   sizeof((**p).is_ref__gc)))
        return 3;

      break;
    }
#endif
#ifdef COMPILE_21452
    case 63: {
      // IfExitKind
      if (__is_neg("227-63", 9, &(p), sizeof(p), &((**p).type),
                   sizeof((**p).type), &(is_ref), sizeof(is_ref), &(args),
                   sizeof(args), &(key), sizeof(key), &(tmp), sizeof(tmp),
                   &(target), sizeof(target), &((**p).refcount__gc),
                   sizeof((**p).refcount__gc), &((**p).is_ref__gc),
                   sizeof((**p).is_ref__gc)))
        return 64;

      break;
    }
#endif
    }
    {
      long long __temp228 = ((**p).type & (32 | 64));
      switch (__choose("__SWITCH228")) {
      case 0: {

        break;
      }
#ifdef COMPILE_21453
      case 1: {
        __temp228 = (((**p).type & (32 | 64)) ||
                     __is_neg("228-1", 9, &(p), sizeof(p), &((**p).type),
                              sizeof((**p).type), &(is_ref), sizeof(is_ref),
                              &(args), sizeof(args), &(key), sizeof(key),
                              &(tmp), sizeof(tmp), &(target), sizeof(target),
                              &((**p).refcount__gc), sizeof((**p).refcount__gc),
                              &((**p).is_ref__gc), sizeof((**p).is_ref__gc)));

        break;
      }
#endif
#ifdef COMPILE_21454
      case 2: {
        __temp228 =
            (((**p).type & (32 | 64)) &&
             !__is_neg("228-2", 9, &(p), sizeof(p), &((**p).type),
                       sizeof((**p).type), &(is_ref), sizeof(is_ref), &(args),
                       sizeof(args), &(key), sizeof(key), &(tmp), sizeof(tmp),
                       &(target), sizeof(target), &((**p).refcount__gc),
                       sizeof((**p).refcount__gc), &((**p).is_ref__gc),
                       sizeof((**p).is_ref__gc)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH253")) {
      case 0: {
        if (__temp228) {
          {
            switch (__choose("__SWITCH229")) {
            case 0:
              break;
#ifdef COMPILE_21455
            case 1: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_21456
            case 2: {
              // AddAndReplaceKind
              (tmp) = (executor_globals.uninitialized_zval_ptr);

              break;
            }
#endif
#ifdef COMPILE_21457
            case 3: {
              // AddAndReplaceKind
              (tmp) = *p;

              break;
            }
#endif
#ifdef COMPILE_21458
            case 4: {
              // AddAndReplaceKind
              *(tmp) = zval_used_for_init;

              break;
            }
#endif
#ifdef COMPILE_21459
            case 5: {
              // AddAndReplaceKind
              *tmp = zval_used_for_init;

              break;
            }
#endif
#ifdef COMPILE_21460
            case 6: {
              // AddAndReplaceKind
              _efree(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21461
            case 7: {
              // AddAndReplaceKind
              _efree(((tmp)->value.ht));

              break;
            }
#endif
#ifdef COMPILE_21462
            case 8: {
              // AddAndReplaceKind
              _efree(((tmp)->value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21463
            case 9: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), (key)->arKey,
                  key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21464
            case 10: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21465
            case 11: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21466
            case 12: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21467
            case 13: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21468
            case 14: {
              // AddAndReplaceKind
              _zend_list_addref((tmp)->value.lval);

              break;
            }
#endif
#ifdef COMPILE_21469
            case 15: {
              // AddAndReplaceKind
              _zend_list_delete((tmp)->value.lval);

              break;
            }
#endif
#ifdef COMPILE_21470
            case 16: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((tmp));

              break;
            }
#endif
#ifdef COMPILE_21471
            case 17: {
              // AddAndReplaceKind
              _zval_dtor(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21472
            case 18: {
              // AddAndReplaceKind
              _zval_dtor_func((tmp));

              break;
            }
#endif
#ifdef COMPILE_21473
            case 19: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(tmp)));

              break;
            }
#endif
#ifdef COMPILE_21474
            case 20: {
              // AddAndReplaceKind
              convert_to_boolean((tmp));

              break;
            }
#endif
#ifdef COMPILE_21475
            case 21: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_21476
            case 22: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((tmp));

              break;
            }
#endif
#ifdef COMPILE_21477
            case 23: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21478
            case 24: {
              // AddAndReplaceKind
              gc_zval_possible_root((tmp));

              break;
            }
#endif
#ifdef COMPILE_21479
            case 25: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((tmp));

              break;
            }
#endif
#ifdef COMPILE_21480
            case 26: {
              // AddAndReplaceKind
              if (!(executor_globals.active_symbol_table)) {
                zend_rebuild_symbol_table();
              }

              break;
            }
#endif
#ifdef COMPILE_21481
            case 27: {
              // AddAndReplaceKind
              if (_zend_hash_quick_add_or_update(
                      target, key->arKey, key->nKeyLength, key->h, &tmp,
                      sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
                zval_addref_p(tmp);
              }

              break;
            }
#endif
#ifdef COMPILE_21482
            case 28: {
              // AddAndReplaceKind
              is_ref = (**(p)).type & 64;

              break;
            }
#endif
#ifdef COMPILE_21483
            case 29: {
              // AddAndReplaceKind
              is_ref = (**p).type & 64;

              break;
            }
#endif
#ifdef COMPILE_21484
            case 30: {
              // AddAndReplaceKind
              tmp = (executor_globals.uninitialized_zval_ptr);

              break;
            }
#endif
#ifdef COMPILE_21485
            case 31: {
              // AddAndReplaceKind
              tmp = *(p);

              break;
            }
#endif
#ifdef COMPILE_21486
            case 32: {
              // AddAndReplaceKind
              tmp = *p;

              break;
            }
#endif
#ifdef COMPILE_21487
            case 33: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_21488
            case 34: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);

              break;
            }
#endif
#ifdef COMPILE_21489
            case 35: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Undefined variable: %s", key->arKey);

              break;
            }
#endif
#ifdef COMPILE_21490
            case 36: {
              // AddAndReplaceKind
              zend_hash_destroy((tmp)->value.ht);

              break;
            }
#endif
#ifdef COMPILE_21491
            case 37: {
              // AddAndReplaceKind
              zend_rebuild_symbol_table();

              break;
            }
#endif
#ifdef COMPILE_21492
            case 38: {
              // AddAndReplaceKind
              zval_addref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21493
            case 39: {
              // AddAndReplaceKind
              zval_addref_p(*((p)));

              break;
            }
#endif
#ifdef COMPILE_21494
            case 40: {
              // AddAndReplaceKind
              zval_addref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21495
            case 41: {
              // AddAndReplaceKind
              zval_set_isref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21496
            case 42: {
              // AddAndReplaceKind
              zval_set_isref_p(*(((p))));

              break;
            }
#endif
#ifdef COMPILE_21497
            case 43: {
              // AddAndReplaceKind
              zval_set_isref_p(*((p)));

              break;
            }
#endif
#ifdef COMPILE_21498
            case 44: {
              // AddAndReplaceKind
              zval_set_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21499
            case 45: {
              // AddAndReplaceKind
              zval_set_refcount_p((tmp), 0);

              break;
            }
#endif
#ifdef COMPILE_21500
            case 46: {
              // AddAndReplaceKind
              zval_set_refcount_p(tmp, 0);

              break;
            }
#endif
#ifdef COMPILE_21501
            case 47: {
              // AddAndReplaceKind
              zval_unset_isref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21502
            case 48: {
              // AddAndReplaceKind
              zval_unset_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21503
            case 49: {
              // AddInitKind
              memset(&(**p), 0, sizeof(*(&(**p))));

              break;
            }
#endif
#ifdef COMPILE_21504
            case 50: {
              // IfExitKind
              if (__is_neg("229-50", 9, &(p), sizeof(p), &(is_ref),
                           sizeof(is_ref), &((**p).type), sizeof((**p).type),
                           &(args), sizeof(args), &(key), sizeof(key), &(tmp),
                           sizeof(tmp), &(target), sizeof(target),
                           &((**p).refcount__gc), sizeof((**p).refcount__gc),
                           &((**p).is_ref__gc), sizeof((**p).is_ref__gc)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_21505
            case 51: {
              // IfExitKind
              if (__is_neg("229-51", 9, &(p), sizeof(p), &(is_ref),
                           sizeof(is_ref), &((**p).type), sizeof((**p).type),
                           &(args), sizeof(args), &(key), sizeof(key), &(tmp),
                           sizeof(tmp), &(target), sizeof(target),
                           &((**p).refcount__gc), sizeof((**p).refcount__gc),
                           &((**p).is_ref__gc), sizeof((**p).is_ref__gc)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_21506
            case 52: {
              // IfExitKind
              if (__is_neg("229-52", 9, &(p), sizeof(p), &(is_ref),
                           sizeof(is_ref), &((**p).type), sizeof((**p).type),
                           &(args), sizeof(args), &(key), sizeof(key), &(tmp),
                           sizeof(tmp), &(target), sizeof(target),
                           &((**p).refcount__gc), sizeof((**p).refcount__gc),
                           &((**p).is_ref__gc), sizeof((**p).is_ref__gc)))
                return 32;

              break;
            }
#endif
#ifdef COMPILE_21507
            case 53: {
              // IfExitKind
              if (__is_neg("229-53", 9, &(p), sizeof(p), &(is_ref),
                           sizeof(is_ref), &((**p).type), sizeof((**p).type),
                           &(args), sizeof(args), &(key), sizeof(key), &(tmp),
                           sizeof(tmp), &(target), sizeof(target),
                           &((**p).refcount__gc), sizeof((**p).refcount__gc),
                           &((**p).is_ref__gc), sizeof((**p).is_ref__gc)))
                return 3;

              break;
            }
#endif
#ifdef COMPILE_21508
            case 54: {
              // IfExitKind
              if (__is_neg("229-54", 9, &(p), sizeof(p), &(is_ref),
                           sizeof(is_ref), &((**p).type), sizeof((**p).type),
                           &(args), sizeof(args), &(key), sizeof(key), &(tmp),
                           sizeof(tmp), &(target), sizeof(target),
                           &((**p).refcount__gc), sizeof((**p).refcount__gc),
                           &((**p).is_ref__gc), sizeof((**p).is_ref__gc)))
                return 64;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH230")) {
            case 0: {
              is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

              break;
            }
#ifdef COMPILE_21509
            case 1: {
              // GuardKind
              if (!__is_neg("230-1", 9, &(p), sizeof(p), &(is_ref),
                            sizeof(is_ref), &((**p).type), sizeof((**p).type),
                            &(args), sizeof(args), &(key), sizeof(key), &(tmp),
                            sizeof(tmp), &(target), sizeof(target),
                            &((**p).refcount__gc), sizeof((**p).refcount__gc),
                            &((**p).is_ref__gc), sizeof((**p).is_ref__gc)))
                is_ref = (**p).type & 64;

              break;
            }
#endif
#ifdef COMPILE_21510
            case 2: {
              // ReplaceKind
              is_ref = (**(p)).type & 64;

              break;
            }
#endif
            }
          }

          {
            switch (__choose("__SWITCH231")) {
            case 0:
              break;
#ifdef COMPILE_21511
            case 1: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_21512
            case 2: {
              // AddAndReplaceKind
              (tmp) = (executor_globals.uninitialized_zval_ptr);

              break;
            }
#endif
#ifdef COMPILE_21513
            case 3: {
              // AddAndReplaceKind
              (tmp) = *p;

              break;
            }
#endif
#ifdef COMPILE_21514
            case 4: {
              // AddAndReplaceKind
              *(tmp) = zval_used_for_init;

              break;
            }
#endif
#ifdef COMPILE_21515
            case 5: {
              // AddAndReplaceKind
              *tmp = zval_used_for_init;

              break;
            }
#endif
#ifdef COMPILE_21516
            case 6: {
              // AddAndReplaceKind
              _efree(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21517
            case 7: {
              // AddAndReplaceKind
              _efree(((tmp)->value.ht));

              break;
            }
#endif
#ifdef COMPILE_21518
            case 8: {
              // AddAndReplaceKind
              _efree(((tmp)->value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21519
            case 9: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), (key)->arKey,
                  key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21520
            case 10: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21521
            case 11: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21522
            case 12: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21523
            case 13: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21524
            case 14: {
              // AddAndReplaceKind
              _zend_list_addref((tmp)->value.lval);

              break;
            }
#endif
#ifdef COMPILE_21525
            case 15: {
              // AddAndReplaceKind
              _zend_list_delete((tmp)->value.lval);

              break;
            }
#endif
#ifdef COMPILE_21526
            case 16: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((tmp));

              break;
            }
#endif
#ifdef COMPILE_21527
            case 17: {
              // AddAndReplaceKind
              _zval_dtor(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21528
            case 18: {
              // AddAndReplaceKind
              _zval_dtor_func((tmp));

              break;
            }
#endif
#ifdef COMPILE_21529
            case 19: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(tmp)));

              break;
            }
#endif
#ifdef COMPILE_21530
            case 20: {
              // AddAndReplaceKind
              convert_to_boolean((tmp));

              break;
            }
#endif
#ifdef COMPILE_21531
            case 21: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_21532
            case 22: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((tmp));

              break;
            }
#endif
#ifdef COMPILE_21533
            case 23: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21534
            case 24: {
              // AddAndReplaceKind
              gc_zval_possible_root((tmp));

              break;
            }
#endif
#ifdef COMPILE_21535
            case 25: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((tmp));

              break;
            }
#endif
#ifdef COMPILE_21536
            case 26: {
              // AddAndReplaceKind
              if (!(executor_globals.active_symbol_table)) {
                zend_rebuild_symbol_table();
              }

              break;
            }
#endif
#ifdef COMPILE_21537
            case 27: {
              // AddAndReplaceKind
              if (_zend_hash_quick_add_or_update(
                      target, key->arKey, key->nKeyLength, key->h, &tmp,
                      sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
                zval_addref_p(tmp);
              }

              break;
            }
#endif
#ifdef COMPILE_21538
            case 28: {
              // AddAndReplaceKind
              is_ref = (**(p)).type & 64;

              break;
            }
#endif
#ifdef COMPILE_21539
            case 29: {
              // AddAndReplaceKind
              is_ref = (**p).type & 64;

              break;
            }
#endif
#ifdef COMPILE_21540
            case 30: {
              // AddAndReplaceKind
              tmp = (executor_globals.uninitialized_zval_ptr);

              break;
            }
#endif
#ifdef COMPILE_21541
            case 31: {
              // AddAndReplaceKind
              tmp = *(p);

              break;
            }
#endif
#ifdef COMPILE_21542
            case 32: {
              // AddAndReplaceKind
              tmp = *p;

              break;
            }
#endif
#ifdef COMPILE_21543
            case 33: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_21544
            case 34: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);

              break;
            }
#endif
#ifdef COMPILE_21545
            case 35: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Undefined variable: %s", key->arKey);

              break;
            }
#endif
#ifdef COMPILE_21546
            case 36: {
              // AddAndReplaceKind
              zend_hash_destroy((tmp)->value.ht);

              break;
            }
#endif
#ifdef COMPILE_21547
            case 37: {
              // AddAndReplaceKind
              zend_rebuild_symbol_table();

              break;
            }
#endif
#ifdef COMPILE_21548
            case 38: {
              // AddAndReplaceKind
              zval_addref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21549
            case 39: {
              // AddAndReplaceKind
              zval_addref_p(*((p)));

              break;
            }
#endif
#ifdef COMPILE_21550
            case 40: {
              // AddAndReplaceKind
              zval_addref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21551
            case 41: {
              // AddAndReplaceKind
              zval_set_isref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21552
            case 42: {
              // AddAndReplaceKind
              zval_set_isref_p(*(((p))));

              break;
            }
#endif
#ifdef COMPILE_21553
            case 43: {
              // AddAndReplaceKind
              zval_set_isref_p(*((p)));

              break;
            }
#endif
#ifdef COMPILE_21554
            case 44: {
              // AddAndReplaceKind
              zval_set_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21555
            case 45: {
              // AddAndReplaceKind
              zval_set_refcount_p((tmp), 0);

              break;
            }
#endif
#ifdef COMPILE_21556
            case 46: {
              // AddAndReplaceKind
              zval_set_refcount_p(tmp, 0);

              break;
            }
#endif
#ifdef COMPILE_21557
            case 47: {
              // AddAndReplaceKind
              zval_unset_isref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21558
            case 48: {
              // AddAndReplaceKind
              zval_unset_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21559
            case 49: {
              // AddInitKind
              memset(&executor_globals, 0, sizeof(*(&executor_globals)));

              break;
            }
#endif
#ifdef COMPILE_21560
            case 50: {
              // IfExitKind
              if (__is_neg(
                      "231-50", 96, &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(p),
                      sizeof(p), &(is_ref), sizeof(is_ref), &(key), sizeof(key),
                      &(tmp), sizeof(tmp), &(args), sizeof(args),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_21561
            case 51: {
              // IfExitKind
              if (__is_neg(
                      "231-51", 96, &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(p),
                      sizeof(p), &(is_ref), sizeof(is_ref), &(key), sizeof(key),
                      &(tmp), sizeof(tmp), &(args), sizeof(args),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_21562
            case 52: {
              // IfExitKind
              if (__is_neg(
                      "231-52", 96, &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(p),
                      sizeof(p), &(is_ref), sizeof(is_ref), &(key), sizeof(key),
                      &(tmp), sizeof(tmp), &(args), sizeof(args),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 32;

              break;
            }
#endif
#ifdef COMPILE_21563
            case 53: {
              // IfExitKind
              if (__is_neg(
                      "231-53", 96, &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(p),
                      sizeof(p), &(is_ref), sizeof(is_ref), &(key), sizeof(key),
                      &(tmp), sizeof(tmp), &(args), sizeof(args),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 3;

              break;
            }
#endif
#ifdef COMPILE_21564
            case 54: {
              // IfExitKind
              if (__is_neg(
                      "231-54", 96, &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(p),
                      sizeof(p), &(is_ref), sizeof(is_ref), &(key), sizeof(key),
                      &(tmp), sizeof(tmp), &(args), sizeof(args),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 64;

              break;
            }
#endif
            }
            {
              long long __temp232 = (!(executor_globals.active_symbol_table));
              switch (__choose("__SWITCH232")) {
              case 0: {

                break;
              }
#ifdef COMPILE_21565
              case 1: {
                __temp232 =
                    ((!(executor_globals.active_symbol_table)) ||
                     __is_neg(
                         "232-1", 96, &(executor_globals.active_symbol_table),
                         sizeof(executor_globals.active_symbol_table),
                         &(executor_globals.active_symbol_table),
                         sizeof(executor_globals.active_symbol_table), &(p),
                         sizeof(p), &(is_ref), sizeof(is_ref), &(key),
                         sizeof(key), &(tmp), sizeof(tmp), &(args),
                         sizeof(args),
                         &(executor_globals.uninitialized_zval_ptr),
                         sizeof(executor_globals.uninitialized_zval_ptr),
                         &(executor_globals.uninitialized_zval_ptr),
                         sizeof(executor_globals.uninitialized_zval_ptr),
                         &(target), sizeof(target),
                         &(executor_globals.return_value_ptr_ptr),
                         sizeof(executor_globals.return_value_ptr_ptr),
                         &(executor_globals.error_zval_ptr),
                         sizeof(executor_globals.error_zval_ptr),
                         &(executor_globals.symtable_cache_limit),
                         sizeof(executor_globals.symtable_cache_limit),
                         &(executor_globals.symtable_cache_ptr),
                         sizeof(executor_globals.symtable_cache_ptr),
                         &(executor_globals.opline_ptr),
                         sizeof(executor_globals.opline_ptr),
                         &(executor_globals.bailout),
                         sizeof(executor_globals.bailout),
                         &(executor_globals.error_reporting),
                         sizeof(executor_globals.error_reporting),
                         &(executor_globals.orig_error_reporting),
                         sizeof(executor_globals.orig_error_reporting),
                         &(executor_globals.exit_status),
                         sizeof(executor_globals.exit_status),
                         &(executor_globals.active_op_array),
                         sizeof(executor_globals.active_op_array),
                         &(executor_globals.function_table),
                         sizeof(executor_globals.function_table),
                         &(executor_globals.class_table),
                         sizeof(executor_globals.class_table),
                         &(executor_globals.zend_constants),
                         sizeof(executor_globals.zend_constants),
                         &(executor_globals.scope),
                         sizeof(executor_globals.scope),
                         &(executor_globals.called_scope),
                         sizeof(executor_globals.called_scope),
                         &(executor_globals.This),
                         sizeof(executor_globals.This),
                         &(executor_globals.precision),
                         sizeof(executor_globals.precision),
                         &(executor_globals.ticks_count),
                         sizeof(executor_globals.ticks_count),
                         &(executor_globals.in_execution),
                         sizeof(executor_globals.in_execution),
                         &(executor_globals.in_autoload),
                         sizeof(executor_globals.in_autoload),
                         &(executor_globals.autoload_func),
                         sizeof(executor_globals.autoload_func),
                         &(executor_globals.full_tables_cleanup),
                         sizeof(executor_globals.full_tables_cleanup),
                         &(executor_globals.no_extensions),
                         sizeof(executor_globals.no_extensions),
                         &(executor_globals.argument_stack),
                         sizeof(executor_globals.argument_stack),
                         &(executor_globals.user_error_handler_error_reporting),
                         sizeof(executor_globals
                                    .user_error_handler_error_reporting),
                         &(executor_globals.user_error_handler),
                         sizeof(executor_globals.user_error_handler),
                         &(executor_globals.user_exception_handler),
                         sizeof(executor_globals.user_exception_handler),
                         &(executor_globals.error_handling),
                         sizeof(executor_globals.error_handling),
                         &(executor_globals.exception_class),
                         sizeof(executor_globals.exception_class),
                         &(executor_globals.timeout_seconds),
                         sizeof(executor_globals.timeout_seconds),
                         &(executor_globals.lambda_count),
                         sizeof(executor_globals.lambda_count),
                         &(executor_globals.ini_directives),
                         sizeof(executor_globals.ini_directives),
                         &(executor_globals.modified_ini_directives),
                         sizeof(executor_globals.modified_ini_directives),
                         &(executor_globals.error_reporting_ini_entry),
                         sizeof(executor_globals.error_reporting_ini_entry),
                         &(executor_globals.exception),
                         sizeof(executor_globals.exception),
                         &(executor_globals.prev_exception),
                         sizeof(executor_globals.prev_exception),
                         &(executor_globals.opline_before_exception),
                         sizeof(executor_globals.opline_before_exception),
                         &(executor_globals.current_execute_data),
                         sizeof(executor_globals.current_execute_data),
                         &(executor_globals.current_module),
                         sizeof(executor_globals.current_module),
                         &(executor_globals.active),
                         sizeof(executor_globals.active),
                         &(executor_globals.start_op),
                         sizeof(executor_globals.start_op),
                         &(executor_globals.saved_fpu_cw_ptr),
                         sizeof(executor_globals.saved_fpu_cw_ptr),
                         &(executor_globals.saved_fpu_cw),
                         sizeof(executor_globals.saved_fpu_cw),
                         &(executor_globals.return_value_ptr_ptr),
                         sizeof(executor_globals.return_value_ptr_ptr),
                         &(executor_globals.error_zval_ptr),
                         sizeof(executor_globals.error_zval_ptr),
                         &(executor_globals.symtable_cache_limit),
                         sizeof(executor_globals.symtable_cache_limit),
                         &(executor_globals.symtable_cache_ptr),
                         sizeof(executor_globals.symtable_cache_ptr),
                         &(executor_globals.opline_ptr),
                         sizeof(executor_globals.opline_ptr),
                         &(executor_globals.bailout),
                         sizeof(executor_globals.bailout),
                         &(executor_globals.error_reporting),
                         sizeof(executor_globals.error_reporting),
                         &(executor_globals.orig_error_reporting),
                         sizeof(executor_globals.orig_error_reporting),
                         &(executor_globals.exit_status),
                         sizeof(executor_globals.exit_status),
                         &(executor_globals.active_op_array),
                         sizeof(executor_globals.active_op_array),
                         &(executor_globals.function_table),
                         sizeof(executor_globals.function_table),
                         &(executor_globals.class_table),
                         sizeof(executor_globals.class_table),
                         &(executor_globals.zend_constants),
                         sizeof(executor_globals.zend_constants),
                         &(executor_globals.scope),
                         sizeof(executor_globals.scope),
                         &(executor_globals.called_scope),
                         sizeof(executor_globals.called_scope),
                         &(executor_globals.This),
                         sizeof(executor_globals.This),
                         &(executor_globals.precision),
                         sizeof(executor_globals.precision),
                         &(executor_globals.ticks_count),
                         sizeof(executor_globals.ticks_count),
                         &(executor_globals.in_execution),
                         sizeof(executor_globals.in_execution),
                         &(executor_globals.in_autoload),
                         sizeof(executor_globals.in_autoload),
                         &(executor_globals.autoload_func),
                         sizeof(executor_globals.autoload_func),
                         &(executor_globals.full_tables_cleanup),
                         sizeof(executor_globals.full_tables_cleanup),
                         &(executor_globals.no_extensions),
                         sizeof(executor_globals.no_extensions),
                         &(executor_globals.argument_stack),
                         sizeof(executor_globals.argument_stack),
                         &(executor_globals.user_error_handler_error_reporting),
                         sizeof(executor_globals
                                    .user_error_handler_error_reporting),
                         &(executor_globals.user_error_handler),
                         sizeof(executor_globals.user_error_handler),
                         &(executor_globals.user_exception_handler),
                         sizeof(executor_globals.user_exception_handler),
                         &(executor_globals.error_handling),
                         sizeof(executor_globals.error_handling),
                         &(executor_globals.exception_class),
                         sizeof(executor_globals.exception_class),
                         &(executor_globals.timeout_seconds),
                         sizeof(executor_globals.timeout_seconds),
                         &(executor_globals.lambda_count),
                         sizeof(executor_globals.lambda_count),
                         &(executor_globals.ini_directives),
                         sizeof(executor_globals.ini_directives),
                         &(executor_globals.modified_ini_directives),
                         sizeof(executor_globals.modified_ini_directives),
                         &(executor_globals.error_reporting_ini_entry),
                         sizeof(executor_globals.error_reporting_ini_entry),
                         &(executor_globals.exception),
                         sizeof(executor_globals.exception),
                         &(executor_globals.prev_exception),
                         sizeof(executor_globals.prev_exception),
                         &(executor_globals.opline_before_exception),
                         sizeof(executor_globals.opline_before_exception),
                         &(executor_globals.current_execute_data),
                         sizeof(executor_globals.current_execute_data),
                         &(executor_globals.current_module),
                         sizeof(executor_globals.current_module),
                         &(executor_globals.active),
                         sizeof(executor_globals.active),
                         &(executor_globals.start_op),
                         sizeof(executor_globals.start_op),
                         &(executor_globals.saved_fpu_cw_ptr),
                         sizeof(executor_globals.saved_fpu_cw_ptr),
                         &(executor_globals.saved_fpu_cw),
                         sizeof(executor_globals.saved_fpu_cw)));

                break;
              }
#endif
#ifdef COMPILE_21566
              case 2: {
                __temp232 =
                    ((!(executor_globals.active_symbol_table)) &&
                     !__is_neg(
                         "232-2", 96, &(executor_globals.active_symbol_table),
                         sizeof(executor_globals.active_symbol_table),
                         &(executor_globals.active_symbol_table),
                         sizeof(executor_globals.active_symbol_table), &(p),
                         sizeof(p), &(is_ref), sizeof(is_ref), &(key),
                         sizeof(key), &(tmp), sizeof(tmp), &(args),
                         sizeof(args),
                         &(executor_globals.uninitialized_zval_ptr),
                         sizeof(executor_globals.uninitialized_zval_ptr),
                         &(executor_globals.uninitialized_zval_ptr),
                         sizeof(executor_globals.uninitialized_zval_ptr),
                         &(target), sizeof(target),
                         &(executor_globals.return_value_ptr_ptr),
                         sizeof(executor_globals.return_value_ptr_ptr),
                         &(executor_globals.error_zval_ptr),
                         sizeof(executor_globals.error_zval_ptr),
                         &(executor_globals.symtable_cache_limit),
                         sizeof(executor_globals.symtable_cache_limit),
                         &(executor_globals.symtable_cache_ptr),
                         sizeof(executor_globals.symtable_cache_ptr),
                         &(executor_globals.opline_ptr),
                         sizeof(executor_globals.opline_ptr),
                         &(executor_globals.bailout),
                         sizeof(executor_globals.bailout),
                         &(executor_globals.error_reporting),
                         sizeof(executor_globals.error_reporting),
                         &(executor_globals.orig_error_reporting),
                         sizeof(executor_globals.orig_error_reporting),
                         &(executor_globals.exit_status),
                         sizeof(executor_globals.exit_status),
                         &(executor_globals.active_op_array),
                         sizeof(executor_globals.active_op_array),
                         &(executor_globals.function_table),
                         sizeof(executor_globals.function_table),
                         &(executor_globals.class_table),
                         sizeof(executor_globals.class_table),
                         &(executor_globals.zend_constants),
                         sizeof(executor_globals.zend_constants),
                         &(executor_globals.scope),
                         sizeof(executor_globals.scope),
                         &(executor_globals.called_scope),
                         sizeof(executor_globals.called_scope),
                         &(executor_globals.This),
                         sizeof(executor_globals.This),
                         &(executor_globals.precision),
                         sizeof(executor_globals.precision),
                         &(executor_globals.ticks_count),
                         sizeof(executor_globals.ticks_count),
                         &(executor_globals.in_execution),
                         sizeof(executor_globals.in_execution),
                         &(executor_globals.in_autoload),
                         sizeof(executor_globals.in_autoload),
                         &(executor_globals.autoload_func),
                         sizeof(executor_globals.autoload_func),
                         &(executor_globals.full_tables_cleanup),
                         sizeof(executor_globals.full_tables_cleanup),
                         &(executor_globals.no_extensions),
                         sizeof(executor_globals.no_extensions),
                         &(executor_globals.argument_stack),
                         sizeof(executor_globals.argument_stack),
                         &(executor_globals.user_error_handler_error_reporting),
                         sizeof(executor_globals
                                    .user_error_handler_error_reporting),
                         &(executor_globals.user_error_handler),
                         sizeof(executor_globals.user_error_handler),
                         &(executor_globals.user_exception_handler),
                         sizeof(executor_globals.user_exception_handler),
                         &(executor_globals.error_handling),
                         sizeof(executor_globals.error_handling),
                         &(executor_globals.exception_class),
                         sizeof(executor_globals.exception_class),
                         &(executor_globals.timeout_seconds),
                         sizeof(executor_globals.timeout_seconds),
                         &(executor_globals.lambda_count),
                         sizeof(executor_globals.lambda_count),
                         &(executor_globals.ini_directives),
                         sizeof(executor_globals.ini_directives),
                         &(executor_globals.modified_ini_directives),
                         sizeof(executor_globals.modified_ini_directives),
                         &(executor_globals.error_reporting_ini_entry),
                         sizeof(executor_globals.error_reporting_ini_entry),
                         &(executor_globals.exception),
                         sizeof(executor_globals.exception),
                         &(executor_globals.prev_exception),
                         sizeof(executor_globals.prev_exception),
                         &(executor_globals.opline_before_exception),
                         sizeof(executor_globals.opline_before_exception),
                         &(executor_globals.current_execute_data),
                         sizeof(executor_globals.current_execute_data),
                         &(executor_globals.current_module),
                         sizeof(executor_globals.current_module),
                         &(executor_globals.active),
                         sizeof(executor_globals.active),
                         &(executor_globals.start_op),
                         sizeof(executor_globals.start_op),
                         &(executor_globals.saved_fpu_cw_ptr),
                         sizeof(executor_globals.saved_fpu_cw_ptr),
                         &(executor_globals.saved_fpu_cw),
                         sizeof(executor_globals.saved_fpu_cw),
                         &(executor_globals.return_value_ptr_ptr),
                         sizeof(executor_globals.return_value_ptr_ptr),
                         &(executor_globals.error_zval_ptr),
                         sizeof(executor_globals.error_zval_ptr),
                         &(executor_globals.symtable_cache_limit),
                         sizeof(executor_globals.symtable_cache_limit),
                         &(executor_globals.symtable_cache_ptr),
                         sizeof(executor_globals.symtable_cache_ptr),
                         &(executor_globals.opline_ptr),
                         sizeof(executor_globals.opline_ptr),
                         &(executor_globals.bailout),
                         sizeof(executor_globals.bailout),
                         &(executor_globals.error_reporting),
                         sizeof(executor_globals.error_reporting),
                         &(executor_globals.orig_error_reporting),
                         sizeof(executor_globals.orig_error_reporting),
                         &(executor_globals.exit_status),
                         sizeof(executor_globals.exit_status),
                         &(executor_globals.active_op_array),
                         sizeof(executor_globals.active_op_array),
                         &(executor_globals.function_table),
                         sizeof(executor_globals.function_table),
                         &(executor_globals.class_table),
                         sizeof(executor_globals.class_table),
                         &(executor_globals.zend_constants),
                         sizeof(executor_globals.zend_constants),
                         &(executor_globals.scope),
                         sizeof(executor_globals.scope),
                         &(executor_globals.called_scope),
                         sizeof(executor_globals.called_scope),
                         &(executor_globals.This),
                         sizeof(executor_globals.This),
                         &(executor_globals.precision),
                         sizeof(executor_globals.precision),
                         &(executor_globals.ticks_count),
                         sizeof(executor_globals.ticks_count),
                         &(executor_globals.in_execution),
                         sizeof(executor_globals.in_execution),
                         &(executor_globals.in_autoload),
                         sizeof(executor_globals.in_autoload),
                         &(executor_globals.autoload_func),
                         sizeof(executor_globals.autoload_func),
                         &(executor_globals.full_tables_cleanup),
                         sizeof(executor_globals.full_tables_cleanup),
                         &(executor_globals.no_extensions),
                         sizeof(executor_globals.no_extensions),
                         &(executor_globals.argument_stack),
                         sizeof(executor_globals.argument_stack),
                         &(executor_globals.user_error_handler_error_reporting),
                         sizeof(executor_globals
                                    .user_error_handler_error_reporting),
                         &(executor_globals.user_error_handler),
                         sizeof(executor_globals.user_error_handler),
                         &(executor_globals.user_exception_handler),
                         sizeof(executor_globals.user_exception_handler),
                         &(executor_globals.error_handling),
                         sizeof(executor_globals.error_handling),
                         &(executor_globals.exception_class),
                         sizeof(executor_globals.exception_class),
                         &(executor_globals.timeout_seconds),
                         sizeof(executor_globals.timeout_seconds),
                         &(executor_globals.lambda_count),
                         sizeof(executor_globals.lambda_count),
                         &(executor_globals.ini_directives),
                         sizeof(executor_globals.ini_directives),
                         &(executor_globals.modified_ini_directives),
                         sizeof(executor_globals.modified_ini_directives),
                         &(executor_globals.error_reporting_ini_entry),
                         sizeof(executor_globals.error_reporting_ini_entry),
                         &(executor_globals.exception),
                         sizeof(executor_globals.exception),
                         &(executor_globals.prev_exception),
                         sizeof(executor_globals.prev_exception),
                         &(executor_globals.opline_before_exception),
                         sizeof(executor_globals.opline_before_exception),
                         &(executor_globals.current_execute_data),
                         sizeof(executor_globals.current_execute_data),
                         &(executor_globals.current_module),
                         sizeof(executor_globals.current_module),
                         &(executor_globals.active),
                         sizeof(executor_globals.active),
                         &(executor_globals.start_op),
                         sizeof(executor_globals.start_op),
                         &(executor_globals.saved_fpu_cw_ptr),
                         sizeof(executor_globals.saved_fpu_cw_ptr),
                         &(executor_globals.saved_fpu_cw),
                         sizeof(executor_globals.saved_fpu_cw)));

                break;
              }
#endif
              }
              switch (__choose("__SWITCH235")) {
              case 0: {
                if (__temp232) {
                  {
                    switch (__choose("__SWITCH233")) {
                    case 0:
                      break;
#ifdef COMPILE_21567
                    case 1: {
                      // AddAndReplaceKind
                      (executor_globals.argument_stack) =
                          zend_vm_stack_new_page(((16 * 1024) - 16));

                      break;
                    }
#endif
#ifdef COMPILE_21568
                    case 2: {
                      // AddAndReplaceKind
                      (tmp) = (executor_globals.uninitialized_zval_ptr);

                      break;
                    }
#endif
#ifdef COMPILE_21569
                    case 3: {
                      // AddAndReplaceKind
                      (tmp) = *p;

                      break;
                    }
#endif
#ifdef COMPILE_21570
                    case 4: {
                      // AddAndReplaceKind
                      *(tmp) = zval_used_for_init;

                      break;
                    }
#endif
#ifdef COMPILE_21571
                    case 5: {
                      // AddAndReplaceKind
                      *tmp = zval_used_for_init;

                      break;
                    }
#endif
#ifdef COMPILE_21572
                    case 6: {
                      // AddAndReplaceKind
                      _efree(((tmp)));

                      break;
                    }
#endif
#ifdef COMPILE_21573
                    case 7: {
                      // AddAndReplaceKind
                      _efree(((tmp)->value.ht));

                      break;
                    }
#endif
#ifdef COMPILE_21574
                    case 8: {
                      // AddAndReplaceKind
                      _efree(((tmp)->value.str.val));

                      break;
                    }
#endif
#ifdef COMPILE_21575
                    case 9: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), (key)->arKey,
                          key->nKeyLength, key->h, &tmp, sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21576
                    case 10: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), key->arKey,
                          (key)->nKeyLength, key->h, &tmp, sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21577
                    case 11: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), key->arKey,
                          key->nKeyLength, (key)->h, &tmp, sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21578
                    case 12: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), key->arKey,
                          key->nKeyLength, key->h, &(tmp), sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21579
                    case 13: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), key->arKey,
                          key->nKeyLength, key->h, &tmp, sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21580
                    case 14: {
                      // AddAndReplaceKind
                      _zend_list_addref((tmp)->value.lval);

                      break;
                    }
#endif
#ifdef COMPILE_21581
                    case 15: {
                      // AddAndReplaceKind
                      _zend_list_delete((tmp)->value.lval);

                      break;
                    }
#endif
#ifdef COMPILE_21582
                    case 16: {
                      // AddAndReplaceKind
                      _zval_copy_ctor_func((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21583
                    case 17: {
                      // AddAndReplaceKind
                      _zval_dtor(((tmp)));

                      break;
                    }
#endif
#ifdef COMPILE_21584
                    case 18: {
                      // AddAndReplaceKind
                      _zval_dtor_func((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21585
                    case 19: {
                      // AddAndReplaceKind
                      _zval_ptr_dtor((&(tmp)));

                      break;
                    }
#endif
#ifdef COMPILE_21586
                    case 20: {
                      // AddAndReplaceKind
                      convert_to_boolean((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21587
                    case 21: {
                      // AddAndReplaceKind
                      exit(1);

                      break;
                    }
#endif
#ifdef COMPILE_21588
                    case 22: {
                      // AddAndReplaceKind
                      gc_remove_zval_from_buffer((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21589
                    case 23: {
                      // AddAndReplaceKind
                      gc_zval_check_possible_root(((tmp)));

                      break;
                    }
#endif
#ifdef COMPILE_21590
                    case 24: {
                      // AddAndReplaceKind
                      gc_zval_possible_root((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21591
                    case 25: {
                      // AddAndReplaceKind
                      i_zval_ptr_dtor((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21592
                    case 26: {
                      // AddAndReplaceKind
                      if (!(executor_globals.active_symbol_table)) {
                        zend_rebuild_symbol_table();
                      }

                      break;
                    }
#endif
#ifdef COMPILE_21593
                    case 27: {
                      // AddAndReplaceKind
                      if (_zend_hash_quick_add_or_update(
                              target, key->arKey, key->nKeyLength, key->h, &tmp,
                              sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
                        zval_addref_p(tmp);
                      }

                      break;
                    }
#endif
#ifdef COMPILE_21594
                    case 28: {
                      // AddAndReplaceKind
                      is_ref = (**(p)).type & 64;

                      break;
                    }
#endif
#ifdef COMPILE_21595
                    case 29: {
                      // AddAndReplaceKind
                      is_ref = (**p).type & 64;

                      break;
                    }
#endif
#ifdef COMPILE_21596
                    case 30: {
                      // AddAndReplaceKind
                      tmp = (executor_globals.uninitialized_zval_ptr);

                      break;
                    }
#endif
#ifdef COMPILE_21597
                    case 31: {
                      // AddAndReplaceKind
                      tmp = *(p);

                      break;
                    }
#endif
#ifdef COMPILE_21598
                    case 32: {
                      // AddAndReplaceKind
                      tmp = *p;

                      break;
                    }
#endif
#ifdef COMPILE_21599
                    case 33: {
                      // AddAndReplaceKind
                      zend_error((1 << 3L),
                                 "A non well formed numeric value encountered");

                      break;
                    }
#endif
#ifdef COMPILE_21600
                    case 34: {
                      // AddAndReplaceKind
                      zend_error((1 << 3L), "Undefined variable: %s",
                                 (key)->arKey);

                      break;
                    }
#endif
#ifdef COMPILE_21601
                    case 35: {
                      // AddAndReplaceKind
                      zend_error((1 << 3L), "Undefined variable: %s",
                                 key->arKey);

                      break;
                    }
#endif
#ifdef COMPILE_21602
                    case 36: {
                      // AddAndReplaceKind
                      zend_hash_destroy((tmp)->value.ht);

                      break;
                    }
#endif
#ifdef COMPILE_21603
                    case 37: {
                      // AddAndReplaceKind
                      zend_rebuild_symbol_table();

                      break;
                    }
#endif
#ifdef COMPILE_21604
                    case 38: {
                      // AddAndReplaceKind
                      zval_addref_p((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21605
                    case 39: {
                      // AddAndReplaceKind
                      zval_addref_p(*((p)));

                      break;
                    }
#endif
#ifdef COMPILE_21606
                    case 40: {
                      // AddAndReplaceKind
                      zval_addref_p(tmp);

                      break;
                    }
#endif
#ifdef COMPILE_21607
                    case 41: {
                      // AddAndReplaceKind
                      zval_set_isref_p((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21608
                    case 42: {
                      // AddAndReplaceKind
                      zval_set_isref_p(*(((p))));

                      break;
                    }
#endif
#ifdef COMPILE_21609
                    case 43: {
                      // AddAndReplaceKind
                      zval_set_isref_p(*((p)));

                      break;
                    }
#endif
#ifdef COMPILE_21610
                    case 44: {
                      // AddAndReplaceKind
                      zval_set_isref_p(tmp);

                      break;
                    }
#endif
#ifdef COMPILE_21611
                    case 45: {
                      // AddAndReplaceKind
                      zval_set_refcount_p((tmp), 0);

                      break;
                    }
#endif
#ifdef COMPILE_21612
                    case 46: {
                      // AddAndReplaceKind
                      zval_set_refcount_p(tmp, 0);

                      break;
                    }
#endif
#ifdef COMPILE_21613
                    case 47: {
                      // AddAndReplaceKind
                      zval_unset_isref_p((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21614
                    case 48: {
                      // AddAndReplaceKind
                      zval_unset_isref_p(tmp);

                      break;
                    }
#endif
#ifdef COMPILE_21615
                    case 49: {
                      // IfExitKind
                      if (__is_neg(
                              "233-49", 96,
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(p), sizeof(p), &(key), sizeof(key), &(is_ref),
                              sizeof(is_ref), &(tmp), sizeof(tmp),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(args), sizeof(args), &(target), sizeof(target),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw)))
                        return 0;

                      break;
                    }
#endif
#ifdef COMPILE_21616
                    case 50: {
                      // IfExitKind
                      if (__is_neg(
                              "233-50", 96,
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(p), sizeof(p), &(key), sizeof(key), &(is_ref),
                              sizeof(is_ref), &(tmp), sizeof(tmp),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(args), sizeof(args), &(target), sizeof(target),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw)))
                        return 1;

                      break;
                    }
#endif
#ifdef COMPILE_21617
                    case 51: {
                      // IfExitKind
                      if (__is_neg(
                              "233-51", 96,
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(p), sizeof(p), &(key), sizeof(key), &(is_ref),
                              sizeof(is_ref), &(tmp), sizeof(tmp),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(args), sizeof(args), &(target), sizeof(target),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw)))
                        return 32;

                      break;
                    }
#endif
#ifdef COMPILE_21618
                    case 52: {
                      // IfExitKind
                      if (__is_neg(
                              "233-52", 96,
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(p), sizeof(p), &(key), sizeof(key), &(is_ref),
                              sizeof(is_ref), &(tmp), sizeof(tmp),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(args), sizeof(args), &(target), sizeof(target),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw)))
                        return 3;

                      break;
                    }
#endif
#ifdef COMPILE_21619
                    case 53: {
                      // IfExitKind
                      if (__is_neg(
                              "233-53", 96,
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(p), sizeof(p), &(key), sizeof(key), &(is_ref),
                              sizeof(is_ref), &(tmp), sizeof(tmp),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(args), sizeof(args), &(target), sizeof(target),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw)))
                        return 64;

                      break;
                    }
#endif
                    }
                    switch (__choose("__SWITCH234")) {
                    case 0: {
                      zend_rebuild_symbol_table(TSRMLS_C);

                      break;
                    }
#ifdef COMPILE_21620
                    case 1: {
                      // GuardKind
                      if (!__is_neg(
                              "234-1", 96,
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(executor_globals.active_symbol_table),
                              sizeof(executor_globals.active_symbol_table),
                              &(p), sizeof(p), &(key), sizeof(key), &(is_ref),
                              sizeof(is_ref), &(tmp), sizeof(tmp), &(args),
                              sizeof(args),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(executor_globals.uninitialized_zval_ptr),
                              sizeof(executor_globals.uninitialized_zval_ptr),
                              &(target), sizeof(target),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw),
                              &(executor_globals.return_value_ptr_ptr),
                              sizeof(executor_globals.return_value_ptr_ptr),
                              &(executor_globals.error_zval_ptr),
                              sizeof(executor_globals.error_zval_ptr),
                              &(executor_globals.symtable_cache_limit),
                              sizeof(executor_globals.symtable_cache_limit),
                              &(executor_globals.symtable_cache_ptr),
                              sizeof(executor_globals.symtable_cache_ptr),
                              &(executor_globals.opline_ptr),
                              sizeof(executor_globals.opline_ptr),
                              &(executor_globals.bailout),
                              sizeof(executor_globals.bailout),
                              &(executor_globals.error_reporting),
                              sizeof(executor_globals.error_reporting),
                              &(executor_globals.orig_error_reporting),
                              sizeof(executor_globals.orig_error_reporting),
                              &(executor_globals.exit_status),
                              sizeof(executor_globals.exit_status),
                              &(executor_globals.active_op_array),
                              sizeof(executor_globals.active_op_array),
                              &(executor_globals.function_table),
                              sizeof(executor_globals.function_table),
                              &(executor_globals.class_table),
                              sizeof(executor_globals.class_table),
                              &(executor_globals.zend_constants),
                              sizeof(executor_globals.zend_constants),
                              &(executor_globals.scope),
                              sizeof(executor_globals.scope),
                              &(executor_globals.called_scope),
                              sizeof(executor_globals.called_scope),
                              &(executor_globals.This),
                              sizeof(executor_globals.This),
                              &(executor_globals.precision),
                              sizeof(executor_globals.precision),
                              &(executor_globals.ticks_count),
                              sizeof(executor_globals.ticks_count),
                              &(executor_globals.in_execution),
                              sizeof(executor_globals.in_execution),
                              &(executor_globals.in_autoload),
                              sizeof(executor_globals.in_autoload),
                              &(executor_globals.autoload_func),
                              sizeof(executor_globals.autoload_func),
                              &(executor_globals.full_tables_cleanup),
                              sizeof(executor_globals.full_tables_cleanup),
                              &(executor_globals.no_extensions),
                              sizeof(executor_globals.no_extensions),
                              &(executor_globals.argument_stack),
                              sizeof(executor_globals.argument_stack),
                              &(executor_globals
                                    .user_error_handler_error_reporting),
                              sizeof(executor_globals
                                         .user_error_handler_error_reporting),
                              &(executor_globals.user_error_handler),
                              sizeof(executor_globals.user_error_handler),
                              &(executor_globals.user_exception_handler),
                              sizeof(executor_globals.user_exception_handler),
                              &(executor_globals.error_handling),
                              sizeof(executor_globals.error_handling),
                              &(executor_globals.exception_class),
                              sizeof(executor_globals.exception_class),
                              &(executor_globals.timeout_seconds),
                              sizeof(executor_globals.timeout_seconds),
                              &(executor_globals.lambda_count),
                              sizeof(executor_globals.lambda_count),
                              &(executor_globals.ini_directives),
                              sizeof(executor_globals.ini_directives),
                              &(executor_globals.modified_ini_directives),
                              sizeof(executor_globals.modified_ini_directives),
                              &(executor_globals.error_reporting_ini_entry),
                              sizeof(
                                  executor_globals.error_reporting_ini_entry),
                              &(executor_globals.exception),
                              sizeof(executor_globals.exception),
                              &(executor_globals.prev_exception),
                              sizeof(executor_globals.prev_exception),
                              &(executor_globals.opline_before_exception),
                              sizeof(executor_globals.opline_before_exception),
                              &(executor_globals.current_execute_data),
                              sizeof(executor_globals.current_execute_data),
                              &(executor_globals.current_module),
                              sizeof(executor_globals.current_module),
                              &(executor_globals.active),
                              sizeof(executor_globals.active),
                              &(executor_globals.start_op),
                              sizeof(executor_globals.start_op),
                              &(executor_globals.saved_fpu_cw_ptr),
                              sizeof(executor_globals.saved_fpu_cw_ptr),
                              &(executor_globals.saved_fpu_cw),
                              sizeof(executor_globals.saved_fpu_cw)))
                        zend_rebuild_symbol_table();

                      break;
                    }
#endif
                    }
                  }
                }

                break;
              }
#ifdef COMPILE_21621
              case 1: {
                // GuardKind
                if (!__is_neg(
                        "235-1", 96, &(executor_globals.active_symbol_table),
                        sizeof(executor_globals.active_symbol_table),
                        &(executor_globals.active_symbol_table),
                        sizeof(executor_globals.active_symbol_table), &(p),
                        sizeof(p), &(key), sizeof(key), &(is_ref),
                        sizeof(is_ref), &(tmp), sizeof(tmp), &(args),
                        sizeof(args),
                        &(executor_globals.uninitialized_zval_ptr),
                        sizeof(executor_globals.uninitialized_zval_ptr),
                        &(executor_globals.uninitialized_zval_ptr),
                        sizeof(executor_globals.uninitialized_zval_ptr),
                        &(target), sizeof(target),
                        &(executor_globals.return_value_ptr_ptr),
                        sizeof(executor_globals.return_value_ptr_ptr),
                        &(executor_globals.error_zval_ptr),
                        sizeof(executor_globals.error_zval_ptr),
                        &(executor_globals.symtable_cache_limit),
                        sizeof(executor_globals.symtable_cache_limit),
                        &(executor_globals.symtable_cache_ptr),
                        sizeof(executor_globals.symtable_cache_ptr),
                        &(executor_globals.opline_ptr),
                        sizeof(executor_globals.opline_ptr),
                        &(executor_globals.bailout),
                        sizeof(executor_globals.bailout),
                        &(executor_globals.error_reporting),
                        sizeof(executor_globals.error_reporting),
                        &(executor_globals.orig_error_reporting),
                        sizeof(executor_globals.orig_error_reporting),
                        &(executor_globals.exit_status),
                        sizeof(executor_globals.exit_status),
                        &(executor_globals.active_op_array),
                        sizeof(executor_globals.active_op_array),
                        &(executor_globals.function_table),
                        sizeof(executor_globals.function_table),
                        &(executor_globals.class_table),
                        sizeof(executor_globals.class_table),
                        &(executor_globals.zend_constants),
                        sizeof(executor_globals.zend_constants),
                        &(executor_globals.scope),
                        sizeof(executor_globals.scope),
                        &(executor_globals.called_scope),
                        sizeof(executor_globals.called_scope),
                        &(executor_globals.This), sizeof(executor_globals.This),
                        &(executor_globals.precision),
                        sizeof(executor_globals.precision),
                        &(executor_globals.ticks_count),
                        sizeof(executor_globals.ticks_count),
                        &(executor_globals.in_execution),
                        sizeof(executor_globals.in_execution),
                        &(executor_globals.in_autoload),
                        sizeof(executor_globals.in_autoload),
                        &(executor_globals.autoload_func),
                        sizeof(executor_globals.autoload_func),
                        &(executor_globals.full_tables_cleanup),
                        sizeof(executor_globals.full_tables_cleanup),
                        &(executor_globals.no_extensions),
                        sizeof(executor_globals.no_extensions),
                        &(executor_globals.argument_stack),
                        sizeof(executor_globals.argument_stack),
                        &(executor_globals.user_error_handler_error_reporting),
                        sizeof(executor_globals
                                   .user_error_handler_error_reporting),
                        &(executor_globals.user_error_handler),
                        sizeof(executor_globals.user_error_handler),
                        &(executor_globals.user_exception_handler),
                        sizeof(executor_globals.user_exception_handler),
                        &(executor_globals.error_handling),
                        sizeof(executor_globals.error_handling),
                        &(executor_globals.exception_class),
                        sizeof(executor_globals.exception_class),
                        &(executor_globals.timeout_seconds),
                        sizeof(executor_globals.timeout_seconds),
                        &(executor_globals.lambda_count),
                        sizeof(executor_globals.lambda_count),
                        &(executor_globals.ini_directives),
                        sizeof(executor_globals.ini_directives),
                        &(executor_globals.modified_ini_directives),
                        sizeof(executor_globals.modified_ini_directives),
                        &(executor_globals.error_reporting_ini_entry),
                        sizeof(executor_globals.error_reporting_ini_entry),
                        &(executor_globals.exception),
                        sizeof(executor_globals.exception),
                        &(executor_globals.prev_exception),
                        sizeof(executor_globals.prev_exception),
                        &(executor_globals.opline_before_exception),
                        sizeof(executor_globals.opline_before_exception),
                        &(executor_globals.current_execute_data),
                        sizeof(executor_globals.current_execute_data),
                        &(executor_globals.current_module),
                        sizeof(executor_globals.current_module),
                        &(executor_globals.active),
                        sizeof(executor_globals.active),
                        &(executor_globals.start_op),
                        sizeof(executor_globals.start_op),
                        &(executor_globals.saved_fpu_cw_ptr),
                        sizeof(executor_globals.saved_fpu_cw_ptr),
                        &(executor_globals.saved_fpu_cw),
                        sizeof(executor_globals.saved_fpu_cw),
                        &(executor_globals.return_value_ptr_ptr),
                        sizeof(executor_globals.return_value_ptr_ptr),
                        &(executor_globals.error_zval_ptr),
                        sizeof(executor_globals.error_zval_ptr),
                        &(executor_globals.symtable_cache_limit),
                        sizeof(executor_globals.symtable_cache_limit),
                        &(executor_globals.symtable_cache_ptr),
                        sizeof(executor_globals.symtable_cache_ptr),
                        &(executor_globals.opline_ptr),
                        sizeof(executor_globals.opline_ptr),
                        &(executor_globals.bailout),
                        sizeof(executor_globals.bailout),
                        &(executor_globals.error_reporting),
                        sizeof(executor_globals.error_reporting),
                        &(executor_globals.orig_error_reporting),
                        sizeof(executor_globals.orig_error_reporting),
                        &(executor_globals.exit_status),
                        sizeof(executor_globals.exit_status),
                        &(executor_globals.active_op_array),
                        sizeof(executor_globals.active_op_array),
                        &(executor_globals.function_table),
                        sizeof(executor_globals.function_table),
                        &(executor_globals.class_table),
                        sizeof(executor_globals.class_table),
                        &(executor_globals.zend_constants),
                        sizeof(executor_globals.zend_constants),
                        &(executor_globals.scope),
                        sizeof(executor_globals.scope),
                        &(executor_globals.called_scope),
                        sizeof(executor_globals.called_scope),
                        &(executor_globals.This), sizeof(executor_globals.This),
                        &(executor_globals.precision),
                        sizeof(executor_globals.precision),
                        &(executor_globals.ticks_count),
                        sizeof(executor_globals.ticks_count),
                        &(executor_globals.in_execution),
                        sizeof(executor_globals.in_execution),
                        &(executor_globals.in_autoload),
                        sizeof(executor_globals.in_autoload),
                        &(executor_globals.autoload_func),
                        sizeof(executor_globals.autoload_func),
                        &(executor_globals.full_tables_cleanup),
                        sizeof(executor_globals.full_tables_cleanup),
                        &(executor_globals.no_extensions),
                        sizeof(executor_globals.no_extensions),
                        &(executor_globals.argument_stack),
                        sizeof(executor_globals.argument_stack),
                        &(executor_globals.user_error_handler_error_reporting),
                        sizeof(executor_globals
                                   .user_error_handler_error_reporting),
                        &(executor_globals.user_error_handler),
                        sizeof(executor_globals.user_error_handler),
                        &(executor_globals.user_exception_handler),
                        sizeof(executor_globals.user_exception_handler),
                        &(executor_globals.error_handling),
                        sizeof(executor_globals.error_handling),
                        &(executor_globals.exception_class),
                        sizeof(executor_globals.exception_class),
                        &(executor_globals.timeout_seconds),
                        sizeof(executor_globals.timeout_seconds),
                        &(executor_globals.lambda_count),
                        sizeof(executor_globals.lambda_count),
                        &(executor_globals.ini_directives),
                        sizeof(executor_globals.ini_directives),
                        &(executor_globals.modified_ini_directives),
                        sizeof(executor_globals.modified_ini_directives),
                        &(executor_globals.error_reporting_ini_entry),
                        sizeof(executor_globals.error_reporting_ini_entry),
                        &(executor_globals.exception),
                        sizeof(executor_globals.exception),
                        &(executor_globals.prev_exception),
                        sizeof(executor_globals.prev_exception),
                        &(executor_globals.opline_before_exception),
                        sizeof(executor_globals.opline_before_exception),
                        &(executor_globals.current_execute_data),
                        sizeof(executor_globals.current_execute_data),
                        &(executor_globals.current_module),
                        sizeof(executor_globals.current_module),
                        &(executor_globals.active),
                        sizeof(executor_globals.active),
                        &(executor_globals.start_op),
                        sizeof(executor_globals.start_op),
                        &(executor_globals.saved_fpu_cw_ptr),
                        sizeof(executor_globals.saved_fpu_cw_ptr),
                        &(executor_globals.saved_fpu_cw),
                        sizeof(executor_globals.saved_fpu_cw)))
                  zend_rebuild_symbol_table();

                break;
              }
#endif
              }
            }
          }
          {
            switch (__choose("__SWITCH236")) {
            case 0:
              break;
#ifdef COMPILE_21622
            case 1: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_21623
            case 2: {
              // AddAndReplaceKind
              (tmp) = (executor_globals.uninitialized_zval_ptr);

              break;
            }
#endif
#ifdef COMPILE_21624
            case 3: {
              // AddAndReplaceKind
              (tmp) = *p;

              break;
            }
#endif
#ifdef COMPILE_21625
            case 4: {
              // AddAndReplaceKind
              *(tmp) = zval_used_for_init;

              break;
            }
#endif
#ifdef COMPILE_21626
            case 5: {
              // AddAndReplaceKind
              *tmp = zval_used_for_init;

              break;
            }
#endif
#ifdef COMPILE_21627
            case 6: {
              // AddAndReplaceKind
              _efree(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21628
            case 7: {
              // AddAndReplaceKind
              _efree(((tmp)->value.ht));

              break;
            }
#endif
#ifdef COMPILE_21629
            case 8: {
              // AddAndReplaceKind
              _efree(((tmp)->value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21630
            case 9: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), (key)->arKey,
                  key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21631
            case 10: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21632
            case 11: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21633
            case 12: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21634
            case 13: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_21635
            case 14: {
              // AddAndReplaceKind
              _zend_list_addref((tmp)->value.lval);

              break;
            }
#endif
#ifdef COMPILE_21636
            case 15: {
              // AddAndReplaceKind
              _zend_list_delete((tmp)->value.lval);

              break;
            }
#endif
#ifdef COMPILE_21637
            case 16: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((tmp));

              break;
            }
#endif
#ifdef COMPILE_21638
            case 17: {
              // AddAndReplaceKind
              _zval_dtor(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21639
            case 18: {
              // AddAndReplaceKind
              _zval_dtor_func((tmp));

              break;
            }
#endif
#ifdef COMPILE_21640
            case 19: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(tmp)));

              break;
            }
#endif
#ifdef COMPILE_21641
            case 20: {
              // AddAndReplaceKind
              convert_to_boolean((tmp));

              break;
            }
#endif
#ifdef COMPILE_21642
            case 21: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_21643
            case 22: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((tmp));

              break;
            }
#endif
#ifdef COMPILE_21644
            case 23: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_21645
            case 24: {
              // AddAndReplaceKind
              gc_zval_possible_root((tmp));

              break;
            }
#endif
#ifdef COMPILE_21646
            case 25: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((tmp));

              break;
            }
#endif
#ifdef COMPILE_21647
            case 26: {
              // AddAndReplaceKind
              if (!(executor_globals.active_symbol_table)) {
                zend_rebuild_symbol_table();
              }

              break;
            }
#endif
#ifdef COMPILE_21648
            case 27: {
              // AddAndReplaceKind
              if (_zend_hash_quick_add_or_update(
                      target, key->arKey, key->nKeyLength, key->h, &tmp,
                      sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
                zval_addref_p(tmp);
              }

              break;
            }
#endif
#ifdef COMPILE_21649
            case 28: {
              // AddAndReplaceKind
              is_ref = (**(p)).type & 64;

              break;
            }
#endif
#ifdef COMPILE_21650
            case 29: {
              // AddAndReplaceKind
              is_ref = (**p).type & 64;

              break;
            }
#endif
#ifdef COMPILE_21651
            case 30: {
              // AddAndReplaceKind
              tmp = (executor_globals.uninitialized_zval_ptr);

              break;
            }
#endif
#ifdef COMPILE_21652
            case 31: {
              // AddAndReplaceKind
              tmp = *(p);

              break;
            }
#endif
#ifdef COMPILE_21653
            case 32: {
              // AddAndReplaceKind
              tmp = *p;

              break;
            }
#endif
#ifdef COMPILE_21654
            case 33: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_21655
            case 34: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);

              break;
            }
#endif
#ifdef COMPILE_21656
            case 35: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Undefined variable: %s", key->arKey);

              break;
            }
#endif
#ifdef COMPILE_21657
            case 36: {
              // AddAndReplaceKind
              zend_hash_destroy((tmp)->value.ht);

              break;
            }
#endif
#ifdef COMPILE_21658
            case 37: {
              // AddAndReplaceKind
              zend_rebuild_symbol_table();

              break;
            }
#endif
#ifdef COMPILE_21659
            case 38: {
              // AddAndReplaceKind
              zval_addref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21660
            case 39: {
              // AddAndReplaceKind
              zval_addref_p(*((p)));

              break;
            }
#endif
#ifdef COMPILE_21661
            case 40: {
              // AddAndReplaceKind
              zval_addref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21662
            case 41: {
              // AddAndReplaceKind
              zval_set_isref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21663
            case 42: {
              // AddAndReplaceKind
              zval_set_isref_p(*(((p))));

              break;
            }
#endif
#ifdef COMPILE_21664
            case 43: {
              // AddAndReplaceKind
              zval_set_isref_p(*((p)));

              break;
            }
#endif
#ifdef COMPILE_21665
            case 44: {
              // AddAndReplaceKind
              zval_set_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21666
            case 45: {
              // AddAndReplaceKind
              zval_set_refcount_p((tmp), 0);

              break;
            }
#endif
#ifdef COMPILE_21667
            case 46: {
              // AddAndReplaceKind
              zval_set_refcount_p(tmp, 0);

              break;
            }
#endif
#ifdef COMPILE_21668
            case 47: {
              // AddAndReplaceKind
              zval_unset_isref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_21669
            case 48: {
              // AddAndReplaceKind
              zval_unset_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_21670
            case 49: {
              // AddInitKind
              memset(&((zval_gc_info *)(tmp))->u, 0,
                     sizeof(*(&((zval_gc_info *)(tmp))->u)));

              break;
            }
#endif
#ifdef COMPILE_21671
            case 50: {
              // AddInitKind
              memset(&(**(p)), 0, sizeof(*(&(**(p)))));

              break;
            }
#endif
#ifdef COMPILE_21672
            case 51: {
              // AddInitKind
              memset(&(**p), 0, sizeof(*(&(**p))));

              break;
            }
#endif
#ifdef COMPILE_21673
            case 52: {
              // AddInitKind
              memset(&(*tmp), 0, sizeof(*(&(*tmp))));

              break;
            }
#endif
#ifdef COMPILE_21674
            case 53: {
              // AddInitKind
              memset(&executor_globals, 0, sizeof(*(&executor_globals)));

              break;
            }
#endif
#ifdef COMPILE_21675
            case 54: {
              // AddInitKind
              memset(((zval_gc_info *)(tmp)), 0,
                     sizeof(*(((zval_gc_info *)(tmp)))));

              break;
            }
#endif
#ifdef COMPILE_21676
            case 55: {
              // AddInitKind
              memset((*(p)), 0, sizeof(*((*(p)))));

              break;
            }
#endif
#ifdef COMPILE_21677
            case 56: {
              // AddInitKind
              memset((*p), 0, sizeof(*((*p))));

              break;
            }
#endif
#ifdef COMPILE_21678
            case 57: {
              // AddInitKind
              memset((tmp), 0, sizeof(*((tmp))));

              break;
            }
#endif
#ifdef COMPILE_21679
            case 58: {
              // AddInitKind
              memset(key, 0, sizeof(*(key)));

              break;
            }
#endif
#ifdef COMPILE_21680
            case 59: {
              // IfExitKind
              if (__is_neg(
                      "236-59", 99, &(p), sizeof(p), &(key), sizeof(key),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(key->arKey), sizeof(key->arKey), &(key->nKeyLength),
                      sizeof(key->nKeyLength), &(key->h), sizeof(key->h),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(is_ref),
                      sizeof(is_ref), &(tmp), sizeof(tmp),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr), &(args),
                      sizeof(args), &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_21681
            case 60: {
              // IfExitKind
              if (__is_neg(
                      "236-60", 99, &(p), sizeof(p), &(key), sizeof(key),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(key->arKey), sizeof(key->arKey), &(key->nKeyLength),
                      sizeof(key->nKeyLength), &(key->h), sizeof(key->h),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(is_ref),
                      sizeof(is_ref), &(tmp), sizeof(tmp),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr), &(args),
                      sizeof(args), &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_21682
            case 61: {
              // IfExitKind
              if (__is_neg(
                      "236-61", 99, &(p), sizeof(p), &(key), sizeof(key),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(key->arKey), sizeof(key->arKey), &(key->nKeyLength),
                      sizeof(key->nKeyLength), &(key->h), sizeof(key->h),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(is_ref),
                      sizeof(is_ref), &(tmp), sizeof(tmp),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr), &(args),
                      sizeof(args), &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 32;

              break;
            }
#endif
#ifdef COMPILE_21683
            case 62: {
              // IfExitKind
              if (__is_neg(
                      "236-62", 99, &(p), sizeof(p), &(key), sizeof(key),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(key->arKey), sizeof(key->arKey), &(key->nKeyLength),
                      sizeof(key->nKeyLength), &(key->h), sizeof(key->h),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(is_ref),
                      sizeof(is_ref), &(tmp), sizeof(tmp),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr), &(args),
                      sizeof(args), &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 3;

              break;
            }
#endif
#ifdef COMPILE_21684
            case 63: {
              // IfExitKind
              if (__is_neg(
                      "236-63", 99, &(p), sizeof(p), &(key), sizeof(key),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table),
                      &(key->arKey), sizeof(key->arKey), &(key->nKeyLength),
                      sizeof(key->nKeyLength), &(key->h), sizeof(key->h),
                      &(executor_globals.active_symbol_table),
                      sizeof(executor_globals.active_symbol_table), &(is_ref),
                      sizeof(is_ref), &(tmp), sizeof(tmp),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr),
                      &(executor_globals.uninitialized_zval_ptr),
                      sizeof(executor_globals.uninitialized_zval_ptr), &(args),
                      sizeof(args), &(target), sizeof(target),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw),
                      &(executor_globals.return_value_ptr_ptr),
                      sizeof(executor_globals.return_value_ptr_ptr),
                      &(executor_globals.error_zval_ptr),
                      sizeof(executor_globals.error_zval_ptr),
                      &(executor_globals.symtable_cache_limit),
                      sizeof(executor_globals.symtable_cache_limit),
                      &(executor_globals.symtable_cache_ptr),
                      sizeof(executor_globals.symtable_cache_ptr),
                      &(executor_globals.opline_ptr),
                      sizeof(executor_globals.opline_ptr),
                      &(executor_globals.bailout),
                      sizeof(executor_globals.bailout),
                      &(executor_globals.error_reporting),
                      sizeof(executor_globals.error_reporting),
                      &(executor_globals.orig_error_reporting),
                      sizeof(executor_globals.orig_error_reporting),
                      &(executor_globals.exit_status),
                      sizeof(executor_globals.exit_status),
                      &(executor_globals.active_op_array),
                      sizeof(executor_globals.active_op_array),
                      &(executor_globals.function_table),
                      sizeof(executor_globals.function_table),
                      &(executor_globals.class_table),
                      sizeof(executor_globals.class_table),
                      &(executor_globals.zend_constants),
                      sizeof(executor_globals.zend_constants),
                      &(executor_globals.scope), sizeof(executor_globals.scope),
                      &(executor_globals.called_scope),
                      sizeof(executor_globals.called_scope),
                      &(executor_globals.This), sizeof(executor_globals.This),
                      &(executor_globals.precision),
                      sizeof(executor_globals.precision),
                      &(executor_globals.ticks_count),
                      sizeof(executor_globals.ticks_count),
                      &(executor_globals.in_execution),
                      sizeof(executor_globals.in_execution),
                      &(executor_globals.in_autoload),
                      sizeof(executor_globals.in_autoload),
                      &(executor_globals.autoload_func),
                      sizeof(executor_globals.autoload_func),
                      &(executor_globals.full_tables_cleanup),
                      sizeof(executor_globals.full_tables_cleanup),
                      &(executor_globals.no_extensions),
                      sizeof(executor_globals.no_extensions),
                      &(executor_globals.argument_stack),
                      sizeof(executor_globals.argument_stack),
                      &(executor_globals.user_error_handler_error_reporting),
                      sizeof(
                          executor_globals.user_error_handler_error_reporting),
                      &(executor_globals.user_error_handler),
                      sizeof(executor_globals.user_error_handler),
                      &(executor_globals.user_exception_handler),
                      sizeof(executor_globals.user_exception_handler),
                      &(executor_globals.error_handling),
                      sizeof(executor_globals.error_handling),
                      &(executor_globals.exception_class),
                      sizeof(executor_globals.exception_class),
                      &(executor_globals.timeout_seconds),
                      sizeof(executor_globals.timeout_seconds),
                      &(executor_globals.lambda_count),
                      sizeof(executor_globals.lambda_count),
                      &(executor_globals.ini_directives),
                      sizeof(executor_globals.ini_directives),
                      &(executor_globals.modified_ini_directives),
                      sizeof(executor_globals.modified_ini_directives),
                      &(executor_globals.error_reporting_ini_entry),
                      sizeof(executor_globals.error_reporting_ini_entry),
                      &(executor_globals.exception),
                      sizeof(executor_globals.exception),
                      &(executor_globals.prev_exception),
                      sizeof(executor_globals.prev_exception),
                      &(executor_globals.opline_before_exception),
                      sizeof(executor_globals.opline_before_exception),
                      &(executor_globals.current_execute_data),
                      sizeof(executor_globals.current_execute_data),
                      &(executor_globals.current_module),
                      sizeof(executor_globals.current_module),
                      &(executor_globals.active),
                      sizeof(executor_globals.active),
                      &(executor_globals.start_op),
                      sizeof(executor_globals.start_op),
                      &(executor_globals.saved_fpu_cw_ptr),
                      sizeof(executor_globals.saved_fpu_cw_ptr),
                      &(executor_globals.saved_fpu_cw),
                      sizeof(executor_globals.saved_fpu_cw)))
                return 64;

              break;
            }
#endif
            }
            {
              long long __temp237 =
                  (zend_hash_quick_find((executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        (void **)&p) == -1);
              switch (__choose("__SWITCH237")) {
              case 0: {

                break;
              }
#ifdef COMPILE_21685
              case 1: {
                __temp237 =
                    ((zend_hash_quick_find(
                          (executor_globals.active_symbol_table), key->arKey,
                          key->nKeyLength, key->h, (void **)&p) == -1) ||
                     __is_neg(
                         "237-1", 99, &(p), sizeof(p), &(key), sizeof(key),
                         &(executor_globals.active_symbol_table),
                         sizeof(executor_globals.active_symbol_table),
                         &(key->arKey), sizeof(key->arKey), &(key->nKeyLength),
                         sizeof(key->nKeyLength), &(key->h), sizeof(key->h),
                         &(executor_globals.active_symbol_table),
                         sizeof(executor_globals.active_symbol_table),
                         &(is_ref), sizeof(is_ref), &(tmp), sizeof(tmp),
                         &(executor_globals.uninitialized_zval_ptr),
                         sizeof(executor_globals.uninitialized_zval_ptr),
                         &(executor_globals.uninitialized_zval_ptr),
                         sizeof(executor_globals.uninitialized_zval_ptr),
                         &(args), sizeof(args), &(target), sizeof(target),
                         &(executor_globals.return_value_ptr_ptr),
                         sizeof(executor_globals.return_value_ptr_ptr),
                         &(executor_globals.error_zval_ptr),
                         sizeof(executor_globals.error_zval_ptr),
                         &(executor_globals.symtable_cache_limit),
                         sizeof(executor_globals.symtable_cache_limit),
                         &(executor_globals.symtable_cache_ptr),
                         sizeof(executor_globals.symtable_cache_ptr),
                         &(executor_globals.opline_ptr),
                         sizeof(executor_globals.opline_ptr),
                         &(executor_globals.bailout),
                         sizeof(executor_globals.bailout),
                         &(executor_globals.error_reporting),
                         sizeof(executor_globals.error_reporting),
                         &(executor_globals.orig_error_reporting),
                         sizeof(executor_globals.orig_error_reporting),
                         &(executor_globals.exit_status),
                         sizeof(executor_globals.exit_status),
                         &(executor_globals.active_op_array),
                         sizeof(executor_globals.active_op_array),
                         &(executor_globals.function_table),
                         sizeof(executor_globals.function_table),
                         &(executor_globals.class_table),
                         sizeof(executor_globals.class_table),
                         &(executor_globals.zend_constants),
                         sizeof(executor_globals.zend_constants),
                         &(executor_globals.scope),
                         sizeof(executor_globals.scope),
                         &(executor_globals.called_scope),
                         sizeof(executor_globals.called_scope),
                         &(executor_globals.This),
                         sizeof(executor_globals.This),
                         &(executor_globals.precision),
                         sizeof(executor_globals.precision),
                         &(executor_globals.ticks_count),
                         sizeof(executor_globals.ticks_count),
                         &(executor_globals.in_execution),
                         sizeof(executor_globals.in_execution),
                         &(executor_globals.in_autoload),
                         sizeof(executor_globals.in_autoload),
                         &(executor_globals.autoload_func),
                         sizeof(executor_globals.autoload_func),
                         &(executor_globals.full_tables_cleanup),
                         sizeof(executor_globals.full_tables_cleanup),
                         &(executor_globals.no_extensions),
                         sizeof(executor_globals.no_extensions),
                         &(executor_globals.argument_stack),
                         sizeof(executor_globals.argument_stack),
                         &(executor_globals.user_error_handler_error_reporting),
                         sizeof(executor_globals
                                    .user_error_handler_error_reporting),
                         &(executor_globals.user_error_handler),
                         sizeof(executor_globals.user_error_handler),
                         &(executor_globals.user_exception_handler),
                         sizeof(executor_globals.user_exception_handler),
                         &(executor_globals.error_handling),
                         sizeof(executor_globals.error_handling),
                         &(executor_globals.exception_class),
                         sizeof(executor_globals.exception_class),
                         &(executor_globals.timeout_seconds),
                         sizeof(executor_globals.timeout_seconds),
                         &(executor_globals.lambda_count),
                         sizeof(executor_globals.lambda_count),
                         &(executor_globals.ini_directives),
                         sizeof(executor_globals.ini_directives),
                         &(executor_globals.modified_ini_directives),
                         sizeof(executor_globals.modified_ini_directives),
                         &(executor_globals.error_reporting_ini_entry),
                         sizeof(executor_globals.error_reporting_ini_entry),
                         &(executor_globals.exception),
                         sizeof(executor_globals.exception),
                         &(executor_globals.prev_exception),
                         sizeof(executor_globals.prev_exception),
                         &(executor_globals.opline_before_exception),
                         sizeof(executor_globals.opline_before_exception),
                         &(executor_globals.current_execute_data),
                         sizeof(executor_globals.current_execute_data),
                         &(executor_globals.current_module),
                         sizeof(executor_globals.current_module),
                         &(executor_globals.active),
                         sizeof(executor_globals.active),
                         &(executor_globals.start_op),
                         sizeof(executor_globals.start_op),
                         &(executor_globals.saved_fpu_cw_ptr),
                         sizeof(executor_globals.saved_fpu_cw_ptr),
                         &(executor_globals.saved_fpu_cw),
                         sizeof(executor_globals.saved_fpu_cw),
                         &(executor_globals.return_value_ptr_ptr),
                         sizeof(executor_globals.return_value_ptr_ptr),
                         &(executor_globals.error_zval_ptr),
                         sizeof(executor_globals.error_zval_ptr),
                         &(executor_globals.symtable_cache_limit),
                         sizeof(executor_globals.symtable_cache_limit),
                         &(executor_globals.symtable_cache_ptr),
                         sizeof(executor_globals.symtable_cache_ptr),
                         &(executor_globals.opline_ptr),
                         sizeof(executor_globals.opline_ptr),
                         &(executor_globals.bailout),
                         sizeof(executor_globals.bailout),
                         &(executor_globals.error_reporting),
                         sizeof(executor_globals.error_reporting),
                         &(executor_globals.orig_error_reporting),
                         sizeof(executor_globals.orig_error_reporting),
                         &(executor_globals.exit_status),
                         sizeof(executor_globals.exit_status),
                         &(executor_globals.active_op_array),
                         sizeof(executor_globals.active_op_array),
                         &(executor_globals.function_table),
                         sizeof(executor_globals.function_table),
                         &(executor_globals.class_table),
                         sizeof(executor_globals.class_table),
                         &(executor_globals.zend_constants),
                         sizeof(executor_globals.zend_constants),
                         &(executor_globals.scope),
                         sizeof(executor_globals.scope),
                         &(executor_globals.called_scope),
                         sizeof(executor_globals.called_scope),
                         &(executor_globals.This),
                         sizeof(executor_globals.This),
                         &(executor_globals.precision),
                         sizeof(executor_globals.precision),
                         &(executor_globals.ticks_count),
                         sizeof(executor_globals.ticks_count),
                         &(executor_globals.in_execution),
                         sizeof(executor_globals.in_execution),
                         &(executor_globals.in_autoload),
                         sizeof(executor_globals.in_autoload),
                         &(executor_globals.autoload_func),
                         sizeof(executor_globals.autoload_func),
                         &(executor_globals.full_tables_cleanup),
                         sizeof(executor_globals.full_tables_cleanup),
                         &(executor_globals.no_extensions),
                         sizeof(executor_globals.no_extensions),
                         &(executor_globals.argument_stack),
                         sizeof(executor_globals.argument_stack),
                         &(executor_globals.user_error_handler_error_reporting),
                         sizeof(executor_globals
                                    .user_error_handler_error_reporting),
                         &(executor_globals.user_error_handler),
                         sizeof(executor_globals.user_error_handler),
                         &(executor_globals.user_exception_handler),
                         sizeof(executor_globals.user_exception_handler),
                         &(executor_globals.error_handling),
                         sizeof(executor_globals.error_handling),
                         &(executor_globals.exception_class),
                         sizeof(executor_globals.exception_class),
                         &(executor_globals.timeout_seconds),
                         sizeof(executor_globals.timeout_seconds),
                         &(executor_globals.lambda_count),
                         sizeof(executor_globals.lambda_count),
                         &(executor_globals.ini_directives),
                         sizeof(executor_globals.ini_directives),
                         &(executor_globals.modified_ini_directives),
                         sizeof(executor_globals.modified_ini_directives),
                         &(executor_globals.error_reporting_ini_entry),
                         sizeof(executor_globals.error_reporting_ini_entry),
                         &(executor_globals.exception),
                         sizeof(executor_globals.exception),
                         &(executor_globals.prev_exception),
                         sizeof(executor_globals.prev_exception),
                         &(executor_globals.opline_before_exception),
                         sizeof(executor_globals.opline_before_exception),
                         &(executor_globals.current_execute_data),
                         sizeof(executor_globals.current_execute_data),
                         &(executor_globals.current_module),
                         sizeof(executor_globals.current_module),
                         &(executor_globals.active),
                         sizeof(executor_globals.active),
                         &(executor_globals.start_op),
                         sizeof(executor_globals.start_op),
                         &(executor_globals.saved_fpu_cw_ptr),
                         sizeof(executor_globals.saved_fpu_cw_ptr),
                         &(executor_globals.saved_fpu_cw),
                         sizeof(executor_globals.saved_fpu_cw)));

                break;
              }
#endif
#ifdef COMPILE_21686
              case 2: {
                __temp237 =
                    ((zend_hash_quick_find(
                          (executor_globals.active_symbol_table), key->arKey,
                          key->nKeyLength, key->h, (void **)&p) == -1) &&
                     !__is_neg(
                         "237-2", 99, &(p), sizeof(p), &(key), sizeof(key),
                         &(executor_globals.active_symbol_table),
                         sizeof(executor_globals.active_symbol_table),
                         &(key->arKey), sizeof(key->arKey), &(key->nKeyLength),
                         sizeof(key->nKeyLength), &(key->h), sizeof(key->h),
                         &(executor_globals.active_symbol_table),
                         sizeof(executor_globals.active_symbol_table),
                         &(is_ref), sizeof(is_ref), &(tmp), sizeof(tmp),
                         &(executor_globals.uninitialized_zval_ptr),
                         sizeof(executor_globals.uninitialized_zval_ptr),
                         &(executor_globals.uninitialized_zval_ptr),
                         sizeof(executor_globals.uninitialized_zval_ptr),
                         &(args), sizeof(args), &(target), sizeof(target),
                         &(executor_globals.return_value_ptr_ptr),
                         sizeof(executor_globals.return_value_ptr_ptr),
                         &(executor_globals.error_zval_ptr),
                         sizeof(executor_globals.error_zval_ptr),
                         &(executor_globals.symtable_cache_limit),
                         sizeof(executor_globals.symtable_cache_limit),
                         &(executor_globals.symtable_cache_ptr),
                         sizeof(executor_globals.symtable_cache_ptr),
                         &(executor_globals.opline_ptr),
                         sizeof(executor_globals.opline_ptr),
                         &(executor_globals.bailout),
                         sizeof(executor_globals.bailout),
                         &(executor_globals.error_reporting),
                         sizeof(executor_globals.error_reporting),
                         &(executor_globals.orig_error_reporting),
                         sizeof(executor_globals.orig_error_reporting),
                         &(executor_globals.exit_status),
                         sizeof(executor_globals.exit_status),
                         &(executor_globals.active_op_array),
                         sizeof(executor_globals.active_op_array),
                         &(executor_globals.function_table),
                         sizeof(executor_globals.function_table),
                         &(executor_globals.class_table),
                         sizeof(executor_globals.class_table),
                         &(executor_globals.zend_constants),
                         sizeof(executor_globals.zend_constants),
                         &(executor_globals.scope),
                         sizeof(executor_globals.scope),
                         &(executor_globals.called_scope),
                         sizeof(executor_globals.called_scope),
                         &(executor_globals.This),
                         sizeof(executor_globals.This),
                         &(executor_globals.precision),
                         sizeof(executor_globals.precision),
                         &(executor_globals.ticks_count),
                         sizeof(executor_globals.ticks_count),
                         &(executor_globals.in_execution),
                         sizeof(executor_globals.in_execution),
                         &(executor_globals.in_autoload),
                         sizeof(executor_globals.in_autoload),
                         &(executor_globals.autoload_func),
                         sizeof(executor_globals.autoload_func),
                         &(executor_globals.full_tables_cleanup),
                         sizeof(executor_globals.full_tables_cleanup),
                         &(executor_globals.no_extensions),
                         sizeof(executor_globals.no_extensions),
                         &(executor_globals.argument_stack),
                         sizeof(executor_globals.argument_stack),
                         &(executor_globals.user_error_handler_error_reporting),
                         sizeof(executor_globals
                                    .user_error_handler_error_reporting),
                         &(executor_globals.user_error_handler),
                         sizeof(executor_globals.user_error_handler),
                         &(executor_globals.user_exception_handler),
                         sizeof(executor_globals.user_exception_handler),
                         &(executor_globals.error_handling),
                         sizeof(executor_globals.error_handling),
                         &(executor_globals.exception_class),
                         sizeof(executor_globals.exception_class),
                         &(executor_globals.timeout_seconds),
                         sizeof(executor_globals.timeout_seconds),
                         &(executor_globals.lambda_count),
                         sizeof(executor_globals.lambda_count),
                         &(executor_globals.ini_directives),
                         sizeof(executor_globals.ini_directives),
                         &(executor_globals.modified_ini_directives),
                         sizeof(executor_globals.modified_ini_directives),
                         &(executor_globals.error_reporting_ini_entry),
                         sizeof(executor_globals.error_reporting_ini_entry),
                         &(executor_globals.exception),
                         sizeof(executor_globals.exception),
                         &(executor_globals.prev_exception),
                         sizeof(executor_globals.prev_exception),
                         &(executor_globals.opline_before_exception),
                         sizeof(executor_globals.opline_before_exception),
                         &(executor_globals.current_execute_data),
                         sizeof(executor_globals.current_execute_data),
                         &(executor_globals.current_module),
                         sizeof(executor_globals.current_module),
                         &(executor_globals.active),
                         sizeof(executor_globals.active),
                         &(executor_globals.start_op),
                         sizeof(executor_globals.start_op),
                         &(executor_globals.saved_fpu_cw_ptr),
                         sizeof(executor_globals.saved_fpu_cw_ptr),
                         &(executor_globals.saved_fpu_cw),
                         sizeof(executor_globals.saved_fpu_cw),
                         &(executor_globals.return_value_ptr_ptr),
                         sizeof(executor_globals.return_value_ptr_ptr),
                         &(executor_globals.error_zval_ptr),
                         sizeof(executor_globals.error_zval_ptr),
                         &(executor_globals.symtable_cache_limit),
                         sizeof(executor_globals.symtable_cache_limit),
                         &(executor_globals.symtable_cache_ptr),
                         sizeof(executor_globals.symtable_cache_ptr),
                         &(executor_globals.opline_ptr),
                         sizeof(executor_globals.opline_ptr),
                         &(executor_globals.bailout),
                         sizeof(executor_globals.bailout),
                         &(executor_globals.error_reporting),
                         sizeof(executor_globals.error_reporting),
                         &(executor_globals.orig_error_reporting),
                         sizeof(executor_globals.orig_error_reporting),
                         &(executor_globals.exit_status),
                         sizeof(executor_globals.exit_status),
                         &(executor_globals.active_op_array),
                         sizeof(executor_globals.active_op_array),
                         &(executor_globals.function_table),
                         sizeof(executor_globals.function_table),
                         &(executor_globals.class_table),
                         sizeof(executor_globals.class_table),
                         &(executor_globals.zend_constants),
                         sizeof(executor_globals.zend_constants),
                         &(executor_globals.scope),
                         sizeof(executor_globals.scope),
                         &(executor_globals.called_scope),
                         sizeof(executor_globals.called_scope),
                         &(executor_globals.This),
                         sizeof(executor_globals.This),
                         &(executor_globals.precision),
                         sizeof(executor_globals.precision),
                         &(executor_globals.ticks_count),
                         sizeof(executor_globals.ticks_count),
                         &(executor_globals.in_execution),
                         sizeof(executor_globals.in_execution),
                         &(executor_globals.in_autoload),
                         sizeof(executor_globals.in_autoload),
                         &(executor_globals.autoload_func),
                         sizeof(executor_globals.autoload_func),
                         &(executor_globals.full_tables_cleanup),
                         sizeof(executor_globals.full_tables_cleanup),
                         &(executor_globals.no_extensions),
                         sizeof(executor_globals.no_extensions),
                         &(executor_globals.argument_stack),
                         sizeof(executor_globals.argument_stack),
                         &(executor_globals.user_error_handler_error_reporting),
                         sizeof(executor_globals
                                    .user_error_handler_error_reporting),
                         &(executor_globals.user_error_handler),
                         sizeof(executor_globals.user_error_handler),
                         &(executor_globals.user_exception_handler),
                         sizeof(executor_globals.user_exception_handler),
                         &(executor_globals.error_handling),
                         sizeof(executor_globals.error_handling),
                         &(executor_globals.exception_class),
                         sizeof(executor_globals.exception_class),
                         &(executor_globals.timeout_seconds),
                         sizeof(executor_globals.timeout_seconds),
                         &(executor_globals.lambda_count),
                         sizeof(executor_globals.lambda_count),
                         &(executor_globals.ini_directives),
                         sizeof(executor_globals.ini_directives),
                         &(executor_globals.modified_ini_directives),
                         sizeof(executor_globals.modified_ini_directives),
                         &(executor_globals.error_reporting_ini_entry),
                         sizeof(executor_globals.error_reporting_ini_entry),
                         &(executor_globals.exception),
                         sizeof(executor_globals.exception),
                         &(executor_globals.prev_exception),
                         sizeof(executor_globals.prev_exception),
                         &(executor_globals.opline_before_exception),
                         sizeof(executor_globals.opline_before_exception),
                         &(executor_globals.current_execute_data),
                         sizeof(executor_globals.current_execute_data),
                         &(executor_globals.current_module),
                         sizeof(executor_globals.current_module),
                         &(executor_globals.active),
                         sizeof(executor_globals.active),
                         &(executor_globals.start_op),
                         sizeof(executor_globals.start_op),
                         &(executor_globals.saved_fpu_cw_ptr),
                         sizeof(executor_globals.saved_fpu_cw_ptr),
                         &(executor_globals.saved_fpu_cw),
                         sizeof(executor_globals.saved_fpu_cw)));

                break;
              }
#endif
              }
              switch (__choose("__SWITCH252")) {
              case 0: {
                if (__temp237) {
                  if (is_ref) {
                    ALLOC_INIT_ZVAL(tmp);
                    Z_SET_ISREF_P(tmp);
                    zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                                        key->nKeyLength, key->h, &tmp,
                                        sizeof(zval *), (void **)&p);
                  } else {
                    tmp = EG(uninitialized_zval_ptr);
                    zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
                  }
                } else {
                  {
                    switch (__choose("__SWITCH238")) {
                    case 0:
                      break;
#ifdef COMPILE_21687
                    case 1: {
                      // AddAndReplaceKind
                      (executor_globals.argument_stack) =
                          zend_vm_stack_new_page(((16 * 1024) - 16));

                      break;
                    }
#endif
#ifdef COMPILE_21688
                    case 2: {
                      // AddAndReplaceKind
                      (tmp) = (executor_globals.uninitialized_zval_ptr);

                      break;
                    }
#endif
#ifdef COMPILE_21689
                    case 3: {
                      // AddAndReplaceKind
                      (tmp) = *p;

                      break;
                    }
#endif
#ifdef COMPILE_21690
                    case 4: {
                      // AddAndReplaceKind
                      *(tmp) = zval_used_for_init;

                      break;
                    }
#endif
#ifdef COMPILE_21691
                    case 5: {
                      // AddAndReplaceKind
                      *tmp = zval_used_for_init;

                      break;
                    }
#endif
#ifdef COMPILE_21692
                    case 6: {
                      // AddAndReplaceKind
                      _efree(((tmp)));

                      break;
                    }
#endif
#ifdef COMPILE_21693
                    case 7: {
                      // AddAndReplaceKind
                      _efree(((tmp)->value.ht));

                      break;
                    }
#endif
#ifdef COMPILE_21694
                    case 8: {
                      // AddAndReplaceKind
                      _efree(((tmp)->value.str.val));

                      break;
                    }
#endif
#ifdef COMPILE_21695
                    case 9: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), (key)->arKey,
                          key->nKeyLength, key->h, &tmp, sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21696
                    case 10: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), key->arKey,
                          (key)->nKeyLength, key->h, &tmp, sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21697
                    case 11: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), key->arKey,
                          key->nKeyLength, (key)->h, &tmp, sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21698
                    case 12: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), key->arKey,
                          key->nKeyLength, key->h, &(tmp), sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21699
                    case 13: {
                      // AddAndReplaceKind
                      _zend_hash_quick_add_or_update(
                          (executor_globals.active_symbol_table), key->arKey,
                          key->nKeyLength, key->h, &tmp, sizeof(zval *),
                          (void **)&p, (1 << 1));

                      break;
                    }
#endif
#ifdef COMPILE_21700
                    case 14: {
                      // AddAndReplaceKind
                      _zend_list_addref((tmp)->value.lval);

                      break;
                    }
#endif
#ifdef COMPILE_21701
                    case 15: {
                      // AddAndReplaceKind
                      _zend_list_delete((tmp)->value.lval);

                      break;
                    }
#endif
#ifdef COMPILE_21702
                    case 16: {
                      // AddAndReplaceKind
                      _zval_copy_ctor_func((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21703
                    case 17: {
                      // AddAndReplaceKind
                      _zval_dtor(((tmp)));

                      break;
                    }
#endif
#ifdef COMPILE_21704
                    case 18: {
                      // AddAndReplaceKind
                      _zval_dtor_func((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21705
                    case 19: {
                      // AddAndReplaceKind
                      _zval_ptr_dtor((&(tmp)));

                      break;
                    }
#endif
#ifdef COMPILE_21706
                    case 20: {
                      // AddAndReplaceKind
                      convert_to_boolean((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21707
                    case 21: {
                      // AddAndReplaceKind
                      exit(1);

                      break;
                    }
#endif
#ifdef COMPILE_21708
                    case 22: {
                      // AddAndReplaceKind
                      gc_remove_zval_from_buffer((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21709
                    case 23: {
                      // AddAndReplaceKind
                      gc_zval_check_possible_root(((tmp)));

                      break;
                    }
#endif
#ifdef COMPILE_21710
                    case 24: {
                      // AddAndReplaceKind
                      gc_zval_possible_root((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21711
                    case 25: {
                      // AddAndReplaceKind
                      i_zval_ptr_dtor((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21712
                    case 26: {
                      // AddAndReplaceKind
                      if (!(executor_globals.active_symbol_table)) {
                        zend_rebuild_symbol_table();
                      }

                      break;
                    }
#endif
#ifdef COMPILE_21713
                    case 27: {
                      // AddAndReplaceKind
                      if (_zend_hash_quick_add_or_update(
                              target, key->arKey, key->nKeyLength, key->h, &tmp,
                              sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
                        zval_addref_p(tmp);
                      }

                      break;
                    }
#endif
#ifdef COMPILE_21714
                    case 28: {
                      // AddAndReplaceKind
                      is_ref = (**(p)).type & 64;

                      break;
                    }
#endif
#ifdef COMPILE_21715
                    case 29: {
                      // AddAndReplaceKind
                      is_ref = (**p).type & 64;

                      break;
                    }
#endif
#ifdef COMPILE_21716
                    case 30: {
                      // AddAndReplaceKind
                      tmp = (executor_globals.uninitialized_zval_ptr);

                      break;
                    }
#endif
#ifdef COMPILE_21717
                    case 31: {
                      // AddAndReplaceKind
                      tmp = *(p);

                      break;
                    }
#endif
#ifdef COMPILE_21718
                    case 32: {
                      // AddAndReplaceKind
                      tmp = *p;

                      break;
                    }
#endif
#ifdef COMPILE_21719
                    case 33: {
                      // AddAndReplaceKind
                      zend_error((1 << 3L),
                                 "A non well formed numeric value encountered");

                      break;
                    }
#endif
#ifdef COMPILE_21720
                    case 34: {
                      // AddAndReplaceKind
                      zend_error((1 << 3L), "Undefined variable: %s",
                                 (key)->arKey);

                      break;
                    }
#endif
#ifdef COMPILE_21721
                    case 35: {
                      // AddAndReplaceKind
                      zend_error((1 << 3L), "Undefined variable: %s",
                                 key->arKey);

                      break;
                    }
#endif
#ifdef COMPILE_21722
                    case 36: {
                      // AddAndReplaceKind
                      zend_hash_destroy((tmp)->value.ht);

                      break;
                    }
#endif
#ifdef COMPILE_21723
                    case 37: {
                      // AddAndReplaceKind
                      zend_rebuild_symbol_table();

                      break;
                    }
#endif
#ifdef COMPILE_21724
                    case 38: {
                      // AddAndReplaceKind
                      zval_addref_p((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21725
                    case 39: {
                      // AddAndReplaceKind
                      zval_addref_p(*((p)));

                      break;
                    }
#endif
#ifdef COMPILE_21726
                    case 40: {
                      // AddAndReplaceKind
                      zval_addref_p(tmp);

                      break;
                    }
#endif
#ifdef COMPILE_21727
                    case 41: {
                      // AddAndReplaceKind
                      zval_set_isref_p((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21728
                    case 42: {
                      // AddAndReplaceKind
                      zval_set_isref_p(*(((p))));

                      break;
                    }
#endif
#ifdef COMPILE_21729
                    case 43: {
                      // AddAndReplaceKind
                      zval_set_isref_p(*((p)));

                      break;
                    }
#endif
#ifdef COMPILE_21730
                    case 44: {
                      // AddAndReplaceKind
                      zval_set_isref_p(tmp);

                      break;
                    }
#endif
#ifdef COMPILE_21731
                    case 45: {
                      // AddAndReplaceKind
                      zval_set_refcount_p((tmp), 0);

                      break;
                    }
#endif
#ifdef COMPILE_21732
                    case 46: {
                      // AddAndReplaceKind
                      zval_set_refcount_p(tmp, 0);

                      break;
                    }
#endif
#ifdef COMPILE_21733
                    case 47: {
                      // AddAndReplaceKind
                      zval_unset_isref_p((tmp));

                      break;
                    }
#endif
#ifdef COMPILE_21734
                    case 48: {
                      // AddAndReplaceKind
                      zval_unset_isref_p(tmp);

                      break;
                    }
#endif
#ifdef COMPILE_21735
                    case 49: {
                      // AddInitKind
                      memset(&((zval_gc_info *)(tmp))->u, 0,
                             sizeof(*(&((zval_gc_info *)(tmp))->u)));

                      break;
                    }
#endif
#ifdef COMPILE_21736
                    case 50: {
                      // AddInitKind
                      memset(&(**(p)), 0, sizeof(*(&(**(p)))));

                      break;
                    }
#endif
#ifdef COMPILE_21737
                    case 51: {
                      // AddInitKind
                      memset(&(**p), 0, sizeof(*(&(**p))));

                      break;
                    }
#endif
#ifdef COMPILE_21738
                    case 52: {
                      // AddInitKind
                      memset(&(*tmp), 0, sizeof(*(&(*tmp))));

                      break;
                    }
#endif
#ifdef COMPILE_21739
                    case 53: {
                      // AddInitKind
                      memset(((zval_gc_info *)(tmp)), 0,
                             sizeof(*(((zval_gc_info *)(tmp)))));

                      break;
                    }
#endif
#ifdef COMPILE_21740
                    case 54: {
                      // AddInitKind
                      memset((*(p)), 0, sizeof(*((*(p)))));

                      break;
                    }
#endif
#ifdef COMPILE_21741
                    case 55: {
                      // AddInitKind
                      memset((*p), 0, sizeof(*((*p))));

                      break;
                    }
#endif
#ifdef COMPILE_21742
                    case 56: {
                      // AddInitKind
                      memset((tmp), 0, sizeof(*((tmp))));

                      break;
                    }
#endif
#ifdef COMPILE_21743
                    case 57: {
                      // IfExitKind
                      if (__is_neg(
                              "238-57", 12, &(is_ref), sizeof(is_ref), &(p),
                              sizeof(p), &((**(p)).type), sizeof((**(p)).type),
                              &(tmp), sizeof(tmp), &(key), sizeof(key),
                              &(target), sizeof(target), &(args), sizeof(args),
                              &((*(p))->refcount__gc),
                              sizeof((*(p))->refcount__gc), &((*(p))->type),
                              sizeof((*(p))->type), &((*(p))->is_ref__gc),
                              sizeof((*(p))->is_ref__gc),
                              &((**(p)).refcount__gc),
                              sizeof((**(p)).refcount__gc),
                              &((**(p)).is_ref__gc),
                              sizeof((**(p)).is_ref__gc)))
                        return 0;

                      break;
                    }
#endif
#ifdef COMPILE_21744
                    case 58: {
                      // IfExitKind
                      if (__is_neg(
                              "238-58", 12, &(is_ref), sizeof(is_ref), &(p),
                              sizeof(p), &((**(p)).type), sizeof((**(p)).type),
                              &(tmp), sizeof(tmp), &(key), sizeof(key),
                              &(target), sizeof(target), &(args), sizeof(args),
                              &((*(p))->refcount__gc),
                              sizeof((*(p))->refcount__gc), &((*(p))->type),
                              sizeof((*(p))->type), &((*(p))->is_ref__gc),
                              sizeof((*(p))->is_ref__gc),
                              &((**(p)).refcount__gc),
                              sizeof((**(p)).refcount__gc),
                              &((**(p)).is_ref__gc),
                              sizeof((**(p)).is_ref__gc)))
                        return 1;

                      break;
                    }
#endif
#ifdef COMPILE_21745
                    case 59: {
                      // IfExitKind
                      if (__is_neg(
                              "238-59", 12, &(is_ref), sizeof(is_ref), &(p),
                              sizeof(p), &((**(p)).type), sizeof((**(p)).type),
                              &(tmp), sizeof(tmp), &(key), sizeof(key),
                              &(target), sizeof(target), &(args), sizeof(args),
                              &((*(p))->refcount__gc),
                              sizeof((*(p))->refcount__gc), &((*(p))->type),
                              sizeof((*(p))->type), &((*(p))->is_ref__gc),
                              sizeof((*(p))->is_ref__gc),
                              &((**(p)).refcount__gc),
                              sizeof((**(p)).refcount__gc),
                              &((**(p)).is_ref__gc),
                              sizeof((**(p)).is_ref__gc)))
                        return 32;

                      break;
                    }
#endif
#ifdef COMPILE_21746
                    case 60: {
                      // IfExitKind
                      if (__is_neg(
                              "238-60", 12, &(is_ref), sizeof(is_ref), &(p),
                              sizeof(p), &((**(p)).type), sizeof((**(p)).type),
                              &(tmp), sizeof(tmp), &(key), sizeof(key),
                              &(target), sizeof(target), &(args), sizeof(args),
                              &((*(p))->refcount__gc),
                              sizeof((*(p))->refcount__gc), &((*(p))->type),
                              sizeof((*(p))->type), &((*(p))->is_ref__gc),
                              sizeof((*(p))->is_ref__gc),
                              &((**(p)).refcount__gc),
                              sizeof((**(p)).refcount__gc),
                              &((**(p)).is_ref__gc),
                              sizeof((**(p)).is_ref__gc)))
                        return 3;

                      break;
                    }
#endif
#ifdef COMPILE_21747
                    case 61: {
                      // IfExitKind
                      if (__is_neg(
                              "238-61", 12, &(is_ref), sizeof(is_ref), &(p),
                              sizeof(p), &((**(p)).type), sizeof((**(p)).type),
                              &(tmp), sizeof(tmp), &(key), sizeof(key),
                              &(target), sizeof(target), &(args), sizeof(args),
                              &((*(p))->refcount__gc),
                              sizeof((*(p))->refcount__gc), &((*(p))->type),
                              sizeof((*(p))->type), &((*(p))->is_ref__gc),
                              sizeof((*(p))->is_ref__gc),
                              &((**(p)).refcount__gc),
                              sizeof((**(p)).refcount__gc),
                              &((**(p)).is_ref__gc),
                              sizeof((**(p)).is_ref__gc)))
                        return 64;

                      break;
                    }
#endif
                    }
                    {
                      long long __temp239 = (is_ref);
                      switch (__choose("__SWITCH239")) {
                      case 0: {

                        break;
                      }
#ifdef COMPILE_21748
                      case 1: {
                        __temp239 =
                            ((is_ref) ||
                             __is_neg(
                                 "239-1", 12, &(is_ref), sizeof(is_ref), &(p),
                                 sizeof(p), &((**(p)).type),
                                 sizeof((**(p)).type), &(tmp), sizeof(tmp),
                                 &(key), sizeof(key), &(target), sizeof(target),
                                 &(args), sizeof(args), &((*(p))->refcount__gc),
                                 sizeof((*(p))->refcount__gc), &((*(p))->type),
                                 sizeof((*(p))->type), &((*(p))->is_ref__gc),
                                 sizeof((*(p))->is_ref__gc),
                                 &((**(p)).refcount__gc),
                                 sizeof((**(p)).refcount__gc),
                                 &((**(p)).is_ref__gc),
                                 sizeof((**(p)).is_ref__gc)));

                        break;
                      }
#endif
#ifdef COMPILE_21749
                      case 2: {
                        __temp239 =
                            ((is_ref) &&
                             !__is_neg(
                                 "239-2", 12, &(is_ref), sizeof(is_ref), &(p),
                                 sizeof(p), &((**(p)).type),
                                 sizeof((**(p)).type), &(tmp), sizeof(tmp),
                                 &(key), sizeof(key), &(target), sizeof(target),
                                 &(args), sizeof(args), &((*(p))->refcount__gc),
                                 sizeof((*(p))->refcount__gc), &((*(p))->type),
                                 sizeof((*(p))->type), &((*(p))->is_ref__gc),
                                 sizeof((*(p))->is_ref__gc),
                                 &((**(p)).refcount__gc),
                                 sizeof((**(p)).refcount__gc),
                                 &((**(p)).is_ref__gc),
                                 sizeof((**(p)).is_ref__gc)));

                        break;
                      }
#endif
                      }
                      switch (__choose("__SWITCH251")) {
                      case 0: {
                        if (__temp239) {
                          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
                          tmp = *p;
                        } else {
                          switch (__choose("__SWITCH240")) {
                          case 0:
                            break;
#ifdef COMPILE_21750
                          case 1: {
                            // AddAndReplaceKind
                            (executor_globals.argument_stack) =
                                zend_vm_stack_new_page(((16 * 1024) - 16));

                            break;
                          }
#endif
#ifdef COMPILE_21751
                          case 2: {
                            // AddAndReplaceKind
                            (tmp) = (executor_globals.uninitialized_zval_ptr);

                            break;
                          }
#endif
#ifdef COMPILE_21752
                          case 3: {
                            // AddAndReplaceKind
                            (tmp) = *p;

                            break;
                          }
#endif
#ifdef COMPILE_21753
                          case 4: {
                            // AddAndReplaceKind
                            *(tmp) = zval_used_for_init;

                            break;
                          }
#endif
#ifdef COMPILE_21754
                          case 5: {
                            // AddAndReplaceKind
                            *tmp = zval_used_for_init;

                            break;
                          }
#endif
#ifdef COMPILE_21755
                          case 6: {
                            // AddAndReplaceKind
                            _efree(((tmp)));

                            break;
                          }
#endif
#ifdef COMPILE_21756
                          case 7: {
                            // AddAndReplaceKind
                            _efree(((tmp)->value.ht));

                            break;
                          }
#endif
#ifdef COMPILE_21757
                          case 8: {
                            // AddAndReplaceKind
                            _efree(((tmp)->value.str.val));

                            break;
                          }
#endif
#ifdef COMPILE_21758
                          case 9: {
                            // AddAndReplaceKind
                            _zend_hash_quick_add_or_update(
                                (executor_globals.active_symbol_table),
                                (key)->arKey, key->nKeyLength, key->h, &tmp,
                                sizeof(zval *), (void **)&p, (1 << 1));

                            break;
                          }
#endif
#ifdef COMPILE_21759
                          case 10: {
                            // AddAndReplaceKind
                            _zend_hash_quick_add_or_update(
                                (executor_globals.active_symbol_table),
                                key->arKey, (key)->nKeyLength, key->h, &tmp,
                                sizeof(zval *), (void **)&p, (1 << 1));

                            break;
                          }
#endif
#ifdef COMPILE_21760
                          case 11: {
                            // AddAndReplaceKind
                            _zend_hash_quick_add_or_update(
                                (executor_globals.active_symbol_table),
                                key->arKey, key->nKeyLength, (key)->h, &tmp,
                                sizeof(zval *), (void **)&p, (1 << 1));

                            break;
                          }
#endif
#ifdef COMPILE_21761
                          case 12: {
                            // AddAndReplaceKind
                            _zend_hash_quick_add_or_update(
                                (executor_globals.active_symbol_table),
                                key->arKey, key->nKeyLength, key->h, &(tmp),
                                sizeof(zval *), (void **)&p, (1 << 1));

                            break;
                          }
#endif
#ifdef COMPILE_21762
                          case 13: {
                            // AddAndReplaceKind
                            _zend_hash_quick_add_or_update(
                                (executor_globals.active_symbol_table),
                                key->arKey, key->nKeyLength, key->h, &tmp,
                                sizeof(zval *), (void **)&p, (1 << 1));

                            break;
                          }
#endif
#ifdef COMPILE_21763
                          case 14: {
                            // AddAndReplaceKind
                            _zend_list_addref((tmp)->value.lval);

                            break;
                          }
#endif
#ifdef COMPILE_21764
                          case 15: {
                            // AddAndReplaceKind
                            _zend_list_delete((tmp)->value.lval);

                            break;
                          }
#endif
#ifdef COMPILE_21765
                          case 16: {
                            // AddAndReplaceKind
                            _zval_copy_ctor_func((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21766
                          case 17: {
                            // AddAndReplaceKind
                            _zval_dtor(((tmp)));

                            break;
                          }
#endif
#ifdef COMPILE_21767
                          case 18: {
                            // AddAndReplaceKind
                            _zval_dtor_func((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21768
                          case 19: {
                            // AddAndReplaceKind
                            _zval_ptr_dtor((&(tmp)));

                            break;
                          }
#endif
#ifdef COMPILE_21769
                          case 20: {
                            // AddAndReplaceKind
                            convert_to_boolean((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21770
                          case 21: {
                            // AddAndReplaceKind
                            exit(1);

                            break;
                          }
#endif
#ifdef COMPILE_21771
                          case 22: {
                            // AddAndReplaceKind
                            gc_remove_zval_from_buffer((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21772
                          case 23: {
                            // AddAndReplaceKind
                            gc_zval_check_possible_root(((tmp)));

                            break;
                          }
#endif
#ifdef COMPILE_21773
                          case 24: {
                            // AddAndReplaceKind
                            gc_zval_possible_root((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21774
                          case 25: {
                            // AddAndReplaceKind
                            i_zval_ptr_dtor((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21775
                          case 26: {
                            // AddAndReplaceKind
                            if (!(executor_globals.active_symbol_table)) {
                              zend_rebuild_symbol_table();
                            }

                            break;
                          }
#endif
#ifdef COMPILE_21776
                          case 27: {
                            // AddAndReplaceKind
                            if (_zend_hash_quick_add_or_update(
                                    target, key->arKey, key->nKeyLength, key->h,
                                    &tmp, sizeof(zval *), ((void *)0),
                                    (1 << 1)) == 0) {
                              zval_addref_p(tmp);
                            }

                            break;
                          }
#endif
#ifdef COMPILE_21777
                          case 28: {
                            // AddAndReplaceKind
                            is_ref = (**(p)).type & 64;

                            break;
                          }
#endif
#ifdef COMPILE_21778
                          case 29: {
                            // AddAndReplaceKind
                            is_ref = (**p).type & 64;

                            break;
                          }
#endif
#ifdef COMPILE_21779
                          case 30: {
                            // AddAndReplaceKind
                            tmp = (executor_globals.uninitialized_zval_ptr);

                            break;
                          }
#endif
#ifdef COMPILE_21780
                          case 31: {
                            // AddAndReplaceKind
                            tmp = *(p);

                            break;
                          }
#endif
#ifdef COMPILE_21781
                          case 32: {
                            // AddAndReplaceKind
                            tmp = *p;

                            break;
                          }
#endif
#ifdef COMPILE_21782
                          case 33: {
                            // AddAndReplaceKind
                            zend_error(
                                (1 << 3L),
                                "A non well formed numeric value encountered");

                            break;
                          }
#endif
#ifdef COMPILE_21783
                          case 34: {
                            // AddAndReplaceKind
                            zend_error((1 << 3L), "Undefined variable: %s",
                                       (key)->arKey);

                            break;
                          }
#endif
#ifdef COMPILE_21784
                          case 35: {
                            // AddAndReplaceKind
                            zend_error((1 << 3L), "Undefined variable: %s",
                                       key->arKey);

                            break;
                          }
#endif
#ifdef COMPILE_21785
                          case 36: {
                            // AddAndReplaceKind
                            zend_hash_destroy((tmp)->value.ht);

                            break;
                          }
#endif
#ifdef COMPILE_21786
                          case 37: {
                            // AddAndReplaceKind
                            zend_rebuild_symbol_table();

                            break;
                          }
#endif
#ifdef COMPILE_21787
                          case 38: {
                            // AddAndReplaceKind
                            zval_addref_p((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21788
                          case 39: {
                            // AddAndReplaceKind
                            zval_addref_p(*((p)));

                            break;
                          }
#endif
#ifdef COMPILE_21789
                          case 40: {
                            // AddAndReplaceKind
                            zval_addref_p(tmp);

                            break;
                          }
#endif
#ifdef COMPILE_21790
                          case 41: {
                            // AddAndReplaceKind
                            zval_set_isref_p((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21791
                          case 42: {
                            // AddAndReplaceKind
                            zval_set_isref_p(*(((p))));

                            break;
                          }
#endif
#ifdef COMPILE_21792
                          case 43: {
                            // AddAndReplaceKind
                            zval_set_isref_p(*((p)));

                            break;
                          }
#endif
#ifdef COMPILE_21793
                          case 44: {
                            // AddAndReplaceKind
                            zval_set_isref_p(tmp);

                            break;
                          }
#endif
#ifdef COMPILE_21794
                          case 45: {
                            // AddAndReplaceKind
                            zval_set_refcount_p((tmp), 0);

                            break;
                          }
#endif
#ifdef COMPILE_21795
                          case 46: {
                            // AddAndReplaceKind
                            zval_set_refcount_p(tmp, 0);

                            break;
                          }
#endif
#ifdef COMPILE_21796
                          case 47: {
                            // AddAndReplaceKind
                            zval_unset_isref_p((tmp));

                            break;
                          }
#endif
#ifdef COMPILE_21797
                          case 48: {
                            // AddAndReplaceKind
                            zval_unset_isref_p(tmp);

                            break;
                          }
#endif
#ifdef COMPILE_21798
                          case 49: {
                            // AddInitKind
                            memset(&((zval_gc_info *)(tmp))->u, 0,
                                   sizeof(*(&((zval_gc_info *)(tmp))->u)));

                            break;
                          }
#endif
#ifdef COMPILE_21799
                          case 50: {
                            // AddInitKind
                            memset(&(**p), 0, sizeof(*(&(**p))));

                            break;
                          }
#endif
#ifdef COMPILE_21800
                          case 51: {
                            // AddInitKind
                            memset(&(*tmp), 0, sizeof(*(&(*tmp))));

                            break;
                          }
#endif
#ifdef COMPILE_21801
                          case 52: {
                            // AddInitKind
                            memset(((zval_gc_info *)(tmp)), 0,
                                   sizeof(*(((zval_gc_info *)(tmp)))));

                            break;
                          }
#endif
#ifdef COMPILE_21802
                          case 53: {
                            // AddInitKind
                            memset((*p), 0, sizeof(*((*p))));

                            break;
                          }
#endif
#ifdef COMPILE_21803
                          case 54: {
                            // AddInitKind
                            memset((tmp), 0, sizeof(*((tmp))));

                            break;
                          }
#endif
#ifdef COMPILE_21804
                          case 55: {
                            // IfExitKind
                            if (__is_neg(
                                    "240-55", 12, &(p), sizeof(p), &(tmp),
                                    sizeof(tmp), &(is_ref), sizeof(is_ref),
                                    &(key), sizeof(key), &(target),
                                    sizeof(target), &(args), sizeof(args),
                                    &((tmp)->refcount__gc),
                                    sizeof((tmp)->refcount__gc), &((tmp)->type),
                                    sizeof((tmp)->type), &((tmp)->is_ref__gc),
                                    sizeof((tmp)->is_ref__gc),
                                    &(zval_used_for_init.refcount__gc),
                                    sizeof(zval_used_for_init.refcount__gc),
                                    &(zval_used_for_init.type),
                                    sizeof(zval_used_for_init.type),
                                    &(zval_used_for_init.is_ref__gc),
                                    sizeof(zval_used_for_init.is_ref__gc)))
                              return 0;

                            break;
                          }
#endif
#ifdef COMPILE_21805
                          case 56: {
                            // IfExitKind
                            if (__is_neg(
                                    "240-56", 12, &(p), sizeof(p), &(tmp),
                                    sizeof(tmp), &(is_ref), sizeof(is_ref),
                                    &(key), sizeof(key), &(target),
                                    sizeof(target), &(args), sizeof(args),
                                    &((tmp)->refcount__gc),
                                    sizeof((tmp)->refcount__gc), &((tmp)->type),
                                    sizeof((tmp)->type), &((tmp)->is_ref__gc),
                                    sizeof((tmp)->is_ref__gc),
                                    &(zval_used_for_init.refcount__gc),
                                    sizeof(zval_used_for_init.refcount__gc),
                                    &(zval_used_for_init.type),
                                    sizeof(zval_used_for_init.type),
                                    &(zval_used_for_init.is_ref__gc),
                                    sizeof(zval_used_for_init.is_ref__gc)))
                              return 1;

                            break;
                          }
#endif
#ifdef COMPILE_21806
                          case 57: {
                            // IfExitKind
                            if (__is_neg(
                                    "240-57", 12, &(p), sizeof(p), &(tmp),
                                    sizeof(tmp), &(is_ref), sizeof(is_ref),
                                    &(key), sizeof(key), &(target),
                                    sizeof(target), &(args), sizeof(args),
                                    &((tmp)->refcount__gc),
                                    sizeof((tmp)->refcount__gc), &((tmp)->type),
                                    sizeof((tmp)->type), &((tmp)->is_ref__gc),
                                    sizeof((tmp)->is_ref__gc),
                                    &(zval_used_for_init.refcount__gc),
                                    sizeof(zval_used_for_init.refcount__gc),
                                    &(zval_used_for_init.type),
                                    sizeof(zval_used_for_init.type),
                                    &(zval_used_for_init.is_ref__gc),
                                    sizeof(zval_used_for_init.is_ref__gc)))
                              return 32;

                            break;
                          }
#endif
#ifdef COMPILE_21807
                          case 58: {
                            // IfExitKind
                            if (__is_neg(
                                    "240-58", 12, &(p), sizeof(p), &(tmp),
                                    sizeof(tmp), &(is_ref), sizeof(is_ref),
                                    &(key), sizeof(key), &(target),
                                    sizeof(target), &(args), sizeof(args),
                                    &((tmp)->refcount__gc),
                                    sizeof((tmp)->refcount__gc), &((tmp)->type),
                                    sizeof((tmp)->type), &((tmp)->is_ref__gc),
                                    sizeof((tmp)->is_ref__gc),
                                    &(zval_used_for_init.refcount__gc),
                                    sizeof(zval_used_for_init.refcount__gc),
                                    &(zval_used_for_init.type),
                                    sizeof(zval_used_for_init.type),
                                    &(zval_used_for_init.is_ref__gc),
                                    sizeof(zval_used_for_init.is_ref__gc)))
                              return 3;

                            break;
                          }
#endif
#ifdef COMPILE_21808
                          case 59: {
                            // IfExitKind
                            if (__is_neg(
                                    "240-59", 12, &(p), sizeof(p), &(tmp),
                                    sizeof(tmp), &(is_ref), sizeof(is_ref),
                                    &(key), sizeof(key), &(target),
                                    sizeof(target), &(args), sizeof(args),
                                    &((tmp)->refcount__gc),
                                    sizeof((tmp)->refcount__gc), &((tmp)->type),
                                    sizeof((tmp)->type), &((tmp)->is_ref__gc),
                                    sizeof((tmp)->is_ref__gc),
                                    &(zval_used_for_init.refcount__gc),
                                    sizeof(zval_used_for_init.refcount__gc),
                                    &(zval_used_for_init.type),
                                    sizeof(zval_used_for_init.type),
                                    &(zval_used_for_init.is_ref__gc),
                                    sizeof(zval_used_for_init.is_ref__gc)))
                              return 64;

                            break;
                          }
#endif
                          }
                          {
                            long long __temp241 = (zval_isref_p(*(p)));
                            switch (__choose("__SWITCH241")) {
                            case 0: {

                              break;
                            }
#ifdef COMPILE_21809
                            case 1: {
                              __temp241 =
                                  ((zval_isref_p(*(p))) ||
                                   __is_neg(
                                       "241-1", 12, &(p), sizeof(p), &(tmp),
                                       sizeof(tmp), &(is_ref), sizeof(is_ref),
                                       &(key), sizeof(key), &(target),
                                       sizeof(target), &(args), sizeof(args),
                                       &((tmp)->refcount__gc),
                                       sizeof((tmp)->refcount__gc),
                                       &((tmp)->type), sizeof((tmp)->type),
                                       &((tmp)->is_ref__gc),
                                       sizeof((tmp)->is_ref__gc),
                                       &(zval_used_for_init.refcount__gc),
                                       sizeof(zval_used_for_init.refcount__gc),
                                       &(zval_used_for_init.type),
                                       sizeof(zval_used_for_init.type),
                                       &(zval_used_for_init.is_ref__gc),
                                       sizeof(zval_used_for_init.is_ref__gc)));

                              break;
                            }
#endif
#ifdef COMPILE_21810
                            case 2: {
                              __temp241 =
                                  ((zval_isref_p(*(p))) &&
                                   !__is_neg(
                                       "241-2", 12, &(p), sizeof(p), &(tmp),
                                       sizeof(tmp), &(is_ref), sizeof(is_ref),
                                       &(key), sizeof(key), &(target),
                                       sizeof(target), &(args), sizeof(args),
                                       &((tmp)->refcount__gc),
                                       sizeof((tmp)->refcount__gc),
                                       &((tmp)->type), sizeof((tmp)->type),
                                       &((tmp)->is_ref__gc),
                                       sizeof((tmp)->is_ref__gc),
                                       &(zval_used_for_init.refcount__gc),
                                       sizeof(zval_used_for_init.refcount__gc),
                                       &(zval_used_for_init.type),
                                       sizeof(zval_used_for_init.type),
                                       &(zval_used_for_init.is_ref__gc),
                                       sizeof(zval_used_for_init.is_ref__gc)));

                              break;
                            }
#endif
                            }
                            switch (__choose("__SWITCH250")) {
                            case 0: {
                              if (__temp241) {
                                {
                                  switch (__choose("__SWITCH242")) {
                                  case 0:
                                    break;
#ifdef COMPILE_21811
                                  case 1: {
                                    // AddAndReplaceKind
                                    (executor_globals.argument_stack) =
                                        zend_vm_stack_new_page(
                                            ((16 * 1024) - 16));

                                    break;
                                  }
#endif
#ifdef COMPILE_21812
                                  case 2: {
                                    // AddAndReplaceKind
                                    (tmp) = (executor_globals
                                                 .uninitialized_zval_ptr);

                                    break;
                                  }
#endif
#ifdef COMPILE_21813
                                  case 3: {
                                    // AddAndReplaceKind
                                    (tmp) = *p;

                                    break;
                                  }
#endif
#ifdef COMPILE_21814
                                  case 4: {
                                    // AddAndReplaceKind
                                    *(tmp) = zval_used_for_init;

                                    break;
                                  }
#endif
#ifdef COMPILE_21815
                                  case 5: {
                                    // AddAndReplaceKind
                                    *tmp = zval_used_for_init;

                                    break;
                                  }
#endif
#ifdef COMPILE_21816
                                  case 6: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21817
                                  case 7: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)->value.ht));

                                    break;
                                  }
#endif
#ifdef COMPILE_21818
                                  case 8: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)->value.str.val));

                                    break;
                                  }
#endif
#ifdef COMPILE_21819
                                  case 9: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        (key)->arKey, key->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21820
                                  case 10: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, (key)->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21821
                                  case 11: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, (key)->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21822
                                  case 12: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        &(tmp), sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21823
                                  case 13: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21824
                                  case 14: {
                                    // AddAndReplaceKind
                                    _zend_list_addref((tmp)->value.lval);

                                    break;
                                  }
#endif
#ifdef COMPILE_21825
                                  case 15: {
                                    // AddAndReplaceKind
                                    _zend_list_delete((tmp)->value.lval);

                                    break;
                                  }
#endif
#ifdef COMPILE_21826
                                  case 16: {
                                    // AddAndReplaceKind
                                    _zval_copy_ctor_func((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21827
                                  case 17: {
                                    // AddAndReplaceKind
                                    _zval_dtor(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21828
                                  case 18: {
                                    // AddAndReplaceKind
                                    _zval_dtor_func((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21829
                                  case 19: {
                                    // AddAndReplaceKind
                                    _zval_ptr_dtor((&(tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21830
                                  case 20: {
                                    // AddAndReplaceKind
                                    convert_to_boolean((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21831
                                  case 21: {
                                    // AddAndReplaceKind
                                    exit(1);

                                    break;
                                  }
#endif
#ifdef COMPILE_21832
                                  case 22: {
                                    // AddAndReplaceKind
                                    gc_remove_zval_from_buffer((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21833
                                  case 23: {
                                    // AddAndReplaceKind
                                    gc_zval_check_possible_root(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21834
                                  case 24: {
                                    // AddAndReplaceKind
                                    gc_zval_possible_root((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21835
                                  case 25: {
                                    // AddAndReplaceKind
                                    i_zval_ptr_dtor((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21836
                                  case 26: {
                                    // AddAndReplaceKind
                                    if (!(executor_globals
                                              .active_symbol_table)) {
                                      zend_rebuild_symbol_table();
                                    }

                                    break;
                                  }
#endif
#ifdef COMPILE_21837
                                  case 27: {
                                    // AddAndReplaceKind
                                    if (_zend_hash_quick_add_or_update(
                                            target, key->arKey, key->nKeyLength,
                                            key->h, &tmp, sizeof(zval *),
                                            ((void *)0), (1 << 1)) == 0) {
                                      zval_addref_p(tmp);
                                    }

                                    break;
                                  }
#endif
#ifdef COMPILE_21838
                                  case 28: {
                                    // AddAndReplaceKind
                                    is_ref = (**(p)).type & 64;

                                    break;
                                  }
#endif
#ifdef COMPILE_21839
                                  case 29: {
                                    // AddAndReplaceKind
                                    is_ref = (**p).type & 64;

                                    break;
                                  }
#endif
#ifdef COMPILE_21840
                                  case 30: {
                                    // AddAndReplaceKind
                                    tmp = (executor_globals
                                               .uninitialized_zval_ptr);

                                    break;
                                  }
#endif
#ifdef COMPILE_21841
                                  case 31: {
                                    // AddAndReplaceKind
                                    tmp = *(p);

                                    break;
                                  }
#endif
#ifdef COMPILE_21842
                                  case 32: {
                                    // AddAndReplaceKind
                                    tmp = *p;

                                    break;
                                  }
#endif
#ifdef COMPILE_21843
                                  case 33: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "A non well formed numeric "
                                               "value encountered");

                                    break;
                                  }
#endif
#ifdef COMPILE_21844
                                  case 34: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "Undefined variable: %s",
                                               (key)->arKey);

                                    break;
                                  }
#endif
#ifdef COMPILE_21845
                                  case 35: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "Undefined variable: %s",
                                               key->arKey);

                                    break;
                                  }
#endif
#ifdef COMPILE_21846
                                  case 36: {
                                    // AddAndReplaceKind
                                    zend_hash_destroy((tmp)->value.ht);

                                    break;
                                  }
#endif
#ifdef COMPILE_21847
                                  case 37: {
                                    // AddAndReplaceKind
                                    zend_rebuild_symbol_table();

                                    break;
                                  }
#endif
#ifdef COMPILE_21848
                                  case 38: {
                                    // AddAndReplaceKind
                                    zval_addref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21849
                                  case 39: {
                                    // AddAndReplaceKind
                                    zval_addref_p(*((p)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21850
                                  case 40: {
                                    // AddAndReplaceKind
                                    zval_addref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21851
                                  case 41: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21852
                                  case 42: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(*(((p))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21853
                                  case 43: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(*((p)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21854
                                  case 44: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21855
                                  case 45: {
                                    // AddAndReplaceKind
                                    zval_set_refcount_p((tmp), 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21856
                                  case 46: {
                                    // AddAndReplaceKind
                                    zval_set_refcount_p(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21857
                                  case 47: {
                                    // AddAndReplaceKind
                                    zval_unset_isref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21858
                                  case 48: {
                                    // AddAndReplaceKind
                                    zval_unset_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21859
                                  case 49: {
                                    // AddInitKind
                                    memset(
                                        &((zval_gc_info *)(tmp))->u, 0,
                                        sizeof(*(&((zval_gc_info *)(tmp))->u)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21860
                                  case 50: {
                                    // AddInitKind
                                    memset(((zval_gc_info *)(tmp)), 0,
                                           sizeof(*(((zval_gc_info *)(tmp)))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21861
                                  case 51: {
                                    // AddInitKind
                                    memset((tmp), 0, sizeof(*((tmp))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21862
                                  case 52: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "242-52", 21, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 0;

                                    break;
                                  }
#endif
#ifdef COMPILE_21863
                                  case 53: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "242-53", 21, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 1;

                                    break;
                                  }
#endif
#ifdef COMPILE_21864
                                  case 54: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "242-54", 21, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 32;

                                    break;
                                  }
#endif
#ifdef COMPILE_21865
                                  case 55: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "242-55", 21, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 3;

                                    break;
                                  }
#endif
#ifdef COMPILE_21866
                                  case 56: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "242-56", 21, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 64;

                                    break;
                                  }
#endif
                                  }
                                  switch (__choose("__SWITCH243")) {
                                  case 0: {
                                    ALLOC_INIT_ZVAL(tmp);

                                    break;
                                  }
#ifdef COMPILE_21867
                                  case 1: {
                                    // GuardKind
                                    if (!__is_neg(
                                            "243-1", 21, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      do {
                                        (tmp) = (zval *)_emalloc(
                                            (sizeof(zval_gc_info)));
                                        ((zval_gc_info *)(tmp))->u.buffered =
                                            ((void *)0);
                                      } while (0);

                                    break;
                                  }
#endif
                                  }
                                }
                                {
                                  switch (__choose("__SWITCH244")) {
                                  case 0:
                                    break;
#ifdef COMPILE_21868
                                  case 1: {
                                    // AddAndReplaceKind
                                    (executor_globals.argument_stack) =
                                        zend_vm_stack_new_page(
                                            ((16 * 1024) - 16));

                                    break;
                                  }
#endif
#ifdef COMPILE_21869
                                  case 2: {
                                    // AddAndReplaceKind
                                    (tmp) = (executor_globals
                                                 .uninitialized_zval_ptr);

                                    break;
                                  }
#endif
#ifdef COMPILE_21870
                                  case 3: {
                                    // AddAndReplaceKind
                                    (tmp) = *p;

                                    break;
                                  }
#endif
#ifdef COMPILE_21871
                                  case 4: {
                                    // AddAndReplaceKind
                                    *(tmp) = zval_used_for_init;

                                    break;
                                  }
#endif
#ifdef COMPILE_21872
                                  case 5: {
                                    // AddAndReplaceKind
                                    *tmp = zval_used_for_init;

                                    break;
                                  }
#endif
#ifdef COMPILE_21873
                                  case 6: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21874
                                  case 7: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)->value.ht));

                                    break;
                                  }
#endif
#ifdef COMPILE_21875
                                  case 8: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)->value.str.val));

                                    break;
                                  }
#endif
#ifdef COMPILE_21876
                                  case 9: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        (key)->arKey, key->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21877
                                  case 10: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, (key)->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21878
                                  case 11: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, (key)->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21879
                                  case 12: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        &(tmp), sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21880
                                  case 13: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21881
                                  case 14: {
                                    // AddAndReplaceKind
                                    _zend_list_addref((tmp)->value.lval);

                                    break;
                                  }
#endif
#ifdef COMPILE_21882
                                  case 15: {
                                    // AddAndReplaceKind
                                    _zend_list_delete((tmp)->value.lval);

                                    break;
                                  }
#endif
#ifdef COMPILE_21883
                                  case 16: {
                                    // AddAndReplaceKind
                                    _zval_copy_ctor_func((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21884
                                  case 17: {
                                    // AddAndReplaceKind
                                    _zval_dtor(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21885
                                  case 18: {
                                    // AddAndReplaceKind
                                    _zval_dtor_func((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21886
                                  case 19: {
                                    // AddAndReplaceKind
                                    _zval_ptr_dtor((&(tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21887
                                  case 20: {
                                    // AddAndReplaceKind
                                    convert_to_boolean((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21888
                                  case 21: {
                                    // AddAndReplaceKind
                                    exit(1);

                                    break;
                                  }
#endif
#ifdef COMPILE_21889
                                  case 22: {
                                    // AddAndReplaceKind
                                    gc_remove_zval_from_buffer((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21890
                                  case 23: {
                                    // AddAndReplaceKind
                                    gc_zval_check_possible_root(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21891
                                  case 24: {
                                    // AddAndReplaceKind
                                    gc_zval_possible_root((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21892
                                  case 25: {
                                    // AddAndReplaceKind
                                    i_zval_ptr_dtor((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21893
                                  case 26: {
                                    // AddAndReplaceKind
                                    if (!(executor_globals
                                              .active_symbol_table)) {
                                      zend_rebuild_symbol_table();
                                    }

                                    break;
                                  }
#endif
#ifdef COMPILE_21894
                                  case 27: {
                                    // AddAndReplaceKind
                                    if (_zend_hash_quick_add_or_update(
                                            target, key->arKey, key->nKeyLength,
                                            key->h, &tmp, sizeof(zval *),
                                            ((void *)0), (1 << 1)) == 0) {
                                      zval_addref_p(tmp);
                                    }

                                    break;
                                  }
#endif
#ifdef COMPILE_21895
                                  case 28: {
                                    // AddAndReplaceKind
                                    is_ref = (**(p)).type & 64;

                                    break;
                                  }
#endif
#ifdef COMPILE_21896
                                  case 29: {
                                    // AddAndReplaceKind
                                    is_ref = (**p).type & 64;

                                    break;
                                  }
#endif
#ifdef COMPILE_21897
                                  case 30: {
                                    // AddAndReplaceKind
                                    tmp = (executor_globals
                                               .uninitialized_zval_ptr);

                                    break;
                                  }
#endif
#ifdef COMPILE_21898
                                  case 31: {
                                    // AddAndReplaceKind
                                    tmp = *(p);

                                    break;
                                  }
#endif
#ifdef COMPILE_21899
                                  case 32: {
                                    // AddAndReplaceKind
                                    tmp = *p;

                                    break;
                                  }
#endif
#ifdef COMPILE_21900
                                  case 33: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "A non well formed numeric "
                                               "value encountered");

                                    break;
                                  }
#endif
#ifdef COMPILE_21901
                                  case 34: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "Undefined variable: %s",
                                               (key)->arKey);

                                    break;
                                  }
#endif
#ifdef COMPILE_21902
                                  case 35: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "Undefined variable: %s",
                                               key->arKey);

                                    break;
                                  }
#endif
#ifdef COMPILE_21903
                                  case 36: {
                                    // AddAndReplaceKind
                                    zend_hash_destroy((tmp)->value.ht);

                                    break;
                                  }
#endif
#ifdef COMPILE_21904
                                  case 37: {
                                    // AddAndReplaceKind
                                    zend_rebuild_symbol_table();

                                    break;
                                  }
#endif
#ifdef COMPILE_21905
                                  case 38: {
                                    // AddAndReplaceKind
                                    zval_addref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21906
                                  case 39: {
                                    // AddAndReplaceKind
                                    zval_addref_p(*((p)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21907
                                  case 40: {
                                    // AddAndReplaceKind
                                    zval_addref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21908
                                  case 41: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21909
                                  case 42: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(*(((p))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21910
                                  case 43: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(*((p)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21911
                                  case 44: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21912
                                  case 45: {
                                    // AddAndReplaceKind
                                    zval_set_refcount_p((tmp), 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21913
                                  case 46: {
                                    // AddAndReplaceKind
                                    zval_set_refcount_p(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21914
                                  case 47: {
                                    // AddAndReplaceKind
                                    zval_unset_isref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21915
                                  case 48: {
                                    // AddAndReplaceKind
                                    zval_unset_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21916
                                  case 49: {
                                    // AddInitKind
                                    memset(&(**p), 0, sizeof(*(&(**p))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21917
                                  case 50: {
                                    // AddInitKind
                                    memset(&(*tmp), 0, sizeof(*(&(*tmp))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21918
                                  case 51: {
                                    // AddInitKind
                                    memset((*p), 0, sizeof(*((*p))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21919
                                  case 52: {
                                    // AddInitKind
                                    memset((tmp), 0, sizeof(*((tmp))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21920
                                  case 53: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "244-53", 21, &(p), sizeof(p),
                                            &(tmp), sizeof(tmp), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 0;

                                    break;
                                  }
#endif
#ifdef COMPILE_21921
                                  case 54: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "244-54", 21, &(p), sizeof(p),
                                            &(tmp), sizeof(tmp), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 1;

                                    break;
                                  }
#endif
#ifdef COMPILE_21922
                                  case 55: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "244-55", 21, &(p), sizeof(p),
                                            &(tmp), sizeof(tmp), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 32;

                                    break;
                                  }
#endif
#ifdef COMPILE_21923
                                  case 56: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "244-56", 21, &(p), sizeof(p),
                                            &(tmp), sizeof(tmp), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 3;

                                    break;
                                  }
#endif
#ifdef COMPILE_21924
                                  case 57: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "244-57", 21, &(p), sizeof(p),
                                            &(tmp), sizeof(tmp), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
                                      return 64;

                                    break;
                                  }
#endif
                                  }
                                  switch (__choose("__SWITCH245")) {
                                  case 0: {
                                    ZVAL_COPY_VALUE(tmp, *p);

                                    break;
                                  }
#ifdef COMPILE_21925
                                  case 1: {
                                    // GuardKind
                                    if (!__is_neg(
                                            "245-1", 21, &(p), sizeof(p),
                                            &(tmp), sizeof(tmp), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc),
                                            &(zval_used_for_init.refcount__gc),
                                            sizeof(zval_used_for_init
                                                       .refcount__gc),
                                            &(zval_used_for_init.type),
                                            sizeof(zval_used_for_init.type),
                                            &(zval_used_for_init.is_ref__gc),
                                            sizeof(
                                                zval_used_for_init.is_ref__gc)))
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
                                  switch (__choose("__SWITCH246")) {
                                  case 0:
                                    break;
#ifdef COMPILE_21926
                                  case 1: {
                                    // AddAndReplaceKind
                                    (executor_globals.argument_stack) =
                                        zend_vm_stack_new_page(
                                            ((16 * 1024) - 16));

                                    break;
                                  }
#endif
#ifdef COMPILE_21927
                                  case 2: {
                                    // AddAndReplaceKind
                                    (tmp) = (executor_globals
                                                 .uninitialized_zval_ptr);

                                    break;
                                  }
#endif
#ifdef COMPILE_21928
                                  case 3: {
                                    // AddAndReplaceKind
                                    (tmp) = *p;

                                    break;
                                  }
#endif
#ifdef COMPILE_21929
                                  case 4: {
                                    // AddAndReplaceKind
                                    *(tmp) = zval_used_for_init;

                                    break;
                                  }
#endif
#ifdef COMPILE_21930
                                  case 5: {
                                    // AddAndReplaceKind
                                    *tmp = zval_used_for_init;

                                    break;
                                  }
#endif
#ifdef COMPILE_21931
                                  case 6: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21932
                                  case 7: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)->value.ht));

                                    break;
                                  }
#endif
#ifdef COMPILE_21933
                                  case 8: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)->value.str.val));

                                    break;
                                  }
#endif
#ifdef COMPILE_21934
                                  case 9: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        (key)->arKey, key->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21935
                                  case 10: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, (key)->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21936
                                  case 11: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, (key)->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21937
                                  case 12: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        &(tmp), sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21938
                                  case 13: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_21939
                                  case 14: {
                                    // AddAndReplaceKind
                                    _zend_list_addref((tmp)->value.lval);

                                    break;
                                  }
#endif
#ifdef COMPILE_21940
                                  case 15: {
                                    // AddAndReplaceKind
                                    _zend_list_delete((tmp)->value.lval);

                                    break;
                                  }
#endif
#ifdef COMPILE_21941
                                  case 16: {
                                    // AddAndReplaceKind
                                    _zval_copy_ctor_func((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21942
                                  case 17: {
                                    // AddAndReplaceKind
                                    _zval_dtor(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21943
                                  case 18: {
                                    // AddAndReplaceKind
                                    _zval_dtor_func((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21944
                                  case 19: {
                                    // AddAndReplaceKind
                                    _zval_ptr_dtor((&(tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21945
                                  case 20: {
                                    // AddAndReplaceKind
                                    convert_to_boolean((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21946
                                  case 21: {
                                    // AddAndReplaceKind
                                    exit(1);

                                    break;
                                  }
#endif
#ifdef COMPILE_21947
                                  case 22: {
                                    // AddAndReplaceKind
                                    gc_remove_zval_from_buffer((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21948
                                  case 23: {
                                    // AddAndReplaceKind
                                    gc_zval_check_possible_root(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21949
                                  case 24: {
                                    // AddAndReplaceKind
                                    gc_zval_possible_root((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21950
                                  case 25: {
                                    // AddAndReplaceKind
                                    i_zval_ptr_dtor((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21951
                                  case 26: {
                                    // AddAndReplaceKind
                                    if (!(executor_globals
                                              .active_symbol_table)) {
                                      zend_rebuild_symbol_table();
                                    }

                                    break;
                                  }
#endif
#ifdef COMPILE_21952
                                  case 27: {
                                    // AddAndReplaceKind
                                    if (_zend_hash_quick_add_or_update(
                                            target, key->arKey, key->nKeyLength,
                                            key->h, &tmp, sizeof(zval *),
                                            ((void *)0), (1 << 1)) == 0) {
                                      zval_addref_p(tmp);
                                    }

                                    break;
                                  }
#endif
#ifdef COMPILE_21953
                                  case 28: {
                                    // AddAndReplaceKind
                                    is_ref = (**(p)).type & 64;

                                    break;
                                  }
#endif
#ifdef COMPILE_21954
                                  case 29: {
                                    // AddAndReplaceKind
                                    is_ref = (**p).type & 64;

                                    break;
                                  }
#endif
#ifdef COMPILE_21955
                                  case 30: {
                                    // AddAndReplaceKind
                                    tmp = (executor_globals
                                               .uninitialized_zval_ptr);

                                    break;
                                  }
#endif
#ifdef COMPILE_21956
                                  case 31: {
                                    // AddAndReplaceKind
                                    tmp = *(p);

                                    break;
                                  }
#endif
#ifdef COMPILE_21957
                                  case 32: {
                                    // AddAndReplaceKind
                                    tmp = *p;

                                    break;
                                  }
#endif
#ifdef COMPILE_21958
                                  case 33: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "A non well formed numeric "
                                               "value encountered");

                                    break;
                                  }
#endif
#ifdef COMPILE_21959
                                  case 34: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "Undefined variable: %s",
                                               (key)->arKey);

                                    break;
                                  }
#endif
#ifdef COMPILE_21960
                                  case 35: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "Undefined variable: %s",
                                               key->arKey);

                                    break;
                                  }
#endif
#ifdef COMPILE_21961
                                  case 36: {
                                    // AddAndReplaceKind
                                    zend_hash_destroy((tmp)->value.ht);

                                    break;
                                  }
#endif
#ifdef COMPILE_21962
                                  case 37: {
                                    // AddAndReplaceKind
                                    zend_rebuild_symbol_table();

                                    break;
                                  }
#endif
#ifdef COMPILE_21963
                                  case 38: {
                                    // AddAndReplaceKind
                                    zval_addref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21964
                                  case 39: {
                                    // AddAndReplaceKind
                                    zval_addref_p(*((p)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21965
                                  case 40: {
                                    // AddAndReplaceKind
                                    zval_addref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21966
                                  case 41: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21967
                                  case 42: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(*(((p))));

                                    break;
                                  }
#endif
#ifdef COMPILE_21968
                                  case 43: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(*((p)));

                                    break;
                                  }
#endif
#ifdef COMPILE_21969
                                  case 44: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21970
                                  case 45: {
                                    // AddAndReplaceKind
                                    zval_set_refcount_p((tmp), 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21971
                                  case 46: {
                                    // AddAndReplaceKind
                                    zval_set_refcount_p(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21972
                                  case 47: {
                                    // AddAndReplaceKind
                                    zval_unset_isref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_21973
                                  case 48: {
                                    // AddAndReplaceKind
                                    zval_unset_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_21974
                                  case 49: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "246-49", 18, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc)))
                                      return 0;

                                    break;
                                  }
#endif
#ifdef COMPILE_21975
                                  case 50: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "246-50", 18, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc)))
                                      return 1;

                                    break;
                                  }
#endif
#ifdef COMPILE_21976
                                  case 51: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "246-51", 18, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc)))
                                      return 32;

                                    break;
                                  }
#endif
#ifdef COMPILE_21977
                                  case 52: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "246-52", 18, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc)))
                                      return 3;

                                    break;
                                  }
#endif
#ifdef COMPILE_21978
                                  case 53: {
                                    // IfExitKind
                                    if (__is_neg(
                                            "246-53", 18, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc)))
                                      return 64;

                                    break;
                                  }
#endif
                                  }
                                  switch (__choose("__SWITCH247")) {
                                  case 0: {
                                    Z_SET_REFCOUNT_P(tmp, 0);

                                    break;
                                  }
#ifdef COMPILE_21979
                                  case 1: {
                                    // AddAndReplaceKind
                                    __overflow(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21980
                                  case 2: {
                                    // AddAndReplaceKind
                                    __sigsetjmp(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21981
                                  case 3: {
                                    // AddAndReplaceKind
                                    __stpcpy(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21982
                                  case 4: {
                                    // AddAndReplaceKind
                                    __zend_realloc(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21983
                                  case 5: {
                                    // AddAndReplaceKind
                                    _array_init(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21984
                                  case 6: {
                                    // AddAndReplaceKind
                                    _estrndup(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21985
                                  case 7: {
                                    // AddAndReplaceKind
                                    _longjmp(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21986
                                  case 8: {
                                    // AddAndReplaceKind
                                    _object_init_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21987
                                  case 9: {
                                    // AddAndReplaceKind
                                    _zend_bailout(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21988
                                  case 10: {
                                    // AddAndReplaceKind
                                    _zend_mm_alloc(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21989
                                  case 11: {
                                    // AddAndReplaceKind
                                    _zend_mm_block_size(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21990
                                  case 12: {
                                    // AddAndReplaceKind
                                    _zend_mm_free(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21991
                                  case 13: {
                                    // AddAndReplaceKind
                                    add_index_null(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21992
                                  case 14: {
                                    // AddAndReplaceKind
                                    add_next_index_bool(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21993
                                  case 15: {
                                    // AddAndReplaceKind
                                    add_next_index_long(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21994
                                  case 16: {
                                    // AddAndReplaceKind
                                    add_next_index_resource(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21995
                                  case 17: {
                                    // AddAndReplaceKind
                                    add_next_index_zval(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21996
                                  case 18: {
                                    // AddAndReplaceKind
                                    add_trait_alias(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21997
                                  case 19: {
                                    // AddAndReplaceKind
                                    bitwise_not_function(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21998
                                  case 20: {
                                    // AddAndReplaceKind
                                    boolean_not_function(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_21999
                                  case 21: {
                                    // AddAndReplaceKind
                                    bzero(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22000
                                  case 22: {
                                    // AddAndReplaceKind
                                    compile_file(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22001
                                  case 23: {
                                    // AddAndReplaceKind
                                    compile_string(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22002
                                  case 24: {
                                    // AddAndReplaceKind
                                    convert_to_long_base(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22003
                                  case 25: {
                                    // AddAndReplaceKind
                                    dlopen(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22004
                                  case 26: {
                                    // AddAndReplaceKind
                                    dlsym(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22005
                                  case 27: {
                                    // AddAndReplaceKind
                                    drand48_r(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22006
                                  case 28: {
                                    // AddAndReplaceKind
                                    execute_internal(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22007
                                  case 29: {
                                    // AddAndReplaceKind
                                    explicit_bzero(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22008
                                  case 30: {
                                    // AddAndReplaceKind
                                    fgetpos(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22009
                                  case 31: {
                                    // AddAndReplaceKind
                                    fopen(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22010
                                  case 32: {
                                    // AddAndReplaceKind
                                    fprintf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22011
                                  case 33: {
                                    // AddAndReplaceKind
                                    fputs(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22012
                                  case 34: {
                                    // AddAndReplaceKind
                                    fscanf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22013
                                  case 35: {
                                    // AddAndReplaceKind
                                    fsetpos(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22014
                                  case 36: {
                                    // AddAndReplaceKind
                                    getloadavg(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22015
                                  case 37: {
                                    // AddAndReplaceKind
                                    index(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22016
                                  case 38: {
                                    // AddAndReplaceKind
                                    init_trait_alias_list(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22017
                                  case 39: {
                                    // AddAndReplaceKind
                                    instanceof_function(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22018
                                  case 40: {
                                    // AddAndReplaceKind
                                    lcong48_r(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22019
                                  case 41: {
                                    // AddAndReplaceKind
                                    longjmp(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22020
                                  case 42: {
                                    // AddAndReplaceKind
                                    lrand48_r(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22021
                                  case 43: {
                                    // AddAndReplaceKind
                                    mblen(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22022
                                  case 44: {
                                    // AddAndReplaceKind
                                    mkstemps(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22023
                                  case 45: {
                                    // AddAndReplaceKind
                                    mrand48_r(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22024
                                  case 46: {
                                    // AddAndReplaceKind
                                    object_properties_init(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22025
                                  case 47: {
                                    // AddAndReplaceKind
                                    on_exit(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22026
                                  case 48: {
                                    // AddAndReplaceKind
                                    open_memstream(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22027
                                  case 49: {
                                    // AddAndReplaceKind
                                    popen(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22028
                                  case 50: {
                                    // AddAndReplaceKind
                                    print_op_array(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22029
                                  case 51: {
                                    // AddAndReplaceKind
                                    printf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22030
                                  case 52: {
                                    // AddAndReplaceKind
                                    random_r(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22031
                                  case 53: {
                                    // AddAndReplaceKind
                                    realloc(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22032
                                  case 54: {
                                    // AddAndReplaceKind
                                    realpath(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22033
                                  case 55: {
                                    // AddAndReplaceKind
                                    rename(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22034
                                  case 56: {
                                    // AddAndReplaceKind
                                    rindex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22035
                                  case 57: {
                                    // AddAndReplaceKind
                                    scanf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22036
                                  case 58: {
                                    // AddAndReplaceKind
                                    seed48_r(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22037
                                  case 59: {
                                    // AddAndReplaceKind
                                    setbuf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22038
                                  case 60: {
                                    // AddAndReplaceKind
                                    setstate_r(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22039
                                  case 61: {
                                    // AddAndReplaceKind
                                    siglongjmp(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22040
                                  case 62: {
                                    // AddAndReplaceKind
                                    sprintf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22041
                                  case 63: {
                                    // AddAndReplaceKind
                                    sscanf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22042
                                  case 64: {
                                    // AddAndReplaceKind
                                    stpcpy(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22043
                                  case 65: {
                                    // AddAndReplaceKind
                                    strcasecmp(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22044
                                  case 66: {
                                    // AddAndReplaceKind
                                    strcat(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22045
                                  case 67: {
                                    // AddAndReplaceKind
                                    strchr(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22046
                                  case 68: {
                                    // AddAndReplaceKind
                                    strcmp(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22047
                                  case 69: {
                                    // AddAndReplaceKind
                                    strcoll(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22048
                                  case 70: {
                                    // AddAndReplaceKind
                                    strcpy(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22049
                                  case 71: {
                                    // AddAndReplaceKind
                                    strcspn(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22050
                                  case 72: {
                                    // AddAndReplaceKind
                                    strndup(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22051
                                  case 73: {
                                    // AddAndReplaceKind
                                    strnlen(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22052
                                  case 74: {
                                    // AddAndReplaceKind
                                    strpbrk(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22053
                                  case 75: {
                                    // AddAndReplaceKind
                                    strrchr(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22054
                                  case 76: {
                                    // AddAndReplaceKind
                                    strsep(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22055
                                  case 77: {
                                    // AddAndReplaceKind
                                    strspn(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22056
                                  case 78: {
                                    // AddAndReplaceKind
                                    strstr(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22057
                                  case 79: {
                                    // AddAndReplaceKind
                                    strtod(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22058
                                  case 80: {
                                    // AddAndReplaceKind
                                    strtof(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22059
                                  case 81: {
                                    // AddAndReplaceKind
                                    strtok(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22060
                                  case 82: {
                                    // AddAndReplaceKind
                                    strtold(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22061
                                  case 83: {
                                    // AddAndReplaceKind
                                    tempnam(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22062
                                  case 84: {
                                    // AddAndReplaceKind
                                    vprintf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22063
                                  case 85: {
                                    // AddAndReplaceKind
                                    vscanf(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22064
                                  case 86: {
                                    // AddAndReplaceKind
                                    wctomb(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22065
                                  case 87: {
                                    // AddAndReplaceKind
                                    zend_add_literal(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22066
                                  case 88: {
                                    // AddAndReplaceKind
                                    zend_add_to_list(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22067
                                  case 89: {
                                    // AddAndReplaceKind
                                    zend_atoi(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22068
                                  case 90: {
                                    // AddAndReplaceKind
                                    zend_atol(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22069
                                  case 91: {
                                    // AddAndReplaceKind
                                    zend_binary_zval_strcasecmp(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22070
                                  case 92: {
                                    // AddAndReplaceKind
                                    zend_binary_zval_strcmp(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22071
                                  case 93: {
                                    // AddAndReplaceKind
                                    zend_call_function(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22072
                                  case 94: {
                                    // AddAndReplaceKind
                                    zend_check_protected(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22073
                                  case 95: {
                                    // AddAndReplaceKind
                                    zend_class_implements(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22074
                                  case 96: {
                                    // AddAndReplaceKind
                                    zend_compare_file_handles(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22075
                                  case 97: {
                                    // AddAndReplaceKind
                                    zend_copy_constants(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22076
                                  case 98: {
                                    // AddAndReplaceKind
                                    zend_delete_global_variable(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22077
                                  case 99: {
                                    // AddAndReplaceKind
                                    zend_dirname(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22078
                                  case 100: {
                                    // AddAndReplaceKind
                                    zend_disable_class(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22079
                                  case 101: {
                                    // AddAndReplaceKind
                                    zend_disable_function(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22080
                                  case 102: {
                                    // AddAndReplaceKind
                                    zend_do_begin_class_member_function_call(
                                        tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22081
                                  case 103: {
                                    // AddAndReplaceKind
                                    zend_do_begin_dynamic_function_call(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22082
                                  case 104: {
                                    // AddAndReplaceKind
                                    zend_do_begin_function_call(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22083
                                  case 105: {
                                    // AddAndReplaceKind
                                    zend_do_begin_namespace(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22084
                                  case 106: {
                                    // AddAndReplaceKind
                                    zend_do_begin_new_object(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22085
                                  case 107: {
                                    // AddAndReplaceKind
                                    zend_do_begin_qm_op(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22086
                                  case 108: {
                                    // AddAndReplaceKind
                                    zend_do_boolean_and_begin(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22087
                                  case 109: {
                                    // AddAndReplaceKind
                                    zend_do_boolean_or_begin(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22088
                                  case 110: {
                                    // AddAndReplaceKind
                                    zend_do_case_after_statement(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22089
                                  case 111: {
                                    // AddAndReplaceKind
                                    zend_do_clone(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22090
                                  case 112: {
                                    // AddAndReplaceKind
                                    zend_do_declare_class_constant(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22091
                                  case 113: {
                                    // AddAndReplaceKind
                                    zend_do_declare_constant(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22092
                                  case 114: {
                                    // AddAndReplaceKind
                                    zend_do_declare_stmt(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22093
                                  case 115: {
                                    // AddAndReplaceKind
                                    zend_do_default_before_statement(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22094
                                  case 116: {
                                    // AddAndReplaceKind
                                    zend_do_end_class_declaration(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22095
                                  case 117: {
                                    // AddAndReplaceKind
                                    zend_do_exit(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22096
                                  case 118: {
                                    // AddAndReplaceKind
                                    zend_do_fetch_class(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22097
                                  case 119: {
                                    // AddAndReplaceKind
                                    zend_do_fetch_lexical_variable(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22098
                                  case 120: {
                                    // AddAndReplaceKind
                                    zend_do_fetch_static_member(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22099
                                  case 121: {
                                    // AddAndReplaceKind
                                    zend_do_for_before_statement(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22100
                                  case 122: {
                                    // AddAndReplaceKind
                                    zend_do_for_cond(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22101
                                  case 123: {
                                    // AddAndReplaceKind
                                    zend_do_foreach_end(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22102
                                  case 124: {
                                    // AddAndReplaceKind
                                    zend_do_if_after_statement(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22103
                                  case 125: {
                                    // AddAndReplaceKind
                                    zend_do_if_cond(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22104
                                  case 126: {
                                    // AddAndReplaceKind
                                    zend_do_implement_interface(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22105
                                  case 127: {
                                    // AddAndReplaceKind
                                    zend_do_implement_trait(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22106
                                  case 128: {
                                    // AddAndReplaceKind
                                    zend_do_inherit_interfaces(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22107
                                  case 129: {
                                    // AddAndReplaceKind
                                    zend_do_inheritance(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22108
                                  case 130: {
                                    // AddAndReplaceKind
                                    zend_do_list_end(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22109
                                  case 131: {
                                    // AddAndReplaceKind
                                    zend_do_mark_last_catch(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22110
                                  case 132: {
                                    // AddAndReplaceKind
                                    zend_do_print(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22111
                                  case 133: {
                                    // AddAndReplaceKind
                                    zend_do_return(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22112
                                  case 134: {
                                    // AddAndReplaceKind
                                    zend_do_shell_exec(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22113
                                  case 135: {
                                    // AddAndReplaceKind
                                    zend_do_verify_access_types(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22114
                                  case 136: {
                                    // AddAndReplaceKind
                                    zend_do_while_cond(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22115
                                  case 137: {
                                    // AddAndReplaceKind
                                    zend_do_while_end(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22116
                                  case 138: {
                                    // AddAndReplaceKind
                                    zend_fcall_info_argn(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22117
                                  case 139: {
                                    // AddAndReplaceKind
                                    zend_fcall_info_args(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22118
                                  case 140: {
                                    // AddAndReplaceKind
                                    zend_fcall_info_args_clear(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22119
                                  case 141: {
                                    // AddAndReplaceKind
                                    zend_get_class_fetch_type(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22120
                                  case 142: {
                                    // AddAndReplaceKind
                                    zend_get_compiled_variable_value(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22121
                                  case 143: {
                                    // AddAndReplaceKind
                                    zend_get_hash_value(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22122
                                  case 144: {
                                    // AddAndReplaceKind
                                    zend_hash_apply(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22123
                                  case 145: {
                                    // AddAndReplaceKind
                                    zend_hash_func(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22124
                                  case 146: {
                                    // AddAndReplaceKind
                                    zend_hash_get_current_key_type_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22125
                                  case 147: {
                                    // AddAndReplaceKind
                                    zend_hash_get_pointer(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22126
                                  case 148: {
                                    // AddAndReplaceKind
                                    zend_hash_index_exists(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22127
                                  case 149: {
                                    // AddAndReplaceKind
                                    zend_hash_internal_pointer_end_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22128
                                  case 150: {
                                    // AddAndReplaceKind
                                    zend_hash_internal_pointer_reset_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22129
                                  case 151: {
                                    // AddAndReplaceKind
                                    zend_hash_move_backwards_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22130
                                  case 152: {
                                    // AddAndReplaceKind
                                    zend_hash_move_forward_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22131
                                  case 153: {
                                    // AddAndReplaceKind
                                    zend_hash_reverse_apply(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22132
                                  case 154: {
                                    // AddAndReplaceKind
                                    zend_hash_set_pointer(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22133
                                  case 155: {
                                    // AddAndReplaceKind
                                    zend_hex_strtod(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22134
                                  case 156: {
                                    // AddAndReplaceKind
                                    zend_init_list(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22135
                                  case 157: {
                                    // AddAndReplaceKind
                                    zend_initialize_class_data(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22136
                                  case 158: {
                                    // AddAndReplaceKind
                                    zend_inline_hash_func(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22137
                                  case 159: {
                                    // AddAndReplaceKind
                                    zend_is_auto_global(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22138
                                  case 160: {
                                    // AddAndReplaceKind
                                    zend_iterator_unwrap(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22139
                                  case 161: {
                                    // AddAndReplaceKind
                                    zend_list_insert(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22140
                                  case 162: {
                                    // AddAndReplaceKind
                                    zend_llist_add_element(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22141
                                  case 163: {
                                    // AddAndReplaceKind
                                    zend_llist_apply(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22142
                                  case 164: {
                                    // AddAndReplaceKind
                                    zend_llist_apply_with_del(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22143
                                  case 165: {
                                    // AddAndReplaceKind
                                    zend_llist_copy(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22144
                                  case 166: {
                                    // AddAndReplaceKind
                                    zend_llist_get_first_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22145
                                  case 167: {
                                    // AddAndReplaceKind
                                    zend_llist_get_last_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22146
                                  case 168: {
                                    // AddAndReplaceKind
                                    zend_llist_get_next_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22147
                                  case 169: {
                                    // AddAndReplaceKind
                                    zend_llist_get_prev_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22148
                                  case 170: {
                                    // AddAndReplaceKind
                                    zend_llist_prepend_element(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22149
                                  case 171: {
                                    // AddAndReplaceKind
                                    zend_llist_sort(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22150
                                  case 172: {
                                    // AddAndReplaceKind
                                    zend_make_callable(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22151
                                  case 173: {
                                    // AddAndReplaceKind
                                    zend_multibyte_set_script_encoding(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22152
                                  case 174: {
                                    // AddAndReplaceKind
                                    zend_multibyte_set_script_encoding_by_string(
                                        tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22153
                                  case 175: {
                                    // AddAndReplaceKind
                                    zend_object_create_proxy(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22154
                                  case 176: {
                                    // AddAndReplaceKind
                                    zend_object_std_init(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22155
                                  case 177: {
                                    // AddAndReplaceKind
                                    zend_object_store_set_object(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22156
                                  case 178: {
                                    // AddAndReplaceKind
                                    zend_objects_destroy_object(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22157
                                  case 179: {
                                    // AddAndReplaceKind
                                    zend_objects_new(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22158
                                  case 180: {
                                    // AddAndReplaceKind
                                    zend_objects_store_init(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22159
                                  case 181: {
                                    // AddAndReplaceKind
                                    zend_oct_strtod(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22160
                                  case 182: {
                                    // AddAndReplaceKind
                                    zend_print_zval(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22161
                                  case 183: {
                                    // AddAndReplaceKind
                                    zend_print_zval_r(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22162
                                  case 184: {
                                    // AddAndReplaceKind
                                    zend_ptr_stack_apply(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22163
                                  case 185: {
                                    // AddAndReplaceKind
                                    zend_ptr_stack_init_ex(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22164
                                  case 186: {
                                    // AddAndReplaceKind
                                    zend_ptr_stack_n_pop(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22165
                                  case 187: {
                                    // AddAndReplaceKind
                                    zend_ptr_stack_n_push(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22166
                                  case 188: {
                                    // AddAndReplaceKind
                                    zend_ptr_stack_push(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22167
                                  case 189: {
                                    // AddAndReplaceKind
                                    zend_quick_get_constant(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22168
                                  case 190: {
                                    // AddAndReplaceKind
                                    zend_resolve_non_class_name(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22169
                                  case 191: {
                                    // AddAndReplaceKind
                                    zend_stack_top(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22170
                                  case 192: {
                                    // AddAndReplaceKind
                                    zend_startup(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22171
                                  case 193: {
                                    // AddAndReplaceKind
                                    zend_std_get_debug_info(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22172
                                  case 194: {
                                    // AddAndReplaceKind
                                    zend_str_tolower(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22173
                                  case 195: {
                                    // AddAndReplaceKind
                                    zend_str_tolower_dup(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22174
                                  case 196: {
                                    // AddAndReplaceKind
                                    zend_stream_open(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22175
                                  case 197: {
                                    // AddAndReplaceKind
                                    zend_string_to_double(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22176
                                  case 198: {
                                    // AddAndReplaceKind
                                    zend_strndup(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22177
                                  case 199: {
                                    // AddAndReplaceKind
                                    zend_strtod(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22178
                                  case 200: {
                                    // AddAndReplaceKind
                                    zend_ts_hash_apply(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22179
                                  case 201: {
                                    // AddAndReplaceKind
                                    zend_ts_hash_func(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22180
                                  case 202: {
                                    // AddAndReplaceKind
                                    zend_ts_hash_index_exists(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22181
                                  case 203: {
                                    // AddAndReplaceKind
                                    zend_ts_hash_reverse_apply(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22182
                                  case 204: {
                                    // AddAndReplaceKind
                                    zval_set_isref_to_p(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22183
                                  case 205: {
                                    // AddAndReplaceKind
                                    zval_update_constant(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22184
                                  case 206: {
                                    // AddAndReplaceKind
                                    zval_update_constant_inline_change(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22185
                                  case 207: {
                                    // AddAndReplaceKind
                                    zval_update_constant_no_inline_change(tmp,
                                                                          0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22186
                                  case 208: {
                                    // GuardKind
                                    if (!__is_neg(
                                            "247-208", 18, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &((*tmp).type),
                                            sizeof((*tmp).type), &((**p).type),
                                            sizeof((**p).type), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args),
                                            &((tmp)->refcount__gc),
                                            sizeof((tmp)->refcount__gc),
                                            &((tmp)->type), sizeof((tmp)->type),
                                            &((tmp)->is_ref__gc),
                                            sizeof((tmp)->is_ref__gc),
                                            &((*p)->refcount__gc),
                                            sizeof((*p)->refcount__gc),
                                            &((*p)->type), sizeof((*p)->type),
                                            &((*p)->is_ref__gc),
                                            sizeof((*p)->is_ref__gc),
                                            &((*tmp).refcount__gc),
                                            sizeof((*tmp).refcount__gc),
                                            &((*tmp).is_ref__gc),
                                            sizeof((*tmp).is_ref__gc),
                                            &((**p).refcount__gc),
                                            sizeof((**p).refcount__gc),
                                            &((**p).is_ref__gc),
                                            sizeof((**p).is_ref__gc)))
                                      zval_set_refcount_p(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22187
                                  case 209: {
                                    // ReplaceKind
                                    zval_set_refcount_p((tmp), 0);

                                    break;
                                  }
#endif
                                  }
                                }
                                {
                                  switch (__choose("__SWITCH248")) {
                                  case 0:
                                    break;
#ifdef COMPILE_22188
                                  case 1: {
                                    // AddAndReplaceKind
                                    (executor_globals.argument_stack) =
                                        zend_vm_stack_new_page(
                                            ((16 * 1024) - 16));

                                    break;
                                  }
#endif
#ifdef COMPILE_22189
                                  case 2: {
                                    // AddAndReplaceKind
                                    (tmp) = (executor_globals
                                                 .uninitialized_zval_ptr);

                                    break;
                                  }
#endif
#ifdef COMPILE_22190
                                  case 3: {
                                    // AddAndReplaceKind
                                    (tmp) = *p;

                                    break;
                                  }
#endif
#ifdef COMPILE_22191
                                  case 4: {
                                    // AddAndReplaceKind
                                    *(tmp) = zval_used_for_init;

                                    break;
                                  }
#endif
#ifdef COMPILE_22192
                                  case 5: {
                                    // AddAndReplaceKind
                                    *tmp = zval_used_for_init;

                                    break;
                                  }
#endif
#ifdef COMPILE_22193
                                  case 6: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_22194
                                  case 7: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)->value.ht));

                                    break;
                                  }
#endif
#ifdef COMPILE_22195
                                  case 8: {
                                    // AddAndReplaceKind
                                    _efree(((tmp)->value.str.val));

                                    break;
                                  }
#endif
#ifdef COMPILE_22196
                                  case 9: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        (key)->arKey, key->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_22197
                                  case 10: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, (key)->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_22198
                                  case 11: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, (key)->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_22199
                                  case 12: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        &(tmp), sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_22200
                                  case 13: {
                                    // AddAndReplaceKind
                                    _zend_hash_quick_add_or_update(
                                        (executor_globals.active_symbol_table),
                                        key->arKey, key->nKeyLength, key->h,
                                        &tmp, sizeof(zval *), (void **)&p,
                                        (1 << 1));

                                    break;
                                  }
#endif
#ifdef COMPILE_22201
                                  case 14: {
                                    // AddAndReplaceKind
                                    _zend_list_addref((tmp)->value.lval);

                                    break;
                                  }
#endif
#ifdef COMPILE_22202
                                  case 15: {
                                    // AddAndReplaceKind
                                    _zend_list_delete((tmp)->value.lval);

                                    break;
                                  }
#endif
#ifdef COMPILE_22203
                                  case 16: {
                                    // AddAndReplaceKind
                                    _zval_copy_ctor_func((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22204
                                  case 17: {
                                    // AddAndReplaceKind
                                    _zval_dtor(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_22205
                                  case 18: {
                                    // AddAndReplaceKind
                                    _zval_dtor_func((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22206
                                  case 19: {
                                    // AddAndReplaceKind
                                    _zval_ptr_dtor((&(tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_22207
                                  case 20: {
                                    // AddAndReplaceKind
                                    convert_to_boolean((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22208
                                  case 21: {
                                    // AddAndReplaceKind
                                    exit(1);

                                    break;
                                  }
#endif
#ifdef COMPILE_22209
                                  case 22: {
                                    // AddAndReplaceKind
                                    gc_remove_zval_from_buffer((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22210
                                  case 23: {
                                    // AddAndReplaceKind
                                    gc_zval_check_possible_root(((tmp)));

                                    break;
                                  }
#endif
#ifdef COMPILE_22211
                                  case 24: {
                                    // AddAndReplaceKind
                                    gc_zval_possible_root((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22212
                                  case 25: {
                                    // AddAndReplaceKind
                                    i_zval_ptr_dtor((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22213
                                  case 26: {
                                    // AddAndReplaceKind
                                    if (!(executor_globals
                                              .active_symbol_table)) {
                                      zend_rebuild_symbol_table();
                                    }

                                    break;
                                  }
#endif
#ifdef COMPILE_22214
                                  case 27: {
                                    // AddAndReplaceKind
                                    if (_zend_hash_quick_add_or_update(
                                            target, key->arKey, key->nKeyLength,
                                            key->h, &tmp, sizeof(zval *),
                                            ((void *)0), (1 << 1)) == 0) {
                                      zval_addref_p(tmp);
                                    }

                                    break;
                                  }
#endif
#ifdef COMPILE_22215
                                  case 28: {
                                    // AddAndReplaceKind
                                    is_ref = (**(p)).type & 64;

                                    break;
                                  }
#endif
#ifdef COMPILE_22216
                                  case 29: {
                                    // AddAndReplaceKind
                                    is_ref = (**p).type & 64;

                                    break;
                                  }
#endif
#ifdef COMPILE_22217
                                  case 30: {
                                    // AddAndReplaceKind
                                    tmp = (executor_globals
                                               .uninitialized_zval_ptr);

                                    break;
                                  }
#endif
#ifdef COMPILE_22218
                                  case 31: {
                                    // AddAndReplaceKind
                                    tmp = *(p);

                                    break;
                                  }
#endif
#ifdef COMPILE_22219
                                  case 32: {
                                    // AddAndReplaceKind
                                    tmp = *p;

                                    break;
                                  }
#endif
#ifdef COMPILE_22220
                                  case 33: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "A non well formed numeric "
                                               "value encountered");

                                    break;
                                  }
#endif
#ifdef COMPILE_22221
                                  case 34: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "Undefined variable: %s",
                                               (key)->arKey);

                                    break;
                                  }
#endif
#ifdef COMPILE_22222
                                  case 35: {
                                    // AddAndReplaceKind
                                    zend_error((1 << 3L),
                                               "Undefined variable: %s",
                                               key->arKey);

                                    break;
                                  }
#endif
#ifdef COMPILE_22223
                                  case 36: {
                                    // AddAndReplaceKind
                                    zend_hash_destroy((tmp)->value.ht);

                                    break;
                                  }
#endif
#ifdef COMPILE_22224
                                  case 37: {
                                    // AddAndReplaceKind
                                    zend_rebuild_symbol_table();

                                    break;
                                  }
#endif
#ifdef COMPILE_22225
                                  case 38: {
                                    // AddAndReplaceKind
                                    zval_addref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22226
                                  case 39: {
                                    // AddAndReplaceKind
                                    zval_addref_p(*((p)));

                                    break;
                                  }
#endif
#ifdef COMPILE_22227
                                  case 40: {
                                    // AddAndReplaceKind
                                    zval_addref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22228
                                  case 41: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22229
                                  case 42: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(*(((p))));

                                    break;
                                  }
#endif
#ifdef COMPILE_22230
                                  case 43: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(*((p)));

                                    break;
                                  }
#endif
#ifdef COMPILE_22231
                                  case 44: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22232
                                  case 45: {
                                    // AddAndReplaceKind
                                    zval_set_refcount_p((tmp), 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22233
                                  case 46: {
                                    // AddAndReplaceKind
                                    zval_set_refcount_p(tmp, 0);

                                    break;
                                  }
#endif
#ifdef COMPILE_22234
                                  case 47: {
                                    // AddAndReplaceKind
                                    zval_unset_isref_p((tmp));

                                    break;
                                  }
#endif
#ifdef COMPILE_22235
                                  case 48: {
                                    // AddAndReplaceKind
                                    zval_unset_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22236
                                  case 49: {
                                    // IfExitKind
                                    if (__is_neg("248-49", 6, &(tmp),
                                                 sizeof(tmp), &(p), sizeof(p),
                                                 &(is_ref), sizeof(is_ref),
                                                 &(key), sizeof(key), &(target),
                                                 sizeof(target), &(args),
                                                 sizeof(args)))
                                      return 0;

                                    break;
                                  }
#endif
#ifdef COMPILE_22237
                                  case 50: {
                                    // IfExitKind
                                    if (__is_neg("248-50", 6, &(tmp),
                                                 sizeof(tmp), &(p), sizeof(p),
                                                 &(is_ref), sizeof(is_ref),
                                                 &(key), sizeof(key), &(target),
                                                 sizeof(target), &(args),
                                                 sizeof(args)))
                                      return 1;

                                    break;
                                  }
#endif
#ifdef COMPILE_22238
                                  case 51: {
                                    // IfExitKind
                                    if (__is_neg("248-51", 6, &(tmp),
                                                 sizeof(tmp), &(p), sizeof(p),
                                                 &(is_ref), sizeof(is_ref),
                                                 &(key), sizeof(key), &(target),
                                                 sizeof(target), &(args),
                                                 sizeof(args)))
                                      return 32;

                                    break;
                                  }
#endif
#ifdef COMPILE_22239
                                  case 52: {
                                    // IfExitKind
                                    if (__is_neg("248-52", 6, &(tmp),
                                                 sizeof(tmp), &(p), sizeof(p),
                                                 &(is_ref), sizeof(is_ref),
                                                 &(key), sizeof(key), &(target),
                                                 sizeof(target), &(args),
                                                 sizeof(args)))
                                      return 3;

                                    break;
                                  }
#endif
#ifdef COMPILE_22240
                                  case 53: {
                                    // IfExitKind
                                    if (__is_neg("248-53", 6, &(tmp),
                                                 sizeof(tmp), &(p), sizeof(p),
                                                 &(is_ref), sizeof(is_ref),
                                                 &(key), sizeof(key), &(target),
                                                 sizeof(target), &(args),
                                                 sizeof(args)))
                                      return 64;

                                    break;
                                  }
#endif
                                  }
                                  switch (__choose("__SWITCH249")) {
                                  case 0: {
                                    Z_UNSET_ISREF_P(tmp);

                                    break;
                                  }
#ifdef COMPILE_22241
                                  case 1: {
                                    // AddAndReplaceKind
                                    _convert_to_string(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22242
                                  case 2: {
                                    // AddAndReplaceKind
                                    _object_init(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22243
                                  case 3: {
                                    // AddAndReplaceKind
                                    _zval_copy_ctor(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22244
                                  case 4: {
                                    // AddAndReplaceKind
                                    _zval_copy_ctor_func(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22245
                                  case 5: {
                                    // AddAndReplaceKind
                                    _zval_dtor(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22246
                                  case 6: {
                                    // AddAndReplaceKind
                                    _zval_dtor_func(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22247
                                  case 7: {
                                    // AddAndReplaceKind
                                    _zval_dtor_wrapper(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22248
                                  case 8: {
                                    // AddAndReplaceKind
                                    _zval_internal_dtor(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22249
                                  case 9: {
                                    // AddAndReplaceKind
                                    add_next_index_null(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22250
                                  case 10: {
                                    // AddAndReplaceKind
                                    convert_scalar_to_number(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22251
                                  case 11: {
                                    // AddAndReplaceKind
                                    convert_to_array(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22252
                                  case 12: {
                                    // AddAndReplaceKind
                                    convert_to_boolean(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22253
                                  case 13: {
                                    // AddAndReplaceKind
                                    convert_to_double(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22254
                                  case 14: {
                                    // AddAndReplaceKind
                                    convert_to_long(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22255
                                  case 15: {
                                    // AddAndReplaceKind
                                    convert_to_null(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22256
                                  case 16: {
                                    // AddAndReplaceKind
                                    convert_to_object(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22257
                                  case 17: {
                                    // AddAndReplaceKind
                                    decrement_function(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22258
                                  case 18: {
                                    // AddAndReplaceKind
                                    gc_remove_zval_from_buffer(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22259
                                  case 19: {
                                    // AddAndReplaceKind
                                    gc_zobj_possible_root(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22260
                                  case 20: {
                                    // AddAndReplaceKind
                                    gc_zval_check_possible_root(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22261
                                  case 21: {
                                    // AddAndReplaceKind
                                    gc_zval_possible_root(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22262
                                  case 22: {
                                    // AddAndReplaceKind
                                    i_zend_is_true(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22263
                                  case 23: {
                                    // AddAndReplaceKind
                                    i_zval_ptr_dtor(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22264
                                  case 24: {
                                    // AddAndReplaceKind
                                    increment_function(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22265
                                  case 25: {
                                    // AddAndReplaceKind
                                    lex_scan(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22266
                                  case 26: {
                                    // AddAndReplaceKind
                                    safe_free_zval_ptr_rel(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22267
                                  case 27: {
                                    // AddAndReplaceKind
                                    zend_is_true(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22268
                                  case 28: {
                                    // AddAndReplaceKind
                                    zend_locale_sprintf_double(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22269
                                  case 29: {
                                    // AddAndReplaceKind
                                    zend_object_store_ctor_failed(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22270
                                  case 30: {
                                    // AddAndReplaceKind
                                    zend_objects_clone_obj(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22271
                                  case 31: {
                                    // AddAndReplaceKind
                                    zend_objects_store_add_ref(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22272
                                  case 32: {
                                    // AddAndReplaceKind
                                    zend_objects_store_clone_obj(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22273
                                  case 33: {
                                    // AddAndReplaceKind
                                    zend_objects_store_del_ref(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22274
                                  case 34: {
                                    // AddAndReplaceKind
                                    zend_objects_store_get_refcount(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22275
                                  case 35: {
                                    // AddAndReplaceKind
                                    zend_print_flat_zval_r(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22276
                                  case 36: {
                                    // AddAndReplaceKind
                                    zend_print_variable(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22277
                                  case 37: {
                                    // AddAndReplaceKind
                                    zend_std_get_constructor(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22278
                                  case 38: {
                                    // AddAndReplaceKind
                                    zend_std_get_properties(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22279
                                  case 39: {
                                    // AddAndReplaceKind
                                    zval_addref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22280
                                  case 40: {
                                    // AddAndReplaceKind
                                    zval_delref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22281
                                  case 41: {
                                    // AddAndReplaceKind
                                    zval_is_true(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22282
                                  case 42: {
                                    // AddAndReplaceKind
                                    zval_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22283
                                  case 43: {
                                    // AddAndReplaceKind
                                    zval_refcount_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22284
                                  case 44: {
                                    // AddAndReplaceKind
                                    zval_set_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22285
                                  case 45: {
                                    // GuardKind
                                    if (!__is_neg("249-45", 6, &(tmp),
                                                  sizeof(tmp), &(p), sizeof(p),
                                                  &(is_ref), sizeof(is_ref),
                                                  &(key), sizeof(key),
                                                  &(target), sizeof(target),
                                                  &(args), sizeof(args)))
                                      zval_unset_isref_p(tmp);

                                    break;
                                  }
#endif
#ifdef COMPILE_22286
                                  case 46: {
                                    // ReplaceKind
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
#ifdef COMPILE_22287
                            case 1: {
                              // AddAndReplaceKind
                              _convert_to_string(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22288
                            case 2: {
                              // AddAndReplaceKind
                              _object_init(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22289
                            case 3: {
                              // AddAndReplaceKind
                              _zval_copy_ctor(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22290
                            case 4: {
                              // AddAndReplaceKind
                              _zval_copy_ctor_func(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22291
                            case 5: {
                              // AddAndReplaceKind
                              _zval_dtor(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22292
                            case 6: {
                              // AddAndReplaceKind
                              _zval_dtor_func(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22293
                            case 7: {
                              // AddAndReplaceKind
                              _zval_dtor_wrapper(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22294
                            case 8: {
                              // AddAndReplaceKind
                              _zval_internal_dtor(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22295
                            case 9: {
                              // AddAndReplaceKind
                              add_next_index_null(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22296
                            case 10: {
                              // AddAndReplaceKind
                              convert_scalar_to_number(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22297
                            case 11: {
                              // AddAndReplaceKind
                              convert_to_array(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22298
                            case 12: {
                              // AddAndReplaceKind
                              convert_to_boolean(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22299
                            case 13: {
                              // AddAndReplaceKind
                              convert_to_double(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22300
                            case 14: {
                              // AddAndReplaceKind
                              convert_to_long(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22301
                            case 15: {
                              // AddAndReplaceKind
                              convert_to_null(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22302
                            case 16: {
                              // AddAndReplaceKind
                              convert_to_object(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22303
                            case 17: {
                              // AddAndReplaceKind
                              decrement_function(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22304
                            case 18: {
                              // AddAndReplaceKind
                              gc_remove_zval_from_buffer(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22305
                            case 19: {
                              // AddAndReplaceKind
                              gc_zobj_possible_root(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22306
                            case 20: {
                              // AddAndReplaceKind
                              gc_zval_check_possible_root(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22307
                            case 21: {
                              // AddAndReplaceKind
                              gc_zval_possible_root(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22308
                            case 22: {
                              // AddAndReplaceKind
                              i_zend_is_true(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22309
                            case 23: {
                              // AddAndReplaceKind
                              i_zval_ptr_dtor(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22310
                            case 24: {
                              // AddAndReplaceKind
                              increment_function(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22311
                            case 25: {
                              // AddAndReplaceKind
                              lex_scan(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22312
                            case 26: {
                              // AddAndReplaceKind
                              safe_free_zval_ptr_rel(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22313
                            case 27: {
                              // AddAndReplaceKind
                              zend_is_true(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22314
                            case 28: {
                              // AddAndReplaceKind
                              zend_locale_sprintf_double(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22315
                            case 29: {
                              // AddAndReplaceKind
                              zend_object_store_ctor_failed(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22316
                            case 30: {
                              // AddAndReplaceKind
                              zend_objects_clone_obj(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22317
                            case 31: {
                              // AddAndReplaceKind
                              zend_objects_store_add_ref(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22318
                            case 32: {
                              // AddAndReplaceKind
                              zend_objects_store_clone_obj(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22319
                            case 33: {
                              // AddAndReplaceKind
                              zend_objects_store_del_ref(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22320
                            case 34: {
                              // AddAndReplaceKind
                              zend_objects_store_get_refcount(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22321
                            case 35: {
                              // AddAndReplaceKind
                              zend_print_flat_zval_r(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22322
                            case 36: {
                              // AddAndReplaceKind
                              zend_print_variable(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22323
                            case 37: {
                              // AddAndReplaceKind
                              zend_std_get_constructor(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22324
                            case 38: {
                              // AddAndReplaceKind
                              zend_std_get_properties(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22325
                            case 39: {
                              // AddAndReplaceKind
                              zval_addref_p(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22326
                            case 40: {
                              // AddAndReplaceKind
                              zval_delref_p(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22327
                            case 41: {
                              // AddAndReplaceKind
                              zval_is_true(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22328
                            case 42: {
                              // AddAndReplaceKind
                              zval_isref_p(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22329
                            case 43: {
                              // AddAndReplaceKind
                              zval_refcount_p(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22330
                            case 44: {
                              // AddAndReplaceKind
                              zval_set_isref_p(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22331
                            case 45: {
                              // GuardKind
                              if (!__is_neg("250-45", 6, &(tmp), sizeof(tmp),
                                            &(p), sizeof(p), &(is_ref),
                                            sizeof(is_ref), &(key), sizeof(key),
                                            &(target), sizeof(target), &(args),
                                            sizeof(args)))
                                zval_unset_isref_p(tmp);

                              break;
                            }
#endif
#ifdef COMPILE_22332
                            case 46: {
                              // ReplaceKind
                              zval_unset_isref_p((tmp));

                              break;
                            }
#endif
                            }
                          }
                        }

                        break;
                      }
#ifdef COMPILE_22333
                      case 1: {
                        // AddAndReplaceKind
                        _convert_to_string(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22334
                      case 2: {
                        // AddAndReplaceKind
                        _object_init(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22335
                      case 3: {
                        // AddAndReplaceKind
                        _zval_copy_ctor(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22336
                      case 4: {
                        // AddAndReplaceKind
                        _zval_copy_ctor_func(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22337
                      case 5: {
                        // AddAndReplaceKind
                        _zval_dtor(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22338
                      case 6: {
                        // AddAndReplaceKind
                        _zval_dtor_func(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22339
                      case 7: {
                        // AddAndReplaceKind
                        _zval_dtor_wrapper(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22340
                      case 8: {
                        // AddAndReplaceKind
                        _zval_internal_dtor(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22341
                      case 9: {
                        // AddAndReplaceKind
                        add_next_index_null(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22342
                      case 10: {
                        // AddAndReplaceKind
                        convert_scalar_to_number(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22343
                      case 11: {
                        // AddAndReplaceKind
                        convert_to_array(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22344
                      case 12: {
                        // AddAndReplaceKind
                        convert_to_boolean(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22345
                      case 13: {
                        // AddAndReplaceKind
                        convert_to_double(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22346
                      case 14: {
                        // AddAndReplaceKind
                        convert_to_long(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22347
                      case 15: {
                        // AddAndReplaceKind
                        convert_to_null(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22348
                      case 16: {
                        // AddAndReplaceKind
                        convert_to_object(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22349
                      case 17: {
                        // AddAndReplaceKind
                        decrement_function(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22350
                      case 18: {
                        // AddAndReplaceKind
                        gc_remove_zval_from_buffer(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22351
                      case 19: {
                        // AddAndReplaceKind
                        gc_zobj_possible_root(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22352
                      case 20: {
                        // AddAndReplaceKind
                        gc_zval_check_possible_root(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22353
                      case 21: {
                        // AddAndReplaceKind
                        gc_zval_possible_root(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22354
                      case 22: {
                        // AddAndReplaceKind
                        i_zend_is_true(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22355
                      case 23: {
                        // AddAndReplaceKind
                        i_zval_ptr_dtor(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22356
                      case 24: {
                        // AddAndReplaceKind
                        increment_function(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22357
                      case 25: {
                        // AddAndReplaceKind
                        lex_scan(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22358
                      case 26: {
                        // AddAndReplaceKind
                        safe_free_zval_ptr_rel(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22359
                      case 27: {
                        // AddAndReplaceKind
                        zend_is_true(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22360
                      case 28: {
                        // AddAndReplaceKind
                        zend_locale_sprintf_double(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22361
                      case 29: {
                        // AddAndReplaceKind
                        zend_object_store_ctor_failed(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22362
                      case 30: {
                        // AddAndReplaceKind
                        zend_objects_clone_obj(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22363
                      case 31: {
                        // AddAndReplaceKind
                        zend_objects_store_add_ref(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22364
                      case 32: {
                        // AddAndReplaceKind
                        zend_objects_store_clone_obj(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22365
                      case 33: {
                        // AddAndReplaceKind
                        zend_objects_store_del_ref(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22366
                      case 34: {
                        // AddAndReplaceKind
                        zend_objects_store_get_refcount(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22367
                      case 35: {
                        // AddAndReplaceKind
                        zend_print_flat_zval_r(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22368
                      case 36: {
                        // AddAndReplaceKind
                        zend_print_variable(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22369
                      case 37: {
                        // AddAndReplaceKind
                        zend_std_get_constructor(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22370
                      case 38: {
                        // AddAndReplaceKind
                        zend_std_get_properties(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22371
                      case 39: {
                        // AddAndReplaceKind
                        zval_addref_p(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22372
                      case 40: {
                        // AddAndReplaceKind
                        zval_delref_p(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22373
                      case 41: {
                        // AddAndReplaceKind
                        zval_is_true(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22374
                      case 42: {
                        // AddAndReplaceKind
                        zval_isref_p(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22375
                      case 43: {
                        // AddAndReplaceKind
                        zval_refcount_p(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22376
                      case 44: {
                        // AddAndReplaceKind
                        zval_set_isref_p(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22377
                      case 45: {
                        // GuardKind
                        if (!__is_neg("251-45", 6, &(tmp), sizeof(tmp), &(p),
                                      sizeof(p), &(is_ref), sizeof(is_ref),
                                      &(key), sizeof(key), &(target),
                                      sizeof(target), &(args), sizeof(args)))
                          zval_unset_isref_p(tmp);

                        break;
                      }
#endif
#ifdef COMPILE_22378
                      case 46: {
                        // ReplaceKind
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
#ifdef COMPILE_22379
              case 1: {
                // AddAndReplaceKind
                _convert_to_string(tmp);

                break;
              }
#endif
#ifdef COMPILE_22380
              case 2: {
                // AddAndReplaceKind
                _object_init(tmp);

                break;
              }
#endif
#ifdef COMPILE_22381
              case 3: {
                // AddAndReplaceKind
                _zval_copy_ctor(tmp);

                break;
              }
#endif
#ifdef COMPILE_22382
              case 4: {
                // AddAndReplaceKind
                _zval_copy_ctor_func(tmp);

                break;
              }
#endif
#ifdef COMPILE_22383
              case 5: {
                // AddAndReplaceKind
                _zval_dtor(tmp);

                break;
              }
#endif
#ifdef COMPILE_22384
              case 6: {
                // AddAndReplaceKind
                _zval_dtor_func(tmp);

                break;
              }
#endif
#ifdef COMPILE_22385
              case 7: {
                // AddAndReplaceKind
                _zval_dtor_wrapper(tmp);

                break;
              }
#endif
#ifdef COMPILE_22386
              case 8: {
                // AddAndReplaceKind
                _zval_internal_dtor(tmp);

                break;
              }
#endif
#ifdef COMPILE_22387
              case 9: {
                // AddAndReplaceKind
                add_next_index_null(tmp);

                break;
              }
#endif
#ifdef COMPILE_22388
              case 10: {
                // AddAndReplaceKind
                convert_scalar_to_number(tmp);

                break;
              }
#endif
#ifdef COMPILE_22389
              case 11: {
                // AddAndReplaceKind
                convert_to_array(tmp);

                break;
              }
#endif
#ifdef COMPILE_22390
              case 12: {
                // AddAndReplaceKind
                convert_to_boolean(tmp);

                break;
              }
#endif
#ifdef COMPILE_22391
              case 13: {
                // AddAndReplaceKind
                convert_to_double(tmp);

                break;
              }
#endif
#ifdef COMPILE_22392
              case 14: {
                // AddAndReplaceKind
                convert_to_long(tmp);

                break;
              }
#endif
#ifdef COMPILE_22393
              case 15: {
                // AddAndReplaceKind
                convert_to_null(tmp);

                break;
              }
#endif
#ifdef COMPILE_22394
              case 16: {
                // AddAndReplaceKind
                convert_to_object(tmp);

                break;
              }
#endif
#ifdef COMPILE_22395
              case 17: {
                // AddAndReplaceKind
                decrement_function(tmp);

                break;
              }
#endif
#ifdef COMPILE_22396
              case 18: {
                // AddAndReplaceKind
                gc_remove_zval_from_buffer(tmp);

                break;
              }
#endif
#ifdef COMPILE_22397
              case 19: {
                // AddAndReplaceKind
                gc_zobj_possible_root(tmp);

                break;
              }
#endif
#ifdef COMPILE_22398
              case 20: {
                // AddAndReplaceKind
                gc_zval_check_possible_root(tmp);

                break;
              }
#endif
#ifdef COMPILE_22399
              case 21: {
                // AddAndReplaceKind
                gc_zval_possible_root(tmp);

                break;
              }
#endif
#ifdef COMPILE_22400
              case 22: {
                // AddAndReplaceKind
                i_zend_is_true(tmp);

                break;
              }
#endif
#ifdef COMPILE_22401
              case 23: {
                // AddAndReplaceKind
                i_zval_ptr_dtor(tmp);

                break;
              }
#endif
#ifdef COMPILE_22402
              case 24: {
                // AddAndReplaceKind
                increment_function(tmp);

                break;
              }
#endif
#ifdef COMPILE_22403
              case 25: {
                // AddAndReplaceKind
                lex_scan(tmp);

                break;
              }
#endif
#ifdef COMPILE_22404
              case 26: {
                // AddAndReplaceKind
                safe_free_zval_ptr_rel(tmp);

                break;
              }
#endif
#ifdef COMPILE_22405
              case 27: {
                // AddAndReplaceKind
                zend_is_true(tmp);

                break;
              }
#endif
#ifdef COMPILE_22406
              case 28: {
                // AddAndReplaceKind
                zend_locale_sprintf_double(tmp);

                break;
              }
#endif
#ifdef COMPILE_22407
              case 29: {
                // AddAndReplaceKind
                zend_object_store_ctor_failed(tmp);

                break;
              }
#endif
#ifdef COMPILE_22408
              case 30: {
                // AddAndReplaceKind
                zend_objects_clone_obj(tmp);

                break;
              }
#endif
#ifdef COMPILE_22409
              case 31: {
                // AddAndReplaceKind
                zend_objects_store_add_ref(tmp);

                break;
              }
#endif
#ifdef COMPILE_22410
              case 32: {
                // AddAndReplaceKind
                zend_objects_store_clone_obj(tmp);

                break;
              }
#endif
#ifdef COMPILE_22411
              case 33: {
                // AddAndReplaceKind
                zend_objects_store_del_ref(tmp);

                break;
              }
#endif
#ifdef COMPILE_22412
              case 34: {
                // AddAndReplaceKind
                zend_objects_store_get_refcount(tmp);

                break;
              }
#endif
#ifdef COMPILE_22413
              case 35: {
                // AddAndReplaceKind
                zend_print_flat_zval_r(tmp);

                break;
              }
#endif
#ifdef COMPILE_22414
              case 36: {
                // AddAndReplaceKind
                zend_print_variable(tmp);

                break;
              }
#endif
#ifdef COMPILE_22415
              case 37: {
                // AddAndReplaceKind
                zend_std_get_constructor(tmp);

                break;
              }
#endif
#ifdef COMPILE_22416
              case 38: {
                // AddAndReplaceKind
                zend_std_get_properties(tmp);

                break;
              }
#endif
#ifdef COMPILE_22417
              case 39: {
                // AddAndReplaceKind
                zval_addref_p(tmp);

                break;
              }
#endif
#ifdef COMPILE_22418
              case 40: {
                // AddAndReplaceKind
                zval_delref_p(tmp);

                break;
              }
#endif
#ifdef COMPILE_22419
              case 41: {
                // AddAndReplaceKind
                zval_is_true(tmp);

                break;
              }
#endif
#ifdef COMPILE_22420
              case 42: {
                // AddAndReplaceKind
                zval_isref_p(tmp);

                break;
              }
#endif
#ifdef COMPILE_22421
              case 43: {
                // AddAndReplaceKind
                zval_refcount_p(tmp);

                break;
              }
#endif
#ifdef COMPILE_22422
              case 44: {
                // AddAndReplaceKind
                zval_set_isref_p(tmp);

                break;
              }
#endif
#ifdef COMPILE_22423
              case 45: {
                // GuardKind
                if (!__is_neg("252-45", 6, &(tmp), sizeof(tmp), &(p), sizeof(p),
                              &(is_ref), sizeof(is_ref), &(key), sizeof(key),
                              &(target), sizeof(target), &(args), sizeof(args)))
                  zval_unset_isref_p(tmp);

                break;
              }
#endif
#ifdef COMPILE_22424
              case 46: {
                // ReplaceKind
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
#ifdef COMPILE_22425
      case 1: {
        // AddAndReplaceKind
        _convert_to_string(tmp);

        break;
      }
#endif
#ifdef COMPILE_22426
      case 2: {
        // AddAndReplaceKind
        _object_init(tmp);

        break;
      }
#endif
#ifdef COMPILE_22427
      case 3: {
        // AddAndReplaceKind
        _zval_copy_ctor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22428
      case 4: {
        // AddAndReplaceKind
        _zval_copy_ctor_func(tmp);

        break;
      }
#endif
#ifdef COMPILE_22429
      case 5: {
        // AddAndReplaceKind
        _zval_dtor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22430
      case 6: {
        // AddAndReplaceKind
        _zval_dtor_func(tmp);

        break;
      }
#endif
#ifdef COMPILE_22431
      case 7: {
        // AddAndReplaceKind
        _zval_dtor_wrapper(tmp);

        break;
      }
#endif
#ifdef COMPILE_22432
      case 8: {
        // AddAndReplaceKind
        _zval_internal_dtor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22433
      case 9: {
        // AddAndReplaceKind
        add_next_index_null(tmp);

        break;
      }
#endif
#ifdef COMPILE_22434
      case 10: {
        // AddAndReplaceKind
        convert_scalar_to_number(tmp);

        break;
      }
#endif
#ifdef COMPILE_22435
      case 11: {
        // AddAndReplaceKind
        convert_to_array(tmp);

        break;
      }
#endif
#ifdef COMPILE_22436
      case 12: {
        // AddAndReplaceKind
        convert_to_boolean(tmp);

        break;
      }
#endif
#ifdef COMPILE_22437
      case 13: {
        // AddAndReplaceKind
        convert_to_double(tmp);

        break;
      }
#endif
#ifdef COMPILE_22438
      case 14: {
        // AddAndReplaceKind
        convert_to_long(tmp);

        break;
      }
#endif
#ifdef COMPILE_22439
      case 15: {
        // AddAndReplaceKind
        convert_to_null(tmp);

        break;
      }
#endif
#ifdef COMPILE_22440
      case 16: {
        // AddAndReplaceKind
        convert_to_object(tmp);

        break;
      }
#endif
#ifdef COMPILE_22441
      case 17: {
        // AddAndReplaceKind
        decrement_function(tmp);

        break;
      }
#endif
#ifdef COMPILE_22442
      case 18: {
        // AddAndReplaceKind
        gc_remove_zval_from_buffer(tmp);

        break;
      }
#endif
#ifdef COMPILE_22443
      case 19: {
        // AddAndReplaceKind
        gc_zobj_possible_root(tmp);

        break;
      }
#endif
#ifdef COMPILE_22444
      case 20: {
        // AddAndReplaceKind
        gc_zval_check_possible_root(tmp);

        break;
      }
#endif
#ifdef COMPILE_22445
      case 21: {
        // AddAndReplaceKind
        gc_zval_possible_root(tmp);

        break;
      }
#endif
#ifdef COMPILE_22446
      case 22: {
        // AddAndReplaceKind
        i_zend_is_true(tmp);

        break;
      }
#endif
#ifdef COMPILE_22447
      case 23: {
        // AddAndReplaceKind
        i_zval_ptr_dtor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22448
      case 24: {
        // AddAndReplaceKind
        increment_function(tmp);

        break;
      }
#endif
#ifdef COMPILE_22449
      case 25: {
        // AddAndReplaceKind
        lex_scan(tmp);

        break;
      }
#endif
#ifdef COMPILE_22450
      case 26: {
        // AddAndReplaceKind
        safe_free_zval_ptr_rel(tmp);

        break;
      }
#endif
#ifdef COMPILE_22451
      case 27: {
        // AddAndReplaceKind
        zend_is_true(tmp);

        break;
      }
#endif
#ifdef COMPILE_22452
      case 28: {
        // AddAndReplaceKind
        zend_locale_sprintf_double(tmp);

        break;
      }
#endif
#ifdef COMPILE_22453
      case 29: {
        // AddAndReplaceKind
        zend_object_store_ctor_failed(tmp);

        break;
      }
#endif
#ifdef COMPILE_22454
      case 30: {
        // AddAndReplaceKind
        zend_objects_clone_obj(tmp);

        break;
      }
#endif
#ifdef COMPILE_22455
      case 31: {
        // AddAndReplaceKind
        zend_objects_store_add_ref(tmp);

        break;
      }
#endif
#ifdef COMPILE_22456
      case 32: {
        // AddAndReplaceKind
        zend_objects_store_clone_obj(tmp);

        break;
      }
#endif
#ifdef COMPILE_22457
      case 33: {
        // AddAndReplaceKind
        zend_objects_store_del_ref(tmp);

        break;
      }
#endif
#ifdef COMPILE_22458
      case 34: {
        // AddAndReplaceKind
        zend_objects_store_get_refcount(tmp);

        break;
      }
#endif
#ifdef COMPILE_22459
      case 35: {
        // AddAndReplaceKind
        zend_print_flat_zval_r(tmp);

        break;
      }
#endif
#ifdef COMPILE_22460
      case 36: {
        // AddAndReplaceKind
        zend_print_variable(tmp);

        break;
      }
#endif
#ifdef COMPILE_22461
      case 37: {
        // AddAndReplaceKind
        zend_std_get_constructor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22462
      case 38: {
        // AddAndReplaceKind
        zend_std_get_properties(tmp);

        break;
      }
#endif
#ifdef COMPILE_22463
      case 39: {
        // AddAndReplaceKind
        zval_addref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22464
      case 40: {
        // AddAndReplaceKind
        zval_delref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22465
      case 41: {
        // AddAndReplaceKind
        zval_is_true(tmp);

        break;
      }
#endif
#ifdef COMPILE_22466
      case 42: {
        // AddAndReplaceKind
        zval_isref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22467
      case 43: {
        // AddAndReplaceKind
        zval_refcount_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22468
      case 44: {
        // AddAndReplaceKind
        zval_set_isref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22469
      case 45: {
        // GuardKind
        if (!__is_neg("253-45", 6, &(tmp), sizeof(tmp), &(p), sizeof(p),
                      &(is_ref), sizeof(is_ref), &(key), sizeof(key), &(target),
                      sizeof(target), &(args), sizeof(args)))
          zval_unset_isref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22470
      case 46: {
        // ReplaceKind
        zval_unset_isref_p((tmp));

        break;
      }
#endif
      }
    }
  }
  // prophet generated patch
  {
    switch (__choose("__SWITCH254")) {
    case 0:
      break;
#ifdef COMPILE_22471
    case 1: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_22472
    case 2: {
      // AddAndReplaceKind
      (tmp) = (executor_globals.uninitialized_zval_ptr);

      break;
    }
#endif
#ifdef COMPILE_22473
    case 3: {
      // AddAndReplaceKind
      (tmp) = *p;

      break;
    }
#endif
#ifdef COMPILE_22474
    case 4: {
      // AddAndReplaceKind
      *(tmp) = zval_used_for_init;

      break;
    }
#endif
#ifdef COMPILE_22475
    case 5: {
      // AddAndReplaceKind
      *tmp = zval_used_for_init;

      break;
    }
#endif
#ifdef COMPILE_22476
    case 6: {
      // AddAndReplaceKind
      _efree(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_22477
    case 7: {
      // AddAndReplaceKind
      _efree(((tmp)->value.ht));

      break;
    }
#endif
#ifdef COMPILE_22478
    case 8: {
      // AddAndReplaceKind
      _efree(((tmp)->value.str.val));

      break;
    }
#endif
#ifdef COMPILE_22479
    case 9: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22480
    case 10: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22481
    case 11: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22482
    case 12: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22483
    case 13: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                     key->arKey, key->nKeyLength, key->h, &tmp,
                                     sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22484
    case 14: {
      // AddAndReplaceKind
      _zend_list_addref((tmp)->value.lval);

      break;
    }
#endif
#ifdef COMPILE_22485
    case 15: {
      // AddAndReplaceKind
      _zend_list_delete((tmp)->value.lval);

      break;
    }
#endif
#ifdef COMPILE_22486
    case 16: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((tmp));

      break;
    }
#endif
#ifdef COMPILE_22487
    case 17: {
      // AddAndReplaceKind
      _zval_dtor(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_22488
    case 18: {
      // AddAndReplaceKind
      _zval_dtor_func((tmp));

      break;
    }
#endif
#ifdef COMPILE_22489
    case 19: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(tmp)));

      break;
    }
#endif
#ifdef COMPILE_22490
    case 20: {
      // AddAndReplaceKind
      convert_to_boolean((tmp));

      break;
    }
#endif
#ifdef COMPILE_22491
    case 21: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_22492
    case 22: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((tmp));

      break;
    }
#endif
#ifdef COMPILE_22493
    case 23: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_22494
    case 24: {
      // AddAndReplaceKind
      gc_zval_possible_root((tmp));

      break;
    }
#endif
#ifdef COMPILE_22495
    case 25: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((tmp));

      break;
    }
#endif
#ifdef COMPILE_22496
    case 26: {
      // AddAndReplaceKind
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_22497
    case 27: {
      // AddAndReplaceKind
      if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                         key->h, &tmp, sizeof(zval *),
                                         ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }

      break;
    }
#endif
#ifdef COMPILE_22498
    case 28: {
      // AddAndReplaceKind
      is_ref = (**(p)).type & 64;

      break;
    }
#endif
#ifdef COMPILE_22499
    case 29: {
      // AddAndReplaceKind
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_22500
    case 30: {
      // AddAndReplaceKind
      tmp = (executor_globals.uninitialized_zval_ptr);

      break;
    }
#endif
#ifdef COMPILE_22501
    case 31: {
      // AddAndReplaceKind
      tmp = *(p);

      break;
    }
#endif
#ifdef COMPILE_22502
    case 32: {
      // AddAndReplaceKind
      tmp = *p;

      break;
    }
#endif
#ifdef COMPILE_22503
    case 33: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_22504
    case 34: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);

      break;
    }
#endif
#ifdef COMPILE_22505
    case 35: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Undefined variable: %s", key->arKey);

      break;
    }
#endif
#ifdef COMPILE_22506
    case 36: {
      // AddAndReplaceKind
      zend_hash_destroy((tmp)->value.ht);

      break;
    }
#endif
#ifdef COMPILE_22507
    case 37: {
      // AddAndReplaceKind
      zend_rebuild_symbol_table();

      break;
    }
#endif
#ifdef COMPILE_22508
    case 38: {
      // AddAndReplaceKind
      zval_addref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_22509
    case 39: {
      // AddAndReplaceKind
      zval_addref_p(*((p)));

      break;
    }
#endif
#ifdef COMPILE_22510
    case 40: {
      // AddAndReplaceKind
      zval_addref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_22511
    case 41: {
      // AddAndReplaceKind
      zval_set_isref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_22512
    case 42: {
      // AddAndReplaceKind
      zval_set_isref_p(*(((p))));

      break;
    }
#endif
#ifdef COMPILE_22513
    case 43: {
      // AddAndReplaceKind
      zval_set_isref_p(*((p)));

      break;
    }
#endif
#ifdef COMPILE_22514
    case 44: {
      // AddAndReplaceKind
      zval_set_isref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_22515
    case 45: {
      // AddAndReplaceKind
      zval_set_refcount_p((tmp), 0);

      break;
    }
#endif
#ifdef COMPILE_22516
    case 46: {
      // AddAndReplaceKind
      zval_set_refcount_p(tmp, 0);

      break;
    }
#endif
#ifdef COMPILE_22517
    case 47: {
      // AddAndReplaceKind
      zval_unset_isref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_22518
    case 48: {
      // AddAndReplaceKind
      zval_unset_isref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_22519
    case 49: {
      // AddInitKind
      memset(key, 0, sizeof(*(key)));

      break;
    }
#endif
#ifdef COMPILE_22520
    case 50: {
      // IfExitKind
      if (__is_neg("254-50", 9, &(key), sizeof(key), &(target), sizeof(target),
                   &(tmp), sizeof(tmp), &(key->arKey), sizeof(key->arKey),
                   &(key->nKeyLength), sizeof(key->nKeyLength), &(key->h),
                   sizeof(key->h), &(p), sizeof(p), &(is_ref), sizeof(is_ref),
                   &(args), sizeof(args)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_22521
    case 51: {
      // IfExitKind
      if (__is_neg("254-51", 9, &(key), sizeof(key), &(target), sizeof(target),
                   &(tmp), sizeof(tmp), &(key->arKey), sizeof(key->arKey),
                   &(key->nKeyLength), sizeof(key->nKeyLength), &(key->h),
                   sizeof(key->h), &(p), sizeof(p), &(is_ref), sizeof(is_ref),
                   &(args), sizeof(args)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_22522
    case 52: {
      // IfExitKind
      if (__is_neg("254-52", 9, &(key), sizeof(key), &(target), sizeof(target),
                   &(tmp), sizeof(tmp), &(key->arKey), sizeof(key->arKey),
                   &(key->nKeyLength), sizeof(key->nKeyLength), &(key->h),
                   sizeof(key->h), &(p), sizeof(p), &(is_ref), sizeof(is_ref),
                   &(args), sizeof(args)))
        return 32;

      break;
    }
#endif
#ifdef COMPILE_22523
    case 53: {
      // IfExitKind
      if (__is_neg("254-53", 9, &(key), sizeof(key), &(target), sizeof(target),
                   &(tmp), sizeof(tmp), &(key->arKey), sizeof(key->arKey),
                   &(key->nKeyLength), sizeof(key->nKeyLength), &(key->h),
                   sizeof(key->h), &(p), sizeof(p), &(is_ref), sizeof(is_ref),
                   &(args), sizeof(args)))
        return 3;

      break;
    }
#endif
#ifdef COMPILE_22524
    case 54: {
      // IfExitKind
      if (__is_neg("254-54", 9, &(key), sizeof(key), &(target), sizeof(target),
                   &(tmp), sizeof(tmp), &(key->arKey), sizeof(key->arKey),
                   &(key->nKeyLength), sizeof(key->nKeyLength), &(key->h),
                   sizeof(key->h), &(p), sizeof(p), &(is_ref), sizeof(is_ref),
                   &(args), sizeof(args)))
        return 64;

      break;
    }
#endif
    }
    {
      long long __temp255 =
          (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                          key->h, &tmp, sizeof(zval *),
                                          ((void *)0), (1 << 1)) == 0);
      switch (__choose("__SWITCH255")) {
      case 0: {

        break;
      }
#ifdef COMPILE_22525
      case 1: {
        __temp255 =
            ((_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) ||
             __is_neg("255-1", 9, &(key), sizeof(key), &(target),
                      sizeof(target), &(tmp), sizeof(tmp), &(key->arKey),
                      sizeof(key->arKey), &(key->nKeyLength),
                      sizeof(key->nKeyLength), &(key->h), sizeof(key->h), &(p),
                      sizeof(p), &(is_ref), sizeof(is_ref), &(args),
                      sizeof(args)));

        break;
      }
#endif
#ifdef COMPILE_22526
      case 2: {
        __temp255 =
            ((_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) &&
             !__is_neg("255-2", 9, &(key), sizeof(key), &(target),
                       sizeof(target), &(tmp), sizeof(tmp), &(key->arKey),
                       sizeof(key->arKey), &(key->nKeyLength),
                       sizeof(key->nKeyLength), &(key->h), sizeof(key->h), &(p),
                       sizeof(p), &(is_ref), sizeof(is_ref), &(args),
                       sizeof(args)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH258")) {
      case 0: {
        if (__temp255) {
          {
            switch (__choose("__SWITCH256")) {
            case 0:
              break;
#ifdef COMPILE_22527
            case 1: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_22528
            case 2: {
              // AddAndReplaceKind
              (tmp) = (executor_globals.uninitialized_zval_ptr);

              break;
            }
#endif
#ifdef COMPILE_22529
            case 3: {
              // AddAndReplaceKind
              (tmp) = *p;

              break;
            }
#endif
#ifdef COMPILE_22530
            case 4: {
              // AddAndReplaceKind
              *(tmp) = zval_used_for_init;

              break;
            }
#endif
#ifdef COMPILE_22531
            case 5: {
              // AddAndReplaceKind
              *tmp = zval_used_for_init;

              break;
            }
#endif
#ifdef COMPILE_22532
            case 6: {
              // AddAndReplaceKind
              _efree(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_22533
            case 7: {
              // AddAndReplaceKind
              _efree(((tmp)->value.ht));

              break;
            }
#endif
#ifdef COMPILE_22534
            case 8: {
              // AddAndReplaceKind
              _efree(((tmp)->value.str.val));

              break;
            }
#endif
#ifdef COMPILE_22535
            case 9: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), (key)->arKey,
                  key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_22536
            case 10: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  (key)->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_22537
            case 11: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, (key)->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_22538
            case 12: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, key->h, &(tmp), sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_22539
            case 13: {
              // AddAndReplaceKind
              _zend_hash_quick_add_or_update(
                  (executor_globals.active_symbol_table), key->arKey,
                  key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                  (1 << 1));

              break;
            }
#endif
#ifdef COMPILE_22540
            case 14: {
              // AddAndReplaceKind
              _zend_list_addref((tmp)->value.lval);

              break;
            }
#endif
#ifdef COMPILE_22541
            case 15: {
              // AddAndReplaceKind
              _zend_list_delete((tmp)->value.lval);

              break;
            }
#endif
#ifdef COMPILE_22542
            case 16: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((tmp));

              break;
            }
#endif
#ifdef COMPILE_22543
            case 17: {
              // AddAndReplaceKind
              _zval_dtor(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_22544
            case 18: {
              // AddAndReplaceKind
              _zval_dtor_func((tmp));

              break;
            }
#endif
#ifdef COMPILE_22545
            case 19: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(tmp)));

              break;
            }
#endif
#ifdef COMPILE_22546
            case 20: {
              // AddAndReplaceKind
              convert_to_boolean((tmp));

              break;
            }
#endif
#ifdef COMPILE_22547
            case 21: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_22548
            case 22: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((tmp));

              break;
            }
#endif
#ifdef COMPILE_22549
            case 23: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((tmp)));

              break;
            }
#endif
#ifdef COMPILE_22550
            case 24: {
              // AddAndReplaceKind
              gc_zval_possible_root((tmp));

              break;
            }
#endif
#ifdef COMPILE_22551
            case 25: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((tmp));

              break;
            }
#endif
#ifdef COMPILE_22552
            case 26: {
              // AddAndReplaceKind
              if (!(executor_globals.active_symbol_table)) {
                zend_rebuild_symbol_table();
              }

              break;
            }
#endif
#ifdef COMPILE_22553
            case 27: {
              // AddAndReplaceKind
              if (_zend_hash_quick_add_or_update(
                      target, key->arKey, key->nKeyLength, key->h, &tmp,
                      sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
                zval_addref_p(tmp);
              }

              break;
            }
#endif
#ifdef COMPILE_22554
            case 28: {
              // AddAndReplaceKind
              is_ref = (**(p)).type & 64;

              break;
            }
#endif
#ifdef COMPILE_22555
            case 29: {
              // AddAndReplaceKind
              is_ref = (**p).type & 64;

              break;
            }
#endif
#ifdef COMPILE_22556
            case 30: {
              // AddAndReplaceKind
              tmp = (executor_globals.uninitialized_zval_ptr);

              break;
            }
#endif
#ifdef COMPILE_22557
            case 31: {
              // AddAndReplaceKind
              tmp = *(p);

              break;
            }
#endif
#ifdef COMPILE_22558
            case 32: {
              // AddAndReplaceKind
              tmp = *p;

              break;
            }
#endif
#ifdef COMPILE_22559
            case 33: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_22560
            case 34: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);

              break;
            }
#endif
#ifdef COMPILE_22561
            case 35: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Undefined variable: %s", key->arKey);

              break;
            }
#endif
#ifdef COMPILE_22562
            case 36: {
              // AddAndReplaceKind
              zend_hash_destroy((tmp)->value.ht);

              break;
            }
#endif
#ifdef COMPILE_22563
            case 37: {
              // AddAndReplaceKind
              zend_rebuild_symbol_table();

              break;
            }
#endif
#ifdef COMPILE_22564
            case 38: {
              // AddAndReplaceKind
              zval_addref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_22565
            case 39: {
              // AddAndReplaceKind
              zval_addref_p(*((p)));

              break;
            }
#endif
#ifdef COMPILE_22566
            case 40: {
              // AddAndReplaceKind
              zval_addref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22567
            case 41: {
              // AddAndReplaceKind
              zval_set_isref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_22568
            case 42: {
              // AddAndReplaceKind
              zval_set_isref_p(*(((p))));

              break;
            }
#endif
#ifdef COMPILE_22569
            case 43: {
              // AddAndReplaceKind
              zval_set_isref_p(*((p)));

              break;
            }
#endif
#ifdef COMPILE_22570
            case 44: {
              // AddAndReplaceKind
              zval_set_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22571
            case 45: {
              // AddAndReplaceKind
              zval_set_refcount_p((tmp), 0);

              break;
            }
#endif
#ifdef COMPILE_22572
            case 46: {
              // AddAndReplaceKind
              zval_set_refcount_p(tmp, 0);

              break;
            }
#endif
#ifdef COMPILE_22573
            case 47: {
              // AddAndReplaceKind
              zval_unset_isref_p((tmp));

              break;
            }
#endif
#ifdef COMPILE_22574
            case 48: {
              // AddAndReplaceKind
              zval_unset_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22575
            case 49: {
              // IfExitKind
              if (__is_neg("256-49", 9, &(tmp), sizeof(tmp), &(key),
                           sizeof(key), &(target), sizeof(target),
                           &(key->arKey), sizeof(key->arKey),
                           &(key->nKeyLength), sizeof(key->nKeyLength),
                           &(key->h), sizeof(key->h), &(p), sizeof(p),
                           &(is_ref), sizeof(is_ref), &(args), sizeof(args)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_22576
            case 50: {
              // IfExitKind
              if (__is_neg("256-50", 9, &(tmp), sizeof(tmp), &(key),
                           sizeof(key), &(target), sizeof(target),
                           &(key->arKey), sizeof(key->arKey),
                           &(key->nKeyLength), sizeof(key->nKeyLength),
                           &(key->h), sizeof(key->h), &(p), sizeof(p),
                           &(is_ref), sizeof(is_ref), &(args), sizeof(args)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_22577
            case 51: {
              // IfExitKind
              if (__is_neg("256-51", 9, &(tmp), sizeof(tmp), &(key),
                           sizeof(key), &(target), sizeof(target),
                           &(key->arKey), sizeof(key->arKey),
                           &(key->nKeyLength), sizeof(key->nKeyLength),
                           &(key->h), sizeof(key->h), &(p), sizeof(p),
                           &(is_ref), sizeof(is_ref), &(args), sizeof(args)))
                return 32;

              break;
            }
#endif
#ifdef COMPILE_22578
            case 52: {
              // IfExitKind
              if (__is_neg("256-52", 9, &(tmp), sizeof(tmp), &(key),
                           sizeof(key), &(target), sizeof(target),
                           &(key->arKey), sizeof(key->arKey),
                           &(key->nKeyLength), sizeof(key->nKeyLength),
                           &(key->h), sizeof(key->h), &(p), sizeof(p),
                           &(is_ref), sizeof(is_ref), &(args), sizeof(args)))
                return 3;

              break;
            }
#endif
#ifdef COMPILE_22579
            case 53: {
              // IfExitKind
              if (__is_neg("256-53", 9, &(tmp), sizeof(tmp), &(key),
                           sizeof(key), &(target), sizeof(target),
                           &(key->arKey), sizeof(key->arKey),
                           &(key->nKeyLength), sizeof(key->nKeyLength),
                           &(key->h), sizeof(key->h), &(p), sizeof(p),
                           &(is_ref), sizeof(is_ref), &(args), sizeof(args)))
                return 64;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH257")) {
            case 0: {
              Z_ADDREF_P(tmp);

              break;
            }
#ifdef COMPILE_22580
            case 1: {
              // AddAndReplaceKind
              _convert_to_string(tmp);

              break;
            }
#endif
#ifdef COMPILE_22581
            case 2: {
              // AddAndReplaceKind
              _object_init(tmp);

              break;
            }
#endif
#ifdef COMPILE_22582
            case 3: {
              // AddAndReplaceKind
              _zval_copy_ctor(tmp);

              break;
            }
#endif
#ifdef COMPILE_22583
            case 4: {
              // AddAndReplaceKind
              _zval_copy_ctor_func(tmp);

              break;
            }
#endif
#ifdef COMPILE_22584
            case 5: {
              // AddAndReplaceKind
              _zval_dtor(tmp);

              break;
            }
#endif
#ifdef COMPILE_22585
            case 6: {
              // AddAndReplaceKind
              _zval_dtor_func(tmp);

              break;
            }
#endif
#ifdef COMPILE_22586
            case 7: {
              // AddAndReplaceKind
              _zval_dtor_wrapper(tmp);

              break;
            }
#endif
#ifdef COMPILE_22587
            case 8: {
              // AddAndReplaceKind
              _zval_internal_dtor(tmp);

              break;
            }
#endif
#ifdef COMPILE_22588
            case 9: {
              // AddAndReplaceKind
              add_next_index_null(tmp);

              break;
            }
#endif
#ifdef COMPILE_22589
            case 10: {
              // AddAndReplaceKind
              convert_scalar_to_number(tmp);

              break;
            }
#endif
#ifdef COMPILE_22590
            case 11: {
              // AddAndReplaceKind
              convert_to_array(tmp);

              break;
            }
#endif
#ifdef COMPILE_22591
            case 12: {
              // AddAndReplaceKind
              convert_to_boolean(tmp);

              break;
            }
#endif
#ifdef COMPILE_22592
            case 13: {
              // AddAndReplaceKind
              convert_to_double(tmp);

              break;
            }
#endif
#ifdef COMPILE_22593
            case 14: {
              // AddAndReplaceKind
              convert_to_long(tmp);

              break;
            }
#endif
#ifdef COMPILE_22594
            case 15: {
              // AddAndReplaceKind
              convert_to_null(tmp);

              break;
            }
#endif
#ifdef COMPILE_22595
            case 16: {
              // AddAndReplaceKind
              convert_to_object(tmp);

              break;
            }
#endif
#ifdef COMPILE_22596
            case 17: {
              // AddAndReplaceKind
              decrement_function(tmp);

              break;
            }
#endif
#ifdef COMPILE_22597
            case 18: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer(tmp);

              break;
            }
#endif
#ifdef COMPILE_22598
            case 19: {
              // AddAndReplaceKind
              gc_zobj_possible_root(tmp);

              break;
            }
#endif
#ifdef COMPILE_22599
            case 20: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(tmp);

              break;
            }
#endif
#ifdef COMPILE_22600
            case 21: {
              // AddAndReplaceKind
              gc_zval_possible_root(tmp);

              break;
            }
#endif
#ifdef COMPILE_22601
            case 22: {
              // AddAndReplaceKind
              i_zend_is_true(tmp);

              break;
            }
#endif
#ifdef COMPILE_22602
            case 23: {
              // AddAndReplaceKind
              i_zval_ptr_dtor(tmp);

              break;
            }
#endif
#ifdef COMPILE_22603
            case 24: {
              // AddAndReplaceKind
              increment_function(tmp);

              break;
            }
#endif
#ifdef COMPILE_22604
            case 25: {
              // AddAndReplaceKind
              lex_scan(tmp);

              break;
            }
#endif
#ifdef COMPILE_22605
            case 26: {
              // AddAndReplaceKind
              safe_free_zval_ptr_rel(tmp);

              break;
            }
#endif
#ifdef COMPILE_22606
            case 27: {
              // AddAndReplaceKind
              zend_is_true(tmp);

              break;
            }
#endif
#ifdef COMPILE_22607
            case 28: {
              // AddAndReplaceKind
              zend_locale_sprintf_double(tmp);

              break;
            }
#endif
#ifdef COMPILE_22608
            case 29: {
              // AddAndReplaceKind
              zend_object_store_ctor_failed(tmp);

              break;
            }
#endif
#ifdef COMPILE_22609
            case 30: {
              // AddAndReplaceKind
              zend_objects_clone_obj(tmp);

              break;
            }
#endif
#ifdef COMPILE_22610
            case 31: {
              // AddAndReplaceKind
              zend_objects_store_add_ref(tmp);

              break;
            }
#endif
#ifdef COMPILE_22611
            case 32: {
              // AddAndReplaceKind
              zend_objects_store_clone_obj(tmp);

              break;
            }
#endif
#ifdef COMPILE_22612
            case 33: {
              // AddAndReplaceKind
              zend_objects_store_del_ref(tmp);

              break;
            }
#endif
#ifdef COMPILE_22613
            case 34: {
              // AddAndReplaceKind
              zend_objects_store_get_refcount(tmp);

              break;
            }
#endif
#ifdef COMPILE_22614
            case 35: {
              // AddAndReplaceKind
              zend_print_flat_zval_r(tmp);

              break;
            }
#endif
#ifdef COMPILE_22615
            case 36: {
              // AddAndReplaceKind
              zend_print_variable(tmp);

              break;
            }
#endif
#ifdef COMPILE_22616
            case 37: {
              // AddAndReplaceKind
              zend_std_get_constructor(tmp);

              break;
            }
#endif
#ifdef COMPILE_22617
            case 38: {
              // AddAndReplaceKind
              zend_std_get_properties(tmp);

              break;
            }
#endif
#ifdef COMPILE_22618
            case 39: {
              // AddAndReplaceKind
              zval_delref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22619
            case 40: {
              // AddAndReplaceKind
              zval_is_true(tmp);

              break;
            }
#endif
#ifdef COMPILE_22620
            case 41: {
              // AddAndReplaceKind
              zval_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22621
            case 42: {
              // AddAndReplaceKind
              zval_refcount_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22622
            case 43: {
              // AddAndReplaceKind
              zval_set_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22623
            case 44: {
              // AddAndReplaceKind
              zval_unset_isref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22624
            case 45: {
              // GuardKind
              if (!__is_neg("257-45", 9, &(tmp), sizeof(tmp), &(key),
                            sizeof(key), &(target), sizeof(target),
                            &(key->arKey), sizeof(key->arKey),
                            &(key->nKeyLength), sizeof(key->nKeyLength),
                            &(key->h), sizeof(key->h), &(p), sizeof(p),
                            &(is_ref), sizeof(is_ref), &(args), sizeof(args)))
                zval_addref_p(tmp);

              break;
            }
#endif
#ifdef COMPILE_22625
            case 46: {
              // ReplaceKind
              zval_addref_p((tmp));

              break;
            }
#endif
            }
          }
        }

        break;
      }
#ifdef COMPILE_22626
      case 1: {
        // AddAndReplaceKind
        _convert_to_string(tmp);

        break;
      }
#endif
#ifdef COMPILE_22627
      case 2: {
        // AddAndReplaceKind
        _object_init(tmp);

        break;
      }
#endif
#ifdef COMPILE_22628
      case 3: {
        // AddAndReplaceKind
        _zval_copy_ctor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22629
      case 4: {
        // AddAndReplaceKind
        _zval_copy_ctor_func(tmp);

        break;
      }
#endif
#ifdef COMPILE_22630
      case 5: {
        // AddAndReplaceKind
        _zval_dtor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22631
      case 6: {
        // AddAndReplaceKind
        _zval_dtor_func(tmp);

        break;
      }
#endif
#ifdef COMPILE_22632
      case 7: {
        // AddAndReplaceKind
        _zval_dtor_wrapper(tmp);

        break;
      }
#endif
#ifdef COMPILE_22633
      case 8: {
        // AddAndReplaceKind
        _zval_internal_dtor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22634
      case 9: {
        // AddAndReplaceKind
        add_next_index_null(tmp);

        break;
      }
#endif
#ifdef COMPILE_22635
      case 10: {
        // AddAndReplaceKind
        convert_scalar_to_number(tmp);

        break;
      }
#endif
#ifdef COMPILE_22636
      case 11: {
        // AddAndReplaceKind
        convert_to_array(tmp);

        break;
      }
#endif
#ifdef COMPILE_22637
      case 12: {
        // AddAndReplaceKind
        convert_to_boolean(tmp);

        break;
      }
#endif
#ifdef COMPILE_22638
      case 13: {
        // AddAndReplaceKind
        convert_to_double(tmp);

        break;
      }
#endif
#ifdef COMPILE_22639
      case 14: {
        // AddAndReplaceKind
        convert_to_long(tmp);

        break;
      }
#endif
#ifdef COMPILE_22640
      case 15: {
        // AddAndReplaceKind
        convert_to_null(tmp);

        break;
      }
#endif
#ifdef COMPILE_22641
      case 16: {
        // AddAndReplaceKind
        convert_to_object(tmp);

        break;
      }
#endif
#ifdef COMPILE_22642
      case 17: {
        // AddAndReplaceKind
        decrement_function(tmp);

        break;
      }
#endif
#ifdef COMPILE_22643
      case 18: {
        // AddAndReplaceKind
        gc_remove_zval_from_buffer(tmp);

        break;
      }
#endif
#ifdef COMPILE_22644
      case 19: {
        // AddAndReplaceKind
        gc_zobj_possible_root(tmp);

        break;
      }
#endif
#ifdef COMPILE_22645
      case 20: {
        // AddAndReplaceKind
        gc_zval_check_possible_root(tmp);

        break;
      }
#endif
#ifdef COMPILE_22646
      case 21: {
        // AddAndReplaceKind
        gc_zval_possible_root(tmp);

        break;
      }
#endif
#ifdef COMPILE_22647
      case 22: {
        // AddAndReplaceKind
        i_zend_is_true(tmp);

        break;
      }
#endif
#ifdef COMPILE_22648
      case 23: {
        // AddAndReplaceKind
        i_zval_ptr_dtor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22649
      case 24: {
        // AddAndReplaceKind
        increment_function(tmp);

        break;
      }
#endif
#ifdef COMPILE_22650
      case 25: {
        // AddAndReplaceKind
        lex_scan(tmp);

        break;
      }
#endif
#ifdef COMPILE_22651
      case 26: {
        // AddAndReplaceKind
        safe_free_zval_ptr_rel(tmp);

        break;
      }
#endif
#ifdef COMPILE_22652
      case 27: {
        // AddAndReplaceKind
        zend_is_true(tmp);

        break;
      }
#endif
#ifdef COMPILE_22653
      case 28: {
        // AddAndReplaceKind
        zend_locale_sprintf_double(tmp);

        break;
      }
#endif
#ifdef COMPILE_22654
      case 29: {
        // AddAndReplaceKind
        zend_object_store_ctor_failed(tmp);

        break;
      }
#endif
#ifdef COMPILE_22655
      case 30: {
        // AddAndReplaceKind
        zend_objects_clone_obj(tmp);

        break;
      }
#endif
#ifdef COMPILE_22656
      case 31: {
        // AddAndReplaceKind
        zend_objects_store_add_ref(tmp);

        break;
      }
#endif
#ifdef COMPILE_22657
      case 32: {
        // AddAndReplaceKind
        zend_objects_store_clone_obj(tmp);

        break;
      }
#endif
#ifdef COMPILE_22658
      case 33: {
        // AddAndReplaceKind
        zend_objects_store_del_ref(tmp);

        break;
      }
#endif
#ifdef COMPILE_22659
      case 34: {
        // AddAndReplaceKind
        zend_objects_store_get_refcount(tmp);

        break;
      }
#endif
#ifdef COMPILE_22660
      case 35: {
        // AddAndReplaceKind
        zend_print_flat_zval_r(tmp);

        break;
      }
#endif
#ifdef COMPILE_22661
      case 36: {
        // AddAndReplaceKind
        zend_print_variable(tmp);

        break;
      }
#endif
#ifdef COMPILE_22662
      case 37: {
        // AddAndReplaceKind
        zend_std_get_constructor(tmp);

        break;
      }
#endif
#ifdef COMPILE_22663
      case 38: {
        // AddAndReplaceKind
        zend_std_get_properties(tmp);

        break;
      }
#endif
#ifdef COMPILE_22664
      case 39: {
        // AddAndReplaceKind
        zval_delref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22665
      case 40: {
        // AddAndReplaceKind
        zval_is_true(tmp);

        break;
      }
#endif
#ifdef COMPILE_22666
      case 41: {
        // AddAndReplaceKind
        zval_isref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22667
      case 42: {
        // AddAndReplaceKind
        zval_refcount_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22668
      case 43: {
        // AddAndReplaceKind
        zval_set_isref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22669
      case 44: {
        // AddAndReplaceKind
        zval_unset_isref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22670
      case 45: {
        // GuardKind
        if (!__is_neg("258-45", 9, &(tmp), sizeof(tmp), &(key), sizeof(key),
                      &(target), sizeof(target), &(key->arKey),
                      sizeof(key->arKey), &(key->nKeyLength),
                      sizeof(key->nKeyLength), &(key->h), sizeof(key->h), &(p),
                      sizeof(p), &(is_ref), sizeof(is_ref), &(args),
                      sizeof(args)))
          zval_addref_p(tmp);

        break;
      }
#endif
#ifdef COMPILE_22671
      case 46: {
        // ReplaceKind
        zval_addref_p((tmp));

        break;
      }
#endif
      }
    }
  }
  // prophet generated patch
  {
    switch (__choose("__SWITCH259")) {
    case 0:
      break;
#ifdef COMPILE_22672
    case 1: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_22673
    case 2: {
      // AddAndReplaceKind
      (tmp) = (executor_globals.uninitialized_zval_ptr);

      break;
    }
#endif
#ifdef COMPILE_22674
    case 3: {
      // AddAndReplaceKind
      (tmp) = *p;

      break;
    }
#endif
#ifdef COMPILE_22675
    case 4: {
      // AddAndReplaceKind
      *(tmp) = zval_used_for_init;

      break;
    }
#endif
#ifdef COMPILE_22676
    case 5: {
      // AddAndReplaceKind
      *tmp = zval_used_for_init;

      break;
    }
#endif
#ifdef COMPILE_22677
    case 6: {
      // AddAndReplaceKind
      _efree(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_22678
    case 7: {
      // AddAndReplaceKind
      _efree(((tmp)->value.ht));

      break;
    }
#endif
#ifdef COMPILE_22679
    case 8: {
      // AddAndReplaceKind
      _efree(((tmp)->value.str.val));

      break;
    }
#endif
#ifdef COMPILE_22680
    case 9: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22681
    case 10: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22682
    case 11: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22683
    case 12: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22684
    case 13: {
      // AddAndReplaceKind
      _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                     key->arKey, key->nKeyLength, key->h, &tmp,
                                     sizeof(zval *), (void **)&p, (1 << 1));

      break;
    }
#endif
#ifdef COMPILE_22685
    case 14: {
      // AddAndReplaceKind
      _zend_list_addref((tmp)->value.lval);

      break;
    }
#endif
#ifdef COMPILE_22686
    case 15: {
      // AddAndReplaceKind
      _zend_list_delete((tmp)->value.lval);

      break;
    }
#endif
#ifdef COMPILE_22687
    case 16: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((tmp));

      break;
    }
#endif
#ifdef COMPILE_22688
    case 17: {
      // AddAndReplaceKind
      _zval_dtor(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_22689
    case 18: {
      // AddAndReplaceKind
      _zval_dtor_func((tmp));

      break;
    }
#endif
#ifdef COMPILE_22690
    case 19: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(tmp)));

      break;
    }
#endif
#ifdef COMPILE_22691
    case 20: {
      // AddAndReplaceKind
      convert_to_boolean((tmp));

      break;
    }
#endif
#ifdef COMPILE_22692
    case 21: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_22693
    case 22: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((tmp));

      break;
    }
#endif
#ifdef COMPILE_22694
    case 23: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((tmp)));

      break;
    }
#endif
#ifdef COMPILE_22695
    case 24: {
      // AddAndReplaceKind
      gc_zval_possible_root((tmp));

      break;
    }
#endif
#ifdef COMPILE_22696
    case 25: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((tmp));

      break;
    }
#endif
#ifdef COMPILE_22697
    case 26: {
      // AddAndReplaceKind
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_22698
    case 27: {
      // AddAndReplaceKind
      if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                         key->h, &tmp, sizeof(zval *),
                                         ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }

      break;
    }
#endif
#ifdef COMPILE_22699
    case 28: {
      // AddAndReplaceKind
      is_ref = (**(p)).type & 64;

      break;
    }
#endif
#ifdef COMPILE_22700
    case 29: {
      // AddAndReplaceKind
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_22701
    case 30: {
      // AddAndReplaceKind
      tmp = (executor_globals.uninitialized_zval_ptr);

      break;
    }
#endif
#ifdef COMPILE_22702
    case 31: {
      // AddAndReplaceKind
      tmp = *(p);

      break;
    }
#endif
#ifdef COMPILE_22703
    case 32: {
      // AddAndReplaceKind
      tmp = *p;

      break;
    }
#endif
#ifdef COMPILE_22704
    case 33: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_22705
    case 34: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);

      break;
    }
#endif
#ifdef COMPILE_22706
    case 35: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Undefined variable: %s", key->arKey);

      break;
    }
#endif
#ifdef COMPILE_22707
    case 36: {
      // AddAndReplaceKind
      zend_hash_destroy((tmp)->value.ht);

      break;
    }
#endif
#ifdef COMPILE_22708
    case 37: {
      // AddAndReplaceKind
      zend_rebuild_symbol_table();

      break;
    }
#endif
#ifdef COMPILE_22709
    case 38: {
      // AddAndReplaceKind
      zval_addref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_22710
    case 39: {
      // AddAndReplaceKind
      zval_addref_p(*((p)));

      break;
    }
#endif
#ifdef COMPILE_22711
    case 40: {
      // AddAndReplaceKind
      zval_addref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_22712
    case 41: {
      // AddAndReplaceKind
      zval_set_isref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_22713
    case 42: {
      // AddAndReplaceKind
      zval_set_isref_p(*(((p))));

      break;
    }
#endif
#ifdef COMPILE_22714
    case 43: {
      // AddAndReplaceKind
      zval_set_isref_p(*((p)));

      break;
    }
#endif
#ifdef COMPILE_22715
    case 44: {
      // AddAndReplaceKind
      zval_set_isref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_22716
    case 45: {
      // AddAndReplaceKind
      zval_set_refcount_p((tmp), 0);

      break;
    }
#endif
#ifdef COMPILE_22717
    case 46: {
      // AddAndReplaceKind
      zval_set_refcount_p(tmp, 0);

      break;
    }
#endif
#ifdef COMPILE_22718
    case 47: {
      // AddAndReplaceKind
      zval_unset_isref_p((tmp));

      break;
    }
#endif
#ifdef COMPILE_22719
    case 48: {
      // AddAndReplaceKind
      zval_unset_isref_p(tmp);

      break;
    }
#endif
#ifdef COMPILE_22720
    case 49: {
      // IfExitKind
      if (__is_neg("259-49", 6, &(tmp), sizeof(tmp), &(key), sizeof(key),
                   &(target), sizeof(target), &(p), sizeof(p), &(is_ref),
                   sizeof(is_ref), &(args), sizeof(args)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_22721
    case 50: {
      // IfExitKind
      if (__is_neg("259-50", 6, &(tmp), sizeof(tmp), &(key), sizeof(key),
                   &(target), sizeof(target), &(p), sizeof(p), &(is_ref),
                   sizeof(is_ref), &(args), sizeof(args)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_22722
    case 51: {
      // IfExitKind
      if (__is_neg("259-51", 6, &(tmp), sizeof(tmp), &(key), sizeof(key),
                   &(target), sizeof(target), &(p), sizeof(p), &(is_ref),
                   sizeof(is_ref), &(args), sizeof(args)))
        return 32;

      break;
    }
#endif
#ifdef COMPILE_22723
    case 52: {
      // IfExitKind
      if (__is_neg("259-52", 6, &(tmp), sizeof(tmp), &(key), sizeof(key),
                   &(target), sizeof(target), &(p), sizeof(p), &(is_ref),
                   sizeof(is_ref), &(args), sizeof(args)))
        return 3;

      break;
    }
#endif
#ifdef COMPILE_22724
    case 53: {
      // IfExitKind
      if (__is_neg("259-53", 6, &(tmp), sizeof(tmp), &(key), sizeof(key),
                   &(target), sizeof(target), &(p), sizeof(p), &(is_ref),
                   sizeof(is_ref), &(args), sizeof(args)))
        return 64;

      break;
    }
#endif
    }
    switch (__choose("__SWITCH260")) {
    case 0: {
      return ZEND_HASH_APPLY_KEEP;

      break;
    }
#ifdef COMPILE_22725
    case 1: {
      // GuardKind
      if (!__is_neg("260-1", 6, &(tmp), sizeof(tmp), &(key), sizeof(key),
                    &(target), sizeof(target), &(p), sizeof(p), &(is_ref),
                    sizeof(is_ref), &(args), sizeof(args)))
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
