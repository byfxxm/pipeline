#include "pch.h"
#include "g_parser.h"
#include "g_parser_imp.h"
#include "semantic.h"

bool g_parser_imp::load_file(std::string file)
{
	__file_stream.open(file);
	return __file_stream.is_open();
}

void g_parser_imp::parse(utilities* utils)
{
	semantic sem(__file_stream);
	part* part_ = nullptr;
	while (part_ = sem.next_code())
	{

	}
}