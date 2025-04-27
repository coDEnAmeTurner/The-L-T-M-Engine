#include <cstdint>
#include <string>
#include <cstring>
#include <emmintrin.h>
#include <intrin.h>
#include <thread>
#include <mutex>
#include <windows.h>
#include <Double-Ended Stack Allocator/DoubleEndedStackAllocator.h>
#include <Double-Buffured Allocator/DoubleBufferedAllocator.h>
#include <Stack Allocator/StackAllocator.h>
#include <Pool Allocator/PoolAllocator.h>
#include <File System/FileHandler.h>
#include <Test.h>


int main() {
	FileHandler o_file("hello.txt", std::ios::in);
	FileHandler w_file("hello_2.txt", std::ios::out);
	o_file.waitForFileHandling();
	w_file.waitForFileHandling();

	o_file.readFileAsync(11);
	const char* str = "Hello World";
	w_file.writeFileAsync(str, 11);
	o_file.waitForFileHandling();

	const char* buffer = o_file.getBuffer();
	printf("Read result: %s\n", buffer);

	w_file.waitForFileHandling();
	printf("Done writing!!");

	return 0;
}