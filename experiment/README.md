# 实验TODO


所有实验在LAN和WAN环境下都做一遍

使用不同的安全参数 （k = s = 32, k = s = 64）


## Building Bolcks
- 比较协议
- MaxPool 和 ArgMax
- Convolution
- Batch Norm

## Micro-benchmark
- [ ]  向量点乘（不同长度）
- [ ]  向量点乘 with truncation
- [ ]  矩阵乘法（不同维数）
- [ ]  可能： 比较协议


## SVM inference
- [ ] 不同数据集
- [ ] 不同模型
- [ ] 不同batch size
- [ ] 可能：比较和明文预测的准确率


## NN inference
- [ ] MNIST CIFAR-10 Tiny-ImageNet
- [ ] 3层神经网络, LeNet, VGG, ResNet （需要用MP-SPDZ实现和对比）
- [ ] 不同batch size
- [ ] Batch Norm?
- [ ] 比较简单的神经网络和明文预测的准确率
