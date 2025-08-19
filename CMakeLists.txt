cmake_minimum_required(VERSION 3.20)

project(VulkanEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)  # MSYS2 provides a CMake config package

# Add source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Create executable
add_executable(VulkanEngine ${SOURCES} ${SHADERS})

target_link_libraries(VulkanEngine PRIVATE
    Vulkan::Vulkan   # links to vulkan-1 on Windows
    glfw             # target from glfw3 CONFIG
)