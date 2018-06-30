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

/** @file pega-texto-memory.h
 * Memory usage statistics for the pega-texto PEG runtime engine.
 */

#ifndef __PEGA_TEXTO_MEMORY_H__
#define __PEGA_TEXTO_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <pega-texto.h>

/**
 * Structure with memory usage statistics about pega-texto.
 *
 * @note The byte counts represent deep memory usage, so the sum of the partial
 * byte counts will be greater than `total_byte_count`.
 */
typedef struct pt_memory {
	/// Total memory count.
	int total_byte_count;
	/// Expression memory count.
	int expression_byte_count;
	/// Grammar memory count.
	int grammar_byte_count;
	/// Array memory count.
	int array_byte_count;
	/// String memory count.
	int string_byte_count;
} pt_memory;

////////////////////////////////////////////////////////////////////////////////
//  Basic usage
////////////////////////////////////////////////////////////////////////////////
/**
 * Get the amount of memory used by an Expression.
 *
 * @return Memory usage statistics about the given Expression.
 */
pt_memory pt_memory_for_expr(pt_expr *expr);

/**
 * Get the amount of memory used by a Grammar.
 *
 * @return Memory usage statistics about the given Grammar.
 */
pt_memory pt_memory_for_grammar(pt_grammar *grammar);


////////////////////////////////////////////////////////////////////////////////
//  Using memory tracker
////////////////////////////////////////////////////////////////////////////////
/**
 * Tracker of used memory.
 *
 * This structure tracks and caches the amount of memory used by the addresses
 * already visited, be it pega-texto Expressions, Grammars, arrays or strings.
 */
typedef struct pt_memory_tracker pt_memory_tracker;

/**
 * Reset a memory tracker.
 *
 * Call this to initialize a memory tracker. All data previously cached will be
 * destroyed.
 */
void pt_memory_tracker_reset(pt_memory_tracker *memtracker);

/**
 * Free the memory used by a memory tracker.
 */
void pt_memory_tracker_destroy(pt_memory_tracker *memtracker);

/**
 * Get the current memory usage statistics tracked by `memtracker`.
 */
pt_memory pt_memory_tracker_current_usage(pt_memory_tracker *memtracker);

/**
 * Get the total amount of memory used by an Expression, in bytes.
 *
 * The memory count will be cached in the memory tracker.
 *
 * @return Number of bytes used by Expression.
 */
int pt_memory_track_expr(pt_memory_tracker *memtracker, pt_expr *expr);

/**
 * Get the total amount of memory used by a Grammar, in bytes.
 *
 * The memory count will be cached in the memory tracker.
 *
 * @return Number of bytes used by Grammar.
 */
int pt_memory_track_grammar(pt_memory_tracker *memtracker, pt_grammar *grammar);

/// Pega-texto-memory version string
#define PT_MEMORY_VERSION "@PROJECT_VERSION@"

#ifdef __cplusplus
}
#endif

#endif

