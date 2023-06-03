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
        this->lambdaShr = this->input_x->getLambdaShr() + this->input_y->getLambdaShr();
    }

    void doRunOnline() override {
        this->deltaClear = this->input_x->getDeltaClear() + this->input_y->getDeltaClear();
    }
};

#endif //MALICIOUS_PPML_ADDITIONGATE_H
