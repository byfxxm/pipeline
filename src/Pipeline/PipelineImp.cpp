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
	thread _th([this]()
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
				if (_p->m_pNextWorker != nullptr)
					_p->m_pNextWorker->m_nState = WS_WORK;
				_p->SwitchToMainFiber();
			}, _pWorker);
		}

		Schedule();

		ConvertFiberToThread();
	});

	_th.detach();
}

void CPipelineImp::Abort()
{
	m_bAbort = true;
}

void CPipelineImp::Schedule()
{
	if (m_vecWorkerList.empty())
		return;

	m_vecWorkerList[0]->m_nState = WS_WORK;

	while (true)
	{
		CWorker* _p = nullptr;
		for (auto _it : m_vecWorkerList)
		{
			if (_it->m_nState == WS_WORK)
			{
				_p = _it;
				break;
			}
		}

		if (_p == nullptr)
			break;

		_ASSERT(_p != nullptr);
		SwitchToFiber(_p->m_pFiber);
	}

	for (auto _it : m_vecWorkerList)
	{
		_it->m_nState = WS_STOP;
	}
}

void CPipelineImp::AddWorker(CWorker* pWorker_)
{
	m_vecWorkerList.push_back(pWorker_);
	pWorker_->m_pCondVar = &m_CondVar;
	pWorker_->m_pMutex = &m_Mutex;
	pWorker_->m_ppMainFiber = &m_pFiber;
	pWorker_->m_pAbort = &m_bAbort;

	if (m_pCurWorker != nullptr)
	{
		pWorker_->m_pPrev = m_pCurWorker->m_pNext;
		pWorker_->m_pPrevWorker = m_pCurWorker;
		m_pCurWorker->m_pNextWorker = pWorker_;
	}

	m_pCurWorker = pWorker_;
}

void CPipelineImp::AddLastWorker(CWorker* pWorker_)
{
	AddWorker(pWorker_);
	delete m_pCurWorker->m_pNext;
	m_pCurWorker->m_pNext = new CRingBuffer<int>(20000);
}