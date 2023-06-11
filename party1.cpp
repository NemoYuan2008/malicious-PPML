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
    auto x = circuit.input(0, 2, 1);
    auto y = circuit.input(0, 2, 1);

    auto a = circuit.add(x, y);
    auto z = circuit.input(0, 1, 2);
    auto b = circuit.multiply(a, z);

    circuit.addEndpoint(b);
    circuit.readOfflineFromFile();
    circuit.runOnline();

    //a = (4, 6), b = (20, 30, 24, 36)
    printVector(b->getLambdaShr());
    printVector(b->getDeltaClear());

    return 0;
}
