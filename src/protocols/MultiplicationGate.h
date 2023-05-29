#ifndef MALICIOUS_PPML_MULTIPLICATIONGATE_H
#define MALICIOUS_PPML_MULTIPLICATIONGATE_H

#include "protocols/Gate.h"

template<typename ShareType>
class MultiplicationGate : public Gate<ShareType> {
public:
    void runOffline() override {
        // generate random lambda_zShr
        // compute triple
    }

protected:
    ShareType lambda_xyShr;
};

#endif //MALICIOUS_PPML_MULTIPLICATIONGATE_H
