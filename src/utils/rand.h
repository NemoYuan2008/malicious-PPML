//
// Created by yuan on 23-5-26.
//

#ifndef MALICIOUS_PPML_RAND_H
#define MALICIOUS_PPML_RAND_H


#include <cstdint>
#include <type_traits>

#include <sodium.h>


template<typename Tp>
Tp getRand() {
    static_assert(std::is_integral_v<Tp>, "Template parameter must be an integral type");
    uint8_t buf[sizeof(Tp)];
    randombytes_buf(buf, sizeof(Tp));
    return *reinterpret_cast<Tp *>(buf);
}


#endif //MALICIOUS_PPML_RAND_H
