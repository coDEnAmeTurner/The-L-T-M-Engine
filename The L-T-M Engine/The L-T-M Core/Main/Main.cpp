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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//8 vec4 of doubles
	PoolAllocator dvec4_m256_pool_allocator(8, 4, sizeof(double), true, true);

	double values[] = { 1.0, 2.0, 3.0, 4.0 };
	alignas(32) char* block = dvec4_m256_pool_allocator.alloc();
	std::memcpy(block, values, sizeof(values));
	__m256d vec = _mm256_load_pd(reinterpret_cast<double*>(block));

	alignas(32) char* block2 = dvec4_m256_pool_allocator.alloc();
	std::memcpy(block2, values, sizeof(values));
	__m256d vec2 = _mm256_load_pd(reinterpret_cast<double*>(block2));

	alignas(32) double result_values[4];
	__m256d result = _mm256_add_pd(vec, vec2);
	_mm256_store_pd(reinterpret_cast<double*>(result_values), result);

	printf("result => %.1f %.1f %.1f %.1f\n",
		result_values[0],
		result_values[1],
		result_values[2],
		result_values[3]);

	return 0;
}