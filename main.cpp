#include <iostream>
#include <thread>
#include "lib.h"

int main(int, char**){
    std::cout << "Version: " << version() << std::endl;
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
