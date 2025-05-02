#include "JobQueue.h"

JobQueue::JobQueue()
{
}

void JobQueue::Push(JobDeclaration* job)
{
	assert(job != nullptr);

	m_queue.enqueue(job);
}

JobDeclaration* JobQueue::Pop()
{
	JobDeclaration* job = nullptr;
	if (m_queue.size_approx() != 0)
		m_queue.try_dequeue(job);

	return job;
}
