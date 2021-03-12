/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* the clang cmd full path */
#define CLANG_CMD "/usr/local/bin/clang"

/* the location of the wrapper for instrument the file */
#define CLANG_WRAP_PATH "/root/test/prophet/wrap"

/* the extra include path arguments that need to pass to clang when build AST
   tree */
#define EXTRA_CLANG_INCLUDE_PATH " -I/usr/local/include  -I/usr/local/lib/clang/11.1.0/include  -I/usr/include/x86_64-linux-gnu  -I/usr/include  "

/* the gcc cmd full path */
#define GCC_CMD "/usr/bin/gcc"

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "prophet"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "fanl@csail.mit.edu, zichaoqi@mit.edu"

/* Define to the full name of this package. */
#define PACKAGE_NAME "prophet"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "prophet 0.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "prophet"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.1"

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `void *', as computed by sizeof. */
#define SIZEOF_VOID_P 8

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "0.1"
