#include "ThreadLTM.h"

ThreadLTM::ThreadLTM()
	: m_masterJob(nullptr), m_queue(nullptr), m_thread(0), m_stack(nullptr), m_doubleEndedStack(nullptr), m_doubleBuffers(nullptr)
{

}

ThreadLTM::ThreadLTM(std::shared_ptr<JobQueue> queue, std::uint8_t core_id, std::shared_ptr<std::mutex> m_mutexThread, std::shared_ptr<std::condition_variable> m_condVarQueue, std::shared_ptr<bool> m_queueReady)
	:m_coreID(core_id), m_queue(queue), m_masterJob(nullptr),
	m_mutexThread(m_mutexThread), m_condVarQueue(m_condVarQueue),
	m_queueReady(m_queueReady)
{
	assert(queue != nullptr);

	m_stack = std::shared_ptr<StackAllocator>(new StackAllocator(WORD_SIZE * pow_of_2(17), MINIMUM_ALIGNMENT)); //1MB
	m_doubleEndedStack = std::shared_ptr<DoubleEndedStackAllocator>(new DoubleEndedStackAllocator(WORD_SIZE * pow_of_2(18), MINIMUM_ALIGNMENT)); //2MB
	m_doubleBuffers = std::shared_ptr<DoubledBufferedAllocator>(new DoubledBufferedAllocator(WORD_SIZE * pow_of_2(17), MINIMUM_ALIGNMENT)); //1MB

	m_thread = std::shared_ptr<std::thread>(new std::thread(&ThreadLTM::entryPointThread, this));
}

void ThreadLTM::destroy()
{
	assert(m_thread != nullptr);

	m_thread->join();
}

void ThreadLTM::entryPointThread()
{
	m_threadID = GetCurrentThreadId();

	set_thread_affinity(GetCurrentThread(), m_coreID);

	ConvertThreadToFiber(nullptr);

	while (true) {
		ScopedLock<SpinLockLTM> lock(&m_lockRunning);
		bool running = GameLoopManager::getRunning();
		if (!running) {
			break;
		}

		this->m_doubleBuffers->swapBuffers();
		this->m_stack->clear();
		this->m_doubleEndedStack->clear();
		this->m_doubleBuffers->clearCurrentBuffer();

		//pop queue
		if (m_masterFiber == nullptr && m_masterJob == nullptr)
		{
			std::unique_lock<std::mutex> lock(*m_mutexThread);
			m_condVarQueue->wait(lock, [&]() {return *m_queueReady; });

			this->m_masterJob = m_queue->Pop();
			lock.release();
			*m_queueReady = false;

			m_masterJob->m_params->m_doubleBuffers = m_doubleBuffers;
			m_masterJob->m_params->m_doubleEndedStack = m_doubleEndedStack;
			m_masterJob->m_params->m_stack = m_stack;
			m_masterJob->m_params->m_fiberParent = GetCurrentFiber();

			FiberLTM fiber(m_masterJob);
			m_masterFiber = &fiber;
			FiberLTM::switchToFiber(fiber.getRef());
			
		}
		
	}

	ConvertFiberToThread();

}




