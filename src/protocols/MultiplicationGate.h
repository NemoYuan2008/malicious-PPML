#ifndef MALICIOUS_PPML_MULTIPLICATIONGATE_H
#define MALICIOUS_PPML_MULTIPLICATIONGATE_H


#include <vector>
#include <thread>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

template<typename ShrType>
class MultiplicationGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    MultiplicationGate(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y)
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

        //Compute delta_zShr = lambda_xyShr + lambdaShr - lambda_xShr * delta_yClear - delta_xClear * lambda_yShr
        //                     + delta_xClear * delta_yClear

        //delta_zShr = lambda_xyShr + lambdaShr
        auto delta_zShr = this->lambda_xyShr;
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), this->lambdaShr.begin(), delta_zShr.begin(),
                       std::plus<>());

        //delta_zShr -= lambda_xShr * delta_yClear
        auto temp = matrixMultiply(lambda_xShr, delta_yClear, this->dimRow, this->dimMid, this->dimCol);
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), temp.begin(), delta_zShr.begin(), std::minus<>());

        //delta_zShr -= delta_xClear * lambda_yShr
        temp = matrixMultiply(delta_xClear, lambda_yShr, this->dimRow, this->dimMid, this->dimCol);
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), temp.begin(), delta_zShr.begin(), std::minus<>());

        if (this->myId() == 0) {
            //delta_zShr += delta_xClear * delta_yClear
            temp = matrixMultiply(delta_xClear, delta_yClear, this->dimRow, this->dimMid, this->dimCol);
            std::transform(std::execution::par_unseq,
                           delta_zShr.begin(), delta_zShr.end(), temp.begin(), delta_zShr.begin(), std::plus<>());
        }

//        auto delta_zShr = matrixAdd(this->lambda_xyShr, this->lambdaShr);
//        matrixSubtractAssign(delta_zShr,
//                             matrixMultiply(lambda_xShr, delta_yClear, this->dimRow, this->dimMid, this->dimCol));
//        matrixSubtractAssign(delta_zShr,
//                             matrixMultiply(delta_xClear, lambda_yShr, this->dimRow, this->dimMid, this->dimCol));
//        if (this->myId() == 0) {
//            matrixAddAssign(delta_zShr,
//                            matrixMultiply(delta_xClear, delta_yClear, this->dimRow, this->dimMid, this->dimCol));
//        }

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
    std::vector<SemiShrType> lambda_xyShr, lambda_xyShrMac;
    int dimMid{};
};

#endif //MALICIOUS_PPML_MULTIPLICATIONGATE_H
