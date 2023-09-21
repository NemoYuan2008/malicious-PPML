#ifndef MALICIOUS_PPML_MULTIPLICATIONCONFIG_H
#define MALICIOUS_PPML_MULTIPLICATIONCONFIG_H


#include "utils/linear_algebra.h"
#include "utils/tensor.h"

const int times = 10;
const int rows = 32;
const int cols = 32;
const int in_channels = 3;
const int out_channels = 1;
const int kernel_size = 3;
const int pad = 1;
const int stride = 1;

const int layers = 4;
const Conv2DOp conv_op = {.kernel_shape_ = {1, in_channels, kernel_size, kernel_size},
        .input_shape_ = {in_channels, rows, cols},
        .output_shape_ = {out_channels, (rows+2*pad+1-kernel_size)/stride, (cols+2*pad+1-kernel_size)/stride},
        .dilations_ = {1, 1},
        .pads_ = {pad, pad, pad, pad},
        .strides_ = {stride, stride}
};

#endif //MALICIOUS_PPML_MULTIPLICATIONCONFIG_H
