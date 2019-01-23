cmake_minimum_required(VERSION 3.8)
project(fractal)


function ("local_lib" name)
    file(GLOB_RECURSE src "${name}/*.cpp")
    add_library("${name}" "${src}")
endfunction ()

function ("local_exe" name)
    cmake_parse_arguments(args "" "NAME" "DEPENDS;LIBRARIES" "${ARGN}")
    if (NOT "${args_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "my_exe: unrecognized options: ${args_UNPARSED_ARGUMENTS}")
    endif ()

    file(GLOB_RECURSE src "${name}/*.cpp")
    add_executable("${name}" "${src}")

    if (NOT "${args_DEPENDS}" STREQUAL "")
        add_dependencies("${name}" "${args_DEPENDS}")
        target_link_libraries("${name}" "${args_DEPENDS}")
    endif ()
    if (NOT "${args_LIBRARIES}" STREQUAL "")
        target_link_libraries("${name}" "${args_LIBRARIES}")
    endif ()
endfunction ()


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
find_package("glfw3" CONFIG REQUIRED)
find_package("spdlog" CONFIG REQUIRED)


local_lib("glad")

local_exe("tetris"
    DEPENDS
        "glad"
    LIBRARIES
        "fmt::fmt"
        "glfw"
        "spdlog::spdlog"
)
