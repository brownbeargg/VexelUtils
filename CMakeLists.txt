cmake_minimum_required(VERSION 3.21)
project(VexelUtils LANGUAGES CXX)

# LIBRARY

file(GLOB_RECURSE CLAWUTILS_SRC
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_library(
    VexelUtils 

    STATIC
    ${CLAWUTILS_SRC}
)

target_compile_features(
    VexelUtils

    PUBLIC
        cxx_std_20
)

target_include_directories(
    VexelUtils

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/source

        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
)

target_precompile_headers(
    VexelUtils

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Vexel/STL.hpp
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
    VexelUtilsTests

    ${CLAWUTILS_TESTS_SRC}
)

target_link_libraries(VexelUtilsTests
    PRIVATE
        VexelUtils
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(VexelUtilsTests)
