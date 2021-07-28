#include "stdafx.h"
#include "Worker.h"

CWorker::CWorker()
{
	m_pLast = nullptr;
}

CWorker::~CWorker()
{

}

void CWorker::Do()
{

}

void CWorker::Write(int n_)
{
	while (!m_Fifo.Put(n_))
	{
		unique_lock<mutex> _lock(*m_pMutex);
		m_pCondVar->notify_all();
		m_pCondVar->wait(_lock);
	}
}

int CWorker::Read()
{
	int _nRet = 0;
	while (!m_pLast->Get(_nRet))
	{
		unique_lock<mutex> _lock(*m_pMutex);
		m_pCondVar->notify_all();
		m_pCondVar->wait(_lock);
	}

	return _nRet;
}