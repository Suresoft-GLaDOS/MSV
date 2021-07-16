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

#include <stdio.h>

#include "zend.h"
#include "zend_API.h"
#include "zend_alloc.h"
#include "zend_compile.h"
#include "zend_extensions.h"

#include "zend_vm.h"

static void
zend_extension_op_array_ctor_handler(zend_extension *extension,
                                     zend_op_array *op_array TSRMLS_DC) {
  if (extension->op_array_ctor) {
    extension->op_array_ctor(op_array);
  }
}

static void
zend_extension_op_array_dtor_handler(zend_extension *extension,
                                     zend_op_array *op_array TSRMLS_DC) {
  if (extension->op_array_dtor) {
    extension->op_array_dtor(op_array);
  }
}

static void op_array_alloc_ops(zend_op_array *op_array, zend_uint size) {
  op_array->opcodes = erealloc(op_array->opcodes, size * sizeof(zend_op));
}

void init_op_array(zend_op_array *op_array, zend_uchar type,
                   int initial_ops_size TSRMLS_DC) {
  op_array->type = type;

  if (CG(interactive)) {
    /* We must avoid a realloc() on the op_array in interactive mode, since
     * pointers to constants will become invalid
     */
    initial_ops_size = INITIAL_INTERACTIVE_OP_ARRAY_SIZE;
  }

  op_array->refcount = (zend_uint *)emalloc(sizeof(zend_uint));
  *op_array->refcount = 1;
  op_array->last = 0;
  op_array->opcodes = NULL;
  op_array_alloc_ops(op_array, initial_ops_size);

  op_array->last_var = 0;
  op_array->vars = NULL;

  op_array->T = 0;

  op_array->function_name = NULL;
  op_array->filename = zend_get_compiled_filename(TSRMLS_C);
  op_array->doc_comment = NULL;
  op_array->doc_comment_len = 0;

  op_array->arg_info = NULL;
  op_array->num_args = 0;
  op_array->required_num_args = 0;

  op_array->scope = NULL;

  op_array->brk_cont_array = NULL;
  op_array->try_catch_array = NULL;
  op_array->last_brk_cont = 0;

  op_array->static_variables = NULL;
  op_array->last_try_catch = 0;

  op_array->this_var = -1;

  op_array->fn_flags = CG(interactive) ? ZEND_ACC_INTERACTIVE : 0;

  op_array->early_binding = -1;

  op_array->last_literal = 0;
  op_array->literals = NULL;

  op_array->run_time_cache = NULL;
  op_array->last_cache_slot = 0;

  memset(op_array->reserved, 0, ZEND_MAX_RESERVED_RESOURCES * sizeof(void *));

  zend_llist_apply_with_argument(
      &zend_extensions,
      (llist_apply_with_arg_func_t)zend_extension_op_array_ctor_handler,
      op_array TSRMLS_CC);
}

ZEND_API void destroy_zend_function(zend_function *function TSRMLS_DC) {
  switch (function->type) {
  case ZEND_USER_FUNCTION:
    destroy_op_array((zend_op_array *)function TSRMLS_CC);
    break;
  case ZEND_INTERNAL_FUNCTION:
    /* do nothing */
    break;
  }
}

ZEND_API void zend_function_dtor(zend_function *function) {
  TSRMLS_FETCH();

  destroy_zend_function(function TSRMLS_CC);
}

static void zend_cleanup_op_array_data(zend_op_array *op_array) {
  // prophet generated patch
  {
    switch (__choose("__SWITCH188")) {
    case 0:
      break;
#ifdef COMPILE_19660
    case 1: {
      // AddAndReplaceKind
      (compiler_globals.context).literals_size = (op_array)->last_literal;

      break;
    }
#endif
#ifdef COMPILE_19661
    case 2: {
      // AddAndReplaceKind
      (compiler_globals.context).opcodes_size = (op_array)->last;

      break;
    }
#endif
#ifdef COMPILE_19662
    case 3: {
      // AddAndReplaceKind
      (compiler_globals.context).vars_size = (op_array)->last_var;

      break;
    }
#endif
#ifdef COMPILE_19663
    case 4: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_19664
    case 5: {
      // AddAndReplaceKind
      (op_array)->last_brk_cont++;

      break;
    }
#endif
#ifdef COMPILE_19665
    case 6: {
      // AddAndReplaceKind
      _efree(((op_array)->arg_info));

      break;
    }
#endif
#ifdef COMPILE_19666
    case 7: {
      // AddAndReplaceKind
      _efree(((op_array)->brk_cont_array));

      break;
    }
#endif
#ifdef COMPILE_19667
    case 8: {
      // AddAndReplaceKind
      _efree(((op_array)->doc_comment));

      break;
    }
#endif
#ifdef COMPILE_19668
    case 9: {
      // AddAndReplaceKind
      _efree(((op_array)->function_name));

      break;
    }
#endif
#ifdef COMPILE_19669
    case 10: {
      // AddAndReplaceKind
      _efree(((op_array)->literals));

      break;
    }
#endif
#ifdef COMPILE_19670
    case 11: {
      // AddAndReplaceKind
      _efree(((op_array)->opcodes));

      break;
    }
#endif
#ifdef COMPILE_19671
    case 12: {
      // AddAndReplaceKind
      _efree(((op_array)->refcount));

      break;
    }
#endif
#ifdef COMPILE_19672
    case 13: {
      // AddAndReplaceKind
      _efree(((op_array)->run_time_cache));

      break;
    }
#endif
#ifdef COMPILE_19673
    case 14: {
      // AddAndReplaceKind
      _efree(((op_array)->static_variables));

      break;
    }
#endif
#ifdef COMPILE_19674
    case 15: {
      // AddAndReplaceKind
      _efree(((op_array)->try_catch_array));

      break;
    }
#endif
#ifdef COMPILE_19675
    case 16: {
      // AddAndReplaceKind
      _efree(((op_array)->vars));

      break;
    }
#endif
#ifdef COMPILE_19676
    case 17: {
      // AddAndReplaceKind
      _zend_bailout("Zend/zend_opcode.c", 432);

      break;
    }
#endif
#ifdef COMPILE_19677
    case 18: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_19678
    case 19: {
      // AddAndReplaceKind
      if (op_array->static_variables) {
        zend_hash_clean(op_array->static_variables);
      }

      break;
    }
#endif
#ifdef COMPILE_19679
    case 20: {
      // AddAndReplaceKind
      memset((op_array)->reserved, 0, 4 * sizeof(void *));

      break;
    }
#endif
#ifdef COMPILE_19680
    case 21: {
      // AddAndReplaceKind
      op_array_alloc_ops((op_array), (compiler_globals.context).opcodes_size);

      break;
    }
#endif
#ifdef COMPILE_19681
    case 22: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_19682
    case 23: {
      // AddAndReplaceKind
      zend_hash_clean((op_array)->static_variables);

      break;
    }
#endif
#ifdef COMPILE_19683
    case 24: {
      // AddAndReplaceKind
      zend_hash_clean(op_array->static_variables);

      break;
    }
#endif
#ifdef COMPILE_19684
    case 25: {
      // AddAndReplaceKind
      zend_hash_destroy((op_array)->static_variables);

      break;
    }
#endif
#ifdef COMPILE_19685
    case 26: {
      // AddAndReplaceKind
      zend_llist_apply_with_argument(
          &zend_extensions,
          (llist_apply_with_arg_func_t)zend_extension_op_array_ctor_handler,
          (op_array));

      break;
    }
#endif
#ifdef COMPILE_19686
    case 27: {
      // AddAndReplaceKind
      zend_llist_apply_with_argument(
          &zend_extensions,
          (llist_apply_with_arg_func_t)zend_extension_op_array_dtor_handler,
          (op_array));

      break;
    }
#endif
#ifdef COMPILE_19687
    case 28: {
      // AddAndReplaceKind
      zend_llist_apply_with_argument(
          &zend_extensions,
          (llist_apply_with_arg_func_t)zend_extension_op_array_handler,
          (op_array));

      break;
    }
#endif
#ifdef COMPILE_19688
    case 29: {
      // AddAndReplaceKind
      zend_printf("Ran out of opcode space!\nYou should probably consider "
                  "writing this huge script into a file!\n");

      break;
    }
#endif
#ifdef COMPILE_19689
    case 30: {
      // AddAndReplaceKind
      zend_update_extended_info((op_array));

      break;
    }
#endif
    }
    {
      long long __temp189 = (op_array->static_variables);
      switch (__choose("__SWITCH189")) {
      case 0: {

        break;
      }
#ifdef COMPILE_19690
      case 1: {
        __temp189 =
            ((op_array->static_variables) &&
             !__is_neg(
                 "189-1", 31, &(op_array), sizeof(op_array),
                 &(op_array->static_variables),
                 sizeof(op_array->static_variables), &(op_array->type),
                 sizeof(op_array->type), &(op_array->function_name),
                 sizeof(op_array->function_name), &(op_array->scope),
                 sizeof(op_array->scope), &(op_array->fn_flags),
                 sizeof(op_array->fn_flags), &(op_array->prototype),
                 sizeof(op_array->prototype), &(op_array->num_args),
                 sizeof(op_array->num_args), &(op_array->required_num_args),
                 sizeof(op_array->required_num_args), &(op_array->arg_info),
                 sizeof(op_array->arg_info), &(op_array->refcount),
                 sizeof(op_array->refcount), &(op_array->opcodes),
                 sizeof(op_array->opcodes), &(op_array->last),
                 sizeof(op_array->last), &(op_array->vars),
                 sizeof(op_array->vars), &(op_array->last_var),
                 sizeof(op_array->last_var), &(op_array->T),
                 sizeof(op_array->T), &(op_array->brk_cont_array),
                 sizeof(op_array->brk_cont_array), &(op_array->last_brk_cont),
                 sizeof(op_array->last_brk_cont), &(op_array->try_catch_array),
                 sizeof(op_array->try_catch_array), &(op_array->last_try_catch),
                 sizeof(op_array->last_try_catch), &(op_array->this_var),
                 sizeof(op_array->this_var), &(op_array->filename),
                 sizeof(op_array->filename), &(op_array->line_start),
                 sizeof(op_array->line_start), &(op_array->line_end),
                 sizeof(op_array->line_end), &(op_array->doc_comment),
                 sizeof(op_array->doc_comment), &(op_array->doc_comment_len),
                 sizeof(op_array->doc_comment_len), &(op_array->early_binding),
                 sizeof(op_array->early_binding), &(op_array->literals),
                 sizeof(op_array->literals), &(op_array->last_literal),
                 sizeof(op_array->last_literal), &(op_array->run_time_cache),
                 sizeof(op_array->run_time_cache), &(op_array->last_cache_slot),
                 sizeof(op_array->last_cache_slot)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH192")) {
      case 0: {
        if (__temp189) {
          {
            switch (__choose("__SWITCH190")) {
            case 0:
              break;
#ifdef COMPILE_19691
            case 1: {
              // AddAndReplaceKind
              (compiler_globals.context).literals_size =
                  (op_array)->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19692
            case 2: {
              // AddAndReplaceKind
              (compiler_globals.context).opcodes_size = (op_array)->last;

              break;
            }
#endif
#ifdef COMPILE_19693
            case 3: {
              // AddAndReplaceKind
              (compiler_globals.context).vars_size = (op_array)->last_var;

              break;
            }
#endif
#ifdef COMPILE_19694
            case 4: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_19695
            case 5: {
              // AddAndReplaceKind
              (op_array)->last_brk_cont++;

              break;
            }
#endif
#ifdef COMPILE_19696
            case 6: {
              // AddAndReplaceKind
              _efree(((op_array)->arg_info));

              break;
            }
#endif
#ifdef COMPILE_19697
            case 7: {
              // AddAndReplaceKind
              _efree(((op_array)->brk_cont_array));

              break;
            }
#endif
#ifdef COMPILE_19698
            case 8: {
              // AddAndReplaceKind
              _efree(((op_array)->doc_comment));

              break;
            }
#endif
#ifdef COMPILE_19699
            case 9: {
              // AddAndReplaceKind
              _efree(((op_array)->function_name));

              break;
            }
#endif
#ifdef COMPILE_19700
            case 10: {
              // AddAndReplaceKind
              _efree(((op_array)->literals));

              break;
            }
#endif
#ifdef COMPILE_19701
            case 11: {
              // AddAndReplaceKind
              _efree(((op_array)->opcodes));

              break;
            }
#endif
#ifdef COMPILE_19702
            case 12: {
              // AddAndReplaceKind
              _efree(((op_array)->refcount));

              break;
            }
#endif
#ifdef COMPILE_19703
            case 13: {
              // AddAndReplaceKind
              _efree(((op_array)->run_time_cache));

              break;
            }
#endif
#ifdef COMPILE_19704
            case 14: {
              // AddAndReplaceKind
              _efree(((op_array)->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19705
            case 15: {
              // AddAndReplaceKind
              _efree(((op_array)->try_catch_array));

              break;
            }
#endif
#ifdef COMPILE_19706
            case 16: {
              // AddAndReplaceKind
              _efree(((op_array)->vars));

              break;
            }
#endif
#ifdef COMPILE_19707
            case 17: {
              // AddAndReplaceKind
              _zend_bailout("Zend/zend_opcode.c", 432);

              break;
            }
#endif
#ifdef COMPILE_19708
            case 18: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_19709
            case 19: {
              // AddAndReplaceKind
              if (op_array->static_variables) {
                zend_hash_clean(op_array->static_variables);
              }

              break;
            }
#endif
#ifdef COMPILE_19710
            case 20: {
              // AddAndReplaceKind
              memset((op_array)->reserved, 0, 4 * sizeof(void *));

              break;
            }
#endif
#ifdef COMPILE_19711
            case 21: {
              // AddAndReplaceKind
              op_array_alloc_ops((op_array),
                                 (compiler_globals.context).opcodes_size);

              break;
            }
#endif
#ifdef COMPILE_19712
            case 22: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_19713
            case 23: {
              // AddAndReplaceKind
              zend_hash_clean((op_array)->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19714
            case 24: {
              // AddAndReplaceKind
              zend_hash_clean(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19715
            case 25: {
              // AddAndReplaceKind
              zend_hash_destroy((op_array)->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19716
            case 26: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)
                      zend_extension_op_array_ctor_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19717
            case 27: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)
                      zend_extension_op_array_dtor_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19718
            case 28: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)zend_extension_op_array_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19719
            case 29: {
              // AddAndReplaceKind
              zend_printf("Ran out of opcode space!\nYou should probably "
                          "consider writing this huge script into a file!\n");

              break;
            }
#endif
#ifdef COMPILE_19720
            case 30: {
              // AddAndReplaceKind
              zend_update_extended_info((op_array));

              break;
            }
#endif
#ifdef COMPILE_19721
            case 31: {
              // AddInitKind
              memset(op_array, 0, sizeof(*(op_array)));

              break;
            }
#endif
#ifdef COMPILE_19722
            case 32: {
              // IfExitKind
              if (__is_neg(
                      "190-32", 31, &(op_array), sizeof(op_array),
                      &(op_array->static_variables),
                      sizeof(op_array->static_variables), &(op_array->type),
                      sizeof(op_array->type), &(op_array->function_name),
                      sizeof(op_array->function_name), &(op_array->scope),
                      sizeof(op_array->scope), &(op_array->fn_flags),
                      sizeof(op_array->fn_flags), &(op_array->prototype),
                      sizeof(op_array->prototype), &(op_array->num_args),
                      sizeof(op_array->num_args),
                      &(op_array->required_num_args),
                      sizeof(op_array->required_num_args),
                      &(op_array->arg_info), sizeof(op_array->arg_info),
                      &(op_array->refcount), sizeof(op_array->refcount),
                      &(op_array->opcodes), sizeof(op_array->opcodes),
                      &(op_array->last), sizeof(op_array->last),
                      &(op_array->vars), sizeof(op_array->vars),
                      &(op_array->last_var), sizeof(op_array->last_var),
                      &(op_array->T), sizeof(op_array->T),
                      &(op_array->brk_cont_array),
                      sizeof(op_array->brk_cont_array),
                      &(op_array->last_brk_cont),
                      sizeof(op_array->last_brk_cont),
                      &(op_array->try_catch_array),
                      sizeof(op_array->try_catch_array),
                      &(op_array->last_try_catch),
                      sizeof(op_array->last_try_catch), &(op_array->this_var),
                      sizeof(op_array->this_var), &(op_array->filename),
                      sizeof(op_array->filename), &(op_array->line_start),
                      sizeof(op_array->line_start), &(op_array->line_end),
                      sizeof(op_array->line_end), &(op_array->doc_comment),
                      sizeof(op_array->doc_comment),
                      &(op_array->doc_comment_len),
                      sizeof(op_array->doc_comment_len),
                      &(op_array->early_binding),
                      sizeof(op_array->early_binding), &(op_array->literals),
                      sizeof(op_array->literals), &(op_array->last_literal),
                      sizeof(op_array->last_literal),
                      &(op_array->run_time_cache),
                      sizeof(op_array->run_time_cache),
                      &(op_array->last_cache_slot),
                      sizeof(op_array->last_cache_slot)))
                return;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH191")) {
            case 0: {
              zend_hash_clean(op_array->static_variables);

              break;
            }
#ifdef COMPILE_19723
            case 1: {
              // AddAndReplaceKind
              zend_destroy_rsrc_list(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19724
            case 2: {
              // AddAndReplaceKind
              zend_hash_destroy(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19725
            case 3: {
              // AddAndReplaceKind
              zend_hash_graceful_destroy(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19726
            case 4: {
              // AddAndReplaceKind
              zend_hash_graceful_reverse_destroy(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19727
            case 5: {
              // AddAndReplaceKind
              zend_hash_rehash(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19728
            case 6: {
              // AddAndReplaceKind
              zend_reset_all_cv(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19729
            case 7: {
              // GuardKind
              if (!__is_neg(
                      "191-7", 31, &(op_array), sizeof(op_array),
                      &(op_array->static_variables),
                      sizeof(op_array->static_variables), &(op_array->type),
                      sizeof(op_array->type), &(op_array->function_name),
                      sizeof(op_array->function_name), &(op_array->scope),
                      sizeof(op_array->scope), &(op_array->fn_flags),
                      sizeof(op_array->fn_flags), &(op_array->prototype),
                      sizeof(op_array->prototype), &(op_array->num_args),
                      sizeof(op_array->num_args),
                      &(op_array->required_num_args),
                      sizeof(op_array->required_num_args),
                      &(op_array->arg_info), sizeof(op_array->arg_info),
                      &(op_array->refcount), sizeof(op_array->refcount),
                      &(op_array->opcodes), sizeof(op_array->opcodes),
                      &(op_array->last), sizeof(op_array->last),
                      &(op_array->vars), sizeof(op_array->vars),
                      &(op_array->last_var), sizeof(op_array->last_var),
                      &(op_array->T), sizeof(op_array->T),
                      &(op_array->brk_cont_array),
                      sizeof(op_array->brk_cont_array),
                      &(op_array->last_brk_cont),
                      sizeof(op_array->last_brk_cont),
                      &(op_array->try_catch_array),
                      sizeof(op_array->try_catch_array),
                      &(op_array->last_try_catch),
                      sizeof(op_array->last_try_catch), &(op_array->this_var),
                      sizeof(op_array->this_var), &(op_array->filename),
                      sizeof(op_array->filename), &(op_array->line_start),
                      sizeof(op_array->line_start), &(op_array->line_end),
                      sizeof(op_array->line_end), &(op_array->doc_comment),
                      sizeof(op_array->doc_comment),
                      &(op_array->doc_comment_len),
                      sizeof(op_array->doc_comment_len),
                      &(op_array->early_binding),
                      sizeof(op_array->early_binding), &(op_array->literals),
                      sizeof(op_array->literals), &(op_array->last_literal),
                      sizeof(op_array->last_literal),
                      &(op_array->run_time_cache),
                      sizeof(op_array->run_time_cache),
                      &(op_array->last_cache_slot),
                      sizeof(op_array->last_cache_slot)))
                zend_hash_clean(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19730
            case 8: {
              // ReplaceKind
              zend_hash_clean((op_array)->static_variables);

              break;
            }
#endif
            }
          }
        }

        break;
      }
#ifdef COMPILE_19731
      case 1: {
        // AddAndReplaceKind
        zend_destroy_rsrc_list(op_array->static_variables);

        break;
      }
#endif
#ifdef COMPILE_19732
      case 2: {
        // AddAndReplaceKind
        zend_hash_destroy(op_array->static_variables);

        break;
      }
#endif
#ifdef COMPILE_19733
      case 3: {
        // AddAndReplaceKind
        zend_hash_graceful_destroy(op_array->static_variables);

        break;
      }
#endif
#ifdef COMPILE_19734
      case 4: {
        // AddAndReplaceKind
        zend_hash_graceful_reverse_destroy(op_array->static_variables);

        break;
      }
#endif
#ifdef COMPILE_19735
      case 5: {
        // AddAndReplaceKind
        zend_hash_rehash(op_array->static_variables);

        break;
      }
#endif
#ifdef COMPILE_19736
      case 6: {
        // AddAndReplaceKind
        zend_reset_all_cv(op_array->static_variables);

        break;
      }
#endif
#ifdef COMPILE_19737
      case 7: {
        // GuardKind
        if (!__is_neg(
                "192-7", 31, &(op_array), sizeof(op_array),
                &(op_array->static_variables),
                sizeof(op_array->static_variables), &(op_array->type),
                sizeof(op_array->type), &(op_array->function_name),
                sizeof(op_array->function_name), &(op_array->scope),
                sizeof(op_array->scope), &(op_array->fn_flags),
                sizeof(op_array->fn_flags), &(op_array->prototype),
                sizeof(op_array->prototype), &(op_array->num_args),
                sizeof(op_array->num_args), &(op_array->required_num_args),
                sizeof(op_array->required_num_args), &(op_array->arg_info),
                sizeof(op_array->arg_info), &(op_array->refcount),
                sizeof(op_array->refcount), &(op_array->opcodes),
                sizeof(op_array->opcodes), &(op_array->last),
                sizeof(op_array->last), &(op_array->vars),
                sizeof(op_array->vars), &(op_array->last_var),
                sizeof(op_array->last_var), &(op_array->T), sizeof(op_array->T),
                &(op_array->brk_cont_array), sizeof(op_array->brk_cont_array),
                &(op_array->last_brk_cont), sizeof(op_array->last_brk_cont),
                &(op_array->try_catch_array), sizeof(op_array->try_catch_array),
                &(op_array->last_try_catch), sizeof(op_array->last_try_catch),
                &(op_array->this_var), sizeof(op_array->this_var),
                &(op_array->filename), sizeof(op_array->filename),
                &(op_array->line_start), sizeof(op_array->line_start),
                &(op_array->line_end), sizeof(op_array->line_end),
                &(op_array->doc_comment), sizeof(op_array->doc_comment),
                &(op_array->doc_comment_len), sizeof(op_array->doc_comment_len),
                &(op_array->early_binding), sizeof(op_array->early_binding),
                &(op_array->literals), sizeof(op_array->literals),
                &(op_array->last_literal), sizeof(op_array->last_literal),
                &(op_array->run_time_cache), sizeof(op_array->run_time_cache),
                &(op_array->last_cache_slot),
                sizeof(op_array->last_cache_slot)))
          zend_hash_clean(op_array->static_variables);

        break;
      }
#endif
#ifdef COMPILE_19738
      case 8: {
        // ReplaceKind
        zend_hash_clean((op_array)->static_variables);

        break;
      }
#endif
      }
    }
  }
}

ZEND_API int zend_cleanup_function_data(zend_function *function TSRMLS_DC) {
  if (function->type == ZEND_USER_FUNCTION) {
    zend_cleanup_op_array_data((zend_op_array *)function);
    return ZEND_HASH_APPLY_KEEP;
  } else {
    return ZEND_HASH_APPLY_STOP;
  }
}

ZEND_API int
zend_cleanup_function_data_full(zend_function *function TSRMLS_DC) {
  if (function->type == ZEND_USER_FUNCTION) {
    zend_cleanup_op_array_data((zend_op_array *)function);
  }
  return 0;
}

static inline void cleanup_user_class_data(zend_class_entry *ce TSRMLS_DC) {
  /* Clean all parts that can contain run-time data */
  /* Note that only run-time accessed data need to be cleaned up, pre-defined
     data can not contain objects and thus are not probelmatic */
  if (ce->ce_flags & ZEND_HAS_STATIC_IN_METHODS) {
    zend_hash_apply(&ce->function_table,
                    (apply_func_t)zend_cleanup_function_data_full TSRMLS_CC);
  }
  if (ce->static_members_table) {
    int i;

    for (i = 0; i < ce->default_static_members_count; i++) {
      if (ce->static_members_table[i]) {
        zval_ptr_dtor(&ce->static_members_table[i]);
        ce->static_members_table[i] = NULL;
      }
    }
    ce->static_members_table = NULL;
  }
}

static inline void cleanup_internal_class_data(zend_class_entry *ce TSRMLS_DC) {
  if (CE_STATIC_MEMBERS(ce)) {
    int i;

    for (i = 0; i < ce->default_static_members_count; i++) {
      zval_ptr_dtor(&CE_STATIC_MEMBERS(ce)[i]);
    }
    efree(CE_STATIC_MEMBERS(ce));
#ifdef ZTS
    CG(static_members_table)[(zend_intptr_t)(ce->static_members_table)] = NULL;
#else
    ce->static_members_table = NULL;
#endif
  }
}

ZEND_API void zend_cleanup_internal_class_data(zend_class_entry *ce TSRMLS_DC) {
  cleanup_internal_class_data(ce TSRMLS_CC);
}

ZEND_API int zend_cleanup_user_class_data(zend_class_entry **pce TSRMLS_DC) {
  if ((*pce)->type == ZEND_USER_CLASS) {
    cleanup_user_class_data(*pce TSRMLS_CC);
    return ZEND_HASH_APPLY_KEEP;
  } else {
    return ZEND_HASH_APPLY_STOP;
  }
}

ZEND_API int zend_cleanup_class_data(zend_class_entry **pce TSRMLS_DC) {
  if ((*pce)->type == ZEND_USER_CLASS) {
    cleanup_user_class_data(*pce TSRMLS_CC);
  } else {
    cleanup_internal_class_data(*pce TSRMLS_CC);
  }
  return 0;
}

void _destroy_zend_class_traits_info(zend_class_entry *ce) {
  if (ce->num_traits > 0 && ce->traits) {
    efree(ce->traits);
  }

  if (ce->trait_aliases) {
    size_t i = 0;
    while (ce->trait_aliases[i]) {
      if (ce->trait_aliases[i]->trait_method) {
        if (ce->trait_aliases[i]->trait_method->method_name) {
          efree(ce->trait_aliases[i]->trait_method->method_name);
        }
        if (ce->trait_aliases[i]->trait_method->class_name) {
          efree(ce->trait_aliases[i]->trait_method->class_name);
        }
        efree(ce->trait_aliases[i]->trait_method);
      }

      if (ce->trait_aliases[i]->alias) {
        efree(ce->trait_aliases[i]->alias);
      }

      efree(ce->trait_aliases[i]);
      i++;
    }

    efree(ce->trait_aliases);
  }

  if (ce->trait_precedences) {
    size_t i = 0;

    while (ce->trait_precedences[i]) {
      efree(ce->trait_precedences[i]->trait_method->method_name);
      efree(ce->trait_precedences[i]->trait_method->class_name);
      efree(ce->trait_precedences[i]->trait_method);

      if (ce->trait_precedences[i]->exclude_from_classes) {
        efree(ce->trait_precedences[i]->exclude_from_classes);
      }

      efree(ce->trait_precedences[i]);
      i++;
    }
    efree(ce->trait_precedences);
  }
}

ZEND_API void destroy_zend_class(zend_class_entry **pce) {
  zend_class_entry *ce = *pce;

  if (--ce->refcount > 0) {
    return;
  }
  switch (ce->type) {
  case ZEND_USER_CLASS:
    if (ce->default_properties_table) {
      int i;

      for (i = 0; i < ce->default_properties_count; i++) {
        if (ce->default_properties_table[i]) {
          zval_ptr_dtor(&ce->default_properties_table[i]);
        }
      }
      efree(ce->default_properties_table);
    }
    if (ce->default_static_members_table) {
      int i;

      for (i = 0; i < ce->default_static_members_count; i++) {
        if (ce->default_static_members_table[i]) {
          zval_ptr_dtor(&ce->default_static_members_table[i]);
        }
      }
      efree(ce->default_static_members_table);
    }
    zend_hash_destroy(&ce->properties_info);
    if (!IS_INTERNED(ce->name)) {
      efree(ce->name);
    }
    zend_hash_destroy(&ce->function_table);
    zend_hash_destroy(&ce->constants_table);
    if (ce->num_interfaces > 0 && ce->interfaces) {
      efree(ce->interfaces);
    }
    if (ce->info.user.doc_comment) {
      efree(ce->info.user.doc_comment);
    }

    _destroy_zend_class_traits_info(ce);

    efree(ce);
    break;
  case ZEND_INTERNAL_CLASS:
    if (ce->default_properties_table) {
      int i;

      for (i = 0; i < ce->default_properties_count; i++) {
        if (ce->default_properties_table[i]) {
          zval_internal_ptr_dtor(&ce->default_properties_table[i]);
        }
      }
      free(ce->default_properties_table);
    }
    if (ce->default_static_members_table) {
      int i;

      for (i = 0; i < ce->default_static_members_count; i++) {
        zval_internal_ptr_dtor(&ce->default_static_members_table[i]);
      }
      free(ce->default_static_members_table);
    }
    zend_hash_destroy(&ce->properties_info);
    if (!IS_INTERNED(ce->name)) {
      free(ce->name);
    }
    zend_hash_destroy(&ce->function_table);
    zend_hash_destroy(&ce->constants_table);
    if (ce->num_interfaces > 0) {
      free(ce->interfaces);
    }
    free(ce);
    break;
  }
}

void zend_class_add_ref(zend_class_entry **ce) { (*ce)->refcount++; }

ZEND_API void destroy_op_array(zend_op_array *op_array TSRMLS_DC) {
  zend_literal *literal = op_array->literals;
  zend_literal *end;
  zend_uint i;

  // prophet generated patch
  {
    switch (__choose("__SWITCH193")) {
    case 0:
      break;
#ifdef COMPILE_19739
    case 1: {
      // AddAndReplaceKind
      (compiler_globals.context).literals_size = (op_array)->last_literal;

      break;
    }
#endif
#ifdef COMPILE_19740
    case 2: {
      // AddAndReplaceKind
      (compiler_globals.context).opcodes_size = (op_array)->last;

      break;
    }
#endif
#ifdef COMPILE_19741
    case 3: {
      // AddAndReplaceKind
      (compiler_globals.context).vars_size = (op_array)->last_var;

      break;
    }
#endif
#ifdef COMPILE_19742
    case 4: {
      // AddAndReplaceKind
      (end) = literal + op_array->last_literal;

      break;
    }
#endif
#ifdef COMPILE_19743
    case 5: {
      // AddAndReplaceKind
      (end)++;

      break;
    }
#endif
#ifdef COMPILE_19744
    case 6: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_19745
    case 7: {
      // AddAndReplaceKind
      (literal) = literal + op_array->last_literal;

      break;
    }
#endif
#ifdef COMPILE_19746
    case 8: {
      // AddAndReplaceKind
      (literal)++;

      break;
    }
#endif
#ifdef COMPILE_19747
    case 9: {
      // AddAndReplaceKind
      (op_array)->last_brk_cont++;

      break;
    }
#endif
#ifdef COMPILE_19748
    case 10: {
      // AddAndReplaceKind
      _efree(((char *)op_array->arg_info[i].class_name));

      break;
    }
#endif
#ifdef COMPILE_19749
    case 11: {
      // AddAndReplaceKind
      _efree(((op_array)->arg_info));

      break;
    }
#endif
#ifdef COMPILE_19750
    case 12: {
      // AddAndReplaceKind
      _efree(((op_array)->brk_cont_array));

      break;
    }
#endif
#ifdef COMPILE_19751
    case 13: {
      // AddAndReplaceKind
      _efree(((op_array)->doc_comment));

      break;
    }
#endif
#ifdef COMPILE_19752
    case 14: {
      // AddAndReplaceKind
      _efree(((op_array)->function_name));

      break;
    }
#endif
#ifdef COMPILE_19753
    case 15: {
      // AddAndReplaceKind
      _efree(((op_array)->literals));

      break;
    }
#endif
#ifdef COMPILE_19754
    case 16: {
      // AddAndReplaceKind
      _efree(((op_array)->opcodes));

      break;
    }
#endif
#ifdef COMPILE_19755
    case 17: {
      // AddAndReplaceKind
      _efree(((op_array)->refcount));

      break;
    }
#endif
#ifdef COMPILE_19756
    case 18: {
      // AddAndReplaceKind
      _efree(((op_array)->run_time_cache));

      break;
    }
#endif
#ifdef COMPILE_19757
    case 19: {
      // AddAndReplaceKind
      _efree(((op_array)->static_variables));

      break;
    }
#endif
#ifdef COMPILE_19758
    case 20: {
      // AddAndReplaceKind
      _efree(((op_array)->try_catch_array));

      break;
    }
#endif
#ifdef COMPILE_19759
    case 21: {
      // AddAndReplaceKind
      _efree(((op_array)->vars));

      break;
    }
#endif
#ifdef COMPILE_19760
    case 22: {
      // AddAndReplaceKind
      _efree((op_array->arg_info));

      break;
    }
#endif
#ifdef COMPILE_19761
    case 23: {
      // AddAndReplaceKind
      _efree((op_array->brk_cont_array));

      break;
    }
#endif
#ifdef COMPILE_19762
    case 24: {
      // AddAndReplaceKind
      _efree((op_array->doc_comment));

      break;
    }
#endif
#ifdef COMPILE_19763
    case 25: {
      // AddAndReplaceKind
      _efree((op_array->function_name));

      break;
    }
#endif
#ifdef COMPILE_19764
    case 26: {
      // AddAndReplaceKind
      _efree((op_array->literals));

      break;
    }
#endif
#ifdef COMPILE_19765
    case 27: {
      // AddAndReplaceKind
      _efree((op_array->opcodes));

      break;
    }
#endif
#ifdef COMPILE_19766
    case 28: {
      // AddAndReplaceKind
      _efree((op_array->refcount));

      break;
    }
#endif
#ifdef COMPILE_19767
    case 29: {
      // AddAndReplaceKind
      _efree((op_array->run_time_cache));

      break;
    }
#endif
#ifdef COMPILE_19768
    case 30: {
      // AddAndReplaceKind
      _efree((op_array->static_variables));

      break;
    }
#endif
#ifdef COMPILE_19769
    case 31: {
      // AddAndReplaceKind
      _efree((op_array->try_catch_array));

      break;
    }
#endif
#ifdef COMPILE_19770
    case 32: {
      // AddAndReplaceKind
      _efree((op_array->vars));

      break;
    }
#endif
#ifdef COMPILE_19771
    case 33: {
      // AddAndReplaceKind
      _zend_bailout("Zend/zend_opcode.c", 432);

      break;
    }
#endif
#ifdef COMPILE_19772
    case 34: {
      // AddAndReplaceKind
      _zval_dtor((&(end)->constant));

      break;
    }
#endif
#ifdef COMPILE_19773
    case 35: {
      // AddAndReplaceKind
      _zval_dtor((&(literal)->constant));

      break;
    }
#endif
#ifdef COMPILE_19774
    case 36: {
      // AddAndReplaceKind
      _zval_dtor((&literal->constant));

      break;
    }
#endif
#ifdef COMPILE_19775
    case 37: {
      // AddAndReplaceKind
      end = (end) + op_array->last_literal;

      break;
    }
#endif
#ifdef COMPILE_19776
    case 38: {
      // AddAndReplaceKind
      end = (literal) + op_array->last_literal;

      break;
    }
#endif
#ifdef COMPILE_19777
    case 39: {
      // AddAndReplaceKind
      end = literal + (op_array)->last_literal;

      break;
    }
#endif
#ifdef COMPILE_19778
    case 40: {
      // AddAndReplaceKind
      end = literal + op_array->last_literal;

      break;
    }
#endif
#ifdef COMPILE_19779
    case 41: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_19780
    case 42: {
      // AddAndReplaceKind
      i = (op_array)->last_var;

      break;
    }
#endif
#ifdef COMPILE_19781
    case 43: {
      // AddAndReplaceKind
      i = op_array->last_var;

      break;
    }
#endif
#ifdef COMPILE_19782
    case 44: {
      // AddAndReplaceKind
      i--;

      break;
    }
#endif
#ifdef COMPILE_19783
    case 45: {
      // AddAndReplaceKind
      if (--(*op_array->refcount) > 0) {
        return;
      }

      break;
    }
#endif
#ifdef COMPILE_19784
    case 46: {
      // AddAndReplaceKind
      if (op_array->arg_info[i].class_name &&
          !(((op_array->arg_info[i].class_name) >=
             (compiler_globals.interned_strings_start)) &&
            ((op_array->arg_info[i].class_name) <
             (compiler_globals.interned_strings_end)))) {
        _efree(((char *)op_array->arg_info[i].class_name));
      }

      break;
    }
#endif
#ifdef COMPILE_19785
    case 47: {
      // AddAndReplaceKind
      if (op_array->brk_cont_array) {
        _efree((op_array->brk_cont_array));
      }

      break;
    }
#endif
#ifdef COMPILE_19786
    case 48: {
      // AddAndReplaceKind
      if (op_array->doc_comment) {
        _efree((op_array->doc_comment));
      }

      break;
    }
#endif
#ifdef COMPILE_19787
    case 49: {
      // AddAndReplaceKind
      if (op_array->fn_flags & 134217728) {
        zend_llist_apply_with_argument(
            &zend_extensions,
            (llist_apply_with_arg_func_t)zend_extension_op_array_dtor_handler,
            op_array);
      }

      break;
    }
#endif
#ifdef COMPILE_19788
    case 50: {
      // AddAndReplaceKind
      if (op_array->function_name) {
        _efree((op_array->function_name));
      }

      break;
    }
#endif
#ifdef COMPILE_19789
    case 51: {
      // AddAndReplaceKind
      if (op_array->run_time_cache) {
        _efree((op_array->run_time_cache));
      }

      break;
    }
#endif
#ifdef COMPILE_19790
    case 52: {
      // AddAndReplaceKind
      if (op_array->static_variables) {
        zend_hash_destroy(op_array->static_variables);
        _efree((op_array->static_variables));
      }

      break;
    }
#endif
#ifdef COMPILE_19791
    case 53: {
      // AddAndReplaceKind
      if (op_array->try_catch_array) {
        _efree((op_array->try_catch_array));
      }

      break;
    }
#endif
#ifdef COMPILE_19792
    case 54: {
      // AddAndReplaceKind
      literal++;

      break;
    }
#endif
#ifdef COMPILE_19793
    case 55: {
      // AddAndReplaceKind
      memset((op_array)->reserved, 0, 4 * sizeof(void *));

      break;
    }
#endif
#ifdef COMPILE_19794
    case 56: {
      // AddAndReplaceKind
      op_array_alloc_ops((op_array), (compiler_globals.context).opcodes_size);

      break;
    }
#endif
#ifdef COMPILE_19795
    case 57: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_19796
    case 58: {
      // AddAndReplaceKind
      zend_hash_clean((op_array)->static_variables);

      break;
    }
#endif
#ifdef COMPILE_19797
    case 59: {
      // AddAndReplaceKind
      zend_hash_destroy((op_array)->static_variables);

      break;
    }
#endif
#ifdef COMPILE_19798
    case 60: {
      // AddAndReplaceKind
      zend_hash_destroy(op_array->static_variables);

      break;
    }
#endif
#ifdef COMPILE_19799
    case 61: {
      // AddAndReplaceKind
      zend_llist_apply_with_argument(
          &zend_extensions,
          (llist_apply_with_arg_func_t)zend_extension_op_array_ctor_handler,
          (op_array));

      break;
    }
#endif
#ifdef COMPILE_19800
    case 62: {
      // AddAndReplaceKind
      zend_llist_apply_with_argument(
          &zend_extensions,
          (llist_apply_with_arg_func_t)zend_extension_op_array_dtor_handler,
          (op_array));

      break;
    }
#endif
#ifdef COMPILE_19801
    case 63: {
      // AddAndReplaceKind
      zend_llist_apply_with_argument(
          &zend_extensions,
          (llist_apply_with_arg_func_t)zend_extension_op_array_dtor_handler,
          op_array);

      break;
    }
#endif
#ifdef COMPILE_19802
    case 64: {
      // AddAndReplaceKind
      zend_llist_apply_with_argument(
          &zend_extensions,
          (llist_apply_with_arg_func_t)zend_extension_op_array_handler,
          (op_array));

      break;
    }
#endif
#ifdef COMPILE_19803
    case 65: {
      // AddAndReplaceKind
      zend_printf("Ran out of opcode space!\nYou should probably consider "
                  "writing this huge script into a file!\n");

      break;
    }
#endif
#ifdef COMPILE_19804
    case 66: {
      // AddAndReplaceKind
      zend_update_extended_info((op_array));

      break;
    }
#endif
    }
    {
      long long __temp194 = (op_array->static_variables);
      switch (__choose("__SWITCH194")) {
      case 0: {

        break;
      }
#ifdef COMPILE_19805
      case 1: {
        __temp194 =
            ((op_array->static_variables) &&
             !__is_neg(
                 "194-1", 34, &(op_array), sizeof(op_array),
                 &(op_array->static_variables),
                 sizeof(op_array->static_variables), &(op_array->literals),
                 sizeof(op_array->literals), &(op_array->run_time_cache),
                 sizeof(op_array->run_time_cache), &(op_array->refcount),
                 sizeof(op_array->refcount), &(op_array->vars),
                 sizeof(op_array->vars), &(i), sizeof(i), &(op_array->last_var),
                 sizeof(op_array->last_var), &(literal), sizeof(literal),
                 &(end), sizeof(end), &(op_array->last_literal),
                 sizeof(op_array->last_literal), &(op_array->opcodes),
                 sizeof(op_array->opcodes), &(op_array->function_name),
                 sizeof(op_array->function_name), &(op_array->doc_comment),
                 sizeof(op_array->doc_comment), &(op_array->brk_cont_array),
                 sizeof(op_array->brk_cont_array), &(op_array->try_catch_array),
                 sizeof(op_array->try_catch_array), &(op_array->fn_flags),
                 sizeof(op_array->fn_flags), &(op_array->arg_info),
                 sizeof(op_array->arg_info), &(op_array->num_args),
                 sizeof(op_array->num_args), &(op_array->type),
                 sizeof(op_array->type), &(op_array->scope),
                 sizeof(op_array->scope), &(op_array->prototype),
                 sizeof(op_array->prototype), &(op_array->required_num_args),
                 sizeof(op_array->required_num_args), &(op_array->last),
                 sizeof(op_array->last), &(op_array->T), sizeof(op_array->T),
                 &(op_array->last_brk_cont), sizeof(op_array->last_brk_cont),
                 &(op_array->last_try_catch), sizeof(op_array->last_try_catch),
                 &(op_array->this_var), sizeof(op_array->this_var),
                 &(op_array->filename), sizeof(op_array->filename),
                 &(op_array->line_start), sizeof(op_array->line_start),
                 &(op_array->line_end), sizeof(op_array->line_end),
                 &(op_array->doc_comment_len),
                 sizeof(op_array->doc_comment_len), &(op_array->early_binding),
                 sizeof(op_array->early_binding), &(op_array->last_cache_slot),
                 sizeof(op_array->last_cache_slot)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH199")) {
      case 0: {
        if (__temp194) {
          {
            switch (__choose("__SWITCH195")) {
            case 0:
              break;
#ifdef COMPILE_19806
            case 1: {
              // AddAndReplaceKind
              (compiler_globals.context).literals_size =
                  (op_array)->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19807
            case 2: {
              // AddAndReplaceKind
              (compiler_globals.context).opcodes_size = (op_array)->last;

              break;
            }
#endif
#ifdef COMPILE_19808
            case 3: {
              // AddAndReplaceKind
              (compiler_globals.context).vars_size = (op_array)->last_var;

              break;
            }
#endif
#ifdef COMPILE_19809
            case 4: {
              // AddAndReplaceKind
              (end) = literal + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19810
            case 5: {
              // AddAndReplaceKind
              (end)++;

              break;
            }
#endif
#ifdef COMPILE_19811
            case 6: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_19812
            case 7: {
              // AddAndReplaceKind
              (literal) = literal + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19813
            case 8: {
              // AddAndReplaceKind
              (literal)++;

              break;
            }
#endif
#ifdef COMPILE_19814
            case 9: {
              // AddAndReplaceKind
              (op_array)->last_brk_cont++;

              break;
            }
#endif
#ifdef COMPILE_19815
            case 10: {
              // AddAndReplaceKind
              _efree(((char *)op_array->arg_info[i].class_name));

              break;
            }
#endif
#ifdef COMPILE_19816
            case 11: {
              // AddAndReplaceKind
              _efree(((op_array)->arg_info));

              break;
            }
#endif
#ifdef COMPILE_19817
            case 12: {
              // AddAndReplaceKind
              _efree(((op_array)->brk_cont_array));

              break;
            }
#endif
#ifdef COMPILE_19818
            case 13: {
              // AddAndReplaceKind
              _efree(((op_array)->doc_comment));

              break;
            }
#endif
#ifdef COMPILE_19819
            case 14: {
              // AddAndReplaceKind
              _efree(((op_array)->function_name));

              break;
            }
#endif
#ifdef COMPILE_19820
            case 15: {
              // AddAndReplaceKind
              _efree(((op_array)->literals));

              break;
            }
#endif
#ifdef COMPILE_19821
            case 16: {
              // AddAndReplaceKind
              _efree(((op_array)->opcodes));

              break;
            }
#endif
#ifdef COMPILE_19822
            case 17: {
              // AddAndReplaceKind
              _efree(((op_array)->refcount));

              break;
            }
#endif
#ifdef COMPILE_19823
            case 18: {
              // AddAndReplaceKind
              _efree(((op_array)->run_time_cache));

              break;
            }
#endif
#ifdef COMPILE_19824
            case 19: {
              // AddAndReplaceKind
              _efree(((op_array)->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19825
            case 20: {
              // AddAndReplaceKind
              _efree(((op_array)->try_catch_array));

              break;
            }
#endif
#ifdef COMPILE_19826
            case 21: {
              // AddAndReplaceKind
              _efree(((op_array)->vars));

              break;
            }
#endif
#ifdef COMPILE_19827
            case 22: {
              // AddAndReplaceKind
              _efree((op_array->arg_info));

              break;
            }
#endif
#ifdef COMPILE_19828
            case 23: {
              // AddAndReplaceKind
              _efree((op_array->brk_cont_array));

              break;
            }
#endif
#ifdef COMPILE_19829
            case 24: {
              // AddAndReplaceKind
              _efree((op_array->doc_comment));

              break;
            }
#endif
#ifdef COMPILE_19830
            case 25: {
              // AddAndReplaceKind
              _efree((op_array->function_name));

              break;
            }
#endif
#ifdef COMPILE_19831
            case 26: {
              // AddAndReplaceKind
              _efree((op_array->literals));

              break;
            }
#endif
#ifdef COMPILE_19832
            case 27: {
              // AddAndReplaceKind
              _efree((op_array->opcodes));

              break;
            }
#endif
#ifdef COMPILE_19833
            case 28: {
              // AddAndReplaceKind
              _efree((op_array->refcount));

              break;
            }
#endif
#ifdef COMPILE_19834
            case 29: {
              // AddAndReplaceKind
              _efree((op_array->run_time_cache));

              break;
            }
#endif
#ifdef COMPILE_19835
            case 30: {
              // AddAndReplaceKind
              _efree((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19836
            case 31: {
              // AddAndReplaceKind
              _efree((op_array->try_catch_array));

              break;
            }
#endif
#ifdef COMPILE_19837
            case 32: {
              // AddAndReplaceKind
              _efree((op_array->vars));

              break;
            }
#endif
#ifdef COMPILE_19838
            case 33: {
              // AddAndReplaceKind
              _zend_bailout("Zend/zend_opcode.c", 432);

              break;
            }
#endif
#ifdef COMPILE_19839
            case 34: {
              // AddAndReplaceKind
              _zval_dtor((&(end)->constant));

              break;
            }
#endif
#ifdef COMPILE_19840
            case 35: {
              // AddAndReplaceKind
              _zval_dtor((&(literal)->constant));

              break;
            }
#endif
#ifdef COMPILE_19841
            case 36: {
              // AddAndReplaceKind
              _zval_dtor((&literal->constant));

              break;
            }
#endif
#ifdef COMPILE_19842
            case 37: {
              // AddAndReplaceKind
              end = (end) + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19843
            case 38: {
              // AddAndReplaceKind
              end = (literal) + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19844
            case 39: {
              // AddAndReplaceKind
              end = literal + (op_array)->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19845
            case 40: {
              // AddAndReplaceKind
              end = literal + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19846
            case 41: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_19847
            case 42: {
              // AddAndReplaceKind
              i = (op_array)->last_var;

              break;
            }
#endif
#ifdef COMPILE_19848
            case 43: {
              // AddAndReplaceKind
              i = op_array->last_var;

              break;
            }
#endif
#ifdef COMPILE_19849
            case 44: {
              // AddAndReplaceKind
              i--;

              break;
            }
#endif
#ifdef COMPILE_19850
            case 45: {
              // AddAndReplaceKind
              if (--(*op_array->refcount) > 0) {
                return;
              }

              break;
            }
#endif
#ifdef COMPILE_19851
            case 46: {
              // AddAndReplaceKind
              if (op_array->arg_info[i].class_name &&
                  !(((op_array->arg_info[i].class_name) >=
                     (compiler_globals.interned_strings_start)) &&
                    ((op_array->arg_info[i].class_name) <
                     (compiler_globals.interned_strings_end)))) {
                _efree(((char *)op_array->arg_info[i].class_name));
              }

              break;
            }
#endif
#ifdef COMPILE_19852
            case 47: {
              // AddAndReplaceKind
              if (op_array->brk_cont_array) {
                _efree((op_array->brk_cont_array));
              }

              break;
            }
#endif
#ifdef COMPILE_19853
            case 48: {
              // AddAndReplaceKind
              if (op_array->doc_comment) {
                _efree((op_array->doc_comment));
              }

              break;
            }
#endif
#ifdef COMPILE_19854
            case 49: {
              // AddAndReplaceKind
              if (op_array->fn_flags & 134217728) {
                zend_llist_apply_with_argument(
                    &zend_extensions,
                    (llist_apply_with_arg_func_t)
                        zend_extension_op_array_dtor_handler,
                    op_array);
              }

              break;
            }
#endif
#ifdef COMPILE_19855
            case 50: {
              // AddAndReplaceKind
              if (op_array->function_name) {
                _efree((op_array->function_name));
              }

              break;
            }
#endif
#ifdef COMPILE_19856
            case 51: {
              // AddAndReplaceKind
              if (op_array->run_time_cache) {
                _efree((op_array->run_time_cache));
              }

              break;
            }
#endif
#ifdef COMPILE_19857
            case 52: {
              // AddAndReplaceKind
              if (op_array->static_variables) {
                zend_hash_destroy(op_array->static_variables);
                _efree((op_array->static_variables));
              }

              break;
            }
#endif
#ifdef COMPILE_19858
            case 53: {
              // AddAndReplaceKind
              if (op_array->try_catch_array) {
                _efree((op_array->try_catch_array));
              }

              break;
            }
#endif
#ifdef COMPILE_19859
            case 54: {
              // AddAndReplaceKind
              literal++;

              break;
            }
#endif
#ifdef COMPILE_19860
            case 55: {
              // AddAndReplaceKind
              memset((op_array)->reserved, 0, 4 * sizeof(void *));

              break;
            }
#endif
#ifdef COMPILE_19861
            case 56: {
              // AddAndReplaceKind
              op_array_alloc_ops((op_array),
                                 (compiler_globals.context).opcodes_size);

              break;
            }
#endif
#ifdef COMPILE_19862
            case 57: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_19863
            case 58: {
              // AddAndReplaceKind
              zend_hash_clean((op_array)->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19864
            case 59: {
              // AddAndReplaceKind
              zend_hash_destroy((op_array)->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19865
            case 60: {
              // AddAndReplaceKind
              zend_hash_destroy(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19866
            case 61: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)
                      zend_extension_op_array_ctor_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19867
            case 62: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)
                      zend_extension_op_array_dtor_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19868
            case 63: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)
                      zend_extension_op_array_dtor_handler,
                  op_array);

              break;
            }
#endif
#ifdef COMPILE_19869
            case 64: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)zend_extension_op_array_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19870
            case 65: {
              // AddAndReplaceKind
              zend_printf("Ran out of opcode space!\nYou should probably "
                          "consider writing this huge script into a file!\n");

              break;
            }
#endif
#ifdef COMPILE_19871
            case 66: {
              // AddAndReplaceKind
              zend_update_extended_info((op_array));

              break;
            }
#endif
#ifdef COMPILE_19872
            case 67: {
              // AddInitKind
              memset(op_array, 0, sizeof(*(op_array)));

              break;
            }
#endif
#ifdef COMPILE_19873
            case 68: {
              // IfExitKind
              if (__is_neg(
                      "195-68", 34, &(op_array), sizeof(op_array),
                      &(op_array->static_variables),
                      sizeof(op_array->static_variables),
                      &(op_array->run_time_cache),
                      sizeof(op_array->run_time_cache), &(op_array->literals),
                      sizeof(op_array->literals), &(op_array->refcount),
                      sizeof(op_array->refcount), &(op_array->vars),
                      sizeof(op_array->vars), &(i), sizeof(i),
                      &(op_array->last_var), sizeof(op_array->last_var),
                      &(literal), sizeof(literal), &(end), sizeof(end),
                      &(op_array->last_literal), sizeof(op_array->last_literal),
                      &(op_array->opcodes), sizeof(op_array->opcodes),
                      &(op_array->function_name),
                      sizeof(op_array->function_name), &(op_array->doc_comment),
                      sizeof(op_array->doc_comment),
                      &(op_array->brk_cont_array),
                      sizeof(op_array->brk_cont_array),
                      &(op_array->try_catch_array),
                      sizeof(op_array->try_catch_array), &(op_array->fn_flags),
                      sizeof(op_array->fn_flags), &(op_array->arg_info),
                      sizeof(op_array->arg_info), &(op_array->num_args),
                      sizeof(op_array->num_args), &(op_array->type),
                      sizeof(op_array->type), &(op_array->scope),
                      sizeof(op_array->scope), &(op_array->prototype),
                      sizeof(op_array->prototype),
                      &(op_array->required_num_args),
                      sizeof(op_array->required_num_args), &(op_array->last),
                      sizeof(op_array->last), &(op_array->T),
                      sizeof(op_array->T), &(op_array->last_brk_cont),
                      sizeof(op_array->last_brk_cont),
                      &(op_array->last_try_catch),
                      sizeof(op_array->last_try_catch), &(op_array->this_var),
                      sizeof(op_array->this_var), &(op_array->filename),
                      sizeof(op_array->filename), &(op_array->line_start),
                      sizeof(op_array->line_start), &(op_array->line_end),
                      sizeof(op_array->line_end), &(op_array->doc_comment_len),
                      sizeof(op_array->doc_comment_len),
                      &(op_array->early_binding),
                      sizeof(op_array->early_binding),
                      &(op_array->last_cache_slot),
                      sizeof(op_array->last_cache_slot)))
                return;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH196")) {
            case 0: {
              zend_hash_destroy(op_array->static_variables);

              break;
            }
#ifdef COMPILE_19874
            case 1: {
              // AddAndReplaceKind
              zend_destroy_rsrc_list(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19875
            case 2: {
              // AddAndReplaceKind
              zend_hash_clean(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19876
            case 3: {
              // AddAndReplaceKind
              zend_hash_graceful_destroy(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19877
            case 4: {
              // AddAndReplaceKind
              zend_hash_graceful_reverse_destroy(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19878
            case 5: {
              // AddAndReplaceKind
              zend_hash_rehash(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19879
            case 6: {
              // AddAndReplaceKind
              zend_reset_all_cv(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19880
            case 7: {
              // GuardKind
              if (!__is_neg(
                      "196-7", 34, &(op_array), sizeof(op_array),
                      &(op_array->static_variables),
                      sizeof(op_array->static_variables),
                      &(op_array->run_time_cache),
                      sizeof(op_array->run_time_cache), &(op_array->literals),
                      sizeof(op_array->literals), &(op_array->refcount),
                      sizeof(op_array->refcount), &(op_array->vars),
                      sizeof(op_array->vars), &(i), sizeof(i),
                      &(op_array->last_var), sizeof(op_array->last_var),
                      &(literal), sizeof(literal), &(end), sizeof(end),
                      &(op_array->last_literal), sizeof(op_array->last_literal),
                      &(op_array->opcodes), sizeof(op_array->opcodes),
                      &(op_array->function_name),
                      sizeof(op_array->function_name), &(op_array->doc_comment),
                      sizeof(op_array->doc_comment),
                      &(op_array->brk_cont_array),
                      sizeof(op_array->brk_cont_array),
                      &(op_array->try_catch_array),
                      sizeof(op_array->try_catch_array), &(op_array->fn_flags),
                      sizeof(op_array->fn_flags), &(op_array->arg_info),
                      sizeof(op_array->arg_info), &(op_array->num_args),
                      sizeof(op_array->num_args), &(op_array->type),
                      sizeof(op_array->type), &(op_array->scope),
                      sizeof(op_array->scope), &(op_array->prototype),
                      sizeof(op_array->prototype),
                      &(op_array->required_num_args),
                      sizeof(op_array->required_num_args), &(op_array->last),
                      sizeof(op_array->last), &(op_array->T),
                      sizeof(op_array->T), &(op_array->last_brk_cont),
                      sizeof(op_array->last_brk_cont),
                      &(op_array->last_try_catch),
                      sizeof(op_array->last_try_catch), &(op_array->this_var),
                      sizeof(op_array->this_var), &(op_array->filename),
                      sizeof(op_array->filename), &(op_array->line_start),
                      sizeof(op_array->line_start), &(op_array->line_end),
                      sizeof(op_array->line_end), &(op_array->doc_comment_len),
                      sizeof(op_array->doc_comment_len),
                      &(op_array->early_binding),
                      sizeof(op_array->early_binding),
                      &(op_array->last_cache_slot),
                      sizeof(op_array->last_cache_slot)))
                zend_hash_destroy(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19881
            case 8: {
              // ReplaceKind
              zend_hash_destroy((op_array)->static_variables);

              break;
            }
#endif
            }
          }
          {
            switch (__choose("__SWITCH197")) {
            case 0:
              break;
#ifdef COMPILE_19882
            case 1: {
              // AddAndReplaceKind
              (compiler_globals.context).literals_size =
                  (op_array)->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19883
            case 2: {
              // AddAndReplaceKind
              (compiler_globals.context).opcodes_size = (op_array)->last;

              break;
            }
#endif
#ifdef COMPILE_19884
            case 3: {
              // AddAndReplaceKind
              (compiler_globals.context).vars_size = (op_array)->last_var;

              break;
            }
#endif
#ifdef COMPILE_19885
            case 4: {
              // AddAndReplaceKind
              (end) = literal + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19886
            case 5: {
              // AddAndReplaceKind
              (end)++;

              break;
            }
#endif
#ifdef COMPILE_19887
            case 6: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_19888
            case 7: {
              // AddAndReplaceKind
              (literal) = literal + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19889
            case 8: {
              // AddAndReplaceKind
              (literal)++;

              break;
            }
#endif
#ifdef COMPILE_19890
            case 9: {
              // AddAndReplaceKind
              (op_array)->last_brk_cont++;

              break;
            }
#endif
#ifdef COMPILE_19891
            case 10: {
              // AddAndReplaceKind
              _efree(((char *)op_array->arg_info[i].class_name));

              break;
            }
#endif
#ifdef COMPILE_19892
            case 11: {
              // AddAndReplaceKind
              _efree(((op_array)->arg_info));

              break;
            }
#endif
#ifdef COMPILE_19893
            case 12: {
              // AddAndReplaceKind
              _efree(((op_array)->brk_cont_array));

              break;
            }
#endif
#ifdef COMPILE_19894
            case 13: {
              // AddAndReplaceKind
              _efree(((op_array)->doc_comment));

              break;
            }
#endif
#ifdef COMPILE_19895
            case 14: {
              // AddAndReplaceKind
              _efree(((op_array)->function_name));

              break;
            }
#endif
#ifdef COMPILE_19896
            case 15: {
              // AddAndReplaceKind
              _efree(((op_array)->literals));

              break;
            }
#endif
#ifdef COMPILE_19897
            case 16: {
              // AddAndReplaceKind
              _efree(((op_array)->opcodes));

              break;
            }
#endif
#ifdef COMPILE_19898
            case 17: {
              // AddAndReplaceKind
              _efree(((op_array)->refcount));

              break;
            }
#endif
#ifdef COMPILE_19899
            case 18: {
              // AddAndReplaceKind
              _efree(((op_array)->run_time_cache));

              break;
            }
#endif
#ifdef COMPILE_19900
            case 19: {
              // AddAndReplaceKind
              _efree(((op_array)->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19901
            case 20: {
              // AddAndReplaceKind
              _efree(((op_array)->try_catch_array));

              break;
            }
#endif
#ifdef COMPILE_19902
            case 21: {
              // AddAndReplaceKind
              _efree(((op_array)->vars));

              break;
            }
#endif
#ifdef COMPILE_19903
            case 22: {
              // AddAndReplaceKind
              _efree((op_array->arg_info));

              break;
            }
#endif
#ifdef COMPILE_19904
            case 23: {
              // AddAndReplaceKind
              _efree((op_array->brk_cont_array));

              break;
            }
#endif
#ifdef COMPILE_19905
            case 24: {
              // AddAndReplaceKind
              _efree((op_array->doc_comment));

              break;
            }
#endif
#ifdef COMPILE_19906
            case 25: {
              // AddAndReplaceKind
              _efree((op_array->function_name));

              break;
            }
#endif
#ifdef COMPILE_19907
            case 26: {
              // AddAndReplaceKind
              _efree((op_array->literals));

              break;
            }
#endif
#ifdef COMPILE_19908
            case 27: {
              // AddAndReplaceKind
              _efree((op_array->opcodes));

              break;
            }
#endif
#ifdef COMPILE_19909
            case 28: {
              // AddAndReplaceKind
              _efree((op_array->refcount));

              break;
            }
#endif
#ifdef COMPILE_19910
            case 29: {
              // AddAndReplaceKind
              _efree((op_array->run_time_cache));

              break;
            }
#endif
#ifdef COMPILE_19911
            case 30: {
              // AddAndReplaceKind
              _efree((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19912
            case 31: {
              // AddAndReplaceKind
              _efree((op_array->try_catch_array));

              break;
            }
#endif
#ifdef COMPILE_19913
            case 32: {
              // AddAndReplaceKind
              _efree((op_array->vars));

              break;
            }
#endif
#ifdef COMPILE_19914
            case 33: {
              // AddAndReplaceKind
              _zend_bailout("Zend/zend_opcode.c", 432);

              break;
            }
#endif
#ifdef COMPILE_19915
            case 34: {
              // AddAndReplaceKind
              _zval_dtor((&(end)->constant));

              break;
            }
#endif
#ifdef COMPILE_19916
            case 35: {
              // AddAndReplaceKind
              _zval_dtor((&(literal)->constant));

              break;
            }
#endif
#ifdef COMPILE_19917
            case 36: {
              // AddAndReplaceKind
              _zval_dtor((&literal->constant));

              break;
            }
#endif
#ifdef COMPILE_19918
            case 37: {
              // AddAndReplaceKind
              end = (end) + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19919
            case 38: {
              // AddAndReplaceKind
              end = (literal) + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19920
            case 39: {
              // AddAndReplaceKind
              end = literal + (op_array)->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19921
            case 40: {
              // AddAndReplaceKind
              end = literal + op_array->last_literal;

              break;
            }
#endif
#ifdef COMPILE_19922
            case 41: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_19923
            case 42: {
              // AddAndReplaceKind
              i = (op_array)->last_var;

              break;
            }
#endif
#ifdef COMPILE_19924
            case 43: {
              // AddAndReplaceKind
              i = op_array->last_var;

              break;
            }
#endif
#ifdef COMPILE_19925
            case 44: {
              // AddAndReplaceKind
              i--;

              break;
            }
#endif
#ifdef COMPILE_19926
            case 45: {
              // AddAndReplaceKind
              if (--(*op_array->refcount) > 0) {
                return;
              }

              break;
            }
#endif
#ifdef COMPILE_19927
            case 46: {
              // AddAndReplaceKind
              if (op_array->arg_info[i].class_name &&
                  !(((op_array->arg_info[i].class_name) >=
                     (compiler_globals.interned_strings_start)) &&
                    ((op_array->arg_info[i].class_name) <
                     (compiler_globals.interned_strings_end)))) {
                _efree(((char *)op_array->arg_info[i].class_name));
              }

              break;
            }
#endif
#ifdef COMPILE_19928
            case 47: {
              // AddAndReplaceKind
              if (op_array->brk_cont_array) {
                _efree((op_array->brk_cont_array));
              }

              break;
            }
#endif
#ifdef COMPILE_19929
            case 48: {
              // AddAndReplaceKind
              if (op_array->doc_comment) {
                _efree((op_array->doc_comment));
              }

              break;
            }
#endif
#ifdef COMPILE_19930
            case 49: {
              // AddAndReplaceKind
              if (op_array->fn_flags & 134217728) {
                zend_llist_apply_with_argument(
                    &zend_extensions,
                    (llist_apply_with_arg_func_t)
                        zend_extension_op_array_dtor_handler,
                    op_array);
              }

              break;
            }
#endif
#ifdef COMPILE_19931
            case 50: {
              // AddAndReplaceKind
              if (op_array->function_name) {
                _efree((op_array->function_name));
              }

              break;
            }
#endif
#ifdef COMPILE_19932
            case 51: {
              // AddAndReplaceKind
              if (op_array->run_time_cache) {
                _efree((op_array->run_time_cache));
              }

              break;
            }
#endif
#ifdef COMPILE_19933
            case 52: {
              // AddAndReplaceKind
              if (op_array->static_variables) {
                zend_hash_destroy(op_array->static_variables);
                _efree((op_array->static_variables));
              }

              break;
            }
#endif
#ifdef COMPILE_19934
            case 53: {
              // AddAndReplaceKind
              if (op_array->try_catch_array) {
                _efree((op_array->try_catch_array));
              }

              break;
            }
#endif
#ifdef COMPILE_19935
            case 54: {
              // AddAndReplaceKind
              literal++;

              break;
            }
#endif
#ifdef COMPILE_19936
            case 55: {
              // AddAndReplaceKind
              memset((op_array)->reserved, 0, 4 * sizeof(void *));

              break;
            }
#endif
#ifdef COMPILE_19937
            case 56: {
              // AddAndReplaceKind
              op_array_alloc_ops((op_array),
                                 (compiler_globals.context).opcodes_size);

              break;
            }
#endif
#ifdef COMPILE_19938
            case 57: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_19939
            case 58: {
              // AddAndReplaceKind
              zend_hash_clean((op_array)->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19940
            case 59: {
              // AddAndReplaceKind
              zend_hash_destroy((op_array)->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19941
            case 60: {
              // AddAndReplaceKind
              zend_hash_destroy(op_array->static_variables);

              break;
            }
#endif
#ifdef COMPILE_19942
            case 61: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)
                      zend_extension_op_array_ctor_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19943
            case 62: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)
                      zend_extension_op_array_dtor_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19944
            case 63: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)
                      zend_extension_op_array_dtor_handler,
                  op_array);

              break;
            }
#endif
#ifdef COMPILE_19945
            case 64: {
              // AddAndReplaceKind
              zend_llist_apply_with_argument(
                  &zend_extensions,
                  (llist_apply_with_arg_func_t)zend_extension_op_array_handler,
                  (op_array));

              break;
            }
#endif
#ifdef COMPILE_19946
            case 65: {
              // AddAndReplaceKind
              zend_printf("Ran out of opcode space!\nYou should probably "
                          "consider writing this huge script into a file!\n");

              break;
            }
#endif
#ifdef COMPILE_19947
            case 66: {
              // AddAndReplaceKind
              zend_update_extended_info((op_array));

              break;
            }
#endif
#ifdef COMPILE_19948
            case 67: {
              // AddInitKind
              memset(op_array, 0, sizeof(*(op_array)));

              break;
            }
#endif
#ifdef COMPILE_19949
            case 68: {
              // IfExitKind
              if (__is_neg(
                      "197-68", 34, &(op_array->static_variables),
                      sizeof(op_array->static_variables), &(op_array),
                      sizeof(op_array), &(op_array->run_time_cache),
                      sizeof(op_array->run_time_cache), &(op_array->literals),
                      sizeof(op_array->literals), &(op_array->refcount),
                      sizeof(op_array->refcount), &(op_array->vars),
                      sizeof(op_array->vars), &(op_array->last_var),
                      sizeof(op_array->last_var), &(i), sizeof(i), &(literal),
                      sizeof(literal), &(op_array->last_literal),
                      sizeof(op_array->last_literal), &(end), sizeof(end),
                      &(op_array->opcodes), sizeof(op_array->opcodes),
                      &(op_array->function_name),
                      sizeof(op_array->function_name), &(op_array->doc_comment),
                      sizeof(op_array->doc_comment),
                      &(op_array->brk_cont_array),
                      sizeof(op_array->brk_cont_array),
                      &(op_array->try_catch_array),
                      sizeof(op_array->try_catch_array), &(op_array->fn_flags),
                      sizeof(op_array->fn_flags), &(op_array->arg_info),
                      sizeof(op_array->arg_info), &(op_array->num_args),
                      sizeof(op_array->num_args),
                      &(op_array->required_num_args),
                      sizeof(op_array->required_num_args), &(op_array->last),
                      sizeof(op_array->last), &(op_array->T),
                      sizeof(op_array->T), &(op_array->last_brk_cont),
                      sizeof(op_array->last_brk_cont),
                      &(op_array->last_try_catch),
                      sizeof(op_array->last_try_catch), &(op_array->this_var),
                      sizeof(op_array->this_var), &(op_array->filename),
                      sizeof(op_array->filename), &(op_array->line_start),
                      sizeof(op_array->line_start), &(op_array->line_end),
                      sizeof(op_array->line_end), &(op_array->doc_comment_len),
                      sizeof(op_array->doc_comment_len),
                      &(op_array->early_binding),
                      sizeof(op_array->early_binding),
                      &(op_array->last_cache_slot),
                      sizeof(op_array->last_cache_slot), &(op_array->type),
                      sizeof(op_array->type), &(op_array->scope),
                      sizeof(op_array->scope), &(op_array->prototype),
                      sizeof(op_array->prototype)))
                return;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH198")) {
            case 0: {
              FREE_HASHTABLE(op_array->static_variables);

              break;
            }
#ifdef COMPILE_19950
            case 1: {
              // AddAndReplaceKind
              _zend_mem_block_size((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19951
            case 2: {
              // AddAndReplaceKind
              dlclose((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19952
            case 3: {
              // AddAndReplaceKind
              free((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19953
            case 4: {
              // AddAndReplaceKind
              list_entry_destructor((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19954
            case 5: {
              // AddAndReplaceKind
              plist_entry_destructor((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19955
            case 6: {
              // AddAndReplaceKind
              zend_vm_stack_free((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19956
            case 7: {
              // AddAndReplaceKind
              zend_vm_stack_free_int((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19957
            case 8: {
              // AddAndReplaceKind
              zend_vm_stack_push((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19958
            case 9: {
              // AddAndReplaceKind
              zend_vm_stack_push_nocheck((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19959
            case 10: {
              // GuardKind
              if (!__is_neg(
                      "198-10", 34, &(op_array), sizeof(op_array),
                      &(op_array->static_variables),
                      sizeof(op_array->static_variables),
                      &(op_array->run_time_cache),
                      sizeof(op_array->run_time_cache), &(op_array->literals),
                      sizeof(op_array->literals), &(op_array->refcount),
                      sizeof(op_array->refcount), &(op_array->vars),
                      sizeof(op_array->vars), &(i), sizeof(i),
                      &(op_array->last_var), sizeof(op_array->last_var),
                      &(literal), sizeof(literal), &(end), sizeof(end),
                      &(op_array->last_literal), sizeof(op_array->last_literal),
                      &(op_array->opcodes), sizeof(op_array->opcodes),
                      &(op_array->function_name),
                      sizeof(op_array->function_name), &(op_array->doc_comment),
                      sizeof(op_array->doc_comment),
                      &(op_array->brk_cont_array),
                      sizeof(op_array->brk_cont_array),
                      &(op_array->try_catch_array),
                      sizeof(op_array->try_catch_array), &(op_array->fn_flags),
                      sizeof(op_array->fn_flags), &(op_array->arg_info),
                      sizeof(op_array->arg_info), &(op_array->num_args),
                      sizeof(op_array->num_args), &(op_array->type),
                      sizeof(op_array->type), &(op_array->scope),
                      sizeof(op_array->scope), &(op_array->prototype),
                      sizeof(op_array->prototype),
                      &(op_array->required_num_args),
                      sizeof(op_array->required_num_args), &(op_array->last),
                      sizeof(op_array->last), &(op_array->T),
                      sizeof(op_array->T), &(op_array->last_brk_cont),
                      sizeof(op_array->last_brk_cont),
                      &(op_array->last_try_catch),
                      sizeof(op_array->last_try_catch), &(op_array->this_var),
                      sizeof(op_array->this_var), &(op_array->filename),
                      sizeof(op_array->filename), &(op_array->line_start),
                      sizeof(op_array->line_start), &(op_array->line_end),
                      sizeof(op_array->line_end), &(op_array->doc_comment_len),
                      sizeof(op_array->doc_comment_len),
                      &(op_array->early_binding),
                      sizeof(op_array->early_binding),
                      &(op_array->last_cache_slot),
                      sizeof(op_array->last_cache_slot)))
                _efree((op_array->static_variables));

              break;
            }
#endif
#ifdef COMPILE_19960
            case 11: {
              // ReplaceKind
              _efree(((op_array)->static_variables));

              break;
            }
#endif
            }
          }
        }

        break;
      }
#ifdef COMPILE_19961
      case 1: {
        // AddAndReplaceKind
        _zend_mem_block_size((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19962
      case 2: {
        // AddAndReplaceKind
        dlclose((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19963
      case 3: {
        // AddAndReplaceKind
        free((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19964
      case 4: {
        // AddAndReplaceKind
        list_entry_destructor((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19965
      case 5: {
        // AddAndReplaceKind
        plist_entry_destructor((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19966
      case 6: {
        // AddAndReplaceKind
        zend_vm_stack_free((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19967
      case 7: {
        // AddAndReplaceKind
        zend_vm_stack_free_int((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19968
      case 8: {
        // AddAndReplaceKind
        zend_vm_stack_push((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19969
      case 9: {
        // AddAndReplaceKind
        zend_vm_stack_push_nocheck((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19970
      case 10: {
        // GuardKind
        if (!__is_neg(
                "199-10", 34, &(op_array), sizeof(op_array),
                &(op_array->static_variables),
                sizeof(op_array->static_variables), &(op_array->run_time_cache),
                sizeof(op_array->run_time_cache), &(op_array->literals),
                sizeof(op_array->literals), &(op_array->refcount),
                sizeof(op_array->refcount), &(op_array->vars),
                sizeof(op_array->vars), &(i), sizeof(i), &(op_array->last_var),
                sizeof(op_array->last_var), &(literal), sizeof(literal), &(end),
                sizeof(end), &(op_array->last_literal),
                sizeof(op_array->last_literal), &(op_array->opcodes),
                sizeof(op_array->opcodes), &(op_array->function_name),
                sizeof(op_array->function_name), &(op_array->doc_comment),
                sizeof(op_array->doc_comment), &(op_array->brk_cont_array),
                sizeof(op_array->brk_cont_array), &(op_array->try_catch_array),
                sizeof(op_array->try_catch_array), &(op_array->fn_flags),
                sizeof(op_array->fn_flags), &(op_array->arg_info),
                sizeof(op_array->arg_info), &(op_array->num_args),
                sizeof(op_array->num_args), &(op_array->type),
                sizeof(op_array->type), &(op_array->scope),
                sizeof(op_array->scope), &(op_array->prototype),
                sizeof(op_array->prototype), &(op_array->required_num_args),
                sizeof(op_array->required_num_args), &(op_array->last),
                sizeof(op_array->last), &(op_array->T), sizeof(op_array->T),
                &(op_array->last_brk_cont), sizeof(op_array->last_brk_cont),
                &(op_array->last_try_catch), sizeof(op_array->last_try_catch),
                &(op_array->this_var), sizeof(op_array->this_var),
                &(op_array->filename), sizeof(op_array->filename),
                &(op_array->line_start), sizeof(op_array->line_start),
                &(op_array->line_end), sizeof(op_array->line_end),
                &(op_array->doc_comment_len), sizeof(op_array->doc_comment_len),
                &(op_array->early_binding), sizeof(op_array->early_binding),
                &(op_array->last_cache_slot),
                sizeof(op_array->last_cache_slot)))
          _efree((op_array->static_variables));

        break;
      }
#endif
#ifdef COMPILE_19971
      case 11: {
        // ReplaceKind
        _efree(((op_array)->static_variables));

        break;
      }
#endif
      }
    }
  }

  if (op_array->run_time_cache) {
    efree(op_array->run_time_cache);
  }

  if (--(*op_array->refcount) > 0) {
    return;
  }

  efree(op_array->refcount);

  if (op_array->vars) {
    i = op_array->last_var;
    while (i > 0) {
      i--;
      str_efree(op_array->vars[i].name);
    }
    efree(op_array->vars);
  }

  if (literal) {
    end = literal + op_array->last_literal;
    while (literal < end) {
      zval_dtor(&literal->constant);
      literal++;
    }
    efree(op_array->literals);
  }
  efree(op_array->opcodes);

  if (op_array->function_name) {
    efree(op_array->function_name);
  }
  if (op_array->doc_comment) {
    efree(op_array->doc_comment);
  }
  if (op_array->brk_cont_array) {
    efree(op_array->brk_cont_array);
  }
  if (op_array->try_catch_array) {
    efree(op_array->try_catch_array);
  }
  if (op_array->fn_flags & ZEND_ACC_DONE_PASS_TWO) {
    zend_llist_apply_with_argument(
        &zend_extensions,
        (llist_apply_with_arg_func_t)zend_extension_op_array_dtor_handler,
        op_array TSRMLS_CC);
  }
  if (op_array->arg_info) {
    for (i = 0; i < op_array->num_args; i++) {
      str_efree((char *)op_array->arg_info[i].name);
      if (op_array->arg_info[i].class_name &&
          !IS_INTERNED(op_array->arg_info[i].class_name)) {
        efree((char *)op_array->arg_info[i].class_name);
      }
    }
    efree(op_array->arg_info);
  }
}

void init_op(zend_op *op TSRMLS_DC) {
  memset(op, 0, sizeof(zend_op));
  op->lineno = CG(zend_lineno);
  SET_UNUSED(op->result);
}

zend_op *get_next_op(zend_op_array *op_array TSRMLS_DC) {
  zend_uint next_op_num = op_array->last++;
  zend_op *next_op;

  if (next_op_num >= CG(context).opcodes_size) {
    if (op_array->fn_flags & ZEND_ACC_INTERACTIVE) {
      /* we messed up */
      zend_printf("Ran out of opcode space!\n"
                  "You should probably consider writing this huge script into "
                  "a file!\n");
      zend_bailout();
    }
    CG(context).opcodes_size *= 4;
    op_array_alloc_ops(op_array, CG(context).opcodes_size);
  }

  next_op = &(op_array->opcodes[next_op_num]);

  init_op(next_op TSRMLS_CC);

  return next_op;
}

int get_next_op_number(zend_op_array *op_array) { return op_array->last; }

zend_brk_cont_element *get_next_brk_cont_element(zend_op_array *op_array) {
  op_array->last_brk_cont++;
  op_array->brk_cont_array =
      erealloc(op_array->brk_cont_array,
               sizeof(zend_brk_cont_element) * op_array->last_brk_cont);
  return &op_array->brk_cont_array[op_array->last_brk_cont - 1];
}

static void zend_update_extended_info(zend_op_array *op_array TSRMLS_DC) {
  zend_op *opline = op_array->opcodes, *end = opline + op_array->last;

  while (opline < end) {
    if (opline->opcode == ZEND_EXT_STMT) {
      if (opline + 1 < end) {
        if ((opline + 1)->opcode == ZEND_EXT_STMT) {
          opline->opcode = ZEND_NOP;
          opline++;
          continue;
        }
        if (opline + 1 < end) {
          opline->lineno = (opline + 1)->lineno;
        }
      } else {
        opline->opcode = ZEND_NOP;
      }
    }
    opline++;
  }
}

static void zend_extension_op_array_handler(zend_extension *extension,
                                            zend_op_array *op_array TSRMLS_DC) {
  if (extension->op_array_handler) {
    extension->op_array_handler(op_array);
  }
}

ZEND_API int pass_two(zend_op_array *op_array TSRMLS_DC) {
  zend_op *opline, *end;

  if (op_array->type != ZEND_USER_FUNCTION &&
      op_array->type != ZEND_EVAL_CODE) {
    return 0;
  }
  if (CG(compiler_options) & ZEND_COMPILE_EXTENDED_INFO) {
    zend_update_extended_info(op_array TSRMLS_CC);
  }
  if (CG(compiler_options) & ZEND_COMPILE_HANDLE_OP_ARRAY) {
    zend_llist_apply_with_argument(
        &zend_extensions,
        (llist_apply_with_arg_func_t)zend_extension_op_array_handler,
        op_array TSRMLS_CC);
  }

  if (!(op_array->fn_flags & ZEND_ACC_INTERACTIVE) &&
      CG(context).vars_size != op_array->last_var) {
    op_array->vars = (zend_compiled_variable *)erealloc(
        op_array->vars, sizeof(zend_compiled_variable) * op_array->last_var);
    CG(context).vars_size = op_array->last_var;
  }
  if (!(op_array->fn_flags & ZEND_ACC_INTERACTIVE) &&
      CG(context).opcodes_size != op_array->last) {
    op_array->opcodes = (zend_op *)erealloc(op_array->opcodes,
                                            sizeof(zend_op) * op_array->last);
    CG(context).opcodes_size = op_array->last;
  }
  if (!(op_array->fn_flags & ZEND_ACC_INTERACTIVE) &&
      CG(context).literals_size != op_array->last_literal) {
    op_array->literals = (zend_literal *)erealloc(
        op_array->literals, sizeof(zend_literal) * op_array->last_literal);
    CG(context).literals_size = op_array->last_literal;
  }

  opline = op_array->opcodes;
  end = opline + op_array->last;
  while (opline < end) {
    if (opline->op1_type == IS_CONST) {
      opline->op1.zv = &op_array->literals[opline->op1.constant].constant;
    }
    if (opline->op2_type == IS_CONST) {
      opline->op2.zv = &op_array->literals[opline->op2.constant].constant;
    }
    switch (opline->opcode) {
    case ZEND_GOTO:
      if (Z_TYPE_P(opline->op2.zv) != IS_LONG) {
        zend_resolve_goto_label(op_array, opline, 1 TSRMLS_CC);
      }
      /* break omitted intentionally */
    case ZEND_JMP:
      opline->op1.jmp_addr = &op_array->opcodes[opline->op1.opline_num];
      break;
    case ZEND_JMPZ:
    case ZEND_JMPNZ:
    case ZEND_JMPZ_EX:
    case ZEND_JMPNZ_EX:
    case ZEND_JMP_SET:
      opline->op2.jmp_addr = &op_array->opcodes[opline->op2.opline_num];
      break;
    }
    ZEND_VM_SET_OPCODE_HANDLER(opline);
    opline++;
  }

  op_array->fn_flags |= ZEND_ACC_DONE_PASS_TWO;
  return 0;
}

int print_class(zend_class_entry *class_entry TSRMLS_DC) {
  printf("Class %s:\n", class_entry->name);
  zend_hash_apply(&class_entry->function_table,
                  (apply_func_t)pass_two TSRMLS_CC);
  printf("End of class %s.\n\n", class_entry->name);
  return 0;
}

ZEND_API unary_op_type get_unary_op(int opcode) {
  switch (opcode) {
  case ZEND_BW_NOT:
    return (unary_op_type)bitwise_not_function;
    break;
  case ZEND_BOOL_NOT:
    return (unary_op_type)boolean_not_function;
    break;
  default:
    return (unary_op_type)NULL;
    break;
  }
}

ZEND_API binary_op_type get_binary_op(int opcode) {
  switch (opcode) {
  case ZEND_ADD:
  case ZEND_ASSIGN_ADD:
    return (binary_op_type)add_function;
    break;
  case ZEND_SUB:
  case ZEND_ASSIGN_SUB:
    return (binary_op_type)sub_function;
    break;
  case ZEND_MUL:
  case ZEND_ASSIGN_MUL:
    return (binary_op_type)mul_function;
    break;
  case ZEND_DIV:
  case ZEND_ASSIGN_DIV:
    return (binary_op_type)div_function;
    break;
  case ZEND_MOD:
  case ZEND_ASSIGN_MOD:
    return (binary_op_type)mod_function;
    break;
  case ZEND_SL:
  case ZEND_ASSIGN_SL:
    return (binary_op_type)shift_left_function;
    break;
  case ZEND_SR:
  case ZEND_ASSIGN_SR:
    return (binary_op_type)shift_right_function;
    break;
  case ZEND_CONCAT:
  case ZEND_ASSIGN_CONCAT:
    return (binary_op_type)concat_function;
    break;
  case ZEND_IS_IDENTICAL:
    return (binary_op_type)is_identical_function;
    break;
  case ZEND_IS_NOT_IDENTICAL:
    return (binary_op_type)is_not_identical_function;
    break;
  case ZEND_IS_EQUAL:
    return (binary_op_type)is_equal_function;
    break;
  case ZEND_IS_NOT_EQUAL:
    return (binary_op_type)is_not_equal_function;
    break;
  case ZEND_IS_SMALLER:
    return (binary_op_type)is_smaller_function;
    break;
  case ZEND_IS_SMALLER_OR_EQUAL:
    return (binary_op_type)is_smaller_or_equal_function;
    break;
  case ZEND_BW_OR:
  case ZEND_ASSIGN_BW_OR:
    return (binary_op_type)bitwise_or_function;
    break;
  case ZEND_BW_AND:
  case ZEND_ASSIGN_BW_AND:
    return (binary_op_type)bitwise_and_function;
    break;
  case ZEND_BW_XOR:
  case ZEND_ASSIGN_BW_XOR:
    return (binary_op_type)bitwise_xor_function;
    break;
  case ZEND_BOOL_XOR:
    return (binary_op_type)boolean_xor_function;
    break;
  default:
    return (binary_op_type)NULL;
    break;
  }
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
