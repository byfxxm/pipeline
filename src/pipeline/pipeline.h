#pragma once
#include "part.h"

#ifdef PIPELINE_EXPORTS
#define PIPELINE_API __declspec(dllexport)
#else
#define PIPELINE_API __declspec(dllimport)
#endif

using read_func = part * (*)();
using write_func = void (*)(part*);
using output_func = write_func;
using syn_func = void (*)();

struct utilities
{
	read_func read{ nullptr };
	write_func write{ nullptr };
	syn_func syn{ nullptr };
};
using procedure_func = void(*)(utilities*);

extern "C"
{
	PIPELINE_API void* pipeline_create();
	PIPELINE_API void pipeline_delete(void* pipeline);
	PIPELINE_API void pipeline_start_async(void* pipeline, output_func output);
	PIPELINE_API void pipeline_stop_async(void* pipeline);
	PIPELINE_API void pipeline_add_procedure(void* pipeline, procedure_func proc);
	PIPELINE_API void pipeline_wait_for_idle(void* pipeline);
}