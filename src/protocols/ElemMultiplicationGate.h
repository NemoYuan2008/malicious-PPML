#ifndef MALICIOUS_PPML_ELEMMULTIPLICATIONGATE_H
#define MALICIOUS_PPML_ELEMMULTIPLICATIONGATE_H


#include <vector>
#include <algorithm>
#include <execution>
#include <thread>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

#ifndef NDEBUG

#include "utils/ioHelper.h"

#endif


template<typename ShrType>
class ElemMultiplicationGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    ElemMultiplicationGate(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y)
            : Gate<ShrType>(input_x, input_y) {
        if (input_x->getDimRow() != input_y->getDimRow() || input_x->getDimCol() != input_y->getDimCol()) {
            throw std::logic_error("Dimensions of the two inputs of element-wise multiplication don't match");
        }
        this->dimRow = input_x->getDimRow();
        this->dimCol = input_x->getDimCol();
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
#ifndef NDEBUG
        std::cout << "\nElemMultiply Online\n";
        std::cout << "LambdaShr:";
        printVector(this->lambdaShr);
        std::cout << "lambda_xyShr: ";
        printVector(this->lambda_xyShr);
        std::cout << std::endl;
#endif

        const auto &delta_xClear = this->input_x->getDeltaClear();
        const auto &delta_yClear = this->input_y->getDeltaClear();
        const auto &lambda_xShr = this->input_x->getLambdaShr();
        const auto &lambda_yShr = this->input_y->getLambdaShr();

        //Compute delta_zShr = lambda_xyShr + lambdaShr - lambda_xShr * delta_yClear - delta_xClear * lambda_yShr
        //                     + delta_xClear * delta_yClear
        std::vector<SemiShrType> temp(delta_xClear.size());

        //delta_zShr = lambda_xyShr + lambdaShr
        auto delta_zShr = this->lambda_xyShr;
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), this->lambdaShr.begin(), delta_zShr.begin(),
                       std::plus<>());

        //delta_zShr -= lambda_xShr * delta_yClear
        std::transform(std::execution::par_unseq,
                       lambda_xShr.begin(), lambda_xShr.end(), delta_yClear.begin(), temp.begin(),
                       std::multiplies<>());
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), temp.begin(), delta_zShr.begin(), std::minus<>());

        //delta_zShr -= delta_xClear * lambda_yShr
        std::transform(std::execution::par_unseq,
                       delta_xClear.begin(), delta_xClear.end(), lambda_yShr.begin(), temp.begin(),
                       std::multiplies<>());
        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), temp.begin(), delta_zShr.begin(), std::minus<>());

        if (this->myId() == 0) {
            //delta_zShr += delta_xClear * delta_yClear
            std::transform(std::execution::par_unseq,
                           delta_xClear.begin(), delta_xClear.end(), delta_yClear.begin(), temp.begin(),
                           std::multiplies<>());
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
//        std::vector<SemiShrType> delta_z_rcv(delta_zShr.size());
#ifndef NDEBUG
        std::cout << "delta_zShr:";
        printVector(delta_zShr);
        std::cout << "ElemMultiply send delta_zShr, size: " << delta_zShr.size() << "\n";
#endif

        std::thread t1([this, &delta_zShr]() {
            this->party->getNetwork().send(1 - this->myId(), delta_zShr);
        });

        this->deltaClear.resize(delta_zShr.size());
        std::thread t2([this]() {
            this->party->getNetwork().rcv(1 - this->myId(), &this->deltaClear, this->deltaClear.size());
        });
        t1.join();
        t2.join();

//        this->deltaClear = matrixAdd(delta_zShr, delta_z_rcv);

        std::transform(std::execution::par_unseq,
                       delta_zShr.begin(), delta_zShr.end(), this->deltaClear.begin(), this->deltaClear.begin(),
                       std::plus<>());
    }


protected:
    std::vector<SemiShrType> lambda_xyShr, lambda_xyShrMac;
};


#endif //MALICIOUS_PPML_ELEMMULTIPLICATIONGATE_H
