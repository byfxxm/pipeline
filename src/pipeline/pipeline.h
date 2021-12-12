#pragma once
#include "part.h"

#ifdef PIPELINE_EXPORTS
#define PIPELINE_API __declspec(dllexport)
#else
#define PIPELINE_API __declspec(dllimport)
#endif

struct utilities
{
	using read_func = std::shared_ptr<part>(*)();
	using write_func = void(*)(const std::shared_ptr<part>&);
	using syn_func = void(*)();

	const read_func read{ nullptr };
	const write_func write{ nullptr };
	const syn_func syn{ nullptr };
	const char* file{ nullptr };
};

using output_func = utilities::write_func;
using procedure_func = void(*)(utilities*);

extern "C"
{
	PIPELINE_API void* pipeline_create();
	PIPELINE_API void pipeline_delete(void* pipeline);
	PIPELINE_API void pipeline_start_async(void* pipeline, output_func output);
	PIPELINE_API void pipeline_stop_async(void* pipeline);
	PIPELINE_API void pipeline_add_procedure(void* pipeline, procedure_func proc);
	PIPELINE_API void pipeline_wait_for_idle(void* pipeline);
	PIPELINE_API void pipeline_set_file(void* pipeline, const char* file);
	PIPELINE_API void pipeline_reset_file(void* pipeline);
}