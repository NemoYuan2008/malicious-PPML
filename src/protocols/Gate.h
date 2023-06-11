#ifndef MALICIOUS_PPML_GATE_H
#define MALICIOUS_PPML_GATE_H


#include <vector>
#include <fstream>
#include <memory>
#include "utils/Party.h"

template<typename ShrType>
class Gate {
public:
    using ClearType = typename ShrType::ClearType;
    using SemiShrType = typename ShrType::SemiShrType;

    Gate() = default;   //Should only be used for tests

    Gate(Party<ShrType> *party, int row, int column)
            : party(party), dimRow(row), dimCol(column) {}    //Used for input gates

    Gate(const std::shared_ptr<Gate> &input_x, const std::shared_ptr<Gate> &input_y)
            : input_x(input_x), input_y(input_y), party(input_x->getParty()) {}

    virtual ~Gate() = default;

    void readOfflineFromFile(std::ifstream &ifs) {
        if (this->isReadOffline())
            return;

        if (input_x && !input_x->isReadOffline())
            input_x->readOfflineFromFile(ifs);
        if (input_y && !input_y->isReadOffline())
            input_y->readOfflineFromFile(ifs);

        this->doReadOfflineFromFile(ifs);

        this->readOffline = true;
    }

    void runOffline() {
        if (this->isEvaluatedOffline())
            return;

        if (input_x && !input_x->isEvaluatedOffline())
            input_x->runOffline();
        if (input_y && !input_y->isEvaluatedOffline())
            input_y->runOffline();

        this->doRunOffline();

        this->evaluatedOffline = true;
    }

    void runOnline() {
        if (this->isEvaluatedOnline())
            return;

        if (input_x && !input_x->isEvaluatedOnline())
            input_x->runOnline();
        if (input_y && !input_y->isEvaluatedOnline())
            input_y->runOnline();

        this->doRunOnline();

        this->evaluatedOnline = true;
    }


private:
    virtual void doRunOffline() = 0;

    virtual void doRunOnline() = 0;

    virtual void doReadOfflineFromFile(std::ifstream &ifs) {
        int size = dimRow * dimCol;
        this->lambdaShr.resize(size);
        this->lambdaShrMac.resize(size);

        for (int i = 0; i < size; ++i) {
            ifs >> this->lambdaShr[i] >> this->lambdaShrMac[i];
        }
//        SemiShrType shr, mac;
//        ifs >> shr >> mac;
//        lambdaShr.push_back(shr);
//        lambdaShrMac.push_back(mac);

//        this->deltaClear.resize(lambdaShr.size());
    }


public:
    [[nodiscard]] bool isEvaluatedOffline() const { return evaluatedOffline; }

    [[nodiscard]] bool isEvaluatedOnline() const { return evaluatedOnline; }

    [[nodiscard]] bool isReadOffline() const { return readOffline; }

    const auto &getLambdaShr() const { return lambdaShr; }

    const auto &getLambdaShrMac() const { return lambdaShrMac; }

    const auto &getDeltaClear() const { return deltaClear; }

//    void setLambdaShr(ShrType p_lambdaShr) { lambdaShr = p_lambdaShr; }

//    void setDeltaClear(ClearType p_deltaClear) { deltaClear = p_deltaClear; }

    void setInputX(const std::shared_ptr<Gate<ShrType>> &inputX) { input_x = inputX; }

    void setInputY(const std::shared_ptr<Gate<ShrType>> &inputY) { input_y = inputY; }

    const std::shared_ptr<Gate<ShrType>> &getInputX() const { return input_x; }

    const std::shared_ptr<Gate<ShrType>> &getInputY() const { return input_y; }

    void setInputXY(const std::shared_ptr<Gate> &x, const std::shared_ptr<Gate> &y) { input_x = x, input_y = y; }

    void setEvaluatedOffline() { evaluatedOffline = true; }     //for debugging

    void setEvaluatedOnline() { evaluatedOnline = true; }       //for debugging

    Party<ShrType> *getParty() { return party; }

    [[nodiscard]] int myId() const { return party->getMyId(); }

    int getDimRow() const { return dimRow; }

    int getDimCol() const { return dimCol; }

protected:
    std::shared_ptr<Gate<ShrType>> input_x, input_y;
    Party<ShrType> *party;

    int dimRow = 1, dimCol = 1; //TODO: initialize in ctor
    std::vector<SemiShrType> lambdaShr, lambdaShrMac;
    std::vector<SemiShrType> deltaClear;    //is ClearType, but stored as SemiShrType for convenience

private:
    bool evaluatedOffline = false;
    bool evaluatedOnline = false;
    bool readOffline = false;
};


#endif //MALICIOUS_PPML_GATE_H
