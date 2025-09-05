cmake_minimum_required(VERSION 3.20)

project(VulkanEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(VulkanEngine ${SOURCES})

target_link_libraries(VulkanEngine PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
)
