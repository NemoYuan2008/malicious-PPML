#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/benchmark.h"
#include "utils/ioHelper.h"
#include "LenetConfig.h"

#include <chrono>

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

    uint32_t dur_time = 0;
    for (int i = 0; i < batch_size; ++i) {

        auto x = circuit.input(0, channel * features, 1);
        auto conv1_weight = circuit.input(0, conv1_op.kernel_shape_[0] * conv1_op.kernel_shape_[1] * conv1_op.kernel_shape_[2] * conv1_op.kernel_shape_[3], 1);
        auto conv1_bias = circuit.input(0, conv1_op.output_shape_[0] * conv1_op.output_shape_[1] * conv1_op.output_shape_[2], 1);
        auto conv2_weight = circuit.input(0, conv2_op.kernel_shape_[0] * conv2_op.kernel_shape_[1] * conv2_op.kernel_shape_[2] * conv2_op.kernel_shape_[3], 1);
        auto conv2_bias = circuit.input(0, conv2_op.output_shape_[0] * conv2_op.output_shape_[1] * conv2_op.output_shape_[2], 1);
        auto fc1_weight = circuit.input(0, linear1, avg_op2.output_shape_[0]*avg_op2.output_shape_[1]*avg_op2.output_shape_[2]);
        auto fc1_bias = circuit.input(0, linear1, 1);
        auto fc2_weight = circuit.input(0, linear2, linear1);
        auto fc2_bias = circuit.input(0, linear2, 1);
        auto fc3_weight = circuit.input(0, classes, linear2);
        auto fc3_bias = circuit.input(0, classes, 1);
        // conv1
        auto conv1 = circuit.conv2DTrunc(x, conv1_weight, conv1_op);
        auto add1 = circuit.add(conv1, conv1_bias);
        auto r1 = circuit.relu(add1);
        // avgpool1
        auto avg1 = circuit.avgPool2D(r1, avg_op1);
        // conv2
        auto conv2 = circuit.conv2DTrunc(avg1, conv2_weight, conv2_op);
        auto add2 = circuit.add(conv2, conv2_bias);
        auto r2 = circuit.relu(add2);
        // avgpool2
        auto avg2 = circuit.avgPool2D(r2, avg_op2);
        // linear1
        auto m1 = circuit.multiplyTrunc(fc1_weight, avg2);
        auto add3 = circuit.add(m1, fc1_bias);
        auto r3 = circuit.relu(add3);
        // linear2
        auto m2 = circuit.multiplyTrunc(fc2_weight, r3);
        auto add4 = circuit.add(m2, fc2_bias);
        auto r4 = circuit.relu(add4);
        // linear3
        auto m5 = circuit.multiplyTrunc(fc3_weight, r4);
        auto add5 = circuit.add(m5, fc3_bias);
        //    auto pred = circuit.argmax(add5);
        auto end = circuit.output(add5);
        circuit.addEndpoint(end);
        circuit.readOfflineFromFile();

        // start time
        auto start = std::chrono::steady_clock::now();

        std::vector<Spdz2kShare64::ClearType> xIn(channel * features),
                c1wIn(conv1_op.kernel_shape_[0]*conv1_op.kernel_shape_[1]*conv1_op.kernel_shape_[2]*conv1_op.kernel_shape_[3]),
                c1bIn(conv1_op.output_shape_[0]*conv1_op.output_shape_[1]*conv1_op.output_shape_[2]),
                c2wIn(conv2_op.kernel_shape_[0]*conv2_op.kernel_shape_[1]*conv2_op.kernel_shape_[2]*conv2_op.kernel_shape_[3]),
                c2bIn(conv2_op.output_shape_[0]*conv2_op.output_shape_[1]*conv2_op.output_shape_[2]),
                f1wIn(linear1 * avg_op2.output_shape_[0]*avg_op2.output_shape_[1]*avg_op2.output_shape_[2]),
                f1bIn(linear1 * 1),
                f2wIn(linear2 * linear1),
                f2bIn(linear2 * 1),
                f3wIn(classes * linear2),
                f3bIn(classes * 1);
        auto c1b_ = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(conv1_op.output_shape_[0]*conv1_op.output_shape_[1]*conv1_op.output_shape_[2],1));
        auto c2b_ = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(conv2_op.output_shape_[0]*conv2_op.output_shape_[1]*conv2_op.output_shape_[2],1));

        xIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(channel * features,1));
        c1wIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(conv1_op.kernel_shape_[0]*conv1_op.kernel_shape_[1]*conv1_op.kernel_shape_[2]*conv1_op.kernel_shape_[3],1));
        c1bIn = c1b_;
        c2wIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(conv2_op.kernel_shape_[0]*conv2_op.kernel_shape_[1]*conv2_op.kernel_shape_[2]*conv2_op.kernel_shape_[3],1));
        c2bIn = c2b_;
        f1wIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(linear1 * avg_op2.output_shape_[0]*avg_op2.output_shape_[1]*avg_op2.output_shape_[2],1));
        f1bIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(linear1,1));
        f2wIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(linear2 * linear1,1));
        f2bIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(linear2,1));
        f3wIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(classes * linear2,1));
        f3bIn = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(classes,1));
//        std::cout << "input: \n";
        x->setInput(xIn);
        conv1_weight->setInput(c1wIn);
        conv1_bias->setInput(c1bIn);
        conv2_weight->setInput(c2wIn);
        conv2_bias->setInput(c2bIn);
        fc1_weight->setInput(f1wIn);
        fc1_bias->setInput(f1bIn);
        fc2_weight->setInput(f2wIn);
        fc2_bias->setInput(f2bIn);
        fc3_weight->setInput(f3wIn);
        fc3_bias->setInput(f3bIn);

        circuit.runOnline();

        // end time
        auto stop = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        dur_time += duration.count();
    }

    std::cout << "average time: " << dur_time / batch_size << "ms \n";
    return 0;
}