#pragma once

namespace g_parser
{
	class g_parser_imp_c
	{
	public:
		bool load_file(std::string);
		void unload_file();
		void parse(utilities_s*);

	private:
		std::ifstream __file_stream;
	};
}