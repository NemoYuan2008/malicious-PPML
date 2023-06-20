#include <iostream>
#include <fstream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeGate.h"
#include "offline/FakeCircuit.h"

int main() {
    auto path = std::filesystem::temp_directory_path();
    std::cout << path << '\n';

    std::array<std::ofstream, 2> files{
            std::ofstream(path / "0.txt"),
            std::ofstream(path / "1.txt")
    };

    FakeOffline<32, 32, 2> offline(files);
    FakeCircuit<Spdz2kShare32, 2> circuit(files, offline);

    //a = x + y, b = a * z, c = z + b, d = a * c
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

    circuit.runOffline();

    return 0;
}
