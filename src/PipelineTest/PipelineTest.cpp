#include <iostream>
#include <Windows.h>
#include "../Pipeline/Pipeline.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/Pipeline.lib")
#else
#pragma comment(lib, "../Release/Pipeline.lib")
#endif

#define printf

int main()
{
	auto _p = Pipeline_Create();
	OutputDebugString("begin\n");

	Pipeline_AddWorker(_p, Worker_Create([](void* pWorker_)
	{
		for (int _i = 0; _i < 100; _i++)
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
			int _n = Worker_Read(pWorker_);
			printf("Worker[10] read %d\n", _n);

			Worker_Write(pWorker_, _n);
			printf("Worker[10] write %d\n", _n);

			if (_n == -1)
				break;
		}
	}));

	Pipeline_Run(_p);

	OutputDebugString("end\n");
	Pipeline_Delete(_p);

	return 0;
}
