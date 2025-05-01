#pragma once
#include <memory>
#include <thread>
#include <StackAllocator.h>
#include <DoubleEndedStackAllocator.h>
#include <DoubleBufferedAllocator.h>
#include <PoolAllocator.h>
#include <Job Properties.h>
#include <JobQueue.h>
#include <Windows.h>
#include <FiberLTM.h>

class ThreadLTM {
public:
	ThreadLTM();
	ThreadLTM(std::shared_ptr<JobQueue> queue);

private:
	std::uint32_t m_threadID = 0;
	FiberLTM* m_masterFiber = nullptr;
	JobDeclaration* m_job;
	std::shared_ptr<JobQueue> m_queue;
	std::shared_ptr<StackAllocator> m_stack;
	std::shared_ptr<DoubleEndedStackAllocator> m_doubleEndedStack;
	std::shared_ptr<DoubledBufferedAllocator> m_doubleBuffers;
	bool m_finished = false; //pass to JobParams so that fiber entry func can notify this thread

	void entryPoint();
};