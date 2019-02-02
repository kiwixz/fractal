FROM kiwixz/arch-vcpkg:2019-01-20

WORKDIR "/mnt/repo"

RUN pacman --color=always --needed --noconfirm -Sy  \
        python3  \
        cmake gcc ninja  \
        clang diffutils  \
        libxcursor libxinerama libxrandr sed  \
    \
    && mv "/opt/vcpkg" "./"  \
    && ln -s "/mnt/repo/vcpkg" "/opt/vcpkg"

COPY "install_libs.py" "vcpkg_config" "/mnt/repo/"
RUN bash -l -c 'python3 "install_libs.py"'  \
    \
    && git -C "vcpkg" gc --aggressive --prune=all  \
    && kiwixz_cleanup_vcpkg

COPY "." "/mnt/repo/"
CMD ["/bin/bash"]
