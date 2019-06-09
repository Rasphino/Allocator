#include "allocator.h"
#include <iostream>
#include <vector>

int main() {
    char* a = new char[10];
    char* b = a + 3;
    std::cout << b - a << std::endl;
    return 0;
}