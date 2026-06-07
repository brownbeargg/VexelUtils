cmake_minimum_required(VERSION 3.21)
project(VexelUtils LANGUAGES CXX)

# LIBRARY

file(GLOB_RECURSE VEXELUTILS_SRC
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

if(WIN32)
    file(GLOB_RECURSE VEXELUTILS_SRC_PLATFORM
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/*.cpp
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    file(GLOB_RECURSE VEXELUTILS_SRC_PLATFORM
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Platform/MacOS/*.cpp
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    file(GLOB_RECURSE VEXELUTILS_SRC_PLATFORM
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/Platform/Linux/*.cpp
    )
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

add_library(
    VexelUtils 

    STATIC
    ${VEXELUTILS_SRC}
    ${VEXELUTILS_SRC_PLATFORM}
)

target_compile_definitions(VexelUtils PUBLIC
    $<$<CONFIG:Debug>:VEX_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:VEX_DEBUG>
    $<$<CONFIG:Release>:VEX_RELEASE>
    $<$<CONFIG:MinSizeRel>:VEX_RELEASE>
)

target_compile_features(
    VexelUtils

    PUBLIC
        cxx_std_20
)

target_include_directories(
    VexelUtils

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Source

        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
)

target_precompile_headers(
    VexelUtils

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/Vexel/Utils.hpp
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
