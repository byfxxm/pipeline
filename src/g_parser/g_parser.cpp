#include "pch.h"
#include "g_parser.h"
#include "g_parser_imp.h"
#include "../pipeline/pipeline.h"

void g_parser_proc(utilities_s* utils)
{
	g_parser_imp_c parser;

	if (!parser.load_file(utils->file))
		return;

	parser.parse(utils);
}