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
 * fb_gfx_dos.h -- DOS gfxlib include file
 *
 * chng: apr/2005 written [DrV]
 *
 */

#ifndef __FB_GFX_DOS_H__
#define __FB_GFX_DOS_H__

#include "../fb_gfx.h"
#include "../../rtlib/dos/fb_dos.h"

#include <conio.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#include <sys/farptr.h>
#include <sys/nearptr.h>
#include <sys/movedata.h>

#include "vga.h"
#include "vesa.h"

/* macros */

#define MASK_LINEAR(addr)   ((addr) & 0x000FFFFF)
#define RM_TO_LINEAR(addr)  ((((addr) & 0xFFFF0000) >> 12) + ((addr) & 0xFFFF))
#define RM_OFFSET(addr)     ((addr) & 0xF)
#define RM_SEGMENT(addr)    (((addr) >> 4) & 0xFFFF)
#define SEGOFF_TO_RM(s, o)  (((s) << 4) + (o))

#define SCREENLIST(w, h) ((h) | (w) << 16)


/* globals */

typedef struct fb_dos_pal_t {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char pad;
} fb_dos_pal_t;

typedef struct fb_dos_t {
	
	int w;
	int h;
	int depth;
	int refresh;
	
	fb_dos_pal_t pal[256];
	int pal_dirty;
	int pal_first;
	int pal_last;
	
	int inited;
	int detected;
	int locked;
	int in_interrupt;
	
	int old_rows;
	int old_cols;
	
	int mouse_ok;
	int mouse_wheel_ok;
	int mouse_cursor;
	int mouse_clip;

	int bios_mode;
	
	void (*update)(void);
	unsigned int update_len;
	
	void (*draw_mouse)(void);
	void (*undraw_mouse)(void);
	void (*set_palette)(void);
	
	__dpmi_regs regs;

	unsigned timer_ticks;
	unsigned timer_step;
	
	int nearptr_ok;
	
	int Bpp; /* **bytes** per pixel */
	unsigned int w_bytes;	/* bytes per scanline */
	size_t size;

	int vesa_ok;
	VbeInfoBlock vesa_info;
	VesaModeInfo vesa_mode_info;
	VesaModeInfo *vesa_modes;
	int num_vesa_modes;
	int vesa_use_pm;
	unsigned long vesa_mmio_linear;
	unsigned int vesa_mmio_sel;

	int mouse_x_old;
	int mouse_y_old;
	int mouse_z_old;
	int mouse_buttons_old;
	char key_old[128];
	
	int palbuf_sel; /* real-mode palette transfer buffer for VESA VBE modes */
	int palbuf_seg;
	
} fb_dos_t;

extern fb_dos_t fb_dos;

/* mouse */

extern int fb_dos_get_mouse(int *x, int *y, int *z, int *buttons, int *clip);
extern void fb_dos_set_mouse(int x, int y, int cursor, int clip);
extern int fb_dos_update_mouse(void);

/* VGA */

extern void fb_dos_vga_wait_vsync(void);
extern void fb_dos_vga_set_palette(void);

/* VESA */

extern void fb_dos_vesa_detect(void);
extern int fb_dos_vesa_set_mode(int w, int h, int depth, int linear);
extern int *fb_dos_vesa_fetch_modes(int depth, int *size);
extern void vesa_get_pm_functions(void);
extern void fb_dos_vesa_set_palette(void);
extern void fb_dos_vesa_set_palette_end(void);

extern VesaPMInfo *fb_dos_vesa_pm_info;
extern intptr_t fb_dos_vesa_pm_bank_switcher;
extern intptr_t fb_dos_vesa_pm_set_palette;

/* shared */

extern void fb_dos_set_palette(int idx, int r, int g, int b);
extern void fb_dos_detect(void);
extern int fb_dos_init(char *title, int w, int h, int depth, int refresh_rate, int flags);
extern void fb_dos_exit(void);
extern void fb_dos_set_window_title(char *title);
extern void fb_dos_lock(void);
extern void fb_dos_unlock(void);

#endif	/* #ifndef __FB_GFX_DOS_H__ */
