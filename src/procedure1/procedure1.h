#pragma once

#ifdef PROCEDURE1_EXPORTS
#define PROCEDURE1_API __declspec(dllexport)
#else
#define PROCEDURE1_API __declspec(dllimport)
#endif

struct utilities_s;

extern "C"
{
	PROCEDURE1_API void procedure1(utilities_s*);
}