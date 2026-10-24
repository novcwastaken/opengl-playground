#!/bin/sh
set -ex

if [ -d build ]; then
    rm -rf build
fi

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=Debug ..