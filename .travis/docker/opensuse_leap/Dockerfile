FROM opensuse:leap

RUN zypper update
RUN zypper groupinstall "Development Tools"
RUN zypper install yum-plugin-ovl centos-release-scl epel-release
RUN zypper install devtoolset-4-gcc* openssl-devel wget cmake git

# Install libappimage build dependencies
RUN zypper install desktop-file-utils librsvg2-devel cairo-devel glib-devel libarchive-devel fuse-devel boost-devel boost-filesystem vim-common

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

RUN zypper install qt5-qtxmlpatterns-devel qt5-qtnetwork-devel qt5-qtbase-devel attica-qt5

WORKDIR /build
VOLUME  ["/sources", "/build"]
