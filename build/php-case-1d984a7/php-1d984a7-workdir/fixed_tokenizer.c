int __get_mutant(); int __is_neg(const char *location,int count, ...); int __abst_hole(); int __choose(const char *);void* memset(void*, int, unsigned long); 
/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2011 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Andrei Zmievski <andrei@php.net>                             |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_tokenizer.h"

#include "zend.h"
#include "zend_language_scanner.h"
#include "zend_language_scanner_defs.h"
#include <zend_language_parser.h>

#define zendtext LANG_SCNG(yy_text)
#define zendleng LANG_SCNG(yy_leng)

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_token_get_all, 0, 0, 1)
	ZEND_ARG_INFO(0, source)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_token_name, 0, 0, 1)
	ZEND_ARG_INFO(0, token)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ tokenizer_functions[]
 *
 * Every user visible function must have an entry in tokenizer_functions[].
 */
const zend_function_entry tokenizer_functions[] = {
	PHP_FE(token_get_all,	arginfo_token_get_all)
	PHP_FE(token_name,		arginfo_token_name)
	{NULL, NULL, NULL}	/* Must be the last line in tokenizer_functions[] */
};
/* }}} */

/* {{{ tokenizer_module_entry
 */
zend_module_entry tokenizer_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"tokenizer",
	tokenizer_functions,
	PHP_MINIT(tokenizer),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(tokenizer),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TOKENIZER
ZEND_GET_MODULE(tokenizer)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(tokenizer)
{
	tokenizer_register_constants(INIT_FUNC_ARGS_PASSTHRU);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(tokenizer)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	php_info_print_table_end();
}
/* }}} */

static void tokenize(zval *return_value TSRMLS_DC)
{
	zval token;
	zval *keyword;
	int token_type;
	zend_bool destroy;
	int token_line = 1;

	//prophet generated patch
	{
	switch(__choose("__SWITCH0"))
	{
	case 0: 
	break;
	#ifdef COMPILE_0
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_3
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_4
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_5
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_6
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_7
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_8
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_9
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_10
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_11
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_12
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_13
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_14
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_15
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_16
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_17
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_18
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_19
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_20
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_21
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_22
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_23
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_24
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_25
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_26
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_27
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_28
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_29
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_30
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_31
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_32
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_33
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_34
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_35
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_36
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_37
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_38
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_39
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_40
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_41
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_42
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_43
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_44
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_45
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_46
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_47
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_48
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_49
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_50
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_51
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_52
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_53
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_54
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_55
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_56
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_57
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_58
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_59
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_60
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_61
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_62
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_63
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_64
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_65
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_66
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_67
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_68
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_69
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_70
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_71
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_72
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_73
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_74
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_75
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_76
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_77
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_78
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_79
	case 80: {
	//IfExitKind
	if (__is_neg("0-80", 5, &(return_value), sizeof (return_value), &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH1"))
	{
	case 0: {
	array_init(return_value);
	
	break;
	}
	#ifdef COMPILE_80
	case 1: {
	//AddAndReplaceKind
	Debug((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_81
	case 2: {
	//AddAndReplaceKind
	__cmsg_nxthdr((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_82
	case 3: {
	//AddAndReplaceKind
	__overflow((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_83
	case 4: {
	//AddAndReplaceKind
	__sigsetjmp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_84
	case 5: {
	//AddAndReplaceKind
	__stpcpy((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_85
	case 6: {
	//AddAndReplaceKind
	__zend_realloc((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_86
	case 7: {
	//AddAndReplaceKind
	_estrndup((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_87
	case 8: {
	//AddAndReplaceKind
	_longjmp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_88
	case 9: {
	//AddAndReplaceKind
	_object_init_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_89
	case 10: {
	//AddAndReplaceKind
	_php_glob_stream_get_count((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_90
	case 11: {
	//AddAndReplaceKind
	_php_stream_filter_append((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_91
	case 12: {
	//AddAndReplaceKind
	_php_stream_filter_flush((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_92
	case 13: {
	//AddAndReplaceKind
	_php_stream_filter_prepend((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_93
	case 14: {
	//AddAndReplaceKind
	_php_stream_flush((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_94
	case 15: {
	//AddAndReplaceKind
	_php_stream_fopen_from_file((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_95
	case 16: {
	//AddAndReplaceKind
	_php_stream_fopen_from_pipe((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_96
	case 17: {
	//AddAndReplaceKind
	_php_stream_free((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_97
	case 18: {
	//AddAndReplaceKind
	_php_stream_free_enclosed((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_98
	case 19: {
	//AddAndReplaceKind
	_php_stream_memory_get_buffer((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_99
	case 20: {
	//AddAndReplaceKind
	_php_stream_mmap_unmap_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_100
	case 21: {
	//AddAndReplaceKind
	_php_stream_printf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_101
	case 22: {
	//AddAndReplaceKind
	_php_stream_putc((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_102
	case 23: {
	//AddAndReplaceKind
	_php_stream_puts((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_103
	case 24: {
	//AddAndReplaceKind
	_php_stream_readdir((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_104
	case 25: {
	//AddAndReplaceKind
	_php_stream_stat((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_105
	case 26: {
	//AddAndReplaceKind
	_php_stream_truncate_set_size((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_106
	case 27: {
	//AddAndReplaceKind
	_zend_bailout((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_107
	case 28: {
	//AddAndReplaceKind
	_zend_mm_alloc((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_108
	case 29: {
	//AddAndReplaceKind
	_zend_mm_block_size((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_109
	case 30: {
	//AddAndReplaceKind
	_zend_mm_free((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_110
	case 31: {
	//AddAndReplaceKind
	access((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_111
	case 32: {
	//AddAndReplaceKind
	add_index_null((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_112
	case 33: {
	//AddAndReplaceKind
	add_next_index_bool((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_113
	case 34: {
	//AddAndReplaceKind
	add_next_index_long((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_114
	case 35: {
	//AddAndReplaceKind
	add_next_index_resource((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_115
	case 36: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_116
	case 37: {
	//AddAndReplaceKind
	add_trait_alias((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_117
	case 38: {
	//AddAndReplaceKind
	adjtime((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_118
	case 39: {
	//AddAndReplaceKind
	alphasort((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_119
	case 40: {
	//AddAndReplaceKind
	ap_php_asprintf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_120
	case 41: {
	//AddAndReplaceKind
	asctime_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_121
	case 42: {
	//AddAndReplaceKind
	bitwise_not_function((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_122
	case 43: {
	//AddAndReplaceKind
	boolean_not_function((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_123
	case 44: {
	//AddAndReplaceKind
	bzero((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_124
	case 45: {
	//AddAndReplaceKind
	cfg_get_double((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_125
	case 46: {
	//AddAndReplaceKind
	cfg_get_entry((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_126
	case 47: {
	//AddAndReplaceKind
	cfg_get_long((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_127
	case 48: {
	//AddAndReplaceKind
	cfg_get_string((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_128
	case 49: {
	//AddAndReplaceKind
	chmod((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_129
	case 50: {
	//AddAndReplaceKind
	compile_file((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_130
	case 51: {
	//AddAndReplaceKind
	compile_string((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_131
	case 52: {
	//AddAndReplaceKind
	convert_to_long_base((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_132
	case 53: {
	//AddAndReplaceKind
	ctime_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_133
	case 54: {
	//AddAndReplaceKind
	display_link_numbers((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_134
	case 55: {
	//AddAndReplaceKind
	dlopen((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_135
	case 56: {
	//AddAndReplaceKind
	dlsym((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_136
	case 57: {
	//AddAndReplaceKind
	drand48_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_137
	case 58: {
	//AddAndReplaceKind
	execl((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_138
	case 59: {
	//AddAndReplaceKind
	execle((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_139
	case 60: {
	//AddAndReplaceKind
	execlp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_140
	case 61: {
	//AddAndReplaceKind
	execute_internal((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_141
	case 62: {
	//AddAndReplaceKind
	execv((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_142
	case 63: {
	//AddAndReplaceKind
	execvp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_143
	case 64: {
	//AddAndReplaceKind
	expand_filepath((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_144
	case 65: {
	//AddAndReplaceKind
	explicit_bzero((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_145
	case 66: {
	//AddAndReplaceKind
	fgetpos((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_146
	case 67: {
	//AddAndReplaceKind
	fopen((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_147
	case 68: {
	//AddAndReplaceKind
	fprintf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_148
	case 69: {
	//AddAndReplaceKind
	fputs((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_149
	case 70: {
	//AddAndReplaceKind
	fscanf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_150
	case 71: {
	//AddAndReplaceKind
	fsetpos((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_151
	case 72: {
	//AddAndReplaceKind
	getcwd((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_152
	case 73: {
	//AddAndReplaceKind
	getdomainname((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_153
	case 74: {
	//AddAndReplaceKind
	getentropy((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_154
	case 75: {
	//AddAndReplaceKind
	gethostname((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_155
	case 76: {
	//AddAndReplaceKind
	getloadavg((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_156
	case 77: {
	//AddAndReplaceKind
	getlogin_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_157
	case 78: {
	//AddAndReplaceKind
	gettimeofday((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_158
	case 79: {
	//AddAndReplaceKind
	gmtime_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_159
	case 80: {
	//AddAndReplaceKind
	index((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_160
	case 81: {
	//AddAndReplaceKind
	init_trait_alias_list((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_161
	case 82: {
	//AddAndReplaceKind
	instanceof_function((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_162
	case 83: {
	//AddAndReplaceKind
	lchmod((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_163
	case 84: {
	//AddAndReplaceKind
	lcong48_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_164
	case 85: {
	//AddAndReplaceKind
	link((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_165
	case 86: {
	//AddAndReplaceKind
	localtime_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_166
	case 87: {
	//AddAndReplaceKind
	longjmp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_167
	case 88: {
	//AddAndReplaceKind
	lrand48_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_168
	case 89: {
	//AddAndReplaceKind
	lstat((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_169
	case 90: {
	//AddAndReplaceKind
	lutimes((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_170
	case 91: {
	//AddAndReplaceKind
	mblen((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_171
	case 92: {
	//AddAndReplaceKind
	mkdir((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_172
	case 93: {
	//AddAndReplaceKind
	mkfifo((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_173
	case 94: {
	//AddAndReplaceKind
	mkstemps((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_174
	case 95: {
	//AddAndReplaceKind
	mrand48_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_175
	case 96: {
	//AddAndReplaceKind
	nanosleep((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_176
	case 97: {
	//AddAndReplaceKind
	object_properties_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_177
	case 98: {
	//AddAndReplaceKind
	on_exit((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_178
	case 99: {
	//AddAndReplaceKind
	open_memstream((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_179
	case 100: {
	//AddAndReplaceKind
	pathconf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_180
	case 101: {
	//AddAndReplaceKind
	php_check_open_basedir_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_181
	case 102: {
	//AddAndReplaceKind
	php_check_specific_open_basedir((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_182
	case 103: {
	//AddAndReplaceKind
	php_info_html_esc_write((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_183
	case 104: {
	//AddAndReplaceKind
	php_ini_activate_per_dir_config((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_184
	case 105: {
	//AddAndReplaceKind
	php_ini_activate_per_host_config((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_185
	case 106: {
	//AddAndReplaceKind
	php_output_handler_alias((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_186
	case 107: {
	//AddAndReplaceKind
	php_output_handler_started((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_187
	case 108: {
	//AddAndReplaceKind
	php_output_write((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_188
	case 109: {
	//AddAndReplaceKind
	php_output_write_unbuffered((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_189
	case 110: {
	//AddAndReplaceKind
	php_printf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_190
	case 111: {
	//AddAndReplaceKind
	php_register_pre_request_shutdown((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_191
	case 112: {
	//AddAndReplaceKind
	php_register_url_stream_wrapper((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_192
	case 113: {
	//AddAndReplaceKind
	php_register_url_stream_wrapper_volatile((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_193
	case 114: {
	//AddAndReplaceKind
	php_sprintf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_194
	case 115: {
	//AddAndReplaceKind
	php_stream_bucket_append((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_195
	case 116: {
	//AddAndReplaceKind
	php_stream_bucket_prepend((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_196
	case 117: {
	//AddAndReplaceKind
	php_stream_context_del_link((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_197
	case 118: {
	//AddAndReplaceKind
	php_stream_context_set((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_198
	case 119: {
	//AddAndReplaceKind
	php_stream_dirent_alphasort((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_199
	case 120: {
	//AddAndReplaceKind
	php_stream_dirent_alphasortr((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_200
	case 121: {
	//AddAndReplaceKind
	php_stream_encloses((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_201
	case 122: {
	//AddAndReplaceKind
	php_stream_filter_append_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_202
	case 123: {
	//AddAndReplaceKind
	php_stream_filter_prepend_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_203
	case 124: {
	//AddAndReplaceKind
	php_stream_filter_register_factory((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_204
	case 125: {
	//AddAndReplaceKind
	php_stream_filter_register_factory_volatile((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_205
	case 126: {
	//AddAndReplaceKind
	php_stream_filter_remove((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_206
	case 127: {
	//AddAndReplaceKind
	php_stream_from_persistent_id((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_207
	case 128: {
	//AddAndReplaceKind
	php_stream_xport_crypto_enable((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_208
	case 129: {
	//AddAndReplaceKind
	php_stream_xport_register((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_209
	case 130: {
	//AddAndReplaceKind
	php_stream_xport_shutdown((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_210
	case 131: {
	//AddAndReplaceKind
	php_write((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_211
	case 132: {
	//AddAndReplaceKind
	popen((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_212
	case 133: {
	//AddAndReplaceKind
	print_op_array((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_213
	case 134: {
	//AddAndReplaceKind
	printf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_214
	case 135: {
	//AddAndReplaceKind
	psiginfo((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_215
	case 136: {
	//AddAndReplaceKind
	putpwent((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_216
	case 137: {
	//AddAndReplaceKind
	random_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_217
	case 138: {
	//AddAndReplaceKind
	realloc((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_218
	case 139: {
	//AddAndReplaceKind
	realpath((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_219
	case 140: {
	//AddAndReplaceKind
	realpath_cache_del((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_220
	case 141: {
	//AddAndReplaceKind
	rename((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_221
	case 142: {
	//AddAndReplaceKind
	rindex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_222
	case 143: {
	//AddAndReplaceKind
	scanf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_223
	case 144: {
	//AddAndReplaceKind
	seed48_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_224
	case 145: {
	//AddAndReplaceKind
	seekdir((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_225
	case 146: {
	//AddAndReplaceKind
	setbuf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_226
	case 147: {
	//AddAndReplaceKind
	setdomainname((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_227
	case 148: {
	//AddAndReplaceKind
	sethostname((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_228
	case 149: {
	//AddAndReplaceKind
	setstate_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_229
	case 150: {
	//AddAndReplaceKind
	settimeofday((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_230
	case 151: {
	//AddAndReplaceKind
	sigaddset((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_231
	case 152: {
	//AddAndReplaceKind
	sigaltstack((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_232
	case 153: {
	//AddAndReplaceKind
	sigdelset((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_233
	case 154: {
	//AddAndReplaceKind
	sigismember((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_234
	case 155: {
	//AddAndReplaceKind
	siglongjmp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_235
	case 156: {
	//AddAndReplaceKind
	sigstack((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_236
	case 157: {
	//AddAndReplaceKind
	sigwait((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_237
	case 158: {
	//AddAndReplaceKind
	sigwaitinfo((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_238
	case 159: {
	//AddAndReplaceKind
	sprintf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_239
	case 160: {
	//AddAndReplaceKind
	sscanf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_240
	case 161: {
	//AddAndReplaceKind
	stat((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_241
	case 162: {
	//AddAndReplaceKind
	stpcpy((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_242
	case 163: {
	//AddAndReplaceKind
	strcasecmp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_243
	case 164: {
	//AddAndReplaceKind
	strcat((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_244
	case 165: {
	//AddAndReplaceKind
	strchr((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_245
	case 166: {
	//AddAndReplaceKind
	strcmp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_246
	case 167: {
	//AddAndReplaceKind
	strcoll((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_247
	case 168: {
	//AddAndReplaceKind
	strcpy((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_248
	case 169: {
	//AddAndReplaceKind
	strcspn((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_249
	case 170: {
	//AddAndReplaceKind
	strndup((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_250
	case 171: {
	//AddAndReplaceKind
	strnlen((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_251
	case 172: {
	//AddAndReplaceKind
	strpbrk((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_252
	case 173: {
	//AddAndReplaceKind
	strrchr((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_253
	case 174: {
	//AddAndReplaceKind
	strsep((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_254
	case 175: {
	//AddAndReplaceKind
	strspn((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_255
	case 176: {
	//AddAndReplaceKind
	strstr((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_256
	case 177: {
	//AddAndReplaceKind
	strtod((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_257
	case 178: {
	//AddAndReplaceKind
	strtof((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_258
	case 179: {
	//AddAndReplaceKind
	strtok((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_259
	case 180: {
	//AddAndReplaceKind
	strtold((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_260
	case 181: {
	//AddAndReplaceKind
	symlink((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_261
	case 182: {
	//AddAndReplaceKind
	tempnam((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_262
	case 183: {
	//AddAndReplaceKind
	timer_gettime((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_263
	case 184: {
	//AddAndReplaceKind
	timespec_get((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_264
	case 185: {
	//AddAndReplaceKind
	truncate((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_265
	case 186: {
	//AddAndReplaceKind
	tsrm_realpath((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_266
	case 187: {
	//AddAndReplaceKind
	utime((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_267
	case 188: {
	//AddAndReplaceKind
	utimes((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_268
	case 189: {
	//AddAndReplaceKind
	virtual_access((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_269
	case 190: {
	//AddAndReplaceKind
	virtual_chdir_file((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_270
	case 191: {
	//AddAndReplaceKind
	virtual_chmod((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_271
	case 192: {
	//AddAndReplaceKind
	virtual_creat((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_272
	case 193: {
	//AddAndReplaceKind
	virtual_filepath((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_273
	case 194: {
	//AddAndReplaceKind
	virtual_fopen((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_274
	case 195: {
	//AddAndReplaceKind
	virtual_getcwd((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_275
	case 196: {
	//AddAndReplaceKind
	virtual_lstat((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_276
	case 197: {
	//AddAndReplaceKind
	virtual_mkdir((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_277
	case 198: {
	//AddAndReplaceKind
	virtual_open((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_278
	case 199: {
	//AddAndReplaceKind
	virtual_popen((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_279
	case 200: {
	//AddAndReplaceKind
	virtual_realpath((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_280
	case 201: {
	//AddAndReplaceKind
	virtual_rename((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_281
	case 202: {
	//AddAndReplaceKind
	virtual_stat((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_282
	case 203: {
	//AddAndReplaceKind
	virtual_utime((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_283
	case 204: {
	//AddAndReplaceKind
	vprintf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_284
	case 205: {
	//AddAndReplaceKind
	vscanf((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_285
	case 206: {
	//AddAndReplaceKind
	wctomb((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_286
	case 207: {
	//AddAndReplaceKind
	zend_add_literal((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_287
	case 208: {
	//AddAndReplaceKind
	zend_add_to_list((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_288
	case 209: {
	//AddAndReplaceKind
	zend_atoi((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_289
	case 210: {
	//AddAndReplaceKind
	zend_atol((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_290
	case 211: {
	//AddAndReplaceKind
	zend_binary_zval_strcasecmp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_291
	case 212: {
	//AddAndReplaceKind
	zend_binary_zval_strcmp((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_292
	case 213: {
	//AddAndReplaceKind
	zend_call_function((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_293
	case 214: {
	//AddAndReplaceKind
	zend_check_protected((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_294
	case 215: {
	//AddAndReplaceKind
	zend_class_implements((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_295
	case 216: {
	//AddAndReplaceKind
	zend_compare_file_handles((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_296
	case 217: {
	//AddAndReplaceKind
	zend_copy_constants((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_297
	case 218: {
	//AddAndReplaceKind
	zend_delete_global_variable((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_298
	case 219: {
	//AddAndReplaceKind
	zend_dirname((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_299
	case 220: {
	//AddAndReplaceKind
	zend_disable_class((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_300
	case 221: {
	//AddAndReplaceKind
	zend_disable_function((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_301
	case 222: {
	//AddAndReplaceKind
	zend_do_begin_class_member_function_call((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_302
	case 223: {
	//AddAndReplaceKind
	zend_do_begin_dynamic_function_call((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_303
	case 224: {
	//AddAndReplaceKind
	zend_do_begin_function_call((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_304
	case 225: {
	//AddAndReplaceKind
	zend_do_begin_namespace((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_305
	case 226: {
	//AddAndReplaceKind
	zend_do_begin_new_object((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_306
	case 227: {
	//AddAndReplaceKind
	zend_do_begin_qm_op((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_307
	case 228: {
	//AddAndReplaceKind
	zend_do_boolean_and_begin((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_308
	case 229: {
	//AddAndReplaceKind
	zend_do_boolean_or_begin((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_309
	case 230: {
	//AddAndReplaceKind
	zend_do_case_after_statement((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_310
	case 231: {
	//AddAndReplaceKind
	zend_do_clone((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_311
	case 232: {
	//AddAndReplaceKind
	zend_do_declare_class_constant((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_312
	case 233: {
	//AddAndReplaceKind
	zend_do_declare_constant((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_313
	case 234: {
	//AddAndReplaceKind
	zend_do_declare_stmt((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_314
	case 235: {
	//AddAndReplaceKind
	zend_do_default_before_statement((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_315
	case 236: {
	//AddAndReplaceKind
	zend_do_end_class_declaration((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_316
	case 237: {
	//AddAndReplaceKind
	zend_do_exit((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_317
	case 238: {
	//AddAndReplaceKind
	zend_do_fetch_class((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_318
	case 239: {
	//AddAndReplaceKind
	zend_do_fetch_lexical_variable((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_319
	case 240: {
	//AddAndReplaceKind
	zend_do_fetch_static_member((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_320
	case 241: {
	//AddAndReplaceKind
	zend_do_for_before_statement((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_321
	case 242: {
	//AddAndReplaceKind
	zend_do_for_cond((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_322
	case 243: {
	//AddAndReplaceKind
	zend_do_foreach_end((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_323
	case 244: {
	//AddAndReplaceKind
	zend_do_if_after_statement((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_324
	case 245: {
	//AddAndReplaceKind
	zend_do_if_cond((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_325
	case 246: {
	//AddAndReplaceKind
	zend_do_implement_interface((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_326
	case 247: {
	//AddAndReplaceKind
	zend_do_implement_trait((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_327
	case 248: {
	//AddAndReplaceKind
	zend_do_inherit_interfaces((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_328
	case 249: {
	//AddAndReplaceKind
	zend_do_inheritance((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_329
	case 250: {
	//AddAndReplaceKind
	zend_do_list_end((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_330
	case 251: {
	//AddAndReplaceKind
	zend_do_mark_last_catch((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_331
	case 252: {
	//AddAndReplaceKind
	zend_do_print((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_332
	case 253: {
	//AddAndReplaceKind
	zend_do_return((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_333
	case 254: {
	//AddAndReplaceKind
	zend_do_shell_exec((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_334
	case 255: {
	//AddAndReplaceKind
	zend_do_verify_access_types((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_335
	case 256: {
	//AddAndReplaceKind
	zend_do_while_cond((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_336
	case 257: {
	//AddAndReplaceKind
	zend_do_while_end((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_337
	case 258: {
	//AddAndReplaceKind
	zend_fcall_info_argn((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_338
	case 259: {
	//AddAndReplaceKind
	zend_fcall_info_args((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_339
	case 260: {
	//AddAndReplaceKind
	zend_fcall_info_args_clear((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_340
	case 261: {
	//AddAndReplaceKind
	zend_get_class_fetch_type((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_341
	case 262: {
	//AddAndReplaceKind
	zend_get_compiled_variable_value((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_342
	case 263: {
	//AddAndReplaceKind
	zend_get_hash_value((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_343
	case 264: {
	//AddAndReplaceKind
	zend_hash_apply((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_344
	case 265: {
	//AddAndReplaceKind
	zend_hash_func((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_345
	case 266: {
	//AddAndReplaceKind
	zend_hash_get_current_key_type_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_346
	case 267: {
	//AddAndReplaceKind
	zend_hash_get_pointer((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_347
	case 268: {
	//AddAndReplaceKind
	zend_hash_index_exists((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_348
	case 269: {
	//AddAndReplaceKind
	zend_hash_internal_pointer_end_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_349
	case 270: {
	//AddAndReplaceKind
	zend_hash_internal_pointer_reset_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_350
	case 271: {
	//AddAndReplaceKind
	zend_hash_move_backwards_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_351
	case 272: {
	//AddAndReplaceKind
	zend_hash_move_forward_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_352
	case 273: {
	//AddAndReplaceKind
	zend_hash_reverse_apply((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_353
	case 274: {
	//AddAndReplaceKind
	zend_hash_set_pointer((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_354
	case 275: {
	//AddAndReplaceKind
	zend_hex_strtod((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_355
	case 276: {
	//AddAndReplaceKind
	zend_ini_boolean_displayer_cb((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_356
	case 277: {
	//AddAndReplaceKind
	zend_ini_color_displayer_cb((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_357
	case 278: {
	//AddAndReplaceKind
	zend_init_list((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_358
	case 279: {
	//AddAndReplaceKind
	zend_initialize_class_data((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_359
	case 280: {
	//AddAndReplaceKind
	zend_inline_hash_func((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_360
	case 281: {
	//AddAndReplaceKind
	zend_is_auto_global((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_361
	case 282: {
	//AddAndReplaceKind
	zend_iterator_unwrap((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_362
	case 283: {
	//AddAndReplaceKind
	zend_list_insert((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_363
	case 284: {
	//AddAndReplaceKind
	zend_llist_add_element((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_364
	case 285: {
	//AddAndReplaceKind
	zend_llist_apply((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_365
	case 286: {
	//AddAndReplaceKind
	zend_llist_apply_with_del((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_366
	case 287: {
	//AddAndReplaceKind
	zend_llist_copy((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_367
	case 288: {
	//AddAndReplaceKind
	zend_llist_get_first_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_368
	case 289: {
	//AddAndReplaceKind
	zend_llist_get_last_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_369
	case 290: {
	//AddAndReplaceKind
	zend_llist_get_next_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_370
	case 291: {
	//AddAndReplaceKind
	zend_llist_get_prev_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_371
	case 292: {
	//AddAndReplaceKind
	zend_llist_prepend_element((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_372
	case 293: {
	//AddAndReplaceKind
	zend_llist_sort((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_373
	case 294: {
	//AddAndReplaceKind
	zend_make_callable((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_374
	case 295: {
	//AddAndReplaceKind
	zend_multibyte_set_script_encoding((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_375
	case 296: {
	//AddAndReplaceKind
	zend_multibyte_set_script_encoding_by_string((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_376
	case 297: {
	//AddAndReplaceKind
	zend_multibyte_yyinput_again((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_377
	case 298: {
	//AddAndReplaceKind
	zend_object_create_proxy((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_378
	case 299: {
	//AddAndReplaceKind
	zend_object_std_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_379
	case 300: {
	//AddAndReplaceKind
	zend_object_store_set_object((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_380
	case 301: {
	//AddAndReplaceKind
	zend_objects_destroy_object((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_381
	case 302: {
	//AddAndReplaceKind
	zend_objects_new((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_382
	case 303: {
	//AddAndReplaceKind
	zend_objects_store_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_383
	case 304: {
	//AddAndReplaceKind
	zend_oct_strtod((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_384
	case 305: {
	//AddAndReplaceKind
	zend_prepare_string_for_scanning((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_385
	case 306: {
	//AddAndReplaceKind
	zend_print_zval((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_386
	case 307: {
	//AddAndReplaceKind
	zend_print_zval_r((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_387
	case 308: {
	//AddAndReplaceKind
	zend_ptr_stack_apply((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_388
	case 309: {
	//AddAndReplaceKind
	zend_ptr_stack_init_ex((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_389
	case 310: {
	//AddAndReplaceKind
	zend_ptr_stack_n_pop((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_390
	case 311: {
	//AddAndReplaceKind
	zend_ptr_stack_n_push((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_391
	case 312: {
	//AddAndReplaceKind
	zend_ptr_stack_push((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_392
	case 313: {
	//AddAndReplaceKind
	zend_quick_get_constant((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_393
	case 314: {
	//AddAndReplaceKind
	zend_register_ini_entries((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_394
	case 315: {
	//AddAndReplaceKind
	zend_resolve_non_class_name((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_395
	case 316: {
	//AddAndReplaceKind
	zend_stack_top((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_396
	case 317: {
	//AddAndReplaceKind
	zend_startup((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_397
	case 318: {
	//AddAndReplaceKind
	zend_std_get_debug_info((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_398
	case 319: {
	//AddAndReplaceKind
	zend_str_tolower((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_399
	case 320: {
	//AddAndReplaceKind
	zend_str_tolower_dup((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_400
	case 321: {
	//AddAndReplaceKind
	zend_stream_open((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_401
	case 322: {
	//AddAndReplaceKind
	zend_string_to_double((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_402
	case 323: {
	//AddAndReplaceKind
	zend_strndup((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_403
	case 324: {
	//AddAndReplaceKind
	zend_strtod((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_404
	case 325: {
	//AddAndReplaceKind
	zend_ts_hash_apply((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_405
	case 326: {
	//AddAndReplaceKind
	zend_ts_hash_func((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_406
	case 327: {
	//AddAndReplaceKind
	zend_ts_hash_index_exists((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_407
	case 328: {
	//AddAndReplaceKind
	zend_ts_hash_reverse_apply((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_408
	case 329: {
	//AddAndReplaceKind
	zval_set_isref_to_p((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_409
	case 330: {
	//AddAndReplaceKind
	zval_set_refcount_p((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_410
	case 331: {
	//AddAndReplaceKind
	zval_update_constant((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_411
	case 332: {
	//AddAndReplaceKind
	zval_update_constant_inline_change((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_412
	case 333: {
	//AddAndReplaceKind
	zval_update_constant_no_inline_change((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_413
	case 334: {
	//GuardKind
	if (!__is_neg("1-334", 5, &(return_value), sizeof (return_value), &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    _array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_414
	case 335: {
	//ReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_415
	case 336: {
	//ReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	}
	}

	ZVAL_NULL(&token);
	//prophet generated patch
	{
	switch(__choose("__SWITCH2"))
	{
	case 0: 
	break;
	#ifdef COMPILE_416
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_417
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_418
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_419
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_420
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_421
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_422
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_423
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_424
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_425
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_426
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_427
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_428
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_429
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_430
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_431
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_432
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_433
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_434
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_435
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_436
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_437
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_438
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_439
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_440
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_441
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_442
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_443
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_444
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_445
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_446
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_447
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_448
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_449
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_450
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_451
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_452
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_453
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_454
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_455
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_456
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_457
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_458
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_459
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_460
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_461
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_462
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_463
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_464
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_465
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_466
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_467
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_468
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_469
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_470
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_471
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_472
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_473
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_474
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_475
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_476
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_477
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_478
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_479
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_480
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_481
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_482
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_483
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_484
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_485
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_486
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_487
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_488
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_489
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_490
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_491
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_492
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_493
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_494
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_495
	case 80: {
	//AddInitKind
	memset(&((zval_gc_info *)(keyword))->u, 0, sizeof (*(&((zval_gc_info *)(keyword))->u)));
	
	break;
	}
	#endif
	#ifdef COMPILE_496
	case 81: {
	//AddInitKind
	memset(&(*&token), 0, sizeof (*(&(*&token))));
	
	break;
	}
	#endif
	#ifdef COMPILE_497
	case 82: {
	//AddInitKind
	memset(&(token), 0, sizeof (*(&(token))));
	
	break;
	}
	#endif
	#ifdef COMPILE_498
	case 83: {
	//AddInitKind
	memset(&(token).value, 0, sizeof (*(&(token).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_499
	case 84: {
	//AddInitKind
	memset(&(token).value.str, 0, sizeof (*(&(token).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_500
	case 85: {
	//AddInitKind
	memset(&compiler_globals, 0, sizeof (*(&compiler_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_501
	case 86: {
	//AddInitKind
	memset(&language_scanner_globals, 0, sizeof (*(&language_scanner_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_502
	case 87: {
	//AddInitKind
	memset(((zval_gc_info *)(keyword)), 0, sizeof (*(((zval_gc_info *)(keyword)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_503
	case 88: {
	//AddInitKind
	memset((keyword), 0, sizeof (*((keyword))));
	
	break;
	}
	#endif
	#ifdef COMPILE_504
	case 89: {
	//IfExitKind
	if (__is_neg("2-89", 8, &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &((*&token).type), sizeof ((*&token).type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &((*&token).refcount__gc), sizeof ((*&token).refcount__gc), &((*&token).is_ref__gc), sizeof ((*&token).is_ref__gc)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH36"))
	{
	case 0: {
	while ((token_type = lex_scan(&token TSRMLS_CC))) {
			{
	switch(__choose("__SWITCH3"))
	{
	case 0: 
	break;
	#ifdef COMPILE_505
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_506
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_507
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_508
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_509
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_510
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_511
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_512
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_513
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_514
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_515
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_516
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_517
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_518
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_519
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_520
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_521
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_522
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_523
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_524
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_525
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_526
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_527
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_528
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_529
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_530
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_531
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_532
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_533
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_534
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_535
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_536
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_537
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_538
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_539
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_540
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_541
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_542
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_543
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_544
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_545
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_546
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_547
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_548
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_549
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_550
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_551
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_552
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_553
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_554
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_555
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_556
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_557
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_558
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_559
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_560
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_561
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_562
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_563
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_564
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_565
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_566
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_567
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_568
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_569
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_570
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_571
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_572
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_573
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_574
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_575
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_576
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_577
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_578
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_579
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_580
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_581
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_582
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_583
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_584
	case 80: {
	//IfExitKind
	if (__is_neg("3-80", 5, &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_585
	case 81: {
	//IfExitKind
	if (__is_neg("3-81", 5, &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH4"))
	{
	case 0: {
	destroy = 1;
	
	break;
	}
	#ifdef COMPILE_586
	case 1: {
	//GuardKind
	if (!__is_neg("4-1", 5, &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    destroy = 1;
	
	break;
	}
	#endif
	}
	}
			{
	switch(__choose("__SWITCH5"))
	{
	case 0: 
	break;
	#ifdef COMPILE_587
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_588
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_589
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_590
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_591
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_592
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_593
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_594
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_595
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_596
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_597
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_598
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_599
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_600
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_601
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_602
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_603
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_604
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_605
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_606
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_607
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_608
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_609
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_610
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_611
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_612
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_613
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_614
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_615
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_616
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_617
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_618
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_619
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_620
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_621
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_622
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_623
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_624
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_625
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_626
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_627
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_628
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_629
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_630
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_631
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_632
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_633
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_634
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_635
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_636
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_637
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_638
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_639
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_640
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_641
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_642
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_643
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_644
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_645
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_646
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_647
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_648
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_649
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_650
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_651
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_652
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_653
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_654
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_655
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_656
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_657
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_658
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_659
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_660
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_661
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_662
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_663
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_664
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_665
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_666
	case 80: {
	//AddInitKind
	memset(&compiler_globals, 0, sizeof (*(&compiler_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_667
	case 81: {
	//AddInitKind
	memset(&language_scanner_globals, 0, sizeof (*(&language_scanner_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_668
	case 82: {
	//IfExitKind
	if (__is_neg("5-82", 5, &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_669
	case 83: {
	//IfExitKind
	if (__is_neg("5-83", 5, &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH8"))
	{
	case 0: {
	switch (token_type) {
				case T_CLOSE_TAG:
					if (zendtext[zendleng - 1] != '>') {
						CG(zend_lineno)++;
					}
				{
	switch(__choose("__SWITCH6"))
	{
	case 0: 
	break;
	#ifdef COMPILE_670
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_671
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_672
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_673
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_674
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_675
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_676
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_677
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_678
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_679
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_680
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_681
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_682
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_683
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_684
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_685
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_686
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_687
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_688
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_689
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_690
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_691
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_692
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_693
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_694
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_695
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_696
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_697
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_698
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_699
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_700
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_701
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_702
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_703
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_704
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_705
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_706
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_707
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_708
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_709
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_710
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_711
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_712
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_713
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_714
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_715
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_716
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_717
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_718
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_719
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_720
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_721
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_722
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_723
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_724
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_725
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_726
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_727
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_728
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_729
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_730
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_731
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_732
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_733
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_734
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_735
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_736
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_737
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_738
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_739
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_740
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_741
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_742
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_743
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_744
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_745
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_746
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_747
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_748
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_749
	case 80: {
	//IfExitKind
	if (__is_neg("6-80", 5, &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_750
	case 81: {
	//IfExitKind
	if (__is_neg("6-81", 5, &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line)))
	    return;
	
	break;
	}
	#endif
	}
	case T_OPEN_TAG:
				case T_OPEN_TAG_WITH_ECHO:
				case T_WHITESPACE:
				case T_COMMENT:
				case T_DOC_COMMENT:
					destroy = 0;
	}
					{
	switch(__choose("__SWITCH7"))
	{
	case 0: 
	break;
	#ifdef COMPILE_751
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_752
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_753
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_754
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_755
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_756
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_757
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_758
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_759
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_760
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_761
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_762
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_763
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_764
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_765
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_766
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_767
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_768
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_769
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_770
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_771
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_772
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_773
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_774
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_775
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_776
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_777
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_778
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_779
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_780
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_781
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_782
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_783
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_784
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_785
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_786
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_787
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_788
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_789
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_790
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_791
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_792
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_793
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_794
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_795
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_796
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_797
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_798
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_799
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_800
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_801
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_802
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_803
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_804
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_805
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_806
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_807
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_808
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_809
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_810
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_811
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_812
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_813
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_814
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_815
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_816
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_817
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_818
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_819
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_820
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_821
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_822
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_823
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_824
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_825
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_826
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_827
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_828
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_829
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_830
	case 80: {
	//IfExitKind
	if (__is_neg("7-80", 5, &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_831
	case 81: {
	//IfExitKind
	if (__is_neg("7-81", 5, &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value)))
	    return;
	
	break;
	}
	#endif
	}
	break;
	}
			}
	
	break;
	}
	#ifdef COMPILE_832
	case 1: {
	//GuardKind
	if (!__is_neg("8-1", 5, &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value)))
	    break;
	
	break;
	}
	#endif
	}
	}
	
			{
	switch(__choose("__SWITCH9"))
	{
	case 0: 
	break;
	#ifdef COMPILE_833
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_834
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_835
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_836
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_837
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_838
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_839
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_840
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_841
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_842
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_843
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_844
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_845
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_846
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_847
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_848
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_849
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_850
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_851
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_852
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_853
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_854
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_855
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_856
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_857
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_858
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_859
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_860
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_861
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_862
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_863
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_864
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_865
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_866
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_867
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_868
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_869
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_870
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_871
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_872
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_873
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_874
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_875
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_876
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_877
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_878
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_879
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_880
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_881
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_882
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_883
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_884
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_885
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_886
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_887
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_888
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_889
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_890
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_891
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_892
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_893
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_894
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_895
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_896
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_897
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_898
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_899
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_900
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_901
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_902
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_903
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_904
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_905
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_906
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_907
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_908
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_909
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_910
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_911
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_912
	case 80: {
	//AddInitKind
	memset(&((zval_gc_info *)(keyword))->u, 0, sizeof (*(&((zval_gc_info *)(keyword))->u)));
	
	break;
	}
	#endif
	#ifdef COMPILE_913
	case 81: {
	//AddInitKind
	memset(&(token), 0, sizeof (*(&(token))));
	
	break;
	}
	#endif
	#ifdef COMPILE_914
	case 82: {
	//AddInitKind
	memset(&(token).value, 0, sizeof (*(&(token).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_915
	case 83: {
	//AddInitKind
	memset(&(token).value.str, 0, sizeof (*(&(token).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_916
	case 84: {
	//AddInitKind
	memset(&compiler_globals, 0, sizeof (*(&compiler_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_917
	case 85: {
	//AddInitKind
	memset(&language_scanner_globals, 0, sizeof (*(&language_scanner_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_918
	case 86: {
	//AddInitKind
	memset(((zval_gc_info *)(keyword)), 0, sizeof (*(((zval_gc_info *)(keyword)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_919
	case 87: {
	//AddInitKind
	memset((keyword), 0, sizeof (*((keyword))));
	
	break;
	}
	#endif
	#ifdef COMPILE_920
	case 88: {
	//IfExitKind
	if (__is_neg("9-88", 8, &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_921
	case 89: {
	//IfExitKind
	if (__is_neg("9-89", 8, &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    return;
	
	break;
	}
	#endif
	}
	{
	long long __temp10=(token_type >= 256) ;
	switch(__choose("__SWITCH10"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_922
	case 1: {
	__temp10= ((token_type >= 256) || __is_neg("10-1", 8, &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)));
	
	break;
	}
	#endif
	#ifdef COMPILE_923
	case 2: {
	__temp10= ((token_type >= 256) && !__is_neg("10-2", 8, &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH28"))
	{
	case 0: {
	if (__temp10) {
				{
	switch(__choose("__SWITCH11"))
	{
	case 0: 
	break;
	#ifdef COMPILE_924
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_925
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_926
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_927
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_928
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_929
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_930
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_931
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_932
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_933
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_934
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_935
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_936
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_937
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_938
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_939
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_940
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_941
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_942
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_943
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_944
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_945
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_946
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_947
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_948
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_949
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_950
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_951
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_952
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_953
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_954
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_955
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_956
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_957
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_958
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_959
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_960
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_961
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_962
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_963
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_964
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_965
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_966
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_967
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_968
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_969
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_970
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_971
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_972
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_973
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_974
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_975
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_976
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_977
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_978
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_979
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_980
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_981
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_982
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_983
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_984
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_985
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_986
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_987
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_988
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_989
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_990
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_991
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_992
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_993
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_994
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_995
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_996
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_997
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_998
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_999
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1000
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_1001
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1002
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1003
	case 80: {
	//AddInitKind
	memset(&((zval_gc_info *)(keyword))->u, 0, sizeof (*(&((zval_gc_info *)(keyword))->u)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1004
	case 81: {
	//AddInitKind
	memset(((zval_gc_info *)(keyword)), 0, sizeof (*(((zval_gc_info *)(keyword)))));
	
	break;
	}
	#endif
	#ifdef COMPILE_1005
	case 82: {
	//AddInitKind
	memset((keyword), 0, sizeof (*((keyword))));
	
	break;
	}
	#endif
	#ifdef COMPILE_1006
	case 83: {
	//IfExitKind
	if (__is_neg("11-83", 8, &(keyword), sizeof (keyword), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_1007
	case 84: {
	//IfExitKind
	if (__is_neg("11-84", 8, &(keyword), sizeof (keyword), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH12"))
	{
	case 0: {
	MAKE_STD_ZVAL(keyword);
	
	break;
	}
	#ifdef COMPILE_1008
	case 1: {
	//GuardKind
	if (!__is_neg("12-1", 8, &(keyword), sizeof (keyword), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_type), sizeof (token_type), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    do {
	        (keyword) = (zval *)_emalloc((sizeof(zval_gc_info)));
	        ((zval_gc_info *)(keyword))->u.buffered = ((void *)0);
	    } while (0);
	
	break;
	}
	#endif
	}
	}
				{
	switch(__choose("__SWITCH13"))
	{
	case 0: 
	break;
	#ifdef COMPILE_1009
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1010
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1011
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1012
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1013
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1014
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_1015
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_1016
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1017
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1018
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_1019
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_1020
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_1021
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1022
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1023
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_1024
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_1025
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_1026
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1027
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_1028
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_1029
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1030
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1031
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1032
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1033
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1034
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1035
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_1036
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1037
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1038
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_1039
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1040
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1041
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1042
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1043
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1044
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1045
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1046
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1047
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1048
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1049
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1050
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1051
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1052
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1053
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1054
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1055
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1056
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1057
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1058
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1059
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1060
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1061
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1062
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1063
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1064
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1065
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1066
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1067
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1068
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1069
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1070
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1071
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1072
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1073
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1074
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1075
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1076
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1077
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1078
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_1079
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_1080
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_1081
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1082
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1083
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1084
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1085
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_1086
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1087
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1088
	case 80: {
	//AddInitKind
	memset((keyword), 0, sizeof (*((keyword))));
	
	break;
	}
	#endif
	#ifdef COMPILE_1089
	case 81: {
	//IfExitKind
	if (__is_neg("13-81", 8, &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_1090
	case 82: {
	//IfExitKind
	if (__is_neg("13-82", 8, &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH14"))
	{
	case 0: {
	array_init(keyword);
	
	break;
	}
	#ifdef COMPILE_1091
	case 1: {
	//AddAndReplaceKind
	Debug((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1092
	case 2: {
	//AddAndReplaceKind
	__cmsg_nxthdr((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1093
	case 3: {
	//AddAndReplaceKind
	__overflow((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1094
	case 4: {
	//AddAndReplaceKind
	__sigsetjmp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1095
	case 5: {
	//AddAndReplaceKind
	__stpcpy((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1096
	case 6: {
	//AddAndReplaceKind
	__zend_realloc((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1097
	case 7: {
	//AddAndReplaceKind
	_estrndup((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1098
	case 8: {
	//AddAndReplaceKind
	_longjmp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1099
	case 9: {
	//AddAndReplaceKind
	_object_init_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1100
	case 10: {
	//AddAndReplaceKind
	_php_glob_stream_get_count((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1101
	case 11: {
	//AddAndReplaceKind
	_php_stream_filter_append((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1102
	case 12: {
	//AddAndReplaceKind
	_php_stream_filter_flush((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1103
	case 13: {
	//AddAndReplaceKind
	_php_stream_filter_prepend((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1104
	case 14: {
	//AddAndReplaceKind
	_php_stream_flush((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1105
	case 15: {
	//AddAndReplaceKind
	_php_stream_fopen_from_file((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1106
	case 16: {
	//AddAndReplaceKind
	_php_stream_fopen_from_pipe((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1107
	case 17: {
	//AddAndReplaceKind
	_php_stream_free((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1108
	case 18: {
	//AddAndReplaceKind
	_php_stream_free_enclosed((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1109
	case 19: {
	//AddAndReplaceKind
	_php_stream_memory_get_buffer((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1110
	case 20: {
	//AddAndReplaceKind
	_php_stream_mmap_unmap_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1111
	case 21: {
	//AddAndReplaceKind
	_php_stream_printf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1112
	case 22: {
	//AddAndReplaceKind
	_php_stream_putc((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1113
	case 23: {
	//AddAndReplaceKind
	_php_stream_puts((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1114
	case 24: {
	//AddAndReplaceKind
	_php_stream_readdir((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1115
	case 25: {
	//AddAndReplaceKind
	_php_stream_stat((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1116
	case 26: {
	//AddAndReplaceKind
	_php_stream_truncate_set_size((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1117
	case 27: {
	//AddAndReplaceKind
	_zend_bailout((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1118
	case 28: {
	//AddAndReplaceKind
	_zend_mm_alloc((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1119
	case 29: {
	//AddAndReplaceKind
	_zend_mm_block_size((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1120
	case 30: {
	//AddAndReplaceKind
	_zend_mm_free((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1121
	case 31: {
	//AddAndReplaceKind
	access((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1122
	case 32: {
	//AddAndReplaceKind
	add_index_null((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1123
	case 33: {
	//AddAndReplaceKind
	add_next_index_bool((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1124
	case 34: {
	//AddAndReplaceKind
	add_next_index_long((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1125
	case 35: {
	//AddAndReplaceKind
	add_next_index_resource((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1126
	case 36: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1127
	case 37: {
	//AddAndReplaceKind
	add_trait_alias((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1128
	case 38: {
	//AddAndReplaceKind
	adjtime((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1129
	case 39: {
	//AddAndReplaceKind
	alphasort((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1130
	case 40: {
	//AddAndReplaceKind
	ap_php_asprintf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1131
	case 41: {
	//AddAndReplaceKind
	asctime_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1132
	case 42: {
	//AddAndReplaceKind
	bitwise_not_function((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1133
	case 43: {
	//AddAndReplaceKind
	boolean_not_function((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1134
	case 44: {
	//AddAndReplaceKind
	bzero((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1135
	case 45: {
	//AddAndReplaceKind
	cfg_get_double((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1136
	case 46: {
	//AddAndReplaceKind
	cfg_get_entry((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1137
	case 47: {
	//AddAndReplaceKind
	cfg_get_long((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1138
	case 48: {
	//AddAndReplaceKind
	cfg_get_string((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1139
	case 49: {
	//AddAndReplaceKind
	chmod((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1140
	case 50: {
	//AddAndReplaceKind
	compile_file((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1141
	case 51: {
	//AddAndReplaceKind
	compile_string((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1142
	case 52: {
	//AddAndReplaceKind
	convert_to_long_base((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1143
	case 53: {
	//AddAndReplaceKind
	ctime_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1144
	case 54: {
	//AddAndReplaceKind
	display_link_numbers((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1145
	case 55: {
	//AddAndReplaceKind
	dlopen((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1146
	case 56: {
	//AddAndReplaceKind
	dlsym((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1147
	case 57: {
	//AddAndReplaceKind
	drand48_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1148
	case 58: {
	//AddAndReplaceKind
	execl((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1149
	case 59: {
	//AddAndReplaceKind
	execle((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1150
	case 60: {
	//AddAndReplaceKind
	execlp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1151
	case 61: {
	//AddAndReplaceKind
	execute_internal((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1152
	case 62: {
	//AddAndReplaceKind
	execv((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1153
	case 63: {
	//AddAndReplaceKind
	execvp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1154
	case 64: {
	//AddAndReplaceKind
	expand_filepath((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1155
	case 65: {
	//AddAndReplaceKind
	explicit_bzero((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1156
	case 66: {
	//AddAndReplaceKind
	fgetpos((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1157
	case 67: {
	//AddAndReplaceKind
	fopen((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1158
	case 68: {
	//AddAndReplaceKind
	fprintf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1159
	case 69: {
	//AddAndReplaceKind
	fputs((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1160
	case 70: {
	//AddAndReplaceKind
	fscanf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1161
	case 71: {
	//AddAndReplaceKind
	fsetpos((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1162
	case 72: {
	//AddAndReplaceKind
	getcwd((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1163
	case 73: {
	//AddAndReplaceKind
	getdomainname((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1164
	case 74: {
	//AddAndReplaceKind
	getentropy((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1165
	case 75: {
	//AddAndReplaceKind
	gethostname((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1166
	case 76: {
	//AddAndReplaceKind
	getloadavg((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1167
	case 77: {
	//AddAndReplaceKind
	getlogin_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1168
	case 78: {
	//AddAndReplaceKind
	gettimeofday((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1169
	case 79: {
	//AddAndReplaceKind
	gmtime_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1170
	case 80: {
	//AddAndReplaceKind
	index((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1171
	case 81: {
	//AddAndReplaceKind
	init_trait_alias_list((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1172
	case 82: {
	//AddAndReplaceKind
	instanceof_function((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1173
	case 83: {
	//AddAndReplaceKind
	lchmod((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1174
	case 84: {
	//AddAndReplaceKind
	lcong48_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1175
	case 85: {
	//AddAndReplaceKind
	link((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1176
	case 86: {
	//AddAndReplaceKind
	localtime_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1177
	case 87: {
	//AddAndReplaceKind
	longjmp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1178
	case 88: {
	//AddAndReplaceKind
	lrand48_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1179
	case 89: {
	//AddAndReplaceKind
	lstat((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1180
	case 90: {
	//AddAndReplaceKind
	lutimes((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1181
	case 91: {
	//AddAndReplaceKind
	mblen((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1182
	case 92: {
	//AddAndReplaceKind
	mkdir((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1183
	case 93: {
	//AddAndReplaceKind
	mkfifo((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1184
	case 94: {
	//AddAndReplaceKind
	mkstemps((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1185
	case 95: {
	//AddAndReplaceKind
	mrand48_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1186
	case 96: {
	//AddAndReplaceKind
	nanosleep((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1187
	case 97: {
	//AddAndReplaceKind
	object_properties_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1188
	case 98: {
	//AddAndReplaceKind
	on_exit((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1189
	case 99: {
	//AddAndReplaceKind
	open_memstream((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1190
	case 100: {
	//AddAndReplaceKind
	pathconf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1191
	case 101: {
	//AddAndReplaceKind
	php_check_open_basedir_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1192
	case 102: {
	//AddAndReplaceKind
	php_check_specific_open_basedir((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1193
	case 103: {
	//AddAndReplaceKind
	php_info_html_esc_write((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1194
	case 104: {
	//AddAndReplaceKind
	php_ini_activate_per_dir_config((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1195
	case 105: {
	//AddAndReplaceKind
	php_ini_activate_per_host_config((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1196
	case 106: {
	//AddAndReplaceKind
	php_output_handler_alias((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1197
	case 107: {
	//AddAndReplaceKind
	php_output_handler_started((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1198
	case 108: {
	//AddAndReplaceKind
	php_output_write((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1199
	case 109: {
	//AddAndReplaceKind
	php_output_write_unbuffered((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1200
	case 110: {
	//AddAndReplaceKind
	php_printf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1201
	case 111: {
	//AddAndReplaceKind
	php_register_pre_request_shutdown((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1202
	case 112: {
	//AddAndReplaceKind
	php_register_url_stream_wrapper((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1203
	case 113: {
	//AddAndReplaceKind
	php_register_url_stream_wrapper_volatile((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1204
	case 114: {
	//AddAndReplaceKind
	php_sprintf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1205
	case 115: {
	//AddAndReplaceKind
	php_stream_bucket_append((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1206
	case 116: {
	//AddAndReplaceKind
	php_stream_bucket_prepend((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1207
	case 117: {
	//AddAndReplaceKind
	php_stream_context_del_link((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1208
	case 118: {
	//AddAndReplaceKind
	php_stream_context_set((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1209
	case 119: {
	//AddAndReplaceKind
	php_stream_dirent_alphasort((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1210
	case 120: {
	//AddAndReplaceKind
	php_stream_dirent_alphasortr((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1211
	case 121: {
	//AddAndReplaceKind
	php_stream_encloses((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1212
	case 122: {
	//AddAndReplaceKind
	php_stream_filter_append_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1213
	case 123: {
	//AddAndReplaceKind
	php_stream_filter_prepend_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1214
	case 124: {
	//AddAndReplaceKind
	php_stream_filter_register_factory((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1215
	case 125: {
	//AddAndReplaceKind
	php_stream_filter_register_factory_volatile((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1216
	case 126: {
	//AddAndReplaceKind
	php_stream_filter_remove((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1217
	case 127: {
	//AddAndReplaceKind
	php_stream_from_persistent_id((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1218
	case 128: {
	//AddAndReplaceKind
	php_stream_xport_crypto_enable((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1219
	case 129: {
	//AddAndReplaceKind
	php_stream_xport_register((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1220
	case 130: {
	//AddAndReplaceKind
	php_stream_xport_shutdown((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1221
	case 131: {
	//AddAndReplaceKind
	php_write((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1222
	case 132: {
	//AddAndReplaceKind
	popen((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1223
	case 133: {
	//AddAndReplaceKind
	print_op_array((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1224
	case 134: {
	//AddAndReplaceKind
	printf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1225
	case 135: {
	//AddAndReplaceKind
	psiginfo((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1226
	case 136: {
	//AddAndReplaceKind
	putpwent((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1227
	case 137: {
	//AddAndReplaceKind
	random_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1228
	case 138: {
	//AddAndReplaceKind
	realloc((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1229
	case 139: {
	//AddAndReplaceKind
	realpath((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1230
	case 140: {
	//AddAndReplaceKind
	realpath_cache_del((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1231
	case 141: {
	//AddAndReplaceKind
	rename((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1232
	case 142: {
	//AddAndReplaceKind
	rindex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1233
	case 143: {
	//AddAndReplaceKind
	scanf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1234
	case 144: {
	//AddAndReplaceKind
	seed48_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1235
	case 145: {
	//AddAndReplaceKind
	seekdir((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1236
	case 146: {
	//AddAndReplaceKind
	setbuf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1237
	case 147: {
	//AddAndReplaceKind
	setdomainname((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1238
	case 148: {
	//AddAndReplaceKind
	sethostname((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1239
	case 149: {
	//AddAndReplaceKind
	setstate_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1240
	case 150: {
	//AddAndReplaceKind
	settimeofday((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1241
	case 151: {
	//AddAndReplaceKind
	sigaddset((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1242
	case 152: {
	//AddAndReplaceKind
	sigaltstack((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1243
	case 153: {
	//AddAndReplaceKind
	sigdelset((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1244
	case 154: {
	//AddAndReplaceKind
	sigismember((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1245
	case 155: {
	//AddAndReplaceKind
	siglongjmp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1246
	case 156: {
	//AddAndReplaceKind
	sigstack((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1247
	case 157: {
	//AddAndReplaceKind
	sigwait((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1248
	case 158: {
	//AddAndReplaceKind
	sigwaitinfo((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1249
	case 159: {
	//AddAndReplaceKind
	sprintf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1250
	case 160: {
	//AddAndReplaceKind
	sscanf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1251
	case 161: {
	//AddAndReplaceKind
	stat((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1252
	case 162: {
	//AddAndReplaceKind
	stpcpy((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1253
	case 163: {
	//AddAndReplaceKind
	strcasecmp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1254
	case 164: {
	//AddAndReplaceKind
	strcat((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1255
	case 165: {
	//AddAndReplaceKind
	strchr((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1256
	case 166: {
	//AddAndReplaceKind
	strcmp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1257
	case 167: {
	//AddAndReplaceKind
	strcoll((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1258
	case 168: {
	//AddAndReplaceKind
	strcpy((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1259
	case 169: {
	//AddAndReplaceKind
	strcspn((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1260
	case 170: {
	//AddAndReplaceKind
	strndup((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1261
	case 171: {
	//AddAndReplaceKind
	strnlen((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1262
	case 172: {
	//AddAndReplaceKind
	strpbrk((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1263
	case 173: {
	//AddAndReplaceKind
	strrchr((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1264
	case 174: {
	//AddAndReplaceKind
	strsep((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1265
	case 175: {
	//AddAndReplaceKind
	strspn((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1266
	case 176: {
	//AddAndReplaceKind
	strstr((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1267
	case 177: {
	//AddAndReplaceKind
	strtod((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1268
	case 178: {
	//AddAndReplaceKind
	strtof((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1269
	case 179: {
	//AddAndReplaceKind
	strtok((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1270
	case 180: {
	//AddAndReplaceKind
	strtold((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1271
	case 181: {
	//AddAndReplaceKind
	symlink((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1272
	case 182: {
	//AddAndReplaceKind
	tempnam((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1273
	case 183: {
	//AddAndReplaceKind
	timer_gettime((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1274
	case 184: {
	//AddAndReplaceKind
	timespec_get((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1275
	case 185: {
	//AddAndReplaceKind
	truncate((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1276
	case 186: {
	//AddAndReplaceKind
	tsrm_realpath((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1277
	case 187: {
	//AddAndReplaceKind
	utime((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1278
	case 188: {
	//AddAndReplaceKind
	utimes((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1279
	case 189: {
	//AddAndReplaceKind
	virtual_access((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1280
	case 190: {
	//AddAndReplaceKind
	virtual_chdir_file((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1281
	case 191: {
	//AddAndReplaceKind
	virtual_chmod((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1282
	case 192: {
	//AddAndReplaceKind
	virtual_creat((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1283
	case 193: {
	//AddAndReplaceKind
	virtual_filepath((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1284
	case 194: {
	//AddAndReplaceKind
	virtual_fopen((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1285
	case 195: {
	//AddAndReplaceKind
	virtual_getcwd((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1286
	case 196: {
	//AddAndReplaceKind
	virtual_lstat((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1287
	case 197: {
	//AddAndReplaceKind
	virtual_mkdir((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1288
	case 198: {
	//AddAndReplaceKind
	virtual_open((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1289
	case 199: {
	//AddAndReplaceKind
	virtual_popen((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1290
	case 200: {
	//AddAndReplaceKind
	virtual_realpath((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1291
	case 201: {
	//AddAndReplaceKind
	virtual_rename((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1292
	case 202: {
	//AddAndReplaceKind
	virtual_stat((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1293
	case 203: {
	//AddAndReplaceKind
	virtual_utime((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1294
	case 204: {
	//AddAndReplaceKind
	vprintf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1295
	case 205: {
	//AddAndReplaceKind
	vscanf((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1296
	case 206: {
	//AddAndReplaceKind
	wctomb((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1297
	case 207: {
	//AddAndReplaceKind
	zend_add_literal((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1298
	case 208: {
	//AddAndReplaceKind
	zend_add_to_list((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1299
	case 209: {
	//AddAndReplaceKind
	zend_atoi((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1300
	case 210: {
	//AddAndReplaceKind
	zend_atol((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1301
	case 211: {
	//AddAndReplaceKind
	zend_binary_zval_strcasecmp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1302
	case 212: {
	//AddAndReplaceKind
	zend_binary_zval_strcmp((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1303
	case 213: {
	//AddAndReplaceKind
	zend_call_function((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1304
	case 214: {
	//AddAndReplaceKind
	zend_check_protected((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1305
	case 215: {
	//AddAndReplaceKind
	zend_class_implements((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1306
	case 216: {
	//AddAndReplaceKind
	zend_compare_file_handles((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1307
	case 217: {
	//AddAndReplaceKind
	zend_copy_constants((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1308
	case 218: {
	//AddAndReplaceKind
	zend_delete_global_variable((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1309
	case 219: {
	//AddAndReplaceKind
	zend_dirname((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1310
	case 220: {
	//AddAndReplaceKind
	zend_disable_class((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1311
	case 221: {
	//AddAndReplaceKind
	zend_disable_function((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1312
	case 222: {
	//AddAndReplaceKind
	zend_do_begin_class_member_function_call((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1313
	case 223: {
	//AddAndReplaceKind
	zend_do_begin_dynamic_function_call((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1314
	case 224: {
	//AddAndReplaceKind
	zend_do_begin_function_call((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1315
	case 225: {
	//AddAndReplaceKind
	zend_do_begin_namespace((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1316
	case 226: {
	//AddAndReplaceKind
	zend_do_begin_new_object((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1317
	case 227: {
	//AddAndReplaceKind
	zend_do_begin_qm_op((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1318
	case 228: {
	//AddAndReplaceKind
	zend_do_boolean_and_begin((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1319
	case 229: {
	//AddAndReplaceKind
	zend_do_boolean_or_begin((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1320
	case 230: {
	//AddAndReplaceKind
	zend_do_case_after_statement((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1321
	case 231: {
	//AddAndReplaceKind
	zend_do_clone((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1322
	case 232: {
	//AddAndReplaceKind
	zend_do_declare_class_constant((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1323
	case 233: {
	//AddAndReplaceKind
	zend_do_declare_constant((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1324
	case 234: {
	//AddAndReplaceKind
	zend_do_declare_stmt((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1325
	case 235: {
	//AddAndReplaceKind
	zend_do_default_before_statement((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1326
	case 236: {
	//AddAndReplaceKind
	zend_do_end_class_declaration((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1327
	case 237: {
	//AddAndReplaceKind
	zend_do_exit((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1328
	case 238: {
	//AddAndReplaceKind
	zend_do_fetch_class((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1329
	case 239: {
	//AddAndReplaceKind
	zend_do_fetch_lexical_variable((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1330
	case 240: {
	//AddAndReplaceKind
	zend_do_fetch_static_member((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1331
	case 241: {
	//AddAndReplaceKind
	zend_do_for_before_statement((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1332
	case 242: {
	//AddAndReplaceKind
	zend_do_for_cond((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1333
	case 243: {
	//AddAndReplaceKind
	zend_do_foreach_end((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1334
	case 244: {
	//AddAndReplaceKind
	zend_do_if_after_statement((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1335
	case 245: {
	//AddAndReplaceKind
	zend_do_if_cond((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1336
	case 246: {
	//AddAndReplaceKind
	zend_do_implement_interface((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1337
	case 247: {
	//AddAndReplaceKind
	zend_do_implement_trait((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1338
	case 248: {
	//AddAndReplaceKind
	zend_do_inherit_interfaces((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1339
	case 249: {
	//AddAndReplaceKind
	zend_do_inheritance((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1340
	case 250: {
	//AddAndReplaceKind
	zend_do_list_end((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1341
	case 251: {
	//AddAndReplaceKind
	zend_do_mark_last_catch((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1342
	case 252: {
	//AddAndReplaceKind
	zend_do_print((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1343
	case 253: {
	//AddAndReplaceKind
	zend_do_return((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1344
	case 254: {
	//AddAndReplaceKind
	zend_do_shell_exec((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1345
	case 255: {
	//AddAndReplaceKind
	zend_do_verify_access_types((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1346
	case 256: {
	//AddAndReplaceKind
	zend_do_while_cond((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1347
	case 257: {
	//AddAndReplaceKind
	zend_do_while_end((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1348
	case 258: {
	//AddAndReplaceKind
	zend_fcall_info_argn((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1349
	case 259: {
	//AddAndReplaceKind
	zend_fcall_info_args((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1350
	case 260: {
	//AddAndReplaceKind
	zend_fcall_info_args_clear((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1351
	case 261: {
	//AddAndReplaceKind
	zend_get_class_fetch_type((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1352
	case 262: {
	//AddAndReplaceKind
	zend_get_compiled_variable_value((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1353
	case 263: {
	//AddAndReplaceKind
	zend_get_hash_value((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1354
	case 264: {
	//AddAndReplaceKind
	zend_hash_apply((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1355
	case 265: {
	//AddAndReplaceKind
	zend_hash_func((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1356
	case 266: {
	//AddAndReplaceKind
	zend_hash_get_current_key_type_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1357
	case 267: {
	//AddAndReplaceKind
	zend_hash_get_pointer((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1358
	case 268: {
	//AddAndReplaceKind
	zend_hash_index_exists((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1359
	case 269: {
	//AddAndReplaceKind
	zend_hash_internal_pointer_end_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1360
	case 270: {
	//AddAndReplaceKind
	zend_hash_internal_pointer_reset_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1361
	case 271: {
	//AddAndReplaceKind
	zend_hash_move_backwards_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1362
	case 272: {
	//AddAndReplaceKind
	zend_hash_move_forward_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1363
	case 273: {
	//AddAndReplaceKind
	zend_hash_reverse_apply((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1364
	case 274: {
	//AddAndReplaceKind
	zend_hash_set_pointer((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1365
	case 275: {
	//AddAndReplaceKind
	zend_hex_strtod((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1366
	case 276: {
	//AddAndReplaceKind
	zend_ini_boolean_displayer_cb((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1367
	case 277: {
	//AddAndReplaceKind
	zend_ini_color_displayer_cb((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1368
	case 278: {
	//AddAndReplaceKind
	zend_init_list((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1369
	case 279: {
	//AddAndReplaceKind
	zend_initialize_class_data((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1370
	case 280: {
	//AddAndReplaceKind
	zend_inline_hash_func((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1371
	case 281: {
	//AddAndReplaceKind
	zend_is_auto_global((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1372
	case 282: {
	//AddAndReplaceKind
	zend_iterator_unwrap((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1373
	case 283: {
	//AddAndReplaceKind
	zend_list_insert((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1374
	case 284: {
	//AddAndReplaceKind
	zend_llist_add_element((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1375
	case 285: {
	//AddAndReplaceKind
	zend_llist_apply((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1376
	case 286: {
	//AddAndReplaceKind
	zend_llist_apply_with_del((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1377
	case 287: {
	//AddAndReplaceKind
	zend_llist_copy((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1378
	case 288: {
	//AddAndReplaceKind
	zend_llist_get_first_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1379
	case 289: {
	//AddAndReplaceKind
	zend_llist_get_last_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1380
	case 290: {
	//AddAndReplaceKind
	zend_llist_get_next_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1381
	case 291: {
	//AddAndReplaceKind
	zend_llist_get_prev_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1382
	case 292: {
	//AddAndReplaceKind
	zend_llist_prepend_element((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1383
	case 293: {
	//AddAndReplaceKind
	zend_llist_sort((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1384
	case 294: {
	//AddAndReplaceKind
	zend_make_callable((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1385
	case 295: {
	//AddAndReplaceKind
	zend_multibyte_set_script_encoding((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1386
	case 296: {
	//AddAndReplaceKind
	zend_multibyte_set_script_encoding_by_string((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1387
	case 297: {
	//AddAndReplaceKind
	zend_multibyte_yyinput_again((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1388
	case 298: {
	//AddAndReplaceKind
	zend_object_create_proxy((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1389
	case 299: {
	//AddAndReplaceKind
	zend_object_std_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1390
	case 300: {
	//AddAndReplaceKind
	zend_object_store_set_object((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1391
	case 301: {
	//AddAndReplaceKind
	zend_objects_destroy_object((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1392
	case 302: {
	//AddAndReplaceKind
	zend_objects_new((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1393
	case 303: {
	//AddAndReplaceKind
	zend_objects_store_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1394
	case 304: {
	//AddAndReplaceKind
	zend_oct_strtod((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1395
	case 305: {
	//AddAndReplaceKind
	zend_prepare_string_for_scanning((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1396
	case 306: {
	//AddAndReplaceKind
	zend_print_zval((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1397
	case 307: {
	//AddAndReplaceKind
	zend_print_zval_r((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1398
	case 308: {
	//AddAndReplaceKind
	zend_ptr_stack_apply((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1399
	case 309: {
	//AddAndReplaceKind
	zend_ptr_stack_init_ex((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1400
	case 310: {
	//AddAndReplaceKind
	zend_ptr_stack_n_pop((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1401
	case 311: {
	//AddAndReplaceKind
	zend_ptr_stack_n_push((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1402
	case 312: {
	//AddAndReplaceKind
	zend_ptr_stack_push((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1403
	case 313: {
	//AddAndReplaceKind
	zend_quick_get_constant((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1404
	case 314: {
	//AddAndReplaceKind
	zend_register_ini_entries((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1405
	case 315: {
	//AddAndReplaceKind
	zend_resolve_non_class_name((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1406
	case 316: {
	//AddAndReplaceKind
	zend_stack_top((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1407
	case 317: {
	//AddAndReplaceKind
	zend_startup((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1408
	case 318: {
	//AddAndReplaceKind
	zend_std_get_debug_info((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1409
	case 319: {
	//AddAndReplaceKind
	zend_str_tolower((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1410
	case 320: {
	//AddAndReplaceKind
	zend_str_tolower_dup((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1411
	case 321: {
	//AddAndReplaceKind
	zend_stream_open((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1412
	case 322: {
	//AddAndReplaceKind
	zend_string_to_double((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1413
	case 323: {
	//AddAndReplaceKind
	zend_strndup((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1414
	case 324: {
	//AddAndReplaceKind
	zend_strtod((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1415
	case 325: {
	//AddAndReplaceKind
	zend_ts_hash_apply((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1416
	case 326: {
	//AddAndReplaceKind
	zend_ts_hash_func((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1417
	case 327: {
	//AddAndReplaceKind
	zend_ts_hash_index_exists((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1418
	case 328: {
	//AddAndReplaceKind
	zend_ts_hash_reverse_apply((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1419
	case 329: {
	//AddAndReplaceKind
	zval_set_isref_to_p((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1420
	case 330: {
	//AddAndReplaceKind
	zval_set_refcount_p((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1421
	case 331: {
	//AddAndReplaceKind
	zval_update_constant((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1422
	case 332: {
	//AddAndReplaceKind
	zval_update_constant_inline_change((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1423
	case 333: {
	//AddAndReplaceKind
	zval_update_constant_no_inline_change((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1424
	case 334: {
	//GuardKind
	if (!__is_neg("14-334", 8, &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    _array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1425
	case 335: {
	//ReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1426
	case 336: {
	//ReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	}
	}
				{
	switch(__choose("__SWITCH15"))
	{
	case 0: 
	break;
	#ifdef COMPILE_1427
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1428
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1429
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1430
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1431
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1432
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_1433
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_1434
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1435
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1436
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_1437
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_1438
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_1439
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1440
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1441
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_1442
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_1443
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_1444
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1445
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_1446
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_1447
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1448
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1449
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1450
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1451
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1452
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1453
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_1454
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1455
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1456
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_1457
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1458
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1459
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1460
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1461
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1462
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1463
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1464
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1465
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1466
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1467
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1468
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1469
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1470
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1471
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1472
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1473
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1474
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1475
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1476
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1477
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1478
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1479
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1480
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1481
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1482
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1483
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1484
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1485
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1486
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1487
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1488
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1489
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1490
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1491
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1492
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1493
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1494
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1495
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1496
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_1497
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_1498
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_1499
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1500
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1501
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1502
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1503
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_1504
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1505
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1506
	case 80: {
	//IfExitKind
	if (__is_neg("15-80", 8, &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_1507
	case 81: {
	//IfExitKind
	if (__is_neg("15-81", 8, &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH16"))
	{
	case 0: {
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#ifdef COMPILE_1508
	case 1: {
	//AddAndReplaceKind
	Debug(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1509
	case 2: {
	//AddAndReplaceKind
	_php_stream_mmap_unmap_ex(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1510
	case 3: {
	//AddAndReplaceKind
	php_printf(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1511
	case 4: {
	//AddAndReplaceKind
	printf(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1512
	case 5: {
	//AddAndReplaceKind
	scanf(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1513
	case 6: {
	//AddAndReplaceKind
	seekdir(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1514
	case 7: {
	//AddAndReplaceKind
	truncate(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1515
	case 8: {
	//GuardKind
	if (!__is_neg("16-8", 8, &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((keyword)->refcount__gc), sizeof ((keyword)->refcount__gc), &((keyword)->is_ref__gc), sizeof ((keyword)->is_ref__gc), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &((keyword)->type), sizeof ((keyword)->type)))
	    add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1516
	case 9: {
	//ReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1517
	case 10: {
	//ReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	}
	}
				{
	switch(__choose("__SWITCH17"))
	{
	case 0: 
	break;
	#ifdef COMPILE_1518
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1519
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1520
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1521
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1522
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1523
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_1524
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_1525
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1526
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1527
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_1528
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_1529
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_1530
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1531
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1532
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_1533
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_1534
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_1535
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1536
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_1537
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_1538
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1539
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1540
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1541
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1542
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1543
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1544
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_1545
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1546
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1547
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_1548
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1549
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1550
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1551
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1552
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1553
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1554
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1555
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1556
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1557
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1558
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1559
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1560
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1561
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1562
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1563
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1564
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1565
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1566
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1567
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1568
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1569
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1570
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1571
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1572
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1573
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1574
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1575
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1576
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1577
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1578
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1579
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1580
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1581
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1582
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1583
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1584
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1585
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1586
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1587
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_1588
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_1589
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_1590
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1591
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1592
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1593
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1594
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_1595
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1596
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1597
	case 80: {
	//AddInitKind
	memset(&(token), 0, sizeof (*(&(token))));
	
	break;
	}
	#endif
	#ifdef COMPILE_1598
	case 81: {
	//AddInitKind
	memset(&(token).value, 0, sizeof (*(&(token).value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1599
	case 82: {
	//AddInitKind
	memset(&(token).value.str, 0, sizeof (*(&(token).value.str)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1600
	case 83: {
	//AddInitKind
	memset(&compiler_globals, 0, sizeof (*(&compiler_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1601
	case 84: {
	//AddInitKind
	memset(&language_scanner_globals, 0, sizeof (*(&language_scanner_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1602
	case 85: {
	//IfExitKind
	if (__is_neg("17-85", 77, &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(token_line), sizeof (token_line), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_1603
	case 86: {
	//IfExitKind
	if (__is_neg("17-86", 77, &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(token_line), sizeof (token_line), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)))
	    return;
	
	break;
	}
	#endif
	}
	{
	long long __temp18=(token_type == 385) ;
	switch(__choose("__SWITCH18"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_1604
	case 1: {
	__temp18= ((token_type == 385) || __is_neg("18-1", 77, &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(token_line), sizeof (token_line), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1605
	case 2: {
	__temp18= ((token_type == 385) && !__is_neg("18-2", 77, &(token_type), sizeof (token_type), &(keyword), sizeof (keyword), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(token_line), sizeof (token_line), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH21"))
	{
	case 0: {
	if (__temp18) {
					if (CG(increment_lineno)) {
						token_line = ++CG(zend_lineno);
						CG(increment_lineno) = 0;
					}
					add_next_index_stringl(keyword, Z_STRVAL(token), Z_STRLEN(token), 1);
					efree(Z_STRVAL(token));
				} else {
					{
	switch(__choose("__SWITCH19"))
	{
	case 0: 
	break;
	#ifdef COMPILE_1606
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1607
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1608
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1609
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1610
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1611
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_1612
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_1613
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1614
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1615
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_1616
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_1617
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_1618
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1619
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1620
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_1621
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_1622
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_1623
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1624
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_1625
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_1626
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1627
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1628
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1629
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1630
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1631
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1632
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_1633
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1634
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1635
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_1636
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1637
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1638
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1639
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1640
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1641
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1642
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1643
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1644
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1645
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1646
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1647
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1648
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1649
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1650
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1651
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1652
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1653
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1654
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1655
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1656
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1657
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1658
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1659
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1660
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1661
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1662
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1663
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1664
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1665
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1666
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1667
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1668
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1669
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1670
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1671
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1672
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1673
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1674
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1675
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_1676
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_1677
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_1678
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1679
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1680
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1681
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1682
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_1683
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1684
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1685
	case 80: {
	//AddInitKind
	memset(&language_scanner_globals, 0, sizeof (*(&language_scanner_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1686
	case 81: {
	//IfExitKind
	if (__is_neg("19-81", 37, &(keyword), sizeof (keyword), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_1687
	case 82: {
	//IfExitKind
	if (__is_neg("19-82", 37, &(keyword), sizeof (keyword), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH20"))
	{
	case 0: {
	add_next_index_stringl(keyword, (char *)zendtext, zendleng, 1);
	
	break;
	}
	#ifdef COMPILE_1688
	case 1: {
	//AddAndReplaceKind
	Debug(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1689
	case 2: {
	//AddAndReplaceKind
	_php_stream_printf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1690
	case 3: {
	//AddAndReplaceKind
	add_assoc_bool_ex(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1691
	case 4: {
	//AddAndReplaceKind
	add_assoc_resource_ex(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1692
	case 5: {
	//AddAndReplaceKind
	add_property_bool_ex(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1693
	case 6: {
	//AddAndReplaceKind
	ap_php_asprintf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1694
	case 7: {
	//AddAndReplaceKind
	execl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1695
	case 8: {
	//AddAndReplaceKind
	execle(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1696
	case 9: {
	//AddAndReplaceKind
	execlp(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1697
	case 10: {
	//AddAndReplaceKind
	fprintf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1698
	case 11: {
	//AddAndReplaceKind
	fscanf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1699
	case 12: {
	//AddAndReplaceKind
	php_printf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1700
	case 13: {
	//AddAndReplaceKind
	php_sprintf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1701
	case 14: {
	//AddAndReplaceKind
	printf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1702
	case 15: {
	//AddAndReplaceKind
	scanf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1703
	case 16: {
	//AddAndReplaceKind
	sprintf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1704
	case 17: {
	//AddAndReplaceKind
	sscanf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1705
	case 18: {
	//GuardKind
	if (!__is_neg("20-18", 37, &(keyword), sizeof (keyword), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1706
	case 19: {
	//ReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1707
	case 20: {
	//ReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	}
	}
				}
	
	break;
	}
	#ifdef COMPILE_1708
	case 1: {
	//AddAndReplaceKind
	Debug(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1709
	case 2: {
	//AddAndReplaceKind
	_php_stream_printf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1710
	case 3: {
	//AddAndReplaceKind
	add_assoc_bool_ex(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1711
	case 4: {
	//AddAndReplaceKind
	add_assoc_resource_ex(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1712
	case 5: {
	//AddAndReplaceKind
	add_property_bool_ex(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1713
	case 6: {
	//AddAndReplaceKind
	ap_php_asprintf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1714
	case 7: {
	//AddAndReplaceKind
	execl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1715
	case 8: {
	//AddAndReplaceKind
	execle(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1716
	case 9: {
	//AddAndReplaceKind
	execlp(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1717
	case 10: {
	//AddAndReplaceKind
	fprintf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1718
	case 11: {
	//AddAndReplaceKind
	fscanf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1719
	case 12: {
	//AddAndReplaceKind
	php_printf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1720
	case 13: {
	//AddAndReplaceKind
	php_sprintf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1721
	case 14: {
	//AddAndReplaceKind
	printf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1722
	case 15: {
	//AddAndReplaceKind
	scanf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1723
	case 16: {
	//AddAndReplaceKind
	sprintf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1724
	case 17: {
	//AddAndReplaceKind
	sscanf(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1725
	case 18: {
	//GuardKind
	if (!__is_neg("21-18", 37, &(keyword), sizeof (keyword), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1726
	case 19: {
	//ReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1727
	case 20: {
	//ReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	}
	}
	}
				{
	switch(__choose("__SWITCH22"))
	{
	case 0: 
	break;
	#ifdef COMPILE_1728
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1729
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1730
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1731
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1732
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1733
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_1734
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_1735
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1736
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1737
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_1738
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_1739
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_1740
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1741
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1742
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_1743
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_1744
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_1745
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1746
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_1747
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_1748
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1749
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1750
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1751
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1752
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1753
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1754
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_1755
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1756
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1757
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_1758
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1759
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1760
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1761
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1762
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1763
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1764
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1765
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1766
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1767
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1768
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1769
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1770
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1771
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1772
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1773
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1774
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1775
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1776
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1777
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1778
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1779
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1780
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1781
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1782
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1783
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1784
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1785
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1786
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1787
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1788
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1789
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1790
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1791
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1792
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1793
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1794
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1795
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1796
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1797
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_1798
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_1799
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_1800
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1801
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1802
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1803
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1804
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_1805
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1806
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1807
	case 80: {
	//IfExitKind
	if (__is_neg("22-80", 5, &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_1808
	case 81: {
	//IfExitKind
	if (__is_neg("22-81", 5, &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH23"))
	{
	case 0: {
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#ifdef COMPILE_1809
	case 1: {
	//AddAndReplaceKind
	Debug(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1810
	case 2: {
	//AddAndReplaceKind
	_php_stream_mmap_unmap_ex(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1811
	case 3: {
	//AddAndReplaceKind
	php_printf(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1812
	case 4: {
	//AddAndReplaceKind
	printf(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1813
	case 5: {
	//AddAndReplaceKind
	scanf(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1814
	case 6: {
	//AddAndReplaceKind
	seekdir(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1815
	case 7: {
	//AddAndReplaceKind
	truncate(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1816
	case 8: {
	//GuardKind
	if (!__is_neg("23-8", 5, &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(return_value), sizeof (return_value), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type)))
	    add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1817
	case 9: {
	//ReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1818
	case 10: {
	//ReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	}
	}
				{
	switch(__choose("__SWITCH24"))
	{
	case 0: 
	break;
	#ifdef COMPILE_1819
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1820
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1821
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1822
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1823
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1824
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_1825
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_1826
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1827
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1828
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_1829
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_1830
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_1831
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1832
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1833
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_1834
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_1835
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_1836
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1837
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_1838
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_1839
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1840
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1841
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1842
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1843
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1844
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1845
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_1846
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1847
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1848
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_1849
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1850
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1851
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1852
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1853
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1854
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1855
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1856
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1857
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1858
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1859
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1860
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1861
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1862
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1863
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1864
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1865
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1866
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1867
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1868
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1869
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1870
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1871
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1872
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1873
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1874
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1875
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1876
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1877
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1878
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1879
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1880
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1881
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1882
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1883
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1884
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1885
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1886
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1887
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1888
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_1889
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_1890
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_1891
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1892
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1893
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1894
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1895
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_1896
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1897
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1898
	case 80: {
	//IfExitKind
	if (__is_neg("24-80", 5, &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_1899
	case 81: {
	//IfExitKind
	if (__is_neg("24-81", 5, &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH25"))
	{
	case 0: {
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#ifdef COMPILE_1900
	case 1: {
	//AddAndReplaceKind
	Debug(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1901
	case 2: {
	//AddAndReplaceKind
	_object_init_ex(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1902
	case 3: {
	//AddAndReplaceKind
	bitwise_not_function(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1903
	case 4: {
	//AddAndReplaceKind
	boolean_not_function(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1904
	case 5: {
	//AddAndReplaceKind
	compile_string(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1905
	case 6: {
	//AddAndReplaceKind
	php_printf(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1906
	case 7: {
	//AddAndReplaceKind
	printf(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1907
	case 8: {
	//AddAndReplaceKind
	scanf(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1908
	case 9: {
	//AddAndReplaceKind
	zend_binary_zval_strcasecmp(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1909
	case 10: {
	//AddAndReplaceKind
	zend_binary_zval_strcmp(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1910
	case 11: {
	//AddAndReplaceKind
	zend_fcall_info_args(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1911
	case 12: {
	//AddAndReplaceKind
	zend_iterator_unwrap(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1912
	case 13: {
	//AddAndReplaceKind
	zend_make_callable(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1913
	case 14: {
	//AddAndReplaceKind
	zend_object_create_proxy(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1914
	case 15: {
	//AddAndReplaceKind
	zend_object_store_set_object(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1915
	case 16: {
	//AddAndReplaceKind
	zend_prepare_string_for_scanning(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1916
	case 17: {
	//AddAndReplaceKind
	zend_std_get_debug_info(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1917
	case 18: {
	//GuardKind
	if (!__is_neg("25-18", 5, &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(destroy), sizeof (destroy), &(token_type), sizeof (token_type)))
	    add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1918
	case 19: {
	//ReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1919
	case 20: {
	//ReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1920
	case 21: {
	//ReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1921
	case 22: {
	//ReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	}
	}
			} else {
				{
	switch(__choose("__SWITCH26"))
	{
	case 0: 
	break;
	#ifdef COMPILE_1922
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1923
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1924
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1925
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1926
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1927
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_1928
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_1929
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_1930
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1931
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_1932
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_1933
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_1934
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1935
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1936
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_1937
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_1938
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_1939
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_1940
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_1941
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_1942
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1943
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1944
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1945
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_1946
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1947
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1948
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_1949
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1950
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1951
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_1952
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1953
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1954
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1955
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1956
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1957
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1958
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1959
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1960
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1961
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1962
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_1963
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_1964
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1965
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1966
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1967
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1968
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1969
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1970
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1971
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1972
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1973
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1974
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1975
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_1976
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1977
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1978
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_1979
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_1980
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1981
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1982
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1983
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_1984
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1985
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1986
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1987
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1988
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1989
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1990
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_1991
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_1992
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_1993
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_1994
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1995
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_1996
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_1997
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_1998
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_1999
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2000
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2001
	case 80: {
	//AddInitKind
	memset(&language_scanner_globals, 0, sizeof (*(&language_scanner_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2002
	case 81: {
	//IfExitKind
	if (__is_neg("26-81", 37, &(return_value), sizeof (return_value), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(keyword), sizeof (keyword), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_2003
	case 82: {
	//IfExitKind
	if (__is_neg("26-82", 37, &(return_value), sizeof (return_value), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(keyword), sizeof (keyword), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH27"))
	{
	case 0: {
	add_next_index_stringl(return_value, (char *)zendtext, zendleng, 1);
	
	break;
	}
	#ifdef COMPILE_2004
	case 1: {
	//AddAndReplaceKind
	Debug(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2005
	case 2: {
	//AddAndReplaceKind
	_php_stream_printf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2006
	case 3: {
	//AddAndReplaceKind
	add_assoc_bool_ex(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2007
	case 4: {
	//AddAndReplaceKind
	add_assoc_resource_ex(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2008
	case 5: {
	//AddAndReplaceKind
	add_property_bool_ex(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2009
	case 6: {
	//AddAndReplaceKind
	ap_php_asprintf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2010
	case 7: {
	//AddAndReplaceKind
	execl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2011
	case 8: {
	//AddAndReplaceKind
	execle(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2012
	case 9: {
	//AddAndReplaceKind
	execlp(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2013
	case 10: {
	//AddAndReplaceKind
	fprintf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2014
	case 11: {
	//AddAndReplaceKind
	fscanf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2015
	case 12: {
	//AddAndReplaceKind
	php_printf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2016
	case 13: {
	//AddAndReplaceKind
	php_sprintf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2017
	case 14: {
	//AddAndReplaceKind
	printf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2018
	case 15: {
	//AddAndReplaceKind
	scanf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2019
	case 16: {
	//AddAndReplaceKind
	sprintf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2020
	case 17: {
	//AddAndReplaceKind
	sscanf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2021
	case 18: {
	//GuardKind
	if (!__is_neg("27-18", 37, &(return_value), sizeof (return_value), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(keyword), sizeof (keyword), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2022
	case 19: {
	//ReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2023
	case 20: {
	//ReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	}
	}
			}
	
	break;
	}
	#ifdef COMPILE_2024
	case 1: {
	//AddAndReplaceKind
	Debug(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2025
	case 2: {
	//AddAndReplaceKind
	_php_stream_printf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2026
	case 3: {
	//AddAndReplaceKind
	add_assoc_bool_ex(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2027
	case 4: {
	//AddAndReplaceKind
	add_assoc_resource_ex(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2028
	case 5: {
	//AddAndReplaceKind
	add_property_bool_ex(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2029
	case 6: {
	//AddAndReplaceKind
	ap_php_asprintf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2030
	case 7: {
	//AddAndReplaceKind
	execl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2031
	case 8: {
	//AddAndReplaceKind
	execle(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2032
	case 9: {
	//AddAndReplaceKind
	execlp(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2033
	case 10: {
	//AddAndReplaceKind
	fprintf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2034
	case 11: {
	//AddAndReplaceKind
	fscanf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2035
	case 12: {
	//AddAndReplaceKind
	php_printf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2036
	case 13: {
	//AddAndReplaceKind
	php_sprintf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2037
	case 14: {
	//AddAndReplaceKind
	printf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2038
	case 15: {
	//AddAndReplaceKind
	scanf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2039
	case 16: {
	//AddAndReplaceKind
	sprintf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2040
	case 17: {
	//AddAndReplaceKind
	sscanf(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2041
	case 18: {
	//GuardKind
	if (!__is_neg("28-18", 37, &(return_value), sizeof (return_value), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(keyword), sizeof (keyword), &(destroy), sizeof (destroy), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2042
	case 19: {
	//ReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2043
	case 20: {
	//ReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	}
	}
	}
			{
	switch(__choose("__SWITCH29"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2044
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2045
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2046
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2047
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2048
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2049
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2050
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2051
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2052
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2053
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2054
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2055
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2056
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2057
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2058
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2059
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2060
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2061
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2062
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2063
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2064
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2065
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2066
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2067
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2068
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2069
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2070
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_2071
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2072
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2073
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2074
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2075
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2076
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2077
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2078
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2079
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2080
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2081
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2082
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2083
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2084
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2085
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2086
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2087
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2088
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2089
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2090
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2091
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2092
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2093
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2094
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2095
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2096
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2097
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2098
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2099
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2100
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2101
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2102
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2103
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2104
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2105
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2106
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2107
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2108
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2109
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2110
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2111
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2112
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2113
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2114
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2115
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2116
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_2117
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_2118
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2119
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2120
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2121
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2122
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2123
	case 80: {
	//AddInitKind
	memset(&(token), 0, sizeof (*(&(token))));
	
	break;
	}
	#endif
	#ifdef COMPILE_2124
	case 81: {
	//IfExitKind
	if (__is_neg("29-81", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_2125
	case 82: {
	//IfExitKind
	if (__is_neg("29-82", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc)))
	    return;
	
	break;
	}
	#endif
	}
	{
	long long __temp30=(destroy && (token).type != 0) ;
	switch(__choose("__SWITCH30"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_2126
	case 1: {
	__temp30= (((destroy) || __is_neg("30-1", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc))) && (token).type != 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2127
	case 2: {
	__temp30= ((destroy && (token).type != 0) || __is_neg("30-2", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2128
	case 3: {
	__temp30= ((destroy && (token).type != 0) && !__is_neg("30-3", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(token_line), sizeof (token_line), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH33"))
	{
	case 0: {
	if (__temp30) {
				{
	switch(__choose("__SWITCH31"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2129
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2130
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2131
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2132
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2133
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2134
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2135
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2136
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2137
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2138
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2139
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2140
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2141
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2142
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2143
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2144
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2145
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2146
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2147
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2148
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2149
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2150
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2151
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2152
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2153
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2154
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2155
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_2156
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2157
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2158
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2159
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2160
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2161
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2162
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2163
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2164
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2165
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2166
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2167
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2168
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2169
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2170
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2171
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2172
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2173
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2174
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2175
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2176
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2177
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2178
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2179
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2180
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2181
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2182
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2183
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2184
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2185
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2186
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2187
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2188
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2189
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2190
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2191
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2192
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2193
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2194
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2195
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2196
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2197
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2198
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2199
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2200
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2201
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_2202
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_2203
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2204
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2205
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2206
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2207
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2208
	case 80: {
	//IfExitKind
	if (__is_neg("31-80", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(token_line), sizeof (token_line), &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_2209
	case 81: {
	//IfExitKind
	if (__is_neg("31-81", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(token_line), sizeof (token_line), &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH32"))
	{
	case 0: {
	zval_dtor(&token);
	
	break;
	}
	#ifdef COMPILE_2210
	case 1: {
	//AddAndReplaceKind
	_convert_to_string((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2211
	case 2: {
	//AddAndReplaceKind
	_object_init((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2212
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2213
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2214
	case 5: {
	//AddAndReplaceKind
	_zval_dtor_func((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2215
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_wrapper((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2216
	case 7: {
	//AddAndReplaceKind
	_zval_internal_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2217
	case 8: {
	//AddAndReplaceKind
	add_next_index_null((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2218
	case 9: {
	//AddAndReplaceKind
	config_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2219
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2220
	case 11: {
	//AddAndReplaceKind
	convert_to_array((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2221
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2222
	case 13: {
	//AddAndReplaceKind
	convert_to_double((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2223
	case 14: {
	//AddAndReplaceKind
	convert_to_long((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2224
	case 15: {
	//AddAndReplaceKind
	convert_to_null((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2225
	case 16: {
	//AddAndReplaceKind
	convert_to_object((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2226
	case 17: {
	//AddAndReplaceKind
	decrement_function((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2227
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2228
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2229
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2230
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2231
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2232
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2233
	case 24: {
	//AddAndReplaceKind
	increment_function((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2234
	case 25: {
	//AddAndReplaceKind
	lex_scan((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2235
	case 26: {
	//AddAndReplaceKind
	php_output_get_contents((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2236
	case 27: {
	//AddAndReplaceKind
	php_output_get_length((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2237
	case 28: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2238
	case 29: {
	//AddAndReplaceKind
	tokenize((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2239
	case 30: {
	//AddAndReplaceKind
	zend_is_true((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2240
	case 31: {
	//AddAndReplaceKind
	zend_locale_sprintf_double((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2241
	case 32: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2242
	case 33: {
	//AddAndReplaceKind
	zend_objects_clone_obj((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2243
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_add_ref((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2244
	case 35: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2245
	case 36: {
	//AddAndReplaceKind
	zend_objects_store_del_ref((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2246
	case 37: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2247
	case 38: {
	//AddAndReplaceKind
	zend_print_flat_zval_r((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2248
	case 39: {
	//AddAndReplaceKind
	zend_print_variable((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2249
	case 40: {
	//AddAndReplaceKind
	zend_std_get_constructor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2250
	case 41: {
	//AddAndReplaceKind
	zend_std_get_properties((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2251
	case 42: {
	//AddAndReplaceKind
	zval_addref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2252
	case 43: {
	//AddAndReplaceKind
	zval_delref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2253
	case 44: {
	//AddAndReplaceKind
	zval_is_true((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2254
	case 45: {
	//AddAndReplaceKind
	zval_isref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2255
	case 46: {
	//AddAndReplaceKind
	zval_refcount_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2256
	case 47: {
	//AddAndReplaceKind
	zval_set_isref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2257
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2258
	case 49: {
	//GuardKind
	if (!__is_neg("32-49", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(token_line), sizeof (token_line), &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc)))
	    _zval_dtor((&token));
	
	break;
	}
	#endif
	}
	}
			}
	
	break;
	}
	#ifdef COMPILE_2259
	case 1: {
	//AddAndReplaceKind
	_convert_to_string((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2260
	case 2: {
	//AddAndReplaceKind
	_object_init((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2261
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2262
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2263
	case 5: {
	//AddAndReplaceKind
	_zval_dtor_func((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2264
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_wrapper((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2265
	case 7: {
	//AddAndReplaceKind
	_zval_internal_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2266
	case 8: {
	//AddAndReplaceKind
	add_next_index_null((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2267
	case 9: {
	//AddAndReplaceKind
	config_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2268
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2269
	case 11: {
	//AddAndReplaceKind
	convert_to_array((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2270
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2271
	case 13: {
	//AddAndReplaceKind
	convert_to_double((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2272
	case 14: {
	//AddAndReplaceKind
	convert_to_long((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2273
	case 15: {
	//AddAndReplaceKind
	convert_to_null((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2274
	case 16: {
	//AddAndReplaceKind
	convert_to_object((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2275
	case 17: {
	//AddAndReplaceKind
	decrement_function((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2276
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2277
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2278
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2279
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2280
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2281
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2282
	case 24: {
	//AddAndReplaceKind
	increment_function((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2283
	case 25: {
	//AddAndReplaceKind
	lex_scan((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2284
	case 26: {
	//AddAndReplaceKind
	php_output_get_contents((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2285
	case 27: {
	//AddAndReplaceKind
	php_output_get_length((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2286
	case 28: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2287
	case 29: {
	//AddAndReplaceKind
	tokenize((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2288
	case 30: {
	//AddAndReplaceKind
	zend_is_true((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2289
	case 31: {
	//AddAndReplaceKind
	zend_locale_sprintf_double((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2290
	case 32: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2291
	case 33: {
	//AddAndReplaceKind
	zend_objects_clone_obj((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2292
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_add_ref((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2293
	case 35: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2294
	case 36: {
	//AddAndReplaceKind
	zend_objects_store_del_ref((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2295
	case 37: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2296
	case 38: {
	//AddAndReplaceKind
	zend_print_flat_zval_r((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2297
	case 39: {
	//AddAndReplaceKind
	zend_print_variable((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2298
	case 40: {
	//AddAndReplaceKind
	zend_std_get_constructor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2299
	case 41: {
	//AddAndReplaceKind
	zend_std_get_properties((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2300
	case 42: {
	//AddAndReplaceKind
	zval_addref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2301
	case 43: {
	//AddAndReplaceKind
	zval_delref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2302
	case 44: {
	//AddAndReplaceKind
	zval_is_true((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2303
	case 45: {
	//AddAndReplaceKind
	zval_isref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2304
	case 46: {
	//AddAndReplaceKind
	zval_refcount_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2305
	case 47: {
	//AddAndReplaceKind
	zval_set_isref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2306
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2307
	case 49: {
	//GuardKind
	if (!__is_neg("33-49", 8, &(destroy), sizeof (destroy), &((token).type), sizeof ((token).type), &(return_value), sizeof (return_value), &(token_line), sizeof (token_line), &(keyword), sizeof (keyword), &(token_type), sizeof (token_type), &((token).refcount__gc), sizeof ((token).refcount__gc), &((token).is_ref__gc), sizeof ((token).is_ref__gc)))
	    _zval_dtor((&token));
	
	break;
	}
	#endif
	}
	}
	}
			ZVAL_NULL(&token);
	
			{
	switch(__choose("__SWITCH34"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2308
	case 1: {
	//AddAndReplaceKind
	((keyword))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2309
	case 2: {
	//AddAndReplaceKind
	((keyword))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2310
	case 3: {
	//AddAndReplaceKind
	((return_value))->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2311
	case 4: {
	//AddAndReplaceKind
	((return_value))->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2312
	case 5: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2313
	case 6: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2314
	case 7: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2315
	case 8: {
	//AddAndReplaceKind
	(keyword)->is_ref__gc = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2316
	case 9: {
	//AddAndReplaceKind
	(keyword)->refcount__gc = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2317
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2318
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2319
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2320
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2321
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2322
	case 15: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2323
	case 16: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2324
	case 17: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2325
	case 18: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2326
	case 19: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2327
	case 20: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2328
	case 21: {
	//AddAndReplaceKind
	_array_init(((keyword)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2329
	case 22: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2330
	case 23: {
	//AddAndReplaceKind
	_array_init((keyword), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2331
	case 24: {
	//AddAndReplaceKind
	_array_init((return_value), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2332
	case 25: {
	//AddAndReplaceKind
	_efree(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2333
	case 26: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2334
	case 27: {
	//AddAndReplaceKind
	_efree(((token).value.str.val));
	
	break;
	}
	#endif
	#ifdef COMPILE_2335
	case 28: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2336
	case 29: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2337
	case 30: {
	//AddAndReplaceKind
	_zval_dtor((&token));
	
	break;
	}
	#endif
	#ifdef COMPILE_2338
	case 31: {
	//AddAndReplaceKind
	_zval_dtor(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2339
	case 32: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2340
	case 33: {
	//AddAndReplaceKind
	_zval_dtor_func((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2341
	case 34: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2342
	case 35: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2343
	case 36: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2344
	case 37: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2345
	case 38: {
	//AddAndReplaceKind
	add_next_index_long((keyword), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2346
	case 39: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2347
	case 40: {
	//AddAndReplaceKind
	add_next_index_long((return_value), token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2348
	case 41: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_line);
	
	break;
	}
	#endif
	#ifdef COMPILE_2349
	case 42: {
	//AddAndReplaceKind
	add_next_index_long(keyword, token_type);
	
	break;
	}
	#endif
	#ifdef COMPILE_2350
	case 43: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2351
	case 44: {
	//AddAndReplaceKind
	add_next_index_stringl((keyword), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2352
	case 45: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2353
	case 46: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2354
	case 47: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2355
	case 48: {
	//AddAndReplaceKind
	add_next_index_stringl(keyword, (token).value.str.val, (token).value.str.len, 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2356
	case 49: {
	//AddAndReplaceKind
	add_next_index_stringl(return_value, (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2357
	case 50: {
	//AddAndReplaceKind
	add_next_index_zval((keyword), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2358
	case 51: {
	//AddAndReplaceKind
	add_next_index_zval((return_value), keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2359
	case 52: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2360
	case 53: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, (return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2361
	case 54: {
	//AddAndReplaceKind
	add_next_index_zval(return_value, keyword);
	
	break;
	}
	#endif
	#ifdef COMPILE_2362
	case 55: {
	//AddAndReplaceKind
	convert_to_boolean((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2363
	case 56: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2364
	case 57: {
	//AddAndReplaceKind
	destroy = 1;
	
	break;
	}
	#endif
	#ifdef COMPILE_2365
	case 58: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2366
	case 59: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2367
	case 60: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2368
	case 61: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((keyword)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2369
	case 62: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2370
	case 63: {
	//AddAndReplaceKind
	gc_zval_possible_root((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2371
	case 64: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2372
	case 65: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2373
	case 66: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2374
	case 67: {
	//AddAndReplaceKind
	if ((compiler_globals.increment_lineno)) {
	    token_line = ++(compiler_globals.zend_lineno);
	    (compiler_globals.increment_lineno) = 0;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2375
	case 68: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2376
	case 69: {
	//AddAndReplaceKind
	if (destroy && (token).type != 0) {
	    _zval_dtor((&token));
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2377
	case 70: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2378
	case 71: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2379
	case 72: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2380
	case 73: {
	//AddAndReplaceKind
	token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_2381
	case 74: {
	//AddAndReplaceKind
	token_line = ++(compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	#ifdef COMPILE_2382
	case 75: {
	//AddAndReplaceKind
	tokenize((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2383
	case 76: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2384
	case 77: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2385
	case 78: {
	//AddAndReplaceKind
	zval_unset_isref_p((keyword));
	
	break;
	}
	#endif
	#ifdef COMPILE_2386
	case 79: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2387
	case 80: {
	//AddInitKind
	memset(&compiler_globals, 0, sizeof (*(&compiler_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2388
	case 81: {
	//IfExitKind
	if (__is_neg("34-81", 77, &(token_line), sizeof (token_line), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(token_type), sizeof (token_type), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)))
	    break;
	
	break;
	}
	#endif
	#ifdef COMPILE_2389
	case 82: {
	//IfExitKind
	if (__is_neg("34-82", 77, &(token_line), sizeof (token_line), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(token_type), sizeof (token_type), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH35"))
	{
	case 0: {
	token_line = CG(zend_lineno);
	
	break;
	}
	#ifdef COMPILE_2390
	case 1: {
	//GuardKind
	if (!__is_neg("35-1", 77, &(token_line), sizeof (token_line), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(token_type), sizeof (token_type), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)))
	    token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	}
	}
		}
	
	break;
	}
	#ifdef COMPILE_2391
	case 1: {
	//GuardKind
	if (!__is_neg("36-1", 77, &(token_line), sizeof (token_line), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(compiler_globals.zend_lineno), sizeof (compiler_globals.zend_lineno), &(destroy), sizeof (destroy), &(return_value), sizeof (return_value), &(keyword), sizeof (keyword), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(compiler_globals.increment_lineno), sizeof (compiler_globals.increment_lineno), &(token_type), sizeof (token_type), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared), &(compiler_globals.active_class_entry), sizeof (compiler_globals.active_class_entry), &(compiler_globals.compiled_filename), sizeof (compiler_globals.compiled_filename), &(compiler_globals.heredoc), sizeof (compiler_globals.heredoc), &(compiler_globals.heredoc_len), sizeof (compiler_globals.heredoc_len), &(compiler_globals.active_op_array), sizeof (compiler_globals.active_op_array), &(compiler_globals.function_table), sizeof (compiler_globals.function_table), &(compiler_globals.class_table), sizeof (compiler_globals.class_table), &(compiler_globals.auto_globals), sizeof (compiler_globals.auto_globals), &(compiler_globals.in_compilation), sizeof (compiler_globals.in_compilation), &(compiler_globals.short_tags), sizeof (compiler_globals.short_tags), &(compiler_globals.asp_tags), sizeof (compiler_globals.asp_tags), &(compiler_globals.unclean_shutdown), sizeof (compiler_globals.unclean_shutdown), &(compiler_globals.ini_parser_unbuffered_errors), sizeof (compiler_globals.ini_parser_unbuffered_errors), &(compiler_globals.catch_begin), sizeof (compiler_globals.catch_begin), &(compiler_globals.ini_parser_param), sizeof (compiler_globals.ini_parser_param), &(compiler_globals.interactive), sizeof (compiler_globals.interactive), &(compiler_globals.start_lineno), sizeof (compiler_globals.start_lineno), &(compiler_globals.access_type), sizeof (compiler_globals.access_type), &(compiler_globals.doc_comment), sizeof (compiler_globals.doc_comment), &(compiler_globals.doc_comment_len), sizeof (compiler_globals.doc_comment_len), &(compiler_globals.compiler_options), sizeof (compiler_globals.compiler_options), &(compiler_globals.current_namespace), sizeof (compiler_globals.current_namespace), &(compiler_globals.current_import), sizeof (compiler_globals.current_import), &(compiler_globals.in_namespace), sizeof (compiler_globals.in_namespace), &(compiler_globals.has_bracketed_namespaces), sizeof (compiler_globals.has_bracketed_namespaces), &(compiler_globals.interned_strings_start), sizeof (compiler_globals.interned_strings_start), &(compiler_globals.interned_strings_end), sizeof (compiler_globals.interned_strings_end), &(compiler_globals.interned_strings_top), sizeof (compiler_globals.interned_strings_top), &(compiler_globals.interned_strings_snapshot_top), sizeof (compiler_globals.interned_strings_snapshot_top), &(compiler_globals.script_encoding_list), sizeof (compiler_globals.script_encoding_list), &(compiler_globals.script_encoding_list_size), sizeof (compiler_globals.script_encoding_list_size), &(compiler_globals.multibyte), sizeof (compiler_globals.multibyte), &(compiler_globals.detect_unicode), sizeof (compiler_globals.detect_unicode), &(compiler_globals.encoding_declared), sizeof (compiler_globals.encoding_declared)))
	    token_line = (compiler_globals.zend_lineno);
	
	break;
	}
	#endif
	}
	}
}

/* {{{ proto array token_get_all(string source)
 */
PHP_FUNCTION(token_get_all)
{
	char *source = NULL;
	int argc = ZEND_NUM_ARGS();
	int source_len;
	zval source_z;
	zend_lex_state original_lex_state;

	//prophet generated patch
	{
	switch(__choose("__SWITCH37"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2392
	case 1: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2393
	case 2: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2394
	case 3: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2395
	case 4: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2396
	case 5: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2397
	case 6: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2398
	case 7: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2399
	case 8: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2400
	case 9: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2401
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2402
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2403
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2404
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2405
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2406
	case 15: {
	//AddAndReplaceKind
	(source)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2407
	case 16: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2408
	case 17: {
	//AddAndReplaceKind
	_array_init(((this_ptr)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2409
	case 18: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2410
	case 19: {
	//AddAndReplaceKind
	_efree(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2411
	case 20: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2412
	case 21: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2413
	case 22: {
	//AddAndReplaceKind
	_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2414
	case 23: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2415
	case 24: {
	//AddAndReplaceKind
	_zval_dtor(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2416
	case 25: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2417
	case 26: {
	//AddAndReplaceKind
	_zval_dtor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2418
	case 27: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2419
	case 28: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2420
	case 29: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2421
	case 30: {
	//AddAndReplaceKind
	add_next_index_stringl((this_ptr), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2422
	case 31: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2423
	case 32: {
	//AddAndReplaceKind
	convert_to_boolean((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2424
	case 33: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2425
	case 34: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2426
	case 35: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2427
	case 36: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2428
	case 37: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2429
	case 38: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2430
	case 39: {
	//AddAndReplaceKind
	gc_zval_possible_root((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2431
	case 40: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2432
	case 41: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2433
	case 42: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2434
	case 43: {
	//AddAndReplaceKind
	if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	    return;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2435
	case 44: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2436
	case 45: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2437
	case 46: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2438
	case 47: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2439
	case 48: {
	//AddAndReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2440
	case 49: {
	//AddAndReplaceKind
	tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2441
	case 50: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2442
	case 51: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2443
	case 52: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2444
	case 53: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2445
	case 54: {
	//AddAndReplaceKind
	zval_unset_isref_p((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2446
	case 55: {
	//IfExitKind
	if (__is_neg("37-55", 5, &(source), sizeof (source), &(argc), sizeof (argc), &(source_len), sizeof (source_len), &(ht), sizeof (ht), &(return_value), sizeof (return_value)))
	    return;
	
	break;
	}
	#endif
	}
	{
	long long __temp38=(zend_parse_parameters(argc, "s", &source, &source_len) == -1) ;
	switch(__choose("__SWITCH38"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_2447
	case 1: {
	__temp38= ((zend_parse_parameters(argc, "s", &source, &source_len) == -1) || __is_neg("38-1", 5, &(source), sizeof (source), &(argc), sizeof (argc), &(source_len), sizeof (source_len), &(ht), sizeof (ht), &(return_value), sizeof (return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2448
	case 2: {
	__temp38= ((zend_parse_parameters(argc, "s", &source, &source_len) == -1) && !__is_neg("38-2", 5, &(source), sizeof (source), &(argc), sizeof (argc), &(source_len), sizeof (source_len), &(ht), sizeof (ht), &(return_value), sizeof (return_value)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH39"))
	{
	case 0: {
	if (__temp38) {
			return;
		}
	
	break;
	}
	#ifdef COMPILE_2449
	case 1: {
	//GuardKind
	if (!__is_neg("39-1", 5, &(source), sizeof (source), &(argc), sizeof (argc), &(source_len), sizeof (source_len), &(ht), sizeof (ht), &(return_value), sizeof (return_value)))
	    if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	        return;
	    }
	
	break;
	}
	#endif
	#ifdef COMPILE_2450
	case 2: {
	//SpecialGuardKind
	if (!__is_neg("39-2", 5, &(source), sizeof (source), &(argc), sizeof (argc), &(source_len), sizeof (source_len), &(ht), sizeof (ht), &(return_value), sizeof (return_value)) && (zend_parse_parameters(argc, "s", &source, &source_len) == -1)) {
	    return;
	}
	
	break;
	}
	#endif
	}
	}
	}

	//prophet generated patch
	{
	switch(__choose("__SWITCH40"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2451
	case 1: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2452
	case 2: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2453
	case 3: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2454
	case 4: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2455
	case 5: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2456
	case 6: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2457
	case 7: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2458
	case 8: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2459
	case 9: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2460
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2461
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2462
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2463
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2464
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2465
	case 15: {
	//AddAndReplaceKind
	(source)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2466
	case 16: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2467
	case 17: {
	//AddAndReplaceKind
	_array_init(((this_ptr)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2468
	case 18: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2469
	case 19: {
	//AddAndReplaceKind
	_efree(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2470
	case 20: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2471
	case 21: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2472
	case 22: {
	//AddAndReplaceKind
	_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2473
	case 23: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2474
	case 24: {
	//AddAndReplaceKind
	_zval_dtor(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2475
	case 25: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2476
	case 26: {
	//AddAndReplaceKind
	_zval_dtor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2477
	case 27: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2478
	case 28: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2479
	case 29: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2480
	case 30: {
	//AddAndReplaceKind
	add_next_index_stringl((this_ptr), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2481
	case 31: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2482
	case 32: {
	//AddAndReplaceKind
	convert_to_boolean((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2483
	case 33: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2484
	case 34: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2485
	case 35: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2486
	case 36: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2487
	case 37: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2488
	case 38: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2489
	case 39: {
	//AddAndReplaceKind
	gc_zval_possible_root((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2490
	case 40: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2491
	case 41: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2492
	case 42: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2493
	case 43: {
	//AddAndReplaceKind
	if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	    return;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2494
	case 44: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2495
	case 45: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2496
	case 46: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2497
	case 47: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2498
	case 48: {
	//AddAndReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2499
	case 49: {
	//AddAndReplaceKind
	tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2500
	case 50: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2501
	case 51: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2502
	case 52: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2503
	case 53: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2504
	case 54: {
	//AddAndReplaceKind
	zval_unset_isref_p((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2505
	case 55: {
	//IfExitKind
	if (__is_neg("40-55", 5, &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(return_value), sizeof (return_value), &(ht), sizeof (ht)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH41"))
	{
	case 0: {
	ZVAL_STRINGL(&source_z, source, source_len, 1);
	
	break;
	}
	#ifdef COMPILE_2506
	case 1: {
	//GuardKind
	if (!__is_neg("41-1", 5, &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(return_value), sizeof (return_value), &(ht), sizeof (ht)))
	    do {
	        const char *__s = (source);
	        int __l = source_len;
	        zval *__z = (&source_z);
	        (*__z).value.str.len = __l;
	        (*__z).value.str.val = (1 ? _estrndup((__s), (__l)) : (char *)__s);
	        (*__z).type = 6;
	    } while (0);
	
	break;
	}
	#endif
	}
	}
	//prophet generated patch
	{
	switch(__choose("__SWITCH42"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2507
	case 1: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2508
	case 2: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2509
	case 3: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2510
	case 4: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2511
	case 5: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2512
	case 6: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2513
	case 7: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2514
	case 8: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2515
	case 9: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2516
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2517
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2518
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2519
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2520
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2521
	case 15: {
	//AddAndReplaceKind
	(source)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2522
	case 16: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2523
	case 17: {
	//AddAndReplaceKind
	_array_init(((this_ptr)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2524
	case 18: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2525
	case 19: {
	//AddAndReplaceKind
	_efree(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2526
	case 20: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2527
	case 21: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2528
	case 22: {
	//AddAndReplaceKind
	_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2529
	case 23: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2530
	case 24: {
	//AddAndReplaceKind
	_zval_dtor(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2531
	case 25: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2532
	case 26: {
	//AddAndReplaceKind
	_zval_dtor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2533
	case 27: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2534
	case 28: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2535
	case 29: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2536
	case 30: {
	//AddAndReplaceKind
	add_next_index_stringl((this_ptr), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2537
	case 31: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2538
	case 32: {
	//AddAndReplaceKind
	convert_to_boolean((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2539
	case 33: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2540
	case 34: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2541
	case 35: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2542
	case 36: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2543
	case 37: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2544
	case 38: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2545
	case 39: {
	//AddAndReplaceKind
	gc_zval_possible_root((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2546
	case 40: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2547
	case 41: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2548
	case 42: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2549
	case 43: {
	//AddAndReplaceKind
	if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	    return;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2550
	case 44: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2551
	case 45: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2552
	case 46: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2553
	case 47: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2554
	case 48: {
	//AddAndReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2555
	case 49: {
	//AddAndReplaceKind
	tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2556
	case 50: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2557
	case 51: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2558
	case 52: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2559
	case 53: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2560
	case 54: {
	//AddAndReplaceKind
	zval_unset_isref_p((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2561
	case 55: {
	//IfExitKind
	if (__is_neg("42-55", 5, &(source), sizeof (source), &(source_len), sizeof (source_len), &(return_value), sizeof (return_value), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH43"))
	{
	case 0: {
	zend_save_lexical_state(&original_lex_state TSRMLS_CC);
	
	break;
	}
	#ifdef COMPILE_2562
	case 1: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2563
	case 2: {
	//GuardKind
	if (!__is_neg("43-2", 5, &(source), sizeof (source), &(source_len), sizeof (source_len), &(return_value), sizeof (return_value), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	}
	}

	//prophet generated patch
	{
	switch(__choose("__SWITCH44"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2564
	case 1: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2565
	case 2: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2566
	case 3: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2567
	case 4: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2568
	case 5: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2569
	case 6: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2570
	case 7: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2571
	case 8: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2572
	case 9: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2573
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2574
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2575
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2576
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2577
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2578
	case 15: {
	//AddAndReplaceKind
	(source)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2579
	case 16: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2580
	case 17: {
	//AddAndReplaceKind
	_array_init(((this_ptr)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2581
	case 18: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2582
	case 19: {
	//AddAndReplaceKind
	_efree(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2583
	case 20: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2584
	case 21: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2585
	case 22: {
	//AddAndReplaceKind
	_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2586
	case 23: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2587
	case 24: {
	//AddAndReplaceKind
	_zval_dtor(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2588
	case 25: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2589
	case 26: {
	//AddAndReplaceKind
	_zval_dtor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2590
	case 27: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2591
	case 28: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2592
	case 29: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2593
	case 30: {
	//AddAndReplaceKind
	add_next_index_stringl((this_ptr), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2594
	case 31: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2595
	case 32: {
	//AddAndReplaceKind
	convert_to_boolean((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2596
	case 33: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2597
	case 34: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2598
	case 35: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2599
	case 36: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2600
	case 37: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2601
	case 38: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2602
	case 39: {
	//AddAndReplaceKind
	gc_zval_possible_root((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2603
	case 40: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2604
	case 41: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2605
	case 42: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2606
	case 43: {
	//AddAndReplaceKind
	if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	    return;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2607
	case 44: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2608
	case 45: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2609
	case 46: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2610
	case 47: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2611
	case 48: {
	//AddAndReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2612
	case 49: {
	//AddAndReplaceKind
	tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2613
	case 50: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2614
	case 51: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2615
	case 52: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2616
	case 53: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2617
	case 54: {
	//AddAndReplaceKind
	zval_unset_isref_p((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2618
	case 55: {
	//IfExitKind
	if (__is_neg("44-55", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    return;
	
	break;
	}
	#endif
	}
	{
	long long __temp45=(zend_prepare_string_for_scanning(&source_z, "") == -1) ;
	switch(__choose("__SWITCH45"))
	{
	case 0: {
	
	break;
	}
	#ifdef COMPILE_2619
	case 1: {
	__temp45= ((zend_prepare_string_for_scanning(&source_z, "") == -1) || __is_neg("45-1", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2620
	case 2: {
	__temp45= ((zend_prepare_string_for_scanning(&source_z, "") == -1) && !__is_neg("45-2", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)));
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH46"))
	{
	case 0: {
	if (__temp45) {
			zend_restore_lexical_state(&original_lex_state TSRMLS_CC);
			RETURN_FALSE;
		}
	
	break;
	}
	#ifdef COMPILE_2621
	case 1: {
	//GuardKind
	if (!__is_neg("46-1", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    if (zend_prepare_string_for_scanning(&source_z, "") == -1) {
	        zend_restore_lexical_state(&original_lex_state);
	        {
	            do {
	                zval *__z = (return_value);
	                (*__z).value.lval = ((0) != 0);
	                (*__z).type = 3;
	            } while (0);
	            return;
	        }
	        ;
	    }
	
	break;
	}
	#endif
	#ifdef COMPILE_2622
	case 2: {
	//SpecialGuardKind
	if (!__is_neg("46-2", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)) && (zend_prepare_string_for_scanning(&source_z, "") == -1)) {
	    zend_restore_lexical_state(&original_lex_state);
	    {
	        do {
	            zval *__z = (return_value);
	            (*__z).value.lval = ((0) != 0);
	            (*__z).type = 3;
	        } while (0);
	        return;
	    }
	    ;
	}
	
	break;
	}
	#endif
	}
	}
	}

	//prophet generated patch
	{
	switch(__choose("__SWITCH47"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2623
	case 1: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2624
	case 2: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2625
	case 3: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2626
	case 4: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2627
	case 5: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2628
	case 6: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2629
	case 7: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2630
	case 8: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2631
	case 9: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2632
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2633
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2634
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2635
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2636
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2637
	case 15: {
	//AddAndReplaceKind
	(source)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2638
	case 16: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2639
	case 17: {
	//AddAndReplaceKind
	_array_init(((this_ptr)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2640
	case 18: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2641
	case 19: {
	//AddAndReplaceKind
	_efree(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2642
	case 20: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2643
	case 21: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2644
	case 22: {
	//AddAndReplaceKind
	_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2645
	case 23: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2646
	case 24: {
	//AddAndReplaceKind
	_zval_dtor(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2647
	case 25: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2648
	case 26: {
	//AddAndReplaceKind
	_zval_dtor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2649
	case 27: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2650
	case 28: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2651
	case 29: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2652
	case 30: {
	//AddAndReplaceKind
	add_next_index_stringl((this_ptr), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2653
	case 31: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2654
	case 32: {
	//AddAndReplaceKind
	convert_to_boolean((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2655
	case 33: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2656
	case 34: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2657
	case 35: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2658
	case 36: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2659
	case 37: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2660
	case 38: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2661
	case 39: {
	//AddAndReplaceKind
	gc_zval_possible_root((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2662
	case 40: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2663
	case 41: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2664
	case 42: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2665
	case 43: {
	//AddAndReplaceKind
	if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	    return;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2666
	case 44: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2667
	case 45: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2668
	case 46: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2669
	case 47: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2670
	case 48: {
	//AddAndReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2671
	case 49: {
	//AddAndReplaceKind
	tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2672
	case 50: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2673
	case 51: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2674
	case 52: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2675
	case 53: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2676
	case 54: {
	//AddAndReplaceKind
	zval_unset_isref_p((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2677
	case 55: {
	//AddInitKind
	memset(&language_scanner_globals, 0, sizeof (*(&language_scanner_globals)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2678
	case 56: {
	//IfExitKind
	if (__is_neg("47-56", 37, &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH48"))
	{
	case 0: {
	LANG_SCNG(yy_state) = yycINITIAL;
	
	break;
	}
	#ifdef COMPILE_2679
	case 1: {
	//GuardKind
	if (!__is_neg("48-1", 37, &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(language_scanner_globals.yy_state), sizeof (language_scanner_globals.yy_state), &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding), &(language_scanner_globals.yy_in), sizeof (language_scanner_globals.yy_in), &(language_scanner_globals.yy_out), sizeof (language_scanner_globals.yy_out), &(language_scanner_globals.yy_leng), sizeof (language_scanner_globals.yy_leng), &(language_scanner_globals.yy_start), sizeof (language_scanner_globals.yy_start), &(language_scanner_globals.yy_text), sizeof (language_scanner_globals.yy_text), &(language_scanner_globals.yy_cursor), sizeof (language_scanner_globals.yy_cursor), &(language_scanner_globals.yy_marker), sizeof (language_scanner_globals.yy_marker), &(language_scanner_globals.yy_limit), sizeof (language_scanner_globals.yy_limit), &(language_scanner_globals.script_org), sizeof (language_scanner_globals.script_org), &(language_scanner_globals.script_org_size), sizeof (language_scanner_globals.script_org_size), &(language_scanner_globals.script_filtered), sizeof (language_scanner_globals.script_filtered), &(language_scanner_globals.script_filtered_size), sizeof (language_scanner_globals.script_filtered_size), &(language_scanner_globals.input_filter), sizeof (language_scanner_globals.input_filter), &(language_scanner_globals.output_filter), sizeof (language_scanner_globals.output_filter), &(language_scanner_globals.script_encoding), sizeof (language_scanner_globals.script_encoding)))
	    (language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2680
	case 2: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2681
	case 3: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2682
	case 4: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2683
	case 5: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2684
	case 6: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2685
	case 7: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2686
	case 8: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2687
	case 9: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2688
	case 10: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2689
	case 11: {
	//ReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	}
	}

	//prophet generated patch
	{
	switch(__choose("__SWITCH49"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2690
	case 1: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2691
	case 2: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2692
	case 3: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2693
	case 4: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2694
	case 5: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2695
	case 6: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2696
	case 7: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2697
	case 8: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2698
	case 9: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2699
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2700
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2701
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2702
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2703
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2704
	case 15: {
	//AddAndReplaceKind
	(source)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2705
	case 16: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2706
	case 17: {
	//AddAndReplaceKind
	_array_init(((this_ptr)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2707
	case 18: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2708
	case 19: {
	//AddAndReplaceKind
	_efree(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2709
	case 20: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2710
	case 21: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2711
	case 22: {
	//AddAndReplaceKind
	_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2712
	case 23: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2713
	case 24: {
	//AddAndReplaceKind
	_zval_dtor(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2714
	case 25: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2715
	case 26: {
	//AddAndReplaceKind
	_zval_dtor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2716
	case 27: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2717
	case 28: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2718
	case 29: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2719
	case 30: {
	//AddAndReplaceKind
	add_next_index_stringl((this_ptr), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2720
	case 31: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2721
	case 32: {
	//AddAndReplaceKind
	convert_to_boolean((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2722
	case 33: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2723
	case 34: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2724
	case 35: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2725
	case 36: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2726
	case 37: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2727
	case 38: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2728
	case 39: {
	//AddAndReplaceKind
	gc_zval_possible_root((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2729
	case 40: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2730
	case 41: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2731
	case 42: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2732
	case 43: {
	//AddAndReplaceKind
	if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	    return;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2733
	case 44: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2734
	case 45: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2735
	case 46: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2736
	case 47: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2737
	case 48: {
	//AddAndReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2738
	case 49: {
	//AddAndReplaceKind
	tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2739
	case 50: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2740
	case 51: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2741
	case 52: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2742
	case 53: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2743
	case 54: {
	//AddAndReplaceKind
	zval_unset_isref_p((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2744
	case 55: {
	//IfExitKind
	if (__is_neg("49-55", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH50"))
	{
	case 0: {
	tokenize(return_value TSRMLS_CC);
	
	break;
	}
	#ifdef COMPILE_2745
	case 1: {
	//AddAndReplaceKind
	_convert_to_string(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2746
	case 2: {
	//AddAndReplaceKind
	_object_init(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2747
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2748
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2749
	case 5: {
	//AddAndReplaceKind
	_zval_dtor(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2750
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_func(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2751
	case 7: {
	//AddAndReplaceKind
	_zval_dtor_wrapper(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2752
	case 8: {
	//AddAndReplaceKind
	_zval_internal_dtor(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2753
	case 9: {
	//AddAndReplaceKind
	add_next_index_null(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2754
	case 10: {
	//AddAndReplaceKind
	config_zval_dtor(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2755
	case 11: {
	//AddAndReplaceKind
	convert_scalar_to_number(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2756
	case 12: {
	//AddAndReplaceKind
	convert_to_array(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2757
	case 13: {
	//AddAndReplaceKind
	convert_to_boolean(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2758
	case 14: {
	//AddAndReplaceKind
	convert_to_double(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2759
	case 15: {
	//AddAndReplaceKind
	convert_to_long(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2760
	case 16: {
	//AddAndReplaceKind
	convert_to_null(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2761
	case 17: {
	//AddAndReplaceKind
	convert_to_object(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2762
	case 18: {
	//AddAndReplaceKind
	decrement_function(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2763
	case 19: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2764
	case 20: {
	//AddAndReplaceKind
	gc_zobj_possible_root(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2765
	case 21: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2766
	case 22: {
	//AddAndReplaceKind
	gc_zval_possible_root(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2767
	case 23: {
	//AddAndReplaceKind
	i_zend_is_true(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2768
	case 24: {
	//AddAndReplaceKind
	i_zval_ptr_dtor(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2769
	case 25: {
	//AddAndReplaceKind
	increment_function(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2770
	case 26: {
	//AddAndReplaceKind
	lex_scan(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2771
	case 27: {
	//AddAndReplaceKind
	php_output_get_contents(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2772
	case 28: {
	//AddAndReplaceKind
	php_output_get_length(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2773
	case 29: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2774
	case 30: {
	//AddAndReplaceKind
	zend_is_true(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2775
	case 31: {
	//AddAndReplaceKind
	zend_locale_sprintf_double(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2776
	case 32: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2777
	case 33: {
	//AddAndReplaceKind
	zend_objects_clone_obj(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2778
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_add_ref(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2779
	case 35: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2780
	case 36: {
	//AddAndReplaceKind
	zend_objects_store_del_ref(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2781
	case 37: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2782
	case 38: {
	//AddAndReplaceKind
	zend_print_flat_zval_r(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2783
	case 39: {
	//AddAndReplaceKind
	zend_print_variable(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2784
	case 40: {
	//AddAndReplaceKind
	zend_std_get_constructor(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2785
	case 41: {
	//AddAndReplaceKind
	zend_std_get_properties(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2786
	case 42: {
	//AddAndReplaceKind
	zval_addref_p(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2787
	case 43: {
	//AddAndReplaceKind
	zval_delref_p(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2788
	case 44: {
	//AddAndReplaceKind
	zval_is_true(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2789
	case 45: {
	//AddAndReplaceKind
	zval_isref_p(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2790
	case 46: {
	//AddAndReplaceKind
	zval_refcount_p(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2791
	case 47: {
	//AddAndReplaceKind
	zval_set_isref_p(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2792
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2793
	case 49: {
	//GuardKind
	if (!__is_neg("50-49", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2794
	case 50: {
	//ReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2795
	case 51: {
	//ReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	}
	}
	
	//prophet generated patch
	{
	switch(__choose("__SWITCH51"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2796
	case 1: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2797
	case 2: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2798
	case 3: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2799
	case 4: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2800
	case 5: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2801
	case 6: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2802
	case 7: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2803
	case 8: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2804
	case 9: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2805
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2806
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2807
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2808
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2809
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2810
	case 15: {
	//AddAndReplaceKind
	(source)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2811
	case 16: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2812
	case 17: {
	//AddAndReplaceKind
	_array_init(((this_ptr)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2813
	case 18: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2814
	case 19: {
	//AddAndReplaceKind
	_efree(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2815
	case 20: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2816
	case 21: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2817
	case 22: {
	//AddAndReplaceKind
	_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2818
	case 23: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2819
	case 24: {
	//AddAndReplaceKind
	_zval_dtor(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2820
	case 25: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2821
	case 26: {
	//AddAndReplaceKind
	_zval_dtor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2822
	case 27: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2823
	case 28: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2824
	case 29: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2825
	case 30: {
	//AddAndReplaceKind
	add_next_index_stringl((this_ptr), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2826
	case 31: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2827
	case 32: {
	//AddAndReplaceKind
	convert_to_boolean((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2828
	case 33: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2829
	case 34: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2830
	case 35: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2831
	case 36: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2832
	case 37: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2833
	case 38: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2834
	case 39: {
	//AddAndReplaceKind
	gc_zval_possible_root((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2835
	case 40: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2836
	case 41: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2837
	case 42: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2838
	case 43: {
	//AddAndReplaceKind
	if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	    return;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2839
	case 44: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2840
	case 45: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2841
	case 46: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2842
	case 47: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2843
	case 48: {
	//AddAndReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2844
	case 49: {
	//AddAndReplaceKind
	tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2845
	case 50: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2846
	case 51: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2847
	case 52: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2848
	case 53: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2849
	case 54: {
	//AddAndReplaceKind
	zval_unset_isref_p((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2850
	case 55: {
	//IfExitKind
	if (__is_neg("51-55", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH52"))
	{
	case 0: {
	zend_restore_lexical_state(&original_lex_state TSRMLS_CC);
	
	break;
	}
	#ifdef COMPILE_2851
	case 1: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2852
	case 2: {
	//GuardKind
	if (!__is_neg("52-2", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	}
	}
	//prophet generated patch
	{
	switch(__choose("__SWITCH53"))
	{
	case 0: 
	break;
	#ifdef COMPILE_2853
	case 1: {
	//AddAndReplaceKind
	(compiler_globals.increment_lineno) = 0;
	
	break;
	}
	#endif
	#ifdef COMPILE_2854
	case 2: {
	//AddAndReplaceKind
	(compiler_globals.zend_lineno)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2855
	case 3: {
	//AddAndReplaceKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	
	break;
	}
	#endif
	#ifdef COMPILE_2856
	case 4: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycINITIAL);
	
	break;
	}
	#endif
	#ifdef COMPILE_2857
	case 5: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_BACKQUOTE);
	
	break;
	}
	#endif
	#ifdef COMPILE_2858
	case 6: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_DOUBLE_QUOTES);
	
	break;
	}
	#endif
	#ifdef COMPILE_2859
	case 7: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_END_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2860
	case 8: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_HEREDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2861
	case 9: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_IN_SCRIPTING);
	
	break;
	}
	#endif
	#ifdef COMPILE_2862
	case 10: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_PROPERTY);
	
	break;
	}
	#endif
	#ifdef COMPILE_2863
	case 11: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_LOOKING_FOR_VARNAME);
	
	break;
	}
	#endif
	#ifdef COMPILE_2864
	case 12: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_NOWDOC);
	
	break;
	}
	#endif
	#ifdef COMPILE_2865
	case 13: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = (yycST_VAR_OFFSET);
	
	break;
	}
	#endif
	#ifdef COMPILE_2866
	case 14: {
	//AddAndReplaceKind
	(language_scanner_globals.yy_state) = yycINITIAL;
	
	break;
	}
	#endif
	#ifdef COMPILE_2867
	case 15: {
	//AddAndReplaceKind
	(source)++;
	
	break;
	}
	#endif
	#ifdef COMPILE_2868
	case 16: {
	//AddAndReplaceKind
	_array_init(((return_value)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2869
	case 17: {
	//AddAndReplaceKind
	_array_init(((this_ptr)), 0);
	
	break;
	}
	#endif
	#ifdef COMPILE_2870
	case 18: {
	//AddAndReplaceKind
	_efree(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2871
	case 19: {
	//AddAndReplaceKind
	_efree(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2872
	case 20: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2873
	case 21: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2874
	case 22: {
	//AddAndReplaceKind
	_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2875
	case 23: {
	//AddAndReplaceKind
	_zval_dtor(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2876
	case 24: {
	//AddAndReplaceKind
	_zval_dtor(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2877
	case 25: {
	//AddAndReplaceKind
	_zval_dtor_func((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2878
	case 26: {
	//AddAndReplaceKind
	_zval_dtor_func((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2879
	case 27: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2880
	case 28: {
	//AddAndReplaceKind
	_zval_ptr_dtor((&(this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2881
	case 29: {
	//AddAndReplaceKind
	add_next_index_stringl((return_value), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2882
	case 30: {
	//AddAndReplaceKind
	add_next_index_stringl((this_ptr), (char *)(language_scanner_globals.yy_text), (language_scanner_globals.yy_leng), 1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2883
	case 31: {
	//AddAndReplaceKind
	convert_to_boolean((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2884
	case 32: {
	//AddAndReplaceKind
	convert_to_boolean((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2885
	case 33: {
	//AddAndReplaceKind
	exit(1);
	
	break;
	}
	#endif
	#ifdef COMPILE_2886
	case 34: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2887
	case 35: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2888
	case 36: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((return_value)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2889
	case 37: {
	//AddAndReplaceKind
	gc_zval_check_possible_root(((this_ptr)));
	
	break;
	}
	#endif
	#ifdef COMPILE_2890
	case 38: {
	//AddAndReplaceKind
	gc_zval_possible_root((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2891
	case 39: {
	//AddAndReplaceKind
	gc_zval_possible_root((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2892
	case 40: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2893
	case 41: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2894
	case 42: {
	//AddAndReplaceKind
	if ((language_scanner_globals.yy_text)[(language_scanner_globals.yy_leng) - 1] != '>') {
	    (compiler_globals.zend_lineno)++;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2895
	case 43: {
	//AddAndReplaceKind
	if (zend_parse_parameters(argc, "s", &source, &source_len) == -1) {
	    return;
	}
	
	break;
	}
	#endif
	#ifdef COMPILE_2896
	case 44: {
	//AddAndReplaceKind
	php_info_print_table_end();
	
	break;
	}
	#endif
	#ifdef COMPILE_2897
	case 45: {
	//AddAndReplaceKind
	php_info_print_table_row(2, "Tokenizer Support", "enabled");
	
	break;
	}
	#endif
	#ifdef COMPILE_2898
	case 46: {
	//AddAndReplaceKind
	php_info_print_table_start();
	
	break;
	}
	#endif
	#ifdef COMPILE_2899
	case 47: {
	//AddAndReplaceKind
	tokenize((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2900
	case 48: {
	//AddAndReplaceKind
	tokenize((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2901
	case 49: {
	//AddAndReplaceKind
	tokenize(return_value);
	
	break;
	}
	#endif
	#ifdef COMPILE_2902
	case 50: {
	//AddAndReplaceKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	
	break;
	}
	#endif
	#ifdef COMPILE_2903
	case 51: {
	//AddAndReplaceKind
	zend_restore_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2904
	case 52: {
	//AddAndReplaceKind
	zend_save_lexical_state(&original_lex_state);
	
	break;
	}
	#endif
	#ifdef COMPILE_2905
	case 53: {
	//AddAndReplaceKind
	zval_unset_isref_p((return_value));
	
	break;
	}
	#endif
	#ifdef COMPILE_2906
	case 54: {
	//AddAndReplaceKind
	zval_unset_isref_p((this_ptr));
	
	break;
	}
	#endif
	#ifdef COMPILE_2907
	case 55: {
	//IfExitKind
	if (__is_neg("53-55", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    return;
	
	break;
	}
	#endif
	}
	switch(__choose("__SWITCH54"))
	{
	case 0: {
	zval_dtor(&source_z);
	
	break;
	}
	#ifdef COMPILE_2908
	case 1: {
	//AddAndReplaceKind
	_convert_to_string((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2909
	case 2: {
	//AddAndReplaceKind
	_object_init((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2910
	case 3: {
	//AddAndReplaceKind
	_zval_copy_ctor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2911
	case 4: {
	//AddAndReplaceKind
	_zval_copy_ctor_func((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2912
	case 5: {
	//AddAndReplaceKind
	_zval_dtor_func((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2913
	case 6: {
	//AddAndReplaceKind
	_zval_dtor_wrapper((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2914
	case 7: {
	//AddAndReplaceKind
	_zval_internal_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2915
	case 8: {
	//AddAndReplaceKind
	add_next_index_null((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2916
	case 9: {
	//AddAndReplaceKind
	config_zval_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2917
	case 10: {
	//AddAndReplaceKind
	convert_scalar_to_number((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2918
	case 11: {
	//AddAndReplaceKind
	convert_to_array((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2919
	case 12: {
	//AddAndReplaceKind
	convert_to_boolean((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2920
	case 13: {
	//AddAndReplaceKind
	convert_to_double((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2921
	case 14: {
	//AddAndReplaceKind
	convert_to_long((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2922
	case 15: {
	//AddAndReplaceKind
	convert_to_null((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2923
	case 16: {
	//AddAndReplaceKind
	convert_to_object((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2924
	case 17: {
	//AddAndReplaceKind
	decrement_function((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2925
	case 18: {
	//AddAndReplaceKind
	gc_remove_zval_from_buffer((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2926
	case 19: {
	//AddAndReplaceKind
	gc_zobj_possible_root((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2927
	case 20: {
	//AddAndReplaceKind
	gc_zval_check_possible_root((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2928
	case 21: {
	//AddAndReplaceKind
	gc_zval_possible_root((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2929
	case 22: {
	//AddAndReplaceKind
	i_zend_is_true((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2930
	case 23: {
	//AddAndReplaceKind
	i_zval_ptr_dtor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2931
	case 24: {
	//AddAndReplaceKind
	increment_function((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2932
	case 25: {
	//AddAndReplaceKind
	lex_scan((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2933
	case 26: {
	//AddAndReplaceKind
	php_output_get_contents((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2934
	case 27: {
	//AddAndReplaceKind
	php_output_get_length((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2935
	case 28: {
	//AddAndReplaceKind
	safe_free_zval_ptr_rel((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2936
	case 29: {
	//AddAndReplaceKind
	tokenize((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2937
	case 30: {
	//AddAndReplaceKind
	zend_is_true((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2938
	case 31: {
	//AddAndReplaceKind
	zend_locale_sprintf_double((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2939
	case 32: {
	//AddAndReplaceKind
	zend_object_store_ctor_failed((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2940
	case 33: {
	//AddAndReplaceKind
	zend_objects_clone_obj((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2941
	case 34: {
	//AddAndReplaceKind
	zend_objects_store_add_ref((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2942
	case 35: {
	//AddAndReplaceKind
	zend_objects_store_clone_obj((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2943
	case 36: {
	//AddAndReplaceKind
	zend_objects_store_del_ref((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2944
	case 37: {
	//AddAndReplaceKind
	zend_objects_store_get_refcount((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2945
	case 38: {
	//AddAndReplaceKind
	zend_print_flat_zval_r((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2946
	case 39: {
	//AddAndReplaceKind
	zend_print_variable((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2947
	case 40: {
	//AddAndReplaceKind
	zend_std_get_constructor((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2948
	case 41: {
	//AddAndReplaceKind
	zend_std_get_properties((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2949
	case 42: {
	//AddAndReplaceKind
	zval_addref_p((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2950
	case 43: {
	//AddAndReplaceKind
	zval_delref_p((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2951
	case 44: {
	//AddAndReplaceKind
	zval_is_true((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2952
	case 45: {
	//AddAndReplaceKind
	zval_isref_p((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2953
	case 46: {
	//AddAndReplaceKind
	zval_refcount_p((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2954
	case 47: {
	//AddAndReplaceKind
	zval_set_isref_p((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2955
	case 48: {
	//AddAndReplaceKind
	zval_unset_isref_p((&source_z));
	
	break;
	}
	#endif
	#ifdef COMPILE_2956
	case 49: {
	//GuardKind
	if (!__is_neg("54-49", 5, &(return_value), sizeof (return_value), &(source), sizeof (source), &(source_len), sizeof (source_len), &(argc), sizeof (argc), &(ht), sizeof (ht)))
	    _zval_dtor((&source_z));
	
	break;
	}
	#endif
	}
	}
}
/* }}} */

/* {{{ proto string token_name(int type)
 */
PHP_FUNCTION(token_name)
{
	int argc = ZEND_NUM_ARGS();
	long type;

	if (zend_parse_parameters(argc TSRMLS_CC, "l", &type) == FAILURE) {
		return;
	}
	RETVAL_STRING(get_token_type_name(type), 1);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
