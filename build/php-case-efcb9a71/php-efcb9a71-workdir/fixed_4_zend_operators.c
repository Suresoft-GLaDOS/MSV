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

#include <ctype.h>

#include "zend.h"
#include "zend_API.h"
#include "zend_closures.h"
#include "zend_exceptions.h"
#include "zend_globals.h"
#include "zend_list.h"
#include "zend_multiply.h"
#include "zend_operators.h"
#include "zend_strtod.h"
#include "zend_variables.h"

#define LONG_SIGN_MASK (1L << (8 * sizeof(long) - 1))

#if ZEND_USE_TOLOWER_L
#include <locale.h>
static _locale_t current_locale = NULL;
/* this is true global! may lead to strange effects on ZTS, but so may
 * setlocale() */
#define zend_tolower(c) _tolower_l(c, current_locale)
#else
#define zend_tolower(c) tolower(c)
#endif

#define TYPE_PAIR(t1, t2) (((t1) << 4) | (t2))

ZEND_API int zend_atoi(const char *str, int str_len) /* {{{ */
{
  int retval;

  if (!str_len) {
    str_len = strlen(str);
  }
  retval = strtol(str, NULL, 0);
  if (str_len > 0) {
    switch (str[str_len - 1]) {
    case 'g':
    case 'G':
      retval *= 1024;
      /* break intentionally missing */
    case 'm':
    case 'M':
      retval *= 1024;
      /* break intentionally missing */
    case 'k':
    case 'K':
      retval *= 1024;
      break;
    }
  }
  return retval;
}
/* }}} */

ZEND_API long zend_atol(const char *str, int str_len) /* {{{ */
{
  long retval;

  if (!str_len) {
    str_len = strlen(str);
  }
  retval = strtol(str, NULL, 0);
  if (str_len > 0) {
    switch (str[str_len - 1]) {
    case 'g':
    case 'G':
      retval *= 1024;
      /* break intentionally missing */
    case 'm':
    case 'M':
      retval *= 1024;
      /* break intentionally missing */
    case 'k':
    case 'K':
      retval *= 1024;
      break;
    }
  }
  return retval;
}
/* }}} */

ZEND_API double zend_string_to_double(const char *number,
                                      zend_uint length) /* {{{ */
{
  double divisor = 10.0;
  double result = 0.0;
  double exponent;
  const char *end = number + length;
  const char *digit = number;

  if (!length) {
    return result;
  }

  while (digit < end) {
    if ((*digit <= '9' && *digit >= '0')) {
      result *= 10;
      result += *digit - '0';
    } else if (*digit == '.') {
      digit++;
      break;
    } else if (toupper(*digit) == 'E') {
      exponent = (double)atoi(digit + 1);
      result *= pow(10.0, exponent);
      return result;
    } else {
      return result;
    }
    digit++;
  }

  while (digit < end) {
    if ((*digit <= '9' && *digit >= '0')) {
      result += (*digit - '0') / divisor;
      divisor *= 10;
    } else if (toupper(*digit) == 'E') {
      exponent = (double)atoi(digit + 1);
      result *= pow(10.0, exponent);
      return result;
    } else {
      return result;
    }
    digit++;
  }
  return result;
}
/* }}} */

ZEND_API void convert_scalar_to_number(zval *op TSRMLS_DC) /* {{{ */
{
  switch (Z_TYPE_P(op)) {
  case IS_STRING: {
    char *strval;

    strval = Z_STRVAL_P(op);
    if ((Z_TYPE_P(op) = is_numeric_string(strval, Z_STRLEN_P(op), &Z_LVAL_P(op),
                                          &Z_DVAL_P(op), 1)) == 0) {
      ZVAL_LONG(op, 0);
    }
    STR_FREE(strval);
    break;
  }
  case IS_BOOL:
    Z_TYPE_P(op) = IS_LONG;
    break;
  case IS_RESOURCE:
    zend_list_delete(Z_LVAL_P(op));
    Z_TYPE_P(op) = IS_LONG;
    break;
  case IS_OBJECT:
    convert_to_long_base(op, 10);
    break;
  case IS_NULL:
    ZVAL_LONG(op, 0);
    break;
  }
}
/* }}} */

/* {{{ zendi_convert_scalar_to_number */
#define zendi_convert_scalar_to_number(op, holder, result)                     \
  if (op == result) {                                                          \
    if (Z_TYPE_P(op) != IS_LONG) {                                             \
      convert_scalar_to_number(op TSRMLS_CC);                                  \
    }                                                                          \
  } else {                                                                     \
    switch (Z_TYPE_P(op)) {                                                    \
    case IS_STRING: {                                                          \
      if ((Z_TYPE(holder) =                                                    \
               is_numeric_string(Z_STRVAL_P(op), Z_STRLEN_P(op),               \
                                 &Z_LVAL(holder), &Z_DVAL(holder), 1)) == 0) { \
        ZVAL_LONG(&(holder), 0);                                               \
      }                                                                        \
      (op) = &(holder);                                                        \
      break;                                                                   \
    }                                                                          \
    case IS_BOOL:                                                              \
    case IS_RESOURCE:                                                          \
      ZVAL_LONG(&(holder), Z_LVAL_P(op));                                      \
      (op) = &(holder);                                                        \
      break;                                                                   \
    case IS_NULL:                                                              \
      ZVAL_LONG(&(holder), 0);                                                 \
      (op) = &(holder);                                                        \
      break;                                                                   \
    case IS_OBJECT:                                                            \
      (holder) = (*(op));                                                      \
      zval_copy_ctor(&(holder));                                               \
      convert_to_long_base(&(holder), 10);                                     \
      if (Z_TYPE(holder) == IS_LONG) {                                         \
        (op) = &(holder);                                                      \
      }                                                                        \
      break;                                                                   \
    }                                                                          \
  }

/* }}} */

/* {{{ zendi_convert_to_long */
#define zendi_convert_to_long(op, holder, result)                              \
  if (op == result) {                                                          \
    convert_to_long(op);                                                       \
  } else if (Z_TYPE_P(op) != IS_LONG) {                                        \
    switch (Z_TYPE_P(op)) {                                                    \
    case IS_NULL:                                                              \
      Z_LVAL(holder) = 0;                                                      \
      break;                                                                   \
    case IS_DOUBLE:                                                            \
      Z_LVAL(holder) = zend_dval_to_lval(Z_DVAL_P(op));                        \
      break;                                                                   \
    case IS_STRING:                                                            \
      Z_LVAL(holder) = strtol(Z_STRVAL_P(op), NULL, 10);                       \
      break;                                                                   \
    case IS_ARRAY:                                                             \
      Z_LVAL(holder) = (zend_hash_num_elements(Z_ARRVAL_P(op)) ? 1 : 0);       \
      break;                                                                   \
    case IS_OBJECT:                                                            \
      (holder) = (*(op));                                                      \
      zval_copy_ctor(&(holder));                                               \
      convert_to_long_base(&(holder), 10);                                     \
      break;                                                                   \
    case IS_BOOL:                                                              \
    case IS_RESOURCE:                                                          \
      Z_LVAL(holder) = Z_LVAL_P(op);                                           \
      break;                                                                   \
    default:                                                                   \
      zend_error(E_WARNING, "Cannot convert to ordinal value");                \
      Z_LVAL(holder) = 0;                                                      \
      break;                                                                   \
    }                                                                          \
    Z_TYPE(holder) = IS_LONG;                                                  \
    (op) = &(holder);                                                          \
  }

/* }}} */

/* {{{ zendi_convert_to_boolean */
#define zendi_convert_to_boolean(op, holder, result)                           \
  if (op == result) {                                                          \
    convert_to_boolean(op);                                                    \
  } else if (Z_TYPE_P(op) != IS_BOOL) {                                        \
    switch (Z_TYPE_P(op)) {                                                    \
    case IS_NULL:                                                              \
      Z_LVAL(holder) = 0;                                                      \
      break;                                                                   \
    case IS_RESOURCE:                                                          \
    case IS_LONG:                                                              \
      Z_LVAL(holder) = (Z_LVAL_P(op) ? 1 : 0);                                 \
      break;                                                                   \
    case IS_DOUBLE:                                                            \
      Z_LVAL(holder) = (Z_DVAL_P(op) ? 1 : 0);                                 \
      break;                                                                   \
    case IS_STRING:                                                            \
      if (Z_STRLEN_P(op) == 0 ||                                               \
          (Z_STRLEN_P(op) == 1 && Z_STRVAL_P(op)[0] == '0')) {                 \
        Z_LVAL(holder) = 0;                                                    \
      } else {                                                                 \
        Z_LVAL(holder) = 1;                                                    \
      }                                                                        \
      break;                                                                   \
    case IS_ARRAY:                                                             \
      Z_LVAL(holder) = (zend_hash_num_elements(Z_ARRVAL_P(op)) ? 1 : 0);       \
      break;                                                                   \
    case IS_OBJECT:                                                            \
      (holder) = (*(op));                                                      \
      zval_copy_ctor(&(holder));                                               \
      convert_to_boolean(&(holder));                                           \
      break;                                                                   \
    default:                                                                   \
      Z_LVAL(holder) = 0;                                                      \
      break;                                                                   \
    }                                                                          \
    Z_TYPE(holder) = IS_BOOL;                                                  \
    (op) = &(holder);                                                          \
  }

/* }}} */

/* {{{ convert_object_to_type */
#define convert_object_to_type(op, ctype, conv_func)                           \
  if (Z_OBJ_HT_P(op)->cast_object) {                                           \
    zval dst;                                                                  \
    if (Z_OBJ_HT_P(op)->cast_object(op, &dst, ctype TSRMLS_CC) == FAILURE) {   \
      zend_error(E_RECOVERABLE_ERROR,                                          \
                 "Object of class %s could not be converted to %s",            \
                 Z_OBJCE_P(op)->name, zend_get_type_by_const(ctype));          \
    } else {                                                                   \
      zval_dtor(op);                                                           \
      Z_TYPE_P(op) = ctype;                                                    \
      op->value = dst.value;                                                   \
    }                                                                          \
  } else {                                                                     \
    if (Z_OBJ_HT_P(op)->get) {                                                 \
      zval *newop = Z_OBJ_HT_P(op)->get(op TSRMLS_CC);                         \
      if (Z_TYPE_P(newop) != IS_OBJECT) {                                      \
        /* for safety - avoid loop */                                          \
        zval_dtor(op);                                                         \
        *op = *newop;                                                          \
        FREE_ZVAL(newop);                                                      \
        conv_func(op);                                                         \
      }                                                                        \
    }                                                                          \
  }

/* }}} */

ZEND_API void convert_to_long(zval *op) /* {{{ */
{
  if (Z_TYPE_P(op) != IS_LONG) {
    convert_to_long_base(op, 10);
  }
}
/* }}} */

ZEND_API void convert_to_long_base(zval *op, int base) /* {{{ */
{
  long tmp;

  switch (Z_TYPE_P(op)) {
  case IS_NULL:
    Z_LVAL_P(op) = 0;
    break;
  case IS_RESOURCE: {
    TSRMLS_FETCH();

    zend_list_delete(Z_LVAL_P(op));
  }
    /* break missing intentionally */
  case IS_BOOL:
  case IS_LONG:
    break;
  case IS_DOUBLE:
    Z_LVAL_P(op) = zend_dval_to_lval(Z_DVAL_P(op));
    break;
  case IS_STRING: {
    char *strval = Z_STRVAL_P(op);

    Z_LVAL_P(op) = strtol(strval, NULL, base);
    STR_FREE(strval);
  } break;
  case IS_ARRAY:
    tmp = (zend_hash_num_elements(Z_ARRVAL_P(op)) ? 1 : 0);
    zval_dtor(op);
    Z_LVAL_P(op) = tmp;
    break;
  case IS_OBJECT: {
    int retval = 1;
    TSRMLS_FETCH();

    convert_object_to_type(op, IS_LONG, convert_to_long);

    if (Z_TYPE_P(op) == IS_LONG) {
      return;
    }
    zend_error(E_NOTICE, "Object of class %s could not be converted to int",
               Z_OBJCE_P(op)->name);

    zval_dtor(op);
    ZVAL_LONG(op, retval);
    return;
  }
  default:
    zend_error(E_WARNING, "Cannot convert to ordinal value");
    zval_dtor(op);
    Z_LVAL_P(op) = 0;
    break;
  }

  Z_TYPE_P(op) = IS_LONG;
}
/* }}} */

ZEND_API void convert_to_double(zval *op) /* {{{ */
{
  double tmp;

  switch (Z_TYPE_P(op)) {
  case IS_NULL:
    Z_DVAL_P(op) = 0.0;
    break;
  case IS_RESOURCE: {
    TSRMLS_FETCH();

    zend_list_delete(Z_LVAL_P(op));
  }
    /* break missing intentionally */
  case IS_BOOL:
  case IS_LONG:
    Z_DVAL_P(op) = (double)Z_LVAL_P(op);
    break;
  case IS_DOUBLE:
    break;
  case IS_STRING: {
    char *strval = Z_STRVAL_P(op);

    Z_DVAL_P(op) = zend_strtod(strval, NULL);
    STR_FREE(strval);
  } break;
  case IS_ARRAY:
    tmp = (zend_hash_num_elements(Z_ARRVAL_P(op)) ? 1 : 0);
    zval_dtor(op);
    Z_DVAL_P(op) = tmp;
    break;
  case IS_OBJECT: {
    double retval = 1.0;
    TSRMLS_FETCH();

    convert_object_to_type(op, IS_DOUBLE, convert_to_double);

    if (Z_TYPE_P(op) == IS_DOUBLE) {
      return;
    }
    zend_error(E_NOTICE, "Object of class %s could not be converted to double",
               Z_OBJCE_P(op)->name);

    zval_dtor(op);
    ZVAL_DOUBLE(op, retval);
    break;
  }
  default:
    zend_error(E_WARNING, "Cannot convert to real value (type=%d)",
               Z_TYPE_P(op));
    zval_dtor(op);
    Z_DVAL_P(op) = 0;
    break;
  }
  Z_TYPE_P(op) = IS_DOUBLE;
}
/* }}} */

ZEND_API void convert_to_null(zval *op) /* {{{ */
{
  if (Z_TYPE_P(op) == IS_OBJECT) {
    if (Z_OBJ_HT_P(op)->cast_object) {
      zval *org;
      TSRMLS_FETCH();

      ALLOC_ZVAL(org);
      *org = *op;
      if (Z_OBJ_HT_P(op)->cast_object(org, op, IS_NULL TSRMLS_CC) == SUCCESS) {
        zval_dtor(org);
        return;
      }
      *op = *org;
      FREE_ZVAL(org);
    }
  }

  zval_dtor(op);
  Z_TYPE_P(op) = IS_NULL;
}
/* }}} */

ZEND_API void convert_to_boolean(zval *op) /* {{{ */
{
  int tmp;

  switch (Z_TYPE_P(op)) {
  case IS_BOOL:
    break;
  case IS_NULL:
    Z_LVAL_P(op) = 0;
    break;
  case IS_RESOURCE: {
    TSRMLS_FETCH();

    zend_list_delete(Z_LVAL_P(op));
  }
    /* break missing intentionally */
  case IS_LONG:
    Z_LVAL_P(op) = (Z_LVAL_P(op) ? 1 : 0);
    break;
  case IS_DOUBLE:
    Z_LVAL_P(op) = (Z_DVAL_P(op) ? 1 : 0);
    break;
  case IS_STRING: {
    char *strval = Z_STRVAL_P(op);

    if (Z_STRLEN_P(op) == 0 ||
        (Z_STRLEN_P(op) == 1 && Z_STRVAL_P(op)[0] == '0')) {
      Z_LVAL_P(op) = 0;
    } else {
      Z_LVAL_P(op) = 1;
    }
    STR_FREE(strval);
  } break;
  case IS_ARRAY:
    tmp = (zend_hash_num_elements(Z_ARRVAL_P(op)) ? 1 : 0);
    zval_dtor(op);
    Z_LVAL_P(op) = tmp;
    break;
  case IS_OBJECT: {
    zend_bool retval = 1;
    TSRMLS_FETCH();

    convert_object_to_type(op, IS_BOOL, convert_to_boolean);

    if (Z_TYPE_P(op) == IS_BOOL) {
      return;
    }

    zval_dtor(op);
    ZVAL_BOOL(op, retval);
    break;
  }
  default:
    zval_dtor(op);
    Z_LVAL_P(op) = 0;
    break;
  }
  Z_TYPE_P(op) = IS_BOOL;
}
/* }}} */

ZEND_API void _convert_to_string(zval *op ZEND_FILE_LINE_DC) /* {{{ */
{
  long lval;
  double dval;

  switch (Z_TYPE_P(op)) {
  case IS_NULL:
    Z_STRVAL_P(op) = STR_EMPTY_ALLOC();
    Z_STRLEN_P(op) = 0;
    break;
  case IS_STRING:
    break;
  case IS_BOOL:
    if (Z_LVAL_P(op)) {
      Z_STRVAL_P(op) = estrndup_rel("1", 1);
      Z_STRLEN_P(op) = 1;
    } else {
      Z_STRVAL_P(op) = STR_EMPTY_ALLOC();
      Z_STRLEN_P(op) = 0;
    }
    break;
  case IS_RESOURCE: {
    long tmp = Z_LVAL_P(op);
    TSRMLS_FETCH();

    zend_list_delete(Z_LVAL_P(op));
    Z_STRLEN_P(op) = zend_spprintf(&Z_STRVAL_P(op), 0, "Resource id #%ld", tmp);
    break;
  }
  case IS_LONG:
    lval = Z_LVAL_P(op);

    Z_STRLEN_P(op) = zend_spprintf(&Z_STRVAL_P(op), 0, "%ld", lval);
    break;
  case IS_DOUBLE: {
    TSRMLS_FETCH();
    dval = Z_DVAL_P(op);
    Z_STRLEN_P(op) =
        zend_spprintf(&Z_STRVAL_P(op), 0, "%.*G", (int)EG(precision), dval);
    /* %G already handles removing trailing zeros from the fractional part, yay
     */
    break;
  }
  case IS_ARRAY:
    zend_error(E_NOTICE, "Array to string conversion");
    zval_dtor(op);
    Z_STRVAL_P(op) = estrndup_rel("Array", sizeof("Array") - 1);
    Z_STRLEN_P(op) = sizeof("Array") - 1;
    break;
  case IS_OBJECT: {
    TSRMLS_FETCH();

    convert_object_to_type(op, IS_STRING, convert_to_string);

    if (Z_TYPE_P(op) == IS_STRING) {
      return;
    }

    zend_error(E_NOTICE, "Object of class %s to string conversion",
               Z_OBJCE_P(op)->name);
    zval_dtor(op);
    Z_STRVAL_P(op) = estrndup_rel("Object", sizeof("Object") - 1);
    Z_STRLEN_P(op) = sizeof("Object") - 1;
    break;
  }
  default:
    zval_dtor(op);
    ZVAL_BOOL(op, 0);
    break;
  }
  Z_TYPE_P(op) = IS_STRING;
}
/* }}} */

static void convert_scalar_to_array(zval *op, int type TSRMLS_DC) /* {{{ */
{
  zval *entry;

  ALLOC_ZVAL(entry);
  *entry = *op;
  INIT_PZVAL(entry);

  switch (type) {
  case IS_ARRAY:
    ALLOC_HASHTABLE(Z_ARRVAL_P(op));
    zend_hash_init(Z_ARRVAL_P(op), 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_index_update(Z_ARRVAL_P(op), 0, (void *)&entry, sizeof(zval *),
                           NULL);
    Z_TYPE_P(op) = IS_ARRAY;
    break;
  case IS_OBJECT:
    object_init(op);
    zend_hash_update(Z_OBJPROP_P(op), "scalar", sizeof("scalar"),
                     (void *)&entry, sizeof(zval *), NULL);
    break;
  }
}
/* }}} */

ZEND_API void convert_to_array(zval *op) /* {{{ */
{
  TSRMLS_FETCH();

  switch (Z_TYPE_P(op)) {
  case IS_ARRAY:
    break;
    /* OBJECTS_OPTIMIZE */
  case IS_OBJECT: {
    zval *tmp;
    HashTable *ht;

    ALLOC_HASHTABLE(ht);
    zend_hash_init(ht, 0, NULL, ZVAL_PTR_DTOR, 0);
    if (Z_OBJCE_P(op) == zend_ce_closure) {
      convert_scalar_to_array(op, IS_ARRAY TSRMLS_CC);
      if (Z_TYPE_P(op) == IS_ARRAY) {
        zend_hash_destroy(ht);
        FREE_HASHTABLE(ht);
        return;
      }
    } else if (Z_OBJ_HT_P(op)->get_properties) {
      HashTable *obj_ht = Z_OBJ_HT_P(op)->get_properties(op TSRMLS_CC);
      if (obj_ht) {
        zend_hash_copy(ht, obj_ht, (copy_ctor_func_t)zval_add_ref, (void *)&tmp,
                       sizeof(zval *));
      }
    } else {
      convert_object_to_type(op, IS_ARRAY, convert_to_array);

      if (Z_TYPE_P(op) == IS_ARRAY) {
        zend_hash_destroy(ht);
        FREE_HASHTABLE(ht);
        return;
      }
    }
    zval_dtor(op);
    Z_TYPE_P(op) = IS_ARRAY;
    Z_ARRVAL_P(op) = ht;
  } break;
  case IS_NULL:
    ALLOC_HASHTABLE(Z_ARRVAL_P(op));
    zend_hash_init(Z_ARRVAL_P(op), 0, NULL, ZVAL_PTR_DTOR, 0);
    Z_TYPE_P(op) = IS_ARRAY;
    break;
  default:
    convert_scalar_to_array(op, IS_ARRAY TSRMLS_CC);
    break;
  }
}
/* }}} */

ZEND_API void convert_to_object(zval *op) /* {{{ */
{
  TSRMLS_FETCH();

  switch (Z_TYPE_P(op)) {
  case IS_ARRAY: {
    object_and_properties_init(op, zend_standard_class_def, Z_ARRVAL_P(op));
    break;
  }
  case IS_OBJECT:
    break;
  case IS_NULL:
    object_init(op);
    break;
  default:
    convert_scalar_to_array(op, IS_OBJECT TSRMLS_CC);
    break;
  }
}
/* }}} */

ZEND_API void multi_convert_to_long_ex(int argc, ...) /* {{{ */
{
  zval **arg;
  va_list ap;

  va_start(ap, argc);

  while (argc--) {
    arg = va_arg(ap, zval **);
    convert_to_long_ex(arg);
  }

  va_end(ap);
}
/* }}} */

ZEND_API void multi_convert_to_double_ex(int argc, ...) /* {{{ */
{
  zval **arg;
  va_list ap;

  va_start(ap, argc);

  while (argc--) {
    arg = va_arg(ap, zval **);
    convert_to_double_ex(arg);
  }

  va_end(ap);
}
/* }}} */

ZEND_API void multi_convert_to_string_ex(int argc, ...) /* {{{ */
{
  zval **arg;
  va_list ap;

  va_start(ap, argc);

  while (argc--) {
    arg = va_arg(ap, zval **);
    convert_to_string_ex(arg);
  }

  va_end(ap);
}
/* }}} */

ZEND_API int add_function(zval *result, zval *op1,
                          zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  int converted = 0;

  while (1) {
    switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
    case TYPE_PAIR(IS_LONG, IS_LONG): {
      long lval = Z_LVAL_P(op1) + Z_LVAL_P(op2);

      /* check for overflow by comparing sign bits */
      if ((Z_LVAL_P(op1) & LONG_SIGN_MASK) ==
              (Z_LVAL_P(op2) & LONG_SIGN_MASK) &&
          (Z_LVAL_P(op1) & LONG_SIGN_MASK) != (lval & LONG_SIGN_MASK)) {

        ZVAL_DOUBLE(result, (double)Z_LVAL_P(op1) + (double)Z_LVAL_P(op2));
      } else {
        ZVAL_LONG(result, lval);
      }
      return SUCCESS;
    }

    case TYPE_PAIR(IS_LONG, IS_DOUBLE):
      ZVAL_DOUBLE(result, ((double)Z_LVAL_P(op1)) + Z_DVAL_P(op2));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_LONG):
      ZVAL_DOUBLE(result, Z_DVAL_P(op1) + ((double)Z_LVAL_P(op2)));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_DOUBLE):
      ZVAL_DOUBLE(result, Z_DVAL_P(op1) + Z_DVAL_P(op2));
      return SUCCESS;

    case TYPE_PAIR(IS_ARRAY, IS_ARRAY): {
      zval *tmp;

      if ((result == op1) && (result == op2)) {
        /* $a += $a */
        return SUCCESS;
      }
      if (result != op1) {
        *result = *op1;
        zval_copy_ctor(result);
      }
      zend_hash_merge(Z_ARRVAL_P(result), Z_ARRVAL_P(op2),
                      (void (*)(void *pData))zval_add_ref, (void *)&tmp,
                      sizeof(zval *), 0);
      return SUCCESS;
    }

    default:
      if (!converted) {
        zendi_convert_scalar_to_number(op1, op1_copy, result);
        zendi_convert_scalar_to_number(op2, op2_copy, result);
        converted = 1;
      } else {
        zend_error(E_ERROR, "Unsupported operand types");
        return FAILURE; /* unknown datatype */
      }
    }
  }
}
/* }}} */

ZEND_API int sub_function(zval *result, zval *op1,
                          zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  int converted = 0;

  while (1) {
    switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
    case TYPE_PAIR(IS_LONG, IS_LONG): {
      long lval = Z_LVAL_P(op1) - Z_LVAL_P(op2);

      /* check for overflow by comparing sign bits */
      if ((Z_LVAL_P(op1) & LONG_SIGN_MASK) !=
              (Z_LVAL_P(op2) & LONG_SIGN_MASK) &&
          (Z_LVAL_P(op1) & LONG_SIGN_MASK) != (lval & LONG_SIGN_MASK)) {

        ZVAL_DOUBLE(result, (double)Z_LVAL_P(op1) - (double)Z_LVAL_P(op2));
      } else {
        ZVAL_LONG(result, lval);
      }
      return SUCCESS;
    }
    case TYPE_PAIR(IS_LONG, IS_DOUBLE):
      ZVAL_DOUBLE(result, ((double)Z_LVAL_P(op1)) - Z_DVAL_P(op2));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_LONG):
      ZVAL_DOUBLE(result, Z_DVAL_P(op1) - ((double)Z_LVAL_P(op2)));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_DOUBLE):
      ZVAL_DOUBLE(result, Z_DVAL_P(op1) - Z_DVAL_P(op2));
      return SUCCESS;

    default:
      if (!converted) {
        zendi_convert_scalar_to_number(op1, op1_copy, result);
        zendi_convert_scalar_to_number(op2, op2_copy, result);
        converted = 1;
      } else {
        zend_error(E_ERROR, "Unsupported operand types");
        return FAILURE; /* unknown datatype */
      }
    }
  }
}
/* }}} */

ZEND_API int mul_function(zval *result, zval *op1,
                          zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  int converted = 0;

  while (1) {
    switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
    case TYPE_PAIR(IS_LONG, IS_LONG): {
      long overflow;

      ZEND_SIGNED_MULTIPLY_LONG(Z_LVAL_P(op1), Z_LVAL_P(op2), Z_LVAL_P(result),
                                Z_DVAL_P(result), overflow);
      Z_TYPE_P(result) = overflow ? IS_DOUBLE : IS_LONG;
      return SUCCESS;
    }
    case TYPE_PAIR(IS_LONG, IS_DOUBLE):
      ZVAL_DOUBLE(result, ((double)Z_LVAL_P(op1)) * Z_DVAL_P(op2));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_LONG):
      ZVAL_DOUBLE(result, Z_DVAL_P(op1) * ((double)Z_LVAL_P(op2)));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_DOUBLE):
      ZVAL_DOUBLE(result, Z_DVAL_P(op1) * Z_DVAL_P(op2));
      return SUCCESS;

    default:
      if (!converted) {
        zendi_convert_scalar_to_number(op1, op1_copy, result);
        zendi_convert_scalar_to_number(op2, op2_copy, result);
        converted = 1;
      } else {
        zend_error(E_ERROR, "Unsupported operand types");
        return FAILURE; /* unknown datatype */
      }
    }
  }
}
/* }}} */

ZEND_API int div_function(zval *result, zval *op1,
                          zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  int converted = 0;

  while (1) {
    switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
    case TYPE_PAIR(IS_LONG, IS_LONG):
      if (Z_LVAL_P(op2) == 0) {
        zend_error(E_WARNING, "Division by zero");
        ZVAL_BOOL(result, 0);
        return FAILURE; /* division by zero */
      } else if (Z_LVAL_P(op2) == -1 && Z_LVAL_P(op1) == LONG_MIN) {
        /* Prevent overflow error/crash */
        ZVAL_DOUBLE(result, (double)LONG_MIN / -1);
        return SUCCESS;
      }
      if (Z_LVAL_P(op1) % Z_LVAL_P(op2) == 0) { /* integer */
        ZVAL_LONG(result, Z_LVAL_P(op1) / Z_LVAL_P(op2));
      } else {
        ZVAL_DOUBLE(result, ((double)Z_LVAL_P(op1)) / Z_LVAL_P(op2));
      }
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_LONG):
      if (Z_LVAL_P(op2) == 0) {
        zend_error(E_WARNING, "Division by zero");
        ZVAL_BOOL(result, 0);
        return FAILURE; /* division by zero */
      }
      ZVAL_DOUBLE(result, Z_DVAL_P(op1) / (double)Z_LVAL_P(op2));
      return SUCCESS;

    case TYPE_PAIR(IS_LONG, IS_DOUBLE):
      if (Z_DVAL_P(op2) == 0) {
        zend_error(E_WARNING, "Division by zero");
        ZVAL_BOOL(result, 0);
        return FAILURE; /* division by zero */
      }
      ZVAL_DOUBLE(result, (double)Z_LVAL_P(op1) / Z_DVAL_P(op2));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_DOUBLE):
      if (Z_DVAL_P(op2) == 0) {
        zend_error(E_WARNING, "Division by zero");
        ZVAL_BOOL(result, 0);
        return FAILURE; /* division by zero */
      }
      ZVAL_DOUBLE(result, Z_DVAL_P(op1) / Z_DVAL_P(op2));
      return SUCCESS;

    default:
      if (!converted) {
        zendi_convert_scalar_to_number(op1, op1_copy, result);
        zendi_convert_scalar_to_number(op2, op2_copy, result);
        converted = 1;
      } else {
        zend_error(E_ERROR, "Unsupported operand types");
        return FAILURE; /* unknown datatype */
      }
    }
  }
}
/* }}} */

ZEND_API int mod_function(zval *result, zval *op1,
                          zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  long op1_lval;

  zendi_convert_to_long(op1, op1_copy, result);
  op1_lval = Z_LVAL_P(op1);
  zendi_convert_to_long(op2, op2_copy, result);

  if (Z_LVAL_P(op2) == 0) {
    zend_error(E_WARNING, "Division by zero");
    ZVAL_BOOL(result, 0);
    return FAILURE; /* modulus by zero */
  }

  if (Z_LVAL_P(op2) == -1) {
    /* Prevent overflow error/crash if op1==LONG_MIN */
    ZVAL_LONG(result, 0);
    return SUCCESS;
  }

  ZVAL_LONG(result, op1_lval % Z_LVAL_P(op2));
  return SUCCESS;
}
/* }}} */

ZEND_API int boolean_xor_function(zval *result, zval *op1,
                                  zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  long op1_lval;

  zendi_convert_to_boolean(op1, op1_copy, result);
  op1_lval = Z_LVAL_P(op1);
  zendi_convert_to_boolean(op2, op2_copy, result);
  ZVAL_BOOL(result, op1_lval ^ Z_LVAL_P(op2));
  return SUCCESS;
}
/* }}} */

ZEND_API int boolean_not_function(zval *result, zval *op1 TSRMLS_DC) /* {{{ */
{
  zval op1_copy;

  zendi_convert_to_boolean(op1, op1_copy, result);
  ZVAL_BOOL(result, !Z_LVAL_P(op1));
  return SUCCESS;
}
/* }}} */

ZEND_API int bitwise_not_function(zval *result, zval *op1 TSRMLS_DC) /* {{{ */
{
  zval op1_copy = *op1;

  op1 = &op1_copy;

  if (Z_TYPE_P(op1) == IS_LONG) {
    ZVAL_LONG(result, ~Z_LVAL_P(op1));
    return SUCCESS;
  } else if (Z_TYPE_P(op1) == IS_DOUBLE) {
    ZVAL_LONG(result, ~zend_dval_to_lval(Z_DVAL_P(op1)));
    return SUCCESS;
  } else if (Z_TYPE_P(op1) == IS_STRING) {
    int i;

    Z_TYPE_P(result) = IS_STRING;
    Z_STRVAL_P(result) = estrndup(Z_STRVAL_P(op1), Z_STRLEN_P(op1));
    Z_STRLEN_P(result) = Z_STRLEN_P(op1);
    for (i = 0; i < Z_STRLEN_P(op1); i++) {
      Z_STRVAL_P(result)[i] = ~Z_STRVAL_P(op1)[i];
    }
    return SUCCESS;
  }
  zend_error(E_ERROR, "Unsupported operand types");
  return FAILURE; /* unknown datatype */
}
/* }}} */

ZEND_API int bitwise_or_function(zval *result, zval *op1,
                                 zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  long op1_lval;

  if (Z_TYPE_P(op1) == IS_STRING && Z_TYPE_P(op2) == IS_STRING) {
    zval *longer, *shorter;
    char *result_str;
    int i, result_len;

    if (Z_STRLEN_P(op1) >= Z_STRLEN_P(op2)) {
      longer = op1;
      shorter = op2;
    } else {
      longer = op2;
      shorter = op1;
    }

    Z_TYPE_P(result) = IS_STRING;
    result_len = Z_STRLEN_P(longer);
    result_str = estrndup(Z_STRVAL_P(longer), Z_STRLEN_P(longer));
    for (i = 0; i < Z_STRLEN_P(shorter); i++) {
      result_str[i] |= Z_STRVAL_P(shorter)[i];
    }
    if (result == op1) {
      STR_FREE(Z_STRVAL_P(result));
    }
    Z_STRVAL_P(result) = result_str;
    Z_STRLEN_P(result) = result_len;
    return SUCCESS;
  }
  // prophet generated patch
  switch (__choose("__ID73")) {
  case 0: {
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#ifdef COMPILE_14121
  case 1: {
    (*(op1)).type = 6;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14122
  case 2: {
    (*(op1)).value.lval++;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14123
  case 3: {
    (*(op1)).value.lval--;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14124
  case 4: {
    (*(op1)).value.str.len++;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14125
  case 5: {
    (*(op2)).type = 6;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14126
  case 6: {
    (*(op2)).value.lval++;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14127
  case 7: {
    (*(op2)).value.lval--;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14128
  case 8: {
    (*(op2)).value.str.len++;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14129
  case 9: {
    (*(result)).type = 6;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14130
  case 10: {
    (*(result)).value.lval++;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14131
  case 11: {
    (*(result)).value.lval--;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14132
  case 12: {
    (*(result)).value.str.len++;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14133
  case 13: {
    (*result).type = 6;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14134
  case 14: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14135
  case 15: {
    _convert_to_string(((op1)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14136
  case 16: {
    _convert_to_string(((op2)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14137
  case 17: {
    _convert_to_string(((result)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14138
  case 18: {
    _efree(((*(op1)).value.str.val));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14139
  case 19: {
    _efree(((*(op2)).value.str.val));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14140
  case 20: {
    _efree(((*(result)).value.str.val));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14141
  case 21: {
    _efree(((*result).value.str.val));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14142
  case 22: {
    _efree(((op1)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14143
  case 23: {
    _efree(((op2)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14144
  case 24: {
    _efree(((result)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14145
  case 25: {
    _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14146
  case 26: {
    _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14147
  case 27: {
    _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14148
  case 28: {
    _zend_list_delete((*(op1)).value.lval);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14149
  case 29: {
    _zend_list_delete((*(op2)).value.lval);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14150
  case 30: {
    _zend_list_delete((*(result)).value.lval);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14151
  case 31: {
    _zval_copy_ctor((&(op1_copy)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14152
  case 32: {
    _zval_copy_ctor((&(op2_copy)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14153
  case 33: {
    _zval_copy_ctor(((op1)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14154
  case 34: {
    _zval_copy_ctor(((op2)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14155
  case 35: {
    _zval_copy_ctor(((result)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14156
  case 36: {
    _zval_copy_ctor_func((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14157
  case 37: {
    _zval_copy_ctor_func((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14158
  case 38: {
    _zval_copy_ctor_func((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14159
  case 39: {
    _zval_dtor(((op1)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14160
  case 40: {
    _zval_dtor(((op2)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14161
  case 41: {
    _zval_dtor(((result)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14162
  case 42: {
    _zval_dtor_func((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14163
  case 43: {
    _zval_dtor_func((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14164
  case 44: {
    _zval_dtor_func((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14165
  case 45: {
    _zval_ptr_dtor((&(op1)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14166
  case 46: {
    _zval_ptr_dtor((&(op2)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14167
  case 47: {
    _zval_ptr_dtor((&(result)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14168
  case 48: {
    convert_scalar_to_array((op1), 4);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14169
  case 49: {
    convert_scalar_to_array((op2), 4);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14170
  case 50: {
    convert_scalar_to_array((result), 4);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14171
  case 51: {
    convert_scalar_to_number((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14172
  case 52: {
    convert_scalar_to_number((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14173
  case 53: {
    convert_scalar_to_number((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14174
  case 54: {
    convert_to_array((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14175
  case 55: {
    convert_to_array((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14176
  case 56: {
    convert_to_array((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14177
  case 57: {
    convert_to_boolean((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14178
  case 58: {
    convert_to_boolean((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14179
  case 59: {
    convert_to_boolean((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14180
  case 60: {
    convert_to_double((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14181
  case 61: {
    convert_to_double((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14182
  case 62: {
    convert_to_double((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14183
  case 63: {
    convert_to_long((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14184
  case 64: {
    convert_to_long((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14185
  case 65: {
    convert_to_long((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14186
  case 66: {
    convert_to_long(op1);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14187
  case 67: {
    convert_to_long(op2);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14188
  case 68: {
    convert_to_long_base(&(op1_copy), 10);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14189
  case 69: {
    convert_to_long_base(&(op2_copy), 10);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14190
  case 70: {
    convert_to_long_base((op1), 10);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14191
  case 71: {
    convert_to_long_base((op2), 10);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14192
  case 72: {
    convert_to_long_base((result), 10);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14193
  case 73: {
    exit(1);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14194
  case 74: {
    gc_remove_zval_from_buffer((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14195
  case 75: {
    gc_remove_zval_from_buffer((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14196
  case 76: {
    gc_remove_zval_from_buffer((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14197
  case 77: {
    gc_zval_check_possible_root(((op1)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14198
  case 78: {
    gc_zval_check_possible_root(((op2)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14199
  case 79: {
    gc_zval_check_possible_root(((result)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14200
  case 80: {
    gc_zval_possible_root((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14201
  case 81: {
    gc_zval_possible_root((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14202
  case 82: {
    gc_zval_possible_root((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14203
  case 83: {
    i_zval_ptr_dtor((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14204
  case 84: {
    i_zval_ptr_dtor((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14205
  case 85: {
    i_zval_ptr_dtor((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14206
  case 86: {
    if (!__is_neg("73-86", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                  "result", "op1", "op1_lval", "op2") &&
        ((*op1).type != 1)) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14207
  case 87: {
    if (!__is_neg("73-87", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                  "result", "op1", "op1_lval", "op2"))
      if ((*op1).type != 1) {
        switch ((*op1).type) {
        case 0:
          (op1_copy).value.lval = 0;
          break;
        case 2:
          (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
          break;
        case 6:
          (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
          break;
        case 4:
          (op1_copy).value.lval =
              (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
          break;
        case 5:
          (op1_copy) = (*(op1));
          _zval_copy_ctor((&(op1_copy)));
          convert_to_long_base(&(op1_copy), 10);
          break;
        case 3:
        case 7:
          (op1_copy).value.lval = (*op1).value.lval;
          break;
        default:
          zend_error((1 << 1L), "Cannot convert to ordinal value");
          (op1_copy).value.lval = 0;
          break;
        }
        (op1_copy).type = 1;
        (op1) = &(op1_copy);
      }

    break;
  }
#endif
#ifdef COMPILE_14208
  case 88: {
    if (((*op1).type != 1) &&
        !__is_neg("73-88", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                  "result", "op1", "op1_lval", "op2")) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14209
  case 89: {
    if (((*op1).type != 1) ||
        __is_neg("73-89", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2")) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14210
  case 90: {
    if ((*result).value.str.val &&
        !((((*result).value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          (((*result).value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      _efree(((*result).value.str.val));
    }
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14211
  case 91: {
    if (__is_neg("73-91", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 0;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14212
  case 92: {
    if (__is_neg("73-92", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 10;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14213
  case 93: {
    if (__is_neg("73-93", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 1;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14214
  case 94: {
    if (__is_neg("73-94", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 2;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14215
  case 95: {
    if (__is_neg("73-95", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 3;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14216
  case 96: {
    if (__is_neg("73-96", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 4;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14217
  case 97: {
    if (__is_neg("73-97", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 5;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14218
  case 98: {
    if (__is_neg("73-98", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 6;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14219
  case 99: {
    if (__is_neg("73-99", 0, 0, 0, 0, 3, (void **){result, op1, op2}, 0, 0, 4,
                 "result", "op1", "op1_lval", "op2"))
      return 7;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14220
  case 100: {
    memset(&(*op1), 0, sizeof(*(&(*op1))));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14221
  case 101: {
    memset(&(*op1).value, 0, sizeof(*(&(*op1).value)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14222
  case 102: {
    memset(&(*op1).value.str, 0, sizeof(*(&(*op1).value.str)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14223
  case 103: {
    memset(&(op1_copy), 0, sizeof(*(&(op1_copy))));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14224
  case 104: {
    memset(&(op1_copy).value, 0, sizeof(*(&(op1_copy).value)));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14225
  case 105: {
    op1_lval = (*(op1)).value.lval;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14226
  case 106: {
    op1_lval = (*(op2)).value.lval;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14227
  case 107: {
    op1_lval = (*(result)).value.lval;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14228
  case 108: {
    op1_lval = (*op1).value.lval;
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14229
  case 109: {
    zend_error((1 << 0L), "String size overflow");
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14230
  case 110: {
    zend_error((1 << 0L), "Unsupported operand types");
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14231
  case 111: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14232
  case 112: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14233
  case 113: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(3));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14234
  case 114: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(4));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14235
  case 115: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(6));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14236
  case 116: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14237
  case 117: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14238
  case 118: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(3));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14239
  case 119: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(4));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14240
  case 120: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(6));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14241
  case 121: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14242
  case 122: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14243
  case 123: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(3));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14244
  case 124: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(4));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14245
  case 125: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(6));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14246
  case 126: {
    zend_error((1 << 1L), "Division by zero");
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14247
  case 127: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14248
  case 128: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(op1)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14249
  case 129: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(op2)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14250
  case 130: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(result)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14251
  case 131: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(op1)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14252
  case 132: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(op2)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14253
  case 133: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(result)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14254
  case 134: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(op1)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14255
  case 135: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(op2)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14256
  case 136: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(result)))->name);
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14257
  case 137: {
    zend_free_obj_get_result((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14258
  case 138: {
    zend_free_obj_get_result((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14259
  case 139: {
    zend_free_obj_get_result((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14260
  case 140: {
    zval_unset_isref_p((op1));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14261
  case 141: {
    zval_unset_isref_p((op2));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14262
  case 142: {
    zval_unset_isref_p((result));
    if ((*op1).type != 1) {
      switch ((*op1).type) {
      case 0:
        (op1_copy).value.lval = 0;
        break;
      case 2:
        (op1_copy).value.lval = zend_dval_to_lval((*op1).value.dval);
        break;
      case 6:
        (op1_copy).value.lval = strtol((*op1).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op1_copy).value.lval =
            (zend_hash_num_elements((*op1).value.ht) ? 1 : 0);
        break;
      case 5:
        (op1_copy) = (*(op1));
        _zval_copy_ctor((&(op1_copy)));
        convert_to_long_base(&(op1_copy), 10);
        break;
      case 3:
      case 7:
        (op1_copy).value.lval = (*op1).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op1_copy).value.lval = 0;
        break;
      }
      (op1_copy).type = 1;
      (op1) = &(op1_copy);
    }

    break;
  }
#endif
  }
  op1_lval = Z_LVAL_P(op1);
  zendi_convert_to_long(op2, op2_copy, result);

  ZVAL_LONG(result, op1_lval | Z_LVAL_P(op2));
  return SUCCESS;
}
/* }}} */

ZEND_API int bitwise_and_function(zval *result, zval *op1,
                                  zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  long op1_lval;

  if (Z_TYPE_P(op1) == IS_STRING && Z_TYPE_P(op2) == IS_STRING) {
    zval *longer, *shorter;
    char *result_str;
    int i, result_len;

    if (Z_STRLEN_P(op1) >= Z_STRLEN_P(op2)) {
      longer = op1;
      shorter = op2;
    } else {
      longer = op2;
      shorter = op1;
    }

    Z_TYPE_P(result) = IS_STRING;
    result_len = Z_STRLEN_P(shorter);
    result_str = estrndup(Z_STRVAL_P(shorter), Z_STRLEN_P(shorter));
    for (i = 0; i < Z_STRLEN_P(shorter); i++) {
      result_str[i] &= Z_STRVAL_P(longer)[i];
    }
    if (result == op1) {
      STR_FREE(Z_STRVAL_P(result));
    }
    Z_STRVAL_P(result) = result_str;
    Z_STRLEN_P(result) = result_len;
    return SUCCESS;
  }

  zendi_convert_to_long(op1, op1_copy, result);
  // prophet generated patch
  switch (__choose("__ID74")) {
  case 0: {
    op1_lval = (*op1).value.lval;

    break;
  }
#ifdef COMPILE_14263
  case 1: {
    (*(op1)).type = 6;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14264
  case 2: {
    (*(op1)).value.lval++;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14265
  case 3: {
    (*(op1)).value.lval--;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14266
  case 4: {
    (*(op1)).value.str.len++;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14267
  case 5: {
    (*(op2)).type = 6;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14268
  case 6: {
    (*(op2)).value.lval++;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14269
  case 7: {
    (*(op2)).value.lval--;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14270
  case 8: {
    (*(op2)).value.str.len++;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14271
  case 9: {
    (*(result)).type = 6;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14272
  case 10: {
    (*(result)).value.lval++;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14273
  case 11: {
    (*(result)).value.lval--;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14274
  case 12: {
    (*(result)).value.str.len++;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14275
  case 13: {
    (*result).type = 6;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14276
  case 14: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14277
  case 15: {
    _convert_to_string(((op1)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14278
  case 16: {
    _convert_to_string(((op2)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14279
  case 17: {
    _convert_to_string(((result)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14280
  case 18: {
    _efree(((*(op1)).value.str.val));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14281
  case 19: {
    _efree(((*(op2)).value.str.val));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14282
  case 20: {
    _efree(((*(result)).value.str.val));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14283
  case 21: {
    _efree(((*result).value.str.val));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14284
  case 22: {
    _efree(((op1)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14285
  case 23: {
    _efree(((op2)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14286
  case 24: {
    _efree(((result)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14287
  case 25: {
    _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14288
  case 26: {
    _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14289
  case 27: {
    _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14290
  case 28: {
    _zend_list_delete((*(op1)).value.lval);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14291
  case 29: {
    _zend_list_delete((*(op2)).value.lval);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14292
  case 30: {
    _zend_list_delete((*(result)).value.lval);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14293
  case 31: {
    _zval_copy_ctor((&(op1_copy)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14294
  case 32: {
    _zval_copy_ctor((&(op2_copy)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14295
  case 33: {
    _zval_copy_ctor(((op1)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14296
  case 34: {
    _zval_copy_ctor(((op2)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14297
  case 35: {
    _zval_copy_ctor(((result)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14298
  case 36: {
    _zval_copy_ctor_func((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14299
  case 37: {
    _zval_copy_ctor_func((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14300
  case 38: {
    _zval_copy_ctor_func((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14301
  case 39: {
    _zval_dtor(((op1)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14302
  case 40: {
    _zval_dtor(((op2)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14303
  case 41: {
    _zval_dtor(((result)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14304
  case 42: {
    _zval_dtor_func((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14305
  case 43: {
    _zval_dtor_func((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14306
  case 44: {
    _zval_dtor_func((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14307
  case 45: {
    _zval_ptr_dtor((&(op1)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14308
  case 46: {
    _zval_ptr_dtor((&(op2)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14309
  case 47: {
    _zval_ptr_dtor((&(result)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14310
  case 48: {
    convert_scalar_to_array((op1), 4);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14311
  case 49: {
    convert_scalar_to_array((op2), 4);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14312
  case 50: {
    convert_scalar_to_array((result), 4);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14313
  case 51: {
    convert_scalar_to_number((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14314
  case 52: {
    convert_scalar_to_number((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14315
  case 53: {
    convert_scalar_to_number((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14316
  case 54: {
    convert_to_array((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14317
  case 55: {
    convert_to_array((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14318
  case 56: {
    convert_to_array((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14319
  case 57: {
    convert_to_boolean((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14320
  case 58: {
    convert_to_boolean((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14321
  case 59: {
    convert_to_boolean((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14322
  case 60: {
    convert_to_double((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14323
  case 61: {
    convert_to_double((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14324
  case 62: {
    convert_to_double((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14325
  case 63: {
    convert_to_long((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14326
  case 64: {
    convert_to_long((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14327
  case 65: {
    convert_to_long((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14328
  case 66: {
    convert_to_long(op1);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14329
  case 67: {
    convert_to_long(op2);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14330
  case 68: {
    convert_to_long_base(&(op1_copy), 10);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14331
  case 69: {
    convert_to_long_base(&(op2_copy), 10);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14332
  case 70: {
    convert_to_long_base((op1), 10);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14333
  case 71: {
    convert_to_long_base((op2), 10);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14334
  case 72: {
    convert_to_long_base((result), 10);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14335
  case 73: {
    exit(1);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14336
  case 74: {
    gc_remove_zval_from_buffer((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14337
  case 75: {
    gc_remove_zval_from_buffer((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14338
  case 76: {
    gc_remove_zval_from_buffer((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14339
  case 77: {
    gc_zval_check_possible_root(((op1)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14340
  case 78: {
    gc_zval_check_possible_root(((op2)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14341
  case 79: {
    gc_zval_check_possible_root(((result)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14342
  case 80: {
    gc_zval_possible_root((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14343
  case 81: {
    gc_zval_possible_root((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14344
  case 82: {
    gc_zval_possible_root((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14345
  case 83: {
    i_zval_ptr_dtor((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14346
  case 84: {
    i_zval_ptr_dtor((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14347
  case 85: {
    i_zval_ptr_dtor((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14348
  case 86: {
    if (!__is_neg("74-86", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                  "op1", "op1_lval", "result", "op2"))
      op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14349
  case 87: {
    if ((*result).value.str.val &&
        !((((*result).value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          (((*result).value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      _efree(((*result).value.str.val));
    }
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14350
  case 88: {
    if (__is_neg("74-88", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 0;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14351
  case 89: {
    if (__is_neg("74-89", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 10;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14352
  case 90: {
    if (__is_neg("74-90", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 1;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14353
  case 91: {
    if (__is_neg("74-91", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 2;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14354
  case 92: {
    if (__is_neg("74-92", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 3;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14355
  case 93: {
    if (__is_neg("74-93", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 4;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14356
  case 94: {
    if (__is_neg("74-94", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 5;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14357
  case 95: {
    if (__is_neg("74-95", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 6;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14358
  case 96: {
    if (__is_neg("74-96", 0, 0, 0, 0, 3, (void **){op1, result, op2}, 0, 0, 4,
                 "op1", "op1_lval", "result", "op2"))
      return 7;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14359
  case 97: {
    memset(&(*op1), 0, sizeof(*(&(*op1))));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14360
  case 98: {
    memset(&(*op1).value, 0, sizeof(*(&(*op1).value)));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14361
  case 99: {
    op1_lval = (*(op1)).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14362
  case 100: {
    op1_lval = (*(op1)).value.lval;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14363
  case 101: {
    op1_lval = (*(op2)).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14364
  case 102: {
    op1_lval = (*(op2)).value.lval;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14365
  case 103: {
    op1_lval = (*(result)).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14366
  case 104: {
    op1_lval = (*(result)).value.lval;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14367
  case 105: {
    op1_lval = (*op1).value.lval;
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14368
  case 106: {
    zend_error((1 << 0L), "String size overflow");
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14369
  case 107: {
    zend_error((1 << 0L), "Unsupported operand types");
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14370
  case 108: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14371
  case 109: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14372
  case 110: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(3));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14373
  case 111: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(4));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14374
  case 112: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(6));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14375
  case 113: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14376
  case 114: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14377
  case 115: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(3));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14378
  case 116: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(4));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14379
  case 117: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(6));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14380
  case 118: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14381
  case 119: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14382
  case 120: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(3));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14383
  case 121: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(4));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14384
  case 122: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(6));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14385
  case 123: {
    zend_error((1 << 1L), "Division by zero");
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14386
  case 124: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14387
  case 125: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(op1)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14388
  case 126: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(op2)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14389
  case 127: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(result)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14390
  case 128: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(op1)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14391
  case 129: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(op2)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14392
  case 130: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(result)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14393
  case 131: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(op1)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14394
  case 132: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(op2)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14395
  case 133: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(result)))->name);
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14396
  case 134: {
    zend_free_obj_get_result((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14397
  case 135: {
    zend_free_obj_get_result((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14398
  case 136: {
    zend_free_obj_get_result((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14399
  case 137: {
    zval_unset_isref_p((op1));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14400
  case 138: {
    zval_unset_isref_p((op2));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
#ifdef COMPILE_14401
  case 139: {
    zval_unset_isref_p((result));
    op1_lval = (*op1).value.lval;

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID75")) {
  case 0: {
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#ifdef COMPILE_14402
  case 1: {
    (*(op1)).type = 6;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14403
  case 2: {
    (*(op1)).value.lval++;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14404
  case 3: {
    (*(op1)).value.lval--;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14405
  case 4: {
    (*(op1)).value.str.len++;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14406
  case 5: {
    (*(op2)).type = 6;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14407
  case 6: {
    (*(op2)).value.lval++;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14408
  case 7: {
    (*(op2)).value.lval--;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14409
  case 8: {
    (*(op2)).value.str.len++;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14410
  case 9: {
    (*(result)).type = 6;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14411
  case 10: {
    (*(result)).value.lval++;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14412
  case 11: {
    (*(result)).value.lval--;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14413
  case 12: {
    (*(result)).value.str.len++;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14414
  case 13: {
    (*result).type = 6;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14415
  case 14: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14416
  case 15: {
    _convert_to_string(((op1)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14417
  case 16: {
    _convert_to_string(((op2)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14418
  case 17: {
    _convert_to_string(((result)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14419
  case 18: {
    _efree(((*(op1)).value.str.val));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14420
  case 19: {
    _efree(((*(op2)).value.str.val));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14421
  case 20: {
    _efree(((*(result)).value.str.val));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14422
  case 21: {
    _efree(((*result).value.str.val));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14423
  case 22: {
    _efree(((op1)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14424
  case 23: {
    _efree(((op2)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14425
  case 24: {
    _efree(((result)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14426
  case 25: {
    _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14427
  case 26: {
    _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14428
  case 27: {
    _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14429
  case 28: {
    _zend_list_delete((*(op1)).value.lval);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14430
  case 29: {
    _zend_list_delete((*(op2)).value.lval);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14431
  case 30: {
    _zend_list_delete((*(result)).value.lval);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14432
  case 31: {
    _zval_copy_ctor((&(op1_copy)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14433
  case 32: {
    _zval_copy_ctor((&(op2_copy)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14434
  case 33: {
    _zval_copy_ctor(((op1)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14435
  case 34: {
    _zval_copy_ctor(((op2)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14436
  case 35: {
    _zval_copy_ctor(((result)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14437
  case 36: {
    _zval_copy_ctor_func((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14438
  case 37: {
    _zval_copy_ctor_func((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14439
  case 38: {
    _zval_copy_ctor_func((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14440
  case 39: {
    _zval_dtor(((op1)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14441
  case 40: {
    _zval_dtor(((op2)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14442
  case 41: {
    _zval_dtor(((result)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14443
  case 42: {
    _zval_dtor_func((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14444
  case 43: {
    _zval_dtor_func((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14445
  case 44: {
    _zval_dtor_func((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14446
  case 45: {
    _zval_ptr_dtor((&(op1)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14447
  case 46: {
    _zval_ptr_dtor((&(op2)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14448
  case 47: {
    _zval_ptr_dtor((&(result)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14449
  case 48: {
    convert_scalar_to_array((op1), 4);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14450
  case 49: {
    convert_scalar_to_array((op2), 4);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14451
  case 50: {
    convert_scalar_to_array((result), 4);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14452
  case 51: {
    convert_scalar_to_number((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14453
  case 52: {
    convert_scalar_to_number((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14454
  case 53: {
    convert_scalar_to_number((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14455
  case 54: {
    convert_to_array((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14456
  case 55: {
    convert_to_array((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14457
  case 56: {
    convert_to_array((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14458
  case 57: {
    convert_to_boolean((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14459
  case 58: {
    convert_to_boolean((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14460
  case 59: {
    convert_to_boolean((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14461
  case 60: {
    convert_to_double((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14462
  case 61: {
    convert_to_double((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14463
  case 62: {
    convert_to_double((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14464
  case 63: {
    convert_to_long((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14465
  case 64: {
    convert_to_long((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14466
  case 65: {
    convert_to_long((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14467
  case 66: {
    convert_to_long(op1);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14468
  case 67: {
    convert_to_long(op2);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14469
  case 68: {
    convert_to_long_base(&(op1_copy), 10);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14470
  case 69: {
    convert_to_long_base(&(op2_copy), 10);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14471
  case 70: {
    convert_to_long_base((op1), 10);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14472
  case 71: {
    convert_to_long_base((op2), 10);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14473
  case 72: {
    convert_to_long_base((result), 10);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14474
  case 73: {
    exit(1);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14475
  case 74: {
    gc_remove_zval_from_buffer((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14476
  case 75: {
    gc_remove_zval_from_buffer((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14477
  case 76: {
    gc_remove_zval_from_buffer((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14478
  case 77: {
    gc_zval_check_possible_root(((op1)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14479
  case 78: {
    gc_zval_check_possible_root(((op2)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14480
  case 79: {
    gc_zval_check_possible_root(((result)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14481
  case 80: {
    gc_zval_possible_root((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14482
  case 81: {
    gc_zval_possible_root((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14483
  case 82: {
    gc_zval_possible_root((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14484
  case 83: {
    i_zval_ptr_dtor((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14485
  case 84: {
    i_zval_ptr_dtor((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14486
  case 85: {
    i_zval_ptr_dtor((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14487
  case 86: {
    if (!__is_neg("75-86", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                  "result", "op2", "op1", "op1_lval") &&
        ((*op2).type != 1)) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14488
  case 87: {
    if (!__is_neg("75-87", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                  "result", "op2", "op1", "op1_lval"))
      if ((*op2).type != 1) {
        switch ((*op2).type) {
        case 0:
          (op2_copy).value.lval = 0;
          break;
        case 2:
          (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
          break;
        case 6:
          (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
          break;
        case 4:
          (op2_copy).value.lval =
              (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
          break;
        case 5:
          (op2_copy) = (*(op2));
          _zval_copy_ctor((&(op2_copy)));
          convert_to_long_base(&(op2_copy), 10);
          break;
        case 3:
        case 7:
          (op2_copy).value.lval = (*op2).value.lval;
          break;
        default:
          zend_error((1 << 1L), "Cannot convert to ordinal value");
          (op2_copy).value.lval = 0;
          break;
        }
        (op2_copy).type = 1;
        (op2) = &(op2_copy);
      }

    break;
  }
#endif
#ifdef COMPILE_14489
  case 88: {
    if (((*op2).type != 1) &&
        !__is_neg("75-88", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                  "result", "op2", "op1", "op1_lval")) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14490
  case 89: {
    if (((*op2).type != 1) ||
        __is_neg("75-89", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval")) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14491
  case 90: {
    if ((*result).value.str.val &&
        !((((*result).value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          (((*result).value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      _efree(((*result).value.str.val));
    }
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14492
  case 91: {
    if (__is_neg("75-91", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 0;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14493
  case 92: {
    if (__is_neg("75-92", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 10;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14494
  case 93: {
    if (__is_neg("75-93", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 1;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14495
  case 94: {
    if (__is_neg("75-94", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 2;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14496
  case 95: {
    if (__is_neg("75-95", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 3;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14497
  case 96: {
    if (__is_neg("75-96", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 4;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14498
  case 97: {
    if (__is_neg("75-97", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 5;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14499
  case 98: {
    if (__is_neg("75-98", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 6;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14500
  case 99: {
    if (__is_neg("75-99", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1", "op1_lval"))
      return 7;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14501
  case 100: {
    memset(&(*op2), 0, sizeof(*(&(*op2))));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14502
  case 101: {
    memset(&(*op2).value, 0, sizeof(*(&(*op2).value)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14503
  case 102: {
    memset(&(*op2).value.str, 0, sizeof(*(&(*op2).value.str)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14504
  case 103: {
    memset(&(op2_copy), 0, sizeof(*(&(op2_copy))));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14505
  case 104: {
    memset(&(op2_copy).value, 0, sizeof(*(&(op2_copy).value)));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14506
  case 105: {
    op1_lval = (*(op1)).value.lval;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14507
  case 106: {
    op1_lval = (*(op2)).value.lval;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14508
  case 107: {
    op1_lval = (*(result)).value.lval;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14509
  case 108: {
    op1_lval = (*op1).value.lval;
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14510
  case 109: {
    zend_error((1 << 0L), "String size overflow");
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14511
  case 110: {
    zend_error((1 << 0L), "Unsupported operand types");
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14512
  case 111: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14513
  case 112: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14514
  case 113: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(3));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14515
  case 114: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(4));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14516
  case 115: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(6));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14517
  case 116: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14518
  case 117: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14519
  case 118: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(3));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14520
  case 119: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(4));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14521
  case 120: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(6));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14522
  case 121: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14523
  case 122: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14524
  case 123: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(3));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14525
  case 124: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(4));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14526
  case 125: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(6));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14527
  case 126: {
    zend_error((1 << 1L), "Division by zero");
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14528
  case 127: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14529
  case 128: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(op1)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14530
  case 129: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(op2)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14531
  case 130: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(result)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14532
  case 131: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(op1)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14533
  case 132: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(op2)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14534
  case 133: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(result)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14535
  case 134: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(op1)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14536
  case 135: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(op2)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14537
  case 136: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(result)))->name);
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14538
  case 137: {
    zend_free_obj_get_result((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14539
  case 138: {
    zend_free_obj_get_result((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14540
  case 139: {
    zend_free_obj_get_result((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14541
  case 140: {
    zval_unset_isref_p((op1));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14542
  case 141: {
    zval_unset_isref_p((op2));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
#ifdef COMPILE_14543
  case 142: {
    zval_unset_isref_p((result));
    if ((*op2).type != 1) {
      switch ((*op2).type) {
      case 0:
        (op2_copy).value.lval = 0;
        break;
      case 2:
        (op2_copy).value.lval = zend_dval_to_lval((*op2).value.dval);
        break;
      case 6:
        (op2_copy).value.lval = strtol((*op2).value.str.val, ((void *)0), 10);
        break;
      case 4:
        (op2_copy).value.lval =
            (zend_hash_num_elements((*op2).value.ht) ? 1 : 0);
        break;
      case 5:
        (op2_copy) = (*(op2));
        _zval_copy_ctor((&(op2_copy)));
        convert_to_long_base(&(op2_copy), 10);
        break;
      case 3:
      case 7:
        (op2_copy).value.lval = (*op2).value.lval;
        break;
      default:
        zend_error((1 << 1L), "Cannot convert to ordinal value");
        (op2_copy).value.lval = 0;
        break;
      }
      (op2_copy).type = 1;
      (op2) = &(op2_copy);
    }

    break;
  }
#endif
  }

  ZVAL_LONG(result, op1_lval & Z_LVAL_P(op2));
  // prophet generated patch
  switch (__choose("__ID76")) {
  case 0: {
    return 0;

    break;
  }
#ifdef COMPILE_14544
  case 1: {
    (*(op1)).type = 6;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14545
  case 2: {
    (*(op1)).value.lval++;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14546
  case 3: {
    (*(op1)).value.lval--;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14547
  case 4: {
    (*(op1)).value.str.len++;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14548
  case 5: {
    (*(op2)).type = 6;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14549
  case 6: {
    (*(op2)).value.lval++;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14550
  case 7: {
    (*(op2)).value.lval--;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14551
  case 8: {
    (*(op2)).value.str.len++;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14552
  case 9: {
    (*(result)).type = 6;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14553
  case 10: {
    (*(result)).value.lval++;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14554
  case 11: {
    (*(result)).value.lval--;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14555
  case 12: {
    (*(result)).value.str.len++;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14556
  case 13: {
    (*result).type = 6;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14557
  case 14: {
    (executor_globals.argument_stack) =
        zend_vm_stack_new_page(((16 * 1024) - 16));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14558
  case 15: {
    _convert_to_string(((op1)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14559
  case 16: {
    _convert_to_string(((op2)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14560
  case 17: {
    _convert_to_string(((result)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14561
  case 18: {
    _efree(((*(op1)).value.str.val));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14562
  case 19: {
    _efree(((*(op2)).value.str.val));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14563
  case 20: {
    _efree(((*(result)).value.str.val));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14564
  case 21: {
    _efree(((*result).value.str.val));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14565
  case 22: {
    _efree(((op1)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14566
  case 23: {
    _efree(((op2)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14567
  case 24: {
    _efree(((result)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14568
  case 25: {
    _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14569
  case 26: {
    _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14570
  case 27: {
    _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                    ((void (*)(void *))_zval_ptr_dtor), (0));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14571
  case 28: {
    _zend_list_delete((*(op1)).value.lval);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14572
  case 29: {
    _zend_list_delete((*(op2)).value.lval);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14573
  case 30: {
    _zend_list_delete((*(result)).value.lval);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14574
  case 31: {
    _zval_copy_ctor((&(op1_copy)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14575
  case 32: {
    _zval_copy_ctor((&(op2_copy)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14576
  case 33: {
    _zval_copy_ctor(((op1)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14577
  case 34: {
    _zval_copy_ctor(((op2)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14578
  case 35: {
    _zval_copy_ctor(((result)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14579
  case 36: {
    _zval_copy_ctor_func((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14580
  case 37: {
    _zval_copy_ctor_func((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14581
  case 38: {
    _zval_copy_ctor_func((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14582
  case 39: {
    _zval_dtor(((op1)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14583
  case 40: {
    _zval_dtor(((op2)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14584
  case 41: {
    _zval_dtor(((result)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14585
  case 42: {
    _zval_dtor_func((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14586
  case 43: {
    _zval_dtor_func((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14587
  case 44: {
    _zval_dtor_func((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14588
  case 45: {
    _zval_ptr_dtor((&(op1)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14589
  case 46: {
    _zval_ptr_dtor((&(op2)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14590
  case 47: {
    _zval_ptr_dtor((&(result)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14591
  case 48: {
    convert_scalar_to_array((op1), 4);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14592
  case 49: {
    convert_scalar_to_array((op2), 4);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14593
  case 50: {
    convert_scalar_to_array((result), 4);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14594
  case 51: {
    convert_scalar_to_number((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14595
  case 52: {
    convert_scalar_to_number((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14596
  case 53: {
    convert_scalar_to_number((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14597
  case 54: {
    convert_to_array((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14598
  case 55: {
    convert_to_array((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14599
  case 56: {
    convert_to_array((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14600
  case 57: {
    convert_to_boolean((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14601
  case 58: {
    convert_to_boolean((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14602
  case 59: {
    convert_to_boolean((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14603
  case 60: {
    convert_to_double((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14604
  case 61: {
    convert_to_double((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14605
  case 62: {
    convert_to_double((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14606
  case 63: {
    convert_to_long((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14607
  case 64: {
    convert_to_long((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14608
  case 65: {
    convert_to_long((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14609
  case 66: {
    convert_to_long(op1);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14610
  case 67: {
    convert_to_long(op2);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14611
  case 68: {
    convert_to_long_base(&(op1_copy), 10);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14612
  case 69: {
    convert_to_long_base(&(op2_copy), 10);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14613
  case 70: {
    convert_to_long_base((op1), 10);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14614
  case 71: {
    convert_to_long_base((op2), 10);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14615
  case 72: {
    convert_to_long_base((result), 10);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14616
  case 73: {
    exit(1);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14617
  case 74: {
    gc_remove_zval_from_buffer((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14618
  case 75: {
    gc_remove_zval_from_buffer((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14619
  case 76: {
    gc_remove_zval_from_buffer((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14620
  case 77: {
    gc_zval_check_possible_root(((op1)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14621
  case 78: {
    gc_zval_check_possible_root(((op2)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14622
  case 79: {
    gc_zval_check_possible_root(((result)));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14623
  case 80: {
    gc_zval_possible_root((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14624
  case 81: {
    gc_zval_possible_root((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14625
  case 82: {
    gc_zval_possible_root((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14626
  case 83: {
    i_zval_ptr_dtor((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14627
  case 84: {
    i_zval_ptr_dtor((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14628
  case 85: {
    i_zval_ptr_dtor((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14629
  case 86: {
    if (!__is_neg("76-86", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                  "result", "op2", "op1_lval", "op1"))
      return 0;

    break;
  }
#endif
#ifdef COMPILE_14630
  case 87: {
    if ((*result).value.str.val &&
        !((((*result).value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          (((*result).value.str.val) <
           (compiler_globals.interned_strings_end)))) {
      _efree(((*result).value.str.val));
    }
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14631
  case 88: {
    if (__is_neg("76-88", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 0;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14632
  case 89: {
    if (__is_neg("76-89", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 10;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14633
  case 90: {
    if (__is_neg("76-90", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 1;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14634
  case 91: {
    if (__is_neg("76-91", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 2;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14635
  case 92: {
    if (__is_neg("76-92", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 3;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14636
  case 93: {
    if (__is_neg("76-93", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 4;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14637
  case 94: {
    if (__is_neg("76-94", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 5;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14638
  case 95: {
    if (__is_neg("76-95", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 6;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14639
  case 96: {
    if (__is_neg("76-96", 0, 0, 0, 0, 3, (void **){result, op2, op1}, 0, 0, 4,
                 "result", "op2", "op1_lval", "op1"))
      return 7;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14640
  case 97: {
    op1_lval = (*(op1)).value.lval;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14641
  case 98: {
    op1_lval = (*(op2)).value.lval;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14642
  case 99: {
    op1_lval = (*(result)).value.lval;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14643
  case 100: {
    op1_lval = (*op1).value.lval;
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14644
  case 101: {
    zend_error((1 << 0L), "String size overflow");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14645
  case 102: {
    zend_error((1 << 0L), "Unsupported operand types");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14646
  case 103: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14647
  case 104: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14648
  case 105: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(3));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14649
  case 106: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(4));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14650
  case 107: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op1)))->name,
               zend_get_type_by_const(6));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14651
  case 108: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14652
  case 109: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14653
  case 110: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(3));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14654
  case 111: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(4));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14655
  case 112: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(op2)))->name,
               zend_get_type_by_const(6));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14656
  case 113: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14657
  case 114: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14658
  case 115: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(3));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14659
  case 116: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(4));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14660
  case 117: {
    zend_error((1 << 12L), "Object of class %s could not be converted to %s",
               zend_get_class_entry(&(*(result)))->name,
               zend_get_type_by_const(6));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14661
  case 118: {
    zend_error((1 << 1L), "Division by zero");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14662
  case 119: {
    zend_error((1 << 3L), "A non well formed numeric value encountered");
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14663
  case 120: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(op1)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14664
  case 121: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(op2)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14665
  case 122: {
    zend_error((1 << 3L), "Object of class %s could not be converted to double",
               zend_get_class_entry(&(*(result)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14666
  case 123: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(op1)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14667
  case 124: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(op2)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14668
  case 125: {
    zend_error((1 << 3L), "Object of class %s could not be converted to int",
               zend_get_class_entry(&(*(result)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14669
  case 126: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(op1)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14670
  case 127: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(op2)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14671
  case 128: {
    zend_error((1 << 3L), "Object of class %s to string conversion",
               zend_get_class_entry(&(*(result)))->name);
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14672
  case 129: {
    zend_free_obj_get_result((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14673
  case 130: {
    zend_free_obj_get_result((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14674
  case 131: {
    zend_free_obj_get_result((result));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14675
  case 132: {
    zval_unset_isref_p((op1));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14676
  case 133: {
    zval_unset_isref_p((op2));
    return 0;

    break;
  }
#endif
#ifdef COMPILE_14677
  case 134: {
    zval_unset_isref_p((result));
    return 0;

    break;
  }
#endif
  }
}
/* }}} */

ZEND_API int bitwise_xor_function(zval *result, zval *op1,
                                  zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  long op1_lval;

  if (Z_TYPE_P(op1) == IS_STRING && Z_TYPE_P(op2) == IS_STRING) {
    zval *longer, *shorter;
    char *result_str;
    int i, result_len;

    if (Z_STRLEN_P(op1) >= Z_STRLEN_P(op2)) {
      longer = op1;
      shorter = op2;
    } else {
      longer = op2;
      shorter = op1;
    }

    Z_TYPE_P(result) = IS_STRING;
    result_len = Z_STRLEN_P(shorter);
    result_str = estrndup(Z_STRVAL_P(shorter), Z_STRLEN_P(shorter));
    for (i = 0; i < Z_STRLEN_P(shorter); i++) {
      result_str[i] ^= Z_STRVAL_P(longer)[i];
    }
    if (result == op1) {
      STR_FREE(Z_STRVAL_P(result));
    }
    Z_STRVAL_P(result) = result_str;
    Z_STRLEN_P(result) = result_len;
    return SUCCESS;
  }

  zendi_convert_to_long(op1, op1_copy, result);
  op1_lval = Z_LVAL_P(op1);
  zendi_convert_to_long(op2, op2_copy, result);

  ZVAL_LONG(result, op1_lval ^ Z_LVAL_P(op2));
  return SUCCESS;
}
/* }}} */

ZEND_API int shift_left_function(zval *result, zval *op1,
                                 zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  long op1_lval;

  zendi_convert_to_long(op1, op1_copy, result);
  op1_lval = Z_LVAL_P(op1);
  zendi_convert_to_long(op2, op2_copy, result);
  ZVAL_LONG(result, op1_lval << Z_LVAL_P(op2));
  return SUCCESS;
}
/* }}} */

ZEND_API int shift_right_function(zval *result, zval *op1,
                                  zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  long op1_lval;

  zendi_convert_to_long(op1, op1_copy, result);
  op1_lval = Z_LVAL_P(op1);
  zendi_convert_to_long(op2, op2_copy, result);
  ZVAL_LONG(result, op1_lval >> Z_LVAL_P(op2));
  return SUCCESS;
}
/* }}} */

/* must support result==op1 */
ZEND_API int add_char_to_string(zval *result, const zval *op1,
                                const zval *op2) /* {{{ */
{
  int length = Z_STRLEN_P(op1) + 1;
  char *buf;

  if (IS_INTERNED(Z_STRVAL_P(op1))) {
    buf = (char *)emalloc(length + 1);
    memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
  } else {
    buf = (char *)erealloc(Z_STRVAL_P(op1), length + 1);
  }
  buf[length - 1] = (char)Z_LVAL_P(op2);
  buf[length] = 0;
  ZVAL_STRINGL(result, buf, length, 0);
  return SUCCESS;
}
/* }}} */

/* must support result==op1 */
ZEND_API int add_string_to_string(zval *result, const zval *op1,
                                  const zval *op2) /* {{{ */
{
  int length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
  char *buf;

  if (IS_INTERNED(Z_STRVAL_P(op1))) {
    buf = (char *)emalloc(length + 1);
    memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
  } else {
    buf = (char *)erealloc(Z_STRVAL_P(op1), length + 1);
  }
  memcpy(buf + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
  buf[length] = 0;
  ZVAL_STRINGL(result, buf, length, 0);
  return SUCCESS;
}
/* }}} */

ZEND_API int concat_function(zval *result, zval *op1,
                             zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  int use_copy1 = 0, use_copy2 = 0;

  if (Z_TYPE_P(op1) != IS_STRING) {
    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
  }
  if (Z_TYPE_P(op2) != IS_STRING) {
    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
  }

  if (use_copy1) {
    /* We have created a converted copy of op1. Therefore, op1 won't become the
     * result so we have to free it.
     */
    if (result == op1) {
      zval_dtor(op1);
    }
    op1 = &op1_copy;
  }
  if (use_copy2) {
    op2 = &op2_copy;
  }
  if (result == op1 &&
      !IS_INTERNED(
          Z_STRVAL_P(op1))) { /* special case, perform operations on result */
    uint res_len = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);

    if (Z_STRLEN_P(result) < 0 ||
        (int)(Z_STRLEN_P(op1) + Z_STRLEN_P(op2)) < 0) {
      efree(Z_STRVAL_P(result));
      ZVAL_EMPTY_STRING(result);
      zend_error(E_ERROR, "String size overflow");
    }

    Z_STRVAL_P(result) = erealloc(Z_STRVAL_P(result), res_len + 1);

    memcpy(Z_STRVAL_P(result) + Z_STRLEN_P(result), Z_STRVAL_P(op2),
           Z_STRLEN_P(op2));
    Z_STRVAL_P(result)[res_len] = 0;
    Z_STRLEN_P(result) = res_len;
  } else {
    int length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
    char *buf = (char *)emalloc(length + 1);

    memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
    memcpy(buf + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
    buf[length] = 0;
    ZVAL_STRINGL(result, buf, length, 0);
  }
  if (use_copy1) {
    zval_dtor(op1);
  }
  if (use_copy2) {
    zval_dtor(op2);
  }
  return SUCCESS;
}
/* }}} */

ZEND_API int string_compare_function(zval *result, zval *op1,
                                     zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  int use_copy1 = 0, use_copy2 = 0;

  if (Z_TYPE_P(op1) != IS_STRING) {
    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
  }
  if (Z_TYPE_P(op2) != IS_STRING) {
    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
  }

  if (use_copy1) {
    op1 = &op1_copy;
  }
  if (use_copy2) {
    op2 = &op2_copy;
  }

  ZVAL_LONG(result, zend_binary_zval_strcmp(op1, op2));

  if (use_copy1) {
    zval_dtor(op1);
  }
  if (use_copy2) {
    zval_dtor(op2);
  }
  return SUCCESS;
}
/* }}} */

#if HAVE_STRCOLL
ZEND_API int string_locale_compare_function(zval *result, zval *op1,
                                            zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;
  int use_copy1 = 0, use_copy2 = 0;

  if (Z_TYPE_P(op1) != IS_STRING) {
    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
  }
  if (Z_TYPE_P(op2) != IS_STRING) {
    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
  }

  if (use_copy1) {
    op1 = &op1_copy;
  }
  if (use_copy2) {
    op2 = &op2_copy;
  }

  ZVAL_LONG(result, strcoll(Z_STRVAL_P(op1), Z_STRVAL_P(op2)));

  if (use_copy1) {
    zval_dtor(op1);
  }
  if (use_copy2) {
    zval_dtor(op2);
  }
  return SUCCESS;
}
/* }}} */
#endif

ZEND_API int numeric_compare_function(zval *result, zval *op1,
                                      zval *op2 TSRMLS_DC) /* {{{ */
{
  zval op1_copy, op2_copy;

  op1_copy = *op1;
  zval_copy_ctor(&op1_copy);

  op2_copy = *op2;
  zval_copy_ctor(&op2_copy);

  convert_to_double(&op1_copy);
  convert_to_double(&op2_copy);

  ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_DVAL(op1_copy) - Z_DVAL(op2_copy)));

  return SUCCESS;
}
/* }}} */

static inline void zend_free_obj_get_result(zval *op TSRMLS_DC) /* {{{ */
{
  if (Z_REFCOUNT_P(op) == 0) {
    GC_REMOVE_ZVAL_FROM_BUFFER(op);
    zval_dtor(op);
    FREE_ZVAL(op);
  } else {
    zval_ptr_dtor(&op);
  }
}
/* }}} */

ZEND_API int compare_function(zval *result, zval *op1,
                              zval *op2 TSRMLS_DC) /* {{{ */
{
  int ret;
  int converted = 0;
  zval op1_copy, op2_copy;
  zval *op_free;

  while (1) {
    switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
    case TYPE_PAIR(IS_LONG, IS_LONG):
      ZVAL_LONG(result, Z_LVAL_P(op1) > Z_LVAL_P(op2)
                            ? 1
                            : (Z_LVAL_P(op1) < Z_LVAL_P(op2) ? -1 : 0));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_LONG):
      Z_DVAL_P(result) = Z_DVAL_P(op1) - (double)Z_LVAL_P(op2);
      ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_DVAL_P(result)));
      return SUCCESS;

    case TYPE_PAIR(IS_LONG, IS_DOUBLE):
      Z_DVAL_P(result) = (double)Z_LVAL_P(op1) - Z_DVAL_P(op2);
      ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_DVAL_P(result)));
      return SUCCESS;

    case TYPE_PAIR(IS_DOUBLE, IS_DOUBLE):
      if (Z_DVAL_P(op1) == Z_DVAL_P(op2)) {
        ZVAL_LONG(result, 0);
      } else {
        Z_DVAL_P(result) = Z_DVAL_P(op1) - Z_DVAL_P(op2);
        ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_DVAL_P(result)));
      }
      return SUCCESS;

    case TYPE_PAIR(IS_ARRAY, IS_ARRAY):
      zend_compare_arrays(result, op1, op2 TSRMLS_CC);
      return SUCCESS;

    case TYPE_PAIR(IS_NULL, IS_NULL):
      ZVAL_LONG(result, 0);
      return SUCCESS;

    case TYPE_PAIR(IS_NULL, IS_BOOL):
      ZVAL_LONG(result, Z_LVAL_P(op2) ? -1 : 0);
      return SUCCESS;

    case TYPE_PAIR(IS_BOOL, IS_NULL):
      ZVAL_LONG(result, Z_LVAL_P(op1) ? 1 : 0);
      return SUCCESS;

    case TYPE_PAIR(IS_BOOL, IS_BOOL):
      ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_LVAL_P(op1) - Z_LVAL_P(op2)));
      return SUCCESS;

    case TYPE_PAIR(IS_STRING, IS_STRING):
      zendi_smart_strcmp(result, op1, op2);
      return SUCCESS;

    case TYPE_PAIR(IS_NULL, IS_STRING):
      ZVAL_LONG(result,
                zend_binary_strcmp("", 0, Z_STRVAL_P(op2), Z_STRLEN_P(op2)));
      return SUCCESS;

    case TYPE_PAIR(IS_STRING, IS_NULL):
      ZVAL_LONG(result,
                zend_binary_strcmp(Z_STRVAL_P(op1), Z_STRLEN_P(op1), "", 0));
      return SUCCESS;

    case TYPE_PAIR(IS_OBJECT, IS_NULL):
      ZVAL_LONG(result, 1);
      return SUCCESS;

    case TYPE_PAIR(IS_NULL, IS_OBJECT):
      ZVAL_LONG(result, -1);
      return SUCCESS;

    case TYPE_PAIR(IS_OBJECT, IS_OBJECT):
      /* If both are objects sharing the same comparision handler then use is */
      if (Z_OBJ_HANDLER_P(op1, compare_objects) ==
          Z_OBJ_HANDLER_P(op2, compare_objects)) {
        if (Z_OBJ_HANDLE_P(op1) == Z_OBJ_HANDLE_P(op2)) {
          /* object handles are identical, apprently this is the same object */
          ZVAL_LONG(result, 0);
          return SUCCESS;
        }
        ZVAL_LONG(result, Z_OBJ_HT_P(op1)->compare_objects(op1, op2 TSRMLS_CC));
        return SUCCESS;
      }
      /* break missing intentionally */

    default:
      if (Z_TYPE_P(op1) == IS_OBJECT) {
        if (Z_OBJ_HT_P(op1)->get) {
          op_free = Z_OBJ_HT_P(op1)->get(op1 TSRMLS_CC);
          ret = compare_function(result, op_free, op2 TSRMLS_CC);
          zend_free_obj_get_result(op_free TSRMLS_CC);
          return ret;
        } else if (Z_TYPE_P(op2) != IS_OBJECT && Z_OBJ_HT_P(op1)->cast_object) {
          ALLOC_INIT_ZVAL(op_free);
          if (Z_OBJ_HT_P(op1)->cast_object(
                  op1, op_free, Z_TYPE_P(op2) TSRMLS_CC) == FAILURE) {
            ZVAL_LONG(result, 1);
            zend_free_obj_get_result(op_free TSRMLS_CC);
            return SUCCESS;
          }
          ret = compare_function(result, op_free, op2 TSRMLS_CC);
          zend_free_obj_get_result(op_free TSRMLS_CC);
          return ret;
        }
      }
      if (Z_TYPE_P(op2) == IS_OBJECT) {
        if (Z_OBJ_HT_P(op2)->get) {
          op_free = Z_OBJ_HT_P(op2)->get(op2 TSRMLS_CC);
          ret = compare_function(result, op1, op_free TSRMLS_CC);
          zend_free_obj_get_result(op_free TSRMLS_CC);
          return ret;
        } else if (Z_TYPE_P(op1) != IS_OBJECT && Z_OBJ_HT_P(op2)->cast_object) {
          ALLOC_INIT_ZVAL(op_free);
          if (Z_OBJ_HT_P(op2)->cast_object(
                  op2, op_free, Z_TYPE_P(op1) TSRMLS_CC) == FAILURE) {
            ZVAL_LONG(result, -1);
            zend_free_obj_get_result(op_free TSRMLS_CC);
            return SUCCESS;
          }
          ret = compare_function(result, op1, op_free TSRMLS_CC);
          zend_free_obj_get_result(op_free TSRMLS_CC);
          return ret;
        }
      }
      if (!converted) {
        if (Z_TYPE_P(op1) == IS_NULL) {
          zendi_convert_to_boolean(op2, op2_copy, result);
          ZVAL_LONG(result, Z_LVAL_P(op2) ? -1 : 0);
          return SUCCESS;
        } else if (Z_TYPE_P(op2) == IS_NULL) {
          zendi_convert_to_boolean(op1, op1_copy, result);
          ZVAL_LONG(result, Z_LVAL_P(op1) ? 1 : 0);
          return SUCCESS;
        } else if (Z_TYPE_P(op1) == IS_BOOL) {
          zendi_convert_to_boolean(op2, op2_copy, result);
          ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_LVAL_P(op1) - Z_LVAL_P(op2)));
          return SUCCESS;
        } else if (Z_TYPE_P(op2) == IS_BOOL) {
          zendi_convert_to_boolean(op1, op1_copy, result);
          ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_LVAL_P(op1) - Z_LVAL_P(op2)));
          return SUCCESS;
        } else {
          zendi_convert_scalar_to_number(op1, op1_copy, result);
          zendi_convert_scalar_to_number(op2, op2_copy, result);
          converted = 1;
        }
      } else if (Z_TYPE_P(op1) == IS_ARRAY) {
        ZVAL_LONG(result, 1);
        return SUCCESS;
      } else if (Z_TYPE_P(op2) == IS_ARRAY) {
        ZVAL_LONG(result, -1);
        return SUCCESS;
      } else if (Z_TYPE_P(op1) == IS_OBJECT) {
        ZVAL_LONG(result, 1);
        return SUCCESS;
      } else if (Z_TYPE_P(op2) == IS_OBJECT) {
        ZVAL_LONG(result, -1);
        return SUCCESS;
      } else {
        ZVAL_LONG(result, 0);
        return FAILURE;
      }
    }
  }
}
/* }}} */

static int hash_zval_identical_function(const zval **z1,
                                        const zval **z2) /* {{{ */
{
  zval result;
  TSRMLS_FETCH();

  /* is_identical_function() returns 1 in case of identity and 0 in case
   * of a difference;
   * whereas this comparison function is expected to return 0 on identity,
   * and non zero otherwise.
   */
  if (is_identical_function(&result, (zval *)*z1, (zval *)*z2 TSRMLS_CC) ==
      FAILURE) {
    return 1;
  }
  return !Z_LVAL(result);
}
/* }}} */

ZEND_API int is_identical_function(zval *result, zval *op1,
                                   zval *op2 TSRMLS_DC) /* {{{ */
{
  Z_TYPE_P(result) = IS_BOOL;
  if (Z_TYPE_P(op1) != Z_TYPE_P(op2)) {
    Z_LVAL_P(result) = 0;
    return SUCCESS;
  }
  switch (Z_TYPE_P(op1)) {
  case IS_NULL:
    Z_LVAL_P(result) = 1;
    break;
  case IS_BOOL:
  case IS_LONG:
  case IS_RESOURCE:
    Z_LVAL_P(result) = (Z_LVAL_P(op1) == Z_LVAL_P(op2));
    break;
  case IS_DOUBLE:
    Z_LVAL_P(result) = (Z_DVAL_P(op1) == Z_DVAL_P(op2));
    break;
  case IS_STRING:
    Z_LVAL_P(result) =
        ((Z_STRLEN_P(op1) == Z_STRLEN_P(op2)) &&
         (!memcmp(Z_STRVAL_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op1))));
    break;
  case IS_ARRAY:
    Z_LVAL_P(result) =
        zend_hash_compare(Z_ARRVAL_P(op1), Z_ARRVAL_P(op2),
                          (compare_func_t)hash_zval_identical_function,
                          1 TSRMLS_CC) == 0;
    break;
  case IS_OBJECT:
    if (Z_OBJ_HT_P(op1) == Z_OBJ_HT_P(op2)) {
      Z_LVAL_P(result) = (Z_OBJ_HANDLE_P(op1) == Z_OBJ_HANDLE_P(op2));
    } else {
      Z_LVAL_P(result) = 0;
    }
    break;
  default:
    Z_LVAL_P(result) = 0;
    return FAILURE;
  }
  return SUCCESS;
}
/* }}} */

ZEND_API int is_not_identical_function(zval *result, zval *op1,
                                       zval *op2 TSRMLS_DC) /* {{{ */
{
  if (is_identical_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
    return FAILURE;
  }
  Z_LVAL_P(result) = !Z_LVAL_P(result);
  return SUCCESS;
}
/* }}} */

ZEND_API int is_equal_function(zval *result, zval *op1,
                               zval *op2 TSRMLS_DC) /* {{{ */
{
  if (compare_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
    return FAILURE;
  }
  ZVAL_BOOL(result, (Z_LVAL_P(result) == 0));
  return SUCCESS;
}
/* }}} */

ZEND_API int is_not_equal_function(zval *result, zval *op1,
                                   zval *op2 TSRMLS_DC) /* {{{ */
{
  if (compare_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
    return FAILURE;
  }
  ZVAL_BOOL(result, (Z_LVAL_P(result) != 0));
  return SUCCESS;
}
/* }}} */

ZEND_API int is_smaller_function(zval *result, zval *op1,
                                 zval *op2 TSRMLS_DC) /* {{{ */
{
  if (compare_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
    return FAILURE;
  }
  ZVAL_BOOL(result, (Z_LVAL_P(result) < 0));
  return SUCCESS;
}
/* }}} */

ZEND_API int is_smaller_or_equal_function(zval *result, zval *op1,
                                          zval *op2 TSRMLS_DC) /* {{{ */
{
  if (compare_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
    return FAILURE;
  }
  ZVAL_BOOL(result, (Z_LVAL_P(result) <= 0));
  return SUCCESS;
}
/* }}} */

ZEND_API zend_bool instanceof_function_ex(
    const zend_class_entry *instance_ce, const zend_class_entry *ce,
    zend_bool interfaces_only TSRMLS_DC) /* {{{ */
{
  zend_uint i;

  for (i = 0; i < instance_ce->num_interfaces; i++) {
    if (instanceof_function(instance_ce->interfaces[i], ce TSRMLS_CC)) {
      return 1;
    }
  }
  if (!interfaces_only) {
    while (instance_ce) {
      if (instance_ce == ce) {
        return 1;
      }
      instance_ce = instance_ce->parent;
    }
  }

  return 0;
}
/* }}} */

ZEND_API zend_bool
instanceof_function(const zend_class_entry *instance_ce,
                    const zend_class_entry *ce TSRMLS_DC) /* {{{ */
{
  return instanceof_function_ex(instance_ce, ce, 0 TSRMLS_CC);
}
/* }}} */

#define LOWER_CASE 1
#define UPPER_CASE 2
#define NUMERIC 3

static void increment_string(zval *str) /* {{{ */
{
  int carry = 0;
  int pos = Z_STRLEN_P(str) - 1;
  char *s = Z_STRVAL_P(str);
  char *t;
  int last = 0; /* Shut up the compiler warning */
  int ch;

  if (Z_STRLEN_P(str) == 0) {
    STR_FREE(Z_STRVAL_P(str));
    Z_STRVAL_P(str) = estrndup("1", sizeof("1") - 1);
    Z_STRLEN_P(str) = 1;
    return;
  }

  if (IS_INTERNED(s)) {
    s = (char *)emalloc(Z_STRLEN_P(str) + 1);
    memcpy(s, Z_STRVAL_P(str), Z_STRLEN_P(str) + 1);
    Z_STRVAL_P(str) = s;
  }

  while (pos >= 0) {
    ch = s[pos];
    if (ch >= 'a' && ch <= 'z') {
      if (ch == 'z') {
        s[pos] = 'a';
        carry = 1;
      } else {
        s[pos]++;
        carry = 0;
      }
      last = LOWER_CASE;
    } else if (ch >= 'A' && ch <= 'Z') {
      if (ch == 'Z') {
        s[pos] = 'A';
        carry = 1;
      } else {
        s[pos]++;
        carry = 0;
      }
      last = UPPER_CASE;
    } else if (ch >= '0' && ch <= '9') {
      if (ch == '9') {
        s[pos] = '0';
        carry = 1;
      } else {
        s[pos]++;
        carry = 0;
      }
      last = NUMERIC;
    } else {
      carry = 0;
      break;
    }
    if (carry == 0) {
      break;
    }
    pos--;
  }

  if (carry) {
    t = (char *)emalloc(Z_STRLEN_P(str) + 1 + 1);
    memcpy(t + 1, Z_STRVAL_P(str), Z_STRLEN_P(str));
    Z_STRLEN_P(str)++;
    t[Z_STRLEN_P(str)] = '\0';
    switch (last) {
    case NUMERIC:
      t[0] = '1';
      break;
    case UPPER_CASE:
      t[0] = 'A';
      break;
    case LOWER_CASE:
      t[0] = 'a';
      break;
    }
    STR_FREE(Z_STRVAL_P(str));
    Z_STRVAL_P(str) = t;
  }
}
/* }}} */

ZEND_API int increment_function(zval *op1) /* {{{ */
{
  switch (Z_TYPE_P(op1)) {
  case IS_LONG:
    if (Z_LVAL_P(op1) == LONG_MAX) {
      /* switch to double */
      double d = (double)Z_LVAL_P(op1);
      ZVAL_DOUBLE(op1, d + 1);
    } else {
      Z_LVAL_P(op1)++;
    }
    break;
  case IS_DOUBLE:
    Z_DVAL_P(op1) = Z_DVAL_P(op1) + 1;
    break;
  case IS_NULL:
    ZVAL_LONG(op1, 1);
    break;
  case IS_STRING: {
    long lval;
    double dval;

    switch (
        is_numeric_string(Z_STRVAL_P(op1), Z_STRLEN_P(op1), &lval, &dval, 0)) {
    case IS_LONG:
      str_efree(Z_STRVAL_P(op1));
      if (lval == LONG_MAX) {
        /* switch to double */
        double d = (double)lval;
        ZVAL_DOUBLE(op1, d + 1);
      } else {
        ZVAL_LONG(op1, lval + 1);
      }
      break;
    case IS_DOUBLE:
      str_efree(Z_STRVAL_P(op1));
      ZVAL_DOUBLE(op1, dval + 1);
      break;
    default:
      /* Perl style string increment */
      increment_string(op1);
      break;
    }
  } break;
  default:
    return FAILURE;
  }
  return SUCCESS;
}
/* }}} */

ZEND_API int decrement_function(zval *op1) /* {{{ */
{
  long lval;
  double dval;

  switch (Z_TYPE_P(op1)) {
  case IS_LONG:
    if (Z_LVAL_P(op1) == LONG_MIN) {
      double d = (double)Z_LVAL_P(op1);
      ZVAL_DOUBLE(op1, d - 1);
    } else {
      Z_LVAL_P(op1)--;
    }
    break;
  case IS_DOUBLE:
    Z_DVAL_P(op1) = Z_DVAL_P(op1) - 1;
    break;
  case IS_STRING:               /* Like perl we only support string increment */
    if (Z_STRLEN_P(op1) == 0) { /* consider as 0 */
      STR_FREE(Z_STRVAL_P(op1));
      ZVAL_LONG(op1, -1);
      break;
    }
    switch (
        is_numeric_string(Z_STRVAL_P(op1), Z_STRLEN_P(op1), &lval, &dval, 0)) {
    case IS_LONG:
      STR_FREE(Z_STRVAL_P(op1));
      if (lval == LONG_MIN) {
        double d = (double)lval;
        ZVAL_DOUBLE(op1, d - 1);
      } else {
        ZVAL_LONG(op1, lval - 1);
      }
      break;
    case IS_DOUBLE:
      STR_FREE(Z_STRVAL_P(op1));
      ZVAL_DOUBLE(op1, dval - 1);
      break;
    }
    break;
  default:
    return FAILURE;
  }

  return SUCCESS;
}
/* }}} */

ZEND_API int zval_is_true(zval *op) /* {{{ */
{
  convert_to_boolean(op);
  return (Z_LVAL_P(op) ? 1 : 0);
}
/* }}} */

#ifdef ZEND_USE_TOLOWER_L
ZEND_API void zend_update_current_locale(void) /* {{{ */
{
  current_locale = _get_current_locale();
}
/* }}} */
#endif

ZEND_API char *zend_str_tolower_copy(char *dest, const char *source,
                                     unsigned int length) /* {{{ */
{
  register unsigned char *str = (unsigned char *)source;
  register unsigned char *result = (unsigned char *)dest;
  register unsigned char *end = str + length;

  while (str < end) {
    *result++ = zend_tolower((int)*str++);
  }
  *result = '\0';

  return dest;
}
/* }}} */

ZEND_API char *zend_str_tolower_dup(const char *source,
                                    unsigned int length) /* {{{ */
{
  return zend_str_tolower_copy((char *)emalloc(length + 1), source, length);
}
/* }}} */

ZEND_API void zend_str_tolower(char *str, unsigned int length) /* {{{ */
{
  register unsigned char *p = (unsigned char *)str;
  register unsigned char *end = p + length;

  while (p < end) {
    *p = zend_tolower((int)*p);
    p++;
  }
}
/* }}} */

ZEND_API int zend_binary_strcmp(const char *s1, uint len1, const char *s2,
                                uint len2) /* {{{ */
{
  int retval;

  if (s1 == s2) {
    return 0;
  }
  retval = memcmp(s1, s2, MIN(len1, len2));
  if (!retval) {
    return (len1 - len2);
  } else {
    return retval;
  }
}
/* }}} */

ZEND_API int zend_binary_strncmp(const char *s1, uint len1, const char *s2,
                                 uint len2, uint length) /* {{{ */
{
  int retval;

  if (s1 == s2) {
    return 0;
  }
  retval = memcmp(s1, s2, MIN(length, MIN(len1, len2)));
  if (!retval) {
    return (MIN(length, len1) - MIN(length, len2));
  } else {
    return retval;
  }
}
/* }}} */

ZEND_API int zend_binary_strcasecmp(const char *s1, uint len1, const char *s2,
                                    uint len2) /* {{{ */
{
  int len;
  int c1, c2;

  if (s1 == s2) {
    return 0;
  }

  len = MIN(len1, len2);
  while (len--) {
    c1 = zend_tolower((int)*(unsigned char *)s1++);
    c2 = zend_tolower((int)*(unsigned char *)s2++);
    if (c1 != c2) {
      return c1 - c2;
    }
  }

  return len1 - len2;
}
/* }}} */

ZEND_API int zend_binary_strncasecmp(const char *s1, uint len1, const char *s2,
                                     uint len2, uint length) /* {{{ */
{
  int len;
  int c1, c2;

  if (s1 == s2) {
    return 0;
  }
  len = MIN(length, MIN(len1, len2));
  while (len--) {
    c1 = zend_tolower((int)*(unsigned char *)s1++);
    c2 = zend_tolower((int)*(unsigned char *)s2++);
    if (c1 != c2) {
      return c1 - c2;
    }
  }

  return MIN(length, len1) - MIN(length, len2);
}
/* }}} */

ZEND_API int zend_binary_zval_strcmp(zval *s1, zval *s2) /* {{{ */
{
  return zend_binary_strcmp(Z_STRVAL_P(s1), Z_STRLEN_P(s1), Z_STRVAL_P(s2),
                            Z_STRLEN_P(s2));
}
/* }}} */

ZEND_API int zend_binary_zval_strncmp(zval *s1, zval *s2, zval *s3) /* {{{ */
{
  return zend_binary_strncmp(Z_STRVAL_P(s1), Z_STRLEN_P(s1), Z_STRVAL_P(s2),
                             Z_STRLEN_P(s2), Z_LVAL_P(s3));
}
/* }}} */

ZEND_API int zend_binary_zval_strcasecmp(zval *s1, zval *s2) /* {{{ */
{
  return zend_binary_strcasecmp(Z_STRVAL_P(s1), Z_STRLEN_P(s1), Z_STRVAL_P(s2),
                                Z_STRLEN_P(s2));
}
/* }}} */

ZEND_API int zend_binary_zval_strncasecmp(zval *s1, zval *s2,
                                          zval *s3) /* {{{ */
{
  return zend_binary_strncasecmp(Z_STRVAL_P(s1), Z_STRLEN_P(s1), Z_STRVAL_P(s2),
                                 Z_STRLEN_P(s2), Z_LVAL_P(s3));
}
/* }}} */

ZEND_API void zendi_smart_strcmp(zval *result, zval *s1, zval *s2) /* {{{ */
{
  int ret1, ret2;
  long lval1, lval2;
  double dval1, dval2;

  if ((ret1 = is_numeric_string(Z_STRVAL_P(s1), Z_STRLEN_P(s1), &lval1, &dval1,
                                0)) &&
      (ret2 = is_numeric_string(Z_STRVAL_P(s2), Z_STRLEN_P(s2), &lval2, &dval2,
                                0))) {
    if ((ret1 == IS_DOUBLE) || (ret2 == IS_DOUBLE)) {
      if (ret1 != IS_DOUBLE) {
        dval1 = (double)lval1;
      } else if (ret2 != IS_DOUBLE) {
        dval2 = (double)lval2;
      } else if (dval1 == dval2 && !zend_finite(dval1)) {
        /* Both values overflowed and have the same sign,
         * so a numeric comparison would be inaccurate */
        goto string_cmp;
      }
      Z_DVAL_P(result) = dval1 - dval2;
      ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_DVAL_P(result)));
    } else { /* they both have to be long's */
      ZVAL_LONG(result, lval1 > lval2 ? 1 : (lval1 < lval2 ? -1 : 0));
    }
  } else {
  string_cmp:
    Z_LVAL_P(result) = zend_binary_zval_strcmp(s1, s2);
    ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_LVAL_P(result)));
  }
}
/* }}} */

static int hash_zval_compare_function(const zval **z1,
                                      const zval **z2 TSRMLS_DC) /* {{{ */
{
  zval result;

  if (compare_function(&result, (zval *)*z1, (zval *)*z2 TSRMLS_CC) ==
      FAILURE) {
    return 1;
  }
  return Z_LVAL(result);
}
/* }}} */

ZEND_API int zend_compare_symbol_tables_i(HashTable *ht1,
                                          HashTable *ht2 TSRMLS_DC) /* {{{ */
{
  return zend_hash_compare(ht1, ht2, (compare_func_t)hash_zval_compare_function,
                           0 TSRMLS_CC);
}
/* }}} */

ZEND_API void zend_compare_symbol_tables(zval *result, HashTable *ht1,
                                         HashTable *ht2 TSRMLS_DC) /* {{{ */
{
  ZVAL_LONG(result, zend_hash_compare(
                        ht1, ht2, (compare_func_t)hash_zval_compare_function,
                        0 TSRMLS_CC));
}
/* }}} */

ZEND_API void zend_compare_arrays(zval *result, zval *a1,
                                  zval *a2 TSRMLS_DC) /* {{{ */
{
  zend_compare_symbol_tables(result, Z_ARRVAL_P(a1), Z_ARRVAL_P(a2) TSRMLS_CC);
}
/* }}} */

ZEND_API void zend_compare_objects(zval *result, zval *o1,
                                   zval *o2 TSRMLS_DC) /* {{{ */
{
  Z_TYPE_P(result) = IS_LONG;

  if (Z_OBJ_HANDLE_P(o1) == Z_OBJ_HANDLE_P(o2)) {
    Z_LVAL_P(result) = 0;
    return;
  }

  if (Z_OBJ_HT_P(o1)->compare_objects == NULL) {
    Z_LVAL_P(result) = 1;
  } else {
    Z_LVAL_P(result) = Z_OBJ_HT_P(o1)->compare_objects(o1, o2 TSRMLS_CC);
  }
}
/* }}} */

ZEND_API void zend_locale_sprintf_double(zval *op ZEND_FILE_LINE_DC) /* {{{ */
{
  TSRMLS_FETCH();

  Z_STRLEN_P(op) = zend_spprintf(&Z_STRVAL_P(op), 0, "%.*G", (int)EG(precision),
                                 (double)Z_DVAL_P(op));
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
