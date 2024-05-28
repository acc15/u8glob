#!/bin/bash

BuildDir="${BuildDir:-build}"
BuildType="${BuildType:-Release}"
Generator="${Generator:-Ninja}"
Toolchain="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
Testing="${Testing:-False}"

ConfigureAppend="${ConfigureAppend:-}"
if [[ $Arch ]]; then
	ConfigureAppend="${ConfigureAppend} -A ${Arch}"
fi

BuildAppend="${BuildAppend:-}"

rm -rf "$BuildDir"
cmake -S . -B "$BuildDir" -G "$Generator" --toolchain "$Toolchain" -DCMAKE_BUILD_TYPE="$BuildType" -DBUILD_TESTING="${Testing}" $ConfigureAppend
cmake --build "$BuildDir" --config "$BuildType" -v $BuildAppend
ctest --test-dir "$BuildDir"

ls -lA "$BuildDir"