#include "stdafx.h"
#include "pipeline_imp.h"
#include "worker.h"

pipeline_imp_c::~pipeline_imp_c()
{
	stop_async();
	wait_for_idle();

	for (auto worker : __worker_list)
		delete worker;
}

void pipeline_imp_c::start_async(output_f output)
{
	if (__running_thread.joinable() || __worker_list.empty() || !output)
		return;

	__worker_list.back()->__write = output;

	__running_thread = std::thread([this]()
		{
			__main_fiber = ConvertThreadToFiber(nullptr);
			{
				__start_working();
				__schedule();
				__end_working();
			}
			ConvertFiberToThread();
		});
}

void pipeline_imp_c::stop_async()
{
	__stopping = true;
}

void pipeline_imp_c::add_procedure(procedure_f proc)
{
	auto worker = new worker_c(proc, __file);

	if (!__worker_list.empty())
		worker->__prev_fifo = __worker_list.back()->__fifo;

	__worker_list.push_back(worker);
}

void pipeline_imp_c::wait_for_idle()
{
	if (__running_thread.joinable())
		__running_thread.join();
}

void pipeline_imp_c::set_file(const char* file)
{
	__file = file;
}
void pipeline_imp_c::reset_file()
{
	__file.clear();
}

void pipeline_imp_c::__schedule()
{
	if (__worker_list.empty())
		return;

	__cur_worker = 0;

	while (!__stopping)
	{
		if (__cur_worker == __worker_list.size())
		{
			intptr_t index = __worker_list.size() - 1;
			for (; index >= 0; --index)
			{
				auto state = __worker_list[index]->get_state();
				if (state == worker_state_t::DONE)
					return;

				if (state == worker_state_t::SYN)
					break;
			}

			if (index < 0)
				return;

			__cur_worker = index;
		}

		__worker_list[__cur_worker]->awake();

		switch (__worker_list[__cur_worker]->get_state())
		{
		case worker_state_t::READING:
			if (__cur_worker == 0)
				throw std::exception("first procedure shouldn't read");

			switch (__worker_list[__cur_worker - 1]->get_state())
			{
			case worker_state_t::READING:
			case worker_state_t::DONE:
			case worker_state_t::SYN:
				++__cur_worker;
				break;

			default:
				--__cur_worker;
				break;
			}

			break;

		case worker_state_t::WRITING:
		case worker_state_t::DONE:
		case worker_state_t::SYN:
			++__cur_worker;
			break;

		default:
			assert(0);
			break;
		}
	}
}

inline void pipeline_imp_c::__start_working()
{
	for (auto worker : __worker_list)
		worker->start_working(__main_fiber);
}

inline void pipeline_imp_c::__end_working()
{
	for (auto worker : __worker_list)
		worker->end_working();
}