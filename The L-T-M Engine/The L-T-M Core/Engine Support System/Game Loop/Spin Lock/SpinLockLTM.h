#pragma once
#include <atomic>

class SpinLock {
	std::atomic<std::size_t> m_atomic;
	thread_local static std::int32_t m_refcount;
public:
	SpinLock() : m_atomic(0)//, m_refcount(0) 
	{
		m_refcount = 0;
	}

	void Acquire();

	void Release();

	bool TryAcquire();

};