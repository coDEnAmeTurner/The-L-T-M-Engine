#pragma once
#include <cstdint>
#include <cassert>
#include <intrin.h>
#include <cmath>
#include <cstring>

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
};

extern inline void* alloc_org_aligned(std::uint32_t bytes, size_t align, std::uint32_t& bytes_store_shift);
extern inline void free_aligned(void* ptr, std::uint32_t& bytes_store_shift);
extern inline std::uint32_t log2(std::uint32_t x); 
extern inline std::uint32_t log2(std::uint32_t x);