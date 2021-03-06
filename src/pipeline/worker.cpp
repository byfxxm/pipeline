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

void worker_c::write(const std::shared_ptr<part_s>& part_)
{
	assert(IsThreadAFiber());
	auto this_worker = (worker_c*)GetFiberData();

	while (!this_worker->__fifo->write(part_))
	{
		this_worker->__state = worker_state_t::WRITING;
		this_worker->asleep();
	}
}

std::shared_ptr<part_s> worker_c::read()
{
	assert(IsThreadAFiber());
	auto this_worker = (worker_c*)GetFiberData();
	std::shared_ptr<part_s> ret;

	while (!this_worker->__prev_fifo->read(ret))
	{
		this_worker->__state = worker_state_t::READING;
		this_worker->asleep();
	}

	return ret;
}

void worker_c::syn()
{
	assert(IsThreadAFiber());
	auto this_worker = (worker_c*)GetFiberData();

	auto part_syn = std::make_shared<part_syn_s>();
	auto fu = part_syn->prom.get_future();
	this_worker->write(part_syn);

	this_worker->__state = worker_state_t::SYN;
	this_worker->asleep();
	fu.wait();
}

void worker_c::start_working(void* main_fiber)
{
	assert(!__fiber);
	__main_fiber = main_fiber;
	__state = worker_state_t::READY;

	__fiber = CreateFiber(0, [](void* p)
		{
			auto this_worker = (worker_c*)p;

			try
			{
				this_worker->__quit_if();
				utilities_s utils{ this_worker->__read,  this_worker->__write, worker_c::syn, this_worker->__file.c_str() };
				this_worker->__proc(&utils);
			}
			catch (quit_s)
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

			this_worker->__state = worker_state_t::DONE;
			this_worker->asleep();
		}, this);
}

void worker_c::end_working()
{
	if (__state != worker_state_t::DONE)
	{
		__state = worker_state_t::QUITING;
		awake();
		assert(__state == worker_state_t::DONE);
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
	if (__state == worker_state_t::QUITING)
		throw quit_s();
}