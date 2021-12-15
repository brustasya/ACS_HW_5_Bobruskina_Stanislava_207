cmake_minimum_required(VERSION 3.20)
project(HW_5 C)

# project name and language
project(task1 LANGUAGES CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS -pthread)
set(BUILD_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")

set(BINARY_DIR "${CMAKE_BINARY_DIR}")

# Source files list
set(SOURCE_FILES
        main.c
        declaration.h)

add_executable(HW_5 ${SOURCE_FILES})
target_link_libraries(HW_5 m)
