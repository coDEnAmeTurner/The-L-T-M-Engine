#include "CPUSpecs.h"

std::shared_ptr<CPUSpecs> CPUSpecs::s_instance = nullptr;

CPUSpecs::CPUSpecs() {
    int cpuInfo[4];

    //get word size
    __cpuid(cpuInfo, 0x80000008);  // Query leaf 0x80000008
    int eax = cpuInfo[0];
    int physical_address_bits = eax & 0xFF;         // bits 7..0
    int virtual_address_bits = (eax >> 8) & 0xFF;   // bits 15..8
    m_wordSize = (physical_address_bits + virtual_address_bits) / BITS_IN_BYTE;

    //get total logical core count
    __cpuid(cpuInfo, 0x0B);
    m_logicalCoreCount = cpuInfo[1] & 0xFFFF;

    //get total physical core count per processor package (die, socket,...)
    __cpuid(cpuInfo, 0x00000004); // Deterministic Cache Parameters Leaf
    m_physicalCoreCount = ((cpuInfo[0] >> 14) & 0xFFF) + 1;


}

void CPUSpecs::init() {
    if (s_instance == nullptr)
        s_instance = std::unique_ptr<CPUSpecs>(new CPUSpecs());
}
void CPUSpecs::destroy() {
    s_instance = nullptr;
}
std::shared_ptr<CPUSpecs> CPUSpecs::getInstance() {
	return s_instance;
}
std::uint8_t CPUSpecs::getWordSize()
{
    return s_instance->m_wordSize;
}
std::uint8_t CPUSpecs::getPhysicalCoreCount()
{
    return s_instance->m_physicalCoreCount;
}

std::uint8_t CPUSpecs::getLogicalCoreCount()
{
    return s_instance->m_logicalCoreCount;
}
