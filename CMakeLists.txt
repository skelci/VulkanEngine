cmake_minimum_required(VERSION 3.20)

project(VulkanEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

set(ENGINE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Engine/Source)
set(GAME_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Game/Source)

file(GLOB_RECURSE SOURCES
    ${ENGINE_SOURCE_DIR}/*.cpp
    ${GAME_SOURCE_DIR}/*.cpp
)

add_executable(VulkanEngine ${SOURCES})

target_include_directories(VulkanEngine PRIVATE "C:/msys64/ucrt64/include")
target_include_directories(VulkanEngine PRIVATE ${ENGINE_SOURCE_DIR})
target_include_directories(VulkanEngine PRIVATE ${ENGINE_SOURCE_DIR}/Core)
target_include_directories(VulkanEngine PRIVATE ${GAME_SOURCE_DIR})

target_link_libraries(VulkanEngine PRIVATE
    Vulkan::Vulkan
    glfw
    glm::glm
    assimp::assimp
)
