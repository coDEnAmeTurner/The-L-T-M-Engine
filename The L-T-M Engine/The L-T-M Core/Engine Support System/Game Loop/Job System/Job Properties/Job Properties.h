#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include <StackAllocator.h>
#include <DoubleBufferedAllocator.h>
#include <DoubleEndedStackAllocator.h>
#include <PoolAllocator.h>

enum class Priority {
	LOW, NORMAL, HIGH, CRITICAL
};

struct Counter {
	//{placeholder}
};

typedef uintptr_t EntryPoint(JobParams* param);

struct JobParams {
	std::shared_ptr<StackAllocator> m_stack = nullptr;
	std::shared_ptr<DoubleEndedStackAllocator> m_doubleEndedStack = nullptr;
	std::shared_ptr<DoubledBufferedAllocator> m_doubleBuffers = nullptr;
	std::vector<std::shared_ptr<PoolAllocator>> m_poolAllocators;

	void* m_param = nullptr;
};

struct JobDeclaration {
	EntryPoint* m_pEntryPoint = nullptr;
	Priority m_priority = Priority::NORMAL;
	Counter* m_pCounter = nullptr;
	
	JobParams* m_params = nullptr;
};