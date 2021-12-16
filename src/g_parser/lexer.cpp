#include "pch.h"
#include "lexer.h"

lexer_c::lexer_c(std::ifstream& fin) : __fin(fin) {}

std::optional<token_s> lexer_c::next_token()
{
	if (!__fin.is_open())
		return std::nullopt;

	std::string tok;
	__fin >> tok;

	if (__fin.eof())
		return std::nullopt;

	return token_s{ token_t(tok[0]), 0};
}