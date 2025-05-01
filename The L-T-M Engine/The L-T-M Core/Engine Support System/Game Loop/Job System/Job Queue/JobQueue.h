#pragma once
#include <queue>
#include <ScopedLock.h>
#include <SpinLockLTM.h>
#include <Job Properties.h>

class JobQueue
{
public:
	JobQueue();
	void Push(JobDeclaration* job);
	JobDeclaration* Pop();
private:
	SpinLock m_lock;
	std::queue<JobDeclaration*> m_queue;
};

