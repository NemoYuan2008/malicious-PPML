#ifndef MALICIOUS_PPML_FAKEOFFLINE_H
#define MALICIOUS_PPML_FAKEOFFLINE_H

#include <cstdlib>
#include <array>
#include "share/types.h"
#include "share/Spdz2kShare.h"

template<int K, int S>
typename Spdz2kShare<K, S>::KSType rand() {
    using KSType = typename Spdz2kShare<K, S>::KSType;
    KSType ret;
    auto *ptr = reinterpret_cast<uint8_t *>(&ret);
    for (int i = 0; i < sizeof(ret); ++i) {
        *ptr = rand();      //TODO: rand() is unsafe
        ++ptr;
    }
    return ret;
}


template<int K, int S, int N>
class FakeOffline {
public:
    using KType = KType_t<K>;
    using SType = SType_t<S>;
    using KSType = KSType_t<K, S>;
    using Shares = std::array<Spdz2kShare<K, S>, N>;

    inline static std::array<KSType, N> splitN(KSType x);

    inline static KSType rand();

    inline Shares generateShares(KSType x);

    inline Shares getSpdz2kTriple(KSType a, KSType b);

private:
    SType key;
};

template<int K, int S, int N>
std::array<typename FakeOffline<K, S, N>::KSType, N> FakeOffline<K, S, N>::splitN(FakeOffline::KSType x) {
    std::array<KSType, N> ret;
    //TODO: randomize x
    ret[0] = x;
    return ret;
}

template<int K, int S, int N>
typename FakeOffline<K, S, N>::KSType FakeOffline<K, S, N>::rand() {
    KSType ret;
    auto *ptr = reinterpret_cast<uint8_t *>(&ret);
    for (int i = 0; i < sizeof(ret); ++i) {
        *ptr = rand();      //TODO: rand() is unsafe
        ++ptr;
    }
    return ret;
}

template<int K, int S, int N>
typename FakeOffline<K, S, N>::Shares FakeOffline<K, S, N>::generateShares(KSType x) {
    KSType mac = x * key;
    auto x_i = splitN(x), mac_i = splitN(mac);
    Shares ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = Spdz2kShare<K, S>(x[i], mac_i[i]);
    }
    return ret;
}

template<int K, int S, int N>
typename FakeOffline<K, S, N>::Shares FakeOffline<K, S, N>::getSpdz2kTriple(KSType a, KSType b) {
    return generateShares(a * b);
}

#endif //MALICIOUS_PPML_FAKEOFFLINE_H
