#pragma once
#include <memory>
#include <thread>
#include <StackAllocator.h>
#include <DoubleEndedStackAllocator.h>
#include <DoubleBufferedAllocator.h>
#include <PoolAllocator.h>
#include <Job Properties.h>
#include <JobQueue.h>

class ThreadLTM {
public:
	ThreadLTM();
	ThreadLTM(std::shared_ptr<JobQueue> queue);

private:
	size_t m_threadID;
	std::shared_ptr<JobQueue> m_queue;
	std::shared_ptr<StackAllocator> m_stack;
	std::shared_ptr<DoubleEndedStackAllocator> m_doubleEndedStack;
	std::shared_ptr<DoubledBufferedAllocator> m_doubleBuffers;

	void entryPoint();
};