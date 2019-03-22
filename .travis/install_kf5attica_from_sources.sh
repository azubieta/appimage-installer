#!/usr/bin/env bash

set -ex

if [ -z "${INSTALL_PREFIX}" ]; then
    echo "Missing INSTALL_PREFIX";
    exit 1;
fi

ATTICA_SOURCE_DIR=/tmp/attica_sources
ATTICA_BUILD_DIR=/tmp/attica_build

git clone https://anongit.kde.org/attica.git --depth=1 ${ATTICA_SOURCE_DIR}
git -C ${ATTICA_SOURCE_DIR} submodule update --init --recursive;

mkdir -p ${ATTICA_BUILD_DIR}; cd ${ATTICA_BUILD_DIR}
cmake ${ATTICA_SOURCE_DIR} \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -DCMAKE_BUILD_TYPE=Release \
    -Decm_DIR=${INSTALL_PREFIX}/share/ECM/cmake

make -j`nproc` install

