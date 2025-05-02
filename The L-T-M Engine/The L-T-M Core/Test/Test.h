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

	for (size_t i = 0; i < 12; i++) {
		void* path_ptr = params->m_stack->allocate(sizeof(std::string));
		std::string* file_path = new(path_ptr) std::string("D:\\The-L-T-M-Engine\\The L-T-M Engine\\The L-T-M Core\\");
		void* name_ptr = params->m_stack->allocate(sizeof(std::string));
		std::string* file_name = new(name_ptr)std::string("hello" + std::to_string(i)+".txt");
		std::ofstream log("log.txt", std::ios::app);
		log << std::to_string(sizeof(std::string)) << std::endl;;
		log << sizeof(std::unique_ptr<std::fstream>) << std::endl;;
		log << sizeof(std::future<void>) << std::endl;;
		log << sizeof(char*) << std::endl;;
		void* file_ptr = params->m_stack->allocate(sizeof(FileHandler), 64, true);
		FileHandler* file_handler = new(file_ptr)FileHandler(*file_name, std::ios::in);
		file_handler->waitForFileHandling();
		file_handler->readFileAsync(100);
		file_handler->waitForFileHandling();
		char* str = file_handler->getBuffer();
		size_t m_size = file_handler->getSize();
		file_name = new(name_ptr)std::string("hello" + std::to_string(i) + "_w.txt");
		file_handler = new(file_ptr)FileHandler(*file_name, std::ios::out);
		file_handler->waitForFileHandling();
		file_handler->writeFileAsync(str, m_size);
		file_handler->waitForFileHandling();

	}
	

	/*std::ofstream log("log.txt", std::ios::app);
	log << "Worker thread ran\n";*/

}

void testJobSystem() {
	JobDeclaration j;
	j.m_pEntryPoint = ReadFile;
	JobParams p;
	j.m_params = &p;

	JobSystem::kickJob(j);
	JobSystem::waitForCounter(j.m_params->m_pCounter);

	printf("Finished reading file!!!!\n");

}
