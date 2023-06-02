#ifndef MALICIOUS_PPML_FAKECIRCUIT_H
#define MALICIOUS_PPML_FAKECIRCUIT_H


#include <ostream>
#include <array>
#include <vector>
#include <memory>

#include "offline/FakeOffline.h"
#include "offline/FakeOfflineGate.h"


template<typename ShrType, int N>
class FakeCircuit {
public:
    FakeCircuit(std::array<std::ostream *, N> &files, const FakeOfflineBase<ShrType, N> &offline)
            : files(files), offline(offline) {}

    FakeCircuit(std::array<std::ofstream, N> &p_files, const FakeOfflineBase<ShrType, N> &offline)
            : offline(offline) {
        for (int i = 0; i < N; ++i) {
            files[i] = &p_files[i];
        }
    }

    void runOffline() {
        for (const auto &gatePtr: endpoints) {
            gatePtr->runOffline();
        }
    }

    std::shared_ptr<FakeInputGate<ShrType, N>>
    input() {
        auto gate = std::make_shared<FakeInputGate<ShrType, N>>(files, offline);
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
    std::array<std::ostream *, N> files;
    const FakeOfflineBase<ShrType, N> &offline;
    std::vector<std::shared_ptr<FakeGate<ShrType, N>>> gates;
    std::vector<std::shared_ptr<FakeGate<ShrType, N>>> endpoints;
};


#endif //MALICIOUS_PPML_FAKECIRCUIT_H
