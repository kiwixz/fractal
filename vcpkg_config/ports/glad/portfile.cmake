include(vcpkg_common_functions)

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Dav1dde/glad
    REF v0.1.29
    SHA512 056c0439b673d86bd10fe733873d4b82cf70861818980f08b3ec23f7b478750064a6d53925cc2cf9445e570cece5f21e12d7af6f1927b64af05d8d1f2afdedcb
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DGLAD_PROFILE=core
        -DGLAD_API=gl=4.5
        -DGLAD_GENERATOR=c
        -DGLAD_REPRODUCIBLE=ON
        -DGLAD_EXPORT=OFF
        -DGLAD_INSTALL=ON
)

vcpkg_install_cmake()
vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/glad)

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
configure_file(${SOURCE_PATH}/LICENSE ${CURRENT_PACKAGES_DIR}/share/glad/copyright COPYONLY)
