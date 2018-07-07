#include "pega-texto-memory.h"

#include <pega-texto.h>
#include <pega-texto/macro-on.h>

#include <ctype.h>
#include <stdio.h>

void print_memory(pt_memory *memory_usage) {
	printf("Expression: %d\n", memory_usage->expression_byte_count);
	printf("Grammar:    %d\n", memory_usage->grammar_byte_count);
	printf("Array:      %d\n", memory_usage->array_byte_count);
	printf("String:     %d\n", memory_usage->string_byte_count);
	printf("Total:      %d\n", memory_usage->total_byte_count);
	printf("\n");
}

int main() {
	pt_rule rules[] = {
		{ "Grammar", SEQ(V("S"), Q(V("Definition"), 1), NOT(ANY)) },
		{ "Definition", SEQ(V("Identifier"), V("Arrow"), V("Exp")) },

		{ "Exp", SEQ(V("Seq"), Q(SEQ(L("/"), V("S"), V("Seq")), 0)) },
		{ "Seq", Q(V("Prefix"), 1) },
		{ "Prefix", SEQ(Q(SEQ(S("&!"), V("S")), -1), V("Suffixed")) },
		{ "Suffixed", SEQ(V("Primary"), Q(V("Suffix"), -1)) },
		{ "Suffix", OR(SEQ(S("+*?"), V("S")), SEQ(L("^"), V("Number"))) },

		{ "Primary", OR(SEQ(L("("), V("S"), V("Exp"), L(")"), V("S")),
		                SEQ(L("{"), V("S"), V("Exp"), L("}"), V("S")),
		                V("Defined"),
		                V("Literal"),
		                V("CaseInsensitive"),
		                V("CharacterSet"),
		                SEQ(L("."), V("S")),
		                SEQ(V("Identifier"), NOT(V("Arrow")))) },
		{ "String", OR(SEQ(L("\""), Q(SEQ(NOT(L("\"")), V("Character")), 0), L("\""), V("S")),
		               SEQ(L("\'"), Q(SEQ(NOT(L("\'")), V("Character")), 0), L("\'"), V("S"))) },
		{ "Literal", V("String") },
		{ "CaseInsensitive", SEQ(L("I"), V( "String")) },
		{ "CharacterSet", SEQ(L("["), Q(L("^"), -1), V("Item"), Q(SEQ(NOT(L("]")), V("Item")), 0), L("]"), V("S")) },
		{ "Item", OR(V("Defined"), V("Range"), V("Character")) },
		{ "Range", SEQ(ANY, L("-"), BUT(L("]"))) },
		{ "Character", OR(SEQ(L("\\"), S("abfnrtv\'\"[]\\")),
		                  SEQ(L("\\"), R("02"), R("07"), R("07")),
		                  SEQ(L("\\"), R("07"), Q(R("07"), -1)),
		                  ANY) },
		{ "Defined", SEQ(L("\\"), S("wWaAcCdDgGlLpPsSuUxX"), V("S")) },

		{ "S", Q(OR(C(isspace), SEQ(L("#"), Q(BUT(L("\n")), 0))), 0) },
		{ "Identifier", SEQ(SEQ(OR(C(isalpha), L("_")), Q(OR(C(isalnum), S("_-")), 0)), V("S")) },
		{ "Arrow", SEQ(L("<-"), V("S")) },
		{ "Number", SEQ(Q(S("+-"), -1), Q(C(isdigit), 1), V("S")) },
		{ NULL, NULL },
	};
	pt_grammar *grammar = pt_create_grammar(rules, 0);
	pt_validate_grammar(grammar, PT_VALIDATE_ABORT);
	pt_memory memory_usage = pt_memory_for_grammar(grammar);
	print_memory(&memory_usage);

	pt_destroy_grammar(grammar);
	return 0;
}

