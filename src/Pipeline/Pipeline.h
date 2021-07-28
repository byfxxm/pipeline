#pragma once

#ifdef PIPELINE_EXPORTS
#define PIPELINE_API extern "C" __declspec(dllexport)
#else
#define PIPELINE_API extern "C" __declspec(dllimport)
#endif

PIPELINE_API void* Pipeline_Create();
PIPELINE_API void Pipeline_Delete(void* pPipeline_);
PIPELINE_API void Run(void* pPipeline_);
