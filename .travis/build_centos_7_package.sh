#!/usr/bin/env bash

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

mkdir docker-centos7-build-release

sudo docker build -t build/centos7 ${SOURCES_DIR}/.travis/docker/centos7/
sudo docker run -v ${PWD}:/source -v ${PWD}/docker-centos7-build-release:/build build/centos7 \
    /bin/bash -c "cmake3 -DINSTALL_DEPENDENCIES=On -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release /source && make -j`nproc` && cpack3 -G RPM"
