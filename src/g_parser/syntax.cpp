#include "pch.h"
#include "syntax.h"
#include "lexer.h"

syntax::syntax(std::ifstream& fin) : __fin(fin) {}

sentence syntax::next_sentence()
{
	lexer lex(__fin);
	std::optional<token_struct> tk_s;
	sentence ret;

	while (!(tk_s = lex.next_token()).has_value())
	{
		switch (tk_s.value().tok)
		{
		case token::G:
			if (ret.pred != predicate::NA)
				throw std::exception("double predicate");

			ret.pred = __match_pred(tk_s.value());
			break;

		default:
			break;
		}
	}

	return ret;
}

predicate syntax::__match_pred(token_struct tk_s)
{
	assert(tk_s.tok == token::G);

	//if 
	//{

	//}
}