#pragma once

template<class T>
class CRingBuffer
{
public:
	CRingBuffer(int nSize_)
	{
		m_nHead = m_nTail = 0;
		m_nBufferSize = nSize_;
		m_pBuffer = new T[m_nBufferSize];
	}

	~CRingBuffer()
	{
		delete[] m_pBuffer;
	}

	bool Put(T data_)
	{
		if (IsFull())
			return false;

		m_pBuffer[m_nTail] = data_;
		m_nTail = (m_nTail + 1) % m_nBufferSize;
		return true;
	}

	bool Get(T& data_)
	{
		if (IsEmpty())
			return false;

		data_ = m_pBuffer[m_nHead];
		m_nHead = (m_nHead + 1) % m_nBufferSize;
		return true;
	}

	bool IsEmpty()
	{
		return m_nHead == m_nTail;
	}

	bool IsFull()
	{
		return (m_nTail + 1) % m_nBufferSize == m_nHead;
	}

	void Clear()
	{
		m_nHead = m_nTail = 0;
	}

private:
	T* m_pBuffer;
	int m_nBufferSize;
	volatile unsigned m_nHead;
	volatile unsigned m_nTail;
};