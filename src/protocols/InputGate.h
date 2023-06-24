#ifndef MALICIOUS_PPML_INPUTGATE_H
#define MALICIOUS_PPML_INPUTGATE_H


#include <vector>
#include <algorithm>
#include <stdexcept>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#ifndef NDEBUG

#include "utils/ioHelper.h"

#endif

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
        int size = this->dimRow * this->dimCol;
        this->lambdaShrMac.resize(size);
        this->lambdaShr.resize(size);

        if (this->ownerId == this->myId())
            lambdaClear.resize(size);

        for (int i = 0; i < size; ++i) {
            if (this->ownerId == this->myId())
                ifs >> this->lambdaClear[i];

            ifs >> this->lambdaShr[i] >> this->lambdaShrMac[i];
        }
    }

    void doRunOffline() override {
        // Generate random lambdaShr, where the owner knows lambdaShr.
    }

    void doRunOnline() override {
        if (this->myId() == this->ownerId) {

            this->deltaClear = matrixAdd(this->lambdaClear, this->inputValue);
#ifndef NDEBUG
            std::cout <<"set input, size: "<<this->deltaClear.size()<<"\n";
#endif
            this->party->getNetwork().send(1 - this->myId(), this->deltaClear);
        } else {
            this->deltaClear.resize(this->dimRow * this->dimCol);
#ifndef NDEBUG
            std::cout <<"receive input, size: "<<this->deltaClear.size()<<"\n";
#endif
            this->party->getNetwork().rcv(1 - this->myId(), &this->deltaClear, this->deltaClear.size());
        }
        sleep(1);

#ifndef NDEBUG
//        std::cout << "\nInputGate Online\n";
//        std::cout << "lambdaShr:";
//        printVector(this->lambdaShr);
//        std::cout << "deltaClear:";
//        printVector(this->deltaClear);
#endif
    }


private:
    // inputValue and lambdaClear should be ClearType, but stored as SemiShrType for convenience
    std::vector<SemiShrType> inputValue;
    std::vector<SemiShrType> lambdaClear;    //should be known to owner
    int ownerId;
};


#endif //MALICIOUS_PPML_INPUTGATE_H
