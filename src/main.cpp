#include "VulkanEngine.hpp"

#include <iostream>


int main() {
    CVulkanEngine app;

    try {
        app.Run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
