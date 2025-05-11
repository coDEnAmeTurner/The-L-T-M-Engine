#pragma once
#include <concurrentqueue.h>
#include <concurrent_queue.h>
#include <queue>
#include <ScopedLock.h>
#include <SpinLockLTM.h>
#include <Job Properties.h>

class JobQueue
{
public:
	JobQueue();
	void Push(std::shared_ptr<JobDeclaration> job);
	std::shared_ptr<JobDeclaration> Pop();
private:
	SpinLockLTM m_mutex;
	//moodycamel::ConcurrentQueue<JobDeclaration*> m_queue;
	//concurrency::concurrent_queue<std::shared_ptr<JobDeclaration>> m_queue;
	std::queue<std::shared_ptr<JobDeclaration>> m_queue;
};

