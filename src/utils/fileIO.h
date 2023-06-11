#ifndef MALICIOUS_PPML_FILEIO_H
#define MALICIOUS_PPML_FILEIO_H


#include <iostream>
#include <memory>
#include <array>
#include <vector>
#include <string>


template<typename Tp>
void printVector(const std::vector<Tp> &v) {
    for (const auto &x: v) {
        std::cout << x << ' ';
    }
}


//Maybe no use
template<size_t N>
std::array<std::unique_ptr<std::ostream>, N>
openOutputFiles(const std::array<std::string, N> &fileNames) {
    std::array<std::unique_ptr<std::ostream>, N> ret;
    for (size_t i = 0; i < N; ++i) {
        ret[i] = std::make_unique<std::ofstream>(fileNames[i], std::ios_base::out);
        *ret[i] << std::hex;
    }
    return ret;
}


#endif //MALICIOUS_PPML_FILEIO_H
