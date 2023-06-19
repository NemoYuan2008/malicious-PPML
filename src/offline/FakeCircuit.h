#ifndef MALICIOUS_PPML_FAKECIRCUIT_H
#define MALICIOUS_PPML_FAKECIRCUIT_H


#include <ostream>
#include <array>
#include <vector>
#include <memory>

#include "offline/FakeOffline.h"
#include "offline/FakeGate.h"


template<typename ShrType, int N>
class FakeCircuit {
public:
    FakeCircuit(std::array<std::ostream *, N> &filePtrs, const FakeOfflineBase<ShrType, N> &offline)
            : files(filePtrs), offline(offline) {}

    FakeCircuit(std::array<std::ofstream, N> &fileObjects, const FakeOfflineBase<ShrType, N> &offline)
            : offline(offline) {
        for (int i = 0; i < N; ++i) {
            files[i] = &fileObjects[i];
        }
    }

    void runOffline() {
        for (const auto &gatePtr: endpoints) {
            gatePtr->runOffline();
        }
    }

    std::shared_ptr<FakeInputGate<ShrType, N>>
    input(int ownerId = 0, int row = 1, int column = 1) {
        auto gate = std::make_shared<FakeInputGate<ShrType, N>>(files, offline, row, column, ownerId);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<FakeAdditionGate<ShrType, N>>
    add(const std::shared_ptr<FakeGate<ShrType, N>> &input_x,
        const std::shared_ptr<FakeGate<ShrType, N>> &input_y) {
        auto gate = std::make_shared<FakeAdditionGate<ShrType, N>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<FakeMultiplicationGate<ShrType, N>>
    multiply(const std::shared_ptr<FakeGate<ShrType, N>> &input_x,
             const std::shared_ptr<FakeGate<ShrType, N>> &input_y) {
        auto gate = std::make_shared<FakeMultiplicationGate<ShrType, N>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<FakeOutputGate<ShrType, N>>
    output(const std::shared_ptr<FakeGate<ShrType, N>> &input_x) {
        auto gate = std::make_shared<FakeOutputGate<ShrType, N>>(input_x);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<FakeMultiplyTruncGate<ShrType, N>>
    multiplyTrunc(const std::shared_ptr<FakeGate<ShrType, N>> &input_x,
                  const std::shared_ptr<FakeGate<ShrType, N>> &input_y) {
        auto gate = std::make_shared<FakeMultiplyTruncGate<ShrType, N>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<FakeGtzGate<ShrType, N>>
    gtz(const std::shared_ptr<FakeGate<ShrType, N>> &input_x) {
        auto gate = std::make_shared<FakeGtzGate<ShrType, N>>(input_x);
        gates.push_back(gate);
        return gate;
    }

    //Offline phase of ltz is the same as gtz
    std::shared_ptr<FakeGtzGate<ShrType, N>>
    ltz(const std::shared_ptr<FakeGate<ShrType, N>> &input_x) {
        auto gate = std::make_shared<FakeGtzGate<ShrType, N>>(input_x);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<FakeConv2DGate<ShrType, N>>
    conv2D(const std::shared_ptr<FakeGate<ShrType, N>> &input_x,
           const std::shared_ptr<FakeGate<ShrType, N>> &input_y,
           const Conv2DOp &op) {
        auto gate = std::make_shared<FakeConv2DGate<ShrType, N>>(input_x, input_y, op);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<FakeElemMultiplicationGate<ShrType, N>>
    elementMultiply(const std::shared_ptr<FakeGate<ShrType, N>> &input_x,
                    const std::shared_ptr<FakeGate<ShrType, N>> &input_y) {
        auto gate = std::make_shared<FakeElemMultiplicationGate<ShrType, N>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<FakeReLUGate<ShrType, N>>
    relu(const std::shared_ptr<FakeGate<ShrType, N>> &input_x) {
        auto gate = std::make_shared<FakeReLUGate<ShrType, N>>(input_x);
        gates.push_back(gate);
        return gate;
    }


    void addEndpoint(const std::shared_ptr<FakeGate<ShrType, N>> &gate) {
        endpoints.push_back(gate);
    }

    const std::vector<std::shared_ptr<FakeGate<ShrType, N>>> &getEndpoints() const { return endpoints; }

private:
    std::array<std::ostream *, N> files;
    const FakeOfflineBase<ShrType, N> &offline;
    std::vector<std::shared_ptr<FakeGate<ShrType, N>>> gates;
    std::vector<std::shared_ptr<FakeGate<ShrType, N>>> endpoints;
};


#endif //MALICIOUS_PPML_FAKECIRCUIT_H
