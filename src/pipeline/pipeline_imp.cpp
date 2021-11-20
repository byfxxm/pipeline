#include "stdafx.h"
#include "pipeline_imp.h"
#include "worker.h"

pipeline_imp::~pipeline_imp()
{
	stop();

	for (auto worker_ : __worker_list)
		delete worker_;
}

void pipeline_imp::start()
{
	__running_thread = thread([this]()
		{
			__main_fiber = ConvertThreadToFiber(nullptr);

			for (auto worker_ : __worker_list)
				worker_->work();

			__schedule();

			ConvertFiberToThread();
		});
}

void pipeline_imp::stop()
{
	__stopping = true;

	if (__running_thread.joinable())
		__running_thread.join();
}

void pipeline_imp::add_procedure(procedure proc)
{
	__worker_list.push_back(new worker(proc));
}

void pipeline_imp::__schedule()
{
	if (__worker_list.empty())
		return;

	__cur_worker = 0;

	while (!__stopping)
	{
		__worker_list[__cur_worker]->awake();

		switch (__worker_list[__cur_worker]->get_state())
		{
		case worker_state_t::WS_READING:
			if (__cur_worker == 0)
				throw exception("first worker shouldn't read!");

			--__cur_worker;
			break;

		case worker_state_t::WS_WRITING:
			if (__cur_worker == __worker_list.size())
				break;

			++__cur_worker;
			break;

		case worker_state_t::WS_DOING:
			assert(0);
			break;

		case worker_state_t::WS_RESTING:
		default:
			break;
		}
	}
}