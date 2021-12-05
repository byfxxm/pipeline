#pragma once

struct part;

namespace g_parser
{
	class semantic
	{
	public:
		semantic(std::ifstream&);
		part* next_code();

	private:
		std::ifstream& __fin;
	};
}