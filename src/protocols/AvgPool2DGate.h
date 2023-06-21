#ifndef MALICIOUS_PPML_AVGPOOL2DGATE_H
#define MALICIOUS_PPML_AVGPOOL2DGATE_H


#include <vector>
#include <thread>
#include <algorithm>
#include <execution>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#include "utils/tensor.h"
#include "utils/fixedPoint.h"


template<typename ShrType>
class AvgPool2DGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    static const SemiShrType fractionBits = FixedPoint::fractionBits;

    AvgPool2DGate(const std::shared_ptr<Gate<ShrType>> &input_x,
                  const MaxPoolOp &op)
            : Gate<ShrType>(input_x, nullptr),
              maxPoolOp(op),
              factor(double2fix<ClearType>(1.0 / op.compute_kernel_size())) {
        //TODO: dimRow or dimCol?
        this->dimRow = maxPoolOp.compute_output_size();
    }

//    const auto &getLambdaXyShr() const { return lambda_xyShr; }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        //TODO: size is different now
        int size = this->convOp.compute_output_size();
        this->lambdaShr.resize(size);
        this->lambdaShrMac.resize(size);
        this->lambdaPreTruncShr.resize(size);
        this->lambdaPreTruncShrMac.resize(size);

        for (int i = 0; i < size; ++i) {
            ifs >> this->lambdaShr[i] >> this->lambdaShrMac[i]
                >> this->lambdaPreTruncShr[i] >> this->lambdaPreTruncShrMac[i];;
        }
    }

    void doRunOffline() override {}

    void doRunOnline() override {
        const auto &delta_xClear = this->input_x->getDeltaClear();
        const auto &lambda_xShr = this->input_x->getLambdaShr();

        //[x] = Delta_x - [lambda_x]
        auto xShr = matrixSubtract(delta_xClear - lambda_xShr);

        auto delta_zShr = sumPool(xShr, maxPoolOp);
        std::for_each(std::execution::par_unseq,
                      delta_zShr.begin(), delta_zShr.end(), [this](auto &x) { x *= factor; });

        //truncation (need communication)
        matrixAddAssign(delta_zShr, lambdaPreTruncShr);

        std::vector<SemiShrType> delta_zRcv(this->deltaClear.size());
        std::thread t1([this, &delta_zShr]() {
            this->party->getNetwork().send(1 - this->myId(), delta_zShr);
        });
        std::thread t2([this, &delta_zRcv]() {
            this->party->getNetwork().rcv(1 - this->myId(), &delta_zRcv, delta_zRcv.size());
        });
        t1.join();
        t2.join();

        this->deltaClear = matrixAdd(delta_zShr, delta_zRcv);
        std::for_each(std::execution::par_unseq, this->deltaClear.begin(), this->deltaClear.end(),
                      [](SemiShrType &x) { x = static_cast<std::make_signed_t<ClearType>>(x) >> fractionBits; });

    }


protected:
    MaxPoolOp maxPoolOp;
    ClearType factor;   //equals 1/kernel_size
    std::vector<SemiShrType> lambdaPreTruncShr, lambdaPreTruncShrMac;
};


#endif //MALICIOUS_PPML_AVGPOOL2DGATE_H
