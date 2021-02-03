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
 *	dev_file_eof - detects EOF for file device
 *
 * chng: jul/2005 written [mjs]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fb.h"

/*:::::*/
int fb_DevFileEof
	(
		struct _FB_FILE *handle
	)
{
    int res;
    FILE *fp;

	FB_LOCK();

    fp = (FILE*) handle->opaque;

	if( fp == NULL )
	{
		FB_UNLOCK();
		return FB_TRUE;
	}

    res = FB_FALSE;

    switch( handle->mode )
    {
    /* non-text mode? */
    case FB_FILE_MODE_BINARY:
    case FB_FILE_MODE_RANDOM:
        /* note: handle->putback_size will be checked by fb_FileEofEx() */
        res = ( ftell( fp ) >= handle->size? FB_TRUE : FB_FALSE );
        break;

    /* text-mode (INPUT, OUTPUT or APPEND) */
    default:
    	/* try feof() first, because the EOF char (27) */
    	if( feof( fp ) )
    	{
        	res = FB_TRUE;
    	}
		/* if in input mode, feof() won't return TRUE if file_pos == file_size */
		else if( handle->mode == FB_FILE_MODE_INPUT )
		{
        	int has_size = handle->hooks->pfnTell!=NULL && handle->hooks->pfnSeek!=NULL;
        	/* note: fseek() is unreliable in text-mode, sise must be calculated
                 	 re-opening the file in binary mode */
        	if( has_size )
        		if( ftell( fp ) >= handle->size )
            		res = FB_TRUE;
    	}
    }

	FB_UNLOCK();

	return res;
}
