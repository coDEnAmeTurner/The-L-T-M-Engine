#include "StackAllocator.h"

std::uint32_t StackAllocator::s_minimum_align = 8;

StackAllocator::StackAllocator(std::uint32_t bytes, std::uint32_t align)
	: m_size(bytes), m_align(align)
{
	assert(m_size > 0 && align > 0 && is_pow_of_2(align)); // Ensure size and alignment are valid

	m_front = reinterpret_cast<char*>(alloc_org_aligned(bytes, align, m_byteStoreShift)); // Allocate memory with alignment

	m_topFrontMarker = 0;
	m_topfront = m_front + m_topFrontMarker;

}

StackAllocator::~StackAllocator() {
	free_aligned(m_front, m_byteStoreShift); // Free the allocated memory
}

void StackAllocator::clear() {
	m_topFrontMarker = 0;
	m_topfront = m_front + m_topFrontMarker;
}

Marker StackAllocator::getTopFrontMarker() const {
	return m_topFrontMarker;
}

char* StackAllocator::getFront() const {
	return m_topfront;
}

void* StackAllocator::allocateFromFront(std::uint32_t bytes, std::uint32_t elem_size, bool is_array) {
	assert(bytes > 0);
	assert(m_topFrontMarker + bytes <= m_size);

	char* rear_ptr = m_front + m_size - 1;
	std::uint32_t align = ternary_pred(
		bytes <= s_minimum_align,
		s_minimum_align,
		bytes
	);

	char* aligned_top = align_pointer(
		m_topfront,
		ternary_pred(
			is_array == true,
			elem_size,
			align
		),
		reinterpret_cast<uintptr_t>(rear_ptr)
	);
	if (aligned_top == m_topfront)
		aligned_top += align;

	assert(aligned_top + bytes - 1 <= rear_ptr);

	std::uint32_t shift = aligned_top - m_topfront;
	assert(shift <= pow_of_2(8));
	aligned_top[-1] = *reinterpret_cast<char*>(&shift);

	m_topfront = aligned_top + bytes;
	m_topFrontMarker = m_topfront - m_front;

	return aligned_top;
}

void StackAllocator::deallocateFromFront(char* ptr) {
	assert(ptr >= m_front);
	assert(ptr <= m_front + m_size - 1);

	union { std::uint8_t ui; char c; } u;
	u.c = *(ptr - 1);

	m_topfront = ptr - u.ui;
	m_topFrontMarker = m_topfront - m_front;
}


std::uint32_t StackAllocator::getSize() const {
	return m_size;
}
