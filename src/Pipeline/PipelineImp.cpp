#include "stdafx.h"
#include "PipelineImp.h"
#include "Worker.h"

CPipelineImp::CPipelineImp()
{
	m_pCurWorker = nullptr;
}

CPipelineImp::~CPipelineImp()
{

}

void CPipelineImp::Run()
{
	for (auto _it : m_vecWorkerList)
	{
		thread _th([_it]()
		{
			_it->Do();
		});

		_th.detach();
	}
}

void CPipelineImp::AddWorker(CWorker* pWorker_)
{
	m_vecWorkerList.push_back(pWorker_);
	
	if (m_pCurWorker == nullptr)
	{
		m_pCurWorker = pWorker_;
		return;
	}

	pWorker_->m_pLast = &m_pCurWorker->m_Fifo;
	pWorker_->m_pCondVar = &m_CondVar;
	pWorker_->m_pMutex = &m_Mutex;
	m_pCurWorker = pWorker_;
}