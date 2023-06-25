#include <iostream>
#include <filesystem>
#include <vector>
#include <torch/script.h>
#include <torch/torch.h>
#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/benchmark.h"
#include "utils/ioHelper.h"
#include "LenetConfig.h"

#include <chrono>

// 读取 MNIST 图像数据
std::vector<std::vector<double>> read_mnist_images(const std::string &filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return {};
    }
    int magic_number, num_images, rows, cols;
    infile.read(reinterpret_cast<char *>(&magic_number), sizeof(magic_number));
    infile.read(reinterpret_cast<char *>(&num_images), sizeof(num_images));
    infile.read(reinterpret_cast<char *>(&rows), sizeof(rows));
    infile.read(reinterpret_cast<char *>(&cols), sizeof(cols));
    magic_number = __builtin_bswap32(magic_number);
    num_images = __builtin_bswap32(num_images);
    rows = __builtin_bswap32(rows);
    cols = __builtin_bswap32(cols);
    std::vector<std::vector<double>> images(num_images, std::vector<double>(rows * cols));
    for (int i = 0; i < num_images; i++) {
        std::vector<unsigned char> buffer(rows * cols);
        infile.read(reinterpret_cast<char *>(buffer.data()), rows * cols);
        for (int j = 0; j < rows * cols; j++) {
            images[i][j] = static_cast<double>(buffer[j]) / 255.0;
        }
    }
    infile.close();
    return images;
}

// 从文件中读取MNIST标签数据
std::vector<int> read_mnist_labels(const std::string &filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return {};
    }
    int magic_number, num_labels;
    infile.read(reinterpret_cast<char *>(&magic_number), sizeof(magic_number));
    infile.read(reinterpret_cast<char *>(&num_labels), sizeof(num_labels));
    magic_number = __builtin_bswap32(magic_number);
    num_labels = __builtin_bswap32(num_labels);
    std::vector<int> labels(num_labels);
    std::vector<unsigned char> buffer(num_labels);
    infile.read(reinterpret_cast<char *>(buffer.data()), num_labels);
    for (int i = 0; i < num_labels; i++) {
        labels[i] = static_cast<int>(buffer[i]);
    }
    infile.close();
    return labels;
}

// read model
std::vector<std::vector<double>> readModelParameters(const std::string &path) {
    torch::jit::script::Module model;
    try {
//        model = torch::jit::load("./lenet_mnist_model");
        model = torch::jit::load(path);
    } catch (const c10::Error &e) {
        std::cerr << "Error loading model!!" << e.msg() << std::endl;
        return {};
    }
    // 存储模型参数的 vector
    std::vector<std::vector<float>> parameter_data;
    for (const auto &parameter: model.parameters()) {
        // 获取参数数据的指针
        const float *data_ptr = parameter.data().data_ptr<float>();
        // 将参数数据存储到 vector
        std::vector<float> parameter_values(data_ptr, data_ptr + parameter.data().numel());
        parameter_data.push_back(parameter_values);
    }
    std::vector<std::vector<double>> resultVector;
    for (const auto &innerVector: parameter_data) {
        std::vector<double> convertedVector;
        for (const auto &num: innerVector) {
            convertedVector.push_back(static_cast<double>(num));
        }
        resultVector.push_back(convertedVector);
    }
    return resultVector;
}

// copy_bias
template<typename ClearType>
std::vector<ClearType> copyVectorNTimes(const std::vector<ClearType> &inputVector, int n) {
    std::vector<ClearType> resultVector;
    for (const auto &num: inputVector) {
        for (int i = 0; i < n; i++) {
            resultVector.push_back(num);
        }
    }
    return resultVector;
}

int main() {
    // read data and preprocess
    std::vector<std::vector<double>> parameter_data = readModelParameters("../../../data/lenet_mnist_model");
    for (int i = 0; i < parameter_data.size(); i++) {
        std::cout << parameter_data[i].size() << " ";
    }
    std::cout << "\n";
    auto origin_test_images = read_mnist_images("../../../data/t10k-images.idx3-ubyte");
    std::vector<int> origin_test_labels = read_mnist_labels("../../../data/t10k-labels.idx1-ubyte");
    std::vector<double> x_test_images;
    std::vector<int> x_test_lables = std::vector<int>(origin_test_labels.begin(),
                                                      origin_test_labels.begin() + batch_size);

//    std::vector<int> pred(x_test_lables.size());
    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    uint32_t dur_time = 0;
    for (int i = 0; i < batch_size; ++i) {

        x_test_images.clear();
        std::copy(origin_test_images[i].begin(), origin_test_images[i].end(), std::back_inserter(x_test_images));

        auto x = circuit.input(0, features, 1);
        auto conv1_weight = circuit.input(0, 6 * 1 * conv_kernel * conv_kernel, 1);
        auto conv1_bias = circuit.input(0, 6 * 24 * 24, 1);
        auto conv2_weight = circuit.input(0, 16 * 6 * conv_kernel * conv_kernel, 1);
        auto conv2_bias = circuit.input(0, 16 * 8 * 8, 1);
        auto fc1_weight = circuit.input(0, 120, 256);
        auto fc1_bias = circuit.input(0, 120, 1);
        auto fc2_weight = circuit.input(0, 84, 120);
        auto fc2_bias = circuit.input(0, 84, 1);
        auto fc3_weight = circuit.input(0, 10, 84);
        auto fc3_bias = circuit.input(0, 10, 1);
        // conv1
        auto conv1 = circuit.conv2DTrunc(x, conv1_weight, conv1_op);
        auto add1 = circuit.add(conv1, conv1_bias);
        auto r1 = circuit.relu(add1);
        // avgpool1
        auto avg1 = circuit.avgPool2D(r1, avg_op1);
        // conv2
        auto conv2 = circuit.conv2DTrunc(avg1, conv2_weight, conv2_op);
        auto add2 = circuit.add(conv2, conv2_bias);
        auto r2 = circuit.relu(add2);
        // avgpool2
        auto avg2 = circuit.avgPool2D(r2, avg_op2);
        // linear1
        auto m1 = circuit.multiplyTrunc(fc1_weight, avg2);
        auto add3 = circuit.add(m1, fc1_bias);
        auto r3 = circuit.relu(add3);
        // linear2
        auto m2 = circuit.multiplyTrunc(fc2_weight, r3);
        auto add4 = circuit.add(m2, fc2_bias);
        auto r4 = circuit.relu(add4);
        // linear3
        auto m5 = circuit.multiplyTrunc(fc3_weight, r4);
        auto add5 = circuit.add(m5, fc3_bias);
        //    auto pred = circuit.argmax(add5);
        auto end = circuit.output(add5);
        circuit.addEndpoint(end);
        circuit.readOfflineFromFile();

        // start time
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<Spdz2kShare64::ClearType> xIn(features),
                c1wIn(6 * 1 * conv_kernel * conv_kernel),
                c1bIn(6 * 24 * 24),
                c2wIn(16 * 6 * conv_kernel * conv_kernel),
                c2bIn(16 * 8 * 8),
                f1wIn(120 * 256),
                f1bIn(120 * 1),
                f2wIn(84 * 120),
                f2bIn(84 * 1),
                f3wIn(10 * 84),
                f3bIn(10 * 1);
        auto c1b_ = copyVectorNTimes(parameter_data[1], 24 * 24);
        auto c2b_ = copyVectorNTimes(parameter_data[3], 8 * 8);
//        std::cout << "c1b_.size: " << c1b_.size() << " c2b_:" << c2b_.size();
        xIn = double2fixVec<Spdz2kShare64::ClearType>(x_test_images);
        c1wIn = double2fixVec<Spdz2kShare64::ClearType>(parameter_data[0]);
        c1bIn = double2fixVec<Spdz2kShare64::ClearType>(c1b_);
        c2wIn = double2fixVec<Spdz2kShare64::ClearType>(parameter_data[2]);
        c2bIn = double2fixVec<Spdz2kShare64::ClearType>(c2b_);
        f1wIn = double2fixVec<Spdz2kShare64::ClearType>(parameter_data[4]);
        f1bIn = double2fixVec<Spdz2kShare64::ClearType>(parameter_data[5]);
        f2wIn = double2fixVec<Spdz2kShare64::ClearType>(parameter_data[6]);
        f2bIn = double2fixVec<Spdz2kShare64::ClearType>(parameter_data[7]);
        f3wIn = double2fixVec<Spdz2kShare64::ClearType>(parameter_data[8]);
        f3bIn = double2fixVec<Spdz2kShare64::ClearType>(parameter_data[9]);
//        std::cout << "input: \n";
        x->setInput(xIn);
        conv1_weight->setInput(c1wIn);
        conv1_bias->setInput(c1bIn);
        conv2_weight->setInput(c2wIn);
        conv2_bias->setInput(c2bIn);
        fc1_weight->setInput(f1wIn);
        fc1_bias->setInput(f1bIn);
        fc2_weight->setInput(f2wIn);
        fc2_bias->setInput(f2bIn);
        fc3_weight->setInput(f3wIn);
        fc3_bias->setInput(f3bIn);

        circuit.runOnline();

        // end time
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        dur_time += duration.count();
//        std::cout << "Result: \n";
//        printVector(end->getClear());
//        auto res = fix2doubleVec(end->getClear());
//        auto maxIt = std::max_element(res.begin(), res.end());
//        int maxIndex = std::distance(res.begin(), maxIt);
//        std::cout << "x_test: " << maxIndex << " ";
//        pred[i] = maxIndex;
    }

//    int count = 0;
//    for (int i = 0; i < x_test_lables.size(); i++) {
//        if (pred[i] == x_test_lables[i]) {
//            count++;
//        }
//    }
//    std::cout << "acc: " << (double) count / batch_size << "\n";

    std::cout << "average time: " << dur_time / batch_size << "ms \n";
    return 0;
}