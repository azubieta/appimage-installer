#!/usr/bin/env bash

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

DOCKER_IMAGE=azubieta90/appimage-user-tool-build:arch

if  [ "$BUILD_DOCKER_IMAGE" = "ON" ]; then
    sudo docker build -t ${DOCKER_IMAGE} ${SOURCES_DIR}

    echo "$DOCKER_PASSWORD" | docker login -u "$DOCKER_USERNAME" --password-stdin
    docker push ${DOCKER_IMAGE}
fi

mkdir docker-build-release && chmod a+rwx docker-build-release
sudo docker run -v ${PWD}:/sources -v ${PWD}/docker-build-release:/build ${DOCKER_IMAGE} \
    /bin/bash -c "cp /sources/.travis/arch/PKGBUILD . && makepkg"
