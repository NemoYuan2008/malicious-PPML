#ifndef MALICIOUS_PPML_AVGPOOLCONFIG_H
#define MALICIOUS_PPML_AVGPOOLCONFIG_H


#include "utils/tensor.h"


const int times = 1;
const int rows = 3;
const int cols = 3;


const MaxPoolOp op = {
        .input_shape_ = {1, 3, 3},
        .output_shape_ = {1, 2, 2},
        .kernel_shape_ = {2, 2},
        .strides_ = {1, 1},
};


#endif //MALICIOUS_PPML_AVGPOOLCONFIG_H
