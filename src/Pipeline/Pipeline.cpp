#include "stdafx.h"
#include "Pipeline.h"
#include "PipelineImp.h"
#include "Worker.h"

void* Pipeline_Create()
{
	return new CPipelineImp();
}

void Pipeline_Delete(void* pPipeline_)
{
	delete (CPipelineImp*)pPipeline_;
}

void Pipeline_Start(void* pPipeline_)
{
	return ((CPipelineImp*)pPipeline_)->Start();
}

void Pipeline_Stop(void* pPipeline_)
{
	return ((CPipelineImp*)pPipeline_)->Stop();
}

void Pipeline_AddWorker(void* pPipeline_, void* pWorker_)
{
	return ((CPipelineImp*)pPipeline_)->AddWorker((CWorker*)pWorker_);
}

void Pipeline_AddLastWorker(void* pPipeline_, void* pWorker_)
{
	return ((CPipelineImp*)pPipeline_)->AddLastWorker((CWorker*)pWorker_);
}

void* Worker_Create(function<void(void* pWorker_)> fDo_)
{
	return new CWorker(fDo_);
}

void Worker_Delete(void* pWorker_)
{
	delete (CWorker*)pWorker_;
}

void Worker_Write(void* pWorker_, int n_)
{
	return ((CWorker*)pWorker_)->Write(n_);
}

int Worker_Read(void* pWorker_)
{
	return ((CWorker*)pWorker_)->Read();
}