#ifndef MALICIOUS_PPML_ARGMAXCONFIG_H
#define MALICIOUS_PPML_ARGMAXCONFIG_H
#include "utils/linear_algebra.h"
#include "utils/tensor.h"
// using cifar10 32*32 dataset
const int times = 1;
const int rows = 224;
const int cols = 224;
const int layers = 4;
const int classes = 1000;
const int initchannels = 64;
// ImageNet 224*224*1000

std::vector<int> expansion1 = {1,2,2,2};
std::vector<int> expansion2 = {1,1,1,1};
std::vector<uint8_t> stride = {1,1,1,1};
std::vector <Conv2DOp> Conv1op(layers),Conv2op(layers),Conv3op(layers);
void InitializeConv(){
    uint32_t conv1_in_channels = initchannels; // initial input channels
    uint32_t conv1_out_channels;
    uint32_t conv2_in_channels;
    uint32_t conv2_out_channels;
    uint32_t conv3_in_channels;
    uint32_t conv3_out_channels;
    uint32_t parameter_size=0;
    for (int i = 0; i < layers; ++i) {
        // cifar-10
//        conv1_out_channels = expansion1[i]*conv1_in_channels;
//        Conv1op[i].kernel_shape_ = {conv1_out_channels,conv1_in_channels,3,3};
//        Conv1op[i].input_shape_ = {conv1_in_channels,rows,cols};
//        Conv1op[i].output_shape_ = {conv1_out_channels,rows,cols};
//        Conv1op[i].dilations_ ={1,1};
//        Conv1op[i].pads_ ={1,1,1,1};
//        Conv1op[i].strides_ ={stride[i],stride[i]};

        //ImageNet 224*224 * 1000
        conv1_out_channels = expansion1[i]*conv1_in_channels;
        Conv1op[i].kernel_shape_ = {conv1_out_channels,conv1_in_channels,7,7};
        Conv1op[i].input_shape_ = {conv1_in_channels,rows,cols};
        Conv1op[i].output_shape_ = {conv1_out_channels,rows,cols};
        Conv1op[i].dilations_ ={1,1};
        Conv1op[i].pads_ ={3,3,3,3};
        Conv1op[i].strides_ ={2,2}; //strides = 2

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
        Conv3op[i].kernel_shape_ = {conv3_out_channels,conv3_in_channels,1,1};
        Conv3op[i].input_shape_ = {conv3_in_channels,rows,cols};
        Conv3op[i].output_shape_ = {conv3_out_channels,rows,cols};
        Conv3op[i].dilations_ ={1,1};
        Conv3op[i].pads_ ={0,0,0,0};
        Conv3op[i].strides_ ={1,1};
        conv1_in_channels = conv2_out_channels;
    }
}
const Conv2DOp conv1_op = {.kernel_shape_ = {initchannels, 3, 3, 3},
        .input_shape_ = {3, rows, cols},
        .output_shape_ = {initchannels, rows, cols},
        .dilations_ = {1, 1},
        .pads_ = {1, 1, 1, 1},
        .strides_ = {1, 1}
};
#endif //MALICIOUS_PPML_ARGMAXCONFIG_H
