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

# Build Appimage
wget -nc https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget -nc https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

for FILE in linuxdeploy*; do
    chmod +x $FILE
    ./$FILE --appimage-extract
done

export LD_LIBRARY_PATH=${QT_INSTALL_PREFIX}/lib:${INSTALL_PREFIX}/lib:${LD_LIBRARY_PATH}
export QT_SELECT=5

squashfs-root/usr/bin/linuxdeploy --appdir=AppDir --plugin qt --output appimage --desktop-file=AppDir/usr/share/applications/org.appimage.user-tool.desktop

