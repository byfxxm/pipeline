#include "stdafx.h"
#include "Worker.h"

CWorker::CWorker(function<void(void*)> fDo_)
{
	m_pNext = new Fifo(4);
	m_pPrev = nullptr;
	m_fDo = fDo_;
}

CWorker::~CWorker()
{
	delete m_pNext;
}

void CWorker::Do()
{
	m_fDo(this);
}

void CWorker::Write(int n_)
{
	while (!m_pNext->Put(n_))
	{
		unique_lock<mutex> _lock(*m_pMutex);
		m_pCondVar->notify_all();
		m_pCondVar->wait(_lock);
	}
}

int CWorker::Read()
{
	int _nRet = 0;
	while (!m_pPrev->Get(_nRet))
	{
		unique_lock<mutex> _lock(*m_pMutex);
		m_pCondVar->notify_all();
		m_pCondVar->wait(_lock);
	}

	return _nRet;
}