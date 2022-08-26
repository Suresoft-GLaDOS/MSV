#define __COMPILE_1
#define __COMPILE_2
#define __COMPILE_3
#define __COMPILE_4
#define __COMPILE_5
#define __COMPILE_6
#define __COMPILE_7
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
        if (old_error_handler)
                (*old_error_handler)(module,fmt,ap);
	status = 1;
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
	int __choose0 = __choose("__SWITCH0");
	if (__choose0 == 0)
	{}
	#ifdef __COMPILE_0
	else if (__choose0 == 1)
	{
	//AddIfStmtKind
	if (!readdata)
	    return;
	}
	#endif
	#ifdef __COMPILE_1
	else if (__choose0 == 2)
	{
	//AddIfStmtKind
	if ((strcmp(optarg, "msb2lsb") == 0))
	    order = 1;
	else
	    usage();
	}
	#endif
	#ifdef __COMPILE_2
	else if (__choose0 == 3)
	{
	//AddIfStmtKind
	if (TIFFSetDirectory(tif, (tdir_t)dirnum))
	    tiffinfo(tif, order, flags);
	}
	#endif
	#ifdef __COMPILE_3
	else if (__choose0 == 4)
	{
	//AddIfStmtKind
	if (TIFFSetSubDirectory(tif, diroff))
	    tiffinfo(tif, order, flags);
	}
	#endif
	#ifdef __COMPILE_4
	else if (__choose0 == 5)
	{
	//AddIfStmtKind
	if (multiplefiles)
	    printf("%s:\n", argv[optind]);
	}
	#endif
	#ifdef __COMPILE_5
	else if (__choose0 == 6)
	{
	//AddIfStmtKind
	if (optind >= argc)
	    usage();
	}
	#endif
	#ifdef __COMPILE_6
	else if (__choose0 == 7)
	{
	//AddStmtAndReplaceAtomKind
	TIFFClose((tif));
	}
	#endif
	#ifdef __COMPILE_7
	else if (__choose0 == 8)
	{
	//AddStmtAndReplaceAtomKind
	TIFFClose(tif && !(1 << 5));
	}
	#endif
	#ifdef __COMPILE_8
	else if (__choose0 == 9)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadContigStripData((tif));
	}
	#endif
	#ifdef __COMPILE_9
	else if (__choose0 == 10)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadContigTileData((tif));
	}
	#endif
	#ifdef __COMPILE_10
	else if (__choose0 == 11)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadData((tif));
	}
	#endif
	#ifdef __COMPILE_11
	else if (__choose0 == 12)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadRawData((tif), 0);
	}
	#endif
	#ifdef __COMPILE_12
	else if (__choose0 == 13)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadSeparateStripData((tif));
	}
	#endif
	#ifdef __COMPILE_13
	else if (__choose0 == 14)
	{
	//AddStmtAndReplaceAtomKind
	TIFFReadSeparateTileData((tif));
	}
	#endif
	#ifdef __COMPILE_14
	else if (__choose0 == 15)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (-1);
	}
	#endif
	#ifdef __COMPILE_15
	else if (__choose0 == 16)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (0);
	}
	#endif
	#ifdef __COMPILE_16
	else if (__choose0 == 17)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (1);
	}
	#endif
	#ifdef __COMPILE_17
	else if (__choose0 == 18)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (2);
	}
	#endif
	#ifdef __COMPILE_18
	else if (__choose0 == 19)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (256);
	}
	#endif
	#ifdef __COMPILE_19
	else if (__choose0 == 20)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (34665);
	}
	#endif
	#ifdef __COMPILE_20
	else if (__choose0 == 21)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (4);
	}
	#endif
	#ifdef __COMPILE_21
	else if (__choose0 == 22)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (512);
	}
	#endif
	#ifdef __COMPILE_22
	else if (__choose0 == 23)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (6);
	}
	#endif
	#ifdef __COMPILE_23
	else if (__choose0 == 24)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (768);
	}
	#endif
	#ifdef __COMPILE_24
	else if (__choose0 == 25)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (argc && !(1 << 5) - optind > 1);
	}
	#endif
	#ifdef __COMPILE_25
	else if (__choose0 == 26)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (argc - optind && !(1 << 5) > 1);
	}
	#endif
	#ifdef __COMPILE_26
	else if (__choose0 == 27)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (argc);
	}
	#endif
	#ifdef __COMPILE_27
	else if (__choose0 == 28)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (c);
	}
	#endif
	#ifdef __COMPILE_28
	else if (__choose0 == 29)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (chopstrips);
	}
	#endif
	#ifdef __COMPILE_29
	else if (__choose0 == 30)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (dirnum);
	}
	#endif
	#ifdef __COMPILE_30
	else if (__choose0 == 31)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (multiplefiles);
	}
	#endif
	#ifdef __COMPILE_31
	else if (__choose0 == 32)
	{
	//AddStmtAndReplaceAtomKind
	multiplefiles = (optind);
	}
	#endif
	#ifdef __COMPILE_32
	else if (__choose0 == 33)
	{
	//AddStmtAndReplaceAtomKind
	old_error_handler = _TIFFerrorHandler && !(1 << 5);
	}
	#endif
	#ifdef __COMPILE_33
	else if (__choose0 == 34)
	{
	//AddStmtAndReplaceAtomKind
	order = (-1);
	}
	#endif
	#ifdef __COMPILE_34
	else if (__choose0 == 35)
	{
	//AddStmtAndReplaceAtomKind
	order = (0);
	}
	#endif
	#ifdef __COMPILE_35
	else if (__choose0 == 36)
	{
	//AddStmtAndReplaceAtomKind
	order = (1);
	}
	#endif
	#ifdef __COMPILE_36
	else if (__choose0 == 37)
	{
	//AddStmtAndReplaceAtomKind
	order = (2);
	}
	#endif
	#ifdef __COMPILE_37
	else if (__choose0 == 38)
	{
	//AddStmtAndReplaceAtomKind
	order = (256);
	}
	#endif
	#ifdef __COMPILE_38
	else if (__choose0 == 39)
	{
	//AddStmtAndReplaceAtomKind
	order = (34665);
	}
	#endif
	#ifdef __COMPILE_39
	else if (__choose0 == 40)
	{
	//AddStmtAndReplaceAtomKind
	order = (4);
	}
	#endif
	#ifdef __COMPILE_40
	else if (__choose0 == 41)
	{
	//AddStmtAndReplaceAtomKind
	order = (512);
	}
	#endif
	#ifdef __COMPILE_41
	else if (__choose0 == 42)
	{
	//AddStmtAndReplaceAtomKind
	order = (6);
	}
	#endif
	#ifdef __COMPILE_42
	else if (__choose0 == 43)
	{
	//AddStmtAndReplaceAtomKind
	order = (768);
	}
	#endif
	#ifdef __COMPILE_43
	else if (__choose0 == 44)
	{
	//AddStmtAndReplaceAtomKind
	order = (order);
	}
	#endif
	#ifdef __COMPILE_44
	else if (__choose0 == 45)
	{
	//AddStmtAndReplaceAtomKind
	printf("%s:\n", (argv)[optind]);
	}
	#endif
	#ifdef __COMPILE_45
	else if (__choose0 == 46)
	{
	//AddStmtAndReplaceAtomKind
	printf("%s:\n", argv && !(1 << 5)[optind]);
	}
	#endif
	#ifdef __COMPILE_46
	else if (__choose0 == 47)
	{
	//AddStmtAndReplaceAtomKind
	printf("%s:\n", argv[optind && !(1 << 5)]);
	}
	#endif
	#ifdef __COMPILE_47
	else if (__choose0 == 48)
	{
	//AddStmtAndReplaceAtomKind
	status = (-1);
	}
	#endif
	#ifdef __COMPILE_48
	else if (__choose0 == 49)
	{
	//AddStmtAndReplaceAtomKind
	status = (0);
	}
	#endif
	#ifdef __COMPILE_49
	else if (__choose0 == 50)
	{
	//AddStmtAndReplaceAtomKind
	status = (1);
	}
	#endif
	#ifdef __COMPILE_50
	else if (__choose0 == 51)
	{
	//AddStmtAndReplaceAtomKind
	status = (2);
	}
	#endif
	#ifdef __COMPILE_51
	else if (__choose0 == 52)
	{
	//AddStmtAndReplaceAtomKind
	status = (256);
	}
	#endif
	#ifdef __COMPILE_52
	else if (__choose0 == 53)
	{
	//AddStmtAndReplaceAtomKind
	status = (34665);
	}
	#endif
	#ifdef __COMPILE_53
	else if (__choose0 == 54)
	{
	//AddStmtAndReplaceAtomKind
	status = (4);
	}
	#endif
	#ifdef __COMPILE_54
	else if (__choose0 == 55)
	{
	//AddStmtAndReplaceAtomKind
	status = (512);
	}
	#endif
	#ifdef __COMPILE_55
	else if (__choose0 == 56)
	{
	//AddStmtAndReplaceAtomKind
	status = (6);
	}
	#endif
	#ifdef __COMPILE_56
	else if (__choose0 == 57)
	{
	//AddStmtAndReplaceAtomKind
	status = (768);
	}
	#endif
	#ifdef __COMPILE_57
	else if (__choose0 == 58)
	{
	//AddStmtAndReplaceAtomKind
	status = (argc);
	}
	#endif
	#ifdef __COMPILE_58
	else if (__choose0 == 59)
	{
	//AddStmtAndReplaceAtomKind
	status = (c);
	}
	#endif
	#ifdef __COMPILE_59
	else if (__choose0 == 60)
	{
	//AddStmtAndReplaceAtomKind
	status = (chopstrips);
	}
	#endif
	#ifdef __COMPILE_60
	else if (__choose0 == 61)
	{
	//AddStmtAndReplaceAtomKind
	status = (dirnum);
	}
	#endif
	#ifdef __COMPILE_61
	else if (__choose0 == 62)
	{
	//AddStmtAndReplaceAtomKind
	status = (multiplefiles);
	}
	#endif
	#ifdef __COMPILE_62
	else if (__choose0 == 63)
	{
	//AddStmtAndReplaceAtomKind
	status = (optind);
	}
	#endif
	#ifdef __COMPILE_63
	else if (__choose0 == 64)
	{
	//AddStmtAndReplaceAtomKind
	tif = (tif);
	}
	#endif
	#ifdef __COMPILE_64
	else if (__choose0 == 65)
	{
	//AddStmtAndReplaceAtomKind
	tif = TIFFOpen((argv)[optind], chopstrips ? "rC" : "rc");
	}
	#endif
	#ifdef __COMPILE_65
	else if (__choose0 == 66)
	{
	//AddStmtAndReplaceAtomKind
	tif = TIFFOpen(argv && !(1 << 5)[optind], chopstrips ? "rC" : "rc");
	}
	#endif
	#ifdef __COMPILE_66
	else if (__choose0 == 67)
	{
	//AddStmtAndReplaceAtomKind
	tif = TIFFOpen(argv[optind && !(1 << 5)], chopstrips ? "rC" : "rc");
	}
	#endif
	#ifdef __COMPILE_67
	else if (__choose0 == 68)
	{
	//AddStmtAndReplaceAtomKind
	tif = TIFFOpen(argv[optind], chopstrips && !(1 << 5) ? "rC" : "rc");
	}
	#endif
	#ifdef __COMPILE_68
	else if (__choose0 == 69)
	{
	//AddStmtAndReplaceAtomKind
	tiffinfo((tif), order, flags);
	}
	#endif
	#ifdef __COMPILE_69
	else if (__choose0 == 70)
	{
	//AddStmtAndReplaceAtomKind
	tiffinfo(tif && !(1 << 5), order, flags);
	}
	#endif
	#ifdef __COMPILE_70
	else if (__choose0 == 71)
	{
	//AddStmtAndReplaceAtomKind
	tiffinfo(tif, order && !(1 << 5), flags);
	}
	#endif
	#ifdef __COMPILE_71
	else if (__choose0 == 72)
	{
	//AddStmtAndReplaceAtomKind
	tiffinfo(tif, order, flags && !(1 << 5));
	}
	#endif
	#ifdef __COMPILE_72
	else if (__choose0 == 73)
	{
	//AddStmtKind
	(void)TIFFSetErrorHandler(PrivateErrorHandler);
	}
	#endif
	#ifdef __COMPILE_73
	else if (__choose0 == 74)
	{
	//AddStmtKind
	TIFFClose(tif);
	}
	#endif
	#ifdef __COMPILE_74
	else if (__choose0 == 75)
	{
	//AddStmtKind
	exit(-1);
	}
	#endif
	#ifdef __COMPILE_75
	else if (__choose0 == 76)
	{
	//AddStmtKind
	multiplefiles = (argc - optind > 1);
	}
	#endif
	#ifdef __COMPILE_76
	else if (__choose0 == 77)
	{
	//AddStmtKind
	old_error_handler = _TIFFerrorHandler;
	}
	#endif
	#ifdef __COMPILE_77
	else if (__choose0 == 78)
	{
	//AddStmtKind
	order = 1;
	}
	#endif
	#ifdef __COMPILE_78
	else if (__choose0 == 79)
	{
	//AddStmtKind
	order = 2;
	}
	#endif
	#ifdef __COMPILE_79
	else if (__choose0 == 80)
	{
	//AddStmtKind
	printf("%s:\n", argv[optind]);
	}
	#endif
	#ifdef __COMPILE_80
	else if (__choose0 == 81)
	{
	//AddStmtKind
	printf("):\n");
	}
	#endif
	#ifdef __COMPILE_81
	else if (__choose0 == 82)
	{
	//AddStmtKind
	printf("\n ");
	}
	#endif
	#ifdef __COMPILE_82
	else if (__choose0 == 83)
	{
	//AddStmtKind
	putchar('\n');
	}
	#endif
	#ifdef __COMPILE_83
	else if (__choose0 == 84)
	{
	//AddStmtKind
	status = 1;
	}
	#endif
	#ifdef __COMPILE_84
	else if (__choose0 == 85)
	{
	//AddStmtKind
	tif = TIFFOpen(argv[optind], chopstrips ? "rC" : "rc");
	}
	#endif
	#ifdef __COMPILE_85
	else if (__choose0 == 86)
	{
	//AddStmtKind
	tiffinfo(tif, order, flags);
	}
	#endif
	#ifdef __COMPILE_86
	else if (__choose0 == 87)
	{
	//AddStmtKind
	usage();
	}
	#endif
	#ifdef __COMPILE_87
	else if (__choose0 == 88)
	{
	//IfExitKind
	if (__is_neg("0-88", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return -1;
	}
	#endif
	#ifdef __COMPILE_88
	else if (__choose0 == 89)
	{
	//IfExitKind
	if (__is_neg("0-89", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 0;
	}
	#endif
	#ifdef __COMPILE_89
	else if (__choose0 == 90)
	{
	//IfExitKind
	if (__is_neg("0-90", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 1;
	}
	#endif
	#ifdef __COMPILE_90
	else if (__choose0 == 91)
	{
	//IfExitKind
	if (__is_neg("0-91", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 256;
	}
	#endif
	#ifdef __COMPILE_91
	else if (__choose0 == 92)
	{
	//IfExitKind
	if (__is_neg("0-92", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 2;
	}
	#endif
	#ifdef __COMPILE_92
	else if (__choose0 == 93)
	{
	//IfExitKind
	if (__is_neg("0-93", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 34665;
	}
	#endif
	#ifdef __COMPILE_93
	else if (__choose0 == 94)
	{
	//IfExitKind
	if (__is_neg("0-94", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 4;
	}
	#endif
	#ifdef __COMPILE_94
	else if (__choose0 == 95)
	{
	//IfExitKind
	if (__is_neg("0-95", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 512;
	}
	#endif
	#ifdef __COMPILE_95
	else if (__choose0 == 96)
	{
	//IfExitKind
	if (__is_neg("0-96", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 6;
	}
	#endif
	#ifdef __COMPILE_96
	else if (__choose0 == 97)
	{
	//IfExitKind
	if (__is_neg("0-97", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc),
				 sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv),
				 sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum),
				 sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff),
				 sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg),
				 sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata),
				 sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    return 768;
	}
	#endif
	int __choose1 = __choose("__SWITCH1");
	if (__choose1 == 0)
	{
	(void) TIFFSetErrorHandler(PrivateErrorHandler);
	
	}
	#ifdef __COMPILE_97
	else if (__choose1 == 1)
	{
	//GuardKind
	if (!__is_neg("1-1", "L134", 19, &(old_error_handler), sizeof (old_error_handler), &(argc), sizeof (argc), &(multiplefiles), sizeof (multiplefiles), &(optind), sizeof (optind), &(argv), sizeof (argv), &(tif), sizeof (tif), &(chopstrips), sizeof (chopstrips), &(dirnum), sizeof (dirnum), &(order), sizeof (order), &(flags), sizeof (flags), &(diroff), sizeof (diroff), &(showwords), sizeof (showwords), &(rawdata), sizeof (rawdata), &(optarg), sizeof (optarg), &(status), sizeof (status), &(stoponerr), sizeof (stoponerr), &(readdata), sizeof (readdata), &(showdata), sizeof (showdata), &(c), sizeof (c)))
	    (void)TIFFSetErrorHandler(PrivateErrorHandler);
	
	}
	#endif
	#ifdef __COMPILE_98
	else if (__choose1 == 2)
	{
	//MSVExtRemoveStmtKind
	{
	}
	
	}
	#endif
	#ifdef __COMPILE_99
	else if (__choose1 == 3)
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
