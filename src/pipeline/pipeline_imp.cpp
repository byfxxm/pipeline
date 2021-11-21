#include "stdafx.h"
#include "pipeline_imp.h"
#include "worker.h"

pipeline_imp::~pipeline_imp()
{
	stop();

	for (auto worker_ : __worker_list)
		delete worker_;
}

void pipeline_imp::start(output_func output)
{
	if (__running_thread.joinable() || __worker_list.empty())
		return;

	__worker_list.back()->__write = output;

	__running_thread = thread([this]()
		{
			__main_fiber = ConvertThreadToFiber(nullptr);

			for (auto worker_ : __worker_list)
				worker_->start_working(__main_fiber);

			__schedule();

			for (auto worker_ : __worker_list)
				worker_->end_working();

			ConvertFiberToThread();
		});
}

void pipeline_imp::stop()
{
	__stopping = true;
	wait_for_idle();
}

void pipeline_imp::add_procedure(procedure_func proc)
{
	auto worker_ = new worker(proc, __cur_worker);

	if (!__worker_list.empty())
		worker_->__last_worker = __worker_list.back();

	__worker_list.push_back(worker_);
}

void pipeline_imp::wait_for_idle()
{
	if (__running_thread.joinable())
		__running_thread.join();
}

void pipeline_imp::__schedule()
{
	if (__worker_list.empty())
		return;

	__cur_worker = 0;

	while (!__stopping && __cur_worker < __worker_list.size())
		__worker_list[__cur_worker]->awake();
}