#ifndef MALICIOUS_PPML_GATE_H
#define MALICIOUS_PPML_GATE_H


#include <memory>
#include "protocols/GateOffline.h"

template<typename ShrType>
class Gate {
public:
    Gate() = default;

    Gate(const std::shared_ptr<Gate> &input_x, const std::shared_ptr<Gate> &input_y)
            : input_x(input_x), input_y(input_y) {}

    virtual void runOffline() = 0;

    const ShrType &getLambda_zShr() const { return lambda_zShr; }  //TODO: return ref of value?

protected:
    //Maybe: define clearLambda here for debugging purpose
    ShrType lambda_zShr;
    std::shared_ptr<Gate> input_x, input_y;
};


template<typename ShareType>
class AdditionGate : public Gate<ShareType> {
public:
    void runOffline() override {
        this->lambda_zShr = this->input_x->getLambda_zShr() + this->input_y->getLambda_zShr();
    }
};


template<typename ShareType>
class MultiplicationGate : public Gate<ShareType> {
public:
    void runOffline() override {
        // generate random lambda_zShr
        // compute triple
    }

protected:
    ShareType lambda_xyShr;
};


#endif //MALICIOUS_PPML_GATE_H
