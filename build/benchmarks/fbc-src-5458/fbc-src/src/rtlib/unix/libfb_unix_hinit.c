/*
 *  libfb - FreeBASIC's runtime library
 *	Copyright (C) 2004-2010 The FreeBASIC development team.
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
 * init.c -- libfb initialization for Linux
 *
 * chng: jan/2005 written [lillo]
 *       feb/2005 rewritten to remove ncurses dependency [lillo]
 *
 */

#include "fb.h"
#include "fb_unix.h"

#ifdef MULTITHREADED
pthread_mutex_t __fb_global_mutex;
pthread_mutex_t __fb_string_mutex;
extern int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);
#endif

FBCONSOLE __fb_con;

typedef void (*SIGHANDLER)(int);
static SIGHANDLER old_sighandler[NSIG];
static const char *seq[] = { "cm", "ho", "cs", "cl", "ce", "WS", "bl", "AF", "AB",
							 "me", "md", "SF", "ve", "vi", "dc", "ks", "ke" };


/*:::::*/
static void *bg_thread(void *arg)
{
	while (__fb_con.inited) {

		BG_LOCK();
		if (__fb_con.keyboard_handler)
			__fb_con.keyboard_handler();
		if (__fb_con.mouse_handler)
			__fb_con.mouse_handler();
		BG_UNLOCK();

		usleep(30000);
	}
	return NULL;
}


/*:::::*/
static int default_getch(void)
{
	return fgetc(__fb_con.f_in);
}


/*:::::*/
static void signal_handler(int sig)
{
	signal(sig, old_sighandler[sig]);

	fb_hEnd(1);

	raise(sig);
}


/*:::::*/
static void console_resize(int sig)
{
	unsigned char *char_buffer, *attr_buffer;
	struct winsize win;
	int r, c, w, h;

	if (!__fb_con.inited)
		return;

	win.ws_row = 0xFFFF;
	ioctl(__fb_con.h_out, TIOCGWINSZ, &win);
	if (win.ws_row == 0xFFFF) {
#ifdef TARGET_LINUX
		fb_hTermOut(SEQ_QUERY_WINDOW, 0, 0);
		if (fscanf(stdin, "\e[8;%d;%dt", &r, &c) == 2) {
			win.ws_row = r;
			win.ws_col = c;
		}
		else
#endif
		{
			win.ws_row = 25;
			win.ws_col = 80;
		}
	}

	char_buffer = calloc(1, win.ws_row * win.ws_col * 2);
	attr_buffer = char_buffer + (win.ws_row * win.ws_col);
	if (__fb_con.char_buffer) {
		h = (__fb_con.h < win.ws_row) ? __fb_con.h : win.ws_row;
		w = (__fb_con.w < win.ws_col) ? __fb_con.w : win.ws_col;
		for (r = 0; r < h; r++) {
			memcpy(char_buffer + (r * win.ws_col), __fb_con.char_buffer + (r * __fb_con.w), w);
			memcpy(attr_buffer + (r * win.ws_col), __fb_con.attr_buffer + (r * __fb_con.w), w);
		}
		free(__fb_con.char_buffer);
	}
	__fb_con.char_buffer = char_buffer;
	__fb_con.attr_buffer = attr_buffer;
	__fb_con.h = win.ws_row;
	__fb_con.w = win.ws_col;
#ifdef TARGET_LINUX
	fflush(stdin);
	fb_hTermOut(SEQ_QUERY_CURSOR, 0, 0);
	fscanf(stdin, "\e[%d;%dR", &__fb_con.cur_y, &__fb_con.cur_x);
#else
	/* !!!TODO!!! reset cursor to known position? */
	__fb_con.cur_y = __fb_con.cur_x = 1;
#endif

	signal(SIGWINCH, console_resize);
}


/*:::::*/
int fb_hTermOut( int code, int param1, int param2 )
{
	const char *extra_seq[] = { "\e(U", "\e(B", "\e[6n", "\e[18t",
		"\e[?1000h\e[?1003h", "\e[?1003l\e[?1000l", "\e[H\e[J\e[0m" };
	char *str;

	if (!__fb_con.inited)
		return -1;

	fflush(stdout);
	if (code > SEQ_MAX) {
		switch (code) {
			case SEQ_SET_COLOR_EX:
				fprintf(__fb_con.f_out, "\e[%dm", param1);
				break;
			
			default:
				fputs(extra_seq[code - SEQ_EXTRA], __fb_con.f_out);
				break;
		}
	}
	else {
		if (!__fb_con.seq[code])
			return -1;
		str = tgoto(__fb_con.seq[code], param1, param2);
		if (!str)
			return -1;
		tputs(str, 1, putchar);
	}
	return 0;
}


/*:::::*/
int fb_hInitConsole ( )
{
	struct termios term_out, term_in;

	if (!__fb_con.inited)
		return -1;

	/* Init terminal I/O */
	__fb_con.f_out = stdout;
	__fb_con.h_out = fileno(stdout);
	if (!isatty(__fb_con.h_out) || !isatty(fileno(stdin)))
		return -1;
	__fb_con.f_in = fopen("/dev/tty", "r+b");
	if (!__fb_con.f_in)
		return -1;
	__fb_con.h_in = fileno(__fb_con.f_in);
	
	/* Cannot control console if process was started in background */
	if (tcgetpgrp(__fb_con.h_out) != getpgid(0))
		return -1;

	/* Output setup */
	if (tcgetattr(__fb_con.h_out, &__fb_con.old_term_out))
		return -1;
	memcpy(&term_out, &__fb_con.old_term_out, sizeof(term_out));
	term_out.c_oflag |= OPOST;
	if (tcsetattr(__fb_con.h_out, TCSANOW, &term_out))
		return -1;
	
	/* Input setup */
	if (tcgetattr(__fb_con.h_in, &__fb_con.old_term_in))
		return -1;
	memcpy(&term_in, &__fb_con.old_term_in, sizeof(term_in));
	/* Send SIGINT on control-C */
	term_in.c_iflag |= BRKINT;
	/* Disable Xon/off and input BREAK condition ignoring */
	term_in.c_iflag &= ~(IXOFF | IXON | IGNBRK);
	/* Character oriented, no echo */
	term_in.c_lflag &= ~(ICANON | ECHO);
	/* No timeout, just don't block */
	term_in.c_cc[VMIN] = 1;
	term_in.c_cc[VTIME] = 0;

	if (tcsetattr(__fb_con.h_in, TCSANOW, &term_in))
		return -1;
	/* Don't block */
	__fb_con.old_in_flags = fcntl(__fb_con.h_in, F_GETFL, 0);
	__fb_con.in_flags = __fb_con.old_in_flags | O_NONBLOCK;
	fcntl(__fb_con.h_in, F_SETFL, __fb_con.in_flags);

#ifdef TARGET_LINUX
	if (__fb_con.inited == INIT_CONSOLE)
		fb_hTermOut(SEQ_INIT_CHARSET, 0, 0);
#endif
	fb_hTermOut(SEQ_INIT_KEYPAD, 0, 0);

	/* Initialize keyboard and mouse handlers if set */
	BG_LOCK();
	if (__fb_con.keyboard_init)
		__fb_con.keyboard_init();
	if (__fb_con.mouse_init)
		__fb_con.mouse_init();
	BG_UNLOCK();

	return 0;
}


/*:::::*/
void fb_unix_hInit ( void )
{
	const int sigs[] = { SIGABRT, SIGFPE, SIGILL, SIGSEGV, SIGTERM, SIGINT, SIGQUIT, -1 };
	char buffer[2048], *p, *term;
	struct termios tty;
    int i;

#ifdef MULTITHREADED
    pthread_mutexattr_t attr;
#endif

#if defined(__GNUC__) && defined(__i386__)
	unsigned int control_word;

	/* Get FPU control word */
	__asm__ __volatile__( "fstcw %0" : "=m" (control_word) : );
	/* Set 64-bit and round to nearest */
	control_word = (control_word & 0xF0FF) | 0x300;
	/* Write back FPU control word */
	__asm__ __volatile__( "fldcw %0" : : "m" (control_word) );
#endif

#ifdef MULTITHREADED
	/* make mutex recursive to behave the same on Win32 and Linux (if possible) */
	pthread_mutexattr_init(&attr);

#	ifdef HAVE_PTHREAD_MUTEX_RECURSIVE
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#	else
#		ifdef HAVE_PTHREAD_MUTEX_RECURSIVE_NP
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#		else
#			error Need PTHREAD_MUTEX_RECURSIVE or PTHREAD_MUTEX_RECURSIVE_NP
#		endif
#	endif

	/* Init multithreading support */
	pthread_mutex_init(&__fb_global_mutex, &attr);
	pthread_mutex_init(&__fb_string_mutex, &attr);
#endif

	pthread_mutex_init( &__fb_con.bg_mutex, NULL );

	memset(&__fb_con, 0, sizeof(__fb_con));

	/* Init termcap */
	term = getenv("TERM");
	if ((!term) || (tgetent(buffer, term) <= 0))
		return;
	BC = UP = 0;
	p = tgetstr("pc", NULL);
	PC = p ? *p : 0;
	if (tcgetattr(1, &tty))
		return;
	ospeed = cfgetospeed(&tty);
	if (!tgetflag("am"))
		return;
	for (i = 0; i < SEQ_MAX; i++)
		__fb_con.seq[i] = tgetstr(seq[i], NULL);

	/* !!!TODO!!! detect other OS consoles? (freebsd: 'cons25', etc?) */
	if ((!strcmp(term, "console")) || (!strncmp(term, "linux", 5)))
		__fb_con.inited = INIT_CONSOLE;
	else
		__fb_con.inited = INIT_X11;

	if (!strncasecmp(term, "eterm", 5))
		__fb_con.term_type = TERM_ETERM;
	else if (!strncmp(term, "xterm", 5))
		__fb_con.term_type = TERM_XTERM;
	else
		__fb_con.term_type = TERM_GENERIC;

	if (fb_hInitConsole()) {
		__fb_con.inited = FALSE;
		return;
	}
	__fb_con.keyboard_getch = default_getch;

	pthread_create( &__fb_con.bg_thread, NULL, bg_thread, NULL );

	/* Install signal handlers to quietly shut down */
	for (i = 0; sigs[i] >= 0; i++)
		old_sighandler[sigs[i]] = signal(sigs[i],  signal_handler);

	__fb_con.char_buffer = NULL;
	__fb_con.fg_color = 7;
	__fb_con.bg_color = 0;
	console_resize(SIGWINCH);
}
