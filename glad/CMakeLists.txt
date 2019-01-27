cmake_minimum_required(VERSION 3.11)
include("FetchContent")

set(target "glad")

FetchContent_Populate(
    "${target}"
    GIT_REPOSITORY "https://github.com/Dav1dde/glad.git"
    GIT_TAG "v0.1.28"
)

set(GLAD_PROFILE "core")
set(GLAD_API "gl=4.5")
set(GLAD_GENERATOR "c")
set(GLAD_REPRODUCIBLE "ON")
set(GLAD_EXPORT "OFF")
set(GLAD_INSTALL "OFF")

add_subdirectory("${${target}_SOURCE_DIR}" "${${target}_BINARY_DIR}")

target_include_directories("glad" INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
