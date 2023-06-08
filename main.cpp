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
#include "utils/Party.h"

using std::cin;
using std::cout;
using std::hex;
using std::make_shared;

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare32> p0(0, 2, (path / "0.txt").string(), false),
            p1(1, 2, (path / "1.txt").string(), false);

    std::array<Circuit<Spdz2kShare32>, 2> circuit{Circuit<Spdz2kShare32>(&p0), Circuit<Spdz2kShare32>(&p1)};
    std::array<std::shared_ptr<InputGate<Spdz2kShare32>>, 2> x, y, z;
    std::array<std::shared_ptr<AdditionGate<Spdz2kShare32>>, 2> a, c;
    std::array<std::shared_ptr<MultiplicationGate<Spdz2kShare32>>, 2> b, d;

    for (int i = 0; i < 2; ++i) {
        x[i] = circuit[i].input(0);
        y[i] = circuit[i].input(0);
        z[i] = circuit[i].input(0);

        a[i] = circuit[i].add(x[i], y[i]);
        b[i] = circuit[i].multiply(a[i], x[i]);
        c[i] = circuit[i].add(z[i], b[i]);
        d[i] = circuit[i].multiply(a[i], c[i]);
    }

    //recursive sequence should be: x, y, a, z, b, d
    for (int i = 0; i < 2; ++i) {
        circuit[i].addEndpoint(d[i]);
        circuit[i].readOfflineFromFile();
    }
    cout << x[0]->getLambdaShr() << '\n'
         << x[1]->getLambdaShr() << '\n'
         << y[0]->getLambdaShr() << '\n'
         << y[1]->getLambdaShr() << '\n'
         << a[0]->getLambdaShr() << '\n'
         << a[1]->getLambdaShr() << '\n';

    return 0;
}
