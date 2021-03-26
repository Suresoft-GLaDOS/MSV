int __get_mutant();
int __is_neg(const char *, int, const int *, int, const char *, int,
             const void **, int, const double *, ...);
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
    // prophet generated patch
    switch (__choose("__ID0")) {
    case 0: {
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#ifdef COMPILE_ - 1478186224
    case 1: {
      (*(zvalue)).value.obj.handlers->del_ref(zvalue);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186223
    case 2: {
      (*zvalue).value.obj.handlers->del_ref((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186222
    case 3: {
      (*zvalue).value.obj.handlers->del_ref(zvalue);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186221
    case 4: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186220
    case 5: {
      _efree(((zvalue)));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186219
    case 6: {
      _efree(((zvalue)->value.ht));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186218
    case 7: {
      _efree(((zvalue)->value.str.val));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186217
    case 8: {
      _efree((zvalue->value.ht));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186216
    case 9: {
      _efree((zvalue->value.str.val));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186215
    case 10: {
      _zend_list_addref((zvalue)->value.lval);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186214
    case 11: {
      _zend_list_delete((zvalue)->value.lval);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186213
    case 12: {
      _zend_list_delete(zvalue->value.lval);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186212
    case 13: {
      _zval_copy_ctor_func((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186211
    case 14: {
      _zval_dtor(((zvalue)));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186210
    case 15: {
      _zval_dtor_func((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186209
    case 16: {
      _zval_ptr_dtor((&(zvalue)));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186208
    case 17: {
      convert_to_boolean((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186207
    case 18: {
      exit(1);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186206
    case 19: {
      gc_remove_zval_from_buffer((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186205
    case 20: {
      gc_zval_check_possible_root(((zvalue)));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186204
    case 21: {
      gc_zval_possible_root((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186203
    case 22: {
      i_zval_ptr_dtor((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186202
    case 23: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186201
    case 24: {
      if (!__is_neg("0-24", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue") &&
          (zvalue->value.str.val &&
           !(((zvalue->value.str.val) >=
              (compiler_globals.interned_strings_start)) &&
             ((zvalue->value.str.val) <
              (compiler_globals.interned_strings_end))))) {
        _efree((zvalue->value.str.val));
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478186200
    case 25: {
      if (!__is_neg("0-25", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
        if (zvalue->value.str.val &&
            !(((zvalue->value.str.val) >=
               (compiler_globals.interned_strings_start)) &&
              ((zvalue->value.str.val) <
               (compiler_globals.interned_strings_end)))) {
          _efree((zvalue->value.str.val));
        }

      break;
    }
#endif
#ifdef COMPILE_ - 1478186199
    case 26: {
      if (((zvalue->value.str.val) ||
           __is_neg("0-26", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0,
                    "zvalue")) &&
          !(((zvalue->value.str.val) >=
             (compiler_globals.interned_strings_start)) &&
            ((zvalue->value.str.val) <
             (compiler_globals.interned_strings_end)))) {
        _efree((zvalue->value.str.val));
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478186198
    case 27: {
      if ((zvalue->value.str.val &&
           !(((zvalue->value.str.val) >=
              (compiler_globals.interned_strings_start)) &&
             ((zvalue->value.str.val) <
              (compiler_globals.interned_strings_end)))) &&
          !__is_neg("0-27", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue")) {
        _efree((zvalue->value.str.val));
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478186197
    case 28: {
      if ((zvalue->value.str.val &&
           !(((zvalue->value.str.val) >=
              (compiler_globals.interned_strings_start)) &&
             ((zvalue->value.str.val) <
              (compiler_globals.interned_strings_end)))) ||
          __is_neg("0-28", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue")) {
        _efree((zvalue->value.str.val));
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478186196
    case 29: {
      if (__is_neg("0-29", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
        return;
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186195
    case 30: {
      if (zvalue->value.ht &&
          (zvalue->value.ht != &(executor_globals.symbol_table))) {
        zend_hash_destroy(zvalue->value.ht);
        _efree((zvalue->value.ht));
      }
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186194
    case 31: {
      if (zvalue->value.str.val &&
          !(((zvalue->value.str.val) >=
             (compiler_globals.interned_strings_start)) &&
            ((zvalue->value.str.val) <
             (compiler_globals.interned_strings_end)))) {
        _efree((zvalue->value.str.val));
      }
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186193
    case 32: {
      memset(&compiler_globals, 0, sizeof(*(&compiler_globals)));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186192
    case 33: {
      memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186191
    case 34: {
      memset(&zvalue->value.str, 0, sizeof(*(&zvalue->value.str)));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186190
    case 35: {
      memset(zvalue, 0, sizeof(*(zvalue)));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186189
    case 36: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186188
    case 37: {
      zend_hash_destroy((zvalue)->value.ht);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186187
    case 38: {
      zend_hash_destroy(zvalue->value.ht);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186186
    case 39: {
      zend_rebuild_symbol_table();
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186185
    case 40: {
      zval_addref_p((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186184
    case 41: {
      zval_set_isref_p((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186183
    case 42: {
      zval_set_refcount_p((zvalue), 0);
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478186182
    case 43: {
      zval_unset_isref_p((zvalue));
      STR_FREE_REL(zvalue->value.str.val);

      break;
    }
#endif
    }
    // prophet generated patch
    switch (__choose("__ID1")) {
    case 0: {
      break;

      break;
    }
#ifdef COMPILE_ - 1478186181
    case 1: {
      (*(zvalue)).value.obj.handlers->del_ref(zvalue);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186180
    case 2: {
      (*zvalue).value.obj.handlers->del_ref((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186179
    case 3: {
      (*zvalue).value.obj.handlers->del_ref(zvalue);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186178
    case 4: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186177
    case 5: {
      _efree(((zvalue)));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186176
    case 6: {
      _efree(((zvalue)->value.ht));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186175
    case 7: {
      _efree(((zvalue)->value.str.val));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186174
    case 8: {
      _efree((zvalue->value.ht));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186173
    case 9: {
      _efree((zvalue->value.str.val));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186172
    case 10: {
      _zend_list_addref((zvalue)->value.lval);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186171
    case 11: {
      _zend_list_delete((zvalue)->value.lval);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186170
    case 12: {
      _zend_list_delete(zvalue->value.lval);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186169
    case 13: {
      _zval_copy_ctor_func((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186168
    case 14: {
      _zval_dtor(((zvalue)));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186167
    case 15: {
      _zval_dtor_func((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186166
    case 16: {
      _zval_ptr_dtor((&(zvalue)));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186165
    case 17: {
      convert_to_boolean((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186164
    case 18: {
      exit(1);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186163
    case 19: {
      gc_remove_zval_from_buffer((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186162
    case 20: {
      gc_zval_check_possible_root(((zvalue)));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186161
    case 21: {
      gc_zval_possible_root((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186160
    case 22: {
      i_zval_ptr_dtor((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186159
    case 23: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186158
    case 24: {
      if (!__is_neg("1-24", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
        break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186157
    case 25: {
      if (__is_neg("1-25", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
        return;
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186156
    case 26: {
      if (zvalue->value.ht &&
          (zvalue->value.ht != &(executor_globals.symbol_table))) {
        zend_hash_destroy(zvalue->value.ht);
        _efree((zvalue->value.ht));
      }
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186155
    case 27: {
      if (zvalue->value.str.val &&
          !(((zvalue->value.str.val) >=
             (compiler_globals.interned_strings_start)) &&
            ((zvalue->value.str.val) <
             (compiler_globals.interned_strings_end)))) {
        _efree((zvalue->value.str.val));
      }
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186154
    case 28: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186153
    case 29: {
      zend_hash_destroy((zvalue)->value.ht);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186152
    case 30: {
      zend_hash_destroy(zvalue->value.ht);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186151
    case 31: {
      zend_rebuild_symbol_table();
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186150
    case 32: {
      zval_addref_p((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186149
    case 33: {
      zval_set_isref_p((zvalue));
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186148
    case 34: {
      zval_set_refcount_p((zvalue), 0);
      break;

      break;
    }
#endif
#ifdef COMPILE_ - 1478186147
    case 35: {
      zval_unset_isref_p((zvalue));
      break;

      break;
    }
#endif
    }
  case IS_ARRAY:
  case IS_CONSTANT_ARRAY: {
    TSRMLS_FETCH();

    if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
      // prophet generated patch
      switch (__choose("__ID2")) {
      case 0: {
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#ifdef COMPILE_ - 1478186146
      case 1: {
        (*(zvalue)).value.obj.handlers->del_ref(zvalue);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186145
      case 2: {
        (*zvalue).value.obj.handlers->del_ref((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186144
      case 3: {
        (*zvalue).value.obj.handlers->del_ref(zvalue);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186143
      case 4: {
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186142
      case 5: {
        _efree(((zvalue)));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186141
      case 6: {
        _efree(((zvalue)->value.ht));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186140
      case 7: {
        _efree(((zvalue)->value.str.val));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186139
      case 8: {
        _efree((zvalue->value.ht));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186138
      case 9: {
        _efree((zvalue->value.str.val));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186137
      case 10: {
        _zend_list_addref((zvalue)->value.lval);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186136
      case 11: {
        _zend_list_delete((zvalue)->value.lval);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186135
      case 12: {
        _zend_list_delete(zvalue->value.lval);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186134
      case 13: {
        _zval_copy_ctor_func((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186133
      case 14: {
        _zval_dtor(((zvalue)));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186132
      case 15: {
        _zval_dtor_func((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186131
      case 16: {
        _zval_ptr_dtor((&(zvalue)));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186130
      case 17: {
        convert_to_boolean((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186129
      case 18: {
        exit(1);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186128
      case 19: {
        gc_remove_zval_from_buffer((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186127
      case 20: {
        gc_zval_check_possible_root(((zvalue)));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186126
      case 21: {
        gc_zval_possible_root((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186125
      case 22: {
        i_zval_ptr_dtor((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186124
      case 23: {
        if (!(executor_globals.active_symbol_table)) {
          zend_rebuild_symbol_table();
        }
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186123
      case 24: {
        if (!__is_neg("2-24", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
          zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186122
      case 25: {
        if (__is_neg("2-25", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
          return;
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186121
      case 26: {
        if (zvalue->value.ht &&
            (zvalue->value.ht != &(executor_globals.symbol_table))) {
          zend_hash_destroy(zvalue->value.ht);
          _efree((zvalue->value.ht));
        }
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186120
      case 27: {
        if (zvalue->value.str.val &&
            !(((zvalue->value.str.val) >=
               (compiler_globals.interned_strings_start)) &&
              ((zvalue->value.str.val) <
               (compiler_globals.interned_strings_end)))) {
          _efree((zvalue->value.str.val));
        }
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186119
      case 28: {
        memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186118
      case 29: {
        memset(zvalue, 0, sizeof(*(zvalue)));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186117
      case 30: {
        zend_destroy_rsrc_list(zvalue->value.ht) break;
      }
#endif
#ifdef COMPILE_ - 1478186116
      case 31: {
        zend_error((1 << 3L), "A non well formed numeric value encountered");
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186115
      case 32: {
        zend_hash_clean(zvalue->value.ht) break;
      }
#endif
#ifdef COMPILE_ - 1478186114
      case 33: {
        zend_hash_destroy((zvalue)->value.ht) break;
      }
#endif
#ifdef COMPILE_ - 1478186113
      case 34: {
        zend_hash_destroy((zvalue)->value.ht);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186112
      case 35: {
        zend_hash_destroy(zvalue->value.ht);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186111
      case 36: {
        zend_hash_graceful_destroy(zvalue->value.ht) break;
      }
#endif
#ifdef COMPILE_ - 1478186110
      case 37: {
        zend_hash_graceful_reverse_destroy(zvalue->value.ht) break;
      }
#endif
#ifdef COMPILE_ - 1478186109
      case 38: {
        zend_hash_rehash(zvalue->value.ht) break;
      }
#endif
#ifdef COMPILE_ - 1478186108
      case 39: {
        zend_rebuild_symbol_table();
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186107
      case 40: {
        zend_reset_all_cv(zvalue->value.ht) break;
      }
#endif
#ifdef COMPILE_ - 1478186106
      case 41: {
        zval_addref_p((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186105
      case 42: {
        zval_set_isref_p((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186104
      case 43: {
        zval_set_refcount_p((zvalue), 0);
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
#ifdef COMPILE_ - 1478186103
      case 44: {
        zval_unset_isref_p((zvalue));
        zend_hash_destroy(zvalue->value.ht);

        break;
      }
#endif
      }
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
switch (__choose("__ID3")) {
case 0: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#ifdef COMPILE_ - 1478186102
case 1: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    (*(zvalue)).value.obj.handlers->del_ref(zvalue);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186101
case 2: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    (*zvalue).value.obj.handlers->del_ref((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186100
case 3: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    (*zvalue).value.obj.handlers->del_ref(zvalue);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186099
case 4: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186098
case 5: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree(((zvalue)));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186097
case 6: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree(((zvalue)->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186096
case 7: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree(((zvalue)->value.ht));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186095
case 8: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree(((zvalue)->value.str.val));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186094
case 9: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.ht));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186093
case 10: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.str.val));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186092
case 11: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _zend_list_addref((zvalue)->value.lval);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186091
case 12: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _zend_list_delete((zvalue)->value.lval);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186090
case 13: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _zend_list_delete(zvalue->value.lval);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186089
case 14: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _zend_mem_block_size((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186088
case 15: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _zval_copy_ctor_func((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186087
case 16: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _zval_dtor(((zvalue)));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186086
case 17: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _zval_dtor_func((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186085
case 18: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _zval_ptr_dtor((&(zvalue)));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186084
case 19: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    convert_to_boolean((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186083
case 20: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    dlclose((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186082
case 21: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    exit(1);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186081
case 22: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    free((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186080
case 23: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    gc_remove_zval_from_buffer((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186079
case 24: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    gc_zval_check_possible_root(((zvalue)));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186078
case 25: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    gc_zval_possible_root((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186077
case 26: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    i_zval_ptr_dtor((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186076
case 27: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186075
case 28: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    if (!__is_neg("3-28", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      _efree((zvalue->value.ht));
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186074
case 29: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    if (__is_neg("3-29", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      return;
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186073
case 30: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    if (zvalue->value.ht &&
        (zvalue->value.ht != &(executor_globals.symbol_table))) {
      zend_hash_destroy(zvalue->value.ht);
      _efree((zvalue->value.ht));
    }
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186072
case 31: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    if (zvalue->value.str.val &&
        !(((zvalue->value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          ((zvalue->value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      _efree((zvalue->value.str.val));
    }
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186071
case 32: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    list_entry_destructor((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186070
case 33: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186069
case 34: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    memset(zvalue, 0, sizeof(*(zvalue)));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186068
case 35: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    plist_entry_destructor((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186067
case 36: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186066
case 37: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zend_hash_destroy((zvalue)->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186065
case 38: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186064
case 39: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zend_rebuild_symbol_table();
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186063
case 40: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zend_vm_stack_free((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186062
case 41: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zend_vm_stack_free_int((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186061
case 42: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zend_vm_stack_push((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186060
case 43: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zend_vm_stack_push_nocheck((zvalue->value.ht))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186059
case 44: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zval_addref_p((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186058
case 45: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zval_set_isref_p((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186057
case 46: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zval_set_refcount_p((zvalue), 0);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186056
case 47: {
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    zval_unset_isref_p((zvalue));
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186055
case 48: {
  (*(zvalue)).value.obj.handlers->del_ref(zvalue);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186054
case 49: {
  (*zvalue).value.obj.handlers->del_ref((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186053
case 50: {
  (*zvalue).value.obj.handlers->del_ref(zvalue);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186052
case 51: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186051
case 52: {
  _efree(((zvalue)));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186050
case 53: {
  _efree(((zvalue)->value.ht));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186049
case 54: {
  _efree(((zvalue)->value.str.val));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186048
case 55: {
  _efree((zvalue->value.ht));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186047
case 56: {
  _efree((zvalue->value.str.val));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186046
case 57: {
  _zend_list_addref((zvalue)->value.lval);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186045
case 58: {
  _zend_list_delete((zvalue)->value.lval);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186044
case 59: {
  _zend_list_delete(zvalue->value.lval);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186043
case 60: {
  _zval_copy_ctor_func((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186042
case 61: {
  _zval_dtor(((zvalue)));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186041
case 62: {
  _zval_dtor_func((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186040
case 63: {
  _zval_ptr_dtor((&(zvalue)));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186039
case 64: {
  convert_to_boolean((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186038
case 65: {
  exit(1);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186037
case 66: {
  gc_remove_zval_from_buffer((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186036
case 67: {
  gc_zval_check_possible_root(((zvalue)));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186035
case 68: {
  gc_zval_possible_root((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186034
case 69: {
  i_zval_ptr_dtor((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186033
case 70: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186032
case 71: {
  if (!__is_neg("3-71", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue") &&
      (zvalue->value.ht &&
       (zvalue->value.ht != &(executor_globals.symbol_table)))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.ht));
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186031
case 72: {
  if (!__is_neg("3-72", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    if (zvalue->value.ht &&
        (zvalue->value.ht != &(executor_globals.symbol_table))) {
      zend_hash_destroy(zvalue->value.ht);
      _efree((zvalue->value.ht));
    }

  break;
}
#endif
#ifdef COMPILE_ - 1478186030
case 73: {
  if (((zvalue->value.ht) ||
       __is_neg("3-73", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue")) &&
      (zvalue->value.ht != &(executor_globals.symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.ht));
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186029
case 74: {
  if ((zvalue->value.ht &&
       (zvalue->value.ht != &(executor_globals.symbol_table))) &&
      !__is_neg("3-74", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue")) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.ht));
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186028
case 75: {
  if ((zvalue->value.ht &&
       (zvalue->value.ht != &(executor_globals.symbol_table))) ||
      __is_neg("3-75", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue")) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.ht));
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186027
case 76: {
  if (__is_neg("3-76", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    return;
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186026
case 77: {
  if (zvalue->value.ht &&
      (zvalue->value.ht != &(executor_globals.symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.ht));
  }
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186025
case 78: {
  if (zvalue->value.str.val &&
      !(((zvalue->value.str.val) >=
         (compiler_globals.interned_strings_start)) &&
        ((zvalue->value.str.val) < (compiler_globals.interned_strings_end)))) {
    _efree((zvalue->value.str.val));
  }
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186024
case 79: {
  memset(&executor_globals, 0, sizeof(*(&executor_globals)));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186023
case 80: {
  memset(&executor_globals.symbol_table, 0,
         sizeof(*(&executor_globals.symbol_table)));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186022
case 81: {
  memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186021
case 82: {
  memset(zvalue, 0, sizeof(*(zvalue)));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186020
case 83: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186019
case 84: {
  zend_hash_destroy((zvalue)->value.ht);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186018
case 85: {
  zend_hash_destroy(zvalue->value.ht);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186017
case 86: {
  zend_rebuild_symbol_table();
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186016
case 87: {
  zval_addref_p((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186015
case 88: {
  zval_set_isref_p((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186014
case 89: {
  zval_set_refcount_p((zvalue), 0);
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478186013
case 90: {
  zval_unset_isref_p((zvalue));
  if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    FREE_HASHTABLE(zvalue->value.ht);
  }

  break;
}
#endif
}
}
// prophet generated patch
switch (__choose("__ID4")) {
case 0: {
  break;

  break;
}
#ifdef COMPILE_ - 1478186012
case 1: {
  (*(zvalue)).value.obj.handlers->del_ref(zvalue);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186011
case 2: {
  (*zvalue).value.obj.handlers->del_ref((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186010
case 3: {
  (*zvalue).value.obj.handlers->del_ref(zvalue);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186009
case 4: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186008
case 5: {
  _efree(((zvalue)));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186007
case 6: {
  _efree(((zvalue)->value.ht));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186006
case 7: {
  _efree(((zvalue)->value.str.val));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186005
case 8: {
  _efree((zvalue->value.ht));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186004
case 9: {
  _efree((zvalue->value.str.val));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186003
case 10: {
  _zend_list_addref((zvalue)->value.lval);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186002
case 11: {
  _zend_list_delete((zvalue)->value.lval);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186001
case 12: {
  _zend_list_delete(zvalue->value.lval);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478186000
case 13: {
  _zval_copy_ctor_func((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185999
case 14: {
  _zval_dtor(((zvalue)));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185998
case 15: {
  _zval_dtor_func((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185997
case 16: {
  _zval_ptr_dtor((&(zvalue)));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185996
case 17: {
  convert_to_boolean((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185995
case 18: {
  exit(1);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185994
case 19: {
  gc_remove_zval_from_buffer((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185993
case 20: {
  gc_zval_check_possible_root(((zvalue)));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185992
case 21: {
  gc_zval_possible_root((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185991
case 22: {
  i_zval_ptr_dtor((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185990
case 23: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185989
case 24: {
  if (!__is_neg("4-24", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185988
case 25: {
  if (__is_neg("4-25", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    return;
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185987
case 26: {
  if (zvalue->value.ht &&
      (zvalue->value.ht != &(executor_globals.symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.ht));
  }
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185986
case 27: {
  if (zvalue->value.str.val &&
      !(((zvalue->value.str.val) >=
         (compiler_globals.interned_strings_start)) &&
        ((zvalue->value.str.val) < (compiler_globals.interned_strings_end)))) {
    _efree((zvalue->value.str.val));
  }
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185985
case 28: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185984
case 29: {
  zend_hash_destroy((zvalue)->value.ht);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185983
case 30: {
  zend_hash_destroy(zvalue->value.ht);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185982
case 31: {
  zend_rebuild_symbol_table();
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185981
case 32: {
  zval_addref_p((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185980
case 33: {
  zval_set_isref_p((zvalue));
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185979
case 34: {
  zval_set_refcount_p((zvalue), 0);
  break;

  break;
}
#endif
#ifdef COMPILE_ - 1478185978
case 35: {
  zval_unset_isref_p((zvalue));
  break;

  break;
}
#endif
}
case IS_OBJECT: {
  TSRMLS_FETCH();

  // prophet generated patch
  switch (__choose("__ID5")) {
  case 0: {
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#ifdef COMPILE_ - 1478185977
  case 1: {
    (*(zvalue)).value.obj.handlers->del_ref(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185976
  case 2: {
    (*(zvalue)).value.obj.handlers->del_ref(zvalue);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185975
  case 3: {
    (*zvalue).value.obj.handlers->del_ref((zvalue)) break;
  }
#endif
#ifdef COMPILE_ - 1478185974
  case 4: {
    (*zvalue).value.obj.handlers->del_ref((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185973
  case 5: {
    (*zvalue).value.obj.handlers->del_ref(zvalue);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185972
  case 6: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185971
  case 7: {
    _convert_to_string(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185970
  case 8: {
    _efree(((zvalue)));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185969
  case 9: {
    _efree(((zvalue)->value.ht));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185968
  case 10: {
    _efree(((zvalue)->value.str.val));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185967
  case 11: {
    _efree((zvalue->value.ht));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185966
  case 12: {
    _efree((zvalue->value.str.val));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185965
  case 13: {
    _object_init(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185964
  case 14: {
    _zend_list_addref((zvalue)->value.lval);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185963
  case 15: {
    _zend_list_delete((zvalue)->value.lval);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185962
  case 16: {
    _zend_list_delete(zvalue->value.lval);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185961
  case 17: {
    _zval_copy_ctor(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185960
  case 18: {
    _zval_copy_ctor_func((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185959
  case 19: {
    _zval_copy_ctor_func(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185958
  case 20: {
    _zval_dtor(((zvalue)));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185957
  case 21: {
    _zval_dtor(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185956
  case 22: {
    _zval_dtor_func((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185955
  case 23: {
    _zval_dtor_func(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185954
  case 24: {
    _zval_dtor_wrapper(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185953
  case 25: {
    _zval_internal_dtor(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185952
  case 26: {
    _zval_ptr_dtor((&(zvalue)));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185951
  case 27: {
    add_next_index_null(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185950
  case 28: {
    convert_scalar_to_number(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185949
  case 29: {
    convert_to_array(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185948
  case 30: {
    convert_to_boolean((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185947
  case 31: {
    convert_to_boolean(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185946
  case 32: {
    convert_to_double(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185945
  case 33: {
    convert_to_long(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185944
  case 34: {
    convert_to_null(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185943
  case 35: {
    convert_to_object(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185942
  case 36: {
    decrement_function(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185941
  case 37: {
    exit(1);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185940
  case 38: {
    gc_remove_zval_from_buffer((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185939
  case 39: {
    gc_remove_zval_from_buffer(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185938
  case 40: {
    gc_zobj_possible_root(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185937
  case 41: {
    gc_zval_check_possible_root(((zvalue)));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185936
  case 42: {
    gc_zval_check_possible_root(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185935
  case 43: {
    gc_zval_possible_root((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185934
  case 44: {
    gc_zval_possible_root(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185933
  case 45: {
    i_zend_is_true(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185932
  case 46: {
    i_zval_ptr_dtor((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185931
  case 47: {
    i_zval_ptr_dtor(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185930
  case 48: {
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185929
  case 49: {
    if (!__is_neg("5-49", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      (*zvalue).value.obj.handlers->del_ref(zvalue);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185928
  case 50: {
    if (__is_neg("5-50", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      return;
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185927
  case 51: {
    if (zvalue->value.ht &&
        (zvalue->value.ht != &(executor_globals.symbol_table))) {
      zend_hash_destroy(zvalue->value.ht);
      _efree((zvalue->value.ht));
    }
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185926
  case 52: {
    if (zvalue->value.str.val &&
        !(((zvalue->value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          ((zvalue->value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      _efree((zvalue->value.str.val));
    }
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185925
  case 53: {
    increment_function(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185924
  case 54: {
    lex_scan(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185923
  case 55: {
    memset(&(*zvalue), 0, sizeof(*(&(*zvalue))));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185922
  case 56: {
    memset(&(*zvalue).value, 0, sizeof(*(&(*zvalue).value)));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185921
  case 57: {
    memset(&(*zvalue).value.obj, 0, sizeof(*(&(*zvalue).value.obj)));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185920
  case 58: {
    memset((*zvalue).value.obj.handlers, 0,
           sizeof(*((*zvalue).value.obj.handlers)));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185919
  case 59: {
    memset(zvalue, 0, sizeof(*(zvalue)));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185918
  case 60: {
    safe_free_zval_ptr_rel(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185917
  case 61: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185916
  case 62: {
    zend_hash_destroy((zvalue)->value.ht);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185915
  case 63: {
    zend_hash_destroy(zvalue->value.ht);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185914
  case 64: {
    zend_is_true(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185913
  case 65: {
    zend_locale_sprintf_double(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185912
  case 66: {
    zend_object_store_ctor_failed(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185911
  case 67: {
    zend_objects_clone_obj(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185910
  case 68: {
    zend_objects_store_add_ref(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185909
  case 69: {
    zend_objects_store_clone_obj(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185908
  case 70: {
    zend_objects_store_del_ref(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185907
  case 71: {
    zend_objects_store_get_refcount(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185906
  case 72: {
    zend_print_flat_zval_r(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185905
  case 73: {
    zend_print_variable(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185904
  case 74: {
    zend_rebuild_symbol_table();
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185903
  case 75: {
    zend_std_get_constructor(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185902
  case 76: {
    zend_std_get_properties(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185901
  case 77: {
    zval_addref_p((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185900
  case 78: {
    zval_addref_p(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185899
  case 79: {
    zval_delref_p(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185898
  case 80: {
    zval_is_true(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185897
  case 81: {
    zval_isref_p(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185896
  case 82: {
    zval_refcount_p(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185895
  case 83: {
    zval_set_isref_p((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185894
  case 84: {
    zval_set_isref_p(zvalue) break;
  }
#endif
#ifdef COMPILE_ - 1478185893
  case 85: {
    zval_set_refcount_p((zvalue), 0);
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185892
  case 86: {
    zval_unset_isref_p((zvalue));
    Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185891
  case 87: {
    zval_unset_isref_p(zvalue) break;
  }
#endif
  }
} break;
case IS_RESOURCE: {
  TSRMLS_FETCH();

  /* destroy resource */
  // prophet generated patch
  switch (__choose("__ID6")) {
  case 0: {
    zend_list_delete(zvalue->value.lval);

    break;
  }
#ifdef COMPILE_ - 1478185890
  case 1: {
    (*(zvalue)).value.obj.handlers->del_ref(zvalue);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185889
  case 2: {
    (*zvalue).value.obj.handlers->del_ref((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185888
  case 3: {
    (*zvalue).value.obj.handlers->del_ref(zvalue);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185887
  case 4: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185886
  case 5: {
    _Exit(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185885
  case 6: {
    _efree(((zvalue)));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185884
  case 7: {
    _efree(((zvalue)->value.ht));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185883
  case 8: {
    _efree(((zvalue)->value.str.val));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185882
  case 9: {
    _efree((zvalue->value.ht));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185881
  case 10: {
    _efree((zvalue->value.str.val));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185880
  case 11: {
    _zend_list_addref((zvalue)->value.lval);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185879
  case 12: {
    _zend_list_addref(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185878
  case 13: {
    _zend_list_delete((zvalue)->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185877
  case 14: {
    _zend_list_delete((zvalue)->value.lval);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185876
  case 15: {
    _zend_list_delete(zvalue->value.lval);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185875
  case 16: {
    _zval_copy_ctor_func((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185874
  case 17: {
    _zval_dtor(((zvalue)));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185873
  case 18: {
    _zval_dtor_func((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185872
  case 19: {
    _zval_ptr_dtor((&(zvalue)));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185871
  case 20: {
    abs(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185870
  case 21: {
    clean_module_constants(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185869
  case 22: {
    convert_to_boolean((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185868
  case 23: {
    exit(1);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185867
  case 24: {
    exit(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185866
  case 25: {
    ffs(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185865
  case 26: {
    gc_remove_zval_from_buffer((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185864
  case 27: {
    gc_zval_check_possible_root(((zvalue)));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185863
  case 28: {
    gc_zval_possible_root((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185862
  case 29: {
    get_binary_op(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185861
  case 30: {
    get_unary_op(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185860
  case 31: {
    i_zval_ptr_dtor((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185859
  case 32: {
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185858
  case 33: {
    if (!__is_neg("6-33", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      _zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185857
  case 34: {
    if (__is_neg("6-34", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      return;
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185856
  case 35: {
    if (zvalue->value.ht &&
        (zvalue->value.ht != &(executor_globals.symbol_table))) {
      zend_hash_destroy(zvalue->value.ht);
      _efree((zvalue->value.ht));
    }
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185855
  case 36: {
    if (zvalue->value.str.val &&
        !(((zvalue->value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          ((zvalue->value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      _efree((zvalue->value.str.val));
    }
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185854
  case 37: {
    memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185853
  case 38: {
    memset(zvalue, 0, sizeof(*(zvalue)));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185852
  case 39: {
    multi_convert_to_double_ex(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185851
  case 40: {
    multi_convert_to_long_ex(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185850
  case 41: {
    multi_convert_to_string_ex(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185849
  case 42: {
    putchar(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185848
  case 43: {
    putchar_unlocked(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185847
  case 44: {
    quick_exit(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185846
  case 45: {
    strerror(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185845
  case 46: {
    strsignal(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185844
  case 47: {
    zend_clean_module_rsrc_dtors(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185843
  case 48: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185842
  case 49: {
    zend_get_parameters_ex(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185841
  case 50: {
    zend_get_type_by_const(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185840
  case 51: {
    zend_hash_destroy((zvalue)->value.ht);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185839
  case 52: {
    zend_hash_destroy(zvalue->value.ht);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185838
  case 53: {
    zend_memory_peak_usage(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185837
  case 54: {
    zend_memory_usage(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185836
  case 55: {
    zend_rebuild_symbol_table();
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185835
  case 56: {
    zend_rsrc_list_get_rsrc_type(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185834
  case 57: {
    zend_timeout(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185833
  case 58: {
    zend_vm_stack_extend(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185832
  case 59: {
    zend_vm_stack_get_arg(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185831
  case 60: {
    zend_vm_stack_new_page(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185830
  case 61: {
    zend_vm_stack_push_args(zvalue->value.lval) break;
  }
#endif
#ifdef COMPILE_ - 1478185829
  case 62: {
    zval_addref_p((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185828
  case 63: {
    zval_set_isref_p((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185827
  case 64: {
    zval_set_refcount_p((zvalue), 0);
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185826
  case 65: {
    zval_unset_isref_p((zvalue));
    zend_list_delete(zvalue->value.lval);

    break;
  }
#endif
  }
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
switch (__choose("__ID7")) {
case 0: {
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

  break;
}
#ifdef COMPILE_ - 1478185825
case 1: {
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
  }
    (*(zvalue)).value.obj.handlers->del_ref(zvalue);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185824
case 2: {
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
  }
    (*zvalue).value.obj.handlers->del_ref((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185823
case 3: {
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
  }
    (*zvalue).value.obj.handlers->del_ref(zvalue);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185822
case 4: {
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
  }
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185821
case 5: {
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
  }
    _efree(((zvalue)));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185820
case 6: {
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
  }
    _efree(((zvalue)->value.ht));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185819
case 7: {
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
  }
    _efree(((zvalue)->value.str.val));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185818
case 8: {
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
  }
    _efree((zvalue->value.ht));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185817
case 9: {
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
  }
    _efree((zvalue->value.str.val));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185816
case 10: {
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
  }
    _zend_list_addref((zvalue)->value.lval);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185815
case 11: {
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
  }
    _zend_list_delete((zvalue)->value.lval);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185814
case 12: {
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
  }
    _zend_list_delete(zvalue->value.lval);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185813
case 13: {
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
  }
    _zval_copy_ctor_func((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185812
case 14: {
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
  }
    _zval_dtor(((zvalue)));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185811
case 15: {
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
  }
    _zval_dtor_func((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185810
case 16: {
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
  }
    _zval_ptr_dtor((&(zvalue)));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185809
case 17: {
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
  }
    convert_to_boolean((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185808
case 18: {
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
  }
    exit(1);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185807
case 19: {
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
  }
    gc_remove_zval_from_buffer((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185806
case 20: {
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
  }
    gc_zval_check_possible_root(((zvalue)));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185805
case 21: {
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
  }
    gc_zval_possible_root((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185804
case 22: {
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
  }
    i_zval_ptr_dtor((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185803
case 23: {
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
  }
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185802
case 24: {
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
  }
    if (!__is_neg("7-24", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185801
case 25: {
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
  }
    if (__is_neg("7-25", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      return;
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185800
case 26: {
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
  }
    if (zvalue->value.ht &&
        (zvalue->value.ht != &(executor_globals.symbol_table))) {
      zend_hash_destroy(zvalue->value.ht);
      _efree((zvalue->value.ht));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185799
case 27: {
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
  }
    if (zvalue->value.str.val &&
        !(((zvalue->value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          ((zvalue->value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      _efree((zvalue->value.str.val));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185798
case 28: {
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
  }
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185797
case 29: {
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
  }
    zend_hash_destroy((zvalue)->value.ht);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185796
case 30: {
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
  }
    zend_hash_destroy(zvalue->value.ht);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185795
case 31: {
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
  }
    zend_rebuild_symbol_table();
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185794
case 32: {
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
  }
    zval_addref_p((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185793
case 33: {
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
  }
    zval_set_isref_p((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185792
case 34: {
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
  }
    zval_set_refcount_p((zvalue), 0);
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185791
case 35: {
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
  }
    zval_unset_isref_p((zvalue));
    break;
  case IS_LONG:
  case IS_DOUBLE:
  case IS_BOOL:
  case IS_NULL:
  default:
    return;
    break;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478185790
case 36: {
  (*(zvalue)).value.obj.handlers->del_ref(zvalue);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185789
case 37: {
  (*zvalue).value.obj.handlers->del_ref((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185788
case 38: {
  (*zvalue).value.obj.handlers->del_ref(zvalue);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185787
case 39: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185786
case 40: {
  _efree(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185785
case 41: {
  _efree(((zvalue)->value.ht));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185784
case 42: {
  _efree(((zvalue)->value.str.val));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185783
case 43: {
  _efree((zvalue->value.ht));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185782
case 44: {
  _efree((zvalue->value.str.val));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185781
case 45: {
  _zend_list_addref((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185780
case 46: {
  _zend_list_delete((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185779
case 47: {
  _zend_list_delete(zvalue->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185778
case 48: {
  _zval_copy_ctor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185777
case 49: {
  _zval_dtor(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185776
case 50: {
  _zval_dtor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185775
case 51: {
  _zval_ptr_dtor((&(zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185774
case 52: {
  convert_to_boolean((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185773
case 53: {
  exit(1);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185772
case 54: {
  gc_remove_zval_from_buffer((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185771
case 55: {
  gc_zval_check_possible_root(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185770
case 56: {
  gc_zval_possible_root((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185769
case 57: {
  i_zval_ptr_dtor((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185768
case 58: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185767
case 59: {
  if (!__is_neg("7-59", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    switch ((*zvalue).type & 15) {
    case 6:
    case 8:
    case 10:;
      if (zvalue->value.str.val &&
          !(((zvalue->value.str.val) >=
             (compiler_globals.interned_strings_start)) &&
            ((zvalue->value.str.val) <
             (compiler_globals.interned_strings_end)))) {
        _efree((zvalue->value.str.val));
      };
      break;
    case 4:
    case 9: {
      ;
      if (zvalue->value.ht &&
          (zvalue->value.ht != &(executor_globals.symbol_table))) {
        zend_hash_destroy(zvalue->value.ht);
        _efree((zvalue->value.ht));
      }
    } break;
    case 5: {
      ;
      (*zvalue).value.obj.handlers->del_ref(zvalue);
    } break;
    case 7: {
      ;
      _zend_list_delete(zvalue->value.lval);
    } break;
    case 1:
    case 2:
    case 3:
    case 0:
    default:
      return;
      break;
    }

  break;
}
#endif
#ifdef COMPILE_ - 1478185766
case 60: {
  if (__is_neg("7-60", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    return;
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185765
case 61: {
  if (zvalue->value.ht &&
      (zvalue->value.ht != &(executor_globals.symbol_table))) {
    zend_hash_destroy(zvalue->value.ht);
    _efree((zvalue->value.ht));
  }
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185764
case 62: {
  if (zvalue->value.str.val &&
      !(((zvalue->value.str.val) >=
         (compiler_globals.interned_strings_start)) &&
        ((zvalue->value.str.val) < (compiler_globals.interned_strings_end)))) {
    _efree((zvalue->value.str.val));
  }
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185763
case 63: {
  memset(&(*zvalue), 0, sizeof(*(&(*zvalue))));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185762
case 64: {
  memset(&(*zvalue).value, 0, sizeof(*(&(*zvalue).value)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185761
case 65: {
  memset(&(*zvalue).value.obj, 0, sizeof(*(&(*zvalue).value.obj)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185760
case 66: {
  memset(&compiler_globals, 0, sizeof(*(&compiler_globals)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185759
case 67: {
  memset(&executor_globals, 0, sizeof(*(&executor_globals)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185758
case 68: {
  memset(&executor_globals.symbol_table, 0,
         sizeof(*(&executor_globals.symbol_table)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185757
case 69: {
  memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185756
case 70: {
  memset(&zvalue->value.str, 0, sizeof(*(&zvalue->value.str)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185755
case 71: {
  memset((*zvalue).value.obj.handlers, 0,
         sizeof(*((*zvalue).value.obj.handlers)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185754
case 72: {
  memset(zvalue, 0, sizeof(*(zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185753
case 73: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185752
case 74: {
  zend_hash_destroy((zvalue)->value.ht);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185751
case 75: {
  zend_hash_destroy(zvalue->value.ht);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185750
case 76: {
  zend_rebuild_symbol_table();
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185749
case 77: {
  zval_addref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185748
case 78: {
  zval_set_isref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185747
case 79: {
  zval_set_refcount_p((zvalue), 0);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185746
case 80: {
  zval_unset_isref_p((zvalue));
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

  break;
}
#endif
}
}

ZEND_API void _zval_internal_dtor(zval *zvalue ZEND_FILE_LINE_DC) {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    // prophet generated patch
    switch (__choose("__ID8")) {
    case 0: {
      str_free(zvalue->value.str.val);

      break;
    }
#ifdef COMPILE_ - 1478185745
    case 1: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185744
    case 2: {
      _efree(((zvalue)));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185743
    case 3: {
      _efree(((zvalue)->value.ht));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185742
    case 4: {
      _efree(((zvalue)->value.str.val));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185741
    case 5: {
      _zend_list_addref((zvalue)->value.lval);
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185740
    case 6: {
      _zend_list_delete((zvalue)->value.lval);
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185739
    case 7: {
      _zval_copy_ctor_func((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185738
    case 8: {
      _zval_dtor(((zvalue)));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185737
    case 9: {
      _zval_dtor_func((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185736
    case 10: {
      _zval_ptr_dtor((&(zvalue)));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185735
    case 11: {
      convert_to_boolean((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185734
    case 12: {
      exit(1);
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185733
    case 13: {
      gc_remove_zval_from_buffer((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185732
    case 14: {
      gc_zval_check_possible_root(((zvalue)));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185731
    case 15: {
      gc_zval_possible_root((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185730
    case 16: {
      i_zval_ptr_dtor((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185729
    case 17: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185728
    case 18: {
      if (!__is_neg("8-18", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
        do {
          if (!(((zvalue->value.str.val) >=
                 (compiler_globals.interned_strings_start)) &&
                ((zvalue->value.str.val) <
                 (compiler_globals.interned_strings_end)))) {
            free(zvalue->value.str.val);
          }
        } while (0);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185727
    case 19: {
      if (__is_neg("8-19", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
        return;
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185726
    case 20: {
      memset(&compiler_globals, 0, sizeof(*(&compiler_globals)));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185725
    case 21: {
      memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185724
    case 22: {
      memset(&zvalue->value.str, 0, sizeof(*(&zvalue->value.str)));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185723
    case 23: {
      memset(zvalue, 0, sizeof(*(zvalue)));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185722
    case 24: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185721
    case 25: {
      zend_hash_destroy((zvalue)->value.ht);
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185720
    case 26: {
      zend_rebuild_symbol_table();
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185719
    case 27: {
      zval_addref_p((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185718
    case 28: {
      zval_set_isref_p((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185717
    case 29: {
      zval_set_refcount_p((zvalue), 0);
      str_free(zvalue->value.str.val);

      break;
    }
#endif
#ifdef COMPILE_ - 1478185716
    case 30: {
      zval_unset_isref_p((zvalue));
      str_free(zvalue->value.str.val);

      break;
    }
#endif
    }
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
switch (__choose("__ID9")) {
case 0: {
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

  break;
}
#ifdef COMPILE_ - 1478185715
case 1: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185714
case 2: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _efree(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185713
case 3: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _efree(((zvalue)->value.ht));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185712
case 4: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _efree(((zvalue)->value.str.val));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185711
case 5: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _zend_list_addref((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185710
case 6: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _zend_list_delete((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185709
case 7: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _zval_copy_ctor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185708
case 8: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _zval_dtor(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185707
case 9: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _zval_dtor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185706
case 10: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    _zval_ptr_dtor((&(zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185705
case 11: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    convert_to_boolean((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185704
case 12: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    exit(1);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185703
case 13: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    gc_remove_zval_from_buffer((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185702
case 14: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    gc_zval_check_possible_root(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185701
case 15: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    gc_zval_possible_root((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185700
case 16: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    i_zval_ptr_dtor((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185699
case 17: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185698
case 18: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    if (!__is_neg("9-18", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185697
case 19: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    if (__is_neg("9-19", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      return;
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185696
case 20: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    zend_error((1 << 3L), "A non well formed numeric value encountered");
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185695
case 21: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    zend_hash_destroy((zvalue)->value.ht);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185694
case 22: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    zend_rebuild_symbol_table();
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185693
case 23: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    zval_addref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185692
case 24: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    zval_set_isref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185691
case 25: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    zval_set_refcount_p((zvalue), 0);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185690
case 26: {
  switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
  case IS_STRING:
  case IS_CONSTANT:
    CHECK_ZVAL_STRING_REL(zvalue);
    str_free(zvalue->value.str.val);
    zval_unset_isref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185689
case 27: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185688
case 28: {
  _efree(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185687
case 29: {
  _efree(((zvalue)->value.ht));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185686
case 30: {
  _efree(((zvalue)->value.str.val));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185685
case 31: {
  _zend_list_addref((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185684
case 32: {
  _zend_list_delete((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185683
case 33: {
  _zval_copy_ctor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185682
case 34: {
  _zval_dtor(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185681
case 35: {
  _zval_dtor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185680
case 36: {
  _zval_ptr_dtor((&(zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185679
case 37: {
  convert_to_boolean((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185678
case 38: {
  exit(1);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185677
case 39: {
  gc_remove_zval_from_buffer((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185676
case 40: {
  gc_zval_check_possible_root(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185675
case 41: {
  gc_zval_possible_root((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185674
case 42: {
  i_zval_ptr_dtor((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185673
case 43: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185672
case 44: {
  if (!__is_neg("9-44", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    switch ((*zvalue).type & 15) {
    case 6:
    case 8:;
      do {
        if (!(((zvalue->value.str.val) >=
               (compiler_globals.interned_strings_start)) &&
              ((zvalue->value.str.val) <
               (compiler_globals.interned_strings_end)))) {
          free(zvalue->value.str.val);
        }
      } while (0);
      break;
    case 4:
    case 9:
    case 5:
    case 7:
      zend_error((1 << 4L),
                 "Internal zval's can't be arrays, objects or resources");
      break;
    case 1:
    case 2:
    case 3:
    case 0:
    default:
      break;
    }

  break;
}
#endif
#ifdef COMPILE_ - 1478185671
case 45: {
  if (__is_neg("9-45", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    return;
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185670
case 46: {
  memset(&(*zvalue), 0, sizeof(*(&(*zvalue))));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185669
case 47: {
  memset(&compiler_globals, 0, sizeof(*(&compiler_globals)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185668
case 48: {
  memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185667
case 49: {
  memset(&zvalue->value.str, 0, sizeof(*(&zvalue->value.str)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185666
case 50: {
  memset(zvalue, 0, sizeof(*(zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185665
case 51: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185664
case 52: {
  zend_hash_destroy((zvalue)->value.ht);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185663
case 53: {
  zend_rebuild_symbol_table();
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185662
case 54: {
  zval_addref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185661
case 55: {
  zval_set_isref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185660
case 56: {
  zval_set_refcount_p((zvalue), 0);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185659
case 57: {
  zval_unset_isref_p((zvalue));
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

  break;
}
#endif
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
    // prophet generated patch
    switch (__choose("__ID10")) {
    case 0: {
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#ifdef COMPILE_ - 1478185658
    case 1: {
      (*(zvalue)).value.obj.handlers->add_ref(zvalue);
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185657
    case 2: {
      (*zvalue).value.obj.handlers->add_ref((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185656
    case 3: {
      (*zvalue).value.obj.handlers->add_ref(zvalue);
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185655
    case 4: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185654
    case 5: {
      (zvalue)->value.str.val =
          (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185653
    case 6: {
      _efree(((zvalue)));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185652
    case 7: {
      _efree(((zvalue)->value.ht));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185651
    case 8: {
      _efree(((zvalue)->value.str.val));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185650
    case 9: {
      _zend_list_addref((zvalue)->value.lval);
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185649
    case 10: {
      _zend_list_addref(zvalue->value.lval);
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185648
    case 11: {
      _zend_list_delete((zvalue)->value.lval);
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185647
    case 12: {
      _zval_copy_ctor_func((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185646
    case 13: {
      _zval_dtor(((zvalue)));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185645
    case 14: {
      _zval_dtor_func((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185644
    case 15: {
      _zval_ptr_dtor((&(zvalue)));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185643
    case 16: {
      convert_to_boolean((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185642
    case 17: {
      exit(1);
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185641
    case 18: {
      gc_remove_zval_from_buffer((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185640
    case 19: {
      gc_zval_check_possible_root(((zvalue)));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185639
    case 20: {
      gc_zval_possible_root((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185638
    case 21: {
      i_zval_ptr_dtor((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185637
    case 22: {
      if (!(((zvalue->value.str.val) >=
             (compiler_globals.interned_strings_start)) &&
            ((zvalue->value.str.val) <
             (compiler_globals.interned_strings_end)))) {
        zvalue->value.str.val =
            (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
      }
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185636
    case 23: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185635
    case 24: {
      if (!__is_neg("10-24", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0,
                    "zvalue") &&
          (!(((zvalue->value.str.val) >=
              (compiler_globals.interned_strings_start)) &&
             ((zvalue->value.str.val) <
              (compiler_globals.interned_strings_end))))) {
        zvalue->value.str.val =
            (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185634
    case 25: {
      if (!__is_neg("10-25", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
        if (!(((zvalue->value.str.val) >=
               (compiler_globals.interned_strings_start)) &&
              ((zvalue->value.str.val) <
               (compiler_globals.interned_strings_end)))) {
          zvalue->value.str.val = (char *)_estrndup((zvalue->value.str.val),
                                                    (zvalue->value.str.len));
        }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185633
    case 26: {
      if ((!(((zvalue->value.str.val) >=
              (compiler_globals.interned_strings_start)) &&
             ((zvalue->value.str.val) <
              (compiler_globals.interned_strings_end)))) &&
          !__is_neg("10-26", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0,
                    "zvalue")) {
        zvalue->value.str.val =
            (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185632
    case 27: {
      if ((!(((zvalue->value.str.val) >=
              (compiler_globals.interned_strings_start)) &&
             ((zvalue->value.str.val) <
              (compiler_globals.interned_strings_end)))) ||
          __is_neg("10-27", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue")) {
        zvalue->value.str.val =
            (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185631
    case 28: {
      if (__is_neg("10-28", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
        return;
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185630
    case 29: {
      if (zvalue->value.ht == &(executor_globals.symbol_table)) {
        return;
      }
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185629
    case 30: {
      memset(&compiler_globals, 0, sizeof(*(&compiler_globals)));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185628
    case 31: {
      memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185627
    case 32: {
      memset(&zvalue->value.str, 0, sizeof(*(&zvalue->value.str)));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185626
    case 33: {
      memset(zvalue, 0, sizeof(*(zvalue)));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185625
    case 34: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185624
    case 35: {
      zend_hash_destroy((zvalue)->value.ht);
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185623
    case 36: {
      zend_rebuild_symbol_table();
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185622
    case 37: {
      zval_addref_p((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185621
    case 38: {
      zval_set_isref_p((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185620
    case 39: {
      zval_set_refcount_p((zvalue), 0);
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185619
    case 40: {
      zval_unset_isref_p((zvalue));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185618
    case 41: {
      zvalue->value.str.val =
          (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
      if (!IS_INTERNED(zvalue->value.str.val)) {
        zvalue->value.str.val =
            (char *)estrndup_rel(zvalue->value.str.val, zvalue->value.str.len);
      }

      break;
    }
#endif
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
switch (__choose("__ID11")) {
case 0: {
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

  break;
}
#ifdef COMPILE_ - 1478185617
case 1: {
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
    (*(zvalue)).value.obj.handlers->add_ref(zvalue);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185616
case 2: {
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
    (*zvalue).value.obj.handlers->add_ref((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185615
case 3: {
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
    (*zvalue).value.obj.handlers->add_ref(zvalue);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185614
case 4: {
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
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185613
case 5: {
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
    (zvalue)->value.str.val =
        (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185612
case 6: {
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
    _efree(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185611
case 7: {
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
    _efree(((zvalue)->value.ht));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185610
case 8: {
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
    _efree(((zvalue)->value.str.val));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185609
case 9: {
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
    _zend_list_addref((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185608
case 10: {
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
    _zend_list_addref(zvalue->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185607
case 11: {
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
    _zend_list_delete((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185606
case 12: {
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
    _zval_copy_ctor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185605
case 13: {
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
    _zval_dtor(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185604
case 14: {
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
    _zval_dtor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185603
case 15: {
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
    _zval_ptr_dtor((&(zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185602
case 16: {
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
    convert_to_boolean((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185601
case 17: {
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
    exit(1);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185600
case 18: {
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
    gc_remove_zval_from_buffer((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185599
case 19: {
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
    gc_zval_check_possible_root(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185598
case 20: {
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
    gc_zval_possible_root((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185597
case 21: {
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
    i_zval_ptr_dtor((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185596
case 22: {
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
    if (!(((zvalue->value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          ((zvalue->value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      zvalue->value.str.val =
          (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185595
case 23: {
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
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185594
case 24: {
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
    if (!__is_neg("11-24", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185593
case 25: {
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
    if (__is_neg("11-25", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
      return;
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185592
case 26: {
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
    if (zvalue->value.ht == &(executor_globals.symbol_table)) {
      return;
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185591
case 27: {
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
    zend_error((1 << 3L), "A non well formed numeric value encountered");
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185590
case 28: {
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
    zend_hash_destroy((zvalue)->value.ht);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185589
case 29: {
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
    zend_rebuild_symbol_table();
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185588
case 30: {
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
    zval_addref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185587
case 31: {
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
    zval_set_isref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185586
case 32: {
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
    zval_set_refcount_p((zvalue), 0);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185585
case 33: {
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
    zval_unset_isref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185584
case 34: {
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
    zvalue->value.str.val =
        (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185583
case 35: {
  (*(zvalue)).value.obj.handlers->add_ref(zvalue);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185582
case 36: {
  (*zvalue).value.obj.handlers->add_ref((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185581
case 37: {
  (*zvalue).value.obj.handlers->add_ref(zvalue);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185580
case 38: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185579
case 39: {
  (zvalue)->value.str.val =
      (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185578
case 40: {
  _efree(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185577
case 41: {
  _efree(((zvalue)->value.ht));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185576
case 42: {
  _efree(((zvalue)->value.str.val));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185575
case 43: {
  _zend_list_addref((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185574
case 44: {
  _zend_list_addref(zvalue->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185573
case 45: {
  _zend_list_delete((zvalue)->value.lval);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185572
case 46: {
  _zval_copy_ctor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185571
case 47: {
  _zval_dtor(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185570
case 48: {
  _zval_dtor_func((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185569
case 49: {
  _zval_ptr_dtor((&(zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185568
case 50: {
  convert_to_boolean((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185567
case 51: {
  exit(1);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185566
case 52: {
  gc_remove_zval_from_buffer((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185565
case 53: {
  gc_zval_check_possible_root(((zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185564
case 54: {
  gc_zval_possible_root((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185563
case 55: {
  i_zval_ptr_dtor((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185562
case 56: {
  if (!(((zvalue->value.str.val) >=
         (compiler_globals.interned_strings_start)) &&
        ((zvalue->value.str.val) < (compiler_globals.interned_strings_end)))) {
    zvalue->value.str.val =
        (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
  }
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185561
case 57: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185560
case 58: {
  if (!__is_neg("11-58", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    switch ((*zvalue).type & 15) {
    case 7: {
      ;
      _zend_list_addref(zvalue->value.lval);
    } break;
    case 3:
    case 1:
    case 0:
      break;
    case 8:
    case 6:;
      if (!(((zvalue->value.str.val) >=
             (compiler_globals.interned_strings_start)) &&
            ((zvalue->value.str.val) <
             (compiler_globals.interned_strings_end)))) {
        zvalue->value.str.val =
            (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
      }
      break;
    case 4:
    case 9: {
      zval *tmp;
      HashTable *original_ht = zvalue->value.ht;
      HashTable *tmp_ht = ((void *)0);
      ;
      if (zvalue->value.ht == &(executor_globals.symbol_table)) {
        return;
      }
      (tmp_ht) = (HashTable *)_emalloc((sizeof(HashTable)));
      _zend_hash_init((tmp_ht), (zend_hash_num_elements(original_ht)),
                      (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
      zend_hash_copy(tmp_ht, original_ht, (copy_ctor_func_t)zval_add_ref,
                     (void *)&tmp, sizeof(zval *));
      zvalue->value.ht = tmp_ht;
    } break;
    case 5: {
      ;
      (*zvalue).value.obj.handlers->add_ref(zvalue);
    } break;
    }

  break;
}
#endif
#ifdef COMPILE_ - 1478185559
case 59: {
  if (__is_neg("11-59", 0, 0, 0, 0, 1, (void **){zvalue}, 0, 0, "zvalue"))
    return;
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185558
case 60: {
  if (zvalue->value.ht == &(executor_globals.symbol_table)) {
    return;
  }
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185557
case 61: {
  memset(&(*zvalue), 0, sizeof(*(&(*zvalue))));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185556
case 62: {
  memset(&(*zvalue).value, 0, sizeof(*(&(*zvalue).value)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185555
case 63: {
  memset(&(*zvalue).value.obj, 0, sizeof(*(&(*zvalue).value.obj)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185554
case 64: {
  memset(&compiler_globals, 0, sizeof(*(&compiler_globals)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185553
case 65: {
  memset(&executor_globals, 0, sizeof(*(&executor_globals)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185552
case 66: {
  memset(&executor_globals.symbol_table, 0,
         sizeof(*(&executor_globals.symbol_table)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185551
case 67: {
  memset(&zvalue->value, 0, sizeof(*(&zvalue->value)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185550
case 68: {
  memset(&zvalue->value.str, 0, sizeof(*(&zvalue->value.str)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185549
case 69: {
  memset((*zvalue).value.obj.handlers, 0,
         sizeof(*((*zvalue).value.obj.handlers)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185548
case 70: {
  memset(zvalue, 0, sizeof(*(zvalue)));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185547
case 71: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185546
case 72: {
  zend_hash_destroy((zvalue)->value.ht);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185545
case 73: {
  zend_rebuild_symbol_table();
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185544
case 74: {
  zval_addref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185543
case 75: {
  zval_set_isref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185542
case 76: {
  zval_set_refcount_p((zvalue), 0);
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185541
case 77: {
  zval_unset_isref_p((zvalue));
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

  break;
}
#endif
#ifdef COMPILE_ - 1478185540
case 78: {
  zvalue->value.str.val =
      (char *)_estrndup((zvalue->value.str.val), (zvalue->value.str.len));
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

  break;
}
#endif
}
}

ZEND_API int zend_print_variable(zval *var) {
  // prophet generated patch
  switch (__choose("__ID12")) {
  case 0: {
    return zend_print_zval(var, 0);

    break;
  }
#ifdef COMPILE_ - 1478185539
  case 1: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185538
  case 2: {
    _efree(((var)));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185537
  case 3: {
    _efree(((var)->value.ht));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185536
  case 4: {
    _efree(((var)->value.str.val));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185535
  case 5: {
    _zend_list_addref((var)->value.lval);
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185534
  case 6: {
    _zend_list_delete((var)->value.lval);
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185533
  case 7: {
    _zval_copy_ctor_func((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185532
  case 8: {
    _zval_dtor(((var)));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185531
  case 9: {
    _zval_dtor_func((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185530
  case 10: {
    _zval_ptr_dtor((&(var)));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185529
  case 11: {
    convert_to_boolean((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185528
  case 12: {
    exit(1);
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185527
  case 13: {
    gc_remove_zval_from_buffer((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185526
  case 14: {
    gc_zval_check_possible_root(((var)));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185525
  case 15: {
    gc_zval_possible_root((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185524
  case 16: {
    i_zval_ptr_dtor((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185523
  case 17: {
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185522
  case 18: {
    if (!__is_neg("12-18", 0, 0, 0, 0, 1, (void **){var}, 0, 0, "var"))
      return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185521
  case 19: {
    if (__is_neg("12-19", 0, 0, 0, 0, 1, (void **){var}, 0, 0, "var"))
      return 0;
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185520
  case 20: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185519
  case 21: {
    zend_hash_destroy((var)->value.ht);
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185518
  case 22: {
    zend_rebuild_symbol_table();
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185517
  case 23: {
    zval_addref_p((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185516
  case 24: {
    zval_set_isref_p((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185515
  case 25: {
    zval_set_refcount_p((var), 0);
    return zend_print_zval(var, 0);

    break;
  }
#endif
#ifdef COMPILE_ - 1478185514
  case 26: {
    zval_unset_isref_p((var));
    return zend_print_zval(var, 0);

    break;
  }
#endif
  }
}

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
    switch (__choose("__ID13")) {
    case 0: {
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#ifdef COMPILE_ - 1478185513
    case 1: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185512
    case 2: {
      (tmp) = (executor_globals.uninitialized_zval_ptr);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185511
    case 3: {
      (tmp) = *p;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185510
    case 4: {
      *(tmp) = zval_used_for_init;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185509
    case 5: {
      *tmp = zval_used_for_init;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185508
    case 6: {
      _efree(((tmp)));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185507
    case 7: {
      _efree(((tmp)->value.ht));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185506
    case 8: {
      _efree(((tmp)->value.str.val));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185505
    case 9: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185504
    case 10: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185503
    case 11: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185502
    case 12: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185501
    case 13: {
      _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                     key->arKey, key->nKeyLength, key->h, &tmp,
                                     sizeof(zval *), (void **)&p, (1 << 1));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185500
    case 14: {
      _zend_list_addref((tmp)->value.lval);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185499
    case 15: {
      _zend_list_delete((tmp)->value.lval);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185498
    case 16: {
      _zval_copy_ctor_func((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185497
    case 17: {
      _zval_dtor(((tmp)));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185496
    case 18: {
      _zval_dtor_func((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185495
    case 19: {
      _zval_ptr_dtor((&(tmp)));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185494
    case 20: {
      convert_to_boolean((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185493
    case 21: {
      exit(1);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185492
    case 22: {
      gc_remove_zval_from_buffer((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185491
    case 23: {
      gc_zval_check_possible_root(((tmp)));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185490
    case 24: {
      gc_zval_possible_root((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185489
    case 25: {
      i_zval_ptr_dtor((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185488
    case 26: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185487
    case 27: {
      if (!__is_neg("13-27", 0, 0, 0, 0, 5,
                    (void **){p, args, key, tmp, target}, 0, 0, "p", "args",
                    "key", "tmp", "target"))
        is_ref = (**p).type & 64;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185486
    case 28: {
      if (__is_neg("13-28", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, "p", "args", "key", "tmp", "target"))
        return 0;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185485
    case 29: {
      if (__is_neg("13-29", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, "p", "args", "key", "tmp", "target"))
        return 1;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185484
    case 30: {
      if (__is_neg("13-30", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, "p", "args", "key", "tmp", "target"))
        return 32;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185483
    case 31: {
      if (__is_neg("13-31", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, "p", "args", "key", "tmp", "target"))
        return 3;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185482
    case 32: {
      if (__is_neg("13-32", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                   0, 0, "p", "args", "key", "tmp", "target"))
        return 64;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185481
    case 33: {
      if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                         key->h, &tmp, sizeof(zval *),
                                         ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185480
    case 34: {
      is_ref = (**(p)).type & 64 break;
    }
#endif
#ifdef COMPILE_ - 1478185479
    case 35: {
      is_ref = (**(p)).type & 64;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185478
    case 36: {
      is_ref = (**p).type & 64;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185477
    case 37: {
      memset(&(**p), 0, sizeof(*(&(**p))));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185476
    case 38: {
      tmp = (executor_globals.uninitialized_zval_ptr);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185475
    case 39: {
      tmp = *(p);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185474
    case 40: {
      tmp = *p;
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185473
    case 41: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185472
    case 42: {
      zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185471
    case 43: {
      zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185470
    case 44: {
      zend_hash_destroy((tmp)->value.ht);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185469
    case 45: {
      zend_rebuild_symbol_table();
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185468
    case 46: {
      zval_addref_p((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185467
    case 47: {
      zval_addref_p(*((p)));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185466
    case 48: {
      zval_addref_p(tmp);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185465
    case 49: {
      zval_set_isref_p((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185464
    case 50: {
      zval_set_isref_p(*(((p))));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185463
    case 51: {
      zval_set_isref_p(*((p)));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185462
    case 52: {
      zval_set_isref_p(tmp);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185461
    case 53: {
      zval_set_refcount_p((tmp), 0);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185460
    case 54: {
      zval_set_refcount_p(tmp, 0);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185459
    case 55: {
      zval_unset_isref_p((tmp));
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
#ifdef COMPILE_ - 1478185458
    case 56: {
      zval_unset_isref_p(tmp);
      is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

      break;
    }
#endif
    }

    // prophet generated patch
    switch (__choose("__ID14")) {
    case 0: {
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#ifdef COMPILE_ - 1478185457
    case 1: {
      if (!EG(active_symbol_table)) {
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185456
    case 2: {
      if (!EG(active_symbol_table)) {
        (tmp) = (executor_globals.uninitialized_zval_ptr);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185455
    case 3: {
      if (!EG(active_symbol_table)) {
        (tmp) = *p;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185454
    case 4: {
      if (!EG(active_symbol_table)) {
        *(tmp) = zval_used_for_init;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185453
    case 5: {
      if (!EG(active_symbol_table)) {
        *tmp = zval_used_for_init;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185452
    case 6: {
      if (!EG(active_symbol_table)) {
        _efree(((tmp)));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185451
    case 7: {
      if (!EG(active_symbol_table)) {
        _efree(((tmp)->value.ht));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185450
    case 8: {
      if (!EG(active_symbol_table)) {
        _efree(((tmp)->value.str.val));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185449
    case 9: {
      if (!EG(active_symbol_table)) {
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       (key)->arKey, key->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185448
    case 10: {
      if (!EG(active_symbol_table)) {
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       key->arKey, (key)->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185447
    case 11: {
      if (!EG(active_symbol_table)) {
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185446
    case 12: {
      if (!EG(active_symbol_table)) {
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185445
    case 13: {
      if (!EG(active_symbol_table)) {
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185444
    case 14: {
      if (!EG(active_symbol_table)) {
        _zend_list_addref((tmp)->value.lval);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185443
    case 15: {
      if (!EG(active_symbol_table)) {
        _zend_list_delete((tmp)->value.lval);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185442
    case 16: {
      if (!EG(active_symbol_table)) {
        _zval_copy_ctor_func((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185441
    case 17: {
      if (!EG(active_symbol_table)) {
        _zval_dtor(((tmp)));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185440
    case 18: {
      if (!EG(active_symbol_table)) {
        _zval_dtor_func((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185439
    case 19: {
      if (!EG(active_symbol_table)) {
        _zval_ptr_dtor((&(tmp)));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185438
    case 20: {
      if (!EG(active_symbol_table)) {
        convert_to_boolean((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185437
    case 21: {
      if (!EG(active_symbol_table)) {
        exit(1);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185436
    case 22: {
      if (!EG(active_symbol_table)) {
        gc_remove_zval_from_buffer((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185435
    case 23: {
      if (!EG(active_symbol_table)) {
        gc_zval_check_possible_root(((tmp)));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185434
    case 24: {
      if (!EG(active_symbol_table)) {
        gc_zval_possible_root((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185433
    case 25: {
      if (!EG(active_symbol_table)) {
        i_zval_ptr_dtor((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185432
    case 26: {
      if (!EG(active_symbol_table)) {
        if (!(executor_globals.active_symbol_table)) {
          zend_rebuild_symbol_table();
        }
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185431
    case 27: {
      if (!EG(active_symbol_table)) {
        if (!__is_neg("14-27", 0, 0, 0, 0, 5,
                      (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                      "tmp", "args", "target"))
          zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185430
    case 28: {
      if (!EG(active_symbol_table)) {
        if (__is_neg("14-28", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 0;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185429
    case 29: {
      if (!EG(active_symbol_table)) {
        if (__is_neg("14-29", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 1;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185428
    case 30: {
      if (!EG(active_symbol_table)) {
        if (__is_neg("14-30", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 32;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185427
    case 31: {
      if (!EG(active_symbol_table)) {
        if (__is_neg("14-31", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 3;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185426
    case 32: {
      if (!EG(active_symbol_table)) {
        if (__is_neg("14-32", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 64;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185425
    case 33: {
      if (!EG(active_symbol_table)) {
        if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                           key->h, &tmp, sizeof(zval *),
                                           ((void *)0), (1 << 1)) == 0) {
          zval_addref_p(tmp);
        }
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185424
    case 34: {
      if (!EG(active_symbol_table)) {
        is_ref = (**(p)).type & 64;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185423
    case 35: {
      if (!EG(active_symbol_table)) {
        is_ref = (**p).type & 64;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185422
    case 36: {
      if (!EG(active_symbol_table)) {
        tmp = (executor_globals.uninitialized_zval_ptr);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185421
    case 37: {
      if (!EG(active_symbol_table)) {
        tmp = *(p);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185420
    case 38: {
      if (!EG(active_symbol_table)) {
        tmp = *p;
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185419
    case 39: {
      if (!EG(active_symbol_table)) {
        zend_error((1 << 3L), "A non well formed numeric value encountered");
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185418
    case 40: {
      if (!EG(active_symbol_table)) {
        zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185417
    case 41: {
      if (!EG(active_symbol_table)) {
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185416
    case 42: {
      if (!EG(active_symbol_table)) {
        zend_hash_destroy((tmp)->value.ht);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185415
    case 43: {
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table();
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185414
    case 44: {
      if (!EG(active_symbol_table)) {
        zval_addref_p((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185413
    case 45: {
      if (!EG(active_symbol_table)) {
        zval_addref_p(*((p)));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185412
    case 46: {
      if (!EG(active_symbol_table)) {
        zval_addref_p(tmp);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185411
    case 47: {
      if (!EG(active_symbol_table)) {
        zval_set_isref_p((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185410
    case 48: {
      if (!EG(active_symbol_table)) {
        zval_set_isref_p(*(((p))));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185409
    case 49: {
      if (!EG(active_symbol_table)) {
        zval_set_isref_p(*((p)));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185408
    case 50: {
      if (!EG(active_symbol_table)) {
        zval_set_isref_p(tmp);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185407
    case 51: {
      if (!EG(active_symbol_table)) {
        zval_set_refcount_p((tmp), 0);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185406
    case 52: {
      if (!EG(active_symbol_table)) {
        zval_set_refcount_p(tmp, 0);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185405
    case 53: {
      if (!EG(active_symbol_table)) {
        zval_unset_isref_p((tmp));
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185404
    case 54: {
      if (!EG(active_symbol_table)) {
        zval_unset_isref_p(tmp);
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185403
    case 55: {
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185402
    case 56: {
      (tmp) = (executor_globals.uninitialized_zval_ptr);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185401
    case 57: {
      (tmp) = *p;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185400
    case 58: {
      *(tmp) = zval_used_for_init;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185399
    case 59: {
      *tmp = zval_used_for_init;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185398
    case 60: {
      _efree(((tmp)));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185397
    case 61: {
      _efree(((tmp)->value.ht));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185396
    case 62: {
      _efree(((tmp)->value.str.val));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185395
    case 63: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), (key)->arKey, key->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185394
    case 64: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, (key)->nKeyLength,
          key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185393
    case 65: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185392
    case 66: {
      _zend_hash_quick_add_or_update(
          (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
          key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185391
    case 67: {
      _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                     key->arKey, key->nKeyLength, key->h, &tmp,
                                     sizeof(zval *), (void **)&p, (1 << 1));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185390
    case 68: {
      _zend_list_addref((tmp)->value.lval);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185389
    case 69: {
      _zend_list_delete((tmp)->value.lval);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185388
    case 70: {
      _zval_copy_ctor_func((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185387
    case 71: {
      _zval_dtor(((tmp)));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185386
    case 72: {
      _zval_dtor_func((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185385
    case 73: {
      _zval_ptr_dtor((&(tmp)));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185384
    case 74: {
      convert_to_boolean((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185383
    case 75: {
      exit(1);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185382
    case 76: {
      gc_remove_zval_from_buffer((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185381
    case 77: {
      gc_zval_check_possible_root(((tmp)));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185380
    case 78: {
      gc_zval_possible_root((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185379
    case 79: {
      i_zval_ptr_dtor((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185378
    case 80: {
      if (!(executor_globals.active_symbol_table)) {
        zend_rebuild_symbol_table();
      }
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185377
    case 81: {
      if (!__is_neg("14-81", 0, 0, 0, 0, 5,
                    (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                    "tmp", "args", "target") &&
          (!(executor_globals.active_symbol_table))) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185376
    case 82: {
      if (!__is_neg("14-82", 0, 0, 0, 0, 5,
                    (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                    "tmp", "args", "target"))
        if (!(executor_globals.active_symbol_table)) {
          zend_rebuild_symbol_table();
        }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185375
    case 83: {
      if ((!(executor_globals.active_symbol_table)) &&
          !__is_neg("14-83", 0, 0, 0, 0, 5,
                    (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                    "tmp", "args", "target")) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185374
    case 84: {
      if ((!(executor_globals.active_symbol_table)) ||
          __is_neg("14-84", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, "p", "key", "tmp", "args", "target")) {
        zend_rebuild_symbol_table();
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185373
    case 85: {
      if (__is_neg("14-85", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, "p", "key", "tmp", "args", "target"))
        return 0;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185372
    case 86: {
      if (__is_neg("14-86", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, "p", "key", "tmp", "args", "target"))
        return 1;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185371
    case 87: {
      if (__is_neg("14-87", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, "p", "key", "tmp", "args", "target"))
        return 32;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185370
    case 88: {
      if (__is_neg("14-88", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, "p", "key", "tmp", "args", "target"))
        return 3;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185369
    case 89: {
      if (__is_neg("14-89", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                   0, 0, "p", "key", "tmp", "args", "target"))
        return 64;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185368
    case 90: {
      if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                         key->h, &tmp, sizeof(zval *),
                                         ((void *)0), (1 << 1)) == 0) {
        zval_addref_p(tmp);
      }
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185367
    case 91: {
      is_ref = (**(p)).type & 64;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185366
    case 92: {
      is_ref = (**p).type & 64;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185365
    case 93: {
      memset(&executor_globals, 0, sizeof(*(&executor_globals)));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185364
    case 94: {
      tmp = (executor_globals.uninitialized_zval_ptr);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185363
    case 95: {
      tmp = *(p);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185362
    case 96: {
      tmp = *p;
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185361
    case 97: {
      zend_error((1 << 3L), "A non well formed numeric value encountered");
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185360
    case 98: {
      zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185359
    case 99: {
      zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185358
    case 100: {
      zend_hash_destroy((tmp)->value.ht);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185357
    case 101: {
      zend_rebuild_symbol_table();
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185356
    case 102: {
      zval_addref_p((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185355
    case 103: {
      zval_addref_p(*((p)));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185354
    case 104: {
      zval_addref_p(tmp);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185353
    case 105: {
      zval_set_isref_p((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185352
    case 106: {
      zval_set_isref_p(*(((p))));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185351
    case 107: {
      zval_set_isref_p(*((p)));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185350
    case 108: {
      zval_set_isref_p(tmp);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185349
    case 109: {
      zval_set_refcount_p((tmp), 0);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185348
    case 110: {
      zval_set_refcount_p(tmp, 0);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185347
    case 111: {
      zval_unset_isref_p((tmp));
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
#ifdef COMPILE_ - 1478185346
    case 112: {
      zval_unset_isref_p(tmp);
      if (!EG(active_symbol_table)) {
        zend_rebuild_symbol_table(TSRMLS_C);
      }

      break;
    }
#endif
    }
    if (zend_hash_quick_find(EG(active_symbol_table), key->arKey,
                             key->nKeyLength, key->h, (void **)&p) == FAILURE) {
      // prophet generated patch
      switch (__choose("__ID15")) {
      case 0: {
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

        break;
      }
#ifdef COMPILE_ - 1478185345
      case 1: {
        if (is_ref) {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185344
      case 2: {
        if (is_ref) {
          (tmp) = (executor_globals.uninitialized_zval_ptr);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185343
      case 3: {
        if (is_ref) {
          (tmp) = *p;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185342
      case 4: {
        if (is_ref) {
          *(tmp) = zval_used_for_init;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185341
      case 5: {
        if (is_ref) {
          *tmp = zval_used_for_init;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185340
      case 6: {
        if (is_ref) {
          _efree(((tmp)));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185339
      case 7: {
        if (is_ref) {
          _efree(((tmp)->value.ht));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185338
      case 8: {
        if (is_ref) {
          _efree(((tmp)->value.str.val));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185337
      case 9: {
        if (is_ref) {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         (key)->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185336
      case 10: {
        if (is_ref) {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, (key)->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185335
      case 11: {
        if (is_ref) {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, (key)->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185334
      case 12: {
        if (is_ref) {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &(tmp), sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185333
      case 13: {
        if (is_ref) {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185332
      case 14: {
        if (is_ref) {
          _zend_list_addref((tmp)->value.lval);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185331
      case 15: {
        if (is_ref) {
          _zend_list_delete((tmp)->value.lval);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185330
      case 16: {
        if (is_ref) {
          _zval_copy_ctor_func((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185329
      case 17: {
        if (is_ref) {
          _zval_dtor(((tmp)));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185328
      case 18: {
        if (is_ref) {
          _zval_dtor_func((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185327
      case 19: {
        if (is_ref) {
          _zval_ptr_dtor((&(tmp)));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185326
      case 20: {
        if (is_ref) {
          convert_to_boolean((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185325
      case 21: {
        if (is_ref) {
          exit(1);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185324
      case 22: {
        if (is_ref) {
          gc_remove_zval_from_buffer((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185323
      case 23: {
        if (is_ref) {
          gc_zval_check_possible_root(((tmp)));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185322
      case 24: {
        if (is_ref) {
          gc_zval_possible_root((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185321
      case 25: {
        if (is_ref) {
          i_zval_ptr_dtor((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185320
      case 26: {
        if (is_ref) {
          if (!(executor_globals.active_symbol_table)) {
            zend_rebuild_symbol_table();
          }
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185319
      case 27: {
        if (is_ref) {
          if (!__is_neg("15-27", 0, 0, 0, 0, 5,
                        (void **){tmp, p, key, args, target}, 0, 0, "tmp", "p",
                        "key", "args", "target"))
            do {
              (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
              ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
            } while (0);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185318
      case 28: {
        if (is_ref) {
          if (__is_neg("15-28", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, args, target}, 0, 0, "tmp", "p",
                       "key", "args", "target"))
            return 0;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185317
      case 29: {
        if (is_ref) {
          if (__is_neg("15-29", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, args, target}, 0, 0, "tmp", "p",
                       "key", "args", "target"))
            return 1;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185316
      case 30: {
        if (is_ref) {
          if (__is_neg("15-30", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, args, target}, 0, 0, "tmp", "p",
                       "key", "args", "target"))
            return 32;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185315
      case 31: {
        if (is_ref) {
          if (__is_neg("15-31", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, args, target}, 0, 0, "tmp", "p",
                       "key", "args", "target"))
            return 3;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185314
      case 32: {
        if (is_ref) {
          if (__is_neg("15-32", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, args, target}, 0, 0, "tmp", "p",
                       "key", "args", "target"))
            return 64;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185313
      case 33: {
        if (is_ref) {
          if (_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
            zval_addref_p(tmp);
          }
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185312
      case 34: {
        if (is_ref) {
          is_ref = (**(p)).type & 64;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185311
      case 35: {
        if (is_ref) {
          is_ref = (**p).type & 64;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185310
      case 36: {
        if (is_ref) {
          memset(&((zval_gc_info *)(tmp))->u, 0,
                 sizeof(*(&((zval_gc_info *)(tmp))->u)));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185309
      case 37: {
        if (is_ref) {
          memset(((zval_gc_info *)(tmp)), 0,
                 sizeof(*(((zval_gc_info *)(tmp)))));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185308
      case 38: {
        if (is_ref) {
          memset((tmp), 0, sizeof(*((tmp))));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185307
      case 39: {
        if (is_ref) {
          tmp = (executor_globals.uninitialized_zval_ptr);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185306
      case 40: {
        if (is_ref) {
          tmp = *(p);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185305
      case 41: {
        if (is_ref) {
          tmp = *p;
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185304
      case 42: {
        if (is_ref) {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185303
      case 43: {
        if (is_ref) {
          zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185302
      case 44: {
        if (is_ref) {
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185301
      case 45: {
        if (is_ref) {
          zend_hash_destroy((tmp)->value.ht);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185300
      case 46: {
        if (is_ref) {
          zend_rebuild_symbol_table();
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185299
      case 47: {
        if (is_ref) {
          zval_addref_p((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185298
      case 48: {
        if (is_ref) {
          zval_addref_p(*((p)));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185297
      case 49: {
        if (is_ref) {
          zval_addref_p(tmp);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185296
      case 50: {
        if (is_ref) {
          zval_set_isref_p((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185295
      case 51: {
        if (is_ref) {
          zval_set_isref_p(*(((p))));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185294
      case 52: {
        if (is_ref) {
          zval_set_isref_p(*((p)));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185293
      case 53: {
        if (is_ref) {
          zval_set_isref_p(tmp);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185292
      case 54: {
        if (is_ref) {
          zval_set_refcount_p((tmp), 0);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185291
      case 55: {
        if (is_ref) {
          zval_set_refcount_p(tmp, 0);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185290
      case 56: {
        if (is_ref) {
          zval_unset_isref_p((tmp));
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185289
      case 57: {
        if (is_ref) {
          zval_unset_isref_p(tmp);
          ALLOC_INIT_ZVAL(tmp);
          Z_SET_ISREF_P(tmp);
          zend_hash_quick_add(EG(active_symbol_table), key->arKey,
                              key->nKeyLength, key->h, &tmp, sizeof(zval *),
                              (void **)&p);
        } else {
          tmp = EG(uninitialized_zval_ptr);
          zend_error(E_NOTICE, "Undefined variable: %s", key->arKey);
        }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185288
      case 58: {
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185287
      case 59: {
        (tmp) = (executor_globals.uninitialized_zval_ptr);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185286
      case 60: {
        (tmp) = *p;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185285
      case 61: {
        *(tmp) = zval_used_for_init;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185284
      case 62: {
        *tmp = zval_used_for_init;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185283
      case 63: {
        _efree(((tmp)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185282
      case 64: {
        _efree(((tmp)->value.ht));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185281
      case 65: {
        _efree(((tmp)->value.str.val));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185280
      case 66: {
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       (key)->arKey, key->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185279
      case 67: {
        _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                       key->arKey, (key)->nKeyLength, key->h,
                                       &tmp, sizeof(zval *), (void **)&p,
                                       (1 << 1));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185278
      case 68: {
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            (key)->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185277
      case 69: {
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &(tmp), sizeof(zval *), (void **)&p, (1 << 1));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185276
      case 70: {
        _zend_hash_quick_add_or_update(
            (executor_globals.active_symbol_table), key->arKey, key->nKeyLength,
            key->h, &tmp, sizeof(zval *), (void **)&p, (1 << 1));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185275
      case 71: {
        _zend_list_addref((tmp)->value.lval);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185274
      case 72: {
        _zend_list_delete((tmp)->value.lval);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185273
      case 73: {
        _zval_copy_ctor_func((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185272
      case 74: {
        _zval_dtor(((tmp)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185271
      case 75: {
        _zval_dtor_func((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185270
      case 76: {
        _zval_ptr_dtor((&(tmp)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185269
      case 77: {
        convert_to_boolean((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185268
      case 78: {
        exit(1);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185267
      case 79: {
        gc_remove_zval_from_buffer((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185266
      case 80: {
        gc_zval_check_possible_root(((tmp)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185265
      case 81: {
        gc_zval_possible_root((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185264
      case 82: {
        i_zval_ptr_dtor((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185263
      case 83: {
        if (!(executor_globals.active_symbol_table)) {
          zend_rebuild_symbol_table();
        }
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185262
      case 84: {
        if (!__is_neg("15-84", 0, 0, 0, 0, 5,
                      (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                      "tmp", "args", "target") &&
            (is_ref)) {
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185261
      case 85: {
        if (!__is_neg("15-85", 0, 0, 0, 0, 5,
                      (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                      "tmp", "args", "target"))
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
                (executor_globals.active_symbol_table), key->arKey,
                key->nKeyLength, key->h, &tmp, sizeof(zval *), (void **)&p,
                (1 << 1));
          } else {
            tmp = (executor_globals.uninitialized_zval_ptr);
            zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          }

        break;
      }
#endif
#ifdef COMPILE_ - 1478185260
      case 86: {
        if ((is_ref) && !__is_neg("15-86", 0, 0, 0, 0, 5,
                                  (void **){p, key, tmp, args, target}, 0, 0,
                                  "p", "key", "tmp", "args", "target")) {
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185259
      case 87: {
        if ((is_ref) || __is_neg("15-87", 0, 0, 0, 0, 5,
                                 (void **){p, key, tmp, args, target}, 0, 0,
                                 "p", "key", "tmp", "args", "target")) {
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185258
      case 88: {
        if (__is_neg("15-88", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 0;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185257
      case 89: {
        if (__is_neg("15-89", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 1;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185256
      case 90: {
        if (__is_neg("15-90", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 32;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185255
      case 91: {
        if (__is_neg("15-91", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 3;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185254
      case 92: {
        if (__is_neg("15-92", 0, 0, 0, 0, 5,
                     (void **){p, key, tmp, args, target}, 0, 0, "p", "key",
                     "tmp", "args", "target"))
          return 64;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185253
      case 93: {
        if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                           key->h, &tmp, sizeof(zval *),
                                           ((void *)0), (1 << 1)) == 0) {
          zval_addref_p(tmp);
        }
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185252
      case 94: {
        is_ref = (**(p)).type & 64;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185251
      case 95: {
        is_ref = (**p).type & 64;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185250
      case 96: {
        memset(&((zval_gc_info *)(tmp))->u, 0,
               sizeof(*(&((zval_gc_info *)(tmp))->u)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185249
      case 97: {
        memset(&executor_globals, 0, sizeof(*(&executor_globals)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185248
      case 98: {
        memset(((zval_gc_info *)(tmp)), 0, sizeof(*(((zval_gc_info *)(tmp)))));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185247
      case 99: {
        memset((tmp), 0, sizeof(*((tmp))));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185246
      case 100: {
        memset(key, 0, sizeof(*(key)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185245
      case 101: {
        tmp = (executor_globals.uninitialized_zval_ptr);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185244
      case 102: {
        tmp = *(p);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185243
      case 103: {
        tmp = *p;
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185242
      case 104: {
        zend_error((1 << 3L), "A non well formed numeric value encountered");
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185241
      case 105: {
        zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185240
      case 106: {
        zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185239
      case 107: {
        zend_hash_destroy((tmp)->value.ht);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185238
      case 108: {
        zend_rebuild_symbol_table();
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185237
      case 109: {
        zval_addref_p((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185236
      case 110: {
        zval_addref_p(*((p)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185235
      case 111: {
        zval_addref_p(tmp);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185234
      case 112: {
        zval_set_isref_p((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185233
      case 113: {
        zval_set_isref_p(*(((p))));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185232
      case 114: {
        zval_set_isref_p(*((p)));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185231
      case 115: {
        zval_set_isref_p(tmp);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185230
      case 116: {
        zval_set_refcount_p((tmp), 0);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185229
      case 117: {
        zval_set_refcount_p(tmp, 0);
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185228
      case 118: {
        zval_unset_isref_p((tmp));
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

        break;
      }
#endif
#ifdef COMPILE_ - 1478185227
      case 119: {
        zval_unset_isref_p(tmp);
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

        break;
      }
#endif
      }
    } else {
      if (is_ref) {
        // prophet generated patch
        switch (__choose("__ID16")) {
        case 0: {
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#ifdef COMPILE_ - 1478185226
        case 1: {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185225
        case 2: {
          (tmp) = (executor_globals.uninitialized_zval_ptr);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185224
        case 3: {
          (tmp) = *p;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185223
        case 4: {
          *(tmp) = zval_used_for_init;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185222
        case 5: {
          *tmp = zval_used_for_init;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185221
        case 6: {
          _efree(((tmp)));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185220
        case 7: {
          _efree(((tmp)->value.ht));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185219
        case 8: {
          _efree(((tmp)->value.str.val));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185218
        case 9: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         (key)->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185217
        case 10: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, (key)->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185216
        case 11: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, (key)->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185215
        case 12: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &(tmp), sizeof(zval *), (void **)&p,
                                         (1 << 1));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185214
        case 13: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185213
        case 14: {
          _zend_list_addref((tmp)->value.lval);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185212
        case 15: {
          _zend_list_delete((tmp)->value.lval);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185211
        case 16: {
          _zval_copy_ctor_func((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185210
        case 17: {
          _zval_dtor(((tmp)));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185209
        case 18: {
          _zval_dtor_func((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185208
        case 19: {
          _zval_ptr_dtor((&(tmp)));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185207
        case 20: {
          convert_to_boolean((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185206
        case 21: {
          exit(1);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185205
        case 22: {
          gc_remove_zval_from_buffer((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185204
        case 23: {
          gc_zval_check_possible_root(((tmp)));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185203
        case 24: {
          gc_zval_possible_root((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185202
        case 25: {
          i_zval_ptr_dtor((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185201
        case 26: {
          if (!(executor_globals.active_symbol_table)) {
            zend_rebuild_symbol_table();
          }
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185200
        case 27: {
          if (!__is_neg("16-27", 0, 0, 0, 0, 5,
                        (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                        "key", "target", "args") &&
              (!zval_isref_p(*p))) {
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
          }

          break;
        }
#endif
#ifdef COMPILE_ - 1478185199
        case 28: {
          if (!__is_neg("16-28", 0, 0, 0, 0, 5,
                        (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                        "key", "target", "args"))
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
            }

          break;
        }
#endif
#ifdef COMPILE_ - 1478185198
        case 29: {
          if ((!zval_isref_p(*p)) &&
              !__is_neg("16-29", 0, 0, 0, 0, 5,
                        (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                        "key", "target", "args")) {
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
          }

          break;
        }
#endif
#ifdef COMPILE_ - 1478185197
        case 30: {
          if ((!zval_isref_p(*p)) ||
              __is_neg("16-30", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args")) {
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
          }

          break;
        }
#endif
#ifdef COMPILE_ - 1478185196
        case 31: {
          if (__is_neg("16-31", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 0;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185195
        case 32: {
          if (__is_neg("16-32", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 1;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185194
        case 33: {
          if (__is_neg("16-33", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 32;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185193
        case 34: {
          if (__is_neg("16-34", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 3;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185192
        case 35: {
          if (__is_neg("16-35", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 64;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185191
        case 36: {
          if (_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
            zval_addref_p(tmp);
          }
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185190
        case 37: {
          is_ref = (**(p)).type & 64;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185189
        case 38: {
          is_ref = (**p).type & 64;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185188
        case 39: {
          memset(&(**(p)), 0, sizeof(*(&(**(p)))));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185187
        case 40: {
          memset((*(p)), 0, sizeof(*((*(p)))));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185186
        case 41: {
          tmp = (executor_globals.uninitialized_zval_ptr);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185185
        case 42: {
          tmp = *(p);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185184
        case 43: {
          tmp = *p;
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185183
        case 44: {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185182
        case 45: {
          zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185181
        case 46: {
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185180
        case 47: {
          zend_hash_destroy((tmp)->value.ht);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185179
        case 48: {
          zend_rebuild_symbol_table();
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185178
        case 49: {
          zval_addref_p((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185177
        case 50: {
          zval_addref_p(*((p)));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185176
        case 51: {
          zval_addref_p(tmp);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185175
        case 52: {
          zval_set_isref_p((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185174
        case 53: {
          zval_set_isref_p(*(((p))));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185173
        case 54: {
          zval_set_isref_p(*((p)));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185172
        case 55: {
          zval_set_isref_p(tmp);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185171
        case 56: {
          zval_set_refcount_p((tmp), 0);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185170
        case 57: {
          zval_set_refcount_p(tmp, 0);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185169
        case 58: {
          zval_unset_isref_p((tmp));
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185168
        case 59: {
          zval_unset_isref_p(tmp);
          SEPARATE_ZVAL_TO_MAKE_IS_REF(p);

          break;
        }
#endif
        }
        tmp = *p;
      } else if (Z_ISREF_PP(p)) {
        // prophet generated patch
        switch (__choose("__ID17")) {
        case 0: {
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#ifdef COMPILE_ - 1478185167
        case 1: {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185166
        case 2: {
          (tmp) = (executor_globals.uninitialized_zval_ptr);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185165
        case 3: {
          (tmp) = *p;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185164
        case 4: {
          *(tmp) = zval_used_for_init;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185163
        case 5: {
          *tmp = zval_used_for_init;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185162
        case 6: {
          _efree(((tmp)));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185161
        case 7: {
          _efree(((tmp)->value.ht));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185160
        case 8: {
          _efree(((tmp)->value.str.val));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185159
        case 9: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         (key)->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185158
        case 10: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, (key)->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185157
        case 11: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, (key)->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185156
        case 12: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &(tmp), sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185155
        case 13: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185154
        case 14: {
          _zend_list_addref((tmp)->value.lval);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185153
        case 15: {
          _zend_list_delete((tmp)->value.lval);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185152
        case 16: {
          _zval_copy_ctor_func((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185151
        case 17: {
          _zval_dtor(((tmp)));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185150
        case 18: {
          _zval_dtor_func((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185149
        case 19: {
          _zval_ptr_dtor((&(tmp)));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185148
        case 20: {
          convert_to_boolean((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185147
        case 21: {
          exit(1);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185146
        case 22: {
          gc_remove_zval_from_buffer((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185145
        case 23: {
          gc_zval_check_possible_root(((tmp)));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185144
        case 24: {
          gc_zval_possible_root((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185143
        case 25: {
          i_zval_ptr_dtor((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185142
        case 26: {
          if (!(executor_globals.active_symbol_table)) {
            zend_rebuild_symbol_table();
          }
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185141
        case 27: {
          if (!__is_neg("17-27", 0, 0, 0, 0, 5,
                        (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                        "key", "target", "args"))
            do {
              (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
              ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
            } while (0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185140
        case 28: {
          if (__is_neg("17-28", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 0;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185139
        case 29: {
          if (__is_neg("17-29", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 1;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185138
        case 30: {
          if (__is_neg("17-30", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 32;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185137
        case 31: {
          if (__is_neg("17-31", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 3;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185136
        case 32: {
          if (__is_neg("17-32", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 64;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185135
        case 33: {
          if (_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
            zval_addref_p(tmp);
          }
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185134
        case 34: {
          is_ref = (**(p)).type & 64;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185133
        case 35: {
          is_ref = (**p).type & 64;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185132
        case 36: {
          memset(&((zval_gc_info *)(tmp))->u, 0,
                 sizeof(*(&((zval_gc_info *)(tmp))->u)));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185131
        case 37: {
          memset(((zval_gc_info *)(tmp)), 0,
                 sizeof(*(((zval_gc_info *)(tmp)))));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185130
        case 38: {
          memset((tmp), 0, sizeof(*((tmp))));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185129
        case 39: {
          tmp = (executor_globals.uninitialized_zval_ptr);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185128
        case 40: {
          tmp = *(p);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185127
        case 41: {
          tmp = *p;
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185126
        case 42: {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185125
        case 43: {
          zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185124
        case 44: {
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185123
        case 45: {
          zend_hash_destroy((tmp)->value.ht);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185122
        case 46: {
          zend_rebuild_symbol_table();
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185121
        case 47: {
          zval_addref_p((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185120
        case 48: {
          zval_addref_p(*((p)));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185119
        case 49: {
          zval_addref_p(tmp);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185118
        case 50: {
          zval_set_isref_p((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185117
        case 51: {
          zval_set_isref_p(*(((p))));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185116
        case 52: {
          zval_set_isref_p(*((p)));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185115
        case 53: {
          zval_set_isref_p(tmp);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185114
        case 54: {
          zval_set_refcount_p((tmp), 0);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185113
        case 55: {
          zval_set_refcount_p(tmp, 0);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185112
        case 56: {
          zval_unset_isref_p((tmp));
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185111
        case 57: {
          zval_unset_isref_p(tmp);
          ALLOC_INIT_ZVAL(tmp);

          break;
        }
#endif
        }
        // prophet generated patch
        switch (__choose("__ID18")) {
        case 0: {
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#ifdef COMPILE_ - 1478185110
        case 1: {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185109
        case 2: {
          (tmp) = (executor_globals.uninitialized_zval_ptr);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185108
        case 3: {
          (tmp) = *p;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185107
        case 4: {
          *(tmp) = zval_used_for_init;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185106
        case 5: {
          *tmp = zval_used_for_init;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185105
        case 6: {
          _efree(((tmp)));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185104
        case 7: {
          _efree(((tmp)->value.ht));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185103
        case 8: {
          _efree(((tmp)->value.str.val));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185102
        case 9: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         (key)->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185101
        case 10: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, (key)->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185100
        case 11: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, (key)->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185099
        case 12: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &(tmp), sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185098
        case 13: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185097
        case 14: {
          _zend_list_addref((tmp)->value.lval);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185096
        case 15: {
          _zend_list_delete((tmp)->value.lval);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185095
        case 16: {
          _zval_copy_ctor_func((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185094
        case 17: {
          _zval_dtor(((tmp)));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185093
        case 18: {
          _zval_dtor_func((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185092
        case 19: {
          _zval_ptr_dtor((&(tmp)));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185091
        case 20: {
          convert_to_boolean((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185090
        case 21: {
          exit(1);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185089
        case 22: {
          gc_remove_zval_from_buffer((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185088
        case 23: {
          gc_zval_check_possible_root(((tmp)));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185087
        case 24: {
          gc_zval_possible_root((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185086
        case 25: {
          i_zval_ptr_dtor((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185085
        case 26: {
          if (!(executor_globals.active_symbol_table)) {
            zend_rebuild_symbol_table();
          }
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185084
        case 27: {
          if (!__is_neg("18-27", 0, 0, 0, 0, 5,
                        (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                        "key", "target", "args"))
            do {
              (tmp)->value = (*p)->value;
              (*tmp).type = (**p).type;
            } while (0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185083
        case 28: {
          if (__is_neg("18-28", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 0;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185082
        case 29: {
          if (__is_neg("18-29", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 1;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185081
        case 30: {
          if (__is_neg("18-30", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 32;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185080
        case 31: {
          if (__is_neg("18-31", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 3;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185079
        case 32: {
          if (__is_neg("18-32", 0, 0, 0, 0, 5,
                       (void **){p, tmp, key, target, args}, 0, 0, "p", "tmp",
                       "key", "target", "args"))
            return 64;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185078
        case 33: {
          if (_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
            zval_addref_p(tmp);
          }
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185077
        case 34: {
          is_ref = (**(p)).type & 64;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185076
        case 35: {
          is_ref = (**p).type & 64;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185075
        case 36: {
          memset(&(**p), 0, sizeof(*(&(**p))));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185074
        case 37: {
          memset(&(*tmp), 0, sizeof(*(&(*tmp))));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185073
        case 38: {
          memset((*p), 0, sizeof(*((*p))));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185072
        case 39: {
          memset((tmp), 0, sizeof(*((tmp))));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185071
        case 40: {
          tmp = (executor_globals.uninitialized_zval_ptr);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185070
        case 41: {
          tmp = *(p);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185069
        case 42: {
          tmp = *p;
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185068
        case 43: {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185067
        case 44: {
          zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185066
        case 45: {
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185065
        case 46: {
          zend_hash_destroy((tmp)->value.ht);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185064
        case 47: {
          zend_rebuild_symbol_table();
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185063
        case 48: {
          zval_addref_p((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185062
        case 49: {
          zval_addref_p(*((p)));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185061
        case 50: {
          zval_addref_p(tmp);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185060
        case 51: {
          zval_set_isref_p((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185059
        case 52: {
          zval_set_isref_p(*(((p))));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185058
        case 53: {
          zval_set_isref_p(*((p)));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185057
        case 54: {
          zval_set_isref_p(tmp);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185056
        case 55: {
          zval_set_refcount_p((tmp), 0);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185055
        case 56: {
          zval_set_refcount_p(tmp, 0);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185054
        case 57: {
          zval_unset_isref_p((tmp));
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185053
        case 58: {
          zval_unset_isref_p(tmp);
          ZVAL_COPY_VALUE(tmp, *p);

          break;
        }
#endif
        }
        // prophet generated patch
        switch (__choose("__ID19")) {
        case 0: {
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#ifdef COMPILE_ - 1478185052
        case 1: {
          (executor_globals.argument_stack) =
              zend_vm_stack_new_page(((16 * 1024) - 16));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185051
        case 2: {
          (tmp) = (executor_globals.uninitialized_zval_ptr);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185050
        case 3: {
          (tmp) = *p;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185049
        case 4: {
          *(tmp) = zval_used_for_init;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185048
        case 5: {
          *tmp = zval_used_for_init;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185047
        case 6: {
          __overflow(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185046
        case 7: {
          __sigsetjmp(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185045
        case 8: {
          __stpcpy(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185044
        case 9: {
          __zend_realloc(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185043
        case 10: {
          _array_init(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185042
        case 11: {
          _efree(((tmp)));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185041
        case 12: {
          _efree(((tmp)->value.ht));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185040
        case 13: {
          _efree(((tmp)->value.str.val));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185039
        case 14: {
          _estrndup(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185038
        case 15: {
          _longjmp(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185037
        case 16: {
          _object_init_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185036
        case 17: {
          _zend_bailout(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185035
        case 18: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         (key)->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185034
        case 19: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, (key)->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185033
        case 20: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, (key)->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185032
        case 21: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &(tmp), sizeof(zval *), (void **)&p,
                                         (1 << 1));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185031
        case 22: {
          _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                         key->arKey, key->nKeyLength, key->h,
                                         &tmp, sizeof(zval *), (void **)&p,
                                         (1 << 1));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185030
        case 23: {
          _zend_list_addref((tmp)->value.lval);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185029
        case 24: {
          _zend_list_delete((tmp)->value.lval);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185028
        case 25: {
          _zend_mm_alloc(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185027
        case 26: {
          _zend_mm_block_size(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185026
        case 27: {
          _zend_mm_free(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185025
        case 28: {
          _zval_copy_ctor_func((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185024
        case 29: {
          _zval_dtor(((tmp)));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185023
        case 30: {
          _zval_dtor_func((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185022
        case 31: {
          _zval_ptr_dtor((&(tmp)));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185021
        case 32: {
          add_index_null(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185020
        case 33: {
          add_next_index_bool(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185019
        case 34: {
          add_next_index_long(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185018
        case 35: {
          add_next_index_resource(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185017
        case 36: {
          add_next_index_zval(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185016
        case 37: {
          add_trait_alias(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185015
        case 38: {
          bitwise_not_function(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185014
        case 39: {
          boolean_not_function(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185013
        case 40: {
          bzero(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185012
        case 41: {
          compile_file(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185011
        case 42: {
          compile_string(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185010
        case 43: {
          convert_to_boolean((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185009
        case 44: {
          convert_to_long_base(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185008
        case 45: {
          dlopen(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185007
        case 46: {
          dlsym(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185006
        case 47: {
          drand48_r(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185005
        case 48: {
          execute_internal(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185004
        case 49: {
          exit(1);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478185003
        case 50: {
          explicit_bzero(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185002
        case 51: {
          fgetpos(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185001
        case 52: {
          fopen(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478185000
        case 53: {
          fprintf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184999
        case 54: {
          fputs(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184998
        case 55: {
          fscanf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184997
        case 56: {
          fsetpos(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184996
        case 57: {
          gc_remove_zval_from_buffer((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184995
        case 58: {
          gc_zval_check_possible_root(((tmp)));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184994
        case 59: {
          gc_zval_possible_root((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184993
        case 60: {
          getloadavg(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184992
        case 61: {
          i_zval_ptr_dtor((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184991
        case 62: {
          if (!(executor_globals.active_symbol_table)) {
            zend_rebuild_symbol_table();
          }
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184990
        case 63: {
          if (!__is_neg("19-63", 0, 0, 0, 0, 5,
                        (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                        "key", "target", "args"))
            zval_set_refcount_p(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184989
        case 64: {
          if (__is_neg("19-64", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 0;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184988
        case 65: {
          if (__is_neg("19-65", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 1;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184987
        case 66: {
          if (__is_neg("19-66", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 32;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184986
        case 67: {
          if (__is_neg("19-67", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 3;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184985
        case 68: {
          if (__is_neg("19-68", 0, 0, 0, 0, 5,
                       (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                       "key", "target", "args"))
            return 64;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184984
        case 69: {
          if (_zend_hash_quick_add_or_update(
                  target, key->arKey, key->nKeyLength, key->h, &tmp,
                  sizeof(zval *), ((void *)0), (1 << 1)) == 0) {
            zval_addref_p(tmp);
          }
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184983
        case 70: {
          index(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184982
        case 71: {
          init_trait_alias_list(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184981
        case 72: {
          instanceof_function(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184980
        case 73: {
          is_ref = (**(p)).type & 64;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184979
        case 74: {
          is_ref = (**p).type & 64;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184978
        case 75: {
          lcong48_r(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184977
        case 76: {
          longjmp(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184976
        case 77: {
          lrand48_r(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184975
        case 78: {
          mblen(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184974
        case 79: {
          mkstemps(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184973
        case 80: {
          mrand48_r(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184972
        case 81: {
          object_properties_init(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184971
        case 82: {
          on_exit(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184970
        case 83: {
          open_memstream(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184969
        case 84: {
          popen(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184968
        case 85: {
          print_op_array(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184967
        case 86: {
          printf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184966
        case 87: {
          random_r(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184965
        case 88: {
          realloc(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184964
        case 89: {
          realpath(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184963
        case 90: {
          rename(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184962
        case 91: {
          rindex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184961
        case 92: {
          scanf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184960
        case 93: {
          seed48_r(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184959
        case 94: {
          setbuf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184958
        case 95: {
          setstate_r(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184957
        case 96: {
          siglongjmp(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184956
        case 97: {
          sprintf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184955
        case 98: {
          sscanf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184954
        case 99: {
          stpcpy(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184953
        case 100: {
          strcasecmp(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184952
        case 101: {
          strcat(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184951
        case 102: {
          strchr(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184950
        case 103: {
          strcmp(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184949
        case 104: {
          strcoll(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184948
        case 105: {
          strcpy(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184947
        case 106: {
          strcspn(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184946
        case 107: {
          strndup(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184945
        case 108: {
          strnlen(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184944
        case 109: {
          strpbrk(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184943
        case 110: {
          strrchr(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184942
        case 111: {
          strsep(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184941
        case 112: {
          strspn(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184940
        case 113: {
          strstr(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184939
        case 114: {
          strtod(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184938
        case 115: {
          strtof(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184937
        case 116: {
          strtok(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184936
        case 117: {
          strtold(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184935
        case 118: {
          tempnam(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184934
        case 119: {
          tmp = (executor_globals.uninitialized_zval_ptr);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184933
        case 120: {
          tmp = *(p);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184932
        case 121: {
          tmp = *p;
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184931
        case 122: {
          vprintf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184930
        case 123: {
          vscanf(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184929
        case 124: {
          wctomb(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184928
        case 125: {
          zend_add_literal(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184927
        case 126: {
          zend_add_to_list(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184926
        case 127: {
          zend_atoi(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184925
        case 128: {
          zend_atol(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184924
        case 129: {
          zend_binary_zval_strcasecmp(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184923
        case 130: {
          zend_binary_zval_strcmp(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184922
        case 131: {
          zend_call_function(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184921
        case 132: {
          zend_check_protected(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184920
        case 133: {
          zend_class_implements(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184919
        case 134: {
          zend_compare_file_handles(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184918
        case 135: {
          zend_copy_constants(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184917
        case 136: {
          zend_delete_global_variable(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184916
        case 137: {
          zend_dirname(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184915
        case 138: {
          zend_disable_class(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184914
        case 139: {
          zend_disable_function(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184913
        case 140: {
          zend_do_begin_class_member_function_call(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184912
        case 141: {
          zend_do_begin_dynamic_function_call(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184911
        case 142: {
          zend_do_begin_function_call(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184910
        case 143: {
          zend_do_begin_namespace(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184909
        case 144: {
          zend_do_begin_new_object(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184908
        case 145: {
          zend_do_begin_qm_op(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184907
        case 146: {
          zend_do_boolean_and_begin(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184906
        case 147: {
          zend_do_boolean_or_begin(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184905
        case 148: {
          zend_do_case_after_statement(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184904
        case 149: {
          zend_do_clone(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184903
        case 150: {
          zend_do_declare_class_constant(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184902
        case 151: {
          zend_do_declare_constant(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184901
        case 152: {
          zend_do_declare_stmt(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184900
        case 153: {
          zend_do_default_before_statement(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184899
        case 154: {
          zend_do_end_class_declaration(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184898
        case 155: {
          zend_do_exit(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184897
        case 156: {
          zend_do_fetch_class(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184896
        case 157: {
          zend_do_fetch_lexical_variable(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184895
        case 158: {
          zend_do_fetch_static_member(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184894
        case 159: {
          zend_do_for_before_statement(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184893
        case 160: {
          zend_do_for_cond(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184892
        case 161: {
          zend_do_foreach_end(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184891
        case 162: {
          zend_do_if_after_statement(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184890
        case 163: {
          zend_do_if_cond(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184889
        case 164: {
          zend_do_implement_interface(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184888
        case 165: {
          zend_do_implement_trait(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184887
        case 166: {
          zend_do_inherit_interfaces(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184886
        case 167: {
          zend_do_inheritance(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184885
        case 168: {
          zend_do_list_end(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184884
        case 169: {
          zend_do_mark_last_catch(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184883
        case 170: {
          zend_do_print(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184882
        case 171: {
          zend_do_return(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184881
        case 172: {
          zend_do_shell_exec(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184880
        case 173: {
          zend_do_verify_access_types(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184879
        case 174: {
          zend_do_while_cond(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184878
        case 175: {
          zend_do_while_end(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184877
        case 176: {
          zend_error((1 << 3L), "A non well formed numeric value encountered");
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184876
        case 177: {
          zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184875
        case 178: {
          zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184874
        case 179: {
          zend_fcall_info_argn(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184873
        case 180: {
          zend_fcall_info_args(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184872
        case 181: {
          zend_fcall_info_args_clear(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184871
        case 182: {
          zend_get_class_fetch_type(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184870
        case 183: {
          zend_get_compiled_variable_value(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184869
        case 184: {
          zend_get_hash_value(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184868
        case 185: {
          zend_hash_apply(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184867
        case 186: {
          zend_hash_destroy((tmp)->value.ht);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184866
        case 187: {
          zend_hash_func(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184865
        case 188: {
          zend_hash_get_current_key_type_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184864
        case 189: {
          zend_hash_get_pointer(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184863
        case 190: {
          zend_hash_index_exists(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184862
        case 191: {
          zend_hash_internal_pointer_end_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184861
        case 192: {
          zend_hash_internal_pointer_reset_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184860
        case 193: {
          zend_hash_move_backwards_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184859
        case 194: {
          zend_hash_move_forward_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184858
        case 195: {
          zend_hash_reverse_apply(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184857
        case 196: {
          zend_hash_set_pointer(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184856
        case 197: {
          zend_hex_strtod(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184855
        case 198: {
          zend_init_list(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184854
        case 199: {
          zend_initialize_class_data(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184853
        case 200: {
          zend_inline_hash_func(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184852
        case 201: {
          zend_is_auto_global(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184851
        case 202: {
          zend_iterator_unwrap(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184850
        case 203: {
          zend_list_insert(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184849
        case 204: {
          zend_llist_add_element(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184848
        case 205: {
          zend_llist_apply(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184847
        case 206: {
          zend_llist_apply_with_del(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184846
        case 207: {
          zend_llist_copy(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184845
        case 208: {
          zend_llist_get_first_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184844
        case 209: {
          zend_llist_get_last_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184843
        case 210: {
          zend_llist_get_next_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184842
        case 211: {
          zend_llist_get_prev_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184841
        case 212: {
          zend_llist_prepend_element(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184840
        case 213: {
          zend_llist_sort(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184839
        case 214: {
          zend_make_callable(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184838
        case 215: {
          zend_multibyte_set_script_encoding(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184837
        case 216: {
          zend_multibyte_set_script_encoding_by_string(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184836
        case 217: {
          zend_object_create_proxy(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184835
        case 218: {
          zend_object_std_init(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184834
        case 219: {
          zend_object_store_set_object(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184833
        case 220: {
          zend_objects_destroy_object(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184832
        case 221: {
          zend_objects_new(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184831
        case 222: {
          zend_objects_store_init(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184830
        case 223: {
          zend_oct_strtod(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184829
        case 224: {
          zend_print_zval(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184828
        case 225: {
          zend_print_zval_r(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184827
        case 226: {
          zend_ptr_stack_apply(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184826
        case 227: {
          zend_ptr_stack_init_ex(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184825
        case 228: {
          zend_ptr_stack_n_pop(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184824
        case 229: {
          zend_ptr_stack_n_push(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184823
        case 230: {
          zend_ptr_stack_push(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184822
        case 231: {
          zend_quick_get_constant(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184821
        case 232: {
          zend_rebuild_symbol_table();
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184820
        case 233: {
          zend_resolve_non_class_name(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184819
        case 234: {
          zend_stack_top(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184818
        case 235: {
          zend_startup(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184817
        case 236: {
          zend_std_get_debug_info(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184816
        case 237: {
          zend_str_tolower(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184815
        case 238: {
          zend_str_tolower_dup(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184814
        case 239: {
          zend_stream_open(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184813
        case 240: {
          zend_string_to_double(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184812
        case 241: {
          zend_strndup(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184811
        case 242: {
          zend_strtod(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184810
        case 243: {
          zend_ts_hash_apply(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184809
        case 244: {
          zend_ts_hash_func(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184808
        case 245: {
          zend_ts_hash_index_exists(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184807
        case 246: {
          zend_ts_hash_reverse_apply(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184806
        case 247: {
          zval_addref_p((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184805
        case 248: {
          zval_addref_p(*((p)));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184804
        case 249: {
          zval_addref_p(tmp);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184803
        case 250: {
          zval_set_isref_p((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184802
        case 251: {
          zval_set_isref_p(*(((p))));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184801
        case 252: {
          zval_set_isref_p(*((p)));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184800
        case 253: {
          zval_set_isref_p(tmp);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184799
        case 254: {
          zval_set_isref_to_p(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184798
        case 255: {
          zval_set_refcount_p((tmp), 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184797
        case 256: {
          zval_set_refcount_p((tmp), 0);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184796
        case 257: {
          zval_set_refcount_p(tmp, 0);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184795
        case 258: {
          zval_unset_isref_p((tmp));
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184794
        case 259: {
          zval_unset_isref_p(tmp);
          Z_SET_REFCOUNT_P(tmp, 0);

          break;
        }
#endif
#ifdef COMPILE_ - 1478184793
        case 260: {
          zval_update_constant(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184792
        case 261: {
          zval_update_constant_inline_change(tmp, 0) break;
        }
#endif
#ifdef COMPILE_ - 1478184791
        case 262: {
          zval_update_constant_no_inline_change(tmp, 0) break;
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
switch (__choose("__ID20")) {
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#ifdef COMPILE_ - 1478184790
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184789
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184788
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184787
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184786
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184785
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
        _convert_to_string(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184784
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184783
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184782
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184781
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
        _object_init(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184780
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184779
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184778
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184777
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184776
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184775
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184774
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184773
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
        _zval_copy_ctor(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184772
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184771
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
        _zval_copy_ctor_func(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184770
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184769
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
        _zval_dtor(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184768
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184767
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
        _zval_dtor_func(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184766
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
        _zval_dtor_wrapper(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184765
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
        _zval_internal_dtor(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184764
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184763
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
        add_next_index_null(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184762
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
        convert_scalar_to_number(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184761
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
        convert_to_array(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184760
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184759
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
        convert_to_boolean(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184758
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
        convert_to_double(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184757
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
        convert_to_long(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184756
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
        convert_to_null(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184755
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
        convert_to_object(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184754
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
        decrement_function(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184753
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184752
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184751
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
        gc_remove_zval_from_buffer(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184750
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
        gc_zobj_possible_root(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184749
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184748
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
        gc_zval_check_possible_root(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184747
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184746
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
        gc_zval_possible_root(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184745
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
        i_zend_is_true(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184744
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184743
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
        i_zval_ptr_dtor(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184742
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184741
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
        if (!__is_neg("20-50", 0, 0, 0, 0, 5,
                      (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                      "key", "target", "args"))
          zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184740
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
        if (__is_neg("20-51", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                     "key", "target", "args"))
          return 0;
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184739
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
        if (__is_neg("20-52", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                     "key", "target", "args"))
          return 1;
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184738
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
        if (__is_neg("20-53", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                     "key", "target", "args"))
          return 32;
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184737
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
        if (__is_neg("20-54", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                     "key", "target", "args"))
          return 3;
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184736
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
        if (__is_neg("20-55", 0, 0, 0, 0, 5,
                     (void **){tmp, p, key, target, args}, 0, 0, "tmp", "p",
                     "key", "target", "args"))
          return 64;
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184735
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184734
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
        increment_function(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184733
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184732
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184731
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
        lex_scan(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184730
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
        safe_free_zval_ptr_rel(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184729
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184728
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184727
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184726
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184725
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184724
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184723
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184722
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
        zend_is_true(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184721
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
        zend_locale_sprintf_double(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184720
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
        zend_object_store_ctor_failed(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184719
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
        zend_objects_clone_obj(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184718
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
        zend_objects_store_add_ref(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184717
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
        zend_objects_store_clone_obj(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184716
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
        zend_objects_store_del_ref(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184715
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
        zend_objects_store_get_refcount(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184714
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
        zend_print_flat_zval_r(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184713
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
        zend_print_variable(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184712
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184711
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
        zend_std_get_constructor(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184710
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
        zend_std_get_properties(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184709
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184708
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184707
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
        zval_addref_p(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184706
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184705
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
        zval_delref_p(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184704
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
        zval_is_true(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184703
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
        zval_isref_p(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184702
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
        zval_refcount_p(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184701
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184700
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184699
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184698
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
        zval_set_isref_p(tmp)
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184697
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184696
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184695
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184694
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
        zval_unset_isref_p((tmp))
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184693
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184692
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184691
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184690
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184689
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184688
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184687
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184686
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184685
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184684
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184683
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184682
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184681
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184680
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184679
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184678
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184677
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184676
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184675
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184674
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184673
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184672
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184671
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184670
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184669
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184668
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184667
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184666
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184665
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
      } else if (!__is_neg("20-126", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, "p",
                           "tmp", "key", "target", "args") &&
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
#ifdef COMPILE_ - 1478184664
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
      } else if (!__is_neg("20-127", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, "p",
                           "tmp", "key", "target", "args"))
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
#ifdef COMPILE_ - 1478184663
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
                 !__is_neg("20-128", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, "p",
                           "tmp", "key", "target", "args")) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184662
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
                 __is_neg("20-129", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args")) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184661
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
      } else if (__is_neg("20-130", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 0;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184660
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
      } else if (__is_neg("20-131", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 1;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184659
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
      } else if (__is_neg("20-132", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 32;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184658
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
      } else if (__is_neg("20-133", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 3;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184657
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
      } else if (__is_neg("20-134", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 64;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184656
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184655
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184654
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184653
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184652
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184651
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184650
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184649
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184648
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184647
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184646
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184645
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184644
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184643
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184642
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184641
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184640
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184639
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184638
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184637
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184636
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184635
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184634
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184633
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184632
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184631
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184630
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184629
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184628
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184627
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184626
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184625
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184624
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184623
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184622
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184621
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184620
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184619
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184618
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184617
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184616
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184615
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184614
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184613
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184612
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184611
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184610
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184609
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184608
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184607
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184606
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184605
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184604
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184603
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184602
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
      } else if (!__is_neg("20-189", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, "p",
                           "tmp", "key", "target", "args") &&
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
#ifdef COMPILE_ - 1478184601
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
      } else if (!__is_neg("20-190", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, "p",
                           "tmp", "key", "target", "args"))
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
#ifdef COMPILE_ - 1478184600
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
                 !__is_neg("20-191", 0, 0, 0, 0, 5,
                           (void **){p, tmp, key, target, args}, 0, 0, "p",
                           "tmp", "key", "target", "args")) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184599
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
                 __is_neg("20-192", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args")) {
        do {
          (tmp) = (zval *)_emalloc((sizeof(zval_gc_info)));
          ((zval_gc_info *)(tmp))->u.buffered = ((void *)0);
        } while (0);
        *tmp = zval_used_for_init;
        ;
        ;
        do {
          (tmp)->value = (*p)->value;
          (*tmp).type = (**p).type;
        } while (0);
        zval_set_refcount_p(tmp, 0);
        zval_unset_isref_p(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184598
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
      } else if (__is_neg("20-193", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 0;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184597
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
      } else if (__is_neg("20-194", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 1;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184596
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
      } else if (__is_neg("20-195", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 32;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184595
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
      } else if (__is_neg("20-196", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 3;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184594
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
      } else if (__is_neg("20-197", 0, 0, 0, 0, 5,
                          (void **){p, tmp, key, target, args}, 0, 0, "p",
                          "tmp", "key", "target", "args"))
        return 64;
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184593
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184592
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184591
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184590
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184589
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184588
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184587
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184586
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184585
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184584
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184583
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184582
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184581
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184580
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184579
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184578
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184577
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184576
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184575
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184574
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184573
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184572
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184571
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184570
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184569
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184568
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184567
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184566
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
      if (Z_ISREF_PP(p)) {
        ALLOC_INIT_ZVAL(tmp);
        ZVAL_COPY_VALUE(tmp, *p);
        Z_SET_REFCOUNT_P(tmp, 0);
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184565
case 226: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184564
case 227: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    (tmp) = (executor_globals.uninitialized_zval_ptr);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184563
case 228: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    (tmp) = *p;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184562
case 229: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    *(tmp) = zval_used_for_init;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184561
case 230: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    *tmp = zval_used_for_init;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184560
case 231: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _efree(((tmp)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184559
case 232: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _efree(((tmp)->value.ht));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184558
case 233: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _efree(((tmp)->value.str.val));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184557
case 234: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   (key)->arKey, key->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184556
case 235: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, (key)->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184555
case 236: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, (key)->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184554
case 237: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, key->h, &(tmp),
                                   sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184553
case 238: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184552
case 239: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_list_addref((tmp)->value.lval);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184551
case 240: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zend_list_delete((tmp)->value.lval);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184550
case 241: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zval_copy_ctor_func((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184549
case 242: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zval_dtor(((tmp)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184548
case 243: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zval_dtor_func((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184547
case 244: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    _zval_ptr_dtor((&(tmp)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184546
case 245: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    convert_to_boolean((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184545
case 246: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    exit(1);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184544
case 247: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    gc_remove_zval_from_buffer((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184543
case 248: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    gc_zval_check_possible_root(((tmp)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184542
case 249: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    gc_zval_possible_root((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184541
case 250: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    i_zval_ptr_dtor((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184540
case 251: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184539
case 252: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (!__is_neg("20-252", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                  0, 0, "p", "key", "tmp", "args", "target") &&
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
#ifdef COMPILE_ - 1478184538
case 253: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (!__is_neg("20-253", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                  0, 0, "p", "key", "tmp", "args", "target"))
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
#ifdef COMPILE_ - 1478184537
case 254: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if ((zend_hash_quick_find((executor_globals.active_symbol_table),
                              key->arKey, key->nKeyLength, key->h,
                              (void **)&p) == -1) &&
        !__is_neg("20-254", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                  0, 0, "p", "key", "tmp", "args", "target")) {
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
#ifdef COMPILE_ - 1478184536
case 255: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if ((zend_hash_quick_find((executor_globals.active_symbol_table),
                              key->arKey, key->nKeyLength, key->h,
                              (void **)&p) == -1) ||
        __is_neg("20-255", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, "p", "key", "tmp", "args", "target")) {
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
#ifdef COMPILE_ - 1478184535
case 256: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("20-256", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, "p", "key", "tmp", "args", "target"))
      return 0;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184534
case 257: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("20-257", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, "p", "key", "tmp", "args", "target"))
      return 1;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184533
case 258: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("20-258", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, "p", "key", "tmp", "args", "target"))
      return 32;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184532
case 259: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("20-259", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, "p", "key", "tmp", "args", "target"))
      return 3;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184531
case 260: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    if (__is_neg("20-260", 0, 0, 0, 0, 5, (void **){p, key, tmp, args, target},
                 0, 0, "p", "key", "tmp", "args", "target"))
      return 64;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184530
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184529
case 262: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    is_ref = (**(p)).type & 64;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184528
case 263: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    is_ref = (**p).type & 64;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184527
case 264: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&((zval_gc_info *)(tmp))->u, 0,
           sizeof(*(&((zval_gc_info *)(tmp))->u)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184526
case 265: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&(**(p)), 0, sizeof(*(&(**(p)))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184525
case 266: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&(**p), 0, sizeof(*(&(**p))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184524
case 267: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&(*tmp), 0, sizeof(*(&(*tmp))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184523
case 268: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(&executor_globals, 0, sizeof(*(&executor_globals)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184522
case 269: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(((zval_gc_info *)(tmp)), 0, sizeof(*(((zval_gc_info *)(tmp)))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184521
case 270: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset((*(p)), 0, sizeof(*((*(p)))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184520
case 271: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset((*p), 0, sizeof(*((*p))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184519
case 272: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset((tmp), 0, sizeof(*((tmp))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184518
case 273: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    memset(key, 0, sizeof(*(key)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184517
case 274: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    tmp = (executor_globals.uninitialized_zval_ptr);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184516
case 275: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    tmp = *(p);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184515
case 276: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    tmp = *p;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184514
case 277: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_error((1 << 3L), "A non well formed numeric value encountered");
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184513
case 278: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184512
case 279: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184511
case 280: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_hash_destroy((tmp)->value.ht);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184510
case 281: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zend_rebuild_symbol_table();
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184509
case 282: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_addref_p((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184508
case 283: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_addref_p(*((p)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184507
case 284: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_addref_p(tmp);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184506
case 285: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_isref_p((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184505
case 286: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_isref_p(*(((p))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184504
case 287: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_isref_p(*((p)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184503
case 288: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_isref_p(tmp);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184502
case 289: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_refcount_p((tmp), 0);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184501
case 290: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_set_refcount_p(tmp, 0);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184500
case 291: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_unset_isref_p((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184499
case 292: {
  if (Z_TYPE_PP(p) & (IS_LEXICAL_VAR | IS_LEXICAL_REF)) {
    is_ref = Z_TYPE_PP(p) & IS_LEXICAL_REF;

    if (!EG(active_symbol_table)) {
      zend_rebuild_symbol_table(TSRMLS_C);
    }
    zval_unset_isref_p(tmp);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184498
case 293: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184497
case 294: {
  (tmp) = (executor_globals.uninitialized_zval_ptr);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184496
case 295: {
  (tmp) = *p;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184495
case 296: {
  *(tmp) = zval_used_for_init;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184494
case 297: {
  *tmp = zval_used_for_init;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184493
case 298: {
  _efree(((tmp)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184492
case 299: {
  _efree(((tmp)->value.ht));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184491
case 300: {
  _efree(((tmp)->value.str.val));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184490
case 301: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 (key)->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184489
case 302: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, (key)->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184488
case 303: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, (key)->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184487
case 304: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &(tmp),
                                 sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184486
case 305: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184485
case 306: {
  _zend_list_addref((tmp)->value.lval);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184484
case 307: {
  _zend_list_delete((tmp)->value.lval);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184483
case 308: {
  _zval_copy_ctor_func((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184482
case 309: {
  _zval_dtor(((tmp)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184481
case 310: {
  _zval_dtor_func((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184480
case 311: {
  _zval_ptr_dtor((&(tmp)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184479
case 312: {
  convert_to_boolean((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184478
case 313: {
  exit(1);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184477
case 314: {
  gc_remove_zval_from_buffer((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184476
case 315: {
  gc_zval_check_possible_root(((tmp)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184475
case 316: {
  gc_zval_possible_root((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184474
case 317: {
  i_zval_ptr_dtor((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184473
case 318: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184472
case 319: {
  if (!__is_neg("20-319", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                0, 0, "p", "args", "key", "tmp", "target") &&
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
#ifdef COMPILE_ - 1478184471
case 320: {
  if (!__is_neg("20-320", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                0, 0, "p", "args", "key", "tmp", "target"))
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
#ifdef COMPILE_ - 1478184470
case 321: {
  if (((**p).type & (32 | 64)) &&
      !__is_neg("20-321", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target},
                0, 0, "p", "args", "key", "tmp", "target")) {
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
#ifdef COMPILE_ - 1478184469
case 322: {
  if (((**p).type & (32 | 64)) ||
      __is_neg("20-322", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, "p", "args", "key", "tmp", "target")) {
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
#ifdef COMPILE_ - 1478184468
case 323: {
  if (__is_neg("20-323", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, "p", "args", "key", "tmp", "target"))
    return 0;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184467
case 324: {
  if (__is_neg("20-324", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, "p", "args", "key", "tmp", "target"))
    return 1;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184466
case 325: {
  if (__is_neg("20-325", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, "p", "args", "key", "tmp", "target"))
    return 32;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184465
case 326: {
  if (__is_neg("20-326", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, "p", "args", "key", "tmp", "target"))
    return 3;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184464
case 327: {
  if (__is_neg("20-327", 0, 0, 0, 0, 5, (void **){p, args, key, tmp, target}, 0,
               0, "p", "args", "key", "tmp", "target"))
    return 64;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184463
case 328: {
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184462
case 329: {
  is_ref = (**(p)).type & 64;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184461
case 330: {
  is_ref = (**p).type & 64;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184460
case 331: {
  memset(&((zval_gc_info *)(tmp))->u, 0,
         sizeof(*(&((zval_gc_info *)(tmp))->u)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184459
case 332: {
  memset(&(**(p)), 0, sizeof(*(&(**(p)))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184458
case 333: {
  memset(&(**p), 0, sizeof(*(&(**p))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184457
case 334: {
  memset(&(*tmp), 0, sizeof(*(&(*tmp))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184456
case 335: {
  memset(&executor_globals, 0, sizeof(*(&executor_globals)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184455
case 336: {
  memset(((zval_gc_info *)(tmp)), 0, sizeof(*(((zval_gc_info *)(tmp)))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184454
case 337: {
  memset((*(p)), 0, sizeof(*((*(p)))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184453
case 338: {
  memset((*p), 0, sizeof(*((*p))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184452
case 339: {
  memset((tmp), 0, sizeof(*((tmp))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184451
case 340: {
  memset(key, 0, sizeof(*(key)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184450
case 341: {
  tmp = (executor_globals.uninitialized_zval_ptr);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184449
case 342: {
  tmp = *(p);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184448
case 343: {
  tmp = *p;
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184447
case 344: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184446
case 345: {
  zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184445
case 346: {
  zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184444
case 347: {
  zend_hash_destroy((tmp)->value.ht);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184443
case 348: {
  zend_rebuild_symbol_table();
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184442
case 349: {
  zval_addref_p((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184441
case 350: {
  zval_addref_p(*((p)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184440
case 351: {
  zval_addref_p(tmp);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184439
case 352: {
  zval_set_isref_p((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184438
case 353: {
  zval_set_isref_p(*(((p))));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184437
case 354: {
  zval_set_isref_p(*((p)));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184436
case 355: {
  zval_set_isref_p(tmp);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184435
case 356: {
  zval_set_refcount_p((tmp), 0);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184434
case 357: {
  zval_set_refcount_p(tmp, 0);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184433
case 358: {
  zval_unset_isref_p((tmp));
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
        Z_UNSET_ISREF_P(tmp);
      } else {
        tmp = *p;
      }
    }
  } else {
    tmp = *p;
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184432
case 359: {
  zval_unset_isref_p(tmp);
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
        Z_UNSET_ISREF_P(tmp);
      } else {
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
switch (__choose("__ID21")) {
case 0: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#ifdef COMPILE_ - 1478184431
case 1: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184430
case 2: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    (tmp) = (executor_globals.uninitialized_zval_ptr);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184429
case 3: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    (tmp) = *p;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184428
case 4: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    *(tmp) = zval_used_for_init;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184427
case 5: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    *tmp = zval_used_for_init;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184426
case 6: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _convert_to_string(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184425
case 7: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _efree(((tmp)));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184424
case 8: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _efree(((tmp)->value.ht));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184423
case 9: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _efree(((tmp)->value.str.val));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184422
case 10: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _object_init(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184421
case 11: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   (key)->arKey, key->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184420
case 12: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, (key)->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184419
case 13: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, (key)->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184418
case 14: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, key->h, &(tmp),
                                   sizeof(zval *), (void **)&p, (1 << 1));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184417
case 15: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                   key->arKey, key->nKeyLength, key->h, &tmp,
                                   sizeof(zval *), (void **)&p, (1 << 1));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184416
case 16: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_list_addref((tmp)->value.lval);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184415
case 17: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zend_list_delete((tmp)->value.lval);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184414
case 18: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_copy_ctor(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184413
case 19: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_copy_ctor_func((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184412
case 20: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_copy_ctor_func(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184411
case 21: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor(((tmp)));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184410
case 22: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184409
case 23: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor_func((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184408
case 24: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor_func(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184407
case 25: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_dtor_wrapper(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184406
case 26: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_internal_dtor(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184405
case 27: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    _zval_ptr_dtor((&(tmp)));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184404
case 28: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    add_next_index_null(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184403
case 29: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_scalar_to_number(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184402
case 30: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_array(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184401
case 31: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_boolean((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184400
case 32: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_boolean(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184399
case 33: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_double(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184398
case 34: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_long(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184397
case 35: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_null(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184396
case 36: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    convert_to_object(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184395
case 37: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    decrement_function(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184394
case 38: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    exit(1);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184393
case 39: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_remove_zval_from_buffer((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184392
case 40: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_remove_zval_from_buffer(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184391
case 41: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zobj_possible_root(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184390
case 42: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zval_check_possible_root(((tmp)));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184389
case 43: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zval_check_possible_root(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184388
case 44: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zval_possible_root((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184387
case 45: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    gc_zval_possible_root(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184386
case 46: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    i_zend_is_true(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184385
case 47: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    i_zval_ptr_dtor((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184384
case 48: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    i_zval_ptr_dtor(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184383
case 49: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (!(executor_globals.active_symbol_table)) {
      zend_rebuild_symbol_table();
    }
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184382
case 50: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (!__is_neg("21-50", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                  0, 0, "tmp", "key", "target", "p", "args"))
      zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184381
case 51: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("21-51", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, "tmp", "key", "target", "p", "args"))
      return 0;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184380
case 52: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("21-52", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, "tmp", "key", "target", "p", "args"))
      return 1;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184379
case 53: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("21-53", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, "tmp", "key", "target", "p", "args"))
      return 32;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184378
case 54: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("21-54", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, "tmp", "key", "target", "p", "args"))
      return 3;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184377
case 55: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (__is_neg("21-55", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args},
                 0, 0, "tmp", "key", "target", "p", "args"))
      return 64;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184376
case 56: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                       key->h, &tmp, sizeof(zval *),
                                       ((void *)0), (1 << 1)) == 0) {
      zval_addref_p(tmp);
    }
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184375
case 57: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    increment_function(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184374
case 58: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    is_ref = (**(p)).type & 64;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184373
case 59: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    is_ref = (**p).type & 64;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184372
case 60: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    lex_scan(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184371
case 61: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    safe_free_zval_ptr_rel(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184370
case 62: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    tmp = (executor_globals.uninitialized_zval_ptr);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184369
case 63: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    tmp = *(p);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184368
case 64: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    tmp = *p;
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184367
case 65: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184366
case 66: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184365
case 67: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184364
case 68: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_hash_destroy((tmp)->value.ht);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184363
case 69: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_is_true(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184362
case 70: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_locale_sprintf_double(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184361
case 71: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_object_store_ctor_failed(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184360
case 72: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_clone_obj(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184359
case 73: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_store_add_ref(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184358
case 74: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_store_clone_obj(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184357
case 75: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_store_del_ref(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184356
case 76: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_objects_store_get_refcount(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184355
case 77: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_print_flat_zval_r(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184354
case 78: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_print_variable(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184353
case 79: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_rebuild_symbol_table();
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184352
case 80: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_std_get_constructor(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184351
case 81: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zend_std_get_properties(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184350
case 82: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p((tmp))
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184349
case 83: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184348
case 84: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p(*((p)));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184347
case 85: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_addref_p(tmp);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184346
case 86: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_delref_p(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184345
case 87: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_is_true(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184344
case 88: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_isref_p(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184343
case 89: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_refcount_p(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184342
case 90: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184341
case 91: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p(*(((p))));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184340
case 92: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p(*((p)));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184339
case 93: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184338
case 94: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_isref_p(tmp);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184337
case 95: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_refcount_p((tmp), 0);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184336
case 96: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_set_refcount_p(tmp, 0);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184335
case 97: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_unset_isref_p((tmp));
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184334
case 98: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_unset_isref_p(tmp)
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184333
case 99: {
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    zval_unset_isref_p(tmp);
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184332
case 100: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184331
case 101: {
  (tmp) = (executor_globals.uninitialized_zval_ptr);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184330
case 102: {
  (tmp) = *p;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184329
case 103: {
  *(tmp) = zval_used_for_init;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184328
case 104: {
  *tmp = zval_used_for_init;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184327
case 105: {
  _efree(((tmp)));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184326
case 106: {
  _efree(((tmp)->value.ht));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184325
case 107: {
  _efree(((tmp)->value.str.val));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184324
case 108: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 (key)->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184323
case 109: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, (key)->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184322
case 110: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, (key)->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184321
case 111: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &(tmp),
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184320
case 112: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184319
case 113: {
  _zend_list_addref((tmp)->value.lval);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184318
case 114: {
  _zend_list_delete((tmp)->value.lval);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184317
case 115: {
  _zval_copy_ctor_func((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184316
case 116: {
  _zval_dtor(((tmp)));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184315
case 117: {
  _zval_dtor_func((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184314
case 118: {
  _zval_ptr_dtor((&(tmp)));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184313
case 119: {
  convert_to_boolean((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184312
case 120: {
  exit(1);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184311
case 121: {
  gc_remove_zval_from_buffer((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184310
case 122: {
  gc_zval_check_possible_root(((tmp)));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184309
case 123: {
  gc_zval_possible_root((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184308
case 124: {
  i_zval_ptr_dtor((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184307
case 125: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184306
case 126: {
  if (!__is_neg("21-126", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args},
                0, 0, "key", "target", "tmp", "p", "args") &&
      (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                      key->h, &tmp, sizeof(zval *), ((void *)0),
                                      (1 << 1)) == 0)) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184305
case 127: {
  if (!__is_neg("21-127", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args},
                0, 0, "key", "target", "tmp", "p", "args"))
    if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                       key->h, &tmp, sizeof(zval *),
                                       ((void *)0), (1 << 1)) == 0) {
      zval_addref_p(tmp);
    }

  break;
}
#endif
#ifdef COMPILE_ - 1478184304
case 128: {
  if ((_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                      key->h, &tmp, sizeof(zval *), ((void *)0),
                                      (1 << 1)) == 0) &&
      !__is_neg("21-128", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args},
                0, 0, "key", "target", "tmp", "p", "args")) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184303
case 129: {
  if ((_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                      key->h, &tmp, sizeof(zval *), ((void *)0),
                                      (1 << 1)) == 0) ||
      __is_neg("21-129", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, "key", "target", "tmp", "p", "args")) {
    zval_addref_p(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184302
case 130: {
  if (__is_neg("21-130", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, "key", "target", "tmp", "p", "args"))
    return 0;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184301
case 131: {
  if (__is_neg("21-131", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, "key", "target", "tmp", "p", "args"))
    return 1;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184300
case 132: {
  if (__is_neg("21-132", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, "key", "target", "tmp", "p", "args"))
    return 32;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184299
case 133: {
  if (__is_neg("21-133", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, "key", "target", "tmp", "p", "args"))
    return 3;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184298
case 134: {
  if (__is_neg("21-134", 0, 0, 0, 0, 5, (void **){key, target, tmp, p, args}, 0,
               0, "key", "target", "tmp", "p", "args"))
    return 64;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184297
case 135: {
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184296
case 136: {
  is_ref = (**(p)).type & 64;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184295
case 137: {
  is_ref = (**p).type & 64;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184294
case 138: {
  memset(key, 0, sizeof(*(key)));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184293
case 139: {
  tmp = (executor_globals.uninitialized_zval_ptr);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184292
case 140: {
  tmp = *(p);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184291
case 141: {
  tmp = *p;
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184290
case 142: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184289
case 143: {
  zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184288
case 144: {
  zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184287
case 145: {
  zend_hash_destroy((tmp)->value.ht);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184286
case 146: {
  zend_rebuild_symbol_table();
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184285
case 147: {
  zval_addref_p((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184284
case 148: {
  zval_addref_p(*((p)));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184283
case 149: {
  zval_addref_p(tmp);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184282
case 150: {
  zval_set_isref_p((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184281
case 151: {
  zval_set_isref_p(*(((p))));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184280
case 152: {
  zval_set_isref_p(*((p)));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184279
case 153: {
  zval_set_isref_p(tmp);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184278
case 154: {
  zval_set_refcount_p((tmp), 0);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184277
case 155: {
  zval_set_refcount_p(tmp, 0);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184276
case 156: {
  zval_unset_isref_p((tmp));
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
#ifdef COMPILE_ - 1478184275
case 157: {
  zval_unset_isref_p(tmp);
  if (zend_hash_quick_add(target, key->arKey, key->nKeyLength, key->h, &tmp,
                          sizeof(zval *), NULL) == SUCCESS) {
    Z_ADDREF_P(tmp);
  }

  break;
}
#endif
}
// prophet generated patch
switch (__choose("__ID22")) {
case 0: {
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#ifdef COMPILE_ - 1478184274
case 1: {
  (executor_globals.argument_stack) =
      zend_vm_stack_new_page(((16 * 1024) - 16));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184273
case 2: {
  (tmp) = (executor_globals.uninitialized_zval_ptr);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184272
case 3: {
  (tmp) = *p;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184271
case 4: {
  *(tmp) = zval_used_for_init;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184270
case 5: {
  *tmp = zval_used_for_init;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184269
case 6: {
  _efree(((tmp)));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184268
case 7: {
  _efree(((tmp)->value.ht));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184267
case 8: {
  _efree(((tmp)->value.str.val));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184266
case 9: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 (key)->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184265
case 10: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, (key)->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184264
case 11: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, (key)->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184263
case 12: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &(tmp),
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184262
case 13: {
  _zend_hash_quick_add_or_update((executor_globals.active_symbol_table),
                                 key->arKey, key->nKeyLength, key->h, &tmp,
                                 sizeof(zval *), (void **)&p, (1 << 1));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184261
case 14: {
  _zend_list_addref((tmp)->value.lval);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184260
case 15: {
  _zend_list_delete((tmp)->value.lval);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184259
case 16: {
  _zval_copy_ctor_func((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184258
case 17: {
  _zval_dtor(((tmp)));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184257
case 18: {
  _zval_dtor_func((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184256
case 19: {
  _zval_ptr_dtor((&(tmp)));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184255
case 20: {
  convert_to_boolean((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184254
case 21: {
  exit(1);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184253
case 22: {
  gc_remove_zval_from_buffer((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184252
case 23: {
  gc_zval_check_possible_root(((tmp)));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184251
case 24: {
  gc_zval_possible_root((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184250
case 25: {
  i_zval_ptr_dtor((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184249
case 26: {
  if (!(executor_globals.active_symbol_table)) {
    zend_rebuild_symbol_table();
  }
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184248
case 27: {
  if (!__is_neg("22-27", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
                0, "tmp", "key", "target", "p", "args"))
    return 0;

  break;
}
#endif
#ifdef COMPILE_ - 1478184247
case 28: {
  if (__is_neg("22-28", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, "tmp", "key", "target", "p", "args"))
    return 0;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184246
case 29: {
  if (__is_neg("22-29", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, "tmp", "key", "target", "p", "args"))
    return 1;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184245
case 30: {
  if (__is_neg("22-30", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, "tmp", "key", "target", "p", "args"))
    return 32;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184244
case 31: {
  if (__is_neg("22-31", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, "tmp", "key", "target", "p", "args"))
    return 3;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184243
case 32: {
  if (__is_neg("22-32", 0, 0, 0, 0, 5, (void **){tmp, key, target, p, args}, 0,
               0, "tmp", "key", "target", "p", "args"))
    return 64;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184242
case 33: {
  if (_zend_hash_quick_add_or_update(target, key->arKey, key->nKeyLength,
                                     key->h, &tmp, sizeof(zval *), ((void *)0),
                                     (1 << 1)) == 0) {
    zval_addref_p(tmp);
  }
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184241
case 34: {
  is_ref = (**(p)).type & 64;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184240
case 35: {
  is_ref = (**p).type & 64;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184239
case 36: {
  tmp = (executor_globals.uninitialized_zval_ptr);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184238
case 37: {
  tmp = *(p);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184237
case 38: {
  tmp = *p;
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184236
case 39: {
  zend_error((1 << 3L), "A non well formed numeric value encountered");
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184235
case 40: {
  zend_error((1 << 3L), "Undefined variable: %s", (key)->arKey);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184234
case 41: {
  zend_error((1 << 3L), "Undefined variable: %s", key->arKey);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184233
case 42: {
  zend_hash_destroy((tmp)->value.ht);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184232
case 43: {
  zend_rebuild_symbol_table();
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184231
case 44: {
  zval_addref_p((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184230
case 45: {
  zval_addref_p(*((p)));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184229
case 46: {
  zval_addref_p(tmp);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184228
case 47: {
  zval_set_isref_p((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184227
case 48: {
  zval_set_isref_p(*(((p))));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184226
case 49: {
  zval_set_isref_p(*((p)));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184225
case 50: {
  zval_set_isref_p(tmp);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184224
case 51: {
  zval_set_refcount_p((tmp), 0);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184223
case 52: {
  zval_set_refcount_p(tmp, 0);
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184222
case 53: {
  zval_unset_isref_p((tmp));
  return ZEND_HASH_APPLY_KEEP;

  break;
}
#endif
#ifdef COMPILE_ - 1478184221
case 54: {
  zval_unset_isref_p(tmp);
  return ZEND_HASH_APPLY_KEEP;

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
