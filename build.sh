#!/bin/bash

Clean="${Clean:-1}"
BuildDir="${BuildDir:-build}"
BuildType="${BuildType:-Release}"
Generator="${Generator:-Ninja}"
Toolchain="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
Testing="${Testing:-False}"

if [[ $Arch ]]; then
	ConfigureAppend="${ConfigureAppend} -A ${Arch}"
fi

if [[ ${Clean,,} =~ ^(yes|true|1)$ ]]; then
	echo "Removing $BuildDir..."
	rm -rf "$BuildDir"
fi

echo "Configuring $BuildDir..."
cmake -S . -B "$BuildDir" -G "$Generator" --toolchain "$Toolchain" -DCMAKE_BUILD_TYPE="$BuildType" -DBUILD_TESTING="${Testing}" $ConfigureAppend

echo "Building $BuildDir..."
cmake --build "$BuildDir" --config "$BuildType" -v $BuildAppend

echo "Testing $BuildDir..."
ctest --test-dir "$BuildDir"

ls -lA "$BuildDir"