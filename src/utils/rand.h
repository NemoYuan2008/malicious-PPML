//
// Created by yuan on 23-5-26.
//

#ifndef MALICIOUS_PPML_RAND_H
#define MALICIOUS_PPML_RAND_H


#include <cstdint>
#include <type_traits>
#include <random>
#include <algorithm>


template<typename Tp>
inline
Tp getRand() {
    static_assert(std::is_integral_v<Tp>, "Template parameter must be an integral type");
    static_assert(sizeof(Tp) == 8 || sizeof(Tp) == 16);

    static std::random_device rd;
    static std::independent_bits_engine<std::default_random_engine,
            8 * sizeof(unsigned long long), unsigned long long> rng(rd());

    //Output of std::independent_bits_engine must be unsigned long long, so we have to use sizeof to determine buffer length
    unsigned long long buf[sizeof(Tp) / sizeof(unsigned long long)];
    std::generate(std::begin(buf), std::end(buf), rng);
    return *reinterpret_cast<Tp *>(buf);
}


template<typename Tp>
inline
Tp getRandFromDevice() {
    static_assert(std::is_integral_v<Tp>, "Template parameter must be an integral type");
    static_assert(sizeof(Tp) == 8 || sizeof(Tp) == 16);

    static std::random_device rd;
    std::random_device::result_type buf[sizeof(Tp) / sizeof(std::random_device::result_type)];
    for (auto *ptr = std::begin(buf); ptr != std::end(buf); ++ptr) {
        *ptr = rd();
    }
    return *reinterpret_cast<Tp *>(buf);
}


// Use boost::random::random_device, slower
//__uint128_t rand_boost() {
//    static boost::random::random_device rng;
//    uint32_t buf[4];
//    rng.generate(std::begin(buf), std::end(buf));
//    return *reinterpret_cast<__uint128_t *>(buf);
//}


#endif //MALICIOUS_PPML_RAND_H
