#pragma once
#include "RingBuffer.h"

using namespace std;
using Fifo = CRingBuffer<int>;

class CWorker
{
public:
	CWorker(function<void(void*)>);
	~CWorker();
	void Do();
	void Write(int);
	int Read();

private:
	Fifo* m_pNext;
	Fifo* m_pPrev;
	function<void(void*)> m_fDo;
	condition_variable* m_pCondVar;
	mutex* m_pMutex;
	friend class CPipelineImp;
};
