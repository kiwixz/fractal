cmake_minimum_required(VERSION 3.8)

set(target "fractal")

file(GLOB_RECURSE src "*.cpp")
add_executable("${target}" "${src}")


find_package("fmt" CONFIG REQUIRED)
find_package("glfw3" CONFIG REQUIRED)
find_package("spdlog" CONFIG REQUIRED)

add_dependencies("${target}"
    "glad"
)
target_link_libraries("${target}"
    "glad"

    "fmt::fmt"
    "glfw"
    "spdlog::spdlog"
    "tinyfiledialogs"
)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries("${target}" "stdc++fs")
endif ()
