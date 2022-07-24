#!/bin/bash
pushd "$(dirname "$0")"

pushd ../
mkdir -p build_linux
pushd build_linux

conan install .. --build missing
cmake .. -G "Unix Makefiles"

popd
popd
popd
