#include "ThreadLTM.h"

ThreadLTM::ThreadLTM()
	:m_job(nullptr), m_queue(nullptr), m_thread(0), m_stack(nullptr), m_doubleEndedStack(nullptr), m_doubleBuffers(nullptr)
{

}

ThreadLTM::ThreadLTM(std::shared_ptr<JobQueue> queue)
	:m_queue(queue), m_job(nullptr)
{
	m_stack = std::shared_ptr<StackAllocator>(new StackAllocator(WORD_SIZE * pow_of_2(17), MINIMUM_ALIGNMENT));
	m_doubleEndedStack = std::shared_ptr<DoubleEndedStackAllocator>(new DoubleEndedStackAllocator(WORD_SIZE * pow_of_2(18), MINIMUM_ALIGNMENT));
	m_doubleBuffers = std::shared_ptr<DoubledBufferedAllocator>(new DoubledBufferedAllocator(WORD_SIZE * pow_of_2(17), MINIMUM_ALIGNMENT));

	m_thread = &std::thread(entryPoint);
}

void ThreadLTM::destroy()
{
	assert(m_thread != nullptr);

	m_thread->join();
}

void ThreadLTM::entryPoint()
{
	ConvertThreadToFiber(nullptr);

	while (true) {
		ScopedLock<SpinLockLTM> lock(&m_lockRunning);
		bool running = GameLoopManager::getRunning();
		if (!running) {
			break;
		}

		//pop queue
		JobDeclaration* job = nullptr;
		if (m_masterFiber != nullptr)
		{
			job = m_queue->Pop();
			this->m_job = job;
		}

		//wrap real func in wrapper func, when real fnc finishes, 
		
		job->m_params->m_doubleBuffers = m_doubleBuffers;
		job->m_params->m_doubleEndedStack = m_doubleEndedStack;
		job->m_params->m_stack = m_stack;
		job->m_params->m_finished = &m_finished;

		if (job != nullptr)
			m_masterFiber = &FiberLTM(job);

		if (m_finished)
			m_masterFiber->destroy();
	}
}


