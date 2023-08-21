#define __COMPILE_0
#define __COMPILE_1
#define __COMPILE_2
#define __COMPILE_3
#define __COMPILE_4
#define __COMPILE_5
#define __COMPILE_6
#define __COMPILE_7
#define __COMPILE_8
#define __COMPILE_9
#define __COMPILE_10
#define __COMPILE_11
#define __COMPILE_12
#define __COMPILE_13
#define __COMPILE_14
#define __COMPILE_15
#define __COMPILE_23
#define __COMPILE_24
#define __COMPILE_25
#define __COMPILE_26
#define __COMPILE_27
#define __COMPILE_28
#define __COMPILE_29
#define __COMPILE_30
#define __COMPILE_31
#define __COMPILE_32
#define __COMPILE_33
#define __COMPILE_34
#define __COMPILE_35
#define __COMPILE_36
#define __COMPILE_37
#define __COMPILE_38
#define __COMPILE_39
#define __COMPILE_40
#define __COMPILE_41
#define __COMPILE_42
#define __COMPILE_43
#define __COMPILE_44
#define __COMPILE_45
#define __COMPILE_46
#define __COMPILE_47
#define __COMPILE_48
#define __COMPILE_49
#define __COMPILE_50
#define __COMPILE_51
#define __COMPILE_52
#define __COMPILE_53
#define __COMPILE_54
#define __COMPILE_55
#define __COMPILE_56
#define __COMPILE_57
#define __COMPILE_58
#define __COMPILE_59
#define __COMPILE_60
#define __COMPILE_61
#define __COMPILE_62
#define __COMPILE_63
#define __COMPILE_64
#define __COMPILE_65
#define __COMPILE_66
#define __COMPILE_67
#define __COMPILE_75
#define __COMPILE_76
#define __COMPILE_77
#define __COMPILE_78
#define __COMPILE_79
#define __COMPILE_80
#define __COMPILE_81
#define __COMPILE_82
#define __COMPILE_83
#define __COMPILE_84
#define __COMPILE_85
#define __COMPILE_86
#define __COMPILE_87
#define __COMPILE_88
#define __COMPILE_89
#define __COMPILE_90
#define __COMPILE_91
#define __COMPILE_92
#define __COMPILE_93
#define __COMPILE_94
#define __COMPILE_95
#define __COMPILE_96
#define __COMPILE_97
#define __COMPILE_98
#define __COMPILE_99
#define __COMPILE_100
#define __COMPILE_101
#define __COMPILE_102
#define __COMPILE_103
#define __COMPILE_104
#define __COMPILE_105
#define __COMPILE_106
#define __COMPILE_107
#define __COMPILE_108
#define __COMPILE_109
#define __COMPILE_111
// compile_fin
int __get_mutant(); int __is_neg(const char *location,char *lid,int count, ...); int __abst_hole(); char *__stat_write_init(const char *func_name);void __write_stat(char *str,const char *var_name,void *var_addr,int size); void __stat_file_close(const char *func_name,char *str);long long __mutate(const long long,const char *,const char *); int __trident_choice(char* lid, char* typestr,int* rvals, char** rvals_ids, int rvals_size,int** lvals, char** lvals_ids, int lvals_size);int __trident_output(char* id, char* typestr, int value);float fabs_trident(float a);int __choose(const char *);void *__var_select(unsigned int var_count,void *vars[]);void *__var_select_2(unsigned int var_count,void *vars[]);long long __const_select(unsigned int const_count, ...);void* memset(void*, int, unsigned long); 
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
   | Author: Jim Winstead <jimw@php.net>                                  |
   +----------------------------------------------------------------------+
 */
/* $Id$ */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

#include "php.h"

#include "url.h"
#include "file.h"
#ifdef _OSD_POSIX
#ifndef APACHE
#error On this EBCDIC platform, PHP is only supported as an Apache module.
#else /*APACHE*/
#ifndef CHARSET_EBCDIC
#define CHARSET_EBCDIC /* this machine uses EBCDIC, not ASCII! */
#endif
#include "ebcdic.h"
#endif /*APACHE*/
#endif /*_OSD_POSIX*/

/* {{{ free_url
 */
PHPAPI void php_url_free(php_url *theurl)
{
	if (theurl->scheme)
		efree(theurl->scheme);
	if (theurl->user)
		efree(theurl->user);
	if (theurl->pass)
		efree(theurl->pass);
	if (theurl->host)
		efree(theurl->host);
	if (theurl->path)
		efree(theurl->path);
	//prophet generated patch
	{
	int __choose0 = __choose("__SWITCH0");
	if (__choose0 == 0)
	{}
	#ifdef __COMPILE_0
	else if (__choose0 == 1)
	{
	//AddIfStmtKind
	if (theurl->fragment)
	    _efree((theurl->fragment));
	}
	#endif
	#ifdef __COMPILE_1
	else if (__choose0 == 2)
	{
	//AddIfStmtKind
	if (theurl->host)
	    _efree((theurl->host));
	}
	#endif
	#ifdef __COMPILE_2
	else if (__choose0 == 3)
	{
	//AddIfStmtKind
	if (theurl->pass)
	    _efree((theurl->pass));
	}
	#endif
	#ifdef __COMPILE_3
	else if (__choose0 == 4)
	{
	//AddIfStmtKind
	if (theurl->path)
	    _efree((theurl->path));
	}
	#endif
	#ifdef __COMPILE_4
	else if (__choose0 == 5)
	{
	//AddIfStmtKind
	if (theurl->query)
	    _efree((theurl->query));
	}
	#endif
	#ifdef __COMPILE_5
	else if (__choose0 == 6)
	{
	//AddIfStmtKind
	if (theurl->scheme)
	    _efree((theurl->scheme));
	}
	#endif
	#ifdef __COMPILE_6
	else if (__choose0 == 7)
	{
	//AddIfStmtKind
	if (theurl->user)
	    _efree((theurl->user));
	}
	#endif
	#ifdef __COMPILE_7
	else if (__choose0 == 8)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)));
	}
	#endif
	#ifdef __COMPILE_8
	else if (__choose0 == 9)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->fragment));
	}
	#endif
	#ifdef __COMPILE_9
	else if (__choose0 == 10)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->host));
	}
	#endif
	#ifdef __COMPILE_10
	else if (__choose0 == 11)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->pass));
	}
	#endif
	#ifdef __COMPILE_11
	else if (__choose0 == 12)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->path));
	}
	#endif
	#ifdef __COMPILE_12
	else if (__choose0 == 13)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->query));
	}
	#endif
	#ifdef __COMPILE_13
	else if (__choose0 == 14)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->scheme));
	}
	#endif
	#ifdef __COMPILE_14
	else if (__choose0 == 15)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->user));
	}
	#endif
	#ifdef __COMPILE_15
	else if (__choose0 == 16)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)));
	}
	#endif
	#ifdef __COMPILE_16
	else if (__choose0 == 17)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->fragment));
	}
	#endif
	#ifdef __COMPILE_17
	else if (__choose0 == 18)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->host));
	}
	#endif
	#ifdef __COMPILE_18
	else if (__choose0 == 19)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->pass));
	}
	#endif
	#ifdef __COMPILE_19
	else if (__choose0 == 20)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->path));
	}
	#endif
	#ifdef __COMPILE_20
	else if (__choose0 == 21)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->query));
	}
	#endif
	#ifdef __COMPILE_21
	else if (__choose0 == 22)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->scheme));
	}
	#endif
	#ifdef __COMPILE_22
	else if (__choose0 == 23)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->user));
	}
	#endif
	#ifdef __COMPILE_23
	else if (__choose0 == 24)
	{
	//AddStmtKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	}
	#endif
	#ifdef __COMPILE_24
	else if (__choose0 == 25)
	{
	//AddStmtKind
	_efree((theurl));
	}
	#endif
	#ifdef __COMPILE_25
	else if (__choose0 == 26)
	{
	//AddStmtKind
	_efree((theurl->fragment));
	}
	#endif
	#ifdef __COMPILE_26
	else if (__choose0 == 27)
	{
	//AddStmtKind
	_efree((theurl->host));
	}
	#endif
	#ifdef __COMPILE_27
	else if (__choose0 == 28)
	{
	//AddStmtKind
	_efree((theurl->pass));
	}
	#endif
	#ifdef __COMPILE_28
	else if (__choose0 == 29)
	{
	//AddStmtKind
	_efree((theurl->path));
	}
	#endif
	#ifdef __COMPILE_29
	else if (__choose0 == 30)
	{
	//AddStmtKind
	_efree((theurl->query));
	}
	#endif
	#ifdef __COMPILE_30
	else if (__choose0 == 31)
	{
	//AddStmtKind
	_efree((theurl->scheme));
	}
	#endif
	#ifdef __COMPILE_31
	else if (__choose0 == 32)
	{
	//AddStmtKind
	_efree((theurl->user));
	}
	#endif
	#ifdef __COMPILE_32
	else if (__choose0 == 33)
	{
	//AddStmtKind
	exit(1);
	}
	#endif
	#ifdef __COMPILE_33
	else if (__choose0 == 34)
	{
	//AddStmtKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	}
	#endif
	int __choose1 = __choose("__SWITCH1");
	{
	int __temp1=(theurl->query)
	;
	if (__choose1 == 0)
	{}
	#ifdef __COMPILE_34
	else if (__choose1 == 1)
	{
	__temp1= (__is_neg("1-1", "L55", 9, &(theurl), sizeof (theurl), &(theurl->query), sizeof (theurl->query), &(theurl->path), sizeof (theurl->path), &(theurl->fragment), sizeof (theurl->fragment), &(theurl->host), sizeof (theurl->host), &(theurl->pass), sizeof (theurl->pass), &(theurl->user), sizeof (theurl->user), &(theurl->scheme), sizeof (theurl->scheme), &(theurl->port), sizeof (theurl->port)));
	}
	#endif
	#ifdef __COMPILE_35
	else if (__choose1 == 2)
	{
	__temp1= ((theurl->query) && !__is_neg("1-2", "L55", 9, &(theurl), sizeof (theurl), &(theurl->query), sizeof (theurl->query), &(theurl->path), sizeof (theurl->path), &(theurl->fragment), sizeof (theurl->fragment), &(theurl->host), sizeof (theurl->host), &(theurl->pass), sizeof (theurl->pass), &(theurl->user), sizeof (theurl->user), &(theurl->scheme), sizeof (theurl->scheme), &(theurl->port), sizeof (theurl->port)));
	}
	#endif
	if (__temp1)
			{
	int __choose2 = __choose("__SWITCH2");
	if (__choose2 == 0)
	{}
	#ifdef __COMPILE_36
	else if (__choose2 == 1)
	{
	//AddIfStmtKind
	if (theurl->fragment)
	    _efree((theurl->fragment));
	}
	#endif
	#ifdef __COMPILE_37
	else if (__choose2 == 2)
	{
	//AddIfStmtKind
	if (theurl->host)
	    _efree((theurl->host));
	}
	#endif
	#ifdef __COMPILE_38
	else if (__choose2 == 3)
	{
	//AddIfStmtKind
	if (theurl->pass)
	    _efree((theurl->pass));
	}
	#endif
	#ifdef __COMPILE_39
	else if (__choose2 == 4)
	{
	//AddIfStmtKind
	if (theurl->path)
	    _efree((theurl->path));
	}
	#endif
	#ifdef __COMPILE_40
	else if (__choose2 == 5)
	{
	//AddIfStmtKind
	if (theurl->query)
	    _efree((theurl->query));
	}
	#endif
	#ifdef __COMPILE_41
	else if (__choose2 == 6)
	{
	//AddIfStmtKind
	if (theurl->scheme)
	    _efree((theurl->scheme));
	}
	#endif
	#ifdef __COMPILE_42
	else if (__choose2 == 7)
	{
	//AddIfStmtKind
	if (theurl->user)
	    _efree((theurl->user));
	}
	#endif
	#ifdef __COMPILE_43
	else if (__choose2 == 8)
	{
	//AddInitKind
	memset(&theurl->fragment, 0, sizeof (*(&theurl->fragment)));
	}
	#endif
	#ifdef __COMPILE_44
	else if (__choose2 == 9)
	{
	//AddInitKind
	memset(&theurl->host, 0, sizeof (*(&theurl->host)));
	}
	#endif
	#ifdef __COMPILE_45
	else if (__choose2 == 10)
	{
	//AddInitKind
	memset(&theurl->pass, 0, sizeof (*(&theurl->pass)));
	}
	#endif
	#ifdef __COMPILE_46
	else if (__choose2 == 11)
	{
	//AddInitKind
	memset(&theurl->path, 0, sizeof (*(&theurl->path)));
	}
	#endif
	#ifdef __COMPILE_47
	else if (__choose2 == 12)
	{
	//AddInitKind
	memset(&theurl->port, 0, sizeof (*(&theurl->port)));
	}
	#endif
	#ifdef __COMPILE_48
	else if (__choose2 == 13)
	{
	//AddInitKind
	memset(&theurl->query, 0, sizeof (*(&theurl->query)));
	}
	#endif
	#ifdef __COMPILE_49
	else if (__choose2 == 14)
	{
	//AddInitKind
	memset(&theurl->scheme, 0, sizeof (*(&theurl->scheme)));
	}
	#endif
	#ifdef __COMPILE_50
	else if (__choose2 == 15)
	{
	//AddInitKind
	memset(&theurl->user, 0, sizeof (*(&theurl->user)));
	}
	#endif
	#ifdef __COMPILE_51
	else if (__choose2 == 16)
	{
	//AddInitKind
	memset(theurl, 0, sizeof (*(theurl)));
	}
	#endif
	#ifdef __COMPILE_52
	else if (__choose2 == 17)
	{
	//AddInitKind
	memset(theurl->fragment, 0, sizeof (*(theurl->fragment)));
	}
	#endif
	#ifdef __COMPILE_53
	else if (__choose2 == 18)
	{
	//AddInitKind
	memset(theurl->host, 0, sizeof (*(theurl->host)));
	}
	#endif
	#ifdef __COMPILE_54
	else if (__choose2 == 19)
	{
	//AddInitKind
	memset(theurl->pass, 0, sizeof (*(theurl->pass)));
	}
	#endif
	#ifdef __COMPILE_55
	else if (__choose2 == 20)
	{
	//AddInitKind
	memset(theurl->path, 0, sizeof (*(theurl->path)));
	}
	#endif
	#ifdef __COMPILE_56
	else if (__choose2 == 21)
	{
	//AddInitKind
	memset(theurl->query, 0, sizeof (*(theurl->query)));
	}
	#endif
	#ifdef __COMPILE_57
	else if (__choose2 == 22)
	{
	//AddInitKind
	memset(theurl->scheme, 0, sizeof (*(theurl->scheme)));
	}
	#endif
	#ifdef __COMPILE_58
	else if (__choose2 == 23)
	{
	//AddInitKind
	memset(theurl->user, 0, sizeof (*(theurl->user)));
	}
	#endif
	#ifdef __COMPILE_59
	else if (__choose2 == 24)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)));
	}
	#endif
	#ifdef __COMPILE_60
	else if (__choose2 == 25)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->fragment));
	}
	#endif
	#ifdef __COMPILE_61
	else if (__choose2 == 26)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->host));
	}
	#endif
	#ifdef __COMPILE_62
	else if (__choose2 == 27)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->pass));
	}
	#endif
	#ifdef __COMPILE_63
	else if (__choose2 == 28)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->path));
	}
	#endif
	#ifdef __COMPILE_64
	else if (__choose2 == 29)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->query));
	}
	#endif
	#ifdef __COMPILE_65
	else if (__choose2 == 30)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->scheme));
	}
	#endif
	#ifdef __COMPILE_66
	else if (__choose2 == 31)
	{
	//AddStmtAndReplaceAtomKind
	_efree(((theurl)->user));
	}
	#endif
	#ifdef __COMPILE_67
	else if (__choose2 == 32)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)));
	}
	#endif
	#ifdef __COMPILE_68
	else if (__choose2 == 33)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->fragment));
	}
	#endif
	#ifdef __COMPILE_69
	else if (__choose2 == 34)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->host));
	}
	#endif
	#ifdef __COMPILE_70
	else if (__choose2 == 35)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->pass));
	}
	#endif
	#ifdef __COMPILE_71
	else if (__choose2 == 36)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->path));
	}
	#endif
	#ifdef __COMPILE_72
	else if (__choose2 == 37)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->query));
	}
	#endif
	#ifdef __COMPILE_73
	else if (__choose2 == 38)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->scheme));
	}
	#endif
	#ifdef __COMPILE_74
	else if (__choose2 == 39)
	{
	//AddStmtAndReplaceAtomKind
	_efree((theurl && !(1 << 5)->user));
	}
	#endif
	#ifdef __COMPILE_75
	else if (__choose2 == 40)
	{
	//AddStmtKind
	(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
	}
	#endif
	#ifdef __COMPILE_76
	else if (__choose2 == 41)
	{
	//AddStmtKind
	_efree((theurl));
	}
	#endif
	#ifdef __COMPILE_77
	else if (__choose2 == 42)
	{
	//AddStmtKind
	_efree((theurl->fragment));
	}
	#endif
	#ifdef __COMPILE_78
	else if (__choose2 == 43)
	{
	//AddStmtKind
	_efree((theurl->host));
	}
	#endif
	#ifdef __COMPILE_79
	else if (__choose2 == 44)
	{
	//AddStmtKind
	_efree((theurl->pass));
	}
	#endif
	#ifdef __COMPILE_80
	else if (__choose2 == 45)
	{
	//AddStmtKind
	_efree((theurl->path));
	}
	#endif
	#ifdef __COMPILE_81
	else if (__choose2 == 46)
	{
	//AddStmtKind
	_efree((theurl->query));
	}
	#endif
	#ifdef __COMPILE_82
	else if (__choose2 == 47)
	{
	//AddStmtKind
	_efree((theurl->scheme));
	}
	#endif
	#ifdef __COMPILE_83
	else if (__choose2 == 48)
	{
	//AddStmtKind
	_efree((theurl->user));
	}
	#endif
	#ifdef __COMPILE_84
	else if (__choose2 == 49)
	{
	//AddStmtKind
	exit(1);
	}
	#endif
	#ifdef __COMPILE_85
	else if (__choose2 == 50)
	{
	//AddStmtKind
	zend_error((1 << 3L), "A non well formed numeric value encountered");
	}
	#endif
	#ifdef __COMPILE_86
	else if (__choose2 == 51)
	{
	//IfExitKind
	if (__is_neg("2-51", "L56", 9, &(theurl), sizeof (theurl), &(theurl->query),
				 sizeof (theurl->query), &(theurl->fragment), sizeof (theurl->fragment), &(theurl->path), sizeof (theurl->path), &(theurl->host),
				 sizeof (theurl->host), &(theurl->pass), sizeof (theurl->pass), &(theurl->user), sizeof (theurl->user), &(theurl->scheme),
				 sizeof (theurl->scheme), &(theurl->port), sizeof (theurl->port)))
	    return;
	}
	#endif
	int __choose3 = __choose("__SWITCH3");
	if (__choose3 == 0)
	{
	efree(theurl->query);
	
	}
	#ifdef __COMPILE_87
	else if (__choose3 == 1)
	{
	//GuardKind
	if (!__is_neg("3-1", "L56", 9, &(theurl), sizeof (theurl), &(theurl->query), sizeof (theurl->query), &(theurl->fragment), sizeof (theurl->fragment), &(theurl->path), sizeof (theurl->path), &(theurl->host), sizeof (theurl->host), &(theurl->pass), sizeof (theurl->pass), &(theurl->user), sizeof (theurl->user), &(theurl->scheme), sizeof (theurl->scheme), &(theurl->port), sizeof (theurl->port)))
	    _efree((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_88
	else if (__choose3 == 2)
	{
	//MSVExtFunctionReplaceKind
	__zend_realloc((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_89
	else if (__choose3 == 3)
	{
	//MSVExtFunctionReplaceKind
	bzero((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_90
	else if (__choose3 == 4)
	{
	//MSVExtFunctionReplaceKind
	dlsym((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_91
	else if (__choose3 == 5)
	{
	//MSVExtFunctionReplaceKind
	explicit_bzero((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_92
	else if (__choose3 == 6)
	{
	//MSVExtFunctionReplaceKind
	getentropy((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_93
	else if (__choose3 == 7)
	{
	//MSVExtFunctionReplaceKind
	php_write((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_94
	else if (__choose3 == 8)
	{
	//MSVExtFunctionReplaceKind
	realloc((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_95
	else if (__choose3 == 9)
	{
	//MSVExtFunctionReplaceKind
	timer_gettime((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_96
	else if (__choose3 == 10)
	{
	//MSVExtFunctionReplaceKind
	zend_add_to_list((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_97
	else if (__choose3 == 11)
	{
	//MSVExtFunctionReplaceKind
	zend_init_list((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_98
	else if (__choose3 == 12)
	{
	//MSVExtFunctionReplaceKind
	zend_list_insert((theurl->query), 0);
	
	}
	#endif
	#ifdef __COMPILE_99
	else if (__choose3 == 13)
	{
	//MSVExtRemoveStmtKind
	{
	}
	
	}
	#endif
	#ifdef __COMPILE_100
	else if (__choose3 == 14)
	{
	//ReplaceFunctionKind
	_zend_mem_block_size((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_101
	else if (__choose3 == 15)
	{
	//ReplaceFunctionKind
	brk((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_102
	else if (__choose3 == 16)
	{
	//ReplaceFunctionKind
	dlclose((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_103
	else if (__choose3 == 17)
	{
	//ReplaceFunctionKind
	free((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_104
	else if (__choose3 == 18)
	{
	//ReplaceFunctionKind
	list_entry_destructor((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_105
	else if (__choose3 == 19)
	{
	//ReplaceFunctionKind
	plist_entry_destructor((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_106
	else if (__choose3 == 20)
	{
	//ReplaceFunctionKind
	zend_vm_stack_free((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_107
	else if (__choose3 == 21)
	{
	//ReplaceFunctionKind
	zend_vm_stack_free_int((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_108
	else if (__choose3 == 22)
	{
	//ReplaceFunctionKind
	zend_vm_stack_push((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_109
	else if (__choose3 == 23)
	{
	//ReplaceFunctionKind
	zend_vm_stack_push_nocheck((theurl->query));
	
	}
	#endif
	#ifdef __COMPILE_110
	else if (__choose3 == 24)
	{
	//ReplaceKind
	_efree(((&theurl)->query));
	
	}
	#endif
	#ifdef __COMPILE_111
	else if (__choose3 == 25)
	{
	//ReplaceKind
	_efree(((theurl)->query));
	
	}
	#endif
	#ifdef __COMPILE_112
	else if (__choose3 == 26)
	{
	//ReplaceKind
	_efree((theurl && !(1 << 5)->query));
	
	}
	#endif
	}
	}
	}
	if (theurl->fragment)
		efree(theurl->fragment);
	efree(theurl);
}
/* }}} */

/* {{{ php_replace_controlchars
 */
PHPAPI char *php_replace_controlchars_ex(char *str, int len)
{
	unsigned char *s = (unsigned char *)str;
	unsigned char *e = (unsigned char *)str + len;
	
	if (!str) {
		return (NULL);
	}
	
	while (s < e) {
	    
		if (iscntrl(*s)) {
			*s='_';
		}	
		s++;
	}
	
	return (str);
} 
/* }}} */

PHPAPI char *php_replace_controlchars(char *str)
{
	return php_replace_controlchars_ex(str, strlen(str));
} 

PHPAPI php_url *php_url_parse(char const *str)
{
	return php_url_parse_ex(str, strlen(str));
}

/* {{{ php_url_parse
 */
PHPAPI php_url *php_url_parse_ex(char const *str, int length)
{
	char port_buf[6];
	php_url *ret = ecalloc(1, sizeof(php_url));
	char const *s, *e, *p, *pp, *ue;
		
	s = str;
	ue = s + length;

	/* parse scheme */
	if ((e = memchr(s, ':', length)) && (e - s)) {
		/* validate scheme */
		p = s;
		while (p < e) {
			/* scheme = 1*[ lowalpha | digit | "+" | "-" | "." ] */
			if (!isalpha(*p) && !isdigit(*p) && *p != '+' && *p != '.' && *p != '-') {
				if (e + 1 < ue) {
					goto parse_port;
				} else {
					goto just_path;
				}
			}
			p++;
		}
	
		if (*(e + 1) == '\0') { /* only scheme is available */
			ret->scheme = estrndup(s, (e - s));
			php_replace_controlchars_ex(ret->scheme, (e - s));
			goto end;
		}

		/* 
		 * certain schemas like mailto: and zlib: may not have any / after them
		 * this check ensures we support those.
		 */
		if (*(e+1) != '/') {
			/* check if the data we get is a port this allows us to 
			 * correctly parse things like a.com:80
			 */
			p = e + 1;
			while (isdigit(*p)) {
				p++;
			}
			
			if ((*p == '\0' || *p == '/') && (p - e) < 7) {
				goto parse_port;
			}
			
			ret->scheme = estrndup(s, (e-s));
			php_replace_controlchars_ex(ret->scheme, (e - s));
			
			length -= ++e - s;
			s = e;
			goto just_path;
		} else {
			ret->scheme = estrndup(s, (e-s));
			php_replace_controlchars_ex(ret->scheme, (e - s));
		
			if (*(e+2) == '/') {
				s = e + 3;
				if (!strncasecmp("file", ret->scheme, sizeof("file"))) {
					if (*(e + 3) == '/') {
						/* support windows drive letters as in:
						   file:///c:/somedir/file.txt
						*/
						if (*(e + 5) == ':') {
							s = e + 4;
						}
						goto nohost;
					}
				}
			} else {
				if (!strncasecmp("file", ret->scheme, sizeof("file"))) {
					s = e + 1;
					goto nohost;
				} else {
					length -= ++e - s;
					s = e;
					goto just_path;
				}	
			}
		}	
	} else if (e) { /* no scheme, look for port */
		parse_port:
		p = e + 1;
		pp = p;
		
		while (pp-p < 6 && isdigit(*pp)) {
			pp++;
		}
		
		if (pp-p < 6 && (*pp == '/' || *pp == '\0')) {
			memcpy(port_buf, p, (pp-p));
			port_buf[pp-p] = '\0';
			ret->port = atoi(port_buf);
		} else {
			goto just_path;
		}
	} else {
		just_path:
		ue = s + length;
		goto nohost;
	}
	
	e = ue;
	
	if (!(p = memchr(s, '/', (ue - s)))) {
		char *query, *fragment;

		query = memchr(s, '?', (ue - s));
		fragment = memchr(s, '#', (ue - s));

		if (query && fragment) {
			if (query > fragment) {
				p = e = fragment;
			} else {
				p = e = query;
			}
		} else if (query) {
			p = e = query;
		} else if (fragment) {
			p = e = fragment;
		}
	} else {
		e = p;
	}	
		
	/* check for login and password */
	if ((p = zend_memrchr(s, '@', (e-s)))) {
		if ((pp = memchr(s, ':', (p-s)))) {
			if ((pp-s) > 0) {
				ret->user = estrndup(s, (pp-s));
				php_replace_controlchars_ex(ret->user, (pp - s));
			}	
		
			pp++;
			if (p-pp > 0) {
				ret->pass = estrndup(pp, (p-pp));
				php_replace_controlchars_ex(ret->pass, (p-pp));
			}	
		} else {
			ret->user = estrndup(s, (p-s));
			php_replace_controlchars_ex(ret->user, (p-s));
		}
		
		s = p + 1;
	}

	/* check for port */
	if (*s == '[' && *(e-1) == ']') {
		/* Short circuit portscan, 
		   we're dealing with an 
		   IPv6 embedded address */
		p = s;
	} else {
		/* memrchr is a GNU specific extension
		   Emulate for wide compatability */
		for(p = e; *p != ':' && p >= s; p--);
	}

	if (p >= s && *p == ':') {
		if (!ret->port) {
			p++;
			if (e-p > 5) { /* port cannot be longer then 5 characters */
				STR_FREE(ret->scheme);
				STR_FREE(ret->user);
				STR_FREE(ret->pass);
				efree(ret);
				return NULL;
			} else if (e - p > 0) {
				memcpy(port_buf, p, (e-p));
				port_buf[e-p] = '\0';
				ret->port = atoi(port_buf);
			}
			p--;
		}	
	} else {
		p = e;
	}
	
	/* check if we have a valid host, if we don't reject the string as url */
	if ((p-s) < 1) {
		STR_FREE(ret->scheme);
		STR_FREE(ret->user);
		STR_FREE(ret->pass);
		efree(ret);
		return NULL;
	}

	ret->host = estrndup(s, (p-s));
	php_replace_controlchars_ex(ret->host, (p - s));
	
	if (e == ue) {
		return ret;
	}
	
	s = e;
	
	nohost:
	
	if ((p = memchr(s, '?', (ue - s)))) {
		pp = strchr(s, '#');

		if (pp && pp < p) {
			p = pp;
			goto label_parse;
		}
	
		if (p - s) {
			ret->path = estrndup(s, (p-s));
			php_replace_controlchars_ex(ret->path, (p - s));
		}	
	
		if (pp) {
			if (pp - ++p) { 
				ret->query = estrndup(p, (pp-p));
				php_replace_controlchars_ex(ret->query, (pp - p));
			}
			p = pp;
			goto label_parse;
		} else if (++p - ue) {
			ret->query = estrndup(p, (ue-p));
			php_replace_controlchars_ex(ret->query, (ue - p));
		}
	} else if ((p = memchr(s, '#', (ue - s)))) {
		if (p - s) {
			ret->path = estrndup(s, (p-s));
			php_replace_controlchars_ex(ret->path, (p - s));
		}	
		
		label_parse:
		p++;
		
		if (ue - p) {
			ret->fragment = estrndup(p, (ue-p));
			php_replace_controlchars_ex(ret->fragment, (ue - p));
		}	
	} else {
		ret->path = estrndup(s, (ue-s));
		php_replace_controlchars_ex(ret->path, (ue - s));
	}
end:
	return ret;
}
/* }}} */

/* {{{ proto mixed parse_url(string url, [int url_component])
   Parse a URL and return its components */
PHP_FUNCTION(parse_url)
{
	char *str;
	int str_len;
	php_url *resource;
	long key = -1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &str, &str_len, &key) == FAILURE) {
		return;
	}

	resource = php_url_parse_ex(str, str_len);
	if (resource == NULL) {
		/* @todo Find a method to determine why php_url_parse_ex() failed */
		RETURN_FALSE;
	}

	if (key > -1) {
		switch (key) {
			case PHP_URL_SCHEME:
				if (resource->scheme != NULL) RETVAL_STRING(resource->scheme, 1);
				break;
			case PHP_URL_HOST:
				if (resource->host != NULL) RETVAL_STRING(resource->host, 1);
				break;
			case PHP_URL_PORT:
				if (resource->port != 0) RETVAL_LONG(resource->port);
				break;
			case PHP_URL_USER:
				if (resource->user != NULL) RETVAL_STRING(resource->user, 1);
				break;
			case PHP_URL_PASS:
				if (resource->pass != NULL) RETVAL_STRING(resource->pass, 1);
				break;
			case PHP_URL_PATH:
				if (resource->path != NULL) RETVAL_STRING(resource->path, 1);
				break;
			case PHP_URL_QUERY:
				if (resource->query != NULL) RETVAL_STRING(resource->query, 1);
				break;
			case PHP_URL_FRAGMENT:
				if (resource->fragment != NULL) RETVAL_STRING(resource->fragment, 1);
				break;
			default:
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid URL component identifier %ld", key);
				RETVAL_FALSE;
		}
		goto done;
	}

	/* allocate an array for return */
	array_init(return_value);

    /* add the various elements to the array */
	if (resource->scheme != NULL)
		add_assoc_string(return_value, "scheme", resource->scheme, 1);
	if (resource->host != NULL)
		add_assoc_string(return_value, "host", resource->host, 1);
	if (resource->port != 0)
		add_assoc_long(return_value, "port", resource->port);
	if (resource->user != NULL)
		add_assoc_string(return_value, "user", resource->user, 1);
	if (resource->pass != NULL)
		add_assoc_string(return_value, "pass", resource->pass, 1);
	if (resource->path != NULL)
		add_assoc_string(return_value, "path", resource->path, 1);
	if (resource->query != NULL)
		add_assoc_string(return_value, "query", resource->query, 1);
	if (resource->fragment != NULL)
		add_assoc_string(return_value, "fragment", resource->fragment, 1);
done:	
	php_url_free(resource);
}
/* }}} */

/* {{{ php_htoi
 */
static int php_htoi(char *s)
{
	int value;
	int c;

	c = ((unsigned char *)s)[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = ((unsigned char *)s)[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (value);
}
/* }}} */

/* rfc1738:

   ...The characters ";",
   "/", "?", ":", "@", "=" and "&" are the characters which may be
   reserved for special meaning within a scheme...

   ...Thus, only alphanumerics, the special characters "$-_.+!*'(),", and
   reserved characters used for their reserved purposes may be used
   unencoded within a URL...

   For added safety, we only leave -_. unencoded.
 */

static unsigned char hexchars[] = "0123456789ABCDEF";

/* {{{ php_url_encode
 */
PHPAPI char *php_url_encode(char const *s, int len, int *new_length)
{
	register unsigned char c;
	unsigned char *to, *start;
	unsigned char const *from, *end;
	
	from = (unsigned char *)s;
	end = (unsigned char *)s + len;
	start = to = (unsigned char *) safe_emalloc(3, len, 1);

	while (from < end) {
		c = *from++;

		if (c == ' ') {
			*to++ = '+';
#ifndef CHARSET_EBCDIC
		} else if ((c < '0' && c != '-' && c != '.') ||
				   (c < 'A' && c > '9') ||
				   (c > 'Z' && c < 'a' && c != '_') ||
				   (c > 'z')) {
			to[0] = '%';
			to[1] = hexchars[c >> 4];
			to[2] = hexchars[c & 15];
			to += 3;
#else /*CHARSET_EBCDIC*/
		} else if (!isalnum(c) && strchr("_-.", c) == NULL) {
			/* Allow only alphanumeric chars and '_', '-', '.'; escape the rest */
			to[0] = '%';
			to[1] = hexchars[os_toascii[c] >> 4];
			to[2] = hexchars[os_toascii[c] & 15];
			to += 3;
#endif /*CHARSET_EBCDIC*/
		} else {
			*to++ = c;
		}
	}
	*to = 0;
	if (new_length) {
		*new_length = to - start;
	}
	return (char *) start;
}
/* }}} */

/* {{{ proto string urlencode(string str)
   URL-encodes string */
PHP_FUNCTION(urlencode)
{
	char *in_str, *out_str;
	int in_str_len, out_str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &in_str,
							  &in_str_len) == FAILURE) {
		return;
	}

	out_str = php_url_encode(in_str, in_str_len, &out_str_len);
	RETURN_STRINGL(out_str, out_str_len, 0);
}
/* }}} */

/* {{{ proto string urldecode(string str)
   Decodes URL-encoded string */
PHP_FUNCTION(urldecode)
{
	char *in_str, *out_str;
	int in_str_len, out_str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &in_str,
							  &in_str_len) == FAILURE) {
		return;
	}

	out_str = estrndup(in_str, in_str_len);
	out_str_len = php_url_decode(out_str, in_str_len);

    RETURN_STRINGL(out_str, out_str_len, 0);
}
/* }}} */

/* {{{ php_url_decode
 */
PHPAPI int php_url_decode(char *str, int len)
{
	char *dest = str;
	char *data = str;

	while (len--) {
		if (*data == '+') {
			*dest = ' ';
		}
		else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) 
				 && isxdigit((int) *(data + 2))) {
#ifndef CHARSET_EBCDIC
			*dest = (char) php_htoi(data + 1);
#else
			*dest = os_toebcdic[(char) php_htoi(data + 1)];
#endif
			data += 2;
			len -= 2;
		} else {
			*dest = *data;
		}
		data++;
		dest++;
	}
	*dest = '\0';
	return dest - str;
}
/* }}} */

/* {{{ php_raw_url_encode
 */
PHPAPI char *php_raw_url_encode(char const *s, int len, int *new_length)
{
	register int x, y;
	unsigned char *str;

	str = (unsigned char *) safe_emalloc(3, len, 1);
	for (x = 0, y = 0; len--; x++, y++) {
		str[y] = (unsigned char) s[x];
#ifndef CHARSET_EBCDIC
		if ((str[y] < '0' && str[y] != '-' && str[y] != '.') ||
			(str[y] < 'A' && str[y] > '9') ||
			(str[y] > 'Z' && str[y] < 'a' && str[y] != '_') ||
			(str[y] > 'z' && str[y] != '~')) {
			str[y++] = '%';
			str[y++] = hexchars[(unsigned char) s[x] >> 4];
			str[y] = hexchars[(unsigned char) s[x] & 15];
#else /*CHARSET_EBCDIC*/
		if (!isalnum(str[y]) && strchr("_-.~", str[y]) != NULL) {
			str[y++] = '%';
			str[y++] = hexchars[os_toascii[(unsigned char) s[x]] >> 4];
			str[y] = hexchars[os_toascii[(unsigned char) s[x]] & 15];
#endif /*CHARSET_EBCDIC*/
		}
	}
	str[y] = '\0';
	if (new_length) {
		*new_length = y;
	}
	return ((char *) str);
}
/* }}} */

/* {{{ proto string rawurlencode(string str)
   URL-encodes string */
PHP_FUNCTION(rawurlencode)
{
	char *in_str, *out_str;
	int in_str_len, out_str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &in_str,
							  &in_str_len) == FAILURE) {
		return;
	}

	out_str = php_raw_url_encode(in_str, in_str_len, &out_str_len);
	RETURN_STRINGL(out_str, out_str_len, 0);
}
/* }}} */

/* {{{ proto string rawurldecode(string str)
   Decodes URL-encodes string */
PHP_FUNCTION(rawurldecode)
{
	char *in_str, *out_str;
	int in_str_len, out_str_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &in_str,
							  &in_str_len) == FAILURE) {
		return;
	}

	out_str = estrndup(in_str, in_str_len);
	out_str_len = php_raw_url_decode(out_str, in_str_len);

    RETURN_STRINGL(out_str, out_str_len, 0);
}
/* }}} */

/* {{{ php_raw_url_decode
 */
PHPAPI int php_raw_url_decode(char *str, int len)
{
	char *dest = str;
	char *data = str;

	while (len--) {
		if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) 
			&& isxdigit((int) *(data + 2))) {
#ifndef CHARSET_EBCDIC
			*dest = (char) php_htoi(data + 1);
#else
			*dest = os_toebcdic[(char) php_htoi(data + 1)];
#endif
			data += 2;
			len -= 2;
		} else {
			*dest = *data;
		}
		data++;
		dest++;
	}
	*dest = '\0';
	return dest - str;
}
/* }}} */

/* {{{ proto array get_headers(string url[, int format])
   fetches all the headers sent by the server in response to a HTTP request */
PHP_FUNCTION(get_headers)
{
	char *url;
	int url_len;
	php_stream_context *context;
	php_stream *stream;
	zval **prev_val, **hdr = NULL, **h;
	HashPosition pos;
	HashTable *hashT;
	long format = 0;
                
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &url, &url_len, &format) == FAILURE) {
		return;
	}
	context = FG(default_context) ? FG(default_context) : (FG(default_context) = php_stream_context_alloc(TSRMLS_C));

	if (!(stream = php_stream_open_wrapper_ex(url, "r", REPORT_ERRORS | STREAM_USE_URL | STREAM_ONLY_GET_HEADERS, NULL, context))) {
		RETURN_FALSE;
	}

	if (!stream->wrapperdata || Z_TYPE_P(stream->wrapperdata) != IS_ARRAY) {
		php_stream_close(stream);
		RETURN_FALSE;
	}

	array_init(return_value);

	/* check for curl-wrappers that provide headers via a special "headers" element */
	if (zend_hash_find(HASH_OF(stream->wrapperdata), "headers", sizeof("headers"), (void **)&h) != FAILURE && Z_TYPE_PP(h) == IS_ARRAY) {
		/* curl-wrappers don't load data until the 1st read */ 
		if (!Z_ARRVAL_PP(h)->nNumOfElements) {
			php_stream_getc(stream);
		}
		zend_hash_find(HASH_OF(stream->wrapperdata), "headers", sizeof("headers"), (void **)&h);
		hashT = Z_ARRVAL_PP(h);	
	} else {
		hashT = HASH_OF(stream->wrapperdata);
	}

	zend_hash_internal_pointer_reset_ex(hashT, &pos);
	while (zend_hash_get_current_data_ex(hashT, (void**)&hdr, &pos) != FAILURE) {
		if (!hdr || Z_TYPE_PP(hdr) != IS_STRING) {
			zend_hash_move_forward_ex(hashT, &pos);
			continue;
		}
		if (!format) {
no_name_header:
			add_next_index_stringl(return_value, Z_STRVAL_PP(hdr), Z_STRLEN_PP(hdr), 1);
		} else {
			char c;
			char *s, *p;

			if ((p = strchr(Z_STRVAL_PP(hdr), ':'))) {
				c = *p;
				*p = '\0';
				s = p + 1;
				while (isspace((int)*(unsigned char *)s)) {
					s++;
				}

				if (zend_hash_find(HASH_OF(return_value), Z_STRVAL_PP(hdr), (p - Z_STRVAL_PP(hdr) + 1), (void **) &prev_val) == FAILURE) {
					add_assoc_stringl_ex(return_value, Z_STRVAL_PP(hdr), (p - Z_STRVAL_PP(hdr) + 1), s, (Z_STRLEN_PP(hdr) - (s - Z_STRVAL_PP(hdr))), 1);
				} else { /* some headers may occur more then once, therefor we need to remake the string into an array */
					convert_to_array(*prev_val);
					add_next_index_stringl(*prev_val, s, (Z_STRLEN_PP(hdr) - (s - Z_STRVAL_PP(hdr))), 1);
				}

				*p = c;
			} else {
				goto no_name_header;
			}
		}
		zend_hash_move_forward_ex(hashT, &pos);
	}

	php_stream_close(stream);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
