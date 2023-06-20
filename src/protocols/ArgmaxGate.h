#ifndef MALICIOUS_PPML_ARGMAXGATE_H
#define MALICIOUS_PPML_ARGMAXGATE_H

#include "protocols/Gate.h"
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#include "protocols/Circuit.h"
#include "SliceGate.h"


template<typename ShrType>
class Circuit;

template<typename ShrType>
class ArgmaxGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    using Gate<ShrType>::Gate;

    explicit ArgmaxGate(const std::shared_ptr<Gate<ShrType>> &input_x)
            : Gate<ShrType>(input_x, nullptr), circuit(input_x->getParty()) {
//        if (input_x->getDimRow() != input_y->getDimRow() || input_x->getDimCol() != input_y->getDimCol()) {
//            throw std::logic_error("Dimension of the two inputs of addition don't match");
//        }
        //TODO: ??
        this->dimRow = this->input_x->getDimRow();
        this->dimCol = this->input_x->getDimCol();
        auto Delta = input_x->getDeltaClear();
        auto Lambda = input_x->getLambdaShr();
        uint32_t count = Delta.size() - 1;
        std::vector<SemiShrType> indexShr(count + 1, 0);
        for (int i = 0; i < indexShr.size(); ++i) {
            if (this->myId() == 0) indexShr[i] = i;
        }
        //set dummy input gate
        auto initmax = this->circuit.slice(input_x, 0);
        auto initmaxInd = this->circuit.dummyInput(input_x, 0); //set dummy input gate
        std::shared_ptr<Gate<ShrType>> max, maxInd;
        for (int i = 0; i < count; ++i) {
            //compare ret and x[i+1]
            //set dummy input gate
            auto next = this->circuit.slice(input_x, i);
            auto nextInd = this->circuit.dummyInput(input_x, i); //set dummy input gate
            // compare max , next
            if (i==0){
                auto sub_ = this->circuit.subtract(initmax, next); // subtract: max - next
                auto b_ = this->circuit.gtz(sub_); //: max-next > 0
                auto product = this->circuit.elementMultiply(b_, sub_);
                auto productInd = this->circuit.multiplyByConstant(b_, -1);
                max = this->circuit.add(product, next); //max = b(max-next) + next
                maxInd = this->circuit.add(productInd, nextInd);
            }else{
                auto sub_ = this->circuit.subtract(max, next); // subtract: max - next
                auto sub_Ind = this->circuit.subtract(maxInd, nextInd); // subtract
                auto b_ = this->circuit.gtz(sub_); //: max-next > 0
                auto product = this->circuit.elementMultiply(b_, sub_);
                auto productInd = this->circuit.elementMultiply(b_, sub_Ind);
                max = this->circuit.add(product, next); //max = b(max-next) + next
                maxInd = this->circuit.add(productInd, nextInd);
            }
        }
        circuit.addEndpoint(maxInd);
    }

private:
    void doRunOffline() override {
        this->circuit.readOfflineFromFile();
        this->lambdaShr = this->circuit.getEndpoints()[0]->getLambdaShr();
    }

    void doRunOnline() override {
        this->circuit.runOnline();
        this->deltaClear = this->circuit.getEndpoints()[0]->getDeltaClear();
    }


    Circuit<ShrType> circuit;
};

#endif //MALICIOUS_PPML_ARGMAXGATE_H
