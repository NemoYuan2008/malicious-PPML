#include <iostream>
#include "share/Share.h"
#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "protocols/OperationOffline.h"

int main() {
    Spdz2kShare64 s(12, 23), k(2, 10);
    auto t = s + k;

    AdditionOffline<64, 64> a(s, k);
    return 0;
}
