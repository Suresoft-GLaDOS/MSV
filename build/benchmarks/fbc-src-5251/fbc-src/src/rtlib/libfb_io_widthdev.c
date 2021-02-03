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
 * io_widthdev.c -- set the with for devices
 *
 * chng: nov/2004 written [v1ctor]
 *
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fb.h"

int fb_DevLptTestProtocol( struct _FB_FILE *handle, const char *filename, size_t filename_len );
int fb_DevComTestProtocol( struct _FB_FILE *handle, const char *filename, size_t filename_len );

typedef struct _DEV_INFO_WIDTH {
    FB_LISTELEM     elem;
    char           *device;
    int             width;
} DEV_INFO_WIDTH;

/** Initialize the list of device info nodes.
 */
static void            fb_hListDevInit      ( FB_LIST *list )
{
    fb_hListDynInit( list );
}

/** Allocate a new device info node.
 *
 * @return pointer to the new node
 */
static DEV_INFO_WIDTH *fb_hListDevElemAlloc ( FB_LIST *list, const char *device, int width )
{
    DEV_INFO_WIDTH *node = (DEV_INFO_WIDTH*) calloc( 1, sizeof(DEV_INFO_WIDTH) );
    node->device = strdup(device);
    node->width = width;
    fb_hListDynElemAdd( list, &node->elem );
    return node;
}

#if 0
/** Remove the device info node and release its memory.
 */
static void            fb_hListDevElemFree  ( FB_LIST *list, DEV_INFO_WIDTH *node )
{
    fb_hListDynElemRemove( list, &node->elem );
    free(node->device);
    free(node);
}
#endif

/** Pointer to the device info list.
 */
static FB_LIST *dev_info_widths = NULL;

/*:::::*/
FBCALL int fb_WidthDev( FBSTRING *dev, int width )
{
    int cur = width;
    DEV_INFO_WIDTH *node;
    size_t i, size;
    char *device;

    FB_LOCK();

    /* create list of device info nodes (if not created yet) */
    if( dev_info_widths==NULL ) {
        dev_info_widths = malloc( sizeof(FB_LIST) );
        fb_hListDevInit( dev_info_widths );
    }

    FB_UNLOCK();

    /* */
    size = FB_STRSIZE(dev);
    device = alloca(size + 1);
    memcpy( device, dev->data, size );
    device[size] = 0;

    /* make the name uppercase */
    for( i=0; i!=size; ++i ) {
        unsigned ch = (unsigned) device[i];
        if( islower(ch) )
            device[i] = (char) toupper(ch);
    }

    FB_LOCK();

    /* Search list of devices for the requested device name */
    for (node = (DEV_INFO_WIDTH*) dev_info_widths->head;
         node != (DEV_INFO_WIDTH*) NULL;
         node = (DEV_INFO_WIDTH*) node->elem.next)
    {
        if( strcmp( device, node->device ) == 0 ) {
            break;
        }
    }

    if( width != -1 ) {
        if( node == NULL ) {
            /* Allocate a new list node if device name not found */
            node = fb_hListDevElemAlloc ( dev_info_widths, device, width );
        } else {
            /* Set device width */
            node->width = width;
        }
    } else if( node != NULL ) {
        cur = node->width;
    }

    /* search the width for all open (and known) devices */
    if( strcmp( device, "SCRN:" )==0 ) {
        /* SCREEN device */
        if( width!=-1 ) {
            fb_Width( width, -1 );
        }
        cur = FB_HANDLE_SCREEN->width;

    } else if ( fb_DevLptTestProtocol( NULL, device, size ) ) {
        /* PRINTER device */
        cur = fb_DevPrinterSetWidth( device, width, cur );

    } else if ( fb_DevComTestProtocol( NULL, device, size ) ) {
        /* SERIAL device */
        cur = fb_DevSerialSetWidth( device, width, cur );

    } else {
        /* unknown device */
    }

	FB_UNLOCK();

    if( width==-1 ) {
        return cur;
    }

    return fb_ErrorSetNum( FB_RTERROR_OK );
}
