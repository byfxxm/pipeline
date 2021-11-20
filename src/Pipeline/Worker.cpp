#include "stdafx.h"
#include "pipeline.h"
#include "worker.h"

worker::worker(procedure_func proc)
	:__proc(proc)
{
}

worker::~worker()
{
	delete __next;
}

void worker::asleep()
{
	SwitchToFiber(__main_fiber);
}

void worker::awake()
{
	SwitchToFiber(__fiber);
}

void worker::write(part* part_)
{
	assert(IsThreadAFiber());
	auto this_worker = (worker*)GetFiberData();

	while (!this_worker->__next->write(part_))
	{
		this_worker->__state = worker_state_t::WS_WRITING;
		this_worker->asleep();
	}
}

part* worker::read()
{
	assert(IsThreadAFiber());
	auto this_worker = (worker*)GetFiberData();

	part* ret{ nullptr };
	while (!this_worker->__prev->read(ret))
	{
		this_worker->__state = worker_state_t::WS_READING;
		this_worker->asleep();
	}

	return ret;
}

void worker::start_working(void* main_fiber)
{
	assert(!__fiber);
	__main_fiber = main_fiber;

	__fiber = CreateFiber(0, [](void* p)
		{
			auto this_worker = (worker*)p;

			try
			{
				this_worker->__proc(this_worker->__read, this_worker->__write);
			}
			catch (...)
			{
				printf("error");
			}

			this_worker->__state = worker_state_t::WS_IDLE;
			this_worker->asleep();
		}, this);
}

void worker::end_working()
{
	if (__state == worker_state_t::WS_IDLE)
		return;

	if (!__prev->write(nullptr))
		assert(0);

	awake();

	if (__fiber)
	{
		DeleteFiber(__fiber);
		__fiber = nullptr;
	}
}

worker_state_t worker::get_state()
{
	return __state;
}