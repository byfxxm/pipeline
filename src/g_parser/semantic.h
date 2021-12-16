#pragma once

struct part;

namespace g_parser
{
	class semantic_c
	{
	public:
		semantic_c(std::ifstream&);
		std::shared_ptr<part> next_code();

	private:
		std::ifstream& __fin;
	};
}