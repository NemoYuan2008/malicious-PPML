#ifndef MALICIOUS_PPML_GATE_H
#define MALICIOUS_PPML_GATE_H


#include "protocols/GateOffline.h"

template<int K, int S>
class Gate {
public:
    using KType = KType_t<K>;
    using SType = SType_t<S>;
    using KSType = KSType_t<K, S>;

protected:
    KType
};


#endif //MALICIOUS_PPML_GATE_H
