//
// Created by xgg on 30 Mar 2022.
//

#ifndef LAB03_LEHMER64_H
#define LAB03_LEHMER64_H

#include <stdint-gcc.h>

static inline uint64_t splitmix64_stateless(uint64_t index) {
    uint64_t z = (index + UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}

static __uint128_t g_lehmer64_state;

static inline void lehmer64_seed(uint64_t seed) {
    g_lehmer64_state = (((__uint128_t)splitmix64_stateless(seed)) << 64) +
                       splitmix64_stateless(seed + 1);
}

static inline uint64_t lehmer64() {
    g_lehmer64_state *= UINT64_C(0xda942042e4dd58b5);
    return g_lehmer64_state >> 64;
}


#endif //LAB03_LEHMER64_H
