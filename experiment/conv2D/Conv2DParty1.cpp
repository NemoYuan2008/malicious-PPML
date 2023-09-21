#include <iostream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "Conv2DConfig.h"
#include "utils/benchmark.h"

int main() {

    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);
    for (int i = 0; i < times; ++i) {


        auto kernel1 = circuit.input(0, conv_op.kernel_shape_[0] * conv_op.kernel_shape_[1],
                                     conv_op.kernel_shape_[2] * conv_op.kernel_shape_[3]);
        auto input_image = circuit.input(0, conv_op.output_shape_[1], conv_op.output_shape_[2]);
        auto a = circuit.conv2DTrunc(input_image, kernel1, conv_op);
        auto o = circuit.output(a);

        circuit.addEndpoint(o);

    }
    circuit.readOfflineFromFile();
    circuit.shakeHand();
    auto start = std::chrono::steady_clock::now();
    circuit.runOnline();
    auto finish = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << duration.count() <<" ms\n";
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






    return 0;
}
