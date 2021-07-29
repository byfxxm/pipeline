#include "stdafx.h"
#include "Worker.h"

CWorker::CWorker(function<void(void*)> fDo_)
{
	m_pNext = new Fifo(4);
	m_pPrev = nullptr;
	m_fDo = fDo_;
	m_nState = WS_REST;
	m_pFiber = nullptr;
	m_ppMainFiber = nullptr;
}

CWorker::~CWorker()
{
	delete m_pNext;
}

void CWorker::Do()
{
	m_fDo(this);
}

inline void CWorker::SwitchToMainFiber()
{
	if (m_ppMainFiber != nullptr && *m_ppMainFiber != nullptr)
		SwitchToFiber(*m_ppMainFiber);
}

void CWorker::Write(int n_)
{
	while (!m_pNext->Put(n_))
	{
	}
}

int CWorker::Read()
{
	int _nRet = 0;
	while (!m_pPrev->Get(_nRet))
	{

	}

	return _nRet;
}