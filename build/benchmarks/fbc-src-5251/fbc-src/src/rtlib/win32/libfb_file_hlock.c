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
 *	file_hlock - low-level lock and unlock functions for Windows
 *
 *  chng: jan/2005 written [v1ctor]
 *
 */


#include <io.h>
#include "fb.h"

#ifdef TARGET_CYGWIN
#include <stdio.h>
#define _get_osfhandle get_osfhandle
#define _fileno fileno
#endif

/*:::::*/
int fb_hFileLock( FILE *f, fb_off_t inipos, fb_off_t size )
{
	int res;

	res = LockFile( (HANDLE)_get_osfhandle( _fileno( f ) ), inipos, 0, size, 0 );

	return fb_ErrorSetNum( res == TRUE? FB_RTERROR_OK: FB_RTERROR_FILEIO );

}

/*:::::*/
int fb_hFileUnlock( FILE *f, fb_off_t inipos, fb_off_t size )
{
	int res;

	res = UnlockFile( (HANDLE)_get_osfhandle( _fileno( f ) ), inipos, 0, size, 0 );

	return fb_ErrorSetNum( res == TRUE? FB_RTERROR_OK: FB_RTERROR_FILEIO );

}






