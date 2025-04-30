#pragma once
#include <cstdint>
#include <cstring>
#include <iostream>
#include <Memory Management Common.h>
#include <L-T-M Core Utilities.h>

class PoolAllocator {
public:
	char* alloc();
	void free(char* target_ptr);
	void clear();

	char* getMemory() const;
	std::uint64_t getSizeInBytes() const;
	char* getCurrentFreePtr() const;

	//memory -> block -> component (comp)
	PoolAllocator(std::uint32_t block_count, std::uint32_t compo_count, std::uint32_t compo_size, bool manual_align = false, bool align_by_block = false);
	~PoolAllocator();
private:
	char* m_memory = nullptr;
	std::size_t m_shift = 0;
	std::size_t m_align = 0;
	std::uint32_t m_blockCount = 0;
	std::uint32_t m_compoCount = 0;
	std::uint32_t m_compoSize = 0;
	bool m_manualAlign = false;
	char* m_currentFreePtr = nullptr;
};