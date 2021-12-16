#pragma once

#ifdef GPARSER_EXPORTS
#define GPARSER_API __declspec(dllexport)
#else
#define GPARSER_API __declspec(dllimport)
#endif

struct utilities_s;

extern "C"
{
	GPARSER_API void g_parser_proc(utilities_s*);
}