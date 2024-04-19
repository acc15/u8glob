cmake_minimum_required(VERSION 3.29.0)
project(u8glob VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
string(TOUPPER ${PROJECT_NAME} PROJECT_VAR)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(${PROJECT_VAR}_ROOT ON)
else()
  set(${PROJECT_VAR}_ROOT OFF)
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(${PROJECT_VAR}_TEST "Build tests" ${${PROJECT_VAR}_ROOT})

if(MSVC)
    add_compile_options(/W4 /WX /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(${PROJECT_NAME})

set(SRC src)
target_sources(${PROJECT_NAME} PRIVATE 
    ${SRC}/elements/range.cpp
    ${SRC}/glob.cpp
    ${SRC}/glob_path.cpp
)

set(INC include/${PROJECT_NAME})
target_sources(${PROJECT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
    ${INC}/elements/star.hpp
    ${INC}/elements/any.hpp
    ${INC}/elements/range.hpp
    ${INC}/match.hpp
    ${INC}/glob.hpp
    ${INC}/glob_path.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE deps/utfcpp/source)

add_subdirectory(cmake)
if(${PROJECT_VAR}_TEST)
    add_subdirectory(test)
endif()

