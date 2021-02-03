#ifndef _MOD_SQL_VHOST_CORE_H_
#define _MOD_SQL_VHOST_CORE_H_

#include "buffer.h"
#include "plugin.h"

#ifdef HAVE_GLIB_H
#include <glib.h>
#endif

#define SQLVHOST_BACKEND_GETVHOST_PARAMS \
	(server *srv, connection *con, void *p_d, buffer *docroot, buffer *host)

#define SQLVHOST_BACKEND_GETVHOST_RETVAL handler_t

#define SQLVHOST_BACKEND_GETVHOST(name) \
	SQLVHOST_BACKEND_GETVHOST_RETVAL name SQLVHOST_BACKEND_GETVHOST_PARAMS

#define SQLVHOST_BACKEND_GETVHOST_PTR(name) \
	SQLVHOST_BACKEND_GETVHOST_RETVAL (* name)SQLVHOST_BACKEND_GETVHOST_PARAMS

typedef struct {
	buffer  *db;
	buffer  *user;
	buffer  *pass;
	buffer  *sock;

	buffer  *hostname;
	unsigned short port;

	buffer  *backend;
	void *backend_data;

	buffer *select_vhost;
	
	unsigned short cache_ttl;
	unsigned short debug;

#ifdef HAVE_GLIB_H
	GHashTable *vhost_table;
#endif

	SQLVHOST_BACKEND_GETVHOST_PTR(get_vhost);
} mod_sql_vhost_core_plugin_config;

/* global plugin data */
typedef struct {
	PLUGIN_DATA;

	buffer 	*docroot;
	buffer 	*host;

	mod_sql_vhost_core_plugin_config **config_storage;

	mod_sql_vhost_core_plugin_config conf;
} mod_sql_vhost_core_plugin_data;



#endif
