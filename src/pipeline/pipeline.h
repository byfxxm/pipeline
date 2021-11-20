#pragma once
#include <functional>

#ifdef PIPELINE_EXPORTS
#define PIPELINE_API extern "C" __declspec(dllexport)
#else
#define PIPELINE_API extern "C" __declspec(dllimport)
#endif

PIPELINE_API void* Pipeline_Create();
PIPELINE_API void Pipeline_Delete(void* pPipeline_);
PIPELINE_API void Pipeline_Run(void* pPipeline_);
PIPELINE_API void Pipeline_Abort(void* pPipeline_);
PIPELINE_API void Pipeline_AddWorker(void* pPipeline_, void* pWorker_);
PIPELINE_API void Pipeline_AddLastWorker(void* pPipeline_, void* pWorker_);

PIPELINE_API void* Worker_Create(std::function<void(void* pWorker_)> fDo_);
PIPELINE_API void Worker_Delete(void* pWorker_);
PIPELINE_API void Worker_Write(void* pWorker_, int n_);
PIPELINE_API int Worker_Read(void* pWorker_);
