#ifndef MALICIOUS_PPML_SHARE_H
#define MALICIOUS_PPML_SHARE_H

#include <cstdint>
#include "share/Spdz2kShare.h"


template <int K, int S>
class Share {
public:
    using DataType = typename Spdz2kShare<K, S>::DataType;
private:
    Spdz2kShare<K, S> sharedDelta;
    DataType publicDelta;
};


#endif //MALICIOUS_PPML_SHARE_H
