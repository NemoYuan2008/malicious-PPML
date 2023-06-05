#ifndef MALICIOUS_PPML_ADDITIONGATE_H
#define MALICIOUS_PPML_ADDITIONGATE_H

#include "protocols/Gate.h"

template<typename ShrType>
class AdditionGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;

    using Gate<ShrType>::Gate;

    void runOffline() override {
//        if (this->isEvaluatedOffline())       TODO? also for On  line
//            return;

        this->runOfflineRecursive();
        this->lambdaShr = this->input_x->getLambdaShr() + this->input_y->getLambdaShr();
        this->evaluatedOffline = true;
    }

    void runOnline() override {
        this->runOnlineRecursive();
        this->deltaClear = this->input_x->getDeltaClear() + this->input_y->getDeltaClear();
        this->evaluatedOnline = true;
    }
};

#endif //MALICIOUS_PPML_ADDITIONGATE_H
