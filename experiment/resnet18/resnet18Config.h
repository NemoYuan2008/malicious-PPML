#ifndef MALICIOUS_PPML_ARGMAXCONFIG_H
#define MALICIOUS_PPML_ARGMAXCONFIG_H
#include "utils/linear_algebra.h"
#include "utils/tensor.h"
// using cifar10 32*32 dataset
const int times = 1;
const int rows = 32;
const int cols = 32;
const int layers = 4;
std::vector<int> expansion1 = {4,2,2,2};
std::vector<int> expansion2 = {2,2,2,2};
std::vector <Conv2DOp> Conv1op(layers),Conv2op(layers),Conv3op(layers);
void InitializeConv(){
    uint32_t conv1_in_channels = 1; // initial input channels
    uint32_t conv1_out_channels;
    uint32_t conv2_in_channels;
    uint32_t conv2_out_channels;
    uint32_t conv3_in_channels;
    uint32_t conv3_out_channels;
    uint32_t parameter_size=0;
    for (int i = 0; i < layers; ++i) {
        conv1_out_channels = expansion1[i]*conv1_in_channels;
        Conv1op[i].kernel_shape_ = {conv1_out_channels,conv1_in_channels,3,3};
        Conv1op[i].input_shape_ = {conv1_in_channels,rows,cols};
        Conv1op[i].output_shape_ = {conv1_out_channels,rows,cols};
        Conv1op[i].dilations_ ={1,1};
        Conv1op[i].pads_ ={1,1,1,1};
        Conv1op[i].strides_ ={1,1};

        conv2_in_channels = conv1_out_channels;
        conv2_out_channels = expansion2[i]*conv2_in_channels;
        Conv2op[i].kernel_shape_ = {conv2_out_channels,conv2_in_channels,3,3};
        Conv2op[i].input_shape_ = {conv2_in_channels,rows,cols};
        Conv2op[i].output_shape_ = {conv2_out_channels,rows,cols};
        Conv2op[i].dilations_ ={1,1};
        Conv2op[i].pads_ ={1,1,1,1};
        Conv2op[i].strides_ ={1,1};

        conv3_in_channels = conv1_in_channels;
        conv3_out_channels = expansion1[i]*expansion2[i]*conv1_in_channels;
        Conv3op[i].kernel_shape_ = {conv3_out_channels,conv3_in_channels,3,3};
        Conv3op[i].input_shape_ = {conv3_in_channels,rows,cols};
        Conv3op[i].output_shape_ = {conv3_out_channels,rows,cols};
        Conv3op[i].dilations_ ={1,1};
        Conv3op[i].pads_ ={1,1,1,1};
        Conv3op[i].strides_ ={1,1};
        conv1_in_channels = conv2_out_channels;
    }
}
#endif //MALICIOUS_PPML_ARGMAXCONFIG_H
