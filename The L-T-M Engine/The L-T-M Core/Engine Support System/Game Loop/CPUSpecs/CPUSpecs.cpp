#include "CPUSpecs.h"

std::shared_ptr<CPUSpecs> CPUSpecs::s_instance = nullptr;

CPUSpecs::CPUSpecs() {
    int cpuInfo[4];

    //get word size
    m_wordSize = 32;
    __cpuid(cpuInfo, 0x80000000);

    if (cpuInfo[0] >= 0x80000001) {
        __cpuid(cpuInfo, 0x80000001);
        m_wordSize = ternary_pred((cpuInfo[3] & (1 << 29)) != 0, (uint8_t)64, m_wordSize); // Check EDX bit 29
    }

    //get number of logical and physical cores
    int smtCount = 0;
    int logicalProcessors = 0;
    for (int level = 0;; ++level) {
        __cpuidex(cpuInfo, 0x0B, level);
        int levelType = (cpuInfo[2] >> 8) & 0xFF;
        int num = cpuInfo[1] & 0xFFFF;

        if (num == 0)
            break;

        if (levelType == 1)  // Core level
            logicalProcessors = num;
        else if (levelType == 2) // SMT (HyperThreading) level
            smtCount = num;
    }
    m_physicalCoreCount = smtCount/ logicalProcessors;
    m_logicalCoreCount = logicalProcessors;
    m_allThreadCount = smtCount;

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

std::uint8_t CPUSpecs::getAllThreadCount()
{
    return s_instance->m_allThreadCount;
}
