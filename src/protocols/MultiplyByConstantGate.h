#ifndef MALICIOUS_PPML_MULTIPLYBYCONSTANTGATE_H
#define MALICIOUS_PPML_MULTIPLYBYCONSTANTGATE_H


#include <stdexcept>
#include <algorithm>
#include <execution>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

template<typename ShrType>
class MultiplyByConstantGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    using Gate<ShrType>::Gate;

    MultiplyByConstantGate(const std::shared_ptr<Gate<ShrType>> &input_x, ClearType c)
            : Gate<ShrType>(input_x, nullptr), c(c) {
        this->dimRow = this->input_x->getDimRow();
        this->dimCol = this->input_x->getDimCol();
    }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        doRunOffline(); //No need to read from file, do local computation
    }

    void doRunOffline() override {
        this->lambdaShr.resize(this->dimCol * this->dimRow);
        this->lambdaShrMac.resize(this->dimCol * this->dimRow);
        std::transform(std::execution::par_unseq,
                       this->input_x->getLambdaShr().begin(), this->input_x->getLambdaShr().end(),
                       this->lambdaShr.begin(), [this](SemiShrType x) {return x * c;});
        std::transform(std::execution::par_unseq,
                       this->input_x->getLambdaShrMac().begin(), this->input_x->getLambdaShrMac().end(),
                       this->lambdaShrMac.begin(), [this](SemiShrType x) {return x * c;});
    }

    void doRunOnline() override {
        this->deltaClear.resize(this->input_x->getDeltaClear().size());
        std::transform(std::execution::par_unseq,
                       this->input_x->getDeltaClear().begin(), this->input_x->getDeltaClear().end(),
                       this->deltaClear.begin(), [this](SemiShrType x) {return x * c;});
    }

    SemiShrType c;
};


#endif //MALICIOUS_PPML_MULTIPLYBYCONSTANTGATE_H
