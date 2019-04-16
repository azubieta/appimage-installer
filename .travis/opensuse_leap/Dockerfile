FROM opensuse/leap:15

# Install libappimage build dependencies
RUN zypper --non-interactive install openssl-devel wget cmake git gcc-c++ \
    desktop-file-utils librsvg2-devel cairo-devel glib2-devel libarchive-devel \
    fuse-devel libboost_filesystem1_66_0-devel boost-devel vim automake libtool patch

# Build and install libappimage
RUN git clone https://github.com/AppImage/libappimage.git --depth=1 /tmp/libappimage
RUN cd /tmp/libappimage && \
    git submodule update --init --recursive && \
    cmake . \
        -DBUILD_TESTING=OFF \
        -DUSE_SYSTEM_LIBARCHIVE=ON \
        -DUSE_SYSTEM_BOOST=ON\
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install
RUN rm -rf /tmp/libappimage

RUN zypper addrepo https://download.opensuse.org/repositories/Kernel:/tools/openSUSE_Leap_15.0/Kernel:tools.repo
RUN zypper addrepo https://download.opensuse.org/repositories/KDE:/Qt5/openSUSE_Leap_15.0/KDE:Qt5.repo
RUN zypper addrepo https://download.opensuse.org/repositories/KDE:/Frameworks5/openSUSE_Leap_15.0/KDE:Frameworks5.repo
RUN zypper --non-interactive --no-gpg-checks refresh

RUN zypper --non-interactive install libqt5-qtbase-devel libQt5XmlPatterns-devel attica-qt5-devel rpm-build

# Install xdg-utils
RUN git clone https://github.com/azubieta/xdg-utils-cxx.git --depth=1 /tmp/xdg_utils_cxx
RUN cd /tmp/xdg_utils_cxx && \
    git submodule update --init --recursive && \
    cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install
RUN rm -rf /tmp/xdg_utils_cxx

# Install yaml-cpp
RUN git clone https://github.com/jbeder/yaml-cpp.git --depth=1 /tmp/yaml-cpp
RUN cd /tmp/yaml-cpp && \
    git submodule update --init --recursive && \
    cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
    make -j`nproc` install
RUN rm -rf cd /tmp/yaml-cpp

WORKDIR /build
VOLUME  ["/sources", "/build"]
