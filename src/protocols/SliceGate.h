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
    }

//    void setLambdaShr(const std::vector<SemiShrType> &value) { this->lambdaShr = value; }

//    void setDeltaClear(const std::vector<SemiShrType> &value) { this->deltaClear = value; }

//    void setDimRow(int row) { this->dimRow = row; }

//    void setDimCol(int col) { this->dimCol = col; }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
//        this->lambdaShr = this->input_x->getLambdaShr()[?];
//        this->lambdaShrMac = ??;
    }

    void doRunOffline() override {}

    void doRunOnline() override {
//        this->deltaClear = this->input_x->getDeltaClear()[?];
    }

    size_t index;
};

#endif //MALICIOUS_PPML_SLICEGATE_H
