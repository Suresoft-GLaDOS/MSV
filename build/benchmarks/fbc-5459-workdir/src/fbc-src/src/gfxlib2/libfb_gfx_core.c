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
 * core.c -- core internal routines
 *
 * chng: jan/2005 written [lillo]
 *       feb/2005 added gfx target handling [lillo]
 *
 */

#include "fb_gfx.h"

static void (*fb_hPutPixelSolid)(FB_GFXCTX *ctx, int x, int y, unsigned int color);
static void *(*fb_hPixelSetSolid)(void *dest, int color, size_t size);
static void (*fb_hPutPixelAlpha)(FB_GFXCTX *ctx, int x, int y, unsigned int color);
static void *(*fb_hPixelSetAlpha)(void *dest, int color, size_t size);
static unsigned int (*fb_hGetPixel)(struct FB_GFXCTX *ctx, int x, int y);

#if defined(TARGET_X86)
extern void *fb_hPixelSet2MMX(void *dest, int color, size_t size);
extern void *fb_hPixelSet4MMX(void *dest, int color, size_t size);
extern void *fb_hPixelSetAlpha4MMX(void *dest, int color, size_t size);
extern void fb_hPutPixelAlpha4MMX(FB_GFXCTX *ctx, int x, int y, unsigned int color);
#endif


void fb_hPostEvent_code_start(void) { }


/*:::::*/
void fb_hPostEvent(EVENT *e)
{
	EVENT *slot;
	
	EVENT_LOCK();
	slot = &__fb_gfx->event_queue[__fb_gfx->event_tail];
	fb_hMemCpy(slot, e, sizeof(EVENT));
	if (((__fb_gfx->event_tail + 1) & (MAX_EVENTS - 1)) == __fb_gfx->event_head)
		__fb_gfx->event_head = (__fb_gfx->event_head + 1) & (MAX_EVENTS - 1);
	__fb_gfx->event_tail = (__fb_gfx->event_tail + 1) & (MAX_EVENTS - 1);
	EVENT_UNLOCK();
}

void fb_hPostEvent_code_end(void) { }


/*:::::*/
FB_GFXCTX *fb_hGetContext(void)
{
	FB_GFXCTX *context;
	
	context = (FB_GFXCTX *)fb_TlsGetCtx(FB_TLSKEY_GFX, sizeof(FB_GFXCTX));
	if ((__fb_gfx) && (context->id != __fb_gfx->id)) {
		/* context has to be initialized; default to screen */
		if (context->line)
			free(context->line);
		fb_hMemSet(context, 0, sizeof(FB_GFXCTX));
		context->id = __fb_gfx->id;
		context->old_view_w = __fb_gfx->w;
		context->old_view_h = context->max_h = __fb_gfx->h;
		context->line = (unsigned char **)malloc(__fb_gfx->h * sizeof(unsigned char *));
        if ((__fb_gfx->depth > 4) && (__fb_gfx->depth <= 8))
            context->fg_color = 15;
        else
            context->fg_color = __fb_gfx->color_mask;
        context->bg_color = MASK_A_32 & __fb_gfx->color_mask;
		context->flags = CTX_BUFFER_INIT;
		fb_hPrepareTarget(context, NULL);
		fb_hSetPixelTransfer(context, MASK_A_32);
	}
	
	return context;
}


/*:::::*/
void fb_hPrepareTarget(FB_GFXCTX *context, void *target)
{
	PUT_HEADER *header;
	unsigned char *data;
	int i, h;
	
	if (target) {
		if (target != context->last_target) {
			if (context->last_target == NULL)
				fb_hMemCpy(context->old_view, context->view, sizeof(int) * 4);
			header = (PUT_HEADER *)target;
			context->view_x = 0;
			context->view_y = 0;
			if (header->type == PUT_HEADER_NEW) {
				context->view_w = header->width;
				context->view_h = h = header->height;
				context->target_bpp = header->bpp;
				context->target_pitch = header->pitch;
				fb_hSetupFuncs(header->bpp);
				data = (unsigned char *)target + sizeof(PUT_HEADER);
			}
			else {
				context->view_w = header->old.width;
				context->view_h = h = header->old.height;
				context->target_bpp = __fb_gfx->bpp;
				context->target_pitch = context->view_w * __fb_gfx->bpp;
				data = (unsigned char *)target + 4;
			}
			if (h > context->max_h) {
				context->line = (unsigned char **)realloc(context->line, h * sizeof(unsigned char *));
				context->max_h = h;
			}
			for (i = 0; i < h; i++)
				context->line[i] = data + (i * context->target_pitch);
			context->flags |= CTX_BUFFER_SET;
		}
	}
	else if (context->flags & (CTX_BUFFER_SET | CTX_BUFFER_INIT)) {
		fb_hMemCpy(context->view, context->old_view, sizeof(int) * 4);
		context->target_bpp = __fb_gfx->bpp;
		context->target_pitch = __fb_gfx->pitch;
		for (i = 0; i < __fb_gfx->h; i++)
			context->line[i] = __fb_gfx->page[context->work_page] + (i * __fb_gfx->pitch);
		fb_hSetupFuncs(__fb_gfx->bpp);
		context->flags &= ~(CTX_BUFFER_SET | CTX_BUFFER_INIT);
	}
	context->last_target = target;
}

void fb_hSetPixelTransfer(FB_GFXCTX *context, unsigned int color)
{
	if ((__fb_gfx->flags & ALPHA_PRIMITIVES) && (context->target_bpp == 4) && ((color & MASK_A_32) != MASK_A_32)) {
		context->put_pixel = fb_hPutPixelAlpha;
		context->pixel_set = fb_hPixelSetAlpha;
	}
	else {
		context->put_pixel = fb_hPutPixelSolid;
		context->pixel_set = fb_hPixelSetSolid;
	}
	context->get_pixel = fb_hGetPixel;
}

/*:::::*/
void fb_hTranslateCoord(FB_GFXCTX *context, float fx, float fy, int *x, int *y)
{
	if (context->flags & CTX_WINDOW_ACTIVE) {
		fx = ((fx - context->win_x) * context->view_w) / (context->win_w - 1);
		fy = ((fy - context->win_y) * context->view_h) / (context->win_h - 1);
	}
	
	*x = (int)(fx > 0 ? fx + 0.5 : fx - 0.5);
	*y = (int)(fy > 0 ? fy + 0.5 : fy - 0.5);
	
	if ((context->flags & (CTX_WINDOW_ACTIVE | CTX_WINDOW_SCREEN)) == CTX_WINDOW_ACTIVE)
		*y = context->view_h - 1 - *y;
		
	if ((context->flags & CTX_VIEW_SCREEN) == 0) {
		*x += context->view_x;
		*y += context->view_y;
	}
}


/*:::::*/
void fb_hFixRelative(FB_GFXCTX *context, int coord_type, float *x1, float *y1, float *x2, float *y2)
{
	coord_type &= COORD_TYPE_MASK;
	
	switch (coord_type) {
		
		case COORD_TYPE_R:
		case COORD_TYPE_RA:
			*x1 += context->last_x;
			*y1 += context->last_y;
			break;
		
		case COORD_TYPE_RR:
			*x1 += context->last_x;
			*y1 += context->last_y;
			/* fallthrough */
		
		case COORD_TYPE_AR:
			*x2 += *x1;
			*y2 += *y1;
			break;
	}
	
	if (x2) {
		context->last_x = *x2;
		context->last_y = *y2;
	}
	else {
		context->last_x = *x1;
		context->last_y = *y1;
	}
}


/*:::::*/
void fb_hFixCoordsOrder(int *x1, int *y1, int *x2, int *y2)
{
	if (*x2 < *x1)
		SWAP(*x1, *x2);
	
	if (*y2 < *y1)
		SWAP(*y1, *y2);
}


/*:::::*/
static void fb_hPutPixel1(FB_GFXCTX *ctx, int x, int y, unsigned int color)
{
	ctx->line[y][x] = color;
}


/*:::::*/
static void fb_hPutPixel2(FB_GFXCTX *ctx, int x, int y, unsigned int color)
{
	((unsigned short *)ctx->line[y])[x] = color;
}


/*:::::*/
static void fb_hPutPixel4(FB_GFXCTX *ctx, int x, int y, unsigned int color)
{
	((unsigned int *)ctx->line[y])[x] = color;
}


/*:::::*/
static void fb_hPutPixelAlpha4(FB_GFXCTX *ctx, int x, int y, unsigned int color)
{
	unsigned int dc, srb, sg, drb, dg, a;
	unsigned int *d = ((unsigned int *)ctx->line[y]) + x;
	
	dc = *d;
	a = color >> 24;
	srb = color & MASK_RB_32;
	sg = color & MASK_G_32;
	drb = dc & MASK_RB_32;
	dg = dc & MASK_G_32;
	srb = ((srb - drb) * a) >> 8;
	sg = ((sg - dg) * a) >> 8;
	*d = ((drb + srb) & MASK_RB_32) | ((dg + sg) & MASK_G_32) | (color & MASK_A_32);
}


/*:::::*/
static unsigned int fb_hGetPixel1(FB_GFXCTX *ctx, int x, int y)
{
	return ctx->line[y][x];
}


/*:::::*/
static unsigned int fb_hGetPixel2(FB_GFXCTX *ctx, int x, int y)
{
	return ((unsigned short *)ctx->line[y])[x];
}


/*:::::*/
static unsigned int fb_hGetPixel4(FB_GFXCTX *ctx, int x, int y)
{
	return ((unsigned int *)ctx->line[y])[x];
}


/*:::::*/
static void *fb_hPixelSet2(void *dest, int color, size_t size)
{
	unsigned short *d = (unsigned short *)dest;
	
	for (; size; size--)
		*d++ = color;
	
	return dest;
}


/*:::::*/
static void *fb_hPixelSet4(void *dest, int color, size_t size)
{
	unsigned int *d = (unsigned int *)dest;
	
	for (; size; size--)
		*d++ = color;
	
	return dest;
}


/*:::::*/
void *fb_hPixelSetAlpha4(void *dest, int color, size_t size)
{
	unsigned int sc, srb, sg, sa, dc, drb, dg, a, irb, ig;
	unsigned int *d = (unsigned int *)dest;
	
	sc = (unsigned int)color;
	srb = sc & MASK_RB_32;
	sg = sc & MASK_G_32;
	sa = sc & MASK_A_32;
	a = sc >> 24;
	for (; size; size--) {
		dc = *d;
		drb = dc & MASK_RB_32;
		dg = dc & MASK_G_32;
		irb = ((srb - drb) * a) >> 8;
		ig = ((sg - dg) * a) >> 8;
		dc = ((drb + irb) & MASK_RB_32) | ((dg + ig) & MASK_G_32) | sa;
		*d++ = dc;
	}
	return dest;
}


/*:::::*/
static void *fb_hPixelCpy2(void *dest, const void *src, size_t size)
{
	return fb_hMemCpy(dest, src, size << 1);
}


/*:::::*/
static void *fb_hPixelCpy4(void *dest, const void *src, size_t size)
{
	return fb_hMemCpy(dest, src, size << 2);
}


/*:::::*/
void fb_hSetupFuncs(int bpp)
{
#if defined(TARGET_X86)
	if (fb_CpuDetect() & 0x800000) {
		__fb_gfx->flags |= HAS_MMX;
		fb_hMemCpy = fb_hMemCpyMMX;
		fb_hMemSet = fb_hMemSetMMX;
	}
	else {
#endif
		fb_hMemCpy = memcpy;
		fb_hMemSet = memset;
#if defined(TARGET_X86)
	}
#endif
	switch (bpp) {
		case 1:
			fb_hPutPixelSolid = fb_hPutPixelAlpha = fb_hPutPixel1;
			fb_hGetPixel = fb_hGetPixel1;
			fb_hPixelSetSolid = fb_hPixelSetAlpha = fb_hMemSet;
			fb_hPixelCpy = fb_hMemCpy;
			break;
		
		case 2:
			fb_hPutPixelSolid = fb_hPutPixelAlpha = fb_hPutPixel2;
			fb_hGetPixel = fb_hGetPixel2;
#if defined(TARGET_X86)
			if (__fb_gfx->flags & HAS_MMX)
				fb_hPixelSetSolid = fb_hPixelSetAlpha = fb_hPixelSet2MMX;
			else
#endif
				fb_hPixelSetSolid = fb_hPixelSetAlpha = fb_hPixelSet2;
			fb_hPixelCpy = fb_hPixelCpy2;
			break;
		
		default:
			fb_hPutPixelSolid = fb_hPutPixel4;
			fb_hGetPixel = fb_hGetPixel4;
#if defined(TARGET_X86)
			if (__fb_gfx->flags & HAS_MMX)
				fb_hPixelSetSolid = fb_hPixelSet4MMX;
			else
#endif
				fb_hPixelSetSolid = fb_hPixelSet4;
			fb_hPixelCpy = fb_hPixelCpy4;
			if (__fb_gfx->flags & ALPHA_PRIMITIVES) {
#if defined(TARGET_X86)
				if (__fb_gfx->flags & HAS_MMX) {
					fb_hPutPixelAlpha = fb_hPutPixelAlpha4MMX;
					fb_hPixelSetAlpha = fb_hPixelSetAlpha4MMX;
				}
				else {
#endif
					fb_hPutPixelAlpha = fb_hPutPixelAlpha4;
					fb_hPixelSetAlpha = fb_hPixelSetAlpha4;
#if defined(TARGET_X86)
				}
#endif
			}
			else {
				fb_hPutPixelAlpha = fb_hPutPixelSolid;
				fb_hPixelSetAlpha = fb_hPixelSetSolid;
			}
			break;
	}
	
	fb_hPixelSet = fb_hPixelSetSolid;
}
