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
    auto a = circuit.subtract(x, y);
    auto z = circuit.input(0, 4, 2);
    auto b = circuit.multiply(a, z);
    auto c = circuit.relu(b);
    auto w = circuit.input(1, 2, 4);
    auto d = circuit.multiply(c, w);
    auto o = circuit.output(d);
    circuit.addEndpoint(o);
    circuit.readOfflineFromFile();

    std::vector<Spdz2kShare32::ClearType> wIn{6, 3, 1, 2, 9, 5, 7, 8};
    w->setInput(wIn);

    //     w
    // [6 3 1 2]
    // [9 5 7 8]

    circuit.runOnline();

    printVector(o->getClear());
    return 0;
}
