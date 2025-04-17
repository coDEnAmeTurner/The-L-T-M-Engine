#include "StackAllocator.h"

StackAllocator::StackAllocator(std::uint32_t bytes, std::uint32_t align) 
				: BaseAllocator(bytes, align) 
{
	m_topMarker = 0;
	m_topaddr = m_memory + m_topMarker;
}

StackAllocator::~StackAllocator() {
	FreeAligned(m_memory, m_byteStoreShift); // Free the allocated memory
}

void* StackAllocator::alloc(std::uint32_t bytes) {
	assert(m_topMarker + bytes <= m_size);

	assert(bytes > 0);

	Marker old_marker = m_topMarker; // Save the old marker

	m_topMarker += bytes;
	m_topaddr = m_memory + m_topMarker;

	return m_memory + old_marker;
}

Marker StackAllocator::getTopMarker() const {
	return m_topMarker;
}

void StackAllocator::freeToMarker(Marker marker) {
	assert(marker <= m_topMarker);

	assert(marker >= 0);

	m_topMarker = marker;
	m_topaddr = m_memory + m_topMarker;
}

void StackAllocator::clear() {
	m_topMarker = 0;
	m_topaddr = m_memory + m_topMarker;
}

char* StackAllocator::getTopAddress() const {
	return m_topaddr;
}


