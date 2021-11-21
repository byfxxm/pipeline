#pragma once
#include "pipeline.h"
#include "worker.h"

class pipeline_imp
{
public:
	~pipeline_imp();
	void start(output_func);
	void stop();
	void add_procedure(procedure_func);
	void wait_for_idle();

private:
	void __schedule();

private:
	thread __running_thread;
	vector<worker*> __worker_list;
	size_t __cur_worker{ 0 };
	void* __main_fiber{ nullptr };
	bool __stopping{ false };
};