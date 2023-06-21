#ifndef MALICIOUS_PPML_TENSOR_H
#define MALICIOUS_PPML_TENSOR_H


#include <array>
#include <utility>
#include <cassert>


struct TensorDimensions {
    std::size_t batch_size_;
    std::size_t num_channels_;
    std::size_t height_;
    std::size_t width_;

    std::size_t get_num_dimensions() const noexcept { return 4; }

    std::size_t get_data_size() const noexcept {
        return batch_size_ * num_channels_ * height_ * width_;
    }

    bool operator==(const TensorDimensions &o) const noexcept {
        return batch_size_ == o.batch_size_ && num_channels_ == o.num_channels_ &&
               height_ == o.height_ && width_ == o.width_;
    }

    bool operator!=(const TensorDimensions &o) const noexcept { return !(*this == o); }
};


struct Conv2DOp {
    std::array<std::size_t, 4> kernel_shape_;
    std::array<std::size_t, 3> input_shape_;
    std::array<std::size_t, 3> output_shape_;

    std::array<std::size_t, 2> dilations_;
    std::array<std::size_t, 4> pads_;
    std::array<std::size_t, 2> strides_;

    bool verify() const noexcept;

    std::array<std::size_t, 3> compute_output_shape() const noexcept;

    std::size_t compute_output_size() const noexcept;

    std::size_t compute_input_size() const noexcept;

    std::size_t compute_kernel_size() const noexcept;

    std::size_t compute_bias_size() const noexcept;

    std::pair<std::size_t, std::size_t> compute_input_matrix_shape() const noexcept;

    std::pair<std::size_t, std::size_t> compute_kernel_matrix_shape() const noexcept;

    std::pair<std::size_t, std::size_t> compute_output_matrix_shape() const noexcept;

    TensorDimensions get_input_tensor_dims() const noexcept;

    TensorDimensions get_kernel_tensor_dims() const noexcept;

    TensorDimensions get_output_tensor_dims() const noexcept;

    bool operator==(const Conv2DOp &) const noexcept;
};


bool Conv2DOp::verify() const noexcept {
    bool result = true;
    result = result && (output_shape_ == compute_output_shape());
    result = result && strides_[0] > 0 && strides_[1] > 0;
    // maybe add more checks here
    return result;
}

std::array<std::size_t, 3> Conv2DOp::compute_output_shape() const noexcept {
    const auto compute_output_dimension = [](auto input_size, auto kernel_size, auto padding_begin,
                                             auto padding_end, auto stride) {
        assert(stride != 0);
        return (input_size - kernel_size + padding_begin + padding_end + stride) / stride;
    };

    std::array<std::size_t, 3> output_shape;
    output_shape[0] = kernel_shape_[0];
    output_shape[1] =
            compute_output_dimension(input_shape_[1], kernel_shape_[2], pads_[0], pads_[2], strides_[0]);
    output_shape[2] =
            compute_output_dimension(input_shape_[2], kernel_shape_[3], pads_[1], pads_[3], strides_[1]);
    return output_shape;
}

std::size_t Conv2DOp::compute_output_size() const noexcept {
    assert(verify());
    auto output_shape = compute_output_shape();
    return output_shape[0] * output_shape[1] * output_shape[2];
}

std::size_t Conv2DOp::compute_input_size() const noexcept {
    assert(verify());
    return input_shape_[0] * input_shape_[1] * input_shape_[2];
}

std::size_t Conv2DOp::compute_kernel_size() const noexcept {
    assert(verify());
    return kernel_shape_[0] * kernel_shape_[1] * kernel_shape_[2] * kernel_shape_[3];
}

std::size_t Conv2DOp::compute_bias_size() const noexcept {
    assert(verify());
    return kernel_shape_[0];
}

std::pair<std::size_t, std::size_t> Conv2DOp::compute_input_matrix_shape() const noexcept {
    assert(verify());
    std::size_t num_rows = kernel_shape_[1] * kernel_shape_[2] * kernel_shape_[3];
    std::size_t num_columns = output_shape_[1] * output_shape_[2];
    return {num_rows, num_columns};
}

std::pair<std::size_t, std::size_t> Conv2DOp::compute_kernel_matrix_shape() const noexcept {
    assert(verify());
    std::size_t num_rows = kernel_shape_[0];
    std::size_t num_columns = kernel_shape_[1] * kernel_shape_[2] * kernel_shape_[3];
    return {num_rows, num_columns};
}

std::pair<std::size_t, std::size_t> Conv2DOp::compute_output_matrix_shape() const noexcept {
    assert(verify());
    std::size_t num_rows = kernel_shape_[0];
    std::size_t num_columns = output_shape_[1] * output_shape_[2];
    return {num_rows, num_columns};
}

TensorDimensions Conv2DOp::get_input_tensor_dims() const noexcept {
    assert(verify());
    return {.batch_size_ = 1,
            .num_channels_ = input_shape_[0],
            .height_ = input_shape_[1],
            .width_ = input_shape_[2]};
}

TensorDimensions Conv2DOp::get_kernel_tensor_dims() const noexcept {
    assert(verify());
    return {.batch_size_ = kernel_shape_[0],
            .num_channels_ = kernel_shape_[1],
            .height_ = kernel_shape_[2],
            .width_ = kernel_shape_[3]};
}

TensorDimensions Conv2DOp::get_output_tensor_dims() const noexcept {
    assert(verify());
    return {.batch_size_ = 1,
            .num_channels_ = output_shape_[0],
            .height_ = output_shape_[1],
            .width_ = output_shape_[2]};
}

bool Conv2DOp::operator==(const Conv2DOp &other) const noexcept {
    assert(verify());
    assert(other.verify());
    bool result = true;
    result = result && kernel_shape_ == other.kernel_shape_;
    result = result && input_shape_ == other.input_shape_;
    result = result && output_shape_ == other.output_shape_;
    result = result && dilations_ == other.dilations_;
    result = result && pads_ == other.pads_;
    result = result && strides_ == other.strides_;
    return result;
}



struct MaxPoolOp {
    std::array<std::size_t, 3> input_shape_;
    std::array<std::size_t, 3> output_shape_;

    std::array<std::size_t, 2> kernel_shape_;
    std::array<std::size_t, 2> strides_;

    bool verify() const noexcept;
    std::array<std::size_t, 3> compute_output_shape() const noexcept;
    std::size_t compute_kernel_size() const noexcept;
    std::size_t compute_input_size() const noexcept;
    std::size_t compute_output_size() const noexcept;
    TensorDimensions get_input_tensor_dims() const noexcept;
    TensorDimensions get_output_tensor_dims() const noexcept;
};


bool MaxPoolOp::verify() const noexcept {
    bool result = true;
    result = result && (output_shape_ == compute_output_shape());
    result = result && strides_[0] > 0 && strides_[1] > 0;
    result = kernel_shape_[0] <= input_shape_[1] && kernel_shape_[1] <= input_shape_[2];
    // maybe add more checks here
    return result;
}

std::array<std::size_t, 3> MaxPoolOp::compute_output_shape() const noexcept {
    const auto compute_output_dimension = [](auto input_size, auto kernel_size, auto stride) {
        assert(stride != 0);
        return (input_size - kernel_size + stride) / stride;
    };

    std::array<std::size_t, 3> output_shape;
    output_shape[0] = input_shape_[0];
    output_shape[1] =
            compute_output_dimension(input_shape_[1], kernel_shape_[2], strides_[0]);
    output_shape[2] =
            compute_output_dimension(input_shape_[2], kernel_shape_[3], strides_[1]);
    return output_shape;
}

std::size_t MaxPoolOp::compute_kernel_size() const noexcept {
    assert(verify());
    return kernel_shape_[0] * kernel_shape_[1];
}

std::size_t MaxPoolOp::compute_input_size() const noexcept {
    assert(verify());
    return input_shape_[0] * input_shape_[1] * input_shape_[2];
}

std::size_t MaxPoolOp::compute_output_size() const noexcept {
    assert(verify());
    return output_shape_[0] * output_shape_[1] * output_shape_[2];
}

TensorDimensions MaxPoolOp::get_input_tensor_dims() const noexcept {
    assert(verify());
    return {.batch_size_ = 1,
            .num_channels_ = input_shape_[0],
            .height_ = input_shape_[1],
            .width_ = input_shape_[2]};
}

TensorDimensions MaxPoolOp::get_output_tensor_dims() const noexcept {
    assert(verify());
    return {.batch_size_ = 1,
            .num_channels_ = output_shape_[0],
            .height_ = output_shape_[1],
            .width_ = output_shape_[2]};
}


#endif //MALICIOUS_PPML_TENSOR_H
