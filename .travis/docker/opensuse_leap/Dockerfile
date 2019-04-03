FROM opensuse:leap

RUN yum -y update
RUN yum -y groupinstall "Development Tools"
RUN yum -y install yum-plugin-ovl centos-release-scl epel-release
RUN yum -y install devtoolset-4-gcc* openssl-devel wget cmake git

# Install libappimage build dependencies
RUN yum -y install desktop-file-utils librsvg2-devel cairo-devel glib-devel libarchive-devel fuse-devel boost-devel boost-filesystem vim-common

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

RUN yum -y install qt5-qtxmlpatterns-devel qt5-qtnetwork-devel qt5-qtbase-devel attica-qt5

WORKDIR /build
VOLUME  ["/sources", "/build"]
