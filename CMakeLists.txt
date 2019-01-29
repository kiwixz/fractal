cmake_minimum_required(VERSION 3.8)
project(fractal)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    message(STATUS "Build type unset, choosing ${CMAKE_BUILD_TYPE}")
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_CXX_STANDARD "17")
set(CMAKE_CXX_STANDARD_REQUIRED "ON")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS}  \
        -Wall -Wextra -Wpedantic  \
        -Wcast-align -Wcast-qual -Wdisabled-optimization -Wduplicated-branches  \
        -Wduplicated-cond -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations  \
        -Wmissing-include-dirs -Wnoexcept -Wnull-dereference -Wold-style-cast  \
        -Woverloaded-virtual -Wredundant-decls -Wrestrict -Wstrict-null-sentinel -Wundef  \
        -Wuseless-cast -Wzero-as-null-pointer-constant"
    )
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -ffast-math -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG
        "-Og -g  \
        -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract  \
        -fsanitize=leak -fsanitize=undefined"
    )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /nologo /MP /diagnostics:caret /W4 /permissive- /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /JMC /RTC1 /ZI")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /GL /Gw /Zi /fp:fast /D NDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NOLOGO")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FASTLINK")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG:INCREMENTAL /DEBUG:FULL")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "")    # /DEBUG not silently ignored before VS2019
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "")  #
endif ()


add_subdirectory("glad")
add_subdirectory("fractal")
