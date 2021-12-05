#include "pch.h"
#include "syntax.h"
#include "lexer.h"

syntax::syntax(std::ifstream& fin) : __fin(fin) {}

std::optional<sentence> syntax::next_sentence()
{
	lexer lex(__fin);
	std::optional<token_s> tk_s;
	std::optional<sentence> ret;

	while (!(tk_s = lex.next_token()).has_value())
	{
		switch (tk_s.value().tok)
		{
		case token::G:
			if (ret.value().pred != predicate::NA)
				throw std::exception("double predicate");

			ret.value().pred = __match_pred(tk_s.value());
			break;

		default:
			break;
		}
	}

	return ret;
}

predicate syntax::__match_pred(token_s tk_s)
{
	assert(tk_s.tok == token::G);
	auto val = std::stof(tk_s.val);

	if (val == 0)
		return predicate::G0;
	
	return predicate::NA;
}