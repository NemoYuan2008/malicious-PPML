#ifndef MALICIOUS_PPML_SUBTRACTIONGATE_H
#define MALICIOUS_PPML_SUBTRACTIONGATE_H


#include <stdexcept>
#include <algorithm>
#include <execution>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

template<typename ShrType>
class SubtractionGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    using Gate<ShrType>::Gate;

    SubtractionGate(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y)
            : Gate<ShrType>(input_x, input_y) {
        if (input_x->getDimRow() != input_y->getDimRow() || input_x->getDimCol() != input_y->getDimCol()) {
            throw std::logic_error("Dimension of the two inputs of subtraction don't match");
        }
        this->dimRow = this->input_x->getDimRow();
        this->dimCol = this->input_x->getDimCol();
    }

private:
//    void doReadOfflineFromFile(std::ifstream &ifs) override {
//        doRunOffline();
//    }

    void doRunOffline() override {
//        this->lambdaShr = matrixSubtract(this->input_x->getLambdaShr(), this->input_y->getLambdaShr());
//        this->lambdaShrMac = matrixSubtract(this->input_x->getLambdaShrMac(), this->input_y->getLambdaShrMac());
    }

    void doRunOnline() override {
//        this->deltaClear.resize(this->input_x->getDeltaClear().size());
//        std::transform(std::execution::par_unseq,
//                       this->input_x->getDeltaClear().begin(), this->input_x->getDeltaClear().end(),
//                       this->input_y->getDeltaClear().begin(),
//                       this->deltaClear,
//                       std::plus<>());
        this->deltaClear = matrixSubtract(this->input_x->getDeltaClear(), this->input_y->getDeltaClear());
    }
};

#endif //MALICIOUS_PPML_SUBTRACTIONGATE_H
