cmake_minimum_required(VERSION 3.29.3)
project(u8glob VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_TESTING "Build tests" ${PROJECT_IS_TOP_LEVEL})
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Exports all symbols from library (Windows only)" ON)
option(U8GLOB_INSTALL "Generate the install target." ON)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Debug postfix")

if(MSVC)
    add_compile_options(
        /W4 # Enable informational warnings (1-4 level)
        /WX # Treat warnings as errors
        /utf-8 # Enable UTF-8 support
        /Zc:wchar_t # wchar_t as builtin type
        /Zc:forScope # Removes possibility to use out of scope loop variables
        /Zc:inline # Removes unreferenced data or functions that are COMDATs, or that only have internal linkage. Greatly reduces binary size
    )
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

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

if(U8GLOB_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    set(PKG_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/u8glob)
    
    set(PKG_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/u8glob-config.cmake)
    set(PKG_VERSION ${CMAKE_CURRENT_BINARY_DIR}/u8glob-config-version.cmake)

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in ${PKG_CONFIG} INSTALL_DESTINATION ${PKG_DIR})
    write_basic_package_version_file(${PKG_VERSION} VERSION ${PROJECT_VERSION} COMPATIBILITY ExactVersion)

    install(TARGETS u8glob EXPORT u8glob-targets LIBRARY FILE_SET HEADERS RUNTIME)
    install(EXPORT u8glob-targets DESTINATION ${PKG_DIR} NAMESPACE u8glob::)
    install(FILES ${PKG_VERSION} ${PKG_CONFIG} DESTINATION ${PKG_DIR})
endif()

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

