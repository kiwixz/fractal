include(vcpkg_common_functions)

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_bitbucket(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO multicoreware/x265
    REF 3.0
    SHA512 698fd31bf30c65896717225de69714523bcbd3d835474f777bf32c3a6d6dbbf941a09db076e13e76917a5ca014c89fca924fcb0ea3d15bc09748b6fc834a4ba2
    HEAD_REF master
)

if (WIN32)
    vcpkg_find_acquire_program(NASM)
    get_filename_component(nasm_path ${NASM} DIRECTORY)
    set(ENV{PATH} "$ENV{PATH};${nasm_path}")
endif ()

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}/source
    PREFER_NINJA
    OPTIONS
        -DENABLE_ASSEMBLY=ON
        -DENABLE_CLI=OFF
        -DENABLE_SHARED=OFF
)

vcpkg_install_cmake()

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
configure_file(${SOURCE_PATH}/COPYING ${CURRENT_PACKAGES_DIR}/share/x265/copyright COPYONLY)
