#include <iostream>
#include <fstream>
#include <memory>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeOfflineGate.h"
#include "offline/FakeCircuit.h"

int main() {
    std::cin >> std::hex;
    std::cout << std::hex;

    FakeOffline<32, 32, 2> offline;
    std::array<std::ostream *, 2> files{&std::cout, &std::cout};

    //a = x + y, b = a * x, c = z + b, d = a * c
    auto x = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(files, offline);
    auto y = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(files, offline);
    auto z = std::make_shared<FakeInputGate<Spdz2kShare32, 2>>(files, offline);

    auto a = std::make_shared<FakeAdditionGate<Spdz2kShare32, 2>>(x, y);
    auto b = std::make_shared<FakeMultiplicationGate<Spdz2kShare32, 2>>(a, x);
    auto c = std::make_shared<FakeAdditionGate<Spdz2kShare32, 2>>(z, b);
    auto d = std::make_shared<FakeMultiplicationGate<Spdz2kShare32, 2>>(a, c);

    d->runOffline();

    std::cout << x->getLambdaClear() << ' ' << y->getLambdaClear() << ' ' << a->getLambdaClear() << '\n';

    return 0;
}
