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
	pt_memory_tracker memtracker = {};
	pt_memory_track_expr(&memtracker, expr);
	pt_memory res = pt_memory_tracker_current_usage(&memtracker);
	pt_memory_tracker_destroy(&memtracker);
	return res;
}

pt_memory pt_memory_for_grammar(pt_grammar *grammar) {
	pt_memory_tracker memtracker = {};
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

/// Track a string memory
static int pt_memory_track_string(pt_memory_tracker *memtracker, const char *str) {
	PWord_t pvalue;
	JLI(pvalue, memtracker->address_table, (Word_t)str);
	if(pvalue == PJERR) {
		return 0;
	}
	else if(*pvalue == 0) {
		int byte_count = strlen(str) + 1;
		*pvalue = byte_count;
		memtracker->memory_usage.string_byte_count += byte_count;
		memtracker->memory_usage.total_byte_count += byte_count;
	}
	return *pvalue;
}
/// Track a pointer array memory
static int pt_memory_track_pointer_array(pt_memory_tracker *memtracker, void *arr, int N) {
	PWord_t pvalue;
	JLI(pvalue, memtracker->address_table, (Word_t)arr);
	if(pvalue == PJERR) {
		return 0;
	}
	else if(*pvalue == 0) {
		int byte_count = N * sizeof(void *);
		*pvalue = byte_count;
		memtracker->memory_usage.array_byte_count += byte_count;
		memtracker->memory_usage.total_byte_count += byte_count;
	}
	return *pvalue;
}

int pt_memory_track_expr(pt_memory_tracker *memtracker, pt_expr *expr) {
	PWord_t pvalue;
	JLI(pvalue, memtracker->address_table, (Word_t)expr);
	if(pvalue == PJERR) {
		return 0;
	}
	else if(*pvalue == 0) {
		Word_t deep_memory_used = 0;
		switch(expr->op) {
			case PT_LITERAL:
			case PT_CASE_INSENSITIVE:
			case PT_SET:
			case PT_RANGE:
			case PT_NON_TERMINAL:
				deep_memory_used += pt_memory_track_string(memtracker, expr->data.characters);
				break;
			case PT_QUANTIFIER:
			case PT_AND:
			case PT_NOT:
			case PT_ERROR:
				deep_memory_used += pt_memory_track_expr(memtracker, expr->data.e);
				break;
			case PT_SEQUENCE:
			case PT_CHOICE: {
					int i;
					for(i = 0; i < expr->N; i++) {
						deep_memory_used += pt_memory_track_expr(memtracker, expr->data.es[i]);
					}
					deep_memory_used += pt_memory_track_pointer_array(memtracker, expr->data.es, expr->N);
				}
				break;
			case PT_ANY:
			case PT_CHARACTER_CLASS:
			case PT_CUSTOM_MATCHER:
				// no extra memory usage here
				break;
		}
		deep_memory_used += sizeof(pt_expr);

		// reacquire pvalue, as other calls to JLI may invalidate it
		JLI(pvalue, memtracker->address_table, (Word_t)expr);

		*pvalue = deep_memory_used;
		memtracker->memory_usage.expression_byte_count += sizeof(pt_expr);
		memtracker->memory_usage.total_byte_count += sizeof(pt_expr);
	}
	return *pvalue;
}

int pt_memory_track_grammar(pt_memory_tracker *memtracker, pt_grammar *grammar) {
	PWord_t pvalue;
	JLI(pvalue, memtracker->address_table, (Word_t)grammar);
	if(pvalue == PJERR) {
		return 0;
	}
	else if(*pvalue == 0) {
		Word_t deep_memory_used = 0;

		int i;
		for(i = 0; i < grammar->N; i++) {
			deep_memory_used += pt_memory_track_expr(memtracker, grammar->es[i]);
			deep_memory_used += pt_memory_track_string(memtracker, grammar->names[i]);
		}
		deep_memory_used += pt_memory_track_pointer_array(memtracker, grammar->es, grammar->N);
		deep_memory_used += pt_memory_track_pointer_array(memtracker, grammar->names, grammar->N);
		deep_memory_used += sizeof(pt_grammar);

		// reacquire pvalue, as other calls to JLI may invalidate it
		JLI(pvalue, memtracker->address_table, (Word_t)grammar);

		*pvalue = deep_memory_used;
		memtracker->memory_usage.grammar_byte_count += sizeof(pt_grammar);
		memtracker->memory_usage.total_byte_count += sizeof(pt_grammar);
		return deep_memory_used;
	}
	return *pvalue;
}
