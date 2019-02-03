cmake_minimum_required(VERSION 3.12)

set(target "fractal")

file(GLOB_RECURSE src "*.cpp")
add_executable("${target}" "${src}")


find_package("cxxopts" CONFIG REQUIRED)
find_package("fmt" CONFIG REQUIRED)
find_package("glad" CONFIG REQUIRED)
find_package("glfw3" CONFIG REQUIRED)
find_package("spdlog" CONFIG REQUIRED)
find_package("tinyfiledialogs" CONFIG REQUIRED)

target_link_libraries("${target}"
    "cxxopts::cxxopts"
    "fmt::fmt"
    "glad::glad"
    "glfw"
    "spdlog::spdlog"
    "tinyfiledialogs::tinyfiledialogs"
)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries("${target}" "stdc++fs")
endif ()
