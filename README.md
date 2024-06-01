# u8glob

Tiny library for glob matching with UTF-8 support

## Dependencies

Required:

* `cmake` for building
* `utfcpp` for UTF-8 support

Optional (if you don't plan to run tests you don't need these):

* `vcpkg` dependency management
* `catch2` for testing

## Clone (with submodules)

    git clone --recurse-submodules https://github.com/acc15/u8glob.git

## Build

Following configurations are supported out-of-box using CMake Workflows

### Multi-Ninja

    cmake --workflow --preset multi

### Visual Studio (64 bit)

    cmake --workflow --preset vs64

## Install

### Linux

    ./install.sh

### Windows

    C:\Program Files\Git\bin\bash.exe install.sh vs64

## Usage

### CMake

    find_package(u8glob REQUIRED)
    target_link_libraries(<your_app> u8glob::u8glob)

#### Windows DLL auto copy

Windows requires that DLL must be in the same dir with executable.
u8glob will automatically copy installed DLL on use.

This behavior may be disabled by setting `U8GLOB_AUTO_COPY_DLL` CMake Cache variable to `OFF`

