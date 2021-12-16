#include "pch.h"
#include "g_parser.h"
#include "g_parser_imp.h"
#include "semantic.h"
#include "../pipeline/pipeline.h"

bool g_parser_imp_c::load_file(std::string file)
{
	__file_stream.open(file);
	return __file_stream.is_open();
}

void g_parser_imp_c::unload_file()
{
	__file_stream.close();
}

void g_parser_imp_c::parse(utilities_s* utils)
{
	semantic_c sem(__file_stream);
	std::shared_ptr<part_s> part;

	while (part = sem.next_code())
		utils->write(part);
}