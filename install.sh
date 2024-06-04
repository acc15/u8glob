#!/bin/bash

export BINARY_DIR="build/install"
sudo=$(command -v sudo || echo "")
rm -rf "$BINARY_DIR"
cmake --workflow --preset $1
$sudo cmake --install "$BINARY_DIR" --config Debug
$sudo cmake --install "$BINARY_DIR" --config Release
