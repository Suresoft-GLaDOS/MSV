#include "network_backends.h"

#ifdef USE_FREEBSD_SENDFILE

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

#include "network.h"
#include "fdevent.h"
#include "log.h"
#include "stat_cache.h"


#ifndef UIO_MAXIOV
# if defined(__FreeBSD__) || defined(__DragonFly__)
/* FreeBSD 4.7, 4.9 defined it in sys/uio.h only if _KERNEL is specified */
#  define UIO_MAXIOV 1024
# endif
#endif

NETWORK_BACKEND_WRITE(freebsdsendfile) {
	chunk *c;
	size_t chunks_written = 0;

	for(c = cq->first; c; c = c->next, chunks_written++) {
		chunk *tc;
		int chunk_finished = 0;
		network_status_t ret;

		switch(c->type) {
		case MEM_CHUNK:
			ret = network_write_chunkqueue_writev_mem(srv, con, sock, cq, c);

			/* check which chunks are finished now */
			for (tc = c; tc; tc = tc->next) {
				/* finished the chunk */
				if (tc->offset == tc->mem->used - 1) {
					/* skip the first c->next as that will be done by the c = c->next in the other for()-loop */
					if (chunk_finished) {
						c = c->next;
					} else {
						chunk_finished = 1;
					}
				} else {
					break;
				}
			}

			if (ret != NETWORK_STATUS_SUCCESS) {
				return ret;
			}

			break;
		case FILE_CHUNK: {
			off_t offset, r;
			size_t toSend;
			stat_cache_entry *sce = NULL;

			if (HANDLER_ERROR == stat_cache_get_entry(srv, con, c->file.name, &sce)) {
				log_error_write(srv, __FILE__, __LINE__, "sb",
						strerror(errno), c->file.name);
				return NETWORK_STATUS_FATAL_ERROR;
			}

			offset = c->file.start + c->offset;
			/* limit the toSend to 2^31-1 bytes in a chunk */
			toSend = c->file.length - c->offset > ((1 << 30) - 1) ?
				((1 << 30) - 1) : c->file.length - c->offset;

			if (-1 == c->file.fd) {
				if (-1 == (c->file.fd = open(c->file.name->ptr, O_RDONLY))) {
					switch (errno) {
					case EMFILE:
						return NETWORK_STATUS_WAIT_FOR_FD;
					default:
						ERROR("opening '%s' failed: %s", SAFE_BUF_STR(c->file.name), strerror(errno));

						return NETWORK_STATUS_FATAL_ERROR;
					}

					return NETWORK_STATUS_FATAL_ERROR;
				}
			}

			r = 0;

			/* FreeBSD sendfile() */
			if (-1 == sendfile(c->file.fd, sock->fd, offset, toSend, NULL, &r, 0)) {
				switch(errno) {
				case EAGAIN:
					break;
				case ENOTCONN:
					return NETWORK_STATUS_CONNECTION_CLOSE;
				default:
					log_error_write(srv, __FILE__, __LINE__, "ssd", "sendfile: ", strerror(errno), errno);
					return NETWORK_STATUS_FATAL_ERROR;
				}
			}

			if (r == 0) {
				/* We got an event to write but we wrote nothing
				 *
				 * - the file shrinked -> error
				 * - the remote side closed inbetween -> remote-close */

				if (HANDLER_ERROR == stat_cache_get_entry(srv, con, c->file.name, &sce)) {
					/* file is gone ? */
					return NETWORK_STATUS_FATAL_ERROR;
				}

				if (offset > sce->st.st_size) {
					/* file shrinked, close the connection */
					return NETWORK_STATUS_FATAL_ERROR;
				}

				return NETWORK_STATUS_CONNECTION_CLOSE;
			}

			c->offset += r;
			cq->bytes_out += r;

			if (c->offset == c->file.length) {
				chunk_finished = 1;
			}

			break;
		}
		default:
			ERROR("chunk-type '%d' not known", c->type);

			return NETWORK_STATUS_FATAL_ERROR;
		}

		if (!chunk_finished) {
			/* not finished yet */

			return NETWORK_STATUS_WAIT_FOR_EVENT;
		}
	}

	return NETWORK_STATUS_SUCCESS;
}

#endif
