/*
 *  libgfx2 - FreeBASIC's alternative gfx library
 *	Copyright (C) 2005 Angelo Mottola (a.mottola@libero.it)
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
*/

/*
 * put_trans.c -- TRANS drawing method for PUT statement
 *
 * chng: mar/2007 written [lillo]
 *
 */

#include "fb_gfx.h"


#if defined(TARGET_X86)

#include "fb_gfx_mmx.h"

extern void fb_hPutTrans1MMX(unsigned char *src, unsigned char *dest, int w, int h, int src_pitch, int dest_pitch, int alpha, BLENDER *blender, void *param);
extern void fb_hPutTrans2MMX(unsigned char *src, unsigned char *dest, int w, int h, int src_pitch, int dest_pitch, int alpha, BLENDER *blender, void *param);
extern void fb_hPutTrans4MMX(unsigned char *src, unsigned char *dest, int w, int h, int src_pitch, int dest_pitch, int alpha, BLENDER *blender, void *param);

#endif


/*:::::*/
void fb_hPutTrans1C(unsigned char *src, unsigned char *dest, int w, int h, int src_pitch, int dest_pitch, int alpha, BLENDER *blender, void *param)
{
	unsigned char *s = (unsigned char *)src;
	unsigned char *d;
	int x;
	
	src_pitch -= w;
	for (; h; h--) {
		d = (unsigned char *)dest;
		for (x = w; x; x--) {
			if (*s)
				*d = (unsigned int)*s;
			s++;
			d++;
		}
		s += src_pitch;
		dest += dest_pitch;
	}
}


/*:::::*/
static void fb_hPutTrans2C(unsigned char *src, unsigned char *dest, int w, int h, int src_pitch, int dest_pitch, int alpha, BLENDER *blender, void *param)
{
	unsigned short *s = (unsigned short *)src;
	unsigned short *d;
	int x;
	
	src_pitch = (src_pitch >> 1) - w;
	for (; h; h--) {
		d = (unsigned short *)dest;
		for (x = w; x; x--) {
			if (*s != MASK_COLOR_16)
				*d = (unsigned short)*s;
			s++;
			d++;
		}
		s += src_pitch;
		dest += dest_pitch;
	}
}


/*:::::*/
static void fb_hPutTrans4C(unsigned char *src, unsigned char *dest, int w, int h, int src_pitch, int dest_pitch, int alpha, BLENDER *blender, void *param)
{
	unsigned int *s = (unsigned int *)src;
	unsigned int *d, c;
	int x;
	
	src_pitch = (src_pitch >> 2) - w;
	for (; h; h--) {
		d = (unsigned int *)dest;
		for (x = w; x; x--) {
			c = *s & 0x00FFFFFF;
			if (c != MASK_COLOR_32)
				*d = c;
			s++;
			d++;
		}
		s += src_pitch;
		dest += dest_pitch;
	}
}


/*:::::*/
void fb_hPutTrans(unsigned char *src, unsigned char *dest, int w, int h, int src_pitch, int dest_pitch, int alpha, BLENDER *blender, void *param)
{
	static PUTTER *all_putters[] = {
		fb_hPutTrans1C, fb_hPutTrans2C, NULL, fb_hPutTrans4C,
#if defined(TARGET_X86)
		fb_hPutTrans1MMX, fb_hPutTrans2MMX, NULL, fb_hPutTrans4MMX,
#endif
	};
	PUTTER *putter;
	FB_GFXCTX *context = fb_hGetContext();
	
	if (!context->putter[PUT_MODE_TRANS]) {
#if defined(TARGET_X86)
		if (__fb_gfx->flags & HAS_MMX)
			context->putter[PUT_MODE_TRANS] = &all_putters[4];
		else
#endif
			context->putter[PUT_MODE_TRANS] = &all_putters[0];
	}
	putter = context->putter[PUT_MODE_TRANS][context->target_bpp - 1];
	
	putter(src, dest, w, h, src_pitch, dest_pitch, alpha, blender, param);
}
