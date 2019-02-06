FROM kiwixz/arch-vcpkg:2019-01-20

WORKDIR "/mnt/repo"

RUN pacman --color=always --needed --noconfirm -Sy  \
        clang cmake lld ninja python3  \
        clang diffutils  \
        sed  `# glad`  \
        libxcursor libxinerama libxrandr  `# glfw`  \
        yasm  `# libvpx`  \
    \
    && mv "/opt/vcpkg" "./"  \
    && ln -s "/mnt/repo/vcpkg" "/opt/vcpkg"

COPY "vcpkg_config/" "/mnt/repo/vcpkg_config/"
COPY "install_libs.py" "/mnt/repo/"
RUN bash -l -c 'python3 "install_libs.py"'  \
    && git -C "vcpkg" gc --aggressive --prune=all  \
    && kiwixz_cleanup_vcpkg

COPY "." "/mnt/repo/"
CMD ["/bin/bash"]
