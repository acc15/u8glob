cmake_minimum_required(VERSION 3.29.0)
project(u8glob VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(U8GLOB_ROOT ON)
else()
  set(U8GLOB_ROOT OFF)
endif()

if(MSVC)
    add_compile_options(/W4 /WX /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(utf8cpp REQUIRED)

set(INC include/u8glob)
set(SRC src)

add_library(u8glob
    ${SRC}/elements/range.cpp  
    ${SRC}/glob.cpp  
    ${SRC}/glob_path.cpp
)

target_sources(u8glob PUBLIC FILE_SET HEADERS BASE_DIRS include FILES 
    ${INC}/elements/star.hpp 
    ${INC}/elements/any.hpp
    ${INC}/elements/range.hpp 
    ${INC}/match.hpp 
    ${INC}/glob.hpp 
    ${INC}/glob_path.hpp
)

target_link_libraries(u8glob utf8::cpp)

include(GNUInstallDirs)
install(TARGETS u8glob FILE_SET HEADERS)

option(U8GLOB_TEST "Build tests" ${U8GLOB_ROOT})
if(U8GLOB_TEST)
    add_subdirectory(test)
endif()

