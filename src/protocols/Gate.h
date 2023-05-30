#ifndef MALICIOUS_PPML_GATE_H
#define MALICIOUS_PPML_GATE_H


#include <memory>

template<typename ShrType>
class Gate {
public:
    using ClearType = typename ShrType::ClearType;

    Gate() = default;

    Gate(const std::shared_ptr<Gate> &input_x, const std::shared_ptr<Gate> &input_y)
            : input_x(input_x), input_y(input_y) {}

    virtual ~Gate() = default;

    virtual void runOffline() = 0;

    virtual void runOnline() = 0;

    bool isEvaluatedOffline() const { return evaluatedOffline; }

    bool isEvaluatedOnline() const { return evaluatedOnline; }

    const ShrType &getLambdaShr() const { return lambdaShr; }

    ClearType getDeltaClear() const { return deltaClear; }

    void setLambdaShr(ShrType lambdaShr) { Gate::lambdaShr = lambdaShr; }

    void setDeltaClear(ClearType p_deltaClear) { deltaClear = p_deltaClear; }

    void setInputX(const std::shared_ptr<Gate<ShrType>> &inputX) { input_x = inputX; }

    void setInputY(const std::shared_ptr<Gate<ShrType>> &inputY) { input_y = inputY; }

    void setInputXY(const std::shared_ptr<Gate> &x, const std::shared_ptr<Gate> &y) { input_x = x, input_y = y; }


protected:
    void runOfflineRecursive() {   //used in runOffline
        if (input_x && input_x->isEvaluatedOffline())
            input_x->runOffline();
        if (input_y && input_y->isEvaluatedOffline())
            input_y->runOffline();
    }

    void runOnlineRecursive() {    //used in runOnline
        if (input_x && input_x->isEvaluatedOnline())
            input_x->runOnline();
        if (input_y && input_y->isEvaluatedOnline())
            input_y->runOnline();
    }

protected:
    //Maybe: define clearLambda here for debugging purpose
    ShrType lambdaShr{};      //We don't set it in ctor, it's set in offline phase
    ClearType deltaClear{};   //We don't set it in ctor, it's set in online phase
    std::shared_ptr<Gate<ShrType>> input_x, input_y;
    bool evaluatedOffline = false;
    bool evaluatedOnline = false;
};


#endif //MALICIOUS_PPML_GATE_H
