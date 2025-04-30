#pragma once
#include <memory>
#include <Job Properties.h>
#include <JobQueue.h>

class ThreadLTM {
public:
	ThreadLTM();
	ThreadLTM(std::shared_ptr<JobQueue> queue);

private:
	size_t m_threadID;
	void* entryPoint(std::shared_ptr<JobDeclaration> job);
};