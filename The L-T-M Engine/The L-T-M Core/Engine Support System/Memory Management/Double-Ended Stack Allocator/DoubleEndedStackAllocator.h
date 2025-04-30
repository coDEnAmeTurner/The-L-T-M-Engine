#pragma once
#include <cstdint>
#include <cassert>
#include <StackAllocator.h>

class DoubleEndedStackAllocator : public StackAllocator
{
public:
	DoubleEndedStackAllocator(std::uint32_t size, std::uint32_t align);
	~DoubleEndedStackAllocator();

	char* allocateFromBack(std::uint32_t size);
	void deallocateFromBack(char* rollback_to_ptr, uint32_t block_size);

	char* getBack_p1() const;
	Marker getTopBackMarker() const;
	char* getTopBack_p1() const;

private:
	char* m_back_p1 = nullptr; // Pointer to the back of the stack
	Marker m_topBackMarker = 0;
	char* m_topBack_p1 = 0;
};