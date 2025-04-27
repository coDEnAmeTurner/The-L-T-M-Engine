#pragma once
#include <cstdint>
#include <vector>
#include <cassert>
#include <malloc.h>
#include <algorithm>
#include <Common/Memory Management Common.h>
#include <L-T-M Core Utilities.h>

#define HANDLE std::uint32_t
#define HANDLE_RESOLVER(i) HeapAllocator::getInstance()->getTable()[i]

struct HandleEntry {
	void* ptr = nullptr;
	size_t size = 0;
};

class HeapAllocator
{
public:
	HeapAllocator() = default;
	~HeapAllocator() = default;

	static void init(std::uint32_t memo_init_size);
	static void destroy();
	static HANDLE alloc(std::uint32_t size);
	static void dealloc(HANDLE handle);

	static void relocate(std::uint32_t bytes);
	static HeapAllocator* getInstance() { return m_instance; }
	std::vector<HandleEntry>& getTable() const { return m_table; }

private:
	static HANDLE m_curRelocHandle;
	static std::uint32_t m_memoSize;
	//m_table: 
	//element can't be erased. It will alter the surrounding handles pointing to in-use blocks.
	//solution - alloc:
	//if m_table is empty: push_back.
	//				not empty: check for empty entry (ptr=null, size=0). 
	//				if there's one: replace the entry 
	//					there's not: push_back
	//solution - dealloc:
	//replace hande entry with empty entry
	//add a free block to freelist. 
	//if adjacent blocks are free: Merge
	//							not: Only add
	static std::vector<HandleEntry> m_table;
	static std::vector<HandleEntry> m_freeList;
	static HeapAllocator* m_instance;
	static void* m_memory;
};

