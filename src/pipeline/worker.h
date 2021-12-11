#pragma once
#include "pipeline.h"
#include "ring_buffer.h"

namespace pipeline
{
	using fifo = ring_buffer<std::shared_ptr<part>, 4>;
	struct quit {};

	enum class worker_state_t
	{
		WS_IDLE,
		WS_BUSY,
		WS_READING,
		WS_WRITING,
		WS_SYN,
		WS_QUITING,
	};

	class worker
	{
	public:
		friend class pipeline_imp;

	public:
		worker(procedure_func, const std::string&);
		~worker();
		static void write(const std::shared_ptr<part>&);
		static std::shared_ptr<part> read();
		static void syn();
		void asleep();
		void awake();
		void start_working(void*);
		void end_working();
		worker_state_t get_state();

	private:
		void __quit_if();

	private:
		fifo* __fifo{ new fifo() };
		fifo* __prev_fifo{ nullptr };
		procedure_func __proc{ nullptr };
		void* __fiber{ nullptr };
		void* __main_fiber{ nullptr };
		utilities::read_func __read{ read };
		utilities::write_func __write{ write };
		worker_state_t __state{ worker_state_t::WS_IDLE };
		const std::string& __file;
	};
}