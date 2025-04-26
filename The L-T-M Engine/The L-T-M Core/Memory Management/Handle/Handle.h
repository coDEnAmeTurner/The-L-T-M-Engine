#pragma once
#include <cstdint>
#include <vector>
#include <cassert>
#include <malloc.h>
#include <algorithm>
#include "../../Utilities/L-T-M Core Utilities.h"

#define HANDLE std::uint32_t
#define HANDLE_RESOLVER(i) HeapAllocator::getInstance()->getTable()[i]

struct HandleEntry {
	void* ptr;
	size_t size;
};

class HeapAllocator
{
public:
	HeapAllocator() = default;
	~HeapAllocator() = default;

	static void init(std::uint32_t size);
	static void destroy();
	static HANDLE alloc(std::uint32_t size);
	static void dealloc(HANDLE handle);
	static void relocate(std::uint32_t block_count);
	static HeapAllocator* getInstance() { return m_instance; }
	std::vector<HandleEntry>& getTable() const { return m_table; }

private:
	static HANDLE m_curRelocHandle;
	static std::uint32_t m_size;
	static std::vector<HandleEntry> m_table;
	static HeapAllocator* m_instance;
};

