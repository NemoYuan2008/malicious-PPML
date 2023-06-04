#ifndef MALICIOUS_PPML_CIRCUIT_H
#define MALICIOUS_PPML_CIRCUIT_H


#include <vector>
#include <memory>
#include "protocols/Gate.h"
#include "protocols/AdditionGate.h"
#include "protocols/InputGate.h"
#include "protocols/MultiplicationGate.h"


template<typename ShrType>
class Circuit {
public:
    Circuit() = default;

    void readOfflineFromFile(std::ifstream &ifs) {
        for (const auto &gate: endpoints) {
            gate->readOfflineFromFile(ifs);
        }
    }

    void runOffline() {
        for (const auto &gate: endpoints) {
            gate->runOffline();
        }
    }

    void runOnline() {
        for (const auto &gate: endpoints) {
            gate->runOnline();
        }
    }

    std::shared_ptr<InputGate<ShrType>>
    input() {
        auto gate = std::make_shared<InputGate<ShrType>>();
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<AdditionGate<ShrType>>
    add(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y) {
        auto gate = std::make_shared<AdditionGate<ShrType>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<MultiplicationGate<ShrType>>
    multiply(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y) {
        auto gate = std::make_shared<MultiplicationGate<ShrType>>(input_x, input_y);
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
