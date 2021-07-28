#pragma once
#include "RingBuffer.h"

using namespace std;
using Fifo = CRingBuffer<int, 4>;

class CWorker
{
public:
	CWorker();
	~CWorker();
	void Do();
	void Write(int);
	int Read();

private:
	Fifo m_Fifo;
	Fifo* m_pLast;
	condition_variable* m_pCondVar;
	mutex* m_pMutex;
	friend class CPipelineImp;
};