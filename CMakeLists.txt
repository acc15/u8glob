cmake_minimum_required(VERSION 3.29.0)
project(u8glob VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# u8glob library configuration
find_package(utf8cpp REQUIRED)
include_directories(u8glob include)
add_library(u8glob 
    src/u8glob/elements/range.cpp
    src/u8glob/glob.cpp
    src/u8glob/glob_path.cpp
)
target_link_libraries(u8glob utf8::cpp)

# Catch2 test configuration
find_package(Catch2 REQUIRED)
add_executable(u8glob_test 
    test/u8glob/glob.cpp
    test/u8glob/glob_path.cpp
)
target_link_libraries(u8glob_test PRIVATE Catch2::Catch2WithMain u8glob)

include(CTest)
include(Catch)
catch_discover_tests(u8glob_test)
