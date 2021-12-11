#pragma once
#include "pipeline.h"
#include "worker.h"

namespace pipeline
{
	class pipeline_imp
	{
	public:
		~pipeline_imp();
		void start_async(output_func);
		void stop_async();
		void add_procedure(procedure_func);
		void wait_for_idle();
		void set_file(const char*);
		void reset_file();

	private:
		void __schedule();
		void __start_working();
		void __end_working();

	private:
		std::thread __running_thread;
		std::vector<worker*> __worker_list;
		size_t __cur_worker{ 0 };
		void* __main_fiber{ nullptr };
		bool __stopping{ false };
		std::string __file;
	};
}