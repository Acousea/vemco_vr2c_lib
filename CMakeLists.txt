cmake_minimum_required(VERSION 3.15)
project(vemco_vr2c_lib)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_TOOLCHAIN_FILE})

if (WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lws2_32")
endif()



# Find Boost
#find_package(Boost REQUIRED COMPONENTS asio)
find_package(boost_asio REQUIRED CONFIG)

# File glob recurse for src/
file(GLOB_RECURSE SRC_FILES
        "src/*.hpp"
        "src/*.cpp"
        "src/**/*.hpp"
        "src/**/*.cpp"
)

file(GLOB_RECURSE TEST_FILES
        "tests/*.hpp"
        "tests/*.cpp"
        "tests/**/*.hpp"
        "tests/**/*.cpp"
)

# Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/v1.15.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Library
add_library(vemco_vr2c_lib STATIC ${SRC_FILES})
target_include_directories(vemco_vr2c_lib PUBLIC src)
target_link_libraries(vemco_vr2c_lib PUBLIC Boost::asio)

# Executable for main
add_executable(vemco_vr2c main.cpp)
target_link_libraries(vemco_vr2c vemco_vr2c_lib)

# Tests
add_executable(tests ${TEST_FILES})
target_link_libraries(tests gtest_main vemco_vr2c_lib)

include(GoogleTest)
gtest_discover_tests(tests)