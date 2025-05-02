#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <StackAllocator.h>
#include <DoubleBufferedAllocator.h>
#include <DoubleEndedStackAllocator.h>
#include <PoolAllocator.h>
#include <MutexLTM.h>
#include <condition_variable>
#include <ScopedLock.h>	
#include <SpinLockLTM.h>

enum class Priority {
	LOW, NORMAL, HIGH, CRITICAL
};

struct Counter {
	std::uint32_t m_count = 1;
	std::mutex m_mutex;
	std::condition_variable m_condVar;

	Counter() {

	}
	Counter(const Counter& c) {
		m_count = c.m_count;
	}
};

struct JobParams {
	//thread provides:
	std::shared_ptr<StackAllocator> m_stack = nullptr;
	std::shared_ptr<DoubleEndedStackAllocator> m_doubleEndedStack = nullptr;
	std::shared_ptr<DoubledBufferedAllocator> m_doubleBuffers = nullptr;

	std::atomic<std::shared_ptr<Counter>> m_pCounter = std::shared_ptr<Counter>(new Counter());
	PVOID m_fiberParent = nullptr;

	//caller provides:
	void* m_param = nullptr;

};
using EntryPoint = void(__stdcall*)(void*);

struct JobDeclaration {
	EntryPoint m_pEntryPoint = nullptr;
	JobParams* m_params = nullptr;
};

