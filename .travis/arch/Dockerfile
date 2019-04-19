FROM archlinux/base

RUN pacman --noconfirm -Sy base-devel git cmake wget vim desktop-file-utils cairo fuse boost qt5-base qt5-xmlpatterns attica libappimage librsvg

# Install xdg-utils
# Install xdg-utils
RUN git clone https://github.com/azubieta/xdg-utils-cxx.git --depth=1 /tmp/xdg_utils_cxx
RUN cd /tmp/xdg_utils_cxx && \
    git submodule update --init --recursive && \
    cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install && \
    cd / && rm -rf /tmp/xdg_utils_cxx

# Install yaml-cpp
RUN git clone https://github.com/jbeder/yaml-cpp.git --depth=1 /tmp/yaml-cpp
RUN cd /tmp/yaml-cpp && \
    git submodule update --init --recursive && \
    cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install && \
    cd / && rm -rf /tmp/yaml-cpp


RUN useradd build
USER build

WORKDIR /build
VOLUME  ["/sources", "/build"]
