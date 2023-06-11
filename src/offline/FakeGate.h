#ifndef MALICIOUS_PPML_FAKEGATE_H
#define MALICIOUS_PPML_FAKEGATE_H


#include <ostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cassert>

#include "offline/FakeOfflineBase.h"
#include "utils/rand.h"
#include "utils/linear_algebra.h"


template<typename ShrType, int N>
class FakeGate {
public:
    using ClearType = typename ShrType::ClearType;
    using SemiShrType = typename ShrType::SemiShrType;

    //This ctor is used by InputGate ctor
    FakeGate(std::array<std::ostream *, N> &files,
             const FakeOfflineBase<ShrType, N> &offline, int row, int column)
            : files(files), offline(offline), dimX(row), dimY(column) {}

    //This ctor is used by other gates, no need to initialize dimX and dimY
    FakeGate(const std::shared_ptr<FakeGate> &p_input_x,
             const std::shared_ptr<FakeGate> &p_input_y)
            : input_x(p_input_x), input_y(p_input_y), files(input_x->files), offline(input_x->offline) {}

//    FakeGate(const std::shared_ptr<FakeGate> &input_x,
//             const std::shared_ptr<FakeGate> &input_y,
//             std::array<std::ostream *, N> &files,
//             const FakeOfflineBase<ShrType, N> &offline)
//            : input_x(input_x), input_y(input_y), files(files), offline(offline) {}

    virtual ~FakeGate() = default;


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
    const auto &getLambdaClear() const { return lambdaClear; }

    const auto &getLambdaShr() const { return lambdaShr; }

    const auto &getLambdaShrMac() const { return lambdaShrMac; }

    [[nodiscard]] int getDimX() const { return dimX; }

    [[nodiscard]] int getDimY() const { return dimY; }

    [[nodiscard]] bool isEvaluatedOffline() const { return evaluatedOffline; }

protected:
    std::shared_ptr<FakeGate> input_x{}, input_y{};
    std::array<std::ostream *, N> &files;
    const FakeOfflineBase<ShrType, N> &offline;

    int dimX = 1, dimY = 1;
    std::vector<SemiShrType> lambdaClear;   //is ClearType, stored as SemiShrType
    std::array<std::vector<SemiShrType>, N> lambdaShr, lambdaShrMac;

private:
    bool evaluatedOffline = false;
};


template<typename ShrType, int N>
class FakeInputGate : public FakeGate<ShrType, N> {
public:
    using typename FakeGate<ShrType, N>::ClearType;
    using typename FakeGate<ShrType, N>::SemiShrType;

    FakeInputGate(std::array<std::ostream *, N> &files, const FakeOfflineBase<ShrType, N> &offline,
                  int row, int column, int ownerId = 0)
            : FakeGate<ShrType, N>(files, offline, row, column), ownerId(ownerId) {
        if (ownerId >= N)
            throw std::out_of_range("ownerId must be less than party numbers N");
    }

private:
    void doRunOffline() override {
        int size = this->dimX * this->dimY;
        for (int j = 0; j < N; ++j) {
            this->lambdaShr[j].reserve(size);
            this->lambdaShrMac[j].reserve(size);
        }

        //Fill lambdaClear with random numbers
        this->lambdaClear.resize(size);
        std::generate(this->lambdaClear.begin(), this->lambdaClear.end(), getRand<ClearType>);

        for (int i = 0; i < size; ++i) {
            auto lambdaShrElem = this->offline.generateShares(this->lambdaClear[i]);

            for (int j = 0; j < N; ++j) { //Owner should know lambdaClear
                this->lambdaShr[j].push_back(lambdaShrElem[j].xi);
                this->lambdaShrMac[j].push_back(lambdaShrElem[j].mi);

                if (j == this->ownerId) {
                    *this->files[j] << this->lambdaClear[i] << ' ';
                }
                *this->files[j] << lambdaShrElem[j].xi << ' ' << lambdaShrElem[j].mi << '\n';
            }
        }

        for (int j = 0; j < N; ++j) {
            *this->files[j] << '\n';
        }

//        this->lambdaClear = getRand<ClearType>();
//        this->lambdaShr = this->offline.generateShares(this->lambdaClear);



    }

private:
    int ownerId;
};


template<typename ShrType, int N>
class FakeAdditionGate : public FakeGate<ShrType, N> {
public:
    using typename FakeGate<ShrType, N>::ClearType;
    using typename FakeGate<ShrType, N>::SemiShrType;

    FakeAdditionGate(const std::shared_ptr<FakeGate<ShrType, N>> &p_input_x,
                     const std::shared_ptr<FakeGate<ShrType, N>> &p_input_y)
            : FakeGate<ShrType, N>(p_input_x, p_input_y) {
        if (p_input_x->getDimX() != p_input_y->getDimX() || p_input_x->getDimY() != p_input_y->getDimY()) {
            throw std::logic_error("Dimension of the two inputs of addition don't match");
        }
        this->dimX = p_input_x->getDimX();
        this->dimY = p_input_x->getDimY();
    }

private:
    void doRunOffline() override {
        // Compute lambdaClear and lambdaShr
        this->lambdaClear = matrixAdd(this->input_x->getLambdaClear(), this->input_y->getLambdaClear());
//        this->lambdaClear = this->input_x->getLambdaClear() + this->input_y->getLambdaClear();
        for (int i = 0; i < N; ++i) {
            this->lambdaShr[i] = matrixAdd(this->input_x->getLambdaShr()[i], this->input_y->getLambdaShr()[i]);
            this->lambdaShrMac[i] = matrixAdd(this->input_x->getLambdaShrMac()[i], this->input_y->getLambdaShrMac()[i]);
//            this->lambdaShr[i] = this->input_x->getLambdaShares()[i] + this->input_y->getLambdaShr()[i];
        }

        // Write lambdaShr to file
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < this->lambdaShr.size(); ++j) {
                *this->files[i] << this->lambdaShr[i][j] << ' ' << this->lambdaShrMac[i][j] << '\n';
            }
            *this->files[i] << '\n';
        }
    }
};


template<typename ShrType, int N>
class FakeMultiplicationGate : public FakeGate<ShrType, N> {
public:
    using typename FakeGate<ShrType, N>::ClearType;
    using typename FakeGate<ShrType, N>::SemiShrType;

    FakeMultiplicationGate(const std::shared_ptr<FakeGate<ShrType, N>> &p_input_x,
                           const std::shared_ptr<FakeGate<ShrType, N>> &p_input_y)
            : FakeGate<ShrType, N>(p_input_x, p_input_y) {
        if (p_input_x->getDimY() != p_input_y->getDimX()) {
            throw std::logic_error("Dimension of the two inputs of multiplication don't match");
        }
        this->dimX = this->input_x->getDimX();
        this->dimY = this->input_y->getDimY();
        this->dimMid = this->input_x->getDimY();
    }

private:
    void doRunOffline() override {
        int size = this->dimX * this->dimY;
        for (int i = 0; i < N; ++i) {
            this->lambdaShr[i].reserve(size);
            this->lambdaShrMac[i].reserve(size);
            this->lambda_xyShr[i].reserve(size);
            this->lambda_xyShrMac[i].reserve(size);
        }

        //Compute lambda_xyClear
        this->lambda_xyClear = matrixMultiply(this->input_x->getLambdaClear(), this->input_y->getLambdaClear(),
                                              this->dimX, this->dimMid, this->dimY);
        assert(size == this->lambda_xyClear.size());

        //Fill lambdaClear with random numbers
        this->lambdaClear.resize(size);
        std::generate(this->lambdaClear.begin(), this->lambdaClear.end(), getRand<ClearType>);


//        this->lambda_xyClear = this->input_x->getLambdaClear() * this->input_y->getLambdaClear();
//        this->lambdaClear = getRand<ClearType>();

        for (int i = 0; i < size; ++i) {
            auto lambdaShrElem = this->offline.generateShares(this->lambdaClear[i]);
            auto lambda_xyShrElem = this->offline.generateShares(this->lambda_xyClear[i]);

            for (int j = 0; j < N; ++j) {
                this->lambdaShr[j].push_back(lambdaShrElem[j].xi);
                this->lambdaShrMac[j].push_back(lambdaShrElem[j].mi);
                this->lambda_xyShr[j].push_back(lambda_xyShrElem[j].xi);
                this->lambda_xyShrMac[j].push_back(lambda_xyShrElem[j].mi);

                *this->files[j] << lambdaShrElem[j].xi << ' '
                                << lambdaShrElem[j].mi << ' '
                                << lambda_xyShrElem[j].xi << ' '
                                << lambda_xyShrElem[j].mi << '\n';
            }
        }

        for (int j = 0; j < N; ++j) {
            *this->files[j] << '\n';
        }
//        this->lambda_xyShr = this->offline.generateShares(this->lambda_xyClear);
//        this->lambdaShr = this->offline.generateShares(this->lambdaClear);

//        for (int i = 0; i < N; ++i) {
//            *this->files[i] << this->lambdaShr[i] << ' ' << this->lambda_xyShr[i] << '\n';
//        }
    }


public:
    const auto &getLambdaXyClear() const { return lambda_xyClear; }

    const auto &getLambdaXyShr() const { return lambda_xyShr; }

    const auto &getLambdaXyShrMac() const { return lambda_xyShrMac; }

protected:
    std::vector<SemiShrType> lambda_xyClear;   //is ClearType, stored as SemiShrType
    std::array<std::vector<SemiShrType>, N> lambda_xyShr, lambda_xyShrMac;
    int dimMid{};
};

#endif //MALICIOUS_PPML_FAKEGATE_H
