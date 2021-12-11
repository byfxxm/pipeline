#include "pch.h"
#include "g_parser.h"
#include "g_parser_imp.h"
#include "semantic.h"
#include "../pipeline/pipeline.h"

bool g_parser_imp::load_file(std::string file)
{
	__file_stream.open(file);
	return __file_stream.is_open();
}

void g_parser_imp::unload_file()
{
	__file_stream.close();
}

void g_parser_imp::parse(utilities* utils)
{
	semantic sem(__file_stream);
	std::shared_ptr<part> part_;

	while (part_ = sem.next_code())
		utils->write(part_);
}