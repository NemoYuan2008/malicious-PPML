//
// Created by zyx on 23-6-23.
//
#ifndef MALICIOUS_PPML_MULTIPLICATIONCONFIG_H
#define MALICIOUS_PPML_MULTIPLICATIONCONFIG_H


const int times = 1;
const int rows = 28;
const int cols = 28;
const int features = rows * cols;
const int batch_size = 1;
const int classes = 10;
const int conv_kernel = 5;
const int avg_kernel = 2;

const Conv2DOp conv1_op = {.kernel_shape_ = {6, 1, 5, 5},
        .input_shape_ = {1, 28, 28},
        .output_shape_ = {6, 24, 24},
        .dilations_ = {1, 1},
        .pads_ = {0, 0, 0, 0},
        .strides_ = {1, 1}
};

const MaxPoolOp avg_op1 = {
        .input_shape_ = {6, 24, 24},
        .output_shape_ = {6, 12, 12},
        .kernel_shape_ = {avg_kernel, avg_kernel},
        .strides_ = {2, 2},
};

const Conv2DOp conv2_op = {.kernel_shape_ = {16, 6, conv_kernel, conv_kernel},
        .input_shape_ = {6, 12, 12},
        .output_shape_ = {16, 8, 8},
        .dilations_ = {1, 1},
        .pads_ = {0, 0, 0, 0},
        .strides_ = {1, 1}
};

const MaxPoolOp avg_op2 = {
        .input_shape_ = {16, 8, 8},
        .output_shape_ = {16, 4, 4},
        .kernel_shape_ = {avg_kernel, avg_kernel},
        .strides_ = {2, 2},
};

#endif //MALICIOUS_PPML_MULTIPLICATIONCONFIG_H
