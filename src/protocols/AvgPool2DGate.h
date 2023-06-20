#ifndef MALICIOUS_PPML_AVGPOOL2DGATE_H
#define MALICIOUS_PPML_AVGPOOL2DGATE_H


#include <vector>
#include <thread>
#include <algorithm>
#include <execution>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#include "utils/tensor.h"


template<typename ShrType>
class AvgPool2DGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    AvgPool2DGate(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y,
                  const MaxPoolOp &op)
            : Gate<ShrType>(input_x, input_y), maxPoolOp(op) {
        //TODO: dimRow or dimCol?
        this->dimRow = op.compute_output_size();
    }

//    const auto &getLambdaXyShr() const { return lambda_xyShr; }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        //TODO: size is different now
        int size = this->convOp.compute_output_size();
        this->lambdaShr.resize(size);
        this->lambdaShrMac.resize(size);
        this->lambda_xyShr.resize(size);
        this->lambda_xyShrMac.resize(size);

        for (int i = 0; i < size; ++i) {
            ifs >> this->lambdaShr[i] >> this->lambdaShrMac[i] >> this->lambda_xyShr[i] >> this->lambda_xyShrMac[i];
        }
    }

    void doRunOffline() override {
        // generate random lambdaShr
        // compute triple
    }

    void doRunOnline() override {
        const auto &delta_xClear = this->input_x->getDeltaClear();
        const auto &delta_yClear = this->input_y->getDeltaClear();
        const auto &lambda_xShr = this->input_x->getLambdaShr();
        const auto &lambda_yShr = this->input_y->getLambdaShr();

        auto outputSize = maxPoolOp.compute_output_size();

//        auto delta
    }


protected:
    MaxPoolOp maxPoolOp;
//    std::vector<SemiShrType> lambda_xyShr, lambda_xyShrMac;
};


#endif //MALICIOUS_PPML_AVGPOOL2DGATE_H
