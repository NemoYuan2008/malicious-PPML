#ifndef MALICIOUS_PPML_INPUTGATE_H
#define MALICIOUS_PPML_INPUTGATE_H


#include <vector>
#include <algorithm>
#include <stdexcept>
#include "protocols/Gate.h"

template<typename ShrType>
class InputGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    explicit InputGate(Party<ShrType> *party, int row, int column, int ownerId = 0)
            : Gate<ShrType>(party, row, column), ownerId(ownerId) {}

    void setInput(const std::vector<ClearType> &input) {
        if (this->myId() != this->ownerId) {
            throw std::logic_error("Not the owner of input gate, cannot set input");
        }
        if (input.size() != this->dimRow * this->dimCol) {
            throw std::invalid_argument("Input vector and gate doesn't match in size");
        }

        this->inputValue = std::vector<SemiShrType>(input.begin(), input.end());
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
            this->deltaClear[0] = this->lambdaClear[0] + this->inputValue[0];
            this->party->getNetwork().send(1 - this->myId(), this->deltaClear[0]);
        } else {
            this->party->getNetwork().rcv(1 - this->myId(), &this->deltaClear[0]);
        }
    }


private:
    // inputValue and lambdaClear should be ClearType, but stored as SemiShrType for convenience
    std::vector<SemiShrType> inputValue;
    std::vector<SemiShrType> lambdaClear;    //should be known to owner
    int ownerId;
};


#endif //MALICIOUS_PPML_INPUTGATE_H
