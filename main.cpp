#include <iostream>
#include <memory>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "protocols/GateOffline.h"
#include "utils/rand.h"
#include "protocols/Gate.h"
#include "protocols/InputGate.h"
#include "protocols/AdditionGate.h"

using std::cout;
using std::hex;
using std::make_shared;

int main() {
    auto in1 = make_shared<InputGate<Spdz2kShare32>>(), in2 = make_shared<InputGate<Spdz2kShare32>>();
    auto a = make_shared<AdditionGate<Spdz2kShare32>>(in1, in2);
    in1->setDeltaClear(10);
    in2->setDeltaClear(20);
    a->runOnline();

    return 0;
}
