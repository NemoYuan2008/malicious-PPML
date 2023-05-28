#ifndef MALICIOUS_PPML_GATE_H
#define MALICIOUS_PPML_GATE_H


#include <memory>
#include "protocols/GateOffline.h"

template<int K, int S>
class Gate {
public:
    using KType = KType_t<K>;
    using SType = SType_t<S>;
    using KSType = KSType_t<K, S>;

    Gate() = default;

    Gate(const std::shared_ptr<Gate> &input_x, const std::shared_ptr<Gate> &input_y)
            : input_x(input_x), input_y(input_y) {}

    virtual void runOffline() = 0;

    const Spdz2kShare<K, S> &getLambda_zShr() const { return lambda_zShr; }  //TODO: return ref of value?

protected:
    //Maybe: define clearLambda here for debugging purpose
    Spdz2kShare<K, S> lambda_zShr;
    std::shared_ptr<Gate> input_x, input_y;
};


template<int K, int S>
class AdditionGate : public Gate<K, S> {
public:
    void runOffline() override {
        this->lambda_zShr = this->input_x->getLambda_zShr() + this->input_y->getLambda_zShr();
    }
};


template<int K, int S>
class MultiplicationGate : public Gate<K, S> {
public:
    void runOffline() override {
        // generate random lambda_zShr
        // compute triple
    }

protected:
    Spdz2kShare<K, S> lambda_xyShr;
};


#endif //MALICIOUS_PPML_GATE_H
