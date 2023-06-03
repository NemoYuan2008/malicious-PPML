#ifndef MALICIOUS_PPML_INPUTGATE_H
#define MALICIOUS_PPML_INPUTGATE_H


#include "protocols/Gate.h"

template<typename ShrType>
class InputGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;

    InputGate() = default;  // parent of InputGate should be nullptr

    ClearType getLambda_zClear() const { return lambdaClear; }

    void setLambda_zClear(ClearType p_lambdaClear) { lambdaClear = p_lambdaClear; }


private:
    void doRunOffline() override {
        // Generate random lambdaShr, where the owner knows lambdaShr.
    }

    void doRunOnline() override {
        // Owner computes deltaClear and broadcast it.
        // Parties save deltaClear
    }


protected:
    ClearType lambdaClear;    //should be known to owner
};


#endif //MALICIOUS_PPML_INPUTGATE_H
