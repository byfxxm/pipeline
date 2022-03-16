#pragma once

namespace pipeline
{
	template<typename T, unsigned N>
	class ring_buffer_c
	{
	public:
		bool write(const T& data)
		{
			if (is_full())
				return false;

			__buffer[__write_index] = data;
			__write_index = (__write_index + 1) % N;
			return true;
		}

		bool read(T& data)
		{
			if (is_empty())
				return false;

			data = __buffer[__read_index];
			__read_index = (__read_index + 1) % N;
			return true;
		}

		bool is_empty()
		{
			return __read_index == __write_index;
		}

		bool is_full()
		{
			return (__write_index + 1) % N == __read_index;
		}

		void clear()
		{
			__read_index = __write_index;
		}

	private:
		T __buffer[N];
		volatile unsigned __read_index{ 0 };
		volatile unsigned __write_index{ 0 };
	};
}