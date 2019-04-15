FROM debian:buster

RUN apt-get update;

# Install libappimage build dependencies
RUN apt-get install -y git g++ automake cmake build-essential libtool wget xxd desktop-file-utils \
    librsvg2-bin libcairo2-dev libglib2.0-dev libarchive-dev libfuse-dev liblzma-dev libboost-filesystem-dev

# Build and install libappimage
RUN git clone https://github.com/AppImage/libappimage.git --depth=1 /tmp/libappimage
RUN git -C /tmp/libappimage submodule update --init --recursive;
RUN cd /tmp/libappimage && \
    cmake . \
        -DBUILD_TESTING=OFF \
        -DUSE_SYSTEM_LIBARCHIVE=ON \
        -DUSE_SYSTEM_XZ=ON\
        -DUSE_SYSTEM_BOOST=ON\
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install

# Build and install kf5-attica
RUN apt-get install -y extra-cmake-modules qttools5-dev libqt5xmlpatterns5-dev

RUN git clone git://anongit.kde.org/attica.git --depth=1 -b 'v5.54.0' /tmp/attica
RUN cd /tmp/attica && git submodule update --init --recursive;

RUN cd /tmp/attica &&\
    cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install


# Install xdg-utils
RUN git clone https://github.com/azubieta/xdg-utils-cxx.git --depth=1 /tmp/xdg_utils_cxx
RUN cd /tmp/xdg_utils_cxx && \
    git submodule update --init --recursive && \
    cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install

# Install yaml-cpp
RUN git clone https://github.com/jbeder/yaml-cpp.git --depth=1 /tmp/yaml-cpp
RUN cd /tmp/yaml-cpp && \
    git submodule update --init --recursive && \
    cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install

WORKDIR /build
VOLUME  ["/sources", "/build"]
