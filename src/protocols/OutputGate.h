#ifndef MALICIOUS_PPML_OUTPUTGATE_H
#define MALICIOUS_PPML_OUTPUTGATE_H


#include <vector>
#include <algorithm>
#include <stdexcept>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

template<typename ShrType>
class OutputGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    OutputGate(const std::shared_ptr<Gate<ShrType>> &inputX)
            : Gate<ShrType>(inputX, nullptr) {  //only one input wire
        this->dimRow = this->input_x->getDimRow();
        this->dimCol = this->input_x->getDimCol();
    }

    void doReadOfflineFromFile(std::ifstream &ifs) override {}  //No preprocessing needed

    std::vector<ClearType> getClear() const { return std::vector<ClearType>(clear.begin(), clear.end()); }

private:
    void doRunOffline() override {}

    void doRunOnline() override {
        int size = this->dimRow * this->dimCol;
        lambdaClear.resize(size);
        this->getParty()->getNetwork().send(1 - this->myId(), this->input_x->getLambdaShr());
        this->getParty()->getNetwork().rcv(1 - this->myId(), &lambdaClear, size);
        matrixAddAssign(lambdaClear, this->input_x->getLambdaShr());
        clear = matrixSubtract(this->input_x->getDeltaClear(), lambdaClear);
    }

private:
    std::vector<SemiShrType> lambdaClear;
    std::vector<SemiShrType> clear;
};

#endif //MALICIOUS_PPML_OUTPUTGATE_H
