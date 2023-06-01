#ifndef MALICIOUS_PPML_FAKEOFFLINE_H
#define MALICIOUS_PPML_FAKEOFFLINE_H


#include <array>
#include <algorithm>
#include <numeric>
#include "share/types.h"
#include "share/Spdz2kShare.h"
#include "utils/rand.h"


//TODO: clean up code

template<typename ShrType, int N>
class FakeOfflineBase {
public:
    using ClearType = typename ShrType::ClearType;
    using Shares = std::array<ShrType, N>;

    virtual Shares generateShares(ClearType x) = 0;
};


//TODO: check types
template<int K, int S, int N>
class FakeOffline : FakeOfflineBase<Spdz2kShare<K, S>, N> {
public:
    using typename FakeOfflineBase<Spdz2kShare<K, S>, N>::ClearType;
    using typename FakeOfflineBase<Spdz2kShare<K, S>, N>::Shares;

private:
    using KType = KType_t<K>;
    using SType = SType_t<S>;
    using KSType = KSType_t<K, S>;

public:
    FakeOffline() : key(getRand<KSType>()) {}

    explicit FakeOffline(KSType p_key) : key(p_key) {}

    Shares generateShares(ClearType x) override;

    static KType openShares(const Shares &shares);

    static std::array<KSType, N> splitN(KSType x);

    KSType getKey() const { return key; }

    Shares getSpdz2kTriple(KSType a, KSType b);   //We don't need it, triples are generated in FakeOfflineGate

private:
    Shares generateShares(KSType x);    //TODO: remove this function and rewrite generateShares(ClearType x)?

    KSType key;
};


template<int K, int S, int N>
inline
typename FakeOffline<K, S, N>::KType FakeOffline<K, S, N>::openShares(const FakeOffline::Shares &shares) {
    KType ret = 0;  // Only need lower k bits
    for (const auto &share: shares) {
        ret += share.getXi();
    }
    return ret;
}

template<int K, int S, int N>
inline
std::array<KSType_t<K, S>, N> FakeOffline<K, S, N>::splitN(FakeOffline::KSType x) {
    std::array<KSType, N> ret;
    // Get N-1 random numbers and compute the last
    std::generate(ret.begin(), ret.end() - 1, getRand<KSType>);
    ret.back() = -std::accumulate(ret.begin(), ret.end() - 1, -x);
    return ret;
}


template<int K, int S, int N>
typename FakeOffline<K, S, N>::Shares FakeOffline<K, S, N>::generateShares(FakeOffline::ClearType x) {
    auto mask = getRand<SType>();   //mask higher S bits of x
    return generateShares(static_cast<KSType>(mask) << K | x);
}


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
