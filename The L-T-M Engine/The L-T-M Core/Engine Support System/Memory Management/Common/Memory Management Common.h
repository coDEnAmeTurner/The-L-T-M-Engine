#pragma once
#include <cstdint>
#include <cassert>
#include <intrin.h>
#include <cmath>
#include <cstring>
#include <L-T-M Core Utilities.h>

extern inline void* alloc_org_aligned(std::uint32_t bytes, size_t align, std::uint32_t& bytes_store_shift);
extern inline void free_aligned(void* ptr, std::uint32_t& bytes_store_shift);
template<typename T>
extern inline T* align_pointer(T* ptr, size_t align, uintptr_t rear_bound);
extern inline uintptr_t align_address(uintptr_t addr, size_t align, uintptr_t rear_bound);

