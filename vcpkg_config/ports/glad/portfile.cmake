include(vcpkg_common_functions)

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Dav1dde/glad
    REF v0.1.28
    SHA512 5895e25bffab4ead346f65011cbb52a434779fed1dad619ffde27f68b52994cd0b048a45b4533b22b54971dfdba935d93fcf6bee5789061d0057af869b95998c
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

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include ${CURRENT_PACKAGES_DIR}/include/KHR)
configure_file(${SOURCE_PATH}/LICENSE ${CURRENT_PACKAGES_DIR}/share/glad/copyright COPYONLY)
