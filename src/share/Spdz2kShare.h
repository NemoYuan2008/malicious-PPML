//
// Created by yuan on 23-5-24.
//

#ifndef MALICIOUS_PPML_SPDZ2KSHARE_H
#define MALICIOUS_PPML_SPDZ2KSHARE_H

#include <cstdint>
#include <type_traits>

template<int K, int S>
class Spdz2kShare {
    static_assert(S <= 64 && K <= 64);

public:
    using MacType = std::conditional_t<S <= 32, uint32_t, uint64_t>;
    using DataType = std::conditional_t<K <= 32, uint32_t, uint64_t>;
    using ShareType = std::conditional_t<S + K <= 64, uint64_t, __uint128_t>;


private:
    MacType mac;
    ShareType x;
};


#endif //MALICIOUS_PPML_SPDZ2KSHARE_H
