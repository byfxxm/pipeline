#pragma once
#include "pipeline.h"
#include "worker.h"

class pipeline_imp
{
public:
	~pipeline_imp();
	void start_async(output_func);
	void stop_async();
	void add_procedure(procedure_func);
	void wait_for_idle();

private:
	void __schedule(size_t = 0);

private:
	thread __running_thread;
	vector<worker*> __worker_list;
	size_t __cur_worker_index{ 0 };
	void* __main_fiber{ nullptr };
	bool __stopping{ false };
};