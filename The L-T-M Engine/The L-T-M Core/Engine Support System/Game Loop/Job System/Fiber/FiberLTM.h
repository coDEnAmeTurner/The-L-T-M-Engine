#pragma once
#include <cassert>
#include <Windows.h>
#include <Job Properties.h>
#include <SpinLockLTM.h>
#include <ScopedLock.h>

class FiberLTM
{
public:
	void* getRef();
	void switchToFiber(FiberLTM& fiber);
	void destroy();

	FiberLTM(JobDeclaration* job);
	FiberLTM(EntryPoint func);
	~FiberLTM();

	operator void* () const { return m_ref; }
private:
	void* m_ref;

	void create(EntryPoint func);
	void create(JobDeclaration* job);
};

