#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "ComparisonConfig.h"
#include "utils/benchmark.h"

using std::cout;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare64> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);
    for (int i = 0; i < times; ++i) {
        auto x = circuit.input(0, rows, cols);
        auto b = circuit.gtz(x);
        auto o = circuit.output(b);
        circuit.addEndpoint(o);
    }
    circuit.readOfflineFromFile();

    circuit.shakeHand();
    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";

    return 0;
}
