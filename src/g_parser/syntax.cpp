#include "pch.h"
#include "syntax.h"
#include "lexer.h"

syntax_c::syntax_c(std::ifstream& fin) : __fin(fin) {}

std::optional<sentence_s> syntax_c::next_sentence_s()
{
	lexer_c lex(__fin);
	std::optional<token_s> tk_s;
	std::optional<sentence_s> ret;

	while (!(tk_s = lex.next_token()).has_value())
	{
		switch (tk_s.value().tok)
		{
		case token_t::G:
			if (ret.value().pred != predicate_t::NA)
				throw std::exception("double predicate_c");

			ret.value().pred = __match_pred(tk_s.value());
			break;

		default:
			break;
		}
	}

	return ret;
}

predicate_t syntax_c::__match_pred(token_s tk_s)
{
	assert(tk_s.tok == token_t::G);
	auto val = std::stof(tk_s.val);

	if (val == 0)
		return predicate_t::G0;
	
	return predicate_t::NA;
}