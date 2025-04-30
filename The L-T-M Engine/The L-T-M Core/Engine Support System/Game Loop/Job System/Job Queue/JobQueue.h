#pragma once
#include <queue>
#include <memory>
#include <Job Properties.h>

class JobQueue
{
public:
	JobQueue();
	void Push();
	std::shared_ptr<JobDeclaration> Pop();
private:
	std::queue<std::shared_ptr<JobDeclaration>> m_queue;
};

