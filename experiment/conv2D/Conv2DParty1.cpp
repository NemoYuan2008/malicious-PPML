#include <iostream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "Conv2DConfig.h"


int main() {
    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    auto x = circuit.input(0, 18, 1);
    auto y = circuit.input(0, 18, 1);
    auto a = circuit.conv2D(x, y, conv_op);
    auto o = circuit.output(a);

    circuit.addEndpoint(o);
    circuit.readOfflineFromFile();

    circuit.runOnline();
    printVector(o->getClear());

    return 0;
}
