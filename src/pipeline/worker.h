#pragma once
#include "pipeline.h"
#include "ring_buffer.h"

namespace pipeline
{
	using fifo = ring_buffer_c<std::shared_ptr<part_s>, 4>;
	struct quit {};

	enum class worker_state_t
	{
		WS_INVALID,
		WS_READY,
		WS_READING,
		WS_WRITING,
		WS_SYN,
		WS_QUITING,
		WS_DONE,
	};

	class worker_c
	{
	public:
		friend class pipeline_imp_c;

	public:
		worker_c(procedure_f, const std::string&);
		~worker_c();
		static void write(const std::shared_ptr<part_s>&);
		static std::shared_ptr<part_s> read();
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
		procedure_f __proc{ nullptr };
		void* __fiber{ nullptr };
		void* __main_fiber{ nullptr };
		utilities_s::read_f __read{ read };
		utilities_s::write_f __write{ write };
		worker_state_t __state{ worker_state_t::WS_INVALID };
		const std::string& __file;
	};
}