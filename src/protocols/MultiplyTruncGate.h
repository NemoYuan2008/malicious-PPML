#ifndef MALICIOUS_PPML_MULTIPLYTRUNCGATE_H
#define MALICIOUS_PPML_MULTIPLYTRUNCGATE_H

#include <vector>
#include <execution>
#include <algorithm>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

#ifndef NDEBUG

#include <iostream>
#include "utils/ioHelper.h"

#endif

template<typename ShrType>
class MultiplyTruncGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    static const SemiShrType truncBits = 8;

    MultiplyTruncGate(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y)
            : Gate<ShrType>(input_x, input_y) {
        if (input_x->getDimCol() != input_y->getDimRow()) {
            throw std::logic_error("Dimension of the two inputs of multiplication don't match");
        }
        this->dimRow = this->input_x->getDimRow();
        this->dimMid = this->input_x->getDimCol();
        this->dimCol = this->input_y->getDimCol();
    }

    const auto &getLambdaXyShr() const { return lambda_xyShr; }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        int size = this->dimRow * this->dimCol;
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
        //TODO: do not do redundant steps
        const auto &delta_xClear = this->input_x->getDeltaClear();
        const auto &delta_yClear = this->input_y->getDeltaClear();
        const auto &lambda_xShr = this->input_x->getLambdaShr();
        const auto &lambda_yShr = this->input_y->getLambdaShr();

        //delta_zShr = lambda_xyShr + lambdaShr - lambda_xShr * delta_yClear - delta_xClear * lambda_yShr
        //             + delta_xClear * delta_yClear
        auto delta_zShr = matrixAdd(this->lambda_xyShr, this->lambdaPreTruncShr);
        matrixSubtractAssign(delta_zShr,
                             matrixMultiply(lambda_xShr, delta_yClear, this->dimRow, this->dimMid, this->dimCol));
        matrixSubtractAssign(delta_zShr,
                             matrixMultiply(delta_xClear, lambda_yShr, this->dimRow, this->dimMid, this->dimCol));
        if (this->myId() == 0) {
            matrixAddAssign(delta_zShr,
                            matrixMultiply(delta_xClear, delta_yClear, this->dimRow, this->dimMid, this->dimCol));
        }

        //TODO: for MAC, do the same above plus the following:
//        delta_zShr.mi += this->party->getPartyKey() * delta_xClear * delta_yClear;

        //TODO: only works for 2PC, extend to n-PC
        std::vector<SemiShrType> delta_z_rcv(delta_zShr.size());

        this->party->getNetwork().send(1 - this->myId(), delta_zShr);
        this->party->getNetwork().rcv(1 - this->myId(), &delta_z_rcv, delta_z_rcv.size());
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
                      [](SemiShrType &x) { x = static_cast<std::make_signed_t<ClearType>>(x) >> truncBits; });

#ifndef NDEBUG
        std::cout << "After Truncation, deltaClear:\n";
        printVector(this->deltaClear);
#endif
    }


protected:
    std::vector<SemiShrType> lambda_xyShr, lambda_xyShrMac;
    std::vector<SemiShrType> lambdaPreTruncShr, lambdaPreTruncShrMac;
    int dimMid{};
};


#endif //MALICIOUS_PPML_MULTIPLYTRUNCGATE_H
