#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include <tap.h>

#include "http_req_range.h"
#include "log.h"

int main(void) {
	http_req_range *r, *ranges = http_request_range_init();
	buffer *b = buffer_init();

	log_init();
	plan_tests(7);

	buffer_copy_string_len(b, CONST_STR_LEN("bytes=0-0"));
	ok(PARSE_SUCCESS == http_request_range_parse(b, ranges), "0-0");
	for (r = ranges; r; r = r->next) {
		diag(".. %jd - %jd", (intmax_t) r->start, (intmax_t) r->end);
	}
	http_request_range_reset(ranges);

	buffer_copy_string_len(b, CONST_STR_LEN("bytes=1-2,3-4"));
	ok(PARSE_SUCCESS == http_request_range_parse(b, ranges), "1-2,3-4");
	for (r = ranges; r; r = r->next) {
		diag(".. %jd - %jd", (intmax_t) r->start, (intmax_t) r->end);
	}
	http_request_range_reset(ranges);

	buffer_copy_string_len(b, CONST_STR_LEN("bytes=-0"));
	ok(PARSE_SUCCESS == http_request_range_parse(b, ranges), "-0");
	for (r = ranges; r; r = r->next) {
		diag(".. %jd - %jd", (intmax_t) r->start, (intmax_t) r->end);
	}
	http_request_range_reset(ranges);

	buffer_copy_string_len(b, CONST_STR_LEN("bytes=0-"));
	ok(PARSE_SUCCESS == http_request_range_parse(b, ranges), "0-");
	for (r = ranges; r; r = r->next) {
		diag(".. %jd - %jd", (intmax_t) r->start, (intmax_t) r->end);
	}
	http_request_range_reset(ranges);

	buffer_copy_string_len(b, CONST_STR_LEN("bytes=0-0,0-"));
	ok(PARSE_SUCCESS == http_request_range_parse(b, ranges), "0-0,0-");
	for (r = ranges; r; r = r->next) {
		diag(".. %jd - %jd", (intmax_t) r->start, (intmax_t) r->end);
	}
	http_request_range_reset(ranges);

	buffer_copy_string_len(b, CONST_STR_LEN("bytes=0-0,-0"));
	ok(PARSE_SUCCESS == http_request_range_parse(b, ranges), "0-0,-0");
	for (r = ranges; r; r = r->next) {
		diag(".. %jd - %jd", (intmax_t) r->start, (intmax_t) r->end);
	}
	http_request_range_reset(ranges);

	buffer_copy_string_len(b, CONST_STR_LEN("bytes=1-2,3-4,5-"));
	ok(PARSE_SUCCESS == http_request_range_parse(b, ranges), "1-2,3-4,5-");
	for (r = ranges; r; r = r->next) {
		diag(".. %jd - %jd", (intmax_t) r->start, (intmax_t) r->end);
	}

	http_request_range_free(ranges);

	buffer_free(b);
	log_free();

	return exit_status();
}
