#pragma once

namespace g_parser
{
	enum class token_t : char
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
		token_t tok{ token_t::NA };
		std::string val;
	};

	class lexer_c
	{
	public:
		lexer_c() = delete;
		lexer_c(std::ifstream&);
		std::optional<token_s> next_token();

	private:
		std::ifstream& __fin;
	};
}