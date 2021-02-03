/**
 * the network chunk-API
 *
 *
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <fcntl.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "chunk.h"

#include "sys-mmap.h"
#include "sys-files.h"

#include "log.h"

/**
 * create a global pool for unused chunks
 *
 * the chunk is moved from queue to queue (by stealing)
 * and moved back into the unused pool. 
 *
 * Instead of having a local pool of unused chunks per queue
 * we use a global pool
 *
 */

static chunk *chunkpool        = NULL;
static size_t chunkpool_chunks = 0;

chunkqueue *chunkqueue_init(void) {
	chunkqueue *cq;

	cq = calloc(1, sizeof(*cq));

	cq->first = NULL;
	cq->last = NULL;

	return cq;
}

static chunk *chunk_init(void) {
	chunk *c;

	c = calloc(1, sizeof(*c));

	c->mem = buffer_init();
	c->file.name = buffer_init();
	c->file.fd = -1;
	c->file.copy.fd = -1;
	c->file.mmap.start = MAP_FAILED;
	c->next = NULL;
	
	c->async.written = -1;

	return c;
}

static void chunk_reset(chunk *c) {
	if (!c) return;

	buffer_reset(c->mem);

	if (c->file.is_temp && !buffer_is_empty(c->file.name)) {
		unlink(c->file.name->ptr);
	}
	c->file.is_temp = 0;

	buffer_reset(c->file.name);

	if (c->file.fd != -1) {
		close(c->file.fd);
		c->file.fd = -1;
	}

	if (c->file.copy.fd != -1) {
		close(c->file.copy.fd);
		c->file.copy.fd = -1;
	}

	if (MAP_FAILED != c->file.mmap.start) {
		munmap(c->file.mmap.start, c->file.mmap.length);
		c->file.mmap.start = MAP_FAILED;
	}

	c->file.length = 0;
	c->file.start = 0;

	c->file.mmap.length = 0;
	c->file.mmap.offset = 0;

	c->file.copy.length = 0;
	c->file.copy.offset = 0;

	c->async.written = -1;
	c->async.ret_val = 0;

	c->offset = 0;
	c->next = NULL;
}

static void chunk_free(chunk *c) {
	if (!c) return;

	/* make sure fd's are closed and tempfile's are deleted. */
	chunk_reset(c);

	buffer_free(c->mem);
	buffer_free(c->file.name);

	free(c);
}

/**
 * mark the chunk as done 
 *
 * @param c chunk to set done
 * @return 1 if done, 0 if not
 */
void chunk_set_done(chunk *c) {
	switch (c->type) {
	case MEM_CHUNK:
		c->offset = c->mem->used - 1;

		break;
	case FILE_CHUNK:
		c->offset = c->file.length;

		break;
	default:
		break;
	}
}

/**
 * check if chunk is finished 
 *
 * @param c chunk to set done
 * @return 1 if done, 0 if not
 */
int chunk_is_done(chunk *c) {
	switch (c->type) {
	case MEM_CHUNK:
		return ((c->mem->used == 0) || (c->offset == (off_t)c->mem->used - 1));
	case FILE_CHUNK:
		return ((c->file.length == 0) || (c->offset == c->file.length));
	case UNUSED_CHUNK:
	default:
		return 1;
	}
}

off_t chunk_length(chunk *c) {
	switch (c->type) {
	case MEM_CHUNK:
		if (c->mem->used == 0) return 0;
		return (off_t)c->mem->used - 1 - c->offset;
	case FILE_CHUNK:
		return c->file.length - c->offset;
	case UNUSED_CHUNK:
		break;
	}
	return 0;
}

void chunkpool_free(void) {
	if (!chunkpool) return;

	/* free the pool */
	while(chunkpool) {
		chunk *c = chunkpool->next;
		chunk_free(chunkpool);
		chunkpool = c;
	}
	chunkpool_chunks = 0;
}

static chunk *chunkpool_get_unused_chunk(void) {
	chunk *c;
	
	/* check if we have an unused chunk */
	if (!chunkpool) {
		c = chunk_init();
	} else {
		/* take the first element from the list (a stack) */
		c = chunkpool;
		chunkpool = c->next;
		c->next = NULL;

		chunkpool_chunks--;
	}

	return c;
}

/**
 * keep unused chunks alive and store them in the chunkpool
 *
 * we only want to keep a small set of chunks alive to balance between
 * memory-usage and mallocs
 *
 * each filter will ask for a chunk
 */
static void chunkpool_add_unused_chunk(chunk *c) {
	if (chunkpool_chunks > 128) {
		chunk_free(c);
	} else {
		chunk_reset(c);
		
		/* prepend the chunk to the chunkpool */
		c->next = chunkpool;
		chunkpool = c;
		chunkpool_chunks++;
	}
}


void chunkqueue_free(chunkqueue *cq) {
	chunk *c, *pc;

	if (!cq) return;

	for (c = cq->first; c; ) {
		pc = c;
		c = c->next;
		chunk_free(pc);
	}

	free(cq);
}

static int chunkqueue_prepend_chunk(chunkqueue *cq, chunk *c) {
	c->next = cq->first;
	cq->first = c;

	if (cq->last == NULL) {
		cq->last = c;
	}

	return 0;
}

static int chunkqueue_append_chunk(chunkqueue *cq, chunk *c) {
	if (cq->last) {
		cq->last->next = c;
	}
	cq->last = c;

	if (cq->first == NULL) {
		cq->first = c;
	}

	return 0;
}

/**
 * reset all chunks of the queue
 */
void chunkqueue_reset(chunkqueue *cq) {
	chunk *c;

	/* mark all read done */
	for (c = cq->first; c; c = c->next) {
		chunk_set_done(c);
	}

	chunkqueue_remove_finished_chunks(cq);

	cq->bytes_in = 0;
	cq->bytes_out = 0;
	cq->is_closed = 0;
}

int chunkqueue_append_file(chunkqueue *cq, buffer *fn, off_t offset, off_t len) {
	chunk *c;

	if (len == 0) return 0;

	c = chunkpool_get_unused_chunk();

	c->type = FILE_CHUNK;

	buffer_copy_string_buffer(c->file.name, fn);
	c->file.start = offset;
	c->file.length = len;
	c->offset = 0;

	chunkqueue_append_chunk(cq, c);

	return 0;
}

int chunkqueue_steal_tempfile(chunkqueue *cq, chunk *in) {
	chunk *c;

	assert(in->type == FILE_CHUNK);
	assert(in->file.is_temp == 1);

	c = chunkpool_get_unused_chunk();

	c->type = FILE_CHUNK;
	buffer_copy_string_buffer(c->file.name, in->file.name);
	c->file.start = in->file.start + in->offset;
	c->file.length = in->file.length - in->offset;
	c->offset = 0;
	c->file.is_temp = 1;
	in->file.is_temp = 0;

	chunkqueue_append_chunk(cq, c);

	return 0;
}

/**
 * move the content of chunk to another chunkqueue. return total bytes copied/stolen.
 */
off_t chunkqueue_steal_chunk(chunkqueue *cq, chunk *c) {
	/* we are copying the whole buffer, just steal it */
	off_t total = 0;
	buffer *b, btmp;

	if (!cq) return 0;
	if (chunk_is_done(c)) return 0;

	switch (c->type) {
	case MEM_CHUNK:
		total = c->mem->used - c->offset - 1;
		if (c->offset == 0) {
			b = chunkqueue_get_append_buffer(cq);
			btmp = *b; *b = *(c->mem); *(c->mem) = btmp;
		} else {
			chunkqueue_append_mem(cq, c->mem->ptr + c->offset, total);
			chunk_set_done(c);
		}
		break;
	case FILE_CHUNK:
		total = c->file.length - c->offset;

		if (c->file.is_temp) {
			chunkqueue_steal_tempfile(cq, c);
		} else {
			chunkqueue_append_file(cq, c->file.name, c->file.start + c->offset, c->file.length - c->offset);
			chunk_set_done(c);
		}

		break;
	case UNUSED_CHUNK:
		return 0;
	}

	return total;
}

/*
 * copy/steal all chunks from in chunkqueue.  return total bytes copied/stolen.
 *
 */
off_t chunkqueue_steal_all_chunks(chunkqueue *cq, chunkqueue *in) {
	off_t total = 0;
	chunk *c;

	if (!cq || !in) return 0;

	for (c = in->first; c; c = c->next) {
		total += chunkqueue_steal_chunk(cq, c);
	}

	return total;
}

/*
 * copy/steal max_len bytes from chunk chain.  return total bytes copied/stolen.
 *
 */
off_t chunkqueue_steal_chunks_len(chunkqueue *out, chunk *c, off_t max_len) {
	off_t total = 0;
	off_t we_have = 0, we_want = 0;
	buffer *b;

	if (!out || !c) return 0;

	/* copy/steal chunks */
	for (; c && max_len > 0; c = c->next) {
		switch (c->type) {
		case FILE_CHUNK:
			we_have = c->file.length - c->offset;

			if (we_have == 0) break;

			if (we_have > max_len) we_have = max_len;

			chunkqueue_append_file(out, c->file.name, c->offset, we_have);

			c->offset += we_have;
			max_len -= we_have;
			total += we_have;

			/* steal the tempfile
			 *
			 * This is tricky:
			 * - we reference the tempfile from the in-queue several times
			 *   if the chunk is larger than max_len
			 * - we can't simply cleanup the in-queue as soon as possible
			 *   as it would remove the tempfiles
			 * - the idea is to 'steal' the tempfiles and attach the is_temp flag to the last
			 *   referencing chunk of the fastcgi-write-queue
			 *
			 */

			if (c->offset == c->file.length) {
				chunk *out_c;

				out_c = out->last;

				/* the last of the out-queue should be a FILE_CHUNK (we just created it)
				 * and the incoming side should have given use a temp-file-chunk */
				assert(out_c->type == FILE_CHUNK);
				assert(c->file.is_temp == 1);

				out_c->file.is_temp = 1;
				c->file.is_temp = 0;
			}

			break;
		case MEM_CHUNK:
			/* skip empty chunks */
			if (c->mem->used == 0) break;
	
			we_have = c->mem->used - c->offset - 1;
			if (we_have == 0) break;
	
			we_want = we_have < max_len ? we_have : max_len;
	
			if (we_have == we_want) {
				/* steal whole chunk */
				chunkqueue_steal_chunk(out, c);
			} else {
				/* copy unused data from chunk */
				b = chunkqueue_get_append_buffer(out);
				buffer_copy_string_len(b, c->mem->ptr + c->offset, we_want);
				c->offset += we_want;
			}
			total += we_want;
			max_len -= we_want;

			break;
		default:
			break;
		}
	}
	return total;
}

/**
 * skip bytes in the chunkqueue 
 *
 * @param cq chunkqueue
 * @param skip bytes to skip
 * @return bytes skipped
 */
off_t chunkqueue_skip(chunkqueue *cq, off_t skip) {
	off_t total = 0;
	off_t we_have = 0, we_want = 0;
	chunk *c;

	if (!cq) return 0;

	/* consume chunks */
	for (c = cq->first; c && skip > 0; c = c->next) {
		we_have = chunk_length(c);

		/* skip empty chunks */
		if (!we_have) continue;

		we_want = we_have < skip ? we_have : skip;

		c->offset += we_want;
		total += we_want;
		skip -= we_want;
	}

	return total;
}

int chunkqueue_append_buffer(chunkqueue *cq, buffer *mem) {
	chunk *c;

	if (mem->used == 0) return 0;

	c = chunkpool_get_unused_chunk();
	c->type = MEM_CHUNK;
	c->offset = 0;
	buffer_copy_string_buffer(c->mem, mem);

	chunkqueue_append_chunk(cq, c);

	return 0;
}

int chunkqueue_prepend_buffer(chunkqueue *cq, buffer *mem) {
	chunk *c;

	if (mem->used == 0) return 0;

	c = chunkpool_get_unused_chunk();
	c->type = MEM_CHUNK;
	c->offset = 0;
	buffer_copy_string_buffer(c->mem, mem);

	chunkqueue_prepend_chunk(cq, c);

	return 0;
}

int chunkqueue_append_mem(chunkqueue *cq, const char * mem, size_t len) {
	chunk *c;

	if (len == 0) return 0;

	c = chunkpool_get_unused_chunk();
	c->type = MEM_CHUNK;
	c->offset = 0;
	buffer_copy_string_len(c->mem, mem, len);

	chunkqueue_append_chunk(cq, c);

	return 0;
}

buffer * chunkqueue_get_prepend_buffer(chunkqueue *cq) {
	chunk *c;

	c = chunkpool_get_unused_chunk();

	c->type = MEM_CHUNK;
	c->offset = 0;
	buffer_reset(c->mem);

	chunkqueue_prepend_chunk(cq, c);

	return c->mem;
}

buffer *chunkqueue_get_append_buffer(chunkqueue *cq) {
	chunk *c;

	c = chunkpool_get_unused_chunk();

	c->type = MEM_CHUNK;
	c->offset = 0;
	buffer_reset(c->mem);

	chunkqueue_append_chunk(cq, c);

	return c->mem;
}

int chunkqueue_set_tempdirs(chunkqueue *cq, array *tempdirs) {
	if (!cq) return -1;

	cq->tempdirs = tempdirs;

	return 0;
}

chunk *chunkqueue_get_append_tempfile(chunkqueue *cq) {
	chunk *c;
	buffer *template = buffer_init_string("/var/tmp/lighttpd-upload-XXXXXX");

	c = chunkpool_get_unused_chunk();

	c->type = FILE_CHUNK;
	c->offset = 0;

	if (cq->tempdirs && cq->tempdirs->used) {
		size_t i;

		/* we have several tempdirs, only if all of them fail we jump out */

		for (i = 0; i < cq->tempdirs->used; i++) {
			data_string *ds = (data_string *)cq->tempdirs->data[i];

			buffer_copy_string_buffer(template, ds->value);
			PATHNAME_APPEND_SLASH(template);
			buffer_append_string_len(template, CONST_STR_LEN("lighttpd-upload-XXXXXX"));

			if (-1 != (c->file.fd = mkstemp(template->ptr))) {
				/* only trigger the unlink if we created the temp-file successfully */
				c->file.is_temp = 1;
				break;
			}
		}
	} else {
		if (-1 != (c->file.fd = mkstemp(template->ptr))) {
			/* only trigger the unlink if we created the temp-file successfully */
			c->file.is_temp = 1;
		}
	}

	buffer_copy_string_buffer(c->file.name, template);
	c->file.length = 0;

	chunkqueue_append_chunk(cq, c);

	buffer_free(template);

	return c;
}


off_t chunkqueue_length(chunkqueue *cq) {
	off_t len = 0;
	chunk *c;

	for (c = cq->first; c; c = c->next) {
		switch (c->type) {
		case MEM_CHUNK:
			len += c->mem->used ? c->mem->used - 1 : 0;
			break;
		case FILE_CHUNK:
			len += c->file.length;
			break;
		default:
			break;
		}
	}

	return len;
}

off_t chunkqueue_written(chunkqueue *cq) {
	off_t len = 0;
	chunk *c;

	for (c = cq->first; c; c = c->next) {
		switch (c->type) {
		case MEM_CHUNK:
		case FILE_CHUNK:
			len += c->offset;
			break;
		default:
			break;
		}
	}

	return len;
}

int chunkqueue_is_empty(chunkqueue *cq) {
	return cq->first ? 0 : 1;
}

int chunkqueue_remove_finished_chunks(chunkqueue *cq) {
	chunk *c;

	for (c = cq->first; c; c = cq->first) {
		if (!chunk_is_done(c)) break;

		/* the chunk is finished, remove it from the queue */
		cq->first = c->next;
		if (c == cq->last) cq->last = NULL;

		chunkpool_add_unused_chunk(c);

	}

	return 0;
}

void chunkqueue_print(chunkqueue *cq) {
	chunk *c;

	for (c = cq->first; c; c = c->next) {
		fprintf(stderr, "(mem) %s", c->mem->ptr + c->offset);
	}
	fprintf(stderr, "\r\n");
}


/**
 * remove the last chunk if it is empty
 */

void chunkqueue_remove_empty_last_chunk(chunkqueue *cq) {
	chunk *c;
	if (!cq->last) return;
	if (!cq->first) return;

	if (cq->last->type != MEM_CHUNK || cq->last->mem->used != 0) return;

	if (cq->first == cq->last) {
		c = cq->first;

		chunk_free(c);
		cq->first = cq->last = NULL;
	} else {
		for (c = cq->first; c->next; c = c->next) {
			if (c->next == cq->last) {
				cq->last = c;

				chunk_free(c->next);
				c->next = NULL;

				return;
			}
		}
	}
}


