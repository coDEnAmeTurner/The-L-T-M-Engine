#include "JobQueue.h"

JobQueue::JobQueue()
{
}

void JobQueue::Push(std::shared_ptr<JobDeclaration> job)
{
	assert(job != nullptr);

	m_queue.push(job);
	//m_queue.enqueue(job);
}

std::shared_ptr<JobDeclaration> JobQueue::Pop()
{
	std::shared_ptr<JobDeclaration> job = nullptr;
	/*if (!m_queue.empty())
		while (!m_queue.try_pop(job)) {}*/

	/*if (m_queue.size_approx() > 0)
		while (!m_queue.try_dequeue(job)) {}*/

	if (!m_queue.empty())
	{
		job = m_queue.front();
		m_queue.pop();
	}

	return job;
}
