#ifndef MALICIOUS_PPML_AVGPOOLCONFIG_H
#define MALICIOUS_PPML_AVGPOOLCONFIG_H


#include "utils/tensor.h"


const int times = 100;
const int rows = 32;
const int cols = 32;
const int kernel_size = 5;
const int stride = 1;

const MaxPoolOp op = {
        .input_shape_ = {1, rows, cols},
        .output_shape_ = {1, (rows+1-kernel_size)/stride, (cols+1-kernel_size)/stride},
        .kernel_shape_ = {kernel_size, kernel_size},
        .strides_ = {1, 1},
};


#endif //MALICIOUS_PPML_AVGPOOLCONFIG_H
