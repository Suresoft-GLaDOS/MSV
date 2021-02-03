/**
 * the network filter-API
 *
 *
 */

#include <stdlib.h>

#include <string.h>
#include <assert.h>

#include "filter.h"

/**
 * create the filter 
 *
 * each filter has a chunkqueue with content and 
 * a prev and a next pointer
 */
static filter *filter_init(void) {
	filter *fl;

	fl = calloc(1, sizeof(*fl));

	fl->next = NULL;
	fl->prev = NULL;

	fl->cq = chunkqueue_init();

	return fl;
}

/**
 * free the filter  
 */
static void filter_free(filter *fl) {
	filter *next, *prev;

	/* free chunkqueue */
	if(fl->cq) {
		chunkqueue_free(fl->cq);
	}
	
	/* remove this filter from chain. */
	next = fl->next;
	prev = fl->prev;

	if(next) {
		next->prev = prev;
	}
	if(prev) {
		prev->next = next;
	}

	free(fl);
}

/**
 * reset the filter  
 */
static void filter_reset(filter *fl) {
	/* reset chunkqueue */
	if(fl->cq) {
		chunkqueue_reset(fl->cq);
	} else {
		fl->cq = chunkqueue_init();
	}
}

/**
 * create a filter chain
 */
filter_chain *filter_chain_init(void) {
	filter_chain *chain;

	chain = calloc(1, sizeof(*chain));

	chain->first = NULL;
	chain->last = NULL;
	filter_chain_create_filter(chain, FILTER_ID_INPUT);

	return chain;
}

void filter_chain_free(filter_chain *chain) {
	filter *first;
	if (!chain) return;

	/* free all filters, after the first filter */
	first = chain->first;
	while(first->next) {
		filter_free(first->next);
	}
	/* free first filter */
	filter_free(first);

	free(chain);
}

void filter_chain_reset(filter_chain *chain) {
	filter *first;
	if (!chain) return;

	/* free all filters, except first filter */
	first = chain->first;
	while(first->next) {
		filter_free(first->next);
	}
	/* reset first filter */
	filter_reset(first);
	chain->last = first;
}

filter *filter_chain_create_filter(filter_chain *chain, int id) {
	filter *fl;
	if (!chain) return NULL;

	fl = filter_init();
	fl->id = id;
	/* add filter to end of chain. */
	if (chain->last != NULL) {
		chain->last->next = fl;
	}
	fl->prev = chain->last;
	chain->last = fl;
	if (chain->first == NULL) {
		chain->first = fl;
	}

	return fl;
}

filter *filter_chain_get_filter(filter_chain *chain, int id) {
	filter *fl;
	if (!chain) return NULL;

	/* find filter with id */
	fl = chain->first;
	while(fl) {
		if (fl->id == id) break;
		fl = fl->next;
	}

	return fl;
}

void filter_chain_remove_filter(filter_chain *chain, filter *fl) {
	if (!chain || !fl) return;

	if (chain->first == fl) {
		chain->first = fl->next;
	}
	if (chain->last == fl) {
		chain->last = fl->prev;
	}
	filter_free(fl);
}

/**
 * move all the content of the last filter to the chunk queue 
 */
off_t filter_chain_copy_output(filter_chain *chain, chunkqueue *out) {
	off_t total;
	chunkqueue *in;

	if (!chain || !out) return 0;
	if (out->is_closed) return 0;

	in = chain->last->cq;
	total = chunkqueue_steal_all_chunks(out, in);
	in->bytes_out += total;
	out->bytes_in += total;

	chunkqueue_remove_finished_chunks(in);

	if (in->is_closed) {
		out->is_closed = 1;
	}

	return total;
}

