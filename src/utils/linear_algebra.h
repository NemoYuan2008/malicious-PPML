#ifndef MALICIOUS_PPML_LINEAR_ALGEBRA_H
#define MALICIOUS_PPML_LINEAR_ALGEBRA_H


#include <vector>
#include <Eigen/Core>

// WARNING: ColMajor!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


template<typename T>
void matrixAdd(const T *x, const T *y, T *output, std::size_t dim) {
    using MatrixType = Eigen::Matrix<T, Eigen::Dynamic, 1>;
    Eigen::Map<MatrixType> matOutput(output, dim, 1);
    Eigen::Map<const MatrixType> matX(x, dim, 1);
    Eigen::Map<const MatrixType> matY(y, dim, 1);
    matOutput = matX + matY;
}


template<typename T>
inline
std::vector<T> matrixAdd(const std::vector<T> &x, const std::vector<T> &y) {
    std::vector<T> output(x.size());
    matrixAdd(x.data(), y.data(), output.data(), output.size());
    return output;
}


template<typename T>
void matrixSubtract(const T *x, const T *y, T *output, std::size_t dim) {
    using MatrixType = Eigen::Matrix<T, Eigen::Dynamic, 1>;
    Eigen::Map<MatrixType> matOutput(output, dim, 1);
    Eigen::Map<const MatrixType> matX(x, dim, 1);
    Eigen::Map<const MatrixType> matY(y, dim, 1);
    matOutput = matX - matY;
}


template<typename T>
inline
std::vector<T> matrixSubtract(const std::vector<T> &x, const std::vector<T> &y) {
    std::vector<T> output(x.size());
    matrixSubtract(x.data(), y.data(), output.data(), output.size());
    return output;
}


template<typename T>
void matrixMultiply(const T *A, const T *B, T *output, std::size_t dim_l, std::size_t dim_m, std::size_t dim_n) {
    using MatrixType = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
    Eigen::Map<MatrixType> matrix_output(output, dim_l, dim_n);
    Eigen::Map<const MatrixType> matrix_A(A, dim_l, dim_m);
    Eigen::Map<const MatrixType> matrix_B(B, dim_m, dim_n);
    matrix_output = matrix_A * matrix_B;
}

template<typename T>
std::vector<T> matrixMultiply(const std::vector<T> &A, const std::vector<T> &B,
                              std::size_t dim_l, std::size_t dim_m, std::size_t dim_n) {
//    assert(A.size() == dim_l * dim_m);
//    assert(B.size() == dim_m * dim_n);
    std::vector<T> output(dim_l * dim_n);
    matrixMultiply(A.data(), B.data(), output.data(), dim_l, dim_m, dim_n);
    return output;
}

#endif //MALICIOUS_PPML_LINEAR_ALGEBRA_H
