#ifndef MALICIOUS_PPML_LINEAR_ALGEBRA_H
#define MALICIOUS_PPML_LINEAR_ALGEBRA_H


#include <vector>
#include <Eigen/Core>
#include <unsupported/Eigen/CXX11/Tensor>

#include "tensor.h"

// WARNING: RowMajor!!!!!!!


//output = x + y
template<typename T>
inline
void matrixAdd(const T *x, const T *y, T *output, std::size_t dim) {
    using MatrixType = Eigen::Matrix<T, 1, Eigen::Dynamic, Eigen::RowMajor>;
    Eigen::Map<MatrixType> matOutput(output, 1, dim);
    Eigen::Map<const MatrixType> matX(x, 1, dim);
    Eigen::Map<const MatrixType> matY(y, 1, dim);
    matOutput = matX + matY;
}


//return x + y
template<typename T>
inline
std::vector<T> matrixAdd(const std::vector<T> &x, const std::vector<T> &y) {
    std::vector<T> output(x.size());
    matrixAdd(x.data(), y.data(), output.data(), output.size());
    return output;
}


//x += y
template<typename T>
inline
void matrixAddAssign(T *x, const T *y, std::size_t dim) {
    using MatrixType = Eigen::Matrix<T, 1, Eigen::Dynamic, Eigen::RowMajor>;
    Eigen::Map<MatrixType> matX(x, 1, dim);
    Eigen::Map<const MatrixType> matY(y, 1, dim);
    matX += matY;
}


//x += y
template<typename T>
inline
void matrixAddAssign(std::vector<T> &x, const std::vector<T> &y) {
    matrixAddAssign(x.data(), y.data(), x.size());
}


//output = x - y
template<typename T>
inline
void matrixSubtract(const T *x, const T *y, T *output, std::size_t dim) {
    using MatrixType = Eigen::Matrix<T, 1, Eigen::Dynamic, Eigen::RowMajor>;
    Eigen::Map<MatrixType> matOutput(output, 1, dim);
    Eigen::Map<const MatrixType> matX(x, 1, dim);
    Eigen::Map<const MatrixType> matY(y, 1, dim);
    matOutput = matX - matY;
}


//return x - y
template<typename T>
inline
std::vector<T> matrixSubtract(const std::vector<T> &x, const std::vector<T> &y) {
    std::vector<T> output(x.size());
    matrixSubtract(x.data(), y.data(), output.data(), output.size());
    return output;
}


//x -= y
template<typename T>
inline
void matrixSubtractAssign(T *x, const T *y, std::size_t dim) {
    using MatrixType = Eigen::Matrix<T, 1, Eigen::Dynamic, Eigen::RowMajor>;
    Eigen::Map<MatrixType> matX(x, 1, dim);
    Eigen::Map<const MatrixType> matY(y, 1, dim);
    matX -= matY;
}


//x -= y
template<typename T>
inline
void matrixSubtractAssign(std::vector<T> &x, const std::vector<T> &y) {
    matrixSubtractAssign(x.data(), y.data(), x.size());
}


template<typename T>
inline
void matrixMultiply(const T *A, const T *B, T *output, std::size_t dim_l, std::size_t dim_m, std::size_t dim_n) {
    using MatrixType = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
    Eigen::Map<MatrixType> matrix_output(output, dim_l, dim_n);
    Eigen::Map<const MatrixType> matrix_A(A, dim_l, dim_m);
    Eigen::Map<const MatrixType> matrix_B(B, dim_m, dim_n);
    matrix_output = matrix_A * matrix_B;
}

template<typename T>
inline
std::vector<T> matrixMultiply(const std::vector<T> &A, const std::vector<T> &B,
                              std::size_t dim_l, std::size_t dim_m, std::size_t dim_n) {
//    assert(A.size() == dim_l * dim_m);
//    assert(B.size() == dim_m * dim_n);
    std::vector<T> output(dim_l * dim_n);
    matrixMultiply(A.data(), B.data(), output.data(), dim_l, dim_m, dim_n);
    return output;
}


template<typename T>
void convolution(const T *input_buffer, const T *kernel_buffer, T *output_buffer, const Conv2DOp &conv_op) {
    using TensorType3 = Eigen::Tensor<T, 3, Eigen::RowMajor>;
    using CTensorType3 = Eigen::Tensor<const T, 3, Eigen::RowMajor>;
    using CTensorType4 = Eigen::Tensor<const T, 4, Eigen::RowMajor>;

    assert(conv_op.verify());
    const auto &output_shape = conv_op.output_shape_;
    const auto &input_shape = conv_op.input_shape_;
    const auto &kernel_shape = conv_op.kernel_shape_;

    Eigen::TensorMap<CTensorType3> input(input_buffer, input_shape[0], input_shape[1],
                                         input_shape[2]);
    Eigen::TensorMap<CTensorType4> kernel(kernel_buffer, kernel_shape[0], kernel_shape[1],
                                          kernel_shape[2], kernel_shape[3]);
    Eigen::TensorMap<TensorType3> output(output_buffer, output_shape[0], output_shape[1],
                                         output_shape[2]);
    const std::array<Eigen::Index, 2> kernel_matrix_dimensions = {
            static_cast<Eigen::Index>(kernel_shape[1] * kernel_shape[2] * kernel_shape[3]),
            static_cast<Eigen::Index>(kernel_shape[0])};
    const std::array<Eigen::Index, 2> input_matrix_dimensions = {
            static_cast<Eigen::Index>(output_shape[1] * output_shape[2]),
            static_cast<Eigen::Index>(kernel_shape[1] * kernel_shape[2] * kernel_shape[3])};

    auto kernel_matrix =
            kernel.shuffle(std::array<int, 4>{3, 2, 1, 0}).reshape(kernel_matrix_dimensions);

    auto input_matrix =
            input.shuffle(Eigen::array<Eigen::Index, 3>{2, 1, 0})
                    .extract_image_patches(kernel_shape[2], kernel_shape[3], conv_op.strides_[0],
                                           conv_op.strides_[1], conv_op.dilations_[0], conv_op.dilations_[1],
                                           1, 1, conv_op.pads_[0], conv_op.pads_[2], conv_op.pads_[1],
                                           conv_op.pads_[3], 0)
                    .reshape(input_matrix_dimensions);

    const std::array<Eigen::IndexPair<Eigen::Index>, 1> contraction_dimensions = {
            Eigen::IndexPair<Eigen::Index>(1, 0)};
    auto output_matrix =
            kernel_matrix.shuffle(std::array<Eigen::Index, 2>{1, 0})
                    .contract(input_matrix.shuffle(std::array<Eigen::Index, 2>{1, 0}), contraction_dimensions)
                    .shuffle(std::array<Eigen::Index, 2>{1, 0});

    const std::array<Eigen::Index, 3> rev_output_dimensions = {
            output.dimension(2), output.dimension(1), output.dimension(0)};
    output =
            output_matrix.reshape(rev_output_dimensions).shuffle(Eigen::array<Eigen::Index, 3>{2, 1, 0});
}

template<typename T>
std::vector<T> convolution(const std::vector<T> &input_buffer,
                           const std::vector<T> &kernel_buffer, const Conv2DOp &conv_op) {
    assert(conv_op.verify());
    assert(input_buffer.size() == conv_op.compute_input_size());
    assert(kernel_buffer.size() == conv_op.compute_kernel_size());
    std::vector<T> output_buffer(conv_op.compute_output_size());
    convolution(input_buffer.data(), kernel_buffer.data(), output_buffer.data(), conv_op);
    return output_buffer;
}


template <typename T>
void sumPool(const T* input, T* output, const MaxPoolOp& op) {
    assert(op.verify());
    using TensorType3C = Eigen::Tensor<const T, 3, Eigen::RowMajor>;
    using TensorType3 = Eigen::Tensor<T, 3, Eigen::RowMajor>;
    const auto in_channels = static_cast<Eigen::Index>(op.input_shape_[0]);
    const auto in_rows = static_cast<Eigen::Index>(op.input_shape_[1]);
    const auto in_columns = static_cast<Eigen::Index>(op.input_shape_[2]);
    const auto out_channels = static_cast<Eigen::Index>(op.output_shape_[0]);
    const auto out_rows = static_cast<Eigen::Index>(op.output_shape_[1]);
    const auto out_columns = static_cast<Eigen::Index>(op.output_shape_[2]);
    const auto kernel_rows = static_cast<Eigen::Index>(op.kernel_shape_[0]);
    const auto kernel_columns = static_cast<Eigen::Index>(op.kernel_shape_[1]);
    const auto stride_rows = static_cast<Eigen::Index>(op.strides_[0]);
    const auto stride_columns = static_cast<Eigen::Index>(op.strides_[1]);

    Eigen::TensorMap<TensorType3C> tensor_src(input, in_channels, in_rows, in_columns);
    Eigen::TensorMap<TensorType3> tensor_dst(output, out_channels, out_rows, out_columns);

    tensor_dst = tensor_src.shuffle(Eigen::array<Eigen::Index, 3>{2, 1, 0})
            .extract_image_patches(kernel_rows, kernel_columns, stride_rows, stride_columns,
                                   1, 1, 1, 1, 0, 0, 0, 0, T(0))
            .sum(Eigen::array<Eigen::Index, 2>{1, 2})
            .reshape(Eigen::array<Eigen::Index, 3>{out_columns, out_rows, out_channels})
            .shuffle(Eigen::array<Eigen::Index, 3>{2, 1, 0});
}


template<typename T>
inline
std::vector<T> sumPool(const std::vector<T> &inputBuf, const MaxPoolOp& op) {
    std::vector<T> outputBuf(op.compute_output_size());
    sumPool(inputBuf.data(), outputBuf.data(), op);
    return outputBuf;
}

#endif //MALICIOUS_PPML_LINEAR_ALGEBRA_H
