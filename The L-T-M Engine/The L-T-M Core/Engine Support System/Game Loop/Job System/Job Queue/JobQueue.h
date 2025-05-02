#pragma once
#include <concurrentqueue.h>
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
	SpinLockLTM m_mutex;
	moodycamel::ConcurrentQueue<JobDeclaration*> m_queue;
};

