#ifndef _HTTP_REQ_RANGE_H_
#define _HTTP_REQ_RANGE_H_

#include <stdio.h>

#include "array.h"
#include "chunk.h"
#include "http_parser.h"

typedef struct _http_req_range {
	off_t start;
	off_t end;
	struct _http_req_range *next;
} http_req_range;

typedef struct {
	int     ok;
	buffer *errmsg;

	http_req_range *ranges;

	buffer_pool *unused_buffers;
} http_req_range_ctx_t;

LI_API http_req_range * http_request_range_init(void);
LI_API void http_request_range_free(http_req_range *range);
LI_API void http_request_range_reset(http_req_range *range);

LI_API parse_status_t http_request_range_parse(buffer *range_hdr, http_req_range *ranges);

/* declare prototypes for the parser */
void *http_req_range_parserAlloc(void *(*mallocProc)(size_t));
void http_req_range_parserFree(void *p,  void (*freeProc)(void*));
void http_req_range_parserTrace(FILE *TraceFILE, char *zTracePrompt);
void http_req_range_parser(void *, int, buffer *, http_req_range_ctx_t *);

#endif
