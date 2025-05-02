#include "ThreadLTM.h"

ThreadLTM::ThreadLTM()
	: m_finished(std::shared_ptr<bool>(new bool(0))), m_masterJob(nullptr), m_queue(nullptr), m_thread(0), m_stack(nullptr), m_doubleEndedStack(nullptr), m_doubleBuffers(nullptr)
{

}

ThreadLTM::ThreadLTM(std::shared_ptr<JobQueue> queue, std::uint8_t core_id, std::shared_ptr<std::mutex> m_mutexThread, std::shared_ptr<std::condition_variable> m_condVarThread)
	:m_finished(std::shared_ptr<bool>(new bool(0))), m_coreID(core_id), m_queue(queue), m_masterJob(nullptr),
	m_mutexThread(m_mutexThread), m_condVarThread(m_condVarThread)
{
	assert(queue != nullptr);

	m_stack = std::shared_ptr<StackAllocator>(new StackAllocator(WORD_SIZE * pow_of_2(17), MINIMUM_ALIGNMENT));
	m_doubleEndedStack = std::shared_ptr<DoubleEndedStackAllocator>(new DoubleEndedStackAllocator(WORD_SIZE * pow_of_2(18), MINIMUM_ALIGNMENT));
	m_doubleBuffers = std::shared_ptr<DoubledBufferedAllocator>(new DoubledBufferedAllocator(WORD_SIZE * pow_of_2(17), MINIMUM_ALIGNMENT));

	m_thread = std::shared_ptr<std::thread>(new std::thread(&ThreadLTM::entryPoint, this));
}

void ThreadLTM::destroy()
{
	assert(m_thread != nullptr);

	m_thread->join();
}

void ThreadLTM::entryPoint()
{
	ConvertThreadToFiber(nullptr);
	m_threadID = GetCurrentThreadId();

	//set_thread_affinity(GetCurrentThread(), m_coreID);
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);

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
			JobDeclaration* job = nullptr;

			std::unique_lock<std::mutex> lock(*m_mutexThread);
			m_condVarThread->wait(lock, [&]() {job = m_queue->Pop(); return job != nullptr; });

			this->m_masterJob = job;

			m_masterJob->m_params->m_doubleBuffers = m_doubleBuffers;
			m_masterJob->m_params->m_doubleEndedStack = m_doubleEndedStack;
			m_masterJob->m_params->m_stack = m_stack;
			m_masterJob->m_params->m_finished.store(m_finished, std::memory_order_release);
			m_masterJob->m_params->m_lockFinished = &m_lockFinished;

			FiberLTM fiber(m_masterJob);
			m_masterFiber = &fiber;
			SwitchToFiber(fiber.getRef());
		}

		if (m_masterJob != nullptr && m_masterFiber != nullptr)
		{
			auto finished = m_finished.load(std::memory_order_acquire);
			if (*finished)
				m_masterFiber->destroy();
		}
		
		std::this_thread::yield();

	}
}


