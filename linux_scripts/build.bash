#!/bin/bash
pushd "$(dirname "$0")"
pushd ../build_linux

cmake --build .

popd
popd
