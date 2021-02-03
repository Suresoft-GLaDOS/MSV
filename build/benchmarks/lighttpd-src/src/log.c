/*
 * make sure _GNU_SOURCE is defined
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>
#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef _WIN32
#undef HAVE_SYSLOG_H
#endif

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif

#include "log.h"
#include "array.h"

#include "sys-files.h"

#ifdef _WIN32
#define STDERR_FILENO 2
#endif

#ifndef O_LARGEFILE
# define O_LARGEFILE 0
#endif

/* Close fd and _try_ to get a /dev/null for it instead.
 * close() alone may trigger some bugs when a
 * process opens another file and gets fd = STDOUT_FILENO or STDERR_FILENO
 * and later tries to just print on stdout/stderr
 *
 * Returns 0 on success and -1 on failure (fd gets closed in all cases)
 */
int openDevNull(int fd) {
	int tmpfd;
	close(fd);
#if defined(__WIN32)
	/* Cygwin should work with /dev/null */
	tmpfd = open("nul", O_RDWR);
#else
	tmpfd = open("/dev/null", O_RDWR);
#endif
	if (tmpfd != -1 && tmpfd != fd) {
		dup2(tmpfd, fd);
		close(tmpfd);
	}
	return (tmpfd != -1) ? 0 : -1;
}

/**
 * open the errorlog
 *
 * we have 3 possibilities:
 * - stderr (default)
 * - syslog
 * - logfile
 *
 * if the open failed, report to the user and die
 *
 */


typedef struct {
	buffer *file;
	unsigned short use_syslog;

	/* the errorlog */
	int fd;
	enum { ERRORLOG_FILE, ERRORLOG_FD, ERRORLOG_SYSLOG } mode;
	buffer *buf;

	time_t cached_ts;
	buffer *cached_ts_str;
} errorlog;

errorlog *myconfig = NULL;

void log_init(void) {
	/* use syslog */
	errorlog *err;

	err = calloc(1, sizeof(*err));

	err->fd = STDERR_FILENO;
	err->mode = ERRORLOG_FD;
	err->buf = buffer_init();
	err->cached_ts_str = buffer_init();

	myconfig = err;
}

void log_free(void) {
	errorlog *err = myconfig;

	if (!err) return;

	switch(err->mode) {
	case ERRORLOG_FILE:
	case ERRORLOG_FD:
		if (-1 != err->fd) {
			if (STDERR_FILENO != err->fd)
				close(err->fd);
			err->fd = -1;
		}
		break;
	case ERRORLOG_SYSLOG:
#ifdef HAVE_SYSLOG_H
		closelog();
#endif
		break;
	}

	buffer_free(err->buf);
	buffer_free(err->cached_ts_str);

	free(err);

	myconfig = NULL;
}

int log_error_open(buffer *file, buffer *breakage_file, int use_syslog, int dont_daemonize) {
	errorlog *err = myconfig;

#ifdef HAVE_SYSLOG_H
	/* perhaps someone wants to use syslog() */
	openlog("lighttpd", LOG_CONS | LOG_PID, LOG_DAEMON);
#endif
	err->mode = ERRORLOG_FD;
	err->fd = STDERR_FILENO;

	if (use_syslog) {
		err->mode = ERRORLOG_SYSLOG;
	} else if (!buffer_is_empty(file)) {
		if (-1 == (err->fd = open(file->ptr, O_APPEND | O_WRONLY | O_CREAT | O_LARGEFILE, 0644))) {
			log_error_write(NULL, __FILE__, __LINE__, "SBSS",
					"opening errorlog '", file,
					"' failed: ", strerror(errno));

			return -1;
		}
#ifdef FD_CLOEXEC
		/* close fd on exec (cgi) */
		fcntl(err->fd, F_SETFD, FD_CLOEXEC);
#endif
		err->mode = ERRORLOG_FILE;
		err->file = file;
	}

	TRACE("%s", "server started");

	if (err->mode == ERRORLOG_FD && !dont_daemonize) {
		/* We can only log to stderr in dont-daemonize mode */
		err->fd = -1;
	}

	if (!buffer_is_empty(breakage_file)) {
		int breakage_fd;

		if (err->mode == ERRORLOG_FD) {
			err->fd = dup(STDERR_FILENO);
#ifdef FD_CLOEXEC
			fcntl(err->fd, F_SETFD, FD_CLOEXEC);
#endif
		}

		if (-1 == (breakage_fd = open(breakage_file->ptr, O_APPEND | O_WRONLY | O_CREAT | O_LARGEFILE, 0644))) {
			log_error_write(NULL, __FILE__, __LINE__, "SBSS",
					"opening breakagelog '", breakage_file,
					"' failed: ", strerror(errno));

			return -1;
		}

		if (STDERR_FILENO != breakage_fd) {
			dup2(breakage_fd, STDERR_FILENO);
			close(breakage_fd);
		}
	} else if (!dont_daemonize) {
		/* move stderr to /dev/null */
		openDevNull(STDERR_FILENO);
	}


	return 0;
}

/**
 * open the errorlog
 *
 * if the open failed, report to the user and die
 * if no filename is given, use syslog instead
 *
 */

int log_error_cycle(void) {
	/* only cycle if we are not in syslog-mode */

	errorlog *err = myconfig;

	if (err->mode == ERRORLOG_FILE) {
		buffer *file = err->file;
		/* already check of opening time */

		int new_fd;

		if (-1 == (new_fd = open(file->ptr, O_APPEND | O_WRONLY | O_CREAT | O_LARGEFILE, 0644))) {
			/* write to old log */
			log_error_write(NULL, __FILE__, __LINE__, "SBSSS",
					"cycling errorlog '", file,
					"' failed: ", strerror(errno),
					", falling back to syslog()");

			close(err->fd);
			err->fd = -1;
#ifdef HAVE_SYSLOG_H
			err->mode = ERRORLOG_SYSLOG;
#endif
		} else {
			/* ok, new log is open, close the old one */
			close(err->fd);
			err->fd = new_fd;
		}
	}

	return 0;
}

int log_error_write(void *srv, const char *filename, unsigned int line, const char *fmt, ...) {
	va_list ap;
	time_t t;

	errorlog *err = myconfig;

	UNUSED(srv);

	switch(err->mode) {
	case ERRORLOG_FILE:
	case ERRORLOG_FD:
		if (-1 == err->fd) return 0;
		/* cache the generated timestamp */
		t = time(NULL);

		if (t != err->cached_ts) {
			buffer_prepare_copy(err->cached_ts_str, 255);
			strftime(err->cached_ts_str->ptr, err->cached_ts_str->size - 1, "%Y-%m-%d %H:%M:%S", localtime(&(t)));
			err->cached_ts_str->used = strlen(err->cached_ts_str->ptr) + 1;
			err->cached_ts = t;
		}

		buffer_copy_string_buffer(err->buf, err->cached_ts_str);
		buffer_append_string_len(err->buf, CONST_STR_LEN(" ("));
		break;
	case ERRORLOG_SYSLOG:
		/* syslog is generating its own timestamps */
		buffer_copy_string_len(err->buf, CONST_STR_LEN("("));
		break;
	}

	buffer_append_string(err->buf, REMOVE_PATH(filename));
	buffer_append_string_len(err->buf, CONST_STR_LEN(":"));
	buffer_append_long(err->buf, line);
	buffer_append_string_len(err->buf, CONST_STR_LEN(") "));

	for(va_start(ap, fmt); *fmt; fmt++) {
		int d;
		char *s;
		buffer *b;
		off_t o;

		switch(*fmt) {
		case 's':           /* string */
			s = va_arg(ap, char *);
			buffer_append_string(err->buf, s);
			buffer_append_string_len(err->buf, CONST_STR_LEN(" "));
			break;
		case 'b':           /* buffer */
			b = va_arg(ap, buffer *);
			buffer_append_string_buffer(err->buf, b);
			buffer_append_string_len(err->buf, CONST_STR_LEN(" "));
			break;
		case 'd':           /* int */
			d = va_arg(ap, int);
			buffer_append_long(err->buf, d);
			buffer_append_string_len(err->buf, CONST_STR_LEN(" "));
			break;
		case 'o':           /* off_t */
			o = va_arg(ap, off_t);
			buffer_append_off_t(err->buf, o);
			buffer_append_string_len(err->buf, CONST_STR_LEN(" "));
			break;
		case 'x':           /* int (hex) */
			d = va_arg(ap, int);
			buffer_append_string_len(err->buf, CONST_STR_LEN("0x"));
			buffer_append_long_hex(err->buf, d);
			buffer_append_string_len(err->buf, CONST_STR_LEN(" "));
			break;
		case 'S':           /* string */
			s = va_arg(ap, char *);
			buffer_append_string(err->buf, s);
			break;
		case 'B':           /* buffer */
			b = va_arg(ap, buffer *);
			buffer_append_string_buffer(err->buf, b);
			break;
		case 'D':           /* int */
			d = va_arg(ap, int);
			buffer_append_long(err->buf, d);
			break;
		case '(':
		case ')':
		case '<':
		case '>':
		case ',':
		case ' ':
			buffer_append_string_len(err->buf, fmt, 1);
			break;
		}
	}
	va_end(ap);

	switch(err->mode) {
	case ERRORLOG_FILE:
	case ERRORLOG_FD:
		buffer_append_string_len(err->buf, CONST_STR_LEN("\n"));
		write(err->fd, err->buf->ptr, err->buf->used - 1);
		break;
#ifdef HAVE_SYSLOG_H
	case ERRORLOG_SYSLOG:
		syslog(LOG_ERR, "%s", err->buf->ptr);
		break;
#endif
	}

	return 0;
}

int log_trace(const char *fmt, ...) {
	buffer *b;
	int l, tries = 0;
	errorlog *err = myconfig;
	va_list ap;
	time_t t;
	int timestrsize = 0;

	b = buffer_init();
	buffer_prepare_copy(b, 4096);

	switch(err->mode) {
	case ERRORLOG_FILE:
	case ERRORLOG_FD:
		if (-1 == err->fd) return 0;
		/* cache the generated timestamp */
		t = time(NULL);

		if (t != err->cached_ts) {
			buffer_prepare_copy(err->cached_ts_str, 255);
			strftime(err->cached_ts_str->ptr, err->cached_ts_str->size - 1, "%Y-%m-%d %H:%M:%S", localtime(&(t)));
			err->cached_ts_str->used = strlen(err->cached_ts_str->ptr) + 1;
			err->cached_ts = t;
		}

		buffer_copy_string_buffer(b, err->cached_ts_str);
		buffer_append_string_len(b, CONST_STR_LEN(" "));
		timestrsize = b->used - 1;
		break;
	case ERRORLOG_SYSLOG:
		/* syslog is generating its own timestamps */
		buffer_copy_string_len(b, CONST_STR_LEN(""));
		timestrsize = b->used - 1;
		break;
	}

	do {
		errno = 0;
		va_start(ap, fmt);
		l = vsnprintf(b->ptr+timestrsize, b->size-timestrsize, fmt, ap);
		va_end(ap);

		/* if 'l' is between -1 and size we are good,
		 * otherwise we have to resize to size 
		 */

		if (l > -1 && ((unsigned int) l) < (b->size-timestrsize)) {
			b->used += l;

			break;
		}

		if (l > -1) {
			/* C99: l is the mem-size we need */
			buffer_prepare_append(b, l + 1); /* allocate a bit more than we need */
		} else if (tries++ >= 3) {
			int e = errno;
			/* glibc 2.0.6 and earlier return -1 if the output was truncated
			 * so we try to increase the buffer size 3 times - so you cannot
			 * print error messages longer than 8 * 4096 = 32k with glib <= 2.0.6
			 */
			buffer_copy_string_len(b, CONST_STR_LEN("log_trace: vsnprintf error: l = "));
			buffer_append_long(b, l);
			if (e) {
				buffer_append_string_len(b, CONST_STR_LEN(", errno = "));
				buffer_append_long(b, errno);
				buffer_append_string_len(b, CONST_STR_LEN(": "));
				buffer_append_string(b, strerror(e));
			}
			break;
		} else {
			buffer_prepare_append(b, 2*b->size);
		}
	} while(1);

	/* write b */
	switch(err->mode) {
	case ERRORLOG_FILE:
	case ERRORLOG_FD:
		buffer_append_string_len(b, CONST_STR_LEN("\n"));
		write(err->fd, b->ptr, b->used - 1);
		break;
#ifdef HAVE_SYSLOG_H
	case ERRORLOG_SYSLOG:
		syslog(LOG_ERR, "%s", b->ptr);
		break;
#endif
	}

	buffer_free(b);

	return 0;
}

#if REMOVE_PATH_FROM_FILE
const char *remove_path(const char *path) {
	char *p = strrchr(path, DIR_SEPERATOR);
	if (NULL != p && *(p) != '\0') {
		return (p + 1);
	}
	return path;
}
#endif

