#include <iostream>
#include <memory>

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
    cin >> hex;
    cout << hex;

    __uint128_t x;
    cin >> x;
    cout << x;

    return 0;
}
