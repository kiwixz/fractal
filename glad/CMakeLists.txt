cmake_minimum_required(VERSION 3.8)

set(target "glad")

file(GLOB_RECURSE src "glad.c")
add_library("${target}" "${src}")
