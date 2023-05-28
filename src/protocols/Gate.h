#ifndef MALICIOUS_PPML_GATE_H
#define MALICIOUS_PPML_GATE_H


#include "protocols/GateOffline.h"

template<int K, int S>
class Gate {
public:
    using KType = KType_t<K>;
    using SType = SType_t<S>;
    using KSType = KSType_t<K, S>;

    virtual void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) = 0;

protected:
    Spdz2kShare<K, S> delta_z;
};


template<int K, int S>
class AdditionGate : public Gate<K, S> {
public:
    virtual void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) override {
        this->delta_z = delta_x + delta_y;
    }
};


template<int K, int S>
class MultiplicationGate : public Gate<K, S> {
public:

    void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) override {
//        this->delta_xy =
    }

protected:
    Spdz2kShare<K, S> delta_xy;
};


#endif //MALICIOUS_PPML_GATE_H
