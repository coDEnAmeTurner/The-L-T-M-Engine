#include "FiberLTM.h"

void* FiberLTM::getRef()
{
	return m_ref;
}

void FiberLTM::incrementCounter(std::atomic<std::shared_ptr<Counter>>& pCounter)
{
	auto cptr = pCounter.load(std::memory_order_acquire);
	assert(cptr != nullptr);

	Counter& c = *pCounter.load(std::memory_order_acquire);
	c.m_count++;
	pCounter.store(std::shared_ptr<Counter>(new Counter(c)));
}

std::shared_ptr<FiberLTM> FiberLTM::createFiber(std::shared_ptr<JobDeclaration> decl)
{
	auto fiber = std::shared_ptr<FiberLTM>(new FiberLTM(decl));
	return fiber;
}

FiberLTM::FiberLTM(std::shared_ptr<JobDeclaration> job)
{
	create(job);
}

FiberLTM::FiberLTM(EntryPoint func)
{
	//create(func);
}

FiberLTM::~FiberLTM()
{
	destroy();
}

void FiberLTM::switchToFiber(PVOID fiber)
{
	assert(fiber != nullptr);

	SwitchToFiber(fiber);
}

void FiberLTM::decrementCounter(std::atomic<std::shared_ptr<Counter>>& pCounter)
{
	auto cptr = pCounter.load(std::memory_order_acquire);
	assert(cptr != nullptr);

	Counter& c = *pCounter.load(std::memory_order_acquire);
	c.m_count--;
	pCounter.store(std::shared_ptr<Counter>(new Counter(c)));
	std::unique_lock<std::mutex> lock(c.m_mutex);
	c.m_condVar.notify_one();
}

void FiberLTM::create(std::shared_ptr<JobDeclaration> job)
{
	assert(m_ref == nullptr);

	m_job = job;
	m_ref = CreateFiber(0, &FiberLTM::entryPointFiber, this);
}

void __stdcall FiberLTM::entryPointFiber(LPVOID p)
{
	FiberLTM* self = reinterpret_cast<FiberLTM*>(p);

	FiberLTM::incrementCounter(self->m_job->m_params->m_pCounter);
	self->m_job->m_pEntryPoint(self->m_job->m_params);
	assert(
		self->m_job != nullptr && 
		self->m_job->m_params != nullptr &&
		self->m_job->m_params->m_fiberParent != nullptr
	);
	FiberLTM::decrementCounter(self->m_job->m_params->m_pCounter);
	FiberLTM::switchToFiber(self->m_job->m_params->m_fiberParent);
}

//void FiberLTM::create(EntryPoint func)
//{
//	assert(m_ref == nullptr);
//
//	m_ref = CreateFiber(0, func, nullptr);
//}

void FiberLTM::destroy()
{
	assert(m_ref != nullptr);

	DeleteFiber(*this);
}


