#include <iostream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"

using std::cout;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

    // a = x + y, b = a * z
    auto x = circuit.input(0, 3, 4);
    auto y = circuit.input(0, 3, 4);

    auto a = circuit.add(x, y);
    auto z = circuit.input(0, 4, 2);
    auto b = circuit.multiply(a, z);
    auto o = circuit.output(b);

    circuit.addEndpoint(o);
    circuit.readOfflineFromFile();
    circuit.runOnline();

    printVector(b->getLambdaShr());
    printVector(b->getDeltaClear());
    printVector(o->getClear());

    return 0;
}
