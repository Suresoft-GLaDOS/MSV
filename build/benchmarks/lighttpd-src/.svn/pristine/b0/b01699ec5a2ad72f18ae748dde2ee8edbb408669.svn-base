#ifndef _LOG_H_
#define _LOG_H_

#include "valgrind/valgrind.h"
#include "buffer.h"

/* Close fd and _try_ to get a /dev/null for it instead.
 * Returns 0 on success and -1 on failure (fd gets closed in all cases)
 */
LI_API int openDevNull(int fd);

LI_API void log_init(void);
LI_API void log_free(void);

LI_API int log_error_open(buffer* file, buffer* breakage_file, int use_syslog, int dont_daemonize);
LI_API int log_error_close();
LI_API int log_error_write(void *srv, const char *filename, unsigned int line, const char *fmt, ...);
LI_API int log_error_cycle();
#define REMOVE_PATH_FROM_FILE 1
#if REMOVE_PATH_FROM_FILE
LI_API const char *remove_path(const char *path);
#define REMOVE_PATH(file) remove_path(file)
#else
#define REMOVE_PATH(file) file
#endif

// TODO: perhaps make portable (detect if cc supports)
#define	__ATTRIBUTE_PRINTF_FORMAT(fmt, arg) __attribute__ ((__format__ (__printf__, fmt, arg)))

#define ERROR(fmt, ...) \
	log_trace("(error) (%s:%d) "fmt, REMOVE_PATH(__FILE__), __LINE__, __VA_ARGS__)

#define TRACE(fmt, ...) \
	log_trace("(trace) (%s:%d) "fmt, REMOVE_PATH(__FILE__), __LINE__, __VA_ARGS__)

#define SEGFAULT(fmt, ...) \
	do { \
		log_trace("(crashing) (%s:%d) "fmt, REMOVE_PATH(__FILE__), __LINE__, __VA_ARGS__); \
		VALGRIND_PRINTF_BACKTRACE(fmt, __VA_ARGS__);\
		abort();\
       	} while(0)
LI_API int log_trace(const char *fmt, ...) __ATTRIBUTE_PRINTF_FORMAT(1, 2);
#endif
