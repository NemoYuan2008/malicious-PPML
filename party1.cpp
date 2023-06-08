#include <iostream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"

using std::cout;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare32> circuit(&party);

    auto x = circuit.input(0);
    auto y = circuit.input(0);
    auto z = circuit.input(0);

    auto a = circuit.add(x, y);
    auto b = circuit.multiply(a, x);
    auto c = circuit.add(z, b);
    auto d = circuit.multiply(a, c);

//    x->setInput(10);
//    y->setInput(20);
//    z->setInput(30);

    circuit.addEndpoint(d);
    circuit.readOfflineFromFile();
    circuit.runOnline();

    cout << x->getLambdaShr() << '\n'
         << x->getDeltaClear() << '\n'
         << y->getLambdaShr() << '\n'
         << y->getDeltaClear() << '\n'
         << z->getLambdaShr() << '\n'
         << z->getDeltaClear() << '\n'
         << a->getLambdaShr() << '\n'
         << a->getDeltaClear() << '\n'
         << b->getLambdaShr() << '\n'
         << b->getDeltaClear() << '\n'
         << c->getLambdaShr() << '\n'
         << c->getDeltaClear() << '\n'
         << d->getLambdaShr() << '\n'
         << d->getDeltaClear() << '\n';

    return 0;
}
