FROM kiwixz/arch-vcpkg:2019-01-20

WORKDIR "/mnt/repo"
COPY "*.py" "/mnt/repo/"

RUN echo -e "\n > Installing system packages..."  \
    && pacman --color=always --needed --noconfirm -Sy  \
        python3  \
        cmake gcc ninja  \
        clang diffutils  \
    \
    && echo -e "\n > Installing dependencies..."  \
    && mv "/opt/vcpkg" "./"  \
    && ln -s "/mnt/repo/vcpkg" "/opt/vcpkg"  \
    && bash -l -c 'python3 "install_libs.py"'  \
    \
    && echo -e "\n > Cleaning up..."  \
    && git -C "vcpkg" gc --aggressive --prune=all  \
    && kiwixz_cleanup_vcpkg

COPY "." "/mnt/repo/"
CMD ["/bin/bash"]
