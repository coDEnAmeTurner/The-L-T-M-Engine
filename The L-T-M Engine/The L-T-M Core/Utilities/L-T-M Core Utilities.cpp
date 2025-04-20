#include "L-T-M Core Utilities.h"

std::uint32_t log2(std::uint32_t x) {
    unsigned long index;
    _BitScanReverse(&index, x);
    return index;
}

bool is_pow_of_2(std::uint32_t x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

std::uint16_t pow_of_2(std::uint32_t x)
{
    assert(x <= 15);

    return 0b0000000000000001 << x;
}

