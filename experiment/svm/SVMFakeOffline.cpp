//
// Created by zyx on 23-6-16.
//
#include <iostream>
#include <fstream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeCircuit.h"

#include "SVMConfig.h"

int main() {

    auto path = std::filesystem::temp_directory_path();
    std::cout << path << '\n';

    std::array<std::ofstream, 2> files{
            std::ofstream(path / "0.txt"),
            std::ofstream(path / "1.txt")
    };

    FakeOffline<64, 64, 2> offline(files);
    FakeCircuit<Spdz2kShare64, 2> circuit(files, offline);

    auto w = circuit.input(0, features, classes);
    auto b = circuit.input(0, batch_size, classes);
    auto x = circuit.input(0, batch_size, features);
    auto y = circuit.multiplyTrunc(x, w);
    auto z = circuit.add(y, b);
    auto pred = circuit.argmax(z);
    auto end = circuit.output(pred);

    circuit.addEndpoint(end);

    circuit.runOffline();
}
