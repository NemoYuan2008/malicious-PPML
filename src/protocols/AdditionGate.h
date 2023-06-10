#ifndef MALICIOUS_PPML_ADDITIONGATE_H
#define MALICIOUS_PPML_ADDITIONGATE_H


#include <stdexcept>
#include <Eigen/Dense>
#include "protocols/Gate.h"

template<typename ShrType>
class AdditionGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;

    using Gate<ShrType>::Gate;

    AdditionGate(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y)
            : Gate<ShrType>(input_x, input_y) {
        if (input_x->getDimX() != input_y->getDimX() || input_x->getDimY() != input_y->getDimY()) {
            throw std::logic_error("Dimension of the two inputs of addition don't match");
        }
        this->dimX = this->input_x->getDimX();
        this->dimY = this->input_x->getDimY();
    }

private:
    void doRunOffline() override {
        this->lambdaShr[0] = this->input_x->getLambdaShr()[0] + this->input_y->getLambdaShr()[0];
        this->lambdaShrMac[0] = this->input_x->getLambdaShrMac()[0] + this->input_y->getLambdaShrMac()[0];
    }

    void doRunOnline() override {
        this->deltaClear[0] = this->input_x->getDeltaClear()[0] + this->input_y->getDeltaClear()[0];
    }
};

#endif //MALICIOUS_PPML_ADDITIONGATE_H
