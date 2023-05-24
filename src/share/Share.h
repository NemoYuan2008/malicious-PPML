#ifndef MALICIOUS_PPML_SHARE_H
#define MALICIOUS_PPML_SHARE_H

#include <cstdint>
#include "share/Spdz2kShare.h"


template <int K, int S>
class Share {
public:
    using DataType = typename Spdz2kShare<K, S>::DataType;

    inline Share operator+=(const Share &rhs);
private:
    Spdz2kShare<K, S> sharedDelta;
    DataType publicDelta;
};

template<int K, int S>
Share<K, S> Share<K, S>::operator+=(const Share &rhs) {
    //TODO: sharedDelta OFFLINE!!!!!!
    sharedDelta += rhs.sharedDelta;
    publicDelta += rhs.publicDelta;
    return *this;
}


#endif //MALICIOUS_PPML_SHARE_H
