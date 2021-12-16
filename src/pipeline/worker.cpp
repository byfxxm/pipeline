#include "stdafx.h"
#include "pipeline.h"
#include "worker.h"

worker_c::worker_c(procedure_f proc, const std::string& file) : __proc(proc), __file(file) {}

worker_c::~worker_c()
{
	delete __fifo;
}

void worker_c::asleep()
{
	assert(IsThreadAFiber());
	SwitchToFiber(__main_fiber);
	__quit_if();
}

void worker_c::awake()
{
	assert(IsThreadAFiber());
	SwitchToFiber(__fiber);
}

void worker_c::write(const std::shared_ptr<part>& part_)
{
	assert(IsThreadAFiber());
	auto this_worker = (worker_c*)GetFiberData();

	while (!this_worker->__fifo->write(part_))
	{
		this_worker->__state = worker_state_t::WS_WRITING;
		this_worker->asleep();
	}
}

std::shared_ptr<part> worker_c::read()
{
	assert(IsThreadAFiber());
	auto this_worker = (worker_c*)GetFiberData();
	std::shared_ptr<part> ret;

	while (!this_worker->__prev_fifo->read(ret))
	{
		this_worker->__state = worker_state_t::WS_READING;
		this_worker->asleep();
	}

	return ret;
}

void worker_c::syn()
{
	assert(IsThreadAFiber());
	auto this_worker = (worker_c*)GetFiberData();

	auto part_syn_ = std::make_shared<part_syn>();
	auto fu = part_syn_->prom.get_future();
	this_worker->write(part_syn_);

	this_worker->__state = worker_state_t::WS_SYN;
	this_worker->asleep();
	fu.wait();
}

void worker_c::start_working(void* main_fiber)
{
	assert(!__fiber);
	__main_fiber = main_fiber;
	__state = worker_state_t::WS_READY;

	__fiber = CreateFiber(0, [](void* p)
		{
			auto this_worker = (worker_c*)p;

			try
			{
				this_worker->__quit_if();
				utilities util{ this_worker->__read,  this_worker->__write, worker_c::syn, this_worker->__file.c_str() };
				this_worker->__proc(&util);
			}
			catch (quit)
			{
			}
			catch (std::exception ex)
			{
				std::cout << ex.what() << std::endl;
			}
			catch (...)
			{
				std::cout << "unknown error!\n" << std::endl;
				throw;
			}

			this_worker->__state = worker_state_t::WS_DONE;
			this_worker->asleep();
		}, this);
}

void worker_c::end_working()
{
	if (__state != worker_state_t::WS_DONE)
	{
		__state = worker_state_t::WS_QUITING;
		awake();
		assert(__state == worker_state_t::WS_DONE);
	}

	assert(__fiber);
	DeleteFiber(__fiber);
	__fiber = nullptr;
}

worker_state_t worker_c::get_state()
{
	return __state;
}

inline void worker_c::__quit_if()
{
	if (__state == worker_state_t::WS_QUITING)
		throw quit();
}