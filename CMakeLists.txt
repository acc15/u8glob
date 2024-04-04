cmake_minimum_required(VERSION 3.29.0)
project(u8glob VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(U8GLOB_ROOT ON)
else()
  set(U8GLOB_ROOT OFF)
endif()

if (MSVC)
    add_compile_options(/W4 /WX /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# u8glob library configuration
find_package(utf8cpp REQUIRED)
add_library(u8glob 
    src/elements/range.cpp
    src/glob.cpp
    src/glob_path.cpp
)

target_include_directories(u8glob PUBLIC include)
target_link_libraries(u8glob utf8::cpp)

option(U8GLOB_TEST "Generate the test target." ${U8GLOB_ROOT})
if (U8GLOB_TEST)
    add_subdirectory(test)
endif()

