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
 * io_pageset.c -- 'screen , pg, pg' (console, no gfx) function for Windows
 *
 * chng: sep/2007 written [v1ctor]
 *
 */

#include "fb.h"
#include "fb_colors.h"

/*:::::*/
HANDLE fb_hConsoleCreateBuffer( void )
{
	HANDLE hnd = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE,
									   		0,
									   		NULL,
									   		CONSOLE_TEXTMODE_BUFFER,
									   		NULL );
	if( hnd == NULL )
		return NULL;

	/* size must be the stdout ones */
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( __fb_con.outHandle, &csbi );
	SetConsoleScreenBufferSize( hnd, csbi.dwSize );

	return hnd;
}

/*:::::*/
static void hHideCursor( int pg1, int pg2 )
{
	CONSOLE_CURSOR_INFO cci;

	GetConsoleCursorInfo( __fb_con.outHandle, &cci );
	cci.bVisible = FALSE;

	SetConsoleCursorInfo( __fb_con.pgHandleTb[pg1], &cci );
	if( pg2 >= 0 )
		SetConsoleCursorInfo( __fb_con.pgHandleTb[pg2], &cci );
}

/*:::::*/
int fb_ConsolePageSet ( int active, int visible )
{
	fb_hConsoleGetHandle( FALSE );

	int res = __fb_con.active | (__fb_con.visible << 8);

	if( active >= 0 )
	{
		if( __fb_con.pgHandleTb[active] == NULL )
		{
            HANDLE hnd = fb_hConsoleCreateBuffer( );
            if( hnd == NULL )
            	return -1;
			else
				__fb_con.pgHandleTb[active] = hnd;
		}

		/* if page isn't visible, hide the cursor */
		if( active != __fb_con.visible )
		{
			hHideCursor( active, -1 );
		}

		__fb_con.active = active;
	}

	if( visible >= 0 )
	{
		if( __fb_con.pgHandleTb[visible] == NULL )
		{
            HANDLE hnd = fb_hConsoleCreateBuffer( );
            if( hnd == NULL )
            	return -1;
			else
				__fb_con.pgHandleTb[visible] = hnd;
		}

		if( __fb_con.visible != visible )
		{
            SetConsoleActiveScreenBuffer( __fb_con.pgHandleTb[visible] );

			/* if pages aren't the same, hide the cursor */
			if( visible != __fb_con.active )
			{
				hHideCursor( __fb_con.active, visible );
			}

			__fb_con.visible = visible;
		}
	}

	return res;
}


