cmake_minimum_required(VERSION 3.20)

project(VulkanEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_STATIC "Attempt to link static CRT and libraries" ON)

if(BUILD_STATIC)
    message(STATUS "Configuring for static linking on MinGW/msys2/ucrt64")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

if(BUILD_STATIC)
    find_package(ZLIB REQUIRED)
endif()

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

# === Link Libraries ===

target_link_libraries(VulkanEngine PRIVATE
    Vulkan::Vulkan
    glm::glm
)

if(BUILD_STATIC)
    target_link_libraries(VulkanEngine PRIVATE
        C:/msys64/ucrt64/lib/libglfw3.a
        C:/msys64/ucrt64/lib/libassimp.a
        C:/msys64/ucrt64/lib/libzlibstatic.a
    )
else()
    target_link_libraries(VulkanEngine PRIVATE
        glfw
        assimp::assimp
    )
endif()
