#include "JobSystem.h"
#pragma once

std::unique_ptr<JobSystem> JobSystem::s_instance = nullptr;

JobSystem::JobSystem()
{

}

void JobSystem::init()
{
	if (s_instance == nullptr)
		s_instance = std::unique_ptr<JobSystem>(new JobSystem());
}

void JobSystem::destroy()
{
	//{placeholder}
}

void JobSystem::KickJob(const JobDeclaration& decl)
{
}

void JobSystem::KickJobs(int count, const JobDeclaration aDecl[])
{
}

void JobSystem::WaitForCounter(Counter* pCounter)
{
}

void JobSystem::KickJobAndWait(const JobDeclaration& decl)
{
}

void JobSystem::KickJobsAndWait(int count, const JobDeclaration aDecl[])
{
}


