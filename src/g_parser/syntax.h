#pragma once
#include "lexer.h"

namespace g_parser
{
	enum class predicate_t : char
	{
		NA,
		G0,
		G1,
	};

	using advs_t = std::vector<token_s>;

	struct sentence_s
	{
		predicate_t pred{ predicate_t::NA };
		advs_t advs;
	};

	class syntax_c
	{
	public:
		syntax_c() = delete;
		syntax_c(std::ifstream&);
		std::optional<sentence_s> next_sentence_s();

	private:
		predicate_t __match_pred(token_s);

	private:
		std::ifstream& __fin;
	};
}