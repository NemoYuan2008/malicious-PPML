#ifndef MALICIOUS_PPML_CONV2DTRUNCGATE_H
#define MALICIOUS_PPML_CONV2DTRUNCGATE_H


#include <vector>
#include <thread>
#include <algorithm>
#include <execution>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#include "utils/tensor.h"
#include "utils/fixedPoint.h"


template<typename ShrType>
class Conv2DTruncGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    static const SemiShrType fractionBits = FixedPoint::fractionBits;

    Conv2DTruncGate(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y,
               const Conv2DOp &op)
            : Gate<ShrType>(input_x, input_y), convOp(op) {
        //TODO: dimRow or dimCol?
        this->dimRow = op.compute_output_size();
    }

    const auto &getLambdaXyShr() const { return lambda_xyShr; }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        int size = this->convOp.compute_output_size();
        this->lambdaShr.resize(size);
        this->lambdaShrMac.resize(size);
        this->lambda_xyShr.resize(size);
        this->lambda_xyShrMac.resize(size);
        this->lambdaPreTruncShr.resize(size);
        this->lambdaPreTruncShrMac.resize(size);

        for (int i = 0; i < size; ++i) {
            ifs >> this->lambdaShr[i] >> this->lambdaShrMac[i]
                >> this->lambda_xyShr[i] >> this->lambda_xyShrMac[i]
                >> this->lambdaPreTruncShr[i] >> this->lambdaPreTruncShrMac[i];
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

        auto outputSize = convOp.compute_output_size();


        //Compute delta_zShr = lambda_xyShr + lambdaShr - lambda_xShr * delta_yClear - delta_xClear * lambda_yShr
        //                     + delta_xClear * delta_yClear

        //delta_zShr = lambda_xyShr + lambdaShr
        auto delta_zShr = this->lambda_xyShr;
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), this->lambdaPreTruncShr.begin(), delta_zShr.begin(),
                       std::plus<>());

        //delta_zShr = lambda_xyShr + lambdaShr
        auto temp = convolution(lambda_xShr, delta_yClear, this->convOp);
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), temp.begin(), delta_zShr.begin(), std::minus<>());

        //delta_zShr -= lambda_xShr * delta_yClear
        temp = convolution(delta_xClear, lambda_yShr, this->convOp);
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), temp.begin(), delta_zShr.begin(), std::minus<>());

        //delta_zShr -= delta_xClear * lambda_yShr
        if (this->myId() == 0) {
            temp = convolution(delta_xClear, delta_yClear, this->convOp);
            std::transform(std::execution::par_unseq,
                           delta_zShr.begin(), delta_zShr.end(), temp.begin(), delta_zShr.begin(), std::plus<>());
        }

//        auto delta_zShr = matrixAdd(this->lambda_xyShr, this->lambdaShr);
//        matrixSubtractAssign(delta_zShr,
//                             convolution(lambda_xShr, delta_yClear, this->convOp));
//        matrixSubtractAssign(delta_zShr,
//                             convolution(delta_xClear, lambda_yShr, this->convOp));
//        if (this->myId() == 0) {
//            matrixAddAssign(delta_zShr,
//                            convolution(delta_xClear, delta_yClear, this->convOp));
//        }

        //TODO: for MAC, do the same above plus the following:
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

#ifndef NDEBUG
        std::cout << "lambdaShr:\n";
        printVector(this->lambdaShr);
        std::cout << "lambdaPreTruncShr:\n";
        printVector(this->lambdaPreTruncShr);
        std::cout << "Before Truncation, deltaClear:\n";
        printVector(this->deltaClear);
#endif

        //upper bits of x is of no use, should be eliminated before shifting right
        std::for_each(std::execution::par_unseq, this->deltaClear.begin(), this->deltaClear.end(),
                      [](SemiShrType &x) { x = static_cast<std::make_signed_t<ClearType>>(x) >> fractionBits; });

#ifndef NDEBUG
        std::cout << "After Truncation, deltaClear:\n";
        printVector(this->deltaClear);
#endif
    }


protected:
    Conv2DOp convOp;
    std::vector<SemiShrType> lambda_xyShr, lambda_xyShrMac;
    std::vector<SemiShrType> lambdaPreTruncShr, lambdaPreTruncShrMac;
};


#endif //MALICIOUS_PPML_CONV2DTRUNCGATE_H
