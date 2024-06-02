cmake_minimum_required(VERSION 3.29.3)
project(u8glob 
    VERSION 0.1.0 
    DESCRIPTION "Tiny library for glob matching with UTF-8 support"
    HOMEPAGE_URL "https://github.com/acc15/u8glob"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_TESTING "Build tests" ${PROJECT_IS_TOP_LEVEL})
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Exports all symbols from library (Windows only)" ON)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Debug postfix")

if(MSVC)
    add_compile_options(
        /W4 # Enable informational warnings (1-4 level)
        /WX # Treat warnings as errors
        /utf-8 # Enable UTF-8 support
        /Zc:wchar_t # wchar_t as builtin type
        /Zc:forScope # Removes possibility to use out of scope loop variables
        
        # Removes unreferenced data or functions that are COMDATs, or that only have internal linkage. 
        # Greatly reduces binary size
        /Zc:inline
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

function(configure_install_pkgconfig target template install_dir)
    configure_file(config.pc.in ${template} @ONLY)
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_LINKER_FILE_BASE_NAME:${target}>.pc"
        INPUT ${template}
        TARGET ${target}
    )
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_LINKER_FILE_BASE_NAME:${target}>.pc"
        DESTINATION ${install_dir}
    )
endfunction()

function(configure_install target)
    
    string(TOUPPER ${target} TARGET)    
    option(${TARGET}_INSTALL "Generate ${target} install" ON)

    if(NOT ${TARGET}_INSTALL)
        return()
    endif()

    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    set(cmake_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${target}")
    set(cmake_config_file "${CMAKE_CURRENT_BINARY_DIR}/${target}-config.cmake")
    set(cmake_version_file "${CMAKE_CURRENT_BINARY_DIR}/${target}-config-version.cmake")
    set(cmake_targets "${target}-targets")
    set(cmake_targets_file "${CMAKE_CURRENT_BINARY_DIR}/${cmake_targets}.cmake")
    set(namespace "${target}::")
    set(pkgconfig_template "${CMAKE_CURRENT_BINARY_DIR}/${target}.pc.in")
    set(pkgconfig_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

    get_target_property(version ${target} VERSION)
    if(NOT version)
        set(version ${PROJECT_VERSION})
    endif()

    configure_package_config_file(config.cmake.in ${cmake_config_file} INSTALL_DESTINATION ${cmake_dir})    
    write_basic_package_version_file(${cmake_version_file} VERSION ${version} COMPATIBILITY ExactVersion)
    install(TARGETS ${target} EXPORT ${cmake_targets} LIBRARY FILE_SET HEADERS RUNTIME)
    install(EXPORT ${cmake_targets} DESTINATION ${cmake_dir} NAMESPACE ${namespace})
    install(FILES ${cmake_config_file} ${cmake_version_file} DESTINATION ${cmake_dir})
    export(EXPORT ${cmake_targets} FILE ${cmake_targets_file} NAMESPACE ${namespace})
    configure_install_pkgconfig(${target} ${pkgconfig_template} ${pkgconfig_dir})

endfunction()

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

