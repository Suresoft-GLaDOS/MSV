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
 * joystick.c -- joystick handling, linux
 *
 * chng: apr/2005 written [lillo]
 *
 */

#include "fb_gfx.h"

#include "fb_gfx_linux.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>


/* From linux/joystick.h */

#define JS_EVENT_BUTTON         0x01
#define JS_EVENT_AXIS           0x02
#define JS_EVENT_INIT           0x80

typedef struct _JS_EVENT {
        unsigned int time;
        short value;
        unsigned char type;
        unsigned char number;
} JS_EVENT;

#define JSIOCGVERSION           _IOR('j', 0x01, unsigned int)

/*:::::*/

typedef struct _JOYDATA {
	int fd;
	float axis[8];
	int buttons;
} JOYDATA;

static JOYDATA joydata[16];
static int inited = FALSE;


/*:::::*/
FBCALL int fb_GfxGetJoystick(int id, int *buttons, float *a1, float *a2, float *a3, float *a4, float *a5, float *a6, float *a7, float *a8)
{
	const char *device[] = { "/dev/input/js",
							 "/dev/js",
							 NULL };
	char device_name[16];
	JOYDATA *joy;
	JS_EVENT event;
	int i, j, k, count = 0;
	int version;

	if (!inited) {
		fb_hMemSet(joydata, 0, sizeof(JOYDATA) * 16);
		for (i = 0; i < 16; i++)
			joydata[i].fd = -1;
		joy = joydata;
		for (i = 0; device[i] && (count < 16); i++) {
			for (j = 0; (j < 16) && (count < 16); j++) {
				sprintf(device_name, "%s%d", device[i], j);
				joy->fd = open(device_name, O_NONBLOCK);
				if (joy->fd >= 0) {
					ioctl(joy->fd, JSIOCGVERSION, &version);
					if (version < 0x10000) {
						close(joy->fd);
						continue;
					}
					for (k = 0; k < 8; k++)
						joy->axis[k] = -1000.0;
					joy++;
					count++;
				}
			}
		}
		inited = TRUE;
	}

	*buttons = -1;
	*a1 = *a2 = *a3 = *a4 = *a5 = *a6 = *a7 = *a8 = -1000.0;

	if ((id < 0) || (id > 15))
		return fb_ErrorSetNum(FB_RTERROR_ILLEGALFUNCTIONCALL);
	joy = &joydata[id];

	if (joy->fd < 0)
		return fb_ErrorSetNum(FB_RTERROR_ILLEGALFUNCTIONCALL);

	while (read(joy->fd, &event, sizeof(event)) > 0) {
		switch (event.type & ~JS_EVENT_INIT) {

			case JS_EVENT_AXIS:
				if (event.number < 8)
					joy->axis[event.number] = (float)event.value / 32767.0;
				break;

			case JS_EVENT_BUTTON:
				if (event.number < 32) {
					if (event.value)
						joy->buttons |= (1 << event.number);
					else
						joy->buttons &= ~(1 << event.number);
				}
				break;
		}
	}

	*a1 = joy->axis[0];
	*a2 = joy->axis[1];
	*a3 = joy->axis[2];
	*a4 = joy->axis[3];
	*a5 = joy->axis[4];
	*a6 = joy->axis[5];
	*a7 = joy->axis[6];
	*a8 = joy->axis[7];
	*buttons = joy->buttons;

	return fb_ErrorSetNum( FB_RTERROR_OK );
}

