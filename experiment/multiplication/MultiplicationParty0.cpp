#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/benchmark.h"

#include "multiplicationConfig.h"


int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    for (int i = 0; i < times; ++i) {
        auto x = circuit.input(0, rows, cols);
        auto y = circuit.input(0, cols, rows);
        auto a = circuit.multiply(x, y);
        auto o = circuit.output(a);
        circuit.addEndpoint(o);

        std::vector<Spdz2kShare64::ClearType> xIn(rows * cols), yIn(rows * cols);
        for (int j = 0; j < rows * cols; j++) {
            xIn[j] = getRand<Spdz2kShare64::ClearType>();
            yIn[j] = getRand<Spdz2kShare64::ClearType>();
        }
        x->setInput(xIn);
        y->setInput(yIn);
    }

    circuit.readOfflineFromFile();

    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";

    return 0;
}