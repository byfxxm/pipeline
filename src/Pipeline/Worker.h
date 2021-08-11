#pragma once
#include "RingBuffer.h"

using namespace std;
using Fifo = CRingBuffer<int>;

enum worker_state_t
{
	WS_REST,
	WS_WORK,
	WS_STOP,
};

class CWorker
{
public:
	CWorker(function<void(void*)>);
	~CWorker();
	void Do();
	void Write(int);
	int Read();

protected:
	void SwitchToMainFiber();

private:
	Fifo* m_pNext;
	Fifo* m_pPrev;
	CWorker* m_pNextWorker;
	CWorker* m_pPrevWorker;
	function<void(void*)> m_fDo;
	condition_variable* m_pCond;
	mutex* m_pMutex;
	worker_state_t m_nState;
	void* m_pFiber;
	void** m_ppMainFiber;
	bool* m_pAbort;
	friend class CPipelineImp;
};