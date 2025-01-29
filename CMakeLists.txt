cmake_minimum_required(VERSION 3.15)
project(vemco_vr2c_lib)

set(CMAKE_CXX_STANDARD 17)

# File glob recurse for src/
file(GLOB_RECURSE SRC_FILES
        "src/*.hpp"
        "src/*.cpp"
        "src/**/*.hpp"
        "src/**/*.cpp"
)

add_library(vemco_vr2c_lib STATIC
        ${SRC_FILES}
)

# Executable
add_executable(vemco_vr2c main.cpp)
