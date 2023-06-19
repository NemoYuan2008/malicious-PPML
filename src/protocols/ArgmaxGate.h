//
// Created by ysx on 6/19/23.
//

#ifndef MALICIOUS_PPML_ARGMAXGATE_H
#define MALICIOUS_PPML_ARGMAXGATE_H
#include "protocols/Gate.h"
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#include "protocols/Circuit.h"


template<typename ShrType>
class Circuit;

template<typename ShrType>
class ArgmxGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    using Gate<ShrType>::Gate;

    ArgmxGate(const std::shared_ptr<Gate<ShrType>> &input_x)
            : Gate<ShrType>(input_x, nullptr), circuit(input_x->getParty()) {
//        if (input_x->getDimRow() != input_y->getDimRow() || input_x->getDimCol() != input_y->getDimCol()) {
//            throw std::logic_error("Dimension of the two inputs of addition don't match");
//        }
        //TODO: ??
        this->dimRow = this->input_x->getDimRow();
        this->dimCol = this->input_x->getDimCol();
        auto Delta = input_x->getDeltaClear();
        uint32_t count = Delta.size() - 1;
        std::vector<SemiShrType> indexShr(count+1,0);
        for (int i = 0; i < indexShr.size(); ++i) {
            if(this->myId()==0) indexShr[i] = i;
        }
        auto max = Delta[0]; //set max <-- delta[0]
        for (int i = 0; i < count; ++i) {
            //compare ret and x[i+1]
            auto next = Delta[i+1];
            // compare max , next

        }
        auto b = this->circuit.gtz(input_x);
        auto z = this->circuit.multiply(input_x, b);
    }

private:
    void doRunOffline() override {
        this->lambdaShr = matrixAdd(this->input_x->getLambdaShr(), this->input_y->getLambdaShr());
        this->lambdaShrMac = matrixAdd(this->input_x->getLambdaShrMac(), this->input_y->getLambdaShrMac());
    }

    void doRunOnline() override {
        this->deltaClear = matrixAdd(this->input_x->getDeltaClear(), this->input_y->getDeltaClear());
    }



    Circuit<ShrType> circuit;
};

#endif //MALICIOUS_PPML_ARGMAXGATE_H
