//
// Created by zyx on 23-6-16.
//
#include <iostream>
#include <filesystem>
#include <vector>
#include <numeric>
#include <Eigen/Dense>
#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/benchmark.h"
#include "utils/fixedPoint.h"
#include "utils/ioHelper.h"
#include "SVMConfig.h"

std::vector<double> generateRandIn(int rows, int cols){
    std::vector<double> ret(rows*cols);
    for (int i = 0; i < rows*cols; ++i) {
        ret[i] = rand();
    }
    return ret;
}

int main() {

    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    auto w = circuit.input(0, features, classes);
    auto b = circuit.input(0, batch_size, classes);
    auto x = circuit.input(0, batch_size, features);
    auto y = circuit.multiplyTrunc(x, w);
    auto z = circuit.add(y, b);
    auto pred = circuit.argmax(z);
    auto end = circuit.output(pred);

    circuit.addEndpoint(end);

    circuit.readOfflineFromFile();

    // start time
    uint32_t dur_time = 0;
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<Spdz2kShare64::ClearType> wIn(classes * features), bIn(classes), xIn(batch_size * features);
    wIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(classes*features,1));
    bIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(batch_size*classes,1));
    xIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(batch_size * features,1));
//    bIn = copyVector(bIn, batch_size);

    w->setInput(wIn);
    b->setInput(bIn);
    x->setInput(xIn);

    circuit.runOnline();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    dur_time += duration.count();

    std::cout << "average time: " << dur_time / batch_size << "ms \n";

//    printVector(end->getClear());

    return 0;
}