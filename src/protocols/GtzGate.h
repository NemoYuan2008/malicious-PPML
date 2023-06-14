#ifndef MALICIOUS_PPML_GTZGATE_H
#define MALICIOUS_PPML_GTZGATE_H


#include <vector>
#include <thread>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"
#include <bitset>

#ifndef NDEBUG

#include "utils/ioHelper.h"

#endif

template<typename ShrType>
class GtzGate : public Gate<ShrType> {
public:
    using typename Gate<ShrType>::ClearType;
    using typename Gate<ShrType>::SemiShrType;

    explicit GtzGate(const std::shared_ptr<Gate<ShrType>> &input_x)
            : Gate<ShrType>(input_x, nullptr) {
        this->dimRow = input_x->getDimRow();
        this->dimCol = input_x->getDimCol();
    }

private:
    void doReadOfflineFromFile(std::ifstream &ifs) override {
        int size = this->dimRow * this->dimCol;
        this->lambdaShr.resize(size);
        this->lambdaShrMac.resize(size);
        this->lambda_xBinShr.resize(size);

        for (int i = 0; i < size; ++i) {
            ifs >> this->lambdaShr[i] >> this->lambdaShrMac[i] >> this->lambda_xBinShr[i];
        }

#ifndef NDEBUG
        std::cout << "lambda from input: ";
        printVector(this->input_x->getLambdaShr());
        std::cout << "lambdaShr: ";
        printVector(this->lambda_xBinShr);
#endif
    }

    void doRunOffline() override {}

    void doRunOnline() override {
        const auto &delta_x_semiShr = this->input_x->getDeltaClear();
        std::vector<ClearType> delta_x(delta_x_semiShr.begin(), delta_x_semiShr.end());

        //TODO: compare delta_x[0] and lambda_xBinShr[0]
        //Maybe:
        std::bitset<sizeof(ClearType) * 8> lambda_xBinShrBits(this->lambda_xBinShr[0]);

    }


protected:
    std::vector<ClearType> lambda_xBinShr;
    bool a = false, b = false, c = false;   //Fake GMW Triples
};


#endif //MALICIOUS_PPML_GTZGATE_H
