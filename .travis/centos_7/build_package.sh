#!/usr/bin/env bash

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

DOCKER_IMAGE=azubieta90/appimage-user-tool-build:centos-7

if  [ "$BUILD_DOCKER_IMAGE" = "ON" ]; then
    sudo docker build -t ${DOCKER_IMAGE} ${SOURCES_DIR}

    echo "$DOCKER_PASSWORD" | docker login -u "$DOCKER_USERNAME" --password-stdin
    docker push ${DOCKER_IMAGE}
fi


export CENTOS7_PACKAGE_REQUIRES="boost-filesystem, libarchive, cairo, librsvg2"
export CENTOS7_PACKAGE_PROVIDES="libappimage.so.1.0()(64bit), libKF5Attica.so.5()(64bit)"

mkdir docker-build-release
sudo docker run -v ${PWD}:/source -v ${PWD}/docker-build-release:/build ${DOCKER_IMAGE} /bin/bash -c "\
        cmake3 /source \
            -DINSTALL_LIBAPPIMAGE=On\
            -DINSTALL_ATTICA=On\
            -DCMAKE_INSTALL_PREFIX=/usr \
            -DCMAKE_BUILD_TYPE=Release \
            -DAPPIMAGE_INSTALLER_V_SUFFIX=\"-centos-7\" \
            -DCPACK_RPM_PACKAGE_REQUIRES='${CENTOS7_PACKAGE_REQUIRES}' \
            -DCPACK_RPM_PACKAGE_PROVIDES='${CENTOS7_PACKAGE_PROVIDES}' &&\
         make -j`nproc` &&\
         cpack3 -G RPM"
