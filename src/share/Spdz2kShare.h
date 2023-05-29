#ifndef MALICIOUS_PPML_SPDZ2KSHARE_H
#define MALICIOUS_PPML_SPDZ2KSHARE_H

#include <iostream>
#include "share/types.h"
#include "share/uint128.h"

template<int K, int S>
class Spdz2kShare;

template<int K, int S>
inline bool operator==(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs);

template<int K, int S>
inline Spdz2kShare<K, S> operator+(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs);

template<int K, int S>
inline Spdz2kShare<K, S> operator-(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs);

template<int K, int S>
std::ostream &operator<<(std::ostream &os, const Spdz2kShare<K, S> &rhs);


//TODO: check types
template<int K, int S>
class Spdz2kShare {
    static_assert(S <= 64 && K <= 64, "S or K must be no larger than 64");

public:
    using KType = KType_t<K>;
    using SType = SType_t<S>;
    using KSType = KSType_t<K, S>;
    using ClearType = KType;

    Spdz2kShare() = default;

    Spdz2kShare(KSType x, KSType mac) : xi(x), mi(mac) {}

    friend bool operator==<S, K>(const Spdz2kShare &lhs, const Spdz2kShare &rhs);

    inline Spdz2kShare operator+=(const Spdz2kShare &rhs);

    friend Spdz2kShare operator+<S, K>(const Spdz2kShare &lhs, const Spdz2kShare &rhs);

    Spdz2kShare operator-=(const Spdz2kShare &rhs);

    friend Spdz2kShare operator-<S, K>(const Spdz2kShare &lhs, const Spdz2kShare &rhs);

    friend std::ostream &operator<<<S, K>(std::ostream &os, const Spdz2kShare &rhs);

    KSType getXi() const { return xi; }

    KSType getMi() const { return mi; }

private:
    KSType xi;
    KSType mi;
};


template<int K, int S>
inline
bool operator==(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs) {
    return lhs.xi == rhs.xi && lhs.mi == rhs.mi;
}

template<int K, int S>
inline
Spdz2kShare<K, S> Spdz2kShare<K, S>::operator+=(const Spdz2kShare &rhs) {
    mi += rhs.mi;
    xi += rhs.xi;
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
    mi -= rhs.mi;
    xi -= rhs.xi;
    return *this;
}

template<int K, int S>
inline
Spdz2kShare<K, S> operator-(const Spdz2kShare<K, S> &lhs, const Spdz2kShare<K, S> &rhs) {
    Spdz2kShare<K, S> ret(lhs);
    ret -= rhs;
    return ret;
}

template<int K, int S>
inline
std::ostream &operator<<(std::ostream &os, const Spdz2kShare<K, S> &rhs) {
    os << std::hex
       << "xi = " << rhs.xi << ", "
       << "mi = " << rhs.mi << std::dec;
    return os;
}


extern template
class Spdz2kShare<32, 32>;

extern template
class Spdz2kShare<64, 64>;


using Spdz2kShare32 = Spdz2kShare<32, 32>;
using Spdz2kShare64 = Spdz2kShare<64, 64>;

#endif //MALICIOUS_PPML_SPDZ2KSHARE_H
