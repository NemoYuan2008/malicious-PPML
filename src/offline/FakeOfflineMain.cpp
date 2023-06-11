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

    //a = x + y, b = a * x, c = z + b, d = a * c
    auto x = circuit.input();
    auto y = circuit.input();
    auto z = circuit.input();

    auto a = circuit.add(x, y);
    auto b = circuit.multiply(a, x);
    auto c = circuit.add(z, b);
    auto d = circuit.multiply(a, c);

    //recursive sequence should be: x, y, a, z, b, c, d
    circuit.addEndpoint(d);
    circuit.runOffline();

    return 0;
}
