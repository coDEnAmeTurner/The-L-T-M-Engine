#pragma once
#include <cstdint>
#include <string>
#include <JobSystem.h>
#include <FileHandler.h>
#include <filesystem>
#include <GameLoop Common.h>

struct Test {
	std::uint32_t m_ui; // 4 bytes
	double m_d; // 8 bytes
	//implicit: 4 bytes for the padding of uint32_t

	Test(std::uint32_t ui, double d) : m_ui(ui), m_d(d) { }

	std::string ToString() {
		return std::to_string(m_ui) + " " + std::to_string(m_d);
	}
};

void __stdcall ReadFile(void* p) {
	JobParams* params = reinterpret_cast<JobParams*>(p);
	auto fin_ptr = params->m_finished.load(std::memory_order_relaxed);
	*fin_ptr = false;
	params->m_finished.store(fin_ptr, std::memory_order_release);
	JobSystem::incrementCounter(params->m_pCounter);

	/*for (size_t i = 0; i < 12; i++) {
		void* path_ptr = params->m_stack->allocate(sizeof(std::string));
		std::string* file_path = new(path_ptr) std::string("D:\\The-L-T-M-Engine\\The L-T-M Engine\\The L-T-M Core\\");
		void* name_ptr = params->m_stack->allocate(sizeof(std::string));
		std::string* file_name = new(name_ptr)std::string("hello" + std::to_string(i)+".txt");
		void* file_ptr = params->m_stack->allocate(sizeof(FileHandler));
		FileHandler* file_handler = new(file_ptr)FileHandler(*file_name, std::ios::in);
		file_handler->waitForFileHandling();
		file_handler->readFileAsync(100);
		file_handler->waitForFileHandling();
		char* str = file_handler->getBuffer();
		file_name = new(name_ptr)std::string("hello" + std::to_string(i) + "_w");
		file_handler = new(file_ptr)FileHandler(*file_name, std::ios::out);
		file_handler->waitForFileHandling();
		file_handler->writeFileAsync(str, 100);
		file_handler->waitForFileHandling();
		__debugbreak();
		printf("%p\n", GetCurrentFiber());

	}*/
	

	std::ofstream log("log.txt", std::ios::app);
	log << "Worker thread ran\n";

	/*JobSystem::decrementCounter(params->m_pCounter);
	*fin_ptr = true;
	params->m_finished.store(fin_ptr, std::memory_order_release);*/
}

//std::atomic<bool> running = true;

void __stdcall worker(void* p) {
	/*while (running.load()) {
		std::cout << "Worker tick\n" << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}*/
	std::cout << "Worker exiting\n" << std::flush;
}

void testJobSystem() {
	JobDeclaration j;
	j.m_pEntryPoint = worker;
	JobParams p;
	j.m_params = &p;

	JobSystem::kickJob(j);
	JobSystem::waitForCounter(j.m_params->m_pCounter);

	printf("Finished reading file!!!!\n");

}
