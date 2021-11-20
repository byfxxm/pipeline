#include "stdafx.h"
#include "pipeline.h"
#include "worker.h"

worker::worker(procedure proc)
	:__proc_f(proc)
{
}

inline void worker::asleep()
{
	SwitchToFiber(__main_fiber);
}

void worker::awake()
{
	SwitchToFiber(__fiber);
	__state = worker_state_t::WS_DOING;
}

void worker::write(part* part_)
{
	assert(IsThreadAFiber());
	auto this_ = (worker*)GetFiberData();

	while (!this_->__next->write(part_))
	{
		this_->__state = worker_state_t::WS_WRITING;
		this_->asleep();
	}
}

part* worker::read()
{
	assert(IsThreadAFiber());
	auto this_ = (worker*)GetFiberData();

	part* ret{ nullptr };
	while (!this_->__prev->read(ret))
	{
		this_->__state = worker_state_t::WS_READING;
		this_->asleep();
	}

	return ret;
}

void worker::work()
{
	if (__fiber)
		DeleteFiber(__fiber);

	__fiber = CreateFiber(0, [](void* p)
		{
			auto this_ = (worker*)p;

			try
			{
				this_->__proc_f(worker::read, worker::write);
			}
			catch (...)
			{

			}

			this_->asleep();
		}, this);
}

worker_state_t worker::get_state()
{
	return __state;
}