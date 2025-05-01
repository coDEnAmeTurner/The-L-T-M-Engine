#pragma once
#include <memory>
#include <cstdint>
#include <CPUSpecs.h>
#include <vector>
#include <ThreadLTM.h>
#include <Job Properties.h>
#include <JobQueue.h>

class JobSystem {
public:
	static void init();
	static void destroy();

	void kickJob(JobDeclaration& decl);
	void kickJobs(int count, JobDeclaration aDecl[]);
	// wait for job to terminate (for its Counter to become zero)
	void waitForCounter(Counter* pCounter);
	void decrementCounter(Counter* pCounter);
	void incrementCounter(Counter* pCounter);
	// kick jobs and wait for completion
	void kickJobAndWait(const JobDeclaration& decl);
	void kickJobsAndWait(int count, const JobDeclaration aDecl[]);
private:
	static std::unique_ptr<JobSystem> s_instance;

	std::vector<std::shared_ptr<ThreadLTM>> m_threadPool;
	std::shared_ptr<JobQueue> m_queueObj;

	JobSystem();
};