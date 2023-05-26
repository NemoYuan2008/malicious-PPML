#include <iostream>
#include "share/Share.h"
#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "protocols/OperationOffline.h"

int main() {
    Spdz2kShare64 s(0x12, 0x23), k(0x2, 0x10);
    std::cout << s;

    return 0;
}
