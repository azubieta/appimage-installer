FROM centos:7

RUN yum -y update
RUN yum -y groupinstall "Development Tools"
RUN yum -y install yum-plugin-ovl centos-release-scl epel-release
RUN yum -y install devtoolset-4-gcc* openssl-devel wget cmake3 git

# Install libappimage build dependencies
RUN yum -y install desktop-file-utils librsvg2-devel cairo-devel glib-devel libarchive-devel fuse-devel boost-devel boost-filesystem vim-common

# Build and install libappimage
RUN git clone https://github.com/AppImage/libappimage.git --depth=1 /tmp/libappimage
RUN cd /tmp/libappimage && git submodule update --init --recursive;
RUN cd /tmp/libappimage && \
    cmake3 . \
        -DBUILD_TESTING=OFF \
        -DUSE_SYSTEM_LIBARCHIVE=ON \
        -DUSE_SYSTEM_BOOST=ON\
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install

RUN yum -y install qt5-qtxmlpatterns-devel qt5-qtnetwork-devel qt5-qtbase-devel

# Build and install extra cmake modules
RUN yum -y install qt5-qtxmlpatterns-devel qt5-qtnetwork-devel qt5-qtbase-devel
RUN git clone git://anongit.kde.org/extra-cmake-modules.git --depth=1 /tmp/ecm
RUN cd /tmp/ecm && git submodule update --init --recursive;

RUN cd /tmp/ecm && \
        cmake3 . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
        make -j`nproc` install

# Build and install kf5-attica
RUN git clone git://anongit.kde.org/attica.git --depth=1 -b 'v5.54.0' /tmp/attica
RUN cd /tmp/attica && git submodule update --init --recursive;

RUN cd /tmp/attica &&\
    cmake3 . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DATTICA_STATIC_BUILD=ON && \
    make -j`nproc` install

# Install xdg-utils
RUN git clone https://github.com/azubieta/xdg-utils-cxx.git --depth=1 /tmp/xdg_utils_cxx
RUN cd /tmp/xdg_utils_cxx && \
    git submodule update --init --recursive && \
    cmake3 . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install


# Install yaml-cpp
RUN git clone https://github.com/jbeder/yaml-cpp.git --depth=1 /tmp/yaml-cpp
RUN cd /tmp/yaml-cpp && \
    git submodule update --init --recursive && \
    cmake3 . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install

WORKDIR /build
VOLUME  ["/sources", "/build"]
