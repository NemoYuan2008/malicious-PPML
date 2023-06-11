#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"

using std::cout;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

    //a = x + y, b = a * z, c = z + b, d = a * c
    auto x = circuit.input(0, 2, 1);
    auto y = circuit.input(0, 2, 1);

    auto a = circuit.add(x, y);
    auto z = circuit.input(0, 1, 2);
    auto b = circuit.multiply(a, z);



    std::vector<Spdz2kShare32::ClearType> xIn({1, 2}), yIn({3, 4}), zIn({5, 6});
    x->setInput(xIn);
    y->setInput(yIn);
    z->setInput(zIn);

//    circuit.addEndpoint(a);
    circuit.addEndpoint(b);
    circuit.readOfflineFromFile();
    circuit.runOnline();

    for (int i = 0; i < 2; ++i) {
        cout << a->getLambdaShr()[i] << ' ' << a->getDeltaClear()[i] << '\n';
    }
    cout << '\n';

    for (int i = 0; i < 4; ++i) {
        cout << b->getLambdaShr()[i] << ' ' << b->getDeltaClear()[i] << '\n';
    }

//    cout << x->getLambdaShr()[0] << '\n'
//         << x->getDeltaClear()[0] << '\n'
//         << y->getLambdaShr()[0] << '\n'
//         << y->getDeltaClear()[0] << '\n'
//         << z->getLambdaShr()[0] << '\n'
//         << z->getDeltaClear()[0] << '\n'
//         << a->getLambdaShr()[0] << '\n'
//         << a->getDeltaClear()[0] << '\n'
//         << b->getLambdaShr()[0] << '\n'
//         << b->getDeltaClear()[0] << '\n'
//         << c->getLambdaShr()[0] << '\n'
//         << c->getDeltaClear()[0] << '\n'
//         << d->getLambdaShr()[0] << '\n'
//         << d->getDeltaClear()[0] << '\n';

    return 0;
}
