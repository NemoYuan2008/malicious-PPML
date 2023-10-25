#include <iostream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/benchmark.h"

#include "LenetConfig.h"


int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare64> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

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

        //without argmax
//        auto end = circuit.output(add5);

        //with argmax
        auto pred = circuit.argmax(add5);
        auto end = circuit.output(pred);

        circuit.addEndpoint(end);
        circuit.readOfflineFromFile();
        circuit.runOnline();
    }

    return 0;
}