#pragma once
#include <cstdint>
#include <cassert>
#include <memory>
#include <Memory Management Common.h>

typedef std::uint32_t Marker;

class StackAllocator {
public:
	using Marker = std::uint32_t;

	StackAllocator(std::uint32_t bytes, std::uint32_t align);
	~StackAllocator();
	std::uint32_t getSize() const;
	void deallocateFromFront(char* ptr);
	void* allocateFromFront(std::uint32_t bytes, std::uint32_t elem_size = 0, bool is_array = false);
	char* getFront() const;
	Marker getTopFrontMarker() const;
	void clear();

	void* allocate(std::uint32_t bytes, std::uint32_t elem_size = 0, bool is_array = false)
	{
		return allocateFromFront(bytes, elem_size, is_array);
	}
	void deallocate(char* ptr)
	{
		deallocateFromFront(ptr);
	}
	char* getMemory() const
	{
		return getFront();
	}
	Marker getTop() const
	{
		return getTopFrontMarker();
	}

protected:
	std::uint32_t m_size;
	char* m_front;
	std::uint32_t m_align;
	std::uint32_t m_byteStoreShift = 0; // Total size of the memory block
	Marker m_topFrontMarker;
	char* m_topfront;
	static std::uint32_t s_minimum_align;
};