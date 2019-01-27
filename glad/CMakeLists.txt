cmake_minimum_required(VERSION 3.8)


configure_file("CMakeLists.txt.in" "external/CMakeLists.txt")
execute_process(
    COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" "."
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/external"
)
execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "."
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/external"
)


set(GLAD_PROFILE "core")
set(GLAD_API "gl=4.5")
set(GLAD_GENERATOR "c")
set(GLAD_REPRODUCIBLE "ON")
set(GLAD_EXPORT "OFF")
set(GLAD_INSTALL "OFF")


add_subdirectory("${CMAKE_CURRENT_BINARY_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/build")


target_include_directories("glad" INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
