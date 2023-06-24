//
// Created by zyx on 23-6-16.
//
#include <iostream>
#include <filesystem>
#include <vector>
#include <numeric>
#include <Eigen/Dense>
#include "share/Spdz2kShare.h"
#include "protocols/Circuit.h"
#include "utils/rand.h"
#include "utils/Party.h"
#include "utils/benchmark.h"
#include "utils/fixedPoint.h"
#include "utils/ioHelper.h"
#include "SVMConfig.h"


// 读取 MNIST 图像数据
std::vector<std::vector<double>> read_mnist_images(const std::string& filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return {};
    }
    int magic_number, num_images, rows, cols;
    infile.read(reinterpret_cast<char*>(&magic_number), sizeof(magic_number));
    infile.read(reinterpret_cast<char*>(&num_images), sizeof(num_images));
    infile.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    infile.read(reinterpret_cast<char*>(&cols), sizeof(cols));
    magic_number = __builtin_bswap32(magic_number);
    num_images = __builtin_bswap32(num_images);
    rows = __builtin_bswap32(rows);
    cols = __builtin_bswap32(cols);
    std::vector<std::vector<double>> images(num_images, std::vector<double>(rows * cols));
    for (int i = 0; i < num_images; i++) {
        std::vector<unsigned char> buffer(rows * cols);
        infile.read(reinterpret_cast<char*>(buffer.data()), rows * cols);
        for (int j = 0; j < rows * cols; j++) {
            images[i][j] = static_cast<double>(buffer[j]) / 255.0;
        }
    }
    infile.close();
    return images;
}

// 从文件中读取MNIST标签数据
std::vector<int> read_mnist_labels(const std::string& filename) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return {};
    }
    int magic_number, num_labels;
    infile.read(reinterpret_cast<char*>(&magic_number), sizeof(magic_number));
    infile.read(reinterpret_cast<char*>(&num_labels), sizeof(num_labels));
    magic_number = __builtin_bswap32(magic_number);
    num_labels = __builtin_bswap32(num_labels);
    std::vector<int> labels(num_labels);
    std::vector<unsigned char> buffer(num_labels);
    infile.read(reinterpret_cast<char*>(buffer.data()), num_labels);
    for (int i = 0; i < num_labels; i++) {
        labels[i] = static_cast<int>(buffer[i]);
    }
    infile.close();
    return labels;
}

// read mnist model
std::vector<double> read_mnist_model(const std::string& filename) {
    std::vector<double> res;
    double v;
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Failed to open file.\n";
        return {};
    }
    while (infile >> v) {
        res.push_back(v);
    }
    return res;
}

// 转置
std::vector<double> transposeMatrix(const std::vector<double>& matrix, int rows, int columns) {
    std::vector<double> transposedMatrix(columns * rows); // 创建一个新的一维向量来存储转置后的矩阵

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            transposedMatrix[j * rows + i] = matrix[i * columns + j]; // 将原矩阵的元素按转置规则存储到新矩阵中
        }
    }
    return transposedMatrix;
}

// transform b.shape to batch_size * classes
template<typename ClearType>
std::vector<ClearType> copyVector(std::vector<ClearType> v, int t) {
    std::vector<ClearType> result;
    for (int i = 0; i < t; i++) {
        result.insert(result.end(), v.begin(), v.end());
    }
    return result;
}

int main() {

//    std::vector<int> vec;
//    std::cout << "Max size of vector: " << vec.max_size() << std::endl;

    std::vector<double> origin_w = read_mnist_model("../../../data/mnist_w.txt");

    std::vector<double> w_t = transposeMatrix(origin_w, classes, features);

    std::vector<double> origin_b = read_mnist_model("../../../data/mnist_b.txt");

//    std::vector<double> batch_origin_b = copyVector(origin_b, batch_size);

//    std::vector<std::vector<double>> origin_test_images;
//    std::cout << origin_test_images.max_size() << std::endl;
    auto origin_test_images = read_mnist_images("../../../data/t10k-images.idx3-ubyte");
    std::vector<int> origin_test_labels = read_mnist_labels("../../../data/t10k-labels.idx1-ubyte");

    std::cout << "w.size: " << origin_w.size() << " b.size: " << origin_b.size() << std::endl;
    std::cout << "Number of test images: " << origin_test_images.size() << std::endl;
    std::cout << "Number of test labels: " << origin_test_labels.size() << std::endl;

    std::vector<double> x_test_images;
    std::vector<int> x_test_lables = std::vector<int>(origin_test_labels.begin(), origin_test_labels.begin() + batch_size);
    for (int i = 0; i < batch_size; i++) {
//        x_test_images.insert(x_test_images.end(), origin_test_images[i].begin(), origin_test_images[i].end());
        x_test_images.insert(x_test_images.end(), origin_test_images[i].begin(), origin_test_images[i].begin() + features);
    }
    //    std::vector<std::vector<double>> origin_train_images = read_mnist_images("../data/train-images.idx3-ubyte");
    //    std::vector<int> origin_train_labels = read_mnist_labels("../data/train-labels.idx1-ubyte");

    // 输出读取的数据
//    std::cout << "w.size: " << origin_w.size() << " b.size: " << origin_b.size() << std::endl;
//    std::cout << "Number of test images: " << origin_test_images.size() << std::endl;
//    std::cout << "Number of test labels: " << origin_test_labels.size() << std::endl;
//    std::cout << "Test image size: " << origin_test_images[0].size() << std::endl;

    auto path = std::filesystem::temp_directory_path();
    Party<Spdz2kShare64> party(0, 2, (path / "0.txt").string());
    Circuit<Spdz2kShare64> circuit(&party);

    auto w = circuit.input(0, features, classes);
    auto b = circuit.input(0, batch_size, classes);
    auto x = circuit.input(0, batch_size, features);
    auto y = circuit.multiplyTrunc(x, w);
    auto z = circuit.add(y, b);
    auto pred = circuit.argmax(z);
    auto end = circuit.output(pred);

    circuit.addEndpoint(end);

    circuit.readOfflineFromFile();

    std::vector<Spdz2kShare64::ClearType> wIn(classes * features), bIn(classes), xIn(batch_size * features);
    wIn = double2fixVec<Spdz2kShare64::ClearType>(w_t);
    bIn = double2fixVec<Spdz2kShare64::ClearType>(origin_b);
    xIn = double2fixVec<Spdz2kShare64::ClearType>(x_test_images);
    bIn = copyVector(bIn, batch_size);
    std::cout << "win.size: " << wIn.size() << " bin.size: " << bIn.size() << " xin.size: " << xIn.size() << "\n";

    w->setInput(wIn);
    b->setInput(bIn);
    x->setInput(xIn);

    circuit.runOnline();

    int count = 0;
    auto res = end->getClear();
    for (int i = 0; i < batch_size; i++) {
        std::cout << x_test_lables[i] << " ";
        if (x_test_lables[i] == res[i]) {
            count++;
        }
    }
    std::cout << "\n";

    printVector(end->getClear());
    std::cout << "acc" << (double)count /batch_size << std::endl;
    return 0;
}