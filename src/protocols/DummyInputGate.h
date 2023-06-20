//#ifndef MALICIOUS_PPML_DUMMYINPUTGATE_H
//#define MALICIOUS_PPML_DUMMYINPUTGATE_H
//
//
//#include <stdexcept>
//#include <algorithm>
//#include <execution>
//#include "protocols/Gate.h"
//#include "utils/Party.h"
//#include "utils/linear_algebra.h"
//
//template<typename ShrType>
//class DummyInputGate : public Gate<ShrType> {
//public:
//    using typename Gate<ShrType>::ClearType;
//    using typename Gate<ShrType>::SemiShrType;
//
//    using Gate<ShrType>::Gate;
//
//    DummyInputGate(Party<ShrType> *party, int row, int col) : Gate<ShrType>(party, row, col) {}
//
//    void setLambdaShr(const std::vector<SemiShrType> &value) { this->lambdaShr = value; }
//
//    void setDeltaClear(const std::vector<SemiShrType> &value) { this->deltaClear = value; }
//
////    void setDimRow(int row) { this->dimRow = row; }
//
////    void setDimCol(int col) { this->dimCol = col; }
//
//private:
//    void doReadOfflineFromFile(std::ifstream &ifs) override {}
//
//    void doRunOffline() override {
//
//    }
//
//    void doRunOnline() override {}
//};
//
//#endif //MALICIOUS_PPML_DUMMYINPUTGATE_H
