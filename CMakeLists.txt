cmake_minimum_required(VERSION 3.8)
project(fractal)


function ("local_lib" name)
    file(GLOB_RECURSE src "${name}/*.cpp" "${name}/*.cxx")
    add_library("${name}" "${src}")
endfunction ()

function ("local_exe" name)
    cmake_parse_arguments(args "" "NAME" "DEPENDS;LIBRARIES" "${ARGN}")
    if (NOT "${args_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "unrecognized options: ${args_UNPARSED_ARGUMENTS}")
    endif ()

    file(GLOB_RECURSE src "${name}/*.cpp" "${name}/*.cxx")
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
set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS}       \
        -Wall -Wextra -Wpedantic  \
        -Wcast-align -Wcast-qual -Wdisabled-optimization -Wduplicated-branches             \
        -Wduplicated-cond -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations       \
        -Wmissing-include-dirs -Wnoexcept -Wnull-dereference -Wold-style-cast              \
        -Woverloaded-virtual -Wredundant-decls -Wrestrict -Wshadow -Wstrict-null-sentinel  \
        -Wundef -Wuseless-cast -Wzero-as-null-pointer-constant"
    )
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -ffast-math -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")
    set(CMAKE_CXX_FLAGS_DEBUG
        "-Og -g  \
        -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract  \
        -fsanitize=leak -fsanitize=undefined"
    )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# /diagnostics:caret would be better but seems not detected by VS in the IDE
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /nologo /MP /diagnostics:column /W4 /permissive- /EHsc")
	set(CMAKE_CXX_FLAGS_DEBUG "/MDd /JMC /RTC1 /ZI")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /GL /Gw /fp:fast /D NDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NOLOGO")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FASTLINK")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG:INCREMENTAL")
	set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG:FULL")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
	set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
	set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "")
	set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
	set(CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}")
endif ()


include_directories("${CMAKE_SOURCE_DIR}")

find_package("fmt" CONFIG REQUIRED)
find_package("glfw3" CONFIG REQUIRED)
find_package("spdlog" CONFIG REQUIRED)


local_lib("glad")

local_exe("fractal"
    DEPENDS
        "glad"
    LIBRARIES
        "fmt::fmt"
        "glfw"
        "spdlog::spdlog"
        "tinyfiledialogs"
)


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options("glad" PRIVATE "-w")
    target_link_libraries("fractal" "stdc++fs")
endif ()
