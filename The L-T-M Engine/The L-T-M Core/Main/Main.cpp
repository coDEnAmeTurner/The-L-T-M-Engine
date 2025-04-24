#include <cstdint>
#include <string>
#include <cstring>
#include <emmintrin.h>
#include <intrin.h>
#include <thread>
#include <mutex>
#include "../Memory Management/Double-Ended Stack Allocator/DoubleEndedStackAllocator.h"
#include "../Memory Management/Double-Buffured Allocator/DoubleBufferedAllocator.h"
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

std::mutex mtx;
std::thread t1;

inline void WriteTest(void* test) {
	std::this_thread::sleep_for(std::chrono::seconds(1));	

	mtx.lock();
	Test* test_obj = new (test) Test(1, 2.0); // placement new	
	printf("Just wrote to test: %s\n", test_obj->ToString().c_str());
	mtx.unlock();

	return;
}

int main() {
	DoubledBufferedAllocator allocator(1024, 8);

	for (int i = 0; i < 4; i++) {
		allocator.swapBuffers();

		if (i % 2 == 0) // every second iteration
			mtx.lock();
		allocator.clearCurrentBuffer();
		if (i % 2 == 0) // every second iteration
			mtx.unlock();

		if (i == 0)
		{
			void* test1 = allocator.alloc(sizeof(Test));
			t1 = std::thread(WriteTest, test1);
		}

		void* buffer = allocator.alloc(sizeof(Test));
		Test* buffer_obj = new (buffer) Test(1, 16.789); // placement new
		printf("Just wrote to buffer: %s\n", buffer_obj->ToString().c_str());

	}

	if (t1.joinable()) t1.join();

	return 0;
}