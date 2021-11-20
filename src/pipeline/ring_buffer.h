#pragma once

template<typename T, unsigned N>
class ring_buffer
{
public:
	bool write(const T& data)
	{
		if (is_full())
			return false;

		__buffer[__tail] = data;
		__tail = (__tail + 1) % N;
		return true;
	}

	bool read(T& data)
	{
		if (is_empty())
			return false;

		data = __buffer[__head];
		__head = (__head + 1) % N;
		return true;
	}

	bool is_empty()
	{
		return __head == __tail;
	}

	bool is_full()
	{
		return (__tail + 1) % N == __head;
	}

	void clear()
	{
		__head = __tail;
	}

private:
	T __buffer[N];
	volatile unsigned __head{ 0 };
	volatile unsigned __tail{ 0 };
};