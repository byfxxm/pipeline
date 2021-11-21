#include <iostream>
#include <Windows.h>
#include <thread>
#include "../pipeline/pipeline.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/pipeline.lib")
#else
#pragma comment(lib, "../Release/pipeline.lib")
#endif

//#define printf
using namespace std;

struct code : public part
{
	code()
	{
		OutputDebugString("construct NcCode\n");
	}

	~code()
	{
		OutputDebugString("destruct NcCode\n");
	}

	int index{ 0 };
};

int main()
{
	auto pipeline = pipeline_create();

	pipeline_add_procedure(pipeline, [](read_func read, write_func write)
		{
			for (int i = 0; i < 10000; i++)
			{
				auto p = new code();
				p->index = i;
				write(p);
			}
		});

	for (int _i = 1; _i < 10; _i++)
	{
		pipeline_add_procedure(pipeline, [](read_func read, write_func write)
			{
				code* code_ = nullptr;

				while (code_ = (code*)read())
				{
					write(code_);
				}
			});
	}

	pipeline_start(pipeline, [](part* p)
		{
			printf("%d\n", ((code*)p)->index);
			delete p;
		});

	thread th([pipeline]()
		{
			getchar();
			pipeline_stop(pipeline);
		});

	th.join();
	pipeline_wait_for_idle(pipeline);
	pipeline_delete(pipeline);

	return 0;
}
