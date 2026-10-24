#!/bin/sh
set -ex

cmake --build build --target opengl-playground -j$(nproc)
./build/opengl-playground