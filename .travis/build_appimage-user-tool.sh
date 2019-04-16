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

cmake ${SOURCES_DIR} -DCMAKE_INSTALL_PREFIX=/usr -Dlibappimage_DIR=${INSTALL_PREFIX}/lib/cmake/libappimage \
    -DINSTALL_LIBAPPIMAGE=ON -DBUILD_SETTINGS_DIALOG=OFF

make install DESTDIR=AppDir
