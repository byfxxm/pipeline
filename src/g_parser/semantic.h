#pragma once

struct part_s;

namespace g_parser
{
	class semantic_c
	{
	public:
		semantic_c(std::ifstream&);
		std::shared_ptr<part_s> next_code();

	private:
		std::ifstream& __fin;
	};
}