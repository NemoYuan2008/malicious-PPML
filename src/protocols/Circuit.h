#ifndef MALICIOUS_PPML_CIRCUIT_H
#define MALICIOUS_PPML_CIRCUIT_H


#include <vector>
#include <memory>
#include "protocols/Gate.h"
#include "protocols/AdditionGate.h"


template<typename ShrType>
class Circuit {
public:
    Circuit() = default;

    void runOffline() {
        for (const auto &gatePtr: endpoints) {
            gatePtr->runOffline();
        }
    }

    void runOnline() {
        for (const auto &gatePtr: endpoints) {
            gatePtr->runOnline();
        }
    }

    std::shared_ptr<AdditionGate<ShrType>>
    add(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y) {
        auto gate = std::make_shared<AdditionGate<ShrType>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    void addEndpoint(const std::shared_ptr<Gate<ShrType>> &gate) {
        endpoints.push_back(gate);
    }

private:
    std::vector<std::shared_ptr<Gate<ShrType>>> gates;
    std::vector<std::shared_ptr<Gate<ShrType>>> endpoints;
};


#endif //MALICIOUS_PPML_CIRCUIT_H
