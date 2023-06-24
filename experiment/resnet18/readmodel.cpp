//
// Created by ysx on 6/23/23.
//
#include <torch/script.h>
#include <iostream>
#include <memory>
#include <string>

int main() {
    auto t = torch::rand({2, 3, 4, 5});
//    std::cout << t[0][0][0] << ' ' << t.size(1) << ' ' << t.size(2) << ' ' << t.size(3) << ' ';
//    std::cout << t.sizes();
    torch::jit::script::Module model;
    //Deserialize the ScriptModule from a file using torch::jit::load()
    model = torch::jit::load("data/resnet18_script_module.pt");
    auto params = model.parameters();
    int count = 0;
    for (const auto &p: model.named_modules()) {
        std::cout<< p.name <<" "<< p.value.modules().size()<<"\n";
//        std::cout<< p.name << " " << p.value.sizes()<<"\n";
//        try {
//            std::cout << p.name << ": " << p.value.size(0) << " " << p.value.size(1) << " " << p.value.size(2) << " "
//                      << p.value.size(3) << "\n";
//        } catch (std::exception &) {
//
//        }

//        std::cout << p.value[0]<<"\n";
//        count++;
//        if (count > 0) break;
    }

    return 0;
}