#include "stdafx.h"
#include "pipeline.h"
#include "worker.h"

worker::worker(procedure_func proc, size_t& cur_worker)
	:__proc(proc), __cur_worker(cur_worker)
{
}

worker::~worker()
{
	delete __fifo;
}

void worker::asleep()
{
	SwitchToFiber(__main_fiber);

	if (__state == worker_state_t::WS_QUIT)
		throw quit();
}

void worker::awake()
{
	SwitchToFiber(__fiber);
}

void worker::write(part* part_)
{
	assert(IsThreadAFiber());
	auto this_worker = (worker*)GetFiberData();

	while (!this_worker->__fifo->write(part_))
	{
		++this_worker->__cur_worker;
		this_worker->asleep();
	}
}

part* worker::read()
{
	assert(IsThreadAFiber());
	auto this_worker = (worker*)GetFiberData();
	part* ret{ nullptr };

	while (!this_worker->__last_worker->__fifo->read(ret))
	{
		if (this_worker->__last_worker->get_state() == worker_state_t::WS_IDLE)
			return nullptr;

		--this_worker->__cur_worker;
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
				this_worker->__state = worker_state_t::WS_BUSY;
				this_worker->__proc(this_worker->__read, this_worker->__write);
			}
			catch (quit)
			{
			}
			catch (...)
			{
				printf("unknown error!");
				throw;
			}

			this_worker->__state = worker_state_t::WS_IDLE;
			++this_worker->__cur_worker;
			this_worker->asleep();
		}, this);
}

void worker::end_working()
{
	if (__state == worker_state_t::WS_IDLE)
		return;

	__state = worker_state_t::WS_QUIT;
	awake();
	assert(__state == worker_state_t::WS_IDLE);

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