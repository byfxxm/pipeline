#pragma once
#include "pipeline.h"
#include "ring_buffer.h"

using fifo = ring_buffer<part*, 4>;

enum class worker_state_t
{
	WS_RESTING,
	WS_DOING,
	WS_READING,
	WS_WRITING,
};

class worker
{
public:
	worker(procedure);
	static void write(part*);
	static part* read();
	void asleep();
	void awake();
	void work();
	worker_state_t get_state();

private:
	fifo* __next{ nullptr };
	fifo* __prev{ nullptr };
	procedure __proc_f{ nullptr };
	void* __fiber{ nullptr };
	void* __main_fiber{ nullptr };
	worker_state_t __state{ worker_state_t::WS_RESTING };
};
