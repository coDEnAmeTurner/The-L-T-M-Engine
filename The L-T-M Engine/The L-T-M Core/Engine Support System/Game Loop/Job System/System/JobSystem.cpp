#pragma once
#include "JobSystem.h"

std::unique_ptr<JobSystem> JobSystem::s_instance = nullptr;

JobSystem::JobSystem()
{
	std::uint8_t thread_count = LOGICAL_CORE_COUNT / 2;
	for (int i = 0; i < thread_count; i++) {
		m_threadPool.emplace_back(std::shared_ptr<ThreadLTM>(new ThreadLTM(m_queueObj)));
	}
}

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
}

void JobSystem::kickJobs(int count, JobDeclaration aDecl[])
{
	for (std::uint8_t i = 0; i < count; i++) {
		if (aDecl[i].m_pCounter != nullptr)
			aDecl[i].m_pCounter->m_count = 1;
		s_instance->kickJob(aDecl[i]);

	}
}

void JobSystem::waitForCounter(Counter* pCounter)
{

}

void JobSystem::kickJobAndWait(const JobDeclaration& decl)
{
}

void JobSystem::kickJobsAndWait(int count, const JobDeclaration aDecl[])
{
}


