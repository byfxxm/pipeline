#include <iostream>
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

	pipeline_add_procedure(pipeline, [](utility* utils)
		{
			code* code_ = nullptr;
			int count = 0;
			while (1)
			{
				code_ = (code*)utils->read();
				utils->write(code_);

				if (count++ % 2000 == 0)
				{
					utils->syn();
					printf("=================%d\n", g_index);
				}
			}
		});

	for (int _i = 0; _i < 10; _i++)
	{
		pipeline_add_procedure(pipeline, [](utility* utils)
			{
				code* code_ = nullptr;

				while (1)
				{
					utils->write(utils->read());
				}
			});
	}

	//pipeline_add_procedure(pipeline, [](utility* utils)
	//	{
	//		throw exception("good");
	//	});

	pipeline_start_async(pipeline, [](part* p)
		{
			if (!p)
				return;

			if (p->tag == tag_t::TAG_SYN)
			{
				((part_syn*)p)->prom.set_value();
				delete (part_syn*)p;
				return;
			}

			printf("%d\n", ((code*)p)->index);
			g_index = ((code*)p)->index;
			delete (code*)p;
		});

#if 1
	thread th([pipeline]()
		{
			this_thread::sleep_for(chrono::seconds(2));
			pipeline_stop_async(pipeline);
			pipeline_wait_for_idle(pipeline);
		});

	th.join();
#endif

	pipeline_wait_for_idle(pipeline);
	pipeline_delete(pipeline);

	return 0;
}
