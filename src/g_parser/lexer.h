#pragma once

namespace g_parser
{
	enum class token : char
	{
		NA,
		G = 'G',
		X = 'X',
		Y = 'Y',
		Z = 'Z',
		VALUE,
		NEXT_LINE,
		SPACE,
		END_OF_FILE,
	};

	struct token_s
	{
		token tok{ token::NA };
		std::string val;
	};

	class lexer
	{
	public:
		lexer() = delete;
		lexer(std::ifstream&);
		std::optional<token_s> next_token();

	private:
		std::ifstream& __fin;
	};
}