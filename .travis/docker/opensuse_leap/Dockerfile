FROM opensuse:42.3

RUN zypper addrepo https://download.opensuse.org/repositories/devel:tools:building/openSUSE_Leap_42.3/devel:tools:building.repo
RUN zypper --non-interactive --no-gpg-checks refresh

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

RUN zypper addrepo https://download.opensuse.org/repositories/Kernel:/tools/openSUSE_Leap_42.3/Kernel:tools.repo
RUN zypper addrepo https://download.opensuse.org/repositories/KDE:/Qt5/openSUSE_Leap_42.3/KDE:Qt5.repo
RUN zypper addrepo https://download.opensuse.org/repositories/KDE:/Frameworks5/openSUSE_Leap_42.3/KDE:Frameworks5.repo
RUN zypper --non-interactive --no-gpg-checks refresh

RUN zypper --non-interactive install libqt5-qtbase-devel libQt5XmlPatterns-devel attica-qt5-devel rpm-build

WORKDIR /build
VOLUME  ["/sources", "/build"]
