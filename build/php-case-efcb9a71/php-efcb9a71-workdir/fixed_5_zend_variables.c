int __get_mutant();
int __is_neg(const char *, int, const int *, int, const char *, int,
             const void **, int, const double *, int, ...);
int __abst_hole();
int __choose(char *);
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

  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    // prophet generated patch
    switch (__choose("__ID77")) {
    case 0: {
      is_ref = (**p).type & 64;

      break;
    }
#ifdef COMPILE_14678
    case 1: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14679
    case 2: {
      (tmp) = (executor_globals.uninitialized_zval_ptr);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14680
    case 3: {
      (tmp) = *p;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14681
    case 4: {
      *(tmp) = zval_used_for_init;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14682
    case 5: {
      *tmp = zval_used_for_init;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14683
    case 6: {
      _efree(((tmp)));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14684
    case 7: {
      _efree(((tmp)->value.ht));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14685
    case 8: {
      _efree(((tmp)->value.str.val));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14686
    case 9: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14687
    case 10: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14688
    case 11: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14689
    case 12: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14690
    case 13: {
      _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                     key->arKey, key->nKeyLength, key->h, &tmp,
                                     sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14691
    case 14: {
      _zend_list_addref((tmp)->value.lval);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14692
    case 15: {
      _zend_list_delete((tmp)->value.lval);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14693
    case 16: {
      _zval_copy_ctor_func((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14694
    case 17: {
      _zval_dtor(((tmp)));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14695
    case 18: {
      _zval_dtor_func((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14696
    case 19: {
      _zval_ptr_dtor((&(tmp)));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14697
    case 20: {
      convert_to_boolean((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14698
    case 21: {
      exit(1);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14699
    case 22: {
      gc_remove_zval_from_buffer((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14700
    case 23: {
      gc_zval_check_possible_root(((tmp)));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14701
    case 24: {
      gc_zval_possible_root((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14702
    case 25: {
      i_zval_ptr_dtor((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14703
    case 26: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14704
    case 27: {
      if (!__is_neg("77-27", 0, 0, 0, 0, 5,
                    (void **){p, args, key, tmp, target}, 0, 0, 6, "p",
                    "is_ref", "args", "key", "tmp", "target"))
        is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14705
    case 28: {
      if (__is_neg("77-28", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
        return 0;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14706
    case 29: {
      if (__is_neg("77-29", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
        return 1;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14707
    case 30: {
      if (__is_neg("77-30", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
        return 32;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14708
    case 31: {
      if (__is_neg("77-31", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
        return 3;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14709
    case 32: {
      if (__is_neg("77-32", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
        return 64;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14710
    case 33: {
      if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                         key->h, &tmp, sizeof(zval *),
                                         ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14711
    case 34: {
      is_ref = (**(p)).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14712
    case 35: {
      is_ref = (**(p)).type & 64;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14713
    case 36: {
      is_ref = (**p).type & 64;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14714
    case 37: {
      memset(&(**p), 0, sizeof(*(&(**p))));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14715
    case 38: {
      tmp = (executor_globals.uninitialized_zval_ptr);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14716
    case 39: {
      tmp = *(p);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14717
    case 40: {
      tmp = *p;
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14718
    case 41: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14719
    case 42: {
      zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14720
    case 43: {
      zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14721
    case 44: {
      zend_hash_destroy((tmp)->value.ht);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14722
    case 45: {
      zend_rebuild_symbol_table();
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14723
    case 46: {
      zval_addref_p((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14724
    case 47: {
      zval_addref_p(*((p)));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14725
    case 48: {
      zval_addref_p(tmp);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14726
    case 49: {
      zval_set_isref_p((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14727
    case 50: {
      zval_set_isref_p(*(((p))));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14728
    case 51: {
      zval_set_isref_p(*((p)));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14729
    case 52: {
      zval_set_isref_p(tmp);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14730
    case 53: {
      zval_set_refcount_p((tmp), 0);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14731
    case 54: {
      zval_set_refcount_p(tmp, 0);
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14732
    case 55: {
      zval_unset_isref_p((tmp));
      is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_14733
    case 56: {
      zval_unset_isref_p(tmp);
      is_ref = (**p).type & 64;

      break;
    }
#endif
    }

    // prophet generated patch
    switch (__choose("__ID78")) {
    case 0: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#ifdef COMPILE_14734
    case 1: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14735
    case 2: {
      (tmp) = (executor_globals.uninitialized_zval_ptr);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14736
    case 3: {
      (tmp) = *p;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14737
    case 4: {
      *(tmp) = zval_used_for_init;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14738
    case 5: {
      *tmp = zval_used_for_init;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14739
    case 6: {
      _efree(((tmp)));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14740
    case 7: {
      _efree(((tmp)->value.ht));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14741
    case 8: {
      _efree(((tmp)->value.str.val));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14742
    case 9: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14743
    case 10: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14744
    case 11: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14745
    case 12: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14746
    case 13: {
      _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                     key->arKey, key->nKeyLength, key->h, &tmp,
                                     sizeof(zval *), (void **)&p, (1 << 1));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14747
    case 14: {
      _zend_list_addref((tmp)->value.lval);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14748
    case 15: {
      _zend_list_delete((tmp)->value.lval);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14749
    case 16: {
      _zval_copy_ctor_func((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14750
    case 17: {
      _zval_dtor(((tmp)));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14751
    case 18: {
      _zval_dtor_func((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14752
    case 19: {
      _zval_ptr_dtor((&(tmp)));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14753
    case 20: {
      convert_to_boolean((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14754
    case 21: {
      exit(1);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14755
    case 22: {
      gc_remove_zval_from_buffer((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14756
    case 23: {
      gc_zval_check_possible_root(((tmp)));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14757
    case 24: {
      gc_zval_possible_root((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14758
    case 25: {
      i_zval_ptr_dtor((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14759
    case 26: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14760
    case 27: {
      if (!__is_neg("78-27", 0, 0, 0, 0, 5,
                    (void **){p, key, tmp, args, target}, 0, 0, 6, "p",
                    "is_ref", "key", "tmp", "args", "target") &&
          (!(executor_globals.active_symbol_table))) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14761
    case 28: {
      if (!__is_neg("78-28", 0, 0, 0, 0, 5,
                    (void **){p, key, tmp, args, target}, 0, 0, 6, "p",
                    "is_ref", "key", "tmp", "args", "target"))
        if (!(executor_globals.active_symbol_table)) {
          zend_rebuild_symbol_table();
        }

      break;
    }
#endif
#ifdef COMPILE_14762
    case 29: {
      if ((!(executor_globals.active_symbol_table)) &&
          !__is_neg("78-29", 0, 0, 0, 0, 5,
                    (void **){p, key, tmp, args, target}, 0, 0, 6, "p",
                    "is_ref", "key", "tmp", "args", "target")) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14763
    case 30: {
      if ((!(executor_globals.active_symbol_table)) ||
          __is_neg("78-30", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, 6, "p", "is_ref", "key", "tmp", "args", "target")) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14764
    case 31: {
      if (__is_neg("78-31", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, 6, "p", "is_ref", "key", "tmp", "args", "target"))
        return 0;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14765
    case 32: {
      if (__is_neg("78-32", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, 6, "p", "is_ref", "key", "tmp", "args", "target"))
        return 1;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14766
    case 33: {
      if (__is_neg("78-33", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, 6, "p", "is_ref", "key", "tmp", "args", "target"))
        return 32;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14767
    case 34: {
      if (__is_neg("78-34", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, 6, "p", "is_ref", "key", "tmp", "args", "target"))
        return 3;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14768
    case 35: {
      if (__is_neg("78-35", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, 6, "p", "is_ref", "key", "tmp", "args", "target"))
        return 64;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14769
    case 36: {
      if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                         key->h, &tmp, sizeof(zval *),
                                         ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14770
    case 37: {
      is_ref = (**(p)).type & 64;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14771
    case 38: {
      is_ref = (**p).type & 64;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14772
    case 39: {
      memset(&executor_globals, 0, sizeof(*(&executor_globals)));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14773
    case 40: {
      tmp = (executor_globals.uninitialized_zval_ptr);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14774
    case 41: {
      tmp = *(p);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14775
    case 42: {
      tmp = *p;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14776
    case 43: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14777
    case 44: {
      zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14778
    case 45: {
      zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14779
    case 46: {
      zend_hash_destroy((tmp)->value.ht);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14780
    case 47: {
      zend_rebuild_symbol_table();
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14781
    case 48: {
      zval_addref_p((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14782
    case 49: {
      zval_addref_p(*((p)));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14783
    case 50: {
      zval_addref_p(tmp);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14784
    case 51: {
      zval_set_isref_p((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14785
    case 52: {
      zval_set_isref_p(*(((p))));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14786
    case 53: {
      zval_set_isref_p(*((p)));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14787
    case 54: {
      zval_set_isref_p(tmp);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14788
    case 55: {
      zval_set_refcount_p((tmp), 0);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14789
    case 56: {
      zval_set_refcount_p(tmp, 0);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14790
    case 57: {
      zval_unset_isref_p((tmp));
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_14791
    case 58: {
      zval_unset_isref_p(tmp);
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        // prophet generated patch
        switch (__choose("__ID79")) {
        case 0: {
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#ifdef COMPILE_14792
        case 1: {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14793
        case 2: {
          (tmp) = (executor_globals.uninitialized_zval_ptr);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14794
        case 3: {
          (tmp) = *p;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14795
        case 4: {
          *(tmp) = zval_used_for_init;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14796
        case 5: {
          *tmp = zval_used_for_init;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14797
        case 6: {
          _efree(((tmp)));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14798
        case 7: {
          _efree(((tmp)->value.ht));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14799
        case 8: {
          _efree(((tmp)->value.str.val));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14800
        case 9: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         (key)->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14801
        case 10: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, (key)->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14802
        case 11: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, (key)->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14803
        case 12: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &(tmp), sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14804
        case 13: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14805
        case 14: {
          _zend_list_addref((tmp)->value.lval);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14806
        case 15: {
          _zend_list_delete((tmp)->value.lval);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14807
        case 16: {
          _zval_copy_ctor_func((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14808
        case 17: {
          _zval_dtor(((tmp)));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14809
        case 18: {
          _zval_dtor_func((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14810
        case 19: {
          _zval_ptr_dtor((&(tmp)));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14811
        case 20: {
          convert_to_boolean((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14812
        case 21: {
          exit(1);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14813
        case 22: {
          gc_remove_zval_from_buffer((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14814
        case 23: {
          gc_zval_check_possible_root(((tmp)));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14815
        case 24: {
          gc_zval_possible_root((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14816
        case 25: {
          i_zval_ptr_dtor((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14817
        case 26: {
          if (!(executor_globals.active_symbol_table)) {
            zend_rebuild_symbol_table();
          }
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14818
        case 27: {
          if (!__is_neg("79-27", 0, 0, 0, 0, 5,
                        (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                        "p", "is_ref", "key", "target", "args"))
            do {
              (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
              ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
            } while (0);

          break;
        }
#endif
#ifdef COMPILE_14819
        case 28: {
          if (__is_neg("79-28", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 0;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14820
        case 29: {
          if (__is_neg("79-29", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 1;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14821
        case 30: {
          if (__is_neg("79-30", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 32;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14822
        case 31: {
          if (__is_neg("79-31", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 3;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14823
        case 32: {
          if (__is_neg("79-32", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 64;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14824
        case 33: {
          if (_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
            zval_addref_p(tmp);
          }
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14825
        case 34: {
          is_ref = (**(p)).type & 64;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14826
        case 35: {
          is_ref = (**p).type & 64;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14827
        case 36: {
          memset(&((zval_gc_info *)(tmp))->u, 0,
                 sizeof(*(&((zval_gc_info *)(tmp))->u)));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14828
        case 37: {
          memset(((zval_gc_info *)(tmp)), 0,
                 sizeof(*(((zval_gc_info *)(tmp)))));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14829
        case 38: {
          memset((tmp), 0, sizeof(*((tmp))));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14830
        case 39: {
          tmp = (executor_globals.uninitialized_zval_ptr);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14831
        case 40: {
          tmp = *(p);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14832
        case 41: {
          tmp = *p;
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14833
        case 42: {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14834
        case 43: {
          zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14835
        case 44: {
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14836
        case 45: {
          zend_hash_destroy((tmp)->value.ht);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14837
        case 46: {
          zend_rebuild_symbol_table();
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14838
        case 47: {
          zval_addref_p((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14839
        case 48: {
          zval_addref_p(*((p)));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14840
        case 49: {
          zval_addref_p(tmp);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14841
        case 50: {
          zval_set_isref_p((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14842
        case 51: {
          zval_set_isref_p(*(((p))));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14843
        case 52: {
          zval_set_isref_p(*((p)));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14844
        case 53: {
          zval_set_isref_p(tmp);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14845
        case 54: {
          zval_set_refcount_p((tmp), 0);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14846
        case 55: {
          zval_set_refcount_p(tmp, 0);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14847
        case 56: {
          zval_unset_isref_p((tmp));
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14848
        case 57: {
          zval_unset_isref_p(tmp);
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);

          break;
        }
#endif
        }
        // prophet generated patch
        switch (__choose("__ID80")) {
        case 0: {
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#ifdef COMPILE_14849
        case 1: {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14850
        case 2: {
          (tmp) = (executor_globals.uninitialized_zval_ptr);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14851
        case 3: {
          (tmp) = *p;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14852
        case 4: {
          *(tmp) = zval_used_for_init;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14853
        case 5: {
          *tmp = zval_used_for_init;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14854
        case 6: {
          _efree(((tmp)));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14855
        case 7: {
          _efree(((tmp)->value.ht));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14856
        case 8: {
          _efree(((tmp)->value.str.val));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14857
        case 9: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         (key)->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14858
        case 10: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, (key)->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14859
        case 11: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, (key)->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14860
        case 12: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &(tmp), sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14861
        case 13: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14862
        case 14: {
          _zend_list_addref((tmp)->value.lval);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14863
        case 15: {
          _zend_list_delete((tmp)->value.lval);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14864
        case 16: {
          _zval_copy_ctor_func((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14865
        case 17: {
          _zval_dtor(((tmp)));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14866
        case 18: {
          _zval_dtor_func((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14867
        case 19: {
          _zval_ptr_dtor((&(tmp)));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14868
        case 20: {
          convert_to_boolean((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14869
        case 21: {
          exit(1);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14870
        case 22: {
          gc_remove_zval_from_buffer((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14871
        case 23: {
          gc_zval_check_possible_root(((tmp)));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14872
        case 24: {
          gc_zval_possible_root((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14873
        case 25: {
          i_zval_ptr_dtor((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14874
        case 26: {
          if (!(executor_globals.active_symbol_table)) {
            zend_rebuild_symbol_table();
          }
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14875
        case 27: {
          if (!__is_neg("80-27", 0, 0, 0, 0, 5,
                        (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                        "tmp", "is_ref", "key", "target", "args"))
            do {
              (tmp)->value = (*p)->value;
              (*tmp).type = (**p).type;
            } while (0);

          break;
        }
#endif
#ifdef COMPILE_14876
        case 28: {
          if (__is_neg("80-28", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                       "tmp", "is_ref", "key", "target", "args"))
            return 0;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14877
        case 29: {
          if (__is_neg("80-29", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                       "tmp", "is_ref", "key", "target", "args"))
            return 1;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14878
        case 30: {
          if (__is_neg("80-30", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                       "tmp", "is_ref", "key", "target", "args"))
            return 32;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14879
        case 31: {
          if (__is_neg("80-31", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                       "tmp", "is_ref", "key", "target", "args"))
            return 3;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14880
        case 32: {
          if (__is_neg("80-32", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                       "tmp", "is_ref", "key", "target", "args"))
            return 64;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14881
        case 33: {
          if (_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
            zval_addref_p(tmp);
          }
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14882
        case 34: {
          is_ref = (**(p)).type & 64;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14883
        case 35: {
          is_ref = (**p).type & 64;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14884
        case 36: {
          memset(&(**p), 0, sizeof(*(&(**p))));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14885
        case 37: {
          memset(&(*tmp), 0, sizeof(*(&(*tmp))));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14886
        case 38: {
          memset((*p), 0, sizeof(*((*p))));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14887
        case 39: {
          memset((tmp), 0, sizeof(*((tmp))));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14888
        case 40: {
          tmp = (executor_globals.uninitialized_zval_ptr);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14889
        case 41: {
          tmp = *(p);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14890
        case 42: {
          tmp = *p;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14891
        case 43: {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14892
        case 44: {
          zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14893
        case 45: {
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14894
        case 46: {
          zend_hash_destroy((tmp)->value.ht);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14895
        case 47: {
          zend_rebuild_symbol_table();
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14896
        case 48: {
          zval_addref_p((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14897
        case 49: {
          zval_addref_p(*((p)));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14898
        case 50: {
          zval_addref_p(tmp);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14899
        case 51: {
          zval_set_isref_p((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14900
        case 52: {
          zval_set_isref_p(*(((p))));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14901
        case 53: {
          zval_set_isref_p(*((p)));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14902
        case 54: {
          zval_set_isref_p(tmp);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14903
        case 55: {
          zval_set_refcount_p((tmp), 0);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14904
        case 56: {
          zval_set_refcount_p(tmp, 0);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14905
        case 57: {
          zval_unset_isref_p((tmp));
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
#ifdef COMPILE_14906
        case 58: {
          zval_unset_isref_p(tmp);
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);

          break;
        }
#endif
        }
        // prophet generated patch
        switch (__choose("__ID81")) {
        case 0: {
          zval_set_refcount_p(tmp, 0);

          break;
        }
#ifdef COMPILE_14907
        case 1: {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14908
        case 2: {
          (tmp) = (executor_globals.uninitialized_zval_ptr);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14909
        case 3: {
          (tmp) = *p;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14910
        case 4: {
          *(tmp) = zval_used_for_init;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14911
        case 5: {
          *tmp = zval_used_for_init;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14912
        case 6: {
          __overflow(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14913
        case 7: {
          __sigsetjmp(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14914
        case 8: {
          __stpcpy(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14915
        case 9: {
          __zend_realloc(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14916
        case 10: {
          _array_init(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14917
        case 11: {
          _efree(((tmp)));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14918
        case 12: {
          _efree(((tmp)->value.ht));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14919
        case 13: {
          _efree(((tmp)->value.str.val));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14920
        case 14: {
          _estrndup(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14921
        case 15: {
          _longjmp(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14922
        case 16: {
          _object_init_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14923
        case 17: {
          _zend_bailout(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14924
        case 18: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         (key)->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14925
        case 19: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, (key)->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14926
        case 20: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, (key)->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14927
        case 21: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &(tmp), sizeof(zval *), (void **)&p,
                                         (1 << 1));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14928
        case 22: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14929
        case 23: {
          _zend_list_addref((tmp)->value.lval);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14930
        case 24: {
          _zend_list_delete((tmp)->value.lval);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14931
        case 25: {
          _zend_mm_alloc(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14932
        case 26: {
          _zend_mm_block_size(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14933
        case 27: {
          _zend_mm_free(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14934
        case 28: {
          _zval_copy_ctor_func((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14935
        case 29: {
          _zval_dtor(((tmp)));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14936
        case 30: {
          _zval_dtor_func((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14937
        case 31: {
          _zval_ptr_dtor((&(tmp)));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14938
        case 32: {
          add_index_null(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14939
        case 33: {
          add_next_index_bool(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14940
        case 34: {
          add_next_index_long(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14941
        case 35: {
          add_next_index_resource(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14942
        case 36: {
          add_next_index_zval(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14943
        case 37: {
          add_trait_alias(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14944
        case 38: {
          bitwise_not_function(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14945
        case 39: {
          boolean_not_function(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14946
        case 40: {
          bzero(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14947
        case 41: {
          compile_file(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14948
        case 42: {
          compile_string(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14949
        case 43: {
          convert_to_boolean((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14950
        case 44: {
          convert_to_long_base(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14951
        case 45: {
          dlopen(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14952
        case 46: {
          dlsym(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14953
        case 47: {
          drand48_r(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14954
        case 48: {
          execute_internal(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14955
        case 49: {
          exit(1);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14956
        case 50: {
          explicit_bzero(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14957
        case 51: {
          fgetpos(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14958
        case 52: {
          fopen(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14959
        case 53: {
          fprintf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14960
        case 54: {
          fputs(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14961
        case 55: {
          fscanf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14962
        case 56: {
          fsetpos(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14963
        case 57: {
          gc_remove_zval_from_buffer((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14964
        case 58: {
          gc_zval_check_possible_root(((tmp)));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14965
        case 59: {
          gc_zval_possible_root((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14966
        case 60: {
          getloadavg(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14967
        case 61: {
          i_zval_ptr_dtor((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14968
        case 62: {
          if (!(executor_globals.active_symbol_table)) {
            zend_rebuild_symbol_table();
          }
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14969
        case 63: {
          if (!__is_neg("81-63", 0, 0, 0, 0, 5,
                        (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                        "p", "is_ref", "key", "target", "args"))
            zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14970
        case 64: {
          if (__is_neg("81-64", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 0;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14971
        case 65: {
          if (__is_neg("81-65", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 1;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14972
        case 66: {
          if (__is_neg("81-66", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 32;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14973
        case 67: {
          if (__is_neg("81-67", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 3;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14974
        case 68: {
          if (__is_neg("81-68", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp",
                       "p", "is_ref", "key", "target", "args"))
            return 64;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14975
        case 69: {
          if (_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
            zval_addref_p(tmp);
          }
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14976
        case 70: {
          index(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14977
        case 71: {
          init_trait_alias_list(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14978
        case 72: {
          instanceof_function(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14979
        case 73: {
          is_ref = (**(p)).type & 64;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14980
        case 74: {
          is_ref = (**p).type & 64;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14981
        case 75: {
          lcong48_r(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14982
        case 76: {
          longjmp(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14983
        case 77: {
          lrand48_r(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14984
        case 78: {
          mblen(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14985
        case 79: {
          mkstemps(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14986
        case 80: {
          mrand48_r(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14987
        case 81: {
          object_properties_init(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14988
        case 82: {
          on_exit(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14989
        case 83: {
          open_memstream(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14990
        case 84: {
          popen(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14991
        case 85: {
          print_op_array(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14992
        case 86: {
          printf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14993
        case 87: {
          random_r(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14994
        case 88: {
          realloc(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14995
        case 89: {
          realpath(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14996
        case 90: {
          rename(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14997
        case 91: {
          rindex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14998
        case 92: {
          scanf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_14999
        case 93: {
          seed48_r(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15000
        case 94: {
          setbuf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15001
        case 95: {
          setstate_r(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15002
        case 96: {
          siglongjmp(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15003
        case 97: {
          sprintf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15004
        case 98: {
          sscanf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15005
        case 99: {
          stpcpy(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15006
        case 100: {
          strcasecmp(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15007
        case 101: {
          strcat(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15008
        case 102: {
          strchr(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15009
        case 103: {
          strcmp(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15010
        case 104: {
          strcoll(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15011
        case 105: {
          strcpy(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15012
        case 106: {
          strcspn(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15013
        case 107: {
          strndup(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15014
        case 108: {
          strnlen(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15015
        case 109: {
          strpbrk(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15016
        case 110: {
          strrchr(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15017
        case 111: {
          strsep(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15018
        case 112: {
          strspn(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15019
        case 113: {
          strstr(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15020
        case 114: {
          strtod(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15021
        case 115: {
          strtof(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15022
        case 116: {
          strtok(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15023
        case 117: {
          strtold(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15024
        case 118: {
          tempnam(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15025
        case 119: {
          tmp = (executor_globals.uninitialized_zval_ptr);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15026
        case 120: {
          tmp = *(p);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15027
        case 121: {
          tmp = *p;
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15028
        case 122: {
          vprintf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15029
        case 123: {
          vscanf(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15030
        case 124: {
          wctomb(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15031
        case 125: {
          zend_add_literal(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15032
        case 126: {
          zend_add_to_list(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15033
        case 127: {
          zend_atoi(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15034
        case 128: {
          zend_atol(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15035
        case 129: {
          zend_binary_zval_strcasecmp(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15036
        case 130: {
          zend_binary_zval_strcmp(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15037
        case 131: {
          zend_call_function(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15038
        case 132: {
          zend_check_protected(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15039
        case 133: {
          zend_class_implements(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15040
        case 134: {
          zend_compare_file_handles(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15041
        case 135: {
          zend_copy_constants(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15042
        case 136: {
          zend_delete_global_variable(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15043
        case 137: {
          zend_dirname(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15044
        case 138: {
          zend_disable_class(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15045
        case 139: {
          zend_disable_function(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15046
        case 140: {
          zend_do_begin_class_member_function_call(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15047
        case 141: {
          zend_do_begin_dynamic_function_call(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15048
        case 142: {
          zend_do_begin_function_call(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15049
        case 143: {
          zend_do_begin_namespace(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15050
        case 144: {
          zend_do_begin_new_object(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15051
        case 145: {
          zend_do_begin_qm_op(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15052
        case 146: {
          zend_do_boolean_and_begin(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15053
        case 147: {
          zend_do_boolean_or_begin(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15054
        case 148: {
          zend_do_case_after_statement(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15055
        case 149: {
          zend_do_clone(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15056
        case 150: {
          zend_do_declare_class_constant(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15057
        case 151: {
          zend_do_declare_constant(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15058
        case 152: {
          zend_do_declare_stmt(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15059
        case 153: {
          zend_do_default_before_statement(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15060
        case 154: {
          zend_do_end_class_declaration(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15061
        case 155: {
          zend_do_exit(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15062
        case 156: {
          zend_do_fetch_class(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15063
        case 157: {
          zend_do_fetch_lexical_variable(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15064
        case 158: {
          zend_do_fetch_static_member(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15065
        case 159: {
          zend_do_for_before_statement(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15066
        case 160: {
          zend_do_for_cond(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15067
        case 161: {
          zend_do_foreach_end(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15068
        case 162: {
          zend_do_if_after_statement(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15069
        case 163: {
          zend_do_if_cond(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15070
        case 164: {
          zend_do_implement_interface(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15071
        case 165: {
          zend_do_implement_trait(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15072
        case 166: {
          zend_do_inherit_interfaces(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15073
        case 167: {
          zend_do_inheritance(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15074
        case 168: {
          zend_do_list_end(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15075
        case 169: {
          zend_do_mark_last_catch(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15076
        case 170: {
          zend_do_print(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15077
        case 171: {
          zend_do_return(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15078
        case 172: {
          zend_do_shell_exec(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15079
        case 173: {
          zend_do_verify_access_types(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15080
        case 174: {
          zend_do_while_cond(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15081
        case 175: {
          zend_do_while_end(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15082
        case 176: {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15083
        case 177: {
          zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15084
        case 178: {
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15085
        case 179: {
          zend_fcall_info_argn(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15086
        case 180: {
          zend_fcall_info_args(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15087
        case 181: {
          zend_fcall_info_args_clear(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15088
        case 182: {
          zend_get_class_fetch_type(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15089
        case 183: {
          zend_get_compiled_variable_value(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15090
        case 184: {
          zend_get_hash_value(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15091
        case 185: {
          zend_hash_apply(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15092
        case 186: {
          zend_hash_destroy((tmp)->value.ht);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15093
        case 187: {
          zend_hash_func(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15094
        case 188: {
          zend_hash_get_current_key_type_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15095
        case 189: {
          zend_hash_get_pointer(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15096
        case 190: {
          zend_hash_index_exists(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15097
        case 191: {
          zend_hash_internal_pointer_end_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15098
        case 192: {
          zend_hash_internal_pointer_reset_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15099
        case 193: {
          zend_hash_move_backwards_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15100
        case 194: {
          zend_hash_move_forward_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15101
        case 195: {
          zend_hash_reverse_apply(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15102
        case 196: {
          zend_hash_set_pointer(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15103
        case 197: {
          zend_hex_strtod(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15104
        case 198: {
          zend_init_list(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15105
        case 199: {
          zend_initialize_class_data(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15106
        case 200: {
          zend_inline_hash_func(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15107
        case 201: {
          zend_is_auto_global(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15108
        case 202: {
          zend_iterator_unwrap(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15109
        case 203: {
          zend_list_insert(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15110
        case 204: {
          zend_llist_add_element(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15111
        case 205: {
          zend_llist_apply(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15112
        case 206: {
          zend_llist_apply_with_del(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15113
        case 207: {
          zend_llist_copy(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15114
        case 208: {
          zend_llist_get_first_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15115
        case 209: {
          zend_llist_get_last_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15116
        case 210: {
          zend_llist_get_next_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15117
        case 211: {
          zend_llist_get_prev_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15118
        case 212: {
          zend_llist_prepend_element(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15119
        case 213: {
          zend_llist_sort(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15120
        case 214: {
          zend_make_callable(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15121
        case 215: {
          zend_multibyte_set_script_encoding(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15122
        case 216: {
          zend_multibyte_set_script_encoding_by_string(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15123
        case 217: {
          zend_object_create_proxy(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15124
        case 218: {
          zend_object_std_init(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15125
        case 219: {
          zend_object_store_set_object(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15126
        case 220: {
          zend_objects_destroy_object(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15127
        case 221: {
          zend_objects_new(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15128
        case 222: {
          zend_objects_store_init(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15129
        case 223: {
          zend_oct_strtod(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15130
        case 224: {
          zend_print_zval(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15131
        case 225: {
          zend_print_zval_r(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15132
        case 226: {
          zend_ptr_stack_apply(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15133
        case 227: {
          zend_ptr_stack_init_ex(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15134
        case 228: {
          zend_ptr_stack_n_pop(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15135
        case 229: {
          zend_ptr_stack_n_push(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15136
        case 230: {
          zend_ptr_stack_push(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15137
        case 231: {
          zend_quick_get_constant(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15138
        case 232: {
          zend_rebuild_symbol_table();
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15139
        case 233: {
          zend_resolve_non_class_name(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15140
        case 234: {
          zend_stack_top(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15141
        case 235: {
          zend_startup(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15142
        case 236: {
          zend_std_get_debug_info(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15143
        case 237: {
          zend_str_tolower(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15144
        case 238: {
          zend_str_tolower_dup(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15145
        case 239: {
          zend_stream_open(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15146
        case 240: {
          zend_string_to_double(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15147
        case 241: {
          zend_strndup(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15148
        case 242: {
          zend_strtod(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15149
        case 243: {
          zend_ts_hash_apply(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15150
        case 244: {
          zend_ts_hash_func(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15151
        case 245: {
          zend_ts_hash_index_exists(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15152
        case 246: {
          zend_ts_hash_reverse_apply(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15153
        case 247: {
          zval_addref_p((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15154
        case 248: {
          zval_addref_p(*((p)));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15155
        case 249: {
          zval_addref_p(tmp);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15156
        case 250: {
          zval_set_isref_p((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15157
        case 251: {
          zval_set_isref_p(*(((p))));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15158
        case 252: {
          zval_set_isref_p(*((p)));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15159
        case 253: {
          zval_set_isref_p(tmp);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15160
        case 254: {
          zval_set_isref_to_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15161
        case 255: {
          zval_set_refcount_p((tmp), 0);

          break;
        }
#endif
#ifdef COMPILE_15162
        case 256: {
          zval_set_refcount_p((tmp), 0);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15163
        case 257: {
          zval_set_refcount_p(tmp, 0);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15164
        case 258: {
          zval_unset_isref_p((tmp));
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15165
        case 259: {
          zval_unset_isref_p(tmp);
          zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15166
        case 260: {
          zval_update_constant(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15167
        case 261: {
          zval_update_constant_inline_change(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_15168
        case 262: {
          zval_update_constant_no_inline_change(tmp, 0);

          break;
        }
#endif
        }
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }
  return ZEND_HASH_APPLY_KEEP;
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
switch (__choose("__ID82")) {
case 0: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#ifdef COMPILE_15169
case 1: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15170
case 2: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        (tmp) = (executor_globals.uninitialized_zval_ptr);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15171
case 3: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        (tmp) = *p;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15172
case 4: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        *(tmp) = zval_used_for_init;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15173
case 5: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        *tmp = zval_used_for_init;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15174
case 6: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _convert_to_string(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15175
case 7: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _efree(((tmp)));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15176
case 8: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _efree(((tmp)->value.ht));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15177
case 9: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _efree(((tmp)->value.str.val));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15178
case 10: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _object_init(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15179
case 11: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       (key)->arKey, key->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15180
case 12: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       key->arKey, (key)->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15181
case 13: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15182
case 14: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15183
case 15: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15184
case 16: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zend_list_addref((tmp)->value.lval);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15185
case 17: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zend_list_delete((tmp)->value.lval);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15186
case 18: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_copy_ctor(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15187
case 19: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_copy_ctor_func((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15188
case 20: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_copy_ctor_func(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15189
case 21: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_dtor(((tmp)));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15190
case 22: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_dtor(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15191
case 23: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_dtor_func((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15192
case 24: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_dtor_func(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15193
case 25: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_dtor_wrapper(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15194
case 26: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_internal_dtor(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15195
case 27: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        _zval_ptr_dtor((&(tmp)));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15196
case 28: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        add_next_index_null(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15197
case 29: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        convert_scalar_to_number(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15198
case 30: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        convert_to_array(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15199
case 31: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        convert_to_boolean((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15200
case 32: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        convert_to_boolean(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15201
case 33: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        convert_to_double(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15202
case 34: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        convert_to_long(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15203
case 35: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        convert_to_null(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15204
case 36: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        convert_to_object(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15205
case 37: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        decrement_function(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15206
case 38: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        exit(1);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15207
case 39: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        gc_remove_zval_from_buffer((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15208
case 40: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        gc_remove_zval_from_buffer(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15209
case 41: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        gc_zobj_possible_root(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15210
case 42: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        gc_zval_check_possible_root(((tmp)));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15211
case 43: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        gc_zval_check_possible_root(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15212
case 44: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        gc_zval_possible_root((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15213
case 45: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        gc_zval_possible_root(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15214
case 46: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        i_zend_is_true(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15215
case 47: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        i_zval_ptr_dtor((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15216
case 48: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        i_zval_ptr_dtor(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15217
case 49: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        if (!(executor_globals.active_symbol_table)) {
          zend_rebuild_symbol_table();
        }
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15218
case 50: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        if (!__is_neg("82-50", 0, 0, 0, 0, 5,
                      (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp", "p",
                      "is_ref", "key", "target", "args"))
          zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15219
case 51: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        if (__is_neg("82-51", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp", "p",
                     "is_ref", "key", "target", "args"))
          return 0;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15220
case 52: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        if (__is_neg("82-52", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp", "p",
                     "is_ref", "key", "target", "args"))
          return 1;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15221
case 53: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        if (__is_neg("82-53", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp", "p",
                     "is_ref", "key", "target", "args"))
          return 32;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15222
case 54: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        if (__is_neg("82-54", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp", "p",
                     "is_ref", "key", "target", "args"))
          return 3;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15223
case 55: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        if (__is_neg("82-55", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, 6, "tmp", "p",
                     "is_ref", "key", "target", "args"))
          return 64;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15224
case 56: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                           key->h, &tmp, sizeof(zval *),
                                           ((void *)0), (1 << 1)) == 0) {
          zval_addref_p(tmp);
        }
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15225
case 57: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        increment_function(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15226
case 58: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        is_ref = (**(p)).type & 64;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15227
case 59: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        is_ref = (**p).type & 64;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15228
case 60: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        lex_scan(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15229
case 61: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        safe_free_zval_ptr_rel(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15230
case 62: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        tmp = (executor_globals.uninitialized_zval_ptr);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15231
case 63: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        tmp = *(p);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15232
case 64: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        tmp = *p;
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15233
case 65: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_error((1 << 3L), "A non well formed numeric value encountered");
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15234
case 66: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15235
case 67: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15236
case 68: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_hash_destroy((tmp)->value.ht);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15237
case 69: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_is_true(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15238
case 70: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_locale_sprintf_double(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15239
case 71: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_object_store_ctor_failed(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15240
case 72: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_objects_clone_obj(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15241
case 73: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_objects_store_add_ref(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15242
case 74: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_objects_store_clone_obj(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15243
case 75: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_objects_store_del_ref(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15244
case 76: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_objects_store_get_refcount(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15245
case 77: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_print_flat_zval_r(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15246
case 78: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_print_variable(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15247
case 79: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_rebuild_symbol_table();
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15248
case 80: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_std_get_constructor(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15249
case 81: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zend_std_get_properties(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15250
case 82: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_addref_p((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15251
case 83: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_addref_p(*((p)));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15252
case 84: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_addref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15253
case 85: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_addref_p(tmp);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15254
case 86: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_delref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15255
case 87: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_is_true(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15256
case 88: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15257
case 89: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_refcount_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15258
case 90: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_set_isref_p((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15259
case 91: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_set_isref_p(*(((p))));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15260
case 92: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_set_isref_p(*((p)));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15261
case 93: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_set_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15262
case 94: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_set_isref_p(tmp);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15263
case 95: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_set_refcount_p((tmp), 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15264
case 96: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15265
case 97: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_unset_isref_p((tmp));
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15266
case 98: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_unset_isref_p((tmp));
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15267
case 99: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        zval_unset_isref_p(tmp);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15268
case 100: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15269
case 101: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        (tmp) = (executor_globals.uninitialized_zval_ptr);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15270
case 102: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        (tmp) = *p;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15271
case 103: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        *(tmp) = zval_used_for_init;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15272
case 104: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        *tmp = zval_used_for_init;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15273
case 105: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _efree(((tmp)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15274
case 106: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _efree(((tmp)->value.ht));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15275
case 107: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _efree(((tmp)->value.str.val));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15276
case 108: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       (key)->arKey, key->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15277
case 109: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       key->arKey, (key)->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15278
case 110: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15279
case 111: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15280
case 112: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15281
case 113: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_list_addref((tmp)->value.lval);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15282
case 114: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_list_delete((tmp)->value.lval);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15283
case 115: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zval_copy_ctor_func((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15284
case 116: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zval_dtor(((tmp)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15285
case 117: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zval_dtor_func((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15286
case 118: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zval_ptr_dtor((&(tmp)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15287
case 119: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        convert_to_boolean((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15288
case 120: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        exit(1);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15289
case 121: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        gc_remove_zval_from_buffer((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15290
case 122: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        gc_zval_check_possible_root(((tmp)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15291
case 123: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        gc_zval_possible_root((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15292
case 124: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        i_zval_ptr_dtor((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15293
case 125: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15294
case 126: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (!__is_neg("82-126", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                           "tmp", "is_ref", "key", "target", "args") &&
                 (zval_isref_p(*(p)))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15295
case 127: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (!__is_neg("82-127", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                           "tmp", "is_ref", "key", "target", "args"))
        if (zval_isref_p(*(p))) {
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);
          *tmp = zval_used_for_init;
          ;
          ;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);
          zval_set_refcount_p(tmp, 0);
          zval_unset_isref_p(tmp);
        } else {
          tmp = *p;
        }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15296
case 128: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if ((zval_isref_p(*(p))) &&
                 !__is_neg("82-128", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                           "tmp", "is_ref", "key", "target", "args")) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15297
case 129: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if ((zval_isref_p(*(p))) ||
                 __is_neg("82-129", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args")) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15298
case 130: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-130", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 0;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15299
case 131: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-131", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 1;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15300
case 132: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-132", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 32;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15301
case 133: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-133", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 3;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15302
case 134: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-134", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 64;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15303
case 135: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (_zend_hash_quick_add_or_update(
                     target, key->arKey, key->nKeyLength, key->h, &tmp,
                     sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15304
case 136: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        is_ref = (**(p)).type & 64;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15305
case 137: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        is_ref = (**p).type & 64;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15306
case 138: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset(&((zval_gc_info *)(tmp))->u, 0,
               sizeof(*(&((zval_gc_info *)(tmp))->u)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15307
case 139: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset(&(**p), 0, sizeof(*(&(**p))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15308
case 140: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset(&(*tmp), 0, sizeof(*(&(*tmp))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15309
case 141: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset(((zval_gc_info *)(tmp)), 0, sizeof(*(((zval_gc_info *)(tmp)))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15310
case 142: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset((*p), 0, sizeof(*((*p))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15311
case 143: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset((tmp), 0, sizeof(*((tmp))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15312
case 144: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        tmp = (executor_globals.uninitialized_zval_ptr);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15313
case 145: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        tmp = *(p);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15314
case 146: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        tmp = *p;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15315
case 147: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_error((1 << 3L), "A non well formed numeric value encountered");
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15316
case 148: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15317
case 149: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15318
case 150: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_hash_destroy((tmp)->value.ht);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15319
case 151: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_rebuild_symbol_table();
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15320
case 152: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_addref_p((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15321
case 153: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_addref_p(*((p)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15322
case 154: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_addref_p(tmp);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15323
case 155: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_isref_p((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15324
case 156: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_isref_p(*(((p))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15325
case 157: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_isref_p(*((p)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15326
case 158: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_isref_p(tmp);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15327
case 159: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_refcount_p((tmp), 0);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15328
case 160: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_refcount_p(tmp, 0);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15329
case 161: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_unset_isref_p((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15330
case 162: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_unset_isref_p(tmp);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15331
case 163: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15332
case 164: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        (tmp) = (executor_globals.uninitialized_zval_ptr);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15333
case 165: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        (tmp) = *p;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15334
case 166: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        *(tmp) = zval_used_for_init;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15335
case 167: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        *tmp = zval_used_for_init;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15336
case 168: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _efree(((tmp)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15337
case 169: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _efree(((tmp)->value.ht));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15338
case 170: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _efree(((tmp)->value.str.val));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15339
case 171: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       (key)->arKey, key->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15340
case 172: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       key->arKey, (key)->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15341
case 173: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15342
case 174: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15343
case 175: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15344
case 176: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_list_addref((tmp)->value.lval);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15345
case 177: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zend_list_delete((tmp)->value.lval);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15346
case 178: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zval_copy_ctor_func((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15347
case 179: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zval_dtor(((tmp)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15348
case 180: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zval_dtor_func((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15349
case 181: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        _zval_ptr_dtor((&(tmp)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15350
case 182: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        convert_to_boolean((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15351
case 183: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        exit(1);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15352
case 184: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        gc_remove_zval_from_buffer((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15353
case 185: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        gc_zval_check_possible_root(((tmp)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15354
case 186: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        gc_zval_possible_root((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15355
case 187: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        i_zval_ptr_dtor((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15356
case 188: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15357
case 189: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (!__is_neg("82-189", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                           "tmp", "is_ref", "key", "target", "args") &&
                 (zval_isref_p(*(p)))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15358
case 190: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (!__is_neg("82-190", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                           "tmp", "is_ref", "key", "target", "args"))
        if (zval_isref_p(*(p))) {
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);
          *tmp = zval_used_for_init;
          ;
          ;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);
          zval_set_refcount_p(tmp, 0);
          zval_unset_isref_p(tmp);
        } else {
          tmp = *p;
        }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15359
case 191: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if ((zval_isref_p(*(p))) &&
                 !__is_neg("82-191", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                           "tmp", "is_ref", "key", "target", "args")) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15360
case 192: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if ((zval_isref_p(*(p))) ||
                 __is_neg("82-192", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args")) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15361
case 193: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-193", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 0;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15362
case 194: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-194", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 1;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15363
case 195: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-195", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 32;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15364
case 196: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-196", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 3;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15365
case 197: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (__is_neg("82-197", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, 6, "p",
                          "tmp", "is_ref", "key", "target", "args"))
        return 64;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15366
case 198: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else if (_zend_hash_quick_add_or_update(
                     target, key->arKey, key->nKeyLength, key->h, &tmp,
                     sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15367
case 199: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        is_ref = (**(p)).type & 64;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15368
case 200: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        is_ref = (**p).type & 64;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15369
case 201: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset(&((zval_gc_info *)(tmp))->u, 0,
               sizeof(*(&((zval_gc_info *)(tmp))->u)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15370
case 202: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset(&(**p), 0, sizeof(*(&(**p))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15371
case 203: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset(&(*tmp), 0, sizeof(*(&(*tmp))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15372
case 204: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset(((zval_gc_info *)(tmp)), 0, sizeof(*(((zval_gc_info *)(tmp)))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15373
case 205: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset((*p), 0, sizeof(*((*p))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15374
case 206: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        memset((tmp), 0, sizeof(*((tmp))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15375
case 207: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        tmp = (executor_globals.uninitialized_zval_ptr);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15376
case 208: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        tmp = *(p);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15377
case 209: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        tmp = *p;
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15378
case 210: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_error((1 << 3L), "A non well formed numeric value encountered");
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15379
case 211: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15380
case 212: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15381
case 213: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_hash_destroy((tmp)->value.ht);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15382
case 214: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zend_rebuild_symbol_table();
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15383
case 215: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_addref_p((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15384
case 216: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_addref_p(*((p)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15385
case 217: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_addref_p(tmp);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15386
case 218: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_isref_p((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15387
case 219: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_isref_p(*(((p))));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15388
case 220: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_isref_p(*((p)));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15389
case 221: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_isref_p(tmp);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15390
case 222: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_refcount_p((tmp), 0);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15391
case 223: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_set_refcount_p(tmp, 0);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15392
case 224: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_unset_isref_p((tmp));
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15393
case 225: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      if (is_ref) {
        ALLOC_INIT_ZVAL(tmp);
        Z_SET_ISREF_P(tmp);
        zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                            key->nKeyLength, key->h, &tmp, sizeof(zval *),
                            (void **)&p);
      } else {
        tmp = EG(uninitialized_zval_ptr);
        zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        SEPARATE_ZVAL_TO_MAKE_IS_REF(p);
        tmp = *p;
      } else
        zval_unset_isref_p(tmp);
      if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15394
case 226: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15395
case 227: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    (tmp) = (executor_globals.uninitialized_zval_ptr);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15396
case 228: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    (tmp) = *p;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15397
case 229: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    *(tmp) = zval_used_for_init;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15398
case 230: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    *tmp = zval_used_for_init;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15399
case 231: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _efree(((tmp)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15400
case 232: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _efree(((tmp)->value.ht));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15401
case 233: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _efree(((tmp)->value.str.val));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15402
case 234: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   (key)->arKey, key->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15403
case 235: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, (key)->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15404
case 236: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, (key)->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15405
case 237: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, key->h, &(tmp),
                                   sizeof(zval *), (void **)&p, (1 << 1));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15406
case 238: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15407
case 239: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_list_addref((tmp)->value.lval);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15408
case 240: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_list_delete((tmp)->value.lval);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15409
case 241: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zval_copy_ctor_func((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15410
case 242: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zval_dtor(((tmp)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15411
case 243: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zval_dtor_func((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15412
case 244: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zval_ptr_dtor((&(tmp)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15413
case 245: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    convert_to_boolean((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15414
case 246: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    exit(1);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15415
case 247: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    gc_remove_zval_from_buffer((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15416
case 248: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    gc_zval_check_possible_root(((tmp)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15417
case 249: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    gc_zval_possible_root((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15418
case 250: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    i_zval_ptr_dtor((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15419
case 251: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15420
case 252: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (!__is_neg("82-252", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                  0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target") &&
        (zend_hash_quick_find((executor_globals.active_symbol_table),
                              key->arKey, key->nKeyLength, key->h,
                              (void **)&p) == -1)) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15421
case 253: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (!__is_neg("82-253", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                  0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target"))
      if (zend_hash_quick_find((executor_globals.active_symbol_table),
                               key->arKey, key->nKeyLength, key->h,
                               (void **)&p) == -1) {
        if (is_ref) {
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);
          *tmp = zval_used_for_init;
          ;
          ;
          zval_set_isref_p(tmp);
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
        } else {
          tmp = (executor_globals.uninitialized_zval_ptr);
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
        }
      } else {
        if (is_ref) {
          if (!zval_isref_p(*p)) {
            do {
              if (zval_refcount_p(*((p))) > 1) {
                zval *new_zv;
                zval_delref_p(*(p));
                do {
                  (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                  ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
                } while (0);
                do {
                  do {
                    (new_zv)->value = (*(p))->value;
                    (*new_zv).type = (**(p)).type;
                  } while (0);
                  zval_set_refcount_p(new_zv, 1);
                  zval_unset_isref_p(new_zv);
                } while (0);
                *(p) = new_zv;
                _zval_copy_ctor((new_zv));
              }
            } while (0);
            zval_set_isref_p(*((p)));
          };
          tmp = *p;
        } else if (zval_isref_p(*(p))) {
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);
          *tmp = zval_used_for_init;
          ;
          ;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);
          zval_set_refcount_p(tmp, 0);
          zval_unset_isref_p(tmp);
        } else {
          tmp = *p;
        }
      }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15422
case 254: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if ((zend_hash_quick_find((executor_globals.active_symbol_table),
                              key->arKey, key->nKeyLength, key->h,
                              (void **)&p) == -1) &&
        !__is_neg("82-254", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                  0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target")) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15423
case 255: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if ((zend_hash_quick_find((executor_globals.active_symbol_table),
                              key->arKey, key->nKeyLength, key->h,
                              (void **)&p) == -1) ||
        __is_neg("82-255", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target")) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15424
case 256: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("82-256", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target"))
      return 0;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15425
case 257: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("82-257", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target"))
      return 1;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15426
case 258: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("82-258", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target"))
      return 32;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15427
case 259: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("82-259", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target"))
      return 3;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15428
case 260: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("82-260", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, 6, "p", "key", "is_ref", "tmp", "args", "target"))
      return 64;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15429
case 261: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                       key->h, &tmp, sizeof(zval *),
                                       ((void *)0), (1 << 1)) == 0) {
      zval_addref_p(tmp);
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15430
case 262: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    is_ref = (**(p)).type & 64;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15431
case 263: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    is_ref = (**p).type & 64;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15432
case 264: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&((zval_gc_info *)(tmp))->u, 0,
           sizeof(*(&((zval_gc_info *)(tmp))->u)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15433
case 265: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&(**(p)), 0, sizeof(*(&(**(p)))));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15434
case 266: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&(**p), 0, sizeof(*(&(**p))));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15435
case 267: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&(*tmp), 0, sizeof(*(&(*tmp))));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15436
case 268: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&executor_globals, 0, sizeof(*(&executor_globals)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15437
case 269: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(((zval_gc_info *)(tmp)), 0, sizeof(*(((zval_gc_info *)(tmp)))));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15438
case 270: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset((*(p)), 0, sizeof(*((*(p)))));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15439
case 271: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset((*p), 0, sizeof(*((*p))));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15440
case 272: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset((tmp), 0, sizeof(*((tmp))));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15441
case 273: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(key, 0, sizeof(*(key)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15442
case 274: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    tmp = (executor_globals.uninitialized_zval_ptr);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15443
case 275: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    tmp = *(p);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15444
case 276: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    tmp = *p;
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15445
case 277: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15446
case 278: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15447
case 279: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15448
case 280: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_hash_destroy((tmp)->value.ht);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15449
case 281: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_rebuild_symbol_table();
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15450
case 282: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_addref_p((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15451
case 283: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_addref_p(*((p)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15452
case 284: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_addref_p(tmp);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15453
case 285: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_isref_p((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15454
case 286: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_isref_p(*(((p))));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15455
case 287: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_isref_p(*((p)));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15456
case 288: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_isref_p(tmp);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15457
case 289: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_refcount_p((tmp), 0);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15458
case 290: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_refcount_p(tmp, 0);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15459
case 291: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_unset_isref_p((tmp));
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15460
case 292: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_unset_isref_p(tmp);
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15461
case 293: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15462
case 294: {
  (tmp) = (executor_globals.uninitialized_zval_ptr);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15463
case 295: {
  (tmp) = *p;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15464
case 296: {
  *(tmp) = zval_used_for_init;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15465
case 297: {
  *tmp = zval_used_for_init;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15466
case 298: {
  _efree(((tmp)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15467
case 299: {
  _efree(((tmp)->value.ht));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15468
case 300: {
  _efree(((tmp)->value.str.val));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15469
case 301: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 (key)->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15470
case 302: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, (key)->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15471
case 303: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, (key)->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15472
case 304: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &(tmp),
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15473
case 305: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15474
case 306: {
  _zend_list_addref((tmp)->value.lval);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15475
case 307: {
  _zend_list_delete((tmp)->value.lval);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15476
case 308: {
  _zval_copy_ctor_func((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15477
case 309: {
  _zval_dtor(((tmp)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15478
case 310: {
  _zval_dtor_func((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15479
case 311: {
  _zval_ptr_dtor((&(tmp)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15480
case 312: {
  convert_to_boolean((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15481
case 313: {
  exit(1);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15482
case 314: {
  gc_remove_zval_from_buffer((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15483
case 315: {
  gc_zval_check_possible_root(((tmp)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15484
case 316: {
  gc_zval_possible_root((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15485
case 317: {
  i_zval_ptr_dtor((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15486
case 318: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15487
case 319: {
  if (!__is_neg("82-319", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                0, 0, 6, "p", "is_ref", "args", "key", "tmp", "target") &&
      ((**p).type & (32 | 64))) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15488
case 320: {
  if (!__is_neg("82-320", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                0, 0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
    if ((**p).type & (32 | 64)) {
      is_ref = (**p).type & 64;
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      if (zend_hash_quick_find((executor_globals.active_symbol_table),
                               key->arKey, key->nKeyLength, key->h,
                               (void **)&p) == -1) {
        if (is_ref) {
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);
          *tmp = zval_used_for_init;
          ;
          ;
          zval_set_isref_p(tmp);
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
        } else {
          tmp = (executor_globals.uninitialized_zval_ptr);
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
        }
      } else {
        if (is_ref) {
          if (!zval_isref_p(*p)) {
            do {
              if (zval_refcount_p(*((p))) > 1) {
                zval *new_zv;
                zval_delref_p(*(p));
                do {
                  (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                  ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
                } while (0);
                do {
                  do {
                    (new_zv)->value = (*(p))->value;
                    (*new_zv).type = (**(p)).type;
                  } while (0);
                  zval_set_refcount_p(new_zv, 1);
                  zval_unset_isref_p(new_zv);
                } while (0);
                *(p) = new_zv;
                _zval_copy_ctor((new_zv));
              }
            } while (0);
            zval_set_isref_p(*((p)));
          };
          tmp = *p;
        } else if (zval_isref_p(*(p))) {
          do {
            (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
            ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
          } while (0);
          *tmp = zval_used_for_init;
          ;
          ;
          do {
            (tmp)->value = (*p)->value;
            (*tmp).type = (**p).type;
          } while (0);
          zval_set_refcount_p(tmp, 0);
          zval_unset_isref_p(tmp);
        } else {
          tmp = *p;
        }
      }
    } else {
      tmp = *p;
    }

  break;
}
#endif
#ifdef COMPILE_15489
case 321: {
  if (((**p).type & (32 | 64)) &&
      !__is_neg("82-321", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                0, 0, 6, "p", "is_ref", "args", "key", "tmp", "target")) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15490
case 322: {
  if (((**p).type & (32 | 64)) ||
      __is_neg("82-322", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, 6, "p", "is_ref", "args", "key", "tmp", "target")) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15491
case 323: {
  if (__is_neg("82-323", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
    return 0;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15492
case 324: {
  if (__is_neg("82-324", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
    return 1;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15493
case 325: {
  if (__is_neg("82-325", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
    return 32;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15494
case 326: {
  if (__is_neg("82-326", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
    return 3;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15495
case 327: {
  if (__is_neg("82-327", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, 6, "p", "is_ref", "args", "key", "tmp", "target"))
    return 64;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15496
case 328: {
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15497
case 329: {
  is_ref = (**(p)).type & 64;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15498
case 330: {
  is_ref = (**p).type & 64;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15499
case 331: {
  memset(&((zval_gc_info *)(tmp))->u, 0,
         sizeof(*(&((zval_gc_info *)(tmp))->u)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15500
case 332: {
  memset(&(**(p)), 0, sizeof(*(&(**(p)))));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15501
case 333: {
  memset(&(**p), 0, sizeof(*(&(**p))));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15502
case 334: {
  memset(&(*tmp), 0, sizeof(*(&(*tmp))));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15503
case 335: {
  memset(&executor_globals, 0, sizeof(*(&executor_globals)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15504
case 336: {
  memset(((zval_gc_info *)(tmp)), 0, sizeof(*(((zval_gc_info *)(tmp)))));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15505
case 337: {
  memset((*(p)), 0, sizeof(*((*(p)))));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15506
case 338: {
  memset((*p), 0, sizeof(*((*p))));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15507
case 339: {
  memset((tmp), 0, sizeof(*((tmp))));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15508
case 340: {
  memset(key, 0, sizeof(*(key)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15509
case 341: {
  tmp = (executor_globals.uninitialized_zval_ptr);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15510
case 342: {
  tmp = *(p);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15511
case 343: {
  tmp = *p;
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15512
case 344: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15513
case 345: {
  zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15514
case 346: {
  zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15515
case 347: {
  zend_hash_destroy((tmp)->value.ht);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15516
case 348: {
  zend_rebuild_symbol_table();
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15517
case 349: {
  zval_addref_p((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15518
case 350: {
  zval_addref_p(*((p)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15519
case 351: {
  zval_addref_p(tmp);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15520
case 352: {
  zval_set_isref_p((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15521
case 353: {
  zval_set_isref_p(*(((p))));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15522
case 354: {
  zval_set_isref_p(*((p)));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15523
case 355: {
  zval_set_isref_p(tmp);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15524
case 356: {
  zval_set_refcount_p((tmp), 0);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15525
case 357: {
  zval_set_refcount_p(tmp, 0);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15526
case 358: {
  zval_unset_isref_p((tmp));
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_15527
case 359: {
  zval_unset_isref_p(tmp);
  if ((**p).type & (32 | 64)) {
    is_ref = (**p).type & 64;
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    if (zend_hash_quick_find((executor_globals.active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == -1) {
      if (is_ref) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        zval_set_isref_p(tmp);
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      } else {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      }
    } else {
      if (is_ref) {
        if (!zval_isref_p(*p)) {
          do {
            if (zval_refcount_p(*((p))) > 1) {
              zval *new_zv;
              zval_delref_p(*(p));
              do {
                (new_zv) = (zval *)_emalloc((sizeof(zval_gc_info)));
                ((zval_gc_info *)(new_zv))->u.buffered = ((void *)0);
              } while (0);
              do {
                do {
                  (new_zv)->value = (*(p))->value;
                  (*new_zv).type = (**(p)).type;
                } while (0);
                zval_set_refcount_p(new_zv, 1);
                zval_unset_isref_p(new_zv);
              } while (0);
              *(p) = new_zv;
              _zval_copy_ctor((new_zv));
            }
          } while (0);
          zval_set_isref_p(*((p)));
        };
        tmp = *p;
      } else if (zval_isref_p(*(p))) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
}
// prophet generated patch
switch (__choose("__ID83")) {
case 0: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p(tmp);
  }

  break;
}
#ifdef COMPILE_15528
case 1: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15529
case 2: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    (tmp) = (executor_globals.uninitialized_zval_ptr);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15530
case 3: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    (tmp) = *p;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15531
case 4: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    *(tmp) = zval_used_for_init;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15532
case 5: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    *tmp = zval_used_for_init;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15533
case 6: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _convert_to_string(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15534
case 7: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _efree(((tmp)));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15535
case 8: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _efree(((tmp)->value.ht));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15536
case 9: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _efree(((tmp)->value.str.val));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15537
case 10: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _object_init(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15538
case 11: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   (key)->arKey, key->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15539
case 12: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, (key)->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15540
case 13: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, (key)->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15541
case 14: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, key->h, &(tmp),
                                   sizeof(zval *), (void **)&p, (1 << 1));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15542
case 15: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15543
case 16: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_list_addref((tmp)->value.lval);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15544
case 17: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_list_delete((tmp)->value.lval);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15545
case 18: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_copy_ctor(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15546
case 19: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_copy_ctor_func((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15547
case 20: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_copy_ctor_func(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15548
case 21: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor(((tmp)));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15549
case 22: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15550
case 23: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor_func((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15551
case 24: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor_func(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15552
case 25: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor_wrapper(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15553
case 26: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_internal_dtor(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15554
case 27: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_ptr_dtor((&(tmp)));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15555
case 28: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    add_next_index_null(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15556
case 29: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_scalar_to_number(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15557
case 30: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_array(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15558
case 31: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_boolean((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15559
case 32: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_boolean(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15560
case 33: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_double(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15561
case 34: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_long(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15562
case 35: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_null(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15563
case 36: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_object(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15564
case 37: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    decrement_function(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15565
case 38: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    exit(1);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15566
case 39: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_remove_zval_from_buffer((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15567
case 40: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_remove_zval_from_buffer(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15568
case 41: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zobj_possible_root(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15569
case 42: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zval_check_possible_root(((tmp)));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15570
case 43: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zval_check_possible_root(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15571
case 44: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zval_possible_root((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15572
case 45: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zval_possible_root(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15573
case 46: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    i_zend_is_true(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15574
case 47: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    i_zval_ptr_dtor((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15575
case 48: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    i_zval_ptr_dtor(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15576
case 49: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15577
case 50: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (!__is_neg("83-50", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                  0, 0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
      zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15578
case 51: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("83-51", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
      return 0;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15579
case 52: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("83-52", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
      return 1;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15580
case 53: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("83-53", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
      return 32;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15581
case 54: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("83-54", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
      return 3;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15582
case 55: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("83-55", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
      return 64;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15583
case 56: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                       key->h, &tmp, sizeof(zval *),
                                       ((void *)0), (1 << 1)) == 0) {
      zval_addref_p(tmp);
    }
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15584
case 57: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    increment_function(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15585
case 58: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    is_ref = (**(p)).type & 64;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15586
case 59: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    is_ref = (**p).type & 64;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15587
case 60: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    lex_scan(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15588
case 61: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    safe_free_zval_ptr_rel(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15589
case 62: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    tmp = (executor_globals.uninitialized_zval_ptr);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15590
case 63: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    tmp = *(p);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15591
case 64: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    tmp = *p;
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15592
case 65: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15593
case 66: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15594
case 67: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15595
case 68: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_hash_destroy((tmp)->value.ht);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15596
case 69: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_is_true(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15597
case 70: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_locale_sprintf_double(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15598
case 71: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_object_store_ctor_failed(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15599
case 72: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_clone_obj(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15600
case 73: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_store_add_ref(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15601
case 74: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_store_clone_obj(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15602
case 75: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_store_del_ref(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15603
case 76: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_store_get_refcount(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15604
case 77: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_print_flat_zval_r(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15605
case 78: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_print_variable(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15606
case 79: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_rebuild_symbol_table();
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15607
case 80: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_std_get_constructor(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15608
case 81: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_std_get_properties(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15609
case 82: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p((tmp));
  }

  break;
}
#endif
#ifdef COMPILE_15610
case 83: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15611
case 84: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p(*((p)));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15612
case 85: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p(tmp);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15613
case 86: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_delref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15614
case 87: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_is_true(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15615
case 88: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_isref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15616
case 89: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_refcount_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15617
case 90: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15618
case 91: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p(*(((p))));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15619
case 92: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p(*((p)));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15620
case 93: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15621
case 94: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p(tmp);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15622
case 95: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_refcount_p((tmp), 0);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15623
case 96: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_refcount_p(tmp, 0);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15624
case 97: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_unset_isref_p((tmp));
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15625
case 98: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_unset_isref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15626
case 99: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_unset_isref_p(tmp);
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15627
case 100: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15628
case 101: {
  (tmp) = (executor_globals.uninitialized_zval_ptr);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15629
case 102: {
  (tmp) = *p;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15630
case 103: {
  *(tmp) = zval_used_for_init;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15631
case 104: {
  *tmp = zval_used_for_init;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15632
case 105: {
  _efree(((tmp)));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15633
case 106: {
  _efree(((tmp)->value.ht));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15634
case 107: {
  _efree(((tmp)->value.str.val));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15635
case 108: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 (key)->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15636
case 109: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, (key)->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15637
case 110: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, (key)->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15638
case 111: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &(tmp),
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15639
case 112: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15640
case 113: {
  _zend_list_addref((tmp)->value.lval);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15641
case 114: {
  _zend_list_delete((tmp)->value.lval);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15642
case 115: {
  _zval_copy_ctor_func((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15643
case 116: {
  _zval_dtor(((tmp)));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15644
case 117: {
  _zval_dtor_func((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15645
case 118: {
  _zval_ptr_dtor((&(tmp)));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15646
case 119: {
  convert_to_boolean((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15647
case 120: {
  exit(1);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15648
case 121: {
  gc_remove_zval_from_buffer((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15649
case 122: {
  gc_zval_check_possible_root(((tmp)));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15650
case 123: {
  gc_zval_possible_root((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15651
case 124: {
  i_zval_ptr_dtor((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15652
case 125: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15653
case 126: {
  if (!__is_neg("83-126", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args},
                0, 0, 6, "key", "target", "tmp", "p", "is_ref", "args") &&
      (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                      key->h, &tmp, sizeof(zval *), ((void *)0),
                                      (1 << 1)) == 0)) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15654
case 127: {
  if (!__is_neg("83-127", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args},
                0, 0, 6, "key", "target", "tmp", "p", "is_ref", "args"))
    if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                       key->h, &tmp, sizeof(zval *),
                                       ((void *)0), (1 << 1)) == 0) {
      zval_addref_p(tmp);
    }

  break;
}
#endif
#ifdef COMPILE_15655
case 128: {
  if ((_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                      key->h, &tmp, sizeof(zval *), ((void *)0),
                                      (1 << 1)) == 0) &&
      !__is_neg("83-128", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args},
                0, 0, 6, "key", "target", "tmp", "p", "is_ref", "args")) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15656
case 129: {
  if ((_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                      key->h, &tmp, sizeof(zval *), ((void *)0),
                                      (1 << 1)) == 0) ||
      __is_neg("83-129", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, 6, "key", "target", "tmp", "p", "is_ref", "args")) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15657
case 130: {
  if (__is_neg("83-130", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, 6, "key", "target", "tmp", "p", "is_ref", "args"))
    return 0;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15658
case 131: {
  if (__is_neg("83-131", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, 6, "key", "target", "tmp", "p", "is_ref", "args"))
    return 1;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15659
case 132: {
  if (__is_neg("83-132", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, 6, "key", "target", "tmp", "p", "is_ref", "args"))
    return 32;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15660
case 133: {
  if (__is_neg("83-133", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, 6, "key", "target", "tmp", "p", "is_ref", "args"))
    return 3;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15661
case 134: {
  if (__is_neg("83-134", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, 6, "key", "target", "tmp", "p", "is_ref", "args"))
    return 64;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15662
case 135: {
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15663
case 136: {
  is_ref = (**(p)).type & 64;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15664
case 137: {
  is_ref = (**p).type & 64;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15665
case 138: {
  memset(key, 0, sizeof(*(key)));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15666
case 139: {
  tmp = (executor_globals.uninitialized_zval_ptr);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15667
case 140: {
  tmp = *(p);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15668
case 141: {
  tmp = *p;
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15669
case 142: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15670
case 143: {
  zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15671
case 144: {
  zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15672
case 145: {
  zend_hash_destroy((tmp)->value.ht);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15673
case 146: {
  zend_rebuild_symbol_table();
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15674
case 147: {
  zval_addref_p((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15675
case 148: {
  zval_addref_p(*((p)));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15676
case 149: {
  zval_addref_p(tmp);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15677
case 150: {
  zval_set_isref_p((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15678
case 151: {
  zval_set_isref_p(*(((p))));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15679
case 152: {
  zval_set_isref_p(*((p)));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15680
case 153: {
  zval_set_isref_p(tmp);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15681
case 154: {
  zval_set_refcount_p((tmp), 0);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15682
case 155: {
  zval_set_refcount_p(tmp, 0);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15683
case 156: {
  zval_unset_isref_p((tmp));
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_15684
case 157: {
  zval_unset_isref_p(tmp);
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
}
// prophet generated patch
switch (__choose("__ID84")) {
case 0: {
  return 0;

  break;
}
#ifdef COMPILE_15685
case 1: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15686
case 2: {
  (tmp) = (executor_globals.uninitialized_zval_ptr);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15687
case 3: {
  (tmp) = *p;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15688
case 4: {
  *(tmp) = zval_used_for_init;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15689
case 5: {
  *tmp = zval_used_for_init;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15690
case 6: {
  _efree(((tmp)));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15691
case 7: {
  _efree(((tmp)->value.ht));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15692
case 8: {
  _efree(((tmp)->value.str.val));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15693
case 9: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 (key)->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15694
case 10: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, (key)->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15695
case 11: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, (key)->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15696
case 12: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &(tmp),
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15697
case 13: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15698
case 14: {
  _zend_list_addref((tmp)->value.lval);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15699
case 15: {
  _zend_list_delete((tmp)->value.lval);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15700
case 16: {
  _zval_copy_ctor_func((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15701
case 17: {
  _zval_dtor(((tmp)));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15702
case 18: {
  _zval_dtor_func((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15703
case 19: {
  _zval_ptr_dtor((&(tmp)));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15704
case 20: {
  convert_to_boolean((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15705
case 21: {
  exit(1);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15706
case 22: {
  gc_remove_zval_from_buffer((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15707
case 23: {
  gc_zval_check_possible_root(((tmp)));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15708
case 24: {
  gc_zval_possible_root((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15709
case 25: {
  i_zval_ptr_dtor((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15710
case 26: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
  return 0;

  break;
}
#endif
#ifdef COMPILE_15711
case 27: {
  if (!__is_neg("84-27", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
                0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
    return 0;

  break;
}
#endif
#ifdef COMPILE_15712
case 28: {
  if (__is_neg("84-28", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
    return 0;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15713
case 29: {
  if (__is_neg("84-29", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
    return 1;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15714
case 30: {
  if (__is_neg("84-30", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
    return 32;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15715
case 31: {
  if (__is_neg("84-31", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
    return 3;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15716
case 32: {
  if (__is_neg("84-32", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, 6, "tmp", "key", "target", "p", "is_ref", "args"))
    return 64;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15717
case 33: {
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }
  return 0;

  break;
}
#endif
#ifdef COMPILE_15718
case 34: {
  is_ref = (**(p)).type & 64;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15719
case 35: {
  is_ref = (**p).type & 64;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15720
case 36: {
  tmp = (executor_globals.uninitialized_zval_ptr);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15721
case 37: {
  tmp = *(p);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15722
case 38: {
  tmp = *p;
  return 0;

  break;
}
#endif
#ifdef COMPILE_15723
case 39: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  return 0;

  break;
}
#endif
#ifdef COMPILE_15724
case 40: {
  zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15725
case 41: {
  zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15726
case 42: {
  zend_hash_destroy((tmp)->value.ht);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15727
case 43: {
  zend_rebuild_symbol_table();
  return 0;

  break;
}
#endif
#ifdef COMPILE_15728
case 44: {
  zval_addref_p((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15729
case 45: {
  zval_addref_p(*((p)));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15730
case 46: {
  zval_addref_p(tmp);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15731
case 47: {
  zval_set_isref_p((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15732
case 48: {
  zval_set_isref_p(*(((p))));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15733
case 49: {
  zval_set_isref_p(*((p)));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15734
case 50: {
  zval_set_isref_p(tmp);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15735
case 51: {
  zval_set_refcount_p((tmp), 0);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15736
case 52: {
  zval_set_refcount_p(tmp, 0);
  return 0;

  break;
}
#endif
#ifdef COMPILE_15737
case 53: {
  zval_unset_isref_p((tmp));
  return 0;

  break;
}
#endif
#ifdef COMPILE_15738
case 54: {
  zval_unset_isref_p(tmp);
  return 0;

  break;
}
#endif
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
