#ifndef MALICIOUS_PPML_BENCHMARK_H
#define MALICIOUS_PPML_BENCHMARK_H


#include <chrono>
#include <utility>

template<typename Func, typename... Args>
auto benchmark(const Func &f, Args &&...args) {
    auto start = std::chrono::steady_clock::now();
    f(std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return duration.count();
}


template<typename Func, typename... Args>
auto benchmark(size_t times, const Func &f, Args &&...args) {
    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < times; ++i) {
        f(std::forward<Args>(args)...);
    }
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return duration.count();
}


#endif //MALICIOUS_PPML_BENCHMARK_H
