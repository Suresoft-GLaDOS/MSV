#ifndef _HTTP_RESP_H_
#define _HTTP_RESP_H_

#include <stdio.h>

#include "buffer.h"
#include "array.h"
#include "array-static.h"
#include "chunk.h"
#include "http_parser.h"

typedef struct {
	int protocol;   /* http/1.0, http/1.1 */
	int status;     /* e.g. 200 */
	buffer *reason; /* e.g. Ok */
	array *headers;
} http_resp;

typedef struct {
	int     ok;
	buffer *errmsg;

	http_resp *resp;

	buffer_pool *unused_buffers;
} http_resp_ctx_t;

LI_API http_resp * http_response_init(void);
LI_API void http_response_free(http_resp *resp);
LI_API void http_response_reset(http_resp *resp);

LI_API parse_status_t http_response_parse_cq(chunkqueue *cq, http_resp *http_response);

/* declare prototypes for the parser */
void *http_resp_parserAlloc(void *(*mallocProc)(size_t));
void http_resp_parserFree(void *p,  void (*freeProc)(void*));
void http_resp_parserTrace(FILE *TraceFILE, char *zTracePrompt);
void http_resp_parser(void *, int, buffer *, http_resp_ctx_t *);

#endif
