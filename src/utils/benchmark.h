#ifndef MALICIOUS_PPML_BENCHMARK_H
#define MALICIOUS_PPML_BENCHMARK_H


#include <chrono>

template<typename Func, typename... Args>
auto benchmark(const Func &f, Args &&...args) {
    auto start = std::chrono::steady_clock::now();
    f(std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return duration.count();
}


#endif //MALICIOUS_PPML_BENCHMARK_H
