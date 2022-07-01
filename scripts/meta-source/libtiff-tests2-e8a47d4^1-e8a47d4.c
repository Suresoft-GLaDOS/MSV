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
#define __COMPILE_16
#define __COMPILE_17
#define __COMPILE_18
#define __COMPILE_19
#define __COMPILE_20
#define __COMPILE_21
#define __COMPILE_22
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
#define __COMPILE_68
#define __COMPILE_69
#define __COMPILE_70
#define __COMPILE_71
#define __COMPILE_72
#define __COMPILE_73
#define __COMPILE_74
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
#define __COMPILE_109
#define __COMPILE_110
#define __COMPILE_111
#define __COMPILE_112
#define __COMPILE_113
#define __COMPILE_114
#define __COMPILE_115
#define __COMPILE_116
#define __COMPILE_117
#define __COMPILE_118
#define __COMPILE_119
#define __COMPILE_120
#define __COMPILE_121
#define __COMPILE_122
#define __COMPILE_123
#define __COMPILE_124
#define __COMPILE_125
#define __COMPILE_126
#define __COMPILE_127
#define __COMPILE_128
#define __COMPILE_129
#define __COMPILE_130
#define __COMPILE_131
#define __COMPILE_132
#define __COMPILE_133
#define __COMPILE_134
#define __COMPILE_135
#define __COMPILE_136
#define __COMPILE_137
#define __COMPILE_138
#define __COMPILE_139
#define __COMPILE_140
#define __COMPILE_141
#define __COMPILE_142
#define __COMPILE_143
#define __COMPILE_144
#define __COMPILE_145
#define __COMPILE_146
#define __COMPILE_147
#define __COMPILE_148
#define __COMPILE_149
#define __COMPILE_150
#define __COMPILE_151
#define __COMPILE_152
#define __COMPILE_153
#define __COMPILE_154
#define __COMPILE_155
#define __COMPILE_156
#define __COMPILE_157
#define __COMPILE_158
#define __COMPILE_159
#define __COMPILE_160
#define __COMPILE_161
#define __COMPILE_162
#define __COMPILE_163
#define __COMPILE_164
#define __COMPILE_165
#define __COMPILE_166
#define __COMPILE_167
#define __COMPILE_168
#define __COMPILE_169
#define __COMPILE_170
#define __COMPILE_171
#define __COMPILE_172
#define __COMPILE_173
#define __COMPILE_174
#define __COMPILE_175
#define __COMPILE_176
#define __COMPILE_177
#define __COMPILE_178
#define __COMPILE_179
#define __COMPILE_180
#define __COMPILE_181
#define __COMPILE_182
#define __COMPILE_183
// compile_fin
int __get_mutant(); int __is_neg(const char *location,char *lid,int count, ...); int __abst_hole(); char *__stat_write_init(const char *func_name);void __write_stat(char *str,const char *var_name,void *var_addr,int size); void __stat_file_close(const char *func_name,char *str);long long __mutate(const long long,const char *,const char *); int __trident_choice(char* lid, char* typestr,int* rvals, char** rvals_ids, int rvals_size,int** lvals, char** lvals_ids, int lvals_size);int __trident_output(char* id, char* typestr, int value);float fabs_trident(float a);int __choose(const char *);void *__var_select(unsigned int var_count,void *vars[]);void *__var_select_2(unsigned int var_count,void *vars[]);long long __const_select(unsigned int const_count, ...);void* memset(void*, int, unsigned long); 
/* $Id$ */

/*
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#include "tif_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef NEED_LIBPORT
# include "libport.h"
#endif

#include "tiffiop.h"

static TIFFErrorHandler old_error_handler = 0;
static int status = 0;                  /* exit status */
static int showdata = 0;		/* show data */
static int rawdata = 0;			/* show raw/decoded data */
static int showwords = 0;		/* show data as bytes/words */
static int readdata = 0;		/* read data in file */
static int stoponerr = 1;		/* stop on first read error */

static	void usage(void);
static	void tiffinfo(TIFF*, uint16, long);

static void
PrivateErrorHandler(const char* module, const char* fmt, va_list ap)
{
        //prophet generated patch
        {
        int __choose0 = __choose("__SWITCH0");
        if (__choose0 == 0)
        {}
        #ifdef __COMPILE_0
        else if (__choose0 == 1)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)((fmt), fmt, ap);
        }
        #endif
        #ifdef __COMPILE_1
        else if (__choose0 == 2)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)((module), fmt, ap);
        }
        #endif
        #ifdef __COMPILE_2
        else if (__choose0 == 3)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, (fmt), ap);
        }
        #endif
        #ifdef __COMPILE_3
        else if (__choose0 == 4)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, (module), ap);
        }
        #endif
        #ifdef __COMPILE_4
        else if (__choose0 == 5)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, fmt, (ap));
        }
        #endif
        #ifdef __COMPILE_5
        else if (__choose0 == 6)
        {
        //AddStmtAndReplaceAtomKind
        status = (0);
        }
        #endif
        #ifdef __COMPILE_6
        else if (__choose0 == 7)
        {
        //AddStmtAndReplaceAtomKind
        status = (1);
        }
        #endif
        #ifdef __COMPILE_7
        else if (__choose0 == 8)
        {
        //AddStmtKind
        (*old_error_handler)(module, fmt, ap);
        }
        #endif
        #ifdef __COMPILE_8
        else if (__choose0 == 9)
        {
        //AddStmtKind
        (void)TIFFSetErrorHandler(PrivateErrorHandler);
        }
        #endif
        #ifdef __COMPILE_9
        else if (__choose0 == 10)
        {
        //AddStmtKind
        exit(-1);
        }
        #endif
        #ifdef __COMPILE_10
        else if (__choose0 == 11)
        {
        //AddStmtKind
        old_error_handler = _TIFFerrorHandler;
        }
        #endif
        #ifdef __COMPILE_11
        else if (__choose0 == 12)
        {
        //AddStmtKind
        printf("):\n");
        }
        #endif
        #ifdef __COMPILE_12
        else if (__choose0 == 13)
        {
        //AddStmtKind
        printf("\n ");
        }
        #endif
        #ifdef __COMPILE_13
        else if (__choose0 == 14)
        {
        //AddStmtKind
        putchar('\n');
        }
        #endif
        #ifdef __COMPILE_14
        else if (__choose0 == 15)
        {
        //AddStmtKind
        status = 1;
        }
        #endif
        #ifdef __COMPILE_15
        else if (__choose0 == 16)
        {
        //AddStmtKind
        usage();
        }
        #endif
        #ifdef __COMPILE_16
        else if (__choose0 == 17)
        {
        //IfExitKind
        if (__is_neg("0-17", "L62", 5, &(old_error_handler), sizeof (old_error_handler), &(module),
        			 sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(status),
        			 sizeof (status)))
            return;
        }
        #endif
        int __choose1 = __choose("__SWITCH1");
        {
        int __temp1=(old_error_handler)
        ;
        if (__choose1 == 0)
        {}
        #ifdef __COMPILE_17
        else if (__choose1 == 1)
        {
        __temp1= (__is_neg("1-1", "L62", 5, &(old_error_handler), sizeof (old_error_handler), &(module), sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(status), sizeof (status)));
        }
        #endif
        #ifdef __COMPILE_18
        else if (__choose1 == 2)
        {
        __temp1= ((old_error_handler) || __is_neg("1-2", "L62", 5, &(old_error_handler), sizeof (old_error_handler), &(module), sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(status), sizeof (status)));
        }
        #endif
        #ifdef __COMPILE_19
        else if (__choose1 == 3)
        {
        __temp1= ((old_error_handler) && !__is_neg("1-3", "L62", 5, &(old_error_handler), sizeof (old_error_handler), &(module), sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(status), sizeof (status)));
        }
        #endif
        int __choose4 = __choose("__SWITCH4");
        if (__choose4 == 0)
        {
        if (__temp1)
                        {
        int __choose2 = __choose("__SWITCH2");
        if (__choose2 == 0)
        {}
        #ifdef __COMPILE_20
        else if (__choose2 == 1)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)((fmt), fmt, ap);
        }
        #endif
        #ifdef __COMPILE_21
        else if (__choose2 == 2)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)((module), fmt, ap);
        }
        #endif
        #ifdef __COMPILE_22
        else if (__choose2 == 3)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, (fmt), ap);
        }
        #endif
        #ifdef __COMPILE_23
        else if (__choose2 == 4)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, (module), ap);
        }
        #endif
        #ifdef __COMPILE_24
        else if (__choose2 == 5)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, fmt, (ap));
        }
        #endif
        #ifdef __COMPILE_25
        else if (__choose2 == 6)
        {
        //AddStmtAndReplaceAtomKind
        status = (0);
        }
        #endif
        #ifdef __COMPILE_26
        else if (__choose2 == 7)
        {
        //AddStmtAndReplaceAtomKind
        status = (1);
        }
        #endif
        #ifdef __COMPILE_27
        else if (__choose2 == 8)
        {
        //AddStmtKind
        (*old_error_handler)(module, fmt, ap);
        }
        #endif
        #ifdef __COMPILE_28
        else if (__choose2 == 9)
        {
        //AddStmtKind
        (void)TIFFSetErrorHandler(PrivateErrorHandler);
        }
        #endif
        #ifdef __COMPILE_29
        else if (__choose2 == 10)
        {
        //AddStmtKind
        exit(-1);
        }
        #endif
        #ifdef __COMPILE_30
        else if (__choose2 == 11)
        {
        //AddStmtKind
        old_error_handler = _TIFFerrorHandler;
        }
        #endif
        #ifdef __COMPILE_31
        else if (__choose2 == 12)
        {
        //AddStmtKind
        printf("):\n");
        }
        #endif
        #ifdef __COMPILE_32
        else if (__choose2 == 13)
        {
        //AddStmtKind
        printf("\n ");
        }
        #endif
        #ifdef __COMPILE_33
        else if (__choose2 == 14)
        {
        //AddStmtKind
        putchar('\n');
        }
        #endif
        #ifdef __COMPILE_34
        else if (__choose2 == 15)
        {
        //AddStmtKind
        status = 1;
        }
        #endif
        #ifdef __COMPILE_35
        else if (__choose2 == 16)
        {
        //AddStmtKind
        usage();
        }
        #endif
        #ifdef __COMPILE_36
        else if (__choose2 == 17)
        {
        //IfExitKind
        if (__is_neg("2-17", "L63", 5, &(module), sizeof (module), &(fmt),
        			 sizeof (fmt), &(ap), sizeof (ap), &(old_error_handler), sizeof (old_error_handler), &(status),
        			 sizeof (status)))
            return;
        }
        #endif
        int __choose3 = __choose("__SWITCH3");
        if (__choose3 == 0)
        {
        (*old_error_handler)(module,fmt,ap);
        
        }
        #ifdef __COMPILE_37
        else if (__choose3 == 1)
        {
        //GuardKind
        if (!__is_neg("3-1", "L63", 5, &(module), sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(old_error_handler), sizeof (old_error_handler), &(status), sizeof (status)))
            (*old_error_handler)(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_38
        else if (__choose3 == 2)
        {
        //MSVExtRemoveStmtKind
        {
        }
        
        }
        #endif
        #ifdef __COMPILE_39
        else if (__choose3 == 3)
        {
        //ReplaceFunctionKind
        PrivateErrorHandler(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_40
        else if (__choose3 == 4)
        {
        //ReplaceFunctionKind
        TIFFError(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_41
        else if (__choose3 == 5)
        {
        //ReplaceFunctionKind
        TIFFWarning(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_42
        else if (__choose3 == 6)
        {
        //ReplaceFunctionKind
        _IO_vfprintf(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_43
        else if (__choose3 == 7)
        {
        //ReplaceFunctionKind
        execl(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_44
        else if (__choose3 == 8)
        {
        //ReplaceFunctionKind
        execle(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_45
        else if (__choose3 == 9)
        {
        //ReplaceFunctionKind
        execlp(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_46
        else if (__choose3 == 10)
        {
        //ReplaceFunctionKind
        fprintf(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_47
        else if (__choose3 == 11)
        {
        //ReplaceFunctionKind
        freopen(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_48
        else if (__choose3 == 12)
        {
        //ReplaceFunctionKind
        printf(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_49
        else if (__choose3 == 13)
        {
        //ReplaceFunctionKind
        sprintf(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_50
        else if (__choose3 == 14)
        {
        //ReplaceFunctionKind
        strcasecmp_l(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_51
        else if (__choose3 == 15)
        {
        //ReplaceFunctionKind
        strcoll_l(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_52
        else if (__choose3 == 16)
        {
        //ReplaceFunctionKind
        vfprintf(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_53
        else if (__choose3 == 17)
        {
        //ReplaceFunctionKind
        vsprintf(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_54
        else if (__choose3 == 18)
        {
        //ReplaceKind
        (*(&ap))(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_55
        else if (__choose3 == 19)
        {
        //ReplaceKind
        (*(&fmt))(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_56
        else if (__choose3 == 20)
        {
        //ReplaceKind
        (*(&module))(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_57
        else if (__choose3 == 21)
        {
        //ReplaceKind
        (*old_error_handler)((&ap), fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_58
        else if (__choose3 == 22)
        {
        //ReplaceKind
        (*old_error_handler)((&fmt), fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_59
        else if (__choose3 == 23)
        {
        //ReplaceKind
        (*old_error_handler)((&module), fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_60
        else if (__choose3 == 24)
        {
        //ReplaceKind
        (*old_error_handler)((fmt), fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_61
        else if (__choose3 == 25)
        {
        //ReplaceKind
        (*old_error_handler)((module), fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_62
        else if (__choose3 == 26)
        {
        //ReplaceKind
        (*old_error_handler)(module, (&ap), ap);
        
        }
        #endif
        #ifdef __COMPILE_63
        else if (__choose3 == 27)
        {
        //ReplaceKind
        (*old_error_handler)(module, (&fmt), ap);
        
        }
        #endif
        #ifdef __COMPILE_64
        else if (__choose3 == 28)
        {
        //ReplaceKind
        (*old_error_handler)(module, (&module), ap);
        
        }
        #endif
        #ifdef __COMPILE_65
        else if (__choose3 == 29)
        {
        //ReplaceKind
        (*old_error_handler)(module, (fmt), ap);
        
        }
        #endif
        #ifdef __COMPILE_66
        else if (__choose3 == 30)
        {
        //ReplaceKind
        (*old_error_handler)(module, (module), ap);
        
        }
        #endif
        #ifdef __COMPILE_67
        else if (__choose3 == 31)
        {
        //ReplaceKind
        (*old_error_handler)(module, fmt, (&ap));
        
        }
        #endif
        #ifdef __COMPILE_68
        else if (__choose3 == 32)
        {
        //ReplaceKind
        (*old_error_handler)(module, fmt, (&fmt));
        
        }
        #endif
        #ifdef __COMPILE_69
        else if (__choose3 == 33)
        {
        //ReplaceKind
        (*old_error_handler)(module, fmt, (&module));
        
        }
        #endif
        #ifdef __COMPILE_70
        else if (__choose3 == 34)
        {
        //ReplaceKind
        (*old_error_handler)(module, fmt, (ap));
        
        }
        #endif
        }
        
        }
        #ifdef __COMPILE_71
        else if (__choose4 == 1)
        {
        //GuardKind
        if (!__is_neg("4-1", "L62", 5, &(old_error_handler), sizeof (old_error_handler), &(module), sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(status), sizeof (status)))
            if (old_error_handler)
                (*old_error_handler)(module, fmt, ap);
        
        }
        #endif
        #ifdef __COMPILE_72
        else if (__choose4 == 2)
        {
        //SpecialGuardKind
        if (!__is_neg("4-2", "L62", 5, &(old_error_handler), sizeof (old_error_handler), &(module), sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(status), sizeof (status)) && (old_error_handler))
            (*old_error_handler)(module, fmt, ap);
        
        }
        #endif
        }
        }
	//prophet generated patch
        {
        int __choose5 = __choose("__SWITCH5");
        if (__choose5 == 0)
        {}
        #ifdef __COMPILE_73
        else if (__choose5 == 1)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)((fmt), fmt, ap);
        }
        #endif
        #ifdef __COMPILE_74
        else if (__choose5 == 2)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)((module), fmt, ap);
        }
        #endif
        #ifdef __COMPILE_75
        else if (__choose5 == 3)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, (fmt), ap);
        }
        #endif
        #ifdef __COMPILE_76
        else if (__choose5 == 4)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, (module), ap);
        }
        #endif
        #ifdef __COMPILE_77
        else if (__choose5 == 5)
        {
        //AddStmtAndReplaceAtomKind
        (*old_error_handler)(module, fmt, (ap));
        }
        #endif
        #ifdef __COMPILE_78
        else if (__choose5 == 6)
        {
        //AddStmtAndReplaceAtomKind
        status = (0);
        }
        #endif
        #ifdef __COMPILE_79
        else if (__choose5 == 7)
        {
        //AddStmtAndReplaceAtomKind
        status = (1);
        }
        #endif
        #ifdef __COMPILE_80
        else if (__choose5 == 8)
        {
        //AddStmtKind
        (*old_error_handler)(module, fmt, ap);
        }
        #endif
        #ifdef __COMPILE_81
        else if (__choose5 == 9)
        {
        //AddStmtKind
        (void)TIFFSetErrorHandler(PrivateErrorHandler);
        }
        #endif
        #ifdef __COMPILE_82
        else if (__choose5 == 10)
        {
        //AddStmtKind
        exit(-1);
        }
        #endif
        #ifdef __COMPILE_83
        else if (__choose5 == 11)
        {
        //AddStmtKind
        old_error_handler = _TIFFerrorHandler;
        }
        #endif
        #ifdef __COMPILE_84
        else if (__choose5 == 12)
        {
        //AddStmtKind
        printf("):\n");
        }
        #endif
        #ifdef __COMPILE_85
        else if (__choose5 == 13)
        {
        //AddStmtKind
        printf("\n ");
        }
        #endif
        #ifdef __COMPILE_86
        else if (__choose5 == 14)
        {
        //AddStmtKind
        putchar('\n');
        }
        #endif
        #ifdef __COMPILE_87
        else if (__choose5 == 15)
        {
        //AddStmtKind
        status = 1;
        }
        #endif
        #ifdef __COMPILE_88
        else if (__choose5 == 16)
        {
        //AddStmtKind
        usage();
        }
        #endif
        #ifdef __COMPILE_89
        else if (__choose5 == 17)
        {
        //IfExitKind
        if (__is_neg("5-17", "L64", 5, &(status), sizeof (status), &(module),
        			 sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(old_error_handler),
        			 sizeof (old_error_handler)))
            return;
        }
        #endif
        int __choose6 = __choose("__SWITCH6");
        if (__choose6 == 0)
        {
        status = 1;
        
        }
        #ifdef __COMPILE_90
        else if (__choose6 == 1)
        {
        //GuardKind
        if (!__is_neg("6-1", "L64", 5, &(status), sizeof (status), &(module), sizeof (module), &(fmt), sizeof (fmt), &(ap), sizeof (ap), &(old_error_handler), sizeof (old_error_handler)))
            status = 1;
        
        }
        #endif
        #ifdef __COMPILE_91
        else if (__choose6 == 2)
        {
        //MSVExtRemoveStmtKind
        {
        }
        
        }
        #endif
        #ifdef __COMPILE_92
        else if (__choose6 == 3)
        {
        //ReplaceKind
        status = (0);
        
        }
        #endif
        #ifdef __COMPILE_93
        else if (__choose6 == 4)
        {
        //ReplaceKind
        status = (1);
        
        }
        #endif
        #ifdef __COMPILE_94
        else if (__choose6 == 5)
        {
        //ReplaceKind
        status = (ap != 0);
        
        }
        #endif
        #ifdef __COMPILE_95
        else if (__choose6 == 6)
        {
        //ReplaceKind
        status = (ap == 0);
        
        }
        #endif
        #ifdef __COMPILE_96
        else if (__choose6 == 7)
        {
        //ReplaceKind
        status = (fmt != 0);
        
        }
        #endif
        #ifdef __COMPILE_97
        else if (__choose6 == 8)
        {
        //ReplaceKind
        status = (fmt == 0);
        
        }
        #endif
        #ifdef __COMPILE_98
        else if (__choose6 == 9)
        {
        //ReplaceKind
        status = (module != 0);
        
        }
        #endif
        #ifdef __COMPILE_99
        else if (__choose6 == 10)
        {
        //ReplaceKind
        status = (module != fmt);
        
        }
        #endif
        #ifdef __COMPILE_100
        else if (__choose6 == 11)
        {
        //ReplaceKind
        status = (module == 0);
        
        }
        #endif
        #ifdef __COMPILE_101
        else if (__choose6 == 12)
        {
        //ReplaceKind
        status = (module == fmt);
        
        }
        #endif
        }
}

int
main(int argc, char* argv[])
{
	int dirnum = -1, multiplefiles, c;
	uint16 order = 0;
	TIFF* tif;
	extern int optind;
	extern char* optarg;
	long flags = 0;
	uint64 diroff = 0;
	int chopstrips = 0;		/* disable strip chopping */

	while ((c = getopt(argc, argv, "f:o:cdDSjilmrsvwz0123456789")) != -1)
		switch (c) {
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':
			dirnum = atoi(&argv[optind-1][1]);
			break;
		case 'd':
			showdata++;
			/* fall thru... */
		case 'D':
			readdata++;
			break;
		case 'c':
			flags |= TIFFPRINT_COLORMAP | TIFFPRINT_CURVES;
			break;
		case 'f':		/* fill order */
			if (streq(optarg, "lsb2msb"))
				order = FILLORDER_LSB2MSB;
			else if (streq(optarg, "msb2lsb"))
				order = FILLORDER_MSB2LSB;
			else
				usage();
			break;
		case 'i':
			stoponerr = 0;
			break;
		case 'o':
			diroff = strtoul(optarg, NULL, 0);
			break;
		case 'j':
			flags |= TIFFPRINT_JPEGQTABLES |
				 TIFFPRINT_JPEGACTABLES |
				 TIFFPRINT_JPEGDCTABLES;
			break;
		case 'r':
			rawdata = 1;
			break;
		case 's':
			flags |= TIFFPRINT_STRIPS;
			break;
		case 'w':
			showwords = 1;
			break;
		case 'z':
			chopstrips = 1;
			break;
		case '?':
			usage();
			/*NOTREACHED*/
		}
	if (optind >= argc)
		usage();

	old_error_handler = _TIFFerrorHandler;
	//prophet generated patch
	{
	int __choose7 = __choose("__SWITCH7");
	if (__choose7 == 0)
	{}
	#ifdef __COMPILE_102
	else if (__choose7 == 1)
	{
	//AddStmtAndReplaceAtomKind
	TIFFClose((tif));
	}
	#endif
	#ifdef __COMPILE_103
	else if (__choose7 == 2)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadContigStripData((tif));
	}
	#endif
	#ifdef __COMPILE_104
	else if (__choose7 == 3)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadContigTileData((tif));
	}
	#endif
	#ifdef __COMPILE_105
	else if (__choose7 == 4)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadData((tif));
	}
	#endif
	#ifdef __COMPILE_106
	else if (__choose7 == 5)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadRawData((tif), 0);
	}
	#endif
	#ifdef __COMPILE_107
	else if (__choose7 == 6)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadSeparateStripData((tif));
	}
	#endif
	#ifdef __COMPILE_108
	else if (__choose7 == 7)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadSeparateTileData((tif));
	}
	#endif
	#ifdef __COMPILE_109
	else if (__choose7 == 8)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (-1);
	}
	#endif
	#ifdef __COMPILE_110
	else if (__choose7 == 9)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (0);
	}
	#endif
	#ifdef __COMPILE_111
	else if (__choose7 == 10)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (1);
	}
	#endif
	#ifdef __COMPILE_112
	else if (__choose7 == 11)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (2);
	}
	#endif
	#ifdef __COMPILE_113
	else if (__choose7 == 12)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (256);
	}
	#endif
	#ifdef __COMPILE_114
	else if (__choose7 == 13)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (34665);
	}
	#endif
	#ifdef __COMPILE_115
	else if (__choose7 == 14)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (4);
	}
	#endif
	#ifdef __COMPILE_116
	else if (__choose7 == 15)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (512);
	}
	#endif
	#ifdef __COMPILE_117
	else if (__choose7 == 16)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (6);
	}
	#endif
	#ifdef __COMPILE_118
	else if (__choose7 == 17)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (768);
	}
	#endif
	#ifdef __COMPILE_119
	else if (__choose7 == 18)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (argc);
	}
	#endif
	#ifdef __COMPILE_120
	else if (__choose7 == 19)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (c);
	}
	#endif
	#ifdef __COMPILE_121
	else if (__choose7 == 20)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (chopstrips);
	}
	#endif
	#ifdef __COMPILE_122
	else if (__choose7 == 21)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (dirnum);
	}
	#endif
	#ifdef __COMPILE_123
	else if (__choose7 == 22)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (multiplefiles);
	}
	#endif
	#ifdef __COMPILE_124
	else if (__choose7 == 23)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (optind);
	}
	#endif
	#ifdef __COMPILE_125
	else if (__choose7 == 24)
	{
	//AddStmtAndReplaceAtomKind
	order = (-1);
	}
	#endif
	#ifdef __COMPILE_126
	else if (__choose7 == 25)
	{
	//AddStmtAndReplaceAtomKind
	order = (0);
	}
	#endif
	#ifdef __COMPILE_127
	else if (__choose7 == 26)
	{
	//AddStmtAndReplaceAtomKind
	order = (1);
	}
	#endif
	#ifdef __COMPILE_128
	else if (__choose7 == 27)
	{
	//AddStmtAndReplaceAtomKind
	order = (2);
	}
	#endif
	#ifdef __COMPILE_129
	else if (__choose7 == 28)
	{
	//AddStmtAndReplaceAtomKind
	order = (256);
	}
	#endif
	#ifdef __COMPILE_130
	else if (__choose7 == 29)
	{
	//AddStmtAndReplaceAtomKind
	order = (34665);
	}
	#endif
	#ifdef __COMPILE_131
	else if (__choose7 == 30)
	{
	//AddStmtAndReplaceAtomKind
	order = (4);
	}
	#endif
	#ifdef __COMPILE_132
	else if (__choose7 == 31)
	{
	//AddStmtAndReplaceAtomKind
	order = (512);
	}
	#endif
	#ifdef __COMPILE_133
	else if (__choose7 == 32)
	{
	//AddStmtAndReplaceAtomKind
	order = (6);
	}
	#endif
	#ifdef __COMPILE_134
	else if (__choose7 == 33)
	{
	//AddStmtAndReplaceAtomKind
	order = (768);
	}
	#endif
	#ifdef __COMPILE_135
	else if (__choose7 == 34)
	{
	//AddStmtAndReplaceAtomKind
	order = (order);
	}
	#endif
	#ifdef __COMPILE_136
	else if (__choose7 == 35)
	{
	//AddStmtAndReplaceAtomKind
	printf("%s:\n", (argv)[optind]);
	}
	#endif
	#ifdef __COMPILE_137
	else if (__choose7 == 36)
	{
	//AddStmtAndReplaceAtomKind
	status = (-1);
	}
	#endif
	#ifdef __COMPILE_138
	else if (__choose7 == 37)
	{
	//AddStmtAndReplaceAtomKind
	status = (0);
	}
	#endif
	#ifdef __COMPILE_139
	else if (__choose7 == 38)
	{
	//AddStmtAndReplaceAtomKind
	status = (1);
	}
	#endif
	#ifdef __COMPILE_140
	else if (__choose7 == 39)
	{
	//AddStmtAndReplaceAtomKind
	status = (2);
	}
	#endif
	#ifdef __COMPILE_141
	else if (__choose7 == 40)
	{
	//AddStmtAndReplaceAtomKind
	status = (256);
	}
	#endif
	#ifdef __COMPILE_142
	else if (__choose7 == 41)
	{
	//AddStmtAndReplaceAtomKind
	status = (34665);
	}
	#endif
	#ifdef __COMPILE_143
	else if (__choose7 == 42)
	{
	//AddStmtAndReplaceAtomKind
	status = (4);
	}
	#endif
	#ifdef __COMPILE_144
	else if (__choose7 == 43)
	{
	//AddStmtAndReplaceAtomKind
	status = (512);
	}
	#endif
	#ifdef __COMPILE_145
	else if (__choose7 == 44)
	{
	//AddStmtAndReplaceAtomKind
	status = (6);
	}
	#endif
	#ifdef __COMPILE_146
	else if (__choose7 == 45)
	{
	//AddStmtAndReplaceAtomKind
	status = (768);
	}
	#endif
	#ifdef __COMPILE_147
	else if (__choose7 == 46)
	{
	//AddStmtAndReplaceAtomKind
	status = (argc);
	}
	#endif
	#ifdef __COMPILE_148
	else if (__choose7 == 47)
	{
	//AddStmtAndReplaceAtomKind
	status = (c);
	}
	#endif
	#ifdef __COMPILE_149
	else if (__choose7 == 48)
	{
	//AddStmtAndReplaceAtomKind
	status = (chopstrips);
	}
	#endif
	#ifdef __COMPILE_150
	else if (__choose7 == 49)
	{
	//AddStmtAndReplaceAtomKind
	status = (dirnum);
	}
	#endif
	#ifdef __COMPILE_151
	else if (__choose7 == 50)
	{
	//AddStmtAndReplaceAtomKind
	status = (multiplefiles);
	}
	#endif
	#ifdef __COMPILE_152
	else if (__choose7 == 51)
	{
	//AddStmtAndReplaceAtomKind
	status = (optind);
	}
	#endif
	#ifdef __COMPILE_153
	else if (__choose7 == 52)
	{
	//AddStmtAndReplaceAtomKind
	tif = (tif);
	}
	#endif
	#ifdef __COMPILE_154
	else if (__choose7 == 53)
	{
	//AddStmtAndReplaceAtomKind
	tif = TIFFOpen((argv)[optind], chopstrips ? "rC" : "rc");
	}
	#endif
	#ifdef __COMPILE_155
	else if (__choose7 == 54)
	{
	//AddStmtAndReplaceAtomKind
	tiffinfo((tif), order, flags);
	}
	#endif
	#ifdef __COMPILE_156
	else if (__choose7 == 55)
	{
	//AddStmtKind
	(void)TIFFSetErrorHandler(PrivateErrorHandler);
	}
	#endif
	#ifdef __COMPILE_157
	else if (__choose7 == 56)
	{
	//AddStmtKind
	TIFFClose(tif);
	}
	#endif
	#ifdef __COMPILE_158
	else if (__choose7 == 57)
	{
	//AddStmtKind
	exit(-1);
	}
	#endif
	#ifdef __COMPILE_159
	else if (__choose7 == 58)
	{
	//AddStmtKind
	multiplefiles = (argc - optind > 1);
	}
	#endif
	#ifdef __COMPILE_160
	else if (__choose7 == 59)
	{
	//AddStmtKind
	old_error_handler = _TIFFerrorHandler;
	}
	#endif
	#ifdef __COMPILE_161
	else if (__choose7 == 60)
	{
	//AddStmtKind
	order = 1;
	}
	#endif
	#ifdef __COMPILE_162
	else if (__choose7 == 61)
	{
	//AddStmtKind
	order = 2;
	}
	#endif
	#ifdef __COMPILE_163
	else if (__choose7 == 62)
	{
	//AddStmtKind
	printf("%s:\n", argv[optind]);
	}
	#endif
	#ifdef __COMPILE_164
	else if (__choose7 == 63)
	{
	//AddStmtKind
	printf("):\n");
	}
	#endif
	#ifdef __COMPILE_165
	else if (__choose7 == 64)
	{
	//AddStmtKind
	printf("\n ");
	}
	#endif
	#ifdef __COMPILE_166
	else if (__choose7 == 65)
	{
	//AddStmtKind
	putchar('\n');
	}
	#endif
	#ifdef __COMPILE_167
	else if (__choose7 == 66)
	{
	//AddStmtKind
	status = 1;
	}
	#endif
	#ifdef __COMPILE_168
	else if (__choose7 == 67)
	{
	//AddStmtKind
	tif = TIFFOpen(argv[optind], chopstrips ? "rC" : "rc");
	}
	#endif
	#ifdef __COMPILE_169
	else if (__choose7 == 68)
	{
	//AddStmtKind
	tiffinfo(tif, order, flags);
	}
	#endif
	#ifdef __COMPILE_170
	else if (__choose7 == 69)
	{
	//AddStmtKind
	usage();
	}
	#endif
	#ifdef __COMPILE_171
	else if (__choose7 == 70)
	{
	//IfExitKind
	if (__is_neg("7-70", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return -1;
	}
	#endif
	#ifdef __COMPILE_172
	else if (__choose7 == 71)
	{
	//IfExitKind
	if (__is_neg("7-71", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 0;
	}
	#endif
	#ifdef __COMPILE_173
	else if (__choose7 == 72)
	{
	//IfExitKind
	if (__is_neg("7-72", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 1;
	}
	#endif
	#ifdef __COMPILE_174
	else if (__choose7 == 73)
	{
	//IfExitKind
	if (__is_neg("7-73", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 256;
	}
	#endif
	#ifdef __COMPILE_175
	else if (__choose7 == 74)
	{
	//IfExitKind
	if (__is_neg("7-74", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 2;
	}
	#endif
	#ifdef __COMPILE_176
	else if (__choose7 == 75)
	{
	//IfExitKind
	if (__is_neg("7-75", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 34665;
	}
	#endif
	#ifdef __COMPILE_177
	else if (__choose7 == 76)
	{
	//IfExitKind
	if (__is_neg("7-76", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 4;
	}
	#endif
	#ifdef __COMPILE_178
	else if (__choose7 == 77)
	{
	//IfExitKind
	if (__is_neg("7-77", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 512;
	}
	#endif
	#ifdef __COMPILE_179
	else if (__choose7 == 78)
	{
	//IfExitKind
	if (__is_neg("7-78", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 6;
	}
	#endif
	#ifdef __COMPILE_180
	else if (__choose7 == 79)
	{
	//IfExitKind
	if (__is_neg("7-79", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 768;
	}
	#endif
	int __choose8 = __choose("__SWITCH8");
	if (__choose8 == 0)
	{
	(void) TIFFSetErrorHandler(PrivateErrorHandler);
	
	}
	#ifdef __COMPILE_181
	else if (__choose8 == 1)
	{
	//GuardKind
	if (!__is_neg("8-1", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc), sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv), sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum), sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff), sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg), sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata), sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    (void)TIFFSetErrorHandler(PrivateErrorHandler);
	
	}
	#endif
	#ifdef __COMPILE_182
	else if (__choose8 == 2)
	{
	//MSVExtRemoveStmtKind
	{
	}
	
	}
	#endif
	#ifdef __COMPILE_183
	else if (__choose8 == 3)
	{
	//ReplaceFunctionKind
	(void)TIFFSetErrorHandler(PrivateErrorHandler);
	
	}
	#endif
	}

	multiplefiles = (argc - optind > 1);
	for (; optind < argc; optind++) {
		if (multiplefiles)
			printf("%s:\n", argv[optind]);
		tif = TIFFOpen(argv[optind], chopstrips ? "rC" : "rc");
		if (tif != NULL) {
			if (dirnum != -1) {
				if (TIFFSetDirectory(tif, (tdir_t) dirnum))
					tiffinfo(tif, order, flags);
			} else if (diroff != 0) {
				if (TIFFSetSubDirectory(tif, diroff))
					tiffinfo(tif, order, flags);
			} else {
				do {
					toff_t offset;

					tiffinfo(tif, order, flags);
					if (TIFFGetField(tif, TIFFTAG_EXIFIFD,
							 &offset)) {
						if (TIFFReadEXIFDirectory(tif, offset))
							tiffinfo(tif, order, flags);
					}
				} while (TIFFReadDirectory(tif));
			}
			TIFFClose(tif);
		}
	}
	return (status);
}

char* stuff[] = {
"usage: tiffinfo [options] input...",
"where options are:",
" -D		read data",
" -i		ignore read errors",
" -c		display data for grey/color response curve or colormap",
" -d		display raw/decoded image data",
" -f lsb2msb	force lsb-to-msb FillOrder for input",
" -f msb2lsb	force msb-to-lsb FillOrder for input",
" -j		show JPEG tables",
" -o offset	set initial directory offset",
" -r		read/display raw image data instead of decoded data",
" -s		display strip offsets and byte counts",
" -w		display raw data in words rather than bytes",
" -z		enable strip chopping",
" -#		set initial directory (first directory is # 0)",
NULL
};

static void
usage(void)
{
	char buf[BUFSIZ];
	int i;

	setbuf(stderr, buf);
        fprintf(stderr, "%s\n\n", TIFFGetVersion());
	for (i = 0; stuff[i] != NULL; i++)
		fprintf(stderr, "%s\n", stuff[i]);
	exit(-1);
}

static void
ShowStrip(tstrip_t strip, unsigned char* pp, uint32 nrow, tsize_t scanline)
{
	register tsize_t cc;

	printf("Strip %lu:\n", (unsigned long) strip);
	while (nrow-- > 0) {
		for (cc = 0; cc < scanline; cc++) {
			printf(" %02x", *pp++);
			if (((cc+1) % 24) == 0)
				putchar('\n');
		}
		putchar('\n');
	}
}

void
TIFFReadContigStripData(TIFF* tif)
{
	unsigned char *buf;
	tsize_t scanline = TIFFScanlineSize(tif);

	buf = (unsigned char *)_TIFFmalloc(TIFFStripSize(tif));
	if (buf) {
		uint32 row, h;
		uint32 rowsperstrip = (uint32)-1;

		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
		for (row = 0; row < h; row += rowsperstrip) {
			uint32 nrow = (row+rowsperstrip > h ?
			    h-row : rowsperstrip);
			tstrip_t strip = TIFFComputeStrip(tif, row, 0);
			if (TIFFReadEncodedStrip(tif, strip, buf, nrow*scanline) < 0) {
				if (stoponerr)
					break;
			} else if (showdata)
				ShowStrip(strip, buf, nrow, scanline);
		}
		_TIFFfree(buf);
	}
}

void
TIFFReadSeparateStripData(TIFF* tif)
{
	unsigned char *buf;
	tsize_t scanline = TIFFScanlineSize(tif);

	buf = (unsigned char *)_TIFFmalloc(TIFFStripSize(tif));
	if (buf) {
		uint32 row, h;
		uint32 rowsperstrip = (uint32)-1;
		tsample_t s, samplesperpixel;

		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
		for (row = 0; row < h; row += rowsperstrip) {
			for (s = 0; s < samplesperpixel; s++) {
				uint32 nrow = (row+rowsperstrip > h ?
				    h-row : rowsperstrip);
				tstrip_t strip = TIFFComputeStrip(tif, row, s);
				if (TIFFReadEncodedStrip(tif, strip, buf, nrow*scanline) < 0) {
					if (stoponerr)
						break;
				} else if (showdata)
					ShowStrip(strip, buf, nrow, scanline);
			}
		}
		_TIFFfree(buf);
	}
}

static void
ShowTile(uint32 row, uint32 col, tsample_t sample,
    unsigned char* pp, uint32 nrow, tsize_t rowsize)
{
	uint32 cc;

	printf("Tile (%lu,%lu", (unsigned long) row, (unsigned long) col);
	if (sample != (tsample_t) -1)
		printf(",%u", sample);
	printf("):\n");
	while (nrow-- > 0) {
	  for (cc = 0; cc < (uint32) rowsize; cc++) {
			printf(" %02x", *pp++);
			if (((cc+1) % 24) == 0)
				putchar('\n');
		}
		putchar('\n');
	}
}

void
TIFFReadContigTileData(TIFF* tif)
{
	unsigned char *buf;
	tsize_t rowsize = TIFFTileRowSize(tif);

	buf = (unsigned char *)_TIFFmalloc(TIFFTileSize(tif));
	if (buf) {
		uint32 tw, th, w, h;
		uint32 row, col;

		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tw);
		TIFFGetField(tif, TIFFTAG_TILELENGTH, &th);
		for (row = 0; row < h; row += th) {
			for (col = 0; col < w; col += tw) {
				if (TIFFReadTile(tif, buf, col, row, 0, 0) < 0) {
					if (stoponerr)
						break;
				} else if (showdata)
					ShowTile(row, col, (tsample_t) -1, buf, th, rowsize);
			}
		}
		_TIFFfree(buf);
	}
}

void
TIFFReadSeparateTileData(TIFF* tif)
{
	unsigned char *buf;
	tsize_t rowsize = TIFFTileRowSize(tif);

	buf = (unsigned char *)_TIFFmalloc(TIFFTileSize(tif));
	if (buf) {
		uint32 tw, th, w, h;
		uint32 row, col;
		tsample_t s, samplesperpixel;

		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
		TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tw);
		TIFFGetField(tif, TIFFTAG_TILELENGTH, &th);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
		for (row = 0; row < h; row += th) {
			for (col = 0; col < w; col += tw) {
				for (s = 0; s < samplesperpixel; s++) {
					if (TIFFReadTile(tif, buf, col, row, 0, s) < 0) {
						if (stoponerr)
							break;
					} else if (showdata)
						ShowTile(row, col, s, buf, th, rowsize);
				}
			}
		}
		_TIFFfree(buf);
	}
}

void
TIFFReadData(TIFF* tif)
{
	uint16 config;

	TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
	if (TIFFIsTiled(tif)) {
		if (config == PLANARCONFIG_CONTIG)
			TIFFReadContigTileData(tif);
		else
			TIFFReadSeparateTileData(tif);
	} else {
		if (config == PLANARCONFIG_CONTIG)
			TIFFReadContigStripData(tif);
		else
			TIFFReadSeparateStripData(tif);
	}
}

static void
ShowRawBytes(unsigned char* pp, uint32 n)
{
	uint32 i;

	for (i = 0; i < n; i++) {
		printf(" %02x", *pp++);
		if (((i+1) % 24) == 0)
			printf("\n ");
	}
	putchar('\n');
}

static void
ShowRawWords(uint16* pp, uint32 n)
{
	uint32 i;

	for (i = 0; i < n; i++) {
		printf(" %04x", *pp++);
		if (((i+1) % 15) == 0)
			printf("\n ");
	}
	putchar('\n');
}

void
TIFFReadRawData(TIFF* tif, int bitrev)
{
	tstrip_t nstrips = TIFFNumberOfStrips(tif);
	const char* what = TIFFIsTiled(tif) ? "Tile" : "Strip";
	uint64* stripbc;

	TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &stripbc);
	if (nstrips > 0) {
		uint32 bufsize = (uint32) stripbc[0];
		tdata_t buf = _TIFFmalloc(bufsize);
		tstrip_t s;

		for (s = 0; s < nstrips; s++) {
			if (stripbc[s] > bufsize) {
				buf = _TIFFrealloc(buf, (tmsize_t)stripbc[s]);
				bufsize = (uint32) stripbc[s];
			}
			if (buf == NULL) {
				fprintf(stderr,
				   "Cannot allocate buffer to read strip %lu\n",
				    (unsigned long) s);
				break;
			}
			if (TIFFReadRawStrip(tif, s, buf, (tmsize_t) stripbc[s]) < 0) {
				fprintf(stderr, "Error reading strip %lu\n",
				    (unsigned long) s);
				if (stoponerr)
					break;
			} else if (showdata) {
				if (bitrev) {
					TIFFReverseBits(buf, (tmsize_t)stripbc[s]);
					printf("%s %lu: (bit reversed)\n ",
					    what, (unsigned long) s);
				} else
					printf("%s %lu:\n ", what,
					    (unsigned long) s);
				if (showwords)
					ShowRawWords((uint16*) buf, (uint32) stripbc[s]>>1);
				else
					ShowRawBytes((unsigned char*) buf, (uint32) stripbc[s]);
			}
		}
		if (buf != NULL)
			_TIFFfree(buf);
	}
}

static void
tiffinfo(TIFF* tif, uint16 order, long flags)
{
	TIFFPrintDirectory(tif, stdout, flags);
	if (!readdata)
		return;
	if (rawdata) {
		if (order) {
			uint16 o;
			TIFFGetFieldDefaulted(tif,
			    TIFFTAG_FILLORDER, &o);
			TIFFReadRawData(tif, o != order);
		} else
			TIFFReadRawData(tif, 0);
	} else {
		if (order)
			TIFFSetField(tif, TIFFTAG_FILLORDER, order);
		TIFFReadData(tif);
	}
}

/* vim: set ts=8 sts=8 sw=8 noet: */
