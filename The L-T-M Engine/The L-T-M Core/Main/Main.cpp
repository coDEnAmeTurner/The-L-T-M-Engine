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

DoubleEndedStackAllocator allocator(1024, 64); 

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	char* t1_ptr = allocator.allocateFromFront(sizeof(std::uint32_t));
	new (t1_ptr) int(5); // placement new
	char* t3_ptr = allocator.allocateFromFront(sizeof(Test));
	new (t3_ptr) Test(6, 7.5); // placement new

	char* t4_ptr = allocator.allocateFromBack(sizeof(double));
	new (t4_ptr) double(7.89); // placement new
	char* t2_ptr = allocator.allocateFromBack(sizeof(Test));
	new (t2_ptr) Test(3, 4.0); // placement new

	int& t1 = *reinterpret_cast<int*>(t1_ptr);
	Test& t2 = *reinterpret_cast<Test*>(t2_ptr);
	Test& t3 = *reinterpret_cast<Test*>(t3_ptr);
	double& t4 = *reinterpret_cast<double*>(t4_ptr);

	std::string t1_str = std::to_string(t1);
	std::string t2_str = t2.ToString();
	std::string t3_str = t3.ToString();
	std::string t4_str = std::to_string(t4);

	const char* t1_cstr = t1_str.c_str();
	const char* t2_cstr = t2_str.c_str();
	const char* t3_cstr = t3_str.c_str();
	const char* t4_cstr = t4_str.c_str();

	assert(_CrtCheckMemory());
	printf("t1: %s\n", t1_cstr);
	printf("t3: %s\n", t3_cstr);
	printf("t2: %s\n", t2_cstr);
	printf("t4: %s\n", t4_cstr);

	return 0;
}