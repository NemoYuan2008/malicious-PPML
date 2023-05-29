#ifndef MALICIOUS_PPML_GATE_H
#define MALICIOUS_PPML_GATE_H


#include <memory>

template<typename ShrType>
class Gate {
public:
    Gate() = default;

    Gate(const std::shared_ptr<Gate> &input_x, const std::shared_ptr<Gate> &input_y)
            : input_x(input_x), input_y(input_y) {}

    virtual void runOffline() = 0;

    const ShrType &getLambda_zShr() const { return lambda_zShr; }

protected:
    //Maybe: define clearLambda here for debugging purpose
    ShrType lambda_zShr;
    std::shared_ptr<Gate> input_x, input_y;
};


#endif //MALICIOUS_PPML_GATE_H
