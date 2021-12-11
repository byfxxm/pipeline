#include "stdafx.h"
#include "pipeline.h"
#include "worker.h"

worker::worker(procedure_func proc, const std::string& file) : __proc(proc), __file(file) {}

worker::~worker()
{
	delete __fifo;
}

void worker::asleep()
{
	assert(IsThreadAFiber());
	SwitchToFiber(__main_fiber);
	__quit_if();
}

void worker::awake()
{
	assert(IsThreadAFiber());
	SwitchToFiber(__fiber);
}

void worker::write(const std::shared_ptr<part>& part_)
{
	assert(IsThreadAFiber());
	auto this_worker = (worker*)GetFiberData();

	while (!this_worker->__fifo->write(part_))
	{
		this_worker->__state = worker_state_t::WS_WRITING;
		this_worker->asleep();
	}
}

std::shared_ptr<part> worker::read()
{
	assert(IsThreadAFiber());
	auto this_worker = (worker*)GetFiberData();
	std::shared_ptr<part> ret;

	while (!this_worker->__prev_fifo->read(ret))
	{
		this_worker->__state = worker_state_t::WS_READING;
		this_worker->asleep();
	}

	return ret;
}

void worker::syn()
{
	assert(IsThreadAFiber());
	auto this_worker = (worker*)GetFiberData();

	auto part_syn_ = std::make_shared<part_syn>();
	auto fu = part_syn_->prom.get_future();
	this_worker->write(part_syn_);

	this_worker->__state = worker_state_t::WS_SYN;
	this_worker->asleep();
	fu.wait();
	this_worker->__state = worker_state_t::WS_BUSY;
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
				this_worker->__quit_if();
				this_worker->__state = worker_state_t::WS_BUSY;
				utilities util{ this_worker->__read,  this_worker->__write, worker::syn, this_worker->__file };
				this_worker->__proc(&util);
			}
			catch (quit)
			{
			}
			catch (std::exception ex)
			{
				printf("%s\n", ex.what());
			}
			catch (...)
			{
				printf("unknown error!\n");
			}

			this_worker->__state = worker_state_t::WS_IDLE;
			this_worker->asleep();
		}, this);
}

void worker::end_working()
{
	if (__state != worker_state_t::WS_IDLE)
	{
		__state = worker_state_t::WS_QUITING;
		awake();
		assert(__state == worker_state_t::WS_IDLE);
	}

	assert(__fiber);
	DeleteFiber(__fiber);
	__fiber = nullptr;
}

worker_state_t worker::get_state()
{
	return __state;
}

inline void worker::__quit_if()
{
	if (__state == worker_state_t::WS_QUITING)
		throw quit();
}