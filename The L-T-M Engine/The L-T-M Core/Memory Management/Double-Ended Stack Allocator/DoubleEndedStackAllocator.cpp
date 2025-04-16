#include "DoubleEndedStackAllocator.h"

DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::uint32_t size)
{
	assert(size > 0);
	m_size = size;
	m_memory = reinterpret_cast<char*>(AllocAligned(size, 16));
	m_front = m_memory;

	//if no -1, m_back will point at the byte after the last byte of the memory block
	m_back = m_memory + size - 1;

	m_topBackMarker = 0; //marker is count from the back
	m_topFrontMarker = 0; //vice versa
}

DoubleEndedStackAllocator::~DoubleEndedStackAllocator()
{
	FreeAligned(m_memory);
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

	char* old_back = m_back;
	m_back -= size;
	m_topBackMarker += size;

	return old_back;
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

std::uint32_t DoubleEndedStackAllocator::getSize() const {
	return m_size;
}
char* DoubleEndedStackAllocator::getMemory() const {
	return m_memory;
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