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

/*
 * str_comp.c -- string compare function
 *
 * chng: oct/2004 written [v1ctor]
 *
 */

#include <stddef.h>
#include <string.h>
#include "fb.h"

/*:::::*/
FBCALL int fb_StrCompare
	(
		void *str1,
		int str1_size,
		void *str2,
		int str2_size
	)
{
	const char *str1_ptr, *str2_ptr;
	int	str1_len, str2_len;
	int	res;

	/* both not null? */
	if( (str1 != NULL) && (str2 != NULL) )
	{
		FB_STRSETUP_FIX( str1, str1_size, str1_ptr, str1_len );
        FB_STRSETUP_FIX( str2, str2_size, str2_ptr, str2_len );

        res = FB_MEMCMP( str1_ptr,
                         str2_ptr,
                         ((str1_len < str2_len) ? str1_len : str2_len) );

        if( (res == 0) && (str1_len != str2_len) )
            res = (( str1_len > str2_len ) ? 1 : -1 );
	}
	/* left null? */
	else if( str1 == NULL )
	{
		/* right also null? return eq */
		if( str2 == NULL )
			res = 0;
		else
		{
			FB_STRSETUP_FIX( str2, str2_size, str2_ptr, str2_len );

			/* is right empty? return eq */
			if( str2_len == 0 )
				res = 0;

			/* else, return lt */
			else
				res = -1;
		}
	}
	/* only right is null */
	else
	{
		FB_STRSETUP_FIX( str1, str1_size, str1_ptr, str1_len );

		/* is left empty? return eq */
		if( str1_len == 0 )
			res = 0;
		/* else, return gt */
		else
			res = 1;
	}


	FB_STRLOCK();

	/* delete temps? */
	if( str1_size == -1 )
		fb_hStrDelTemp_NoLock( (FBSTRING *)str1 );
	if( str2_size == -1 )
		fb_hStrDelTemp_NoLock( (FBSTRING *)str2 );

	FB_STRUNLOCK();

	return res;
}


