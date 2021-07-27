int __get_mutant(); int __is_neg(const char *location,int count, ...); int __abst_hole(); int __choose(const char *);void* memset(void*, int, unsigned long); 
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
#include "zend_operators.h"
#include "zend_variables.h"
#include "zend_globals.h"
#include "zend_list.h"
#include "zend_API.h"
#include "zend_multiply.h"
#include "zend_strtod.h"
#include "zend_exceptions.h"
#include "zend_closures.h"

#define LONG_SIGN_MASK (1L << (8*sizeof(long)-1))

#if ZEND_USE_TOLOWER_L
#include <locale.h>
static _locale_t current_locale = NULL;
/* this is true global! may lead to strange effects on ZTS, but so may setlocale() */
#define zend_tolower(c) _tolower_l(c, current_locale)
#else
#define zend_tolower(c) tolower(c)
#endif

#define TYPE_PAIR(t1,t2) (((t1) << 4) | (t2))

ZEND_API int zend_atoi(const char *str, int str_len) /* {{{ */
{
	int retval;

	if (!str_len) {
		str_len = strlen(str);
	}
	retval = strtol(str, NULL, 0);
	if (str_len>0) {
		switch (str[str_len-1]) {
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
	if (str_len>0) {
		switch (str[str_len-1]) {
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

ZEND_API double zend_string_to_double(const char *number, zend_uint length) /* {{{ */
{
	double divisor = 10.0;
	double result = 0.0;
	double exponent;
	const char *end = number+length;
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
			exponent = (double) atoi(digit+1);
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
			exponent = (double) atoi(digit+1);
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
		case IS_STRING:
			{
				char *strval;

				strval = Z_STRVAL_P(op);
				if ((Z_TYPE_P(op)=is_numeric_string(strval, Z_STRLEN_P(op), &Z_LVAL_P(op), &Z_DVAL_P(op), 1)) == 0) {
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
#define zendi_convert_scalar_to_number(op, holder, result)			\
	if (op==result) {												\
		if (Z_TYPE_P(op) != IS_LONG) {								\
			convert_scalar_to_number(op TSRMLS_CC);					\
		}															\
	} else {														\
		switch (Z_TYPE_P(op)) {										\
			case IS_STRING:											\
				{													\
					if ((Z_TYPE(holder)=is_numeric_string(Z_STRVAL_P(op), Z_STRLEN_P(op), &Z_LVAL(holder), &Z_DVAL(holder), 1)) == 0) {	\
						ZVAL_LONG(&(holder), 0);							\
					}														\
					(op) = &(holder);										\
					break;													\
				}															\
			case IS_BOOL:													\
			case IS_RESOURCE:												\
				ZVAL_LONG(&(holder), Z_LVAL_P(op));							\
				(op) = &(holder);											\
				break;														\
			case IS_NULL:													\
				ZVAL_LONG(&(holder), 0);									\
				(op) = &(holder);											\
				break;														\
			case IS_OBJECT:													\
				(holder) = (*(op));											\
				zval_copy_ctor(&(holder));									\
				convert_to_long_base(&(holder), 10);						\
				if (Z_TYPE(holder) == IS_LONG) {							\
					(op) = &(holder);										\
				}															\
				break;														\
		}																	\
	}

/* }}} */

/* {{{ zendi_convert_to_long */
#define zendi_convert_to_long(op, holder, result)					\
	if (op == result) {												\
		convert_to_long(op);										\
	} else if (Z_TYPE_P(op) != IS_LONG) {							\
		switch (Z_TYPE_P(op)) {										\
			case IS_NULL:											\
				Z_LVAL(holder) = 0;									\
				break;												\
			case IS_DOUBLE:											\
				Z_LVAL(holder) = zend_dval_to_lval(Z_DVAL_P(op));	\
				break;												\
			case IS_STRING:											\
				Z_LVAL(holder) = strtol(Z_STRVAL_P(op), NULL, 10);	\
				break;												\
			case IS_ARRAY:											\
				Z_LVAL(holder) = (zend_hash_num_elements(Z_ARRVAL_P(op))?1:0);	\
				break;												\
			case IS_OBJECT:											\
				(holder) = (*(op));									\
				zval_copy_ctor(&(holder));							\
				convert_to_long_base(&(holder), 10);				\
				break;												\
			case IS_BOOL:											\
			case IS_RESOURCE:										\
				Z_LVAL(holder) = Z_LVAL_P(op);						\
				break;												\
			default:												\
				zend_error(E_WARNING, "Cannot convert to ordinal value");	\
				Z_LVAL(holder) = 0;									\
				break;												\
		}															\
		Z_TYPE(holder) = IS_LONG;									\
		(op) = &(holder);											\
	}

/* }}} */

/* {{{ zendi_convert_to_boolean */
#define zendi_convert_to_boolean(op, holder, result)				\
	if (op==result) {												\
		convert_to_boolean(op);										\
	} else if (Z_TYPE_P(op) != IS_BOOL) {							\
		switch (Z_TYPE_P(op)) {										\
			case IS_NULL:											\
				Z_LVAL(holder) = 0;									\
				break;												\
			case IS_RESOURCE:										\
			case IS_LONG:											\
				Z_LVAL(holder) = (Z_LVAL_P(op) ? 1 : 0);			\
				break;												\
			case IS_DOUBLE:											\
				Z_LVAL(holder) = (Z_DVAL_P(op) ? 1 : 0);			\
				break;												\
			case IS_STRING:											\
				if (Z_STRLEN_P(op) == 0								\
					|| (Z_STRLEN_P(op)==1 && Z_STRVAL_P(op)[0]=='0')) {	\
					Z_LVAL(holder) = 0;								\
				} else {											\
					Z_LVAL(holder) = 1;								\
				}													\
				break;												\
			case IS_ARRAY:											\
				Z_LVAL(holder) = (zend_hash_num_elements(Z_ARRVAL_P(op))?1:0);	\
				break;												\
			case IS_OBJECT:											\
				(holder) = (*(op));									\
				zval_copy_ctor(&(holder));							\
				convert_to_boolean(&(holder));						\
				break;												\
			default:												\
				Z_LVAL(holder) = 0;									\
				break;												\
		}															\
		Z_TYPE(holder) = IS_BOOL;									\
		(op) = &(holder);											\
	}

/* }}} */

/* {{{ convert_object_to_type */
#define convert_object_to_type(op, ctype, conv_func)										\
	if (Z_OBJ_HT_P(op)->cast_object) {														\
		zval dst;																			\
		if (Z_OBJ_HT_P(op)->cast_object(op, &dst, ctype TSRMLS_CC) == FAILURE) {			\
			zend_error(E_RECOVERABLE_ERROR,													\
				"Object of class %s could not be converted to %s", Z_OBJCE_P(op)->name,		\
			zend_get_type_by_const(ctype));													\
		} else {																			\
			zval_dtor(op);																	\
			Z_TYPE_P(op) = ctype;															\
			op->value = dst.value;															\
		}																					\
	} else {																				\
		if (Z_OBJ_HT_P(op)->get) {															\
			zval *newop = Z_OBJ_HT_P(op)->get(op TSRMLS_CC);								\
			if (Z_TYPE_P(newop) != IS_OBJECT) {												\
				/* for safety - avoid loop */												\
				zval_dtor(op);																\
				*op = *newop;																\
				FREE_ZVAL(newop);															\
				conv_func(op);																\
			}																				\
		}																					\
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
		case IS_STRING:
			{
				char *strval = Z_STRVAL_P(op);

				Z_LVAL_P(op) = strtol(strval, NULL, base);
				STR_FREE(strval);
			}
			break;
		case IS_ARRAY:
			tmp = (zend_hash_num_elements(Z_ARRVAL_P(op))?1:0);
			zval_dtor(op);
			Z_LVAL_P(op) = tmp;
			break;
		case IS_OBJECT:
			{
				int retval = 1;
				TSRMLS_FETCH();

				convert_object_to_type(op, IS_LONG, convert_to_long);

				if (Z_TYPE_P(op) == IS_LONG) {
					return;
				}
				zend_error(E_NOTICE, "Object of class %s could not be converted to int", Z_OBJCE_P(op)->name);

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
			Z_DVAL_P(op) = (double) Z_LVAL_P(op);
			break;
		case IS_DOUBLE:
			break;
		case IS_STRING:
			{
				char *strval = Z_STRVAL_P(op);

				Z_DVAL_P(op) = zend_strtod(strval, NULL);
				STR_FREE(strval);
			}
			break;
		case IS_ARRAY:
			tmp = (zend_hash_num_elements(Z_ARRVAL_P(op))?1:0);
			zval_dtor(op);
			Z_DVAL_P(op) = tmp;
			break;
		case IS_OBJECT:
			{
				double retval = 1.0;
				TSRMLS_FETCH();

				convert_object_to_type(op, IS_DOUBLE, convert_to_double);

				if (Z_TYPE_P(op) == IS_DOUBLE) {
					return;
				}
				zend_error(E_NOTICE, "Object of class %s could not be converted to double", Z_OBJCE_P(op)->name);

				zval_dtor(op);
				ZVAL_DOUBLE(op, retval);
				break;
			}
		default:
			zend_error(E_WARNING, "Cannot convert to real value (type=%d)", Z_TYPE_P(op));
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
		case IS_STRING:
			{
				char *strval = Z_STRVAL_P(op);

				if (Z_STRLEN_P(op) == 0
					|| (Z_STRLEN_P(op)==1 && Z_STRVAL_P(op)[0]=='0')) {
					Z_LVAL_P(op) = 0;
				} else {
					Z_LVAL_P(op) = 1;
				}
				STR_FREE(strval);
			}
			break;
		case IS_ARRAY:
			tmp = (zend_hash_num_elements(Z_ARRVAL_P(op))?1:0);
			zval_dtor(op);
			Z_LVAL_P(op) = tmp;
			break;
		case IS_OBJECT:
			{
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
			Z_STRLEN_P(op) = zend_spprintf(&Z_STRVAL_P(op), 0, "%.*G", (int) EG(precision), dval);
			/* %G already handles removing trailing zeros from the fractional part, yay */
			break;
		}
		case IS_ARRAY:
			zend_error(E_NOTICE, "Array to string conversion");
			zval_dtor(op);
			Z_STRVAL_P(op) = estrndup_rel("Array", sizeof("Array")-1);
			Z_STRLEN_P(op) = sizeof("Array")-1;
			break;
		case IS_OBJECT: {
			TSRMLS_FETCH();

			convert_object_to_type(op, IS_STRING, convert_to_string);

			if (Z_TYPE_P(op) == IS_STRING) {
				return;
			}

			zend_error(E_NOTICE, "Object of class %s to string conversion", Z_OBJCE_P(op)->name);
			zval_dtor(op);
			Z_STRVAL_P(op) = estrndup_rel("Object", sizeof("Object")-1);
			Z_STRLEN_P(op) = sizeof("Object")-1;
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
			zend_hash_index_update(Z_ARRVAL_P(op), 0, (void *) &entry, sizeof(zval *), NULL);
			Z_TYPE_P(op) = IS_ARRAY;
			break;
		case IS_OBJECT:
			object_init(op);
			zend_hash_update(Z_OBJPROP_P(op), "scalar", sizeof("scalar"), (void *) &entry, sizeof(zval *), NULL);
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
		case IS_OBJECT:
			{
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
						zend_hash_copy(ht, obj_ht, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
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
			}
			break;
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
		case IS_ARRAY:
			{
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

ZEND_API int add_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	zval op1_copy, op2_copy;
	int converted = 0;

	while (1) {
		switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
			case TYPE_PAIR(IS_LONG, IS_LONG): {
				long lval = Z_LVAL_P(op1) + Z_LVAL_P(op2);

				/* check for overflow by comparing sign bits */
				if ((Z_LVAL_P(op1) & LONG_SIGN_MASK) == (Z_LVAL_P(op2) & LONG_SIGN_MASK)
					&& (Z_LVAL_P(op1) & LONG_SIGN_MASK) != (lval & LONG_SIGN_MASK)) {

					ZVAL_DOUBLE(result, (double) Z_LVAL_P(op1) + (double) Z_LVAL_P(op2));
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
				zend_hash_merge(Z_ARRVAL_P(result), Z_ARRVAL_P(op2), (void (*)(void *pData)) zval_add_ref, (void *) &tmp, sizeof(zval *), 0);
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

ZEND_API int sub_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	zval op1_copy, op2_copy;
	int converted = 0;

	while (1) {
		switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
			case TYPE_PAIR(IS_LONG, IS_LONG): {
				long lval = Z_LVAL_P(op1) - Z_LVAL_P(op2);

				/* check for overflow by comparing sign bits */
				if ((Z_LVAL_P(op1) & LONG_SIGN_MASK) != (Z_LVAL_P(op2) & LONG_SIGN_MASK)
					&& (Z_LVAL_P(op1) & LONG_SIGN_MASK) != (lval & LONG_SIGN_MASK)) {

					ZVAL_DOUBLE(result, (double) Z_LVAL_P(op1) - (double) Z_LVAL_P(op2));
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

ZEND_API int mul_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	zval op1_copy, op2_copy;
	int converted = 0;

	while (1) {
		switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
			case TYPE_PAIR(IS_LONG, IS_LONG): {
				long overflow;

				ZEND_SIGNED_MULTIPLY_LONG(Z_LVAL_P(op1),Z_LVAL_P(op2), Z_LVAL_P(result),Z_DVAL_P(result),overflow);
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

ZEND_API int div_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	zval op1_copy, op2_copy;
	int converted = 0;

	while (1) {
		switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
			case TYPE_PAIR(IS_LONG, IS_LONG):
				if (Z_LVAL_P(op2) == 0) {
					zend_error(E_WARNING, "Division by zero");
					ZVAL_BOOL(result, 0);
					return FAILURE;			/* division by zero */
				} else if (Z_LVAL_P(op2) == -1 && Z_LVAL_P(op1) == LONG_MIN) {
					/* Prevent overflow error/crash */
					ZVAL_DOUBLE(result, (double) LONG_MIN / -1);
					return SUCCESS;
				}
				if (Z_LVAL_P(op1) % Z_LVAL_P(op2) == 0) { /* integer */
					ZVAL_LONG(result, Z_LVAL_P(op1) / Z_LVAL_P(op2));
				} else {
					ZVAL_DOUBLE(result, ((double) Z_LVAL_P(op1)) / Z_LVAL_P(op2));
				}
				return SUCCESS;

			case TYPE_PAIR(IS_DOUBLE, IS_LONG):
				if (Z_LVAL_P(op2) == 0) {
					zend_error(E_WARNING, "Division by zero");
					ZVAL_BOOL(result, 0);
					return FAILURE;			/* division by zero */
				}
				ZVAL_DOUBLE(result, Z_DVAL_P(op1) / (double)Z_LVAL_P(op2));
				return SUCCESS;

			case TYPE_PAIR(IS_LONG, IS_DOUBLE):
				if (Z_DVAL_P(op2) == 0) {
					zend_error(E_WARNING, "Division by zero");
					ZVAL_BOOL(result, 0);
					return FAILURE;			/* division by zero */
				}
				ZVAL_DOUBLE(result, (double)Z_LVAL_P(op1) / Z_DVAL_P(op2));
				return SUCCESS;

			case TYPE_PAIR(IS_DOUBLE, IS_DOUBLE):
				if (Z_DVAL_P(op2) == 0) {
					zend_error(E_WARNING, "Division by zero");
					ZVAL_BOOL(result, 0);
					return FAILURE;			/* division by zero */
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

ZEND_API int mod_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	zval op1_copy, op2_copy;
	long op1_lval;

	zendi_convert_to_long(op1, op1_copy, result);
	op1_lval = Z_LVAL_P(op1);
	zendi_convert_to_long(op2, op2_copy, result);

	if (Z_LVAL_P(op2) == 0) {
		zend_error(E_WARNING, "Division by zero");
		ZVAL_BOOL(result, 0);
		return FAILURE;			/* modulus by zero */
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

ZEND_API int boolean_xor_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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
	return FAILURE;				/* unknown datatype */
}
/* }}} */

ZEND_API int bitwise_or_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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
		if (result==op1) {
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

ZEND_API int bitwise_and_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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
		if (result==op1) {
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

ZEND_API int bitwise_xor_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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
		if (result==op1) {
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

ZEND_API int shift_left_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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

ZEND_API int shift_right_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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
ZEND_API int add_char_to_string(zval *result, const zval *op1, const zval *op2) /* {{{ */
{
	int length = Z_STRLEN_P(op1) + 1;
	char *buf;

	if (IS_INTERNED(Z_STRVAL_P(op1))) {
		buf = (char *) emalloc(length + 1);
		memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	} else {
		buf = (char *) erealloc(Z_STRVAL_P(op1), length + 1);
	}
	buf[length - 1] = (char) Z_LVAL_P(op2);
	buf[length] = 0;
	ZVAL_STRINGL(result, buf, length, 0);
	return SUCCESS;
}
/* }}} */

/* must support result==op1 */
ZEND_API int add_string_to_string(zval *result, const zval *op1, const zval *op2) /* {{{ */
{
	int length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
	char *buf;

	if (IS_INTERNED(Z_STRVAL_P(op1))) {
		buf = (char *) emalloc(length+1);
		memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	} else {
		buf = (char *) erealloc(Z_STRVAL_P(op1), length+1);
	}
	memcpy(buf + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	buf[length] = 0;
	ZVAL_STRINGL(result, buf, length, 0);
	return SUCCESS;
}
/* }}} */

ZEND_API int concat_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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
		/* We have created a converted copy of op1. Therefore, op1 won't become the result so
		 * we have to free it.
		 */
		if (result == op1) {
			zval_dtor(op1);
		}
		op1 = &op1_copy;
	}
	if (use_copy2) {
		op2 = &op2_copy;
	}
	//prophet generated patch
	{
	switch(__choose("__SWITCH72"))
	{
	case 0: 
	break;
	#ifdef COMPILE_8080
	case 1: {
	//AddAndReplaceKind
	(*(op1)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8081
	case 2: {
	//AddAndReplaceKind
	(*(op1)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8082
	case 3: {
	//AddAndReplaceKind
	(*(op1)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8083
	case 4: {
	//AddAndReplaceKind
	(*(op2)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8084
	case 5: {
	//AddAndReplaceKind
	(*(op2)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8085
	case 6: {
	//AddAndReplaceKind
	(*(op2)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8086
	case 7: {
	//AddAndReplaceKind
	(*(result)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8087
	case 8: {
	//AddAndReplaceKind
	(*(result)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8088
	case 9: {
	//AddAndReplaceKind
	(*(result)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8089
	case 10: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_8090
	case 11: {
	//AddAndReplaceKind
	(op1) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8091
	case 12: {
	//AddAndReplaceKind
	(op1) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8092
	case 13: {
	//AddAndReplaceKind
	(op2) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8093
	case 14: {
	//AddAndReplaceKind
	(op2) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8094
	case 15: {
	//AddAndReplaceKind
	(result) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8095
	case 16: {
	//AddAndReplaceKind
	(result) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8096
	case 17: {
	//AddAndReplaceKind
	_convert_to_string(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8097
	case 18: {
	//AddAndReplaceKind
	_convert_to_string(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8098
	case 19: {
	//AddAndReplaceKind
	_convert_to_string(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8099
	case 20: {
	//AddAndReplaceKind
	_efree(((*(op1)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8100
	case 21: {
	//AddAndReplaceKind
	_efree(((*(op2)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8101
	case 22: {
	//AddAndReplaceKind
	_efree(((*(result)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8102
	case 23: {
	//AddAndReplaceKind
	_efree(((*result).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8103
	case 24: {
	//AddAndReplaceKind
	_efree(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8104
	case 25: {
	//AddAndReplaceKind
	_efree(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8105
	case 26: {
	//AddAndReplaceKind
	_efree(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8106
	case 27: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8107
	case 28: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8108
	case 29: {
	//AddAndReplaceKind
	_zend_hash_init(((*(result)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8109
	case 30: {
	//AddAndReplaceKind
	_zend_list_delete((*(op1)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8110
	case 31: {
	//AddAndReplaceKind
	_zend_list_delete((*(op2)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8111
	case 32: {
	//AddAndReplaceKind
	_zend_list_delete((*(result)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8112
	case 33: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8113
	case 34: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8114
	case 35: {
	//AddAndReplaceKind
	_zval_copy_ctor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8115
	case 36: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8116
	case 37: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8117
	case 38: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8118
	case 39: {
	//AddAndReplaceKind
	_zval_dtor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8119
	case 40: {
	//AddAndReplaceKind
	_zval_dtor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8120
	case 41: {
	//AddAndReplaceKind
	_zval_dtor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8121
	case 42: {
	//AddAndReplaceKind
	_zval_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8122
	case 43: {
	//AddAndReplaceKind
	_zval_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8123
	case 44: {
	//AddAndReplaceKind
	_zval_dtor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8124
	case 45: {
	//AddAndReplaceKind
	_zval_dtor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8125
	case 46: {
	//AddAndReplaceKind
	_zval_dtor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8126
	case 47: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8127
	case 48: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8128
	case 49: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8129
	case 50: {
	//AddAndReplaceKind
	convert_scalar_to_array((op1), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8130
	case 51: {
	//AddAndReplaceKind
	convert_scalar_to_array((op2), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8131
	case 52: {
	//AddAndReplaceKind
	convert_scalar_to_array((result), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8132
	case 53: {
	//AddAndReplaceKind
	convert_scalar_to_number((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8133
	case 54: {
	//AddAndReplaceKind
	convert_scalar_to_number((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8134
	case 55: {
	//AddAndReplaceKind
	convert_scalar_to_number((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8135
	case 56: {
	//AddAndReplaceKind
	convert_to_array((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8136
	case 57: {
	//AddAndReplaceKind
	convert_to_array((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8137
	case 58: {
	//AddAndReplaceKind
	convert_to_array((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8138
	case 59: {
	//AddAndReplaceKind
	convert_to_boolean((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8139
	case 60: {
	//AddAndReplaceKind
	convert_to_boolean((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8140
	case 61: {
	//AddAndReplaceKind
	convert_to_boolean((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8141
	case 62: {
	//AddAndReplaceKind
	convert_to_double((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8142
	case 63: {
	//AddAndReplaceKind
	convert_to_double((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8143
	case 64: {
	//AddAndReplaceKind
	convert_to_double((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8144
	case 65: {
	//AddAndReplaceKind
	convert_to_long((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8145
	case 66: {
	//AddAndReplaceKind
	convert_to_long((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8146
	case 67: {
	//AddAndReplaceKind
	convert_to_long((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8147
	case 68: {
	//AddAndReplaceKind
	convert_to_long_base((op1), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8148
	case 69: {
	//AddAndReplaceKind
	convert_to_long_base((op2), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8149
	case 70: {
	//AddAndReplaceKind
	convert_to_long_base((result), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8150
	case 71: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8151
	case 72: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8152
	case 73: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8153
	case 74: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8154
	case 75: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8155
	case 76: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8156
	case 77: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8157
	case 78: {
	//AddAndReplaceKind
	gc_zval_possible_root((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8158
	case 79: {
	//AddAndReplaceKind
	gc_zval_possible_root((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8159
	case 80: {
	//AddAndReplaceKind
	gc_zval_possible_root((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8160
	case 81: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8161
	case 82: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8162
	case 83: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8163
	case 84: {
	//AddAndReplaceKind
	if ((*op1).type != 6) {
	    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8164
	case 85: {
	//AddAndReplaceKind
	if ((*op2).type != 6) {
	    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8165
	case 86: {
	//AddAndReplaceKind
	if (result == op1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8166
	case 87: {
	//AddAndReplaceKind
	if (use_copy1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8167
	case 88: {
	//AddAndReplaceKind
	if (use_copy2) {
	    _zval_dtor((op2));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8168
	case 89: {
	//AddAndReplaceKind
	if (use_copy2) {
	    op2 = &op2_copy;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8169
	case 90: {
	//AddAndReplaceKind
	memcpy((*(op1)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8170
	case 91: {
	//AddAndReplaceKind
	memcpy((*(op2)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8171
	case 92: {
	//AddAndReplaceKind
	memcpy((*(result)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8172
	case 93: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op1)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8173
	case 94: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op2)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8174
	case 95: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(result)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8175
	case 96: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op1)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8176
	case 97: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op2)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8177
	case 98: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(result)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8178
	case 99: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op1)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8179
	case 100: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op2)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8180
	case 101: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(result)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8181
	case 102: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8182
	case 103: {
	//AddAndReplaceKind
	op1 = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8183
	case 104: {
	//AddAndReplaceKind
	op2 = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8184
	case 105: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "String size overflow");
	
	break;
	}
	#endif
	#ifdef COMPILE_8185
	case 106: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "Unsupported operand types");
	
	break;
	}
	#endif
	#ifdef COMPILE_8186
	case 107: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8187
	case 108: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8188
	case 109: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8189
	case 110: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8190
	case 111: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8191
	case 112: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8192
	case 113: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8193
	case 114: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8194
	case 115: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8195
	case 116: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8196
	case 117: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8197
	case 118: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8198
	case 119: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8199
	case 120: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8200
	case 121: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8201
	case 122: {
	//AddAndReplaceKind
	zend_error((1 << 1L), "Division by zero");
	
	break;
	}
	#endif
	#ifdef COMPILE_8202
	case 123: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_8203
	case 124: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8204
	case 125: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8205
	case 126: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8206
	case 127: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8207
	case 128: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8208
	case 129: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8209
	case 130: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8210
	case 131: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8211
	case 132: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8212
	case 133: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8213
	case 134: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8214
	case 135: {
	//AddAndReplaceKind
	zend_free_obj_get_result((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8215
	case 136: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8216
	case 137: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8217
	case 138: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8218
	case 139: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8219
	case 140: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8220
	case 141: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8221
	case 142: {
	//AddAndReplaceKind
	zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8222
	case 143: {
	//AddAndReplaceKind
	zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8223
	case 144: {
	//AddAndReplaceKind
	zval_unset_isref_p((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8224
	case 145: {
	//AddAndReplaceKind
	zval_unset_isref_p((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8225
	case 146: {
	//AddAndReplaceKind
	zval_unset_isref_p((result));
	
	break;
	}
	#endif
	}
	{
	long long __temp73=(result == op1 && !((((*op1).value.str.val) >= (compiler_globals.interned_strings_start)) && (((*op1).value.str.val) < (compiler_globals.interned_strings_end)))) ;
	switch(__choose("__SWITCH73"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_8226
	case 1: {
	__temp73= ((result == op1 && !((((*op1).value.str.val) >= (compiler_globals.interned_strings_start)) && (((*op1).value.str.val) < (compiler_globals.interned_strings_end)))) && !__is_neg("73-1", 87, &(result), sizeof (result), &(op1), sizeof (op1), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &((*op1).value.str.val), sizeof ((*op1).value.str.val), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(op2), sizeof (op2), &((*op1).value.str.len), sizeof ((*op1).value.str.len), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(use_copy2), sizeof (use_copy2), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &(use_copy1), sizeof (use_copy1), &((*op2).type), sizeof ((*op2).type), &((*op1).type), sizeof ((*op1).type), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &((*op1).refcount__gc), sizeof ((*op1).refcount__gc), &((*op1).is_ref__gc), sizeof ((*op1).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH85"))
	{
	case 0: {
	if (__temp73) {	/* special case, perform operations on result */
			uint res_len = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
	
			{
	switch(__choose("__SWITCH74"))
	{
	case 0: 
	break;
	#ifdef COMPILE_8227
	case 1: {
	//AddAndReplaceKind
	(*(op1)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8228
	case 2: {
	//AddAndReplaceKind
	(*(op1)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8229
	case 3: {
	//AddAndReplaceKind
	(*(op1)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8230
	case 4: {
	//AddAndReplaceKind
	(*(op1)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8231
	case 5: {
	//AddAndReplaceKind
	(*(op1)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8232
	case 6: {
	//AddAndReplaceKind
	(*(op1)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8233
	case 7: {
	//AddAndReplaceKind
	(*(op2)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8234
	case 8: {
	//AddAndReplaceKind
	(*(op2)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8235
	case 9: {
	//AddAndReplaceKind
	(*(op2)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8236
	case 10: {
	//AddAndReplaceKind
	(*(op2)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8237
	case 11: {
	//AddAndReplaceKind
	(*(op2)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8238
	case 12: {
	//AddAndReplaceKind
	(*(op2)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8239
	case 13: {
	//AddAndReplaceKind
	(*(result)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8240
	case 14: {
	//AddAndReplaceKind
	(*(result)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8241
	case 15: {
	//AddAndReplaceKind
	(*(result)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8242
	case 16: {
	//AddAndReplaceKind
	(*(result)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8243
	case 17: {
	//AddAndReplaceKind
	(*(result)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8244
	case 18: {
	//AddAndReplaceKind
	(*(result)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8245
	case 19: {
	//AddAndReplaceKind
	(*result).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8246
	case 20: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8247
	case 21: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8248
	case 22: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(result)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8249
	case 23: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8250
	case 24: {
	//AddAndReplaceKind
	(*result).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8251
	case 25: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_8252
	case 26: {
	//AddAndReplaceKind
	(op1) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8253
	case 27: {
	//AddAndReplaceKind
	(op1) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8254
	case 28: {
	//AddAndReplaceKind
	(op2) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8255
	case 29: {
	//AddAndReplaceKind
	(op2) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8256
	case 30: {
	//AddAndReplaceKind
	(result) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8257
	case 31: {
	//AddAndReplaceKind
	(result) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8258
	case 32: {
	//AddAndReplaceKind
	_convert_to_string(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8259
	case 33: {
	//AddAndReplaceKind
	_convert_to_string(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8260
	case 34: {
	//AddAndReplaceKind
	_convert_to_string(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8261
	case 35: {
	//AddAndReplaceKind
	_efree(((*(op1)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8262
	case 36: {
	//AddAndReplaceKind
	_efree(((*(op2)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8263
	case 37: {
	//AddAndReplaceKind
	_efree(((*(result)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8264
	case 38: {
	//AddAndReplaceKind
	_efree(((*result).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8265
	case 39: {
	//AddAndReplaceKind
	_efree(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8266
	case 40: {
	//AddAndReplaceKind
	_efree(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8267
	case 41: {
	//AddAndReplaceKind
	_efree(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8268
	case 42: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8269
	case 43: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8270
	case 44: {
	//AddAndReplaceKind
	_zend_hash_init(((*(result)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8271
	case 45: {
	//AddAndReplaceKind
	_zend_list_delete((*(op1)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8272
	case 46: {
	//AddAndReplaceKind
	_zend_list_delete((*(op2)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8273
	case 47: {
	//AddAndReplaceKind
	_zend_list_delete((*(result)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8274
	case 48: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8275
	case 49: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8276
	case 50: {
	//AddAndReplaceKind
	_zval_copy_ctor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8277
	case 51: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8278
	case 52: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8279
	case 53: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8280
	case 54: {
	//AddAndReplaceKind
	_zval_dtor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8281
	case 55: {
	//AddAndReplaceKind
	_zval_dtor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8282
	case 56: {
	//AddAndReplaceKind
	_zval_dtor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8283
	case 57: {
	//AddAndReplaceKind
	_zval_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8284
	case 58: {
	//AddAndReplaceKind
	_zval_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8285
	case 59: {
	//AddAndReplaceKind
	_zval_dtor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8286
	case 60: {
	//AddAndReplaceKind
	_zval_dtor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8287
	case 61: {
	//AddAndReplaceKind
	_zval_dtor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8288
	case 62: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8289
	case 63: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8290
	case 64: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8291
	case 65: {
	//AddAndReplaceKind
	convert_scalar_to_array((op1), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8292
	case 66: {
	//AddAndReplaceKind
	convert_scalar_to_array((op2), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8293
	case 67: {
	//AddAndReplaceKind
	convert_scalar_to_array((result), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8294
	case 68: {
	//AddAndReplaceKind
	convert_scalar_to_number((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8295
	case 69: {
	//AddAndReplaceKind
	convert_scalar_to_number((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8296
	case 70: {
	//AddAndReplaceKind
	convert_scalar_to_number((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8297
	case 71: {
	//AddAndReplaceKind
	convert_to_array((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8298
	case 72: {
	//AddAndReplaceKind
	convert_to_array((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8299
	case 73: {
	//AddAndReplaceKind
	convert_to_array((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8300
	case 74: {
	//AddAndReplaceKind
	convert_to_boolean((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8301
	case 75: {
	//AddAndReplaceKind
	convert_to_boolean((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8302
	case 76: {
	//AddAndReplaceKind
	convert_to_boolean((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8303
	case 77: {
	//AddAndReplaceKind
	convert_to_double((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8304
	case 78: {
	//AddAndReplaceKind
	convert_to_double((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8305
	case 79: {
	//AddAndReplaceKind
	convert_to_double((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8306
	case 80: {
	//AddAndReplaceKind
	convert_to_long((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8307
	case 81: {
	//AddAndReplaceKind
	convert_to_long((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8308
	case 82: {
	//AddAndReplaceKind
	convert_to_long((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8309
	case 83: {
	//AddAndReplaceKind
	convert_to_long_base((op1), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8310
	case 84: {
	//AddAndReplaceKind
	convert_to_long_base((op2), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8311
	case 85: {
	//AddAndReplaceKind
	convert_to_long_base((result), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8312
	case 86: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8313
	case 87: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8314
	case 88: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8315
	case 89: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8316
	case 90: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8317
	case 91: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8318
	case 92: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8319
	case 93: {
	//AddAndReplaceKind
	gc_zval_possible_root((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8320
	case 94: {
	//AddAndReplaceKind
	gc_zval_possible_root((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8321
	case 95: {
	//AddAndReplaceKind
	gc_zval_possible_root((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8322
	case 96: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8323
	case 97: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8324
	case 98: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8325
	case 99: {
	//AddAndReplaceKind
	if ((*op1).type != 6) {
	    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8326
	case 100: {
	//AddAndReplaceKind
	if ((*op2).type != 6) {
	    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8327
	case 101: {
	//AddAndReplaceKind
	if (result == op1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8328
	case 102: {
	//AddAndReplaceKind
	if (use_copy1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8329
	case 103: {
	//AddAndReplaceKind
	if (use_copy2) {
	    _zval_dtor((op2));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8330
	case 104: {
	//AddAndReplaceKind
	if (use_copy2) {
	    op2 = &op2_copy;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8331
	case 105: {
	//AddAndReplaceKind
	memcpy((*(op1)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8332
	case 106: {
	//AddAndReplaceKind
	memcpy((*(op2)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8333
	case 107: {
	//AddAndReplaceKind
	memcpy((*(result)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8334
	case 108: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op1)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8335
	case 109: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op2)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8336
	case 110: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(result)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8337
	case 111: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op1)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8338
	case 112: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op2)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8339
	case 113: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(result)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8340
	case 114: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op1)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8341
	case 115: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op2)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8342
	case 116: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(result)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8343
	case 117: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8344
	case 118: {
	//AddAndReplaceKind
	op1 = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8345
	case 119: {
	//AddAndReplaceKind
	op2 = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8346
	case 120: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "String size overflow");
	
	break;
	}
	#endif
	#ifdef COMPILE_8347
	case 121: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "Unsupported operand types");
	
	break;
	}
	#endif
	#ifdef COMPILE_8348
	case 122: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8349
	case 123: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8350
	case 124: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8351
	case 125: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8352
	case 126: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8353
	case 127: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8354
	case 128: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8355
	case 129: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8356
	case 130: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8357
	case 131: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8358
	case 132: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8359
	case 133: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8360
	case 134: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8361
	case 135: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8362
	case 136: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8363
	case 137: {
	//AddAndReplaceKind
	zend_error((1 << 1L), "Division by zero");
	
	break;
	}
	#endif
	#ifdef COMPILE_8364
	case 138: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_8365
	case 139: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8366
	case 140: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8367
	case 141: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8368
	case 142: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8369
	case 143: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8370
	case 144: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8371
	case 145: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8372
	case 146: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8373
	case 147: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8374
	case 148: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8375
	case 149: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8376
	case 150: {
	//AddAndReplaceKind
	zend_free_obj_get_result((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8377
	case 151: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8378
	case 152: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8379
	case 153: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8380
	case 154: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8381
	case 155: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8382
	case 156: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8383
	case 157: {
	//AddAndReplaceKind
	zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8384
	case 158: {
	//AddAndReplaceKind
	zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8385
	case 159: {
	//AddAndReplaceKind
	zval_unset_isref_p((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8386
	case 160: {
	//AddAndReplaceKind
	zval_unset_isref_p((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8387
	case 161: {
	//AddAndReplaceKind
	zval_unset_isref_p((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8388
	case 162: {
	//AddInitKind
	memset(&(*op1), 0, sizeof (*(&(*op1))));
	
	break;
	}
	#endif
	#ifdef COMPILE_8389
	case 163: {
	//AddInitKind
	memset(&(*op1).value, 0, sizeof (*(&(*op1).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8390
	case 164: {
	//AddInitKind
	memset(&(*op1).value.str, 0, sizeof (*(&(*op1).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8391
	case 165: {
	//AddInitKind
	memset(&(*op2), 0, sizeof (*(&(*op2))));
	
	break;
	}
	#endif
	#ifdef COMPILE_8392
	case 166: {
	//AddInitKind
	memset(&(*op2).value, 0, sizeof (*(&(*op2).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8393
	case 167: {
	//AddInitKind
	memset(&(*op2).value.str, 0, sizeof (*(&(*op2).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8394
	case 168: {
	//AddInitKind
	memset(&(*result), 0, sizeof (*(&(*result))));
	
	break;
	}
	#endif
	#ifdef COMPILE_8395
	case 169: {
	//AddInitKind
	memset(&(*result).value, 0, sizeof (*(&(*result).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8396
	case 170: {
	//AddInitKind
	memset(&(*result).value.str, 0, sizeof (*(&(*result).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8397
	case 171: {
	//IfExitKind
	if (__is_neg("74-171", 21, &(result), sizeof (result), &(op1), sizeof (op1), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op1).value.str.len), sizeof ((*op1).value.str.len), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*op1).value.str.val), sizeof ((*op1).value.str.val), &(use_copy2), sizeof (use_copy2), &(res_len), sizeof (res_len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &(use_copy1), sizeof (use_copy1), &((*op2).type), sizeof ((*op2).type), &((*op1).type), sizeof ((*op1).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op1).refcount__gc), sizeof ((*op1).refcount__gc), &((*op1).is_ref__gc), sizeof ((*op1).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8398
	case 172: {
	//IfExitKind
	if (__is_neg("74-172", 21, &(result), sizeof (result), &(op1), sizeof (op1), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op1).value.str.len), sizeof ((*op1).value.str.len), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*op1).value.str.val), sizeof ((*op1).value.str.val), &(use_copy2), sizeof (use_copy2), &(res_len), sizeof (res_len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &(use_copy1), sizeof (use_copy1), &((*op2).type), sizeof ((*op2).type), &((*op1).type), sizeof ((*op1).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op1).refcount__gc), sizeof ((*op1).refcount__gc), &((*op1).is_ref__gc), sizeof ((*op1).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_8399
	case 173: {
	//IfExitKind
	if (__is_neg("74-173", 21, &(result), sizeof (result), &(op1), sizeof (op1), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op1).value.str.len), sizeof ((*op1).value.str.len), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*op1).value.str.val), sizeof ((*op1).value.str.val), &(use_copy2), sizeof (use_copy2), &(res_len), sizeof (res_len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &(use_copy1), sizeof (use_copy1), &((*op2).type), sizeof ((*op2).type), &((*op1).type), sizeof ((*op1).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op1).refcount__gc), sizeof ((*op1).refcount__gc), &((*op1).is_ref__gc), sizeof ((*op1).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 6;
	
	break;
	}
	#endif
	}
	{
	long long __temp75=((*result).value.str.len < 0 || (int)((*op1).value.str.len + (*op2).value.str.len) < 0) ;
	switch(__choose("__SWITCH75"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_8400
	case 1: {
	__temp75= (((*result).value.str.len < 0 || (int)((*op1).value.str.len + (*op2).value.str.len) < 0) || __is_neg("75-1", 21, &(result), sizeof (result), &(op1), sizeof (op1), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op1).value.str.len), sizeof ((*op1).value.str.len), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*op1).value.str.val), sizeof ((*op1).value.str.val), &(use_copy2), sizeof (use_copy2), &(res_len), sizeof (res_len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &(use_copy1), sizeof (use_copy1), &((*op2).type), sizeof ((*op2).type), &((*op1).type), sizeof ((*op1).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op1).refcount__gc), sizeof ((*op1).refcount__gc), &((*op1).is_ref__gc), sizeof ((*op1).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8401
	case 2: {
	__temp75= (((*result).value.str.len < 0 || (int)((*op1).value.str.len + (*op2).value.str.len) < 0) && !__is_neg("75-2", 21, &(result), sizeof (result), &(op1), sizeof (op1), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op1).value.str.len), sizeof ((*op1).value.str.len), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*op1).value.str.val), sizeof ((*op1).value.str.val), &(use_copy2), sizeof (use_copy2), &(res_len), sizeof (res_len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &(use_copy1), sizeof (use_copy1), &((*op2).type), sizeof ((*op2).type), &((*op1).type), sizeof ((*op1).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op1).refcount__gc), sizeof ((*op1).refcount__gc), &((*op1).is_ref__gc), sizeof ((*op1).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH76"))
	{
	case 0: {
	if (__temp75) {
				efree(Z_STRVAL_P(result));
				ZVAL_EMPTY_STRING(result);
				zend_error(E_ERROR, "String size overflow");
			}
	
	break;
	}
	#ifdef COMPILE_8402
	case 1: {
	//GuardKind
	if (!__is_neg("76-1", 21, &(result), sizeof (result), &(op1), sizeof (op1), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op1).value.str.len), sizeof ((*op1).value.str.len), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*op1).value.str.val), sizeof ((*op1).value.str.val), &(use_copy2), sizeof (use_copy2), &(res_len), sizeof (res_len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &(use_copy1), sizeof (use_copy1), &((*op2).type), sizeof ((*op2).type), &((*op1).type), sizeof ((*op1).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op1).refcount__gc), sizeof ((*op1).refcount__gc), &((*op1).is_ref__gc), sizeof ((*op1).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    if ((*result).value.str.len < 0 || (int)((*op1).value.str.len + (*op2).value.str.len) < 0) {
	        _efree(((*result).value.str.val));
	        do {
	            zval *__z = (result);
	            (*__z).value.str.len = 0;
	            (*__z).value.str.val = _estrndup((""), (sizeof ("") - 1));
	            (*__z).type = 6;
	        } while (0);
	        zend_error((1 << 0L), "String size overflow");
	    }
	
	break;
	}
	#endif
	#ifdef COMPILE_8403
	case 2: {
	//SpecialGuardKind
	if (!__is_neg("76-2", 21, &(result), sizeof (result), &(op1), sizeof (op1), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op1).value.str.len), sizeof ((*op1).value.str.len), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*op1).value.str.val), sizeof ((*op1).value.str.val), &(use_copy2), sizeof (use_copy2), &(res_len), sizeof (res_len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &(use_copy1), sizeof (use_copy1), &((*op2).type), sizeof ((*op2).type), &((*op1).type), sizeof ((*op1).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op1).refcount__gc), sizeof ((*op1).refcount__gc), &((*op1).is_ref__gc), sizeof ((*op1).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)) && ((*result).value.str.len < 0 || (int)((*op1).value.str.len + (*op2).value.str.len) < 0)) {
	    _efree(((*result).value.str.val));
	    do {
	        zval *__z = (result);
	        (*__z).value.str.len = 0;
	        (*__z).value.str.val = _estrndup((""), (sizeof ("") - 1));
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
	switch(__choose("__SWITCH77"))
	{
	case 0: 
	break;
	#ifdef COMPILE_8404
	case 1: {
	//AddAndReplaceKind
	(*(op1)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8405
	case 2: {
	//AddAndReplaceKind
	(*(op1)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8406
	case 3: {
	//AddAndReplaceKind
	(*(op1)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8407
	case 4: {
	//AddAndReplaceKind
	(*(op1)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8408
	case 5: {
	//AddAndReplaceKind
	(*(op1)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8409
	case 6: {
	//AddAndReplaceKind
	(*(op1)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8410
	case 7: {
	//AddAndReplaceKind
	(*(op2)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8411
	case 8: {
	//AddAndReplaceKind
	(*(op2)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8412
	case 9: {
	//AddAndReplaceKind
	(*(op2)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8413
	case 10: {
	//AddAndReplaceKind
	(*(op2)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8414
	case 11: {
	//AddAndReplaceKind
	(*(op2)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8415
	case 12: {
	//AddAndReplaceKind
	(*(op2)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8416
	case 13: {
	//AddAndReplaceKind
	(*(result)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8417
	case 14: {
	//AddAndReplaceKind
	(*(result)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8418
	case 15: {
	//AddAndReplaceKind
	(*(result)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8419
	case 16: {
	//AddAndReplaceKind
	(*(result)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8420
	case 17: {
	//AddAndReplaceKind
	(*(result)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8421
	case 18: {
	//AddAndReplaceKind
	(*(result)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8422
	case 19: {
	//AddAndReplaceKind
	(*result).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8423
	case 20: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8424
	case 21: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8425
	case 22: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(result)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8426
	case 23: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8427
	case 24: {
	//AddAndReplaceKind
	(*result).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8428
	case 25: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_8429
	case 26: {
	//AddAndReplaceKind
	(op1) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8430
	case 27: {
	//AddAndReplaceKind
	(op1) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8431
	case 28: {
	//AddAndReplaceKind
	(op2) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8432
	case 29: {
	//AddAndReplaceKind
	(op2) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8433
	case 30: {
	//AddAndReplaceKind
	(result) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8434
	case 31: {
	//AddAndReplaceKind
	(result) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8435
	case 32: {
	//AddAndReplaceKind
	_convert_to_string(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8436
	case 33: {
	//AddAndReplaceKind
	_convert_to_string(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8437
	case 34: {
	//AddAndReplaceKind
	_convert_to_string(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8438
	case 35: {
	//AddAndReplaceKind
	_efree(((*(op1)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8439
	case 36: {
	//AddAndReplaceKind
	_efree(((*(op2)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8440
	case 37: {
	//AddAndReplaceKind
	_efree(((*(result)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8441
	case 38: {
	//AddAndReplaceKind
	_efree(((*result).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8442
	case 39: {
	//AddAndReplaceKind
	_efree(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8443
	case 40: {
	//AddAndReplaceKind
	_efree(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8444
	case 41: {
	//AddAndReplaceKind
	_efree(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8445
	case 42: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8446
	case 43: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8447
	case 44: {
	//AddAndReplaceKind
	_zend_hash_init(((*(result)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8448
	case 45: {
	//AddAndReplaceKind
	_zend_list_delete((*(op1)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8449
	case 46: {
	//AddAndReplaceKind
	_zend_list_delete((*(op2)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8450
	case 47: {
	//AddAndReplaceKind
	_zend_list_delete((*(result)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8451
	case 48: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8452
	case 49: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8453
	case 50: {
	//AddAndReplaceKind
	_zval_copy_ctor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8454
	case 51: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8455
	case 52: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8456
	case 53: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8457
	case 54: {
	//AddAndReplaceKind
	_zval_dtor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8458
	case 55: {
	//AddAndReplaceKind
	_zval_dtor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8459
	case 56: {
	//AddAndReplaceKind
	_zval_dtor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8460
	case 57: {
	//AddAndReplaceKind
	_zval_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8461
	case 58: {
	//AddAndReplaceKind
	_zval_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8462
	case 59: {
	//AddAndReplaceKind
	_zval_dtor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8463
	case 60: {
	//AddAndReplaceKind
	_zval_dtor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8464
	case 61: {
	//AddAndReplaceKind
	_zval_dtor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8465
	case 62: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8466
	case 63: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8467
	case 64: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8468
	case 65: {
	//AddAndReplaceKind
	convert_scalar_to_array((op1), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8469
	case 66: {
	//AddAndReplaceKind
	convert_scalar_to_array((op2), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8470
	case 67: {
	//AddAndReplaceKind
	convert_scalar_to_array((result), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8471
	case 68: {
	//AddAndReplaceKind
	convert_scalar_to_number((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8472
	case 69: {
	//AddAndReplaceKind
	convert_scalar_to_number((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8473
	case 70: {
	//AddAndReplaceKind
	convert_scalar_to_number((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8474
	case 71: {
	//AddAndReplaceKind
	convert_to_array((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8475
	case 72: {
	//AddAndReplaceKind
	convert_to_array((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8476
	case 73: {
	//AddAndReplaceKind
	convert_to_array((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8477
	case 74: {
	//AddAndReplaceKind
	convert_to_boolean((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8478
	case 75: {
	//AddAndReplaceKind
	convert_to_boolean((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8479
	case 76: {
	//AddAndReplaceKind
	convert_to_boolean((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8480
	case 77: {
	//AddAndReplaceKind
	convert_to_double((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8481
	case 78: {
	//AddAndReplaceKind
	convert_to_double((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8482
	case 79: {
	//AddAndReplaceKind
	convert_to_double((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8483
	case 80: {
	//AddAndReplaceKind
	convert_to_long((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8484
	case 81: {
	//AddAndReplaceKind
	convert_to_long((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8485
	case 82: {
	//AddAndReplaceKind
	convert_to_long((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8486
	case 83: {
	//AddAndReplaceKind
	convert_to_long_base((op1), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8487
	case 84: {
	//AddAndReplaceKind
	convert_to_long_base((op2), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8488
	case 85: {
	//AddAndReplaceKind
	convert_to_long_base((result), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8489
	case 86: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8490
	case 87: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8491
	case 88: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8492
	case 89: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8493
	case 90: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8494
	case 91: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8495
	case 92: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8496
	case 93: {
	//AddAndReplaceKind
	gc_zval_possible_root((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8497
	case 94: {
	//AddAndReplaceKind
	gc_zval_possible_root((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8498
	case 95: {
	//AddAndReplaceKind
	gc_zval_possible_root((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8499
	case 96: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8500
	case 97: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8501
	case 98: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8502
	case 99: {
	//AddAndReplaceKind
	if ((*op1).type != 6) {
	    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8503
	case 100: {
	//AddAndReplaceKind
	if ((*op2).type != 6) {
	    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8504
	case 101: {
	//AddAndReplaceKind
	if (result == op1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8505
	case 102: {
	//AddAndReplaceKind
	if (use_copy1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8506
	case 103: {
	//AddAndReplaceKind
	if (use_copy2) {
	    _zval_dtor((op2));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8507
	case 104: {
	//AddAndReplaceKind
	if (use_copy2) {
	    op2 = &op2_copy;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8508
	case 105: {
	//AddAndReplaceKind
	memcpy((*(op1)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8509
	case 106: {
	//AddAndReplaceKind
	memcpy((*(op2)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8510
	case 107: {
	//AddAndReplaceKind
	memcpy((*(result)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8511
	case 108: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op1)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8512
	case 109: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op2)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8513
	case 110: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(result)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8514
	case 111: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op1)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8515
	case 112: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op2)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8516
	case 113: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(result)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8517
	case 114: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op1)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8518
	case 115: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op2)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8519
	case 116: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(result)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8520
	case 117: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8521
	case 118: {
	//AddAndReplaceKind
	op1 = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8522
	case 119: {
	//AddAndReplaceKind
	op2 = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8523
	case 120: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "String size overflow");
	
	break;
	}
	#endif
	#ifdef COMPILE_8524
	case 121: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "Unsupported operand types");
	
	break;
	}
	#endif
	#ifdef COMPILE_8525
	case 122: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8526
	case 123: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8527
	case 124: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8528
	case 125: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8529
	case 126: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8530
	case 127: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8531
	case 128: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8532
	case 129: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8533
	case 130: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8534
	case 131: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8535
	case 132: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8536
	case 133: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8537
	case 134: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8538
	case 135: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8539
	case 136: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8540
	case 137: {
	//AddAndReplaceKind
	zend_error((1 << 1L), "Division by zero");
	
	break;
	}
	#endif
	#ifdef COMPILE_8541
	case 138: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_8542
	case 139: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8543
	case 140: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8544
	case 141: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8545
	case 142: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8546
	case 143: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8547
	case 144: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8548
	case 145: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8549
	case 146: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8550
	case 147: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8551
	case 148: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8552
	case 149: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8553
	case 150: {
	//AddAndReplaceKind
	zend_free_obj_get_result((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8554
	case 151: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8555
	case 152: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8556
	case 153: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8557
	case 154: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8558
	case 155: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8559
	case 156: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8560
	case 157: {
	//AddAndReplaceKind
	zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8561
	case 158: {
	//AddAndReplaceKind
	zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8562
	case 159: {
	//AddAndReplaceKind
	zval_unset_isref_p((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8563
	case 160: {
	//AddAndReplaceKind
	zval_unset_isref_p((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8564
	case 161: {
	//AddAndReplaceKind
	zval_unset_isref_p((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8565
	case 162: {
	//AddInitKind
	memset(&(*result), 0, sizeof (*(&(*result))));
	
	break;
	}
	#endif
	#ifdef COMPILE_8566
	case 163: {
	//AddInitKind
	memset(&(*result).value, 0, sizeof (*(&(*result).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8567
	case 164: {
	//AddInitKind
	memset(&(*result).value.str, 0, sizeof (*(&(*result).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8568
	case 165: {
	//IfExitKind
	if (__is_neg("77-165", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &(op1), sizeof (op1), &(use_copy2), sizeof (use_copy2), &(use_copy1), sizeof (use_copy1), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8569
	case 166: {
	//IfExitKind
	if (__is_neg("77-166", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &(op1), sizeof (op1), &(use_copy2), sizeof (use_copy2), &(use_copy1), sizeof (use_copy1), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_8570
	case 167: {
	//IfExitKind
	if (__is_neg("77-167", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &(op1), sizeof (op1), &(use_copy2), sizeof (use_copy2), &(use_copy1), sizeof (use_copy1), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    return 6;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH78"))
	{
	case 0: {
	Z_STRVAL_P(result) = erealloc(Z_STRVAL_P(result), res_len+1);
	
	break;
	}
	#ifdef COMPILE_8571
	case 1: {
	//GuardKind
	if (!__is_neg("78-1", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &(op1), sizeof (op1), &(use_copy2), sizeof (use_copy2), &(use_copy1), sizeof (use_copy1), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    (*result).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8572
	case 2: {
	//ReplaceKind
	(*(op1)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8573
	case 3: {
	//ReplaceKind
	(*(op2)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8574
	case 4: {
	//ReplaceKind
	(*(result)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8575
	case 5: {
	//ReplaceKind
	(*result).value.str.val = _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8576
	case 6: {
	//ReplaceKind
	(*result).value.str.val = _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8577
	case 7: {
	//ReplaceKind
	(*result).value.str.val = _erealloc(((*(result)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	}
	}
	
			{
	switch(__choose("__SWITCH79"))
	{
	case 0: 
	break;
	#ifdef COMPILE_8578
	case 1: {
	//AddAndReplaceKind
	(*(op1)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8579
	case 2: {
	//AddAndReplaceKind
	(*(op1)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8580
	case 3: {
	//AddAndReplaceKind
	(*(op1)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8581
	case 4: {
	//AddAndReplaceKind
	(*(op1)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8582
	case 5: {
	//AddAndReplaceKind
	(*(op1)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8583
	case 6: {
	//AddAndReplaceKind
	(*(op1)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8584
	case 7: {
	//AddAndReplaceKind
	(*(op2)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8585
	case 8: {
	//AddAndReplaceKind
	(*(op2)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8586
	case 9: {
	//AddAndReplaceKind
	(*(op2)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8587
	case 10: {
	//AddAndReplaceKind
	(*(op2)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8588
	case 11: {
	//AddAndReplaceKind
	(*(op2)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8589
	case 12: {
	//AddAndReplaceKind
	(*(op2)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8590
	case 13: {
	//AddAndReplaceKind
	(*(result)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8591
	case 14: {
	//AddAndReplaceKind
	(*(result)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8592
	case 15: {
	//AddAndReplaceKind
	(*(result)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8593
	case 16: {
	//AddAndReplaceKind
	(*(result)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8594
	case 17: {
	//AddAndReplaceKind
	(*(result)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8595
	case 18: {
	//AddAndReplaceKind
	(*(result)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8596
	case 19: {
	//AddAndReplaceKind
	(*result).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8597
	case 20: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8598
	case 21: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8599
	case 22: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(result)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8600
	case 23: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8601
	case 24: {
	//AddAndReplaceKind
	(*result).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8602
	case 25: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_8603
	case 26: {
	//AddAndReplaceKind
	(op1) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8604
	case 27: {
	//AddAndReplaceKind
	(op1) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8605
	case 28: {
	//AddAndReplaceKind
	(op2) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8606
	case 29: {
	//AddAndReplaceKind
	(op2) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8607
	case 30: {
	//AddAndReplaceKind
	(result) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8608
	case 31: {
	//AddAndReplaceKind
	(result) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8609
	case 32: {
	//AddAndReplaceKind
	_convert_to_string(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8610
	case 33: {
	//AddAndReplaceKind
	_convert_to_string(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8611
	case 34: {
	//AddAndReplaceKind
	_convert_to_string(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8612
	case 35: {
	//AddAndReplaceKind
	_efree(((*(op1)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8613
	case 36: {
	//AddAndReplaceKind
	_efree(((*(op2)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8614
	case 37: {
	//AddAndReplaceKind
	_efree(((*(result)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8615
	case 38: {
	//AddAndReplaceKind
	_efree(((*result).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8616
	case 39: {
	//AddAndReplaceKind
	_efree(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8617
	case 40: {
	//AddAndReplaceKind
	_efree(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8618
	case 41: {
	//AddAndReplaceKind
	_efree(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8619
	case 42: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8620
	case 43: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8621
	case 44: {
	//AddAndReplaceKind
	_zend_hash_init(((*(result)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8622
	case 45: {
	//AddAndReplaceKind
	_zend_list_delete((*(op1)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8623
	case 46: {
	//AddAndReplaceKind
	_zend_list_delete((*(op2)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8624
	case 47: {
	//AddAndReplaceKind
	_zend_list_delete((*(result)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8625
	case 48: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8626
	case 49: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8627
	case 50: {
	//AddAndReplaceKind
	_zval_copy_ctor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8628
	case 51: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8629
	case 52: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8630
	case 53: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8631
	case 54: {
	//AddAndReplaceKind
	_zval_dtor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8632
	case 55: {
	//AddAndReplaceKind
	_zval_dtor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8633
	case 56: {
	//AddAndReplaceKind
	_zval_dtor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8634
	case 57: {
	//AddAndReplaceKind
	_zval_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8635
	case 58: {
	//AddAndReplaceKind
	_zval_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8636
	case 59: {
	//AddAndReplaceKind
	_zval_dtor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8637
	case 60: {
	//AddAndReplaceKind
	_zval_dtor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8638
	case 61: {
	//AddAndReplaceKind
	_zval_dtor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8639
	case 62: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8640
	case 63: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8641
	case 64: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8642
	case 65: {
	//AddAndReplaceKind
	convert_scalar_to_array((op1), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8643
	case 66: {
	//AddAndReplaceKind
	convert_scalar_to_array((op2), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8644
	case 67: {
	//AddAndReplaceKind
	convert_scalar_to_array((result), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8645
	case 68: {
	//AddAndReplaceKind
	convert_scalar_to_number((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8646
	case 69: {
	//AddAndReplaceKind
	convert_scalar_to_number((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8647
	case 70: {
	//AddAndReplaceKind
	convert_scalar_to_number((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8648
	case 71: {
	//AddAndReplaceKind
	convert_to_array((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8649
	case 72: {
	//AddAndReplaceKind
	convert_to_array((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8650
	case 73: {
	//AddAndReplaceKind
	convert_to_array((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8651
	case 74: {
	//AddAndReplaceKind
	convert_to_boolean((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8652
	case 75: {
	//AddAndReplaceKind
	convert_to_boolean((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8653
	case 76: {
	//AddAndReplaceKind
	convert_to_boolean((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8654
	case 77: {
	//AddAndReplaceKind
	convert_to_double((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8655
	case 78: {
	//AddAndReplaceKind
	convert_to_double((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8656
	case 79: {
	//AddAndReplaceKind
	convert_to_double((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8657
	case 80: {
	//AddAndReplaceKind
	convert_to_long((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8658
	case 81: {
	//AddAndReplaceKind
	convert_to_long((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8659
	case 82: {
	//AddAndReplaceKind
	convert_to_long((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8660
	case 83: {
	//AddAndReplaceKind
	convert_to_long_base((op1), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8661
	case 84: {
	//AddAndReplaceKind
	convert_to_long_base((op2), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8662
	case 85: {
	//AddAndReplaceKind
	convert_to_long_base((result), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8663
	case 86: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8664
	case 87: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8665
	case 88: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8666
	case 89: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8667
	case 90: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8668
	case 91: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8669
	case 92: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8670
	case 93: {
	//AddAndReplaceKind
	gc_zval_possible_root((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8671
	case 94: {
	//AddAndReplaceKind
	gc_zval_possible_root((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8672
	case 95: {
	//AddAndReplaceKind
	gc_zval_possible_root((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8673
	case 96: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8674
	case 97: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8675
	case 98: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8676
	case 99: {
	//AddAndReplaceKind
	if ((*op1).type != 6) {
	    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8677
	case 100: {
	//AddAndReplaceKind
	if ((*op2).type != 6) {
	    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8678
	case 101: {
	//AddAndReplaceKind
	if (result == op1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8679
	case 102: {
	//AddAndReplaceKind
	if (use_copy1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8680
	case 103: {
	//AddAndReplaceKind
	if (use_copy2) {
	    _zval_dtor((op2));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8681
	case 104: {
	//AddAndReplaceKind
	if (use_copy2) {
	    op2 = &op2_copy;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8682
	case 105: {
	//AddAndReplaceKind
	memcpy((*(op1)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8683
	case 106: {
	//AddAndReplaceKind
	memcpy((*(op2)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8684
	case 107: {
	//AddAndReplaceKind
	memcpy((*(result)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8685
	case 108: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op1)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8686
	case 109: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op2)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8687
	case 110: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(result)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8688
	case 111: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op1)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8689
	case 112: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op2)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8690
	case 113: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(result)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8691
	case 114: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op1)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8692
	case 115: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op2)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8693
	case 116: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(result)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8694
	case 117: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8695
	case 118: {
	//AddAndReplaceKind
	op1 = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8696
	case 119: {
	//AddAndReplaceKind
	op2 = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8697
	case 120: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "String size overflow");
	
	break;
	}
	#endif
	#ifdef COMPILE_8698
	case 121: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "Unsupported operand types");
	
	break;
	}
	#endif
	#ifdef COMPILE_8699
	case 122: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8700
	case 123: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8701
	case 124: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8702
	case 125: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8703
	case 126: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8704
	case 127: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8705
	case 128: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8706
	case 129: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8707
	case 130: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8708
	case 131: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8709
	case 132: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8710
	case 133: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8711
	case 134: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8712
	case 135: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8713
	case 136: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8714
	case 137: {
	//AddAndReplaceKind
	zend_error((1 << 1L), "Division by zero");
	
	break;
	}
	#endif
	#ifdef COMPILE_8715
	case 138: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_8716
	case 139: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8717
	case 140: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8718
	case 141: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8719
	case 142: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8720
	case 143: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8721
	case 144: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8722
	case 145: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8723
	case 146: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8724
	case 147: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8725
	case 148: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8726
	case 149: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8727
	case 150: {
	//AddAndReplaceKind
	zend_free_obj_get_result((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8728
	case 151: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8729
	case 152: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8730
	case 153: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8731
	case 154: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8732
	case 155: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8733
	case 156: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8734
	case 157: {
	//AddAndReplaceKind
	zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8735
	case 158: {
	//AddAndReplaceKind
	zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8736
	case 159: {
	//AddAndReplaceKind
	zval_unset_isref_p((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8737
	case 160: {
	//AddAndReplaceKind
	zval_unset_isref_p((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8738
	case 161: {
	//AddAndReplaceKind
	zval_unset_isref_p((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8739
	case 162: {
	//AddInitKind
	memset(&(*op2), 0, sizeof (*(&(*op2))));
	
	break;
	}
	#endif
	#ifdef COMPILE_8740
	case 163: {
	//AddInitKind
	memset(&(*op2).value, 0, sizeof (*(&(*op2).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8741
	case 164: {
	//AddInitKind
	memset(&(*op2).value.str, 0, sizeof (*(&(*op2).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8742
	case 165: {
	//AddInitKind
	memset(&(*result), 0, sizeof (*(&(*result))));
	
	break;
	}
	#endif
	#ifdef COMPILE_8743
	case 166: {
	//AddInitKind
	memset(&(*result).value, 0, sizeof (*(&(*result).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8744
	case 167: {
	//AddInitKind
	memset(&(*result).value.str, 0, sizeof (*(&(*result).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8745
	case 168: {
	//IfExitKind
	if (__is_neg("79-168", 16, &(result), sizeof (result), &(op2), sizeof (op2), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(res_len), sizeof (res_len), &(op1), sizeof (op1), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*op2).type), sizeof ((*op2).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8746
	case 169: {
	//IfExitKind
	if (__is_neg("79-169", 16, &(result), sizeof (result), &(op2), sizeof (op2), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(res_len), sizeof (res_len), &(op1), sizeof (op1), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*op2).type), sizeof ((*op2).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_8747
	case 170: {
	//IfExitKind
	if (__is_neg("79-170", 16, &(result), sizeof (result), &(op2), sizeof (op2), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(res_len), sizeof (res_len), &(op1), sizeof (op1), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*op2).type), sizeof ((*op2).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 6;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH80"))
	{
	case 0: {
	memcpy(Z_STRVAL_P(result)+Z_STRLEN_P(result), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	
	break;
	}
	#ifdef COMPILE_8748
	case 1: {
	//AddAndReplaceKind
	__builtin_va_start((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8749
	case 2: {
	//AddAndReplaceKind
	bcmp((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8750
	case 3: {
	//AddAndReplaceKind
	memcmp((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8751
	case 4: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8752
	case 5: {
	//AddAndReplaceKind
	memmove((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8753
	case 6: {
	//AddAndReplaceKind
	printf((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8754
	case 7: {
	//AddAndReplaceKind
	scanf((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8755
	case 8: {
	//GuardKind
	if (!__is_neg("80-8", 16, &(result), sizeof (result), &(op2), sizeof (op2), &((*result).value.str.val), sizeof ((*result).value.str.val), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(res_len), sizeof (res_len), &(op1), sizeof (op1), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*op2).type), sizeof ((*op2).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8756
	case 9: {
	//ReplaceKind
	memcpy((*(op1)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8757
	case 10: {
	//ReplaceKind
	memcpy((*(op2)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8758
	case 11: {
	//ReplaceKind
	memcpy((*(result)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8759
	case 12: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*(op1)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8760
	case 13: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*(op2)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8761
	case 14: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*(result)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8762
	case 15: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op1)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8763
	case 16: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op2)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8764
	case 17: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(result)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8765
	case 18: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op1)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8766
	case 19: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op2)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8767
	case 20: {
	//ReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(result)).value.str.len);
	
	break;
	}
	#endif
	}
	}
			{
	switch(__choose("__SWITCH81"))
	{
	case 0: 
	break;
	#ifdef COMPILE_8768
	case 1: {
	//AddAndReplaceKind
	(*(op1)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8769
	case 2: {
	//AddAndReplaceKind
	(*(op1)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8770
	case 3: {
	//AddAndReplaceKind
	(*(op1)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8771
	case 4: {
	//AddAndReplaceKind
	(*(op1)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8772
	case 5: {
	//AddAndReplaceKind
	(*(op1)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8773
	case 6: {
	//AddAndReplaceKind
	(*(op1)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8774
	case 7: {
	//AddAndReplaceKind
	(*(op2)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8775
	case 8: {
	//AddAndReplaceKind
	(*(op2)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8776
	case 9: {
	//AddAndReplaceKind
	(*(op2)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8777
	case 10: {
	//AddAndReplaceKind
	(*(op2)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8778
	case 11: {
	//AddAndReplaceKind
	(*(op2)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8779
	case 12: {
	//AddAndReplaceKind
	(*(op2)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8780
	case 13: {
	//AddAndReplaceKind
	(*(result)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8781
	case 14: {
	//AddAndReplaceKind
	(*(result)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8782
	case 15: {
	//AddAndReplaceKind
	(*(result)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8783
	case 16: {
	//AddAndReplaceKind
	(*(result)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8784
	case 17: {
	//AddAndReplaceKind
	(*(result)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8785
	case 18: {
	//AddAndReplaceKind
	(*(result)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8786
	case 19: {
	//AddAndReplaceKind
	(*result).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8787
	case 20: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8788
	case 21: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8789
	case 22: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(result)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8790
	case 23: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8791
	case 24: {
	//AddAndReplaceKind
	(*result).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8792
	case 25: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_8793
	case 26: {
	//AddAndReplaceKind
	(op1) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8794
	case 27: {
	//AddAndReplaceKind
	(op1) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8795
	case 28: {
	//AddAndReplaceKind
	(op2) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8796
	case 29: {
	//AddAndReplaceKind
	(op2) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8797
	case 30: {
	//AddAndReplaceKind
	(result) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8798
	case 31: {
	//AddAndReplaceKind
	(result) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8799
	case 32: {
	//AddAndReplaceKind
	_convert_to_string(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8800
	case 33: {
	//AddAndReplaceKind
	_convert_to_string(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8801
	case 34: {
	//AddAndReplaceKind
	_convert_to_string(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8802
	case 35: {
	//AddAndReplaceKind
	_efree(((*(op1)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8803
	case 36: {
	//AddAndReplaceKind
	_efree(((*(op2)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8804
	case 37: {
	//AddAndReplaceKind
	_efree(((*(result)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8805
	case 38: {
	//AddAndReplaceKind
	_efree(((*result).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8806
	case 39: {
	//AddAndReplaceKind
	_efree(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8807
	case 40: {
	//AddAndReplaceKind
	_efree(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8808
	case 41: {
	//AddAndReplaceKind
	_efree(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8809
	case 42: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8810
	case 43: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8811
	case 44: {
	//AddAndReplaceKind
	_zend_hash_init(((*(result)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8812
	case 45: {
	//AddAndReplaceKind
	_zend_list_delete((*(op1)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8813
	case 46: {
	//AddAndReplaceKind
	_zend_list_delete((*(op2)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8814
	case 47: {
	//AddAndReplaceKind
	_zend_list_delete((*(result)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8815
	case 48: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8816
	case 49: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8817
	case 50: {
	//AddAndReplaceKind
	_zval_copy_ctor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8818
	case 51: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8819
	case 52: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8820
	case 53: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8821
	case 54: {
	//AddAndReplaceKind
	_zval_dtor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8822
	case 55: {
	//AddAndReplaceKind
	_zval_dtor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8823
	case 56: {
	//AddAndReplaceKind
	_zval_dtor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8824
	case 57: {
	//AddAndReplaceKind
	_zval_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8825
	case 58: {
	//AddAndReplaceKind
	_zval_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8826
	case 59: {
	//AddAndReplaceKind
	_zval_dtor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8827
	case 60: {
	//AddAndReplaceKind
	_zval_dtor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8828
	case 61: {
	//AddAndReplaceKind
	_zval_dtor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8829
	case 62: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8830
	case 63: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8831
	case 64: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8832
	case 65: {
	//AddAndReplaceKind
	convert_scalar_to_array((op1), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8833
	case 66: {
	//AddAndReplaceKind
	convert_scalar_to_array((op2), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8834
	case 67: {
	//AddAndReplaceKind
	convert_scalar_to_array((result), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_8835
	case 68: {
	//AddAndReplaceKind
	convert_scalar_to_number((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8836
	case 69: {
	//AddAndReplaceKind
	convert_scalar_to_number((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8837
	case 70: {
	//AddAndReplaceKind
	convert_scalar_to_number((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8838
	case 71: {
	//AddAndReplaceKind
	convert_to_array((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8839
	case 72: {
	//AddAndReplaceKind
	convert_to_array((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8840
	case 73: {
	//AddAndReplaceKind
	convert_to_array((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8841
	case 74: {
	//AddAndReplaceKind
	convert_to_boolean((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8842
	case 75: {
	//AddAndReplaceKind
	convert_to_boolean((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8843
	case 76: {
	//AddAndReplaceKind
	convert_to_boolean((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8844
	case 77: {
	//AddAndReplaceKind
	convert_to_double((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8845
	case 78: {
	//AddAndReplaceKind
	convert_to_double((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8846
	case 79: {
	//AddAndReplaceKind
	convert_to_double((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8847
	case 80: {
	//AddAndReplaceKind
	convert_to_long((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8848
	case 81: {
	//AddAndReplaceKind
	convert_to_long((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8849
	case 82: {
	//AddAndReplaceKind
	convert_to_long((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8850
	case 83: {
	//AddAndReplaceKind
	convert_to_long_base((op1), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8851
	case 84: {
	//AddAndReplaceKind
	convert_to_long_base((op2), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8852
	case 85: {
	//AddAndReplaceKind
	convert_to_long_base((result), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_8853
	case 86: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8854
	case 87: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8855
	case 88: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8856
	case 89: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8857
	case 90: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8858
	case 91: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8859
	case 92: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8860
	case 93: {
	//AddAndReplaceKind
	gc_zval_possible_root((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8861
	case 94: {
	//AddAndReplaceKind
	gc_zval_possible_root((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8862
	case 95: {
	//AddAndReplaceKind
	gc_zval_possible_root((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8863
	case 96: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8864
	case 97: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8865
	case 98: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8866
	case 99: {
	//AddAndReplaceKind
	if ((*op1).type != 6) {
	    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8867
	case 100: {
	//AddAndReplaceKind
	if ((*op2).type != 6) {
	    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8868
	case 101: {
	//AddAndReplaceKind
	if (result == op1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8869
	case 102: {
	//AddAndReplaceKind
	if (use_copy1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8870
	case 103: {
	//AddAndReplaceKind
	if (use_copy2) {
	    _zval_dtor((op2));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8871
	case 104: {
	//AddAndReplaceKind
	if (use_copy2) {
	    op2 = &op2_copy;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_8872
	case 105: {
	//AddAndReplaceKind
	memcpy((*(op1)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8873
	case 106: {
	//AddAndReplaceKind
	memcpy((*(op2)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8874
	case 107: {
	//AddAndReplaceKind
	memcpy((*(result)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8875
	case 108: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op1)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8876
	case 109: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op2)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8877
	case 110: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(result)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8878
	case 111: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op1)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8879
	case 112: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op2)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8880
	case 113: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(result)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8881
	case 114: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op1)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8882
	case 115: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op2)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8883
	case 116: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(result)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8884
	case 117: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_8885
	case 118: {
	//AddAndReplaceKind
	op1 = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8886
	case 119: {
	//AddAndReplaceKind
	op2 = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8887
	case 120: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "String size overflow");
	
	break;
	}
	#endif
	#ifdef COMPILE_8888
	case 121: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "Unsupported operand types");
	
	break;
	}
	#endif
	#ifdef COMPILE_8889
	case 122: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8890
	case 123: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8891
	case 124: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8892
	case 125: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8893
	case 126: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8894
	case 127: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8895
	case 128: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8896
	case 129: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8897
	case 130: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8898
	case 131: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8899
	case 132: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8900
	case 133: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8901
	case 134: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_8902
	case 135: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_8903
	case 136: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_8904
	case 137: {
	//AddAndReplaceKind
	zend_error((1 << 1L), "Division by zero");
	
	break;
	}
	#endif
	#ifdef COMPILE_8905
	case 138: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_8906
	case 139: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8907
	case 140: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8908
	case 141: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8909
	case 142: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8910
	case 143: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8911
	case 144: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8912
	case 145: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8913
	case 146: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8914
	case 147: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_8915
	case 148: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8916
	case 149: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8917
	case 150: {
	//AddAndReplaceKind
	zend_free_obj_get_result((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8918
	case 151: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8919
	case 152: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8920
	case 153: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8921
	case 154: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8922
	case 155: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8923
	case 156: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8924
	case 157: {
	//AddAndReplaceKind
	zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_8925
	case 158: {
	//AddAndReplaceKind
	zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_8926
	case 159: {
	//AddAndReplaceKind
	zval_unset_isref_p((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8927
	case 160: {
	//AddAndReplaceKind
	zval_unset_isref_p((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8928
	case 161: {
	//AddAndReplaceKind
	zval_unset_isref_p((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8929
	case 162: {
	//AddInitKind
	memset(&(*result), 0, sizeof (*(&(*result))));
	
	break;
	}
	#endif
	#ifdef COMPILE_8930
	case 163: {
	//AddInitKind
	memset(&(*result).value, 0, sizeof (*(&(*result).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8931
	case 164: {
	//AddInitKind
	memset(&(*result).value.str, 0, sizeof (*(&(*result).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8932
	case 165: {
	//IfExitKind
	if (__is_neg("81-165", 16, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(op1), sizeof (op1), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*op2).type), sizeof ((*op2).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8933
	case 166: {
	//IfExitKind
	if (__is_neg("81-166", 16, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(op1), sizeof (op1), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*op2).type), sizeof ((*op2).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_8934
	case 167: {
	//IfExitKind
	if (__is_neg("81-167", 16, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(op1), sizeof (op1), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*op2).type), sizeof ((*op2).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    return 6;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH82"))
	{
	case 0: {
	Z_STRVAL_P(result)[res_len]=0;
	
	break;
	}
	#ifdef COMPILE_8935
	case 1: {
	//GuardKind
	if (!__is_neg("82-1", 16, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op2), sizeof (op2), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*op2).value.str.val), sizeof ((*op2).value.str.val), &((*op2).value.str.len), sizeof ((*op2).value.str.len), &(op1), sizeof (op1), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*op2).type), sizeof ((*op2).type), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc), &((*op2).refcount__gc), sizeof ((*op2).refcount__gc), &((*op2).is_ref__gc), sizeof ((*op2).is_ref__gc)))
	    (*result).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8936
	case 2: {
	//ReplaceKind
	(*(op1)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8937
	case 3: {
	//ReplaceKind
	(*(op2)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8938
	case 4: {
	//ReplaceKind
	(*(result)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	}
	}
			{
	switch(__choose("__SWITCH83"))
	{
	case 0: 
	break;
	#ifdef COMPILE_8939
	case 1: {
	//AddAndReplaceKind
	(*(op1)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8940
	case 2: {
	//AddAndReplaceKind
	(*(op1)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8941
	case 3: {
	//AddAndReplaceKind
	(*(op1)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8942
	case 4: {
	//AddAndReplaceKind
	(*(op1)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8943
	case 5: {
	//AddAndReplaceKind
	(*(op1)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8944
	case 6: {
	//AddAndReplaceKind
	(*(op1)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8945
	case 7: {
	//AddAndReplaceKind
	(*(op2)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8946
	case 8: {
	//AddAndReplaceKind
	(*(op2)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8947
	case 9: {
	//AddAndReplaceKind
	(*(op2)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8948
	case 10: {
	//AddAndReplaceKind
	(*(op2)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8949
	case 11: {
	//AddAndReplaceKind
	(*(op2)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8950
	case 12: {
	//AddAndReplaceKind
	(*(op2)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8951
	case 13: {
	//AddAndReplaceKind
	(*(result)).value.lval++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8952
	case 14: {
	//AddAndReplaceKind
	(*(result)).value.lval--;
	
	break;
	}
	#endif
	#ifdef COMPILE_8953
	case 15: {
	//AddAndReplaceKind
	(*(result)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8954
	case 16: {
	//AddAndReplaceKind
	(*(result)).value.str.len++;
	
	break;
	}
	#endif
	#ifdef COMPILE_8955
	case 17: {
	//AddAndReplaceKind
	(*(result)).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8956
	case 18: {
	//AddAndReplaceKind
	(*(result)).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8957
	case 19: {
	//AddAndReplaceKind
	(*result).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_8958
	case 20: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op1)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8959
	case 21: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(op2)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8960
	case 22: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*(result)).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8961
	case 23: {
	//AddAndReplaceKind
	(*result).value.str.val = _erealloc(((*result).value.str.val), (res_len + 1), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_8962
	case 24: {
	//AddAndReplaceKind
	(*result).value.str.val[res_len] = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8963
	case 25: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_8964
	case 26: {
	//AddAndReplaceKind
	(op1) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8965
	case 27: {
	//AddAndReplaceKind
	(op1) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8966
	case 28: {
	//AddAndReplaceKind
	(op2) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8967
	case 29: {
	//AddAndReplaceKind
	(op2) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8968
	case 30: {
	//AddAndReplaceKind
	(result) = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8969
	case 31: {
	//AddAndReplaceKind
	(result) = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_8970
	case 32: {
	//AddAndReplaceKind
	_convert_to_string(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8971
	case 33: {
	//AddAndReplaceKind
	_convert_to_string(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8972
	case 34: {
	//AddAndReplaceKind
	_convert_to_string(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8973
	case 35: {
	//AddAndReplaceKind
	_efree(((*(op1)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8974
	case 36: {
	//AddAndReplaceKind
	_efree(((*(op2)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8975
	case 37: {
	//AddAndReplaceKind
	_efree(((*(result)).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8976
	case 38: {
	//AddAndReplaceKind
	_efree(((*result).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_8977
	case 39: {
	//AddAndReplaceKind
	_efree(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8978
	case 40: {
	//AddAndReplaceKind
	_efree(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8979
	case 41: {
	//AddAndReplaceKind
	_efree(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8980
	case 42: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op1)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8981
	case 43: {
	//AddAndReplaceKind
	_zend_hash_init(((*(op2)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8982
	case 44: {
	//AddAndReplaceKind
	_zend_hash_init(((*(result)).value.ht), (0), (((void *)0)), ((void (*)(void *))_zval_ptr_dtor), (0));
	
	break;
	}
	#endif
	#ifdef COMPILE_8983
	case 45: {
	//AddAndReplaceKind
	_zend_list_delete((*(op1)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8984
	case 46: {
	//AddAndReplaceKind
	_zend_list_delete((*(op2)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8985
	case 47: {
	//AddAndReplaceKind
	_zend_list_delete((*(result)).value.lval);
	
	break;
	}
	#endif
	#ifdef COMPILE_8986
	case 48: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8987
	case 49: {
	//AddAndReplaceKind
	_zval_copy_ctor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8988
	case 50: {
	//AddAndReplaceKind
	_zval_copy_ctor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8989
	case 51: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8990
	case 52: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8991
	case 53: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_8992
	case 54: {
	//AddAndReplaceKind
	_zval_dtor(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8993
	case 55: {
	//AddAndReplaceKind
	_zval_dtor(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8994
	case 56: {
	//AddAndReplaceKind
	_zval_dtor(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_8995
	case 57: {
	//AddAndReplaceKind
	_zval_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8996
	case 58: {
	//AddAndReplaceKind
	_zval_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8997
	case 59: {
	//AddAndReplaceKind
	_zval_dtor_func((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_8998
	case 60: {
	//AddAndReplaceKind
	_zval_dtor_func((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_8999
	case 61: {
	//AddAndReplaceKind
	_zval_dtor_func((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9000
	case 62: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9001
	case 63: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9002
	case 64: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9003
	case 65: {
	//AddAndReplaceKind
	convert_scalar_to_array((op1), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_9004
	case 66: {
	//AddAndReplaceKind
	convert_scalar_to_array((op2), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_9005
	case 67: {
	//AddAndReplaceKind
	convert_scalar_to_array((result), 4);
	
	break;
	}
	#endif
	#ifdef COMPILE_9006
	case 68: {
	//AddAndReplaceKind
	convert_scalar_to_number((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9007
	case 69: {
	//AddAndReplaceKind
	convert_scalar_to_number((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9008
	case 70: {
	//AddAndReplaceKind
	convert_scalar_to_number((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9009
	case 71: {
	//AddAndReplaceKind
	convert_to_array((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9010
	case 72: {
	//AddAndReplaceKind
	convert_to_array((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9011
	case 73: {
	//AddAndReplaceKind
	convert_to_array((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9012
	case 74: {
	//AddAndReplaceKind
	convert_to_boolean((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9013
	case 75: {
	//AddAndReplaceKind
	convert_to_boolean((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9014
	case 76: {
	//AddAndReplaceKind
	convert_to_boolean((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9015
	case 77: {
	//AddAndReplaceKind
	convert_to_double((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9016
	case 78: {
	//AddAndReplaceKind
	convert_to_double((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9017
	case 79: {
	//AddAndReplaceKind
	convert_to_double((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9018
	case 80: {
	//AddAndReplaceKind
	convert_to_long((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9019
	case 81: {
	//AddAndReplaceKind
	convert_to_long((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9020
	case 82: {
	//AddAndReplaceKind
	convert_to_long((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9021
	case 83: {
	//AddAndReplaceKind
	convert_to_long_base((op1), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_9022
	case 84: {
	//AddAndReplaceKind
	convert_to_long_base((op2), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_9023
	case 85: {
	//AddAndReplaceKind
	convert_to_long_base((result), 10);
	
	break;
	}
	#endif
	#ifdef COMPILE_9024
	case 86: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9025
	case 87: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9026
	case 88: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9027
	case 89: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9028
	case 90: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op1)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9029
	case 91: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((op2)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9030
	case 92: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((result)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9031
	case 93: {
	//AddAndReplaceKind
	gc_zval_possible_root((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9032
	case 94: {
	//AddAndReplaceKind
	gc_zval_possible_root((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9033
	case 95: {
	//AddAndReplaceKind
	gc_zval_possible_root((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9034
	case 96: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9035
	case 97: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9036
	case 98: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9037
	case 99: {
	//AddAndReplaceKind
	if ((*op1).type != 6) {
	    zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9038
	case 100: {
	//AddAndReplaceKind
	if ((*op2).type != 6) {
	    zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9039
	case 101: {
	//AddAndReplaceKind
	if (result == op1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9040
	case 102: {
	//AddAndReplaceKind
	if (use_copy1) {
	    _zval_dtor((op1));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9041
	case 103: {
	//AddAndReplaceKind
	if (use_copy2) {
	    _zval_dtor((op2));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9042
	case 104: {
	//AddAndReplaceKind
	if (use_copy2) {
	    op2 = &op2_copy;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_9043
	case 105: {
	//AddAndReplaceKind
	memcpy((*(op1)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9044
	case 106: {
	//AddAndReplaceKind
	memcpy((*(op2)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9045
	case 107: {
	//AddAndReplaceKind
	memcpy((*(result)).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9046
	case 108: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op1)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9047
	case 109: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(op2)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9048
	case 110: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*(result)).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9049
	case 111: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op1)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9050
	case 112: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(op2)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9051
	case 113: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*(result)).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9052
	case 114: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op1)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9053
	case 115: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(op2)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9054
	case 116: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*(result)).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9055
	case 117: {
	//AddAndReplaceKind
	memcpy((*result).value.str.val + (*result).value.str.len, (*op2).value.str.val, (*op2).value.str.len);
	
	break;
	}
	#endif
	#ifdef COMPILE_9056
	case 118: {
	//AddAndReplaceKind
	op1 = &op1_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_9057
	case 119: {
	//AddAndReplaceKind
	op2 = &op2_copy;
	
	break;
	}
	#endif
	#ifdef COMPILE_9058
	case 120: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "String size overflow");
	
	break;
	}
	#endif
	#ifdef COMPILE_9059
	case 121: {
	//AddAndReplaceKind
	zend_error((1 << 0L), "Unsupported operand types");
	
	break;
	}
	#endif
	#ifdef COMPILE_9060
	case 122: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9061
	case 123: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9062
	case 124: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_9063
	case 125: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_9064
	case 126: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op1)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_9065
	case 127: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9066
	case 128: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9067
	case 129: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_9068
	case 130: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_9069
	case 131: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(op2)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_9070
	case 132: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9071
	case 133: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9072
	case 134: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(3));
	
	break;
	}
	#endif
	#ifdef COMPILE_9073
	case 135: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(4));
	
	break;
	}
	#endif
	#ifdef COMPILE_9074
	case 136: {
	//AddAndReplaceKind
	zend_error((1 << 12L), "Object of class %s could not be converted to %s", zend_get_class_entry(&(*(result)))->name, zend_get_type_by_const(6));
	
	break;
	}
	#endif
	#ifdef COMPILE_9075
	case 137: {
	//AddAndReplaceKind
	zend_error((1 << 1L), "Division by zero");
	
	break;
	}
	#endif
	#ifdef COMPILE_9076
	case 138: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_9077
	case 139: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9078
	case 140: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9079
	case 141: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to double", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9080
	case 142: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9081
	case 143: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9082
	case 144: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s could not be converted to int", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9083
	case 145: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op1)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9084
	case 146: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(op2)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9085
	case 147: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "Object of class %s to string conversion", zend_get_class_entry(&(*(result)))->name);
	
	break;
	}
	#endif
	#ifdef COMPILE_9086
	case 148: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9087
	case 149: {
	//AddAndReplaceKind
	zend_free_obj_get_result((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9088
	case 150: {
	//AddAndReplaceKind
	zend_free_obj_get_result((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9089
	case 151: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9090
	case 152: {
	//AddAndReplaceKind
	zend_make_printable_zval((op1), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_9091
	case 153: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9092
	case 154: {
	//AddAndReplaceKind
	zend_make_printable_zval((op2), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_9093
	case 155: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9094
	case 156: {
	//AddAndReplaceKind
	zend_make_printable_zval((result), &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_9095
	case 157: {
	//AddAndReplaceKind
	zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	
	break;
	}
	#endif
	#ifdef COMPILE_9096
	case 158: {
	//AddAndReplaceKind
	zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	
	break;
	}
	#endif
	#ifdef COMPILE_9097
	case 159: {
	//AddAndReplaceKind
	zval_unset_isref_p((op1));
	
	break;
	}
	#endif
	#ifdef COMPILE_9098
	case 160: {
	//AddAndReplaceKind
	zval_unset_isref_p((op2));
	
	break;
	}
	#endif
	#ifdef COMPILE_9099
	case 161: {
	//AddAndReplaceKind
	zval_unset_isref_p((result));
	
	break;
	}
	#endif
	#ifdef COMPILE_9100
	case 162: {
	//AddInitKind
	memset(&(*result), 0, sizeof (*(&(*result))));
	
	break;
	}
	#endif
	#ifdef COMPILE_9101
	case 163: {
	//AddInitKind
	memset(&(*result).value, 0, sizeof (*(&(*result).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9102
	case 164: {
	//AddInitKind
	memset(&(*result).value.str, 0, sizeof (*(&(*result).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_9103
	case 165: {
	//IfExitKind
	if (__is_neg("83-165", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op1), sizeof (op1), &(op2), sizeof (op2), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    return 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_9104
	case 166: {
	//IfExitKind
	if (__is_neg("83-166", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op1), sizeof (op1), &(op2), sizeof (op2), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    return 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9105
	case 167: {
	//IfExitKind
	if (__is_neg("83-167", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op1), sizeof (op1), &(op2), sizeof (op2), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    return 6;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH84"))
	{
	case 0: {
	Z_STRLEN_P(result) = res_len;
	
	break;
	}
	#ifdef COMPILE_9106
	case 1: {
	//GuardKind
	if (!__is_neg("84-1", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op1), sizeof (op1), &(op2), sizeof (op2), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    (*result).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_9107
	case 2: {
	//ReplaceKind
	(*(op1)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_9108
	case 3: {
	//ReplaceKind
	(*(op2)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_9109
	case 4: {
	//ReplaceKind
	(*(result)).value.str.len = res_len;
	
	break;
	}
	#endif
	}
	}
		} else {
			int length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
			char *buf = (char *) emalloc(length + 1);
	
			memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
			memcpy(buf + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
			buf[length] = 0;
			ZVAL_STRINGL(result, buf, length, 0);
		}
	
	break;
	}
	#ifdef COMPILE_9110
	case 1: {
	//GuardKind
	if (!__is_neg("85-1", 11, &(result), sizeof (result), &(res_len), sizeof (res_len), &((*result).value.str.len), sizeof ((*result).value.str.len), &((*result).value.str.val), sizeof ((*result).value.str.val), &(op1), sizeof (op1), &(op2), sizeof (op2), &(use_copy1), sizeof (use_copy1), &(use_copy2), sizeof (use_copy2), &((*result).refcount__gc), sizeof ((*result).refcount__gc), &((*result).type), sizeof ((*result).type), &((*result).is_ref__gc), sizeof ((*result).is_ref__gc)))
	    (*result).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_9111
	case 2: {
	//ReplaceKind
	(*(op1)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_9112
	case 3: {
	//ReplaceKind
	(*(op2)).value.str.len = res_len;
	
	break;
	}
	#endif
	#ifdef COMPILE_9113
	case 4: {
	//ReplaceKind
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

ZEND_API int string_compare_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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
ZEND_API int string_locale_compare_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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

ZEND_API int numeric_compare_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	zval op1_copy, op2_copy;

	op1_copy = *op1;
	zval_copy_ctor(&op1_copy);

	op2_copy = *op2;
	zval_copy_ctor(&op2_copy);

	convert_to_double(&op1_copy);
	convert_to_double(&op2_copy);

	ZVAL_LONG(result, ZEND_NORMALIZE_BOOL(Z_DVAL(op1_copy)-Z_DVAL(op2_copy)));

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

ZEND_API int compare_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	int ret;
	int converted = 0;
	zval op1_copy, op2_copy;
	zval *op_free;

	while (1) {
		switch (TYPE_PAIR(Z_TYPE_P(op1), Z_TYPE_P(op2))) {
			case TYPE_PAIR(IS_LONG, IS_LONG):
				ZVAL_LONG(result, Z_LVAL_P(op1)>Z_LVAL_P(op2)?1:(Z_LVAL_P(op1)<Z_LVAL_P(op2)?-1:0));
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
				ZVAL_LONG(result, zend_binary_strcmp("", 0, Z_STRVAL_P(op2), Z_STRLEN_P(op2)));
				return SUCCESS;

			case TYPE_PAIR(IS_STRING, IS_NULL):
				ZVAL_LONG(result, zend_binary_strcmp(Z_STRVAL_P(op1), Z_STRLEN_P(op1), "", 0));
				return SUCCESS;

			case TYPE_PAIR(IS_OBJECT, IS_NULL):
				ZVAL_LONG(result, 1);
				return SUCCESS;

			case TYPE_PAIR(IS_NULL, IS_OBJECT):
				ZVAL_LONG(result, -1);
				return SUCCESS;

			case TYPE_PAIR(IS_OBJECT, IS_OBJECT):
				/* If both are objects sharing the same comparision handler then use is */
				if (Z_OBJ_HANDLER_P(op1,compare_objects) == Z_OBJ_HANDLER_P(op2,compare_objects)) {
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
						if (Z_OBJ_HT_P(op1)->cast_object(op1, op_free, Z_TYPE_P(op2) TSRMLS_CC) == FAILURE) {
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
						if (Z_OBJ_HT_P(op2)->cast_object(op2, op_free, Z_TYPE_P(op1) TSRMLS_CC) == FAILURE) {
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
				} else if (Z_TYPE_P(op1)==IS_ARRAY) {
					ZVAL_LONG(result, 1);
					return SUCCESS;
				} else if (Z_TYPE_P(op2)==IS_ARRAY) {
					ZVAL_LONG(result, -1);
					return SUCCESS;
				} else if (Z_TYPE_P(op1)==IS_OBJECT) {
					ZVAL_LONG(result, 1);
					return SUCCESS;
				} else if (Z_TYPE_P(op2)==IS_OBJECT) {
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

static int hash_zval_identical_function(const zval **z1, const zval **z2) /* {{{ */
{
	zval result;
	TSRMLS_FETCH();

	/* is_identical_function() returns 1 in case of identity and 0 in case
	 * of a difference;
	 * whereas this comparison function is expected to return 0 on identity,
	 * and non zero otherwise.
	 */
	if (is_identical_function(&result, (zval *) *z1, (zval *) *z2 TSRMLS_CC)==FAILURE) {
		return 1;
	}
	return !Z_LVAL(result);
}
/* }}} */

ZEND_API int is_identical_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
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
			Z_LVAL_P(result) = ((Z_STRLEN_P(op1) == Z_STRLEN_P(op2))
				&& (!memcmp(Z_STRVAL_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op1))));
			break;
		case IS_ARRAY:
			Z_LVAL_P(result) = zend_hash_compare(Z_ARRVAL_P(op1), Z_ARRVAL_P(op2), (compare_func_t) hash_zval_identical_function, 1 TSRMLS_CC)==0;
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

ZEND_API int is_not_identical_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	if (is_identical_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
		return FAILURE;
	}
	Z_LVAL_P(result) = !Z_LVAL_P(result);
	return SUCCESS;
}
/* }}} */

ZEND_API int is_equal_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	if (compare_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
		return FAILURE;
	}
	ZVAL_BOOL(result, (Z_LVAL_P(result) == 0));
	return SUCCESS;
}
/* }}} */

ZEND_API int is_not_equal_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	if (compare_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
		return FAILURE;
	}
	ZVAL_BOOL(result, (Z_LVAL_P(result) != 0));
	return SUCCESS;
}
/* }}} */

ZEND_API int is_smaller_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	if (compare_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
		return FAILURE;
	}
	ZVAL_BOOL(result, (Z_LVAL_P(result) < 0));
	return SUCCESS;
}
/* }}} */

ZEND_API int is_smaller_or_equal_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	if (compare_function(result, op1, op2 TSRMLS_CC) == FAILURE) {
		return FAILURE;
	}
	ZVAL_BOOL(result, (Z_LVAL_P(result) <= 0));
	return SUCCESS;
}
/* }}} */

ZEND_API zend_bool instanceof_function_ex(const zend_class_entry *instance_ce, const zend_class_entry *ce, zend_bool interfaces_only TSRMLS_DC) /* {{{ */
{
	zend_uint i;

	for (i=0; i<instance_ce->num_interfaces; i++) {
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

ZEND_API zend_bool instanceof_function(const zend_class_entry *instance_ce, const zend_class_entry *ce TSRMLS_DC) /* {{{ */
{
	return instanceof_function_ex(instance_ce, ce, 0 TSRMLS_CC);
}
/* }}} */

#define LOWER_CASE 1
#define UPPER_CASE 2
#define NUMERIC 3

static void increment_string(zval *str) /* {{{ */
{
	int carry=0;
	int pos=Z_STRLEN_P(str)-1;
	char *s=Z_STRVAL_P(str);
	char *t;
	int last=0; /* Shut up the compiler warning */
	int ch;

	if (Z_STRLEN_P(str) == 0) {
		STR_FREE(Z_STRVAL_P(str));
		Z_STRVAL_P(str) = estrndup("1", sizeof("1")-1);
		Z_STRLEN_P(str) = 1;
		return;
	}

	if (IS_INTERNED(s)) {
		s = (char*) emalloc(Z_STRLEN_P(str) + 1);
		memcpy(s, Z_STRVAL_P(str), Z_STRLEN_P(str) + 1);
		Z_STRVAL_P(str) = s;
	}

	while (pos >= 0) {
		ch = s[pos];
		if (ch >= 'a' && ch <= 'z') {
			if (ch == 'z') {
				s[pos] = 'a';
				carry=1;
			} else {
				s[pos]++;
				carry=0;
			}
			last=LOWER_CASE;
		} else if (ch >= 'A' && ch <= 'Z') {
			if (ch == 'Z') {
				s[pos] = 'A';
				carry=1;
			} else {
				s[pos]++;
				carry=0;
			}
			last=UPPER_CASE;
		} else if (ch >= '0' && ch <= '9') {
			if (ch == '9') {
				s[pos] = '0';
				carry=1;
			} else {
				s[pos]++;
				carry=0;
			}
			last = NUMERIC;
		} else {
			carry=0;
			break;
		}
		if (carry == 0) {
			break;
		}
		pos--;
	}

	if (carry) {
		t = (char *) emalloc(Z_STRLEN_P(str)+1+1);
		memcpy(t+1, Z_STRVAL_P(str), Z_STRLEN_P(str));
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
				ZVAL_DOUBLE(op1, d+1);
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

				switch (is_numeric_string(Z_STRVAL_P(op1), Z_STRLEN_P(op1), &lval, &dval, 0)) {
					case IS_LONG:
						str_efree(Z_STRVAL_P(op1));
						if (lval == LONG_MAX) {
							/* switch to double */
							double d = (double)lval;
							ZVAL_DOUBLE(op1, d+1);
						} else {
							ZVAL_LONG(op1, lval+1);
						}
						break;
					case IS_DOUBLE:
						str_efree(Z_STRVAL_P(op1));
						ZVAL_DOUBLE(op1, dval+1);
						break;
					default:
						/* Perl style string increment */
						increment_string(op1);
						break;
				}
			}
			break;
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
				ZVAL_DOUBLE(op1, d-1);
			} else {
			Z_LVAL_P(op1)--;
			}
			break;
		case IS_DOUBLE:
			Z_DVAL_P(op1) = Z_DVAL_P(op1) - 1;
			break;
		case IS_STRING:		/* Like perl we only support string increment */
			if (Z_STRLEN_P(op1) == 0) { /* consider as 0 */
				STR_FREE(Z_STRVAL_P(op1));
				ZVAL_LONG(op1, -1);
				break;
			}
			switch (is_numeric_string(Z_STRVAL_P(op1), Z_STRLEN_P(op1), &lval, &dval, 0)) {
				case IS_LONG:
					STR_FREE(Z_STRVAL_P(op1));
					if (lval == LONG_MIN) {
						double d = (double)lval;
						ZVAL_DOUBLE(op1, d-1);
					} else {
						ZVAL_LONG(op1, lval-1);
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

ZEND_API char *zend_str_tolower_copy(char *dest, const char *source, unsigned int length) /* {{{ */
{
	register unsigned char *str = (unsigned char*)source;
	register unsigned char *result = (unsigned char*)dest;
	register unsigned char *end = str + length;

	while (str < end) {
		*result++ = zend_tolower((int)*str++);
	}
	*result = '\0';

	return dest;
}
/* }}} */

ZEND_API char *zend_str_tolower_dup(const char *source, unsigned int length) /* {{{ */
{
	return zend_str_tolower_copy((char *)emalloc(length+1), source, length);
}
/* }}} */

ZEND_API void zend_str_tolower(char *str, unsigned int length) /* {{{ */
{
	register unsigned char *p = (unsigned char*)str;
	register unsigned char *end = p + length;

	while (p < end) {
		*p = zend_tolower((int)*p);
		p++;
	}
}
/* }}} */

ZEND_API int zend_binary_strcmp(const char *s1, uint len1, const char *s2, uint len2) /* {{{ */
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

ZEND_API int zend_binary_strncmp(const char *s1, uint len1, const char *s2, uint len2, uint length) /* {{{ */
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

ZEND_API int zend_binary_strcasecmp(const char *s1, uint len1, const char *s2, uint len2) /* {{{ */
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

ZEND_API int zend_binary_strncasecmp(const char *s1, uint len1, const char *s2, uint len2, uint length) /* {{{ */
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
	return zend_binary_strcmp(Z_STRVAL_P(s1), Z_STRLEN_P(s1), Z_STRVAL_P(s2), Z_STRLEN_P(s2));
}
/* }}} */

ZEND_API int zend_binary_zval_strncmp(zval *s1, zval *s2, zval *s3) /* {{{ */
{
	return zend_binary_strncmp(Z_STRVAL_P(s1), Z_STRLEN_P(s1), Z_STRVAL_P(s2), Z_STRLEN_P(s2), Z_LVAL_P(s3));
}
/* }}} */

ZEND_API int zend_binary_zval_strcasecmp(zval *s1, zval *s2) /* {{{ */
{
	return zend_binary_strcasecmp(Z_STRVAL_P(s1), Z_STRLEN_P(s1), Z_STRVAL_P(s2), Z_STRLEN_P(s2));
}
/* }}} */

ZEND_API int zend_binary_zval_strncasecmp(zval *s1, zval *s2, zval *s3) /* {{{ */
{
	return zend_binary_strncasecmp(Z_STRVAL_P(s1), Z_STRLEN_P(s1), Z_STRVAL_P(s2), Z_STRLEN_P(s2), Z_LVAL_P(s3));
}
/* }}} */

ZEND_API void zendi_smart_strcmp(zval *result, zval *s1, zval *s2) /* {{{ */
{
	int ret1, ret2;
	long lval1, lval2;
	double dval1, dval2;

	if ((ret1=is_numeric_string(Z_STRVAL_P(s1), Z_STRLEN_P(s1), &lval1, &dval1, 0)) &&
		(ret2=is_numeric_string(Z_STRVAL_P(s2), Z_STRLEN_P(s2), &lval2, &dval2, 0))) {
		if ((ret1==IS_DOUBLE) || (ret2==IS_DOUBLE)) {
			if (ret1!=IS_DOUBLE) {
				dval1 = (double) lval1;
			} else if (ret2!=IS_DOUBLE) {
				dval2 = (double) lval2;
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

static int hash_zval_compare_function(const zval **z1, const zval **z2 TSRMLS_DC) /* {{{ */
{
	zval result;

	if (compare_function(&result, (zval *) *z1, (zval *) *z2 TSRMLS_CC)==FAILURE) {
		return 1;
	}
	return Z_LVAL(result);
}
/* }}} */

ZEND_API int zend_compare_symbol_tables_i(HashTable *ht1, HashTable *ht2 TSRMLS_DC) /* {{{ */
{
	return zend_hash_compare(ht1, ht2, (compare_func_t) hash_zval_compare_function, 0 TSRMLS_CC);
}
/* }}} */

ZEND_API void zend_compare_symbol_tables(zval *result, HashTable *ht1, HashTable *ht2 TSRMLS_DC) /* {{{ */
{
	ZVAL_LONG(result, zend_hash_compare(ht1, ht2, (compare_func_t) hash_zval_compare_function, 0 TSRMLS_CC));
}
/* }}} */

ZEND_API void zend_compare_arrays(zval *result, zval *a1, zval *a2 TSRMLS_DC) /* {{{ */
{
	zend_compare_symbol_tables(result, Z_ARRVAL_P(a1), Z_ARRVAL_P(a2) TSRMLS_CC);
}
/* }}} */

ZEND_API void zend_compare_objects(zval *result, zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
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

	Z_STRLEN_P(op) = zend_spprintf(&Z_STRVAL_P(op), 0, "%.*G", (int) EG(precision), (double)Z_DVAL_P(op));
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
