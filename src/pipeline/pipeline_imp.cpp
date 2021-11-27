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
	auto worker_ = new worker(proc);

	if (!__worker_list.empty())
		worker_->__prev_fifo = __worker_list.back()->__fifo;

	__worker_list.push_back(worker_);
}

void pipeline_imp::wait_for_idle()
{
	if (__running_thread.joinable())
		__running_thread.join();
}

void pipeline_imp::__schedule(size_t index)
{
	if (__worker_list.empty())
		return;

	__cur_worker_index = index;

	while (!__stopping)
	{
		if (__cur_worker_index == __worker_list.size())
			break;

		__worker_list[__cur_worker_index]->awake();

		switch (__worker_list[__cur_worker_index]->get_state())
		{
		case worker_state_t::WS_READING:
			if (__cur_worker_index <= index || __worker_list[__cur_worker_index - 1]->get_state() == worker_state_t::WS_IDLE)
			{
				__worker_list[__cur_worker_index]->__state = worker_state_t::WS_IDLE;
				++__cur_worker_index;
				break;
			}

			--__cur_worker_index;
			break;

		case worker_state_t::WS_WRITING:
		case worker_state_t::WS_IDLE:
			++__cur_worker_index;
			break;

		case worker_state_t::WS_QUIT:
			break;

		default:
			break;
		}
	}
}