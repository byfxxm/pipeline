#include "pch.h"
#include "g_parser.h"
#include "g_parser_imp.h"
#include "../pipeline/pipeline.h"

void g_parser_proc(utilities* utils)
{
	g_parser_imp parser;

	if (!parser.load_file(utils->file))
		return;

	parser.parse(utils);
}