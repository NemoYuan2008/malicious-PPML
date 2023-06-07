#ifndef MALICIOUS_PPML_FAKEOFFLINE_H
#define MALICIOUS_PPML_FAKEOFFLINE_H


#include <array>
#include <algorithm>
#include <numeric>
#include "offline/FakeOfflineBase.h"
#include "share/types.h"
#include "share/Spdz2kShare.h"
#include "utils/rand.h"


//TODO: clean up code



//TODO: check types
template<int K, int S, int N>
class FakeOffline : public FakeOfflineBase<Spdz2kShare<K, S>, N> {
public:
    using KType = KType_t<K>;
    using SType = SType_t<S>;
    using KSType = KSType_t<K, S>;

    using typename FakeOfflineBase<Spdz2kShare<K, S>, N>::ClearType;
    using typename FakeOfflineBase<Spdz2kShare<K, S>, N>::Shares;
    using PartyKeyType = typename Spdz2kShare<K, S>::PartyKeyType;

public:
    FakeOffline() {     // Should only be used for tests
        key = 0;
        for (int i = 0; i < N; ++i) {
            keyShares[i] = getRand<SType>();
            key += static_cast<KSType>(keyShares[i]);
        }
    }

    explicit FakeOffline(std::array<std::ostream *, N> &filePtrs) : files(filePtrs) {
        key = 0;
        for (int i = 0; i < N; ++i) {
            keyShares[i] = getRand<SType>();
            key += static_cast<KSType>(keyShares[i]);

            *files[i] << keyShares[i] << '\n';
        }
    }

    explicit FakeOffline(std::array<std::ofstream, N> &fileObjects) {
        key = 0;
        for (int i = 0; i < N; ++i) {
            keyShares[i] = getRand<SType>();
            key += static_cast<KSType>(keyShares[i]);

            fileObjects[i] << keyShares[i] << '\n';
            files[i] = &fileObjects[i];
        }
    }

    Shares generateShares(ClearType x) const override;

    static KType openShares(const Shares &shares);

    static std::array<KSType, N> splitN(KSType x);

    KSType getGlobalKey() const { return key; }

    PartyKeyType getPartyKey(int id) const { return keyShares.at(id); }

private:
    std::array<SType, N> keyShares;
    KSType key;
    std::array<std::ostream *, N> files;
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
typename FakeOffline<K, S, N>::Shares FakeOffline<K, S, N>::generateShares(FakeOffline::ClearType x) const {
    auto mask = getRand<SType>();   //mask higher S bits of x
    KSType x_masked = static_cast<KSType>(mask) << K | x;

    KSType mac = x_masked * key;
    auto x_i = splitN(x_masked), mac_i = splitN(mac);

    Shares ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = Spdz2kShare<K, S>(x_i[i], mac_i[i]);
    }
    return ret;
}

#endif //MALICIOUS_PPML_FAKEOFFLINE_H
