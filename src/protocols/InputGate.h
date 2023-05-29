#ifndef MALICIOUS_PPML_INPUTGATE_H
#define MALICIOUS_PPML_INPUTGATE_H


#include "protocols/Gate.h"

template<typename ShareType>
class InputGate : public Gate<ShareType> {
public:
    InputGate() = default;  // parent of InputGate should be nullptr

    void runOffline() override {
        // Generate random lambda_zShr, where the owner knows lambda_zShr.
        // Owner computes delta_zClear and broadcast it.
        // Parties save delta_zClear
    }

protected:
    typename ShareType::ClearType lambda_zClear;    //should be known to owner
};


#endif //MALICIOUS_PPML_INPUTGATE_H
