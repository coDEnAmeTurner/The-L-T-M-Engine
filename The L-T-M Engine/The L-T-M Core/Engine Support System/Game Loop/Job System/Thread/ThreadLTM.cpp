#include "ThreadLTM.h"

ThreadLTM::ThreadLTM()
	:m_queue(nullptr), m_threadID(0), m_stack(nullptr), m_doubleEndedStack(nullptr), m_doubleBuffers(nullptr)
{

}

ThreadLTM::ThreadLTM(std::shared_ptr<JobQueue> queue)
	:m_queue(queue)
{
	m_stack = std::shared_ptr<StackAllocator>(new StackAllocator(WORD_SIZE * pow_of_2(17), MINIMUM_ALIGNMENT));
	m_doubleEndedStack = std::shared_ptr<DoubleEndedStackAllocator>(new DoubleEndedStackAllocator(WORD_SIZE * pow_of_2(18), MINIMUM_ALIGNMENT));
	m_doubleBuffers = std::shared_ptr<DoubledBufferedAllocator>(new DoubledBufferedAllocator(WORD_SIZE * pow_of_2(17), MINIMUM_ALIGNMENT));

	std::thread t(entryPoint);
}

void ThreadLTM::entryPoint()
{
	while (true) {

	}
}


