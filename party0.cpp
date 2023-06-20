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
    auto a = circuit.subtract(x, y);
    auto e = circuit.multiplyByConstant(a, 23);
    auto z = circuit.input(0, 4, 2);
    auto b = circuit.multiply(e, z);
    auto f = circuit.addConstant(b, 12);
    auto c = circuit.relu(f);
    auto w = circuit.input(1, 2, 4);
    auto d = circuit.multiply(c, w);
    auto o = circuit.output(d);
    circuit.addEndpoint(o);
    circuit.readOfflineFromFile();

    //     x                     y                z
    //[4, 2, 6, 3],         [3, 5, 3, 2],       [2, 3],
    //[1, 6, 8, 5],         [6, 8, 0, 4],       [5, 7],
    //[3, 4, 5, 7],         [6, 8, 3, 5],       [2, 5],
    //                                          [7, 9],

    std::vector<Spdz2kShare32::ClearType>
            xIn{4, 2, 6, 3, 1, 6, 8, 5, 3, 4, 5, 7},
            yIn{3, 5, 3, 2, 6, 8, 0, 4, 6, 8, 3, 5},
            zIn{2, 3, 5, 7, 2, 5, 7, 9};
    x->setInput(xIn);
    y->setInput(yIn);
    z->setInput(zIn);

    circuit.runOnline();


    // [ 54  30  42  48]
    // [198 109 143 166]
    // [  0   0   0   0]
    printVector(o->getClear());

    return 0;
}
