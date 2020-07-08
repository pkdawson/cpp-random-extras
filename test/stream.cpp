#include "../xoshiro.hpp"
#include <random>
#include <chrono>
#include <iostream>

using namespace xoshiro;

// for testing with Dieharder, etc

int main()
{
    xoshiro256pp rng(std::chrono::system_clock::now().time_since_epoch().count());
    xoshiro256pp::result_type x;

    while (true) {
        x = rng();
        unsigned char* p = reinterpret_cast<unsigned char*>(&x);
        for (int b = 0; b < sizeof(x); ++b) {
            std::cout << p[b];
        }
    }
    return 0;
}
