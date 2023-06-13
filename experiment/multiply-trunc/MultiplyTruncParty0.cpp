#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "MultiplyTruncConfig.h"


int main() {
    std::cin >> std::hex;
    std::cout << std::hex;

    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    auto x = circuit.input(0, 1, 1);
    auto y = circuit.input(0, 1, 1);
    auto a = circuit.multiplyTrunc(x, y);
    auto o = circuit.output(a);
    circuit.addEndpoint(o);
    circuit.readOfflineFromFile();

    std::vector<Spdz2kShare64::ClearType> xIn(1), yIn(1);
    std::cout << "Please input x, y (in hexadecimal): ";
    std::cin >> xIn[0] >> yIn[0];

    x->setInput(xIn);
    y->setInput(yIn);

    circuit.runOnline();
    printVector(o->getClear());

    return 0;
}
