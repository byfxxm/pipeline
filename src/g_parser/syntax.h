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

	using advs_t = std::vector<token_s>;

	struct sentence
	{
		predicate pred{ predicate::NA };
		advs_t advs;
	};

	class syntax
	{
	public:
		syntax() = delete;
		syntax(std::ifstream&);
		std::optional<sentence> next_sentence();

	private:
		predicate __match_pred(token_s);

	private:
		std::ifstream& __fin;
	};
}