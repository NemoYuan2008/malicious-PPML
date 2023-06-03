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

    FakeGate(std::array<std::ostream *, N> &files,
             const FakeOfflineBase<ShrType, N> &offline)
            : files(files), offline(offline) {}

    FakeGate(const std::shared_ptr<FakeGate> &input_x,
             const std::shared_ptr<FakeGate> &input_y,
             std::array<std::ostream *, N> &files,
             const FakeOfflineBase<ShrType, N> &offline)
            : input_x(input_x), input_y(input_y), files(files), offline(offline) {}

    FakeGate(const std::shared_ptr<FakeGate> &input_x,
             const std::shared_ptr<FakeGate> &input_y)
            : FakeGate(input_x, input_y, input_x->files, input_x->offline) {}

    void runOffline() {
        if (this->isEvaluatedOffline())
            return;

        if (input_x && !input_x->isEvaluatedOffline())
            input_x->runOffline();
        if (input_y && !input_y->isEvaluatedOffline())
            input_y->runOffline();

        this->doRunOffline();

        this->evaluatedOffline = true;
    }

private:
    virtual void doRunOffline() = 0;    //Used in runOffline, do the offline job of this gate

public:
    ClearType getLambdaClear() const { return lambdaClear; }

    const Shares &getLambdaShares() const { return lambdaShares; }

    [[nodiscard]] bool isEvaluatedOffline() const { return evaluatedOffline; }

protected:
    std::shared_ptr<FakeGate> input_x{}, input_y{};
    std::array<std::ostream *, N> &files;
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

    void doRunOffline() override {
        this->lambdaClear = getRand<ClearType>();
        this->lambdaShares = this->offline.generateShares(this->lambdaClear);

        for (int i = 0; i < N; ++i) {
            *this->files[i] << this->lambdaShares[i] << '\n';
        }
    }
};


template<typename ShrType, int N>
class FakeAdditionGate : public FakeGate<ShrType, N> {
public:
    using FakeGate<ShrType, N>::FakeGate;
    using typename FakeGate<ShrType, N>::ClearType;

    void doRunOffline() override {
        // Compute lambdaClear and lambdaShares
        this->lambdaClear = this->input_x->getLambdaClear() + this->input_y->getLambdaClear();
        for (int i = 0; i < N; ++i) {
            this->lambdaShares[i] = this->input_x->getLambdaShares()[i] + this->input_y->getLambdaShares()[i];
        }

        // Write lambdaShares to file
        for (int i = 0; i < N; ++i) {
            *this->files[i] << this->lambdaShares[i] << '\n';
        }
    }
};


template<typename ShrType, int N>
class FakeMultiplicationGate : public FakeGate<ShrType, N> {
public:
    using FakeGate<ShrType, N>::FakeGate;
    using typename FakeGate<ShrType, N>::ClearType;
    using typename FakeGate<ShrType, N>::Shares;

    void doRunOffline() override {
        this->lambda_xyClear = this->input_x->getLambdaClear() * this->input_y->getLambdaClear();
        this->lambdaClear = getRand<ClearType>();

        this->lambda_xyShares = this->offline.generateShares(this->lambda_xyClear);
        this->lambdaShares = this->offline.generateShares(this->lambdaClear);

        for (int i = 0; i < N; ++i) {
            *this->files[i] << this->lambdaShares[i] << ' ' << this->lambda_xyShares[i] << '\n';
        }
    }

    ClearType getLambdaXyClear() const { return lambda_xyClear; }

    const Shares &getLambdaXyShares() const { return lambda_xyShares; }

protected:
    ClearType lambda_xyClear;
    Shares lambda_xyShares;
};

#endif //MALICIOUS_PPML_FAKEOFFLINEGATE_H
