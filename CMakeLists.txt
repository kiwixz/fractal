cmake_minimum_required(VERSION 3.8)
project(fractal)


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS}       \
    -O3 -ffast-math           \
    -Wall -Wextra -Wpedantic  \
    -Wcast-align -Wcast-qual -Wdisabled-optimization -Wduplicated-branches             \
    -Wduplicated-cond -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations       \
    -Wmissing-include-dirs -Wnoexcept -Wnull-dereference -Wold-style-cast              \
    -Woverloaded-virtual -Wredundant-decls -Wrestrict -Wshadow -Wstrict-null-sentinel  \
    -Wundef -Wuseless-cast -Wzero-as-null-pointer-constant"
)
set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")

include_directories("${CMAKE_SOURCE_DIR}")

find_package("fmt" CONFIG REQUIRED)
find_package("spdlog" CONFIG REQUIRED)


file(GLOB_RECURSE src "fractal/*.cpp")
add_executable("fractal" "${src}")
target_link_libraries("fractal"
    "fmt::fmt"
    "spdlog::spdlog"
)
