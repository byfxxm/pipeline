#include "stdafx.h"
#include "PipelineImp.h"

void* Pipeline_Create()
{
	return new CPipelineImp();
}

void Pipeline_Delete(void* pPipeline_)
{
	delete (CPipelineImp*)pPipeline_;
}

void Run(void* pPipeline_)
{
	return ((CPipelineImp*)pPipeline_)->Run();
}