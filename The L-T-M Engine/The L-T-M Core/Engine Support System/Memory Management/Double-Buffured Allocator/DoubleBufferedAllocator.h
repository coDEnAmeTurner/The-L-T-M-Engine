#pragma once
#include <cstdint>
#include <Stack Allocator/StackAllocator.h>

class DoubledBufferedAllocator {
public:
	void swapBuffers();

	void clearCurrentBuffer();

	void* alloc(std::uint32_t bytes, std::uint32_t arr_elem_size = 0, bool is_array = 0);

	std::uint32_t getCurStack() const;

	DoubledBufferedAllocator(std::uint32_t bytes, std::uint32_t align);
	~DoubledBufferedAllocator();
private:
	std::uint32_t m_curStack;
	StackAllocator* m_stack[2];
};