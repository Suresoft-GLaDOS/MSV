#define __COMPILE_0
#define __COMPILE_1
#define __COMPILE_2
#define __COMPILE_3
#define __COMPILE_4
#define __COMPILE_5
#define __COMPILE_6
#define __COMPILE_7
#define __COMPILE_8
#define __COMPILE_9
#define __COMPILE_10
#define __COMPILE_11
#define __COMPILE_12
#define __COMPILE_13
#define __COMPILE_14
#define __COMPILE_15
#define __COMPILE_16
#define __COMPILE_17
#define __COMPILE_18
#define __COMPILE_19
#define __COMPILE_20
#define __COMPILE_21
#define __COMPILE_22
#define __COMPILE_23
#define __COMPILE_24
#define __COMPILE_25
#define __COMPILE_26
#define __COMPILE_27
#define __COMPILE_28
#define __COMPILE_29
#define __COMPILE_30
#define __COMPILE_31
#define __COMPILE_32
#define __COMPILE_33
#define __COMPILE_34
#define __COMPILE_35
#define __COMPILE_36
#define __COMPILE_37
#define __COMPILE_38
#define __COMPILE_39
#define __COMPILE_40
#define __COMPILE_41
#define __COMPILE_42
#define __COMPILE_43
#define __COMPILE_44
#define __COMPILE_45
#define __COMPILE_46
#define __COMPILE_47
#define __COMPILE_49
#define __COMPILE_52
#define __COMPILE_53
#define __COMPILE_55
#define __COMPILE_56
#define __COMPILE_57
#define __COMPILE_58
#define __COMPILE_59
#define __COMPILE_60
#define __COMPILE_61
#define __COMPILE_62
#define __COMPILE_63
#define __COMPILE_64
#define __COMPILE_65
#define __COMPILE_66
#define __COMPILE_67
#define __COMPILE_68
#define __COMPILE_69
#define __COMPILE_70
#define __COMPILE_71
#define __COMPILE_72
#define __COMPILE_73
#define __COMPILE_74
#define __COMPILE_75
#define __COMPILE_76
#define __COMPILE_77
#define __COMPILE_78
#define __COMPILE_79
#define __COMPILE_80
#define __COMPILE_81
#define __COMPILE_82
#define __COMPILE_83
#define __COMPILE_84
#define __COMPILE_85
#define __COMPILE_86
#define __COMPILE_87
#define __COMPILE_88
#define __COMPILE_89
#define __COMPILE_99
#define __COMPILE_100
#define __COMPILE_101
#define __COMPILE_102
#define __COMPILE_103
#define __COMPILE_104
#define __COMPILE_105
#define __COMPILE_106
#define __COMPILE_107
#define __COMPILE_108
#define __COMPILE_109
#define __COMPILE_110
#define __COMPILE_112
#define __COMPILE_113
#define __COMPILE_114
#define __COMPILE_115
#define __COMPILE_116
#define __COMPILE_117
#define __COMPILE_118
#define __COMPILE_119
#define __COMPILE_120
#define __COMPILE_122
#define __COMPILE_123
#define __COMPILE_124
#define __COMPILE_125
#define __COMPILE_126
#define __COMPILE_127
#define __COMPILE_128
#define __COMPILE_129
#define __COMPILE_130
#define __COMPILE_131
#define __COMPILE_132
#define __COMPILE_133
#define __COMPILE_134
#define __COMPILE_135
#define __COMPILE_136
#define __COMPILE_137
#define __COMPILE_138
#define __COMPILE_139
#define __COMPILE_140
#define __COMPILE_141
#define __COMPILE_142
#define __COMPILE_143
#define __COMPILE_144
#define __COMPILE_145
#define __COMPILE_147
#define __COMPILE_149
#define __COMPILE_150
#define __COMPILE_151
#define __COMPILE_152
#define __COMPILE_153
#define __COMPILE_154
#define __COMPILE_155
#define __COMPILE_156
#define __COMPILE_157
#define __COMPILE_158
#define __COMPILE_159
#define __COMPILE_160
#define __COMPILE_161
#define __COMPILE_162
#define __COMPILE_163
#define __COMPILE_164
#define __COMPILE_165
#define __COMPILE_166
#define __COMPILE_167
#define __COMPILE_168
#define __COMPILE_169
#define __COMPILE_170
#define __COMPILE_171
#define __COMPILE_172
#define __COMPILE_173
#define __COMPILE_174
#define __COMPILE_175
#define __COMPILE_176
#define __COMPILE_177
#define __COMPILE_178
#define __COMPILE_179
#define __COMPILE_180
#define __COMPILE_181
#define __COMPILE_182
#define __COMPILE_183
#define __COMPILE_184
#define __COMPILE_185
// compile_fin
int __get_mutant(); int __is_neg(const char *location,char *lid,int count, ...); int __abst_hole(); char *__stat_write_init(const char *func_name);void __write_stat(char *str,const char *var_name,void *var_addr,int size); void __stat_file_close(const char *func_name,char *str);long long __mutate(const long long,const char *,const char *); int __trident_choice(char* lid, char* typestr,int* rvals, char** rvals_ids, int rvals_size,int** lvals, char** lvals_ids, int lvals_size);int __trident_output(char* id, char* typestr, int value);float fabs_trident(float a);int __choose(const char *);void *__var_select(unsigned int var_count,void *vars[]);void *__var_select_2(unsigned int var_count,void *vars[]);long long __const_select(unsigned int const_count, ...);
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "log.h"
#include "http_resp.h"
#include "http_resp_parser.h"

/* declare prototypes for the parser */
void *http_resp_parserAlloc(void *(*mallocProc)(size_t));
void http_resp_parserFree(void *p,  void (*freeProc)(void*));
void http_resp_parserTrace(FILE *TraceFILE, char *zTracePrompt);
void http_resp_parser(void *, int, buffer *, http_resp_ctx_t *);

typedef struct {
	chunkqueue *cq;

	chunk *c; /* current chunk in the chunkqueue */
	size_t offset; /* current offset in current chunk */

	chunk *lookup_c;
	size_t lookup_offset;

	int is_key;
	int is_statusline;
} http_resp_tokenizer_t;

http_resp *http_response_init(void) {
	http_resp *resp = calloc(1, sizeof(*resp));

	resp->reason = buffer_init();
	resp->headers = array_init();
	resp->status = -1;

	return resp;
}

void http_response_reset(http_resp *resp) {
	if (!resp) return;

	buffer_reset(resp->reason);
	array_reset(resp->headers);
	resp->status = -1;

}

void http_response_free(http_resp *resp) {
	if (!resp) return;

	buffer_free(resp->reason);
	array_free(resp->headers);

	free(resp);
}

static int http_resp_get_next_char(http_resp_tokenizer_t *t, unsigned char *c) {
	if (t->offset == t->c->mem->used - 1) {
		/* end of chunk, open next chunk */

		if (!t->c->next) return -1;

		t->c = t->c->next;
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

static int http_resp_lookup_next_char(http_resp_tokenizer_t *t, unsigned char *c) {
	if (t->lookup_offset == t->lookup_c->mem->used - 1) {
		/* end of chunk, open next chunk */

		if (!t->lookup_c->next) return -1;

		t->lookup_c = t->lookup_c->next;
		t->lookup_offset = 0;
	}

	*c = t->lookup_c->mem->ptr[t->lookup_offset++];
#if 0
	fprintf(stderr, "%s.%d: lookup: %c (%d) at offset: %d\r\n", __FILE__, __LINE__, *c > 31 ? *c : ' ', *c, t->lookup_offset - 1);
#endif

	return 0;
}


static int http_resp_tokenizer(
	http_resp_tokenizer_t *t,
	int *token_id,
	buffer *token
) {
	unsigned char c;
	int tid = 0;

	/* push the token to the parser */

	while (tid == 0 && 0 == http_resp_get_next_char(t, &c)) {
		switch (c) {
		case ':':
			tid = TK_COLON;

			t->is_key = 0;

			break;
		case ' ':
		case '\t':
			/* ignore WS */

			break;
		case '\r':
			if (0 != http_resp_lookup_next_char(t, &c)) return -1;

			if (c == '\n') {
				tid = TK_CRLF;

				t->c = t->lookup_c;
				t->offset = t->lookup_offset;

				t->is_statusline = 0;
				t->is_key = 1;
			} else {
				fprintf(stderr, "%s.%d: CR with out LF\r\n", __FILE__, __LINE__);
				return -1;
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
					if (c == ':') { t->is_statusline = 0; break; } /* this is not a status line by a real header */
					if (c == 32) break; /* the space is a splitter in the statusline */
				} else {
					if (t->is_key) {
						if (c == ':') break; /* the : is the splitter between key and value */
					}
				}
				if (0 != http_resp_lookup_next_char(t, &c)) return -1;
			}

			if (t->c == t->lookup_c &&
				t->offset == t->lookup_offset + 1) {

				fprintf(stderr, "%s.%d: invalid char in string\n", __FILE__, __LINE__);
				return -1;
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

		return 1;
	}

	return -1;
}

parse_status_t http_response_parse_cq(chunkqueue *cq, http_resp *resp) {
	http_resp_tokenizer_t t;
	void *pParser = NULL;
	int token_id = 0;
	buffer *token = NULL;
	http_resp_ctx_t context;
	parse_status_t ret = PARSE_UNSET;
	int last_token_id = 0;

	if(!cq->first) return PARSE_NEED_MORE;
	t.cq = cq;
	t.c = cq->first;
	t.offset = t.c->offset;
	t.is_key = 0;
	t.is_statusline = 1;

	context.ok = 1;
	context.errmsg = buffer_init();
	context.resp = resp;
	context.unused_buffers = buffer_pool_init();

	array_reset(resp->headers);

	pParser = http_resp_parserAlloc( malloc );
	token = buffer_pool_get(context.unused_buffers);
#if 0
	http_resp_parserTrace(stderr, "http-response: ");
#endif

	while((1 == http_resp_tokenizer(&t, &token_id, token)) && context.ok) {
		http_resp_parser(pParser, token_id, token, &context);

		token = buffer_pool_get(context.unused_buffers);

		/* CRLF CRLF ... the header end sequence */
		if (last_token_id == TK_CRLF &&
		    token_id == TK_CRLF) break;

		last_token_id = token_id;
	}

	/* oops, the parser failed */
	if (context.ok == 0) {
		ret = PARSE_ERROR;

		if (!buffer_is_empty(context.errmsg)) {
			TRACE("parsing failed: %s", SAFE_BUF_STR(context.errmsg));
		} else {
			TRACE("%s", "parsing failed ...");
		}
	}

	http_resp_parser(pParser, 0, token, &context);
	http_resp_parserFree(pParser, free);

	if (!buffer_is_empty(context.errmsg)) {
		TRACE("parsing failed: %s", SAFE_BUF_STR(context.errmsg));
	}
	if (context.ok == 0) {
		/* we are missing the some tokens */

		if (!buffer_is_empty(context.errmsg)) {
			TRACE("parsing failed: %s", SAFE_BUF_STR(context.errmsg));
		}

		//prophet generated patch
		{
		int __choose0 = __choose("__SWITCH0");
		if (__choose0 == 0)
		{}
		#ifdef __COMPILE_0
		else if (__choose0 == 1)
		{
		//AddIfStmtKind
		if (!buffer_is_empty(context.errmsg)) {
		    log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 248, context.errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		} else {
		    log_trace("%s.%d: (trace) %s", remove_path("src/http_resp.c"), 250, "parsing failed ...");
		}
		}
		#endif
		#ifdef __COMPILE_1
		else if (__choose0 == 2)
		{
		//AddIfStmtKind
		if (!buffer_is_empty(context.errmsg)) {
		    log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 258, context.errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		}
		#endif
		#ifdef __COMPILE_2
		else if (__choose0 == 3)
		{
		//AddIfStmtKind
		if (!buffer_is_empty(context.errmsg)) {
		    log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 264, context.errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		}
		#endif
		#ifdef __COMPILE_3
		else if (__choose0 == 4)
		{
		//AddIfStmtKind
		if (!cq->first)
		    return PARSE_NEED_MORE;
		}
		#endif
		#ifdef __COMPILE_4
		else if (__choose0 == 5)
		{
		//AddIfStmtKind
		if (ret == PARSE_UNSET) {
		    ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : PARSE_ERROR;
		}
		}
		#endif
		#ifdef __COMPILE_5
		else if (__choose0 == 6)
		{
		//AddInitKind
		memset(&context, 0, sizeof (*(&context)));
		}
		#endif
		#ifdef __COMPILE_6
		else if (__choose0 == 7)
		{
		//AddInitKind
		memset(&context.errmsg, 0, sizeof (*(&context.errmsg)));
		}
		#endif
		#ifdef __COMPILE_7
		else if (__choose0 == 8)
		{
		//AddInitKind
		memset(&context.ok, 0, sizeof (*(&context.ok)));
		}
		#endif
		#ifdef __COMPILE_8
		else if (__choose0 == 9)
		{
		//AddInitKind
		memset(&context.resp, 0, sizeof (*(&context.resp)));
		}
		#endif
		#ifdef __COMPILE_9
		else if (__choose0 == 10)
		{
		//AddInitKind
		memset(&context.unused_buffers, 0, sizeof (*(&context.unused_buffers)));
		}
		#endif
		#ifdef __COMPILE_10
		else if (__choose0 == 11)
		{
		//AddInitKind
		memset(&cq->bytes_in, 0, sizeof (*(&cq->bytes_in)));
		}
		#endif
		#ifdef __COMPILE_11
		else if (__choose0 == 12)
		{
		//AddInitKind
		memset(&cq->bytes_out, 0, sizeof (*(&cq->bytes_out)));
		}
		#endif
		#ifdef __COMPILE_12
		else if (__choose0 == 13)
		{
		//AddInitKind
		memset(&cq->first, 0, sizeof (*(&cq->first)));
		}
		#endif
		#ifdef __COMPILE_13
		else if (__choose0 == 14)
		{
		//AddInitKind
		memset(&cq->is_closed, 0, sizeof (*(&cq->is_closed)));
		}
		#endif
		#ifdef __COMPILE_14
		else if (__choose0 == 15)
		{
		//AddInitKind
		memset(&cq->last, 0, sizeof (*(&cq->last)));
		}
		#endif
		#ifdef __COMPILE_15
		else if (__choose0 == 16)
		{
		//AddInitKind
		memset(&cq->tempdirs, 0, sizeof (*(&cq->tempdirs)));
		}
		#endif
		#ifdef __COMPILE_16
		else if (__choose0 == 17)
		{
		//AddInitKind
		memset(&resp->headers, 0, sizeof (*(&resp->headers)));
		}
		#endif
		#ifdef __COMPILE_17
		else if (__choose0 == 18)
		{
		//AddInitKind
		memset(&resp->protocol, 0, sizeof (*(&resp->protocol)));
		}
		#endif
		#ifdef __COMPILE_18
		else if (__choose0 == 19)
		{
		//AddInitKind
		memset(&resp->reason, 0, sizeof (*(&resp->reason)));
		}
		#endif
		#ifdef __COMPILE_19
		else if (__choose0 == 20)
		{
		//AddInitKind
		memset(&resp->status, 0, sizeof (*(&resp->status)));
		}
		#endif
		#ifdef __COMPILE_20
		else if (__choose0 == 21)
		{
		//AddInitKind
		memset(&t.c, 0, sizeof (*(&t.c)));
		}
		#endif
		#ifdef __COMPILE_21
		else if (__choose0 == 22)
		{
		//AddInitKind
		memset(&t.c->async, 0, sizeof (*(&t.c->async)));
		}
		#endif
		#ifdef __COMPILE_22
		else if (__choose0 == 23)
		{
		//AddInitKind
		memset(&t.c->file, 0, sizeof (*(&t.c->file)));
		}
		#endif
		#ifdef __COMPILE_23
		else if (__choose0 == 24)
		{
		//AddInitKind
		memset(&t.c->mem, 0, sizeof (*(&t.c->mem)));
		}
		#endif
		#ifdef __COMPILE_24
		else if (__choose0 == 25)
		{
		//AddInitKind
		memset(&t.c->next, 0, sizeof (*(&t.c->next)));
		}
		#endif
		#ifdef __COMPILE_25
		else if (__choose0 == 26)
		{
		//AddInitKind
		memset(&t.c->offset, 0, sizeof (*(&t.c->offset)));
		}
		#endif
		#ifdef __COMPILE_26
		else if (__choose0 == 27)
		{
		//AddInitKind
		memset(&t.c->type, 0, sizeof (*(&t.c->type)));
		}
		#endif
		#ifdef __COMPILE_27
		else if (__choose0 == 28)
		{
		//AddInitKind
		memset(&t.cq, 0, sizeof (*(&t.cq)));
		}
		#endif
		#ifdef __COMPILE_28
		else if (__choose0 == 29)
		{
		//AddInitKind
		memset(&t.is_key, 0, sizeof (*(&t.is_key)));
		}
		#endif
		#ifdef __COMPILE_29
		else if (__choose0 == 30)
		{
		//AddInitKind
		memset(&t.is_statusline, 0, sizeof (*(&t.is_statusline)));
		}
		#endif
		#ifdef __COMPILE_30
		else if (__choose0 == 31)
		{
		//AddInitKind
		memset(&t.lookup_c, 0, sizeof (*(&t.lookup_c)));
		}
		#endif
		#ifdef __COMPILE_31
		else if (__choose0 == 32)
		{
		//AddInitKind
		memset(&t.lookup_offset, 0, sizeof (*(&t.lookup_offset)));
		}
		#endif
		#ifdef __COMPILE_32
		else if (__choose0 == 33)
		{
		//AddInitKind
		memset(&t.offset, 0, sizeof (*(&t.offset)));
		}
		#endif
		#ifdef __COMPILE_33
		else if (__choose0 == 34)
		{
		//AddInitKind
		memset(context.errmsg, 0, sizeof (*(context.errmsg)));
		}
		#endif
		#ifdef __COMPILE_34
		else if (__choose0 == 35)
		{
		//AddInitKind
		memset(context.resp, 0, sizeof (*(context.resp)));
		}
		#endif
		#ifdef __COMPILE_35
		else if (__choose0 == 36)
		{
		//AddInitKind
		memset(context.unused_buffers, 0, sizeof (*(context.unused_buffers)));
		}
		#endif
		#ifdef __COMPILE_36
		else if (__choose0 == 37)
		{
		//AddInitKind
		memset(cq->first, 0, sizeof (*(cq->first)));
		}
		#endif
		#ifdef __COMPILE_37
		else if (__choose0 == 38)
		{
		//AddInitKind
		memset(cq->last, 0, sizeof (*(cq->last)));
		}
		#endif
		#ifdef __COMPILE_38
		else if (__choose0 == 39)
		{
		//AddInitKind
		memset(cq->tempdirs, 0, sizeof (*(cq->tempdirs)));
		}
		#endif
		#ifdef __COMPILE_39
		else if (__choose0 == 40)
		{
		//AddInitKind
		memset(resp->headers, 0, sizeof (*(resp->headers)));
		}
		#endif
		#ifdef __COMPILE_40
		else if (__choose0 == 41)
		{
		//AddInitKind
		memset(resp->reason, 0, sizeof (*(resp->reason)));
		}
		#endif
		#ifdef __COMPILE_41
		else if (__choose0 == 42)
		{
		//AddInitKind
		memset(t.c, 0, sizeof (*(t.c)));
		}
		#endif
		#ifdef __COMPILE_42
		else if (__choose0 == 43)
		{
		//AddInitKind
		memset(t.c->mem, 0, sizeof (*(t.c->mem)));
		}
		#endif
		#ifdef __COMPILE_43
		else if (__choose0 == 44)
		{
		//AddInitKind
		memset(t.c->next, 0, sizeof (*(t.c->next)));
		}
		#endif
		#ifdef __COMPILE_44
		else if (__choose0 == 45)
		{
		//AddInitKind
		memset(t.cq, 0, sizeof (*(t.cq)));
		}
		#endif
		#ifdef __COMPILE_45
		else if (__choose0 == 46)
		{
		//AddInitKind
		memset(t.lookup_c, 0, sizeof (*(t.lookup_c)));
		}
		#endif
		#ifdef __COMPILE_46
		else if (__choose0 == 47)
		{
		//AddStmtAndReplaceAtomKind
		array_free((resp)->headers);
		}
		#endif
		#ifdef __COMPILE_47
		else if (__choose0 == 48)
		{
		//AddStmtAndReplaceAtomKind
		array_reset((resp)->headers);
		}
		#endif
		#ifdef __COMPILE_48
		else if (__choose0 == 49)
		{
		//AddStmtAndReplaceAtomKind
		array_reset(resp && !(1 << 5)->headers);
		}
		#endif
		#ifdef __COMPILE_49
		else if (__choose0 == 50)
		{
		//AddStmtAndReplaceAtomKind
		buffer_free((resp)->reason);
		}
		#endif
		#ifdef __COMPILE_50
		else if (__choose0 == 51)
		{
		//AddStmtAndReplaceAtomKind
		buffer_free(context && !(1 << 5).errmsg);
		}
		#endif
		#ifdef __COMPILE_51
		else if (__choose0 == 52)
		{
		//AddStmtAndReplaceAtomKind
		buffer_pool_append(context && !(1 << 5).unused_buffers, token);
		}
		#endif
		#ifdef __COMPILE_52
		else if (__choose0 == 53)
		{
		//AddStmtAndReplaceAtomKind
		buffer_pool_append(context.unused_buffers, (token));
		}
		#endif
		#ifdef __COMPILE_53
		else if (__choose0 == 54)
		{
		//AddStmtAndReplaceAtomKind
		buffer_pool_append(context.unused_buffers, token && !(1 << 5));
		}
		#endif
		#ifdef __COMPILE_54
		else if (__choose0 == 55)
		{
		//AddStmtAndReplaceAtomKind
		buffer_pool_free(context && !(1 << 5).unused_buffers);
		}
		#endif
		#ifdef __COMPILE_55
		else if (__choose0 == 56)
		{
		//AddStmtAndReplaceAtomKind
		buffer_reset((resp)->reason);
		}
		#endif
		#ifdef __COMPILE_56
		else if (__choose0 == 57)
		{
		//AddStmtAndReplaceAtomKind
		context.errmsg = (token);
		}
		#endif
		#ifdef __COMPILE_57
		else if (__choose0 == 58)
		{
		//AddStmtAndReplaceAtomKind
		context.ok = (0);
		}
		#endif
		#ifdef __COMPILE_58
		else if (__choose0 == 59)
		{
		//AddStmtAndReplaceAtomKind
		context.ok = (1);
		}
		#endif
		#ifdef __COMPILE_59
		else if (__choose0 == 60)
		{
		//AddStmtAndReplaceAtomKind
		context.ok = (248);
		}
		#endif
		#ifdef __COMPILE_60
		else if (__choose0 == 61)
		{
		//AddStmtAndReplaceAtomKind
		context.ok = (250);
		}
		#endif
		#ifdef __COMPILE_61
		else if (__choose0 == 62)
		{
		//AddStmtAndReplaceAtomKind
		context.ok = (258);
		}
		#endif
		#ifdef __COMPILE_62
		else if (__choose0 == 63)
		{
		//AddStmtAndReplaceAtomKind
		context.ok = (264);
		}
		#endif
		#ifdef __COMPILE_63
		else if (__choose0 == 64)
		{
		//AddStmtAndReplaceAtomKind
		context.ok = (last_token_id);
		}
		#endif
		#ifdef __COMPILE_64
		else if (__choose0 == 65)
		{
		//AddStmtAndReplaceAtomKind
		context.ok = (token_id);
		}
		#endif
		#ifdef __COMPILE_65
		else if (__choose0 == 66)
		{
		//AddStmtAndReplaceAtomKind
		context.resp = (resp);
		}
		#endif
		#ifdef __COMPILE_66
		else if (__choose0 == 67)
		{
		//AddStmtAndReplaceAtomKind
		context.resp = resp && !(1 << 5);
		}
		#endif
		#ifdef __COMPILE_67
		else if (__choose0 == 68)
		{
		//AddStmtAndReplaceAtomKind
		free((resp));
		}
		#endif
		#ifdef __COMPILE_68
		else if (__choose0 == 69)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser((pParser), 0, token, &context);
		}
		#endif
		#ifdef __COMPILE_69
		else if (__choose0 == 70)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser((pParser), token_id, token, &context);
		}
		#endif
		#ifdef __COMPILE_70
		else if (__choose0 == 71)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser && !(1 << 5), 0, token, &context);
		}
		#endif
		#ifdef __COMPILE_71
		else if (__choose0 == 72)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser && !(1 << 5), token_id, token, &context);
		}
		#endif
		#ifdef __COMPILE_72
		else if (__choose0 == 73)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser, 0, (token), &context);
		}
		#endif
		#ifdef __COMPILE_73
		else if (__choose0 == 74)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser, 0, token && !(1 << 5), &context);
		}
		#endif
		#ifdef __COMPILE_74
		else if (__choose0 == 75)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser, 0, token, &context && !(1 << 5));
		}
		#endif
		#ifdef __COMPILE_75
		else if (__choose0 == 76)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser, token_id && !(1 << 5), token, &context);
		}
		#endif
		#ifdef __COMPILE_76
		else if (__choose0 == 77)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser, token_id, (token), &context);
		}
		#endif
		#ifdef __COMPILE_77
		else if (__choose0 == 78)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser, token_id, token && !(1 << 5), &context);
		}
		#endif
		#ifdef __COMPILE_78
		else if (__choose0 == 79)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parser(pParser, token_id, token, &context && !(1 << 5));
		}
		#endif
		#ifdef __COMPILE_79
		else if (__choose0 == 80)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parserFree((pParser), free);
		}
		#endif
		#ifdef __COMPILE_80
		else if (__choose0 == 81)
		{
		//AddStmtAndReplaceAtomKind
		http_resp_parserFree(pParser && !(1 << 5), free);
		}
		#endif
		#ifdef __COMPILE_81
		else if (__choose0 == 82)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = (0);
		}
		#endif
		#ifdef __COMPILE_82
		else if (__choose0 == 83)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = (1);
		}
		#endif
		#ifdef __COMPILE_83
		else if (__choose0 == 84)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = (248);
		}
		#endif
		#ifdef __COMPILE_84
		else if (__choose0 == 85)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = (250);
		}
		#endif
		#ifdef __COMPILE_85
		else if (__choose0 == 86)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = (258);
		}
		#endif
		#ifdef __COMPILE_86
		else if (__choose0 == 87)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = (264);
		}
		#endif
		#ifdef __COMPILE_87
		else if (__choose0 == 88)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = (last_token_id);
		}
		#endif
		#ifdef __COMPILE_88
		else if (__choose0 == 89)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = (token_id);
		}
		#endif
		#ifdef __COMPILE_89
		else if (__choose0 == 90)
		{
		//AddStmtAndReplaceAtomKind
		last_token_id = token_id && !(1 << 5);
		}
		#endif
		#ifdef __COMPILE_90
		else if (__choose0 == 91)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 248, context && !(1 << 5).errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_91
		else if (__choose0 == 92)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 248, context.errmsg && context && !(1 << 5).errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_92
		else if (__choose0 == 93)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 248, context.errmsg && context.errmsg->ptr ? context && !(1 << 5).errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_93
		else if (__choose0 == 94)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 258, context && !(1 << 5).errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_94
		else if (__choose0 == 95)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 258, context.errmsg && context && !(1 << 5).errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_95
		else if (__choose0 == 96)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 258, context.errmsg && context.errmsg->ptr ? context && !(1 << 5).errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_96
		else if (__choose0 == 97)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 264, context && !(1 << 5).errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_97
		else if (__choose0 == 98)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 264, context.errmsg && context && !(1 << 5).errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_98
		else if (__choose0 == 99)
		{
		//AddStmtAndReplaceAtomKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 264, context.errmsg && context.errmsg->ptr ? context && !(1 << 5).errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_99
		else if (__choose0 == 100)
		{
		//AddStmtAndReplaceAtomKind
		pParser = (pParser);
		}
		#endif
		#ifdef __COMPILE_100
		else if (__choose0 == 101)
		{
		//AddStmtAndReplaceAtomKind
		ret = (0);
		}
		#endif
		#ifdef __COMPILE_101
		else if (__choose0 == 102)
		{
		//AddStmtAndReplaceAtomKind
		ret = (1);
		}
		#endif
		#ifdef __COMPILE_102
		else if (__choose0 == 103)
		{
		//AddStmtAndReplaceAtomKind
		ret = (248);
		}
		#endif
		#ifdef __COMPILE_103
		else if (__choose0 == 104)
		{
		//AddStmtAndReplaceAtomKind
		ret = (250);
		}
		#endif
		#ifdef __COMPILE_104
		else if (__choose0 == 105)
		{
		//AddStmtAndReplaceAtomKind
		ret = (258);
		}
		#endif
		#ifdef __COMPILE_105
		else if (__choose0 == 106)
		{
		//AddStmtAndReplaceAtomKind
		ret = (264);
		}
		#endif
		#ifdef __COMPILE_106
		else if (__choose0 == 107)
		{
		//AddStmtAndReplaceAtomKind
		ret = (PARSE_ERROR);
		}
		#endif
		#ifdef __COMPILE_107
		else if (__choose0 == 108)
		{
		//AddStmtAndReplaceAtomKind
		ret = (PARSE_NEED_MORE);
		}
		#endif
		#ifdef __COMPILE_108
		else if (__choose0 == 109)
		{
		//AddStmtAndReplaceAtomKind
		ret = (PARSE_SUCCESS);
		}
		#endif
		#ifdef __COMPILE_109
		else if (__choose0 == 110)
		{
		//AddStmtAndReplaceAtomKind
		ret = (PARSE_UNSET);
		}
		#endif
		#ifdef __COMPILE_110
		else if (__choose0 == 111)
		{
		//AddStmtAndReplaceAtomKind
		ret = (ret);
		}
		#endif
		#ifdef __COMPILE_111
		else if (__choose0 == 112)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context && !(1 << 5).errmsg) ? PARSE_NEED_MORE : PARSE_ERROR;
		}
		#endif
		#ifdef __COMPILE_112
		else if (__choose0 == 113)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context.errmsg) ? (PARSE_ERROR) : PARSE_ERROR;
		}
		#endif
		#ifdef __COMPILE_113
		else if (__choose0 == 114)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context.errmsg) ? (PARSE_NEED_MORE) : PARSE_ERROR;
		}
		#endif
		#ifdef __COMPILE_114
		else if (__choose0 == 115)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context.errmsg) ? (PARSE_SUCCESS) : PARSE_ERROR;
		}
		#endif
		#ifdef __COMPILE_115
		else if (__choose0 == 116)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context.errmsg) ? (PARSE_UNSET) : PARSE_ERROR;
		}
		#endif
		#ifdef __COMPILE_116
		else if (__choose0 == 117)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : (PARSE_ERROR);
		}
		#endif
		#ifdef __COMPILE_117
		else if (__choose0 == 118)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : (PARSE_NEED_MORE);
		}
		#endif
		#ifdef __COMPILE_118
		else if (__choose0 == 119)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : (PARSE_SUCCESS);
		}
		#endif
		#ifdef __COMPILE_119
		else if (__choose0 == 120)
		{
		//AddStmtAndReplaceAtomKind
		ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : (PARSE_UNSET);
		}
		#endif
		#ifdef __COMPILE_120
		else if (__choose0 == 121)
		{
		//AddStmtAndReplaceAtomKind
		t.c = (cq)->first;
		}
		#endif
		#ifdef __COMPILE_121
		else if (__choose0 == 122)
		{
		//AddStmtAndReplaceAtomKind
		t.c = cq && !(1 << 5)->first;
		}
		#endif
		#ifdef __COMPILE_122
		else if (__choose0 == 123)
		{
		//AddStmtAndReplaceAtomKind
		t.cq = (cq);
		}
		#endif
		#ifdef __COMPILE_123
		else if (__choose0 == 124)
		{
		//AddStmtAndReplaceAtomKind
		t.cq = cq && !(1 << 5);
		}
		#endif
		#ifdef __COMPILE_124
		else if (__choose0 == 125)
		{
		//AddStmtAndReplaceAtomKind
		t.is_key = (0);
		}
		#endif
		#ifdef __COMPILE_125
		else if (__choose0 == 126)
		{
		//AddStmtAndReplaceAtomKind
		t.is_key = (1);
		}
		#endif
		#ifdef __COMPILE_126
		else if (__choose0 == 127)
		{
		//AddStmtAndReplaceAtomKind
		t.is_key = (248);
		}
		#endif
		#ifdef __COMPILE_127
		else if (__choose0 == 128)
		{
		//AddStmtAndReplaceAtomKind
		t.is_key = (250);
		}
		#endif
		#ifdef __COMPILE_128
		else if (__choose0 == 129)
		{
		//AddStmtAndReplaceAtomKind
		t.is_key = (258);
		}
		#endif
		#ifdef __COMPILE_129
		else if (__choose0 == 130)
		{
		//AddStmtAndReplaceAtomKind
		t.is_key = (264);
		}
		#endif
		#ifdef __COMPILE_130
		else if (__choose0 == 131)
		{
		//AddStmtAndReplaceAtomKind
		t.is_key = (last_token_id);
		}
		#endif
		#ifdef __COMPILE_131
		else if (__choose0 == 132)
		{
		//AddStmtAndReplaceAtomKind
		t.is_key = (token_id);
		}
		#endif
		#ifdef __COMPILE_132
		else if (__choose0 == 133)
		{
		//AddStmtAndReplaceAtomKind
		t.is_statusline = (0);
		}
		#endif
		#ifdef __COMPILE_133
		else if (__choose0 == 134)
		{
		//AddStmtAndReplaceAtomKind
		t.is_statusline = (1);
		}
		#endif
		#ifdef __COMPILE_134
		else if (__choose0 == 135)
		{
		//AddStmtAndReplaceAtomKind
		t.is_statusline = (248);
		}
		#endif
		#ifdef __COMPILE_135
		else if (__choose0 == 136)
		{
		//AddStmtAndReplaceAtomKind
		t.is_statusline = (250);
		}
		#endif
		#ifdef __COMPILE_136
		else if (__choose0 == 137)
		{
		//AddStmtAndReplaceAtomKind
		t.is_statusline = (258);
		}
		#endif
		#ifdef __COMPILE_137
		else if (__choose0 == 138)
		{
		//AddStmtAndReplaceAtomKind
		t.is_statusline = (264);
		}
		#endif
		#ifdef __COMPILE_138
		else if (__choose0 == 139)
		{
		//AddStmtAndReplaceAtomKind
		t.is_statusline = (last_token_id);
		}
		#endif
		#ifdef __COMPILE_139
		else if (__choose0 == 140)
		{
		//AddStmtAndReplaceAtomKind
		t.is_statusline = (token_id);
		}
		#endif
		#ifdef __COMPILE_140
		else if (__choose0 == 141)
		{
		//AddStmtAndReplaceAtomKind
		t.offset = (0);
		}
		#endif
		#ifdef __COMPILE_141
		else if (__choose0 == 142)
		{
		//AddStmtAndReplaceAtomKind
		t.offset = (1);
		}
		#endif
		#ifdef __COMPILE_142
		else if (__choose0 == 143)
		{
		//AddStmtAndReplaceAtomKind
		t.offset = (248);
		}
		#endif
		#ifdef __COMPILE_143
		else if (__choose0 == 144)
		{
		//AddStmtAndReplaceAtomKind
		t.offset = (250);
		}
		#endif
		#ifdef __COMPILE_144
		else if (__choose0 == 145)
		{
		//AddStmtAndReplaceAtomKind
		t.offset = (258);
		}
		#endif
		#ifdef __COMPILE_145
		else if (__choose0 == 146)
		{
		//AddStmtAndReplaceAtomKind
		t.offset = (264);
		}
		#endif
		#ifdef __COMPILE_146
		else if (__choose0 == 147)
		{
		//AddStmtAndReplaceAtomKind
		t.offset = t && !(1 << 5).c->offset;
		}
		#endif
		#ifdef __COMPILE_147
		else if (__choose0 == 148)
		{
		//AddStmtAndReplaceAtomKind
		token = (token);
		}
		#endif
		#ifdef __COMPILE_148
		else if (__choose0 == 149)
		{
		//AddStmtAndReplaceAtomKind
		token = buffer_pool_get(context && !(1 << 5).unused_buffers);
		}
		#endif
		#ifdef __COMPILE_149
		else if (__choose0 == 150)
		{
		//AddStmtKind
		array_reset(resp->headers);
		}
		#endif
		#ifdef __COMPILE_150
		else if (__choose0 == 151)
		{
		//AddStmtKind
		buffer_free(context.errmsg);
		}
		#endif
		#ifdef __COMPILE_151
		else if (__choose0 == 152)
		{
		//AddStmtKind
		buffer_pool_append(context.unused_buffers, token);
		}
		#endif
		#ifdef __COMPILE_152
		else if (__choose0 == 153)
		{
		//AddStmtKind
		buffer_pool_free(context.unused_buffers);
		}
		#endif
		#ifdef __COMPILE_153
		else if (__choose0 == 154)
		{
		//AddStmtKind
		context.errmsg = buffer_init();
		}
		#endif
		#ifdef __COMPILE_154
		else if (__choose0 == 155)
		{
		//AddStmtKind
		context.ok = 1;
		}
		#endif
		#ifdef __COMPILE_155
		else if (__choose0 == 156)
		{
		//AddStmtKind
		context.resp = resp;
		}
		#endif
		#ifdef __COMPILE_156
		else if (__choose0 == 157)
		{
		//AddStmtKind
		context.unused_buffers = buffer_pool_init();
		}
		#endif
		#ifdef __COMPILE_157
		else if (__choose0 == 158)
		{
		//AddStmtKind
		http_resp_parser(pParser, 0, token, &context);
		}
		#endif
		#ifdef __COMPILE_158
		else if (__choose0 == 159)
		{
		//AddStmtKind
		http_resp_parser(pParser, token_id, token, &context);
		}
		#endif
		#ifdef __COMPILE_159
		else if (__choose0 == 160)
		{
		//AddStmtKind
		http_resp_parserFree(pParser, free);
		}
		#endif
		#ifdef __COMPILE_160
		else if (__choose0 == 161)
		{
		//AddStmtKind
		last_token_id = token_id;
		}
		#endif
		#ifdef __COMPILE_161
		else if (__choose0 == 162)
		{
		//AddStmtKind
		log_trace("%s.%d: (trace) %s", remove_path("src/http_resp.c"), 250, "parsing failed ...");
		}
		#endif
		#ifdef __COMPILE_162
		else if (__choose0 == 163)
		{
		//AddStmtKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 248, context.errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_163
		else if (__choose0 == 164)
		{
		//AddStmtKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 258, context.errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_164
		else if (__choose0 == 165)
		{
		//AddStmtKind
		log_trace("%s.%d: (trace) parsing failed: %s", remove_path("src/http_resp.c"), 264, context.errmsg && context.errmsg->ptr ? context.errmsg->ptr : "(null)");
		}
		#endif
		#ifdef __COMPILE_165
		else if (__choose0 == 166)
		{
		//AddStmtKind
		pParser = http_resp_parserAlloc(malloc);
		}
		#endif
		#ifdef __COMPILE_166
		else if (__choose0 == 167)
		{
		//AddStmtKind
		ret = PARSE_ERROR;
		}
		#endif
		#ifdef __COMPILE_167
		else if (__choose0 == 168)
		{
		//AddStmtKind
		ret = PARSE_SUCCESS;
		}
		#endif
		#ifdef __COMPILE_168
		else if (__choose0 == 169)
		{
		//AddStmtKind
		ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : PARSE_ERROR;
		}
		#endif
		#ifdef __COMPILE_169
		else if (__choose0 == 170)
		{
		//AddStmtKind
		t.c = cq->first;
		}
		#endif
		#ifdef __COMPILE_170
		else if (__choose0 == 171)
		{
		//AddStmtKind
		t.cq = cq;
		}
		#endif
		#ifdef __COMPILE_171
		else if (__choose0 == 172)
		{
		//AddStmtKind
		t.is_key = 0;
		}
		#endif
		#ifdef __COMPILE_172
		else if (__choose0 == 173)
		{
		//AddStmtKind
		t.is_statusline = 1;
		}
		#endif
		#ifdef __COMPILE_173
		else if (__choose0 == 174)
		{
		//AddStmtKind
		t.offset = t.c->offset;
		}
		#endif
		#ifdef __COMPILE_174
		else if (__choose0 == 175)
		{
		//AddStmtKind
		token = buffer_pool_get(context.unused_buffers);
		}
		#endif
		#ifdef __COMPILE_175
		else if (__choose0 == 176)
		{
		//IfExitKind
		if (__is_neg("0-176", "L267", 11, &(ret), sizeof (ret), &(context.errmsg),
					 sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser),
					 sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id),
					 sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp),
					 sizeof (context.resp)))
		    return 0;
		}
		#endif
		#ifdef __COMPILE_176
		else if (__choose0 == 177)
		{
		//IfExitKind
		if (__is_neg("0-177", "L267", 11, &(ret), sizeof (ret), &(context.errmsg),
					 sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser),
					 sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id),
					 sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp),
					 sizeof (context.resp)))
		    return 1;
		}
		#endif
		#ifdef __COMPILE_177
		else if (__choose0 == 178)
		{
		//IfExitKind
		if (__is_neg("0-178", "L267", 11, &(ret), sizeof (ret), &(context.errmsg),
					 sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser),
					 sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id),
					 sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp),
					 sizeof (context.resp)))
		    return 248;
		}
		#endif
		#ifdef __COMPILE_178
		else if (__choose0 == 179)
		{
		//IfExitKind
		if (__is_neg("0-179", "L267", 11, &(ret), sizeof (ret), &(context.errmsg),
					 sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser),
					 sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id),
					 sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp),
					 sizeof (context.resp)))
		    return 250;
		}
		#endif
		#ifdef __COMPILE_179
		else if (__choose0 == 180)
		{
		//IfExitKind
		if (__is_neg("0-180", "L267", 11, &(ret), sizeof (ret), &(context.errmsg),
					 sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser),
					 sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id),
					 sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp),
					 sizeof (context.resp)))
		    return 258;
		}
		#endif
		#ifdef __COMPILE_180
		else if (__choose0 == 181)
		{
		//IfExitKind
		if (__is_neg("0-181", "L267", 11, &(ret), sizeof (ret), &(context.errmsg),
					 sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser),
					 sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id),
					 sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp),
					 sizeof (context.resp)))
		    return 264;
		}
		#endif
		int __choose1 = __choose("__SWITCH1");
		{
		int __temp1=(ret == PARSE_UNSET) ;
		if (__choose1 == 0)
		{}
		#ifdef __COMPILE_181
		else if (__choose1 == 1)
		{
		__temp1= (__is_neg("1-1", "L267", 11, &(ret), sizeof (ret), &(context.errmsg), sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser), sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id), sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp), sizeof (context.resp)));
		}
		#endif
		#ifdef __COMPILE_182
		else if (__choose1 == 2)
		{
		__temp1= ((ret == PARSE_UNSET) || __is_neg("1-2", "L267", 11, &(ret), sizeof (ret), &(context.errmsg), sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser), sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id), sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp), sizeof (context.resp)));
		}
		#endif
		#ifdef __COMPILE_183
		else if (__choose1 == 3)
		{
		__temp1= ((ret == PARSE_UNSET) && !__is_neg("1-3", "L267", 11, &(ret), sizeof (ret), &(context.errmsg), sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser), sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id), sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp), sizeof (context.resp)));
		}
		#endif
		int __choose2 = __choose("__SWITCH2");
		if (__choose2 == 0)
		{
		if (__temp1) {
					ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : PARSE_ERROR;
				}
		
		}
		#ifdef __COMPILE_184
		else if (__choose2 == 1)
		{
		//GuardKind
		if (!__is_neg("2-1", "L267", 11, &(ret), sizeof (ret), &(context.errmsg), sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser), sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id), sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp), sizeof (context.resp)))
		    if (ret == PARSE_UNSET) {
		        ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : PARSE_ERROR;
		    }
		
		}
		#endif
		#ifdef __COMPILE_185
		else if (__choose2 == 2)
		{
		//SpecialGuardKind
		if (!__is_neg("2-2", "L267", 11, &(ret), sizeof (ret), &(context.errmsg), sizeof (context.errmsg), &(cq), sizeof (cq), &(context.ok), sizeof (context.ok), &(pParser), sizeof (pParser), &(token), sizeof (token), &(context.unused_buffers), sizeof (context.unused_buffers), &(token_id), sizeof (token_id), &(last_token_id), sizeof (last_token_id), &(resp), sizeof (resp), &(context.resp), sizeof (context.resp)) && (ret == PARSE_UNSET)) {
		    ret = buffer_is_empty(context.errmsg) ? PARSE_NEED_MORE : PARSE_ERROR;
		}
		
		}
		#endif
		}
		}
	} else {
		chunk *c;

		for (c = cq->first; c != t.c; c = c->next) {
			c->offset = c->mem->used - 1;
			cq->bytes_out += c->mem->used - 1;
		}

		c->offset = t.offset;
		cq->bytes_out += t.offset;

		ret = PARSE_SUCCESS;
	}

	buffer_pool_append(context.unused_buffers, token);
	buffer_pool_free(context.unused_buffers);
	buffer_free(context.errmsg);

	return ret;
}

