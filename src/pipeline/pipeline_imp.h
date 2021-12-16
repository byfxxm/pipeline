#pragma once
#include "pipeline.h"
#include "worker.h"

namespace pipeline
{
	class pipeline_imp_c
	{
	public:
		~pipeline_imp_c();
		void start_async(output_f);
		void stop_async();
		void add_procedure(procedure_f);
		void wait_for_idle();
		void set_file(const char*);
		void reset_file();

	private:
		void __schedule();
		void __start_working();
		void __end_working();

	private:
		std::thread __running_thread;
		std::vector<worker_c*> __worker_list;
		size_t __cur_worker{ 0 };
		void* __main_fiber{ nullptr };
		bool __stopping{ false };
		std::string __file;
	};
}