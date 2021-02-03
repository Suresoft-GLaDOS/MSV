#include "base.h"
#include "timing.h"

void timing_log(server *srv, connection *con, int field) {
#ifdef HAVE_GLIB_H
	if (srv->srvconf.log_timing) {
		g_get_current_time(&(con->timestamps[field]));
	}
#endif
}

