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

struct FuncParams {
	int i = 0;
};

void __stdcall ReadFile(std::shared_ptr<JobParams> p) {
	FuncParams* funcp = reinterpret_cast<FuncParams*>(p->m_funcParams.get());

	std::string filename = "log" + std::to_string(funcp->i) + ".txt";
	std::ofstream log(filename, std::ios::app);
	for (size_t i = 0; i < 100000; i++) {
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		log << "log from " << i << " loop" << std::endl;;
		log << sizeof(std::unique_ptr<std::fstream>) << std::endl;;
		log << sizeof(std::future<void>) << std::endl;
		log << sizeof(char*) << std::endl;;
	}
	printf("Finished writing %s!!!!\n", filename.c_str());

}

void testJobSystem() {
	for (int i = 0; i < 5; i++) 
	{
		std::shared_ptr<JobDeclaration> j = std::shared_ptr<JobDeclaration>(new JobDeclaration());
		j->m_pEntryPoint = ReadFile;
		std::shared_ptr<JobParams> p = std::shared_ptr<JobParams>(new JobParams());
		j->m_params = p;
		std::shared_ptr<FuncParams> funcp = std::shared_ptr<FuncParams>(new FuncParams());
		funcp->i = i;
		j->m_params->m_funcParams = funcp;

		JobSystem::kickJob(j);
		std::cout << "Just kick job " << i << std::endl;
	}

}
