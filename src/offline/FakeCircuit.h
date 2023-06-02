#ifndef MALICIOUS_PPML_FAKECIRCUIT_H
#define MALICIOUS_PPML_FAKECIRCUIT_H


#include <vector>
#include <memory>
#include "FakeOfflineGate.h"


template<typename ShrType, int N>
class FakeCircuit {
public:
    FakeCircuit() = default;

    void runOffline() {
        for (const auto &gatePtr: endpoints) {
            gatePtr->runOffline();
        }
    }

    std::shared_ptr<FakeInputGate<ShrType, N>>
    input() {
        auto gate = std::make_shared<FakeInputGate<ShrType, N>>();
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

    void addEndpoint(const std::shared_ptr<FakeGate<ShrType, N>> &gate) {
        endpoints.push_back(gate);
    }

private:
    std::vector<std::shared_ptr<FakeGate<ShrType, N>>> gates;
    std::vector<std::shared_ptr<FakeGate<ShrType, N>>> endpoints;
};


#endif //MALICIOUS_PPML_FAKECIRCUIT_H