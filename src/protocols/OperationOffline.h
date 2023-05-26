#ifndef MALICIOUS_PPML_OPERATIONOFFLINE_H
#define MALICIOUS_PPML_OPERATIONOFFLINE_H

#include "share/Spdz2kShare.h"

template<int K, int S>
class OperationOffline {
public:
    OperationOffline(const Spdz2kShare<K, S> &p_delta_z) : delta_z(p_delta_z) {}

    virtual void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) = 0;

protected:
    Spdz2kShare<K, S> delta_z;
};


template<int K, int S>
class AdditionOffline : public OperationOffline<K, S> {
public:
    AdditionOffline(const Spdz2kShare<K, S> &p_delta_z) : OperationOffline<K, S>(p_delta_z) {}

    void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) override {
        this->delta_z = delta_x + delta_y;
    }
};


template<int K, int S>
class MultiplicationOffline : public OperationOffline<K, S> {
protected:
    Spdz2kShare<K, S> delta_xy;
public:
    MultiplicationOffline(const Spdz2kShare<K, S> &p_delta_z, const Spdz2kShare<K, S> &p_delta_xy)
            : OperationOffline<K, S>(p_delta_z), delta_xy(p_delta_xy) {}

    void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) override {
//        this->delta_xy =
    }
};


template<int K, int S>
class MultiplicationOfflineFake : public MultiplicationOffline<K, S> {
public:
    MultiplicationOfflineFake(const Spdz2kShare<K, S> &p_delta_z, const Spdz2kShare<K, S> &p_delta_xy)
            : MultiplicationOffline<K, S>(p_delta_z, p_delta_xy) {}

    void runOffline(const Spdz2kShare<K, S> &delta_x, const Spdz2kShare<K, S> &delta_y) override {
        //Read from file
    }
};

#endif //MALICIOUS_PPML_OPERATIONOFFLINE_H
