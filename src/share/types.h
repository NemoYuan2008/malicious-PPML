#ifndef MALICIOUS_PPML_TYPES_H
#define MALICIOUS_PPML_TYPES_H

#include <cstdint>
#include <type_traits>

template<int K>
using KType_t = std::conditional_t<K <= 32, uint32_t, uint64_t>;

template<int S>
using SType_t = KType_t<S>; // Same as KType_t, using a different name to prevent confusion

template<int K, int S>
using KSType_t = std::conditional_t<K + S <= 64, uint64_t, __uint128_t>;

#endif //MALICIOUS_PPML_TYPES_H
