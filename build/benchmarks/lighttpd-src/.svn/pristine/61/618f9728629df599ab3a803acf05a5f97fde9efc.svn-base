#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#endif

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <assert.h>
#include <locale.h>

#include <stdio.h>

#include "settings.h"
#include "server.h"
#include "buffer.h"
#include "network.h"
#include "network_backends.h"
#include "log.h"
#include "keyvalue.h"
#include "response.h"
#include "request.h"
#include "chunk.h"
#include "fdevent.h"
#include "connections.h"
#include "stat_cache.h"
#include "plugin.h"
#include "joblist.h"
#include "status_counter.h"

/**
 * stack-size of the aio-threads
 *
 * the default is 8Mbyte which is a bit to much. Reducing it to 64k seems to be fine
 * If you experience random segfaults, increase it.
 */

#define LI_THREAD_STACK_SIZE (64 * 1024)


#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
#ifdef _WIN32
#include "xgetopt.h"
#endif
#endif

#include "valgrind/valgrind.h"

#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#ifdef HAVE_PWD_H
#include <grp.h>
#include <pwd.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#endif

#ifdef USE_OPENSSL
#include <openssl/err.h>
#endif

#ifndef __sgi
/* IRIX doesn't like the alarm based time() optimization */
/* #define USE_ALARM */
#endif

#ifdef _WIN32
#undef HAVE_SIGNAL
#endif

#include "sys-files.h"
#include "sys-process.h"
#include "sys-socket.h"

#ifdef HAVE_GETUID
# ifndef HAVE_ISSETUGID

static int l_issetugid() {
	return (geteuid() != getuid() || getegid() != getgid());
}

#  define issetugid l_issetugid
# endif
#endif

static volatile sig_atomic_t srv_shutdown = 0;
static volatile sig_atomic_t graceful_shutdown = 0;
static volatile sig_atomic_t graceful_restart = 0;
static volatile sig_atomic_t handle_sig_alarm = 1;
static volatile sig_atomic_t handle_sig_hup = 0;
static volatile siginfo_t last_sigterm_info;
static volatile siginfo_t last_sighup_info;

#if defined(HAVE_SIGACTION) && defined(SA_SIGINFO)
static void sigaction_handler(int sig, siginfo_t *si, void *context) {
	static siginfo_t empty_siginfo;
	UNUSED(context);

	if (!si) si = &empty_siginfo;

	switch (sig) {
	case SIGTERM: 
		srv_shutdown = 1; 
		memcpy((siginfo_t*) &last_sigterm_info, si, sizeof(*si));
		break;
	case SIGINT:
		if (graceful_shutdown) {
			srv_shutdown = 1;
		} else {
			graceful_shutdown = 1;
		}

		memcpy((siginfo_t*) &last_sigterm_info, si, sizeof(*si));

		break;
	case SIGALRM: 
		handle_sig_alarm = 1; 
		break;
	case SIGHUP: 
		handle_sig_hup = 1; 
		memcpy((siginfo_t*) &last_sighup_info, si, sizeof(*si));
		break;
	case SIGCHLD: 
		break;
	}
}
#elif defined(HAVE_SIGNAL) || defined(HAVE_SIGACTION)
static void signal_handler(int sig) {
	switch (sig) {
	case SIGTERM: srv_shutdown = 1; break;
	case SIGINT:
	     if (graceful_shutdown) srv_shutdown = 1;
	     else graceful_shutdown = 1;

	     break;
	case SIGALRM: handle_sig_alarm = 1; break;
	case SIGHUP:  handle_sig_hup = 1; break;
	case SIGCHLD:  break;
	}
}
#endif

#ifdef HAVE_FORK
static void daemonize(void) {
#ifdef SIGTTOU
	signal(SIGTTOU, SIG_IGN);
#endif
#ifdef SIGTTIN
	signal(SIGTTIN, SIG_IGN);
#endif
#ifdef SIGTSTP
	signal(SIGTSTP, SIG_IGN);
#endif
	if (0 != fork()) exit(0);

	if (-1 == setsid()) exit(0);

	signal(SIGHUP, SIG_IGN);

	if (0 != fork()) exit(0);

	if (0 != chdir("/")) exit(0);
}
#endif

static server *server_init(void) {
	int i;
	FILE *frandom = NULL;

	server *srv = calloc(1, sizeof(*srv));
	assert(srv);

	srv->max_fds = 1024;
#define CLEAN(x) \
	srv->x = buffer_init();

	CLEAN(response_header);
	CLEAN(parse_full_path);
	CLEAN(ts_debug_str);
	CLEAN(ts_date_str);
	CLEAN(response_range);
	CLEAN(tmp_buf);
	srv->empty_string = buffer_init_string("");
	CLEAN(cond_check_buf);

	CLEAN(srvconf.errorlog_file);
	CLEAN(srvconf.breakagelog_file);
	CLEAN(srvconf.groupname);
	CLEAN(srvconf.username);
	CLEAN(srvconf.changeroot);
	CLEAN(srvconf.bindhost);
	CLEAN(srvconf.event_handler);
	CLEAN(srvconf.pid_file);

	CLEAN(tmp_chunk_len);
#undef CLEAN

#define CLEAN(x) \
	srv->x = array_init();

	CLEAN(config_context);
	CLEAN(config_touched);
#undef CLEAN

	for (i = 0; i < FILE_CACHE_MAX; i++) {
		srv->mtime_cache[i].mtime = (time_t)-1;
		srv->mtime_cache[i].str = buffer_init();
	}

	if ((NULL != (frandom = fopen("/dev/urandom", "rb")) || NULL != (frandom = fopen("/dev/random", "rb")))
	            && 1 == fread(srv->entropy, sizeof(srv->entropy), 1, frandom)) {
		srand(*(unsigned int*)srv->entropy);
		srv->is_real_entropy = 1;
	} else {
		unsigned int j;
		srand(time(NULL) ^ getpid());
		srv->is_real_entropy = 0;
		for (j = 0; j < sizeof(srv->entropy); j++)
			srv->entropy[j] = rand();
	}
	if (frandom) fclose(frandom);

	srv->cur_ts = time(NULL);
	srv->startup_ts = srv->cur_ts;

	srv->conns = calloc(1, sizeof(*srv->conns));
	assert(srv->conns);

	srv->joblist = calloc(1, sizeof(*srv->joblist));
	assert(srv->joblist);

	srv->joblist_prev = calloc(1, sizeof(*srv->joblist));
	assert(srv->joblist_prev);

	srv->fdwaitqueue = calloc(1, sizeof(*srv->fdwaitqueue));
	assert(srv->fdwaitqueue);

	srv->srvconf.modules = array_init();
	srv->srvconf.modules_dir = buffer_init_string(LIBRARY_DIR);
	srv->srvconf.network_backend = buffer_init();
	srv->srvconf.upload_tempdirs = array_init();

	srv->split_vals = array_init();

#ifdef USE_LINUX_AIO_SENDFILE
	srv->linux_io_ctx = NULL;
	/**
	 * we can't call io_setup before the fork() in daemonize()
	 */
#endif

	return srv;
}

static void server_free(server *srv) {
	size_t i;

	for (i = 0; i < FILE_CACHE_MAX; i++) {
		buffer_free(srv->mtime_cache[i].str);
	}


#ifdef USE_LINUX_AIO_SENDFILE
	if (srv->linux_io_ctx) {
		io_destroy(srv->linux_io_ctx);
	}
#endif

#define CLEAN(x) \
	buffer_free(srv->x);

	CLEAN(response_header);
	CLEAN(parse_full_path);
	CLEAN(ts_debug_str);
	CLEAN(ts_date_str);
	CLEAN(response_range);
	CLEAN(tmp_buf);
	CLEAN(empty_string);
	CLEAN(cond_check_buf);

	CLEAN(srvconf.errorlog_file);
	CLEAN(srvconf.breakagelog_file);
	CLEAN(srvconf.groupname);
	CLEAN(srvconf.username);
	CLEAN(srvconf.changeroot);
	CLEAN(srvconf.bindhost);
	CLEAN(srvconf.event_handler);
	CLEAN(srvconf.pid_file);
	CLEAN(srvconf.modules_dir);
	CLEAN(srvconf.network_backend);

	CLEAN(tmp_chunk_len);
#undef CLEAN

#ifdef USE_GTHREAD
	fdevent_unregister(srv->ev, srv->wakeup_iosocket);
	iosocket_free(srv->wakeup_iosocket);
#endif

#if 0
	fdevent_unregister(srv->ev, srv->fd);
#endif
	fdevent_free(srv->ev);

	free(srv->conns);

	if (srv->config_storage) {
		for (i = 0; i < srv->config_context->used; i++) {
			specific_config *s = srv->config_storage[i];

			if (!s) continue;

			buffer_free(s->document_root);
			buffer_free(s->server_name);
			buffer_free(s->server_tag);
			buffer_free(s->ssl_pemfile);
			buffer_free(s->ssl_ca_file);
			buffer_free(s->error_handler);
			buffer_free(s->errorfile_prefix);
			array_free(s->mimetypes);
			buffer_free(s->ssl_cipher_list);
			buffer_free(s->ssl_verifyclient_username);
#ifdef USE_OPENSSL
			SSL_CTX_free(s->ssl_ctx);
#endif

			free(s);
		}
		free(srv->config_storage);
		srv->config_storage = NULL;
	}

#define CLEAN(x) \
	array_free(srv->x);

	CLEAN(config_context);
	CLEAN(config_touched);
	CLEAN(srvconf.upload_tempdirs);
#undef CLEAN

	joblist_free(srv, srv->joblist);
	joblist_free(srv, srv->joblist_prev);
	fdwaitqueue_free(srv, srv->fdwaitqueue);

	if (srv->stat_cache) {
		stat_cache_free(srv->stat_cache);
	}

	array_free(srv->srvconf.modules);
	array_free(srv->split_vals);
#ifdef USE_OPENSSL
	if (srv->ssl_is_init) {
		CRYPTO_cleanup_all_ex_data();
		ERR_free_strings();
		ERR_remove_state(0);
		EVP_cleanup();
	}
#endif
	free(srv);
}

static void show_version (void) {
#ifdef USE_OPENSSL
# define TEXT_SSL " (ssl)"
#else
# define TEXT_SSL
#endif
	char *b = PACKAGE_NAME "-" PACKAGE_VERSION TEXT_SSL \
" - a light and fast webserver\n" \
"Build-Date: " __DATE__ " " __TIME__ "\n";
;
#undef TEXT_SSL
	if (-1 == write(STDOUT_FILENO, b, strlen(b))) {
		/* what to do if this happens ? */

		exit(-1);
	}
}

static void show_features (void) {
	const fdevent_handler_info_t *handler;
	const network_backend_info_t *backend;

	const char *features = 
#ifdef HAVE_IPV6
      "\t+ IPv6 support\n"
#else
      "\t- IPv6 support\n"
#endif
#if defined HAVE_ZLIB_H && defined HAVE_LIBZ
      "\t+ zlib support\n"
#else
      "\t- zlib support\n"
#endif
#if defined HAVE_BZLIB_H && defined HAVE_LIBBZ2
      "\t+ bzip2 support\n"
#else
      "\t- bzip2 support\n"
#endif
#ifdef HAVE_LIBCRYPT
      "\t+ crypt support\n"
#else
      "\t- crypt support\n"
#endif
#ifdef USE_OPENSSL
      "\t+ SSL Support\n"
#else
      "\t- SSL Support\n"
#endif
#ifdef HAVE_LIBPCRE
      "\t+ PCRE support\n"
#else
      "\t- PCRE support\n"
#endif
#ifdef HAVE_MYSQL
      "\t+ MySQL support\n"
#else
      "\t- MySQL support\n"
#endif
#if defined(HAVE_LDAP_H) && defined(HAVE_LBER_H) && defined(HAVE_LIBLDAP) && defined(HAVE_LIBLBER)
      "\t+ LDAP support\n"
#else
      "\t- LDAP support\n"
#endif
#ifdef HAVE_MEMCACHE_H
      "\t+ memcached support\n"
#else
      "\t- memcached support\n"
#endif
#ifdef HAVE_FAM_H
      "\t+ FAM support\n"
#else
      "\t- FAM support\n"
#endif
#ifdef HAVE_LUA_H
      "\t+ LUA support\n"
#else
      "\t- LUA support\n"
#endif
#ifdef HAVE_LIBXML_H
      "\t+ xml support\n"
#else
      "\t- xml support\n"
#endif
#ifdef HAVE_SQLITE3_H
      "\t+ SQLite support\n"
#else
      "\t- SQLite support\n"
#endif
#ifdef HAVE_GDBM_H
      "\t+ GDBM support\n"
#else
      "\t- GDBM support\n"
#endif
      ;

	show_version();

	printf("\nEvent Handlers:\n\n");
	for (handler = fdevent_get_handlers(); handler->name; handler++) {
		printf("\t%c %s", handler->init ? '+' : '-', handler->name);
		if (handler->description) {
			printf(": %s\n", handler->description);
		}
		else {
			printf("\n");
		}
	}

	printf("\nNetwork Backends:\n\n");
	for (backend = network_get_backends(); backend->name; backend++) {
		printf("\t%c %s", backend->write_handler ? '+' : '-', backend->name);
		if (backend->description) {
			printf(": %s\n", backend->description);
		}
		else {
			printf("\n");
		}
	}

#ifdef USE_MMAP
	printf("\t+ (mmap) support\n");
#else
	printf("\t- (mmap) support\n");
#endif
	printf("\nFeatures:\n\n%s", features);
}

static void show_help (void) {
#ifdef USE_OPENSSL
# define TEXT_SSL " (ssl)"
#else
# define TEXT_SSL
#endif
	char *b = PACKAGE_NAME "-" PACKAGE_VERSION TEXT_SSL " ("__DATE__ " " __TIME__ ")" \
" - a light and fast webserver\n" \
"usage:\n" \
" -f <name>  filename of the config-file\n" \
" -m <name>  module directory (default: "LIBRARY_DIR")\n" \
" -p         print the parsed config-file in internal form, and exit\n" \
" -t         test the config-file, and exit\n" \
" -D         don't go to background (default: go to background)\n" \
" -I         go to background on SIGINT (useful with -D)\n" \
"            has no effect when using kqueue or /dev/poll\n" \
" -v         show version\n" \
" -V         show compile-time features\n" \
" -h         show this help\n" \
"\n"
;
#undef TEXT_SSL
#undef TEXT_IPV6
	if (-1 == write(STDOUT_FILENO, b, strlen(b))) {
		exit(-1);
	}
}

/**
 * call this function whenever you get a EMFILE or ENFILE as return-value
 *
 * after each socket(), accept(), connect() or open() call
 *
 */
int server_out_of_fds(server *srv, connection *con) {
	/* we get NULL of accept() ran out of FDs */

	if (con) {
		fdwaitqueue_append(srv, con);
	}

	return 0;
}

static int lighty_mainloop(server *srv) {
	fdevent_revents *revents = fdevent_revents_init();
	int poll_errno;
	size_t conns_user_at_sockets_disabled = 0;

	/* the getevents and the poll() have to run in parallel
	 * as soon as one has data, it has to interrupt the otherone */

	/* main-loop */
	while (!srv_shutdown) {
		int n;
		size_t ndx;
		time_t min_ts;

		if (handle_sig_hup) {
			handler_t r;

			/* reset notification */
			handle_sig_hup = 0;

#if 0
      			pid_t pid;

			/* send the old process into a graceful-shutdown and start a
			 * new process right away
			 *
			 * BUGS:
			 * - if webserver is running on port < 1024 (e.g. 80, 433)
			 *   we don't have the permissions to bind to that port anymore
			 *
			 *
			 *  */
			if (0 == (pid = fork())) {
				execve(argv[0], argv, envp);

				exit(-1);
			} else if (pid == -1) {

			} else {
				/* parent */

				graceful_shutdown = 1; /* shutdown without killing running connections */
				graceful_restart = 1;  /* don't delete pid file */
			}
#else
			/* cycle logfiles */

			switch(r = plugins_call_handle_sighup(srv)) {
			case HANDLER_GO_ON:
				break;
			default:
				log_error_write(srv, __FILE__, __LINE__, "sd", "sighup-handler return with an error", r);
				break;
			}

			if (-1 == log_error_cycle()) {
				log_error_write(srv, __FILE__, __LINE__, "s", "cycling errorlog failed, dying");

				return -1;
			} else {
				TRACE("logfiles cycled by UID=%d, PID=%d", 
						last_sighup_info.si_uid, 
						last_sighup_info.si_pid);
			}
#endif
		}

		if (handle_sig_alarm) {
			/* a new second */

#ifdef USE_ALARM
			/* reset notification */
			handle_sig_alarm = 0;
#endif

			/* get current time */
			min_ts = time(NULL);

			if (min_ts != srv->cur_ts) {
				int cs = 0;
				connections *conns = srv->conns;
				handler_t r;

				switch(r = plugins_call_handle_trigger(srv)) {
				case HANDLER_GO_ON:
					break;
				case HANDLER_ERROR:
					log_error_write(srv, __FILE__, __LINE__, "s", "one of the triggers failed");
					break;
				default:
					log_error_write(srv, __FILE__, __LINE__, "d", r);
					break;
				}

				/* trigger waitpid */
				srv->cur_ts = min_ts;

				/* cleanup stat-cache */
				stat_cache_trigger_cleanup(srv);
				/**
				 * check all connections for timeouts
				 *
				 */
				for (ndx = 0; ndx < conns->used; ndx++) {
					int changed = 0;
					connection *con;
					int t_diff;

					con = conns->ptr[ndx];

					switch (con->state) {
					case CON_STATE_READ_REQUEST_HEADER:
					case CON_STATE_READ_REQUEST_CONTENT:
						if (con->recv->is_closed) {
							if (srv->cur_ts - con->read_idle_ts > con->conf.max_connection_idle) {
								/* time - out */
#if 0
								TRACE("(connection process timeout) [%s]", SAFE_BUF_STR(con->dst_addr_buf));
#endif
								connection_set_state(srv, con, CON_STATE_ERROR);
								changed = 1;
							}
						}

						if (con->request_count == 1) {
							if (srv->cur_ts - con->read_idle_ts > con->conf.max_read_idle) {
								/* time - out */
#if 0
								TRACE("(initial read timeout) [%s]", SAFE_BUF_STR(con->dst_addr_buf));
#endif
								connection_set_state(srv, con, CON_STATE_ERROR);
								changed = 1;
							}
						} else {
							if (srv->cur_ts - con->read_idle_ts > con->keep_alive_idle) {
								/* time - out */
#if 0
								TRACE("(keep-alive read timeout) [%s]", SAFE_BUF_STR(con->dst_addr_buf));
#endif
								connection_set_state(srv, con, CON_STATE_ERROR);
								changed = 1;
							}
						}
						break;
					case CON_STATE_WRITE_RESPONSE_HEADER:
					case CON_STATE_WRITE_RESPONSE_CONTENT:


						if (con->write_request_ts != 0 &&
						    srv->cur_ts - con->write_request_ts > con->conf.max_write_idle) {
							/* time - out */
							if (con->conf.log_timeouts) {
								log_error_write(srv, __FILE__, __LINE__, "sbsosds",
									"NOTE: a request for",
									con->request.uri,
									"timed out after writing",
									con->bytes_written,
									"bytes. We waited",
									(int)con->conf.max_write_idle,
									"seconds. If this a problem increase server.max-write-idle");
							}
							connection_set_state(srv, con, CON_STATE_ERROR);
							changed = 1;
						}
						break;
					default:
						/* the other ones are uninteresting */
						break;
					}
					/* we don't like div by zero */
					if (0 == (t_diff = srv->cur_ts - con->connection_start)) t_diff = 1;

					if (con->traffic_limit_reached &&
					    (con->conf.kbytes_per_second == 0 ||
					     ((con->bytes_written / t_diff) < con->conf.kbytes_per_second * 1024))) {
						/* enable connection again */
						con->traffic_limit_reached = 0;

						changed = 1;
					}

					if (changed) {
						connection_state_machine(srv, con);
					}
					con->bytes_written_cur_second = 0;
					*(con->conf.global_bytes_per_second_cnt_ptr) = 0;

#if 0
					if (cs == 0) {
						fprintf(stderr, "connection-state: ");
						cs = 1;
					}

					fprintf(stderr, "c[%d,%d]: %s ",
						con->fd,
						con->fcgi.fd,
						connection_get_state(con->state));
#endif
				}

				if (cs == 1) fprintf(stderr, "\n");
			}
		}

		if (srv->sockets_disabled) {
			/* our server sockets are disabled, why ? */

			if ((srv->fdwaitqueue->used == 0) &&
			    (srv->conns->used <= srv->max_conns * 9 / 10) &&
			    (0 == graceful_shutdown)) {
				size_t i;

				for (i = 0; i < srv->srv_sockets.used; i++) {
					server_socket *srv_socket = srv->srv_sockets.ptr[i];
					fdevent_event_add(srv->ev, srv_socket->sock, FDEVENT_IN);
				}

				TRACE("[note] sockets enabled again%s", "");

				srv->sockets_disabled = 0;
			}
		} else {
			if ((srv->fdwaitqueue->used) || /* looks like some cons are waiting for FDs*/
			    (srv->conns->used >= srv->max_conns) || /* out of connections */
			    (graceful_shutdown)) { /* graceful_shutdown */
				size_t i;

				/* disable server-fds */

				for (i = 0; i < srv->srv_sockets.used; i++) {
					server_socket *srv_socket = srv->srv_sockets.ptr[i];

					fdevent_event_del(srv->ev, srv_socket->sock);

					if (graceful_shutdown) {
						/* we don't want this socket anymore,
						 *
						 * closing it right away will make it possible for
						 * the next lighttpd to take over (graceful restart)
						 *  */

						fdevent_unregister(srv->ev, srv_socket->sock);
						closesocket(srv_socket->sock->fd);
						srv_socket->sock->fd = -1;

#ifdef HAVE_FORK
						/* FreeBSD kqueue could possibly work with rfork(RFFDG)
						* while Solaris /dev/poll would require re-registering
						* all fd */
						if (srv->srvconf.daemonize_on_shutdown &&
							srv->event_handler != FDEVENT_HANDLER_FREEBSD_KQUEUE &&
							srv->event_handler != FDEVENT_HANDLER_SOLARIS_DEVPOLL) {
							daemonize();
						}
#endif

						/* network_close() will cleanup after us */
					}
				}

				if (graceful_shutdown) {
					TRACE("[note] graceful shutdown started by UID=%d, PID=%d", last_sigterm_info.si_uid, last_sigterm_info.si_pid);
				} else if (srv->fdwaitqueue->used) {
					TRACE("[note] out of FDs, server-socket get disabled for a while, we have %zu connections open and they are waiting for %zu FDs",
					    srv->conns->used, srv->fdwaitqueue->used);
				} else if (srv->conns->used >= srv->max_conns) {
					TRACE("[note] we reached our connection limit of %zu connections. Disabling server-sockets for a while", srv->max_conns);
				}

				srv->sockets_disabled = 1;

				/* we count the number of free fds indirectly.
				 *
				 * instead of checking the fds we only check the connection handles we free'd since
				 * the server-sockets got disabled
				 * */
				conns_user_at_sockets_disabled = srv->conns->used;
			}
		}

		if (graceful_shutdown && srv->conns->used == 0) {
			/* we are in graceful shutdown phase and all connections are closed
			 * we are ready to terminate without harming anyone */
			srv_shutdown = 1;
			continue;
		}

		/* we still have some fds to share */
		if (!srv_shutdown && srv->fdwaitqueue->used) {
			/* ok, we are back to the problem of 'how many fds do we have available ?' */
			connection *con;
			int fd;
			int avail_fds = conns_user_at_sockets_disabled - srv->conns->used;

			if (-1 == (fd = open("/dev/null", O_RDONLY))) {
				switch (errno) {
				case EMFILE:
					avail_fds = 0;

					break;
				default:
					break;
				}
			} else {
				close(fd);

				/* we have at least one FD as we just checked */
				if (!avail_fds) avail_fds++;
			}


			TRACE("conns used: %zu, fd-waitqueue has %zu entries, fds to share: %d", srv->conns->used, srv->fdwaitqueue->used, avail_fds);

			while (avail_fds-- && NULL != (con = fdwaitqueue_unshift(srv, srv->fdwaitqueue))) {
				connection_state_machine(srv, con);
			}
		}
		n = fdevent_poll(srv->ev, 1000);
		poll_errno = errno;
#ifdef USE_GTHREAD
		g_atomic_int_set(&srv->did_wakeup, 0);
#endif

		if (n > 0) {
			/* n is the number of events */
			size_t i;
			fdevent_get_revents(srv->ev, n, revents);

			/* handle client connections first
			 *
			 * this is a bit of a hack, but we have to make sure than we handle
			 * close-events before the connection is reused for a keep-alive
			 * request
			 *
			 * this is mostly an issue for mod_proxy_core, but you never know
			 *
			 */

			for (i = 0; i < revents->used; i++) {
				fdevent_revent *revent = revents->ptr[i];
				handler_t r;

				/* skip server-fds */
				if (revent->handler == network_server_handle_fdevent) continue;

				switch (r = (*(revent->handler))(srv, revent->context, revent->revents)) {
				case HANDLER_WAIT_FOR_FD:
					server_out_of_fds(srv, NULL);
				case HANDLER_FINISHED:
				case HANDLER_GO_ON:
				case HANDLER_WAIT_FOR_EVENT:
					break;
				case HANDLER_ERROR:
					/* should never happen */
					SEGFAULT("got HANDLER_ERROR from a plugin: %s", "dieing");
					break;
				default:
					ERROR("got handler_t(%d) from a plugin: ignored", r);
					break;
				}
			}

			for (i = 0; i < revents->used; i++) {
				fdevent_revent *revent = revents->ptr[i];
				handler_t r;

				/* server fds only */
				if (revent->handler != network_server_handle_fdevent) continue;

				switch (r = (*(revent->handler))(srv, revent->context, revent->revents)) {
				case HANDLER_WAIT_FOR_FD:
					server_out_of_fds(srv, NULL);
				case HANDLER_FINISHED:
				case HANDLER_GO_ON:
				case HANDLER_WAIT_FOR_EVENT:
					break;
				case HANDLER_ERROR:
					/* should never happen */
					SEGFAULT("got HANDLER_ERROR from a plugin: %s", "dieing");
					break;
				default:
					ERROR("got handler_t(%d) from a plugin: ignored", r);
					break;
				}
			}

		} else if (n < 0 && poll_errno != EINTR) {
			ERROR("fdevent_poll failed: %s", strerror(poll_errno));
		}

		/*
		 * Note: Two joblist's are needed so a connection can be added back into the joblist
		 * without getting stuck inside the for loop.
		 */
#ifdef USE_GTHREAD
		{
			connection *con;
			while (NULL != (con = g_async_queue_try_pop(srv->joblist_queue))) {
				joblist_append(srv, con);
			}
		}
#endif
		if(srv->joblist->used > 0) {
			connections *joblist = srv->joblist;
			/* switch joblist queues. */
			srv->joblist = srv->joblist_prev;
			srv->joblist_prev = joblist;
		}
		for (ndx = 0; ndx < srv->joblist_prev->used; ndx++) {
			connection *con = srv->joblist_prev->ptr[ndx];
			handler_t r;

			con->in_joblist = 0;
			connection_state_machine(srv, con);

			switch(r = plugins_call_handle_joblist(srv, con)) {
			case HANDLER_FINISHED:
			case HANDLER_GO_ON:
				break;
			default:
				ERROR("got handler_t(%d) from a plugin: ignored", r);
				break;
			}
		}

		srv->joblist_prev->used = 0;
	}

	fdevent_revents_free(revents);

	return 0;
}

#ifdef USE_GTHREAD
static handler_t wakeup_handle_fdevent(void *s, void *context, int revent) {
	server     *srv = (server *)s;
	connection *con = context;
	char buf[16];
	UNUSED(con);
	UNUSED(revent);

	(void) read(srv->wakeup_iosocket->fd, buf, sizeof(buf));
	return HANDLER_GO_ON; 
}
#endif

int main (int argc, char **argv, char **envp) {
	server *srv = NULL;
	int print_config = 0;
	int test_config = 0;
	int i_am_root;
	int o;
	int num_childs = 0;
	int pid_fd = -1, fd;
	size_t i;
#ifdef USE_GTHREAD
	GThread **stat_cache_threads;
	GThread **aio_write_threads = NULL;
#ifdef USE_LINUX_AIO_SENDFILE
	GThread *linux_aio_read_thread_id = NULL;
#endif
	GError *gerr = NULL;
#endif

#ifdef HAVE_SIGACTION
	struct sigaction act;
#endif
#ifdef HAVE_GETRLIMIT
	struct rlimit rlim;
#endif

#ifdef USE_ALARM
	struct itimerval interval;

	interval.it_interval.tv_sec = 1;
	interval.it_interval.tv_usec = 0;
	interval.it_value.tv_sec = 1;
	interval.it_value.tv_usec = 0;
#endif

	UNUSED(envp);

	log_init();
	status_counter_init();

	/* for nice %b handling in strfime() */
	setlocale(LC_TIME, "C");

	if (NULL == (srv = server_init())) {
		fprintf(stderr, "did this really happen?\n");
		return -1;
	}

	/* init structs done */

	srv->srvconf.port = 0;
#ifdef HAVE_GETUID
	i_am_root = (getuid() == 0);
#else
	i_am_root = 0;
#endif
	srv->srvconf.dont_daemonize = 0;
	srv->srvconf.daemonize_on_shutdown = 0;
	srv->srvconf.max_stat_threads = 4;
	srv->srvconf.max_read_threads = 8;

	while(-1 != (o = getopt(argc, argv, "f:m:hvVDIpt"))) {
		switch(o) {
		case 'f':
			if (config_read(srv, optarg)) {
				server_free(srv);
				return -1;
			}
			break;
		case 'm':
			buffer_copy_string(srv->srvconf.modules_dir, optarg);
			break;
		case 'p': print_config = 1; break;
		case 't': test_config = 1; break;
		case 'D': srv->srvconf.dont_daemonize = 1; break;
		case 'I': srv->srvconf.daemonize_on_shutdown = 1; break;
		case 'v': show_version(); return 0;
		case 'V': show_features(); return 0;
		case 'h': show_help(); return 0;
		default:
			show_help();
			server_free(srv);
			return -1;
		}
	}

	if (!srv->config_storage) {
		log_error_write(srv, __FILE__, __LINE__, "s",
				"No configuration available. Try using -f option.");

		server_free(srv);
		return -1;
	}

	if (print_config) {
		data_unset *dc = srv->config_context->data[0];
		if (dc) {
			dc->print(dc, 0);
			fprintf(stdout, "\n");
		} else {
			/* shouldn't happend */
			fprintf(stdout, "global config not found\n");
		}
	}

	if (test_config) {
		printf("Syntax OK\n");
	}

	if (test_config || print_config) {
		server_free(srv);
		return 0;
	}

	/* close stdin and stdout, as they are not needed */
	/* move stdin to /dev/null */
	if (-1 != (fd = open("/dev/null", O_RDONLY))) {
		close(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}

	/* move stdout to /dev/null */
	if (-1 != (fd = open("/dev/null", O_WRONLY))) {
		close(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}

	if (0 != config_set_defaults(srv)) {
		log_error_write(srv, __FILE__, __LINE__, "s",
				"setting default values failed");
		server_free(srv);
		return -1;
	}

	/* UID handling */
#ifdef HAVE_GETUID
	if (!i_am_root && issetugid()) {
		/* we are setuid-root */

		log_error_write(srv, __FILE__, __LINE__, "s",
				"Are you nuts ? Don't apply a SUID bit to this binary");

		server_free(srv);
		return -1;
	}
#endif

	/* check document-root */
	if (srv->config_storage[0]->document_root->used <= 1) {
		log_error_write(srv, __FILE__, __LINE__, "s",
				"document-root is not set\n");

		server_free(srv);

		return -1;
	}

	if (plugins_load(srv)) {
		log_error_write(srv, __FILE__, __LINE__, "s",
				"loading plugins finally failed");

		plugins_free(srv);
		server_free(srv);

		return -1;
	}

#ifndef _WIN32
	/* open pid file BEFORE chroot */
	if (srv->srvconf.pid_file->used) {
		if (-1 == (pid_fd = open(srv->srvconf.pid_file->ptr, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))) {
			struct stat st;
			if (errno != EEXIST) {
				log_error_write(srv, __FILE__, __LINE__, "sbs",
					"opening pid-file failed:", srv->srvconf.pid_file, strerror(errno));
				return -1;
			}

			if (0 != stat(srv->srvconf.pid_file->ptr, &st)) {
				log_error_write(srv, __FILE__, __LINE__, "sbs",
						"stating existing pid-file failed:", srv->srvconf.pid_file, strerror(errno));
			}

			if (!S_ISREG(st.st_mode)) {
				log_error_write(srv, __FILE__, __LINE__, "sb",
						"pid-file exists and isn't regular file:", srv->srvconf.pid_file);
				return -1;
			}

			if (-1 == (pid_fd = open(srv->srvconf.pid_file->ptr, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))) {
				log_error_write(srv, __FILE__, __LINE__, "sbs",
						"opening pid-file failed:", srv->srvconf.pid_file, strerror(errno));
				return -1;
			}
		}
	}
#endif
	if (srv->event_handler == FDEVENT_HANDLER_SELECT) {
		/* select limits itself
		 *
		 * as it is a hard limit and will lead to a segfault we add some safety
		 * */
        fprintf(stderr, "%s.%d: max parallel connections: %d\r\n", __FILE__, __LINE__, FD_SETSIZE);
		srv->max_fds = FD_SETSIZE - 4;
	} else {
		srv->max_fds = 4096;
	}

	if (i_am_root) {
		struct group *grp = NULL;
		struct passwd *pwd = NULL;
		int use_rlimit = 1;

		/* valgrind only supports 1024 fds */
		if (RUNNING_ON_VALGRIND) use_rlimit = 0;

#ifdef HAVE_GETRLIMIT
		if (0 != getrlimit(RLIMIT_NOFILE, &rlim)) {
			log_error_write(srv, __FILE__, __LINE__,
					"ss", "couldn't get 'max filedescriptors'",
					strerror(errno));
			return -1;
		}

		if (use_rlimit && srv->srvconf.max_fds) {
			/* set rlimits */

			rlim.rlim_cur = srv->srvconf.max_fds;
			rlim.rlim_max = srv->srvconf.max_fds;

			if (0 != setrlimit(RLIMIT_NOFILE, &rlim)) {
				log_error_write(srv, __FILE__, __LINE__,
						"ss", "couldn't set 'max filedescriptors'",
						strerror(errno));
				return -1;
			}
		}

		if (srv->event_handler == FDEVENT_HANDLER_SELECT) {
			srv->max_fds = rlim.rlim_cur < ((int)FD_SETSIZE) - 200 ? rlim.rlim_cur : FD_SETSIZE - 200;
		} else {
			srv->max_fds = rlim.rlim_cur;
		}

		/* set core file rlimit, if enable_cores is set */
		if (use_rlimit && srv->srvconf.enable_cores && getrlimit(RLIMIT_CORE, &rlim) == 0) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_CORE, &rlim);
		}
#endif
		if (srv->event_handler == FDEVENT_HANDLER_SELECT) {
			/* don't raise the limit above FD_SET_SIZE */
			if (srv->max_fds > ((int)FD_SETSIZE) - 200) {
				log_error_write(srv, __FILE__, __LINE__, "sd",
						"can't raise max filedescriptors above",  FD_SETSIZE - 200,
						"if event-handler is 'select'. Use 'poll' or something else or reduce server.max-fds.");
				return -1;
			}
		}


#ifdef HAVE_PWD_H
		/* set user and group */
		if (srv->srvconf.username->used) {
			if (NULL == (pwd = getpwnam(srv->srvconf.username->ptr))) {
				log_error_write(srv, __FILE__, __LINE__, "sb",
						"can't find username", srv->srvconf.username);
				return -1;
			}

			if (pwd->pw_uid == 0) {
				log_error_write(srv, __FILE__, __LINE__, "s",
						"I will not set uid to 0\n");
				return -1;
			}
		}

		if (srv->srvconf.groupname->used) {
			if (NULL == (grp = getgrnam(srv->srvconf.groupname->ptr))) {
				log_error_write(srv, __FILE__, __LINE__, "sb",
					"can't find groupname", srv->srvconf.groupname);
				return -1;
			}
			if (grp->gr_gid == 0) {
				log_error_write(srv, __FILE__, __LINE__, "s",
						"I will not set gid to 0\n");
				return -1;
			}
		}
#endif
		/* we need root-perms for port < 1024 */
		if (0 != network_init(srv)) {
			plugins_free(srv);
			server_free(srv);

			return -1;
		}

#ifdef HAVE_PWD_H
		/**
		 * initgroups() has to be called before chroot()
		 */
		if (srv->srvconf.groupname->used) {
			setgid(grp->gr_gid);
			setgroups(0, NULL);
			if (srv->srvconf.username->used) {
				initgroups(srv->srvconf.username->ptr, grp->gr_gid);
			}
		}
#endif
#ifdef HAVE_CHROOT
		if (srv->srvconf.changeroot->used) {
			tzset();

			if (-1 == chroot(srv->srvconf.changeroot->ptr)) {
				log_error_write(srv, __FILE__, __LINE__, "ss", "chroot failed: ", strerror(errno));
				return -1;
			}
			if (-1 == chdir("/")) {
				log_error_write(srv, __FILE__, __LINE__, "ss", "chdir failed: ", strerror(errno));
				return -1;
			}
		}
#endif
#ifdef HAVE_PWD_H
		/* drop root privs */
		if (srv->srvconf.username->used) setuid(pwd->pw_uid);
#endif
#if defined(HAVE_SYS_PRCTL_H) && defined(PR_SET_DUMPABLE)
		/**
		 * IRIX 6.5.x has prctl() but no PR_SET_DUMPABLE
		 */
		if (srv->srvconf.enable_cores) {
			prctl(PR_SET_DUMPABLE, 1, 0, 0, 0);
		}
#endif
	} else {

#ifdef HAVE_GETRLIMIT
		if (0 != getrlimit(RLIMIT_NOFILE, &rlim)) {
			log_error_write(srv, __FILE__, __LINE__,
					"ss", "couldn't get 'max filedescriptors'",
					strerror(errno));
			return -1;
		}

		if (srv->event_handler == FDEVENT_HANDLER_SELECT) {
			srv->max_fds = rlim.rlim_cur < ((int)FD_SETSIZE) - 4 ? rlim.rlim_cur : FD_SETSIZE - 4;
		} else {
			srv->max_fds = rlim.rlim_cur;
		}

		/* set core file rlimit, if enable_cores is set */
		if (srv->srvconf.enable_cores && getrlimit(RLIMIT_CORE, &rlim) == 0) {
			rlim.rlim_cur = rlim.rlim_max;
			setrlimit(RLIMIT_CORE, &rlim);
		}

#endif
		if (srv->event_handler == FDEVENT_HANDLER_SELECT) {
			/* don't raise the limit above FD_SET_SIZE */
			if (srv->max_fds > ((int)FD_SETSIZE) - 4) {
				log_error_write(srv, __FILE__, __LINE__, "sd",
						"can't raise max filedescriptors above",  FD_SETSIZE - 4,
						"if event-handler is 'select'. Use 'poll' or something else or reduce server.max-fds.");
				return -1;
			}
		}

		if (0 != network_init(srv)) {
			plugins_free(srv);
			server_free(srv);

			return -1;
		}
	}

	/* set max-conns */
	if (srv->srvconf.max_conns > srv->max_fds/2) {
		/* we can't have more connections than max-fds/2 */
		log_error_write(srv, __FILE__, __LINE__, "sdd", "can't have more connections than fds/2: ", srv->srvconf.max_conns, srv->max_fds);
		srv->max_conns = srv->max_fds/2;
	} else if (srv->srvconf.max_conns) {
		/* otherwise respect the wishes of the user */
		srv->max_conns = srv->srvconf.max_conns;
	} else {
		/* or use the default: we really don't want to hit max-fds */
		srv->max_conns = srv->max_fds/3;
	}

	if (HANDLER_GO_ON != plugins_call_init(srv)) {
		log_error_write(srv, __FILE__, __LINE__, "s", "Initialization of plugins failed. Going down.");

		plugins_free(srv);
		network_close(srv);
		server_free(srv);

		return -1;
	}

#ifdef HAVE_FORK
	/* network is up, let's deamonize ourself */
	if (srv->srvconf.dont_daemonize == 0) daemonize();
#endif

#ifdef HAVE_PWD_H
	srv->gid = getgid();
	srv->uid = getuid();
#endif

	/* write pid file */
	if (pid_fd != -1) {
		buffer_copy_long(srv->tmp_buf, getpid());
		buffer_append_string_len(srv->tmp_buf, CONST_STR_LEN("\n"));
		if (-1 == write(pid_fd, srv->tmp_buf->ptr, srv->tmp_buf->used - 1)) {
			ERROR("writing to PID to '%s' failed: %s, ignored", "...", strerror(errno));
		}
		close(pid_fd);
		pid_fd = -1;
	}

	if (HANDLER_GO_ON != plugins_call_set_defaults(srv)) {
		log_error_write(srv, __FILE__, __LINE__, "s", "Configuration of plugins failed. Going down.");

		plugins_free(srv);
		network_close(srv);
		server_free(srv);

		return -1;
	}

	/* dump unused config-keys */
	for (i = 0; i < srv->config_context->used; i++) {
		array *config = ((data_config *)srv->config_context->data[i])->value;
		size_t j;

		for (j = 0; config && j < config->used; j++) {
			data_unset *du = config->data[j];

			/* all var.* is known as user defined variable */
			if (strncmp(du->key->ptr, "var.", sizeof("var.") - 1) == 0) {
				continue;
			}

			if (NULL == array_get_element(srv->config_touched, CONST_BUF_LEN(du->key))) {
				log_error_write(srv, __FILE__, __LINE__, "sbs",
						"WARNING: unknown config-key:",
						du->key,
						"(ignored)");
			}
		}
	}

	if (srv->config_unsupported) {
		log_error_write(srv, __FILE__, __LINE__, "s",
				"Configuration contains unsupported keys. Going down.");
	}

	if (srv->config_deprecated) {
		log_error_write(srv, __FILE__, __LINE__, "s",
				"Configuration contains deprecated keys. Going down.");
	}

	if (srv->config_unsupported || srv->config_deprecated) {
		plugins_free(srv);
		network_close(srv);
		server_free(srv);

		return -1;
	}

	if (-1 == log_error_open(srv->srvconf.errorlog_file, srv->srvconf.breakagelog_file, srv->srvconf.errorlog_use_syslog, srv->srvconf.dont_daemonize)) {
		log_error_write(srv, __FILE__, __LINE__, "s",
				"opening errorlog failed, dying");

		plugins_free(srv);
		network_close(srv);
		server_free(srv);
		return -1;
	}

#ifdef HAVE_SIGACTION
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &act, NULL);
# if defined(SA_SIGINFO)
	act.sa_sigaction = sigaction_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
# else
	act.sa_handler = signal_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
# endif
	sigaction(SIGINT,  &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGHUP,  &act, NULL);
	sigaction(SIGALRM, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);

#elif defined(HAVE_SIGNAL)
	/* ignore the SIGPIPE from sendfile() */
	signal(SIGPIPE, SIG_IGN);
	signal(SIGALRM, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGHUP,  signal_handler);
	signal(SIGCHLD,  signal_handler);
	signal(SIGINT,  signal_handler);
#endif

#ifdef USE_ALARM
	signal(SIGALRM, signal_handler);

	/* setup periodic timer (1 second) */
	if (setitimer(ITIMER_REAL, &interval, NULL)) {
		log_error_write(srv, __FILE__, __LINE__, "s", "setting timer failed");
		return -1;
	}

	getitimer(ITIMER_REAL, &interval);
#endif

#ifdef HAVE_FORK
	/* start watcher and workers */
	num_childs = srv->srvconf.max_worker;
	if (num_childs > 0) {
		int child = 0;
		while (!child && !srv_shutdown) {
			if (num_childs > 0) {
				switch (fork()) {
				case -1:
					return -1;
				case 0:
					child = 1;
					break;
				default:
					num_childs--;
					break;
				}
			} else {
				int status;

				if (-1 != wait(&status)) {
					/* a child terminated, restart it */
					num_childs++;
				} else {
					/* we got interrupted */

					switch (errno) {
					case EINTR:
						/** 
						 * we got asked to rotate the logs
						 *
						 * as we are just the parent and have no main-loop we have to fix it ourself
						 */
						if (handle_sig_hup) {
							handle_sig_hup = 0;

							log_error_cycle();
						}
						break;
					default:
						TRACE("(angel) wait() failed with: %s (errno=%d)", strerror(errno), errno);
						break;
					}
				}
			}
		}

		if (srv_shutdown) {
			/* kill all childs */
			kill(0, SIGTERM);
		}

		/* if we are the parent, leave here */
		if (!child) return 0;
	}
#endif

	if (NULL == (srv->ev = fdevent_init(srv->max_fds + 1, srv->event_handler))) {
		log_error_write(srv, __FILE__, __LINE__,
				"s", "fdevent_init failed");
		return -1;
	}
	/*
	 * kqueue() is called here, select resets its internals,
	 * all server sockets get their handlers
	 *
	 * */
	if (0 != network_register_fdevents(srv)) {
		plugins_free(srv);
		network_close(srv);
		server_free(srv);

		return -1;
	}

#ifdef USE_GTHREAD
	if (pipe(srv->wakeup_pipe) == -1) {
		log_error_write(srv, __FILE__, __LINE__, "s", "pipe() failed");
		return -1;
	}
	srv->wakeup_iosocket = iosocket_init();
	srv->wakeup_iosocket->type = IOSOCKET_TYPE_PIPE;	
	srv->wakeup_iosocket->fd = srv->wakeup_pipe[0];
	srv->did_wakeup = 0;
	fdevent_fcntl_set(srv->ev, srv->wakeup_iosocket);
	/* block on write */
#ifdef FD_CLOEXEC
	/* close fd on exec (cgi) */
	fcntl(srv->wakeup_pipe[1], F_SETFD, FD_CLOEXEC);
#endif
	fdevent_register(srv->ev, srv->wakeup_iosocket, wakeup_handle_fdevent, NULL);
	fdevent_event_add(srv->ev, srv->wakeup_iosocket, FDEVENT_IN);
#endif

	/* might fail if user is using fam (not gamin) and famd isn't running */
	if (NULL == (srv->stat_cache = stat_cache_init())) {
		log_error_write(srv, __FILE__, __LINE__, "s",
			"stat-cache could not be setup, dieing.");
		return -1;
	}

#ifdef USE_GTHREAD
	g_thread_init(NULL);

	srv->stat_queue = g_async_queue_new();
	srv->joblist_queue = g_async_queue_new();
	srv->aio_write_queue = g_async_queue_new();
#ifdef HAVE_SYS_INOTIFY_H
	if (srv->srvconf.stat_cache_engine == STAT_CACHE_ENGINE_INOTIFY) {
		srv->stat_cache->sock->fd = inotify_init();

		fdevent_register(srv->ev, srv->stat_cache->sock, stat_cache_handle_fdevent, NULL);
		fdevent_event_add(srv->ev, srv->stat_cache->sock, FDEVENT_IN);
	}
#endif
	stat_cache_threads = calloc(srv->srvconf.max_stat_threads, sizeof(*stat_cache_threads));

	for (i = 0; i < srv->srvconf.max_stat_threads; i++) {
		stat_cache_threads[i] = g_thread_create(stat_cache_thread, srv, 1, &gerr);
		if (gerr) {
			ERROR("g_thread_create failed: %s", gerr->message);

			return -1;
		}
	}

#ifndef _WIN32
	switch (srv->network_backend) {
	case NETWORK_BACKEND_GTHREAD_AIO:
		aio_write_threads = calloc(srv->srvconf.max_read_threads, sizeof(*aio_write_threads));
		for (i = 0; i < srv->srvconf.max_read_threads; i++) {
			aio_write_threads[i] = g_thread_create_full(network_gthread_aio_read_thread, srv, LI_THREAD_STACK_SIZE, 1, TRUE, G_THREAD_PRIORITY_NORMAL, &gerr);
			if (gerr) {
				ERROR("g_thread_create failed: %s", gerr->message);

				return -1;
			}
		}
		break;
#ifdef USE_GTHREAD_SENDFILE
	case NETWORK_BACKEND_GTHREAD_SENDFILE:
		aio_write_threads = calloc(srv->srvconf.max_read_threads, sizeof(*aio_write_threads));
		for (i = 0; i < srv->srvconf.max_read_threads; i++) {
			aio_write_threads[i] = g_thread_create_full(network_gthread_sendfile_read_thread, srv, LI_THREAD_STACK_SIZE, 1, TRUE, G_THREAD_PRIORITY_NORMAL, &gerr);
			if (gerr) {
				ERROR("g_thread_create failed: %s", gerr->message);

				return -1;
			}
		}
		break;
#endif
#ifdef USE_GTHREAD_FREEBSD_SENDFILE
	case NETWORK_BACKEND_GTHREAD_FREEBSD_SENDFILE:
		aio_write_threads = calloc(srv->srvconf.max_read_threads, sizeof(*aio_write_threads));
		for (i = 0; i < srv->srvconf.max_read_threads; i++) {
			aio_write_threads[i] = g_thread_create_full(network_gthread_freebsd_sendfile_read_thread, srv, LI_THREAD_STACK_SIZE, 1, TRUE, G_THREAD_PRIORITY_NORMAL, &gerr);
			if (gerr) {
				ERROR("g_thread_create failed: %s", gerr->message);

				return -1;
			}
		}
		break;
#endif
#ifdef USE_POSIX_AIO
	case NETWORK_BACKEND_POSIX_AIO:
		srv->posix_aio_iocbs = calloc(srv->srvconf.max_read_threads, sizeof(*srv->posix_aio_iocbs));
		break;
#endif
#ifdef USE_LINUX_AIO_SENDFILE
	case NETWORK_BACKEND_LINUX_AIO_SENDFILE:
		TRACE("WARNING: You selected the experimental network.backend '%s'", "linux-aio-sendfile");
		srv->linux_io_iocbs = calloc(srv->srvconf.max_read_threads, sizeof(*srv->linux_io_iocbs));
		if (0 != (i = io_setup(srv->srvconf.max_read_threads, &(srv->linux_io_ctx)))) {
			ERROR("io-setup() failed somehow %s", strerror(-i));

			return -1;
		}
		linux_aio_read_thread_id = g_thread_create(linux_aio_read_thread, srv, 1, &gerr);
		if (gerr) {
			ERROR("g_thread_create failed: %s", gerr->message);

			return -1;
		}
		break;
#endif
	default:
		break;
	}
#endif /* ifndef _WIN32 */

#endif /* USE_GTHREAD */

	for (i = 0; i < srv->srv_sockets.used; i++) {
		server_socket *srv_socket = srv->srv_sockets.ptr[i];
		if (-1 == fdevent_fcntl_set(srv->ev, srv_socket->sock)) {
			log_error_write(srv, __FILE__, __LINE__, "ss", "fcntl failed:", strerror(errno));
			return -1;
		}
	}

	lighty_mainloop(srv);

	if (0 == graceful_restart &&
	    srv->srvconf.pid_file->used &&
	    srv->srvconf.changeroot->used == 0) {
		if (0 != unlink(srv->srvconf.pid_file->ptr)) {
			if (errno != EACCES && errno != EPERM) {
				log_error_write(srv, __FILE__, __LINE__, "sbds",
						"unlink failed for:",
						srv->srvconf.pid_file,
						errno,
						strerror(errno));
			}
		}
	}

	/* kill the threads */

	srv->is_shutdown = 1;
#ifdef USE_GTHREAD
#ifdef USE_LINUX_AIO_SENDFILE
	if (srv->network_backend == NETWORK_BACKEND_LINUX_AIO_SENDFILE) {
		g_thread_join(linux_aio_read_thread_id);
		free(srv->linux_io_iocbs);
	}
#endif
#ifdef USE_POSIX_AIO
	if (srv->network_backend == NETWORK_BACKEND_POSIX_AIO) {
		free(srv->posix_aio_iocbs);
	}
#endif
	if (aio_write_threads != NULL) {
		for (i = 0; i < srv->srvconf.max_read_threads; i++) {
			g_thread_join(aio_write_threads[i]);
		}
		free(aio_write_threads);
	}

	for (i = 0; i < srv->srvconf.max_stat_threads; i++) {
                g_async_queue_push(srv->stat_queue, (void *) 1);
	}

	for (i = 0; i < srv->srvconf.max_stat_threads; i++) {
		g_thread_join(stat_cache_threads[i]);
	}

	/* the ref-count should be 0 now */
	g_async_queue_unref(srv->stat_queue);
	g_async_queue_unref(srv->joblist_queue);
	g_async_queue_unref(srv->aio_write_queue);
#endif
	/* clean-up */
	network_close(srv);
	connections_free(srv);
	plugins_free(srv);
	server_free(srv);

	TRACE("server stopped by UID=%d, PID=%d", last_sigterm_info.si_uid, last_sigterm_info.si_pid);

	log_free();
	status_counter_free();

#ifdef USE_GTHREAD
	free(stat_cache_threads);
#endif

	chunkpool_free();

	return 0;
}
