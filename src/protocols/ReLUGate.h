#ifndef MALICIOUS_PPML_RELUGATE_H
#define MALICIOUS_PPML_RELUGATE_H


#include "protocols/Gate.h"
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#include "protocols/Circuit.h"


template<typename ShrType>
class Circuit;

template<typename ShrType>
class ReLUGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    using Gate<ShrType>::Gate;

    explicit ReLUGate(const std::shared_ptr<Gate<ShrType>> &input_x)
            : Gate<ShrType>(input_x, nullptr), circuit(input_x->getParty()) {
//        if (input_x->getDimRow() != input_y->getDimRow() || input_x->getDimCol() != input_y->getDimCol()) {
//            throw std::logic_error("Dimension of the two inputs of addition don't match");
//        }
        //TODO: ??
        this->dimRow = this->input_x->getDimRow();
        this->dimCol = this->input_x->getDimCol();

        auto b = this->circuit.gtz(this->input_x);
        auto z = this->circuit.elementMultiply(this->input_x, b);
        circuit.addEndpoint(z);
    }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        this->circuit.readOfflineFromFile();
    }

    void doRunOffline() override { }

    void doRunOnline() override {
        this->circuit.runOnline();
    }

    Circuit<ShrType> circuit;
};


#endif //MALICIOUS_PPML_RELUGATE_H
