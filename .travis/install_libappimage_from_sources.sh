#!/usr/bin/env bash

set -ex

export LIBAPPIMAGE_INSTALL_PREFIX=/tmp/libappimage_prefix
LIBAPPIMAGE_SOURCE_DIR=/tmp/libappimage_source

git clone https://github.com/AppImage/libappimage.git --depth=1 ${LIBAPPIMAGE_SOURCE_DIR}
git -C ${LIBAPPIMAGE_SOURCE_DIR} submodule update --init --recursive;

mkdir -p /tmp/libappimage_source; cd /tmp/libappimage_source
cmake ${LIBAPPIMAGE_SOURCE_DIR} -DCMAKE_INSTALL_PREFIX=${LIBAPPIMAGE_INSTALL_PREFIX} -DBUILD_TESTING=OFF -DUSE_SYSTEM_LIBARCHIVE=ON -DUSE_SYSTEM_XZ=ON
make -j`nproc` install

