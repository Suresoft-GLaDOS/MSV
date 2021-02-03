/*
 *  libfb - FreeBASIC's runtime library
 *	Copyright (C) 2004-2008 The FreeBASIC development team.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  As a special exception, the copyright holders of this library give
 *  you permission to link this library with independent modules to
 *  produce an executable, regardless of the license terms of these
 *  independent modules, and to copy and distribute the resulting
 *  executable under terms of your choice, provided that you also meet,
 *  for each linked independent module, the terms and conditions of the
 *  license of that module. An independent module is a module which is
 *  not derived from or based on this library. If you modify this library,
 *  you may extend this exception to your version of the library, but
 *  you are not obligated to do so. If you do not wish to do so, delete
 *  this exception statement from your version.
 */

#ifndef __FB_STRING_H__
#define __FB_STRING_H__

#ifndef FB_STRLOCK
    /** Acquire a semaphore (recursive mutex) especially for STRING functions.
     */
# define FB_STRLOCK()
#endif
#ifndef FB_STRUNLOCK
    /** Release a semaphore (recursive mutex) especially for STRING functions.
     */
# define FB_STRUNLOCK()
#endif

/**************************************************************************************************
 * strings
 **************************************************************************************************/

#include <string.h>
#include <stdio.h>

/** Flag to identify a string as a temporary string.
 *
 * This flag is stored in struct _FBSTRING::len so it's absolutely required
 * to use FB_STRSIZE(s) to query a strings length.
 */
#define FB_TEMPSTRBIT 0x80000000L

/** Returns if the string is a temporary string.
 */
#define FB_ISTEMP(s) ((((FBSTRING *)s)->len & FB_TEMPSTRBIT) != 0)

/** Returns a string length.
 */
#define FB_STRSIZE(s) (((FBSTRING *)s)->len & ~FB_TEMPSTRBIT)

/** Returns the string data.
 */
#define FB_STRPTR(s,size)                     \
    ( s == NULL? NULL : ( size == -1? ((FBSTRING *)s)->data : (char *)s ) )

#define FB_STRSETUP_FIX(s,size,ptr,len)                     \
do {                                                        \
    if( s == NULL )                                         \
    {                                                       \
        ptr = NULL;                                         \
        len = 0;                                            \
    }                                                       \
    else                                                    \
    {                                                       \
        if( size == -1 )                                    \
        {                                                   \
            ptr = ((FBSTRING *)s)->data;                    \
            len = FB_STRSIZE( s );                          \
        }                                                   \
        else                                                \
        {                                                   \
            ptr = (char *)s;                                \
            /* always get the real len, as fix-len string */ \
            /* will have garbage at end (nulls or spaces) */ \
            len = strlen( (char *)s );                      \
        }                                                   \
    }                                                       \
} while (0)

#define FB_STRSETUP_DYN(s,size,ptr,len)                     \
do {                                                        \
    if( s == NULL )                                         \
    {                                                       \
        ptr = NULL;                                         \
        len = 0;                                            \
    }                                                       \
    else                                                    \
    {                                                       \
        switch ( size ) {                                   \
        case -1:                                            \
            ptr = ((FBSTRING *)s)->data;                    \
            len = FB_STRSIZE( s );                          \
            break;                                          \
        case 0:                                             \
            ptr = (char *) s;                               \
            len = strlen( ptr );                            \
            break;                                          \
        default:                                            \
            ptr = (char *) s;                               \
            len = size - 1; /* without terminating NUL */   \
            break;                                          \
        }                                                   \
    }                                                       \
} while (0)

/** Structure containing information about a specific string.
 *
 * This structure hols all informations about a specific string. This is
 * required to allow BASIC-style strings that may contain NUL characters.
 */
typedef struct _FBSTRING {
    char           *data;    /**< pointer to the real string data */
    int             len;     /**< String length. */
    int             size;    /**< Size of allocated memory block. */
} FBSTRING;


typedef struct _FB_STR_TMPDESC {
    FB_LISTELEM     elem;
    FBSTRING        desc;
} FB_STR_TMPDESC;


/* protos */

/** Sets the length of a string (without reallocation).
 *
 * This function preserves any flags set for this string descriptor.
 */
static __inline__ void fb_hStrSetLength( FBSTRING *str, size_t size ) {
    str->len = size | (str->len & FB_TEMPSTRBIT);
}

FBCALL FBSTRING    *fb_hStrAllocTmpDesc 	( void );
FBCALL int          fb_hStrDelTempDesc  	( FBSTRING *str );
FBCALL FBSTRING    *fb_hStrAlloc			( FBSTRING *str, int size );
FBCALL FBSTRING    *fb_hStrRealloc      	( FBSTRING *str, int size, int preserve );
FBCALL FBSTRING    *fb_hStrAllocTemp    	( FBSTRING *str, int size );
FBCALL FBSTRING    *fb_hStrAllocTemp_NoLock	( FBSTRING *str, int size );
FBCALL int          fb_hStrDelTemp      	( FBSTRING *str );
FBCALL int          fb_hStrDelTemp_NoLock  	( FBSTRING *str );
FBCALL void         fb_hStrCopy         	( char *dst, const char *src, int bytes );
FBCALL char        *fb_hStrSkipChar     	( char *s, int len, int c );
FBCALL char        *fb_hStrSkipCharRev  	( char *s, int len, int c );


/* public */

FBCALL void 	   *fb_StrInit			( void *dst, int dst_size, void *src, int src_size, 
										  int fill_rem );
FBCALL void        *fb_StrAssign        ( void *dst, int dst_size, void *src, int src_size,
										  int fill_rem );
FBCALL void 	   *fb_StrAssignEx		( void *dst, int dst_size, void *src, int src_size,
										  int fill_rem, int is_init );
FBCALL void         fb_StrDelete        ( FBSTRING *str );
FBCALL FBSTRING    *fb_StrConcat        ( FBSTRING *dst, void *str1, int str1_size,
										  void *str2, int str2_size );
FBCALL void        *fb_StrConcatAssign  ( void *dst, int dst_size, void *src, int src_size,
										  int fillrem );
FBCALL int          fb_StrCompare       ( void *str1, int str1_size, void *str2,
										  int str2_size );
FBCALL FBSTRING    *fb_StrAllocTempResult ( FBSTRING *src );
FBCALL FBSTRING    *fb_StrAllocTempDescF( char *str, int str_size );
FBCALL FBSTRING    *fb_StrAllocTempDescV( FBSTRING *str );
FBCALL FBSTRING    *fb_StrAllocTempDescZ( const char *str );
FBCALL int          fb_StrLen           ( void *str, int str_size );

FBCALL FBSTRING    *fb_IntToStr        	( int num );
FBCALL FBSTRING    *fb_UIntToStr        ( unsigned int num );
FBCALL FBSTRING    *fb_FloatToStr      	( float num );
FBCALL FBSTRING    *fb_DoubleToStr     	( double num );

#define FB_F2A_ADDBLANK     0x00000001

FBCALL double       fb_hStr2Double      ( char *src, int len );
FBCALL int          fb_hStr2Int         ( char *src, int len );
FBCALL unsigned int fb_hStr2UInt        ( char *src, int len );
FBCALL long long    fb_hStr2Longint     ( char *src, int len );
FBCALL unsigned long long fb_hStr2ULongint ( char *src, int len );
FBCALL int          fb_hStrRadix2Int    ( char *src, int len, int radix );
FBCALL long long 	fb_hStrRadix2Longint( char *s, int len, int radix );
       char        *fb_hFloat2Str       ( double val, char *buffer, int digits, int mask );

       FBSTRING    *fb_CHR              ( int args, ... );
FBCALL unsigned int fb_ASC              ( FBSTRING *str, int pos );
FBCALL double       fb_VAL              ( FBSTRING *str );
FBCALL double       fb_CVD              ( FBSTRING *str );
FBCALL int          fb_CVI              ( FBSTRING *str );
FBCALL FBSTRING    *fb_HEX              ( int num );
FBCALL FBSTRING    *fb_OCT              ( int num );
FBCALL FBSTRING    *fb_BIN              ( int num );
FBCALL FBSTRING    *fb_MKD              ( double num );
FBCALL FBSTRING    *fb_MKI              ( int num );
FBCALL FBSTRING    *fb_MKS              ( float num );
FBCALL FBSTRING    *fb_LEFT             ( FBSTRING *str, int chars );
FBCALL FBSTRING    *fb_RIGHT            ( FBSTRING *str, int chars );
FBCALL FBSTRING    *fb_SPACE            ( int chars );
FBCALL FBSTRING    *fb_LTRIM            ( FBSTRING *str );
FBCALL FBSTRING    *fb_LTrimEx          ( FBSTRING *str, FBSTRING *pattern );
FBCALL FBSTRING    *fb_LTrimAny         ( FBSTRING *str, FBSTRING *pattern );
FBCALL FBSTRING    *fb_RTRIM            ( FBSTRING *str );
FBCALL FBSTRING    *fb_RTrimEx          ( FBSTRING *str, FBSTRING *pattern );
FBCALL FBSTRING    *fb_RTrimAny         ( FBSTRING *str, FBSTRING *pattern );
FBCALL FBSTRING    *fb_TRIM             ( FBSTRING *src );
FBCALL FBSTRING    *fb_TrimEx           ( FBSTRING *str, FBSTRING *pattern );
FBCALL FBSTRING    *fb_TrimAny          ( FBSTRING *str, FBSTRING *pattern );
FBCALL FBSTRING    *fb_LCASE            ( FBSTRING *str );
FBCALL FBSTRING    *fb_UCASE            ( FBSTRING *str );
FBCALL FBSTRING    *fb_StrFill1         ( int cnt, int fchar );
FBCALL FBSTRING    *fb_StrFill2         ( int cnt, FBSTRING *src );
FBCALL int          fb_StrInstr         ( int start, FBSTRING *src, FBSTRING *patt );
FBCALL int          fb_StrInstrAny      ( int start, FBSTRING *src, FBSTRING *patt );
FBCALL int          fb_StrInstrRev      ( FBSTRING *src, FBSTRING *patt, int start );
FBCALL int          fb_StrInstrRevAny   ( FBSTRING *src, FBSTRING *patt, int start );
FBCALL FBSTRING    *fb_StrMid           ( FBSTRING *src, int start, int len );
FBCALL void         fb_StrAssignMid     ( FBSTRING *dst, int start, int len, FBSTRING *src );

/**************************************************************************************************
 * Unicode strings
 **************************************************************************************************/

FBCALL void         fb_WstrDelete       ( FB_WCHAR *str );
FBCALL FB_WCHAR    *fb_WstrAssign       ( FB_WCHAR *dst, int dst_chars, FB_WCHAR *src );
FBCALL FB_WCHAR    *fb_WstrAssignFromA 	( FB_WCHAR *dst, int dst_chars, void *src, 
										  int src_chars );
FBCALL void 	   *fb_WstrAssignToA	( void *dst, int dst_chars, FB_WCHAR *src, int fill_rem );
FBCALL void 	   *fb_WstrAssignToAEx	( void *dst, int dst_chars, FB_WCHAR *src, 
										  int fill_rem, int is_init );
FBCALL FB_WCHAR    *fb_WstrConcat 		( const FB_WCHAR *str1, const FB_WCHAR *str2 );
FBCALL FB_WCHAR    *fb_WstrConcatWA 	( const FB_WCHAR *str1, const void *str2,
										  int str2_size );
FBCALL FB_WCHAR    *fb_WstrConcatAW 	( const void *str1, int str1_size,
										  const FB_WCHAR *str2 );
FBCALL FB_WCHAR    *fb_WstrConcatAssign ( FB_WCHAR *dst, int dst_chars,
										  const FB_WCHAR *src );

FBCALL int          fb_WstrLen          ( FB_WCHAR *str );
FBCALL int 			fb_WstrCompare 		( const FB_WCHAR *str1, const FB_WCHAR *str2 );

FBCALL FB_WCHAR    *fb_IntToWstr     	( int num );
FBCALL FB_WCHAR    *fb_UIntToWstr    	( unsigned int num );
FBCALL FB_WCHAR    *fb_FloatToWstr   	( float num );
       FB_WCHAR    *fb_FloatExToWstr  	( double val, FB_WCHAR *buffer, int digits,
       									  int mask );
FBCALL FB_WCHAR    *fb_DoubleToWstr  	( double num );
FBCALL FB_WCHAR    *fb_StrToWstr		( const char *src );

FBCALL FBSTRING    *fb_WstrToStr		( const FB_WCHAR *src );
FBCALL double       fb_WstrToDouble     ( const FB_WCHAR *src, int len );
FBCALL int          fb_WstrToInt        ( const FB_WCHAR *src, int len );
FBCALL unsigned int fb_WstrToUInt       ( const FB_WCHAR *src, int len );
FBCALL long long    fb_WstrToLongint    ( const FB_WCHAR *src, int len );
FBCALL unsigned long long fb_WstrToULongint ( const FB_WCHAR *src, int len );
FBCALL int          fb_WstrRadix2Int    ( const FB_WCHAR *src, int len, int radix );
FBCALL long long 	fb_WstrRadix2Longint( const FB_WCHAR *s, int len, int radix );

FB_WCHAR    	   *fb_WstrChr			( int args, ... );
FBCALL unsigned int fb_WstrAsc          ( const FB_WCHAR *str, int pos );
FBCALL double       fb_WstrVal          ( const FB_WCHAR *str );
FBCALL int			fb_WstrValInt       ( const FB_WCHAR *str );
FBCALL unsigned int	fb_WstrValUInt      ( const FB_WCHAR *str );
FBCALL long long	fb_WstrValLng       ( const FB_WCHAR *str );
FBCALL unsigned long long fb_WstrValULng( const FB_WCHAR *str );
FBCALL FB_WCHAR    *fb_WstrLeft         ( const FB_WCHAR *str, int chars );
FBCALL FB_WCHAR    *fb_WstrRight        ( const FB_WCHAR *str, int chars );
FBCALL FB_WCHAR    *fb_WstrSpace        ( int chars );
FBCALL FB_WCHAR    *fb_WstrLTrim        ( const FB_WCHAR *str );
FBCALL FB_WCHAR    *fb_WstrLTrimEx      ( const FB_WCHAR *str, const FB_WCHAR *pattern );
FBCALL FB_WCHAR    *fb_WstrLTrimAny     ( const FB_WCHAR *str, const FB_WCHAR *pattern );
FBCALL FB_WCHAR    *fb_WstrRTrim        ( const FB_WCHAR *str );
FBCALL FB_WCHAR    *fb_WstrRTrimEx      ( const FB_WCHAR *str, const FB_WCHAR *pattern );
FBCALL FB_WCHAR    *fb_WstrRTrimAny     ( const FB_WCHAR *str, const FB_WCHAR *pattern );
FBCALL FB_WCHAR    *fb_WstrTrim         ( const FB_WCHAR *src );
FBCALL FB_WCHAR    *fb_WstrTrimEx       ( const FB_WCHAR *str, const FB_WCHAR *pattern );
FBCALL FB_WCHAR    *fb_WstrTrimAny      ( const FB_WCHAR *str, const FB_WCHAR *pattern );
FBCALL FB_WCHAR    *fb_WstrLcase        ( const FB_WCHAR *str );
FBCALL FB_WCHAR    *fb_WstrUcase        ( const FB_WCHAR *str );
FBCALL FB_WCHAR    *fb_WstrFill1        ( int cnt, FB_WCHAR c );
FBCALL FB_WCHAR    *fb_WstrFill2        ( int cnt, const FB_WCHAR *src );
FBCALL int          fb_WstrInstr        ( int start, const FB_WCHAR *src,
										  const FB_WCHAR *patt );
FBCALL int          fb_WstrInstrAny     ( int start, const FB_WCHAR *src,
										  const FB_WCHAR *patt );
FBCALL int          fb_WstrInstrRev     ( const FB_WCHAR *src, 
										  const FB_WCHAR *patt, int start );
FBCALL int          fb_WstrInstrRevAny  ( const FB_WCHAR *src,
										  const FB_WCHAR *patt, int start );
FBCALL FB_WCHAR    *fb_WstrMid          ( const FB_WCHAR *src, int start, int len );
FBCALL void         fb_WstrAssignMid    ( FB_WCHAR *dst, int dst_len, int start, int len,
										  const FB_WCHAR *src );
/**************************************************************************************************
 * VB-compatible functions
 **************************************************************************************************/

FBCALL FBSTRING *   fb_StrFormat        ( double value, FBSTRING *mask );

FBCALL FBSTRING *   fb_hStrFormat       ( double value, const char *mask,
                                          size_t mask_length );

#endif /* __FB_STRING_H__ */
