#ifndef MALICIOUS_PPML_MULTIPLICATIONCONFIG_H
#define MALICIOUS_PPML_MULTIPLICATIONCONFIG_H


#include "utils/linear_algebra.h"
#include "utils/tensor.h"

const int times = 100;
const int rows = 1;
const int cols = 10000;

const Conv2DOp conv_op = {.kernel_shape_ = {1, 2, 3, 3},
        .input_shape_ = {2, 3, 3},
        .output_shape_ = {1, 1, 1},
        .dilations_ = {1, 1},
        .pads_ = {0, 0, 0, 0},
        .strides_ = {1, 1}
};

#endif //MALICIOUS_PPML_MULTIPLICATIONCONFIG_H
