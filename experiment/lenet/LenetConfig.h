//
// Created by zyx on 23-6-23.
//
#ifndef MALICIOUS_PPML_MULTIPLICATIONCONFIG_H
#define MALICIOUS_PPML_MULTIPLICATIONCONFIG_H

// mnist: rows = cols = 28;
// cifar10: rows = cols = 32;

const int times = 1;
const int rows = 32;
const int cols = 32;
const int features = rows * cols;
const int batch_size = 10;
const int classes = 10;
const int conv_kernel = 5;
const int avg_kernel = 2;
const int linear1 = 120;
const int linear2 = 84;

const Conv2DOp conv1_op = {.kernel_shape_ = {6, 1, conv_kernel, conv_kernel},
        .input_shape_ = {1, rows, cols},
        .output_shape_ = {6, rows - conv_kernel + 1, cols - conv_kernel + 1},
        .dilations_ = {1, 1},
        .pads_ = {0, 0, 0, 0},
        .strides_ = {1, 1}
};

const MaxPoolOp avg_op1 = {
        .input_shape_ = {6, rows - conv_kernel + 1, rows - conv_kernel + 1},
        .output_shape_ = {6,(rows - conv_kernel + 1) / 2, (rows - conv_kernel + 1) / 2},
        .kernel_shape_ = {avg_kernel, avg_kernel},
        .strides_ = {2, 2},
};

const Conv2DOp conv2_op = {.kernel_shape_ = {16, 6, conv_kernel, conv_kernel},
        .input_shape_ = {6, (rows - conv_kernel + 1) / 2, (rows - conv_kernel + 1) / 2},
        .output_shape_ = {16, (rows - conv_kernel + 1) / 2 - conv_kernel + 1, (rows - conv_kernel + 1) / 2 - conv_kernel + 1},
        .dilations_ = {1, 1},
        .pads_ = {0, 0, 0, 0},
        .strides_ = {1, 1}
};

const MaxPoolOp avg_op2 = {
        .input_shape_ = {16, (rows - conv_kernel + 1) / 2 - conv_kernel + 1, (rows - conv_kernel + 1) / 2 - conv_kernel + 1},
        .output_shape_ = {16, ((rows - conv_kernel + 1) / 2 - conv_kernel + 1) / 2, ((rows - conv_kernel + 1) / 2 - conv_kernel + 1) / 2},
        .kernel_shape_ = {avg_kernel, avg_kernel},
        .strides_ = {2, 2},
};

#endif //MALICIOUS_PPML_MULTIPLICATIONCONFIG_H
