#ifndef MALICIOUS_PPML_INPUTGATE_H
#define MALICIOUS_PPML_INPUTGATE_H


#include "protocols/Gate.h"

template<typename ShrType>
class InputGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;

    InputGate() = default;  // parent of InputGate should be nullptr

    void runOffline() override {
        // Generate random lambdaShr, where the owner knows lambdaShr.
    }

    void runOnline() override {
        // Owner computes deltaClear and broadcast it.
        // Parties save deltaClear
    }

    ClearType getLambda_zClear() const { return lambda_zClear; }

    void setLambda_zClear(ClearType lambdaZClear) { lambda_zClear = lambdaZClear; }  //for debugging

protected:
    ClearType lambda_zClear;    //should be known to owner
};


#endif //MALICIOUS_PPML_INPUTGATE_H
