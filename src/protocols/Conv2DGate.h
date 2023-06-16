#ifndef MALICIOUS_PPML_CONV2DGATE_H
#define MALICIOUS_PPML_CONV2DGATE_H


#include <vector>
#include <thread>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#include "utils/tensor.h"


template<typename ShrType>
class Conv2DGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    Conv2DGate(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y,
               const Conv2DOp &op)
            : Gate<ShrType>(input_x, input_y), convOp(op) {
        //Don't use dimRow and dimCol in ConvGate
    }

    const auto &getLambdaXyShr() const { return lambda_xyShr; }

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
        //TODO: do not do redundant steps
        const auto &delta_xClear = this->input_x->getDeltaClear();
        const auto &delta_yClear = this->input_y->getDeltaClear();
        const auto &lambda_xShr = this->input_x->getLambdaShr();
        const auto &lambda_yShr = this->input_y->getLambdaShr();

        auto outputSize = convOp.compute_output_size();


        //delta_zShr = lambda_xyShr + lambdaShr - lambda_xShr * delta_yClear - delta_xClear * lambda_yShr
        //             + delta_xClear * delta_yClear
        auto delta_zShr = matrixAdd(this->lambda_xyShr, this->lambdaShr);
        matrixSubtractAssign(delta_zShr,
                             convolution(lambda_xShr, delta_yClear, this->convOp));
        matrixSubtractAssign(delta_zShr,
                             convolution(delta_xClear, lambda_yShr, this->convOp));
        if (this->myId() == 0) {
            matrixAddAssign(delta_zShr,
                            convolution(delta_xClear, delta_yClear, this->convOp));
        }

        //TODO: for MAC, do the same above plus the following:
//        delta_zShr.mi += this->party->getPartyKey() * delta_xClear * delta_yClear;

        //TODO: only works for 2PC, extend to n-PC
        std::vector<SemiShrType> delta_z_rcv(delta_zShr.size());

        std::thread t1([this, &delta_zShr]() {
            this->party->getNetwork().send(1 - this->myId(), delta_zShr);
        });
        std::thread t2([this, &delta_z_rcv]() {
            this->party->getNetwork().rcv(1 - this->myId(), &delta_z_rcv, delta_z_rcv.size());
        });
        t1.join();
        t2.join();

        this->deltaClear = matrixAdd(delta_zShr, delta_z_rcv);
    }


protected:
    Conv2DOp convOp;
    std::vector<SemiShrType> lambda_xyShr, lambda_xyShrMac;
};


#endif //MALICIOUS_PPML_CONV2DGATE_H
