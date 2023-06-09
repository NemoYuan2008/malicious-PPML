#ifndef MALICIOUS_PPML_INPUTGATE_H
#define MALICIOUS_PPML_INPUTGATE_H


#include <stdexcept>
#include "protocols/Gate.h"

template<typename ShrType>
class InputGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    explicit InputGate(Party<ShrType> *party, int ownerId = 0) : Gate<ShrType>(party), ownerId(ownerId) {}

    void setInput(ClearType input) {
        if (this->myId() != this->ownerId) {
            throw std::logic_error("Not the owner of input gate, cannot set input");
        }
        this->inputValue = input;
    }

    ClearType getLambdaClear() const { return lambdaClear; }

    void setLambdaClear(ClearType p_lambdaClear) { lambdaClear = p_lambdaClear; }


private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        lambdaClear.resize(1);
        this->lambdaShrMac.resize(1);
        this->lambdaShr.resize(1);
        this->deltaClear.resize(1);

        if (this->ownerId == this->myId())
            ifs >> this->lambdaClear[0];

        ifs >> this->lambdaShr[0] >> this->lambdaShrMac[0];
    }

    void doRunOffline() override {
        // Generate random lambdaShr, where the owner knows lambdaShr.
    }

    void doRunOnline() override {
        if (this->myId() == this->ownerId) {
            this->deltaClear[0] = this->lambdaClear[0] + this->inputValue;
            this->party->getNetwork().send(1 - this->myId(), this->deltaClear[0]);
        } else {
            this->party->getNetwork().rcv(1 - this->myId(), &this->deltaClear[0]);
        }
    }


private:
    ClearType inputValue;
    std::vector<SemiShrType> lambdaClear;    //should be known to owner
//    ClearType lambdaClear;    //should be known to owner
    int ownerId;
};


#endif //MALICIOUS_PPML_INPUTGATE_H
