#ifndef MALICIOUS_PPML_FAKEOFFLINE_H
#define MALICIOUS_PPML_FAKEOFFLINE_H


#include <array>
#include <algorithm>
#include <numeric>
#include "share/types.h"
#include "share/Spdz2kShare.h"
#include "utils/rand.h"

template<int K, int S, int N>
class FakeOffline {
public:
    using KType = KType_t<K>;
    using SType = SType_t<S>;
    using KSType = KSType_t<K, S>;
    using Shares = std::array<Spdz2kShare<K, S>, N>;

    FakeOffline(SType p_key) : key(p_key) {}

    inline static std::array<KSType, N> splitN(KSType x);

    inline Shares generateShares(KSType x);

    inline Shares getSpdz2kTriple(KSType a, KSType b);

private:
    //TODO: really SType???
    SType key;
};

template<int K, int S, int N>
inline
std::array<KSType_t<K, S>, N> FakeOffline<K, S, N>::splitN(FakeOffline::KSType x) {
    std::array<KSType, N> ret;
    std::generate(ret.begin(), ret.end() - 1, getRand<KSType>);
    ret.back() = -std::accumulate(ret.begin(), ret.end() - 1, -x);
    return ret;
}


//It is OK to pass a KType argument
template<int K, int S, int N>
typename FakeOffline<K, S, N>::Shares FakeOffline<K, S, N>::generateShares(KSType x) {
    KSType mac = x * key;
    auto x_i = splitN(x), mac_i = splitN(mac);
    Shares ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = Spdz2kShare<K, S>(x_i[i], mac_i[i]);
    }
    return ret;
}

template<int K, int S, int N>
typename FakeOffline<K, S, N>::Shares FakeOffline<K, S, N>::getSpdz2kTriple(KSType a, KSType b) {
    return generateShares(a * b);
}

#endif //MALICIOUS_PPML_FAKEOFFLINE_H
