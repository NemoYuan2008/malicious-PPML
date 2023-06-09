#ifndef MALICIOUS_PPML_ADDITIONGATE_H
#define MALICIOUS_PPML_ADDITIONGATE_H

#include "protocols/Gate.h"

template<typename ShrType>
class AdditionGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;

    using Gate<ShrType>::Gate;


private:
    void doRunOffline() override {
        this->lambdaShr[0] = this->input_x->getLambdaShr()[0] + this->input_y->getLambdaShr()[0];
        this->lambdaShrMac[0] = this->input_x->getLambdaShrMac()[0] + this->input_y->getLambdaShrMac()[0];
    }

    void doRunOnline() override {
        this->deltaClear[0] = this->input_x->getDeltaClear()[0] + this->input_y->getDeltaClear()[0];
    }
};

#endif //MALICIOUS_PPML_ADDITIONGATE_H
