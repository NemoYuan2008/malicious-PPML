#include <iostream>
#include <filesystem>
#include <vector>

#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/Party.h"
#include "utils/ioHelper.h"
#include "resnet18Config.h"
#include "utils/fixedPoint.h"
#include <chrono>
#define NDEBUG
std::vector<double> generateRandIn(int rows, int cols){
    std::vector<double> ret(rows*cols);
    for (int i = 0; i < rows*cols; ++i) {
        ret[i] = rand();
    }
    return ret;
}

uint32_t parameter_size = 0;

template<typename ShrType>
std::shared_ptr<Gate<ShrType>> ResBlock0(int layer, Circuit<ShrType> &circuit, std::shared_ptr<Gate<ShrType>> xIn){
    auto kernel1 = circuit.input(0,Conv1op[layer].kernel_shape_[0]*Conv1op[layer].kernel_shape_[1],Conv1op[layer].kernel_shape_[2]*Conv1op[layer].kernel_shape_[3]);
    auto bn1_bias = circuit.input(0,Conv1op[layer].output_shape_[0]*Conv1op[layer].output_shape_[1]*Conv1op[layer].output_shape_[2],1);
    auto bn1_scale = circuit.input(0,Conv1op[layer].output_shape_[0]*Conv1op[layer].output_shape_[1]*Conv1op[layer].output_shape_[2],1);


    auto kernel2 = circuit.input(0,Conv2op[layer].kernel_shape_[0]*Conv2op[layer].kernel_shape_[1],Conv2op[layer].kernel_shape_[2]*Conv2op[layer].kernel_shape_[3]);
    auto bn2_bias = circuit.input(0,Conv2op[layer].output_shape_[0]*Conv2op[layer].output_shape_[1]*Conv2op[layer].output_shape_[2],1);
    auto bn2_scale = circuit.input(0,Conv2op[layer].output_shape_[0]*Conv2op[layer].output_shape_[1]*Conv2op[layer].output_shape_[2],1);

    auto kernel3 = circuit.input(0,Conv3op[layer].kernel_shape_[0]*Conv3op[layer].kernel_shape_[1],Conv3op[layer].kernel_shape_[2]*Conv3op[layer].kernel_shape_[3]);
    auto bn3_bias = circuit.input(0,Conv3op[layer].output_shape_[0]*Conv3op[layer].output_shape_[1]*Conv3op[layer].output_shape_[2],1);
    auto bn3_scale = circuit.input(0,Conv3op[layer].output_shape_[0]*Conv3op[layer].output_shape_[1]*Conv3op[layer].output_shape_[2],1);

    kernel1->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv1op[layer].kernel_shape_[0]*Conv1op[layer].kernel_shape_[1],Conv1op[layer].kernel_shape_[2]*Conv1op[layer].kernel_shape_[3])));

    bn1_bias->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv1op[layer].output_shape_[0]*Conv1op[layer].output_shape_[1]*Conv1op[layer].output_shape_[2],1)));

    bn1_scale->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv1op[layer].output_shape_[0]*Conv1op[layer].output_shape_[1]*Conv1op[layer].output_shape_[2],1)));

    kernel2->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv2op[layer].kernel_shape_[0]*Conv2op[layer].kernel_shape_[1],Conv2op[layer].kernel_shape_[2]*Conv2op[layer].kernel_shape_[3])));

    //read conv ops and bn parameters

    bn2_bias->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv2op[layer].output_shape_[0]*Conv2op[layer].output_shape_[1]*Conv2op[layer].output_shape_[2],1)));

    bn2_scale->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv2op[layer].output_shape_[0]*Conv2op[layer].output_shape_[1]*Conv2op[layer].output_shape_[2],1)));

    kernel3->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv3op[layer].kernel_shape_[0]*Conv3op[layer].kernel_shape_[1],Conv3op[layer].kernel_shape_[2]*Conv3op[layer].kernel_shape_[3])));

    bn3_bias->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv3op[layer].output_shape_[0]*Conv3op[layer].output_shape_[1]*Conv3op[layer].output_shape_[2],1)));

    bn3_scale->setInput(double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(Conv3op[layer].output_shape_[0]*Conv3op[layer].output_shape_[1]*Conv3op[layer].output_shape_[2],1)));
    // read rows,cols , computed bias, scale: scale = weight/var, bias = bias - mean*scale
    std::shared_ptr<Gate<ShrType>> out;
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
    std::shared_ptr<Gate<Spdz2kShare64>> shortcut = circuit.conv2DTrunc(xIn,kernel3,Conv3op[layer]);
    shortcut = circuit.elementMultiply(shortcut,bn3_scale);
    shortcut = circuit.add(shortcut,bn3_bias);
    out = circuit.add(out, shortcut);
    // relu
    out = circuit.relu(out);

    return out;
}


int main() {

    auto path = std::filesystem::temp_directory_path();
    InitializeConv();
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);
    std::shared_ptr<Gate<Spdz2kShare64>> out;
    auto start = std::chrono::high_resolution_clock::now();
    auto input_image = circuit.input(0,Conv1op[0].output_shape_[1],Conv1op[0].output_shape_[2]);
    auto input_x = double2fixVec<Spdz2kShare64::ClearType>(generateRandIn(rows,cols));
    input_image->setInput(input_x);
    out = circuit.addConstant(input_image,0);

    for (int i = 0; i < layers; ++i) {
        out = ResBlock0(i,circuit,out);
    }
    circuit.addEndpoint(out);

    circuit.readOfflineFromFile();
    circuit.runOnline();
    auto end = std::chrono::high_resolution_clock ::now();
    auto duration =std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout<<"running time: "<<duration.count()<<" s\n";
    return 0;
}
