#pragma once
#include <cstdint>
#include "../Stack Allocator/StackAllocator.h"

class DoubledBufferedAllocator {
public:
	void swapBuffers();

	void clearCurrentBuffer();

	void* alloc(std::uint32_t bytes, std::uint32_t arr_elem_size, bool is_array);

	std::uint32_t getCurStack() const;

private:
	std::uint32_t m_curStack;
	StackAllocator m_stack[2];
};