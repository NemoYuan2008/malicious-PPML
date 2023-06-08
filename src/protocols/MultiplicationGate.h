#ifndef MALICIOUS_PPML_MULTIPLICATIONGATE_H
#define MALICIOUS_PPML_MULTIPLICATIONGATE_H

#include "protocols/Gate.h"

template<typename ShrType>
class MultiplicationGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;

    using Gate<ShrType>::Gate;

    const ShrType &getLambdaXyShr() const { return lambda_xyShr; }

    void setLambdaXyShr(const ShrType &lambdaXyShr) { lambda_xyShr = lambdaXyShr; }


protected:
    ShrType lambda_xyShr;


private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        ifs >> this->lambdaShr >> this->lambda_xyShr;
    }

    void doRunOffline() override {
        // generate random lambdaShr
        // compute triple
    }

    void doRunOnline() override {
        //TODO: do not do redundant steps
        auto delta_xClear = this->input_x->getDeltaClear();
        auto delta_yClear = this->input_y->getDeltaClear();
        const auto &lambda_xShr = this->input_x->getLambdaShr();
        const auto &lambda_yShr = this->input_y->getLambdaShr();

        auto delta_zShr = lambda_xyShr + this->lambdaShr - lambda_xShr * delta_yClear - lambda_yShr * delta_xClear;

        //Add constant term delta_xClear * delta_yClear
        if (this->myId() == 0) {
            delta_zShr.xi += delta_xClear * delta_yClear;
        }
        delta_zShr.mi += this->party->getPartyKey() * delta_xClear * delta_yClear;

        //TODO: only works for 2PC, extend to n-PC
        decltype(delta_zShr.xi) delta_z_rcv = 0;

        this->party->getNetwork().send(1 - this->myId(), delta_zShr.xi);
        this->party->getNetwork().rcv(1 - this->myId(), &delta_z_rcv);

        this->deltaClear = static_cast<ClearType>(delta_zShr.xi + delta_z_rcv);
    }
};

#endif //MALICIOUS_PPML_MULTIPLICATIONGATE_H
