#include <cstdint>
#include <string>
#include <cstring>
#include "../Memory Management/Double-Ended Stack Allocator/DoubleEndedStackAllocator.h"

struct Test {
	std::uint32_t m_ui; // 4 bytes
	double m_d; // 8 bytes
	//implicit: 4 bytes for the padding of uint32_t

	Test(std::uint32_t ui, double d) : m_ui(ui), m_d(d) { }

	std::string ToString() {
		return std::to_string(m_ui) + " " + std::to_string(m_d);
	}
};

DoubleEndedStackAllocator allocator(1024, 16); // 1 MB

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	char* t1_ptr = allocator.allocateFromFront(sizeof(Test));
	new (t1_ptr) Test(1, 2.0); // placement new

	char* t2_ptr = allocator.allocateFromBack(sizeof(Test));
	new (t2_ptr) Test(3, 4.0); // placement new

	Test& t1 = *reinterpret_cast<Test*>(t1_ptr);
	Test& t2 = *reinterpret_cast<Test*>(t2_ptr);

	printf("size of Test: %zu\n", sizeof(Test)); // 16 bytes or is it?
	
	std::string t1_str = t1.ToString();
	std::string t2_str = t2.ToString();

	const char* t1_cstr = t1_str.c_str();
	const char* t2_cstr = t2_str.c_str();

	assert(_CrtCheckMemory());
	printf("t1: %s\n", t1_cstr);
	printf("t2: %s\n", t2_cstr);

	return 0;
}