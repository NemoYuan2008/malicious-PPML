#include <iostream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/benchmark.h"

#include "ReLUConfig.h"


int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

//    for (int i = 0; i < times; ++i) {
    auto x = circuit.input(0, rows, cols);
    auto a = circuit.relu(x);
    auto o = circuit.output(a);
    circuit.addEndpoint(o);
//    }

    circuit.readOfflineFromFile();

    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";

    return 0;
}