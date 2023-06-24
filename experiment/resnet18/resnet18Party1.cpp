#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "resnet18Config.h"

template<typename ShrType>
std::shared_ptr<Gate<ShrType>> ResBlock1(Circuit<ShrType> &circuit, std::shared_ptr<Gate<ShrType>> xIn){
    auto kernel1 = circuit.input(0,conv1_op.kernel_shape_[0]*conv1_op.kernel_shape_[1],conv1_op.kernel_shape_[2]*conv1_op.kernel_shape_[3]);
    auto bn1_bias = circuit.input(0,conv1_op.output_shape_[0]*conv1_op.output_shape_[1]*conv1_op.output_shape_[2],1);
    auto bn1_scale = circuit.input(0,conv1_op.output_shape_[0]*conv1_op.output_shape_[1]*conv1_op.output_shape_[2],1);


    auto kernel2 = circuit.input(0,conv2_op.kernel_shape_[0]*conv2_op.kernel_shape_[1],conv2_op.kernel_shape_[2]*conv2_op.kernel_shape_[3]);
    auto bn2_bias = circuit.input(0,conv2_op.output_shape_[0]*conv2_op.output_shape_[1]*conv2_op.output_shape_[2],1);
    auto bn2_scale = circuit.input(0,conv2_op.output_shape_[0]*conv2_op.output_shape_[1]*conv2_op.output_shape_[2],1);

    auto kernel3 = circuit.input(0,conv3_op.kernel_shape_[0]*conv3_op.kernel_shape_[1],conv3_op.kernel_shape_[2]*conv3_op.kernel_shape_[3]);
    auto bn3_bias = circuit.input(0,conv3_op.output_shape_[0]*conv3_op.output_shape_[1]*conv3_op.output_shape_[2],1);
    auto bn3_scale = circuit.input(0,conv3_op.output_shape_[0]*conv3_op.output_shape_[1]*conv3_op.output_shape_[2],1);

    // read rows,cols , computed bias, scale: scale = weight/var, bias = bias - mean*scale
    std::shared_ptr<Gate<ShrType>> out;
    out = circuit.conv2DTrunc(xIn,kernel1,conv1_op); //conv1
    out = circuit.elementMultiply(out,bn1_scale); // bn1
    out = circuit.add(out,bn1_bias);
    out = circuit.relu(out); // relu
    //conv2
    out = circuit.conv2DTrunc(out,kernel2,conv2_op);
    //bn2
    out = circuit.elementMultiply(out,bn2_scale);
    out = circuit.add(out,bn2_bias);
    // add shortcut
//    out = circuit.add(out,xIn);
    std::shared_ptr<Gate<Spdz2kShare64>> shortcut = circuit.conv2DTrunc(xIn,kernel3,conv3_op);
    shortcut = circuit.elementMultiply(shortcut,bn3_scale);
    shortcut = circuit.add(shortcut,bn3_bias);
    out = circuit.add(out, shortcut);
    // relu
    out = circuit.relu(out);

    return out;
}

int main() {
    auto path = std::filesystem::temp_directory_path();

    Party<Spdz2kShare64> party(1, 2, (path / "1.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);
    std::shared_ptr<Gate<Spdz2kShare64>> out;
    auto input_image = circuit.input(0,conv1_op.output_shape_[1],conv1_op.output_shape_[2]);
    out = circuit.addConstant(input_image,0);
    for (int i = 0; i < layers; ++i) {
        out = ResBlock1(circuit,out);
    }
    circuit.addEndpoint(out);
    circuit.readOfflineFromFile();
    circuit.runOnline();

    return 0;
}
