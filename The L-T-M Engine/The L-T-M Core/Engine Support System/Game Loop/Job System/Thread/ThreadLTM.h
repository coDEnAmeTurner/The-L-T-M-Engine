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
#include <SpinLockLTM.h>
#include <ScopedLock.h>
#include <GameLoopManager.h>
#include <string>

class ThreadLTM {
public:
	ThreadLTM();
	ThreadLTM(std::shared_ptr<JobQueue> queue, std::uint8_t core_id, std::shared_ptr<std::mutex> m_mutexThread,std::shared_ptr<std::condition_variable> m_condVarThread);

	void destroy();
private:
	std::shared_ptr<std::thread> m_thread = nullptr;
	std::uint32_t m_threadID = 0;
	std::uint8_t m_coreID = 0;

	FiberLTM* m_masterFiber = nullptr;
	JobDeclaration* m_masterJob;
	std::shared_ptr<JobQueue> m_queue;
	std::shared_ptr<StackAllocator> m_stack;
	std::shared_ptr<DoubleEndedStackAllocator> m_doubleEndedStack;
	std::shared_ptr<DoubledBufferedAllocator> m_doubleBuffers;
	std::atomic<std::shared_ptr<bool>> m_finished = 0; //pass to JobParams so that fiber entry func can notify this thread

	SpinLockLTM m_lockRunning;
	SpinLockLTM m_lockFinished;
	std::shared_ptr<std::mutex> m_mutexThread = nullptr;
	std::shared_ptr<std::condition_variable> m_condVarThread = nullptr;
	
	void entryPoint();
};