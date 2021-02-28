#include <iostream>
#include "bitarray.hpp"



int main(int argc, char** argv){
    bit_array<7,unsigned short> ba7;
    ba7.v.push_back(1 | (1 << 7) | (1 << 14) );ba7.v.push_back(1);
    auto v1 = ba7[0];
    auto v2 = ba7[1];
    auto v3 = ba7[2];

    std::cout << v1 << " "; //expect 1
    std::cout << v2 << " "; //expect 1
    std::cout << v3 << " "; //expect 5

    std::cout << "\n";



}