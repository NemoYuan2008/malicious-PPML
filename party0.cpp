#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"

using std::cout;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

    // a = x + y, b = a * z
    auto x = circuit.input(0, 3, 4);
    auto y = circuit.input(0, 3, 4);

    auto a = circuit.add(x, y);
    auto z = circuit.input(0, 4, 2);
    auto b = circuit.multiply(a, z);
    auto o = circuit.output(b);


    std::vector<Spdz2kShare32::ClearType>
            xIn{4, 2, 6, 3, 1, 6, 8, 5, 3, 4, 5, 7},
            yIn{3, 5, 3, 2, 6, 8, 0, 4, 6, 8, 3, 5},
            zIn{2, 3, 5, 7, 2, 5, 7, 9};

    //     x                     y                z
    //[4, 3, 8, 4],         [3, 2, 0, 8],       [2, 2]
    //[2, 1, 5, 5],         [5, 6, 4, 3],       [3, 5]
    //[6, 6, 3, 7],         [3, 8, 6, 5],       [5, 7],
    //                                          [7, 9],

    x->setInput(xIn);
    y->setInput(yIn);
    z->setInput(zIn);

    circuit.addEndpoint(o);
    circuit.readOfflineFromFile();
    circuit.runOnline();

    //    b
    //[153, 203],
    //[136, 184],
    //[189, 259]

    printVector(b->getLambdaShr());
    printVector(b->getDeltaClear());
    printVector(o->getClear());

    return 0;
}
