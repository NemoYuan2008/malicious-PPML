#ifndef MALICIOUS_PPML_FAKEOFFLINEGATE_H
#define MALICIOUS_PPML_FAKEOFFLINEGATE_H


#include "protocols/Gate.h"
#include "protocols/InputGate.h"
#include "protocols/AdditionGate.h"
#include "protocols/MultiplicationGate.h"
#include "utils/rand.h"


template<typename ShrType>
class FakeGate : public Gate<ShrType> {
public:
    using Gate<ShrType>::Gate;
    using typename Gate<ShrType>::ClearType;

    void runOffline() = 0;

    void runOnline() override {}    // dummy override

    ClearType getLambdaClear() const { return lambdaClear; }

protected:
    ClearType lambdaClear;
};


template<typename ShrType>
class FakeInputGate : public FakeGate<ShrType> {
public:
    using Gate<ShrType>::Gate;
    using typename Gate<ShrType>::ClearType;

    void runOffline() override {
        //Input gate mustn't have inputs
        this->lambdaClear = getRand<ClearType>();
        this->evaluatedOffline = true;
    }
};


template<typename ShrType>
class FakeAdditionGate : public FakeGate<ShrType> {
public:
    using Gate<ShrType>::Gate;
    using typename Gate<ShrType>::ClearType;

    void runOffline() override {
        this->runOfflineRecursive();
        this->lambdaClear = this->input_x->getLambdaClear() + this->input_y->getLambdaClear();
        this->evaluatedOffline = true;
    }
};


template<typename ShrType>
class FakeMultiplicationGate : public FakeGate<ShrType> {
public:
    using Gate<ShrType>::Gate;
    using typename Gate<ShrType>::ClearType;

    void runOffline() override {
        this->runOfflineRecursive();
        this->lambda_xyClear = this->input_x->getLambdaClear() * this->input_y->getLambdaClear();
        this->lambdaClear = getRand<ClearType>();
    }

protected:
    ClearType lambda_xyClear;
};

#endif //MALICIOUS_PPML_FAKEOFFLINEGATE_H
