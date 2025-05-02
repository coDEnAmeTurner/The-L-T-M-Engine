#pragma once
#include <Windows.h>
#include <ScopedLock.h>

class MutexLTM {
public:
	void Acquire();
	void Release();

	MutexLTM();
	~MutexLTM();
private:
	CRITICAL_SECTION m_critSec;
};

