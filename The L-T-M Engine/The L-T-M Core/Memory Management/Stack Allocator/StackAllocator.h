#include <cstdint>
#include <cassert>
#include "../Common/Memory Management Common.h"

class StackAllocator : public BaseAllocator
{
public:
	using Marker = std::uint32_t;

	StackAllocator(std::uint32_t bytes, std::uint32_t align);
	~StackAllocator();

	char* getTopAddress() const;
	Marker getTopMarker() const;
	void freeToPtr(char* ptr);
	void* alloc(std::uint32_t bytes, std::uint32_t elem_size = 0, bool is_array = false);
	void clear();
private:
	Marker m_topMarker;
	char* m_topaddr;
};