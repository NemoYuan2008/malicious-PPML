#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "utils/fixedPoint.h"
#include "utils/benchmark.h"
#include "Conv2DConfig.h"
#include <chrono>
std::vector<double> generateRandIn(int rows, int cols) {
    std::vector<double> ret(rows * cols);
    for (int i = 0; i < rows * cols; ++i) {
        ret[i] = rand();
    }
    return ret;
}

int main() {
    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);
    auto kernel_in = double2fixVec<Spdz2kShare64::ClearType>(
            generateRandIn(conv_op.kernel_shape_[0] * conv_op.kernel_shape_[1],
                           conv_op.kernel_shape_[2] * conv_op.kernel_shape_[3]));
    auto input_x = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(rows, cols));
    for (int i = 0; i < times; ++i) {


        auto kernel1 = circuit.input(0, conv_op.kernel_shape_[0] * conv_op.kernel_shape_[1],
                                     conv_op.kernel_shape_[2] * conv_op.kernel_shape_[3]);
        auto input_image = circuit.input(0, conv_op.output_shape_[1], conv_op.output_shape_[2]);

        input_image->setInput(input_x);
        kernel1->setInput(kernel_in);

        auto a = circuit.conv2DTrunc(input_image, kernel1, conv_op);
        auto o = circuit.output(a);

        circuit.addEndpoint(o);
    }
    circuit.readOfflineFromFile();
    circuit.shakeHand();
//
//    std::vector<Spdz2kShare64::ClearType> xIn{
//            1, 2, 3, 1, 1, 1, 2, 2, 2,
//            2, 1, 3, 4, 2, 1, 2, 2, 1,
//    };
//    std::vector<Spdz2kShare64::ClearType> yIn{
//            0, 1, 2, 3, 2, 1, 1, 1, 1,
//            1, 1, 1, 0, 0, 0, 1, 1, 1,
//    };
//
//    std::for_each(xIn.begin(), xIn.end(), [](auto &num) { num = double2fix<Spdz2kShare64::ClearType>(num); });
//    std::for_each(yIn.begin(), yIn.end(), [](auto &num) { num = double2fix<Spdz2kShare64::ClearType>(num); });


    auto start = std::chrono::steady_clock::now();
    circuit.runOnline();
    auto finish = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << duration.count() <<" ms\n";
//    std::cout << benchmark([&]() { circuit.runOnline(); }) << "ms\n";
//    circuit.runOnline();


    return 0;
}
