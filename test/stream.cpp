#include "../xoshiro.hpp"
#include <random>
#include <chrono>
#include <iostream>

int main()
{
    xoshiro64 rng(std::chrono::system_clock::now().time_since_epoch().count());
    xoshiro64::result_type x;

    // xoshiro64
    // 1.22e+07
    // WEAK: diehard_sums

    while (true) {
        x = rng();
        unsigned char* p = reinterpret_cast<unsigned char*>(&x);
        for (int b = 0; b < sizeof(x); ++b) {
            std::cout << p[b];
        }
    }
    return 0;
}
