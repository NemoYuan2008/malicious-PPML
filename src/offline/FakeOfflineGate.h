#ifndef MALICIOUS_PPML_FAKEOFFLINEGATE_H
#define MALICIOUS_PPML_FAKEOFFLINEGATE_H


#include <ostream>
#include <memory>
#include <algorithm>
#include <functional>
#include "offline/FakeOfflineBase.h"
#include "utils/rand.h"


template<typename ShrType, int N>
class FakeGate {
public:
    using ClearType = typename ShrType::ClearType;
    using Shares = std::array<ShrType, N>;

    explicit FakeGate(std::ofstream &ofs, FakeOfflineBase<ShrType, N> &offline) : file(ofs), offline(offline) {}

    FakeGate(const std::shared_ptr<FakeGate> &input_x, const std::shared_ptr<FakeGate> &input_y,
             std::ofstream &ofs, const FakeOfflineBase<ShrType, N> &offline)
            : input_x(input_x), input_y(input_y), file(ofs), offline(offline) {}

    FakeGate(const std::shared_ptr<FakeGate> &input_x, const std::shared_ptr<FakeGate> &input_y)
            : FakeGate(input_x, input_y, input_x->file, input_x->offline) {}

    virtual void runOffline() = 0;

    ClearType getLambdaClear() const { return lambdaClear; }

    const Shares &getLambdaShares() const { return lambdaShares; }

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
    Shares lambdaShares;
    bool evaluatedOffline = false;
};


template<typename ShrType, int N>
class FakeInputGate : public FakeGate<ShrType, N> {
public:
    using FakeGate<ShrType, N>::FakeGate;
    using typename FakeGate<ShrType, N>::ClearType;

    void runOffline() override {
        //Input gate mustn't have input wires, no need to call runOfflineRecursive()
        this->lambdaClear = getRand<ClearType>();
        this->lambdaShares = this->offline.generateShares(this->lambdaClear);
        //TODO: write lambdaShares to file
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

        //lambdaShares in addition gate is not generated using offline.generateShares,
        //it's generated by locally adding shares of inputs
        //lambdaShares = input_x.lambdaShares + input_y.lambdaShares
        std::transform(this->input_x->getLambdaShares.begin(), this->input_x->getLambdaShares.end(),
                       this->input_y->getLambdaShares.begin(),
                       this->lambdaShares.begin(), std::plus<>());

        //TODO: write lambdaShares to file

        this->evaluatedOffline = true;
    }
};


template<typename ShrType, int N>
class FakeMultiplicationGate : public FakeGate<ShrType, N> {
public:
    using FakeGate<ShrType, N>::FakeGate;
    using typename FakeGate<ShrType, N>::ClearType;
    using typename FakeGate<ShrType, N>::Shares;

    void runOffline() override {
        this->runOfflineRecursive();

        this->lambda_xyClear = this->input_x->getLambdaClear() * this->input_y->getLambdaClear();
        this->lambdaClear = getRand<ClearType>();

        this->lambdaShares = this->offline.generateShares(this->lambdaClear);
        this->lambda_xyShares = this->offline.generateShares(this->lambda_xyClear);

        //TODO: write lambdaShares to file
        this->evaluatedOffline = true;
    }

    ClearType getLambdaXyClear() const { return lambda_xyClear; }

    const Shares &getLambdaXyShares() const { return lambda_xyShares; }

protected:
    ClearType lambda_xyClear;
    Shares lambda_xyShares;
};

#endif //MALICIOUS_PPML_FAKEOFFLINEGATE_H
