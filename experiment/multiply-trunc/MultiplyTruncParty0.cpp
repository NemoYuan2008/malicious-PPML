#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "utils/fixedPoint.h"
#include "MultiplyTruncConfig.h"
#include "utils/benchmark.h"

int main() {
    std::cin >> std::hex;
    std::cout << std::hex;

    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);
    std::vector<Spdz2kShare64::ClearType> xIn({double2fix<Spdz2kShare64::ClearType>(-10)});
    std::vector<Spdz2kShare64::ClearType> yIn({double2fix<Spdz2kShare64::ClearType>(12.325535)});
    for (int i = 0; i < times; ++i) {
        auto x = circuit.input(0, rows, cols);
        auto y = circuit.input(0, rows, cols);
        auto a = circuit.multiplyTrunc(x, y);
        auto o = circuit.output(a);
        x->setInput(xIn);
        y->setInput(yIn);
        circuit.addEndpoint(o);
    }
    circuit.readOfflineFromFile();
    circuit.shakeHand();
    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";


    return 0;
}
