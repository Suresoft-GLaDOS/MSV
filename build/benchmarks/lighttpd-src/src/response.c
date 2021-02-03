#include <sys/types.h>
#include <sys/stat.h>

#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>

#include <stdio.h>

#include "settings.h"

#include "response.h"
#include "keyvalue.h"
#include "log.h"
#include "stat_cache.h"
#include "chunk.h"

#include "connections.h"

#include "plugin.h"

#include "sys-socket.h"
#include "sys-files.h"
#include "sys-strings.h"

int http_response_write_header(server *srv, connection *con, chunkqueue *raw) {
	buffer *b;
	size_t i;
	int have_date = 0;
	int have_server = 0;
	int allow_keep_alive = 0;

	b = chunkqueue_get_prepend_buffer(raw);

	if (con->request.http_version == HTTP_VERSION_1_1) {
		buffer_copy_string_len(b, CONST_STR_LEN("HTTP/1.1 "));
	} else {
		buffer_copy_string_len(b, CONST_STR_LEN("HTTP/1.0 "));
	}
	buffer_append_long(b, con->http_status);
	buffer_append_string_len(b, CONST_STR_LEN(" "));
	buffer_append_string(b, get_http_status_name(con->http_status));

	if (con->response.transfer_encoding & HTTP_TRANSFER_ENCODING_CHUNKED) {
		response_header_overwrite(srv, con, CONST_STR_LEN("Transfer-Encoding"), CONST_STR_LEN("chunked"));
		allow_keep_alive = 1;
	} else if ((con->http_status >= 100 && con->http_status < 200) ||
		    con->http_status == 204 ||
		    con->http_status == 304) {
		/* 1xx, 204 and 304 never have a content-body -> 
		 *   never have a Content-Length and are always 
		 *   able to do keep-alive
		 */
		allow_keep_alive = 1;
	} else if (con->response.content_length >= 0) {
		buffer_copy_off_t(srv->tmp_buf, con->response.content_length);
		response_header_overwrite(srv, con, CONST_STR_LEN("Content-Length"), srv->tmp_buf->ptr, srv->tmp_buf->used - 1);

		allow_keep_alive = 1;
	}

	/* keep-alive needs Content-Length or chunked encoding. */
	if (!allow_keep_alive) con->keep_alive = 0;

	/* disable keep-alive if requested */
	if (con->request_count > con->conf.max_keep_alive_requests || 0 == con->conf.max_keep_alive_idle) {
		con->keep_alive = 0;
	} else {
		con->keep_alive_idle = con->conf.max_keep_alive_idle;
	}

	if (con->request.http_version != HTTP_VERSION_1_1 || con->keep_alive == 0) {
		if (con->keep_alive) {
			response_header_overwrite(srv, con, CONST_STR_LEN("Connection"), CONST_STR_LEN("keep-alive"));
		} else {
			response_header_overwrite(srv, con, CONST_STR_LEN("Connection"), CONST_STR_LEN("close"));
		}
	}


	/* add all headers */
	for (i = 0; i < con->response.headers->used; i++) {
		data_string *ds;

		ds = (data_string *)con->response.headers->data[i];

		if (ds->value->used && ds->key->used &&
		    0 != strncasecmp(ds->key->ptr, CONST_STR_LEN("X-LIGHTTPD-")) &&
		    0 != strcasecmp(ds->key->ptr, "X-Sendfile")) {
			if (0 == strcasecmp(ds->key->ptr, "Date")) have_date = 1;
			if (0 == strcasecmp(ds->key->ptr, "Server")) have_server = 1;

			buffer_append_string_len(b, CONST_STR_LEN("\r\n"));
			buffer_append_string_buffer(b, ds->key);
			buffer_append_string_len(b, CONST_STR_LEN(": "));
			buffer_append_string_buffer(b, ds->value);
#if 0
			log_error_write(srv, __FILE__, __LINE__, "bb",
					ds->key, ds->value);
#endif
		}
	}

	if (!have_date) {
		/* HTTP/1.1 requires a Date: header */
		buffer_append_string_len(b, CONST_STR_LEN("\r\nDate: "));

		/* cache the generated timestamp */
		if (srv->cur_ts != srv->last_generated_date_ts) {
			buffer_prepare_copy(srv->ts_date_str, 255);

			strftime(srv->ts_date_str->ptr, srv->ts_date_str->size - 1,
				 "%a, %d %b %Y %H:%M:%S GMT", gmtime(&(srv->cur_ts)));

			srv->ts_date_str->used = strlen(srv->ts_date_str->ptr) + 1;

			srv->last_generated_date_ts = srv->cur_ts;
		}

		buffer_append_string_buffer(b, srv->ts_date_str);
	}

	if (!have_server) {
		if (buffer_is_empty(con->conf.server_tag)) {
			buffer_append_string_len(b, CONST_STR_LEN("\r\nServer: " PACKAGE_NAME "/" PACKAGE_VERSION));
		} else {
			buffer_append_string_len(b, CONST_STR_LEN("\r\nServer: "));
			buffer_append_string_buffer(b, con->conf.server_tag);
		}
	}

	buffer_append_string_len(b, CONST_STR_LEN("\r\n\r\n"));


	con->bytes_header = b->used - 1;
	raw->bytes_in += b->used - 1;

	if (con->conf.log_response_header) {
		log_error_write(srv, __FILE__, __LINE__, "sSb", "Response-Header:", "\n", b);
	}

	return 0;
}

#ifdef USE_OPENSSL
static void https_add_ssl_entries(connection *con) {
	X509 *xs;
	X509_NAME *xn;
	X509_NAME_ENTRY *xe;
	if (
		SSL_get_verify_result(con->sock->ssl) != X509_V_OK
		|| !(xs = SSL_get_peer_certificate(con->sock->ssl))
	) {
		return;
	}

	xn = X509_get_subject_name(xs);
	for (int i = 0, nentries = X509_NAME_entry_count(xn); i < nentries; ++i) {
		int xobjnid;
		const char * xobjsn;
		data_string *envds;

		if (!(xe = X509_NAME_get_entry(xn, i))) {
			continue;
		}
		xobjnid = OBJ_obj2nid((ASN1_OBJECT*)X509_NAME_ENTRY_get_object(xe));
		xobjsn = OBJ_nid2sn(xobjnid);
		if (!xobjsn) {
			continue;
		}

		if (NULL == (envds = (data_string *)array_get_unused_element(con->environment, TYPE_STRING))) {
			envds = data_string_init();
		}
		buffer_copy_string_len(envds->key, CONST_STR_LEN("SSL_CLIENT_S_DN_"));
		buffer_append_string(envds->key, xobjsn);
		buffer_copy_string_len(
			envds->value,
			(const char *)xe->value->data, xe->value->length
		);
		/* pick one of the exported values as "authed user", for example
		 * ssl.verifyclient.username   = "SSL_CLIENT_S_DN_UID" or "SSL_CLIENT_S_DN_emailAddress"
		 */
		if (buffer_is_equal(con->conf.ssl_verifyclient_username, envds->key)) {
			buffer_copy_string_buffer(con->authed_user, envds->value);
		}
		array_insert_unique(con->environment, (data_unset *)envds);
	}
	if (con->conf.ssl_verifyclient_export_cert) {
		BIO *bio;
		if (NULL != (bio = BIO_new(BIO_s_mem()))) {
			data_string *envds;
			int n;

			PEM_write_bio_X509(bio, xs);
			n = BIO_pending(bio);

			if (NULL == (envds = (data_string *)array_get_unused_element(con->environment, TYPE_STRING))) {
				envds = data_string_init();
			}

			buffer_copy_string_len(envds->key, CONST_STR_LEN("SSL_CLIENT_CERT"));
			buffer_prepare_copy(envds->value, n+1);
			BIO_read(bio, envds->value->ptr, n);
			BIO_free(bio);
			envds->value->ptr[n] = '\0';
			envds->value->used = n+1;
			array_insert_unique(con->environment, (data_unset *)envds);
		}
	}
	X509_free(xs);
}
#endif


handler_t handle_get_backend(server *srv, connection *con) {
	handler_t r;

	/* looks like someone has already made a decision */
	if (con->mode == DIRECT &&
	    (con->http_status != 0 && con->http_status != 200)) {
		/* remove a packets in the queue */

		return HANDLER_FINISHED;
	}

	/* no decision yet, build conf->filename */
	if (con->mode == DIRECT && con->physical.path->used == 0) {
		char *qstr;

		/* we only come here when we have to parse the full request again
		 *
		 * a HANDLER_COMEBACK from mod_rewrite and mod_fastcgi might be a
		 * problem here as mod_setenv might get called multiple times
		 *
		 * fastcgi-auth might lead to a COMEBACK too
		 * fastcgi again dead server too
		 *
		 * mod_compress might add headers twice too
		 *
		 *  */

		if (con->conf.log_condition_handling) {
			TRACE("run condition: %s", "");
		}
		config_patch_connection(srv, con, COMP_SERVER_SOCKET); /* SERVERsocket */

		/**
		 * prepare strings
		 *
		 * - uri.path_raw
		 * - uri.path (secure)
		 * - uri.query
		 *
		 */

		/**
		 * Name according to RFC 2396
		 *
		 * - scheme
		 * - authority
		 * - path
		 * - query
		 *
		 * (scheme)://(authority)(path)?(query)#fragment
		 *
		 *
		 */

		if (con->conf.is_ssl) {
			buffer_copy_string_len(con->uri.scheme, CONST_STR_LEN("https"));
		} else {
			buffer_copy_string_len(con->uri.scheme, CONST_STR_LEN("http"));
		}
		buffer_copy_string_buffer(con->uri.authority, con->request.http_host);
		buffer_to_lower(con->uri.authority);

		config_patch_connection(srv, con, COMP_HTTP_SCHEME);    /* Scheme:      */
		config_patch_connection(srv, con, COMP_HTTP_HOST);      /* Host:        */
		config_patch_connection(srv, con, COMP_HTTP_REMOTE_IP); /* Client-IP */
		config_patch_connection(srv, con, COMP_HTTP_REFERER);   /* Referer:     */
		config_patch_connection(srv, con, COMP_HTTP_USER_AGENT);/* User-Agent:  */
		config_patch_connection(srv, con, COMP_HTTP_COOKIE);    /* Cookie:  */
		config_patch_connection(srv, con, COMP_HTTP_REQUEST_METHOD); /* REQUEST_METHOD */

		/** their might be a fragment which has to be cut away */
		if (NULL != (qstr = strchr(con->request.uri->ptr, '#'))) {
			con->request.uri->used = qstr - con->request.uri->ptr;
			con->request.uri->ptr[con->request.uri->used++] = '\0';
		}

		/** extract query string from request.uri */
		if (NULL != (qstr = strchr(con->request.uri->ptr, '?'))) {
			buffer_copy_string    (con->uri.query, qstr + 1);
			buffer_copy_string_len(con->uri.path_raw, con->request.uri->ptr, qstr - con->request.uri->ptr);
		} else {
			buffer_reset     (con->uri.query);
			buffer_copy_string_buffer(con->uri.path_raw, con->request.uri);
		}

		if (con->conf.log_request_handling) {
			TRACE("-- %s", "splitting Request-URI");
			TRACE("Request-URI  : %s", SAFE_BUF_STR(con->request.uri));
			TRACE("URI-scheme   : %s", SAFE_BUF_STR(con->uri.scheme));
			TRACE("URI-authority: %s", SAFE_BUF_STR(con->uri.authority));
			TRACE("URI-path     : %s", SAFE_BUF_STR(con->uri.path_raw));
			TRACE("URI-query    : %s", SAFE_BUF_STR(con->uri.query));
		}

		if (srv->sockets_disabled) {
			con->keep_alive = 0;
		}


		/**
		 *
		 * call plugins
		 *
		 * - based on the raw URL
		 *
		 */

		switch(r = plugins_call_handle_uri_raw(srv, con)) {
		case HANDLER_GO_ON:
			break;
		case HANDLER_FINISHED:
		case HANDLER_COMEBACK:
		case HANDLER_WAIT_FOR_EVENT:
		case HANDLER_ERROR:
			return r;
		default:
			ERROR("plugins_call_handle_uri_raw() returned unexpected: %d", r);
			break;
		}

		/* build filename
		 *
		 * - decode url-encodings  (e.g. %20 -> ' ')
		 * - remove path-modifiers (e.g. /../)
		 */



		if (con->request.http_method == HTTP_METHOD_OPTIONS &&
		    con->uri.path_raw->ptr[0] == '*' && con->uri.path_raw->ptr[1] == '\0') {
			/* OPTIONS * ... */
			buffer_copy_string_buffer(con->uri.path, con->uri.path_raw);
		} else {
			buffer_copy_string_buffer(srv->tmp_buf, con->uri.path_raw);
			buffer_urldecode_path(srv->tmp_buf);
			buffer_path_simplify(con->uri.path, srv->tmp_buf);
		}

		if (con->conf.log_request_handling) {
			TRACE("-- %s", "sanitizing URI");
			TRACE("URI-path     : %s", SAFE_BUF_STR(con->uri.path));
		}

#ifdef USE_OPENSSL
		if (con->conf.is_ssl && con->conf.ssl_verifyclient) {
			https_add_ssl_entries(con);
		}
#endif

		/**
		 *
		 * call plugins
		 *
		 * - based on the clean URL
		 *
		 */

		config_patch_connection(srv, con, COMP_HTTP_URL); /* HTTPurl */
		config_patch_connection(srv, con, COMP_HTTP_QUERY_STRING); /* HTTPqs */

		/* do we have to downgrade to 1.0 ? */
		if (!con->conf.allow_http11) {
			con->request.http_version = HTTP_VERSION_1_0;
		}

		switch(r = plugins_call_handle_uri_clean(srv, con)) {
		case HANDLER_GO_ON:
			break;
		case HANDLER_FINISHED:
		case HANDLER_COMEBACK:
		case HANDLER_WAIT_FOR_EVENT:
		case HANDLER_ERROR:
			return r;
		default:
			ERROR("plugins_call_handle_uri_clean() returned unexpected: %d", r);
			break;
		}

		if (con->request.http_method == HTTP_METHOD_OPTIONS &&
		    con->uri.path->ptr[0] == '*' && con->uri.path_raw->ptr[1] == '\0') {
			/* option requests are handled directly without checking the path */

			response_header_insert(srv, con, CONST_STR_LEN("Allow"), CONST_STR_LEN("OPTIONS, GET, HEAD, POST"));

			con->http_status = 200;
			/* no more content to send */
			con->send->is_closed = 1;

			return HANDLER_FINISHED;
		}

		/***
		 *
		 * border
		 *
		 * logical filename (URI) becomes a physical filename here
		 *
		 *
		 *
		 */




		/* 1. stat()
		 * ... ISREG() -> ok, go on
		 * ... ISDIR() -> index-file -> redirect
		 *
		 * 2. pathinfo()
		 * ... ISREG()
		 *
		 * 3. -> 404
		 *
		 */

		/*
		 * SEARCH DOCUMENT ROOT
		 */

		/* set a default */

		buffer_copy_string_buffer(con->physical.doc_root, con->conf.document_root);
		buffer_copy_string_buffer(con->physical.rel_path, con->uri.path);

		filename_unix2local(con->physical.rel_path);
#if defined(_WIN32) || defined(__CYGWIN__)
		/* strip dots and spaces from the end
		 *
		 * windows/dos handle those filenames as the same file
		 *
		 * foo == foo. == foo..... == "foo...   " == "foo..  ./"
		 *
		 * This will affect PATHINFO in some cases
		 *
		 * on native windows we could prepend the filename with \\?\ to circumvent
		 * this behaviour. I have no idea how to push this through cygwin
		 *
		 * */

		if (con->physical.rel_path->used > 1) {
			buffer *b = con->physical.rel_path;
			size_t i;

			if (b->used > 2 &&
			    b->ptr[b->used-2] == '/' &&
			    (b->ptr[b->used-3] == ' ' ||
			     b->ptr[b->used-3] == '.')) {
				b->ptr[b->used--] = '\0';
			}

			for (i = b->used - 2; b->used > 1; i--) {
				if (b->ptr[i] == ' ' ||
				    b->ptr[i] == '.') {
					b->ptr[b->used--] = '\0';
				} else {
					break;
				}
			}
		}
#endif

		if (con->conf.log_request_handling) {
			TRACE("-- %s", "before doc_root");
			TRACE("Doc-Root     : %s", SAFE_BUF_STR(con->physical.doc_root));
			TRACE("Rel-Path     : %s", SAFE_BUF_STR(con->physical.rel_path));
			TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
		}
		/* the docroot plugin should set the doc_root and might also set the physical.path
		 * for us (all vhost-plugins are supposed to set the doc_root)
		 * */
		switch(r = plugins_call_handle_docroot(srv, con)) {
		case HANDLER_GO_ON:
			break;
		case HANDLER_FINISHED:
		case HANDLER_COMEBACK:
		case HANDLER_WAIT_FOR_EVENT:
		case HANDLER_ERROR:
			return r;
		default:
			ERROR("plugins_call_handle_docroot() returned unexpected: %d", r);
			break;
		}

		/* The default Mac OS X and Windows filesystems can't distiguish between
		 * upper- and lowercase, so convert to lowercase
		 */
		if (con->conf.force_lowercase_filenames) {
			buffer_to_lower(con->physical.rel_path);
		}

		/* the docroot plugins might set the servername; if they don't we take http-host */
		if (buffer_is_empty(con->server_name)) {
			buffer_copy_string_buffer(con->server_name, con->uri.authority);
		}

		/**
		 * create physical filename
		 * -> physical.path = docroot + rel_path
		 *
		 */

		buffer_copy_string_buffer(con->physical.path, con->physical.doc_root);
		PATHNAME_APPEND_SLASH(con->physical.path);
		buffer_copy_string_buffer(con->physical.basedir, con->physical.path);
		if (con->physical.rel_path->used &&
		    con->physical.rel_path->ptr[0] == DIR_SEPERATOR) {
			buffer_append_string_len(con->physical.path, con->physical.rel_path->ptr + 1, con->physical.rel_path->used - 2);
		} else {
			buffer_append_string_buffer(con->physical.path, con->physical.rel_path);
		}

		/* win32: directories can't have a trailing slash */
		if (con->physical.path->ptr[con->physical.path->used - 2] == DIR_SEPERATOR) {
			con->physical.path->ptr[con->physical.path->used - 2] = '\0';
			con->physical.path->used--;
		}

		if (con->conf.log_request_handling) {
			TRACE("-- %s", "after doc_root");
			TRACE("Doc-Root     : %s", SAFE_BUF_STR(con->physical.doc_root));
			TRACE("Rel-Path     : %s", SAFE_BUF_STR(con->physical.rel_path));
			TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
		}

		switch(r = plugins_call_handle_physical(srv, con)) {
		case HANDLER_GO_ON:
			break;
		case HANDLER_FINISHED:
		case HANDLER_COMEBACK:
		case HANDLER_WAIT_FOR_EVENT:
		case HANDLER_ERROR:
			return r;
		default:
			ERROR("plugins_call_handle_physical() returned unexpected: %d", r);
			break;
		}

		config_patch_connection(srv, con, COMP_PHYSICAL_PATH);    /* physical-path  */

		if (con->conf.log_request_handling) {
			TRACE("-- %s", "logical -> physical");
			TRACE("Doc-Root     : %s", SAFE_BUF_STR(con->physical.doc_root));
			TRACE("Rel-Path     : %s", SAFE_BUF_STR(con->physical.rel_path));
			TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
		}
	}

	/*
	 * No one took the file away from the normal path of execution yet (like mod_access)
	 *
	 * we don't have a backend yet, try to resolve the physical path and go on
	 *
	 */

	if (con->mode == DIRECT) {
		char *slash = NULL;
		char *pathinfo = NULL;
		int found = 0;
		stat_cache_entry *sce = NULL;

		if (con->conf.log_request_handling) {
			TRACE("-- %s", "handling physical path");
			TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
		}

		switch ((r = stat_cache_get_entry_async(srv, con, con->physical.path, &sce))) {
		case HANDLER_GO_ON:
			/* file exists */

			if (con->conf.log_request_handling) {
				TRACE("-- %s", "file found");
				TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
			}

#ifdef HAVE_LSTAT
			if ((sce->is_symlink != 0) && !con->conf.follow_symlink) {
				con->http_status = 403;

				if (con->conf.log_request_handling) {
					TRACE("-- %s", "access denied due symlink restriction");
					TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
				}

				buffer_reset(con->physical.path);
				return HANDLER_FINISHED;
			};
#endif

			if (S_ISDIR(sce->st.st_mode)) {
				if (con->uri.path->ptr[con->uri.path->used - 2] != '/') {
					/* redirect to .../ */

					http_response_redirect_to_directory(srv, con);

					return HANDLER_FINISHED;
				}
#ifdef HAVE_LSTAT
			} else if (!S_ISREG(sce->st.st_mode) && !sce->is_symlink) {
#else
			} else if (!S_ISREG(sce->st.st_mode)) {
#endif
				/* any special handling of non-reg files ?*/


			}
			break;
		case HANDLER_WAIT_FOR_EVENT:
			return HANDLER_WAIT_FOR_EVENT;
		case HANDLER_ERROR:
			switch (errno) {
			case EACCES:
				con->http_status = 403;

				if (con->conf.log_request_handling) {
					TRACE("-- %s", "access denied");
					TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
				}

				buffer_reset(con->physical.path);
				return HANDLER_FINISHED;
			case ENOENT:
				con->http_status = 404;

				if (con->conf.log_request_handling) {
					TRACE("-- %s", "file not found");
					TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
				}

				buffer_reset(con->physical.path);
				return HANDLER_FINISHED;
			case ENOTDIR:
				/* PATH_INFO ! :) */
				break;
			case EMFILE:
				return HANDLER_WAIT_FOR_FD;
			default:
				/* we have no idea what happened, so tell the user. */
				con->http_status = 500;

				ERROR("checking file '%s' (%s) failed: %d (%s) -> sending status 500",
					SAFE_BUF_STR(con->uri.path),
					SAFE_BUF_STR(con->physical.path),
					errno, strerror(errno));

				buffer_reset(con->physical.path);

				return HANDLER_FINISHED;
			}

			/* not found, perhaps PATHINFO */

			buffer_copy_string_buffer(srv->tmp_buf, con->physical.path);

			do {
				if (slash) {
					buffer_copy_string_len(con->physical.path, srv->tmp_buf->ptr, slash - srv->tmp_buf->ptr);
				} else {
					buffer_copy_string_buffer(con->physical.path, srv->tmp_buf);
				}

				if (HANDLER_ERROR != stat_cache_get_entry(srv, con, con->physical.path, &sce)) {
					found = S_ISREG(sce->st.st_mode);
					break;
				}

				if (pathinfo != NULL) {
					*pathinfo = '\0';
				}
				slash = strrchr(srv->tmp_buf->ptr, '/');

				if (pathinfo != NULL) {
					/* restore '/' */
					*pathinfo = '/';
				}

				if (slash) pathinfo = slash;
			} while ((found == 0) && (slash != NULL) && ((size_t)(slash - srv->tmp_buf->ptr) > (con->physical.basedir->used - 2)));

			if (found == 0) {
				/* no, it really doesn't exists */
				con->http_status = 404;

				if (con->conf.log_file_not_found) {
					TRACE("file not found: %s -> %s", 
							SAFE_BUF_STR(con->uri.path),
							SAFE_BUF_STR(con->physical.path));
				}

				buffer_reset(con->physical.path);

				return HANDLER_FINISHED;
			}

#ifdef HAVE_LSTAT
			if ((sce->is_symlink != 0) && !con->conf.follow_symlink) {
				con->http_status = 403;

				if (con->conf.log_request_handling) {
					log_error_write(srv, __FILE__, __LINE__,  "s",  "-- access denied due symlink restriction");
					log_error_write(srv, __FILE__, __LINE__,  "sb", "Path         :", con->physical.path);
				}

				buffer_reset(con->physical.path);
				return HANDLER_FINISHED;
			};
#endif

			/* we have a PATHINFO */
			if (pathinfo) {
				buffer_copy_string(con->request.pathinfo, pathinfo);

				/*
				 * shorten uri.path
				 */

				con->uri.path->used -= strlen(pathinfo);
				con->uri.path->ptr[con->uri.path->used - 1] = '\0';
			}

			if (con->conf.log_request_handling) {
				TRACE("-- %s", "after pathinfo check");
				TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
				TRACE("URI          : %s", SAFE_BUF_STR(con->uri.path));
				TRACE("Pathinfo     : %s", SAFE_BUF_STR(con->request.pathinfo));
			}
			break;
		default:
			ERROR("stat_cache_get_entry_async() returned unexpected: %d", r);
			break;
		}

		config_patch_connection(srv, con, COMP_PHYSICAL_PATH_EXISTS);    /* physical-path  */

		if (con->conf.log_request_handling) {
			TRACE("-- %s", "handling subrequest");
			TRACE("Path         : %s", SAFE_BUF_STR(con->physical.path));
		}

		/* call the handlers */
		switch(r = plugins_call_handle_start_backend(srv, con)) {
		case HANDLER_GO_ON:
			break;
		case HANDLER_FINISHED:
		case HANDLER_COMEBACK:
		case HANDLER_WAIT_FOR_EVENT:
		case HANDLER_ERROR:
			return r;

		default:
			ERROR("plugins_call_handle_start_backend() returned unexpected: %d", r);
			return r;
		}

		if (con->conf.log_request_handling) {
			TRACE("-- %s", "subrequest finished");
		}
	}

	if (con->mode == DIRECT) {
		/* if we are still here, no one wanted the file; status 403 is ok I think */
		con->http_status = 403;

		if (con->conf.log_request_handling) {
			TRACE("%s", "aaaaaaah, sending 403");
		}

		return HANDLER_FINISHED;
	} else {
		return HANDLER_GO_ON;
	}
}



