cmake_minimum_required(VERSION 3.21)
project(ClawUtils LANGUAGES CXX)

# LIBRARY

file(GLOB_RECURSE CLAWUTILS_SRC
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_library(
    ClawUtils 

    INTERFACE
    ${CLAWUTILS_SRC}
)

target_compile_features(
    ClawUtils

    INTERFACE
        cxx_std_20
)

target_include_directories(
    ClawUtils

    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_precompile_headers(
    ClawUtils

    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ClawSTL.hpp
)

# TESTING

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE CLAWUTILS_TESTS_SRC
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

add_executable(
    ClawUtilsTests

    ${CLAWUTILS_TESTS_SRC}
)

target_link_libraries(ClawUtilsTests
    PRIVATE
        ClawUtils
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ClawUtilsTests)
