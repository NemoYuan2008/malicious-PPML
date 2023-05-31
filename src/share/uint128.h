#ifndef MALICIOUS_PPML_UINT128_H
#define MALICIOUS_PPML_UINT128_H

#include <iostream>
#include <iomanip>
#include <cstdint>

std::ostream &operator<<(std::ostream &os, __uint128_t x);

std::istream &operator>>(std::istream &is, __uint128_t &x);

//std::ostream &write128(std::ostream &os, __uint128_t x);

//std::istream &read128(std::istream &is, __uint128_t &x);

#endif //MALICIOUS_PPML_UINT128_H
