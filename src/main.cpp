#include "Engine/EngineStatics.hpp"

#include <iostream>


int main() {
    try {
        EngineStatics::Engine = new CEngine();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    delete EngineStatics::Engine;

    return EXIT_SUCCESS;
}
