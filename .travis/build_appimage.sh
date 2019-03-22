#!/usr/bin/env bash

set -ex

if [ -z "${INSTALL_PREFIX}" ]; then
    echo "Missing INSTALL_PREFIX";
    exit 1;
fi

# Find the right sources dir path
SOURCES_DIR=`realpath $0`
SOURCES_DIR=`dirname ${SOURCES_DIR}`
SOURCES_DIR=`dirname ${SOURCES_DIR}`

cmake ${SOURCES_DIR} -DCMAKE_INSTALL_PREFIX=/usr -Dlibappimage_DIR=${INSTALL_PREFIX}/lib/cmake/libappimage
make -j`nproc` install DESTDIR=AppDir

wget -nc https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget -nc https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

chmod +x linuxdeploy-*

export LD_LIBRARY_PATH=${QT_INSTALL_PREFIX}/lib:${INSTALL_PREFIX}/lib:${LD_LIBRARY_PATH}

./linuxdeploy-x86_64.AppImage --appdir=AppDir --plugin qt --output appimage
