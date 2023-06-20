#!/bin/bash

# 可能需要的库
# pip install torch
# pip install torchvision

# 前五个命令只需要执行一次，之后可以注释掉
make clean
Scripts/tldr.sh
make -j 8 online
make Fake-Offline.x spdz2k-party.x
./Fake-Offline.x 2 -Z 64 -S 64

./compile.py -R 64 torch_mnist_lenet_predict

# 打开新终端并执行第6个命令
gnome-terminal -- bash -c "./spdz2k-party.x -F -N 2 0 torch_mnist_lenet_predict -v"

# 打开新终端并执行最后一个命令
gnome-terminal -- bash -c "./spdz2k-party.x -F -N 2 1 torch_mnist_lenet_predict -v"
