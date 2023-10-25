#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "ArgmaxConfig.h"

using std::cout;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

    auto x = circuit.input(0, rows, cols);
    auto y = circuit.input(0, rows, cols);
    auto z = circuit.add(x, y);
    auto b = circuit.argmax(z);
    auto o = circuit.output(b);
    circuit.addEndpoint(o);

    circuit.readOfflineFromFile();

    std::vector<Spdz2kShare32::ClearType> xIn = {10,2,3,24,5,30,5,102,5,0,
                                                 0,2,32,40,100,1,2,3,0,0};
    x->setInput(xIn);
    y->setInput(xIn);

    circuit.runOnline();
    printVector(o->getClear());

    return 0;
}
