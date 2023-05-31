#ifndef MALICIOUS_PPML_FAKEOFFLINEGATE_H
#define MALICIOUS_PPML_FAKEOFFLINEGATE_H


#include <fstream>
#include <memory>
#include "protocols/Gate.h"
#include "protocols/InputGate.h"
#include "protocols/AdditionGate.h"
#include "protocols/MultiplicationGate.h"
#include "utils/rand.h"


template<typename ShrType>
class FakeGate {
public:
    using typename Gate<ShrType>::ClearType;

    explicit FakeGate(std::ofstream &ofs) : Gate<ShrType>(), file(ofs) {}

    FakeGate(const std::shared_ptr<FakeGate> &input_x, const std::shared_ptr<FakeGate> &input_y, std::ofstream &ofs)
            : Gate<ShrType>(input_x, input_y), file(ofs) {}

    FakeGate(const std::shared_ptr<FakeGate> &input_x, const std::shared_ptr<FakeGate> &input_y)
            : FakeGate(input_x, input_y, input_x->file) {}

    virtual void runOffline() = 0;

    ClearType getLambdaClear() const { return lambdaClear; }

protected:
    std::shared_ptr<FakeGate> input_x, input_y;
    std::ofstream &file;
    ClearType lambdaClear{};
};


template<typename ShrType>
class FakeInputGate : public FakeGate<ShrType> {
public:
    using FakeGate<ShrType>::FakeGate;
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
    using FakeGate<ShrType>::FakeGate;
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
