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
	OutputDebugString("begin\n");

	pipeline_add_procedure(pipeline, [](read read_f, write write_f)
		{
			for (int i = 0; i < 10000000; i++)
			{
				auto p = new code();
				p->index = i;
				write_f(p);
				printf("Worker[0] write %d\n", i);
			}
		});

	for (int _i = 1; _i < 10; _i++)
	{
		pipeline_add_procedure(pipeline, [](read read_f, write write_f)
			{
				while (1)
				{
					code* code_ = (code*)read_f();
					printf("read %d\n", code_->index);

					write_f(code_);
					printf("write %d\n", code_->index);
				}
			});
	}

	pipeline_start(pipeline);
	pipeline_delete(pipeline);

	return 0;
}
