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
        this->dimRow = input_x->getDimRow();
        this->dimCol = 1;
        this->cols = input_x->getDimCol();
    }

//    void setLambdaShr(const std::vector<SemiShrType> &value) { this->lambdaShr = value; }

//    void setDeltaClear(const std::vector<SemiShrType> &value) { this->deltaClear = value; }

//    void setDimRow(int row) { this->dimRow = row; }

//    void setDimCol(int col) { this->dimCol = col; }

private:

    void doReadOfflineFromFile(std::ifstream &ifs) override {
        const auto &lambda = this->input_x.getLambdaShr();
        const auto &lambdaMac = this->input_x.getLambdaShrMac();
        for (int i = 0; i < this->dimRow; ++i) {
            this->lambdaShr.push_back(lambda[i*this->cols + index]);
            this->lambdaShrMac.push_back(lambdaMac[i*this->cols + index]);
        }

    }

    void doRunOffline() override {}

    void doRunOnline() override {
//        this->deltaClear = this->input_x->getDeltaClear()[?];
        const auto &delta = this->input_x.getDeltaClear();
        for (int i = 0; i < this->dimRow; ++i) {
            this->lambdaShr.push_back(delta[i*this->cols + index]);
        }
    }
    size_t cols;
    size_t index;
};

#endif //MALICIOUS_PPML_SLICEGATE_H
