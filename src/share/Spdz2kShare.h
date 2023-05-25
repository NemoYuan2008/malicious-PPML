//
// Created by yuan on 23-5-24.
//

#ifndef MALICIOUS_PPML_SPDZ2KSHARE_H
#define MALICIOUS_PPML_SPDZ2KSHARE_H

#include <cstdint>
#include <type_traits>


template<int K, int S>
class Spdz2kShare;

template<int K, int S>
inline Spdz2kShare<K, S> operator+(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs);

template<int K, int S>
inline Spdz2kShare<K, S> operator-(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs);


template<int K, int S>
class Spdz2kShare {
    static_assert(S <= 64 && K <= 64, "S or K larger than 64 is not supported");

public:
    using MacType = std::conditional_t<S <= 32, uint32_t, uint64_t>;
    using DataType = std::conditional_t<K <= 32, uint32_t, uint64_t>;
    using ShareType = std::conditional_t<S + K <= 64, uint64_t, __uint128_t>;

    Spdz2kShare() = default;

    Spdz2kShare(ShareType x, MacType mac) : x(x), mac(mac) {}

    inline Spdz2kShare operator+=(const Spdz2kShare &rhs);

    friend inline Spdz2kShare operator+<S, K>(const Spdz2kShare &lhs, const Spdz2kShare &rhs);

    inline Spdz2kShare operator-=(const Spdz2kShare &rhs);

    friend inline Spdz2kShare operator-<S, K>(const Spdz2kShare &lhs, const Spdz2kShare &rhs);


private:
    ShareType x;
    MacType mac;
};


template<int K, int S>
inline
Spdz2kShare<K, S> Spdz2kShare<K, S>::operator+=(const Spdz2kShare &rhs) {
    mac += rhs.mac;
    x += rhs.x;
    return *this;
}

template<int K, int S>
inline
Spdz2kShare<K, S> operator+(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs) {
    Spdz2kShare<K, S> ret(lhs);
    ret += rhs;
    return ret;
}

template<int K, int S>
inline
Spdz2kShare<K, S> Spdz2kShare<K, S>::operator-=(const Spdz2kShare &rhs) {
    mac -= rhs.mac;
    x -= rhs.x;
    return *this;
}

template<int K, int S>
inline
Spdz2kShare<K, S> operator-(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs) {
    Spdz2kShare<K, S> ret(lhs);
    ret -= rhs;
    return ret;
}


extern template
class Spdz2kShare<32, 32>;

extern template
class Spdz2kShare<64, 64>;

#endif //MALICIOUS_PPML_SPDZ2KSHARE_H
