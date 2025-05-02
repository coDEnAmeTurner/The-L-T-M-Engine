#pragma once
#include <memory>
#include <cstdint>
#include <CPUSpecs.h>
#include <vector>
#include <ThreadLTM.h>
#include <Job Properties.h>
#include <JobQueue.h>
#include <MutexLTM.h>
#include <SpinLockLTM.h>

class JobSystem {
public:
	static void init();
	static void destroy();

	static void kickJob(JobDeclaration& decl);
	static void kickJobs(int count, JobDeclaration aDecl[]);
	// wait for job to terminate (for its Counter to become zero)
	static void waitForCounter(std::atomic<std::shared_ptr<Counter>>& pCounter);
	static void decrementCounter(std::atomic<std::shared_ptr<Counter>>& pCounter);
	static void incrementCounter(std::atomic<std::shared_ptr<Counter>>& pCounter);
	// kick jobs and wait for completion
	static void kickJobAndWait(JobDeclaration& decl);
	static void kickJobsAndWait(int count, JobDeclaration aDecl[]);
private:
	static std::unique_ptr<JobSystem> s_instance;

	std::vector<std::shared_ptr<ThreadLTM>> m_threadPool;
	std::shared_ptr<JobQueue> m_queueObj;
	std::shared_ptr<std::mutex> m_mutexThread = std::shared_ptr<std::mutex>(new std::mutex());
	std::shared_ptr<std::condition_variable> m_condVarThread = std::shared_ptr<std::condition_variable>(new std::condition_variable());

	JobSystem();
	//void initCounter(JobDeclaration& decl);
};