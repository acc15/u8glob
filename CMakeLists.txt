cmake_minimum_required(VERSION 3.29.3)
project(u8glob VERSION 0.1.0 
    DESCRIPTION "Tiny library for glob matching with UTF-8 support"
    HOMEPAGE_URL "https://github.com/acc15/u8glob"
    LANGUAGES CXX)

include(cmake/helpers.cmake)
configure_defaults(WITH_SHARED_LIBS)

add_library(u8glob)

target_sources(u8glob PRIVATE 
    ${SRC}/elements/range.cpp
    ${SRC}/glob.cpp
    ${SRC}/glob_path.cpp
)

target_sources(u8glob PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
    ${INC}/match.hpp
    ${INC}/elements/star.hpp
    ${INC}/elements/any.hpp
    ${INC}/elements/range.hpp
    ${INC}/glob.hpp
    ${INC}/glob_path.hpp
)

target_include_directories(u8glob PRIVATE deps/utfcpp/source)
configure_install(u8glob)

testing(TARGETS u8glob SRC
    ${TEST}/glob.cpp
    ${TEST}/glob_path.cpp
    ${TEST}/elements/range.cpp
)

