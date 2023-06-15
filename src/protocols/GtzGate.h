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
        auto ret = BitLT(delta_x, this->lambda_xBinShr);
#ifndef NDEBUG
        std::cout << "delta_x: ";
        printVector(delta_x);
        std::cout<< "Lambda_xBinShr: ";
        printVector(this->lambda_xBinShr);
        std::cout << "\n Ret value of BitLT:";
        printVector(ret);
#endif
    }

    std::vector<bool> BitLT(std::vector<ClearType> &pInt, // output s = (pInt < sInt)
                            std::vector<ClearType> &sInt){
        std::vector<std::bitset<sizeof(ClearType) * 8>> b_(sInt.size());
        std::vector<std::bitset<sizeof(ClearType) * 8>> a_(pInt.size());
        for (int i = 0; i < sInt.size(); ++i) {
            b_[i] = ~sInt[i]; //b_[i][j] = 1 - b[i][j]
            a_[i] = pInt[i];
        }
        auto s = CarryOutCin(a_,b_,1);
        for (int i = 0; i < s.size(); ++i) {
            s[i] = 1 ^ s[i]; //s[i] = 1 -s[i]
        }
        return s;
    }

    std::vector<bool> CarryOutCin(std::vector<std::bitset<sizeof(ClearType) * 8>> &aIn,
                    std::vector<std::bitset<sizeof(ClearType) * 8>> &bIn, bool cIn){ //a<-delta_x, b<-lambda_xBinShr

        std::vector<std::bitset<sizeof(ClearType) * 8>> p(bIn.size());
        std::vector<std::bitset<sizeof(ClearType) * 8>> g(bIn.size());
        //compute p[i] = a[i]^b[i], g[i] = a[i]*b[i]
        int numBits = sizeof(ClearType)*8;
        for (int i = 0; i < bIn.size(); ++i) {
            for (int j=0; j<numBits; j++){
                p[i][j] = aIn[i][j]^bIn[i][j]; //p = a+b -2ab
                g[i][j] = aIn[i][j]&bIn[i][j]; //g = a*b
            }
        }
        for (int i = 0; i < bIn.size(); ++i) {
            g[i][0] = g[i][0] + cIn*p[i][0]; // g1 = g1 + c*p1
        }
        return CarryOutAux(p,g,numBits);
    }

    std::vector<bool> CarryOutAux(std::vector<std::bitset<sizeof(ClearType) * 8>> &p,
                             std::vector<std::bitset<sizeof(ClearType) * 8>> &g, int k){  //k bits, (p2,g2)*(p1,g1) = (p2p1,g2+p2g1)
        if (k>1){
            int u_len = k/2; // round down bit length, if k%2=1, push back the last one bit at the end
            std::vector<std::bitset<sizeof(ClearType) * 8>> u_p(p.size());
            std::vector<std::bitset<sizeof(ClearType) * 8>> u_g(p.size());
            // compute u[k/2..1] = (d[2k] * d[2k-1],...)---- compute p2*p1 and g2*p1 need 2 triples
            // (k/2)*2 triples per invocation
            int numTriples = g.size()*u_len*2; //parallel g.size() comparisons
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
            for (int i = 0; i < g.size(); ++i) { // parallel comparison
                for (int j =0; j< u_len; ++j) {
                    vec_loc = index_triple*2/8;
                    bit_loc = (index_triple*2)%8;
                    index_triple++;
                    sendmsg[vec_loc][bit_loc] = p[i][2*j] ^ a; //[p1] -[a]
                    sendmsg[vec_loc][bit_loc+1] = p[i][2*j+1] ^ b; //[p2] -[b]
                    vec_loc = index_triple*2/8;
                    bit_loc = (index_triple*2)%8;
                    index_triple++;
                    sendmsg[vec_loc][bit_loc] = p[i][2*j] ^ a; //[p1] -[a]
                    sendmsg[vec_loc][bit_loc+1] = g[i][2*j+1] ^ b; //[g2] -[b]
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
            //compute
            bool alpha, beta, z;
            index_triple = 0;
            for (int i = 0; i < g.size(); ++i) { // parallel comparison
                for (int j =0; j< u_len; ++j) {  // compute u_p, u_g
                    vec_loc = index_triple*2/8;
                    bit_loc = (index_triple*2)%8;
                    index_triple++;
                    alpha = sendmsg[vec_loc][bit_loc] ^ rcvmsg[vec_loc][bit_loc]; // alpha = p1 -a
                    beta = sendmsg[vec_loc][bit_loc+1] ^ rcvmsg[vec_loc][bit_loc+1]; // beta = p2 - b
                    z = c ^ (alpha & b) ^ (beta & a) ^ (alpha & beta); // z = p1p2
                    u_p[i][j] = z;
                    vec_loc = index_triple*2/8;
                    bit_loc = (index_triple*2)%8;
                    index_triple++;
                    alpha = sendmsg[vec_loc][bit_loc] + rcvmsg[vec_loc][bit_loc];  // open p1 -a
                    beta = sendmsg[vec_loc][bit_loc+1] + rcvmsg[vec_loc][bit_loc+1]; // open  g2 -b
                    z = c ^ (alpha & b) ^ (beta & a) ^ (alpha & beta); // z = g2p1
                    u_g[i][j] = g[i][2*j+1] ^ z; // u_g = g2 + g2p1
                }
            }
            if (k%2 == 1){
                u_len += 1;
                for (int i = 0; i < p.size(); ++i) {
                    u_p[i][u_len] = p[i][k-1];
                    u_g[i][u_len] = g[i][k-1];
                }
            }
            return CarryOutAux(u_p,u_g,u_len); // u_len : bit length
        }
        else{
            std::vector<bool> rst(g.size());
            for (int i = 0; i < rst.size(); ++i) {
                rst[i] = g[i][0];
            }
            return rst; // Actcually only care g[..][0]
        }
    }
protected:
    std::vector<ClearType> lambda_xBinShr;
    bool a = false, b = false, c = false;   //Fake GMW Triples
};


#endif //MALICIOUS_PPML_GTZGATE_H
