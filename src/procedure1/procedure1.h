#pragma once

#ifdef PROCEDURE1_EXPORTS
#define PROCEDURE1_API __declspec(dllexport)
#else
#define PROCEDURE1_API __declspec(dllimport)
#endif

struct utility;

extern "C"
{
	PROCEDURE1_API void procedure1(utility*);
}