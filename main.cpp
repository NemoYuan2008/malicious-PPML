#include <iostream>
#include <bitset>
#include <vector>
#include "utils/fixedPoint.h"

using std::cin;
using std::cout;
using std::hex;

std::vector<bool> CarryOutAux(std::vector<std::bitset<sizeof(uint32_t) * 8>> p,
                              std::vector<std::bitset<sizeof(uint32_t) * 8>> g, int k) {  //k bits, (p2,g2)*(p1,g1) = (p2p1,g2+p2g1)

    if (k > 1) {
        int u_len = k / 2; // round down bit length, if k%2=1, push back the last one bit at the end
        std::vector<std::bitset<sizeof(uint32_t) * 8>> u_p(p.size(), 0);
        std::vector<std::bitset<sizeof(uint32_t) * 8>> u_g(p.size(), 0);
        for (int i = 0; i < g.size(); ++i) { // parallel comparison
            for (int j = 0; j < u_len; ++j) {  // compute u_p, u_g
                u_p[i][j] = p[i][2 * j + 1] & p[i][2 * j];
                u_g[i][j] = g[i][2 * j + 1] ^ (p[i][2 * j + 1] & g[i][2*j]); // u_g = g2 + p2g1
            }
        }
        auto ret = CarryOutAux(u_p, u_g, u_len); // u_len : bit length
        return ret;
    } else {
        std::vector<bool> ret(p.size());
        for (int i = 0; i < p.size(); ++i) {
            ret[i] = g[i][0];
            return ret; // Actcually only care g[..][0]
        }
    }
}

std::vector<bool> CarryOutCin(std::vector<std::bitset<sizeof(uint32_t) * 8>> &aIn,
                              std::vector<std::bitset<sizeof(uint32_t) * 8>> &bIn, bool cIn){ //a<-delta_x, b<-lambda_xBinShr

    std::vector<std::bitset<sizeof(uint32_t) * 8>> p(bIn.size());
    std::vector<std::bitset<sizeof(uint32_t) * 8>> g(bIn.size());
    //compute p[i] = a[i]^b[i], g[i] = a[i]*b[i]
    int numBits = sizeof(uint32_t)*8;
    for (int i = 0; i < bIn.size(); ++i) {
        for (int j=0; j<numBits; j++){
            p[i][j] = aIn[i][j]^bIn[i][j]; //p = a+b -2ab
            g[i][j] = aIn[i][j]&bIn[i][j]; //g = a*b
        }
    }
    for (int i = 0; i < bIn.size(); ++i) {
        g[i][0] = g[i][0] ^ (cIn&p[i][0]); // g1 = g1 + c*p1
    }
    return CarryOutAux(p,g,numBits);
}

std::vector<bool> BitLT(std::vector<uint32_t> &pInt, // output s = (pInt < sInt)
                        std::vector<uint32_t> &sInt){
    std::vector<std::bitset<sizeof(uint32_t) * 8>> b_(sInt.size());
    std::vector<std::bitset<sizeof(uint32_t) * 8>> a_(pInt.size());
    for (int i = 0; i < sInt.size(); ++i) {
        b_[i] = ~sInt[i]; //b_[i][j] = 1 - b[i][j]
        a_[i] = pInt[i];
    }
    auto s = CarryOutCin(a_,b_,1);
    for (int i = 0; i < s.size(); ++i) {
        s[i] = !s[i]; //s[i] = 1 -s[i]
    }
    return s;
}


int main() {
//    cout << hex;
//    double x = -132.35, y = 230.165;
//
//    auto xFix = double2fix<uint64_t>(x);
//    auto yFix = double2fix<uint64_t>(y);
//    auto zFix = static_cast<int64_t>(xFix * yFix) >> 8;
//    cout << x * y << ' ' << fix2double(zFix);
    uint32_t shr1, shr2, delta_x,lambda_x;
//    delta_x = 578495535;
//    shr1 = 3363169791;
//    shr2 = 3926835663;
//    lambda_x = shr1 ^ shr2;
//    cout<< "delta_x: "<< delta_x <<"\n" << "lambda_x: " << lambda_x <<"\n";
    uint32_t n = 1;
    std::vector<std::bitset<sizeof (uint32_t)*8>> a(1);
    std::vector<std::bitset<sizeof (uint32_t)*8>> b(1);
    std::vector<std::bitset<sizeof (uint32_t)*8>> p(1);
    std::vector<std::bitset<sizeof (uint32_t)*8>> g(1);
    std::vector<uint32_t> x(1);
    std::vector<uint32_t> y(1);
    x[0] = 3875437382;
    y[0] = 3875437381;
    a[0] = 254;
    b[0] = 0;
    std::cout<<"a: "<<a[0]<<"\n b: "<<b[0]<<"\n";
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < sizeof (uint32_t)*8; ++j) {
            cout<< "a,b: "<< a[i][j]<<" "<<b[i][j]<<"\n";
            p[i][j] = a[i][j]^b[i][j];
            g[i][j] = a[i][j]&b[i][j];
        }
    }
    auto ret = BitLT(x,y);
    cout<< "ret value: "<<ret[0]<<"\n";
//    bool a=false,b=false,c=false;
//    bool z, x ,y, alpha, beta,z1,z2;
//    bool s1x,s2x, s1y, s2y;
//    x =0,y=0;
//    cout << (x & y) <<"\n";
//    s1x = 0, s1y = 0;
//    s2x = x^s1x, s2y = y^s1y;
//    alpha = x^a, beta = y ^ b;
//    z1 = c ^ (alpha & s1y) ^ (beta & s1x) ^ (alpha & beta);
//    z2 = c ^ (alpha & s2y) ^ (beta & s2x);
//    cout <<z1<<z2 <<"\n";
    return 0;
}
