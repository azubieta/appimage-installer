#!/usr/bin/env bash

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

mkdir docker-build-relaese
chmod a+rwx docker-build-relaese

sudo docker build -t build/arch ${SOURCES_DIR}
sudo docker run -v ${PWD}:/sources -v ${PWD}/docker-build-release:/build build/arch \
    /bin/bash -c "cp /sources/.travis/arch/PKGBUILD . && makepkg"
