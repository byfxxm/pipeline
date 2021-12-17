#include <iostream>
#include <Windows.h>
#include <thread>
#include "../pipeline/pipeline.h"
#include "../procedure1/procedure1.h"
#include "../g_parser/g_parser.h"

#ifdef _WIN64
#define PLATFORM "x64"
#else
#define PLATFORM
#endif
#ifdef _DEBUG
#define CONFIG "Debug"
#else
#define CONFIG "Release"
#endif
#define LIB_DIR "../" PLATFORM "/" CONFIG "/"

#pragma comment(lib, LIB_DIR "procedure1.lib")
#pragma comment(lib, LIB_DIR "pipeline.lib")
#pragma comment(lib, LIB_DIR "g_parser.lib")

struct code_s : public part_s
{
	int index{ 0 };
};

static int g_index = 0;

int main()
{
	auto pipeline = pipeline_create();

	pipeline_add_procedure(pipeline, procedure1);

	pipeline_add_procedure(pipeline, [](utilities_s* utils)
		{
			for (int i = 0; i < 5001; ++i)
				utils->write(utils->read());
			
			throw std::exception("=========== exception");
		});

	pipeline_add_procedure(pipeline, [](utilities_s* utils)
		{
			int count = 0;

			while (1)
			{
				utils->write(utils->read());

				if (count++ % 1000 == 0)
				{
					utils->syn();
					std::cout << "=================" << g_index << std::endl;
				}
			}
		});

	for (int _i = 0; _i < 2; _i++)
	{
		pipeline_add_procedure(pipeline, [](utilities_s* utils)
			{
				while (1)
					utils->write(utils->read());
			});
	}

	pipeline_start_async(pipeline, [](const std::shared_ptr<part_s>& p)
		{
			if (!p)
				return;

			if (p->tag == tag_t::SYN)
			{
				std::static_pointer_cast<part_syn_s>(p)->prom.set_value();
				return;
			}

			auto p_ = std::static_pointer_cast<code_s>(p);
			std::cout << p_->index << std::endl;
			g_index = p_->index;
		});

#if 0
	std::thread th([pipeline]()
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			pipeline_stop_async(pipeline);
			pipeline_wait_for_idle(pipeline);
		});

	th.join();
#endif

	pipeline_wait_for_idle(pipeline);
	pipeline_delete(pipeline);

	return 0;
}