#ifndef MALICIOUS_PPML_FAKEOFFLINEBASE_H
#define MALICIOUS_PPML_FAKEOFFLINEBASE_H


#include <array>

template<typename ShrType, int N>
class FakeOfflineBase {
public:
    using ClearType = typename ShrType::ClearType;
    using Shares = std::array<ShrType, N>;

    virtual Shares generateShares(ClearType x) const = 0;
};


#endif //MALICIOUS_PPML_FAKEOFFLINEBASE_H
