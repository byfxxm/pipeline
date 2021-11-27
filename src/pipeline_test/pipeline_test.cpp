﻿#include <iostream>
#include <Windows.h>
#include <thread>
#include "../pipeline/pipeline.h"
#include "../procedure1/procedure1.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/pipeline.lib")
#pragma comment(lib, "../Debug/procedure1.lib")
#else
#pragma comment(lib, "../Release/pipeline.lib")
#pragma comment(lib, "../Release/procedure1.lib")
#endif

using namespace std;

struct code : public part
{
	int index{ 0 };
};

static int g_index = 0;

int main()
{
	auto pipeline = pipeline_create();

	pipeline_add_procedure(pipeline, procedure1);

	pipeline_add_procedure(pipeline, [](utility* util)
		{
			code* code_ = nullptr;
			int count = 0;
			while (1)
			{
				code_ = (code*)util->read();
				util->write(code_);

				if (count++ % 2000 == 0)
				{
					util->syn();
					printf("=================%d\n", g_index);
				}
			}
		});

	for (int _i = 0; _i < 10; _i++)
	{
		pipeline_add_procedure(pipeline, [](utility* util)
			{
				code* code_ = nullptr;

				while (code_ = (code*)util->read())
				{
					util->write(code_);
				}
			});
	}

	pipeline_start_async(pipeline, [](part* p)
		{
			if (!p)
				return;

			part_syn* part_syn_ = dynamic_cast<part_syn*>(p);
			if (part_syn_)
			{
				part_syn_->prom.set_value();
				delete p;
				return;
			}

			printf("%d\n", ((code*)p)->index);
			g_index = ((code*)p)->index;
			delete p;
		});

	thread th([pipeline]()
		{
			this_thread::sleep_for(chrono::seconds(2));
			pipeline_stop_async(pipeline);
			pipeline_wait_for_idle(pipeline);
		});

	th.join();
	pipeline_wait_for_idle(pipeline);
	pipeline_delete(pipeline);

	return 0;
}
