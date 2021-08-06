#pragma once

using namespace std;
class CWorker;

class CPipelineImp
{
public:
	CPipelineImp();
	~CPipelineImp();
	void Start();
	void Stop();
	void AddWorker(CWorker*);
	void AddLastWorker(CWorker*);

private:
	void Schedule();
	vector<CWorker*> m_vecWorkerList;
	CWorker* m_pCurWorker;
	condition_variable m_CondVar;
	mutex m_Mutex;
	void* m_pFiber;
	bool m_bExit;
};