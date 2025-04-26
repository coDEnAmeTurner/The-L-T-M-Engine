#pragma once
#include <intrin.h>
#include<cstdint>
#include<cassert>

template<typename T>
inline T ternary_pred(const bool& cond, const T& val_t, const T& val_f) {
    const std::uint64_t condition = (uint64_t)(cond);
    // convert 1U -> 0xFFFFFFFFU
    // convert 0U -> 0x00000000U
    const std::uint64_t mask = 0U - condition;

    union { std::uint64_t i; T t; } u;
    union { std::uint64_t i; T t; } u1;
    u.t = val_t;
    u1.t = val_f;
    // select quotient when mask is all ones, or default
    // value d when mask is all zeros (NOTE: this won't
    // work as written -- you'd need to use a union to
    // interpret the floats as unsigned for masking)
    union { std::uint64_t i; T t; } u2;
    u2.i = (u.i & mask) | (u1.i & ~mask);
    return u2.t;
}

extern inline std::uint32_t log2(std::uint32_t x);
extern inline bool is_pow_of_2(std::uint32_t x);
extern inline std::uint16_t pow_of_2(std::uint32_t x); 
extern inline bool check_u32_divisor(std::uint32_t numerator, std::uint32_t denominator);

#define MINIMUM_ALIGNMENT 8