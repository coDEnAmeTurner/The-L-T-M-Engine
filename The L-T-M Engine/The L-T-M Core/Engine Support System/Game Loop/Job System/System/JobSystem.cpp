#pragma once
#include "JobSystem.h"

std::unique_ptr<JobSystem> JobSystem::s_instance = nullptr;

JobSystem::JobSystem()
{
	m_queueObj = std::shared_ptr< JobQueue>(new JobQueue());
	std::uint8_t thread_count = ALL_THREAD_COUNT / 2;
	for (int i = 0; i < thread_count; i++) {
		m_threadPool.emplace_back(std::shared_ptr<ThreadLTM>(new ThreadLTM(m_queueObj, i + 1, m_mutexThread, m_condVarThread)));
	}
}

//void JobSystem::initCounter(JobDeclaration& decl)
//{
//	std::shared_ptr<Counter> c = std::shared_ptr<Counter>(new Counter());
//	incrementCounter(c);
//	decl.m_pCounter = c;
//}

void JobSystem::init()
{
	if (s_instance == nullptr)
		s_instance = std::unique_ptr<JobSystem>(new JobSystem());
}

void JobSystem::destroy()
{
	for (const auto& t : s_instance->m_threadPool) {
		t->destroy();
	}
}

void JobSystem::kickJob(JobDeclaration& decl)
{
	s_instance->m_queueObj->Push(&decl);
	std::unique_lock<std::mutex> lock(*s_instance->m_mutexThread);
	s_instance->m_condVarThread->notify_all();
}

void JobSystem::kickJobs(int count, JobDeclaration aDecl[])
{
	for (size_t i = 0; i < count; i++) {
		JobParams* p = aDecl[i].m_params;
		if (p->m_pCounter.load(std::memory_order_relaxed) != nullptr)
			FiberLTM::incrementCounter(p->m_pCounter);

		s_instance->kickJob(aDecl[i]);

	}
}

void JobSystem::waitForCounter(std::atomic<std::shared_ptr<Counter>>& pCounter)
{
	auto cptr = pCounter.load(std::memory_order_acquire);
	assert(cptr != nullptr);

	std::uint8_t count;
	std::unique_lock<std::mutex> lock(cptr->m_mutex);
	cptr->m_condVar.wait(lock, [&]() {count = pCounter.load(std::memory_order_acquire)->m_count; return count == 0; });

	return;
}


void JobSystem::kickJobAndWait(JobDeclaration& decl)
{
	JobParams* p = decl.m_params;
	kickJob(decl);
	waitForCounter(p->m_pCounter);
}

void JobSystem::kickJobsAndWait(int count, JobDeclaration aDecl[])
{
	kickJobs(count, aDecl);

	for (size_t i = 0; i < count; i++) {
		JobParams* p = aDecl[i].m_params;
		waitForCounter(p->m_pCounter);
	}
}



