#!/usr/bin/env bash

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

DOCKER_IMAGE=azubieta90/appimage-user-tool-build:ubuntu-bionic

if  [ "$BUILD_DOCKER_IMAGE" = "ON" ]; then
    sudo docker build -t ${DOCKER_IMAGE} ${SOURCES_DIR}

    echo "$DOCKER_PASSWORD" | docker login -u "$DOCKER_USERNAME" --password-stdin
    docker push ${DOCKER_IMAGE}
fi

mkdir docker-build-release
sudo docker run -v ${PWD}:/source -v ${PWD}/docker-build-release:/build ${DOCKER_IMAGE} \
    /bin/bash -c "cmake -DINSTALL_LIBAPPIMAGE=On -DINSTALL_ATTICA=On -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release /source && make -j`nproc` && cpack -G DEB -R 0.1.5-ubuntu-bionic"
