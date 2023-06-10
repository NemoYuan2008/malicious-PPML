#ifndef MALICIOUS_PPML_ADDITIONGATE_H
#define MALICIOUS_PPML_ADDITIONGATE_H


#include <stdexcept>
#include <Eigen/Dense>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

template<typename ShrType>
class AdditionGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

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
        this->lambdaShr = matrixAdd(this->input_x->getLambdaShr(), this->input_y->getLambdaShr());
        this->lambdaShrMac = matrixAdd(this->input_x->getLambdaShrMac(), this->input_y->getLambdaShrMac());
    }

    void doRunOnline() override {
        this->deltaClear = matrixAdd(this->input_x->getDeltaClear(), this->input_y->getDeltaClear());
    }
};

#endif //MALICIOUS_PPML_ADDITIONGATE_H
