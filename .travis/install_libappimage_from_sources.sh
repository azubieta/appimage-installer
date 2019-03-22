#!/usr/bin/env bash

set -ex

if [ -z "${INSTALL_PREFIX}" ]; then
    echo "Missing INSTALL_PREFIX";
    exit 1;
fi

LIBAPPIMAGE_SOURCE_DIR=/tmp/libappimage_source
LIBAPPIMAGE_BUILD_DIR=/tmp/libappimage_build

git clone https://github.com/AppImage/libappimage.git --depth=1 ${LIBAPPIMAGE_SOURCE_DIR}
git -C ${LIBAPPIMAGE_SOURCE_DIR} submodule update --init --recursive;

mkdir -p ${LIBAPPIMAGE_BUILD_DIR}; cd ${LIBAPPIMAGE_BUILD_DIR}
cmake ${LIBAPPIMAGE_SOURCE_DIR} \
    -DBUILD_TESTING=OFF \
    -DUSE_SYSTEM_LIBARCHIVE=ON \
    -DUSE_SYSTEM_XZ=ON\
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -DCMAKE_BUILD_TYPE=Release

sudo make -j`nproc` install

