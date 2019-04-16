#!/usr/bin/env bash

set -xe

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

DOCKER_IMAGE=azubieta90/appimage-user-tool-build:opensuse-leap

if  [ "$BUILD_DOCKER_IMAGE" = "ON" ]; then
    sudo docker build -t ${DOCKER_IMAGE} ${SOURCES_DIR}

    echo "$DOCKER_PASSWORD" | docker login -u "$DOCKER_USERNAME" --password-stdin
    docker push ${DOCKER_IMAGE}
fi

export OPENSUSE_PACKAGE_REQUIRES="libboost_filesystem1_61_0, libarchive13, cairo, librsvg2"
export OPENSUSE_PACKAGE_PROVIDES="libappimage.so.1.0()(64bit), libKF5Attica.so.5()(64bit)"

mkdir -p docker-build-release
sudo docker run -v ${PWD}:/source -v ${PWD}/docker-build-release:/build ${DOCKER_IMAGE} /bin/bash -c "\
        cmake /source \
            -DINSTALL_LIBAPPIMAGE=On\
            -DINSTALL_ATTICA=On\
            -DCMAKE_INSTALL_PREFIX=/usr \
            -DCMAKE_BUILD_TYPE=Release \
            -DAPPIMAGE_USER_TOOL_V_SUFFIX=\"-opensuse-leap\" \
            -DCPACK_RPM_PACKAGE_REQUIRES='${OPENSUSE_PACKAGE_REQUIRES}' \
            -DCPACK_RPM_PACKAGE_PROVIDES='${OPENSUSE_PACKAGE_PROVIDES}' &&\
         make -j`nproc` &&\
         cpack -G RPM"
