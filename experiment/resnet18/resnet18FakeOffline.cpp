#include <iostream>
#include <fstream>
#include <filesystem>

#include "share/Spdz2kShare.h"
#include "offline/FakeOffline.h"
#include "offline/FakeGate.h"
#include "offline/FakeCircuit.h"

#include "resnet18Config.h"
#include <chrono>
uint32_t parameter_size = 0;
template<typename ShrType, int N>
std::shared_ptr<FakeGate<ShrType, N>> ResBlock(int layer, FakeCircuit<Spdz2kShare64, 2> &circuit, std::shared_ptr<FakeGate<ShrType, N>> xIn){

    auto kernel1 = circuit.input(0,Conv1op[layer].kernel_shape_[0]*Conv1op[layer].kernel_shape_[1],Conv1op[layer].kernel_shape_[2]*Conv1op[layer].kernel_shape_[3]);
    auto bn1_bias = circuit.input(0,Conv1op[layer].output_shape_[0]*Conv1op[layer].output_shape_[1]*Conv1op[layer].output_shape_[2],1);
    auto bn1_scale = circuit.input(0,Conv1op[layer].output_shape_[0]*Conv1op[layer].output_shape_[1]*Conv1op[layer].output_shape_[2],1);

    parameter_size += Conv1op[layer].kernel_shape_[0]*Conv1op[layer].kernel_shape_[1]*Conv1op[layer].kernel_shape_[2]*Conv1op[layer].kernel_shape_[3];
    parameter_size += Conv1op[layer].output_shape_[0]*Conv1op[layer].output_shape_[1]*Conv1op[layer].output_shape_[2];
    parameter_size += Conv1op[layer].output_shape_[0]*Conv1op[layer].output_shape_[1]*Conv1op[layer].output_shape_[2];

    auto kernel2 = circuit.input(0,Conv2op[layer].kernel_shape_[0]*Conv2op[layer].kernel_shape_[1],Conv2op[layer].kernel_shape_[2]*Conv2op[layer].kernel_shape_[3]);
    auto bn2_bias = circuit.input(0,Conv2op[layer].output_shape_[0]*Conv2op[layer].output_shape_[1]*Conv2op[layer].output_shape_[2],1);
    auto bn2_scale = circuit.input(0,Conv2op[layer].output_shape_[0]*Conv2op[layer].output_shape_[1]*Conv2op[layer].output_shape_[2],1);

    parameter_size += Conv2op[layer].kernel_shape_[0]*Conv2op[layer].kernel_shape_[1]*Conv2op[layer].kernel_shape_[2]*Conv2op[layer].kernel_shape_[3];
    parameter_size += Conv2op[layer].output_shape_[0]*Conv2op[layer].output_shape_[1]*Conv2op[layer].output_shape_[2];
    parameter_size += Conv2op[layer].output_shape_[0]*Conv2op[layer].output_shape_[1]*Conv2op[layer].output_shape_[2];

    auto kernel3 = circuit.input(0,Conv3op[layer].kernel_shape_[0]*Conv3op[layer].kernel_shape_[1],Conv3op[layer].kernel_shape_[2]*Conv3op[layer].kernel_shape_[3]);
    auto bn3_bias = circuit.input(0,Conv3op[layer].output_shape_[0]*Conv3op[layer].output_shape_[1]*Conv3op[layer].output_shape_[2],1);
    auto bn3_scale = circuit.input(0,Conv3op[layer].output_shape_[0]*Conv3op[layer].output_shape_[1]*Conv3op[layer].output_shape_[2],1);

    parameter_size += Conv3op[layer].kernel_shape_[0]*Conv3op[layer].kernel_shape_[1]*Conv3op[layer].kernel_shape_[2]*Conv3op[layer].kernel_shape_[3];
    parameter_size += Conv3op[layer].output_shape_[0]*Conv3op[layer].output_shape_[1]*Conv3op[layer].output_shape_[2];
    parameter_size += Conv3op[layer].output_shape_[0]*Conv3op[layer].output_shape_[1]*Conv3op[layer].output_shape_[2];
    // read rows,cols , computed bias, scale: scale = weight/var, bias = bias - mean*scale
    std::shared_ptr<FakeGate<ShrType, N>> out;
    out = circuit.conv2DTrunc(xIn,kernel1,Conv1op[layer]); //conv1

    out = circuit.elementMultiply(out,bn1_scale); // bn1
    out = circuit.add(out,bn1_bias);
    out = circuit.relu(out); // relu
    //conv2
    out = circuit.conv2DTrunc(out,kernel2,Conv2op[layer]);
    //bn2
    out = circuit.elementMultiply(out,bn2_scale);
    out = circuit.add(out,bn2_bias);
    // add shortcut
//    out = circuit.add(out,xIn);
    std::shared_ptr<FakeGate<Spdz2kShare64, 2>> shortcut = circuit.conv2DTrunc(xIn,kernel3,Conv3op[layer]);

    shortcut = circuit.elementMultiply(shortcut,bn3_scale);
    shortcut = circuit.add(shortcut,bn3_bias);
    out = circuit.add(out, shortcut);
    // relu
    out = circuit.relu(out);

    return out;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto path = std::filesystem::temp_directory_path();
    std::cout << path << '\n';

    std::array<std::ofstream, 2> files{
            std::ofstream(path / "0.txt"),
            std::ofstream(path / "1.txt")
    };

    FakeOffline<64, 64, 2> offline(files);
    FakeCircuit<Spdz2kShare64, 2> circuit(files, offline);
    std::shared_ptr<FakeGate<Spdz2kShare64, 2>> out;
    
    InitializeConv();
    
    auto input_image = circuit.input(0,3*rows,cols);
    out = circuit.addConstant(input_image,0);
    auto kernel1 = circuit.input(0,conv1_op.kernel_shape_[0]*conv1_op.kernel_shape_[1],conv1_op.kernel_shape_[2]*conv1_op.kernel_shape_[3]);
    auto bn1_bias = circuit.input(0,conv1_op.output_shape_[0]*conv1_op.output_shape_[1]*conv1_op.output_shape_[2],1);
    auto bn1_scale = circuit.input(0,conv1_op.output_shape_[0]*conv1_op.output_shape_[1]*conv1_op.output_shape_[2],1);

    out = circuit.conv2DTrunc(out,kernel1,conv1_op);
    out = circuit.elementMultiply(out,bn1_scale);
    out = circuit.add(out,bn1_bias);
    for (int i = 0; i < layers; ++i) {
        out = ResBlock(i,circuit,out);
    }
    //FC layer
    auto fc = circuit.input(0,classes,out->getDimCol()*out->getDimRow());
    out = circuit.multiplyTrunc(fc,out);
    circuit.addEndpoint(out);

    circuit.runOffline();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout<< "running time: "<<duration.count()<<" s\n";
    std::cout<< "number of parameter: " << parameter_size <<"\n";

    return 0;
}
