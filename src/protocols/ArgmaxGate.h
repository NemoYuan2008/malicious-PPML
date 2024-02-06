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
        this->dimRow = 1;
        this->dimCol = input_x->getDimCol();

        this->cols = input_x->getDimRow();
        uint32_t batchsize = this->dimRow;
        uint32_t count = this->cols - 1;
        auto initmax = this->circuit.slice(input_x, 0);
        auto initmaxInd = 0;
        std::shared_ptr<Gate<ShrType>> max, maxInd;
//        max = initmax;
        for (int i = 0; i < count; ++i) {
            auto next = this->circuit.slice(input_x,i+1);
            int nextInd = i+1;
            // compare max , next
            if (i == 0) {
                auto sub_ = this->circuit.subtract(initmax, next); // subtract: max - next
                auto b_ = this->circuit.gtz(sub_); //: max-next > 0
                auto product = this->circuit.elementMultiply(b_,sub_);
                auto productInd = this->circuit.multiplyByConstant(b_, static_cast<ClearType>(initmaxInd - nextInd));
                max = this->circuit.add(product, next); //max = b(max-next) + next
                maxInd = this->circuit.addConstant(productInd, nextInd);
            } else {
                auto sub_ = this->circuit.subtract(max, next); // subtract: max - next
                auto sub_Ind = this->circuit.addConstant(maxInd, static_cast<ClearType>(-nextInd)); // subtract
                auto b_ = this->circuit.gtz(sub_); //: max-next > 0
                auto product = this->circuit.elementMultiply(b_, sub_);
                auto productInd = this->circuit.elementMultiply(b_, sub_Ind);
                max = this->circuit.add(product, next); //max = b(max-next) + next
                maxInd = this->circuit.addConstant(productInd, nextInd);
            }
        }

        circuit.addEndpoint(maxInd);
    }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        this->circuit.readOfflineFromFile();
        this->lambdaShr = this->circuit.getEndpoints()[0]->getLambdaShr();
    }


    void doRunOffline() override {

    }

    void doRunOnline() override {
        this->circuit.runOnline();
        this->deltaClear = this->circuit.getEndpoints()[0]->getDeltaClear();
    }


    Circuit<ShrType> circuit;
    size_t cols;
};

#endif //MALICIOUS_PPML_ARGMAXGATE_H