#include "stdafx.h"
#include "pipeline_imp.h"
#include "worker.h"

pipeline_imp::~pipeline_imp()
{
	stop_async();
	wait_for_idle();

	for (auto worker_ : __worker_list)
		delete worker_;
}

void pipeline_imp::start_async(output_func output)
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

void pipeline_imp::stop_async()
{
	__stopping = true;
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

void pipeline_imp::__schedule()
{
	if (__worker_list.empty())
		return;

	__cur_worker_index = 0;

	while (!__stopping)
	{
		if (__cur_worker_index == __worker_list.size())
		{
			size_t index = 0;
			for (; index < __worker_list.size(); ++index)
			{
				if (__worker_list[index]->get_state() == worker_state_t::WS_SYN)
					break;
			}

			if (index != __worker_list.size())
			{
				__cur_worker_index = index;
				continue;
			}

			break;
		}

		__worker_list[__cur_worker_index]->awake();

		switch (__worker_list[__cur_worker_index]->get_state())
		{
		case worker_state_t::WS_READING:
			if (__cur_worker_index == 0
				|| __worker_list[__cur_worker_index - 1]->get_state() == worker_state_t::WS_READY
				|| __worker_list[__cur_worker_index - 1]->get_state() == worker_state_t::WS_SYN
				|| __worker_list[__cur_worker_index - 1]->get_state() == worker_state_t::WS_DONE)
			{
				__worker_list[__cur_worker_index]->__state = worker_state_t::WS_READY;
				++__cur_worker_index;
				break;
			}

			--__cur_worker_index;
			break;

		case worker_state_t::WS_WRITING:
		case worker_state_t::WS_READY:
		case worker_state_t::WS_SYN:
		case worker_state_t::WS_DONE:
			++__cur_worker_index;
			break;

		case worker_state_t::WS_QUITING:
			break;

		default:
			break;
		}
	}
}