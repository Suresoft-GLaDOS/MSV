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

#ifndef __FB_COLOR_H__
#define __FB_COLOR_H__

#ifdef WIN32


#define FB_COLOR_BLACK 		(0)
#define FB_COLOR_BLUE   	(FOREGROUND_BLUE)
#define FB_COLOR_GREEN     	(FOREGROUND_GREEN)
#define FB_COLOR_CYAN   	(FOREGROUND_GREEN|FOREGROUND_BLUE)
#define FB_COLOR_RED       	(FOREGROUND_RED)
#define FB_COLOR_MAGENTA   	(FOREGROUND_RED|FOREGROUND_BLUE)
#define FB_COLOR_BROWN     	(FOREGROUND_RED|FOREGROUND_GREEN)
#define FB_COLOR_WHITE     	(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)
#define FB_COLOR_GREY   	(FOREGROUND_INTENSITY)
#define FB_COLOR_LBLUE     	(FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define FB_COLOR_LGREEN    	(FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define FB_COLOR_LCYAN     	(FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define FB_COLOR_LRED   	(FOREGROUND_RED|FOREGROUND_INTENSITY)
#define FB_COLOR_LMAGENTA  	(FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY)
#define FB_COLOR_YELLOW    	(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY)
#define FB_COLOR_BWHITE    	(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY)

#elif defined(DJGPP)

#define FB_COLOR_BLACK 		(0)
#define FB_COLOR_BLUE   	(1)
#define FB_COLOR_GREEN     	(2)
#define FB_COLOR_CYAN   	(3)
#define FB_COLOR_RED       	(4)
#define FB_COLOR_MAGENTA   	(5)
#define FB_COLOR_BROWN     	(6)
#define FB_COLOR_WHITE     	(7)
#define FB_COLOR_GREY   	(8)
#define FB_COLOR_LBLUE     	(9)
#define FB_COLOR_LGREEN    	(10)
#define FB_COLOR_LCYAN     	(11)
#define FB_COLOR_LRED   	(12)
#define FB_COLOR_LMAGENTA  	(13)
#define FB_COLOR_YELLOW    	(14)
#define FB_COLOR_BWHITE    	(15)

#endif /* WIN32 */

#endif /* __FB_COLOR_H__ */
