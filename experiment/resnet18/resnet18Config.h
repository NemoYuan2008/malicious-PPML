#ifndef MALICIOUS_PPML_ARGMAXCONFIG_H
#define MALICIOUS_PPML_ARGMAXCONFIG_H
#include "utils/linear_algebra.h"
#include "utils/tensor.h"

const int times = 1;
const int rows = 32;
const int cols = 32;
const int layers = 4;
const Conv2DOp conv1_op = {.kernel_shape_ = {4, 1, 3, 3},
        .input_shape_ = {1, 32, 32},
        .output_shape_ = {4, 32, 32},
        .dilations_ = {1, 1},
        .pads_ = {1, 1, 1, 1},
        .strides_ = {1, 1}
};

const Conv2DOp conv2_op = {.kernel_shape_ = {1, 4, 3, 3},
        .input_shape_ = {4, 32, 32},
        .output_shape_ = {1, 32, 32},
        .dilations_ = {1, 1},
        .pads_ = {1, 1, 1, 1},
        .strides_ = {1, 1}
};

const Conv2DOp conv3_op = {.kernel_shape_ = {1, 1, 3, 3},
        .input_shape_ = {1, 32, 32},
        .output_shape_ = {1, 32, 32},
        .dilations_ = {1, 1},
        .pads_ = {1, 1, 1, 1},
        .strides_ = {1, 1}
};



#endif //MALICIOUS_PPML_ARGMAXCONFIG_H
