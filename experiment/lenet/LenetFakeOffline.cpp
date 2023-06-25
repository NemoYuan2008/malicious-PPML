//
// Created by zyx on 23-6-23.
//
#include <iostream>
#include <fstream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeCircuit.h"

#include "LenetConfig.h"
#include <chrono>

int main() {
    auto start = std::chrono::high_resolution_clock ::now();
    auto path = std::filesystem::temp_directory_path();
    std::cout << path << '\n';

    std::array<std::ofstream, 2> files{
            std::ofstream(path / "0.txt"),
            std::ofstream(path / "1.txt")
    };

    FakeOffline<64, 64, 2> offline(files);
    FakeCircuit<Spdz2kShare64, 2> circuit(files, offline);

    for (int i = 0; i < batch_size; ++i) {
        auto x = circuit.input(0, features, 1);
        auto conv1_weight = circuit.input(0, 6 * 1 * conv_kernel * conv_kernel, 1);
        auto conv1_bias = circuit.input(0, 6 * 24 * 24, 1);
        auto conv2_weight = circuit.input(0, 16 * 6 * conv_kernel * conv_kernel, 1);
        auto conv2_bias = circuit.input(0, 16 * 8 * 8, 1);
        auto fc1_weight = circuit.input(0, 120, 256);
        auto fc1_bias = circuit.input(0, 120, 1);
        auto fc2_weight = circuit.input(0, 84, 120);
        auto fc2_bias = circuit.input(0, 84, 1);
        auto fc3_weight = circuit.input(0, 10, 84);
        auto fc3_bias = circuit.input(0, 10, 1);
        // conv1
        auto conv1 = circuit.conv2DTrunc(x, conv1_weight, conv1_op);
        auto add1 = circuit.add(conv1, conv1_bias);
        auto r1 = circuit.relu(add1);
        // avgpool1
        auto avg1 = circuit.avgPool2D(r1, avg_op1);
        // conv2
        auto conv2 = circuit.conv2DTrunc(avg1, conv2_weight, conv2_op);
        auto add2 = circuit.add(conv2, conv2_bias);
        auto r2 = circuit.relu(add2);
        // avgpool2
        auto avg2 = circuit.avgPool2D(r2, avg_op2);
        // linear1
        auto m1 = circuit.multiplyTrunc(fc1_weight, avg2);
        auto add3 = circuit.add(m1, fc1_bias);
        auto r3 = circuit.relu(add3);
        // linear2
        auto m2 = circuit.multiplyTrunc(fc2_weight, r3);
        auto add4 = circuit.add(m2, fc2_bias);
        auto r4 = circuit.relu(add4);
        // linear3
        auto m5 = circuit.multiplyTrunc(fc3_weight, r4);
        auto add5 = circuit.add(m5, fc3_bias);
        //    auto pred = circuit.argmax(add5);
        auto end = circuit.output(add5);

        circuit.addEndpoint(end);
        //    }
        circuit.runOffline();
    }

    auto stop = std::chrono::high_resolution_clock ::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout<<"running time: "<<duration.count()<<" s\n";

    return 0;
}