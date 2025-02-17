cmake_minimum_required(VERSION 3.15)
project(vemco_vr2c_lib)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_TOOLCHAIN_FILE})

set(TARGET_PLATFORM "x86_64" CACHE STRING "The target platform")
set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS "x86_64" "Arduino")
set_property(CACHE TARGET_PLATFORM PROPERTY VALUE "Arduino")

message("Selected platform: ${TARGET_PLATFORM}")

# File glob recurse for src/
file(GLOB_RECURSE SRC_FILES
        "src/*.hpp"
        "src/*.cpp"
        "src/**/*.hpp"
        "src/**/*.cpp"
)

# ----------------------- Library


# Include the BoostSerial and libaries only for x86_64
if (${TARGET_PLATFORM} STREQUAL "x86_64")
    message(STATUS "Building for x86_64")
    if (WIN32)
        set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lws2_32")
    endif ()
    # Find Boost
    #find_package(Boost REQUIRED COMPONENTS asio)
    find_package(boost_asio REQUIRED CONFIG)

    # Check if boost asio was found
    if (NOT boost_asio_FOUND)
        message(FATAL_ERROR "Boost ASIO not found")
        return()
    endif ()

    # Remove Arduino-specific files using REGEX
    list(FILTER SRC_FILES EXCLUDE REGEX ".*[/\\]ArduinoSerial[/\\].*")

    # Add BoostSerial-specific files
    add_library(vemco_vr2c_lib STATIC ${SRC_FILES})
    target_include_directories(vemco_vr2c_lib PUBLIC src)
    target_link_libraries(vemco_vr2c_lib PUBLIC Boost::asio)

elseif (${TARGET_PLATFORM} STREQUAL "Arduino")
    message(STATUS "Building for Arduino")
    # Remove BoostSerial-specific files using REGEX
    list(FILTER SRC_FILES EXCLUDE REGEX ".*[/\\]BoostSerial[/\\].*")

    # Add Arduino-specific files
    add_library(vemco_vr2c_lib STATIC ${SRC_FILES})
    target_include_directories(vemco_vr2c_lib PUBLIC src)

else ()
    message(FATAL_ERROR "Invalid TARGET_PLATFORM")
    return()
endif ()

# ----------------------- Executable
# Executable for main
add_executable(vemco_vr2c_exe main.cpp)
target_link_libraries(vemco_vr2c_exe vemco_vr2c_lib)

# --------------------------------- Tests
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

# Tests
add_executable(tests ${TEST_FILES})
target_link_libraries(tests gtest_main vemco_vr2c_lib)

include(GoogleTest)
gtest_discover_tests(tests)