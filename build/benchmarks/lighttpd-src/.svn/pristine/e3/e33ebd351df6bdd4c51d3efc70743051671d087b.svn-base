#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "log.h"
#include "http_req.h"
#include "http_req_parser.h"

typedef struct {
	chunkqueue *cq;

	chunk *c; /* current chunk in the chunkqueue */
	size_t offset; /* current offset in current chunk */

	chunk *lookup_c;
	size_t lookup_offset;

	int last_token_id;

	int is_key;
	int is_statusline;
} http_req_tokenizer_t;

http_req *http_request_init(void) {
	http_req *req = calloc(1, sizeof(*req));

	req->uri_raw = buffer_init();
	req->headers = array_init();

	return req;
}

void http_request_reset(http_req *req) {
	if (!req) return;

	buffer_reset(req->uri_raw);
	array_reset(req->headers);

}

void http_request_free(http_req *req) {
	if (!req) return;

	buffer_free(req->uri_raw);
	array_free(req->headers);

	free(req);
}

static int http_req_get_next_char(http_req_tokenizer_t *t, unsigned char *c) {
	if (t->c->mem->used == 0) {
		TRACE("chunk-len: %zd", t->c->mem->used);
	}

	if (t->offset == t->c->mem->used - 1) {
		/* end of chunk, open next chunk */

		if (!t->c->next) return -1;

		t->c = t->c->next;
		/* skip empty chunks */
		while (t->c && t->c->mem->used == 0) t->c = t->c->next;
		if (!t->c) return -1;

		t->offset = 0;
	}

	*c = t->c->mem->ptr[t->offset++];

	t->lookup_offset = t->offset;
	t->lookup_c = t->c;

#if 0
	fprintf(stderr, "%s.%d: get: %c (%d) at offset: %d\r\n", __FILE__, __LINE__, *c > 31 ? *c : ' ', *c, t->offset - 1);
#endif

	return 0;
}

static int http_req_lookup_next_char(http_req_tokenizer_t *t, unsigned char *c) {
	if (t->lookup_c->mem->used == 0) {
		TRACE("chunk-len: %zd", t->lookup_c->mem->used);
	}
	if (t->lookup_offset == t->lookup_c->mem->used - 1) {
		/* end of chunk, open next chunk */

		if (!t->lookup_c->next) return -1;

		t->lookup_c = t->lookup_c->next;

		/* skip empty chunks */
		while (t->lookup_c && t->lookup_c->mem->used == 0) t->lookup_c = t->lookup_c->next;
		if (!t->lookup_c) return -1;

		t->lookup_offset = 0;
	}

	*c = t->lookup_c->mem->ptr[t->lookup_offset++];
#if 0
	fprintf(stderr, "%s.%d: lookup: %c (%d) at offset: %d\r\n", __FILE__, __LINE__, *c > 31 ? *c : ' ', *c, t->lookup_offset - 1);
#endif

	return 0;
}

typedef enum {
	PARSER_UNSET,
	PARSER_OK,
	PARSER_ERROR,
	PARSER_EOF
} http_req_parser_t;

static http_req_parser_t http_req_tokenizer(
	http_req_tokenizer_t *t,
	int *token_id,
	buffer *token
) {
	unsigned char c;
	int tid = 0;

	/* push the token to the parser */

	while (tid == 0 && 0 == http_req_get_next_char(t, &c)) {
		switch (c) {
		case ':':
			tid = TK_COLON;

			t->is_key = 0;

			break;
		case ' ':
		case '\t':
			if (t->last_token_id == TK_CRLF) {
				/* WS as the start of a line */

				tid = TK_TAB;
				t->is_key = 0;
			}
			/* ignore the rest of the WS-chars */
			break;
		case '\r':
			if (0 != http_req_lookup_next_char(t, &c)) return PARSER_EOF;

			if (c == '\n') {
				tid = TK_CRLF;

				t->c = t->lookup_c;
				t->offset = t->lookup_offset;

				t->is_statusline = 0;
				t->is_key = 1;
			} else {
				ERROR("CR with out LF at pos: %zu", t->offset);
				return PARSER_ERROR;
			}
			break;
		case '\n':
			tid = TK_CRLF;

			t->is_statusline = 0;
			t->is_key = 1;

			break;
		default:
			while (c >= 32 && c != 127 && c != 255) {
				if (t->is_statusline) {
					if (c == 32) break; /* the space is a splitter in the statusline */
				} else {
					if (t->is_key) {
						if (c == ':') break; /* the : is the splitter between key and value */
						if (c == ' ') break; /* no spaces in keys */
					}
				}
				if (0 != http_req_lookup_next_char(t, &c)) return PARSER_EOF;
			}

			if (t->c == t->lookup_c &&
				t->offset == t->lookup_offset + 1) {

				ERROR("invalid char (%d) at pos: %zu", c, t->offset);
				return PARSER_ERROR;
			}

			tid = TK_STRING;

			/* the lookup points to the first invalid char */
			t->lookup_offset--;

			/* no overlapping string */
			if (t->c == t->lookup_c) {
				buffer_copy_string_len(token, t->c->mem->ptr + t->offset - 1, t->lookup_offset - t->offset + 1);
			} else {
				/* first chunk */
				buffer_copy_string_len(token, t->c->mem->ptr + t->offset - 1, t->c->mem->used - t->offset);

				/* chunks in the middle */
				for (t->c = t->c->next; t->c != t->lookup_c; t->c = t->c->next) {
					buffer_append_string_buffer(token, t->c->mem);
					t->offset = t->c->mem->used - 1;
				}

				/* last chunk */
				buffer_append_string_len(token, t->c->mem->ptr, t->lookup_offset);
			}

			t->offset = t->lookup_offset;

			break;
		}
	}

	if (tid) {
		*token_id = tid;

		return PARSER_OK;
	}

	return PARSER_EOF;
}

parse_status_t http_request_parse_cq(chunkqueue *cq, http_req *req) {
	http_req_tokenizer_t t;
	void *pParser = NULL;
	int token_id = 0;
	buffer *token = NULL;
	http_req_ctx_t context;
	parse_status_t ret = PARSE_UNSET;
	http_req_parser_t parser_ret;

	t.cq = cq;
	t.c = cq->first;
	t.offset = t.c->offset;
	t.is_key = 0;
	t.is_statusline = 1;
	t.last_token_id = 0;

	context.ok = 1;
	context.errmsg = buffer_init();
	context.req = req;
	context.unused_buffers = buffer_pool_init();

	pParser = http_req_parserAlloc( malloc );
	token = buffer_init();

	array_reset(req->headers);

	while((PARSER_OK == (parser_ret = http_req_tokenizer(&t, &token_id, token))) && context.ok) {
		http_req_parser(pParser, token_id, token, &context);

		token = buffer_pool_get(context.unused_buffers);

		/* CRLF CRLF ... the header end sequence */
		if (t.last_token_id == TK_CRLF &&
		    token_id == TK_CRLF) break;

		t.last_token_id = token_id;
	}

	// Tokenizer failed
	if (parser_ret == PARSER_ERROR) {
		ret = PARSE_ERROR;
	}

	/* oops, the parser failed */
	if (context.ok == 0) {
		ret = PARSE_ERROR;

		if (!buffer_is_empty(context.errmsg)) {
			TRACE("parsing failed: %s", SAFE_BUF_STR(context.errmsg));
		} else {
			chunk *c;
			buffer *hdr = buffer_init();
			
			for (c = cq->first; c; c = c->next) {
				if (c == cq->first) {
					buffer_append_string_len(hdr, c->mem->ptr + c->offset, c->mem->used - 1 - c->offset);
				} else {
					buffer_append_string_buffer(hdr, c->mem);
				}
			}

			TRACE("parsing failed at token (%s [%d]), header: %s", SAFE_BUF_STR(token), token_id, SAFE_BUF_STR(hdr));

			buffer_free(hdr);
		}
	}

	http_req_parser(pParser, 0, token, &context);
	http_req_parserFree(pParser, free);

	if (context.ok == 0) {
		/* we are missing the some tokens */

		if (!buffer_is_empty(context.errmsg)) {
			TRACE("parsing failed: %s", SAFE_BUF_STR(context.errmsg));
		}

		if (ret == PARSE_UNSET) {
			ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : PARSE_ERROR;
		}
	} else if (parser_ret == PARSER_EOF) { // didn't see CRLF CRLF, no other error till now
		ret = PARSE_NEED_MORE;
	} else {
		chunk *c;

		for (c = cq->first; c != t.c; c = c->next) {
			c->offset = c->mem->used - 1;
		}

		c->offset = t.offset;

		ret = PARSE_SUCCESS;
	}

	buffer_pool_append(context.unused_buffers, token);
	buffer_pool_free(context.unused_buffers);
	buffer_free(context.errmsg);

	return ret;
}


