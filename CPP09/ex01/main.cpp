#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    bool ok = false;
    int result = RPN::evaluate(argv[1], ok);
    if (!ok) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    std::cout << result << std::endl;
    return 0;
}
