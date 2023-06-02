#include <iostream>
#include <fstream>
#include <memory>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeOfflineGate.h"
#include "offline/FakeCircuit.h"

int main() {
    std::array<std::ofstream, 2> files{
            std::ofstream("0.txt"),
            std::ofstream("1.txt")
    };
    //We must write hex numbers
    for (auto &f: files) {
        f << std::hex;
    }

    FakeOffline<32, 32, 2> offline;
    FakeCircuit<Spdz2kShare32, 2> circuit(files, offline);

    //a = x + y, b = a * x, c = z + b, d = a * c
    auto x = circuit.input();
    auto y = circuit.input();
    auto z = circuit.input();

    auto a = circuit.add(x, y);
    auto b = circuit.multiply(a, x);
    auto c = circuit.add(z, b);
    auto d = circuit.multiply(a, c);

    circuit.addEndpoint(d);
    circuit.runOffline();

    return 0;
}
