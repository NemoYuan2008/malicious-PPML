#include <iostream>
#include <memory>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "utils/rand.h"
#include "protocols/Gate.h"
#include "protocols/InputGate.h"
#include "protocols/AdditionGate.h"
#include "protocols/Circuit.h"

using std::cout;
using std::hex;
using std::make_shared;

int main() {
    Circuit<Spdz2kShare32> circuit;
    auto x = make_shared<InputGate<Spdz2kShare32>>(), y = make_shared<InputGate<Spdz2kShare32>>();
    auto a = circuit.add(x, y);
    auto b = circuit.add(a, x);
    auto c = circuit.add(a, b);
    uint32_t xClear = 10, yClear = 20;
    x->setDeltaClear(xClear);
    y->setDeltaClear(yClear);
    circuit.addEndpoint(b);
    circuit.addEndpoint(c);
    circuit.runOnline();

    return 0;
}
