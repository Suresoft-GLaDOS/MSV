int __get_mutant();
int __is_neg(const char *, int, const int *, int, const char *, int,
             const void **, int, const double *, int, ...);
int __abst_hole();
int __choose(char *);
void *memset(void *, int, unsigned long);
/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2011 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Edin Kadribasic <edink@php.net>                              |
   |         Marcus Boerger <helly@php.net>                               |
   |         Johannes Schlueter <johannes@php.net>                        |
   |         Parts based on CGI SAPI Module by                            |
   |         Rasmus Lerdorf, Stig Bakken and Zeev Suraski                 |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#include "php.h"
#include "php_globals.h"
#include "php_variables.h"
#include "zend_hash.h"
#include "zend_interfaces.h"
#include "zend_modules.h"

#ifdef HAVE_REFLECTION
#include "ext/reflection/php_reflection.h"
#endif

#include "SAPI.h"

#include "php.h"
#include <stdio.h>
#ifdef PHP_WIN32
#include "win32/signal.h"
#include "win32/time.h"
#include <process.h>
#endif
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_SIGNAL_H
#include <signal.h>
#endif
#if HAVE_SETLOCALE
#include <locale.h>
#endif
#include "cli.h"
#include "ext/standard/php_standard.h"
#include "fopen_wrappers.h"
#include "php_globals.h"
#include "php_ini.h"
#include "php_main.h"
#include "zend.h"
#include "zend_extensions.h"
#ifdef PHP_WIN32
#include "win32/php_registry.h"
#include <fcntl.h>
#include <io.h>
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef __riscos__
#include <unixlib/local.h>
#endif

#include "zend_compile.h"
#include "zend_exceptions.h"
#include "zend_execute.h"
#include "zend_highlight.h"
#include "zend_indent.h"

#include "php_getopt.h"

#ifndef PHP_WIN32
#define php_select(m, r, w, e, t) select(m, r, w, e, t)
#else
#include "win32/select.h"
#endif

PHPAPI extern char *php_ini_opened_path;
PHPAPI extern char *php_ini_scanned_path;
PHPAPI extern char *php_ini_scanned_files;

#ifndef O_BINARY
#define O_BINARY 0
#endif

#define PHP_MODE_STANDARD 1
#define PHP_MODE_HIGHLIGHT 2
#define PHP_MODE_INDENT 3
#define PHP_MODE_LINT 4
#define PHP_MODE_STRIP 5
#define PHP_MODE_CLI_DIRECT 6
#define PHP_MODE_PROCESS_STDIN 7
#define PHP_MODE_REFLECTION_FUNCTION 8
#define PHP_MODE_REFLECTION_CLASS 9
#define PHP_MODE_REFLECTION_EXTENSION 10
#define PHP_MODE_REFLECTION_EXT_INFO 11
#define PHP_MODE_REFLECTION_ZEND_EXTENSION 12
#define PHP_MODE_SHOW_INI_CONFIG 13

cli_shell_callbacks_t cli_shell_callbacks = {NULL, NULL, NULL};
PHP_CLI_API cli_shell_callbacks_t *php_cli_get_shell_callbacks() {
  return &cli_shell_callbacks;
}

const char HARDCODED_INI[] = "html_errors=0\n"
                             "register_argc_argv=1\n"
                             "implicit_flush=1\n"
                             "output_buffering=0\n"
                             "max_execution_time=0\n"
                             "max_input_time=-1\n\0";

static char *php_optarg = NULL;
static int php_optind = 1;

static const opt_struct OPTIONS[] = {
    {'a', 0, "interactive"},
    {'B', 1, "process-begin"},
    {'C', 0, "no-chdir"}, /* for compatibility with CGI (do not chdir to script
                             directory) */
    {'c', 1, "php-ini"},
    {'d', 1, "define"},
    {'E', 1, "process-end"},
    {'e', 0, "profile-info"},
    {'F', 1, "process-file"},
    {'f', 1, "file"},
    {'h', 0, "help"},
    {'i', 0, "info"},
    {'l', 0, "syntax-check"},
    {'m', 0, "modules"},
    {'n', 0, "no-php-ini"},
    {'q', 0, "no-header"}, /* for compatibility with CGI (do not generate HTTP
                              headers) */
    {'R', 1, "process-code"},
    {'H', 0, "hide-args"},
    {'r', 1, "run"},
    {'s', 0, "syntax-highlight"},
    {'s', 0, "syntax-highlighting"},
    {'w', 0, "strip"},
    {'?', 0, "usage"}, /* help alias (both '?' and 'usage') */
    {'v', 0, "version"},
    {'z', 1, "zend-extension"},
#ifdef HAVE_REFLECTION
    {10, 1, "rf"},
    {10, 1, "rfunction"},
    {11, 1, "rc"},
    {11, 1, "rclass"},
    {12, 1, "re"},
    {12, 1, "rextension"},
    {13, 1, "rz"},
    {13, 1, "rzendextension"},
#endif
    {14, 1, "ri"},
    {14, 1, "rextinfo"},
    {15, 0, "ini"},
    {'-', 0, NULL} /* end of args */
};

static int print_module_info(zend_module_entry *module TSRMLS_DC) /* {{{ */
{
  php_printf("%s\n", module->name);
  return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

static int module_name_cmp(const void *a, const void *b TSRMLS_DC) /* {{{ */
{
  Bucket *f = *((Bucket **)a);
  Bucket *s = *((Bucket **)b);

  return strcasecmp(((zend_module_entry *)f->pData)->name,
                    ((zend_module_entry *)s->pData)->name);
}
/* }}} */

static void print_modules(TSRMLS_D) /* {{{ */
{
  HashTable sorted_registry;
  zend_module_entry tmp;

  zend_hash_init(&sorted_registry, 50, NULL, NULL, 1);
  zend_hash_copy(&sorted_registry, &module_registry, NULL, &tmp,
                 sizeof(zend_module_entry));
  zend_hash_sort(&sorted_registry, zend_qsort, module_name_cmp, 0 TSRMLS_CC);
  zend_hash_apply(&sorted_registry, (apply_func_t)print_module_info TSRMLS_CC);
  zend_hash_destroy(&sorted_registry);
}
/* }}} */

static int print_extension_info(zend_extension *ext,
                                void *arg TSRMLS_DC) /* {{{ */
{
  php_printf("%s\n", ext->name);
  return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

static int extension_name_cmp(const zend_llist_element **f,
                              const zend_llist_element **s TSRMLS_DC) /* {{{ */
{
  return strcmp(((zend_extension *)(*f)->data)->name,
                ((zend_extension *)(*s)->data)->name);
}
/* }}} */

static void print_extensions(TSRMLS_D) /* {{{ */
{
  zend_llist sorted_exts;

  zend_llist_copy(&sorted_exts, &zend_extensions);
  sorted_exts.dtor = NULL;
  zend_llist_sort(&sorted_exts, extension_name_cmp TSRMLS_CC);
  zend_llist_apply(&sorted_exts,
                   (llist_apply_func_t)print_extension_info TSRMLS_CC);
  zend_llist_destroy(&sorted_exts);
}
/* }}} */

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

static inline int sapi_cli_select(int fd TSRMLS_DC) {
  fd_set wfd, dfd;
  struct timeval tv;
  int ret;

  FD_ZERO(&wfd);
  FD_ZERO(&dfd);

  PHP_SAFE_FD_SET(fd, &wfd);

  tv.tv_sec = FG(default_socket_timeout);
  tv.tv_usec = 0;

  ret = php_select(fd + 1, &dfd, &wfd, &dfd, &tv);

  return ret != -1;
}

PHP_CLI_API size_t sapi_cli_single_write(const char *str,
                                         uint str_length TSRMLS_DC) /* {{{ */
{
#ifdef PHP_WRITE_STDOUT
  long ret;
#else
  size_t ret;
#endif

  if (cli_shell_callbacks.cli_shell_write) {
    size_t shell_wrote;
    shell_wrote =
        cli_shell_callbacks.cli_shell_write(str, str_length TSRMLS_CC);
    if (shell_wrote > -1) {
      return shell_wrote;
    }
  }

#ifdef PHP_WRITE_STDOUT
  do {
    ret = write(STDOUT_FILENO, str, str_length);
  } while (ret <= 0 && errno == EAGAIN &&
           sapi_cli_select(STDOUT_FILENO TSRMLS_CC));

  if (ret <= 0) {
    return 0;
  }

  return ret;
#else
  ret = fwrite(str, 1, MIN(str_length, 16384), stdout);
  return ret;
#endif
}
/* }}} */

static int sapi_cli_ub_write(const char *str,
                             uint str_length TSRMLS_DC) /* {{{ */
{
  const char *ptr = str;
  uint remaining = str_length;
  size_t ret;

  if (!str_length) {
    return 0;
  }

  if (cli_shell_callbacks.cli_shell_ub_write) {
    int ub_wrote;
    ub_wrote =
        cli_shell_callbacks.cli_shell_ub_write(str, str_length TSRMLS_CC);
    if (ub_wrote > -1) {
      return ub_wrote;
    }
  }

  while (remaining > 0) {
    ret = sapi_cli_single_write(ptr, remaining TSRMLS_CC);
    if (!ret) {
#ifndef PHP_CLI_WIN32_NO_CONSOLE
      php_handle_aborted_connection();
#endif
      break;
    }
    ptr += ret;
    remaining -= ret;
  }

  return (ptr - str);
}
/* }}} */

static void sapi_cli_flush(void *server_context) /* {{{ */
{
  /* Ignore EBADF here, it's caused by the fact that STDIN/STDOUT/STDERR streams
   * are/could be closed before fflush() is called.
   */
  if (fflush(stdout) == EOF && errno != EBADF) {
#ifndef PHP_CLI_WIN32_NO_CONSOLE
    php_handle_aborted_connection();
#endif
  }
}
/* }}} */

static char *php_self = "";
static char *script_filename = "";

static void
sapi_cli_register_variables(zval *track_vars_array TSRMLS_DC) /* {{{ */
{
  unsigned int len;
  char *docroot = "";

  /* In CGI mode, we consider the environment to be a part of the server
   * variables
   */
  php_import_environment_variables(track_vars_array TSRMLS_CC);

  /* Build the special-case PHP_SELF variable for the CLI version */
  len = strlen(php_self);
  if (sapi_module.input_filter(PARSE_SERVER, "PHP_SELF", &php_self, len,
                               &len TSRMLS_CC)) {
    php_register_variable("PHP_SELF", php_self, track_vars_array TSRMLS_CC);
  }
  if (sapi_module.input_filter(PARSE_SERVER, "SCRIPT_NAME", &php_self, len,
                               &len TSRMLS_CC)) {
    php_register_variable("SCRIPT_NAME", php_self, track_vars_array TSRMLS_CC);
  }
  /* filenames are empty for stdin */
  len = strlen(script_filename);
  if (sapi_module.input_filter(PARSE_SERVER, "SCRIPT_FILENAME",
                               &script_filename, len, &len TSRMLS_CC)) {
    php_register_variable("SCRIPT_FILENAME", script_filename,
                          track_vars_array TSRMLS_CC);
  }
  if (sapi_module.input_filter(PARSE_SERVER, "PATH_TRANSLATED",
                               &script_filename, len, &len TSRMLS_CC)) {
    php_register_variable("PATH_TRANSLATED", script_filename,
                          track_vars_array TSRMLS_CC);
  }
  /* just make it available */
  len = 0U;
  if (sapi_module.input_filter(PARSE_SERVER, "DOCUMENT_ROOT", &docroot, len,
                               &len TSRMLS_CC)) {
    php_register_variable("DOCUMENT_ROOT", docroot, track_vars_array TSRMLS_CC);
  }
}
/* }}} */

static void sapi_cli_log_message(char *message TSRMLS_DC) /* {{{ */
{
  fprintf(stderr, "%s\n", message);
}
/* }}} */

static int sapi_cli_deactivate(TSRMLS_D) /* {{{ */
{
  fflush(stdout);
  if (SG(request_info).argv0) {
    free(SG(request_info).argv0);
    SG(request_info).argv0 = NULL;
  }
  return SUCCESS;
}
/* }}} */

static char *sapi_cli_read_cookies(TSRMLS_D) /* {{{ */
{
  return NULL;
}
/* }}} */

static int sapi_cli_header_handler(sapi_header_struct *h,
                                   sapi_header_op_enum op,
                                   sapi_headers_struct *s TSRMLS_DC) /* {{{ */
{
  return 0;
}
/* }}} */

static int
sapi_cli_send_headers(sapi_headers_struct *sapi_headers TSRMLS_DC) /* {{{ */
{
  /* We do nothing here, this function is needed to prevent that the fallback
   * header handling is called. */
  return SAPI_HEADER_SENT_SUCCESSFULLY;
}
/* }}} */

static void sapi_cli_send_header(sapi_header_struct *sapi_header,
                                 void *server_context TSRMLS_DC) /* {{{ */
{}
/* }}} */

static int php_cli_startup(sapi_module_struct *sapi_module) /* {{{ */
{
  if (php_module_startup(sapi_module, NULL, 0) == FAILURE) {
    return FAILURE;
  }
  return SUCCESS;
}
/* }}} */

/* {{{ sapi_cli_ini_defaults */

/* overwriteable ini defaults must be set in sapi_cli_ini_defaults() */
#define INI_DEFAULT(name, value)                                               \
  Z_SET_REFCOUNT(tmp, 0);                                                      \
  Z_UNSET_ISREF(tmp);                                                          \
  ZVAL_STRINGL(&tmp, zend_strndup(value, sizeof(value) - 1),                   \
               sizeof(value) - 1, 0);                                          \
  zend_hash_update(configuration_hash, name, sizeof(name), &tmp, sizeof(zval), \
                   NULL);

static void sapi_cli_ini_defaults(HashTable *configuration_hash) {
  zval tmp;
  INI_DEFAULT("report_zend_debug", "0");
  INI_DEFAULT("display_errors", "1");
}
/* }}} */

/* {{{ sapi_module_struct cli_sapi_module
 */
static sapi_module_struct cli_sapi_module = {
    "cli",                    /* name */
    "Command Line Interface", /* pretty name */

    php_cli_startup,             /* startup */
    php_module_shutdown_wrapper, /* shutdown */

    NULL,                /* activate */
    sapi_cli_deactivate, /* deactivate */

    sapi_cli_ub_write, /* unbuffered write */
    sapi_cli_flush,    /* flush */
    NULL,              /* get uid */
    NULL,              /* getenv */

    php_error, /* error handler */

    sapi_cli_header_handler, /* header handler */
    sapi_cli_send_headers,   /* send headers handler */
    sapi_cli_send_header,    /* send header handler */

    NULL,                  /* read POST data */
    sapi_cli_read_cookies, /* read Cookies */

    sapi_cli_register_variables, /* register server variables */
    sapi_cli_log_message,        /* Log message */
    NULL,                        /* Get request time */
    NULL,                        /* Child terminate */

    STANDARD_SAPI_MODULE_PROPERTIES};
/* }}} */

/* {{{ arginfo ext/standard/dl.c */
ZEND_BEGIN_ARG_INFO(arginfo_dl, 0)
ZEND_ARG_INFO(0, extension_filename)
ZEND_END_ARG_INFO()
/* }}} */

static const zend_function_entry additional_functions[] = {
    ZEND_FE(dl, arginfo_dl){NULL, NULL, NULL}};

/* {{{ php_cli_usage
 */
static void php_cli_usage(char *argv0) {
  char *prog;

  prog = strrchr(argv0, '/');
  if (prog) {
    prog++;
  } else {
    prog = "php";
  }

  php_printf(
      "Usage: %s [options] [-f] <file> [--] [args...]\n"
      "       %s [options] -r <code> [--] [args...]\n"
      "       %s [options] [-B <begin_code>] -R <code> [-E <end_code>] [--] "
      "[args...]\n"
      "       %s [options] [-B <begin_code>] -F <file> [-E <end_code>] [--] "
      "[args...]\n"
      "       %s [options] -- [args...]\n"
      "       %s [options] -a\n"
      "\n"
#if (HAVE_LIBREADLINE || HAVE_LIBEDIT) && !defined(COMPILE_DL_READLINE)
      "  -a               Run as interactive shell\n"
#else
      "  -a               Run interactively\n"
#endif
      "  -c <path>|<file> Look for php.ini file in this directory\n"
      "  -n               No php.ini file will be used\n"
      "  -d foo[=bar]     Define INI entry foo with value 'bar'\n"
      "  -e               Generate extended information for debugger/profiler\n"
      "  -f <file>        Parse and execute <file>.\n"
      "  -h               This help\n"
      "  -i               PHP information\n"
      "  -l               Syntax check only (lint)\n"
      "  -m               Show compiled in modules\n"
      "  -r <code>        Run PHP <code> without using script tags <?..?>\n"
      "  -B <begin_code>  Run PHP <begin_code> before processing input lines\n"
      "  -R <code>        Run PHP <code> for every input line\n"
      "  -F <file>        Parse and execute <file> for every input line\n"
      "  -E <end_code>    Run PHP <end_code> after processing all input lines\n"
      "  -H               Hide any passed arguments from external tools.\n"
      "  -s               Output HTML syntax highlighted source.\n"
      "  -v               Version number\n"
      "  -w               Output source with stripped comments and "
      "whitespace.\n"
      "  -z <file>        Load Zend extension <file>.\n"
      "\n"
      "  args...          Arguments passed to script. Use -- args when first "
      "argument\n"
      "                   starts with - or script is read from stdin\n"
      "\n"
      "  --ini            Show configuration file names\n"
      "\n"
#if (HAVE_REFLECTION)
      "  --rf <name>      Show information about function <name>.\n"
      "  --rc <name>      Show information about class <name>.\n"
      "  --re <name>      Show information about extension <name>.\n"
      "  --rz <name>      Show information about Zend extension <name>.\n"
#endif
      "  --ri <name>      Show configuration for extension <name>.\n"
      "\n",
      prog, prog, prog, prog, prog, prog);
}
/* }}} */

static php_stream *s_in_process = NULL;

static void cli_register_file_handles(TSRMLS_D) /* {{{ */
{
  zval *zin, *zout, *zerr;
  php_stream *s_in, *s_out, *s_err;
  php_stream_context *sc_in = NULL, *sc_out = NULL, *sc_err = NULL;
  zend_constant ic, oc, ec;

  MAKE_STD_ZVAL(zin);
  MAKE_STD_ZVAL(zout);
  MAKE_STD_ZVAL(zerr);

  s_in = php_stream_open_wrapper_ex("php://stdin", "rb", 0, NULL, sc_in);
  s_out = php_stream_open_wrapper_ex("php://stdout", "wb", 0, NULL, sc_out);
  s_err = php_stream_open_wrapper_ex("php://stderr", "wb", 0, NULL, sc_err);

  if (s_in == NULL || s_out == NULL || s_err == NULL) {
    FREE_ZVAL(zin);
    FREE_ZVAL(zout);
    FREE_ZVAL(zerr);
    if (s_in)
      php_stream_close(s_in);
    if (s_out)
      php_stream_close(s_out);
    if (s_err)
      php_stream_close(s_err);
    return;
  }

#if PHP_DEBUG
  /* do not close stdout and stderr */
  s_out->flags |= PHP_STREAM_FLAG_NO_CLOSE;
  s_err->flags |= PHP_STREAM_FLAG_NO_CLOSE;
#endif

  s_in_process = s_in;

  php_stream_to_zval(s_in, zin);
  php_stream_to_zval(s_out, zout);
  php_stream_to_zval(s_err, zerr);

  ic.value = *zin;
  ic.flags = CONST_CS;
  ic.name = zend_strndup(ZEND_STRL("STDIN"));
  ic.name_len = sizeof("STDIN");
  ic.module_number = 0;
  zend_register_constant(&ic TSRMLS_CC);

  oc.value = *zout;
  oc.flags = CONST_CS;
  oc.name = zend_strndup(ZEND_STRL("STDOUT"));
  oc.name_len = sizeof("STDOUT");
  oc.module_number = 0;
  zend_register_constant(&oc TSRMLS_CC);

  ec.value = *zerr;
  ec.flags = CONST_CS;
  ec.name = zend_strndup(ZEND_STRL("STDERR"));
  ec.name_len = sizeof("STDERR");
  ec.module_number = 0;
  zend_register_constant(&ec TSRMLS_CC);

  FREE_ZVAL(zin);
  FREE_ZVAL(zout);
  FREE_ZVAL(zerr);
}
/* }}} */

static const char *param_mode_conflict =
    "Either execute direct code, process stdin or use a file.\n";

/* {{{ cli_seek_file_begin
 */
static int cli_seek_file_begin(zend_file_handle *file_handle, char *script_file,
                               int *lineno TSRMLS_DC) {
  char c;

  *lineno = 1;

  file_handle->type = ZEND_HANDLE_FP;
  file_handle->opened_path = NULL;
  file_handle->free_filename = 0;
  if (!(file_handle->handle.fp = VCWD_FOPEN(script_file, "rb"))) {
    php_printf("Could not open input file: %s\n", script_file);
    return FAILURE;
  }
  file_handle->filename = script_file;

  /* #!php support */
  c = fgetc(file_handle->handle.fp);
  if (c == '#' && (c = fgetc(file_handle->handle.fp)) == '!') {
    while (c != '\n' && c != '\r' && c != EOF) {
      c = fgetc(file_handle->handle.fp); /* skip to end of line */
    }
    /* handle situations where line is terminated by \r\n */
    if (c == '\r') {
      if (fgetc(file_handle->handle.fp) != '\n') {
        long pos = ftell(file_handle->handle.fp);
        fseek(file_handle->handle.fp, pos - 1, SEEK_SET);
      }
    }
    *lineno = 2;
  } else {
    rewind(file_handle->handle.fp);
  }

  return SUCCESS;
}
/* }}} */

/* {{{ main
 */
#ifdef PHP_CLI_WIN32_NO_CONSOLE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd)
#else
int main(int argc, char *argv[])
#endif
{
  volatile int exit_status = SUCCESS;
  int c;
  zend_file_handle file_handle;
  /* temporary locals */
  int behavior = PHP_MODE_STANDARD;
  char *reflection_what = NULL;
  int orig_optind = php_optind;
  char *orig_optarg = php_optarg;
  char *arg_free = NULL, **arg_excp = &arg_free;
  char *script_file = NULL;
  int interactive = 0;
  volatile int module_started = 0;
  volatile int request_started = 0;
  int lineno = 0;
  char *exec_direct = NULL, *exec_run = NULL, *exec_begin = NULL,
       *exec_end = NULL;
  const char *param_error = NULL;
  int hide_argv = 0;
/* end of temporary locals */
#ifdef ZTS
  void ***tsrm_ls;
#endif
#ifdef PHP_CLI_WIN32_NO_CONSOLE
  int argc = __argc;
  char **argv = __argv;
#endif
  int ini_entries_len = 0;

#if defined(PHP_WIN32) && defined(_DEBUG) && defined(PHP_WIN32_DEBUG_HEAP)
  {
    int tmp_flag;
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    tmp_flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmp_flag |= _CRTDBG_DELAY_FREE_MEM_DF;
    tmp_flag |= _CRTDBG_LEAK_CHECK_DF;

    _CrtSetDbgFlag(tmp_flag);
  }
#endif

#ifdef HAVE_SIGNAL_H
#if defined(SIGPIPE) && defined(SIG_IGN)
  signal(SIGPIPE, SIG_IGN); /* ignore SIGPIPE in standalone mode so
                                                          that sockets created
                               via fsockopen() don't kill PHP if the remote site
                                                          closes it.  in
                               apache|apxs mode apache does that for us!
                               thies@thieso.net 20000419 */
#endif
#endif

#ifdef ZTS
  tsrm_startup(1, 1, 0, NULL);
  tsrm_ls = ts_resource(0);
#endif

  cli_sapi_module.ini_defaults = sapi_cli_ini_defaults;
  cli_sapi_module.php_ini_path_override = NULL;
  cli_sapi_module.phpinfo_as_text = 1;
  cli_sapi_module.php_ini_ignore_cwd = 1;
  sapi_startup(&cli_sapi_module);

#ifdef PHP_WIN32
  _fmode = _O_BINARY; /*sets default for file streams to binary */
  setmode(_fileno(stdin), O_BINARY);  /* make the stdio mode be binary */
  setmode(_fileno(stdout), O_BINARY); /* make the stdio mode be binary */
  setmode(_fileno(stderr), O_BINARY); /* make the stdio mode be binary */
#endif

  ini_entries_len = sizeof(HARDCODED_INI) - 2;
  cli_sapi_module.ini_entries = malloc(sizeof(HARDCODED_INI));
  memcpy(cli_sapi_module.ini_entries, HARDCODED_INI, sizeof(HARDCODED_INI));

  while ((c = php_getopt(argc, argv, OPTIONS, &php_optarg, &php_optind, 0,
                         2)) != -1) {
    switch (c) {
    case 'c':
      if (cli_sapi_module.php_ini_path_override) {
        free(cli_sapi_module.php_ini_path_override);
      }
      cli_sapi_module.php_ini_path_override = strdup(php_optarg);
      break;
    case 'n':
      cli_sapi_module.php_ini_ignore = 1;
      break;
    case 'd': {
      /* define ini entries on command line */
      int len = strlen(php_optarg);
      char *val;

      if ((val = strchr(php_optarg, '='))) {
        val++;
        if (!isalnum(*val) && *val != '"' && *val != '\'' && *val != '\0') {
          cli_sapi_module.ini_entries =
              realloc(cli_sapi_module.ini_entries,
                      ini_entries_len + len + sizeof("\"\"\n\0"));
          memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
                 (val - php_optarg));
          ini_entries_len += (val - php_optarg);
          memcpy(cli_sapi_module.ini_entries + ini_entries_len, "\"", 1);
          ini_entries_len++;
          memcpy(cli_sapi_module.ini_entries + ini_entries_len, val,
                 len - (val - php_optarg));
          ini_entries_len += len - (val - php_optarg);
          memcpy(cli_sapi_module.ini_entries + ini_entries_len, "\"\n\0",
                 sizeof("\"\n\0"));
          ini_entries_len += sizeof("\n\0\"") - 2;
        } else {
          cli_sapi_module.ini_entries =
              realloc(cli_sapi_module.ini_entries,
                      ini_entries_len + len + sizeof("\n\0"));
          memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
                 len);
          memcpy(cli_sapi_module.ini_entries + ini_entries_len + len, "\n\0",
                 sizeof("\n\0"));
          ini_entries_len += len + sizeof("\n\0") - 2;
        }
      } else {
        cli_sapi_module.ini_entries =
            realloc(cli_sapi_module.ini_entries,
                    ini_entries_len + len + sizeof("=1\n\0"));
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg, len);
        memcpy(cli_sapi_module.ini_entries + ini_entries_len + len, "=1\n\0",
               sizeof("=1\n\0"));
        ini_entries_len += len + sizeof("=1\n\0") - 2;
      }
      break;
    }
    }
  }
  php_optind = orig_optind;
  php_optarg = orig_optarg;

  cli_sapi_module.executable_location = argv[0];
  cli_sapi_module.additional_functions = additional_functions;

  /* startup after we get the above ini override se we get things right */
  if (cli_sapi_module.startup(&cli_sapi_module) == FAILURE) {
    /* there is no way to see if we must call zend_ini_deactivate()
     * since we cannot check if EG(ini_directives) has been initialised
     * because the executor's constructor does not set initialize it.
     * Apart from that there seems no need for zend_ini_deactivate() yet.
     * So we goto out_err.*/
    exit_status = 1;
    goto out_err;
  }
  module_started = 1;

  zend_first_try {
    CG(in_compilation) = 0; /* not initialized but needed for several options */
    EG(uninitialized_zval_ptr) = NULL;

    while ((c = php_getopt(argc, argv, OPTIONS, &php_optarg, &php_optind, 0,
                           2)) != -1) {
      switch (c) {

      case 'h': /* help & quit */
      case '?':
        if (php_request_startup(TSRMLS_C) == FAILURE) {
          goto err;
        }
        request_started = 1;
        php_cli_usage(argv[0]);
        php_output_end_all(TSRMLS_C);
        exit_status = 0;
        goto out;

      case 'i': /* php info & quit */
        if (php_request_startup(TSRMLS_C) == FAILURE) {
          goto err;
        }
        request_started = 1;
        php_print_info(0xFFFFFFFF TSRMLS_CC);
        php_output_end_all(TSRMLS_C);
        exit_status = 0;
        goto out;

      case 'm': /* list compiled in modules */
        if (php_request_startup(TSRMLS_C) == FAILURE) {
          goto err;
        }
        request_started = 1;
        php_printf("[PHP Modules]\n");
        print_modules(TSRMLS_C);
        php_printf("\n[Zend Modules]\n");
        print_extensions(TSRMLS_C);
        php_printf("\n");
        php_output_end_all(TSRMLS_C);
        exit_status = 0;
        goto out;

      case 'v': /* show php version & quit */
        if (php_request_startup(TSRMLS_C) == FAILURE) {
          goto err;
        }

        request_started = 1;
        php_printf("PHP %s (%s) (built: %s %s) %s\nCopyright (c) 1997-2011 The "
                   "PHP Group\n%s",
                   PHP_VERSION, sapi_module.name, __DATE__, __TIME__,
#if ZEND_DEBUG && defined(HAVE_GCOV)
                   "(DEBUG GCOV)",
#elif ZEND_DEBUG
                   "(DEBUG)",
#elif defined(HAVE_GCOV)
                   "(GCOV)",
#else
                   "",
#endif
                   get_zend_version());
        php_output_end_all(TSRMLS_C);
        exit_status = 0;
        goto out;

      default:
        break;
      }
    }

    /* Set some CLI defaults */
    SG(options) |= SAPI_OPTION_NO_CHDIR;

    php_optind = orig_optind;
    php_optarg = orig_optarg;
    while ((c = php_getopt(argc, argv, OPTIONS, &php_optarg, &php_optind, 0,
                           2)) != -1) {
      switch (c) {

      case 'a': /* interactive mode */
        if (!interactive) {
          if (behavior != PHP_MODE_STANDARD) {
            param_error = param_mode_conflict;
            break;
          }

          interactive = 1;
        }
        break;

      case 'C': /* don't chdir to the script directory */
        /* This is default so NOP */
        break;

      case 'e': /* enable extended info output */
        CG(compiler_options) |= ZEND_COMPILE_EXTENDED_INFO;
        break;

      case 'F':
        if (behavior == PHP_MODE_PROCESS_STDIN) {
          if (exec_run || script_file) {
            param_error = "You can use -R or -F only once.\n";
            break;
          }
        } else if (behavior != PHP_MODE_STANDARD) {
          param_error = param_mode_conflict;
          break;
        }
        behavior = PHP_MODE_PROCESS_STDIN;
        script_file = php_optarg;
        break;

      case 'f': /* parse file */
        if (behavior == PHP_MODE_CLI_DIRECT ||
            behavior == PHP_MODE_PROCESS_STDIN) {
          param_error = param_mode_conflict;
          break;
        } else if (script_file) {
          param_error = "You can use -f only once.\n";
          break;
        }
        script_file = php_optarg;
        break;

      case 'l': /* syntax check mode */
        if (behavior != PHP_MODE_STANDARD) {
          break;
        }
        behavior = PHP_MODE_LINT;
        break;

#if 0 /* not yet operational, see also below ... */
			case '': /* generate indented source mode*/
				if (behavior == PHP_MODE_CLI_DIRECT || behavior == PHP_MODE_PROCESS_STDIN) {
					param_error = "Source indenting only works for files.\n";
					break;
				}
				behavior=PHP_MODE_INDENT;
				break;
#endif

      case 'q': /* do not generate HTTP headers */
        /* This is default so NOP */
        break;

      case 'r': /* run code from command line */
        if (behavior == PHP_MODE_CLI_DIRECT) {
          if (exec_direct || script_file) {
            param_error = "You can use -r only once.\n";
            break;
          }
        } else if (behavior != PHP_MODE_STANDARD || interactive) {
          param_error = param_mode_conflict;
          break;
        }
        behavior = PHP_MODE_CLI_DIRECT;
        exec_direct = php_optarg;
        break;

      case 'R':
        if (behavior == PHP_MODE_PROCESS_STDIN) {
          if (exec_run || script_file) {
            param_error = "You can use -R or -F only once.\n";
            break;
          }
        } else if (behavior != PHP_MODE_STANDARD) {
          param_error = param_mode_conflict;
          break;
        }
        behavior = PHP_MODE_PROCESS_STDIN;
        exec_run = php_optarg;
        break;

      case 'B':
        if (behavior == PHP_MODE_PROCESS_STDIN) {
          if (exec_begin) {
            param_error = "You can use -B only once.\n";
            break;
          }
        } else if (behavior != PHP_MODE_STANDARD || interactive) {
          param_error = param_mode_conflict;
          break;
        }
        behavior = PHP_MODE_PROCESS_STDIN;
        exec_begin = php_optarg;
        break;

      case 'E':
        if (behavior == PHP_MODE_PROCESS_STDIN) {
          if (exec_end) {
            param_error = "You can use -E only once.\n";
            break;
          }
        } else if (behavior != PHP_MODE_STANDARD || interactive) {
          param_error = param_mode_conflict;
          break;
        }
        behavior = PHP_MODE_PROCESS_STDIN;
        exec_end = php_optarg;
        break;

      case 's': /* generate highlighted HTML from source */
        if (behavior == PHP_MODE_CLI_DIRECT ||
            behavior == PHP_MODE_PROCESS_STDIN) {
          param_error = "Source highlighting only works for files.\n";
          break;
        }
        behavior = PHP_MODE_HIGHLIGHT;
        break;

      case 'w':
        if (behavior == PHP_MODE_CLI_DIRECT ||
            behavior == PHP_MODE_PROCESS_STDIN) {
          param_error = "Source stripping only works for files.\n";
          break;
        }
        behavior = PHP_MODE_STRIP;
        break;

      case 'z': /* load extension file */
        zend_load_extension(php_optarg);
        break;
      case 'H':
        hide_argv = 1;
        break;

#ifdef HAVE_REFLECTION
      case 10:
        behavior = PHP_MODE_REFLECTION_FUNCTION;
        reflection_what = php_optarg;
        break;
      case 11:
        behavior = PHP_MODE_REFLECTION_CLASS;
        reflection_what = php_optarg;
        break;
      case 12:
        behavior = PHP_MODE_REFLECTION_EXTENSION;
        reflection_what = php_optarg;
        break;
      case 13:
        behavior = PHP_MODE_REFLECTION_ZEND_EXTENSION;
        reflection_what = php_optarg;
        break;
#endif
      case 14:
        behavior = PHP_MODE_REFLECTION_EXT_INFO;
        reflection_what = php_optarg;
        break;
      case 15:
        behavior = PHP_MODE_SHOW_INI_CONFIG;
        break;
      default:
        break;
      }
    }

    if (param_error) {
      PUTS(param_error);
      exit_status = 1;
      goto err;
    }

    if (interactive) {
#if (HAVE_LIBREADLINE || HAVE_LIBEDIT) && !defined(COMPILE_DL_READLINE)
      printf("Interactive shell\n\n");
#else
      printf("Interactive mode enabled\n\n");
#endif
      fflush(stdout);
    }

    CG(interactive) = interactive;

    /* only set script_file if not set already and not in direct mode and not at
     * end of parameter list */
    if (argc > php_optind && !script_file && behavior != PHP_MODE_CLI_DIRECT &&
        behavior != PHP_MODE_PROCESS_STDIN &&
        strcmp(argv[php_optind - 1], "--")) {
      script_file = argv[php_optind];
      php_optind++;
    }
    if (script_file) {
      if (cli_seek_file_begin(&file_handle, script_file, &lineno TSRMLS_CC) !=
          SUCCESS) {
        goto err;
      }
      script_filename = script_file;
    } else {
      /* We could handle PHP_MODE_PROCESS_STDIN in a different manner  */
      /* here but this would make things only more complicated. And it */
      /* is consitent with the way -R works where the stdin file handle*/
      /* is also accessible. */
      file_handle.filename = "-";
      file_handle.handle.fp = stdin;
    }
    file_handle.type = ZEND_HANDLE_FP;
    file_handle.opened_path = NULL;
    file_handle.free_filename = 0;
    php_self = file_handle.filename;

    /* before registering argv to module exchange the *new* argv[0] */
    /* we can achieve this without allocating more memory */
    SG(request_info).argc = argc - php_optind + 1;
    arg_excp = argv + php_optind - 1;
    arg_free = argv[php_optind - 1];
    SG(request_info).path_translated = file_handle.filename;
    argv[php_optind - 1] = file_handle.filename;
    SG(request_info).argv = argv + php_optind - 1;

    if (php_request_startup(TSRMLS_C) == FAILURE) {
      *arg_excp = arg_free;
      fclose(file_handle.handle.fp);
      PUTS("Could not startup.\n");
      goto err;
    }
    request_started = 1;
    CG(start_lineno) = lineno;
    *arg_excp = arg_free; /* reconstuct argv */

    if (hide_argv) {
      int i;
      for (i = 1; i < argc; i++) {
        memset(argv[i], 0, strlen(argv[i]));
      }
    }

    zend_is_auto_global("_SERVER", sizeof("_SERVER") - 1 TSRMLS_CC);

    PG(during_request_startup) = 0;
    switch (behavior) {
    case PHP_MODE_STANDARD:
      if (strcmp(file_handle.filename, "-")) {
        cli_register_file_handles(TSRMLS_C);
      }

      if (interactive && cli_shell_callbacks.cli_shell_run) {
        exit_status = cli_shell_callbacks.cli_shell_run(TSRMLS_C);
      } else {
        php_execute_script(&file_handle TSRMLS_CC);
        exit_status = EG(exit_status);
      }

      break;
    case PHP_MODE_LINT:
      exit_status = php_lint_script(&file_handle TSRMLS_CC);
      if (exit_status == SUCCESS) {
        zend_printf("No syntax errors detected in %s\n", file_handle.filename);
      } else {
        zend_printf("Errors parsing %s\n", file_handle.filename);
      }
      break;
    case PHP_MODE_STRIP:
      if (open_file_for_scanning(&file_handle TSRMLS_CC) == SUCCESS) {
        zend_strip(TSRMLS_C);
      }
      goto out;
      break;
    case PHP_MODE_HIGHLIGHT: {
      zend_syntax_highlighter_ini syntax_highlighter_ini;

      if (open_file_for_scanning(&file_handle TSRMLS_CC) == SUCCESS) {
        php_get_highlight_struct(&syntax_highlighter_ini);
        zend_highlight(&syntax_highlighter_ini TSRMLS_CC);
      }
      goto out;
    } break;
#if 0
			/* Zeev might want to do something with this one day */
		case PHP_MODE_INDENT:
			open_file_for_scanning(&file_handle TSRMLS_CC);
			zend_indent();
			zend_file_handle_dtor(file_handle.handle TSRMLS_CC);
			goto out;
			break;
#endif
    case PHP_MODE_CLI_DIRECT:
      cli_register_file_handles(TSRMLS_C);
      if (zend_eval_string_ex(exec_direct, NULL, "Command line code",
                              1 TSRMLS_CC) == FAILURE) {
        exit_status = 254;
      }
      break;

    case PHP_MODE_PROCESS_STDIN: {
      char *input;
      size_t len, index = 0;
      zval *argn, *argi;

      // prophet generated patch
      switch (__choose("__ID95")) {
      case 0: {
        cli_register_file_handles();

        break;
      }
#ifdef COMPILE_21457
      case 1: {
        ((argi))->is_ref__gc = 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21458
      case 2: {
        ((argi))->refcount__gc = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21459
      case 3: {
        ((argn))->is_ref__gc = 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21460
      case 4: {
        ((argn))->refcount__gc = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21461
      case 5: {
        (*(argi)).type = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21462
      case 6: {
        (*(argi)).type = 6;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21463
      case 7: {
        (*(argi)).value.lval = ++index;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21464
      case 8: {
        (*(argi)).value.lval = index;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21465
      case 9: {
        (*(argi)).value.str.len = ++len;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21466
      case 10: {
        (*(argi)).value.str.val = _estrndup((input), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21467
      case 11: {
        (*(argn)).type = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21468
      case 12: {
        (*(argn)).type = 6;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21469
      case 13: {
        (*(argn)).value.lval = ++index;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21470
      case 14: {
        (*(argn)).value.lval = index;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21471
      case 15: {
        (*(argn)).value.str.len = ++len;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21472
      case 16: {
        (*(argn)).value.str.val = _estrndup((input), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21473
      case 17: {
        (*argi).type = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21474
      case 18: {
        (*argi).value.lval = ++index;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21475
      case 19: {
        (*argi).value.lval = index;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21476
      case 20: {
        (*argn).type = 6;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21477
      case 21: {
        (*argn).value.str.len = ++len;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21478
      case 22: {
        (*argn).value.str.val = _estrndup(((arg_free)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21479
      case 23: {
        (*argn).value.str.val = _estrndup(((exec_begin)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21480
      case 24: {
        (*argn).value.str.val = _estrndup(((exec_direct)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21481
      case 25: {
        (*argn).value.str.val = _estrndup(((exec_end)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21482
      case 26: {
        (*argn).value.str.val = _estrndup(((exec_run)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21483
      case 27: {
        (*argn).value.str.val = _estrndup(((input)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21484
      case 28: {
        (*argn).value.str.val = _estrndup(((orig_optarg)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21485
      case 29: {
        (*argn).value.str.val = _estrndup(((reflection_what)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21486
      case 30: {
        (*argn).value.str.val = _estrndup(((script_file)), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21487
      case 31: {
        (*argn).value.str.val = _estrndup((input), (len));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21488
      case 32: {
        (arg_excp) = argv + php_optind - 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21489
      case 33: {
        (arg_excp)[php_optind - 1] = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21490
      case 34: {
        (arg_free) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21491
      case 35: {
        (arg_free) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21492
      case 36: {
        (arg_free) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21493
      case 37: {
        (arg_free) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21494
      case 38: {
        (arg_free) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21495
      case 39: {
        (arg_free) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21496
      case 40: {
        (arg_free)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21497
      case 41: {
        (arg_free)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21498
      case 42: {
        (argi)->is_ref__gc = 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21499
      case 43: {
        (argi)->refcount__gc = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21500
      case 44: {
        (argn)->is_ref__gc = 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21501
      case 45: {
        (argn)->refcount__gc = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21502
      case 46: {
        (argv) = argv + php_optind - 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21503
      case 47: {
        (argv)[php_optind - 1] = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21504
      case 48: {
        (compiler_globals.in_compilation) = 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21505
      case 49: {
        (compiler_globals.interactive) = interactive;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21506
      case 50: {
        (compiler_globals.start_lineno) = lineno;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21507
      case 51: {
        (core_globals.during_request_startup) = 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21508
      case 52: {
        (exec_begin) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21509
      case 53: {
        (exec_begin) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21510
      case 54: {
        (exec_begin) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21511
      case 55: {
        (exec_begin) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21512
      case 56: {
        (exec_begin) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21513
      case 57: {
        (exec_begin) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21514
      case 58: {
        (exec_begin)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21515
      case 59: {
        (exec_begin)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21516
      case 60: {
        (exec_direct) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21517
      case 61: {
        (exec_direct) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21518
      case 62: {
        (exec_direct) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21519
      case 63: {
        (exec_direct) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21520
      case 64: {
        (exec_direct) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21521
      case 65: {
        (exec_direct) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21522
      case 66: {
        (exec_direct)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21523
      case 67: {
        (exec_direct)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21524
      case 68: {
        (exec_end) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21525
      case 69: {
        (exec_end) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21526
      case 70: {
        (exec_end) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21527
      case 71: {
        (exec_end) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21528
      case 72: {
        (exec_end) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21529
      case 73: {
        (exec_end) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21530
      case 74: {
        (exec_end)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21531
      case 75: {
        (exec_end)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21532
      case 76: {
        (exec_run) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21533
      case 77: {
        (exec_run) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21534
      case 78: {
        (exec_run) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21535
      case 79: {
        (exec_run) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21536
      case 80: {
        (exec_run) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21537
      case 81: {
        (exec_run) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21538
      case 82: {
        (exec_run)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21539
      case 83: {
        (exec_run)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21540
      case 84: {
        (executor_globals.argument_stack) =
            zend_vm_stack_new_page(((16 * 1024) - 16));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21541
      case 85: {
        (executor_globals.bailout) = &__bailout;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21542
      case 86: {
        (executor_globals.bailout) = ((void *)0);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21543
      case 87: {
        (executor_globals.bailout) = (__orig_bailout);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21544
      case 88: {
        (executor_globals.bailout) = __orig_bailout;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21545
      case 89: {
        (executor_globals.exception) = ((void *)0);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21546
      case 90: {
        (executor_globals.uninitialized_zval_ptr) = ((void *)0);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21547
      case 91: {
        (input) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21548
      case 92: {
        (input) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21549
      case 93: {
        (input) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21550
      case 94: {
        (input) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21551
      case 95: {
        (input) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21552
      case 96: {
        (input) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21553
      case 97: {
        (input)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21554
      case 98: {
        (input)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21555
      case 99: {
        (orig_optarg) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21556
      case 100: {
        (orig_optarg) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21557
      case 101: {
        (orig_optarg) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21558
      case 102: {
        (orig_optarg) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21559
      case 103: {
        (orig_optarg) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21560
      case 104: {
        (orig_optarg) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21561
      case 105: {
        (orig_optarg)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21562
      case 106: {
        (orig_optarg)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21563
      case 107: {
        (param_error) = "Source highlighting only works for files.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21564
      case 108: {
        (param_error) = "Source stripping only works for files.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21565
      case 109: {
        (param_error) = "You can use -B only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21566
      case 110: {
        (param_error) = "You can use -E only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21567
      case 111: {
        (param_error) = "You can use -R or -F only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21568
      case 112: {
        (param_error) = "You can use -f only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21569
      case 113: {
        (param_error) = "You can use -r only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21570
      case 114: {
        (param_error) = param_mode_conflict;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21571
      case 115: {
        (param_error)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21572
      case 116: {
        (reflection_what) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21573
      case 117: {
        (reflection_what) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21574
      case 118: {
        (reflection_what) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21575
      case 119: {
        (reflection_what) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21576
      case 120: {
        (reflection_what) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21577
      case 121: {
        (reflection_what) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21578
      case 122: {
        (reflection_what)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21579
      case 123: {
        (reflection_what)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21580
      case 124: {
        (sapi_globals.request_info).argc = argc - php_optind + 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21581
      case 125: {
        (sapi_globals.request_info).argv = (arg_excp) + php_optind - 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21582
      case 126: {
        (sapi_globals.request_info).argv = (argv) + php_optind - 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21583
      case 127: {
        (sapi_globals.request_info).argv = argv + php_optind - 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21584
      case 128: {
        (sapi_globals.request_info).argv0 = ((void *)0);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21585
      case 129: {
        (sapi_globals.request_info).path_translated = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21586
      case 130: {
        (script_file) = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21587
      case 131: {
        (script_file) = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21588
      case 132: {
        (script_file) = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21589
      case 133: {
        (script_file) = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21590
      case 134: {
        (script_file) = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21591
      case 135: {
        (script_file) = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21592
      case 136: {
        (script_file)++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21593
      case 137: {
        (script_file)[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21594
      case 138: {
        *(arg_excp) = arg_free;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21595
      case 139: {
        *(argv) = arg_free;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21596
      case 140: {
        *arg_excp = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21597
      case 141: {
        *arg_excp = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21598
      case 142: {
        *arg_excp = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21599
      case 143: {
        *arg_excp = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21600
      case 144: {
        *arg_excp = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21601
      case 145: {
        *arg_excp = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21602
      case 146: {
        *arg_excp = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21603
      case 147: {
        *arg_excp = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21604
      case 148: {
        *arg_excp = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21605
      case 149: {
        *arg_excp = arg_free;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21606
      case 150: {
        _efree(((arg_free)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21607
      case 151: {
        _efree(((argi)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21608
      case 152: {
        _efree(((argn)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21609
      case 153: {
        _efree(((exec_begin)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21610
      case 154: {
        _efree(((exec_direct)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21611
      case 155: {
        _efree(((exec_end)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21612
      case 156: {
        _efree(((exec_run)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21613
      case 157: {
        _efree(((input)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21614
      case 158: {
        _efree(((orig_optarg)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21615
      case 159: {
        _efree(((reflection_what)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21616
      case 160: {
        _efree(((script_file)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21617
      case 161: {
        _efree((input));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21618
      case 162: {
        _zend_hash_add_or_update(&(executor_globals.symbol_table), "argi",
                                 sizeof("argi"), &(argi), sizeof(zval *),
                                 ((void *)0), (1 << 0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21619
      case 163: {
        _zend_hash_add_or_update(&(executor_globals.symbol_table), "argi",
                                 sizeof("argi"), &(argn), sizeof(zval *),
                                 ((void *)0), (1 << 0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21620
      case 164: {
        _zend_hash_add_or_update(&(executor_globals.symbol_table), "argi",
                                 sizeof("argi"), &argi, sizeof(zval *),
                                 ((void *)0), (1 << 0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21621
      case 165: {
        _zend_hash_add_or_update(&(executor_globals.symbol_table), "argn",
                                 sizeof("argn"), &(argi), sizeof(zval *),
                                 ((void *)0), (1 << 0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21622
      case 166: {
        _zend_hash_add_or_update(&(executor_globals.symbol_table), "argn",
                                 sizeof("argn"), &(argn), sizeof(zval *),
                                 ((void *)0), (1 << 0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21623
      case 167: {
        _zend_hash_add_or_update(&(executor_globals.symbol_table), "argn",
                                 sizeof("argn"), &argn, sizeof(zval *),
                                 ((void *)0), (1 << 0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21624
      case 168: {
        _zval_copy_ctor_func((argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21625
      case 169: {
        _zval_copy_ctor_func((argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21626
      case 170: {
        _zval_dtor(((argi)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21627
      case 171: {
        _zval_dtor(((argn)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21628
      case 172: {
        _zval_dtor_func((argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21629
      case 173: {
        _zval_dtor_func((argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21630
      case 174: {
        _zval_ptr_dtor((&(argi)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21631
      case 175: {
        _zval_ptr_dtor((&(argn)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21632
      case 176: {
        _zval_ptr_dtor((&(executor_globals.exception)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21633
      case 177: {
        arg_excp = (arg_excp) + php_optind - 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21634
      case 178: {
        arg_excp = (argv) + php_optind - 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21635
      case 179: {
        arg_excp = argv + php_optind - 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21636
      case 180: {
        arg_free = (arg_excp)[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21637
      case 181: {
        arg_free = (argv)[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21638
      case 182: {
        arg_free = argv[php_optind - 1];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21639
      case 183: {
        argv[php_optind - 1] = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21640
      case 184: {
        behavior = 2;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21641
      case 185: {
        behavior = 4;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21642
      case 186: {
        behavior = 5;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21643
      case 187: {
        behavior = 6;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21644
      case 188: {
        behavior = 7;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21645
      case 189: {
        cli_register_file_handles();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21646
      case 190: {
        cli_sapi_module.additional_functions = additional_functions;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21647
      case 191: {
        cli_sapi_module.executable_location = (arg_excp)[0];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21648
      case 192: {
        cli_sapi_module.executable_location = (argv)[0];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21649
      case 193: {
        cli_sapi_module.executable_location = argv[0];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21650
      case 194: {
        cli_sapi_module.ini_defaults = sapi_cli_ini_defaults;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21651
      case 195: {
        cli_sapi_module.ini_entries = malloc(sizeof(HARDCODED_INI));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21652
      case 196: {
        cli_sapi_module.php_ini_ignore_cwd = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21653
      case 197: {
        cli_sapi_module.php_ini_path_override = ((void *)0);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21654
      case 198: {
        cli_sapi_module.php_ini_path_override = strdup((arg_free));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21655
      case 199: {
        cli_sapi_module.php_ini_path_override = strdup((exec_begin));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21656
      case 200: {
        cli_sapi_module.php_ini_path_override = strdup((exec_direct));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21657
      case 201: {
        cli_sapi_module.php_ini_path_override = strdup((exec_end));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21658
      case 202: {
        cli_sapi_module.php_ini_path_override = strdup((exec_run));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21659
      case 203: {
        cli_sapi_module.php_ini_path_override = strdup((input));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21660
      case 204: {
        cli_sapi_module.php_ini_path_override = strdup((orig_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21661
      case 205: {
        cli_sapi_module.php_ini_path_override = strdup((reflection_what));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21662
      case 206: {
        cli_sapi_module.php_ini_path_override = strdup((script_file));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21663
      case 207: {
        cli_sapi_module.php_ini_path_override = strdup(php_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21664
      case 208: {
        cli_sapi_module.phpinfo_as_text = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21665
      case 209: {
        convert_to_boolean((argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21666
      case 210: {
        convert_to_boolean((argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21667
      case 211: {
        display_ini_entries(((void *)0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21668
      case 212: {
        exec_begin = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21669
      case 213: {
        exec_begin = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21670
      case 214: {
        exec_begin = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21671
      case 215: {
        exec_begin = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21672
      case 216: {
        exec_begin = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21673
      case 217: {
        exec_begin = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21674
      case 218: {
        exec_begin = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21675
      case 219: {
        exec_begin = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21676
      case 220: {
        exec_begin = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21677
      case 221: {
        exec_begin = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21678
      case 222: {
        exec_direct = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21679
      case 223: {
        exec_direct = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21680
      case 224: {
        exec_direct = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21681
      case 225: {
        exec_direct = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21682
      case 226: {
        exec_direct = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21683
      case 227: {
        exec_direct = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21684
      case 228: {
        exec_direct = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21685
      case 229: {
        exec_direct = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21686
      case 230: {
        exec_direct = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21687
      case 231: {
        exec_direct = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21688
      case 232: {
        exec_end = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21689
      case 233: {
        exec_end = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21690
      case 234: {
        exec_end = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21691
      case 235: {
        exec_end = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21692
      case 236: {
        exec_end = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21693
      case 237: {
        exec_end = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21694
      case 238: {
        exec_end = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21695
      case 239: {
        exec_end = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21696
      case 240: {
        exec_end = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21697
      case 241: {
        exec_end = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21698
      case 242: {
        exec_run = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21699
      case 243: {
        exec_run = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21700
      case 244: {
        exec_run = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21701
      case 245: {
        exec_run = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21702
      case 246: {
        exec_run = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21703
      case 247: {
        exec_run = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21704
      case 248: {
        exec_run = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21705
      case 249: {
        exec_run = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21706
      case 250: {
        exec_run = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21707
      case 251: {
        exec_run = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21708
      case 252: {
        exit(1);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21709
      case 253: {
        exit(exit_status);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21710
      case 254: {
        exit_status = (executor_globals.exit_status);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21711
      case 255: {
        exit_status = 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21712
      case 256: {
        exit_status = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21713
      case 257: {
        exit_status = 254;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21714
      case 258: {
        exit_status = cli_shell_callbacks.cli_shell_run();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21715
      case 259: {
        fclose(file_handle.handle.fp);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21716
      case 260: {
        file_handle.filename = "-";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21717
      case 261: {
        file_handle.free_filename = 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21718
      case 262: {
        file_handle.opened_path = ((void *)0);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21719
      case 263: {
        file_handle.type = (ZEND_HANDLE_FD);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21720
      case 264: {
        file_handle.type = (ZEND_HANDLE_FILENAME);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21721
      case 265: {
        file_handle.type = (ZEND_HANDLE_FP);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21722
      case 266: {
        file_handle.type = (ZEND_HANDLE_MAPPED);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21723
      case 267: {
        file_handle.type = (ZEND_HANDLE_STREAM);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21724
      case 268: {
        file_handle.type = ZEND_HANDLE_FP;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21725
      case 269: {
        free((sapi_globals.request_info).argv0);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21726
      case 270: {
        free(cli_sapi_module.ini_entries);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21727
      case 271: {
        free(cli_sapi_module.php_ini_path_override);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21728
      case 272: {
        gc_remove_zval_from_buffer((argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21729
      case 273: {
        gc_remove_zval_from_buffer((argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21730
      case 274: {
        gc_zval_check_possible_root(((argi)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21731
      case 275: {
        gc_zval_check_possible_root(((argn)));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21732
      case 276: {
        gc_zval_possible_root((argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21733
      case 277: {
        gc_zval_possible_root((argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21734
      case 278: {
        i_zval_ptr_dtor((argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21735
      case 279: {
        i_zval_ptr_dtor((argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21736
      case 280: {
        if (!__is_neg("95-280", 9,
                      (int *){lineno, behavior, interactive, argc, hide_argv,
                              php_optind, c, orig_optind, ini_entries_len},
                      0, 0, 30,
                      (void **){exec_begin,
                                argi,
                                input,
                                s_in_process,
                                exec_direct,
                                argn,
                                exec_run,
                                script_file,
                                exec_end,
                                zend_printf,
                                reflection_what,
                                reflection_method_ptr,
                                reflection_function_ptr,
                                reflection_class_ptr,
                                argv,
                                reflection_extension_ptr,
                                reflection_zend_extension_ptr,
                                arg_free,
                                arg_excp,
                                php_self,
                                reflection_ptr,
                                script_filename,
                                php_ini_opened_path,
                                php_ini_scanned_path,
                                php_ini_scanned_files,
                                __orig_bailout,
                                param_error,
                                php_optarg,
                                param_mode_conflict,
                                orig_optarg},
                      0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                      "input", "s_in_process", "exec_direct", "len", "argn",
                      "exec_run", "script_file", "lineno", "exec_end",
                      "zend_printf", "behavior", "interactive",
                      "reflection_what", "reflection_method_ptr",
                      "reflection_function_ptr", "reflection_class_ptr", "argv",
                      "argc", "reflection_extension_ptr", "hide_argv",
                      "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                      "request_started", "php_optind", "php_self",
                      "reflection_ptr", "script_filename",
                      "php_ini_opened_path", "php_ini_scanned_path",
                      "php_ini_scanned_files", "__orig_bailout", "param_error",
                      "php_optarg", "module_started", "param_mode_conflict",
                      "c", "orig_optarg", "orig_optind", "ini_entries_len"))
          cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21737
      case 281: {
        if (!strcmp(reflection_what, "main")) {
          display_ini_entries(((void *)0));
        } else {
          zend_printf("Extension '%s' not present.\n", reflection_what);
          exit_status = 1;
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21738
      case 282: {
        if ((sapi_globals.request_info).argv0) {
          free((sapi_globals.request_info).argv0);
          (sapi_globals.request_info).argv0 = ((void *)0);
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21739
      case 283: {
        if (__is_neg("95-283", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          goto err;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21740
      case 284: {
        if (__is_neg("95-284", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          goto out;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21741
      case 285: {
        if (__is_neg("95-285", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          goto out_err;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21742
      case 286: {
        if (__is_neg("95-286", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return -1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21743
      case 287: {
        if (__is_neg("95-287", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 0;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21744
      case 288: {
        if (__is_neg("95-288", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 10;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21745
      case 289: {
        if (__is_neg("95-289", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 11;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21746
      case 290: {
        if (__is_neg("95-290", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 12;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21747
      case 291: {
        if (__is_neg("95-291", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 13;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21748
      case 292: {
        if (__is_neg("95-292", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 14;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21749
      case 293: {
        if (__is_neg("95-293", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 15;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21750
      case 294: {
        if (__is_neg("95-294", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21751
      case 295: {
        if (__is_neg("95-295", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 254;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21752
      case 296: {
        if (__is_neg("95-296", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 2;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21753
      case 297: {
        if (__is_neg("95-297", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 4;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21754
      case 298: {
        if (__is_neg("95-298", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 5;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21755
      case 299: {
        if (__is_neg("95-299", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 6;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21756
      case 300: {
        if (__is_neg("95-300", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 7;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21757
      case 301: {
        if (__is_neg("95-301", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 8;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21758
      case 302: {
        if (__is_neg("95-302", 9,
                     (int *){lineno, behavior, interactive, argc, hide_argv,
                             php_optind, c, orig_optind, ini_entries_len},
                     0, 0, 30,
                     (void **){exec_begin,
                               argi,
                               input,
                               s_in_process,
                               exec_direct,
                               argn,
                               exec_run,
                               script_file,
                               exec_end,
                               zend_printf,
                               reflection_what,
                               reflection_method_ptr,
                               reflection_function_ptr,
                               reflection_class_ptr,
                               argv,
                               reflection_extension_ptr,
                               reflection_zend_extension_ptr,
                               arg_free,
                               arg_excp,
                               php_self,
                               reflection_ptr,
                               script_filename,
                               php_ini_opened_path,
                               php_ini_scanned_path,
                               php_ini_scanned_files,
                               __orig_bailout,
                               param_error,
                               php_optarg,
                               param_mode_conflict,
                               orig_optarg},
                     0, 0, 44, "exec_begin", "exit_status", "argi", "index",
                     "input", "s_in_process", "exec_direct", "len", "argn",
                     "exec_run", "script_file", "lineno", "exec_end",
                     "zend_printf", "behavior", "interactive",
                     "reflection_what", "reflection_method_ptr",
                     "reflection_function_ptr", "reflection_class_ptr", "argv",
                     "argc", "reflection_extension_ptr", "hide_argv",
                     "reflection_zend_extension_ptr", "arg_free", "arg_excp",
                     "request_started", "php_optind", "php_self",
                     "reflection_ptr", "script_filename", "php_ini_opened_path",
                     "php_ini_scanned_path", "php_ini_scanned_files",
                     "__orig_bailout", "param_error", "php_optarg",
                     "module_started", "param_mode_conflict", "c",
                     "orig_optarg", "orig_optind", "ini_entries_len"))
          return 9;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21759
      case 303: {
        if (argc > php_optind && !script_file && behavior != 6 &&
            behavior != 7 && strcmp(argv[php_optind - 1], "--")) {
          script_file = argv[php_optind];
          php_optind++;
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21760
      case 304: {
        if (cli_sapi_module.ini_entries) {
          free(cli_sapi_module.ini_entries);
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21761
      case 305: {
        if (cli_sapi_module.php_ini_path_override) {
          free(cli_sapi_module.php_ini_path_override);
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21762
      case 306: {
        if (cli_seek_file_begin(&file_handle, script_file, &lineno) != 0) {
          exit_status = 1;
        } else {
          (compiler_globals.start_lineno) = lineno;
          php_execute_script(&file_handle);
          exit_status = (executor_globals.exit_status);
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21763
      case 307: {
        if (exec_begin &&
            zend_eval_string_ex(exec_begin, ((void *)0),
                                "Command line begin code", 1) == -1) {
          exit_status = 254;
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21764
      case 308: {
        if (exec_end && zend_eval_string_ex(exec_end, ((void *)0),
                                            "Command line end code", 1) == -1) {
          exit_status = 254;
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21765
      case 309: {
        if (exit_status == 0) {
          exit_status = (executor_globals.exit_status);
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21766
      case 310: {
        if (exit_status == 0) {
          zend_printf("No syntax errors detected in %s\n",
                      file_handle.filename);
        } else {
          zend_printf("Errors parsing %s\n", file_handle.filename);
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21767
      case 311: {
        if (interactive && cli_shell_callbacks.cli_shell_run) {
          exit_status = cli_shell_callbacks.cli_shell_run();
        } else {
          php_execute_script(&file_handle);
          exit_status = (executor_globals.exit_status);
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21768
      case 312: {
        if (module_started) {
          php_module_shutdown();
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21769
      case 313: {
        if (open_file_for_scanning(&file_handle) == 0) {
          zend_strip();
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21770
      case 314: {
        if (request_started) {
          php_request_shutdown((void *)0);
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21771
      case 315: {
        if (strcmp(file_handle.filename, "-")) {
          cli_register_file_handles();
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21772
      case 316: {
        if (zend_eval_string_ex(exec_direct, ((void *)0), "Command line code",
                                1) == -1) {
          exit_status = 254;
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21773
      case 317: {
        if (zend_eval_string_ex(exec_run, ((void *)0), "Command line run code",
                                1) == -1) {
          exit_status = 254;
        }
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21774
      case 318: {
        ini_entries_len = sizeof(HARDCODED_INI) - 2;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21775
      case 319: {
        ini_entries_len++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21776
      case 320: {
        input[len] = '\x00';
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21777
      case 321: {
        interactive = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21778
      case 322: {
        len = strlen((arg_free));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21779
      case 323: {
        len = strlen((exec_begin));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21780
      case 324: {
        len = strlen((exec_direct));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21781
      case 325: {
        len = strlen((exec_end));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21782
      case 326: {
        len = strlen((exec_run));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21783
      case 327: {
        len = strlen((input));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21784
      case 328: {
        len = strlen((orig_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21785
      case 329: {
        len = strlen((reflection_what));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21786
      case 330: {
        len = strlen((script_file));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21787
      case 331: {
        len = strlen(input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21788
      case 332: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, "\"", 1);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21789
      case 333: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, "\"\n\000",
               sizeof("\"\n\000"));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21790
      case 334: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((arg_free)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21791
      case 335: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((exec_begin)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21792
      case 336: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((exec_direct)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21793
      case 337: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((exec_end)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21794
      case 338: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((exec_run)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21795
      case 339: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((input)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21796
      case 340: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((orig_optarg)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21797
      case 341: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((reflection_what)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21798
      case 342: {
        memcpy(cli_sapi_module.ini_entries + ini_entries_len, php_optarg,
               ((script_file)-php_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21799
      case 343: {
        memcpy(cli_sapi_module.ini_entries, HARDCODED_INI,
               sizeof(HARDCODED_INI));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21800
      case 344: {
        module_started = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21801
      case 345: {
        param_error = "Source highlighting only works for files.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21802
      case 346: {
        param_error = "Source stripping only works for files.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21803
      case 347: {
        param_error = "You can use -B only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21804
      case 348: {
        param_error = "You can use -E only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21805
      case 349: {
        param_error = "You can use -R or -F only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21806
      case 350: {
        param_error = "You can use -f only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21807
      case 351: {
        param_error = "You can use -r only once.\n";
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21808
      case 352: {
        param_error = (param_error);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21809
      case 353: {
        param_error = param_mode_conflict;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21810
      case 354: {
        php_cli_usage((arg_excp)[0]);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21811
      case 355: {
        php_cli_usage((argv)[0]);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21812
      case 356: {
        php_cli_usage(argv[0]);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21813
      case 357: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted GLOBALS variable overwrite");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21814
      case 358: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite",
                          (arg_free));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21815
      case 359: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite",
                          (exec_begin));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21816
      case 360: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite",
                          (exec_direct));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21817
      case 361: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite",
                          (exec_end));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21818
      case 362: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite",
                          (exec_run));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21819
      case 363: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite", (input));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21820
      case 364: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite",
                          (orig_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21821
      case 365: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite",
                          (reflection_what));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21822
      case 366: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted long input array (%s) overwrite",
                          (script_file));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21823
      case 367: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (arg_free));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21824
      case 368: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (exec_begin));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21825
      case 369: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (exec_direct));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21826
      case 370: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (exec_end));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21827
      case 371: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (exec_run));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21828
      case 372: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (input));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21829
      case 373: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (orig_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21830
      case 374: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (reflection_what));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21831
      case 375: {
        php_error_docref0(((void *)0), (1 << 1L),
                          "Attempted super-global (%s) variable overwrite",
                          (script_file));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21832
      case 376: {
        php_execute_script(&file_handle);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21833
      case 377: {
        php_handle_aborted_connection();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21834
      case 378: {
        php_import_environment_variables((argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21835
      case 379: {
        php_import_environment_variables((argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21836
      case 380: {
        php_module_shutdown();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21837
      case 381: {
        php_optarg = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21838
      case 382: {
        php_optarg = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21839
      case 383: {
        php_optarg = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21840
      case 384: {
        php_optarg = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21841
      case 385: {
        php_optarg = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21842
      case 386: {
        php_optarg = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21843
      case 387: {
        php_optarg = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21844
      case 388: {
        php_optarg = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21845
      case 389: {
        php_optarg = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21846
      case 390: {
        php_optarg = orig_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21847
      case 391: {
        php_optind = orig_optind;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21848
      case 392: {
        php_optind++;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21849
      case 393: {
        php_output_end_all();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21850
      case 394: {
        php_print_info(4294967295U);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21851
      case 395: {
        php_printf("Could not open input file: %s\n", (arg_free));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21852
      case 396: {
        php_printf("Could not open input file: %s\n", (exec_begin));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21853
      case 397: {
        php_printf("Could not open input file: %s\n", (exec_direct));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21854
      case 398: {
        php_printf("Could not open input file: %s\n", (exec_end));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21855
      case 399: {
        php_printf("Could not open input file: %s\n", (exec_run));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21856
      case 400: {
        php_printf("Could not open input file: %s\n", (input));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21857
      case 401: {
        php_printf("Could not open input file: %s\n", (orig_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21858
      case 402: {
        php_printf("Could not open input file: %s\n", (reflection_what));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21859
      case 403: {
        php_printf("Could not open input file: %s\n", (script_file));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21860
      case 404: {
        php_printf("PHP %s (%s) (built: %s %s) %s\nCopyright (c) 1997-2011 The "
                   "PHP Group\n%s",
                   "5.3.99-dev", sapi_module.name, "Mar 24 2021", "17:43:12",
                   "", get_zend_version());
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21861
      case 405: {
        php_printf("[PHP Modules]\n");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21862
      case 406: {
        php_printf("\n");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21863
      case 407: {
        php_printf("\n[Zend Modules]\n");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21864
      case 408: {
        php_register_variable("PATH_TRANSLATED", script_filename, (argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21865
      case 409: {
        php_register_variable("PATH_TRANSLATED", script_filename, (argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21866
      case 410: {
        php_register_variable("PHP_SELF", php_self, (argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21867
      case 411: {
        php_register_variable("PHP_SELF", php_self, (argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21868
      case 412: {
        php_register_variable("SCRIPT_FILENAME", script_filename, (argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21869
      case 413: {
        php_register_variable("SCRIPT_FILENAME", script_filename, (argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21870
      case 414: {
        php_register_variable("SCRIPT_NAME", php_self, (argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21871
      case 415: {
        php_register_variable("SCRIPT_NAME", php_self, (argn));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21872
      case 416: {
        php_request_shutdown((void *)0);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21873
      case 417: {
        php_self = file_handle.filename;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21874
      case 418: {
        print_extensions();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21875
      case 419: {
        print_modules();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21876
      case 420: {
        printf("Interactive mode enabled\n\n");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21877
      case 421: {
        reflection_what = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21878
      case 422: {
        reflection_what = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21879
      case 423: {
        reflection_what = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21880
      case 424: {
        reflection_what = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21881
      case 425: {
        reflection_what = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21882
      case 426: {
        reflection_what = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21883
      case 427: {
        reflection_what = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21884
      case 428: {
        reflection_what = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21885
      case 429: {
        reflection_what = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21886
      case 430: {
        reflection_what = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21887
      case 431: {
        request_started = 1;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21888
      case 432: {
        sapi_shutdown();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21889
      case 433: {
        sapi_startup(&cli_sapi_module);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21890
      case 434: {
        script_file = (arg_excp)[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21891
      case 435: {
        script_file = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21892
      case 436: {
        script_file = (argv)[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21893
      case 437: {
        script_file = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21894
      case 438: {
        script_file = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21895
      case 439: {
        script_file = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21896
      case 440: {
        script_file = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21897
      case 441: {
        script_file = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21898
      case 442: {
        script_file = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21899
      case 443: {
        script_file = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21900
      case 444: {
        script_file = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21901
      case 445: {
        script_file = argv[php_optind];
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21902
      case 446: {
        script_file = php_optarg;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21903
      case 447: {
        script_filename = (arg_free);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21904
      case 448: {
        script_filename = (exec_begin);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21905
      case 449: {
        script_filename = (exec_direct);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21906
      case 450: {
        script_filename = (exec_end);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21907
      case 451: {
        script_filename = (exec_run);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21908
      case 452: {
        script_filename = (input);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21909
      case 453: {
        script_filename = (orig_optarg);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21910
      case 454: {
        script_filename = (reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21911
      case 455: {
        script_filename = (script_file);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21912
      case 456: {
        script_filename = script_file;
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21913
      case 457: {
        signal(13, ((__sighandler_t)1));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21914
      case 458: {
        zend_call_method(((void *)0), reflection_ptr, ((void *)0), "export",
                         sizeof("export") - 1, ((void *)0), 1, (argi),
                         ((void *)0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21915
      case 459: {
        zend_call_method(((void *)0), reflection_ptr, ((void *)0), "export",
                         sizeof("export") - 1, ((void *)0), 1, (argn),
                         ((void *)0));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21916
      case 460: {
        zend_error((1 << 3L), "A non well formed numeric value encountered");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21917
      case 461: {
        zend_ini_deactivate();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21918
      case 462: {
        zend_is_auto_global("_SERVER", sizeof("_SERVER") - 1);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21919
      case 463: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (arg_free) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21920
      case 464: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (exec_begin) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21921
      case 465: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (exec_direct) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21922
      case 466: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (exec_end) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21923
      case 467: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (exec_run) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21924
      case 468: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (input) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21925
      case 469: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (orig_optarg) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21926
      case 470: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (reflection_what) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21927
      case 471: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    (script_file) ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21928
      case 472: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (arg_free) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21929
      case 473: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (exec_begin) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21930
      case 474: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (exec_direct) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21931
      case 475: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (exec_end) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21932
      case 476: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (exec_run) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21933
      case 477: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (input) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21934
      case 478: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (orig_optarg) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21935
      case 479: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (reflection_what) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21936
      case 480: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? (script_file) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21937
      case 481: {
        zend_printf("Additional .ini files parsed:      %s\n",
                    php_ini_scanned_files ? php_ini_scanned_files : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21938
      case 482: {
        zend_printf("Configuration File (php.ini) Path: %s\n",
                    "/usr/local/lib");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21939
      case 483: {
        zend_printf("Errors parsing %s\n", file_handle.filename);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21940
      case 484: {
        zend_printf("Exception: %s\n", (*(argi)).value.str.val);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21941
      case 485: {
        zend_printf("Exception: %s\n", (*(argn)).value.str.val);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21942
      case 486: {
        zend_printf("Extension '%s' not present.\n", (arg_free));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21943
      case 487: {
        zend_printf("Extension '%s' not present.\n", (exec_begin));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21944
      case 488: {
        zend_printf("Extension '%s' not present.\n", (exec_direct));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21945
      case 489: {
        zend_printf("Extension '%s' not present.\n", (exec_end));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21946
      case 490: {
        zend_printf("Extension '%s' not present.\n", (exec_run));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21947
      case 491: {
        zend_printf("Extension '%s' not present.\n", (input));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21948
      case 492: {
        zend_printf("Extension '%s' not present.\n", (orig_optarg));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21949
      case 493: {
        zend_printf("Extension '%s' not present.\n", (reflection_what));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21950
      case 494: {
        zend_printf("Extension '%s' not present.\n", (script_file));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21951
      case 495: {
        zend_printf("Extension '%s' not present.\n", reflection_what);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21952
      case 496: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (arg_free) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21953
      case 497: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (exec_begin) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21954
      case 498: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (exec_direct) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21955
      case 499: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (exec_end) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21956
      case 500: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (exec_run) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21957
      case 501: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (input) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21958
      case 502: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (orig_optarg) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21959
      case 503: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (reflection_what) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21960
      case 504: {
        zend_printf("Loaded Configuration File:         %s\n",
                    (script_file) ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21961
      case 505: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (arg_free) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21962
      case 506: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (exec_begin) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21963
      case 507: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (exec_direct) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21964
      case 508: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (exec_end) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21965
      case 509: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (exec_run) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21966
      case 510: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (input) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21967
      case 511: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (orig_optarg) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21968
      case 512: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (reflection_what) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21969
      case 513: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? (script_file) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21970
      case 514: {
        zend_printf("Loaded Configuration File:         %s\n",
                    php_ini_opened_path ? php_ini_opened_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21971
      case 515: {
        zend_printf("No syntax errors detected in %s\n", file_handle.filename);
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21972
      case 516: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (arg_free) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21973
      case 517: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (exec_begin) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21974
      case 518: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (exec_direct) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21975
      case 519: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (exec_end) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21976
      case 520: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (exec_run) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21977
      case 521: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (input) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21978
      case 522: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (orig_optarg) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21979
      case 523: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (reflection_what) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21980
      case 524: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    (script_file) ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21981
      case 525: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (arg_free) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21982
      case 526: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (exec_begin) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21983
      case 527: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (exec_direct) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21984
      case 528: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (exec_end) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21985
      case 529: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (exec_run) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21986
      case 530: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (input) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21987
      case 531: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (orig_optarg) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21988
      case 532: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (reflection_what) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21989
      case 533: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? (script_file) : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21990
      case 534: {
        zend_printf("Scan for additional .ini files in: %s\n",
                    php_ini_scanned_path ? php_ini_scanned_path : "(none)");
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21991
      case 535: {
        zend_strip();
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21992
      case 536: {
        zval_unset_isref_p((argi));
        cli_register_file_handles();

        break;
      }
#endif
#ifdef COMPILE_21993
      case 537: {
        zval_unset_isref_p((argn));
        cli_register_file_handles();

        break;
      }
#endif
      }

      if (exec_begin &&
          zend_eval_string_ex(exec_begin, NULL, "Command line begin code",
                              1 TSRMLS_CC) == FAILURE) {
        exit_status = 254;
      }
      ALLOC_ZVAL(argi);
      Z_TYPE_P(argi) = IS_LONG;
      Z_LVAL_P(argi) = index;
      INIT_PZVAL(argi);
      zend_hash_update(&EG(symbol_table), "argi", sizeof("argi"), &argi,
                       sizeof(zval *), NULL);
      while (exit_status == SUCCESS &&
             (input = php_stream_gets(s_in_process, NULL, 0)) != NULL) {
        len = strlen(input);
        while (len-- && (input[len] == '\n' || input[len] == '\r')) {
          input[len] = '\0';
        }
        ALLOC_ZVAL(argn);
        Z_TYPE_P(argn) = IS_STRING;
        Z_STRLEN_P(argn) = ++len;
        Z_STRVAL_P(argn) = estrndup(input, len);
        INIT_PZVAL(argn);
        zend_hash_update(&EG(symbol_table), "argn", sizeof("argn"), &argn,
                         sizeof(zval *), NULL);
        Z_LVAL_P(argi) = ++index;
        if (exec_run) {
          if (zend_eval_string_ex(exec_run, NULL, "Command line run code",
                                  1 TSRMLS_CC) == FAILURE) {
            exit_status = 254;
          }
        } else {
          if (script_file) {
            if (cli_seek_file_begin(&file_handle, script_file,
                                    &lineno TSRMLS_CC) != SUCCESS) {
              exit_status = 1;
            } else {
              CG(start_lineno) = lineno;
              php_execute_script(&file_handle TSRMLS_CC);
              exit_status = EG(exit_status);
            }
          }
        }
        efree(input);
      }
      if (exec_end &&
          zend_eval_string_ex(exec_end, NULL, "Command line end code",
                              1 TSRMLS_CC) == FAILURE) {
        exit_status = 254;
      }

      break;
    }
#ifdef HAVE_REFLECTION
    case PHP_MODE_REFLECTION_FUNCTION:
    case PHP_MODE_REFLECTION_CLASS:
    case PHP_MODE_REFLECTION_EXTENSION:
    case PHP_MODE_REFLECTION_ZEND_EXTENSION: {
      zend_class_entry *pce = NULL;
      zval *arg, *ref;
      zend_execute_data execute_data;

      switch (behavior) {
      default:
        break;
      case PHP_MODE_REFLECTION_FUNCTION:
        if (strstr(reflection_what, "::")) {
          pce = reflection_method_ptr;
        } else {
          pce = reflection_function_ptr;
        }
        break;
      case PHP_MODE_REFLECTION_CLASS:
        pce = reflection_class_ptr;
        break;
      case PHP_MODE_REFLECTION_EXTENSION:
        pce = reflection_extension_ptr;
        break;
      case PHP_MODE_REFLECTION_ZEND_EXTENSION:
        pce = reflection_zend_extension_ptr;
        break;
      }

      MAKE_STD_ZVAL(arg);
      ZVAL_STRING(arg, reflection_what, 1);
      ALLOC_ZVAL(ref);
      object_init_ex(ref, pce);
      INIT_PZVAL(ref);

      memset(&execute_data, 0, sizeof(zend_execute_data));
      EG(current_execute_data) = &execute_data;
      EX(function_state).function = pce->constructor;
      zend_call_method_with_1_params(&ref, pce, &pce->constructor,
                                     "__construct", NULL, arg);

      if (EG(exception)) {
        zval *msg = zend_read_property(zend_exception_get_default(TSRMLS_C),
                                       EG(exception), "message",
                                       sizeof("message") - 1, 0 TSRMLS_CC);
        zend_printf("Exception: %s\n", Z_STRVAL_P(msg));
        zval_ptr_dtor(&EG(exception));
        EG(exception) = NULL;
      } else {
        zend_call_method_with_1_params(NULL, reflection_ptr, NULL, "export",
                                       NULL, ref);
      }
      zval_ptr_dtor(&ref);
      zval_ptr_dtor(&arg);

      break;
    }
#endif /* reflection */
    case PHP_MODE_REFLECTION_EXT_INFO: {
      int len = strlen(reflection_what);
      char *lcname = zend_str_tolower_dup(reflection_what, len);
      zend_module_entry *module;

      if (zend_hash_find(&module_registry, lcname, len + 1, (void **)&module) ==
          FAILURE) {
        if (!strcmp(reflection_what, "main")) {
          display_ini_entries(NULL);
        } else {
          zend_printf("Extension '%s' not present.\n", reflection_what);
          exit_status = 1;
        }
      } else {
        php_info_print_module(module TSRMLS_CC);
      }

      efree(lcname);
      break;
    }
    case PHP_MODE_SHOW_INI_CONFIG: {
      zend_printf("Configuration File (php.ini) Path: %s\n",
                  PHP_CONFIG_FILE_PATH);
      zend_printf("Loaded Configuration File:         %s\n",
                  php_ini_opened_path ? php_ini_opened_path : "(none)");
      zend_printf("Scan for additional .ini files in: %s\n",
                  php_ini_scanned_path ? php_ini_scanned_path : "(none)");
      zend_printf("Additional .ini files parsed:      %s\n",
                  php_ini_scanned_files ? php_ini_scanned_files : "(none)");
      break;
    }
    }
  }
  zend_end_try();

out:
  if (request_started) {
    php_request_shutdown((void *)0);
  }
  if (exit_status == 0) {
    exit_status = EG(exit_status);
  }
out_err:
  if (cli_sapi_module.php_ini_path_override) {
    free(cli_sapi_module.php_ini_path_override);
  }
  if (cli_sapi_module.ini_entries) {
    free(cli_sapi_module.ini_entries);
  }

  if (module_started) {
    php_module_shutdown(TSRMLS_C);
  }
  sapi_shutdown();
#ifdef ZTS
  tsrm_shutdown();
#endif

  exit(exit_status);

err:
  sapi_deactivate(TSRMLS_C);
  zend_ini_deactivate(TSRMLS_C);
  exit_status = 1;
  goto out_err;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
