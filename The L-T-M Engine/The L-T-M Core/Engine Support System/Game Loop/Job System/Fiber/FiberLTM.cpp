#include "FiberLTM.h"

void* FiberLTM::getRef()
{
	return m_ref;
}

void FiberLTM::switchToFiber(FiberLTM& fiber)
{
	assert(fiber.m_ref != nullptr);

	SwitchToFiber(fiber);
}

FiberLTM::FiberLTM(JobDeclaration* job)
{
	create(job);
}

FiberLTM::FiberLTM(EntryPoint func)
{
	create(func);
}

FiberLTM::~FiberLTM()
{
	destroy();
}

void FiberLTM::create(JobDeclaration* job)
{
	assert(m_ref == nullptr);

	m_ref = CreateFiber(0, job->m_pEntryPoint, job->m_params);
}

void FiberLTM::create(EntryPoint func )
{
	assert(m_ref == nullptr);

	m_ref = CreateFiber(0, func, nullptr);
}

void FiberLTM::destroy()
{
	assert(m_ref != nullptr);

	DeleteFiber(*this);
}


