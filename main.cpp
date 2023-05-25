#include <iostream>
#include "share/Share.h"
#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"

int main() {
    Spdz2kShare64 s(12, 23), k(2, 10);
    auto t = s + k;


    return 0;
}
