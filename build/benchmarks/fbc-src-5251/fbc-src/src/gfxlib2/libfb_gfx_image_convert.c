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
 * image_convert.c -- bpp conversion
 *
 * chng: jan/2005 written [lillo]
 *
 */

#include "fb_gfx.h"

/* !!!FIXME!!! little-endian only */

/*:::::*/
void fb_image_convert_8to8(const unsigned char *src, unsigned char *dest, int w)
{
	for (; w; w--)
		*dest++ = *src++ & __fb_gfx->color_mask;
}


/*:::::*/
void fb_image_convert_8to16(const unsigned char *src, unsigned char *dest, int w)
{
	int r, g, b;
	unsigned short *d = (unsigned short *)dest;

	for (; w; w--) {
		r = __fb_gfx->device_palette[*src] & 0xFF;
		g = (__fb_gfx->device_palette[*src] >> 8) & 0xFF;
		b = (__fb_gfx->device_palette[*src] >> 16) & 0xFF;
		*d++ = (b >> 3) | ((g << 3) & 0x07E0) | ((r << 8) & 0xF800);
		src++;
	}
}


/*:::::*/
void fb_image_convert_8to32(const unsigned char *src, unsigned char *dest, int w)
{
	int r, g, b;
	unsigned int *d = (unsigned int *)dest;

	for (; w; w--) {
		r = __fb_gfx->device_palette[*src] & 0xFF;
		g = (__fb_gfx->device_palette[*src] >> 8) & 0xFF;
		b = (__fb_gfx->device_palette[*src] >> 16) & 0xFF;
		*d++ = 0xFF000000 | b | (g << 8) | (r << 16);
		src++;
	}
}

/*:::::*/
void fb_image_convert_24to16(const unsigned char *src, unsigned char *dest, int w)
{
	unsigned short *d = (unsigned short *)dest;
	for (; w; w--) {
		*d++ = (((unsigned short)src[0] << 8) & 0xF800) | (((unsigned short)src[1] << 3) & 0x07E0) | ((unsigned short)src[2] >> 3);
		src += 3;
	}
}


/*:::::*/
void fb_image_convert_24to32(const unsigned char *src, unsigned char *dest, int w)
{
	unsigned int *d = (unsigned int *)dest;

	for (; w; w--) {
		*d++ = 0xFF000000 | ((unsigned int)src[0] << 16) | ((unsigned int)src[1] << 8) | ((unsigned int)src[2]);
		src += 3;
	}
}

/*:::::*/
void fb_image_convert_32to16(const unsigned char *src, unsigned char *dest, int w)
{
	unsigned short *d = (unsigned short *)dest;
	unsigned int c;

	for (; w; w--)
	{
		c = *(unsigned int *)src & 0x00FFFFFF;
		*d++ = (unsigned short)((c >> (16+3)) | ((c >> 5) & 0x07E0) | ((c << 8) & 0xF800));
		src += sizeof( unsigned int );
	}
}

/*:::::*/
void fb_image_convert_32to32(const unsigned char *src, unsigned char *dest, int w)
{
	unsigned int *d = (unsigned int *)dest;
	unsigned int c;

	for (; w; w--)
	{
		c = *(unsigned int *)src;
		*d++ = (c >> 16) | (c & 0xFF00FF00) | (c << 16);
		src += sizeof( unsigned int );
	}
}

/*:::::*/
void fb_image_convert_24bgrto16(const unsigned char *src, unsigned char *dest, int w)
{
	unsigned short *d = (unsigned short *)dest;
	for (; w; w--) {
		*d++ = ((unsigned short)src[0] >> 3) | (((unsigned short)src[1] << 3) & 0x07E0) | (((unsigned short)src[2] << 8) & 0xF800);
		src += 3;
	}
}


/*:::::*/
void fb_image_convert_24bgrto32(const unsigned char *src, unsigned char *dest, int w)
{
	unsigned int *d = (unsigned int *)dest;

	for (; w; w--) {
		*d++ = 0xFF000000 | (*(unsigned int *)src & 0xFFFFFF);
		src += 3;
	}
}

/*:::::*/
void fb_image_convert_32bgrto16(const unsigned char *src, unsigned char *dest, int w)
{
	unsigned short *d = (unsigned short *)dest;
	const unsigned int *s = (const unsigned int *)src;

	for (; w; w--) {
		*d++ = (unsigned short)(((*s & 0xFF) >> 3) | ((*s >> 5) & 0x07E0) | ((*s >> 8) & 0xF800));
		s++;
	}
}

/*:::::*/
void fb_image_convert_32bgrto32(const unsigned char *src, unsigned char *dest, int w)
{
	fb_hMemCpy(dest, src, w << 2);
}

/*:::::*/
FBCALL void fb_GfxImageConvertRow( const unsigned char *src, int src_bpp,
								   unsigned char *dest, int dst_bpp,
								   int width, int isrgb )
{
	FBGFX_IMAGE_CONVERT convert = NULL;

	if (src_bpp <= 8)
	{
		switch (BYTES_PER_PIXEL( dst_bpp ))
		{
			case 1: convert = fb_image_convert_8to8;  break;
			case 2: convert = fb_image_convert_8to16; break;
			case 3:
			case 4: convert = fb_image_convert_8to32; break;
		}
	}
	else if (src_bpp == 24)
	{
		switch (BYTES_PER_PIXEL( dst_bpp ))
		{
			case 1: return;
			case 2: convert = (isrgb != 0? fb_image_convert_24to16: fb_image_convert_24bgrto16); break;
			case 3:
			case 4: convert = (isrgb != 0? fb_image_convert_24to32: fb_image_convert_24bgrto32); break;
		}
	}
	else if (src_bpp == 32)
	{
		switch (BYTES_PER_PIXEL( dst_bpp ))
		{
			case 1: return;
			case 2: convert = (isrgb != 0? fb_image_convert_32to16: fb_image_convert_32bgrto16); break;
			case 3:
			case 4: convert = (isrgb != 0? fb_image_convert_32to32: fb_image_convert_32bgrto32); break;
		}
	}
	else
		return;

	convert( src, dest, width );
}
