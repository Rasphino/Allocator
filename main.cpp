#include "allocator.h"
#include <iostream>
#include <vector>

int main() {
    std::vector<int, MyLib::allocator<int>> v;
    v.push_back(112);
    v.push_back(314);
    v.push_back(113);
    v.push_back(234536);
    v.push_back(21);
    v.push_back(31);
    for (const auto& item : v) {
        std::cout << item << std::endl;
    }
    return 0;
}