#include <stdlib.h>

#include "iosocket.h"
#include "sys-socket.h"
#include "sys-files.h"
#include "array-static.h"

iosocket *iosocket_init(void) {
	STRUCT_INIT(iosocket, sock);

	sock->fde_ndx = -1;
	sock->fd = -1;

	sock->type = IOSOCKET_TYPE_SOCKET;

#if defined USE_OPENSSL && ! defined OPENSSL_NO_TLSEXT
	sock->tlsext_server_name = buffer_init();
#endif

	return sock;
}

void iosocket_free(iosocket *sock) {
	if (!sock) return;

	if (sock->fd != -1) {
		switch (sock->type) {
		case IOSOCKET_TYPE_SOCKET:
			closesocket(sock->fd);
			break;
		case IOSOCKET_TYPE_PIPE:
			close(sock->fd);
			break;
		default:
			break;
		}
	}

#if defined USE_OPENSSL && ! defined OPENSSL_NO_TLSEXT
	buffer_free(sock->tlsext_server_name);
#endif

	free(sock);
}
