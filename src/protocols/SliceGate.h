#ifndef MALICIOUS_PPML_SLICEGATE_H
#define MALICIOUS_PPML_SLICEGATE_H


#include <stdexcept>
#include <algorithm>
#include <execution>
#include "protocols/Gate.h"
#include "utils/Party.h"
#include "utils/linear_algebra.h"

template<typename ShrType>
class SliceGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    using Gate<ShrType>::Gate;

    SliceGate(const std::shared_ptr<Gate<ShrType>> &input_x, size_t index)
            : Gate<ShrType>(input_x, nullptr), index(index) {
        this->dimRow = 1;
        this->dimCol = input_x->getDimCol(); // TODO: dimension check
        //1 * dimCol, slice  1*dimCol elements to compare, the index-th col of input_x
        this->cols = this->input_x->getDimRow();
    }

private:

    void doReadOfflineFromFile(std::ifstream &ifs) override {
        const auto &lambda = this->input_x->getLambdaShr();
        const auto &lambdaMac = this->input_x->getLambdaShrMac();
        for (int i = 0; i < this->dimRow; ++i) {
            this->lambdaShr.push_back(lambda[i*this->cols + index]);
            this->lambdaShrMac.push_back(lambdaMac[i*this->cols + index]);
        }

    }

    void doRunOffline() override {

    }

    void doRunOnline() override {
//        this->deltaClear = this->input_x->getDeltaClear()[?];
        const auto &delta = this->input_x->getDeltaClear();
        for (int i = 0; i < this->dimCol; ++i) {
//            this->lambdaShr.push_back(delta[i*this->cols + index]);
            this->deltaClear.push_back(delta[i*this->cols + index]);
        }
    }
    size_t cols;
    size_t index;
};

#endif //MALICIOUS_PPML_SLICEGATE_H
