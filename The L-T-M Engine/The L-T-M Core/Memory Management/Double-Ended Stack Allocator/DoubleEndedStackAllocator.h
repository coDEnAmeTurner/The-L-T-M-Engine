#pragma once
#include <cstdint>
#include <cassert>
#include "../Common/Memory Management Common.h"

class DoubleEndedStackAllocator
{
public:
	DoubleEndedStackAllocator(std::uint32_t size);
	~DoubleEndedStackAllocator();

	char* allocateFromFront(std::uint32_t size);
	char* allocateFromBack(std::uint32_t size);
	void deallocateFromFront(Marker rollback_marker);
	void deallocateFromBack(Marker rollback_marker);

	std::uint32_t getSize() const;
	char* getMemory() const;
	char* getFront() const;
	char* getBack() const;
	Marker getTopFrontMarker() const;
	Marker getTopBackMarker() const;
private:
	std::uint32_t m_size = 0; // Total size of the memory block
	char* m_memory = nullptr; // Pointer to the memory block
	char* m_front = nullptr; // Pointer to the front of the stack
	char* m_back = nullptr; // Pointer to the back of the stack
	Marker m_topFrontMarker = 0;
	Marker m_topBackMarker = 0;

	// Disable copy constructor and assignment operator
	DoubleEndedStackAllocator(const DoubleEndedStackAllocator&) = delete;
	DoubleEndedStackAllocator& operator=(const DoubleEndedStackAllocator&) = delete;
};