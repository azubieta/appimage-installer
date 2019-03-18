#!/usr/bin/env bash

pushd /tmp
git clone https://github.com/AppImage/libappimage.git --depth=1

pushd libappimage
git submodule update --init --recursive
cmake . -DCMAKE_INSTALL_PREFIX=/usr
make -j`nproc` install DESTDIR=/tmp/libappimage_prefix
popd

popd

