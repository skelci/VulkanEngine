#include "Engine/Engine.hpp"

#include <iostream>


int main() {
    CEngine* Engine;

    try {
        Engine = new CEngine();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    delete Engine;

    return EXIT_SUCCESS;
}
