#pragma once
#include "lexer.h"

namespace g_parser
{
	enum class predicate : char
	{
		NA,
		G0,
		G1,
	};

	struct sentence
	{
		predicate pred{ predicate::NA };
		std::vector<token_s> advs;
	};

	class syntax
	{
	public:
		syntax() = delete;
		syntax(std::ifstream&);
		sentence next_sentence();

	private:
		predicate __match_pred(token_s);

	private:
		std::ifstream& __fin;
	};
}