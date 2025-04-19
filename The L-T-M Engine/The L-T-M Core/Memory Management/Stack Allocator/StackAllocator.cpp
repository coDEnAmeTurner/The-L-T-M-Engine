#include "StackAllocator.h"

StackAllocator::StackAllocator(std::uint32_t bytes, std::uint32_t align) 
				: BaseAllocator(bytes, align) 
{
	m_topMarker = 0;
	m_topaddr = m_memory + m_topMarker;
}

StackAllocator::~StackAllocator() {
	free_aligned(m_memory, m_byteStoreShift); // Free the allocated memory
}

void* StackAllocator::alloc(std::uint32_t bytes, std::uint32_t elem_size, bool is_array) {
	assert(bytes > 0);
	assert(m_topMarker + bytes <= m_size);

	char* rear_ptr = m_memory + m_size - 1;
	std::uint32_t align = ternary_pred(
		bytes <= s_minimum_align,
		s_minimum_align,
		bytes
	);

	char* aligned_top = align_pointer(
		m_topaddr,
		ternary_pred(
			is_array == true,
			elem_size,
			align
		),
		reinterpret_cast<uintptr_t>(rear_ptr)
	);
	if (aligned_top == m_topaddr)
		aligned_top += align;

	assert(aligned_top + bytes - 1 <= rear_ptr);

	std::uint32_t shift = aligned_top - m_topaddr;
	assert(shift <= pow_of_2(8));
	aligned_top[-1] = *reinterpret_cast<char*>(&shift);

	m_topaddr = aligned_top + bytes;
	m_topMarker = m_topaddr - m_memory;

	return aligned_top;
}

void StackAllocator::freeToPtr(char* ptr) {
	assert(ptr >= m_memory);
	assert(ptr <= m_memory + m_size - 1);

	union { std::uint8_t ui; char c; } u;
	u.c = *(ptr - 1);

	m_topaddr = ptr - u.ui;
	m_topMarker = m_topaddr - m_memory;
}

void StackAllocator::clear() {
	m_topMarker = 0;
	m_topaddr = m_memory + m_topMarker;
}

Marker StackAllocator::getTopMarker() const {
	return m_topMarker;
}

char* StackAllocator::getTopAddress() const {
	return m_topaddr;
}


