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
 * x11.c -- x11 window management code shared by x11 and opengl drivers
 *
 * chng: jan/2005 written [lillo]
 *
 */

#ifdef WITH_X

#include "fb_gfx.h"
#include "fb_gfx_x11.h"
#include <unistd.h>


X11DRIVER fb_x11;

static pthread_t thread;
static pthread_mutex_t mutex;
static pthread_cond_t cond;

static Drawable root_window;
static Atom wm_delete_window, wm_intern_hints;
static Colormap color_map = None;
static Time last_click_time = 0;
static int orig_size, target_size, current_size, real_h;
static int orig_rate, target_rate;
static Rotation orig_rotation;
static Cursor blank_cursor, arrow_cursor = None;
static int is_running = FALSE, has_focus, cursor_shown, xlib_inited = FALSE;
static int mouse_x, mouse_y, mouse_wheel, mouse_buttons, mouse_on;
static int mouse_x_root, mouse_y_root;


/*:::::*/
static int calc_comp_height( int h )
{
	if( h < 240 )
		return 240;
	else if( h < 480 )
		return 480;
	else if( h < 600 )
		return 600;
	else if( h < 768 )
		return 768;
	else if( h < 1024 )
		return 1024;
	else
		return h;
}


/*:::::*/
static int key_repeated(XEvent *event)
{
	/* this function is shamelessly copied from SDL, which
	 * shamelessly copied it from yet another place :P
	 */
	XEvent peek_event;

	if (XPending(fb_x11.display)) {
		XPeekEvent(fb_x11.display, &peek_event);
		if ((peek_event.type == KeyPress) && (peek_event.xkey.keycode == event->xkey.keycode) &&
		    ((peek_event.xkey.time - event->xkey.time) < 2)) {
			XNextEvent(fb_x11.display, &peek_event);
			return TRUE;
		}
	}
	return FALSE;
}


/*:::::*/
static int translate_key(XEvent *event)
{
	unsigned char key[8];
	int k = 0;

	if (XLookupString(&(event->xkey), (char *)key, 8, NULL, NULL) == 1) {
		return key[0];
	}
	else {
		switch (XKeycodeToKeysym(fb_x11.display, event->xkey.keycode, 0)) {
			case XK_Up:			k = KEY_UP;			break;
			case XK_Down:		k = KEY_DOWN; 		break;
			case XK_Left:		k = KEY_LEFT;		break;
			case XK_Right:		k = KEY_RIGHT;		break;
			case XK_Insert:		k = KEY_INS;		break;
			case XK_Delete:		k = KEY_DEL;		break;
			case XK_Home:		k = KEY_HOME;		break;
			case XK_End:		k = KEY_END;		break;
			case XK_Page_Up:	k = KEY_PAGE_UP;	break;
			case XK_Page_Down:	k = KEY_PAGE_DOWN;	break;
			case XK_F1:			k = KEY_F(1);		break;
			case XK_F2:			k = KEY_F(2);		break;
			case XK_F3:			k = KEY_F(3);		break;
			case XK_F4:			k = KEY_F(4);		break;
			case XK_F5:			k = KEY_F(5);		break;
			case XK_F6:			k = KEY_F(6);		break;
			case XK_F7:			k = KEY_F(7);		break;
			case XK_F8:			k = KEY_F(8);		break;
			case XK_F9:			k = KEY_F(9);		break;
			case XK_F10:		k = KEY_F(10);		break;
		}
	}
	
	return k;
}


/*:::::*/
static void *window_thread(void *arg)
{
	XEvent event;
	EVENT e;
	
	(void)arg;
	
	is_running = TRUE;
	if (fb_x11.init())
		is_running = FALSE;
	cursor_shown = TRUE;
	mouse_x_root = -1;
	
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	
	while (is_running)
	{
		fb_hX11Lock();
		
		fb_x11.update();
		
		/* This line is causing fbgfx OpenGL code to freeze. - GOK
		 * XSync(fb_x11.display, False);
		 */
		while (XPending(fb_x11.display)) {
			e.type = 0;
			XNextEvent(fb_x11.display, &event);
			switch (event.type) {
				
				case FocusIn:
				case MapNotify:
					if (!has_focus) {
						has_focus = TRUE;
						e.type = EVENT_WINDOW_GOT_FOCUS;
					}
					/* fallthrough */
					
				case Expose:
					fb_hMemSet(__fb_gfx->dirty, TRUE, fb_x11.h);
					break;
				
				case FocusOut:
					fb_hMemSet(__fb_gfx->key, FALSE, 128);
					has_focus = mouse_on = FALSE;
					e.type = EVENT_WINDOW_LOST_FOCUS;
					break;
				
				case EnterNotify:
					if (has_focus) {
						mouse_on = TRUE;
						e.type = EVENT_MOUSE_ENTER;
					}
					break;
				
				case LeaveNotify:
					if (has_focus) {
						mouse_on = FALSE;
						e.type = EVENT_MOUSE_EXIT;
					}
					break;
				
				case MotionNotify:
					if (mouse_x_root < 0) {
						e.dx = e.dy = 0;
					}
					else {
						e.dx = event.xmotion.x_root - mouse_x_root;
						e.dy = event.xmotion.y_root - mouse_y_root;
					}
					mouse_x_root = event.xmotion.x_root;
					mouse_y_root = event.xmotion.y_root;
					mouse_x = event.xmotion.x;
					mouse_y = event.xmotion.y - fb_x11.display_offset;
					if ((mouse_y < 0) || (mouse_y >= fb_x11.h))
						mouse_on = FALSE;
					else
						mouse_on = TRUE;
					if (has_focus) {
						e.type = EVENT_MOUSE_MOVE;
						e.x = mouse_x;
						e.y = mouse_y;
					}
					break;
				
				case ButtonPress:
					e.type = EVENT_MOUSE_BUTTON_PRESS;
					switch (event.xbutton.button) {
						case Button1:	mouse_buttons |= BUTTON_LEFT  ; e.button = BUTTON_LEFT  ; break;
						case Button3:	mouse_buttons |= BUTTON_RIGHT ; e.button = BUTTON_RIGHT ; break;
						case Button2:	mouse_buttons |= BUTTON_MIDDLE; e.button = BUTTON_MIDDLE; break;
						case Button4:	e.z = mouse_wheel++; e.type = EVENT_MOUSE_WHEEL; break;
						case Button5:	e.z = mouse_wheel--; e.type = EVENT_MOUSE_WHEEL; break;
					}
					if ((event.xbutton.button != Button4) && (event.xbutton.button != Button5)) {
						if (event.xbutton.time - last_click_time < DOUBLE_CLICK_TIME)
							e.type = EVENT_MOUSE_DOUBLE_CLICK;
						last_click_time = event.xbutton.time;
					}
					break;
					
				case ButtonRelease:
					e.type = EVENT_MOUSE_BUTTON_RELEASE;
					switch (event.xbutton.button) {
						case Button1:	mouse_buttons &= ~BUTTON_LEFT  ; e.button = BUTTON_LEFT  ; break;
						case Button3:	mouse_buttons &= ~BUTTON_RIGHT ; e.button = BUTTON_RIGHT ; break;
						case Button2:	mouse_buttons &= ~BUTTON_MIDDLE; e.button = BUTTON_MIDDLE; break;
						default:		e.type = 0; break;
					}
					break;
				
				case ConfigureNotify:
					if ((event.xconfigure.width != fb_x11.w) || ((event.xconfigure.height != fb_x11.h) &&
																   (event.xconfigure.height != real_h))) {
						/* Window has been maximized: simulate ALT-Enter */
						__fb_gfx->key[0x1C] = __fb_gfx->key[0x38] = TRUE;
					}
					else
						break;
					/* fallthrough */

				case KeyPress:
					if (has_focus) {
						if (event.type == KeyPress) {
							e.scancode = fb_x11.keymap[event.xkey.keycode];
							e.ascii = 0;
							__fb_gfx->key[e.scancode] = TRUE;
						}
						if ((__fb_gfx->key[0x1C]) && (__fb_gfx->key[0x38]) && (!(fb_x11.flags & DRIVER_NO_SWITCH))) {
							fb_x11.exit();
							fb_x11.flags ^= DRIVER_FULLSCREEN;
							if (fb_x11.init()) {
								fb_x11.exit();
								fb_x11.flags ^= DRIVER_FULLSCREEN;
								fb_x11.init();
							}
							fb_hRestorePalette();
							fb_hMemSet(__fb_gfx->key, FALSE, 128);
						}
						else {
							e.ascii = translate_key(&event);
							if (e.ascii)
								fb_hPostKey(e.ascii);
						}
						if (event.type == KeyPress)
							e.type = EVENT_KEY_PRESS;
					}
					break;
				
				case KeyRelease:
					if (has_focus) {
						e.scancode = fb_x11.keymap[event.xkey.keycode];
						e.ascii = translate_key(&event);
						if (key_repeated(&event)) {
							if (e.ascii)
								fb_hPostKey(e.ascii);
							e.type = EVENT_KEY_REPEAT;
						}
						else {
							__fb_gfx->key[e.scancode] = FALSE;
							e.type = EVENT_KEY_RELEASE;
						}
					}
					break;
				
				/* Dead Code? The window is created without a ClientMessage attribute. - GOK
				case ClientMessage:
					if ((Atom)event.xclient.data.l[0] == wm_delete_window) {
						fb_hPostKey(KEY_QUIT);
						e.type = EVENT_WINDOW_CLOSE;
					}
					break;
				*/
			}
			if (e.type)
				fb_hPostEvent(&e);
		}
		
		pthread_cond_signal(&cond);
		
		fb_hX11Unlock();
		
		usleep(30000);
	}
	
	fb_x11.exit();
	
	return NULL;
}


/*:::::*/
int fb_hX11EnterFullscreen(int *h)
{
	if ((!fb_x11.config) || (target_size < 0))
		return -1;
	
	if (target_rate < 0) {
		if (XRRSetScreenConfig(fb_x11.display, fb_x11.config, root_window, target_size, orig_rotation, CurrentTime) == BadValue)
			return -1;
	}
	else {
		if (XRRSetScreenConfigAndRate(fb_x11.display, fb_x11.config, root_window, target_size, orig_rotation, target_rate, CurrentTime) == BadValue)
			return -1;
		__fb_gfx->refresh_rate = fb_x11.refresh_rate = target_rate;
	}
	
	XWarpPointer(fb_x11.display, None, fb_x11.window, 0, 0, 0, 0, fb_x11.w >> 1, real_h >> 1);
	XSync(fb_x11.display, False);
	while (XGrabPointer(fb_x11.display, fb_x11.window, True, 0,
			    GrabModeAsync, GrabModeAsync, fb_x11.window, None, CurrentTime) != GrabSuccess)
		usleep(10000);
	if (XGrabKeyboard(fb_x11.display, root_window, False,
	    GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess)
		return -1;

	current_size = target_size;
	*h = real_h;

	return 0;
}


/*:::::*/
void fb_hX11LeaveFullscreen(void)
{
	if ((!fb_x11.config) || (target_size < 0))
		return;
	
	if (current_size != orig_size) {
		if ((target_rate <= 0) || (XRRSetScreenConfigAndRate(fb_x11.display, fb_x11.config, root_window, orig_size, orig_rotation, orig_rate, CurrentTime) == BadValue))
			XRRSetScreenConfig(fb_x11.display, fb_x11.config, root_window, orig_size, orig_rotation, CurrentTime);
		XUngrabPointer(fb_x11.display, CurrentTime);
		XUngrabKeyboard(fb_x11.display, CurrentTime);
		current_size = orig_size;
		__fb_gfx->refresh_rate = fb_x11.refresh_rate = orig_rate;
	}
}

/*:::::*/
void WaitMapped(Window w)
{
	XEvent e;
	do {
		XMaskEvent(fb_x11.display, StructureNotifyMask, &e);
	} while ((e.type != MapNotify) || (e.xmap.event != w));
}

/*:::::*/
void fb_hX11InitWindow(int x, int y)
{
	XEvent event;
	
	if (!(fb_x11.flags & DRIVER_FULLSCREEN)){
		/* windowed */
		XResizeWindow(fb_x11.display, fb_x11.wmwindow, fb_x11.w, fb_x11.h);
		XResizeWindow(fb_x11.display, fb_x11.window, fb_x11.w, fb_x11.h);
	
		if (!(fb_x11.flags & DRIVER_NO_FRAME)) {
			XReparentWindow(fb_x11.display, fb_x11.window, fb_x11.wmwindow, 0, 0);
			XMapRaised(fb_x11.display,fb_x11.wmwindow);
			WaitMapped(fb_x11.wmwindow);
		}
		XMapRaised(fb_x11.display, fb_x11.window);
		if (fb_x11.flags & DRIVER_NO_FRAME)
			XMoveWindow(fb_x11.display, fb_x11.window, x, y);
		WaitMapped(fb_x11.window);
		XRaiseWindow(fb_x11.display, fb_x11.window);
	} else {
		/* fullscreen */
		XMoveResizeWindow(fb_x11.display, fb_x11.fswindow, 0, 0, fb_x11.w, fb_x11.h);
		XMoveResizeWindow(fb_x11.display, fb_x11.window, 0, 0, fb_x11.w, fb_x11.h);	
		XReparentWindow(fb_x11.display, fb_x11.window, fb_x11.fswindow, 0, 0);
		XMapRaised(fb_x11.display, fb_x11.fswindow);
		/* use XSync instead of WaitMapped for unmanaged windows */
		XSync(fb_x11.display, False);  
		XMapRaised(fb_x11.display, fb_x11.window);
		XSync(fb_x11.display, False);
		XRaiseWindow(fb_x11.display, fb_x11.window);
	}
	
	if (fb_x11.flags & DRIVER_ALWAYS_ON_TOP) {
		fb_hMemSet(&event, 0, sizeof(event));
		event.xclient.type = ClientMessage;
		event.xclient.send_event = True;
		event.xclient.message_type = XInternAtom(fb_x11.display, "_NET_WM_STATE", False);
		event.xclient.window = fb_x11.wmwindow;
		event.xclient.format = 32;
		event.xclient.data.l[0] = 1;
		event.xclient.data.l[1] = XInternAtom(fb_x11.display, "_NET_WM_STATE_ABOVE", False);
		XSendEvent(fb_x11.display, root_window, False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
	}
}


/*:::::*/
void fb_hXlibInit(void)
{
	if (!xlib_inited) {
		XInitThreads();
		xlib_inited = TRUE;
	}
}


/*:::::*/
int fb_hX11Init(char *title, int w, int h, int depth, int refresh_rate, int flags)
{
	XPixmapFormatValues *format;
	XSetWindowAttributes attribs;
	XWMHints hints;
	XpmAttributes xpm_attribs;
	XSizeHints *size;
	Pixmap pixmap;
	XColor color;
	XGCValues gc_values;
	XRRScreenSize *sizes;
	short *rates;
	int version, dummy;
	int i, j, num_formats, num_sizes, num_rates, supersized_h;
	int gc_mask, keycode_min, keycode_max;
	KeySym keysym;
	const char *intern_atoms[] = { "_MOTIF_WM_HINTS", "KWM_WIN_DECORATION", "_WIN_HINTS" };
	int intern_hints[] = { 0x2, 0, 0, 0, 0 };
	
	is_running = FALSE;
	fb_hXlibInit();
	
	fb_x11.w = w;
	fb_x11.h = h;
	fb_x11.flags = flags;
	fb_x11.refresh_rate = refresh_rate;
	
	target_size = -1;
	target_rate = -1;
	current_size = -1;
	supersized_h = calc_comp_height(fb_x11.h);
	
	color_map = None;
	arrow_cursor = None;
	wm_intern_hints = None;
	
	if (fb_x11.visual) {
		fb_x11.depth = depth;
	}
	else {
		fb_x11.display = XOpenDisplay(NULL);
		if (!fb_x11.display)
			return -1;
		fb_x11.screen = XDefaultScreen(fb_x11.display);
		fb_x11.visual = XDefaultVisual(fb_x11.display, fb_x11.screen);
		fb_x11.depth = XDefaultDepth(fb_x11.display, fb_x11.screen);
		format = XListPixmapFormats(fb_x11.display, &num_formats);
		for (i = 0; i < num_formats; i++) {
			if (format[i].depth == fb_x11.depth) {
				if (format[i].bits_per_pixel == 16)
					fb_x11.visual_depth = format[i].depth;
				else
					fb_x11.visual_depth = format[i].bits_per_pixel;
				break;
			}
		}
		XFree(format);
	}
	root_window = XDefaultRootWindow(fb_x11.display);
	
	attribs.border_pixel = attribs.background_pixel = XBlackPixel(fb_x11.display, fb_x11.screen);
	attribs.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
			     PointerMotionMask | FocusChangeMask | EnterWindowMask | LeaveWindowMask | ExposureMask | StructureNotifyMask;
	attribs.backing_store = NotUseful;
	attribs.colormap = XCreateColormap( fb_x11.display, root_window, fb_x11.visual, AllocNone);
	fb_x11.window = XCreateWindow(fb_x11.display, root_window, 0, 0, fb_x11.w, fb_x11.h,
					0, fb_x11.depth, InputOutput, fb_x11.visual,
					CWBackPixel | CWBorderPixel | CWEventMask | CWBackingStore | CWColormap, &attribs);
	fb_x11.wmwindow = XCreateWindow(fb_x11.display, root_window, 0, 0, fb_x11.w, fb_x11.h,
					0, fb_x11.depth, InputOutput, fb_x11.visual,
					CWBackPixel | CWBorderPixel | CWEventMask | CWBackingStore | CWColormap, &attribs);
	attribs.override_redirect = True;
	fb_x11.fswindow = XCreateWindow(fb_x11.display, root_window, 0, 0, fb_x11.w, fb_x11.h,
					0, fb_x11.depth, InputOutput, fb_x11.visual,
					CWOverrideRedirect | CWBackPixel | CWBorderPixel | CWEventMask | CWBackingStore | CWColormap, &attribs);
					
	if (!fb_x11.window)
		return -1;
	XStoreName(fb_x11.display, fb_x11.window, title);
	if (fb_program_icon) {
		hints.flags = IconPixmapHint | IconMaskHint;
		xpm_attribs.valuemask = XpmReturnAllocPixels | XpmReturnExtensions;
		XpmCreatePixmapFromData(fb_x11.display, fb_x11.window, fb_program_icon, &hints.icon_pixmap, &hints.icon_mask, &xpm_attribs);
		XSetWMHints(fb_x11.display, fb_x11.wmwindow, &hints);
	}
	
	size = XAllocSizeHints();
	size->flags = PBaseSize | PMinSize | PMaxSize | PResizeInc;
	size->min_width = size->base_width = fb_x11.w;
	size->min_height = size->base_height = fb_x11.h;
	if (flags & DRIVER_NO_SWITCH) {
		size->max_width = size->min_width;
		size->max_height = size->min_height;
	}
	else {
		size->max_width = XDisplayWidth(fb_x11.display, fb_x11.screen);
		size->max_height = XDisplayHeight(fb_x11.display, fb_x11.screen);
	}
	size->width_inc = 0x10000;
	size->height_inc = 0x10000;
	XSetWMNormalHints(fb_x11.display, fb_x11.window, size);
	XSetWMNormalHints(fb_x11.display, fb_x11.fswindow, size);
	size->max_width = size->min_width;
	size->max_height = size->min_height;
	XSetWMNormalHints(fb_x11.display, fb_x11.wmwindow, size);
	XFree(size);
	
	if (flags & DRIVER_NO_FRAME) {
		for (i = 0; i < 3; i++) {
			wm_intern_hints = XInternAtom(fb_x11.display, intern_atoms[i], True);
			if (wm_intern_hints != None) {
				XChangeProperty(fb_x11.display, fb_x11.window, wm_intern_hints, wm_intern_hints,
					32, PropModeReplace, (unsigned char *)&intern_hints[i], (i == 0) ? 5 : 1);
				break;
			}
		}
		if (wm_intern_hints == None)
			return -1;
	}
	
	wm_delete_window = XInternAtom(fb_x11.display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(fb_x11.display, fb_x11.wmwindow, &wm_delete_window, 1);
	
	if (fb_x11.visual->class == PseudoColor) {
		color_map = XCreateColormap(fb_x11.display, root_window, fb_x11.visual, AllocAll);
		XSetWindowColormap(fb_x11.display, fb_x11.window, color_map);
	}
	XClearWindow(fb_x11.display, fb_x11.window);
	
	pixmap = XCreatePixmap(fb_x11.display, fb_x11.window, 1, 1, 1);
	gc_mask = GCFunction | GCForeground | GCBackground;
	gc_values.function = GXcopy;
	gc_values.foreground = gc_values.background = 0;
	fb_x11.gc = XCreateGC(fb_x11.display, pixmap, gc_mask, &gc_values);
	XDrawPoint(fb_x11.display, pixmap, fb_x11.gc, 0, 0);
	XFreeGC(fb_x11.display, fb_x11.gc);
	color.pixel = color.red = color.green = color.blue = 0;
	color.flags = DoRed | DoGreen | DoBlue;
	blank_cursor = XCreatePixmapCursor(fb_x11.display, pixmap, pixmap, &color, &color, 0, 0);
	arrow_cursor = XCreateFontCursor(fb_x11.display, XC_left_ptr);
	XFreePixmap(fb_x11.display, pixmap);
	fb_x11.gc = DefaultGC(fb_x11.display, fb_x11.screen);
	XSync(fb_x11.display, False);
	
	if (XRRQueryExtension(fb_x11.display, &dummy, &dummy) &&
	    XRRQueryVersion(fb_x11.display, &version, &dummy) && (version >= 1)) {
		fb_x11.config = XRRGetScreenInfo(fb_x11.display, root_window);
		orig_size = current_size = XRRConfigCurrentConfiguration(fb_x11.config, &orig_rotation);
		orig_rate = XRRConfigCurrentRate(fb_x11.config);
		sizes = XRRConfigSizes(fb_x11.config, &num_sizes);
		for (i = 0; i < num_sizes; i++) {
			if (sizes[i].width == fb_x11.w) {
				if (sizes[i].height == fb_x11.h) {
					target_size = i;
					real_h = fb_x11.h;
					break;
				}
				else if (sizes[i].height == supersized_h) {
					target_size = i;
					real_h = supersized_h;
					break;
				}
			}
		}
		if ((fb_x11.refresh_rate > 0) && (target_size >= 0)) {
			rates = XRRConfigRates(fb_x11.config, target_size, &num_rates);
			for (i = 0; i < num_rates; i++) {
				if (rates[i] == fb_x11.refresh_rate) {
					target_rate = rates[i];
					break;
				}
			}
		}
		else
			fb_x11.refresh_rate = orig_rate;
		__fb_gfx->refresh_rate = fb_x11.refresh_rate;
	}
	
	XDisplayKeycodes(fb_x11.display, &keycode_min, &keycode_max);
	keycode_min = MAX(keycode_min, 0);
	keycode_max = MIN(keycode_max, 255);
	for (i = keycode_min; i <= keycode_max; i++) {
		keysym = XKeycodeToKeysym(fb_x11.display, i, 0);
		if (keysym != NoSymbol) {
			for (j = 0; (fb_keysym_to_scancode[j].scancode) && (fb_keysym_to_scancode[j].keysym != keysym); j++)
				;
			fb_x11.keymap[i] = fb_keysym_to_scancode[j].scancode;
		}
	}
	if (flags & DRIVER_FULLSCREEN) {
		mouse_on = TRUE;
		mouse_x = fb_x11.w >> 1;
		mouse_y = fb_x11.h >> 1;
	}
	else
		mouse_on = FALSE;
	mouse_buttons = mouse_wheel = 0;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_mutex_lock(&mutex);
	if (!pthread_create(&thread, NULL, window_thread, NULL)) {
		pthread_cond_wait(&cond, &mutex);
		pthread_mutex_unlock(&mutex);
		if (is_running)
			return 0;
		pthread_join(thread, NULL);
	}
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	
	return -1;
}


/*:::::*/
void fb_hX11Exit(void)
{
	if (is_running) {
		is_running = FALSE;
		pthread_join(thread, NULL);
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
	}
	if (fb_x11.display) {
		fb_hX11LeaveFullscreen();
		XSync(fb_x11.display, False);
		if (arrow_cursor != None) {
			XUndefineCursor(fb_x11.display, fb_x11.window);
			XFreeCursor(fb_x11.display, arrow_cursor);
			XFreeCursor(fb_x11.display, blank_cursor);
		}
		if (color_map != None)
			XFreeColormap(fb_x11.display, color_map);
		if (wm_intern_hints != None)
			XDeleteProperty(fb_x11.display, fb_x11.window, wm_intern_hints);
		if (fb_x11.window != None) XDestroyWindow(fb_x11.display, fb_x11.window);
		if (fb_x11.fswindow != None) XDestroyWindow(fb_x11.display, fb_x11.fswindow);
		if (fb_x11.wmwindow != None) XDestroyWindow(fb_x11.display, fb_x11.wmwindow);
		if (fb_x11.config) {
			XRRFreeScreenConfigInfo(fb_x11.config);
			fb_x11.config = NULL;
		}
		if(fb_x11.display) XCloseDisplay(fb_x11.display);
		fb_x11.display = NULL;
	}
}


/*:::::*/
void fb_hX11Lock(void)
{
	pthread_mutex_lock(&mutex);
	XLockDisplay(fb_x11.display);
}


/*:::::*/
void fb_hX11Unlock(void)
{
	XUnlockDisplay(fb_x11.display);
	pthread_mutex_unlock(&mutex);
}


/*:::::*/
void fb_hX11SetPalette(int index, int r, int g, int b)
{
	XColor color;
	
	if (fb_x11.visual->class == PseudoColor) {
		color.pixel = index;
		color.red = (r << 8) | r;
		color.green = (g << 8) | g;
		color.blue = (b << 8) | b;
		color.flags = DoRed | DoGreen | DoBlue;
		XStoreColors(fb_x11.display, color_map, &color, 1);
	}
}


/*:::::*/
void fb_hX11WaitVSync(void)
{
	usleep(1000000 / ((fb_x11.refresh_rate > 0) ? fb_x11.refresh_rate : 60));
}


/*:::::*/
int fb_hX11GetMouse(int *x, int *y, int *z, int *buttons, int *clip)
{
	Window root, child;
	int root_x, root_y, win_x, win_y;
	unsigned int buttons_mask;
	
	if ((!mouse_on) || (!has_focus))
		return -1;
	
	/* prefer XQueryPointer to have a more responsive mouse position retrieval */
	*z = mouse_wheel;
	if (XQueryPointer(fb_x11.display, fb_x11.window, &root, &child, &root_x, &root_y, &win_x, &win_y, &buttons_mask)) {
		*x = win_x;
		*y = win_y;
		*buttons = (buttons_mask & Button1Mask ? 0x1 : 0) |
				   (buttons_mask & Button3Mask ? 0x2 : 0) |
				   (buttons_mask & Button2Mask ? 0x4 : 0);
	}
	else {
		*x = mouse_x;
		*y = mouse_y;
		*buttons = mouse_buttons;
	}
	
	*clip = fb_x11.mouse_clip;
	
	return 0;
}


/*:::::*/
void fb_hX11SetMouse(int x, int y, int show, int clip)
{
	if ((x >= 0) && (has_focus)) {
		mouse_on = TRUE;
		mouse_x = MID(0, x, fb_x11.w - 1);
		mouse_y = MID(0, y, fb_x11.h - 1) + fb_x11.display_offset;
		XWarpPointer(fb_x11.display, None, fb_x11.window, 0, 0, 0, 0, mouse_x, mouse_y);
	}
	if ((show > 0) && (!cursor_shown)) {
		XUndefineCursor(fb_x11.display, fb_x11.window);
		XDefineCursor(fb_x11.display, fb_x11.window, arrow_cursor);
		cursor_shown = TRUE;
	}
	else if ((show == 0) && (cursor_shown)) {
		XUndefineCursor(fb_x11.display, fb_x11.window);
		XDefineCursor(fb_x11.display, fb_x11.window, blank_cursor);
		cursor_shown = FALSE;
	}
	if (clip == 0) {
		fb_x11.mouse_clip = FALSE;
		XUngrabPointer(fb_x11.display, CurrentTime);
	}
	else if (clip > 0) {
		fb_x11.mouse_clip = TRUE;
		while (1) {
			if (XGrabPointer(fb_x11.display, fb_x11.window, True, 0, GrabModeAsync, GrabModeAsync, fb_x11.window, None, CurrentTime) == GrabSuccess)
				break;
			usleep(100000);
		}
	}
}


/*:::::*/
void fb_hX11SetWindowTitle(char *title)
{
	XStoreName(fb_x11.display, fb_x11.wmwindow, title);
}


/*:::::*/
int fb_hX11SetWindowPos(int x, int y)
{
	Window window, root, parent, *children;
	XWindowAttributes attribs = { 0 };
	XSetWindowAttributes set_attribs;
	XEvent event;
	unsigned int num_children;
	int dx = 0, dy = 0;
	
	if (fb_x11.flags & DRIVER_FULLSCREEN)
		return 0;
	fb_hX11Lock();
	parent = fb_x11.window;
	do {
		window = parent;
		dx += attribs.x;
		dy += attribs.y;
		XGetWindowAttributes(fb_x11.display, window, &attribs);
		XQueryTree(fb_x11.display, window, &root, &parent, &children, &num_children);
		if (children) XFree(children);
	} while (parent != root_window);
	if (x == 0x80000000)
		x = attribs.x;
	else
		x -= dx;
	if (y == 0x80000000)
		y = attribs.y;
	else
		y -= dy;
	
	set_attribs.override_redirect = True;
	XChangeWindowAttributes(fb_x11.display, window, CWOverrideRedirect, &set_attribs);
	XMoveWindow(fb_x11.display, window, x, y);
	set_attribs.override_redirect = False;
	XChangeWindowAttributes(fb_x11.display, window, CWOverrideRedirect, &set_attribs);
	XClearWindow(fb_x11.display, fb_x11.wmwindow);
	
	/* remove any mouse motion events */
	while (XCheckWindowEvent(fb_x11.display, fb_x11.window, PointerMotionMask, &event))
		;
	fb_hX11Unlock();
	
	return ((attribs.x + dx) & 0xFFFF) | ((attribs.y + dy) << 16);
}


/*:::::*/
int *fb_hX11FetchModes(int depth, int *size)
{
	Display *dpy;
	XRRScreenConfiguration *cfg;
	XRRScreenSize *rr_sizes;
	int i, *sizes = NULL;

	if ((depth != 8) && (depth != 15) && (depth != 16) && (depth != 24) && (depth != 32))
		return NULL;

	if (fb_x11.display)
		dpy = fb_x11.display;
	else
		dpy = XOpenDisplay(NULL);
	if (!dpy)
		return NULL;
	
	if (fb_x11.config)
		cfg = fb_x11.config;
	else
		cfg = XRRGetScreenInfo(dpy, XDefaultRootWindow(dpy));
	if (!cfg)
		return NULL;
	
	rr_sizes = XRRConfigSizes(cfg, size);
	if ((rr_sizes) && (*size > 0)) {
		sizes = (int *)malloc(*size * sizeof(int));
		for (i = 0; i < *size; i++)
			sizes[i] = (rr_sizes[i].width << 16) | (rr_sizes[i].height);
	}	
	if (!fb_x11.config)
		XRRFreeScreenConfigInfo(cfg);
	if (!fb_x11.display)
		XCloseDisplay(dpy);
	
	return sizes;
}


/*:::::*/
int fb_hX11ScreenInfo(int *width, int *height, int *depth, int *refresh)
{
	XRRScreenConfiguration *cfg;
	Display *dpy;
	int dummy, version;
	
	dpy = XOpenDisplay(NULL);
	if (!dpy)
		return -1;

	*width = XDisplayWidth(dpy, XDefaultScreen(dpy));
	*height = XDisplayHeight(dpy, XDefaultScreen(dpy));
	*depth = XDefaultDepth(dpy, XDefaultScreen(dpy));
	if (XRRQueryExtension(dpy, &dummy, &dummy) &&
	    XRRQueryVersion(dpy, &version, &dummy) && (version >= 1)) {
	    cfg = XRRGetScreenInfo(dpy, XDefaultRootWindow(dpy));
		*refresh = XRRConfigCurrentRate(cfg);
		XRRFreeScreenConfigInfo(cfg);
	}
	else
		*refresh = 0;
	
	XCloseDisplay(dpy);

	return 0;
}


/*:::::*/
int fb_hGetWindowHandle(void)
{
	return (fb_x11.display ? (int)fb_x11.window : 0);
}

#else

/*:::::*/
int fb_hGetWindowHandle(void)
{
	return 0;
}

#endif

