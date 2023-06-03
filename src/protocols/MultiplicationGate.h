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
    void doRunOffline() override {
        // generate random lambdaShr
        // compute triple
    }

    void doRunOnline() override {
        auto delta_xClear = this->input_x->getDeltaClear(), delta_yClear = this->input_y->getDeltaClear();
        const auto &lambda_xShr = this->input_x->getLambdaShr(), &lambda_yShr = this->input_y->getLambdaShr();
        auto delta_zShr = delta_xClear * delta_yClear + lambda_xShr * delta_yClear + lambda_yShr * delta_xClear +
                          lambda_xyShr + this->lambdaShr;
        //TODO: open delta_zShr and set this->deltaShr
    }
};

#endif //MALICIOUS_PPML_MULTIPLICATIONGATE_H
