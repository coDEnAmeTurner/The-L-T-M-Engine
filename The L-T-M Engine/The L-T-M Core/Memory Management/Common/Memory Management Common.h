#pragma once
#include <cstdint>
#include <cassert>
#include <intrin.h>
#include <cmath>
#include <cstring>
#include "../../Utilities/L-T-M Core Utilities.h"

typedef std::uint32_t Marker;

class BaseAllocator {
public:
	std::uint32_t getSize() const;
	char* getMemory() const;

	BaseAllocator(std::uint32_t bytes, std::uint32_t align);
protected:
	std::uint32_t m_size;
	char* m_memory;
	std::uint32_t m_align;
	std::uint32_t m_byteStoreShift = 0; // Total size of the memory block
	static std::uint32_t s_minimum_align;
};

extern inline void* alloc_org_aligned(std::uint32_t bytes, size_t align, std::uint32_t& bytes_store_shift);
extern inline void free_aligned(void* ptr, std::uint32_t& bytes_store_shift);
template<typename T>
extern inline T* align_pointer(T* ptr, size_t align, uintptr_t rear_bound);
extern inline uintptr_t align_address(uintptr_t addr, size_t align, uintptr_t rear_bound);

