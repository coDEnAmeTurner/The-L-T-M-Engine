#include "DoubleEndedStackAllocator.h"

DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::uint32_t m_size, std::uint32_t align)
	: StackAllocator(m_size, align) // Call the base class constructor
{
	//using 1 past byte method: back is pointing at 1 byte out of bound
	m_back_p1 = m_front + m_size;
	m_topBack_p1 = m_back_p1;
	m_topBackMarker = 0;
}

DoubleEndedStackAllocator::~DoubleEndedStackAllocator()
{
	m_back_p1 = nullptr;
	m_topBackMarker = 0;
}

char* DoubleEndedStackAllocator::allocateFromBack(std::uint32_t m_size)
{
	assert(m_size > 0);

	std::uint32_t align = ternary_pred(m_size >= m_align, m_size, m_align);
	char* noaligned_topback = m_topBack_p1 - m_size;
	char* pre_aligned_topback = noaligned_topback - align;

	assert(pre_aligned_topback >= m_topfront);

	char* aligned_topback = align_pointer(
		pre_aligned_topback,
		align,
		reinterpret_cast<uintptr_t>(noaligned_topback + 1)
	);
	aligned_topback = ternary_pred(aligned_topback == pre_aligned_topback, aligned_topback + align, aligned_topback);
	
	assert(aligned_topback - 1 >= m_topfront);

	std::uint32_t shift = aligned_topback - pre_aligned_topback;
	aligned_topback[-1] = *reinterpret_cast<char*>(&shift);

	m_topBack_p1 = pre_aligned_topback;
	m_topBackMarker = m_topBack_p1 - m_back_p1;

	return aligned_topback;
}

void DoubleEndedStackAllocator::deallocateFromBack(char* rollback_to_ptr, uint32_t block_size)
{
	assert(rollback_to_ptr >= m_topBack_p1 && rollback_to_ptr < m_back_p1);
	assert(rollback_to_ptr - 1 >= m_topfront);

	union { uint16_t t; char c; } u;
	u.c = rollback_to_ptr[-1];

	uint16_t shift = u.t;
	char* org_rollback_ptr = rollback_to_ptr - shift;

	m_topBack_p1 = org_rollback_ptr + block_size;
	m_topBackMarker = m_back_p1 - m_topBack_p1;
}

char* DoubleEndedStackAllocator::getBack_p1() const {
	return m_back_p1;
}

Marker DoubleEndedStackAllocator::getTopBackMarker() const {
	return m_topBackMarker;
}


char* DoubleEndedStackAllocator::getTopBack_p1() const {
	return m_topBack_p1;
}
