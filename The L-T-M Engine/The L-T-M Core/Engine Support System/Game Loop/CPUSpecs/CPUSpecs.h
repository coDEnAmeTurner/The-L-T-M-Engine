#pragma once
#include <intrin.h>
#include <cstdint>
#include <memory>
#include <L-T-M Core Utilities.h>

class CPUSpecs
{
public:
	static std::shared_ptr<CPUSpecs> getInstance();
	static void init();
	static void destroy();
	static std::uint8_t getWordSize();
	static std::uint8_t getPhysicalCoreCount();
	static std::uint8_t getLogicalCoreCount();

#define WORD_SIZE CPUSpecs::getWordSize()
#define PHYSICAL_CORE_COUNT CPUSpecs::getPhysicalCoreCount()
#define LOGICAL_CORE_COUNT CPUSpecs::getLogicalCoreCount()

private:
	static std::shared_ptr<CPUSpecs> s_instance;

	//instance members
	std::uint8_t m_wordSize;
	std::uint8_t m_physicalCoreCount;
	std::uint8_t m_logicalCoreCount;


	CPUSpecs();
};

#define BITS_IN_BYTE 8
#define MINIMUM_ALIGNMENT WORD_SIZE
