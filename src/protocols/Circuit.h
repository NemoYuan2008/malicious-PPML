#ifndef MALICIOUS_PPML_CIRCUIT_H
#define MALICIOUS_PPML_CIRCUIT_H


#include <vector>
#include <memory>
#include "protocols/Gate.h"
#include "protocols/AdditionGate.h"
#include "protocols/SubtractionGate.h"
#include "protocols/InputGate.h"
#include "protocols/DummyInputGate.h"
#include "protocols/MultiplicationGate.h"
#include "protocols/AddConstantGate.h"
#include "protocols/MultiplyByConstantGate.h"
#include "protocols/MultiplyTruncGate.h"
#include "protocols/Conv2DGate.h"
#include "protocols/Conv2DTruncGate.h"
#include "protocols/AvgPool2DGate.h"
#include "protocols/LtzGate.h"
#include "protocols/GtzGate.h"
#include "protocols/ElemMultiplicationGate.h"
#include "protocols/ReLUGate.h"
#include "protocols/SliceGate.h"
#include "protocols/OutputGate.h"
#include "utils/Party.h"


template<typename ShrType>
class Circuit {
public:
    using ClearType = typename ShrType::ClearType;

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

    std::shared_ptr<SliceGate<ShrType>>
    slice(const std::shared_ptr<Gate<ShrType>> &input_x, std::size_t index) {
        auto gate = std::make_shared<SliceGate<ShrType>>(input_x, index);
        gates.push_back(gate);
        return gate;
    }

//    std::shared_ptr<DummyInputGate<ShrType>>
//    dummyInput(int row = 1, int col = 1) {
//        auto gate = std::make_shared<DummyInputGate<ShrType>>(party, row, col);
//        gates.push_back(gate);
//        return gate;
//    }

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

    std::shared_ptr<SubtractionGate<ShrType>>
    subtract(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y) {
        auto gate = std::make_shared<SubtractionGate<ShrType>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<MultiplicationGate<ShrType>>
    multiply(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y) {
        auto gate = std::make_shared<MultiplicationGate<ShrType>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<AddConstantGate<ShrType>>
    addConstant(const std::shared_ptr<Gate<ShrType>> &input_x, ClearType c) {
        auto gate = std::make_shared<AddConstantGate<ShrType>>(input_x, c);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<MultiplyByConstantGate<ShrType>>
    multiplyByConstant(const std::shared_ptr<Gate<ShrType>> &input_x, ClearType c) {
        auto gate = std::make_shared<MultiplyByConstantGate<ShrType>>(input_x, c);
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
    ltz(const std::shared_ptr<Gate<ShrType>> &input_x) {
        auto gate = std::make_shared<LtzGate<ShrType>>(input_x);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<GtzGate<ShrType>>
    gtz(const std::shared_ptr<Gate<ShrType>> &input_x) {
        auto gate = std::make_shared<GtzGate<ShrType>>(input_x);
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

    std::shared_ptr<Conv2DTruncGate<ShrType>>
    conv2DTrunc(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y,
                const Conv2DOp &op) {
        auto gate = std::make_shared<Conv2DTruncGate<ShrType>>(input_x, input_y, op);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<AvgPool2DGate<ShrType>>
    avgPool2D(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y,
              const MaxPoolOp &op) {
        auto gate = std::make_shared<AvgPool2DGate<ShrType>>(input_x, op);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<ElemMultiplicationGate<ShrType>>
    elementMultiply(const std::shared_ptr<Gate<ShrType>> &input_x, const std::shared_ptr<Gate<ShrType>> &input_y) {
        auto gate = std::make_shared<ElemMultiplicationGate<ShrType>>(input_x, input_y);
        gates.push_back(gate);
        return gate;
    }

    std::shared_ptr<ReLUGate<ShrType>>
    relu(const std::shared_ptr<Gate<ShrType>> &input_x) {
        auto gate = std::make_shared<ReLUGate<ShrType>>(input_x);
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

    const auto &getEndpoints() const { return endpoints; }

private:
    std::vector<std::shared_ptr<Gate<ShrType>>> gates;
    std::vector<std::shared_ptr<Gate<ShrType>>> endpoints;
    Party<ShrType> *party;
};


#endif //MALICIOUS_PPML_CIRCUIT_H
