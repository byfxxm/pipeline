#pragma once
#include "pipeline.h"
#include "ring_buffer.h"

using fifo = ring_buffer<part*, 4>;

enum class worker_state_t
{
	WS_IDLE,
	WS_READING,
	WS_WRITING,
};

class worker
{
public:
	worker(procedure_func);
	~worker();
	static void write(part*);
	static part* read();
	void asleep();
	void awake();
	void start_working(void*);
	void end_working();
	worker_state_t get_state();

private:
	friend class pipeline_imp;
	fifo* __next{ new fifo() };
	fifo* __prev{ nullptr };
	procedure_func __proc{ nullptr };
	void* __fiber{ nullptr };
	void* __main_fiber{ nullptr };
	worker_state_t __state{ worker_state_t::WS_IDLE };
	read_func __read{ read };
	write_func __write{ write };
};
