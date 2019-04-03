FROM opensuse:42.3

RUN zypper addrepo https://download.opensuse.org/repositories/devel:tools:building/openSUSE_Leap_42.3/devel:tools:building.repo
RUN zypper refresh --force --non-interactive

# Install libappimage build dependencies
RUN zypper --non-interactive install openssl-devel wget cmake git gcc-c++ \
    desktop-file-utils librsvg2-devel cairo-devel glib2-devel libarchive-devel \
    fuse-devel libboost_filesystem1_61_0 boost-devel vim automake libtool patch

# Build and install libappimage
RUN git clone https://github.com/AppImage/libappimage.git --depth=1 /tmp/libappimage
RUN cd /tmp/libappimage && git submodule update --init --recursive;
RUN cd /tmp/libappimage && \
    cmake . \
        -DBUILD_TESTING=OFF \
        -DUSE_SYSTEM_LIBARCHIVE=ON \
        -DUSE_SYSTEM_BOOST=ON\
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install

RUN zypper --non-interactive install qt5-qtxmlpatterns-devel qt5-qtnetwork-devel qt5-qtbase-devel attica-qt5

WORKDIR /build
VOLUME  ["/sources", "/build"]
