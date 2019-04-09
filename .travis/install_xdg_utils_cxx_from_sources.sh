#!/usr/bin/env bash

set -ex

if [ -z "${INSTALL_PREFIX}" ]; then
    echo "Missing INSTALL_PREFIX";
    exit 1;
fi

XDGUTILS_SOURCE_DIR=/tmp/xdg_utils_cxx_sources
XDGUTILS_BUILD_DIR=/tmp/xdg_utils_cxx_build

git clone https://github.com/azubieta/xdg-utils-cxx.git --depth=1 ${XDGUTILS_SOURCE_DIR}
git -C ${XDGUTILS_SOURCE_DIR} submodule update --init --recursive;

mkdir -p ${XDGUTILS_BUILD_DIR}; cd ${XDGUTILS_BUILD_DIR}
cmake ${XDGUTILS_SOURCE_DIR} -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} -DCMAKE_BUILD_TYPE=Release
sudo make -j`nproc` install

