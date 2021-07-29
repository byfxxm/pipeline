#include "stdafx.h"
#include "PipelineImp.h"
#include "Worker.h"

CPipelineImp::CPipelineImp()
{
	m_pCurWorker = nullptr;
	m_pFiber = nullptr;
	m_bAbort = false;
}

CPipelineImp::~CPipelineImp()
{

}

void CPipelineImp::Run()
{
	m_pFiber = ConvertThreadToFiber(nullptr);

	for (auto _pWorker : m_vecWorkerList)
	{
		_pWorker->m_pFiber = CreateFiber(0, [](void* p_)
		{
			CWorker* _p = (CWorker*)p_;
			try
			{
				_p->Do();
			}
			catch (...)
			{

			}

			_p->m_nState = WS_STOP;
		}, _pWorker);
	}

	Schedule();

	ConvertFiberToThread();
}

void CPipelineImp::Schedule()
{
	if (m_vecWorkerList.empty())
		return;

	m_vecWorkerList[0]->m_nState = WS_WORK;

	while (!m_bAbort)
	{
		CWorker* _p = nullptr;
		for (auto _it : m_vecWorkerList)
		{
			if (_it->m_nState)
			{
				_p = _it;
				break;
			}
		}

		_ASSERT(_p != nullptr);
		SwitchToFiber(_p->m_pFiber);
	}
}

void CPipelineImp::AddWorker(CWorker* pWorker_)
{
	m_vecWorkerList.push_back(pWorker_);
	pWorker_->m_pCondVar = &m_CondVar;
	pWorker_->m_pMutex = &m_Mutex;
	pWorker_->m_ppMainFiber = &m_pFiber;

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