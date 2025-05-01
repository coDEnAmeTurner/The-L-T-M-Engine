#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <StackAllocator.h>
#include <DoubleBufferedAllocator.h>
#include <DoubleEndedStackAllocator.h>
#include <PoolAllocator.h>
#include <SpinLockLTM.h>

enum class Priority {
	LOW, NORMAL, HIGH, CRITICAL
};

struct Counter {
	std::uint32_t m_count;
	SpinLockLTM m_lock;
};

struct JobParams {
	std::shared_ptr<StackAllocator> m_stack = nullptr;
	std::shared_ptr<DoubleEndedStackAllocator> m_doubleEndedStack = nullptr;
	std::shared_ptr<DoubledBufferedAllocator> m_doubleBuffers = nullptr;
	bool* m_finished = nullptr; //notify thread current fiber is finished

	void* m_param = nullptr;
};
using EntryPoint = void(__stdcall*)(void*);

struct JobDeclaration {
	EntryPoint m_pEntryPoint = nullptr;
	Counter* m_pCounter = nullptr;
	
	JobParams* m_params = nullptr;
};