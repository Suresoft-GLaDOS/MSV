/*
 *  libfb - FreeBASIC's runtime library
 *	Copyright (C) 2004-2011 The FreeBASIC development team.
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

/*
 * con_lineinp_wstr - console line input function for wstrings
 *
 * chng: nov/2005 written [v1ctor]
 *
 */

#include "fb.h"

static const char *pszDefaultQuestion = "? ";

#if defined(TARGET_WIN32) || defined(TARGET_CYGWIN) || defined(TARGET_DOS)

/*:::::*/
int fb_ConsoleLineInputWstr( const FB_WCHAR *text, FB_WCHAR *dst, int max_chars,
							 int addquestion, int addnewline )
{
    FBSTRING *tmp_result;

    /* !!!FIXME!!! no support for unicode input */

    FB_LOCK();

    fb_PrintBufferEx( NULL, 0, FB_PRINT_FORCE_ADJUST );

    if( text != NULL )
    {
        fb_PrintWstr( 0, text, 0 );

        if( addquestion != FB_FALSE )
            fb_PrintFixString( 0, pszDefaultQuestion, 0 );
    }

    FB_UNLOCK();

    tmp_result = fb_ConReadLine( FALSE );

    if( addnewline )
      fb_PrintVoid( 0, FB_PRINT_NEWLINE );

    if( tmp_result == NULL )
    	return fb_ErrorSetNum( FB_RTERROR_OUTOFMEM );

	fb_WstrAssignFromA( dst, max_chars, tmp_result, -1 );

	return fb_ErrorSetNum( FB_RTERROR_OK );
}

#else

static char *hWrapper( char *buffer, size_t count, FILE *fp )
{
    return fb_ReadString( buffer, count, fp );
}

/*:::::*/
int fb_ConsoleLineInputWstr( const FB_WCHAR *text, FB_WCHAR *dst, int max_chars,
							 int addquestion, int addnewline )
{
	int res;
    size_t len;
    int old_x, old_y;

    /* !!!FIXME!!! no support for unicode input */

    fb_PrintBufferEx( NULL, 0, FB_PRINT_FORCE_ADJUST );
    fb_GetXY( &old_x, &old_y );

	FB_LOCK();

    if( text != NULL )
    {
		fb_PrintWstr( 0, text, 0 );

        if( addquestion != FB_FALSE )
            fb_PrintFixString( 0, pszDefaultQuestion, 0 );
    }

    {
        FBSTRING str_result = { 0 };

        res = fb_DevFileReadLineDumb( stdin, &str_result, hWrapper );

        len = FB_STRSIZE(&str_result);

        if( !addnewline )
        {
            int cols, rows;
            int old_y;

            fb_GetSize( &cols, &rows );
            fb_GetXY( NULL, &old_y );

            old_x += len - 1;
            old_x %= cols;
            old_x += 1;
            old_y -= 1;

            fb_Locate( old_y, old_x, -1, 0, 0 );
        }

        fb_WstrAssignFromA( dst, max_chars, (void *)&str_result, -1 );

        fb_StrDelete( &str_result );
    }

	FB_UNLOCK();

    return res;
}

#endif
