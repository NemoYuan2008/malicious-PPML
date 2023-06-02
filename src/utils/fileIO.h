#ifndef MALICIOUS_PPML_FILEIO_H
#define MALICIOUS_PPML_FILEIO_H


#include <iostream>
#include <memory>
#include <array>
#include <string>


//Maybe no use
template<size_t N>
std::array<std::unique_ptr<std::ostream>, N>
openOutputFiles(const std::array<std::string, N> &fileNames) {
    std::array<std::unique_ptr<std::ostream>, N> ret;
    for (size_t i = 0; i < N; ++i) {
        ret[i] = std::make_unique<std::ofstream>(fileNames[i], std::ios_base::out);
    }
    return ret;
}




#endif //MALICIOUS_PPML_FILEIO_H
