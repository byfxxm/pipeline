#pragma once

namespace g_parser
{
	class g_parser_imp
	{
	public:
		bool load_file(std::string);
		void parse(utilities*);

	private:
		std::ifstream __file_stream;
	};
}