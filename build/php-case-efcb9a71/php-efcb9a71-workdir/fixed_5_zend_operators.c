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
  zendi_convert_to_long(op1, op1_copy, result);
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
  op1_lval = Z_LVAL_P(op1);
  zendi_convert_to_long(op2, op2_copy, result);

  ZVAL_LONG(result, op1_lval & Z_LVAL_P(op2));
  return SUCCESS;
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

  // prophet generated patch
  {
    switch (__choose("__SWITCH200")) {
    case 0:
      break;
#ifdef COMPILE_19972
    case 1: {
      // AddAndReplaceKind
      (*(result)).value.lval++;

      break;
    }
#endif
#ifdef COMPILE_19973
    case 2: {
      // AddAndReplaceKind
      (*(result)).value.lval--;

      break;
    }
#endif
#ifdef COMPILE_19974
    case 3: {
      // AddAndReplaceKind
      (*(result)).value.str.len++;

      break;
    }
#endif
#ifdef COMPILE_19975
    case 4: {
      // AddAndReplaceKind
      (buf) = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_19976
    case 5: {
      // AddAndReplaceKind
      (buf) = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_19977
    case 6: {
      // AddAndReplaceKind
      (buf)++;

      break;
    }
#endif
#ifdef COMPILE_19978
    case 7: {
      // AddAndReplaceKind
      (buf)[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_19979
    case 8: {
      // AddAndReplaceKind
      (buf)[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_19980
    case 9: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_19981
    case 10: {
      // AddAndReplaceKind
      _convert_to_string(((result)));

      break;
    }
#endif
#ifdef COMPILE_19982
    case 11: {
      // AddAndReplaceKind
      _efree(((*(result)).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_19983
    case 12: {
      // AddAndReplaceKind
      _efree(((buf)));

      break;
    }
#endif
#ifdef COMPILE_19984
    case 13: {
      // AddAndReplaceKind
      _efree(((result)));

      break;
    }
#endif
#ifdef COMPILE_19985
    case 14: {
      // AddAndReplaceKind
      _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_19986
    case 15: {
      // AddAndReplaceKind
      _zend_list_delete((*(result)).value.lval);

      break;
    }
#endif
#ifdef COMPILE_19987
    case 16: {
      // AddAndReplaceKind
      _zval_copy_ctor(((result)));

      break;
    }
#endif
#ifdef COMPILE_19988
    case 17: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((result));

      break;
    }
#endif
#ifdef COMPILE_19989
    case 18: {
      // AddAndReplaceKind
      _zval_dtor(((result)));

      break;
    }
#endif
#ifdef COMPILE_19990
    case 19: {
      // AddAndReplaceKind
      _zval_dtor_func((result));

      break;
    }
#endif
#ifdef COMPILE_19991
    case 20: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(result)));

      break;
    }
#endif
#ifdef COMPILE_19992
    case 21: {
      // AddAndReplaceKind
      buf = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_19993
    case 22: {
      // AddAndReplaceKind
      buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_19994
    case 23: {
      // AddAndReplaceKind
      buf[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_19995
    case 24: {
      // AddAndReplaceKind
      buf[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_19996
    case 25: {
      // AddAndReplaceKind
      convert_scalar_to_array((result), 4);

      break;
    }
#endif
#ifdef COMPILE_19997
    case 26: {
      // AddAndReplaceKind
      convert_scalar_to_number((result));

      break;
    }
#endif
#ifdef COMPILE_19998
    case 27: {
      // AddAndReplaceKind
      convert_to_array((result));

      break;
    }
#endif
#ifdef COMPILE_19999
    case 28: {
      // AddAndReplaceKind
      convert_to_boolean((result));

      break;
    }
#endif
#ifdef COMPILE_20000
    case 29: {
      // AddAndReplaceKind
      convert_to_double((result));

      break;
    }
#endif
#ifdef COMPILE_20001
    case 30: {
      // AddAndReplaceKind
      convert_to_long((result));

      break;
    }
#endif
#ifdef COMPILE_20002
    case 31: {
      // AddAndReplaceKind
      convert_to_long_base((result), 10);

      break;
    }
#endif
#ifdef COMPILE_20003
    case 32: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_20004
    case 33: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((result));

      break;
    }
#endif
#ifdef COMPILE_20005
    case 34: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((result)));

      break;
    }
#endif
#ifdef COMPILE_20006
    case 35: {
      // AddAndReplaceKind
      gc_zval_possible_root((result));

      break;
    }
#endif
#ifdef COMPILE_20007
    case 36: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((result));

      break;
    }
#endif
#ifdef COMPILE_20008
    case 37: {
      // AddAndReplaceKind
      if (((((*op1).value.str.val) >=
            (compiler_globals.interned_strings_start)) &&
           (((*op1).value.str.val) <
            (compiler_globals.interned_strings_end)))) {
        buf = (char *)_emalloc((length + 1));
        memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);
      } else {
        buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);
      }

      break;
    }
#endif
#ifdef COMPILE_20009
    case 38: {
      // AddAndReplaceKind
      memcpy((buf), (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20010
    case 39: {
      // AddAndReplaceKind
      memcpy(buf, (*(op1)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20011
    case 40: {
      // AddAndReplaceKind
      memcpy(buf, (*(op2)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20012
    case 41: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op1)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20013
    case 42: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op2)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20014
    case 43: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20015
    case 44: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "String size overflow");

      break;
    }
#endif
#ifdef COMPILE_20016
    case 45: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Unsupported operand types");

      break;
    }
#endif
#ifdef COMPILE_20017
    case 46: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(1));

      break;
    }
#endif
#ifdef COMPILE_20018
    case 47: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(2));

      break;
    }
#endif
#ifdef COMPILE_20019
    case 48: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(3));

      break;
    }
#endif
#ifdef COMPILE_20020
    case 49: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(4));

      break;
    }
#endif
#ifdef COMPILE_20021
    case 50: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(6));

      break;
    }
#endif
#ifdef COMPILE_20022
    case 51: {
      // AddAndReplaceKind
      zend_error((1 << 1L), "Division by zero");

      break;
    }
#endif
#ifdef COMPILE_20023
    case 52: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_20024
    case 53: {
      // AddAndReplaceKind
      zend_error((1 << 3L),
                 "Object of class %s could not be converted to double",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20025
    case 54: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s could not be converted to int",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20026
    case 55: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s to string conversion",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20027
    case 56: {
      // AddAndReplaceKind
      zend_free_obj_get_result((result));

      break;
    }
#endif
#ifdef COMPILE_20028
    case 57: {
      // AddAndReplaceKind
      zval_unset_isref_p((result));

      break;
    }
#endif
#ifdef COMPILE_20029
    case 58: {
      // AddInitKind
      memset(&(*op1), 0, sizeof(*(&(*op1))));

      break;
    }
#endif
#ifdef COMPILE_20030
    case 59: {
      // AddInitKind
      memset(&(*op1).value, 0, sizeof(*(&(*op1).value)));

      break;
    }
#endif
#ifdef COMPILE_20031
    case 60: {
      // AddInitKind
      memset(&(*op1).value.str, 0, sizeof(*(&(*op1).value.str)));

      break;
    }
#endif
#ifdef COMPILE_20032
    case 61: {
      // AddInitKind
      memset(&compiler_globals, 0, sizeof(*(&compiler_globals)));

      break;
    }
#endif
#ifdef COMPILE_20033
    case 62: {
      // IfExitKind
      if (__is_neg(
              "200-62", 82, &(op1), sizeof(op1),
              &(compiler_globals.interned_strings_start),
              sizeof(compiler_globals.interned_strings_start),
              &(compiler_globals.interned_strings_end),
              sizeof(compiler_globals.interned_strings_end),
              &((*op1).value.str.val), sizeof((*op1).value.str.val),
              &(compiler_globals.interned_strings_start),
              sizeof(compiler_globals.interned_strings_start),
              &(compiler_globals.interned_strings_end),
              sizeof(compiler_globals.interned_strings_end), &(length),
              sizeof(length), &(buf), sizeof(buf), &((*op1).value.str.len),
              sizeof((*op1).value.str.len), &(op2), sizeof(op2), &(result),
              sizeof(result), &(compiler_globals.active_class_entry),
              sizeof(compiler_globals.active_class_entry),
              &(compiler_globals.compiled_filename),
              sizeof(compiler_globals.compiled_filename),
              &(compiler_globals.zend_lineno),
              sizeof(compiler_globals.zend_lineno), &(compiler_globals.heredoc),
              sizeof(compiler_globals.heredoc), &(compiler_globals.heredoc_len),
              sizeof(compiler_globals.heredoc_len),
              &(compiler_globals.active_op_array),
              sizeof(compiler_globals.active_op_array),
              &(compiler_globals.function_table),
              sizeof(compiler_globals.function_table),
              &(compiler_globals.class_table),
              sizeof(compiler_globals.class_table),
              &(compiler_globals.auto_globals),
              sizeof(compiler_globals.auto_globals),
              &(compiler_globals.in_compilation),
              sizeof(compiler_globals.in_compilation),
              &(compiler_globals.short_tags),
              sizeof(compiler_globals.short_tags), &(compiler_globals.asp_tags),
              sizeof(compiler_globals.asp_tags),
              &(compiler_globals.unclean_shutdown),
              sizeof(compiler_globals.unclean_shutdown),
              &(compiler_globals.ini_parser_unbuffered_errors),
              sizeof(compiler_globals.ini_parser_unbuffered_errors),
              &(compiler_globals.catch_begin),
              sizeof(compiler_globals.catch_begin),
              &(compiler_globals.ini_parser_param),
              sizeof(compiler_globals.ini_parser_param),
              &(compiler_globals.interactive),
              sizeof(compiler_globals.interactive),
              &(compiler_globals.start_lineno),
              sizeof(compiler_globals.start_lineno),
              &(compiler_globals.increment_lineno),
              sizeof(compiler_globals.increment_lineno),
              &(compiler_globals.access_type),
              sizeof(compiler_globals.access_type),
              &(compiler_globals.doc_comment),
              sizeof(compiler_globals.doc_comment),
              &(compiler_globals.doc_comment_len),
              sizeof(compiler_globals.doc_comment_len),
              &(compiler_globals.compiler_options),
              sizeof(compiler_globals.compiler_options),
              &(compiler_globals.current_namespace),
              sizeof(compiler_globals.current_namespace),
              &(compiler_globals.current_import),
              sizeof(compiler_globals.current_import),
              &(compiler_globals.in_namespace),
              sizeof(compiler_globals.in_namespace),
              &(compiler_globals.has_bracketed_namespaces),
              sizeof(compiler_globals.has_bracketed_namespaces),
              &(compiler_globals.interned_strings_top),
              sizeof(compiler_globals.interned_strings_top),
              &(compiler_globals.interned_strings_snapshot_top),
              sizeof(compiler_globals.interned_strings_snapshot_top),
              &(compiler_globals.script_encoding_list),
              sizeof(compiler_globals.script_encoding_list),
              &(compiler_globals.script_encoding_list_size),
              sizeof(compiler_globals.script_encoding_list_size),
              &(compiler_globals.multibyte), sizeof(compiler_globals.multibyte),
              &(compiler_globals.detect_unicode),
              sizeof(compiler_globals.detect_unicode),
              &(compiler_globals.encoding_declared),
              sizeof(compiler_globals.encoding_declared),
              &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
              &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
              sizeof((*op1).is_ref__gc), &(compiler_globals.active_class_entry),
              sizeof(compiler_globals.active_class_entry),
              &(compiler_globals.compiled_filename),
              sizeof(compiler_globals.compiled_filename),
              &(compiler_globals.zend_lineno),
              sizeof(compiler_globals.zend_lineno), &(compiler_globals.heredoc),
              sizeof(compiler_globals.heredoc), &(compiler_globals.heredoc_len),
              sizeof(compiler_globals.heredoc_len),
              &(compiler_globals.active_op_array),
              sizeof(compiler_globals.active_op_array),
              &(compiler_globals.function_table),
              sizeof(compiler_globals.function_table),
              &(compiler_globals.class_table),
              sizeof(compiler_globals.class_table),
              &(compiler_globals.auto_globals),
              sizeof(compiler_globals.auto_globals),
              &(compiler_globals.in_compilation),
              sizeof(compiler_globals.in_compilation),
              &(compiler_globals.short_tags),
              sizeof(compiler_globals.short_tags), &(compiler_globals.asp_tags),
              sizeof(compiler_globals.asp_tags),
              &(compiler_globals.unclean_shutdown),
              sizeof(compiler_globals.unclean_shutdown),
              &(compiler_globals.ini_parser_unbuffered_errors),
              sizeof(compiler_globals.ini_parser_unbuffered_errors),
              &(compiler_globals.catch_begin),
              sizeof(compiler_globals.catch_begin),
              &(compiler_globals.ini_parser_param),
              sizeof(compiler_globals.ini_parser_param),
              &(compiler_globals.interactive),
              sizeof(compiler_globals.interactive),
              &(compiler_globals.start_lineno),
              sizeof(compiler_globals.start_lineno),
              &(compiler_globals.increment_lineno),
              sizeof(compiler_globals.increment_lineno),
              &(compiler_globals.access_type),
              sizeof(compiler_globals.access_type),
              &(compiler_globals.doc_comment),
              sizeof(compiler_globals.doc_comment),
              &(compiler_globals.doc_comment_len),
              sizeof(compiler_globals.doc_comment_len),
              &(compiler_globals.compiler_options),
              sizeof(compiler_globals.compiler_options),
              &(compiler_globals.current_namespace),
              sizeof(compiler_globals.current_namespace),
              &(compiler_globals.current_import),
              sizeof(compiler_globals.current_import),
              &(compiler_globals.in_namespace),
              sizeof(compiler_globals.in_namespace),
              &(compiler_globals.has_bracketed_namespaces),
              sizeof(compiler_globals.has_bracketed_namespaces),
              &(compiler_globals.interned_strings_top),
              sizeof(compiler_globals.interned_strings_top),
              &(compiler_globals.interned_strings_snapshot_top),
              sizeof(compiler_globals.interned_strings_snapshot_top),
              &(compiler_globals.script_encoding_list),
              sizeof(compiler_globals.script_encoding_list),
              &(compiler_globals.script_encoding_list_size),
              sizeof(compiler_globals.script_encoding_list_size),
              &(compiler_globals.multibyte), sizeof(compiler_globals.multibyte),
              &(compiler_globals.detect_unicode),
              sizeof(compiler_globals.detect_unicode),
              &(compiler_globals.encoding_declared),
              sizeof(compiler_globals.encoding_declared)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_20034
    case 63: {
      // IfExitKind
      if (__is_neg(
              "200-63", 82, &(op1), sizeof(op1),
              &(compiler_globals.interned_strings_start),
              sizeof(compiler_globals.interned_strings_start),
              &(compiler_globals.interned_strings_end),
              sizeof(compiler_globals.interned_strings_end),
              &((*op1).value.str.val), sizeof((*op1).value.str.val),
              &(compiler_globals.interned_strings_start),
              sizeof(compiler_globals.interned_strings_start),
              &(compiler_globals.interned_strings_end),
              sizeof(compiler_globals.interned_strings_end), &(length),
              sizeof(length), &(buf), sizeof(buf), &((*op1).value.str.len),
              sizeof((*op1).value.str.len), &(op2), sizeof(op2), &(result),
              sizeof(result), &(compiler_globals.active_class_entry),
              sizeof(compiler_globals.active_class_entry),
              &(compiler_globals.compiled_filename),
              sizeof(compiler_globals.compiled_filename),
              &(compiler_globals.zend_lineno),
              sizeof(compiler_globals.zend_lineno), &(compiler_globals.heredoc),
              sizeof(compiler_globals.heredoc), &(compiler_globals.heredoc_len),
              sizeof(compiler_globals.heredoc_len),
              &(compiler_globals.active_op_array),
              sizeof(compiler_globals.active_op_array),
              &(compiler_globals.function_table),
              sizeof(compiler_globals.function_table),
              &(compiler_globals.class_table),
              sizeof(compiler_globals.class_table),
              &(compiler_globals.auto_globals),
              sizeof(compiler_globals.auto_globals),
              &(compiler_globals.in_compilation),
              sizeof(compiler_globals.in_compilation),
              &(compiler_globals.short_tags),
              sizeof(compiler_globals.short_tags), &(compiler_globals.asp_tags),
              sizeof(compiler_globals.asp_tags),
              &(compiler_globals.unclean_shutdown),
              sizeof(compiler_globals.unclean_shutdown),
              &(compiler_globals.ini_parser_unbuffered_errors),
              sizeof(compiler_globals.ini_parser_unbuffered_errors),
              &(compiler_globals.catch_begin),
              sizeof(compiler_globals.catch_begin),
              &(compiler_globals.ini_parser_param),
              sizeof(compiler_globals.ini_parser_param),
              &(compiler_globals.interactive),
              sizeof(compiler_globals.interactive),
              &(compiler_globals.start_lineno),
              sizeof(compiler_globals.start_lineno),
              &(compiler_globals.increment_lineno),
              sizeof(compiler_globals.increment_lineno),
              &(compiler_globals.access_type),
              sizeof(compiler_globals.access_type),
              &(compiler_globals.doc_comment),
              sizeof(compiler_globals.doc_comment),
              &(compiler_globals.doc_comment_len),
              sizeof(compiler_globals.doc_comment_len),
              &(compiler_globals.compiler_options),
              sizeof(compiler_globals.compiler_options),
              &(compiler_globals.current_namespace),
              sizeof(compiler_globals.current_namespace),
              &(compiler_globals.current_import),
              sizeof(compiler_globals.current_import),
              &(compiler_globals.in_namespace),
              sizeof(compiler_globals.in_namespace),
              &(compiler_globals.has_bracketed_namespaces),
              sizeof(compiler_globals.has_bracketed_namespaces),
              &(compiler_globals.interned_strings_top),
              sizeof(compiler_globals.interned_strings_top),
              &(compiler_globals.interned_strings_snapshot_top),
              sizeof(compiler_globals.interned_strings_snapshot_top),
              &(compiler_globals.script_encoding_list),
              sizeof(compiler_globals.script_encoding_list),
              &(compiler_globals.script_encoding_list_size),
              sizeof(compiler_globals.script_encoding_list_size),
              &(compiler_globals.multibyte), sizeof(compiler_globals.multibyte),
              &(compiler_globals.detect_unicode),
              sizeof(compiler_globals.detect_unicode),
              &(compiler_globals.encoding_declared),
              sizeof(compiler_globals.encoding_declared),
              &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
              &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
              sizeof((*op1).is_ref__gc), &(compiler_globals.active_class_entry),
              sizeof(compiler_globals.active_class_entry),
              &(compiler_globals.compiled_filename),
              sizeof(compiler_globals.compiled_filename),
              &(compiler_globals.zend_lineno),
              sizeof(compiler_globals.zend_lineno), &(compiler_globals.heredoc),
              sizeof(compiler_globals.heredoc), &(compiler_globals.heredoc_len),
              sizeof(compiler_globals.heredoc_len),
              &(compiler_globals.active_op_array),
              sizeof(compiler_globals.active_op_array),
              &(compiler_globals.function_table),
              sizeof(compiler_globals.function_table),
              &(compiler_globals.class_table),
              sizeof(compiler_globals.class_table),
              &(compiler_globals.auto_globals),
              sizeof(compiler_globals.auto_globals),
              &(compiler_globals.in_compilation),
              sizeof(compiler_globals.in_compilation),
              &(compiler_globals.short_tags),
              sizeof(compiler_globals.short_tags), &(compiler_globals.asp_tags),
              sizeof(compiler_globals.asp_tags),
              &(compiler_globals.unclean_shutdown),
              sizeof(compiler_globals.unclean_shutdown),
              &(compiler_globals.ini_parser_unbuffered_errors),
              sizeof(compiler_globals.ini_parser_unbuffered_errors),
              &(compiler_globals.catch_begin),
              sizeof(compiler_globals.catch_begin),
              &(compiler_globals.ini_parser_param),
              sizeof(compiler_globals.ini_parser_param),
              &(compiler_globals.interactive),
              sizeof(compiler_globals.interactive),
              &(compiler_globals.start_lineno),
              sizeof(compiler_globals.start_lineno),
              &(compiler_globals.increment_lineno),
              sizeof(compiler_globals.increment_lineno),
              &(compiler_globals.access_type),
              sizeof(compiler_globals.access_type),
              &(compiler_globals.doc_comment),
              sizeof(compiler_globals.doc_comment),
              &(compiler_globals.doc_comment_len),
              sizeof(compiler_globals.doc_comment_len),
              &(compiler_globals.compiler_options),
              sizeof(compiler_globals.compiler_options),
              &(compiler_globals.current_namespace),
              sizeof(compiler_globals.current_namespace),
              &(compiler_globals.current_import),
              sizeof(compiler_globals.current_import),
              &(compiler_globals.in_namespace),
              sizeof(compiler_globals.in_namespace),
              &(compiler_globals.has_bracketed_namespaces),
              sizeof(compiler_globals.has_bracketed_namespaces),
              &(compiler_globals.interned_strings_top),
              sizeof(compiler_globals.interned_strings_top),
              &(compiler_globals.interned_strings_snapshot_top),
              sizeof(compiler_globals.interned_strings_snapshot_top),
              &(compiler_globals.script_encoding_list),
              sizeof(compiler_globals.script_encoding_list),
              &(compiler_globals.script_encoding_list_size),
              sizeof(compiler_globals.script_encoding_list_size),
              &(compiler_globals.multibyte), sizeof(compiler_globals.multibyte),
              &(compiler_globals.detect_unicode),
              sizeof(compiler_globals.detect_unicode),
              &(compiler_globals.encoding_declared),
              sizeof(compiler_globals.encoding_declared)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_20035
    case 64: {
      // IfExitKind
      if (__is_neg(
              "200-64", 82, &(op1), sizeof(op1),
              &(compiler_globals.interned_strings_start),
              sizeof(compiler_globals.interned_strings_start),
              &(compiler_globals.interned_strings_end),
              sizeof(compiler_globals.interned_strings_end),
              &((*op1).value.str.val), sizeof((*op1).value.str.val),
              &(compiler_globals.interned_strings_start),
              sizeof(compiler_globals.interned_strings_start),
              &(compiler_globals.interned_strings_end),
              sizeof(compiler_globals.interned_strings_end), &(length),
              sizeof(length), &(buf), sizeof(buf), &((*op1).value.str.len),
              sizeof((*op1).value.str.len), &(op2), sizeof(op2), &(result),
              sizeof(result), &(compiler_globals.active_class_entry),
              sizeof(compiler_globals.active_class_entry),
              &(compiler_globals.compiled_filename),
              sizeof(compiler_globals.compiled_filename),
              &(compiler_globals.zend_lineno),
              sizeof(compiler_globals.zend_lineno), &(compiler_globals.heredoc),
              sizeof(compiler_globals.heredoc), &(compiler_globals.heredoc_len),
              sizeof(compiler_globals.heredoc_len),
              &(compiler_globals.active_op_array),
              sizeof(compiler_globals.active_op_array),
              &(compiler_globals.function_table),
              sizeof(compiler_globals.function_table),
              &(compiler_globals.class_table),
              sizeof(compiler_globals.class_table),
              &(compiler_globals.auto_globals),
              sizeof(compiler_globals.auto_globals),
              &(compiler_globals.in_compilation),
              sizeof(compiler_globals.in_compilation),
              &(compiler_globals.short_tags),
              sizeof(compiler_globals.short_tags), &(compiler_globals.asp_tags),
              sizeof(compiler_globals.asp_tags),
              &(compiler_globals.unclean_shutdown),
              sizeof(compiler_globals.unclean_shutdown),
              &(compiler_globals.ini_parser_unbuffered_errors),
              sizeof(compiler_globals.ini_parser_unbuffered_errors),
              &(compiler_globals.catch_begin),
              sizeof(compiler_globals.catch_begin),
              &(compiler_globals.ini_parser_param),
              sizeof(compiler_globals.ini_parser_param),
              &(compiler_globals.interactive),
              sizeof(compiler_globals.interactive),
              &(compiler_globals.start_lineno),
              sizeof(compiler_globals.start_lineno),
              &(compiler_globals.increment_lineno),
              sizeof(compiler_globals.increment_lineno),
              &(compiler_globals.access_type),
              sizeof(compiler_globals.access_type),
              &(compiler_globals.doc_comment),
              sizeof(compiler_globals.doc_comment),
              &(compiler_globals.doc_comment_len),
              sizeof(compiler_globals.doc_comment_len),
              &(compiler_globals.compiler_options),
              sizeof(compiler_globals.compiler_options),
              &(compiler_globals.current_namespace),
              sizeof(compiler_globals.current_namespace),
              &(compiler_globals.current_import),
              sizeof(compiler_globals.current_import),
              &(compiler_globals.in_namespace),
              sizeof(compiler_globals.in_namespace),
              &(compiler_globals.has_bracketed_namespaces),
              sizeof(compiler_globals.has_bracketed_namespaces),
              &(compiler_globals.interned_strings_top),
              sizeof(compiler_globals.interned_strings_top),
              &(compiler_globals.interned_strings_snapshot_top),
              sizeof(compiler_globals.interned_strings_snapshot_top),
              &(compiler_globals.script_encoding_list),
              sizeof(compiler_globals.script_encoding_list),
              &(compiler_globals.script_encoding_list_size),
              sizeof(compiler_globals.script_encoding_list_size),
              &(compiler_globals.multibyte), sizeof(compiler_globals.multibyte),
              &(compiler_globals.detect_unicode),
              sizeof(compiler_globals.detect_unicode),
              &(compiler_globals.encoding_declared),
              sizeof(compiler_globals.encoding_declared),
              &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
              &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
              sizeof((*op1).is_ref__gc), &(compiler_globals.active_class_entry),
              sizeof(compiler_globals.active_class_entry),
              &(compiler_globals.compiled_filename),
              sizeof(compiler_globals.compiled_filename),
              &(compiler_globals.zend_lineno),
              sizeof(compiler_globals.zend_lineno), &(compiler_globals.heredoc),
              sizeof(compiler_globals.heredoc), &(compiler_globals.heredoc_len),
              sizeof(compiler_globals.heredoc_len),
              &(compiler_globals.active_op_array),
              sizeof(compiler_globals.active_op_array),
              &(compiler_globals.function_table),
              sizeof(compiler_globals.function_table),
              &(compiler_globals.class_table),
              sizeof(compiler_globals.class_table),
              &(compiler_globals.auto_globals),
              sizeof(compiler_globals.auto_globals),
              &(compiler_globals.in_compilation),
              sizeof(compiler_globals.in_compilation),
              &(compiler_globals.short_tags),
              sizeof(compiler_globals.short_tags), &(compiler_globals.asp_tags),
              sizeof(compiler_globals.asp_tags),
              &(compiler_globals.unclean_shutdown),
              sizeof(compiler_globals.unclean_shutdown),
              &(compiler_globals.ini_parser_unbuffered_errors),
              sizeof(compiler_globals.ini_parser_unbuffered_errors),
              &(compiler_globals.catch_begin),
              sizeof(compiler_globals.catch_begin),
              &(compiler_globals.ini_parser_param),
              sizeof(compiler_globals.ini_parser_param),
              &(compiler_globals.interactive),
              sizeof(compiler_globals.interactive),
              &(compiler_globals.start_lineno),
              sizeof(compiler_globals.start_lineno),
              &(compiler_globals.increment_lineno),
              sizeof(compiler_globals.increment_lineno),
              &(compiler_globals.access_type),
              sizeof(compiler_globals.access_type),
              &(compiler_globals.doc_comment),
              sizeof(compiler_globals.doc_comment),
              &(compiler_globals.doc_comment_len),
              sizeof(compiler_globals.doc_comment_len),
              &(compiler_globals.compiler_options),
              sizeof(compiler_globals.compiler_options),
              &(compiler_globals.current_namespace),
              sizeof(compiler_globals.current_namespace),
              &(compiler_globals.current_import),
              sizeof(compiler_globals.current_import),
              &(compiler_globals.in_namespace),
              sizeof(compiler_globals.in_namespace),
              &(compiler_globals.has_bracketed_namespaces),
              sizeof(compiler_globals.has_bracketed_namespaces),
              &(compiler_globals.interned_strings_top),
              sizeof(compiler_globals.interned_strings_top),
              &(compiler_globals.interned_strings_snapshot_top),
              sizeof(compiler_globals.interned_strings_snapshot_top),
              &(compiler_globals.script_encoding_list),
              sizeof(compiler_globals.script_encoding_list),
              &(compiler_globals.script_encoding_list_size),
              sizeof(compiler_globals.script_encoding_list_size),
              &(compiler_globals.multibyte), sizeof(compiler_globals.multibyte),
              &(compiler_globals.detect_unicode),
              sizeof(compiler_globals.detect_unicode),
              &(compiler_globals.encoding_declared),
              sizeof(compiler_globals.encoding_declared)))
        return 6;

      break;
    }
#endif
    }
    {
      long long __temp201 = ((
          (((*op1).value.str.val) >=
           (compiler_globals.interned_strings_start)) &&
          (((*op1).value.str.val) < (compiler_globals.interned_strings_end))));
      switch (__choose("__SWITCH201")) {
      case 0: {

        break;
      }
#ifdef COMPILE_20036
      case 1: {
        __temp201 =
            ((((((*op1).value.str.val) >=
                (compiler_globals.interned_strings_start)) &&
               (((*op1).value.str.val) <
                (compiler_globals.interned_strings_end)))) ||
             __is_neg(
                 "201-1", 82, &(op1), sizeof(op1),
                 &(compiler_globals.interned_strings_start),
                 sizeof(compiler_globals.interned_strings_start),
                 &(compiler_globals.interned_strings_end),
                 sizeof(compiler_globals.interned_strings_end),
                 &((*op1).value.str.val), sizeof((*op1).value.str.val),
                 &(compiler_globals.interned_strings_start),
                 sizeof(compiler_globals.interned_strings_start),
                 &(compiler_globals.interned_strings_end),
                 sizeof(compiler_globals.interned_strings_end), &(length),
                 sizeof(length), &(buf), sizeof(buf), &((*op1).value.str.len),
                 sizeof((*op1).value.str.len), &(op2), sizeof(op2), &(result),
                 sizeof(result), &(compiler_globals.active_class_entry),
                 sizeof(compiler_globals.active_class_entry),
                 &(compiler_globals.compiled_filename),
                 sizeof(compiler_globals.compiled_filename),
                 &(compiler_globals.zend_lineno),
                 sizeof(compiler_globals.zend_lineno),
                 &(compiler_globals.heredoc), sizeof(compiler_globals.heredoc),
                 &(compiler_globals.heredoc_len),
                 sizeof(compiler_globals.heredoc_len),
                 &(compiler_globals.active_op_array),
                 sizeof(compiler_globals.active_op_array),
                 &(compiler_globals.function_table),
                 sizeof(compiler_globals.function_table),
                 &(compiler_globals.class_table),
                 sizeof(compiler_globals.class_table),
                 &(compiler_globals.auto_globals),
                 sizeof(compiler_globals.auto_globals),
                 &(compiler_globals.in_compilation),
                 sizeof(compiler_globals.in_compilation),
                 &(compiler_globals.short_tags),
                 sizeof(compiler_globals.short_tags),
                 &(compiler_globals.asp_tags),
                 sizeof(compiler_globals.asp_tags),
                 &(compiler_globals.unclean_shutdown),
                 sizeof(compiler_globals.unclean_shutdown),
                 &(compiler_globals.ini_parser_unbuffered_errors),
                 sizeof(compiler_globals.ini_parser_unbuffered_errors),
                 &(compiler_globals.catch_begin),
                 sizeof(compiler_globals.catch_begin),
                 &(compiler_globals.ini_parser_param),
                 sizeof(compiler_globals.ini_parser_param),
                 &(compiler_globals.interactive),
                 sizeof(compiler_globals.interactive),
                 &(compiler_globals.start_lineno),
                 sizeof(compiler_globals.start_lineno),
                 &(compiler_globals.increment_lineno),
                 sizeof(compiler_globals.increment_lineno),
                 &(compiler_globals.access_type),
                 sizeof(compiler_globals.access_type),
                 &(compiler_globals.doc_comment),
                 sizeof(compiler_globals.doc_comment),
                 &(compiler_globals.doc_comment_len),
                 sizeof(compiler_globals.doc_comment_len),
                 &(compiler_globals.compiler_options),
                 sizeof(compiler_globals.compiler_options),
                 &(compiler_globals.current_namespace),
                 sizeof(compiler_globals.current_namespace),
                 &(compiler_globals.current_import),
                 sizeof(compiler_globals.current_import),
                 &(compiler_globals.in_namespace),
                 sizeof(compiler_globals.in_namespace),
                 &(compiler_globals.has_bracketed_namespaces),
                 sizeof(compiler_globals.has_bracketed_namespaces),
                 &(compiler_globals.interned_strings_top),
                 sizeof(compiler_globals.interned_strings_top),
                 &(compiler_globals.interned_strings_snapshot_top),
                 sizeof(compiler_globals.interned_strings_snapshot_top),
                 &(compiler_globals.script_encoding_list),
                 sizeof(compiler_globals.script_encoding_list),
                 &(compiler_globals.script_encoding_list_size),
                 sizeof(compiler_globals.script_encoding_list_size),
                 &(compiler_globals.multibyte),
                 sizeof(compiler_globals.multibyte),
                 &(compiler_globals.detect_unicode),
                 sizeof(compiler_globals.detect_unicode),
                 &(compiler_globals.encoding_declared),
                 sizeof(compiler_globals.encoding_declared),
                 &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                 &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
                 sizeof((*op1).is_ref__gc),
                 &(compiler_globals.active_class_entry),
                 sizeof(compiler_globals.active_class_entry),
                 &(compiler_globals.compiled_filename),
                 sizeof(compiler_globals.compiled_filename),
                 &(compiler_globals.zend_lineno),
                 sizeof(compiler_globals.zend_lineno),
                 &(compiler_globals.heredoc), sizeof(compiler_globals.heredoc),
                 &(compiler_globals.heredoc_len),
                 sizeof(compiler_globals.heredoc_len),
                 &(compiler_globals.active_op_array),
                 sizeof(compiler_globals.active_op_array),
                 &(compiler_globals.function_table),
                 sizeof(compiler_globals.function_table),
                 &(compiler_globals.class_table),
                 sizeof(compiler_globals.class_table),
                 &(compiler_globals.auto_globals),
                 sizeof(compiler_globals.auto_globals),
                 &(compiler_globals.in_compilation),
                 sizeof(compiler_globals.in_compilation),
                 &(compiler_globals.short_tags),
                 sizeof(compiler_globals.short_tags),
                 &(compiler_globals.asp_tags),
                 sizeof(compiler_globals.asp_tags),
                 &(compiler_globals.unclean_shutdown),
                 sizeof(compiler_globals.unclean_shutdown),
                 &(compiler_globals.ini_parser_unbuffered_errors),
                 sizeof(compiler_globals.ini_parser_unbuffered_errors),
                 &(compiler_globals.catch_begin),
                 sizeof(compiler_globals.catch_begin),
                 &(compiler_globals.ini_parser_param),
                 sizeof(compiler_globals.ini_parser_param),
                 &(compiler_globals.interactive),
                 sizeof(compiler_globals.interactive),
                 &(compiler_globals.start_lineno),
                 sizeof(compiler_globals.start_lineno),
                 &(compiler_globals.increment_lineno),
                 sizeof(compiler_globals.increment_lineno),
                 &(compiler_globals.access_type),
                 sizeof(compiler_globals.access_type),
                 &(compiler_globals.doc_comment),
                 sizeof(compiler_globals.doc_comment),
                 &(compiler_globals.doc_comment_len),
                 sizeof(compiler_globals.doc_comment_len),
                 &(compiler_globals.compiler_options),
                 sizeof(compiler_globals.compiler_options),
                 &(compiler_globals.current_namespace),
                 sizeof(compiler_globals.current_namespace),
                 &(compiler_globals.current_import),
                 sizeof(compiler_globals.current_import),
                 &(compiler_globals.in_namespace),
                 sizeof(compiler_globals.in_namespace),
                 &(compiler_globals.has_bracketed_namespaces),
                 sizeof(compiler_globals.has_bracketed_namespaces),
                 &(compiler_globals.interned_strings_top),
                 sizeof(compiler_globals.interned_strings_top),
                 &(compiler_globals.interned_strings_snapshot_top),
                 sizeof(compiler_globals.interned_strings_snapshot_top),
                 &(compiler_globals.script_encoding_list),
                 sizeof(compiler_globals.script_encoding_list),
                 &(compiler_globals.script_encoding_list_size),
                 sizeof(compiler_globals.script_encoding_list_size),
                 &(compiler_globals.multibyte),
                 sizeof(compiler_globals.multibyte),
                 &(compiler_globals.detect_unicode),
                 sizeof(compiler_globals.detect_unicode),
                 &(compiler_globals.encoding_declared),
                 sizeof(compiler_globals.encoding_declared)));

        break;
      }
#endif
#ifdef COMPILE_20037
      case 2: {
        __temp201 =
            ((((((*op1).value.str.val) >=
                (compiler_globals.interned_strings_start)) &&
               (((*op1).value.str.val) <
                (compiler_globals.interned_strings_end)))) &&
             !__is_neg(
                 "201-2", 82, &(op1), sizeof(op1),
                 &(compiler_globals.interned_strings_start),
                 sizeof(compiler_globals.interned_strings_start),
                 &(compiler_globals.interned_strings_end),
                 sizeof(compiler_globals.interned_strings_end),
                 &((*op1).value.str.val), sizeof((*op1).value.str.val),
                 &(compiler_globals.interned_strings_start),
                 sizeof(compiler_globals.interned_strings_start),
                 &(compiler_globals.interned_strings_end),
                 sizeof(compiler_globals.interned_strings_end), &(length),
                 sizeof(length), &(buf), sizeof(buf), &((*op1).value.str.len),
                 sizeof((*op1).value.str.len), &(op2), sizeof(op2), &(result),
                 sizeof(result), &(compiler_globals.active_class_entry),
                 sizeof(compiler_globals.active_class_entry),
                 &(compiler_globals.compiled_filename),
                 sizeof(compiler_globals.compiled_filename),
                 &(compiler_globals.zend_lineno),
                 sizeof(compiler_globals.zend_lineno),
                 &(compiler_globals.heredoc), sizeof(compiler_globals.heredoc),
                 &(compiler_globals.heredoc_len),
                 sizeof(compiler_globals.heredoc_len),
                 &(compiler_globals.active_op_array),
                 sizeof(compiler_globals.active_op_array),
                 &(compiler_globals.function_table),
                 sizeof(compiler_globals.function_table),
                 &(compiler_globals.class_table),
                 sizeof(compiler_globals.class_table),
                 &(compiler_globals.auto_globals),
                 sizeof(compiler_globals.auto_globals),
                 &(compiler_globals.in_compilation),
                 sizeof(compiler_globals.in_compilation),
                 &(compiler_globals.short_tags),
                 sizeof(compiler_globals.short_tags),
                 &(compiler_globals.asp_tags),
                 sizeof(compiler_globals.asp_tags),
                 &(compiler_globals.unclean_shutdown),
                 sizeof(compiler_globals.unclean_shutdown),
                 &(compiler_globals.ini_parser_unbuffered_errors),
                 sizeof(compiler_globals.ini_parser_unbuffered_errors),
                 &(compiler_globals.catch_begin),
                 sizeof(compiler_globals.catch_begin),
                 &(compiler_globals.ini_parser_param),
                 sizeof(compiler_globals.ini_parser_param),
                 &(compiler_globals.interactive),
                 sizeof(compiler_globals.interactive),
                 &(compiler_globals.start_lineno),
                 sizeof(compiler_globals.start_lineno),
                 &(compiler_globals.increment_lineno),
                 sizeof(compiler_globals.increment_lineno),
                 &(compiler_globals.access_type),
                 sizeof(compiler_globals.access_type),
                 &(compiler_globals.doc_comment),
                 sizeof(compiler_globals.doc_comment),
                 &(compiler_globals.doc_comment_len),
                 sizeof(compiler_globals.doc_comment_len),
                 &(compiler_globals.compiler_options),
                 sizeof(compiler_globals.compiler_options),
                 &(compiler_globals.current_namespace),
                 sizeof(compiler_globals.current_namespace),
                 &(compiler_globals.current_import),
                 sizeof(compiler_globals.current_import),
                 &(compiler_globals.in_namespace),
                 sizeof(compiler_globals.in_namespace),
                 &(compiler_globals.has_bracketed_namespaces),
                 sizeof(compiler_globals.has_bracketed_namespaces),
                 &(compiler_globals.interned_strings_top),
                 sizeof(compiler_globals.interned_strings_top),
                 &(compiler_globals.interned_strings_snapshot_top),
                 sizeof(compiler_globals.interned_strings_snapshot_top),
                 &(compiler_globals.script_encoding_list),
                 sizeof(compiler_globals.script_encoding_list),
                 &(compiler_globals.script_encoding_list_size),
                 sizeof(compiler_globals.script_encoding_list_size),
                 &(compiler_globals.multibyte),
                 sizeof(compiler_globals.multibyte),
                 &(compiler_globals.detect_unicode),
                 sizeof(compiler_globals.detect_unicode),
                 &(compiler_globals.encoding_declared),
                 sizeof(compiler_globals.encoding_declared),
                 &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                 &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
                 sizeof((*op1).is_ref__gc),
                 &(compiler_globals.active_class_entry),
                 sizeof(compiler_globals.active_class_entry),
                 &(compiler_globals.compiled_filename),
                 sizeof(compiler_globals.compiled_filename),
                 &(compiler_globals.zend_lineno),
                 sizeof(compiler_globals.zend_lineno),
                 &(compiler_globals.heredoc), sizeof(compiler_globals.heredoc),
                 &(compiler_globals.heredoc_len),
                 sizeof(compiler_globals.heredoc_len),
                 &(compiler_globals.active_op_array),
                 sizeof(compiler_globals.active_op_array),
                 &(compiler_globals.function_table),
                 sizeof(compiler_globals.function_table),
                 &(compiler_globals.class_table),
                 sizeof(compiler_globals.class_table),
                 &(compiler_globals.auto_globals),
                 sizeof(compiler_globals.auto_globals),
                 &(compiler_globals.in_compilation),
                 sizeof(compiler_globals.in_compilation),
                 &(compiler_globals.short_tags),
                 sizeof(compiler_globals.short_tags),
                 &(compiler_globals.asp_tags),
                 sizeof(compiler_globals.asp_tags),
                 &(compiler_globals.unclean_shutdown),
                 sizeof(compiler_globals.unclean_shutdown),
                 &(compiler_globals.ini_parser_unbuffered_errors),
                 sizeof(compiler_globals.ini_parser_unbuffered_errors),
                 &(compiler_globals.catch_begin),
                 sizeof(compiler_globals.catch_begin),
                 &(compiler_globals.ini_parser_param),
                 sizeof(compiler_globals.ini_parser_param),
                 &(compiler_globals.interactive),
                 sizeof(compiler_globals.interactive),
                 &(compiler_globals.start_lineno),
                 sizeof(compiler_globals.start_lineno),
                 &(compiler_globals.increment_lineno),
                 sizeof(compiler_globals.increment_lineno),
                 &(compiler_globals.access_type),
                 sizeof(compiler_globals.access_type),
                 &(compiler_globals.doc_comment),
                 sizeof(compiler_globals.doc_comment),
                 &(compiler_globals.doc_comment_len),
                 sizeof(compiler_globals.doc_comment_len),
                 &(compiler_globals.compiler_options),
                 sizeof(compiler_globals.compiler_options),
                 &(compiler_globals.current_namespace),
                 sizeof(compiler_globals.current_namespace),
                 &(compiler_globals.current_import),
                 sizeof(compiler_globals.current_import),
                 &(compiler_globals.in_namespace),
                 sizeof(compiler_globals.in_namespace),
                 &(compiler_globals.has_bracketed_namespaces),
                 sizeof(compiler_globals.has_bracketed_namespaces),
                 &(compiler_globals.interned_strings_top),
                 sizeof(compiler_globals.interned_strings_top),
                 &(compiler_globals.interned_strings_snapshot_top),
                 sizeof(compiler_globals.interned_strings_snapshot_top),
                 &(compiler_globals.script_encoding_list),
                 sizeof(compiler_globals.script_encoding_list),
                 &(compiler_globals.script_encoding_list_size),
                 sizeof(compiler_globals.script_encoding_list_size),
                 &(compiler_globals.multibyte),
                 sizeof(compiler_globals.multibyte),
                 &(compiler_globals.detect_unicode),
                 sizeof(compiler_globals.detect_unicode),
                 &(compiler_globals.encoding_declared),
                 sizeof(compiler_globals.encoding_declared)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH204")) {
      case 0: {
        if (__temp201) {
          buf = (char *)emalloc(length + 1);
          memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
        } else {
          {
            switch (__choose("__SWITCH202")) {
            case 0:
              break;
#ifdef COMPILE_20038
            case 1: {
              // AddAndReplaceKind
              (*(result)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20039
            case 2: {
              // AddAndReplaceKind
              (*(result)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20040
            case 3: {
              // AddAndReplaceKind
              (*(result)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20041
            case 4: {
              // AddAndReplaceKind
              (buf) = (char *)_emalloc((length + 1));

              break;
            }
#endif
#ifdef COMPILE_20042
            case 5: {
              // AddAndReplaceKind
              (buf) =
                  (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20043
            case 6: {
              // AddAndReplaceKind
              (buf)++;

              break;
            }
#endif
#ifdef COMPILE_20044
            case 7: {
              // AddAndReplaceKind
              (buf)[length - 1] = (char)(*op2).value.lval;

              break;
            }
#endif
#ifdef COMPILE_20045
            case 8: {
              // AddAndReplaceKind
              (buf)[length] = 0;

              break;
            }
#endif
#ifdef COMPILE_20046
            case 9: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_20047
            case 10: {
              // AddAndReplaceKind
              _convert_to_string(((result)));

              break;
            }
#endif
#ifdef COMPILE_20048
            case 11: {
              // AddAndReplaceKind
              _efree(((*(result)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20049
            case 12: {
              // AddAndReplaceKind
              _efree(((buf)));

              break;
            }
#endif
#ifdef COMPILE_20050
            case 13: {
              // AddAndReplaceKind
              _efree(((result)));

              break;
            }
#endif
#ifdef COMPILE_20051
            case 14: {
              // AddAndReplaceKind
              _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20052
            case 15: {
              // AddAndReplaceKind
              _zend_list_delete((*(result)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20053
            case 16: {
              // AddAndReplaceKind
              _zval_copy_ctor(((result)));

              break;
            }
#endif
#ifdef COMPILE_20054
            case 17: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((result));

              break;
            }
#endif
#ifdef COMPILE_20055
            case 18: {
              // AddAndReplaceKind
              _zval_dtor(((result)));

              break;
            }
#endif
#ifdef COMPILE_20056
            case 19: {
              // AddAndReplaceKind
              _zval_dtor_func((result));

              break;
            }
#endif
#ifdef COMPILE_20057
            case 20: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(result)));

              break;
            }
#endif
#ifdef COMPILE_20058
            case 21: {
              // AddAndReplaceKind
              buf = (char *)_emalloc((length + 1));

              break;
            }
#endif
#ifdef COMPILE_20059
            case 22: {
              // AddAndReplaceKind
              buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20060
            case 23: {
              // AddAndReplaceKind
              buf[length - 1] = (char)(*op2).value.lval;

              break;
            }
#endif
#ifdef COMPILE_20061
            case 24: {
              // AddAndReplaceKind
              buf[length] = 0;

              break;
            }
#endif
#ifdef COMPILE_20062
            case 25: {
              // AddAndReplaceKind
              convert_scalar_to_array((result), 4);

              break;
            }
#endif
#ifdef COMPILE_20063
            case 26: {
              // AddAndReplaceKind
              convert_scalar_to_number((result));

              break;
            }
#endif
#ifdef COMPILE_20064
            case 27: {
              // AddAndReplaceKind
              convert_to_array((result));

              break;
            }
#endif
#ifdef COMPILE_20065
            case 28: {
              // AddAndReplaceKind
              convert_to_boolean((result));

              break;
            }
#endif
#ifdef COMPILE_20066
            case 29: {
              // AddAndReplaceKind
              convert_to_double((result));

              break;
            }
#endif
#ifdef COMPILE_20067
            case 30: {
              // AddAndReplaceKind
              convert_to_long((result));

              break;
            }
#endif
#ifdef COMPILE_20068
            case 31: {
              // AddAndReplaceKind
              convert_to_long_base((result), 10);

              break;
            }
#endif
#ifdef COMPILE_20069
            case 32: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_20070
            case 33: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((result));

              break;
            }
#endif
#ifdef COMPILE_20071
            case 34: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((result)));

              break;
            }
#endif
#ifdef COMPILE_20072
            case 35: {
              // AddAndReplaceKind
              gc_zval_possible_root((result));

              break;
            }
#endif
#ifdef COMPILE_20073
            case 36: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((result));

              break;
            }
#endif
#ifdef COMPILE_20074
            case 37: {
              // AddAndReplaceKind
              if (((((*op1).value.str.val) >=
                    (compiler_globals.interned_strings_start)) &&
                   (((*op1).value.str.val) <
                    (compiler_globals.interned_strings_end)))) {
                buf = (char *)_emalloc((length + 1));
                memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);
              } else {
                buf =
                    (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);
              }

              break;
            }
#endif
#ifdef COMPILE_20075
            case 38: {
              // AddAndReplaceKind
              memcpy((buf), (*op1).value.str.val, (*op1).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20076
            case 39: {
              // AddAndReplaceKind
              memcpy(buf, (*(op1)).value.str.val, (*op1).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20077
            case 40: {
              // AddAndReplaceKind
              memcpy(buf, (*(op2)).value.str.val, (*op1).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20078
            case 41: {
              // AddAndReplaceKind
              memcpy(buf, (*op1).value.str.val, (*(op1)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20079
            case 42: {
              // AddAndReplaceKind
              memcpy(buf, (*op1).value.str.val, (*(op2)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20080
            case 43: {
              // AddAndReplaceKind
              memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20081
            case 44: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "String size overflow");

              break;
            }
#endif
#ifdef COMPILE_20082
            case 45: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Unsupported operand types");

              break;
            }
#endif
#ifdef COMPILE_20083
            case 46: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20084
            case 47: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20085
            case 48: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20086
            case 49: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20087
            case 50: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20088
            case 51: {
              // AddAndReplaceKind
              zend_error((1 << 1L), "Division by zero");

              break;
            }
#endif
#ifdef COMPILE_20089
            case 52: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_20090
            case 53: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20091
            case 54: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20092
            case 55: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20093
            case 56: {
              // AddAndReplaceKind
              zend_free_obj_get_result((result));

              break;
            }
#endif
#ifdef COMPILE_20094
            case 57: {
              // AddAndReplaceKind
              zval_unset_isref_p((result));

              break;
            }
#endif
#ifdef COMPILE_20095
            case 58: {
              // AddInitKind
              memset(&(*op1), 0, sizeof(*(&(*op1))));

              break;
            }
#endif
#ifdef COMPILE_20096
            case 59: {
              // AddInitKind
              memset(&(*op1).value, 0, sizeof(*(&(*op1).value)));

              break;
            }
#endif
#ifdef COMPILE_20097
            case 60: {
              // AddInitKind
              memset(&(*op1).value.str, 0, sizeof(*(&(*op1).value.str)));

              break;
            }
#endif
#ifdef COMPILE_20098
            case 61: {
              // IfExitKind
              if (__is_neg(
                      "202-61", 10, &(op1), sizeof(op1), &(length),
                      sizeof(length), &(buf), sizeof(buf),
                      &((*op1).value.str.val), sizeof((*op1).value.str.val),
                      &(op2), sizeof(op2), &((*op1).value.str.len),
                      sizeof((*op1).value.str.len), &(result), sizeof(result),
                      &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                      &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
                      sizeof((*op1).is_ref__gc)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_20099
            case 62: {
              // IfExitKind
              if (__is_neg(
                      "202-62", 10, &(op1), sizeof(op1), &(length),
                      sizeof(length), &(buf), sizeof(buf),
                      &((*op1).value.str.val), sizeof((*op1).value.str.val),
                      &(op2), sizeof(op2), &((*op1).value.str.len),
                      sizeof((*op1).value.str.len), &(result), sizeof(result),
                      &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                      &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
                      sizeof((*op1).is_ref__gc)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_20100
            case 63: {
              // IfExitKind
              if (__is_neg(
                      "202-63", 10, &(op1), sizeof(op1), &(length),
                      sizeof(length), &(buf), sizeof(buf),
                      &((*op1).value.str.val), sizeof((*op1).value.str.val),
                      &(op2), sizeof(op2), &((*op1).value.str.len),
                      sizeof((*op1).value.str.len), &(result), sizeof(result),
                      &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                      &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
                      sizeof((*op1).is_ref__gc)))
                return 6;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH203")) {
            case 0: {
              buf = (char *)erealloc(Z_STRVAL_P(op1), length + 1);

              break;
            }
#ifdef COMPILE_20101
            case 1: {
              // GuardKind
              if (!__is_neg(
                      "203-1", 10, &(op1), sizeof(op1), &(length),
                      sizeof(length), &(buf), sizeof(buf),
                      &((*op1).value.str.val), sizeof((*op1).value.str.val),
                      &(op2), sizeof(op2), &((*op1).value.str.len),
                      sizeof((*op1).value.str.len), &(result), sizeof(result),
                      &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                      &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
                      sizeof((*op1).is_ref__gc)))
                buf =
                    (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20102
            case 2: {
              // ReplaceKind
              (buf) =
                  (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20103
            case 3: {
              // ReplaceKind
              buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

              break;
            }
#endif
            }
          }
        }

        break;
      }
#ifdef COMPILE_20104
      case 1: {
        // GuardKind
        if (!__is_neg("204-1", 10, &(op1), sizeof(op1), &(length),
                      sizeof(length), &(buf), sizeof(buf),
                      &((*op1).value.str.val), sizeof((*op1).value.str.val),
                      &(op2), sizeof(op2), &((*op1).value.str.len),
                      sizeof((*op1).value.str.len), &(result), sizeof(result),
                      &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                      &((*op1).type), sizeof((*op1).type), &((*op1).is_ref__gc),
                      sizeof((*op1).is_ref__gc)))
          buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

        break;
      }
#endif
#ifdef COMPILE_20105
      case 2: {
        // ReplaceKind
        (buf) = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

        break;
      }
#endif
#ifdef COMPILE_20106
      case 3: {
        // ReplaceKind
        buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

        break;
      }
#endif
      }
    }
  }
  // prophet generated patch
  {
    switch (__choose("__SWITCH205")) {
    case 0:
      break;
#ifdef COMPILE_20107
    case 1: {
      // AddAndReplaceKind
      (*(result)).value.lval++;

      break;
    }
#endif
#ifdef COMPILE_20108
    case 2: {
      // AddAndReplaceKind
      (*(result)).value.lval--;

      break;
    }
#endif
#ifdef COMPILE_20109
    case 3: {
      // AddAndReplaceKind
      (*(result)).value.str.len++;

      break;
    }
#endif
#ifdef COMPILE_20110
    case 4: {
      // AddAndReplaceKind
      (buf) = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_20111
    case 5: {
      // AddAndReplaceKind
      (buf) = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_20112
    case 6: {
      // AddAndReplaceKind
      (buf)++;

      break;
    }
#endif
#ifdef COMPILE_20113
    case 7: {
      // AddAndReplaceKind
      (buf)[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20114
    case 8: {
      // AddAndReplaceKind
      (buf)[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20115
    case 9: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_20116
    case 10: {
      // AddAndReplaceKind
      _convert_to_string(((result)));

      break;
    }
#endif
#ifdef COMPILE_20117
    case 11: {
      // AddAndReplaceKind
      _efree(((*(result)).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_20118
    case 12: {
      // AddAndReplaceKind
      _efree(((buf)));

      break;
    }
#endif
#ifdef COMPILE_20119
    case 13: {
      // AddAndReplaceKind
      _efree(((result)));

      break;
    }
#endif
#ifdef COMPILE_20120
    case 14: {
      // AddAndReplaceKind
      _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_20121
    case 15: {
      // AddAndReplaceKind
      _zend_list_delete((*(result)).value.lval);

      break;
    }
#endif
#ifdef COMPILE_20122
    case 16: {
      // AddAndReplaceKind
      _zval_copy_ctor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20123
    case 17: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20124
    case 18: {
      // AddAndReplaceKind
      _zval_dtor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20125
    case 19: {
      // AddAndReplaceKind
      _zval_dtor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20126
    case 20: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(result)));

      break;
    }
#endif
#ifdef COMPILE_20127
    case 21: {
      // AddAndReplaceKind
      buf = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_20128
    case 22: {
      // AddAndReplaceKind
      buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_20129
    case 23: {
      // AddAndReplaceKind
      buf[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20130
    case 24: {
      // AddAndReplaceKind
      buf[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20131
    case 25: {
      // AddAndReplaceKind
      convert_scalar_to_array((result), 4);

      break;
    }
#endif
#ifdef COMPILE_20132
    case 26: {
      // AddAndReplaceKind
      convert_scalar_to_number((result));

      break;
    }
#endif
#ifdef COMPILE_20133
    case 27: {
      // AddAndReplaceKind
      convert_to_array((result));

      break;
    }
#endif
#ifdef COMPILE_20134
    case 28: {
      // AddAndReplaceKind
      convert_to_boolean((result));

      break;
    }
#endif
#ifdef COMPILE_20135
    case 29: {
      // AddAndReplaceKind
      convert_to_double((result));

      break;
    }
#endif
#ifdef COMPILE_20136
    case 30: {
      // AddAndReplaceKind
      convert_to_long((result));

      break;
    }
#endif
#ifdef COMPILE_20137
    case 31: {
      // AddAndReplaceKind
      convert_to_long_base((result), 10);

      break;
    }
#endif
#ifdef COMPILE_20138
    case 32: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_20139
    case 33: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((result));

      break;
    }
#endif
#ifdef COMPILE_20140
    case 34: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((result)));

      break;
    }
#endif
#ifdef COMPILE_20141
    case 35: {
      // AddAndReplaceKind
      gc_zval_possible_root((result));

      break;
    }
#endif
#ifdef COMPILE_20142
    case 36: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((result));

      break;
    }
#endif
#ifdef COMPILE_20143
    case 37: {
      // AddAndReplaceKind
      if (((((*op1).value.str.val) >=
            (compiler_globals.interned_strings_start)) &&
           (((*op1).value.str.val) <
            (compiler_globals.interned_strings_end)))) {
        buf = (char *)_emalloc((length + 1));
        memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);
      } else {
        buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);
      }

      break;
    }
#endif
#ifdef COMPILE_20144
    case 38: {
      // AddAndReplaceKind
      memcpy((buf), (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20145
    case 39: {
      // AddAndReplaceKind
      memcpy(buf, (*(op1)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20146
    case 40: {
      // AddAndReplaceKind
      memcpy(buf, (*(op2)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20147
    case 41: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op1)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20148
    case 42: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op2)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20149
    case 43: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20150
    case 44: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "String size overflow");

      break;
    }
#endif
#ifdef COMPILE_20151
    case 45: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Unsupported operand types");

      break;
    }
#endif
#ifdef COMPILE_20152
    case 46: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(1));

      break;
    }
#endif
#ifdef COMPILE_20153
    case 47: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(2));

      break;
    }
#endif
#ifdef COMPILE_20154
    case 48: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(3));

      break;
    }
#endif
#ifdef COMPILE_20155
    case 49: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(4));

      break;
    }
#endif
#ifdef COMPILE_20156
    case 50: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(6));

      break;
    }
#endif
#ifdef COMPILE_20157
    case 51: {
      // AddAndReplaceKind
      zend_error((1 << 1L), "Division by zero");

      break;
    }
#endif
#ifdef COMPILE_20158
    case 52: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_20159
    case 53: {
      // AddAndReplaceKind
      zend_error((1 << 3L),
                 "Object of class %s could not be converted to double",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20160
    case 54: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s could not be converted to int",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20161
    case 55: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s to string conversion",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20162
    case 56: {
      // AddAndReplaceKind
      zend_free_obj_get_result((result));

      break;
    }
#endif
#ifdef COMPILE_20163
    case 57: {
      // AddAndReplaceKind
      zval_unset_isref_p((result));

      break;
    }
#endif
#ifdef COMPILE_20164
    case 58: {
      // AddInitKind
      memset(&(*op2), 0, sizeof(*(&(*op2))));

      break;
    }
#endif
#ifdef COMPILE_20165
    case 59: {
      // AddInitKind
      memset(&(*op2).value, 0, sizeof(*(&(*op2).value)));

      break;
    }
#endif
#ifdef COMPILE_20166
    case 60: {
      // IfExitKind
      if (__is_neg("205-60", 8, &(op2), sizeof(op2), &(length), sizeof(length),
                   &(buf), sizeof(buf), &(result), sizeof(result), &(op1),
                   sizeof(op1), &((*op2).refcount__gc),
                   sizeof((*op2).refcount__gc), &((*op2).type),
                   sizeof((*op2).type), &((*op2).is_ref__gc),
                   sizeof((*op2).is_ref__gc)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_20167
    case 61: {
      // IfExitKind
      if (__is_neg("205-61", 8, &(op2), sizeof(op2), &(length), sizeof(length),
                   &(buf), sizeof(buf), &(result), sizeof(result), &(op1),
                   sizeof(op1), &((*op2).refcount__gc),
                   sizeof((*op2).refcount__gc), &((*op2).type),
                   sizeof((*op2).type), &((*op2).is_ref__gc),
                   sizeof((*op2).is_ref__gc)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_20168
    case 62: {
      // IfExitKind
      if (__is_neg("205-62", 8, &(op2), sizeof(op2), &(length), sizeof(length),
                   &(buf), sizeof(buf), &(result), sizeof(result), &(op1),
                   sizeof(op1), &((*op2).refcount__gc),
                   sizeof((*op2).refcount__gc), &((*op2).type),
                   sizeof((*op2).type), &((*op2).is_ref__gc),
                   sizeof((*op2).is_ref__gc)))
        return 6;

      break;
    }
#endif
    }
    switch (__choose("__SWITCH206")) {
    case 0: {
      buf[length - 1] = (char)Z_LVAL_P(op2);

      break;
    }
#ifdef COMPILE_20169
    case 1: {
      // GuardKind
      if (!__is_neg("206-1", 8, &(op2), sizeof(op2), &(length), sizeof(length),
                    &(buf), sizeof(buf), &(result), sizeof(result), &(op1),
                    sizeof(op1), &((*op2).refcount__gc),
                    sizeof((*op2).refcount__gc), &((*op2).type),
                    sizeof((*op2).type), &((*op2).is_ref__gc),
                    sizeof((*op2).is_ref__gc)))
        buf[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20170
    case 2: {
      // ReplaceKind
      (buf)[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20171
    case 3: {
      // ReplaceKind
      buf[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
    }
  }
  // prophet generated patch
  {
    switch (__choose("__SWITCH207")) {
    case 0:
      break;
#ifdef COMPILE_20172
    case 1: {
      // AddAndReplaceKind
      (*(result)).value.lval++;

      break;
    }
#endif
#ifdef COMPILE_20173
    case 2: {
      // AddAndReplaceKind
      (*(result)).value.lval--;

      break;
    }
#endif
#ifdef COMPILE_20174
    case 3: {
      // AddAndReplaceKind
      (*(result)).value.str.len++;

      break;
    }
#endif
#ifdef COMPILE_20175
    case 4: {
      // AddAndReplaceKind
      (buf) = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_20176
    case 5: {
      // AddAndReplaceKind
      (buf) = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_20177
    case 6: {
      // AddAndReplaceKind
      (buf)++;

      break;
    }
#endif
#ifdef COMPILE_20178
    case 7: {
      // AddAndReplaceKind
      (buf)[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20179
    case 8: {
      // AddAndReplaceKind
      (buf)[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20180
    case 9: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_20181
    case 10: {
      // AddAndReplaceKind
      _convert_to_string(((result)));

      break;
    }
#endif
#ifdef COMPILE_20182
    case 11: {
      // AddAndReplaceKind
      _efree(((*(result)).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_20183
    case 12: {
      // AddAndReplaceKind
      _efree(((buf)));

      break;
    }
#endif
#ifdef COMPILE_20184
    case 13: {
      // AddAndReplaceKind
      _efree(((result)));

      break;
    }
#endif
#ifdef COMPILE_20185
    case 14: {
      // AddAndReplaceKind
      _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_20186
    case 15: {
      // AddAndReplaceKind
      _zend_list_delete((*(result)).value.lval);

      break;
    }
#endif
#ifdef COMPILE_20187
    case 16: {
      // AddAndReplaceKind
      _zval_copy_ctor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20188
    case 17: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20189
    case 18: {
      // AddAndReplaceKind
      _zval_dtor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20190
    case 19: {
      // AddAndReplaceKind
      _zval_dtor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20191
    case 20: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(result)));

      break;
    }
#endif
#ifdef COMPILE_20192
    case 21: {
      // AddAndReplaceKind
      buf = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_20193
    case 22: {
      // AddAndReplaceKind
      buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_20194
    case 23: {
      // AddAndReplaceKind
      buf[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20195
    case 24: {
      // AddAndReplaceKind
      buf[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20196
    case 25: {
      // AddAndReplaceKind
      convert_scalar_to_array((result), 4);

      break;
    }
#endif
#ifdef COMPILE_20197
    case 26: {
      // AddAndReplaceKind
      convert_scalar_to_number((result));

      break;
    }
#endif
#ifdef COMPILE_20198
    case 27: {
      // AddAndReplaceKind
      convert_to_array((result));

      break;
    }
#endif
#ifdef COMPILE_20199
    case 28: {
      // AddAndReplaceKind
      convert_to_boolean((result));

      break;
    }
#endif
#ifdef COMPILE_20200
    case 29: {
      // AddAndReplaceKind
      convert_to_double((result));

      break;
    }
#endif
#ifdef COMPILE_20201
    case 30: {
      // AddAndReplaceKind
      convert_to_long((result));

      break;
    }
#endif
#ifdef COMPILE_20202
    case 31: {
      // AddAndReplaceKind
      convert_to_long_base((result), 10);

      break;
    }
#endif
#ifdef COMPILE_20203
    case 32: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_20204
    case 33: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((result));

      break;
    }
#endif
#ifdef COMPILE_20205
    case 34: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((result)));

      break;
    }
#endif
#ifdef COMPILE_20206
    case 35: {
      // AddAndReplaceKind
      gc_zval_possible_root((result));

      break;
    }
#endif
#ifdef COMPILE_20207
    case 36: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((result));

      break;
    }
#endif
#ifdef COMPILE_20208
    case 37: {
      // AddAndReplaceKind
      if (((((*op1).value.str.val) >=
            (compiler_globals.interned_strings_start)) &&
           (((*op1).value.str.val) <
            (compiler_globals.interned_strings_end)))) {
        buf = (char *)_emalloc((length + 1));
        memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);
      } else {
        buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);
      }

      break;
    }
#endif
#ifdef COMPILE_20209
    case 38: {
      // AddAndReplaceKind
      memcpy((buf), (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20210
    case 39: {
      // AddAndReplaceKind
      memcpy(buf, (*(op1)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20211
    case 40: {
      // AddAndReplaceKind
      memcpy(buf, (*(op2)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20212
    case 41: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op1)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20213
    case 42: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op2)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20214
    case 43: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20215
    case 44: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "String size overflow");

      break;
    }
#endif
#ifdef COMPILE_20216
    case 45: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Unsupported operand types");

      break;
    }
#endif
#ifdef COMPILE_20217
    case 46: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(1));

      break;
    }
#endif
#ifdef COMPILE_20218
    case 47: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(2));

      break;
    }
#endif
#ifdef COMPILE_20219
    case 48: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(3));

      break;
    }
#endif
#ifdef COMPILE_20220
    case 49: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(4));

      break;
    }
#endif
#ifdef COMPILE_20221
    case 50: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(6));

      break;
    }
#endif
#ifdef COMPILE_20222
    case 51: {
      // AddAndReplaceKind
      zend_error((1 << 1L), "Division by zero");

      break;
    }
#endif
#ifdef COMPILE_20223
    case 52: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_20224
    case 53: {
      // AddAndReplaceKind
      zend_error((1 << 3L),
                 "Object of class %s could not be converted to double",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20225
    case 54: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s could not be converted to int",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20226
    case 55: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s to string conversion",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20227
    case 56: {
      // AddAndReplaceKind
      zend_free_obj_get_result((result));

      break;
    }
#endif
#ifdef COMPILE_20228
    case 57: {
      // AddAndReplaceKind
      zval_unset_isref_p((result));

      break;
    }
#endif
#ifdef COMPILE_20229
    case 58: {
      // IfExitKind
      if (__is_neg("207-58", 8, &(length), sizeof(length), &(buf), sizeof(buf),
                   &(result), sizeof(result), &(op2), sizeof(op2), &(op1),
                   sizeof(op1), &((*op2).refcount__gc),
                   sizeof((*op2).refcount__gc), &((*op2).type),
                   sizeof((*op2).type), &((*op2).is_ref__gc),
                   sizeof((*op2).is_ref__gc)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_20230
    case 59: {
      // IfExitKind
      if (__is_neg("207-59", 8, &(length), sizeof(length), &(buf), sizeof(buf),
                   &(result), sizeof(result), &(op2), sizeof(op2), &(op1),
                   sizeof(op1), &((*op2).refcount__gc),
                   sizeof((*op2).refcount__gc), &((*op2).type),
                   sizeof((*op2).type), &((*op2).is_ref__gc),
                   sizeof((*op2).is_ref__gc)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_20231
    case 60: {
      // IfExitKind
      if (__is_neg("207-60", 8, &(length), sizeof(length), &(buf), sizeof(buf),
                   &(result), sizeof(result), &(op2), sizeof(op2), &(op1),
                   sizeof(op1), &((*op2).refcount__gc),
                   sizeof((*op2).refcount__gc), &((*op2).type),
                   sizeof((*op2).type), &((*op2).is_ref__gc),
                   sizeof((*op2).is_ref__gc)))
        return 6;

      break;
    }
#endif
    }
    switch (__choose("__SWITCH208")) {
    case 0: {
      buf[length] = 0;

      break;
    }
#ifdef COMPILE_20232
    case 1: {
      // GuardKind
      if (!__is_neg("208-1", 8, &(length), sizeof(length), &(buf), sizeof(buf),
                    &(result), sizeof(result), &(op2), sizeof(op2), &(op1),
                    sizeof(op1), &((*op2).refcount__gc),
                    sizeof((*op2).refcount__gc), &((*op2).type),
                    sizeof((*op2).type), &((*op2).is_ref__gc),
                    sizeof((*op2).is_ref__gc)))
        buf[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20233
    case 2: {
      // ReplaceKind
      (buf)[length] = 0;

      break;
    }
#endif
    }
  }
  // prophet generated patch
  {
    switch (__choose("__SWITCH209")) {
    case 0:
      break;
#ifdef COMPILE_20234
    case 1: {
      // AddAndReplaceKind
      (*(result)).value.lval++;

      break;
    }
#endif
#ifdef COMPILE_20235
    case 2: {
      // AddAndReplaceKind
      (*(result)).value.lval--;

      break;
    }
#endif
#ifdef COMPILE_20236
    case 3: {
      // AddAndReplaceKind
      (*(result)).value.str.len++;

      break;
    }
#endif
#ifdef COMPILE_20237
    case 4: {
      // AddAndReplaceKind
      (buf) = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_20238
    case 5: {
      // AddAndReplaceKind
      (buf) = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_20239
    case 6: {
      // AddAndReplaceKind
      (buf)++;

      break;
    }
#endif
#ifdef COMPILE_20240
    case 7: {
      // AddAndReplaceKind
      (buf)[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20241
    case 8: {
      // AddAndReplaceKind
      (buf)[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20242
    case 9: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_20243
    case 10: {
      // AddAndReplaceKind
      _convert_to_string(((result)));

      break;
    }
#endif
#ifdef COMPILE_20244
    case 11: {
      // AddAndReplaceKind
      _efree(((*(result)).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_20245
    case 12: {
      // AddAndReplaceKind
      _efree(((buf)));

      break;
    }
#endif
#ifdef COMPILE_20246
    case 13: {
      // AddAndReplaceKind
      _efree(((result)));

      break;
    }
#endif
#ifdef COMPILE_20247
    case 14: {
      // AddAndReplaceKind
      _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_20248
    case 15: {
      // AddAndReplaceKind
      _zend_list_delete((*(result)).value.lval);

      break;
    }
#endif
#ifdef COMPILE_20249
    case 16: {
      // AddAndReplaceKind
      _zval_copy_ctor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20250
    case 17: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20251
    case 18: {
      // AddAndReplaceKind
      _zval_dtor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20252
    case 19: {
      // AddAndReplaceKind
      _zval_dtor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20253
    case 20: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(result)));

      break;
    }
#endif
#ifdef COMPILE_20254
    case 21: {
      // AddAndReplaceKind
      buf = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_20255
    case 22: {
      // AddAndReplaceKind
      buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_20256
    case 23: {
      // AddAndReplaceKind
      buf[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20257
    case 24: {
      // AddAndReplaceKind
      buf[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20258
    case 25: {
      // AddAndReplaceKind
      convert_scalar_to_array((result), 4);

      break;
    }
#endif
#ifdef COMPILE_20259
    case 26: {
      // AddAndReplaceKind
      convert_scalar_to_number((result));

      break;
    }
#endif
#ifdef COMPILE_20260
    case 27: {
      // AddAndReplaceKind
      convert_to_array((result));

      break;
    }
#endif
#ifdef COMPILE_20261
    case 28: {
      // AddAndReplaceKind
      convert_to_boolean((result));

      break;
    }
#endif
#ifdef COMPILE_20262
    case 29: {
      // AddAndReplaceKind
      convert_to_double((result));

      break;
    }
#endif
#ifdef COMPILE_20263
    case 30: {
      // AddAndReplaceKind
      convert_to_long((result));

      break;
    }
#endif
#ifdef COMPILE_20264
    case 31: {
      // AddAndReplaceKind
      convert_to_long_base((result), 10);

      break;
    }
#endif
#ifdef COMPILE_20265
    case 32: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_20266
    case 33: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((result));

      break;
    }
#endif
#ifdef COMPILE_20267
    case 34: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((result)));

      break;
    }
#endif
#ifdef COMPILE_20268
    case 35: {
      // AddAndReplaceKind
      gc_zval_possible_root((result));

      break;
    }
#endif
#ifdef COMPILE_20269
    case 36: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((result));

      break;
    }
#endif
#ifdef COMPILE_20270
    case 37: {
      // AddAndReplaceKind
      if (((((*op1).value.str.val) >=
            (compiler_globals.interned_strings_start)) &&
           (((*op1).value.str.val) <
            (compiler_globals.interned_strings_end)))) {
        buf = (char *)_emalloc((length + 1));
        memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);
      } else {
        buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);
      }

      break;
    }
#endif
#ifdef COMPILE_20271
    case 38: {
      // AddAndReplaceKind
      memcpy((buf), (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20272
    case 39: {
      // AddAndReplaceKind
      memcpy(buf, (*(op1)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20273
    case 40: {
      // AddAndReplaceKind
      memcpy(buf, (*(op2)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20274
    case 41: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op1)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20275
    case 42: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op2)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20276
    case 43: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20277
    case 44: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "String size overflow");

      break;
    }
#endif
#ifdef COMPILE_20278
    case 45: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Unsupported operand types");

      break;
    }
#endif
#ifdef COMPILE_20279
    case 46: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(1));

      break;
    }
#endif
#ifdef COMPILE_20280
    case 47: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(2));

      break;
    }
#endif
#ifdef COMPILE_20281
    case 48: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(3));

      break;
    }
#endif
#ifdef COMPILE_20282
    case 49: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(4));

      break;
    }
#endif
#ifdef COMPILE_20283
    case 50: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(6));

      break;
    }
#endif
#ifdef COMPILE_20284
    case 51: {
      // AddAndReplaceKind
      zend_error((1 << 1L), "Division by zero");

      break;
    }
#endif
#ifdef COMPILE_20285
    case 52: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_20286
    case 53: {
      // AddAndReplaceKind
      zend_error((1 << 3L),
                 "Object of class %s could not be converted to double",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20287
    case 54: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s could not be converted to int",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20288
    case 55: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s to string conversion",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20289
    case 56: {
      // AddAndReplaceKind
      zend_free_obj_get_result((result));

      break;
    }
#endif
#ifdef COMPILE_20290
    case 57: {
      // AddAndReplaceKind
      zval_unset_isref_p((result));

      break;
    }
#endif
#ifdef COMPILE_20291
    case 58: {
      // IfExitKind
      if (__is_neg("209-58", 5, &(result), sizeof(result), &(length),
                   sizeof(length), &(buf), sizeof(buf), &(op2), sizeof(op2),
                   &(op1), sizeof(op1)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_20292
    case 59: {
      // IfExitKind
      if (__is_neg("209-59", 5, &(result), sizeof(result), &(length),
                   sizeof(length), &(buf), sizeof(buf), &(op2), sizeof(op2),
                   &(op1), sizeof(op1)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_20293
    case 60: {
      // IfExitKind
      if (__is_neg("209-60", 5, &(result), sizeof(result), &(length),
                   sizeof(length), &(buf), sizeof(buf), &(op2), sizeof(op2),
                   &(op1), sizeof(op1)))
        return 6;

      break;
    }
#endif
    }
    switch (__choose("__SWITCH210")) {
    case 0: {
      ZVAL_STRINGL(result, buf, length, 0);

      break;
    }
#ifdef COMPILE_20294
    case 1: {
      // GuardKind
      if (!__is_neg("210-1", 5, &(result), sizeof(result), &(length),
                    sizeof(length), &(buf), sizeof(buf), &(op2), sizeof(op2),
                    &(op1), sizeof(op1)))
        do {
          const char *__s = (buf);
          int __l = length;
          zval *__z = (result);
          (*__z).value.str.len = __l;
          (*__z).value.str.val = (0 ? _estrndup((__s), (__l)) : (char *)__s);
          (*__z).type = 6;
        } while (0);

      break;
    }
#endif
    }
  }
  // prophet generated patch
  {
    switch (__choose("__SWITCH211")) {
    case 0:
      break;
#ifdef COMPILE_20295
    case 1: {
      // AddAndReplaceKind
      (*(result)).value.lval++;

      break;
    }
#endif
#ifdef COMPILE_20296
    case 2: {
      // AddAndReplaceKind
      (*(result)).value.lval--;

      break;
    }
#endif
#ifdef COMPILE_20297
    case 3: {
      // AddAndReplaceKind
      (*(result)).value.str.len++;

      break;
    }
#endif
#ifdef COMPILE_20298
    case 4: {
      // AddAndReplaceKind
      (buf) = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_20299
    case 5: {
      // AddAndReplaceKind
      (buf) = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_20300
    case 6: {
      // AddAndReplaceKind
      (buf)++;

      break;
    }
#endif
#ifdef COMPILE_20301
    case 7: {
      // AddAndReplaceKind
      (buf)[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20302
    case 8: {
      // AddAndReplaceKind
      (buf)[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20303
    case 9: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_20304
    case 10: {
      // AddAndReplaceKind
      _convert_to_string(((result)));

      break;
    }
#endif
#ifdef COMPILE_20305
    case 11: {
      // AddAndReplaceKind
      _efree(((*(result)).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_20306
    case 12: {
      // AddAndReplaceKind
      _efree(((buf)));

      break;
    }
#endif
#ifdef COMPILE_20307
    case 13: {
      // AddAndReplaceKind
      _efree(((result)));

      break;
    }
#endif
#ifdef COMPILE_20308
    case 14: {
      // AddAndReplaceKind
      _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_20309
    case 15: {
      // AddAndReplaceKind
      _zend_list_delete((*(result)).value.lval);

      break;
    }
#endif
#ifdef COMPILE_20310
    case 16: {
      // AddAndReplaceKind
      _zval_copy_ctor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20311
    case 17: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20312
    case 18: {
      // AddAndReplaceKind
      _zval_dtor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20313
    case 19: {
      // AddAndReplaceKind
      _zval_dtor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20314
    case 20: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(result)));

      break;
    }
#endif
#ifdef COMPILE_20315
    case 21: {
      // AddAndReplaceKind
      buf = (char *)_emalloc((length + 1));

      break;
    }
#endif
#ifdef COMPILE_20316
    case 22: {
      // AddAndReplaceKind
      buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);

      break;
    }
#endif
#ifdef COMPILE_20317
    case 23: {
      // AddAndReplaceKind
      buf[length - 1] = (char)(*op2).value.lval;

      break;
    }
#endif
#ifdef COMPILE_20318
    case 24: {
      // AddAndReplaceKind
      buf[length] = 0;

      break;
    }
#endif
#ifdef COMPILE_20319
    case 25: {
      // AddAndReplaceKind
      convert_scalar_to_array((result), 4);

      break;
    }
#endif
#ifdef COMPILE_20320
    case 26: {
      // AddAndReplaceKind
      convert_scalar_to_number((result));

      break;
    }
#endif
#ifdef COMPILE_20321
    case 27: {
      // AddAndReplaceKind
      convert_to_array((result));

      break;
    }
#endif
#ifdef COMPILE_20322
    case 28: {
      // AddAndReplaceKind
      convert_to_boolean((result));

      break;
    }
#endif
#ifdef COMPILE_20323
    case 29: {
      // AddAndReplaceKind
      convert_to_double((result));

      break;
    }
#endif
#ifdef COMPILE_20324
    case 30: {
      // AddAndReplaceKind
      convert_to_long((result));

      break;
    }
#endif
#ifdef COMPILE_20325
    case 31: {
      // AddAndReplaceKind
      convert_to_long_base((result), 10);

      break;
    }
#endif
#ifdef COMPILE_20326
    case 32: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_20327
    case 33: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((result));

      break;
    }
#endif
#ifdef COMPILE_20328
    case 34: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((result)));

      break;
    }
#endif
#ifdef COMPILE_20329
    case 35: {
      // AddAndReplaceKind
      gc_zval_possible_root((result));

      break;
    }
#endif
#ifdef COMPILE_20330
    case 36: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((result));

      break;
    }
#endif
#ifdef COMPILE_20331
    case 37: {
      // AddAndReplaceKind
      if (((((*op1).value.str.val) >=
            (compiler_globals.interned_strings_start)) &&
           (((*op1).value.str.val) <
            (compiler_globals.interned_strings_end)))) {
        buf = (char *)_emalloc((length + 1));
        memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);
      } else {
        buf = (char *)_erealloc(((*op1).value.str.val), (length + 1), 0);
      }

      break;
    }
#endif
#ifdef COMPILE_20332
    case 38: {
      // AddAndReplaceKind
      memcpy((buf), (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20333
    case 39: {
      // AddAndReplaceKind
      memcpy(buf, (*(op1)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20334
    case 40: {
      // AddAndReplaceKind
      memcpy(buf, (*(op2)).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20335
    case 41: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op1)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20336
    case 42: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*(op2)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20337
    case 43: {
      // AddAndReplaceKind
      memcpy(buf, (*op1).value.str.val, (*op1).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20338
    case 44: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "String size overflow");

      break;
    }
#endif
#ifdef COMPILE_20339
    case 45: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Unsupported operand types");

      break;
    }
#endif
#ifdef COMPILE_20340
    case 46: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(1));

      break;
    }
#endif
#ifdef COMPILE_20341
    case 47: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(2));

      break;
    }
#endif
#ifdef COMPILE_20342
    case 48: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(3));

      break;
    }
#endif
#ifdef COMPILE_20343
    case 49: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(4));

      break;
    }
#endif
#ifdef COMPILE_20344
    case 50: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(6));

      break;
    }
#endif
#ifdef COMPILE_20345
    case 51: {
      // AddAndReplaceKind
      zend_error((1 << 1L), "Division by zero");

      break;
    }
#endif
#ifdef COMPILE_20346
    case 52: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_20347
    case 53: {
      // AddAndReplaceKind
      zend_error((1 << 3L),
                 "Object of class %s could not be converted to double",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20348
    case 54: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s could not be converted to int",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20349
    case 55: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s to string conversion",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20350
    case 56: {
      // AddAndReplaceKind
      zend_free_obj_get_result((result));

      break;
    }
#endif
#ifdef COMPILE_20351
    case 57: {
      // AddAndReplaceKind
      zval_unset_isref_p((result));

      break;
    }
#endif
#ifdef COMPILE_20352
    case 58: {
      // IfExitKind
      if (__is_neg("211-58", 5, &(result), sizeof(result), &(length),
                   sizeof(length), &(buf), sizeof(buf), &(op2), sizeof(op2),
                   &(op1), sizeof(op1)))
        return 0;

      break;
    }
#endif
#ifdef COMPILE_20353
    case 59: {
      // IfExitKind
      if (__is_neg("211-59", 5, &(result), sizeof(result), &(length),
                   sizeof(length), &(buf), sizeof(buf), &(op2), sizeof(op2),
                   &(op1), sizeof(op1)))
        return 1;

      break;
    }
#endif
#ifdef COMPILE_20354
    case 60: {
      // IfExitKind
      if (__is_neg("211-60", 5, &(result), sizeof(result), &(length),
                   sizeof(length), &(buf), sizeof(buf), &(op2), sizeof(op2),
                   &(op1), sizeof(op1)))
        return 6;

      break;
    }
#endif
    }
    switch (__choose("__SWITCH212")) {
    case 0: {
      return SUCCESS;

      break;
    }
#ifdef COMPILE_20355
    case 1: {
      // GuardKind
      if (!__is_neg("212-1", 5, &(result), sizeof(result), &(length),
                    sizeof(length), &(buf), sizeof(buf), &(op2), sizeof(op2),
                    &(op1), sizeof(op1)))
        return 0;

      break;
    }
#endif
    }
  }
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
  // prophet generated patch
  {
    switch (__choose("__SWITCH213")) {
    case 0:
      break;
#ifdef COMPILE_20356
    case 1: {
      // AddAndReplaceKind
      (*(op1)).value.lval++;

      break;
    }
#endif
#ifdef COMPILE_20357
    case 2: {
      // AddAndReplaceKind
      (*(op1)).value.lval--;

      break;
    }
#endif
#ifdef COMPILE_20358
    case 3: {
      // AddAndReplaceKind
      (*(op1)).value.str.len++;

      break;
    }
#endif
#ifdef COMPILE_20359
    case 4: {
      // AddAndReplaceKind
      (*(op2)).value.lval++;

      break;
    }
#endif
#ifdef COMPILE_20360
    case 5: {
      // AddAndReplaceKind
      (*(op2)).value.lval--;

      break;
    }
#endif
#ifdef COMPILE_20361
    case 6: {
      // AddAndReplaceKind
      (*(op2)).value.str.len++;

      break;
    }
#endif
#ifdef COMPILE_20362
    case 7: {
      // AddAndReplaceKind
      (*(result)).value.lval++;

      break;
    }
#endif
#ifdef COMPILE_20363
    case 8: {
      // AddAndReplaceKind
      (*(result)).value.lval--;

      break;
    }
#endif
#ifdef COMPILE_20364
    case 9: {
      // AddAndReplaceKind
      (*(result)).value.str.len++;

      break;
    }
#endif
#ifdef COMPILE_20365
    case 10: {
      // AddAndReplaceKind
      (executor_globals.argument_stack) =
          zend_vm_stack_new_page(((16 * 1024) - 16));

      break;
    }
#endif
#ifdef COMPILE_20366
    case 11: {
      // AddAndReplaceKind
      (op1) = &op1_copy;

      break;
    }
#endif
#ifdef COMPILE_20367
    case 12: {
      // AddAndReplaceKind
      (op1) = &op2_copy;

      break;
    }
#endif
#ifdef COMPILE_20368
    case 13: {
      // AddAndReplaceKind
      (op2) = &op1_copy;

      break;
    }
#endif
#ifdef COMPILE_20369
    case 14: {
      // AddAndReplaceKind
      (op2) = &op2_copy;

      break;
    }
#endif
#ifdef COMPILE_20370
    case 15: {
      // AddAndReplaceKind
      (result) = &op1_copy;

      break;
    }
#endif
#ifdef COMPILE_20371
    case 16: {
      // AddAndReplaceKind
      (result) = &op2_copy;

      break;
    }
#endif
#ifdef COMPILE_20372
    case 17: {
      // AddAndReplaceKind
      _convert_to_string(((op1)));

      break;
    }
#endif
#ifdef COMPILE_20373
    case 18: {
      // AddAndReplaceKind
      _convert_to_string(((op2)));

      break;
    }
#endif
#ifdef COMPILE_20374
    case 19: {
      // AddAndReplaceKind
      _convert_to_string(((result)));

      break;
    }
#endif
#ifdef COMPILE_20375
    case 20: {
      // AddAndReplaceKind
      _efree(((*(op1)).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_20376
    case 21: {
      // AddAndReplaceKind
      _efree(((*(op2)).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_20377
    case 22: {
      // AddAndReplaceKind
      _efree(((*(result)).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_20378
    case 23: {
      // AddAndReplaceKind
      _efree(((*result).value.str.val));

      break;
    }
#endif
#ifdef COMPILE_20379
    case 24: {
      // AddAndReplaceKind
      _efree(((op1)));

      break;
    }
#endif
#ifdef COMPILE_20380
    case 25: {
      // AddAndReplaceKind
      _efree(((op2)));

      break;
    }
#endif
#ifdef COMPILE_20381
    case 26: {
      // AddAndReplaceKind
      _efree(((result)));

      break;
    }
#endif
#ifdef COMPILE_20382
    case 27: {
      // AddAndReplaceKind
      _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_20383
    case 28: {
      // AddAndReplaceKind
      _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_20384
    case 29: {
      // AddAndReplaceKind
      _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                      ((void (*)(void *))_zval_ptr_dtor), (0));

      break;
    }
#endif
#ifdef COMPILE_20385
    case 30: {
      // AddAndReplaceKind
      _zend_list_delete((*(op1)).value.lval);

      break;
    }
#endif
#ifdef COMPILE_20386
    case 31: {
      // AddAndReplaceKind
      _zend_list_delete((*(op2)).value.lval);

      break;
    }
#endif
#ifdef COMPILE_20387
    case 32: {
      // AddAndReplaceKind
      _zend_list_delete((*(result)).value.lval);

      break;
    }
#endif
#ifdef COMPILE_20388
    case 33: {
      // AddAndReplaceKind
      _zval_copy_ctor(((op1)));

      break;
    }
#endif
#ifdef COMPILE_20389
    case 34: {
      // AddAndReplaceKind
      _zval_copy_ctor(((op2)));

      break;
    }
#endif
#ifdef COMPILE_20390
    case 35: {
      // AddAndReplaceKind
      _zval_copy_ctor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20391
    case 36: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((op1));

      break;
    }
#endif
#ifdef COMPILE_20392
    case 37: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((op2));

      break;
    }
#endif
#ifdef COMPILE_20393
    case 38: {
      // AddAndReplaceKind
      _zval_copy_ctor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20394
    case 39: {
      // AddAndReplaceKind
      _zval_dtor(((op1)));

      break;
    }
#endif
#ifdef COMPILE_20395
    case 40: {
      // AddAndReplaceKind
      _zval_dtor(((op2)));

      break;
    }
#endif
#ifdef COMPILE_20396
    case 41: {
      // AddAndReplaceKind
      _zval_dtor(((result)));

      break;
    }
#endif
#ifdef COMPILE_20397
    case 42: {
      // AddAndReplaceKind
      _zval_dtor((op1));

      break;
    }
#endif
#ifdef COMPILE_20398
    case 43: {
      // AddAndReplaceKind
      _zval_dtor((op2));

      break;
    }
#endif
#ifdef COMPILE_20399
    case 44: {
      // AddAndReplaceKind
      _zval_dtor_func((op1));

      break;
    }
#endif
#ifdef COMPILE_20400
    case 45: {
      // AddAndReplaceKind
      _zval_dtor_func((op2));

      break;
    }
#endif
#ifdef COMPILE_20401
    case 46: {
      // AddAndReplaceKind
      _zval_dtor_func((result));

      break;
    }
#endif
#ifdef COMPILE_20402
    case 47: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(op1)));

      break;
    }
#endif
#ifdef COMPILE_20403
    case 48: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(op2)));

      break;
    }
#endif
#ifdef COMPILE_20404
    case 49: {
      // AddAndReplaceKind
      _zval_ptr_dtor((&(result)));

      break;
    }
#endif
#ifdef COMPILE_20405
    case 50: {
      // AddAndReplaceKind
      convert_scalar_to_array((op1), 4);

      break;
    }
#endif
#ifdef COMPILE_20406
    case 51: {
      // AddAndReplaceKind
      convert_scalar_to_array((op2), 4);

      break;
    }
#endif
#ifdef COMPILE_20407
    case 52: {
      // AddAndReplaceKind
      convert_scalar_to_array((result), 4);

      break;
    }
#endif
#ifdef COMPILE_20408
    case 53: {
      // AddAndReplaceKind
      convert_scalar_to_number((op1));

      break;
    }
#endif
#ifdef COMPILE_20409
    case 54: {
      // AddAndReplaceKind
      convert_scalar_to_number((op2));

      break;
    }
#endif
#ifdef COMPILE_20410
    case 55: {
      // AddAndReplaceKind
      convert_scalar_to_number((result));

      break;
    }
#endif
#ifdef COMPILE_20411
    case 56: {
      // AddAndReplaceKind
      convert_to_array((op1));

      break;
    }
#endif
#ifdef COMPILE_20412
    case 57: {
      // AddAndReplaceKind
      convert_to_array((op2));

      break;
    }
#endif
#ifdef COMPILE_20413
    case 58: {
      // AddAndReplaceKind
      convert_to_array((result));

      break;
    }
#endif
#ifdef COMPILE_20414
    case 59: {
      // AddAndReplaceKind
      convert_to_boolean((op1));

      break;
    }
#endif
#ifdef COMPILE_20415
    case 60: {
      // AddAndReplaceKind
      convert_to_boolean((op2));

      break;
    }
#endif
#ifdef COMPILE_20416
    case 61: {
      // AddAndReplaceKind
      convert_to_boolean((result));

      break;
    }
#endif
#ifdef COMPILE_20417
    case 62: {
      // AddAndReplaceKind
      convert_to_double((op1));

      break;
    }
#endif
#ifdef COMPILE_20418
    case 63: {
      // AddAndReplaceKind
      convert_to_double((op2));

      break;
    }
#endif
#ifdef COMPILE_20419
    case 64: {
      // AddAndReplaceKind
      convert_to_double((result));

      break;
    }
#endif
#ifdef COMPILE_20420
    case 65: {
      // AddAndReplaceKind
      convert_to_long((op1));

      break;
    }
#endif
#ifdef COMPILE_20421
    case 66: {
      // AddAndReplaceKind
      convert_to_long((op2));

      break;
    }
#endif
#ifdef COMPILE_20422
    case 67: {
      // AddAndReplaceKind
      convert_to_long((result));

      break;
    }
#endif
#ifdef COMPILE_20423
    case 68: {
      // AddAndReplaceKind
      convert_to_long_base((op1), 10);

      break;
    }
#endif
#ifdef COMPILE_20424
    case 69: {
      // AddAndReplaceKind
      convert_to_long_base((op2), 10);

      break;
    }
#endif
#ifdef COMPILE_20425
    case 70: {
      // AddAndReplaceKind
      convert_to_long_base((result), 10);

      break;
    }
#endif
#ifdef COMPILE_20426
    case 71: {
      // AddAndReplaceKind
      exit(1);

      break;
    }
#endif
#ifdef COMPILE_20427
    case 72: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((op1));

      break;
    }
#endif
#ifdef COMPILE_20428
    case 73: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((op2));

      break;
    }
#endif
#ifdef COMPILE_20429
    case 74: {
      // AddAndReplaceKind
      gc_remove_zval_from_buffer((result));

      break;
    }
#endif
#ifdef COMPILE_20430
    case 75: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((op1)));

      break;
    }
#endif
#ifdef COMPILE_20431
    case 76: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((op2)));

      break;
    }
#endif
#ifdef COMPILE_20432
    case 77: {
      // AddAndReplaceKind
      gc_zval_check_possible_root(((result)));

      break;
    }
#endif
#ifdef COMPILE_20433
    case 78: {
      // AddAndReplaceKind
      gc_zval_possible_root((op1));

      break;
    }
#endif
#ifdef COMPILE_20434
    case 79: {
      // AddAndReplaceKind
      gc_zval_possible_root((op2));

      break;
    }
#endif
#ifdef COMPILE_20435
    case 80: {
      // AddAndReplaceKind
      gc_zval_possible_root((result));

      break;
    }
#endif
#ifdef COMPILE_20436
    case 81: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((op1));

      break;
    }
#endif
#ifdef COMPILE_20437
    case 82: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((op2));

      break;
    }
#endif
#ifdef COMPILE_20438
    case 83: {
      // AddAndReplaceKind
      i_zval_ptr_dtor((result));

      break;
    }
#endif
#ifdef COMPILE_20439
    case 84: {
      // AddAndReplaceKind
      if ((*op1).type != 6) {
        zend_make_printable_zval(op1, &op1_copy, &use_copy1);
      }

      break;
    }
#endif
#ifdef COMPILE_20440
    case 85: {
      // AddAndReplaceKind
      if ((*op2).type != 6) {
        zend_make_printable_zval(op2, &op2_copy, &use_copy2);
      }

      break;
    }
#endif
#ifdef COMPILE_20441
    case 86: {
      // AddAndReplaceKind
      if (result == op1) {
        _zval_dtor((op1));
      }

      break;
    }
#endif
#ifdef COMPILE_20442
    case 87: {
      // AddAndReplaceKind
      if (use_copy1) {
        _zval_dtor((op1));
      }

      break;
    }
#endif
#ifdef COMPILE_20443
    case 88: {
      // AddAndReplaceKind
      if (use_copy2) {
        _zval_dtor((op2));
      }

      break;
    }
#endif
#ifdef COMPILE_20444
    case 89: {
      // AddAndReplaceKind
      if (use_copy2) {
        op2 = &op2_copy;
      }

      break;
    }
#endif
#ifdef COMPILE_20445
    case 90: {
      // AddAndReplaceKind
      memcpy((*(op1)).value.str.val + (*result).value.str.len,
             (*op2).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20446
    case 91: {
      // AddAndReplaceKind
      memcpy((*(op2)).value.str.val + (*result).value.str.len,
             (*op2).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20447
    case 92: {
      // AddAndReplaceKind
      memcpy((*(result)).value.str.val + (*result).value.str.len,
             (*op2).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20448
    case 93: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*(op1)).value.str.len,
             (*op2).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20449
    case 94: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*(op2)).value.str.len,
             (*op2).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20450
    case 95: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*(result)).value.str.len,
             (*op2).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20451
    case 96: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*result).value.str.len,
             (*(op1)).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20452
    case 97: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*result).value.str.len,
             (*(op2)).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20453
    case 98: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*result).value.str.len,
             (*(result)).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20454
    case 99: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*result).value.str.len,
             (*op2).value.str.val, (*(op1)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20455
    case 100: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*result).value.str.len,
             (*op2).value.str.val, (*(op2)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20456
    case 101: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*result).value.str.len,
             (*op2).value.str.val, (*(result)).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20457
    case 102: {
      // AddAndReplaceKind
      memcpy((*result).value.str.val + (*result).value.str.len,
             (*op2).value.str.val, (*op2).value.str.len);

      break;
    }
#endif
#ifdef COMPILE_20458
    case 103: {
      // AddAndReplaceKind
      op1 = &op1_copy;

      break;
    }
#endif
#ifdef COMPILE_20459
    case 104: {
      // AddAndReplaceKind
      op2 = &op2_copy;

      break;
    }
#endif
#ifdef COMPILE_20460
    case 105: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "String size overflow");

      break;
    }
#endif
#ifdef COMPILE_20461
    case 106: {
      // AddAndReplaceKind
      zend_error((1 << 0L), "Unsupported operand types");

      break;
    }
#endif
#ifdef COMPILE_20462
    case 107: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op1)))->name,
                 zend_get_type_by_const(1));

      break;
    }
#endif
#ifdef COMPILE_20463
    case 108: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op1)))->name,
                 zend_get_type_by_const(2));

      break;
    }
#endif
#ifdef COMPILE_20464
    case 109: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op1)))->name,
                 zend_get_type_by_const(3));

      break;
    }
#endif
#ifdef COMPILE_20465
    case 110: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op1)))->name,
                 zend_get_type_by_const(4));

      break;
    }
#endif
#ifdef COMPILE_20466
    case 111: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op1)))->name,
                 zend_get_type_by_const(6));

      break;
    }
#endif
#ifdef COMPILE_20467
    case 112: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op2)))->name,
                 zend_get_type_by_const(1));

      break;
    }
#endif
#ifdef COMPILE_20468
    case 113: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op2)))->name,
                 zend_get_type_by_const(2));

      break;
    }
#endif
#ifdef COMPILE_20469
    case 114: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op2)))->name,
                 zend_get_type_by_const(3));

      break;
    }
#endif
#ifdef COMPILE_20470
    case 115: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op2)))->name,
                 zend_get_type_by_const(4));

      break;
    }
#endif
#ifdef COMPILE_20471
    case 116: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(op2)))->name,
                 zend_get_type_by_const(6));

      break;
    }
#endif
#ifdef COMPILE_20472
    case 117: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(1));

      break;
    }
#endif
#ifdef COMPILE_20473
    case 118: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(2));

      break;
    }
#endif
#ifdef COMPILE_20474
    case 119: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(3));

      break;
    }
#endif
#ifdef COMPILE_20475
    case 120: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(4));

      break;
    }
#endif
#ifdef COMPILE_20476
    case 121: {
      // AddAndReplaceKind
      zend_error((1 << 12L), "Object of class %s could not be converted to %s",
                 zend_get_class_entry(&(*(result)))->name,
                 zend_get_type_by_const(6));

      break;
    }
#endif
#ifdef COMPILE_20477
    case 122: {
      // AddAndReplaceKind
      zend_error((1 << 1L), "Division by zero");

      break;
    }
#endif
#ifdef COMPILE_20478
    case 123: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "A non well formed numeric value encountered");

      break;
    }
#endif
#ifdef COMPILE_20479
    case 124: {
      // AddAndReplaceKind
      zend_error((1 << 3L),
                 "Object of class %s could not be converted to double",
                 zend_get_class_entry(&(*(op1)))->name);

      break;
    }
#endif
#ifdef COMPILE_20480
    case 125: {
      // AddAndReplaceKind
      zend_error((1 << 3L),
                 "Object of class %s could not be converted to double",
                 zend_get_class_entry(&(*(op2)))->name);

      break;
    }
#endif
#ifdef COMPILE_20481
    case 126: {
      // AddAndReplaceKind
      zend_error((1 << 3L),
                 "Object of class %s could not be converted to double",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20482
    case 127: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s could not be converted to int",
                 zend_get_class_entry(&(*(op1)))->name);

      break;
    }
#endif
#ifdef COMPILE_20483
    case 128: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s could not be converted to int",
                 zend_get_class_entry(&(*(op2)))->name);

      break;
    }
#endif
#ifdef COMPILE_20484
    case 129: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s could not be converted to int",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20485
    case 130: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s to string conversion",
                 zend_get_class_entry(&(*(op1)))->name);

      break;
    }
#endif
#ifdef COMPILE_20486
    case 131: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s to string conversion",
                 zend_get_class_entry(&(*(op2)))->name);

      break;
    }
#endif
#ifdef COMPILE_20487
    case 132: {
      // AddAndReplaceKind
      zend_error((1 << 3L), "Object of class %s to string conversion",
                 zend_get_class_entry(&(*(result)))->name);

      break;
    }
#endif
#ifdef COMPILE_20488
    case 133: {
      // AddAndReplaceKind
      zend_free_obj_get_result((op1));

      break;
    }
#endif
#ifdef COMPILE_20489
    case 134: {
      // AddAndReplaceKind
      zend_free_obj_get_result((op2));

      break;
    }
#endif
#ifdef COMPILE_20490
    case 135: {
      // AddAndReplaceKind
      zend_free_obj_get_result((result));

      break;
    }
#endif
#ifdef COMPILE_20491
    case 136: {
      // AddAndReplaceKind
      zend_make_printable_zval((op1), &op1_copy, &use_copy1);

      break;
    }
#endif
#ifdef COMPILE_20492
    case 137: {
      // AddAndReplaceKind
      zend_make_printable_zval((op1), &op2_copy, &use_copy2);

      break;
    }
#endif
#ifdef COMPILE_20493
    case 138: {
      // AddAndReplaceKind
      zend_make_printable_zval((op2), &op1_copy, &use_copy1);

      break;
    }
#endif
#ifdef COMPILE_20494
    case 139: {
      // AddAndReplaceKind
      zend_make_printable_zval((op2), &op2_copy, &use_copy2);

      break;
    }
#endif
#ifdef COMPILE_20495
    case 140: {
      // AddAndReplaceKind
      zend_make_printable_zval((result), &op1_copy, &use_copy1);

      break;
    }
#endif
#ifdef COMPILE_20496
    case 141: {
      // AddAndReplaceKind
      zend_make_printable_zval((result), &op2_copy, &use_copy2);

      break;
    }
#endif
#ifdef COMPILE_20497
    case 142: {
      // AddAndReplaceKind
      zend_make_printable_zval(op1, &op1_copy, &use_copy1);

      break;
    }
#endif
#ifdef COMPILE_20498
    case 143: {
      // AddAndReplaceKind
      zend_make_printable_zval(op2, &op2_copy, &use_copy2);

      break;
    }
#endif
#ifdef COMPILE_20499
    case 144: {
      // AddAndReplaceKind
      zval_unset_isref_p((op1));

      break;
    }
#endif
#ifdef COMPILE_20500
    case 145: {
      // AddAndReplaceKind
      zval_unset_isref_p((op2));

      break;
    }
#endif
#ifdef COMPILE_20501
    case 146: {
      // AddAndReplaceKind
      zval_unset_isref_p((result));

      break;
    }
#endif
    }
    {
      long long __temp214 =
          (result == op1 && !((((*op1).value.str.val) >=
                               (compiler_globals.interned_strings_start)) &&
                              (((*op1).value.str.val) <
                               (compiler_globals.interned_strings_end))));
      switch (__choose("__SWITCH214")) {
      case 0: {

        break;
      }
#ifdef COMPILE_20502
      case 1: {
        __temp214 =
            ((result == op1 && !((((*op1).value.str.val) >=
                                  (compiler_globals.interned_strings_start)) &&
                                 (((*op1).value.str.val) <
                                  (compiler_globals.interned_strings_end)))) &&
             !__is_neg(
                 "214-1", 87, &(result), sizeof(result), &(op1), sizeof(op1),
                 &(compiler_globals.interned_strings_start),
                 sizeof(compiler_globals.interned_strings_start),
                 &(compiler_globals.interned_strings_end),
                 sizeof(compiler_globals.interned_strings_end),
                 &((*op1).value.str.val), sizeof((*op1).value.str.val),
                 &(compiler_globals.interned_strings_start),
                 sizeof(compiler_globals.interned_strings_start),
                 &(compiler_globals.interned_strings_end),
                 sizeof(compiler_globals.interned_strings_end), &(op2),
                 sizeof(op2), &((*op1).value.str.len),
                 sizeof((*op1).value.str.len), &((*op2).value.str.len),
                 sizeof((*op2).value.str.len), &(use_copy2), sizeof(use_copy2),
                 &((*op2).value.str.val), sizeof((*op2).value.str.val),
                 &(use_copy1), sizeof(use_copy1), &((*op2).type),
                 sizeof((*op2).type), &((*op1).type), sizeof((*op1).type),
                 &(compiler_globals.active_class_entry),
                 sizeof(compiler_globals.active_class_entry),
                 &(compiler_globals.compiled_filename),
                 sizeof(compiler_globals.compiled_filename),
                 &(compiler_globals.zend_lineno),
                 sizeof(compiler_globals.zend_lineno),
                 &(compiler_globals.heredoc), sizeof(compiler_globals.heredoc),
                 &(compiler_globals.heredoc_len),
                 sizeof(compiler_globals.heredoc_len),
                 &(compiler_globals.active_op_array),
                 sizeof(compiler_globals.active_op_array),
                 &(compiler_globals.function_table),
                 sizeof(compiler_globals.function_table),
                 &(compiler_globals.class_table),
                 sizeof(compiler_globals.class_table),
                 &(compiler_globals.auto_globals),
                 sizeof(compiler_globals.auto_globals),
                 &(compiler_globals.in_compilation),
                 sizeof(compiler_globals.in_compilation),
                 &(compiler_globals.short_tags),
                 sizeof(compiler_globals.short_tags),
                 &(compiler_globals.asp_tags),
                 sizeof(compiler_globals.asp_tags),
                 &(compiler_globals.unclean_shutdown),
                 sizeof(compiler_globals.unclean_shutdown),
                 &(compiler_globals.ini_parser_unbuffered_errors),
                 sizeof(compiler_globals.ini_parser_unbuffered_errors),
                 &(compiler_globals.catch_begin),
                 sizeof(compiler_globals.catch_begin),
                 &(compiler_globals.ini_parser_param),
                 sizeof(compiler_globals.ini_parser_param),
                 &(compiler_globals.interactive),
                 sizeof(compiler_globals.interactive),
                 &(compiler_globals.start_lineno),
                 sizeof(compiler_globals.start_lineno),
                 &(compiler_globals.increment_lineno),
                 sizeof(compiler_globals.increment_lineno),
                 &(compiler_globals.access_type),
                 sizeof(compiler_globals.access_type),
                 &(compiler_globals.doc_comment),
                 sizeof(compiler_globals.doc_comment),
                 &(compiler_globals.doc_comment_len),
                 sizeof(compiler_globals.doc_comment_len),
                 &(compiler_globals.compiler_options),
                 sizeof(compiler_globals.compiler_options),
                 &(compiler_globals.current_namespace),
                 sizeof(compiler_globals.current_namespace),
                 &(compiler_globals.current_import),
                 sizeof(compiler_globals.current_import),
                 &(compiler_globals.in_namespace),
                 sizeof(compiler_globals.in_namespace),
                 &(compiler_globals.has_bracketed_namespaces),
                 sizeof(compiler_globals.has_bracketed_namespaces),
                 &(compiler_globals.interned_strings_top),
                 sizeof(compiler_globals.interned_strings_top),
                 &(compiler_globals.interned_strings_snapshot_top),
                 sizeof(compiler_globals.interned_strings_snapshot_top),
                 &(compiler_globals.script_encoding_list),
                 sizeof(compiler_globals.script_encoding_list),
                 &(compiler_globals.script_encoding_list_size),
                 sizeof(compiler_globals.script_encoding_list_size),
                 &(compiler_globals.multibyte),
                 sizeof(compiler_globals.multibyte),
                 &(compiler_globals.detect_unicode),
                 sizeof(compiler_globals.detect_unicode),
                 &(compiler_globals.encoding_declared),
                 sizeof(compiler_globals.encoding_declared),
                 &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                 &((*op1).is_ref__gc), sizeof((*op1).is_ref__gc),
                 &((*op2).refcount__gc), sizeof((*op2).refcount__gc),
                 &((*op2).is_ref__gc), sizeof((*op2).is_ref__gc),
                 &(compiler_globals.active_class_entry),
                 sizeof(compiler_globals.active_class_entry),
                 &(compiler_globals.compiled_filename),
                 sizeof(compiler_globals.compiled_filename),
                 &(compiler_globals.zend_lineno),
                 sizeof(compiler_globals.zend_lineno),
                 &(compiler_globals.heredoc), sizeof(compiler_globals.heredoc),
                 &(compiler_globals.heredoc_len),
                 sizeof(compiler_globals.heredoc_len),
                 &(compiler_globals.active_op_array),
                 sizeof(compiler_globals.active_op_array),
                 &(compiler_globals.function_table),
                 sizeof(compiler_globals.function_table),
                 &(compiler_globals.class_table),
                 sizeof(compiler_globals.class_table),
                 &(compiler_globals.auto_globals),
                 sizeof(compiler_globals.auto_globals),
                 &(compiler_globals.in_compilation),
                 sizeof(compiler_globals.in_compilation),
                 &(compiler_globals.short_tags),
                 sizeof(compiler_globals.short_tags),
                 &(compiler_globals.asp_tags),
                 sizeof(compiler_globals.asp_tags),
                 &(compiler_globals.unclean_shutdown),
                 sizeof(compiler_globals.unclean_shutdown),
                 &(compiler_globals.ini_parser_unbuffered_errors),
                 sizeof(compiler_globals.ini_parser_unbuffered_errors),
                 &(compiler_globals.catch_begin),
                 sizeof(compiler_globals.catch_begin),
                 &(compiler_globals.ini_parser_param),
                 sizeof(compiler_globals.ini_parser_param),
                 &(compiler_globals.interactive),
                 sizeof(compiler_globals.interactive),
                 &(compiler_globals.start_lineno),
                 sizeof(compiler_globals.start_lineno),
                 &(compiler_globals.increment_lineno),
                 sizeof(compiler_globals.increment_lineno),
                 &(compiler_globals.access_type),
                 sizeof(compiler_globals.access_type),
                 &(compiler_globals.doc_comment),
                 sizeof(compiler_globals.doc_comment),
                 &(compiler_globals.doc_comment_len),
                 sizeof(compiler_globals.doc_comment_len),
                 &(compiler_globals.compiler_options),
                 sizeof(compiler_globals.compiler_options),
                 &(compiler_globals.current_namespace),
                 sizeof(compiler_globals.current_namespace),
                 &(compiler_globals.current_import),
                 sizeof(compiler_globals.current_import),
                 &(compiler_globals.in_namespace),
                 sizeof(compiler_globals.in_namespace),
                 &(compiler_globals.has_bracketed_namespaces),
                 sizeof(compiler_globals.has_bracketed_namespaces),
                 &(compiler_globals.interned_strings_top),
                 sizeof(compiler_globals.interned_strings_top),
                 &(compiler_globals.interned_strings_snapshot_top),
                 sizeof(compiler_globals.interned_strings_snapshot_top),
                 &(compiler_globals.script_encoding_list),
                 sizeof(compiler_globals.script_encoding_list),
                 &(compiler_globals.script_encoding_list_size),
                 sizeof(compiler_globals.script_encoding_list_size),
                 &(compiler_globals.multibyte),
                 sizeof(compiler_globals.multibyte),
                 &(compiler_globals.detect_unicode),
                 sizeof(compiler_globals.detect_unicode),
                 &(compiler_globals.encoding_declared),
                 sizeof(compiler_globals.encoding_declared)));

        break;
      }
#endif
      }
      switch (__choose("__SWITCH226")) {
      case 0: {
        if (__temp214) { /* special case, perform operations on result */
          uint res_len = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);

          {
            switch (__choose("__SWITCH215")) {
            case 0:
              break;
#ifdef COMPILE_20503
            case 1: {
              // AddAndReplaceKind
              (*(op1)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20504
            case 2: {
              // AddAndReplaceKind
              (*(op1)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20505
            case 3: {
              // AddAndReplaceKind
              (*(op1)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20506
            case 4: {
              // AddAndReplaceKind
              (*(op1)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20507
            case 5: {
              // AddAndReplaceKind
              (*(op1)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20508
            case 6: {
              // AddAndReplaceKind
              (*(op1)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20509
            case 7: {
              // AddAndReplaceKind
              (*(op2)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20510
            case 8: {
              // AddAndReplaceKind
              (*(op2)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20511
            case 9: {
              // AddAndReplaceKind
              (*(op2)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20512
            case 10: {
              // AddAndReplaceKind
              (*(op2)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20513
            case 11: {
              // AddAndReplaceKind
              (*(op2)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20514
            case 12: {
              // AddAndReplaceKind
              (*(op2)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20515
            case 13: {
              // AddAndReplaceKind
              (*(result)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20516
            case 14: {
              // AddAndReplaceKind
              (*(result)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20517
            case 15: {
              // AddAndReplaceKind
              (*(result)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20518
            case 16: {
              // AddAndReplaceKind
              (*(result)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20519
            case 17: {
              // AddAndReplaceKind
              (*(result)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20520
            case 18: {
              // AddAndReplaceKind
              (*(result)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20521
            case 19: {
              // AddAndReplaceKind
              (*result).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20522
            case 20: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20523
            case 21: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20524
            case 22: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(result)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20525
            case 23: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20526
            case 24: {
              // AddAndReplaceKind
              (*result).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20527
            case 25: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_20528
            case 26: {
              // AddAndReplaceKind
              (op1) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20529
            case 27: {
              // AddAndReplaceKind
              (op1) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20530
            case 28: {
              // AddAndReplaceKind
              (op2) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20531
            case 29: {
              // AddAndReplaceKind
              (op2) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20532
            case 30: {
              // AddAndReplaceKind
              (result) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20533
            case 31: {
              // AddAndReplaceKind
              (result) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20534
            case 32: {
              // AddAndReplaceKind
              _convert_to_string(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20535
            case 33: {
              // AddAndReplaceKind
              _convert_to_string(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20536
            case 34: {
              // AddAndReplaceKind
              _convert_to_string(((result)));

              break;
            }
#endif
#ifdef COMPILE_20537
            case 35: {
              // AddAndReplaceKind
              _efree(((*(op1)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20538
            case 36: {
              // AddAndReplaceKind
              _efree(((*(op2)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20539
            case 37: {
              // AddAndReplaceKind
              _efree(((*(result)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20540
            case 38: {
              // AddAndReplaceKind
              _efree(((*result).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20541
            case 39: {
              // AddAndReplaceKind
              _efree(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20542
            case 40: {
              // AddAndReplaceKind
              _efree(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20543
            case 41: {
              // AddAndReplaceKind
              _efree(((result)));

              break;
            }
#endif
#ifdef COMPILE_20544
            case 42: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20545
            case 43: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20546
            case 44: {
              // AddAndReplaceKind
              _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20547
            case 45: {
              // AddAndReplaceKind
              _zend_list_delete((*(op1)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20548
            case 46: {
              // AddAndReplaceKind
              _zend_list_delete((*(op2)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20549
            case 47: {
              // AddAndReplaceKind
              _zend_list_delete((*(result)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20550
            case 48: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20551
            case 49: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20552
            case 50: {
              // AddAndReplaceKind
              _zval_copy_ctor(((result)));

              break;
            }
#endif
#ifdef COMPILE_20553
            case 51: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_20554
            case 52: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_20555
            case 53: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((result));

              break;
            }
#endif
#ifdef COMPILE_20556
            case 54: {
              // AddAndReplaceKind
              _zval_dtor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20557
            case 55: {
              // AddAndReplaceKind
              _zval_dtor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20558
            case 56: {
              // AddAndReplaceKind
              _zval_dtor(((result)));

              break;
            }
#endif
#ifdef COMPILE_20559
            case 57: {
              // AddAndReplaceKind
              _zval_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_20560
            case 58: {
              // AddAndReplaceKind
              _zval_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_20561
            case 59: {
              // AddAndReplaceKind
              _zval_dtor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_20562
            case 60: {
              // AddAndReplaceKind
              _zval_dtor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_20563
            case 61: {
              // AddAndReplaceKind
              _zval_dtor_func((result));

              break;
            }
#endif
#ifdef COMPILE_20564
            case 62: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op1)));

              break;
            }
#endif
#ifdef COMPILE_20565
            case 63: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op2)));

              break;
            }
#endif
#ifdef COMPILE_20566
            case 64: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(result)));

              break;
            }
#endif
#ifdef COMPILE_20567
            case 65: {
              // AddAndReplaceKind
              convert_scalar_to_array((op1), 4);

              break;
            }
#endif
#ifdef COMPILE_20568
            case 66: {
              // AddAndReplaceKind
              convert_scalar_to_array((op2), 4);

              break;
            }
#endif
#ifdef COMPILE_20569
            case 67: {
              // AddAndReplaceKind
              convert_scalar_to_array((result), 4);

              break;
            }
#endif
#ifdef COMPILE_20570
            case 68: {
              // AddAndReplaceKind
              convert_scalar_to_number((op1));

              break;
            }
#endif
#ifdef COMPILE_20571
            case 69: {
              // AddAndReplaceKind
              convert_scalar_to_number((op2));

              break;
            }
#endif
#ifdef COMPILE_20572
            case 70: {
              // AddAndReplaceKind
              convert_scalar_to_number((result));

              break;
            }
#endif
#ifdef COMPILE_20573
            case 71: {
              // AddAndReplaceKind
              convert_to_array((op1));

              break;
            }
#endif
#ifdef COMPILE_20574
            case 72: {
              // AddAndReplaceKind
              convert_to_array((op2));

              break;
            }
#endif
#ifdef COMPILE_20575
            case 73: {
              // AddAndReplaceKind
              convert_to_array((result));

              break;
            }
#endif
#ifdef COMPILE_20576
            case 74: {
              // AddAndReplaceKind
              convert_to_boolean((op1));

              break;
            }
#endif
#ifdef COMPILE_20577
            case 75: {
              // AddAndReplaceKind
              convert_to_boolean((op2));

              break;
            }
#endif
#ifdef COMPILE_20578
            case 76: {
              // AddAndReplaceKind
              convert_to_boolean((result));

              break;
            }
#endif
#ifdef COMPILE_20579
            case 77: {
              // AddAndReplaceKind
              convert_to_double((op1));

              break;
            }
#endif
#ifdef COMPILE_20580
            case 78: {
              // AddAndReplaceKind
              convert_to_double((op2));

              break;
            }
#endif
#ifdef COMPILE_20581
            case 79: {
              // AddAndReplaceKind
              convert_to_double((result));

              break;
            }
#endif
#ifdef COMPILE_20582
            case 80: {
              // AddAndReplaceKind
              convert_to_long((op1));

              break;
            }
#endif
#ifdef COMPILE_20583
            case 81: {
              // AddAndReplaceKind
              convert_to_long((op2));

              break;
            }
#endif
#ifdef COMPILE_20584
            case 82: {
              // AddAndReplaceKind
              convert_to_long((result));

              break;
            }
#endif
#ifdef COMPILE_20585
            case 83: {
              // AddAndReplaceKind
              convert_to_long_base((op1), 10);

              break;
            }
#endif
#ifdef COMPILE_20586
            case 84: {
              // AddAndReplaceKind
              convert_to_long_base((op2), 10);

              break;
            }
#endif
#ifdef COMPILE_20587
            case 85: {
              // AddAndReplaceKind
              convert_to_long_base((result), 10);

              break;
            }
#endif
#ifdef COMPILE_20588
            case 86: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_20589
            case 87: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op1));

              break;
            }
#endif
#ifdef COMPILE_20590
            case 88: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op2));

              break;
            }
#endif
#ifdef COMPILE_20591
            case 89: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((result));

              break;
            }
#endif
#ifdef COMPILE_20592
            case 90: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20593
            case 91: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20594
            case 92: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((result)));

              break;
            }
#endif
#ifdef COMPILE_20595
            case 93: {
              // AddAndReplaceKind
              gc_zval_possible_root((op1));

              break;
            }
#endif
#ifdef COMPILE_20596
            case 94: {
              // AddAndReplaceKind
              gc_zval_possible_root((op2));

              break;
            }
#endif
#ifdef COMPILE_20597
            case 95: {
              // AddAndReplaceKind
              gc_zval_possible_root((result));

              break;
            }
#endif
#ifdef COMPILE_20598
            case 96: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_20599
            case 97: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_20600
            case 98: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((result));

              break;
            }
#endif
#ifdef COMPILE_20601
            case 99: {
              // AddAndReplaceKind
              if ((*op1).type != 6) {
                zend_make_printable_zval(op1, &op1_copy, &use_copy1);
              }

              break;
            }
#endif
#ifdef COMPILE_20602
            case 100: {
              // AddAndReplaceKind
              if ((*op2).type != 6) {
                zend_make_printable_zval(op2, &op2_copy, &use_copy2);
              }

              break;
            }
#endif
#ifdef COMPILE_20603
            case 101: {
              // AddAndReplaceKind
              if (result == op1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_20604
            case 102: {
              // AddAndReplaceKind
              if (use_copy1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_20605
            case 103: {
              // AddAndReplaceKind
              if (use_copy2) {
                _zval_dtor((op2));
              }

              break;
            }
#endif
#ifdef COMPILE_20606
            case 104: {
              // AddAndReplaceKind
              if (use_copy2) {
                op2 = &op2_copy;
              }

              break;
            }
#endif
#ifdef COMPILE_20607
            case 105: {
              // AddAndReplaceKind
              memcpy((*(op1)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20608
            case 106: {
              // AddAndReplaceKind
              memcpy((*(op2)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20609
            case 107: {
              // AddAndReplaceKind
              memcpy((*(result)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20610
            case 108: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op1)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20611
            case 109: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op2)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20612
            case 110: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(result)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20613
            case 111: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op1)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20614
            case 112: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op2)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20615
            case 113: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(result)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20616
            case 114: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op1)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20617
            case 115: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op2)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20618
            case 116: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(result)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20619
            case 117: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20620
            case 118: {
              // AddAndReplaceKind
              op1 = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20621
            case 119: {
              // AddAndReplaceKind
              op2 = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20622
            case 120: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "String size overflow");

              break;
            }
#endif
#ifdef COMPILE_20623
            case 121: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Unsupported operand types");

              break;
            }
#endif
#ifdef COMPILE_20624
            case 122: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20625
            case 123: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20626
            case 124: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20627
            case 125: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20628
            case 126: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20629
            case 127: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20630
            case 128: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20631
            case 129: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20632
            case 130: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20633
            case 131: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20634
            case 132: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20635
            case 133: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20636
            case 134: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20637
            case 135: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20638
            case 136: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20639
            case 137: {
              // AddAndReplaceKind
              zend_error((1 << 1L), "Division by zero");

              break;
            }
#endif
#ifdef COMPILE_20640
            case 138: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_20641
            case 139: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20642
            case 140: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_20643
            case 141: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20644
            case 142: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20645
            case 143: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_20646
            case 144: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20647
            case 145: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20648
            case 146: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_20649
            case 147: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20650
            case 148: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op1));

              break;
            }
#endif
#ifdef COMPILE_20651
            case 149: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op2));

              break;
            }
#endif
#ifdef COMPILE_20652
            case 150: {
              // AddAndReplaceKind
              zend_free_obj_get_result((result));

              break;
            }
#endif
#ifdef COMPILE_20653
            case 151: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_20654
            case 152: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_20655
            case 153: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_20656
            case 154: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_20657
            case 155: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_20658
            case 156: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_20659
            case 157: {
              // AddAndReplaceKind
              zend_make_printable_zval(op1, &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_20660
            case 158: {
              // AddAndReplaceKind
              zend_make_printable_zval(op2, &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_20661
            case 159: {
              // AddAndReplaceKind
              zval_unset_isref_p((op1));

              break;
            }
#endif
#ifdef COMPILE_20662
            case 160: {
              // AddAndReplaceKind
              zval_unset_isref_p((op2));

              break;
            }
#endif
#ifdef COMPILE_20663
            case 161: {
              // AddAndReplaceKind
              zval_unset_isref_p((result));

              break;
            }
#endif
#ifdef COMPILE_20664
            case 162: {
              // AddInitKind
              memset(&(*op1), 0, sizeof(*(&(*op1))));

              break;
            }
#endif
#ifdef COMPILE_20665
            case 163: {
              // AddInitKind
              memset(&(*op1).value, 0, sizeof(*(&(*op1).value)));

              break;
            }
#endif
#ifdef COMPILE_20666
            case 164: {
              // AddInitKind
              memset(&(*op1).value.str, 0, sizeof(*(&(*op1).value.str)));

              break;
            }
#endif
#ifdef COMPILE_20667
            case 165: {
              // AddInitKind
              memset(&(*op2), 0, sizeof(*(&(*op2))));

              break;
            }
#endif
#ifdef COMPILE_20668
            case 166: {
              // AddInitKind
              memset(&(*op2).value, 0, sizeof(*(&(*op2).value)));

              break;
            }
#endif
#ifdef COMPILE_20669
            case 167: {
              // AddInitKind
              memset(&(*op2).value.str, 0, sizeof(*(&(*op2).value.str)));

              break;
            }
#endif
#ifdef COMPILE_20670
            case 168: {
              // AddInitKind
              memset(&(*result), 0, sizeof(*(&(*result))));

              break;
            }
#endif
#ifdef COMPILE_20671
            case 169: {
              // AddInitKind
              memset(&(*result).value, 0, sizeof(*(&(*result).value)));

              break;
            }
#endif
#ifdef COMPILE_20672
            case 170: {
              // AddInitKind
              memset(&(*result).value.str, 0, sizeof(*(&(*result).value.str)));

              break;
            }
#endif
#ifdef COMPILE_20673
            case 171: {
              // IfExitKind
              if (__is_neg(
                      "215-171", 21, &(result), sizeof(result), &(op1),
                      sizeof(op1), &(op2), sizeof(op2),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op1).value.str.len),
                      sizeof((*op1).value.str.len), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &((*result).value.str.val),
                      sizeof((*result).value.str.val), &((*op1).value.str.val),
                      sizeof((*op1).value.str.val), &(use_copy2),
                      sizeof(use_copy2), &(res_len), sizeof(res_len),
                      &((*op2).value.str.val), sizeof((*op2).value.str.val),
                      &(use_copy1), sizeof(use_copy1), &((*op2).type),
                      sizeof((*op2).type), &((*op1).type), sizeof((*op1).type),
                      &((*result).refcount__gc), sizeof((*result).refcount__gc),
                      &((*result).type), sizeof((*result).type),
                      &((*result).is_ref__gc), sizeof((*result).is_ref__gc),
                      &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                      &((*op1).is_ref__gc), sizeof((*op1).is_ref__gc),
                      &((*op2).refcount__gc), sizeof((*op2).refcount__gc),
                      &((*op2).is_ref__gc), sizeof((*op2).is_ref__gc)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_20674
            case 172: {
              // IfExitKind
              if (__is_neg(
                      "215-172", 21, &(result), sizeof(result), &(op1),
                      sizeof(op1), &(op2), sizeof(op2),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op1).value.str.len),
                      sizeof((*op1).value.str.len), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &((*result).value.str.val),
                      sizeof((*result).value.str.val), &((*op1).value.str.val),
                      sizeof((*op1).value.str.val), &(use_copy2),
                      sizeof(use_copy2), &(res_len), sizeof(res_len),
                      &((*op2).value.str.val), sizeof((*op2).value.str.val),
                      &(use_copy1), sizeof(use_copy1), &((*op2).type),
                      sizeof((*op2).type), &((*op1).type), sizeof((*op1).type),
                      &((*result).refcount__gc), sizeof((*result).refcount__gc),
                      &((*result).type), sizeof((*result).type),
                      &((*result).is_ref__gc), sizeof((*result).is_ref__gc),
                      &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                      &((*op1).is_ref__gc), sizeof((*op1).is_ref__gc),
                      &((*op2).refcount__gc), sizeof((*op2).refcount__gc),
                      &((*op2).is_ref__gc), sizeof((*op2).is_ref__gc)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_20675
            case 173: {
              // IfExitKind
              if (__is_neg(
                      "215-173", 21, &(result), sizeof(result), &(op1),
                      sizeof(op1), &(op2), sizeof(op2),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op1).value.str.len),
                      sizeof((*op1).value.str.len), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &((*result).value.str.val),
                      sizeof((*result).value.str.val), &((*op1).value.str.val),
                      sizeof((*op1).value.str.val), &(use_copy2),
                      sizeof(use_copy2), &(res_len), sizeof(res_len),
                      &((*op2).value.str.val), sizeof((*op2).value.str.val),
                      &(use_copy1), sizeof(use_copy1), &((*op2).type),
                      sizeof((*op2).type), &((*op1).type), sizeof((*op1).type),
                      &((*result).refcount__gc), sizeof((*result).refcount__gc),
                      &((*result).type), sizeof((*result).type),
                      &((*result).is_ref__gc), sizeof((*result).is_ref__gc),
                      &((*op1).refcount__gc), sizeof((*op1).refcount__gc),
                      &((*op1).is_ref__gc), sizeof((*op1).is_ref__gc),
                      &((*op2).refcount__gc), sizeof((*op2).refcount__gc),
                      &((*op2).is_ref__gc), sizeof((*op2).is_ref__gc)))
                return 6;

              break;
            }
#endif
            }
            {
              long long __temp216 =
                  ((*result).value.str.len < 0 ||
                   (int)((*op1).value.str.len + (*op2).value.str.len) < 0);
              switch (__choose("__SWITCH216")) {
              case 0: {

                break;
              }
#ifdef COMPILE_20676
              case 1: {
                __temp216 =
                    (((*result).value.str.len < 0 ||
                      (int)((*op1).value.str.len + (*op2).value.str.len) < 0) ||
                     __is_neg(
                         "216-1", 21, &(result), sizeof(result), &(op1),
                         sizeof(op1), &(op2), sizeof(op2),
                         &((*result).value.str.len),
                         sizeof((*result).value.str.len),
                         &((*op1).value.str.len), sizeof((*op1).value.str.len),
                         &((*op2).value.str.len), sizeof((*op2).value.str.len),
                         &((*result).value.str.val),
                         sizeof((*result).value.str.val),
                         &((*op1).value.str.val), sizeof((*op1).value.str.val),
                         &(use_copy2), sizeof(use_copy2), &(res_len),
                         sizeof(res_len), &((*op2).value.str.val),
                         sizeof((*op2).value.str.val), &(use_copy1),
                         sizeof(use_copy1), &((*op2).type), sizeof((*op2).type),
                         &((*op1).type), sizeof((*op1).type),
                         &((*result).refcount__gc),
                         sizeof((*result).refcount__gc), &((*result).type),
                         sizeof((*result).type), &((*result).is_ref__gc),
                         sizeof((*result).is_ref__gc), &((*op1).refcount__gc),
                         sizeof((*op1).refcount__gc), &((*op1).is_ref__gc),
                         sizeof((*op1).is_ref__gc), &((*op2).refcount__gc),
                         sizeof((*op2).refcount__gc), &((*op2).is_ref__gc),
                         sizeof((*op2).is_ref__gc)));

                break;
              }
#endif
#ifdef COMPILE_20677
              case 2: {
                __temp216 =
                    (((*result).value.str.len < 0 ||
                      (int)((*op1).value.str.len + (*op2).value.str.len) < 0) &&
                     !__is_neg(
                         "216-2", 21, &(result), sizeof(result), &(op1),
                         sizeof(op1), &(op2), sizeof(op2),
                         &((*result).value.str.len),
                         sizeof((*result).value.str.len),
                         &((*op1).value.str.len), sizeof((*op1).value.str.len),
                         &((*op2).value.str.len), sizeof((*op2).value.str.len),
                         &((*result).value.str.val),
                         sizeof((*result).value.str.val),
                         &((*op1).value.str.val), sizeof((*op1).value.str.val),
                         &(use_copy2), sizeof(use_copy2), &(res_len),
                         sizeof(res_len), &((*op2).value.str.val),
                         sizeof((*op2).value.str.val), &(use_copy1),
                         sizeof(use_copy1), &((*op2).type), sizeof((*op2).type),
                         &((*op1).type), sizeof((*op1).type),
                         &((*result).refcount__gc),
                         sizeof((*result).refcount__gc), &((*result).type),
                         sizeof((*result).type), &((*result).is_ref__gc),
                         sizeof((*result).is_ref__gc), &((*op1).refcount__gc),
                         sizeof((*op1).refcount__gc), &((*op1).is_ref__gc),
                         sizeof((*op1).is_ref__gc), &((*op2).refcount__gc),
                         sizeof((*op2).refcount__gc), &((*op2).is_ref__gc),
                         sizeof((*op2).is_ref__gc)));

                break;
              }
#endif
              }
              switch (__choose("__SWITCH217")) {
              case 0: {
                if (__temp216) {
                  efree(Z_STRVAL_P(result));
                  ZVAL_EMPTY_STRING(result);
                  zend_error(E_ERROR, "String size overflow");
                }

                break;
              }
#ifdef COMPILE_20678
              case 1: {
                // GuardKind
                if (!__is_neg(
                        "217-1", 21, &(result), sizeof(result), &(op1),
                        sizeof(op1), &(op2), sizeof(op2),
                        &((*result).value.str.len),
                        sizeof((*result).value.str.len),
                        &((*op1).value.str.len), sizeof((*op1).value.str.len),
                        &((*op2).value.str.len), sizeof((*op2).value.str.len),
                        &((*result).value.str.val),
                        sizeof((*result).value.str.val),
                        &((*op1).value.str.val), sizeof((*op1).value.str.val),
                        &(use_copy2), sizeof(use_copy2), &(res_len),
                        sizeof(res_len), &((*op2).value.str.val),
                        sizeof((*op2).value.str.val), &(use_copy1),
                        sizeof(use_copy1), &((*op2).type), sizeof((*op2).type),
                        &((*op1).type), sizeof((*op1).type),
                        &((*result).refcount__gc),
                        sizeof((*result).refcount__gc), &((*result).type),
                        sizeof((*result).type), &((*result).is_ref__gc),
                        sizeof((*result).is_ref__gc), &((*op1).refcount__gc),
                        sizeof((*op1).refcount__gc), &((*op1).is_ref__gc),
                        sizeof((*op1).is_ref__gc), &((*op2).refcount__gc),
                        sizeof((*op2).refcount__gc), &((*op2).is_ref__gc),
                        sizeof((*op2).is_ref__gc)))
                  if ((*result).value.str.len < 0 ||
                      (int)((*op1).value.str.len + (*op2).value.str.len) < 0) {
                    _efree(((*result).value.str.val));
                    do {
                      zval *__z = (result);
                      (*__z).value.str.len = 0;
                      (*__z).value.str.val = _estrndup((""), (sizeof("") - 1));
                      (*__z).type = 6;
                    } while (0);
                    zend_error((1 << 0L), "String size overflow");
                  }

                break;
              }
#endif
#ifdef COMPILE_20679
              case 2: {
                // SpecialGuardKind
                if (!__is_neg(
                        "217-2", 21, &(result), sizeof(result), &(op1),
                        sizeof(op1), &(op2), sizeof(op2),
                        &((*result).value.str.len),
                        sizeof((*result).value.str.len),
                        &((*op1).value.str.len), sizeof((*op1).value.str.len),
                        &((*op2).value.str.len), sizeof((*op2).value.str.len),
                        &((*result).value.str.val),
                        sizeof((*result).value.str.val),
                        &((*op1).value.str.val), sizeof((*op1).value.str.val),
                        &(use_copy2), sizeof(use_copy2), &(res_len),
                        sizeof(res_len), &((*op2).value.str.val),
                        sizeof((*op2).value.str.val), &(use_copy1),
                        sizeof(use_copy1), &((*op2).type), sizeof((*op2).type),
                        &((*op1).type), sizeof((*op1).type),
                        &((*result).refcount__gc),
                        sizeof((*result).refcount__gc), &((*result).type),
                        sizeof((*result).type), &((*result).is_ref__gc),
                        sizeof((*result).is_ref__gc), &((*op1).refcount__gc),
                        sizeof((*op1).refcount__gc), &((*op1).is_ref__gc),
                        sizeof((*op1).is_ref__gc), &((*op2).refcount__gc),
                        sizeof((*op2).refcount__gc), &((*op2).is_ref__gc),
                        sizeof((*op2).is_ref__gc)) &&
                    ((*result).value.str.len < 0 ||
                     (int)((*op1).value.str.len + (*op2).value.str.len) < 0)) {
                  _efree(((*result).value.str.val));
                  do {
                    zval *__z = (result);
                    (*__z).value.str.len = 0;
                    (*__z).value.str.val = _estrndup((""), (sizeof("") - 1));
                    (*__z).type = 6;
                  } while (0);
                  zend_error((1 << 0L), "String size overflow");
                }

                break;
              }
#endif
              }
            }
          }

          {
            switch (__choose("__SWITCH218")) {
            case 0:
              break;
#ifdef COMPILE_20680
            case 1: {
              // AddAndReplaceKind
              (*(op1)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20681
            case 2: {
              // AddAndReplaceKind
              (*(op1)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20682
            case 3: {
              // AddAndReplaceKind
              (*(op1)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20683
            case 4: {
              // AddAndReplaceKind
              (*(op1)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20684
            case 5: {
              // AddAndReplaceKind
              (*(op1)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20685
            case 6: {
              // AddAndReplaceKind
              (*(op1)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20686
            case 7: {
              // AddAndReplaceKind
              (*(op2)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20687
            case 8: {
              // AddAndReplaceKind
              (*(op2)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20688
            case 9: {
              // AddAndReplaceKind
              (*(op2)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20689
            case 10: {
              // AddAndReplaceKind
              (*(op2)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20690
            case 11: {
              // AddAndReplaceKind
              (*(op2)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20691
            case 12: {
              // AddAndReplaceKind
              (*(op2)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20692
            case 13: {
              // AddAndReplaceKind
              (*(result)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20693
            case 14: {
              // AddAndReplaceKind
              (*(result)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20694
            case 15: {
              // AddAndReplaceKind
              (*(result)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20695
            case 16: {
              // AddAndReplaceKind
              (*(result)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20696
            case 17: {
              // AddAndReplaceKind
              (*(result)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20697
            case 18: {
              // AddAndReplaceKind
              (*(result)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20698
            case 19: {
              // AddAndReplaceKind
              (*result).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20699
            case 20: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20700
            case 21: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20701
            case 22: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(result)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20702
            case 23: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20703
            case 24: {
              // AddAndReplaceKind
              (*result).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20704
            case 25: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_20705
            case 26: {
              // AddAndReplaceKind
              (op1) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20706
            case 27: {
              // AddAndReplaceKind
              (op1) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20707
            case 28: {
              // AddAndReplaceKind
              (op2) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20708
            case 29: {
              // AddAndReplaceKind
              (op2) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20709
            case 30: {
              // AddAndReplaceKind
              (result) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20710
            case 31: {
              // AddAndReplaceKind
              (result) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20711
            case 32: {
              // AddAndReplaceKind
              _convert_to_string(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20712
            case 33: {
              // AddAndReplaceKind
              _convert_to_string(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20713
            case 34: {
              // AddAndReplaceKind
              _convert_to_string(((result)));

              break;
            }
#endif
#ifdef COMPILE_20714
            case 35: {
              // AddAndReplaceKind
              _efree(((*(op1)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20715
            case 36: {
              // AddAndReplaceKind
              _efree(((*(op2)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20716
            case 37: {
              // AddAndReplaceKind
              _efree(((*(result)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20717
            case 38: {
              // AddAndReplaceKind
              _efree(((*result).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20718
            case 39: {
              // AddAndReplaceKind
              _efree(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20719
            case 40: {
              // AddAndReplaceKind
              _efree(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20720
            case 41: {
              // AddAndReplaceKind
              _efree(((result)));

              break;
            }
#endif
#ifdef COMPILE_20721
            case 42: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20722
            case 43: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20723
            case 44: {
              // AddAndReplaceKind
              _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20724
            case 45: {
              // AddAndReplaceKind
              _zend_list_delete((*(op1)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20725
            case 46: {
              // AddAndReplaceKind
              _zend_list_delete((*(op2)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20726
            case 47: {
              // AddAndReplaceKind
              _zend_list_delete((*(result)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20727
            case 48: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20728
            case 49: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20729
            case 50: {
              // AddAndReplaceKind
              _zval_copy_ctor(((result)));

              break;
            }
#endif
#ifdef COMPILE_20730
            case 51: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_20731
            case 52: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_20732
            case 53: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((result));

              break;
            }
#endif
#ifdef COMPILE_20733
            case 54: {
              // AddAndReplaceKind
              _zval_dtor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20734
            case 55: {
              // AddAndReplaceKind
              _zval_dtor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20735
            case 56: {
              // AddAndReplaceKind
              _zval_dtor(((result)));

              break;
            }
#endif
#ifdef COMPILE_20736
            case 57: {
              // AddAndReplaceKind
              _zval_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_20737
            case 58: {
              // AddAndReplaceKind
              _zval_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_20738
            case 59: {
              // AddAndReplaceKind
              _zval_dtor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_20739
            case 60: {
              // AddAndReplaceKind
              _zval_dtor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_20740
            case 61: {
              // AddAndReplaceKind
              _zval_dtor_func((result));

              break;
            }
#endif
#ifdef COMPILE_20741
            case 62: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op1)));

              break;
            }
#endif
#ifdef COMPILE_20742
            case 63: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op2)));

              break;
            }
#endif
#ifdef COMPILE_20743
            case 64: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(result)));

              break;
            }
#endif
#ifdef COMPILE_20744
            case 65: {
              // AddAndReplaceKind
              convert_scalar_to_array((op1), 4);

              break;
            }
#endif
#ifdef COMPILE_20745
            case 66: {
              // AddAndReplaceKind
              convert_scalar_to_array((op2), 4);

              break;
            }
#endif
#ifdef COMPILE_20746
            case 67: {
              // AddAndReplaceKind
              convert_scalar_to_array((result), 4);

              break;
            }
#endif
#ifdef COMPILE_20747
            case 68: {
              // AddAndReplaceKind
              convert_scalar_to_number((op1));

              break;
            }
#endif
#ifdef COMPILE_20748
            case 69: {
              // AddAndReplaceKind
              convert_scalar_to_number((op2));

              break;
            }
#endif
#ifdef COMPILE_20749
            case 70: {
              // AddAndReplaceKind
              convert_scalar_to_number((result));

              break;
            }
#endif
#ifdef COMPILE_20750
            case 71: {
              // AddAndReplaceKind
              convert_to_array((op1));

              break;
            }
#endif
#ifdef COMPILE_20751
            case 72: {
              // AddAndReplaceKind
              convert_to_array((op2));

              break;
            }
#endif
#ifdef COMPILE_20752
            case 73: {
              // AddAndReplaceKind
              convert_to_array((result));

              break;
            }
#endif
#ifdef COMPILE_20753
            case 74: {
              // AddAndReplaceKind
              convert_to_boolean((op1));

              break;
            }
#endif
#ifdef COMPILE_20754
            case 75: {
              // AddAndReplaceKind
              convert_to_boolean((op2));

              break;
            }
#endif
#ifdef COMPILE_20755
            case 76: {
              // AddAndReplaceKind
              convert_to_boolean((result));

              break;
            }
#endif
#ifdef COMPILE_20756
            case 77: {
              // AddAndReplaceKind
              convert_to_double((op1));

              break;
            }
#endif
#ifdef COMPILE_20757
            case 78: {
              // AddAndReplaceKind
              convert_to_double((op2));

              break;
            }
#endif
#ifdef COMPILE_20758
            case 79: {
              // AddAndReplaceKind
              convert_to_double((result));

              break;
            }
#endif
#ifdef COMPILE_20759
            case 80: {
              // AddAndReplaceKind
              convert_to_long((op1));

              break;
            }
#endif
#ifdef COMPILE_20760
            case 81: {
              // AddAndReplaceKind
              convert_to_long((op2));

              break;
            }
#endif
#ifdef COMPILE_20761
            case 82: {
              // AddAndReplaceKind
              convert_to_long((result));

              break;
            }
#endif
#ifdef COMPILE_20762
            case 83: {
              // AddAndReplaceKind
              convert_to_long_base((op1), 10);

              break;
            }
#endif
#ifdef COMPILE_20763
            case 84: {
              // AddAndReplaceKind
              convert_to_long_base((op2), 10);

              break;
            }
#endif
#ifdef COMPILE_20764
            case 85: {
              // AddAndReplaceKind
              convert_to_long_base((result), 10);

              break;
            }
#endif
#ifdef COMPILE_20765
            case 86: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_20766
            case 87: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op1));

              break;
            }
#endif
#ifdef COMPILE_20767
            case 88: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op2));

              break;
            }
#endif
#ifdef COMPILE_20768
            case 89: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((result));

              break;
            }
#endif
#ifdef COMPILE_20769
            case 90: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20770
            case 91: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20771
            case 92: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((result)));

              break;
            }
#endif
#ifdef COMPILE_20772
            case 93: {
              // AddAndReplaceKind
              gc_zval_possible_root((op1));

              break;
            }
#endif
#ifdef COMPILE_20773
            case 94: {
              // AddAndReplaceKind
              gc_zval_possible_root((op2));

              break;
            }
#endif
#ifdef COMPILE_20774
            case 95: {
              // AddAndReplaceKind
              gc_zval_possible_root((result));

              break;
            }
#endif
#ifdef COMPILE_20775
            case 96: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_20776
            case 97: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_20777
            case 98: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((result));

              break;
            }
#endif
#ifdef COMPILE_20778
            case 99: {
              // AddAndReplaceKind
              if ((*op1).type != 6) {
                zend_make_printable_zval(op1, &op1_copy, &use_copy1);
              }

              break;
            }
#endif
#ifdef COMPILE_20779
            case 100: {
              // AddAndReplaceKind
              if ((*op2).type != 6) {
                zend_make_printable_zval(op2, &op2_copy, &use_copy2);
              }

              break;
            }
#endif
#ifdef COMPILE_20780
            case 101: {
              // AddAndReplaceKind
              if (result == op1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_20781
            case 102: {
              // AddAndReplaceKind
              if (use_copy1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_20782
            case 103: {
              // AddAndReplaceKind
              if (use_copy2) {
                _zval_dtor((op2));
              }

              break;
            }
#endif
#ifdef COMPILE_20783
            case 104: {
              // AddAndReplaceKind
              if (use_copy2) {
                op2 = &op2_copy;
              }

              break;
            }
#endif
#ifdef COMPILE_20784
            case 105: {
              // AddAndReplaceKind
              memcpy((*(op1)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20785
            case 106: {
              // AddAndReplaceKind
              memcpy((*(op2)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20786
            case 107: {
              // AddAndReplaceKind
              memcpy((*(result)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20787
            case 108: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op1)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20788
            case 109: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op2)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20789
            case 110: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(result)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20790
            case 111: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op1)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20791
            case 112: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op2)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20792
            case 113: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(result)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20793
            case 114: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op1)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20794
            case 115: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op2)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20795
            case 116: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(result)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20796
            case 117: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20797
            case 118: {
              // AddAndReplaceKind
              op1 = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20798
            case 119: {
              // AddAndReplaceKind
              op2 = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20799
            case 120: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "String size overflow");

              break;
            }
#endif
#ifdef COMPILE_20800
            case 121: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Unsupported operand types");

              break;
            }
#endif
#ifdef COMPILE_20801
            case 122: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20802
            case 123: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20803
            case 124: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20804
            case 125: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20805
            case 126: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20806
            case 127: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20807
            case 128: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20808
            case 129: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20809
            case 130: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20810
            case 131: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20811
            case 132: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20812
            case 133: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20813
            case 134: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20814
            case 135: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20815
            case 136: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20816
            case 137: {
              // AddAndReplaceKind
              zend_error((1 << 1L), "Division by zero");

              break;
            }
#endif
#ifdef COMPILE_20817
            case 138: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_20818
            case 139: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20819
            case 140: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_20820
            case 141: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20821
            case 142: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20822
            case 143: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_20823
            case 144: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20824
            case 145: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20825
            case 146: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_20826
            case 147: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20827
            case 148: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op1));

              break;
            }
#endif
#ifdef COMPILE_20828
            case 149: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op2));

              break;
            }
#endif
#ifdef COMPILE_20829
            case 150: {
              // AddAndReplaceKind
              zend_free_obj_get_result((result));

              break;
            }
#endif
#ifdef COMPILE_20830
            case 151: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_20831
            case 152: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_20832
            case 153: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_20833
            case 154: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_20834
            case 155: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_20835
            case 156: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_20836
            case 157: {
              // AddAndReplaceKind
              zend_make_printable_zval(op1, &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_20837
            case 158: {
              // AddAndReplaceKind
              zend_make_printable_zval(op2, &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_20838
            case 159: {
              // AddAndReplaceKind
              zval_unset_isref_p((op1));

              break;
            }
#endif
#ifdef COMPILE_20839
            case 160: {
              // AddAndReplaceKind
              zval_unset_isref_p((op2));

              break;
            }
#endif
#ifdef COMPILE_20840
            case 161: {
              // AddAndReplaceKind
              zval_unset_isref_p((result));

              break;
            }
#endif
#ifdef COMPILE_20841
            case 162: {
              // AddInitKind
              memset(&(*result), 0, sizeof(*(&(*result))));

              break;
            }
#endif
#ifdef COMPILE_20842
            case 163: {
              // AddInitKind
              memset(&(*result).value, 0, sizeof(*(&(*result).value)));

              break;
            }
#endif
#ifdef COMPILE_20843
            case 164: {
              // AddInitKind
              memset(&(*result).value.str, 0, sizeof(*(&(*result).value.str)));

              break;
            }
#endif
#ifdef COMPILE_20844
            case 165: {
              // IfExitKind
              if (__is_neg("218-165", 11, &(result), sizeof(result), &(res_len),
                           sizeof(res_len), &((*result).value.str.val),
                           sizeof((*result).value.str.val), &(op2), sizeof(op2),
                           &((*result).value.str.len),
                           sizeof((*result).value.str.len), &(op1), sizeof(op1),
                           &(use_copy2), sizeof(use_copy2), &(use_copy1),
                           sizeof(use_copy1), &((*result).refcount__gc),
                           sizeof((*result).refcount__gc), &((*result).type),
                           sizeof((*result).type), &((*result).is_ref__gc),
                           sizeof((*result).is_ref__gc)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_20845
            case 166: {
              // IfExitKind
              if (__is_neg("218-166", 11, &(result), sizeof(result), &(res_len),
                           sizeof(res_len), &((*result).value.str.val),
                           sizeof((*result).value.str.val), &(op2), sizeof(op2),
                           &((*result).value.str.len),
                           sizeof((*result).value.str.len), &(op1), sizeof(op1),
                           &(use_copy2), sizeof(use_copy2), &(use_copy1),
                           sizeof(use_copy1), &((*result).refcount__gc),
                           sizeof((*result).refcount__gc), &((*result).type),
                           sizeof((*result).type), &((*result).is_ref__gc),
                           sizeof((*result).is_ref__gc)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_20846
            case 167: {
              // IfExitKind
              if (__is_neg("218-167", 11, &(result), sizeof(result), &(res_len),
                           sizeof(res_len), &((*result).value.str.val),
                           sizeof((*result).value.str.val), &(op2), sizeof(op2),
                           &((*result).value.str.len),
                           sizeof((*result).value.str.len), &(op1), sizeof(op1),
                           &(use_copy2), sizeof(use_copy2), &(use_copy1),
                           sizeof(use_copy1), &((*result).refcount__gc),
                           sizeof((*result).refcount__gc), &((*result).type),
                           sizeof((*result).type), &((*result).is_ref__gc),
                           sizeof((*result).is_ref__gc)))
                return 6;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH219")) {
            case 0: {
              Z_STRVAL_P(result) = erealloc(Z_STRVAL_P(result), res_len + 1);

              break;
            }
#ifdef COMPILE_20847
            case 1: {
              // GuardKind
              if (!__is_neg("219-1", 11, &(result), sizeof(result), &(res_len),
                            sizeof(res_len), &((*result).value.str.val),
                            sizeof((*result).value.str.val), &(op2),
                            sizeof(op2), &((*result).value.str.len),
                            sizeof((*result).value.str.len), &(op1),
                            sizeof(op1), &(use_copy2), sizeof(use_copy2),
                            &(use_copy1), sizeof(use_copy1),
                            &((*result).refcount__gc),
                            sizeof((*result).refcount__gc), &((*result).type),
                            sizeof((*result).type), &((*result).is_ref__gc),
                            sizeof((*result).is_ref__gc)))
                (*result).value.str.val =
                    _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20848
            case 2: {
              // ReplaceKind
              (*(op1)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20849
            case 3: {
              // ReplaceKind
              (*(op2)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20850
            case 4: {
              // ReplaceKind
              (*(result)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20851
            case 5: {
              // ReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20852
            case 6: {
              // ReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20853
            case 7: {
              // ReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(result)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
            }
          }

          {
            switch (__choose("__SWITCH220")) {
            case 0:
              break;
#ifdef COMPILE_20854
            case 1: {
              // AddAndReplaceKind
              (*(op1)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20855
            case 2: {
              // AddAndReplaceKind
              (*(op1)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20856
            case 3: {
              // AddAndReplaceKind
              (*(op1)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20857
            case 4: {
              // AddAndReplaceKind
              (*(op1)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20858
            case 5: {
              // AddAndReplaceKind
              (*(op1)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20859
            case 6: {
              // AddAndReplaceKind
              (*(op1)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20860
            case 7: {
              // AddAndReplaceKind
              (*(op2)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20861
            case 8: {
              // AddAndReplaceKind
              (*(op2)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20862
            case 9: {
              // AddAndReplaceKind
              (*(op2)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20863
            case 10: {
              // AddAndReplaceKind
              (*(op2)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20864
            case 11: {
              // AddAndReplaceKind
              (*(op2)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20865
            case 12: {
              // AddAndReplaceKind
              (*(op2)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20866
            case 13: {
              // AddAndReplaceKind
              (*(result)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_20867
            case 14: {
              // AddAndReplaceKind
              (*(result)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_20868
            case 15: {
              // AddAndReplaceKind
              (*(result)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20869
            case 16: {
              // AddAndReplaceKind
              (*(result)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_20870
            case 17: {
              // AddAndReplaceKind
              (*(result)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20871
            case 18: {
              // AddAndReplaceKind
              (*(result)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20872
            case 19: {
              // AddAndReplaceKind
              (*result).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_20873
            case 20: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20874
            case 21: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20875
            case 22: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(result)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20876
            case 23: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_20877
            case 24: {
              // AddAndReplaceKind
              (*result).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_20878
            case 25: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_20879
            case 26: {
              // AddAndReplaceKind
              (op1) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20880
            case 27: {
              // AddAndReplaceKind
              (op1) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20881
            case 28: {
              // AddAndReplaceKind
              (op2) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20882
            case 29: {
              // AddAndReplaceKind
              (op2) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20883
            case 30: {
              // AddAndReplaceKind
              (result) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20884
            case 31: {
              // AddAndReplaceKind
              (result) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20885
            case 32: {
              // AddAndReplaceKind
              _convert_to_string(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20886
            case 33: {
              // AddAndReplaceKind
              _convert_to_string(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20887
            case 34: {
              // AddAndReplaceKind
              _convert_to_string(((result)));

              break;
            }
#endif
#ifdef COMPILE_20888
            case 35: {
              // AddAndReplaceKind
              _efree(((*(op1)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20889
            case 36: {
              // AddAndReplaceKind
              _efree(((*(op2)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20890
            case 37: {
              // AddAndReplaceKind
              _efree(((*(result)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20891
            case 38: {
              // AddAndReplaceKind
              _efree(((*result).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_20892
            case 39: {
              // AddAndReplaceKind
              _efree(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20893
            case 40: {
              // AddAndReplaceKind
              _efree(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20894
            case 41: {
              // AddAndReplaceKind
              _efree(((result)));

              break;
            }
#endif
#ifdef COMPILE_20895
            case 42: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20896
            case 43: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20897
            case 44: {
              // AddAndReplaceKind
              _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_20898
            case 45: {
              // AddAndReplaceKind
              _zend_list_delete((*(op1)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20899
            case 46: {
              // AddAndReplaceKind
              _zend_list_delete((*(op2)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20900
            case 47: {
              // AddAndReplaceKind
              _zend_list_delete((*(result)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_20901
            case 48: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20902
            case 49: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20903
            case 50: {
              // AddAndReplaceKind
              _zval_copy_ctor(((result)));

              break;
            }
#endif
#ifdef COMPILE_20904
            case 51: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_20905
            case 52: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_20906
            case 53: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((result));

              break;
            }
#endif
#ifdef COMPILE_20907
            case 54: {
              // AddAndReplaceKind
              _zval_dtor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20908
            case 55: {
              // AddAndReplaceKind
              _zval_dtor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20909
            case 56: {
              // AddAndReplaceKind
              _zval_dtor(((result)));

              break;
            }
#endif
#ifdef COMPILE_20910
            case 57: {
              // AddAndReplaceKind
              _zval_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_20911
            case 58: {
              // AddAndReplaceKind
              _zval_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_20912
            case 59: {
              // AddAndReplaceKind
              _zval_dtor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_20913
            case 60: {
              // AddAndReplaceKind
              _zval_dtor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_20914
            case 61: {
              // AddAndReplaceKind
              _zval_dtor_func((result));

              break;
            }
#endif
#ifdef COMPILE_20915
            case 62: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op1)));

              break;
            }
#endif
#ifdef COMPILE_20916
            case 63: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op2)));

              break;
            }
#endif
#ifdef COMPILE_20917
            case 64: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(result)));

              break;
            }
#endif
#ifdef COMPILE_20918
            case 65: {
              // AddAndReplaceKind
              convert_scalar_to_array((op1), 4);

              break;
            }
#endif
#ifdef COMPILE_20919
            case 66: {
              // AddAndReplaceKind
              convert_scalar_to_array((op2), 4);

              break;
            }
#endif
#ifdef COMPILE_20920
            case 67: {
              // AddAndReplaceKind
              convert_scalar_to_array((result), 4);

              break;
            }
#endif
#ifdef COMPILE_20921
            case 68: {
              // AddAndReplaceKind
              convert_scalar_to_number((op1));

              break;
            }
#endif
#ifdef COMPILE_20922
            case 69: {
              // AddAndReplaceKind
              convert_scalar_to_number((op2));

              break;
            }
#endif
#ifdef COMPILE_20923
            case 70: {
              // AddAndReplaceKind
              convert_scalar_to_number((result));

              break;
            }
#endif
#ifdef COMPILE_20924
            case 71: {
              // AddAndReplaceKind
              convert_to_array((op1));

              break;
            }
#endif
#ifdef COMPILE_20925
            case 72: {
              // AddAndReplaceKind
              convert_to_array((op2));

              break;
            }
#endif
#ifdef COMPILE_20926
            case 73: {
              // AddAndReplaceKind
              convert_to_array((result));

              break;
            }
#endif
#ifdef COMPILE_20927
            case 74: {
              // AddAndReplaceKind
              convert_to_boolean((op1));

              break;
            }
#endif
#ifdef COMPILE_20928
            case 75: {
              // AddAndReplaceKind
              convert_to_boolean((op2));

              break;
            }
#endif
#ifdef COMPILE_20929
            case 76: {
              // AddAndReplaceKind
              convert_to_boolean((result));

              break;
            }
#endif
#ifdef COMPILE_20930
            case 77: {
              // AddAndReplaceKind
              convert_to_double((op1));

              break;
            }
#endif
#ifdef COMPILE_20931
            case 78: {
              // AddAndReplaceKind
              convert_to_double((op2));

              break;
            }
#endif
#ifdef COMPILE_20932
            case 79: {
              // AddAndReplaceKind
              convert_to_double((result));

              break;
            }
#endif
#ifdef COMPILE_20933
            case 80: {
              // AddAndReplaceKind
              convert_to_long((op1));

              break;
            }
#endif
#ifdef COMPILE_20934
            case 81: {
              // AddAndReplaceKind
              convert_to_long((op2));

              break;
            }
#endif
#ifdef COMPILE_20935
            case 82: {
              // AddAndReplaceKind
              convert_to_long((result));

              break;
            }
#endif
#ifdef COMPILE_20936
            case 83: {
              // AddAndReplaceKind
              convert_to_long_base((op1), 10);

              break;
            }
#endif
#ifdef COMPILE_20937
            case 84: {
              // AddAndReplaceKind
              convert_to_long_base((op2), 10);

              break;
            }
#endif
#ifdef COMPILE_20938
            case 85: {
              // AddAndReplaceKind
              convert_to_long_base((result), 10);

              break;
            }
#endif
#ifdef COMPILE_20939
            case 86: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_20940
            case 87: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op1));

              break;
            }
#endif
#ifdef COMPILE_20941
            case 88: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op2));

              break;
            }
#endif
#ifdef COMPILE_20942
            case 89: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((result));

              break;
            }
#endif
#ifdef COMPILE_20943
            case 90: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op1)));

              break;
            }
#endif
#ifdef COMPILE_20944
            case 91: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op2)));

              break;
            }
#endif
#ifdef COMPILE_20945
            case 92: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((result)));

              break;
            }
#endif
#ifdef COMPILE_20946
            case 93: {
              // AddAndReplaceKind
              gc_zval_possible_root((op1));

              break;
            }
#endif
#ifdef COMPILE_20947
            case 94: {
              // AddAndReplaceKind
              gc_zval_possible_root((op2));

              break;
            }
#endif
#ifdef COMPILE_20948
            case 95: {
              // AddAndReplaceKind
              gc_zval_possible_root((result));

              break;
            }
#endif
#ifdef COMPILE_20949
            case 96: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_20950
            case 97: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_20951
            case 98: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((result));

              break;
            }
#endif
#ifdef COMPILE_20952
            case 99: {
              // AddAndReplaceKind
              if ((*op1).type != 6) {
                zend_make_printable_zval(op1, &op1_copy, &use_copy1);
              }

              break;
            }
#endif
#ifdef COMPILE_20953
            case 100: {
              // AddAndReplaceKind
              if ((*op2).type != 6) {
                zend_make_printable_zval(op2, &op2_copy, &use_copy2);
              }

              break;
            }
#endif
#ifdef COMPILE_20954
            case 101: {
              // AddAndReplaceKind
              if (result == op1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_20955
            case 102: {
              // AddAndReplaceKind
              if (use_copy1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_20956
            case 103: {
              // AddAndReplaceKind
              if (use_copy2) {
                _zval_dtor((op2));
              }

              break;
            }
#endif
#ifdef COMPILE_20957
            case 104: {
              // AddAndReplaceKind
              if (use_copy2) {
                op2 = &op2_copy;
              }

              break;
            }
#endif
#ifdef COMPILE_20958
            case 105: {
              // AddAndReplaceKind
              memcpy((*(op1)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20959
            case 106: {
              // AddAndReplaceKind
              memcpy((*(op2)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20960
            case 107: {
              // AddAndReplaceKind
              memcpy((*(result)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20961
            case 108: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op1)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20962
            case 109: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op2)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20963
            case 110: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(result)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20964
            case 111: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op1)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20965
            case 112: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op2)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20966
            case 113: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(result)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20967
            case 114: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op1)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20968
            case 115: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op2)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20969
            case 116: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(result)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20970
            case 117: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_20971
            case 118: {
              // AddAndReplaceKind
              op1 = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_20972
            case 119: {
              // AddAndReplaceKind
              op2 = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_20973
            case 120: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "String size overflow");

              break;
            }
#endif
#ifdef COMPILE_20974
            case 121: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Unsupported operand types");

              break;
            }
#endif
#ifdef COMPILE_20975
            case 122: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20976
            case 123: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20977
            case 124: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20978
            case 125: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20979
            case 126: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20980
            case 127: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20981
            case 128: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20982
            case 129: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20983
            case 130: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20984
            case 131: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20985
            case 132: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_20986
            case 133: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_20987
            case 134: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_20988
            case 135: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_20989
            case 136: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_20990
            case 137: {
              // AddAndReplaceKind
              zend_error((1 << 1L), "Division by zero");

              break;
            }
#endif
#ifdef COMPILE_20991
            case 138: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_20992
            case 139: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20993
            case 140: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_20994
            case 141: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20995
            case 142: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20996
            case 143: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_20997
            case 144: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_20998
            case 145: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_20999
            case 146: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_21000
            case 147: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_21001
            case 148: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op1));

              break;
            }
#endif
#ifdef COMPILE_21002
            case 149: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op2));

              break;
            }
#endif
#ifdef COMPILE_21003
            case 150: {
              // AddAndReplaceKind
              zend_free_obj_get_result((result));

              break;
            }
#endif
#ifdef COMPILE_21004
            case 151: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21005
            case 152: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21006
            case 153: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21007
            case 154: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21008
            case 155: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21009
            case 156: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21010
            case 157: {
              // AddAndReplaceKind
              zend_make_printable_zval(op1, &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21011
            case 158: {
              // AddAndReplaceKind
              zend_make_printable_zval(op2, &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21012
            case 159: {
              // AddAndReplaceKind
              zval_unset_isref_p((op1));

              break;
            }
#endif
#ifdef COMPILE_21013
            case 160: {
              // AddAndReplaceKind
              zval_unset_isref_p((op2));

              break;
            }
#endif
#ifdef COMPILE_21014
            case 161: {
              // AddAndReplaceKind
              zval_unset_isref_p((result));

              break;
            }
#endif
#ifdef COMPILE_21015
            case 162: {
              // AddInitKind
              memset(&(*op2), 0, sizeof(*(&(*op2))));

              break;
            }
#endif
#ifdef COMPILE_21016
            case 163: {
              // AddInitKind
              memset(&(*op2).value, 0, sizeof(*(&(*op2).value)));

              break;
            }
#endif
#ifdef COMPILE_21017
            case 164: {
              // AddInitKind
              memset(&(*op2).value.str, 0, sizeof(*(&(*op2).value.str)));

              break;
            }
#endif
#ifdef COMPILE_21018
            case 165: {
              // AddInitKind
              memset(&(*result), 0, sizeof(*(&(*result))));

              break;
            }
#endif
#ifdef COMPILE_21019
            case 166: {
              // AddInitKind
              memset(&(*result).value, 0, sizeof(*(&(*result).value)));

              break;
            }
#endif
#ifdef COMPILE_21020
            case 167: {
              // AddInitKind
              memset(&(*result).value.str, 0, sizeof(*(&(*result).value.str)));

              break;
            }
#endif
#ifdef COMPILE_21021
            case 168: {
              // IfExitKind
              if (__is_neg(
                      "220-168", 16, &(result), sizeof(result), &(op2),
                      sizeof(op2), &((*result).value.str.val),
                      sizeof((*result).value.str.val),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op2).value.str.val),
                      sizeof((*op2).value.str.val), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &(res_len), sizeof(res_len),
                      &(op1), sizeof(op1), &(use_copy1), sizeof(use_copy1),
                      &(use_copy2), sizeof(use_copy2), &((*op2).type),
                      sizeof((*op2).type), &((*result).refcount__gc),
                      sizeof((*result).refcount__gc), &((*result).type),
                      sizeof((*result).type), &((*result).is_ref__gc),
                      sizeof((*result).is_ref__gc), &((*op2).refcount__gc),
                      sizeof((*op2).refcount__gc), &((*op2).is_ref__gc),
                      sizeof((*op2).is_ref__gc)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_21022
            case 169: {
              // IfExitKind
              if (__is_neg(
                      "220-169", 16, &(result), sizeof(result), &(op2),
                      sizeof(op2), &((*result).value.str.val),
                      sizeof((*result).value.str.val),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op2).value.str.val),
                      sizeof((*op2).value.str.val), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &(res_len), sizeof(res_len),
                      &(op1), sizeof(op1), &(use_copy1), sizeof(use_copy1),
                      &(use_copy2), sizeof(use_copy2), &((*op2).type),
                      sizeof((*op2).type), &((*result).refcount__gc),
                      sizeof((*result).refcount__gc), &((*result).type),
                      sizeof((*result).type), &((*result).is_ref__gc),
                      sizeof((*result).is_ref__gc), &((*op2).refcount__gc),
                      sizeof((*op2).refcount__gc), &((*op2).is_ref__gc),
                      sizeof((*op2).is_ref__gc)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_21023
            case 170: {
              // IfExitKind
              if (__is_neg(
                      "220-170", 16, &(result), sizeof(result), &(op2),
                      sizeof(op2), &((*result).value.str.val),
                      sizeof((*result).value.str.val),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op2).value.str.val),
                      sizeof((*op2).value.str.val), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &(res_len), sizeof(res_len),
                      &(op1), sizeof(op1), &(use_copy1), sizeof(use_copy1),
                      &(use_copy2), sizeof(use_copy2), &((*op2).type),
                      sizeof((*op2).type), &((*result).refcount__gc),
                      sizeof((*result).refcount__gc), &((*result).type),
                      sizeof((*result).type), &((*result).is_ref__gc),
                      sizeof((*result).is_ref__gc), &((*op2).refcount__gc),
                      sizeof((*op2).refcount__gc), &((*op2).is_ref__gc),
                      sizeof((*op2).is_ref__gc)))
                return 6;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH221")) {
            case 0: {
              memcpy(Z_STRVAL_P(result) + Z_STRLEN_P(result), Z_STRVAL_P(op2),
                     Z_STRLEN_P(op2));

              break;
            }
#ifdef COMPILE_21024
            case 1: {
              // AddAndReplaceKind
              __builtin_va_start((*result).value.str.val +
                                     (*result).value.str.len,
                                 (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21025
            case 2: {
              // AddAndReplaceKind
              bcmp((*result).value.str.val + (*result).value.str.len,
                   (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21026
            case 3: {
              // AddAndReplaceKind
              memcmp((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21027
            case 4: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21028
            case 5: {
              // AddAndReplaceKind
              memmove((*result).value.str.val + (*result).value.str.len,
                      (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21029
            case 6: {
              // AddAndReplaceKind
              printf((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21030
            case 7: {
              // AddAndReplaceKind
              scanf((*result).value.str.val + (*result).value.str.len,
                    (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21031
            case 8: {
              // GuardKind
              if (!__is_neg(
                      "221-8", 16, &(result), sizeof(result), &(op2),
                      sizeof(op2), &((*result).value.str.val),
                      sizeof((*result).value.str.val),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op2).value.str.val),
                      sizeof((*op2).value.str.val), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &(res_len), sizeof(res_len),
                      &(op1), sizeof(op1), &(use_copy1), sizeof(use_copy1),
                      &(use_copy2), sizeof(use_copy2), &((*op2).type),
                      sizeof((*op2).type), &((*result).refcount__gc),
                      sizeof((*result).refcount__gc), &((*result).type),
                      sizeof((*result).type), &((*result).is_ref__gc),
                      sizeof((*result).is_ref__gc), &((*op2).refcount__gc),
                      sizeof((*op2).refcount__gc), &((*op2).is_ref__gc),
                      sizeof((*op2).is_ref__gc)))
                memcpy((*result).value.str.val + (*result).value.str.len,
                       (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21032
            case 9: {
              // ReplaceKind
              memcpy((*(op1)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21033
            case 10: {
              // ReplaceKind
              memcpy((*(op2)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21034
            case 11: {
              // ReplaceKind
              memcpy((*(result)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21035
            case 12: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*(op1)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21036
            case 13: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*(op2)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21037
            case 14: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*(result)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21038
            case 15: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op1)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21039
            case 16: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op2)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21040
            case 17: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(result)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21041
            case 18: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op1)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21042
            case 19: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op2)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21043
            case 20: {
              // ReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(result)).value.str.len);

              break;
            }
#endif
            }
          }
          {
            switch (__choose("__SWITCH222")) {
            case 0:
              break;
#ifdef COMPILE_21044
            case 1: {
              // AddAndReplaceKind
              (*(op1)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_21045
            case 2: {
              // AddAndReplaceKind
              (*(op1)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_21046
            case 3: {
              // AddAndReplaceKind
              (*(op1)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21047
            case 4: {
              // AddAndReplaceKind
              (*(op1)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_21048
            case 5: {
              // AddAndReplaceKind
              (*(op1)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21049
            case 6: {
              // AddAndReplaceKind
              (*(op1)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21050
            case 7: {
              // AddAndReplaceKind
              (*(op2)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_21051
            case 8: {
              // AddAndReplaceKind
              (*(op2)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_21052
            case 9: {
              // AddAndReplaceKind
              (*(op2)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21053
            case 10: {
              // AddAndReplaceKind
              (*(op2)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_21054
            case 11: {
              // AddAndReplaceKind
              (*(op2)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21055
            case 12: {
              // AddAndReplaceKind
              (*(op2)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21056
            case 13: {
              // AddAndReplaceKind
              (*(result)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_21057
            case 14: {
              // AddAndReplaceKind
              (*(result)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_21058
            case 15: {
              // AddAndReplaceKind
              (*(result)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21059
            case 16: {
              // AddAndReplaceKind
              (*(result)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_21060
            case 17: {
              // AddAndReplaceKind
              (*(result)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21061
            case 18: {
              // AddAndReplaceKind
              (*(result)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21062
            case 19: {
              // AddAndReplaceKind
              (*result).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21063
            case 20: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21064
            case 21: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21065
            case 22: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(result)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21066
            case 23: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21067
            case 24: {
              // AddAndReplaceKind
              (*result).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21068
            case 25: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_21069
            case 26: {
              // AddAndReplaceKind
              (op1) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_21070
            case 27: {
              // AddAndReplaceKind
              (op1) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_21071
            case 28: {
              // AddAndReplaceKind
              (op2) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_21072
            case 29: {
              // AddAndReplaceKind
              (op2) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_21073
            case 30: {
              // AddAndReplaceKind
              (result) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_21074
            case 31: {
              // AddAndReplaceKind
              (result) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_21075
            case 32: {
              // AddAndReplaceKind
              _convert_to_string(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21076
            case 33: {
              // AddAndReplaceKind
              _convert_to_string(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21077
            case 34: {
              // AddAndReplaceKind
              _convert_to_string(((result)));

              break;
            }
#endif
#ifdef COMPILE_21078
            case 35: {
              // AddAndReplaceKind
              _efree(((*(op1)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21079
            case 36: {
              // AddAndReplaceKind
              _efree(((*(op2)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21080
            case 37: {
              // AddAndReplaceKind
              _efree(((*(result)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21081
            case 38: {
              // AddAndReplaceKind
              _efree(((*result).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21082
            case 39: {
              // AddAndReplaceKind
              _efree(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21083
            case 40: {
              // AddAndReplaceKind
              _efree(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21084
            case 41: {
              // AddAndReplaceKind
              _efree(((result)));

              break;
            }
#endif
#ifdef COMPILE_21085
            case 42: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_21086
            case 43: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_21087
            case 44: {
              // AddAndReplaceKind
              _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_21088
            case 45: {
              // AddAndReplaceKind
              _zend_list_delete((*(op1)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_21089
            case 46: {
              // AddAndReplaceKind
              _zend_list_delete((*(op2)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_21090
            case 47: {
              // AddAndReplaceKind
              _zend_list_delete((*(result)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_21091
            case 48: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21092
            case 49: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21093
            case 50: {
              // AddAndReplaceKind
              _zval_copy_ctor(((result)));

              break;
            }
#endif
#ifdef COMPILE_21094
            case 51: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_21095
            case 52: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_21096
            case 53: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((result));

              break;
            }
#endif
#ifdef COMPILE_21097
            case 54: {
              // AddAndReplaceKind
              _zval_dtor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21098
            case 55: {
              // AddAndReplaceKind
              _zval_dtor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21099
            case 56: {
              // AddAndReplaceKind
              _zval_dtor(((result)));

              break;
            }
#endif
#ifdef COMPILE_21100
            case 57: {
              // AddAndReplaceKind
              _zval_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_21101
            case 58: {
              // AddAndReplaceKind
              _zval_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_21102
            case 59: {
              // AddAndReplaceKind
              _zval_dtor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_21103
            case 60: {
              // AddAndReplaceKind
              _zval_dtor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_21104
            case 61: {
              // AddAndReplaceKind
              _zval_dtor_func((result));

              break;
            }
#endif
#ifdef COMPILE_21105
            case 62: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op1)));

              break;
            }
#endif
#ifdef COMPILE_21106
            case 63: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op2)));

              break;
            }
#endif
#ifdef COMPILE_21107
            case 64: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(result)));

              break;
            }
#endif
#ifdef COMPILE_21108
            case 65: {
              // AddAndReplaceKind
              convert_scalar_to_array((op1), 4);

              break;
            }
#endif
#ifdef COMPILE_21109
            case 66: {
              // AddAndReplaceKind
              convert_scalar_to_array((op2), 4);

              break;
            }
#endif
#ifdef COMPILE_21110
            case 67: {
              // AddAndReplaceKind
              convert_scalar_to_array((result), 4);

              break;
            }
#endif
#ifdef COMPILE_21111
            case 68: {
              // AddAndReplaceKind
              convert_scalar_to_number((op1));

              break;
            }
#endif
#ifdef COMPILE_21112
            case 69: {
              // AddAndReplaceKind
              convert_scalar_to_number((op2));

              break;
            }
#endif
#ifdef COMPILE_21113
            case 70: {
              // AddAndReplaceKind
              convert_scalar_to_number((result));

              break;
            }
#endif
#ifdef COMPILE_21114
            case 71: {
              // AddAndReplaceKind
              convert_to_array((op1));

              break;
            }
#endif
#ifdef COMPILE_21115
            case 72: {
              // AddAndReplaceKind
              convert_to_array((op2));

              break;
            }
#endif
#ifdef COMPILE_21116
            case 73: {
              // AddAndReplaceKind
              convert_to_array((result));

              break;
            }
#endif
#ifdef COMPILE_21117
            case 74: {
              // AddAndReplaceKind
              convert_to_boolean((op1));

              break;
            }
#endif
#ifdef COMPILE_21118
            case 75: {
              // AddAndReplaceKind
              convert_to_boolean((op2));

              break;
            }
#endif
#ifdef COMPILE_21119
            case 76: {
              // AddAndReplaceKind
              convert_to_boolean((result));

              break;
            }
#endif
#ifdef COMPILE_21120
            case 77: {
              // AddAndReplaceKind
              convert_to_double((op1));

              break;
            }
#endif
#ifdef COMPILE_21121
            case 78: {
              // AddAndReplaceKind
              convert_to_double((op2));

              break;
            }
#endif
#ifdef COMPILE_21122
            case 79: {
              // AddAndReplaceKind
              convert_to_double((result));

              break;
            }
#endif
#ifdef COMPILE_21123
            case 80: {
              // AddAndReplaceKind
              convert_to_long((op1));

              break;
            }
#endif
#ifdef COMPILE_21124
            case 81: {
              // AddAndReplaceKind
              convert_to_long((op2));

              break;
            }
#endif
#ifdef COMPILE_21125
            case 82: {
              // AddAndReplaceKind
              convert_to_long((result));

              break;
            }
#endif
#ifdef COMPILE_21126
            case 83: {
              // AddAndReplaceKind
              convert_to_long_base((op1), 10);

              break;
            }
#endif
#ifdef COMPILE_21127
            case 84: {
              // AddAndReplaceKind
              convert_to_long_base((op2), 10);

              break;
            }
#endif
#ifdef COMPILE_21128
            case 85: {
              // AddAndReplaceKind
              convert_to_long_base((result), 10);

              break;
            }
#endif
#ifdef COMPILE_21129
            case 86: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_21130
            case 87: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op1));

              break;
            }
#endif
#ifdef COMPILE_21131
            case 88: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op2));

              break;
            }
#endif
#ifdef COMPILE_21132
            case 89: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((result));

              break;
            }
#endif
#ifdef COMPILE_21133
            case 90: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21134
            case 91: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21135
            case 92: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((result)));

              break;
            }
#endif
#ifdef COMPILE_21136
            case 93: {
              // AddAndReplaceKind
              gc_zval_possible_root((op1));

              break;
            }
#endif
#ifdef COMPILE_21137
            case 94: {
              // AddAndReplaceKind
              gc_zval_possible_root((op2));

              break;
            }
#endif
#ifdef COMPILE_21138
            case 95: {
              // AddAndReplaceKind
              gc_zval_possible_root((result));

              break;
            }
#endif
#ifdef COMPILE_21139
            case 96: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_21140
            case 97: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_21141
            case 98: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((result));

              break;
            }
#endif
#ifdef COMPILE_21142
            case 99: {
              // AddAndReplaceKind
              if ((*op1).type != 6) {
                zend_make_printable_zval(op1, &op1_copy, &use_copy1);
              }

              break;
            }
#endif
#ifdef COMPILE_21143
            case 100: {
              // AddAndReplaceKind
              if ((*op2).type != 6) {
                zend_make_printable_zval(op2, &op2_copy, &use_copy2);
              }

              break;
            }
#endif
#ifdef COMPILE_21144
            case 101: {
              // AddAndReplaceKind
              if (result == op1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_21145
            case 102: {
              // AddAndReplaceKind
              if (use_copy1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_21146
            case 103: {
              // AddAndReplaceKind
              if (use_copy2) {
                _zval_dtor((op2));
              }

              break;
            }
#endif
#ifdef COMPILE_21147
            case 104: {
              // AddAndReplaceKind
              if (use_copy2) {
                op2 = &op2_copy;
              }

              break;
            }
#endif
#ifdef COMPILE_21148
            case 105: {
              // AddAndReplaceKind
              memcpy((*(op1)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21149
            case 106: {
              // AddAndReplaceKind
              memcpy((*(op2)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21150
            case 107: {
              // AddAndReplaceKind
              memcpy((*(result)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21151
            case 108: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op1)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21152
            case 109: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op2)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21153
            case 110: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(result)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21154
            case 111: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op1)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21155
            case 112: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op2)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21156
            case 113: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(result)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21157
            case 114: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op1)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21158
            case 115: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op2)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21159
            case 116: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(result)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21160
            case 117: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21161
            case 118: {
              // AddAndReplaceKind
              op1 = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_21162
            case 119: {
              // AddAndReplaceKind
              op2 = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_21163
            case 120: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "String size overflow");

              break;
            }
#endif
#ifdef COMPILE_21164
            case 121: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Unsupported operand types");

              break;
            }
#endif
#ifdef COMPILE_21165
            case 122: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_21166
            case 123: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_21167
            case 124: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_21168
            case 125: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_21169
            case 126: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_21170
            case 127: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_21171
            case 128: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_21172
            case 129: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_21173
            case 130: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_21174
            case 131: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_21175
            case 132: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_21176
            case 133: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_21177
            case 134: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_21178
            case 135: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_21179
            case 136: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_21180
            case 137: {
              // AddAndReplaceKind
              zend_error((1 << 1L), "Division by zero");

              break;
            }
#endif
#ifdef COMPILE_21181
            case 138: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_21182
            case 139: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_21183
            case 140: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_21184
            case 141: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_21185
            case 142: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_21186
            case 143: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_21187
            case 144: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_21188
            case 145: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_21189
            case 146: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_21190
            case 147: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_21191
            case 148: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op1));

              break;
            }
#endif
#ifdef COMPILE_21192
            case 149: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op2));

              break;
            }
#endif
#ifdef COMPILE_21193
            case 150: {
              // AddAndReplaceKind
              zend_free_obj_get_result((result));

              break;
            }
#endif
#ifdef COMPILE_21194
            case 151: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21195
            case 152: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21196
            case 153: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21197
            case 154: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21198
            case 155: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21199
            case 156: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21200
            case 157: {
              // AddAndReplaceKind
              zend_make_printable_zval(op1, &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21201
            case 158: {
              // AddAndReplaceKind
              zend_make_printable_zval(op2, &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21202
            case 159: {
              // AddAndReplaceKind
              zval_unset_isref_p((op1));

              break;
            }
#endif
#ifdef COMPILE_21203
            case 160: {
              // AddAndReplaceKind
              zval_unset_isref_p((op2));

              break;
            }
#endif
#ifdef COMPILE_21204
            case 161: {
              // AddAndReplaceKind
              zval_unset_isref_p((result));

              break;
            }
#endif
#ifdef COMPILE_21205
            case 162: {
              // AddInitKind
              memset(&(*result), 0, sizeof(*(&(*result))));

              break;
            }
#endif
#ifdef COMPILE_21206
            case 163: {
              // AddInitKind
              memset(&(*result).value, 0, sizeof(*(&(*result).value)));

              break;
            }
#endif
#ifdef COMPILE_21207
            case 164: {
              // AddInitKind
              memset(&(*result).value.str, 0, sizeof(*(&(*result).value.str)));

              break;
            }
#endif
#ifdef COMPILE_21208
            case 165: {
              // IfExitKind
              if (__is_neg(
                      "222-165", 16, &(result), sizeof(result), &(res_len),
                      sizeof(res_len), &((*result).value.str.val),
                      sizeof((*result).value.str.val), &(op2), sizeof(op2),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op2).value.str.val),
                      sizeof((*op2).value.str.val), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &(op1), sizeof(op1),
                      &(use_copy1), sizeof(use_copy1), &(use_copy2),
                      sizeof(use_copy2), &((*op2).type), sizeof((*op2).type),
                      &((*result).refcount__gc), sizeof((*result).refcount__gc),
                      &((*result).type), sizeof((*result).type),
                      &((*result).is_ref__gc), sizeof((*result).is_ref__gc),
                      &((*op2).refcount__gc), sizeof((*op2).refcount__gc),
                      &((*op2).is_ref__gc), sizeof((*op2).is_ref__gc)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_21209
            case 166: {
              // IfExitKind
              if (__is_neg(
                      "222-166", 16, &(result), sizeof(result), &(res_len),
                      sizeof(res_len), &((*result).value.str.val),
                      sizeof((*result).value.str.val), &(op2), sizeof(op2),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op2).value.str.val),
                      sizeof((*op2).value.str.val), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &(op1), sizeof(op1),
                      &(use_copy1), sizeof(use_copy1), &(use_copy2),
                      sizeof(use_copy2), &((*op2).type), sizeof((*op2).type),
                      &((*result).refcount__gc), sizeof((*result).refcount__gc),
                      &((*result).type), sizeof((*result).type),
                      &((*result).is_ref__gc), sizeof((*result).is_ref__gc),
                      &((*op2).refcount__gc), sizeof((*op2).refcount__gc),
                      &((*op2).is_ref__gc), sizeof((*op2).is_ref__gc)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_21210
            case 167: {
              // IfExitKind
              if (__is_neg(
                      "222-167", 16, &(result), sizeof(result), &(res_len),
                      sizeof(res_len), &((*result).value.str.val),
                      sizeof((*result).value.str.val), &(op2), sizeof(op2),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op2).value.str.val),
                      sizeof((*op2).value.str.val), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &(op1), sizeof(op1),
                      &(use_copy1), sizeof(use_copy1), &(use_copy2),
                      sizeof(use_copy2), &((*op2).type), sizeof((*op2).type),
                      &((*result).refcount__gc), sizeof((*result).refcount__gc),
                      &((*result).type), sizeof((*result).type),
                      &((*result).is_ref__gc), sizeof((*result).is_ref__gc),
                      &((*op2).refcount__gc), sizeof((*op2).refcount__gc),
                      &((*op2).is_ref__gc), sizeof((*op2).is_ref__gc)))
                return 6;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH223")) {
            case 0: {
              Z_STRVAL_P(result)[res_len] = 0;

              break;
            }
#ifdef COMPILE_21211
            case 1: {
              // GuardKind
              if (!__is_neg(
                      "223-1", 16, &(result), sizeof(result), &(res_len),
                      sizeof(res_len), &((*result).value.str.val),
                      sizeof((*result).value.str.val), &(op2), sizeof(op2),
                      &((*result).value.str.len),
                      sizeof((*result).value.str.len), &((*op2).value.str.val),
                      sizeof((*op2).value.str.val), &((*op2).value.str.len),
                      sizeof((*op2).value.str.len), &(op1), sizeof(op1),
                      &(use_copy1), sizeof(use_copy1), &(use_copy2),
                      sizeof(use_copy2), &((*op2).type), sizeof((*op2).type),
                      &((*result).refcount__gc), sizeof((*result).refcount__gc),
                      &((*result).type), sizeof((*result).type),
                      &((*result).is_ref__gc), sizeof((*result).is_ref__gc),
                      &((*op2).refcount__gc), sizeof((*op2).refcount__gc),
                      &((*op2).is_ref__gc), sizeof((*op2).is_ref__gc)))
                (*result).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21212
            case 2: {
              // ReplaceKind
              (*(op1)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21213
            case 3: {
              // ReplaceKind
              (*(op2)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21214
            case 4: {
              // ReplaceKind
              (*(result)).value.str.val[res_len] = 0;

              break;
            }
#endif
            }
          }
          {
            switch (__choose("__SWITCH224")) {
            case 0:
              break;
#ifdef COMPILE_21215
            case 1: {
              // AddAndReplaceKind
              (*(op1)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_21216
            case 2: {
              // AddAndReplaceKind
              (*(op1)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_21217
            case 3: {
              // AddAndReplaceKind
              (*(op1)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21218
            case 4: {
              // AddAndReplaceKind
              (*(op1)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_21219
            case 5: {
              // AddAndReplaceKind
              (*(op1)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21220
            case 6: {
              // AddAndReplaceKind
              (*(op1)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21221
            case 7: {
              // AddAndReplaceKind
              (*(op2)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_21222
            case 8: {
              // AddAndReplaceKind
              (*(op2)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_21223
            case 9: {
              // AddAndReplaceKind
              (*(op2)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21224
            case 10: {
              // AddAndReplaceKind
              (*(op2)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_21225
            case 11: {
              // AddAndReplaceKind
              (*(op2)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21226
            case 12: {
              // AddAndReplaceKind
              (*(op2)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21227
            case 13: {
              // AddAndReplaceKind
              (*(result)).value.lval++;

              break;
            }
#endif
#ifdef COMPILE_21228
            case 14: {
              // AddAndReplaceKind
              (*(result)).value.lval--;

              break;
            }
#endif
#ifdef COMPILE_21229
            case 15: {
              // AddAndReplaceKind
              (*(result)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21230
            case 16: {
              // AddAndReplaceKind
              (*(result)).value.str.len++;

              break;
            }
#endif
#ifdef COMPILE_21231
            case 17: {
              // AddAndReplaceKind
              (*(result)).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21232
            case 18: {
              // AddAndReplaceKind
              (*(result)).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21233
            case 19: {
              // AddAndReplaceKind
              (*result).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21234
            case 20: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21235
            case 21: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21236
            case 22: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*(result)).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21237
            case 23: {
              // AddAndReplaceKind
              (*result).value.str.val =
                  _erealloc(((*result).value.str.val), (res_len + 1), 0);

              break;
            }
#endif
#ifdef COMPILE_21238
            case 24: {
              // AddAndReplaceKind
              (*result).value.str.val[res_len] = 0;

              break;
            }
#endif
#ifdef COMPILE_21239
            case 25: {
              // AddAndReplaceKind
              (executor_globals.argument_stack) =
                  zend_vm_stack_new_page(((16 * 1024) - 16));

              break;
            }
#endif
#ifdef COMPILE_21240
            case 26: {
              // AddAndReplaceKind
              (op1) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_21241
            case 27: {
              // AddAndReplaceKind
              (op1) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_21242
            case 28: {
              // AddAndReplaceKind
              (op2) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_21243
            case 29: {
              // AddAndReplaceKind
              (op2) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_21244
            case 30: {
              // AddAndReplaceKind
              (result) = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_21245
            case 31: {
              // AddAndReplaceKind
              (result) = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_21246
            case 32: {
              // AddAndReplaceKind
              _convert_to_string(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21247
            case 33: {
              // AddAndReplaceKind
              _convert_to_string(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21248
            case 34: {
              // AddAndReplaceKind
              _convert_to_string(((result)));

              break;
            }
#endif
#ifdef COMPILE_21249
            case 35: {
              // AddAndReplaceKind
              _efree(((*(op1)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21250
            case 36: {
              // AddAndReplaceKind
              _efree(((*(op2)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21251
            case 37: {
              // AddAndReplaceKind
              _efree(((*(result)).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21252
            case 38: {
              // AddAndReplaceKind
              _efree(((*result).value.str.val));

              break;
            }
#endif
#ifdef COMPILE_21253
            case 39: {
              // AddAndReplaceKind
              _efree(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21254
            case 40: {
              // AddAndReplaceKind
              _efree(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21255
            case 41: {
              // AddAndReplaceKind
              _efree(((result)));

              break;
            }
#endif
#ifdef COMPILE_21256
            case 42: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_21257
            case 43: {
              // AddAndReplaceKind
              _zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_21258
            case 44: {
              // AddAndReplaceKind
              _zend_hash_init(((*(result)).value.ht), (0), (((void *)0)),
                              ((void (*)(void *))_zval_ptr_dtor), (0));

              break;
            }
#endif
#ifdef COMPILE_21259
            case 45: {
              // AddAndReplaceKind
              _zend_list_delete((*(op1)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_21260
            case 46: {
              // AddAndReplaceKind
              _zend_list_delete((*(op2)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_21261
            case 47: {
              // AddAndReplaceKind
              _zend_list_delete((*(result)).value.lval);

              break;
            }
#endif
#ifdef COMPILE_21262
            case 48: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21263
            case 49: {
              // AddAndReplaceKind
              _zval_copy_ctor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21264
            case 50: {
              // AddAndReplaceKind
              _zval_copy_ctor(((result)));

              break;
            }
#endif
#ifdef COMPILE_21265
            case 51: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_21266
            case 52: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_21267
            case 53: {
              // AddAndReplaceKind
              _zval_copy_ctor_func((result));

              break;
            }
#endif
#ifdef COMPILE_21268
            case 54: {
              // AddAndReplaceKind
              _zval_dtor(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21269
            case 55: {
              // AddAndReplaceKind
              _zval_dtor(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21270
            case 56: {
              // AddAndReplaceKind
              _zval_dtor(((result)));

              break;
            }
#endif
#ifdef COMPILE_21271
            case 57: {
              // AddAndReplaceKind
              _zval_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_21272
            case 58: {
              // AddAndReplaceKind
              _zval_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_21273
            case 59: {
              // AddAndReplaceKind
              _zval_dtor_func((op1));

              break;
            }
#endif
#ifdef COMPILE_21274
            case 60: {
              // AddAndReplaceKind
              _zval_dtor_func((op2));

              break;
            }
#endif
#ifdef COMPILE_21275
            case 61: {
              // AddAndReplaceKind
              _zval_dtor_func((result));

              break;
            }
#endif
#ifdef COMPILE_21276
            case 62: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op1)));

              break;
            }
#endif
#ifdef COMPILE_21277
            case 63: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(op2)));

              break;
            }
#endif
#ifdef COMPILE_21278
            case 64: {
              // AddAndReplaceKind
              _zval_ptr_dtor((&(result)));

              break;
            }
#endif
#ifdef COMPILE_21279
            case 65: {
              // AddAndReplaceKind
              convert_scalar_to_array((op1), 4);

              break;
            }
#endif
#ifdef COMPILE_21280
            case 66: {
              // AddAndReplaceKind
              convert_scalar_to_array((op2), 4);

              break;
            }
#endif
#ifdef COMPILE_21281
            case 67: {
              // AddAndReplaceKind
              convert_scalar_to_array((result), 4);

              break;
            }
#endif
#ifdef COMPILE_21282
            case 68: {
              // AddAndReplaceKind
              convert_scalar_to_number((op1));

              break;
            }
#endif
#ifdef COMPILE_21283
            case 69: {
              // AddAndReplaceKind
              convert_scalar_to_number((op2));

              break;
            }
#endif
#ifdef COMPILE_21284
            case 70: {
              // AddAndReplaceKind
              convert_scalar_to_number((result));

              break;
            }
#endif
#ifdef COMPILE_21285
            case 71: {
              // AddAndReplaceKind
              convert_to_array((op1));

              break;
            }
#endif
#ifdef COMPILE_21286
            case 72: {
              // AddAndReplaceKind
              convert_to_array((op2));

              break;
            }
#endif
#ifdef COMPILE_21287
            case 73: {
              // AddAndReplaceKind
              convert_to_array((result));

              break;
            }
#endif
#ifdef COMPILE_21288
            case 74: {
              // AddAndReplaceKind
              convert_to_boolean((op1));

              break;
            }
#endif
#ifdef COMPILE_21289
            case 75: {
              // AddAndReplaceKind
              convert_to_boolean((op2));

              break;
            }
#endif
#ifdef COMPILE_21290
            case 76: {
              // AddAndReplaceKind
              convert_to_boolean((result));

              break;
            }
#endif
#ifdef COMPILE_21291
            case 77: {
              // AddAndReplaceKind
              convert_to_double((op1));

              break;
            }
#endif
#ifdef COMPILE_21292
            case 78: {
              // AddAndReplaceKind
              convert_to_double((op2));

              break;
            }
#endif
#ifdef COMPILE_21293
            case 79: {
              // AddAndReplaceKind
              convert_to_double((result));

              break;
            }
#endif
#ifdef COMPILE_21294
            case 80: {
              // AddAndReplaceKind
              convert_to_long((op1));

              break;
            }
#endif
#ifdef COMPILE_21295
            case 81: {
              // AddAndReplaceKind
              convert_to_long((op2));

              break;
            }
#endif
#ifdef COMPILE_21296
            case 82: {
              // AddAndReplaceKind
              convert_to_long((result));

              break;
            }
#endif
#ifdef COMPILE_21297
            case 83: {
              // AddAndReplaceKind
              convert_to_long_base((op1), 10);

              break;
            }
#endif
#ifdef COMPILE_21298
            case 84: {
              // AddAndReplaceKind
              convert_to_long_base((op2), 10);

              break;
            }
#endif
#ifdef COMPILE_21299
            case 85: {
              // AddAndReplaceKind
              convert_to_long_base((result), 10);

              break;
            }
#endif
#ifdef COMPILE_21300
            case 86: {
              // AddAndReplaceKind
              exit(1);

              break;
            }
#endif
#ifdef COMPILE_21301
            case 87: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op1));

              break;
            }
#endif
#ifdef COMPILE_21302
            case 88: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((op2));

              break;
            }
#endif
#ifdef COMPILE_21303
            case 89: {
              // AddAndReplaceKind
              gc_remove_zval_from_buffer((result));

              break;
            }
#endif
#ifdef COMPILE_21304
            case 90: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op1)));

              break;
            }
#endif
#ifdef COMPILE_21305
            case 91: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((op2)));

              break;
            }
#endif
#ifdef COMPILE_21306
            case 92: {
              // AddAndReplaceKind
              gc_zval_check_possible_root(((result)));

              break;
            }
#endif
#ifdef COMPILE_21307
            case 93: {
              // AddAndReplaceKind
              gc_zval_possible_root((op1));

              break;
            }
#endif
#ifdef COMPILE_21308
            case 94: {
              // AddAndReplaceKind
              gc_zval_possible_root((op2));

              break;
            }
#endif
#ifdef COMPILE_21309
            case 95: {
              // AddAndReplaceKind
              gc_zval_possible_root((result));

              break;
            }
#endif
#ifdef COMPILE_21310
            case 96: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op1));

              break;
            }
#endif
#ifdef COMPILE_21311
            case 97: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((op2));

              break;
            }
#endif
#ifdef COMPILE_21312
            case 98: {
              // AddAndReplaceKind
              i_zval_ptr_dtor((result));

              break;
            }
#endif
#ifdef COMPILE_21313
            case 99: {
              // AddAndReplaceKind
              if ((*op1).type != 6) {
                zend_make_printable_zval(op1, &op1_copy, &use_copy1);
              }

              break;
            }
#endif
#ifdef COMPILE_21314
            case 100: {
              // AddAndReplaceKind
              if ((*op2).type != 6) {
                zend_make_printable_zval(op2, &op2_copy, &use_copy2);
              }

              break;
            }
#endif
#ifdef COMPILE_21315
            case 101: {
              // AddAndReplaceKind
              if (result == op1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_21316
            case 102: {
              // AddAndReplaceKind
              if (use_copy1) {
                _zval_dtor((op1));
              }

              break;
            }
#endif
#ifdef COMPILE_21317
            case 103: {
              // AddAndReplaceKind
              if (use_copy2) {
                _zval_dtor((op2));
              }

              break;
            }
#endif
#ifdef COMPILE_21318
            case 104: {
              // AddAndReplaceKind
              if (use_copy2) {
                op2 = &op2_copy;
              }

              break;
            }
#endif
#ifdef COMPILE_21319
            case 105: {
              // AddAndReplaceKind
              memcpy((*(op1)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21320
            case 106: {
              // AddAndReplaceKind
              memcpy((*(op2)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21321
            case 107: {
              // AddAndReplaceKind
              memcpy((*(result)).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21322
            case 108: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op1)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21323
            case 109: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(op2)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21324
            case 110: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*(result)).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21325
            case 111: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op1)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21326
            case 112: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(op2)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21327
            case 113: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*(result)).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21328
            case 114: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op1)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21329
            case 115: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(op2)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21330
            case 116: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*(result)).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21331
            case 117: {
              // AddAndReplaceKind
              memcpy((*result).value.str.val + (*result).value.str.len,
                     (*op2).value.str.val, (*op2).value.str.len);

              break;
            }
#endif
#ifdef COMPILE_21332
            case 118: {
              // AddAndReplaceKind
              op1 = &op1_copy;

              break;
            }
#endif
#ifdef COMPILE_21333
            case 119: {
              // AddAndReplaceKind
              op2 = &op2_copy;

              break;
            }
#endif
#ifdef COMPILE_21334
            case 120: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "String size overflow");

              break;
            }
#endif
#ifdef COMPILE_21335
            case 121: {
              // AddAndReplaceKind
              zend_error((1 << 0L), "Unsupported operand types");

              break;
            }
#endif
#ifdef COMPILE_21336
            case 122: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_21337
            case 123: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_21338
            case 124: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_21339
            case 125: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_21340
            case 126: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op1)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_21341
            case 127: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_21342
            case 128: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_21343
            case 129: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_21344
            case 130: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_21345
            case 131: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(op2)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_21346
            case 132: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(1));

              break;
            }
#endif
#ifdef COMPILE_21347
            case 133: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(2));

              break;
            }
#endif
#ifdef COMPILE_21348
            case 134: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(3));

              break;
            }
#endif
#ifdef COMPILE_21349
            case 135: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(4));

              break;
            }
#endif
#ifdef COMPILE_21350
            case 136: {
              // AddAndReplaceKind
              zend_error((1 << 12L),
                         "Object of class %s could not be converted to %s",
                         zend_get_class_entry(&(*(result)))->name,
                         zend_get_type_by_const(6));

              break;
            }
#endif
#ifdef COMPILE_21351
            case 137: {
              // AddAndReplaceKind
              zend_error((1 << 1L), "Division by zero");

              break;
            }
#endif
#ifdef COMPILE_21352
            case 138: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "A non well formed numeric value encountered");

              break;
            }
#endif
#ifdef COMPILE_21353
            case 139: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_21354
            case 140: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_21355
            case 141: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to double",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_21356
            case 142: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_21357
            case 143: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_21358
            case 144: {
              // AddAndReplaceKind
              zend_error((1 << 3L),
                         "Object of class %s could not be converted to int",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_21359
            case 145: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op1)))->name);

              break;
            }
#endif
#ifdef COMPILE_21360
            case 146: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(op2)))->name);

              break;
            }
#endif
#ifdef COMPILE_21361
            case 147: {
              // AddAndReplaceKind
              zend_error((1 << 3L), "Object of class %s to string conversion",
                         zend_get_class_entry(&(*(result)))->name);

              break;
            }
#endif
#ifdef COMPILE_21362
            case 148: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op1));

              break;
            }
#endif
#ifdef COMPILE_21363
            case 149: {
              // AddAndReplaceKind
              zend_free_obj_get_result((op2));

              break;
            }
#endif
#ifdef COMPILE_21364
            case 150: {
              // AddAndReplaceKind
              zend_free_obj_get_result((result));

              break;
            }
#endif
#ifdef COMPILE_21365
            case 151: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21366
            case 152: {
              // AddAndReplaceKind
              zend_make_printable_zval((op1), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21367
            case 153: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21368
            case 154: {
              // AddAndReplaceKind
              zend_make_printable_zval((op2), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21369
            case 155: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21370
            case 156: {
              // AddAndReplaceKind
              zend_make_printable_zval((result), &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21371
            case 157: {
              // AddAndReplaceKind
              zend_make_printable_zval(op1, &op1_copy, &use_copy1);

              break;
            }
#endif
#ifdef COMPILE_21372
            case 158: {
              // AddAndReplaceKind
              zend_make_printable_zval(op2, &op2_copy, &use_copy2);

              break;
            }
#endif
#ifdef COMPILE_21373
            case 159: {
              // AddAndReplaceKind
              zval_unset_isref_p((op1));

              break;
            }
#endif
#ifdef COMPILE_21374
            case 160: {
              // AddAndReplaceKind
              zval_unset_isref_p((op2));

              break;
            }
#endif
#ifdef COMPILE_21375
            case 161: {
              // AddAndReplaceKind
              zval_unset_isref_p((result));

              break;
            }
#endif
#ifdef COMPILE_21376
            case 162: {
              // AddInitKind
              memset(&(*result), 0, sizeof(*(&(*result))));

              break;
            }
#endif
#ifdef COMPILE_21377
            case 163: {
              // AddInitKind
              memset(&(*result).value, 0, sizeof(*(&(*result).value)));

              break;
            }
#endif
#ifdef COMPILE_21378
            case 164: {
              // AddInitKind
              memset(&(*result).value.str, 0, sizeof(*(&(*result).value.str)));

              break;
            }
#endif
#ifdef COMPILE_21379
            case 165: {
              // IfExitKind
              if (__is_neg("224-165", 11, &(result), sizeof(result), &(res_len),
                           sizeof(res_len), &((*result).value.str.len),
                           sizeof((*result).value.str.len),
                           &((*result).value.str.val),
                           sizeof((*result).value.str.val), &(op1), sizeof(op1),
                           &(op2), sizeof(op2), &(use_copy1), sizeof(use_copy1),
                           &(use_copy2), sizeof(use_copy2),
                           &((*result).refcount__gc),
                           sizeof((*result).refcount__gc), &((*result).type),
                           sizeof((*result).type), &((*result).is_ref__gc),
                           sizeof((*result).is_ref__gc)))
                return 0;

              break;
            }
#endif
#ifdef COMPILE_21380
            case 166: {
              // IfExitKind
              if (__is_neg("224-166", 11, &(result), sizeof(result), &(res_len),
                           sizeof(res_len), &((*result).value.str.len),
                           sizeof((*result).value.str.len),
                           &((*result).value.str.val),
                           sizeof((*result).value.str.val), &(op1), sizeof(op1),
                           &(op2), sizeof(op2), &(use_copy1), sizeof(use_copy1),
                           &(use_copy2), sizeof(use_copy2),
                           &((*result).refcount__gc),
                           sizeof((*result).refcount__gc), &((*result).type),
                           sizeof((*result).type), &((*result).is_ref__gc),
                           sizeof((*result).is_ref__gc)))
                return 1;

              break;
            }
#endif
#ifdef COMPILE_21381
            case 167: {
              // IfExitKind
              if (__is_neg("224-167", 11, &(result), sizeof(result), &(res_len),
                           sizeof(res_len), &((*result).value.str.len),
                           sizeof((*result).value.str.len),
                           &((*result).value.str.val),
                           sizeof((*result).value.str.val), &(op1), sizeof(op1),
                           &(op2), sizeof(op2), &(use_copy1), sizeof(use_copy1),
                           &(use_copy2), sizeof(use_copy2),
                           &((*result).refcount__gc),
                           sizeof((*result).refcount__gc), &((*result).type),
                           sizeof((*result).type), &((*result).is_ref__gc),
                           sizeof((*result).is_ref__gc)))
                return 6;

              break;
            }
#endif
            }
            switch (__choose("__SWITCH225")) {
            case 0: {
              Z_STRLEN_P(result) = res_len;

              break;
            }
#ifdef COMPILE_21382
            case 1: {
              // GuardKind
              if (!__is_neg("225-1", 11, &(result), sizeof(result), &(res_len),
                            sizeof(res_len), &((*result).value.str.len),
                            sizeof((*result).value.str.len),
                            &((*result).value.str.val),
                            sizeof((*result).value.str.val), &(op1),
                            sizeof(op1), &(op2), sizeof(op2), &(use_copy1),
                            sizeof(use_copy1), &(use_copy2), sizeof(use_copy2),
                            &((*result).refcount__gc),
                            sizeof((*result).refcount__gc), &((*result).type),
                            sizeof((*result).type), &((*result).is_ref__gc),
                            sizeof((*result).is_ref__gc)))
                (*result).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21383
            case 2: {
              // ReplaceKind
              (*(op1)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21384
            case 3: {
              // ReplaceKind
              (*(op2)).value.str.len = res_len;

              break;
            }
#endif
#ifdef COMPILE_21385
            case 4: {
              // ReplaceKind
              (*(result)).value.str.len = res_len;

              break;
            }
#endif
            }
          }
        } else {
          int length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
          char *buf = (char *)emalloc(length + 1);

          memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
          memcpy(buf + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
          buf[length] = 0;
          ZVAL_STRINGL(result, buf, length, 0);
        }

        break;
      }
#ifdef COMPILE_21386
      case 1: {
        // GuardKind
        if (!__is_neg("226-1", 11, &(result), sizeof(result), &(res_len),
                      sizeof(res_len), &((*result).value.str.len),
                      sizeof((*result).value.str.len),
                      &((*result).value.str.val),
                      sizeof((*result).value.str.val), &(op1), sizeof(op1),
                      &(op2), sizeof(op2), &(use_copy1), sizeof(use_copy1),
                      &(use_copy2), sizeof(use_copy2),
                      &((*result).refcount__gc), sizeof((*result).refcount__gc),
                      &((*result).type), sizeof((*result).type),
                      &((*result).is_ref__gc), sizeof((*result).is_ref__gc)))
          (*result).value.str.len = res_len;

        break;
      }
#endif
#ifdef COMPILE_21387
      case 2: {
        // ReplaceKind
        (*(op1)).value.str.len = res_len;

        break;
      }
#endif
#ifdef COMPILE_21388
      case 3: {
        // ReplaceKind
        (*(op2)).value.str.len = res_len;

        break;
      }
#endif
#ifdef COMPILE_21389
      case 4: {
        // ReplaceKind
        (*(result)).value.str.len = res_len;

        break;
      }
#endif
      }
    }
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
