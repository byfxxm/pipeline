#include "pch.h"
#include "lexer.h"

lexer::lexer(std::ifstream& fin) : __fin(fin) {}

std::optional<token_struct> lexer::next_token()
{
	if (!__fin.is_open())
		return std::nullopt;

	std::string tok;
	__fin >> tok;

	if (__fin.eof())
		return std::nullopt;

	return token_struct{ token(tok[0]), 0};
}