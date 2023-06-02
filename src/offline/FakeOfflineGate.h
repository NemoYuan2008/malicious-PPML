#ifndef MALICIOUS_PPML_FAKEOFFLINEGATE_H
#define MALICIOUS_PPML_FAKEOFFLINEGATE_H


#include <ostream>
#include <memory>
#include "offline/FakeOfflineBase.h"
#include "utils/rand.h"


template<typename ShrType, int N>
class FakeGate {
public:
    using ClearType = typename ShrType::ClearType;

    explicit FakeGate(std::ofstream &ofs, FakeOfflineBase<ShrType, N> &offline) : file(ofs), offline(offline) {}

    FakeGate(const std::shared_ptr<FakeGate> &input_x, const std::shared_ptr<FakeGate> &input_y,
             std::ofstream &ofs, const FakeOfflineBase<ShrType, N> &offline)
            : input_x(input_x), input_y(input_y), file(ofs), offline(offline) {}

    FakeGate(const std::shared_ptr<FakeGate> &input_x, const std::shared_ptr<FakeGate> &input_y)
            : FakeGate(input_x, input_y, input_x->file, input_x->offline) {}

    virtual void runOffline() = 0;

    ClearType getLambdaClear() const { return lambdaClear; }

    [[nodiscard]] bool isEvaluatedOffline() const { return evaluatedOffline; }

protected:
    void runOfflineRecursive() {   //used in runOffline
        if (input_x && !input_x->isEvaluatedOffline())
            input_x->runOffline();
        if (input_y && !input_y->isEvaluatedOffline())
            input_y->runOffline();
    }

protected:
    std::shared_ptr<FakeGate> input_x{}, input_y{};
    std::ofstream &file;
    const FakeOfflineBase<ShrType, N> &offline;
    ClearType lambdaClear{};
    bool evaluatedOffline = false;
};


template<typename ShrType, int N>
class FakeInputGate : public FakeGate<ShrType, N> {
public:
    using FakeGate<ShrType, N>::FakeGate;
    using typename FakeGate<ShrType, N>::ClearType;

    void runOffline() override {
        //Input gate mustn't have inputs
        this->lambdaClear = getRand<ClearType>();
        this->evaluatedOffline = true;
    }
};


template<typename ShrType, int N>
class FakeAdditionGate : public FakeGate<ShrType, N> {
public:
    using FakeGate<ShrType, N>::FakeGate;
    using typename FakeGate<ShrType, N>::ClearType;

    void runOffline() override {
        this->runOfflineRecursive();
        this->lambdaClear = this->input_x->getLambdaClear() + this->input_y->getLambdaClear();
        this->evaluatedOffline = true;
    }
};


template<typename ShrType, int N>
class FakeMultiplicationGate : public FakeGate<ShrType, N> {
public:
    using FakeGate<ShrType, N>::FakeGate;
    using typename FakeGate<ShrType, N>::ClearType;

    void runOffline() override {
        this->runOfflineRecursive();
        this->lambda_xyClear = this->input_x->getLambdaClear() * this->input_y->getLambdaClear();
        this->lambdaClear = getRand<ClearType>();
    }

protected:
    ClearType lambda_xyClear;
};

#endif //MALICIOUS_PPML_FAKEOFFLINEGATE_H
