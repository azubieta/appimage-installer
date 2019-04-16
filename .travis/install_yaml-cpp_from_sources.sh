#!/usr/bin/env bash


set -ex

if [ -z "${INSTALL_PREFIX}" ]; then
    echo "Missing INSTALL_PREFIX";
    exit 1;
fi

YAMLCPP_SOURCE_DIR=/tmp/yaml_cpp_sources
YAMLCPP_BUILD_DIR=/tmp/yaml_cpp_build

git clone https://github.com/jbeder/yaml-cpp.git --depth=1 ${YAMLCPP_SOURCE_DIR}
git -C ${YAMLCPP_SOURCE_DIR} submodule update --init --recursive;

mkdir -p ${YAMLCPP_BUILD_DIR}; cd ${YAMLCPP_BUILD_DIR}
cmake ${YAMLCPP_SOURCE_DIR} -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} -DCMAKE_BUILD_TYPE=Release
sudo make -j`nproc` install
