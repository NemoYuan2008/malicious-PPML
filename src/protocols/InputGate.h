#ifndef MALICIOUS_PPML_INPUTGATE_H
#define MALICIOUS_PPML_INPUTGATE_H


#include <stdexcept>
#include "protocols/Gate.h"

template<typename ShrType>
class InputGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;

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
        if (this->ownerId == this->myId())
            ifs >> this->lambdaClear;
        ifs >> this->lambdaShr;
    }

    void doRunOffline() override {
        // Generate random lambdaShr, where the owner knows lambdaShr.
    }

    void doRunOnline() override {
        if (this->myId() == this->ownerId) {
            this->deltaClear = this->lambdaClear + this->inputValue;
            this->party->getNetwork().send(1 - this->myId(), &this->deltaClear);
        } else {
            this->party->getNetwork().rcv(1 - this->myId(), &this->deltaClear);
        }
    }


private:
    ClearType inputValue;
    ClearType lambdaClear;    //should be known to owner
    int ownerId;
};


#endif //MALICIOUS_PPML_INPUTGATE_H
