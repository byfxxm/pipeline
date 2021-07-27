#pragma once

template<class T, unsigned N>
class CRingBuffer
{
public:
	CRingBuffer() :m_nHead(0), m_nTail(0) {}

	bool Put(T data_)
	{
		if (IsFull())
			return false;

		m_Buffer[m_nTail] = data_;
		m_nTail = (m_nTail + 1) % (N + 1);
		return true;
	}

	bool Get(T& data_)
	{
		if (IsEmpty())
			return false;

		data_ = m_Buffer[m_nHead];
		m_nHead = (m_nHead + 1) % (N + 1);
		return true;
	}

	bool IsEmpty()
	{
		return m_nHead == m_nTail;
	}

	bool IsFull()
	{
		return (m_nTail + N + 2) % (N + 1) == m_nHead;
	}

	void Clear()
	{
		m_nHead = m_nTail = 0;
	}

private:
	T m_Buffer[N + 1];
	volatile unsigned m_nHead;
	volatile unsigned m_nTail;
};