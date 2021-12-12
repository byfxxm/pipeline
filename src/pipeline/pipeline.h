#pragma once
#include <memory>
#include "part.h"

#ifdef PIPELINE_EXPORTS
#define PIPELINE_API __declspec(dllexport)
#else
#define PIPELINE_API __declspec(dllimport)
#endif

struct utilities
{
	using read_f = std::shared_ptr<part>(*)();
	using write_f = void(*)(const std::shared_ptr<part>&);
	using syn_f = void(*)();

	const read_f read{ nullptr };
	const write_f write{ nullptr };
	const syn_f syn{ nullptr };
	const char* file{ nullptr };
};

using output_f = utilities::write_f;
using procedure_f = void(*)(utilities*);

extern "C"
{
	PIPELINE_API void* pipeline_create();
	PIPELINE_API void pipeline_delete(void* pipeline);
	PIPELINE_API void pipeline_start_async(void* pipeline, output_f output);
	PIPELINE_API void pipeline_stop_async(void* pipeline);
	PIPELINE_API void pipeline_add_procedure(void* pipeline, procedure_f proc);
	PIPELINE_API void pipeline_wait_for_idle(void* pipeline);
	PIPELINE_API void pipeline_set_file(void* pipeline, const char* file);
	PIPELINE_API void pipeline_reset_file(void* pipeline);
}