#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "Conv2DConfig.h"


int main() {
    std::cin >> std::hex;
    std::cout << std::hex;

    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    auto x = circuit.input(0, 18, 1);
    auto y = circuit.input(0, 18, 1);
    auto a = circuit.conv2D(x, y, conv_op);
    auto o = circuit.output(a);

    circuit.addEndpoint(o);
    circuit.readOfflineFromFile();

    std::vector<Spdz2kShare64::ClearType> xIn{
            1, 2, 3, 1, 1, 1, 2, 2, 2,
            2, 1, 3, 4, 2, 1, 2, 2, 1,
    };
    std::vector<Spdz2kShare64::ClearType> yIn{
            0, 1, 2, 3, 2, 1, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 1, 1, 1,
    };

    x->setInput(xIn);
    y->setInput(yIn);

    circuit.runOnline();
    printVector(o->getClear());

    return 0;
}