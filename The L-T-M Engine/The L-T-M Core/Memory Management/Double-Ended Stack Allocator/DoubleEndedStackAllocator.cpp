#include "DoubleEndedStackAllocator.h"

DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::uint32_t size, std::uint32_t align)
	: BaseAllocator(size, align) // Call the base class constructor
{
	//if no -1, m_back will point at the byte after the last byte of the memory block
	m_front = m_memory;

	//using 1 past byte method: back is pointing at 1 byte out of bound
	m_back = m_memory + size;

	m_topBackMarker = 0; //marker is count from the back
	m_topFrontMarker = 0; //vice versa
}

DoubleEndedStackAllocator::~DoubleEndedStackAllocator()
{
	free_aligned(m_memory, m_byteStoreShift);
	m_memory = nullptr;
	m_front = nullptr;
	m_back = nullptr;
	m_size = 0;
	m_topBackMarker = 0;
	m_topFrontMarker = 0;
}

char* DoubleEndedStackAllocator::allocateFromFront(std::uint32_t size)
{
	assert(size > 0);

	//allow == since will be allocated from old front to new front - 1
	assert(m_front + size <= m_back);

	char* old_front = m_front;
	m_front += size;
	m_topFrontMarker += size;

	return old_front;
}

char* DoubleEndedStackAllocator::allocateFromBack(std::uint32_t size)
{
	assert(size > 0);

	//allow == since will be allocated from old back to new back + 1
	assert(m_back - size >= m_front);

	m_back -= size;
	m_topBackMarker += size;

	return m_back;
}

void DoubleEndedStackAllocator::deallocateFromFront(Marker rollback_marker)
{
	assert(rollback_marker <= m_topFrontMarker);

	m_front -= rollback_marker;
	m_topFrontMarker -= rollback_marker;
}

void DoubleEndedStackAllocator::deallocateFromBack(Marker rollback_marker)
{
	assert(rollback_marker <= m_topBackMarker);

	m_back += rollback_marker;
	m_topBackMarker -= rollback_marker;
}

char* DoubleEndedStackAllocator::getFront() const {
	return m_front;

}
char* DoubleEndedStackAllocator::getBack() const {
	return m_back;
}
Marker DoubleEndedStackAllocator::getTopFrontMarker() const {
	return m_topFrontMarker;
}
Marker DoubleEndedStackAllocator::getTopBackMarker() const {
	return m_topBackMarker;
}