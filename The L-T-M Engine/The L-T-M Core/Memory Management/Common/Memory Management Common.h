#pragma once
#include <cstdint>

typedef std::uint32_t Marker;

extern void* AllocAligned(std::uint32_t bytes, std::uint32_t align);
extern void FreeAligned(void* ptr);