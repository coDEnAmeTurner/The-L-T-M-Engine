#include "DoubleBufferedAllocator.h"	

void DoubledBufferedAllocator::swapBuffers() {
	m_curStack = (std::uint32_t)!m_curStack;
}

void DoubledBufferedAllocator::clearCurrentBuffer() {
	m_stack[m_curStack].clear();
}

void* DoubledBufferedAllocator::alloc(std::uint32_t bytes, std::uint32_t arr_elem_size, bool is_array) {
	return m_stack[m_curStack].allocate(bytes, arr_elem_size, is_array);
}

std::uint32_t DoubledBufferedAllocator::getCurStack() const {
	return m_curStack;
}
