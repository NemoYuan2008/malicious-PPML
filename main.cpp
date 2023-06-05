#include <iostream>
#include <memory>
#include <filesystem>
#include <array>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "utils/rand.h"
#include "protocols/Gate.h"
#include "protocols/InputGate.h"
#include "protocols/AdditionGate.h"
#include "protocols/Circuit.h"

using std::cin;
using std::cout;
using std::hex;
using std::make_shared;

int main() {
    auto path = std::filesystem::temp_directory_path();
    cout << path << '\n';
    std::array<std::ifstream, 2> inFiles{std::ifstream(path / "0.txt"), std::ifstream(path / "1.txt")};
    std::array<Circuit<Spdz2kShare32>, 2> circuit;
    std::array<std::shared_ptr<InputGate<Spdz2kShare32>>, 2> x, y, z;
    std::array<std::shared_ptr<AdditionGate<Spdz2kShare32>>, 2> a, c;
    std::array<std::shared_ptr<MultiplicationGate<Spdz2kShare32>>, 2> b, d;

    for (int i = 0; i < 2; ++i) {
        x[i] = circuit[i].input();
        y[i] = circuit[i].input();
        z[i] = circuit[i].input();

        a[i] = circuit[i].add(x[i], y[i]);
        b[i] = circuit[i].multiply(a[i], x[i]);
        c[i] = circuit[i].add(z[i], b[i]);
        d[i] = circuit[i].multiply(a[i], c[i]);
    }

    for (int i = 0; i < 2; ++i) {
        circuit[i].addEndpoint(d[i]);
        circuit[i].readOfflineFromFile(inFiles[i]);
    }
    cout << x[0]->getLambdaShr() << '\n'
         << x[1]->getLambdaShr() << '\n'
         << y[0]->getLambdaShr() << '\n'
         << y[1]->getLambdaShr() << '\n'
         << a[0]->getLambdaShr() << '\n'
         << a[1]->getLambdaShr() << '\n';
    
    return 0;
}
