#ifndef MALICIOUS_PPML_GATEOFFLINE_H
#define MALICIOUS_PPML_GATEOFFLINE_H

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"


//TODO: getter for delta values
template<int K, int S>
class GateOffline {
public:
    GateOffline() = default;
    GateOffline(const Spdz2kShare<K, S> &p_delta_z) : delta_z(p_delta_z) {}

    virtual void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) = 0;

//protected:
    Spdz2kShare<K, S> delta_z;
};


template<int K, int S>
class AdditionOffline : public GateOffline<K, S> {
public:
    AdditionOffline() = default;
    AdditionOffline(const Spdz2kShare<K, S> &p_delta_z) : GateOffline<K, S>(p_delta_z) {}

    void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) override {
        this->delta_z = delta_x + delta_y;
    }
};


template<int K, int S>
class MultiplicationOffline : public GateOffline<K, S> {
public:
    MultiplicationOffline() = default;
    MultiplicationOffline(const Spdz2kShare<K, S> &p_delta_z, const Spdz2kShare<K, S> &p_delta_xy)
            : GateOffline<K, S>(p_delta_z), delta_xy(p_delta_xy) {}

    void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) override {
//        this->delta_xy =
    }

    Spdz2kShare<K, S> delta_xy;
};


template<int K, int S>
class MultiplicationOfflineFake : public MultiplicationOffline<K, S> {
public:
    MultiplicationOfflineFake() = default;
    MultiplicationOfflineFake(const Spdz2kShare<K, S> &p_delta_z, const Spdz2kShare<K, S> &p_delta_xy)
            : MultiplicationOffline<K, S>(p_delta_z, p_delta_xy) {}

    void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) override {

    }

    void runFakeOffline(const Spdz2kShare<K, S> &delta_z, const Spdz2kShare<K, S> &delta_xy) {
        this->delta_z = delta_z;
        this->delta_xy = delta_xy;
    }
};

#endif //MALICIOUS_PPML_GATEOFFLINE_H
