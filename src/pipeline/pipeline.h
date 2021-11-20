#pragma once
#include <functional>

#ifdef PIPELINE_EXPORTS
#define PIPELINE_API __declspec(dllexport)
#else
#define PIPELINE_API __declspec(dllimport)
#endif

extern "C"
{
	struct part {};
	using read = part * (*)();
	using write = void(*)(part*);
	using procedure = void(*)(read, write);

	PIPELINE_API void* pipeline_create();
	PIPELINE_API void pipeline_delete(void* pipeline);
	PIPELINE_API void pipeline_start(void* pipeline);
	PIPELINE_API void pipeline_stop(void* pipeline);
	PIPELINE_API void pipeline_add_procedure(void* pipeline, procedure proc);
}