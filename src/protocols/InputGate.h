#ifndef MALICIOUS_PPML_INPUTGATE_H
#define MALICIOUS_PPML_INPUTGATE_H


#include "protocols/Gate.h"

template<typename ShrType>
class InputGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;

     explicit InputGate(int id = 0, int ownerId = 0): Gate<ShrType>(id), ownerId(ownerId) {}

    ClearType getLambdaClear() const { return lambdaClear; }

    void setLambdaClear(ClearType p_lambdaClear) { lambdaClear = p_lambdaClear; }


private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        //TODO: player_id
        ifs >> this->lambdaShr;
    }

    void doRunOffline() override {
        // Generate random lambdaShr, where the owner knows lambdaShr.
    }

    void doRunOnline() override {
        // Owner computes deltaClear and broadcast it.
        // Parties save deltaClear
    }


private:
    ClearType lambdaClear;    //should be known to owner
    int ownerId;
};


#endif //MALICIOUS_PPML_INPUTGATE_H
