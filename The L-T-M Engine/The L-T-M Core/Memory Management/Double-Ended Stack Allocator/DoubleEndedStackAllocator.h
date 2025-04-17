#pragma once
#include <cstdint>
#include <cassert>
#include "../Common/Memory Management Common.h"

class DoubleEndedStackAllocator : public BaseAllocator
{
public:
	DoubleEndedStackAllocator(std::uint32_t size, std::uint32_t align);
	~DoubleEndedStackAllocator();

	char* allocateFromFront(std::uint32_t size);
	char* allocateFromBack(std::uint32_t size);
	void deallocateFromFront(Marker rollback_marker);
	void deallocateFromBack(Marker rollback_marker);

	char* getFront() const;
	char* getBack() const;
	Marker getTopFrontMarker() const;
	Marker getTopBackMarker() const;


private:
	char* m_front = nullptr; // Pointer to the front of the stack
	char* m_back = nullptr; // Pointer to the back of the stack
	Marker m_topFrontMarker = 0;
	Marker m_topBackMarker = 0;

};