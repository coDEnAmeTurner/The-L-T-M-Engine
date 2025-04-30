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

#define WordSize CPUSpecs::getWordSize()
#define PhysicalCoreCount CPUSpecs::getPhysicalCoreCount()
#define LogicalCoreCount CPUSpecs::getLogicalCoreCount()
#define BITS_IN_BYTE 8
#define MINIMUM_ALIGNMENT WordSize

private:
	static std::shared_ptr<CPUSpecs> s_instance;

	//instance members
	std::uint8_t m_wordSize;
	std::uint8_t m_physicalCoreCount;
	std::uint8_t m_logicalCoreCount;

	CPUSpecs();
};

