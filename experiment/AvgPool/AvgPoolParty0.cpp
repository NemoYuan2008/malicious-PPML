#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/fixedPoint.h"
#include "utils/benchmark.h"

#include "AvgPoolConfig.h"


int main() {
    std::cout << std::hex;
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

//    for (int i = 0; i < times; ++i) {
    auto x = circuit.input(0, rows, cols);
    auto a = circuit.avgPool2D(x, op);
    auto o = circuit.output(a);
    circuit.addEndpoint(o);

    std::vector<Spdz2kShare64::ClearType> xIn(rows * cols);
    for (int j = 0; j < rows * cols; j++) {
        xIn[j] = double2fix<Spdz2kShare64::ClearType>(j);
    }
    x->setInput(xIn);

    std::cout << "xIn:\n";
    printVector(xIn);
//    }

    circuit.readOfflineFromFile();

    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";

    std::cout << "Result: \n";
    printVector(o->getClear());

    return 0;
}