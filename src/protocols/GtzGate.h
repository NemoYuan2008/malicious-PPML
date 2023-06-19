#ifndef MALICIOUS_PPML_GTZGATE_H
#define MALICIOUS_PPML_GTZGATE_H


#include <vector>
#include <thread>
#include <bitset>
#include <algorithm>
#include <execution>
#include <functional>
#include "protocols/Gate.h"
#include "utils/linear_algebra.h"

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
        auto ret = BitLT(delta_x, this->lambda_xBinShr);
        if (this->myId() == 0) {
            std::transform(std::execution::par_unseq, ret.begin(), ret.end(), ret.begin(), std::logical_not<>());
        }

#ifndef NDEBUG
        std::cout << "delta_x: ";
        printVector(delta_x);
        std::cout << "Lambda_xBinShr: ";
        printVector(this->lambda_xBinShr);
        std::cout << "\n Ret value of BitLT:";
        printVector(ret);
#endif

        //TODO: this is fake

        int msgBytes = (ret.size() + 7) / 8; // round up
        std::vector<std::bitset<sizeof(uint8_t) * 8>> sendmsg(msgBytes, 0);
        std::vector<std::bitset<sizeof(uint8_t) * 8>> rcvmsg(msgBytes, 0);

        int vec_loc, bit_loc;
        for (int j = 0; j < ret.size(); ++j) {
            vec_loc = j / 8;
            bit_loc = j % 8;
            sendmsg[vec_loc][bit_loc] = ret[j]; //[p1] -[a]
        }

        std::thread t1([this, &sendmsg]() { this->getParty()->getNetwork().send(1 - this->myId(), sendmsg); });
        std::thread t2(
                [this, &rcvmsg]() { this->getParty()->getNetwork().rcv(1 - this->myId(), &rcvmsg, rcvmsg.size()); });
        t1.join();
        t2.join();

        std::vector<bool> rcv(ret.size());
        for (int j = 0; j < rcv.size(); ++j) {
            vec_loc = j / 8;
            bit_loc = j % 8;
            rcv[j] = rcvmsg[vec_loc][bit_loc]; //[p1] -[a]
        }


        std::vector<SemiShrType> zShr(ret.size(), 0);

        if (this->myId() == 0) {
            std::transform(std::execution::par_unseq,
                           ret.begin(), ret.end(), rcv.begin(), zShr.begin(), std::bit_xor<>());
        }

        this->deltaClear = matrixAdd(this->lambdaShr, zShr);
        std::vector<SemiShrType> deltaRcv(zShr.size());
        std::thread t3([this]() { this->getParty()->getNetwork().send(1 - this->myId(), this->deltaClear); });
        std::thread t4([this, &deltaRcv]() {
            this->getParty()->getNetwork().rcv(1 - this->myId(), &deltaRcv, deltaRcv.size());
        });
        t3.join();
        t4.join();

        matrixAddAssign(this->deltaClear, deltaRcv);
    }

    bool LT(int32_t x_loc, int32_t y_loc){ // (x<y)
        auto delta = this->getDeltaClear();
        auto lambda = this->lambda_xBinShr;
        auto delta_xy = delta[x_loc] - delta[y_loc];
        auto lambda_xy = lambda[x_loc]^lambda[y_loc];

    }

    template<typename T>
    bool BitLT_Ind(T aInt,T bInt){ // aInt < bInt
        T a_, b_;
        if (this->myId()==0) b_ = ~bInt;
        else b_ = bInt;
        a_ = aInt;
        auto ret = CarryOutCin_Ind(a_,b_,1);
        if (this->myId()==0) ret = 1^ret;
        return ret;
    }

    template<typename T>
    bool CarryOutCin_Ind(T a_, T b_, bool cIn){
        std::bitset<sizeof(T)*8> aIn, bIn;
        aIn = a_; bIn = b_;
        std::bitset<sizeof(T) * 8> p;
        std::bitset<sizeof(T) * 8> g;
        int numBits = sizeof(T)*8;

        int msgBytes = (numBits*2+7)/8; // round up
        std::vector<std::bitset<sizeof(uint8_t)*8>> sendmsg(msgBytes,0);
        std::vector<std::bitset<sizeof(uint8_t)*8>> rcvmsg(msgBytes,0);
        int32_t vec_loc, bit_loc;
        for (int j =0; j< numBits; ++j) {
            vec_loc = numBits*2/8;
            bit_loc = (numBits*2)%8;
            sendmsg[vec_loc][bit_loc] = aIn[j] ^ a; //[x] -[a]
            sendmsg[vec_loc][bit_loc+1] = bIn[j] ^ b; //[y] -[b]
        }

        std::thread t1([this, &sendmsg]() {
            this->party->getNetwork().send(1 - this->myId(), sendmsg);
        });
        std::thread t2([this, &rcvmsg]() {
            this->party->getNetwork().rcv(1 - this->myId(), &rcvmsg, rcvmsg.size());
        });
        t1.join();
        t2.join();

        bool alpha, beta, x_, y_, z;
        for (int j=0; j<numBits; j++){
            vec_loc = numBits*2/8;
            bit_loc = (numBits*2)%8;
            alpha = sendmsg[vec_loc][bit_loc] ^ rcvmsg[vec_loc][bit_loc]; // alpha = x -a
            beta = sendmsg[vec_loc][bit_loc+1] ^ rcvmsg[vec_loc][bit_loc+1]; // beta = y - b
            x_ = aIn[j], y_=bIn[j]; //x_ -- [x], y_ -- [y]
            z = c ^ (alpha & y_) ^ (beta & x_); // z = xy
            if (this->myId()==0) z ^= (alpha&beta);
            g[j] = z;
            p[j] = aIn[j] ^ bIn[j]; //g = a*b
        }
        g[0] = g[0] ^ (cIn&p[0]); // g1 = g1 + c*p1
        return CarryOutAux(p,g,numBits);
    }

    template<typename T>
    bool CarryOutAux(std::bitset<sizeof(T) * 8> p,
                     std::bitset<sizeof(T) * 8> g, int k){  //k bits, (p2,g2)*(p1,g1) = (p2p1,g2+p2g1)
        if (k>1){
            int u_len = k/2; // round down bit length, if k%2=1, push back the last one bit at the end
            std::bitset<sizeof(T) * 8> u_p, u_g;
            // compute u[k/2..1] = (d[2k] * d[2k-1],...)---- compute p2*p1 and g2*p1 need 2 triples
            // (k/2)*2 triples per invocation
            int numTriples = u_len*2; //parallel g.size() comparisons
            //prepare beaver's triples
            //[alpha] = [x] - [a]
            //[beta] = [y] - [b]
            // open alpha, beta
            // compute [z] = [c] + alpha*[b] + beta*[a] + alpha*beta

            // each triple sholud send alpha, beta -- 2 bits
            int msgBytes = (numTriples*2+7)/8; // round up
            std::vector<std::bitset<sizeof(uint8_t)*8>> sendmsg(msgBytes,0);
            std::vector<std::bitset<sizeof(uint8_t)*8>> rcvmsg(msgBytes,0);
            int vec_loc, bit_loc;
            int index_triple = 0;
            // load the msg
            for (int j =0; j< u_len; ++j) {
                vec_loc = index_triple*2/8;
                bit_loc = (index_triple*2)%8;
                index_triple++;
                sendmsg[vec_loc][bit_loc] = p[2*j] ^ a; //[p1] -[a]
                sendmsg[vec_loc][bit_loc+1] = p[2*j+1] ^ b; //[p2] -[b]
                vec_loc = index_triple*2/8;
                bit_loc = (index_triple*2)%8;
                index_triple++;
                sendmsg[vec_loc][bit_loc] = g[2*j] ^ a; //[g1] -[a]
                sendmsg[vec_loc][bit_loc+1] = p[2*j+1] ^ b; //[p2] -[b]
            }
            //send & rcv
            //send numTriples, sendmsg; receive numTriples rcvmsg
            std::thread t1([this, &sendmsg]() {
                this->party->getNetwork().send(1 - this->myId(), sendmsg);
            });
            std::thread t2([this, &rcvmsg]() {
                this->party->getNetwork().rcv(1 - this->myId(), &rcvmsg, rcvmsg.size());
            });
            t1.join();
            t2.join();
//#ifndef NDEBUG
//            std::cout<<"sendmsg: ";printVector(sendmsg);
//            std::cout<<"rcvmsg: "; printVector(rcvmsg);
//#endif
            //compute
            bool alpha, beta, z, x_, y_;
            index_triple = 0;
            for (int j =0; j< u_len; ++j) {  // compute u_p, u_g
                vec_loc = index_triple*2/8;
                bit_loc = (index_triple*2)%8;
                index_triple++;
                alpha = sendmsg[vec_loc][bit_loc] ^ rcvmsg[vec_loc][bit_loc]; // alpha = p1 -a
                beta = sendmsg[vec_loc][bit_loc+1] ^ rcvmsg[vec_loc][bit_loc+1]; // beta = p2 - b
                x_ = p[2*j], y_=p[2*j+1]; //x_ -- p1, y_ -- p2
                z = c ^ (alpha & y_) ^ (beta & x_); // z = p1p2
                if (this->myId()==0) z ^= (alpha&beta);
                u_p[j] = z;
                vec_loc = index_triple*2/8;
                bit_loc = (index_triple*2)%8;
                index_triple++;
                alpha = sendmsg[vec_loc][bit_loc] ^ rcvmsg[vec_loc][bit_loc];  // open g1 -a
                beta = sendmsg[vec_loc][bit_loc+1] ^ rcvmsg[vec_loc][bit_loc+1]; // open  p2 -b
                x_ = g[2*j], y_=p[2*j+1]; // x_ -- g1 y_ -- p2
                z = c ^ (alpha & y_) ^ (beta & x_); // z = p2g1
                if (this->myId()==0) z ^= (alpha&beta);
                u_g[j] = g[2*j+1] ^ z; // u_g = g2 + p2g1
            }
            if (index_triple < numTriples) std::cout<<"triples amount error\n";
            if (k%2 == 1){
                u_len += 1;
                for (int i = 0; i < p.size(); ++i) {
                    u_p[i][u_len] = p[i][k-1];
                    u_g[i][u_len] = g[i][k-1];
                }
            }
            auto ret = CarryOutAux(u_p,u_g,u_len); // u_len : bit length
            return ret;
        }
        else{
            return g[0]; // Actcually only care g[..][0]
        }
    }

    std::vector<bool> BitLT(std::vector<ClearType> &pInt, // output s = (pInt < sInt)
                            std::vector<ClearType> &sInt) {
        std::vector<std::bitset<sizeof(ClearType) * 8>> b_(sInt.size());
        std::vector<std::bitset<sizeof(ClearType) * 8>> a_(pInt.size());
        for (int i = 0; i < sInt.size(); ++i) {
            if (this->myId() == 0) b_[i] = ~sInt[i]; //b_[i][j] = 1 - b[i][j]
            else b_[i] = sInt[i];
            a_[i] = pInt[i];
        }
        auto s = CarryOutCin(a_, b_, 1);
        for (int i = 0; i < s.size(); ++i) {
            if (this->myId() == 0) s[i] = 1 ^ s[i]; //s[i] = 1 -s[i]
        }
        return s;
    }

    std::vector<bool> CarryOutCin(std::vector<std::bitset<sizeof(ClearType) * 8>> &aIn,
                                  std::vector<std::bitset<sizeof(ClearType) * 8>> &bIn,
                                  bool cIn) { //a<-delta_x, b<-lambda_xBinShr

        std::vector<std::bitset<sizeof(ClearType) * 8>> p(bIn.size());
        std::vector<std::bitset<sizeof(ClearType) * 8>> g(bIn.size());
        //compute p[i] = a[i]^b[i], g[i] = a[i]*b[i]
        int numBits = sizeof(ClearType) * 8;
        for (int i = 0; i < bIn.size(); ++i) {
            for (int j = 0; j < numBits; j++) {
                if (this->myId() == 0) p[i][j] = aIn[i][j] ^ bIn[i][j];
                else p[i][j] = bIn[i][j]; //p = a+b -2ab
                g[i][j] = aIn[i][j] & bIn[i][j]; //g = a*b
            }
        }
        for (int i = 0; i < bIn.size(); ++i) {
            g[i][0] = g[i][0] ^ (cIn & p[i][0]); // g1 = g1 + c*p1
        }
        return CarryOutAux(p, g, numBits);
    }

    std::vector<bool> CarryOutAux(std::vector<std::bitset<sizeof(ClearType) * 8>> p,
                                  std::vector<std::bitset<sizeof(ClearType) * 8>> g,
                                  int k) {  //k bits, (p2,g2)*(p1,g1) = (p2p1,g2+p2g1)
        if (k > 1) {
            int u_len = k / 2; // round down bit length, if k%2=1, push back the last one bit at the end
            std::vector<std::bitset<sizeof(ClearType) * 8>> u_p(p.size(), 0);
            std::vector<std::bitset<sizeof(ClearType) * 8>> u_g(p.size(), 0);
            // compute u[k/2..1] = (d[2k] * d[2k-1],...)---- compute p2*p1 and g2*p1 need 2 triples
            // (k/2)*2 triples per invocation
            int numTriples = g.size() * u_len * 2; //parallel g.size() comparisons
            //prepare beaver's triples
            //[alpha] = [x] - [a]
            //[beta] = [y] - [b]
            // open alpha, beta
            // compute [z] = [c] + alpha*[b] + beta*[a] + alpha*beta

            // each triple should send alpha, beta -- 2 bits
            int msgBytes = (numTriples * 2 + 7) / 8; // round up
            std::vector<std::bitset<sizeof(uint8_t) * 8>> sendmsg(msgBytes, 0);
            std::vector<std::bitset<sizeof(uint8_t) * 8>> rcvmsg(msgBytes, 0);
            int vec_loc, bit_loc;
            int index_triple = 0;
            // load the msg
            for (int i = 0; i < g.size(); ++i) { // parallel comparison
                for (int j = 0; j < u_len; ++j) {
                    vec_loc = index_triple * 2 / 8;
                    bit_loc = (index_triple * 2) % 8;
                    index_triple++;
                    sendmsg[vec_loc][bit_loc] = p[i][2 * j] ^ a; //[p1] -[a]
                    sendmsg[vec_loc][bit_loc + 1] = p[i][2 * j + 1] ^ b; //[p2] -[b]
                    vec_loc = index_triple * 2 / 8;
                    bit_loc = (index_triple * 2) % 8;
                    index_triple++;
                    sendmsg[vec_loc][bit_loc] = g[i][2 * j] ^ a; //[g1] -[a]
                    sendmsg[vec_loc][bit_loc + 1] = p[i][2 * j + 1] ^ b; //[p2] -[b]
                }
            }
            //send & rcv
            //send numTriples, sendmsg; receive numTriples rcvmsg
            std::thread t1([this, &sendmsg]() {
                this->party->getNetwork().send(1 - this->myId(), sendmsg);
            });
            std::thread t2([this, &rcvmsg]() {
                this->party->getNetwork().rcv(1 - this->myId(), &rcvmsg, rcvmsg.size());
            });
            t1.join();
            t2.join();
//#ifndef NDEBUG
//            std::cout<<"sendmsg: ";printVector(sendmsg);
//            std::cout<<"rcvmsg: "; printVector(rcvmsg);
//#endif
            //compute
            bool alpha, beta, z, x_, y_;
            index_triple = 0;
            for (int i = 0; i < g.size(); ++i) { // parallel comparison
                for (int j = 0; j < u_len; ++j) {  // compute u_p, u_g
                    vec_loc = index_triple * 2 / 8;
                    bit_loc = (index_triple * 2) % 8;
                    index_triple++;
                    alpha = sendmsg[vec_loc][bit_loc] ^ rcvmsg[vec_loc][bit_loc]; // alpha = p1 -a
                    beta = sendmsg[vec_loc][bit_loc + 1] ^ rcvmsg[vec_loc][bit_loc + 1]; // beta = p2 - b
                    x_ = p[i][2 * j], y_ = p[i][2 * j + 1]; //x_ -- p1, y_ -- p2
                    z = c ^ (alpha & y_) ^ (beta & x_); // z = p1p2
                    if (this->myId() == 0) z ^= (alpha & beta);
                    u_p[i][j] = z;
                    vec_loc = index_triple * 2 / 8;
                    bit_loc = (index_triple * 2) % 8;
                    index_triple++;
                    alpha = sendmsg[vec_loc][bit_loc] ^ rcvmsg[vec_loc][bit_loc];  // open g1 -a
                    beta = sendmsg[vec_loc][bit_loc + 1] ^ rcvmsg[vec_loc][bit_loc + 1]; // open  p2 -b
                    x_ = g[i][2 * j], y_ = p[i][2 * j + 1]; // x_ -- g1 y_ -- p2
                    z = c ^ (alpha & y_) ^ (beta & x_); // z = p2g1
                    if (this->myId() == 0) z ^= (alpha & beta);
                    u_g[i][j] = g[i][2 * j + 1] ^ z; // u_g = g2 + p2g1
                }
            }
            if (index_triple < numTriples) std::cout << "triples amount error\n";
            if (k % 2 == 1) {
                u_len += 1;
                for (int i = 0; i < p.size(); ++i) {
                    u_p[i][u_len] = p[i][k - 1];
                    u_g[i][u_len] = g[i][k - 1];
                }
            }
            auto ret = CarryOutAux(u_p, u_g, u_len); // u_len : bit length
            return ret;
        } else {
            std::vector<bool> ret(g.size());
            for (int i = 0; i < ret.size(); ++i) {
                ret[i] = g[i][0];
            }
            return ret; // Actually only care g[..][0]
        }
    }

protected:
    std::vector<ClearType> lambda_xBinShr;
    bool a = false, b = false, c = false;   //Fake GMW Triples
};


#endif //MALICIOUS_PPML_GTZGATE_H
