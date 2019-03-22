#!/usr/bin/env bash

set -ex

if [ -z "${INSTALL_PREFIX}" ]; then
    echo "Missing INSTALL_PREFIX";
    exit 1;
fi

ECM_SOURCE_DIR=/tmp/ecm_sources
ECM_BUILD_DIR=/tmp/ecm_build

git clone https://anongit.kde.org/extra-cmake-modules.git --depth=1 ${ECM_SOURCE_DIR}
git -C ${ECM_SOURCE_DIR} submodule update --init --recursive;

mkdir -p ECM_BUILD_DIR; cd ${ECM_SOURCE_DIR}
cmake ${ECM_SOURCE_DIR} -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} -DCMAKE_BUILD_TYPE=Release
make -j`nproc` install

