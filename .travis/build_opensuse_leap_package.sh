#!/usr/bin/env bash

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

mkdir docker-centos7-build-release

export OPENSUSE_PACKAGE_REQUIRES="libboost_filesystem1_61_0, libarchive13, cairo, librsvg2"
export OPENSUSE_PACKAGE_PROVIDES="libappimage.so.1.0()(64bit), libKF5Attica.so.5()(64bit)"

sudo docker build -t build/opensuse:leap ${SOURCES_DIR}/.travis/docker/opensuse_leap/
sudo docker run -v ${PWD}:/source -v ${PWD}/docker-build-release:/build build/opensuse:leap /bin/bash -c "\
        cmake /source \
            -DINSTALL_LIBAPPIMAGE=On\
            -DINSTALL_ATTICA=On\
            -DCMAKE_INSTALL_PREFIX=/usr \
            -DCMAKE_BUILD_TYPE=Release \
            -DAPPIMAGE_USER_TOOL_V_SUFFIX=\"-opensuse-42.3\" \
            -DCPACK_RPM_PACKAGE_REQUIRES='${OPENSUSE_PACKAGE_REQUIRES}' \
            -DCPACK_RPM_PACKAGE_PROVIDES='${OPENSUSE_PACKAGE_PROVIDES}' &&\
         make -j`nproc` &&\
         cpack -G RPM"
