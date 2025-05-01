#pragma once
#include <Windows.h>

class MutexLTM {
public:
	void Acquire();
	void Release();

	MutexLTM();
	~MutexLTM();
private:
	CRITICAL_SECTION m_critSec;
};