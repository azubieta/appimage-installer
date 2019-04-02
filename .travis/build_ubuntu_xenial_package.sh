#!/usr/bin/env bash

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

mkdir docker-build-release

sudo docker build -t build/ubuntu:xenial ${SOURCES_DIR}/.travis/docker/ubuntu_xenial/
sudo docker run -v ${PWD}:/source -v ${PWD}/docker-build-release:/build build/ubuntu:xenial \
    /bin/bash -c "cmake -DINSTALL_LIBAPPIMAGE=On -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release /source && make -j`nproc` && cpack deb"
