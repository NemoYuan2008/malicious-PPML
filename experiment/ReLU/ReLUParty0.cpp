#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/benchmark.h"

#include "ReLUConfig.h"


int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

//    for (int i = 0; i < times; ++i) {
    auto x = circuit.input(0, rows, cols);
    auto a = circuit.relu(x);
    auto o = circuit.output(a);
    circuit.addEndpoint(o);

    std::vector<Spdz2kShare32::ClearType> xIn(rows * cols);
    for (int j = 0; j < rows * cols; j++) {
        xIn[j] = j;
    }
    x->setInput(xIn);
//    }

    circuit.readOfflineFromFile();

//    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";
    circuit.runOnline();

    std::cout << "Result: \n";
    printVector(o->getClear());

    return 0;
}