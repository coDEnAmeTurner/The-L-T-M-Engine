#include "JobQueue.h"

JobQueue::JobQueue()
{
}

void JobQueue::Push(JobDeclaration* job)
{
	assert(job != nullptr);

	ScopedLock<SpinLock> lock(&m_lock);
	m_queue.push(job);
}

JobDeclaration* JobQueue::Pop()
{
	m_lock.Acquire();
	auto job = m_queue.front();
	m_lock.Release();

	return job;
}
