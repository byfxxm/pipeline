#include "stdafx.h"
#include "PipelineImp.h"
#include "Worker.h"

CPipelineImp::CPipelineImp()
{

}

CPipelineImp::~CPipelineImp()
{

}

void CPipelineImp::Run()
{
	thread* _pThs = new thread[m_vecWorkerList.size()];

	for (auto _it : m_vecWorkerList)
	{
		thread _th([_it]()
		{
			_it->Do();
		});

		_pThs->swap(_th);
	}

	for (unsigned _i = 0; _i < m_vecWorkerList.size(); _i++)
	{
		_pThs->join();
	}

	delete[] _pThs;
}

void CPipelineImp::AddWorker(CWorker* pWorker_)
{
	m_vecWorkerList.push_back(pWorker_);
	pWorker_->m_pCondVar = &m_CondVar;
	pWorker_->m_pMutex = &m_Mutex;

	if (m_pCurWorker != nullptr)
		pWorker_->m_pPrev = m_pCurWorker->m_pNext;

	m_pCurWorker = pWorker_;
}

void CPipelineImp::AddLastWorker(CWorker* pWorker_)
{
	AddWorker(pWorker_);
	delete m_pCurWorker->m_pNext;
	m_pCurWorker->m_pNext = new CRingBuffer<int>(20000);
}