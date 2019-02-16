cmake_minimum_required(VERSION 3.12)

set(target "fractal")

file(GLOB_RECURSE src "*.cpp")
list(REMOVE_ITEM src "main.cpp")
list(FILTER src EXCLUDE REGEX ".test.cpp$")

add_library("${target}_base" "${src}")
add_executable("${target}" "main.cpp")
target_link_libraries("${target}" "${target}_base")

if ("${BUILD_TESTS}")
    file(GLOB_RECURSE test_src "*.test.cpp")
    add_executable("${target}_test" "${test_src}")
    target_link_libraries("${target}_test" "${target}_base")
    add_test(NAME "${target}_test" COMMAND "${target}_test")
endif ()


find_library(x265_LIB NAMES "x265" "x265-static")

find_package("cxxopts" CONFIG REQUIRED)
find_package("doctest" CONFIG REQUIRED)
find_package("fmt" CONFIG REQUIRED)
find_package("glad" CONFIG REQUIRED)
find_package("glfw3" CONFIG REQUIRED)
find_package("spdlog" CONFIG REQUIRED)
find_package("tinyfiledialogs" CONFIG REQUIRED)

target_link_libraries("${target}_base"
    "${x265_LIB}"

    "cxxopts::cxxopts"
    "doctest::doctest"
    "fmt::fmt"
    "glad::glad"
    "glfw"
    "spdlog::spdlog"
    "tinyfiledialogs::tinyfiledialogs"
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries("${target}_base" "stdc++fs")
endif ()
