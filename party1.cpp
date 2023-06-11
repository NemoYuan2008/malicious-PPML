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

    //a = x + y, b = a * z, c = z + b, d = a * c
    auto x = circuit.input(0, 2, 1);
    auto y = circuit.input(0, 2, 1);

    auto a = circuit.add(x, y);
    auto z = circuit.input(0, 1, 2);
    auto b = circuit.multiply(a, z);

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

    return 0;
}
