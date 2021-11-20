#pragma once
#include "pipeline.h"
#include "worker.h"

class pipeline_imp
{
public:
	~pipeline_imp();
	void start();
	void stop();
	void add_procedure(procedure);

private:
	void __schedule();

private:
	thread __running_thread;
	vector<worker*> __worker_list;
	int __cur_worker{ 0 };
	void* __main_fiber{ nullptr };
	bool __stopping{ false };
};