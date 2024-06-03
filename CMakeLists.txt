cmake_minimum_required(VERSION 3.29.3)
project(u8glob 
    VERSION 0.1.0 
    DESCRIPTION "Tiny library for glob matching with UTF-8 support"
    HOMEPAGE_URL "https://github.com/acc15/u8glob"
    LANGUAGES CXX)

include(cmake/helpers.cmake)
configure_compilers()

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_TESTING "Build tests" ${PROJECT_IS_TOP_LEVEL})
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Exports all symbols from library (Windows only)" ON)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Debug postfix")

add_library(u8glob)

set(SRC src)
target_sources(u8glob PRIVATE 
    ${SRC}/elements/range.cpp
    ${SRC}/glob.cpp
    ${SRC}/glob_path.cpp
)

set(INC include/u8glob)
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

if(BUILD_TESTING)
    
    find_package(Catch2 REQUIRED)

    set(TEST test)
    add_executable(u8glob_test 
        ${TEST}/glob.cpp
        ${TEST}/glob_path.cpp
        ${TEST}/elements/range.cpp
    )

    target_link_libraries(u8glob_test Catch2::Catch2WithMain u8glob)

    include(CTest)
    include(Catch)
    catch_discover_tests(u8glob_test)

endif()

