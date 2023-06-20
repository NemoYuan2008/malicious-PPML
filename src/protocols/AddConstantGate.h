#ifndef MALICIOUS_PPML_ADDCONSTANTGATE_H
#define MALICIOUS_PPML_ADDCONSTANTGATE_H


#include <stdexcept>
#include <algorithm>
#include <execution>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

template<typename ShrType>
class AddConstantGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    using Gate<ShrType>::Gate;

    AddConstantGate(const std::shared_ptr<Gate<ShrType>> &input_x, ClearType c)
            : Gate<ShrType>(input_x, nullptr), c(c) {
        this->dimRow = this->input_x->getDimRow();
        this->dimCol = this->input_x->getDimCol();
    }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        this->lambdaShr = this->input_x->getLambdaShr();
        this->lambdaShrMac = this->input_x->getLambdaShrMac();
    }  //No preprocessing

    void doRunOffline() override {}

    void doRunOnline() override {
        this->deltaClear.resize(this->input_x->getDeltaClear().size());
        std::transform(std::execution::par_unseq,
                       this->input_x->getDeltaClear().begin(), this->input_x->getDeltaClear().end(),
                       this->deltaClear.begin(), [this](SemiShrType x) { return x + c; });
    }

    ClearType c;
};


#endif //MALICIOUS_PPML_ADDCONSTANTGATE_H
