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
 * inkey.c -- inkey$ handling
 *
 * chng: jan/2005 written [lillo]
 *
 */

#include "fb_gfx.h"


static int key_buffer[KEY_BUFFER_LEN], key_head = 0, key_tail = 0;
static int key_buffer_changed = FALSE;


/*:::::*/
int fb_hGfxInputBufferChanged( void )
{
	int res;

	DRIVER_LOCK();

    res = key_buffer_changed;
    key_buffer_changed = FALSE;

	DRIVER_UNLOCK();

	return res;
}

/*:::::*/
void fb_hPostKey(int key)
{
	key_buffer[key_tail] = key;
	if (((key_tail + 1) & (KEY_BUFFER_LEN - 1)) == key_head)
		key_head = (key_head + 1) & (KEY_BUFFER_LEN - 1);
    key_tail = (key_tail + 1) & (KEY_BUFFER_LEN - 1);
    key_buffer_changed = TRUE;
}

#ifdef __DJGPP__
void fb_hPostKey_End(void)
{ /* this function is here to get the length of the fb_hPostKey function so
     the DOS gfxlib driver can lock it into physical memory for use in an
     interrupt handler */ }
#endif


/*:::::*/
static int get_key(void)
{
	int key = 0;

	DRIVER_LOCK();

	if (key_head != key_tail) {
		key = key_buffer[key_head];
		key_head = (key_head + 1) & (KEY_BUFFER_LEN - 1);
	}

    /* Reset the status for "key buffer changed" when a key
     * was removed from the input queue. */
    fb_hGfxInputBufferChanged();

	DRIVER_UNLOCK();

	return key;
}


/*:::::*/
int fb_GfxGetkey(void)
{
	int key = 0;

	if (!__fb_gfx)
		return 0;

	do {
		key = get_key();
		fb_Sleep(20);
	} while (key == 0);

	return key;
}

/*:::::*/
int fb_GfxKeyHit(void)
{
	int res;

	DRIVER_LOCK();

	res = (key_head != key_tail? 1: 0);

	DRIVER_UNLOCK();

	return res;
}

/*:::::*/
FBSTRING *fb_GfxInkey(void)
{
	const unsigned char code[KEY_MAX_SPECIALS] = {
		'k', 'H', 'P', 'K', 'M', 'R', 'S', 'G', 'O', 'I', 'Q',
		';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D'
	};
	FBSTRING *res;
	int key;

	if ((__fb_gfx) && (key = get_key())) {
        if (key > 0xFF) {
            res = (FBSTRING *)fb_hStrAllocTmpDesc();
            fb_hStrAllocTemp(res, 2);
            if( (key & 0xFF)==0xFF ) {
                res->data[1] = (char) (key >> 8);
            } else {
                key = MIN(key - 0x100, KEY_MAX_SPECIALS - 1);
                res->data[1] = code[key];
            }
            res->data[0] = FB_EXT_CHAR;
			res->data[2] = '\0';

			return res;
		}
		else
			return fb_CHR( 1, key );
	}

	return &__fb_ctx.null_desc;
}

/*:::::*/
int fb_GfxIsRedir(int is_input)
{
	return FALSE;
}

