#include "EngineStatics.hpp"

#include <iostream>


int main() {
    try {
        new CEngine();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    delete GEngine;

    return EXIT_SUCCESS;
}
