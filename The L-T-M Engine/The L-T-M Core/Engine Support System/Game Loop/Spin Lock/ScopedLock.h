#pragma once
template<class LOCK>
class ScopedLock
{
	typedef LOCK lock_t;
	lock_t* m_mutex;

public:
	explicit ScopedLock(lock_t* lock)
		: m_mutex(lock)
	{
		m_mutex->Acquire();
	}

	~ScopedLock()
	{
		m_mutex->Release();
	}
};

