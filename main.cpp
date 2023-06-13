#include <iostream>
#include "utils/fixedPoint.h"

using std::cin;
using std::cout;
using std::hex;

int main() {
    cout << hex;
    double x = -132.35, y = 230.165;

    auto xFix = double2fix<uint64_t>(x);
    auto yFix = double2fix<uint64_t>(y);
    auto zFix = static_cast<int64_t>(xFix * yFix) >> 8;
    cout << x * y << ' ' << fix2double(zFix);

    return 0;
}
