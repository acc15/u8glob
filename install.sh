#!/bin/bash

build_dir="build/$1"
sudo=$(command -v sudo || echo "")
rm -rf "$build_dir"
cmake --workflow --preset $1
$sudo cmake --install "$build_dir" --config Debug
$sudo cmake --install "$build_dir" --config Release
