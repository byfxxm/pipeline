#include <iostream>
#include <Windows.h>
#include <thread>
#include "../Pipeline/Pipeline.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/Pipeline.lib")
#else
#pragma comment(lib, "../Release/Pipeline.lib")
#endif

//#define printf
using namespace std;

class NcCode
{
public:
	NcCode()
	{
		OutputDebugString("construct NcCode\n");
	}

	~NcCode()
	{
		OutputDebugString("destruct NcCode\n");
	}
};

int main()
{
	auto _p = Pipeline_Create();
	OutputDebugString("begin\n");

	Pipeline_AddWorker(_p, Worker_Create([](void* pWorker_)
	{
		shared_ptr<NcCode> p(new NcCode);

		for (int _i = 0; _i < 10000000; _i++)
		{
			Worker_Write(pWorker_, _i);
			printf("Worker[0] write %d\n", _i);
		}

		Worker_Write(pWorker_, -1);
		printf("Worker[0] write -1\n");
	}));

	for (int _i = 1; _i < 10; _i++)
	{
		Pipeline_AddWorker(_p, Worker_Create([_i](void* pWorker_)
		{
			while (true)
			{
				shared_ptr<NcCode> p(new NcCode);
				shared_ptr<void> p1(Pipeline_Create(), [](void* p_) {Pipeline_Delete(p_); });

				int _n = Worker_Read(pWorker_);
				printf("Worker[%d] read %d\n", _i, _n);
				
				Worker_Write(pWorker_, _n);
				printf("Worker[%d] write %d\n", _i, _n);

				if (_n == -1)
					break;
			}
		}));
	}

	Pipeline_AddLastWorker(_p, Worker_Create([](void* pWorker_)
	{
		while (true)
		{
			shared_ptr<NcCode> p(new NcCode);

			int _n = Worker_Read(pWorker_);
			printf("Worker[10] read %d\n", _n);

			Worker_Write(pWorker_, _n);
			printf("Worker[10] write %d\n", _n);

			if (_n == -1)
				break;
		}
	}));

	thread _th([_p]()
	{
		this_thread::sleep_for(chrono::milliseconds(1500));
		Pipeline_Stop(_p);
	});

	Pipeline_Start(_p);

	_th.join();

	OutputDebugString("end\n");
	Pipeline_Delete(_p);

	return 0;
}
