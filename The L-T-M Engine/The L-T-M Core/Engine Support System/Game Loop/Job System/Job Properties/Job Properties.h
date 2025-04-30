#pragma once
#include <cstdint>
#include <vector>
#include <memory>

typedef uintptr_t EntryPoint(uintptr_t param);

enum class Priority {
	LOW, NORMAL, HIGH, CRITICAL
};

struct Counter {
	//{placeholder}
};

struct JobDeclaration {
	EntryPoint* m_pEntryPoint;
	uintptr_t m_param;
	Priority m_priority;
	Counter* m_pCounter;
	std::vector<std::shared_ptr<JobDeclaration>> m_associates;
};