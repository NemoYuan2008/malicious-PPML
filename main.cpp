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
    auto in1 = make_shared<InputGate<Spdz2kShare32>>(), in2 = make_shared<InputGate<Spdz2kShare32>>();
    auto a = circuit.add(in1, in2);
    auto b = circuit.add(a, in2);
    circuit.addEndpoint(b);

    in1->setDeltaClear(10);
    in2->setDeltaClear(20);

    circuit.runOnline();
    cout << b->getDeltaClear() << '\n';

    return 0;
}
