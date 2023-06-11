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
    auto x = circuit.input(0, 2, 1);
    auto y = circuit.input(0, 2, 1);

    auto a = circuit.add(x, y);
    auto z = circuit.input(0, 1, 2);
    auto b = circuit.multiply(a, z);

//    circuit.addEndpoint(a);
    circuit.addEndpoint(b);
    circuit.runOffline();

    return 0;
}
