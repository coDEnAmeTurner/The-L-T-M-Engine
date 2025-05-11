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
	void destroy();
	static void switchToFiber(PVOID fiber);
	static void decrementCounter(std::atomic<std::shared_ptr<Counter>>& pCounter);
	static void incrementCounter(std::atomic<std::shared_ptr<Counter>>& pCounter);
	static std::shared_ptr<FiberLTM> createFiber(std::shared_ptr<JobDeclaration> decl);

	FiberLTM(std::shared_ptr<JobDeclaration> job);
	FiberLTM(EntryPoint func);
	~FiberLTM();

	operator void* () const { return m_ref; }
private:
	void* m_ref;
	std::shared_ptr<JobDeclaration> m_job;

	//void create(EntryPoint func);
	void create(std::shared_ptr<JobDeclaration> job);
	static void __stdcall entryPointFiber(LPVOID p);

};

