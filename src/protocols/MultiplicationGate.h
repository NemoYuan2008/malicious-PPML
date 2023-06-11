#ifndef MALICIOUS_PPML_MULTIPLICATIONGATE_H
#define MALICIOUS_PPML_MULTIPLICATIONGATE_H


#include <vector>
#include "protocols/Gate.h"

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
        this->dimCol = this->input_y->getDimCol();
    }

    const auto &getLambdaXyShr() const { return lambda_xyShr; }

//    void setLambdaXyShr(const ShrType &lambdaXyShr) { lambda_xyShr = lambdaXyShr; }


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
        //TODO: do not do redundant steps
        //TODO: MAC
        auto delta_xClear = this->input_x->getDeltaClear()[0];
        auto delta_yClear = this->input_y->getDeltaClear()[0];
        auto lambda_xShr = this->input_x->getLambdaShr()[0];
        auto lambda_yShr = this->input_y->getLambdaShr()[0];

        auto delta_zShr =
                lambda_xyShr[0] + this->lambdaShr[0] - lambda_xShr * delta_yClear - lambda_yShr * delta_xClear;

        //Add constant term delta_xClear * delta_yClear
        if (this->myId() == 0) {
            delta_zShr += delta_xClear * delta_yClear;
        }
//        delta_zShr.mi += this->party->getPartyKey() * delta_xClear * delta_yClear;

        //TODO: only works for 2PC, extend to n-PC
        decltype(delta_zShr) delta_z_rcv = 0;

        this->party->getNetwork().send(1 - this->myId(), delta_zShr);
        this->party->getNetwork().rcv(1 - this->myId(), &delta_z_rcv);

        this->deltaClear.resize(1);
        this->deltaClear[0] = static_cast<ClearType>(delta_zShr + delta_z_rcv);
    }


protected:
    std::vector<SemiShrType> lambda_xyShr, lambda_xyShrMac;
};

#endif //MALICIOUS_PPML_MULTIPLICATIONGATE_H
