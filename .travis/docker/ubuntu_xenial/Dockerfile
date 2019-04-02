FROM ubuntu:xenial

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

# Install
RUN apt-get install -y qttools5-dev libkf5attica-dev libqt5xmlpatterns5-dev
WORKDIR /build
VOLUME  ["/sources", "/build"]
