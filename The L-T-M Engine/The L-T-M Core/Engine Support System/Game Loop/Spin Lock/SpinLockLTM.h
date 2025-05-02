#pragma once
#include <atomic>
#include <ScopedLock.h>

class SpinLockLTM {
	std::atomic<std::size_t> m_atomic;
	thread_local static std::int32_t m_refcount;
public:
	SpinLockLTM() : m_atomic(0)//, m_refcount(0) 
	{
		m_refcount = 0;
	}

	void Acquire();

	void Release();

	bool TryAcquire();

};

