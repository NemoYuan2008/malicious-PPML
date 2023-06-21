//
// Created by zyx on 23-6-16.
//

#include <iostream>
#include <filesystem>
#include <vector>
#include "utils/ioHelper.h"
#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/benchmark.h"

#include "SVMConfig.h"

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare64> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    auto w = circuit.input(0, features, classes);
    auto b = circuit.input(0, batch_size, classes);
    auto x = circuit.input(0, batch_size, features);
    auto y = circuit.multiplyTrunc(x, w);
    auto z = circuit.add(y, b);
    auto pred = circuit.argmax(z);
    auto end = circuit.output(pred);


    circuit.addEndpoint(end);

    circuit.readOfflineFromFile();
    circuit.runOnline();
//    printVector(end->getClear());
    return 0;
}