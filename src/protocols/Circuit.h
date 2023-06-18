#ifndef MALICIOUS_PPML_CIRCUIT_H
#define MALICIOUS_PPML_CIRCUIT_H


#include <vector>
#include <memory>
#include "protocols/Gate.h"
#include "protocols/AdditionGate.h"
#include "protocols/InputGate.h"
#include "protocols/MultiplicationGate.h"
#include "protocols/MultiplyTruncGate.h"
#include "protocols/Conv2DGate.h"
#include "protocols/LtzGate.h"
#include "protocols/OutputGate.h"
#include "utils/Party.h"


template<typename ShrType>
class Circuit {
public:
    Circuit() = default;    //should only be used for tests
    explicit Circuit(Party<ShrType> *party) : party(party) {}

    void readOfflineFromFile(std::ifstream &ifs) {
        for (const auto &gate: endpoints) {
            gate->readOfflineFromFile(ifs);
        }
    }

    void readOfflineFromFile() {
        readOfflineFromFile(party->getFileStream());
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
    input(int ownerId = 0, int row = 1, int column = 1) {
        auto gate = std::make_shared<InputGate<ShrType>>(party, row, column, ownerId);
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

    std::shared_ptr<MultiplyTruncGate<ShrType>>
    multiplyTrunc(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y) {
        auto gate = std::make_shared<MultiplyTruncGate<ShrType>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<LtzGate<ShrType>>
    gtz(const std::shared_ptr<Gate<ShrType>> &input_x) {
        auto gate = std::make_shared<LtzGate<ShrType>>(input_x);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<Conv2DGate<ShrType>>
    conv2D(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y,
           const Conv2DOp &op) {
        auto gate = std::make_shared<Conv2DGate<ShrType>>(input_x, input_y, op);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<OutputGate<ShrType>>
    output(const std::shared_ptr<Gate<ShrType>> &input_x) {
        auto gate = std::make_shared<OutputGate<ShrType>>(input_x);
        gates.push_back(gate);
        return gate;
    }

    void addEndpoint(const std::shared_ptr<Gate<ShrType>> &gate) {
        endpoints.push_back(gate);
    }

private:
    std::vector<std::shared_ptr<Gate<ShrType>>> gates;
    std::vector<std::shared_ptr<Gate<ShrType>>> endpoints;
    Party<ShrType> *party;
};


#endif //MALICIOUS_PPML_CIRCUIT_H
