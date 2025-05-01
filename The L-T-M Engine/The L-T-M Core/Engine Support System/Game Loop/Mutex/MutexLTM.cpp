#include "MutexLTM.h"

void MutexLTM::Acquire()
{
	EnterCriticalSection(&m_critSec);
}

void MutexLTM::Release()
{
	LeaveCriticalSection(&m_critSec);
}

MutexLTM::MutexLTM()
{
	InitializeCriticalSection(&m_critSec);
}

MutexLTM::~MutexLTM()
{
	DeleteCriticalSection(&m_critSec);
}
