#!/bin/sh

BuildDir="${BuildDir:-build}"
BuildType="${BuildType:-Release}"
Generator="${Generator:-Ninja}"
Toolchain="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"

rm -rf "$BuildDir"
cmake -S . -B "$BuildDir" -G "$Generator" --toolchain "$Toolchain" -DCMAKE_BUILD_TYPE="$BuildType" -DBUILD_TESTING=OFF
cmake --build "$BuildDir" --config "$BuildType" -v

ls -lA "$BuildDir"