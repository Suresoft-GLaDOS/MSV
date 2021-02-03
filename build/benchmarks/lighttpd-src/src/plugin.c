#include <string.h>
#include <stdlib.h>

#include <stdio.h>

#include "plugin.h"
#include "log.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sys-files.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif
/*
 *
 * if you change this enum to add a new callback, be sure
 * - that PLUGIN_FUNC_SIZEOF is the last entry
 * - that you add PLUGIN_TO_SLOT twice:
 *   1. as callback-dispatcher
 *   2. in plugins_call_init()
 *
 */

typedef struct {
	PLUGIN_DATA;
} plugin_data;

typedef enum {
	PLUGIN_FUNC_UNSET,
		PLUGIN_FUNC_HANDLE_URI_CLEAN,
		PLUGIN_FUNC_HANDLE_URI_RAW,
		PLUGIN_FUNC_HANDLE_RESPONSE_DONE,
		PLUGIN_FUNC_HANDLE_CONNECTION_CLOSE,
		PLUGIN_FUNC_HANDLE_TRIGGER,
		PLUGIN_FUNC_HANDLE_SIGHUP,
		PLUGIN_FUNC_HANDLE_START_BACKEND,
		PLUGIN_FUNC_HANDLE_SEND_REQUEST_CONTENT,
		PLUGIN_FUNC_HANDLE_RESPONSE_HEADER,
		PLUGIN_FUNC_HANDLE_READ_RESPONSE_CONTENT,
		PLUGIN_FUNC_HANDLE_FILTER_RESPONSE_CONTENT,
		PLUGIN_FUNC_HANDLE_JOBLIST,
		PLUGIN_FUNC_HANDLE_DOCROOT,
		PLUGIN_FUNC_HANDLE_PHYSICAL,
		PLUGIN_FUNC_CONNECTION_RESET,
		PLUGIN_FUNC_INIT,
		PLUGIN_FUNC_CLEANUP,
		PLUGIN_FUNC_SET_DEFAULTS,

		PLUGIN_FUNC_SIZEOF
} plugin_t;

static plugin *plugin_init(void) {
	plugin *p;

	p = calloc(1, sizeof(*p));

	p->required_plugins = array_init();

	return p;
}

static void plugin_free(plugin *p) {
	int use_dlclose = 1;

	if (p->name) buffer_free(p->name);

	array_free(p->required_plugins);

	/* if we are running under valgrind, 
	 * don't unload the plugins to keep the symbols intact */
	if (RUNNING_ON_VALGRIND) use_dlclose = 0;

#ifndef LIGHTTPD_STATIC
	if (use_dlclose && p->lib) {
#ifdef _WIN32
		FreeLibrary(p->lib);
#else
		dlclose(p->lib);
#endif
	}
#endif

	free(p);
}

static int plugins_register(server *srv, plugin *p) {
	plugin **ps;
	if (0 == srv->plugins.size) {
		srv->plugins.size = 4;
		srv->plugins.ptr  = malloc(srv->plugins.size * sizeof(*ps));
		srv->plugins.used = 0;
	} else if (srv->plugins.used == srv->plugins.size) {
		srv->plugins.size += 4;
		srv->plugins.ptr   = realloc(srv->plugins.ptr, srv->plugins.size * sizeof(*ps));
	}

	ps = srv->plugins.ptr;
	ps[srv->plugins.used++] = p;

	return 0;
}

/**
 *
 *
 *
 */

#ifdef LIGHTTPD_STATIC

#define PLUGIN_STATIC(x) int x ## _plugin_init(plugin *p)

PLUGIN_STATIC(mod_access);
PLUGIN_STATIC(mod_accesslog);
PLUGIN_STATIC(mod_alias);
PLUGIN_STATIC(mod_auth);
PLUGIN_STATIC(mod_cgi);
PLUGIN_STATIC(mod_dirlisting);
PLUGIN_STATIC(mod_evasive);
PLUGIN_STATIC(mod_evhost);
PLUGIN_STATIC(mod_expire);
PLUGIN_STATIC(mod_deflate);
PLUGIN_STATIC(mod_compress);
PLUGIN_STATIC(mod_flv_streaming);
PLUGIN_STATIC(mod_chunked);
PLUGIN_STATIC(mod_indexfile);
PLUGIN_STATIC(mod_mysql_vhost);
PLUGIN_STATIC(mod_postgresql_vhost);
PLUGIN_STATIC(mod_proxy_backend_ajp13);
PLUGIN_STATIC(mod_proxy_backend_fastcgi);
PLUGIN_STATIC(mod_proxy_backend_http);
PLUGIN_STATIC(mod_proxy_backend_scgi);
PLUGIN_STATIC(mod_proxy_core);
PLUGIN_STATIC(mod_redirect);
PLUGIN_STATIC(mod_rewrite);
PLUGIN_STATIC(mod_secdownload);
PLUGIN_STATIC(mod_setenv);
PLUGIN_STATIC(mod_simple_vhost);
PLUGIN_STATIC(mod_sql_vhost_core);
PLUGIN_STATIC(mod_ssi);
PLUGIN_STATIC(mod_staticfile);
PLUGIN_STATIC(mod_status);
PLUGIN_STATIC(mod_trigger_b4_dl);
PLUGIN_STATIC(mod_uploadprogress);
PLUGIN_STATIC(mod_userdir);
PLUGIN_STATIC(mod_usertrack);
PLUGIN_STATIC(mod_webdav);
PLUGIN_STATIC(mod_magnet);

#undef PLUGIN_STATIC

#define PLUGIN_STATIC(x) { #x, x ## _plugin_init }

struct {
	const char *name;
	int (*init)(plugin *pl);
} const static_plugins[] = {
PLUGIN_STATIC(mod_access),
PLUGIN_STATIC(mod_accesslog),
PLUGIN_STATIC(mod_alias),
PLUGIN_STATIC(mod_auth),
PLUGIN_STATIC(mod_cgi),
PLUGIN_STATIC(mod_dirlisting),
PLUGIN_STATIC(mod_evasive),
PLUGIN_STATIC(mod_evhost),
PLUGIN_STATIC(mod_expire),
PLUGIN_STATIC(mod_deflate),
PLUGIN_STATIC(mod_compress),
PLUGIN_STATIC(mod_flv_streaming),
PLUGIN_STATIC(mod_chunked),
PLUGIN_STATIC(mod_indexfile),
PLUGIN_STATIC(mod_mysql_vhost),
PLUGIN_STATIC(mod_postgresql_vhost),
PLUGIN_STATIC(mod_proxy_backend_ajp13),
PLUGIN_STATIC(mod_proxy_backend_fastcgi),
PLUGIN_STATIC(mod_proxy_backend_http),
PLUGIN_STATIC(mod_proxy_backend_scgi),
PLUGIN_STATIC(mod_proxy_core),
PLUGIN_STATIC(mod_redirect),
PLUGIN_STATIC(mod_rewrite),
PLUGIN_STATIC(mod_secdownload),
PLUGIN_STATIC(mod_setenv),
PLUGIN_STATIC(mod_simple_vhost),
PLUGIN_STATIC(mod_sql_vhost_core),
PLUGIN_STATIC(mod_ssi),
PLUGIN_STATIC(mod_staticfile),
PLUGIN_STATIC(mod_status),
PLUGIN_STATIC(mod_trigger_b4_dl),
PLUGIN_STATIC(mod_uploadprogress),
PLUGIN_STATIC(mod_userdir),
PLUGIN_STATIC(mod_usertrack),
PLUGIN_STATIC(mod_webdav),
PLUGIN_STATIC(mod_magnet),

	{ NULL, NULL }
};

#undef PLUGIN_STATIC

#endif

int plugins_load(server *srv) {
	plugin *p;
	int (*init)(plugin *pl);

	const char *error;
	size_t i, j, k;

	for (i = 0; i < srv->srvconf.modules->used; i++) {
		data_string *d = (data_string *)srv->srvconf.modules->data[i];
		char *modules = d->value->ptr;

		p = plugin_init();

#ifdef LIGHTTPD_STATIC
		for (j = 0; static_plugins[j].name; j++) {
			if (0 == strcmp(BUF_STR(d->value), static_plugins[j].name)) {
				init = static_plugins[j].init;

				break;
			}
		}

		if (static_plugins[j].name == NULL) {
			ERROR("the plugin '%s' is not compiled in", SAFE_BUF_STR(d->value));
			return -1;
		}
#else
		buffer_copy_string_buffer(srv->tmp_buf, srv->srvconf.modules_dir);

		if (strlen(srv->srvconf.modules_dir->ptr) != 0) buffer_append_string(srv->tmp_buf, DIR_SEPERATOR_STR);
		buffer_append_string(srv->tmp_buf, modules);
#if defined(_WIN32) || defined(__CYGWIN__)
		buffer_append_string_len(srv->tmp_buf, CONST_STR_LEN(".dll"));
#else
		buffer_append_string_len(srv->tmp_buf, CONST_STR_LEN(".so"));
#endif

#ifdef _WIN32
		if (NULL == (p->lib = LoadLibrary(srv->tmp_buf->ptr))) {
			LPVOID lpMsgBuf;
			FormatMessage(
		        	FORMAT_MESSAGE_ALLOCATE_BUFFER |
		       		FORMAT_MESSAGE_FROM_SYSTEM,
		        	NULL,
		        	GetLastError(),
		        	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		        	(LPTSTR) &lpMsgBuf,
		        	0, NULL );

			log_error_write(srv, __FILE__, __LINE__, "ssb", "LoadLibrary() failed",
					lpMsgBuf, srv->tmp_buf);

			plugin_free(p);

			return -1;

		}
#else
		if (NULL == (p->lib = dlopen(srv->tmp_buf->ptr, RTLD_NOW|RTLD_GLOBAL))) {
			log_error_write(srv, __FILE__, __LINE__, "sbs", "dlopen() failed for:",
					srv->tmp_buf, dlerror());

			plugin_free(p);

			return -1;
		}

#endif
		buffer_reset(srv->tmp_buf);
		buffer_copy_string(srv->tmp_buf, modules);
		buffer_append_string_len(srv->tmp_buf, CONST_STR_LEN("_plugin_init"));

#ifdef _WIN32
		init = (int (*)(plugin *pl)) GetProcAddress(p->lib, srv->tmp_buf->ptr);

		if (init == NULL)  {
			LPVOID lpMsgBuf;
			FormatMessage(
		        	FORMAT_MESSAGE_ALLOCATE_BUFFER |
		       		FORMAT_MESSAGE_FROM_SYSTEM,
		        	NULL,
		        	GetLastError(),
		        	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		        	(LPTSTR) &lpMsgBuf,
		        	0, NULL );

			log_error_write(srv, __FILE__, __LINE__, "sbs", "getprocaddress failed:", srv->tmp_buf, lpMsgBuf);

			plugin_free(p);
			return -1;
		}

#else
#if 1
		init = (int (*)(plugin *))(intptr_t)dlsym(p->lib, srv->tmp_buf->ptr);
#else
		*(void **)(&init) = dlsym(p->lib, srv->tmp_buf->ptr);
#endif
		if ((error = dlerror()) != NULL)  {
			ERROR("dlsym(%s) failed: %s", SAFE_BUF_STR(srv->tmp_buf), error);

			plugin_free(p);
			return -1;
		}

#endif
#endif
		if ((*init)(p)) {
			ERROR("plugin-init failed for %s", SAFE_BUF_STR(d->value));

			plugin_free(p);
			return -1;
		}

		/* check if the required plugin is loaded */
		for (k = 0; k < p->required_plugins->used; k++) {
			data_string *req = (data_string *)p->required_plugins->data[k];

			for (j = 0; j < i; j++) {
				data_string *mod = (data_string *)srv->srvconf.modules->data[j];

				if (buffer_is_equal(req->value, mod->value)) break;
			}

			if (j == i) {
				/* not found */
				log_error_write(srv, __FILE__, __LINE__, "ssbs", modules, "failed to load. required plugin", req->value, "was not loaded" );

				plugin_free(p);

				return -1;
			}
		}
		plugins_register(srv, p);
	}

	return 0;
}

#define PLUGIN_TO_SLOT(x, y) \
	handler_t plugins_call_##y(server *srv, connection *con) {\
		plugin **slot;\
		size_t j;\
                if (!srv->plugin_slots) return HANDLER_GO_ON;\
                slot = ((plugin ***)(srv->plugin_slots))[x];\
		if (!slot) return HANDLER_GO_ON;\
		for (j = 0; j < srv->plugins.used && slot[j]; j++) { \
			plugin *p = slot[j];\
			handler_t r;\
			switch(r = p->y(srv, con, p->data)) {\
			case HANDLER_GO_ON:\
				break;\
			case HANDLER_FINISHED:\
			case HANDLER_COMEBACK:\
			case HANDLER_WAIT_FOR_EVENT:\
			case HANDLER_WAIT_FOR_FD:\
			case HANDLER_ERROR:\
				if (con->conf.log_request_handling) TRACE("-- plugins_call_...: plugin '%s' returns %d", SAFE_BUF_STR(p->name), r); \
				return r;\
			default:\
				ERROR("-- plugins_call_...: plugin '%s' returns %d (unexpected)", SAFE_BUF_STR(p->name), r); \
				return HANDLER_ERROR;\
			}\
		}\
		return HANDLER_GO_ON;\
	}

/**
 * plugins that use
 *
 * - server *srv
 * - connection *con
 * - void *p_d (plugin_data *)
 */

PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_URI_CLEAN, handle_uri_clean)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_URI_RAW, handle_uri_raw)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_DOCROOT, handle_docroot)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_PHYSICAL, handle_physical)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_START_BACKEND, handle_start_backend)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_SEND_REQUEST_CONTENT, handle_send_request_content)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_RESPONSE_HEADER, handle_response_header)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_READ_RESPONSE_CONTENT, handle_read_response_content)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_FILTER_RESPONSE_CONTENT, handle_filter_response_content)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_CONNECTION_CLOSE, handle_connection_close)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_RESPONSE_DONE, handle_response_done)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_JOBLIST, handle_joblist)
PLUGIN_TO_SLOT(PLUGIN_FUNC_CONNECTION_RESET, connection_reset)

#undef PLUGIN_TO_SLOT

#define PLUGIN_TO_SLOT(x, y) \
	handler_t plugins_call_##y(server *srv) {\
		plugin **slot;\
		size_t j;\
                if (!srv->plugin_slots) return HANDLER_GO_ON;\
                slot = ((plugin ***)(srv->plugin_slots))[x];\
		if (!slot) return HANDLER_GO_ON;\
		for (j = 0; j < srv->plugins.used && slot[j]; j++) { \
			plugin *p = slot[j];\
			handler_t r;\
			switch(r = p->y(srv, p->data)) {\
			case HANDLER_GO_ON:\
				break;\
			case HANDLER_FINISHED:\
			case HANDLER_COMEBACK:\
			case HANDLER_WAIT_FOR_EVENT:\
			case HANDLER_WAIT_FOR_FD:\
			case HANDLER_ERROR:\
				return r;\
			default:\
				log_error_write(srv, __FILE__, __LINE__, "sbsd", #x, p->name, "unknown state:", r);\
				return HANDLER_ERROR;\
			}\
		}\
		return HANDLER_GO_ON;\
	}

/**
 * plugins that use
 *
 * - server *srv
 * - void *p_d (plugin_data *)
 */

PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_TRIGGER, handle_trigger)
PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_SIGHUP, handle_sighup)
PLUGIN_TO_SLOT(PLUGIN_FUNC_CLEANUP, cleanup)
PLUGIN_TO_SLOT(PLUGIN_FUNC_SET_DEFAULTS, set_defaults)

#undef PLUGIN_TO_SLOT

#if 0
/**
 *
 * special handler
 *
 */
handler_t plugins_call_handle_fdevent(server *srv, const fd_conn *fdc) {
	size_t i;
	plugin **ps;

	ps = srv->plugins.ptr;

	for (i = 0; i < srv->plugins.used; i++) {
		plugin *p = ps[i];
		if (p->handle_fdevent) {
			handler_t r;
			switch(r = p->handle_fdevent(srv, fdc, p->data)) {
			case HANDLER_GO_ON:
				break;
			case HANDLER_FINISHED:
			case HANDLER_COMEBACK:
			case HANDLER_WAIT_FOR_EVENT:
			case HANDLER_ERROR:
				return r;
			default:
				log_error_write(srv, __FILE__, __LINE__, "d", r);
				break;
			}
		}
	}

	return HANDLER_GO_ON;
}
#endif
/**
 *
 * - call init function of all plugins to init the plugin-internals
 * - added each plugin that supports has callback to the corresponding slot
 *
 * - is only called once.
 */

handler_t plugins_call_init(server *srv) {
	size_t i;
	plugin **ps;

	ps = srv->plugins.ptr;

	/* fill slots */

	srv->plugin_slots = calloc(PLUGIN_FUNC_SIZEOF, sizeof(ps));

	for (i = 0; i < srv->plugins.used; i++) {
		size_t j;
		/* check which calls are supported */

		plugin *p = ps[i];

#define PLUGIN_TO_SLOT(x, y) \
	if (p->y) { \
		plugin **slot = ((plugin ***)(srv->plugin_slots))[x]; \
		if (!slot) { \
			slot = calloc(srv->plugins.used, sizeof(*slot));\
			((plugin ***)(srv->plugin_slots))[x] = slot; \
		} \
		for (j = 0; j < srv->plugins.used; j++) { \
			if (slot[j]) continue;\
			slot[j] = p;\
			break;\
		}\
	}


		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_URI_CLEAN, handle_uri_clean);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_URI_RAW, handle_uri_raw);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_DOCROOT, handle_docroot);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_PHYSICAL, handle_physical);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_START_BACKEND, handle_start_backend);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_SEND_REQUEST_CONTENT, handle_send_request_content);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_RESPONSE_HEADER, handle_response_header);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_READ_RESPONSE_CONTENT, handle_read_response_content);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_FILTER_RESPONSE_CONTENT, handle_filter_response_content)
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_RESPONSE_DONE, handle_response_done);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_CONNECTION_CLOSE, handle_connection_close);

		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_JOBLIST, handle_joblist);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_CONNECTION_RESET, connection_reset);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_CLEANUP, cleanup);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_SET_DEFAULTS, set_defaults);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_TRIGGER, handle_trigger);
		PLUGIN_TO_SLOT(PLUGIN_FUNC_HANDLE_SIGHUP, handle_sighup);
#undef PLUGIN_TO_SLOT

		if (p->init) {
			if (NULL == (p->data = p->init(srv))) {
				log_error_write(srv, __FILE__, __LINE__, "sb",
						"plugin-init failed for module", p->name);
				return HANDLER_ERROR;
			}

			/* used for con->mode, DIRECT == 0, plugins above that */
			((plugin_data *)(p->data))->id = i + 1;

			if (p->version != LIGHTTPD_VERSION_ID) {
				log_error_write(srv, __FILE__, __LINE__, "sb",
						"plugin-version doesn't match lighttpd-version for", p->name);
				return HANDLER_ERROR;
			}
		} else {
			p->data = NULL;
		}
	}

	return HANDLER_GO_ON;
}

/**
 * get the config-storage of the named plugin
 */
void *plugin_get_config(server *srv, const char *name) {
	size_t i;

	for (i = 0; i < srv->plugins.used; i++) {
		plugin *p = ((plugin **)srv->plugins.ptr)[i];

		if (buffer_is_equal_string(p->name, name, strlen(name))) {
			return p->data;
		}
	}

	return NULL;
}

void plugins_free(server *srv) {
	size_t i;
	plugins_call_cleanup(srv);

	for (i = 0; i < srv->plugins.used; i++) {
		plugin *p = ((plugin **)srv->plugins.ptr)[i];

		plugin_free(p);
	}

	for (i = 0; srv->plugin_slots && i < PLUGIN_FUNC_SIZEOF; i++) {
		plugin **slot = ((plugin ***)(srv->plugin_slots))[i];

		if (slot) free(slot);
	}

	free(srv->plugin_slots);
	srv->plugin_slots = NULL;

	free(srv->plugins.ptr);
	srv->plugins.ptr = NULL;
	srv->plugins.used = 0;
}
