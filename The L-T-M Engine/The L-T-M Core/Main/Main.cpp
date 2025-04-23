#include <cstdint>
#include <string>
#include <cstring>
#include <emmintrin.h>
#include <intrin.h>
#include "../Memory Management/Double-Ended Stack Allocator/DoubleEndedStackAllocator.h"
#include "../Memory Management/Stack Allocator/StackAllocator.h"
#include "../Memory Management/Pool Allocator/PoolAllocator.h"

struct Test {
	std::uint32_t m_ui; // 4 bytes
	double m_d; // 8 bytes
	//implicit: 4 bytes for the padding of uint32_t

	Test(std::uint32_t ui, double d) : m_ui(ui), m_d(d) { }

	std::string ToString() {
		return std::to_string(m_ui) + " " + std::to_string(m_d);
	}
};

DoubleEndedStackAllocator allocator(1024, 64);
StackAllocator stack_allocator(1024, 64);

int main() {


	return 0;
}