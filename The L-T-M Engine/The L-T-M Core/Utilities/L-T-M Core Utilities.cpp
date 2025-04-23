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
bool check_u32_divisor(std::uint32_t numerator, std::uint32_t denominator) {
    std::uint32_t deno_expo = log2(denominator);
    std::uint32_t nume_expo = log2(numerator);

    return nume_expo >= deno_expo;
}

