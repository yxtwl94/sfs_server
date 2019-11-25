#!/bin/sh

set -x # 执行指令后，会先显示该指令及所下的参数。

BUILD_DIR=${BUILD_DIR:-../sfs_server/build}

mkdir $BUILD_DIR \
    && cd build \
    && cmake ..