#pragma once
#include <cstdint>
#include <cstring>
#include <iostream>
#include "../Common/Memory Management Common.h"
#include "../../Utilities/L-T-M Core Utilities.h"

class PoolAllocator {
public:
	char* alloc();
	void free(char* target_ptr);

	char* getMemory() const;
	std::uint64_t getSizeInBytes() const;
	char* getCurrentFreePtr() const;

	//memory -> block -> component (comp)
	PoolAllocator(std::uint32_t block_count, std::uint32_t compo_count, std::uint32_t compo_size, bool manual_align = false, bool align_by_block = false);
	~PoolAllocator();
private:
	char* m_memory = nullptr;
	std::uint32_t m_blockCount = 0;
	std::uint32_t m_compoCount = 0;
	std::uint32_t m_compoSize = 0;
	bool m_manualAlign = false;
	char* m_currentFreePtr = nullptr;
};