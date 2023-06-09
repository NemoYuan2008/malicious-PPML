#ifndef MALICIOUS_PPML_MULTIPLICATIONGATE_H
#define MALICIOUS_PPML_MULTIPLICATIONGATE_H

#include "protocols/Gate.h"

template<typename ShrType>
class MultiplicationGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    using Gate<ShrType>::Gate;

    const ShrType &getLambdaXyShr() const { return lambda_xyShr; }

    void setLambdaXyShr(const ShrType &lambdaXyShr) { lambda_xyShr = lambdaXyShr; }


protected:
    SemiShrType lambda_xyShr, lambda_xyShrMac;       //TODO: MAC


private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        this->lambdaShr.resize(1);
        this->lambdaShrMac.resize(1);
        this->deltaClear.resize(1); //should be done in read*, not do*

        ifs >> this->lambdaShr[0] >> this->lambdaShrMac[0] >> this->lambda_xyShr >> this->lambda_xyShrMac;
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

        auto delta_zShr = lambda_xyShr + this->lambdaShr[0] - lambda_xShr * delta_yClear - lambda_yShr * delta_xClear;

        //Add constant term delta_xClear * delta_yClear
        if (this->myId() == 0) {
            delta_zShr += delta_xClear * delta_yClear;
        }
//        delta_zShr.mi += this->party->getPartyKey() * delta_xClear * delta_yClear;

        //TODO: only works for 2PC, extend to n-PC
        decltype(delta_zShr) delta_z_rcv = 0;

        this->party->getNetwork().send(1 - this->myId(), delta_zShr);
        this->party->getNetwork().rcv(1 - this->myId(), &delta_z_rcv);

        this->deltaClear[0] = static_cast<ClearType>(delta_zShr + delta_z_rcv);
    }
};

#endif //MALICIOUS_PPML_MULTIPLICATIONGATE_H
