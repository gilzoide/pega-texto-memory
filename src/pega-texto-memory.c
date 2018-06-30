/*
 * Copyright 2018 Gil Barbosa Reis <gilzoide@gmail.com>
 * This file is part of pega-texto-memory.
 *
 * Pega-texto-memory is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Pega-texto-memory is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this pega-texto-memory.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pega-texto-memory.h"

#include <Judy.h>

#include <string.h>

typedef struct pt_memory_tracker {
	/// Table of already visited addresses (Expressions, Grammars, arrays and strings)
	// Address -> memory in bytes
	void *address_table;
	/// Aggregated memory usage statistics
	pt_memory memory_usage;
} pt_memory_tracker;

pt_memory pt_memory_for_expr(pt_expr *expr) {
	pt_memory_tracker memtracker;
	pt_memory_tracker_reset(&memtracker);
	pt_memory_track_expr(&memtracker, expr);
	pt_memory res = pt_memory_tracker_current_usage(&memtracker);
	pt_memory_tracker_destroy(&memtracker);
	return res;
}

pt_memory pt_memory_for_grammar(pt_grammar *grammar) {
	pt_memory_tracker memtracker;
	pt_memory_tracker_reset(&memtracker);
	pt_memory_track_grammar(&memtracker, grammar);
	pt_memory res = pt_memory_tracker_current_usage(&memtracker);
	pt_memory_tracker_destroy(&memtracker);
	return res;
}

void pt_memory_tracker_reset(pt_memory_tracker *memtracker) {
	pt_memory_tracker_destroy(memtracker);
	memset(memtracker, 0, sizeof(pt_memory_tracker));
}

void pt_memory_tracker_destroy(pt_memory_tracker *memtracker) {
	Word_t rc;
	JLFA(rc, memtracker->address_table);
}

pt_memory pt_memory_tracker_current_usage(pt_memory_tracker *memtracker) {
	return memtracker->memory_usage;
}

int pt_memory_track_expr(pt_memory_tracker *memtracker, pt_expr *expr) {
	PWord_t pvalue;
	JLI(pvalue, memtracker->address_table, (Word_t)expr);
	if(pvalue == PJERR) {
		return -1;
	}
	else if(*pvalue == 0) {
		Word_t shallow_memory_used = 0;
		Word_t deep_memory_used = 0;

		*pvalue = deep_memory_used;
	}
	return *pvalue;
}

int pt_memory_track_grammar(pt_memory_tracker *memtracker, pt_grammar *grammar) {
	PWord_t pvalue;
	JLI(pvalue, memtracker->address_table, (Word_t)grammar);
	if(pvalue == PJERR) {
		return -1;
	}
	else if(*pvalue == 0) {
		Word_t shallow_memory_used = 0;
		Word_t deep_memory_used = 0;

		*pvalue = deep_memory_used;
	}
	return *pvalue;
}
