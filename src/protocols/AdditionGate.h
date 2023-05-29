#ifndef MALICIOUS_PPML_ADDITIONGATE_H
#define MALICIOUS_PPML_ADDITIONGATE_H

#include "protocols/Gate.h"

template<typename ShareType>
class AdditionGate : public Gate<ShareType> {
public:
    void runOffline() override {
        this->lambda_zShr = this->input_x->getLambda_zShr() + this->input_y->getLambda_zShr();
    }
};

#endif //MALICIOUS_PPML_ADDITIONGATE_H
