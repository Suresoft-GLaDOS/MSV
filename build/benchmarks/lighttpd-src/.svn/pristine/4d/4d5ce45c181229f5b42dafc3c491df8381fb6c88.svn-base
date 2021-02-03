#ifndef _JOB_LIST_H_
#define _JOB_LIST_H_

#include "base.h"

LI_API void joblist_append(server *srv, connection *con);
LI_API void joblist_free(server *srv, connections *joblist);

#ifdef USE_GTHREAD
LI_API void joblist_async_append(server *srv, connection *con);

LI_API void server_wakeup(server *srv);
#endif

LI_API int fdwaitqueue_append(server *srv, connection *con);
LI_API void fdwaitqueue_free(server *srv, connections *fdwaitqueue);
LI_API connection* fdwaitqueue_unshift(server *srv, connections *fdwaitqueue);

#endif
