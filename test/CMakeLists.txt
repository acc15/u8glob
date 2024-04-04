find_package(Catch2 REQUIRED)

add_executable(u8glob_test 
    glob.cpp
    glob_path.cpp
)

target_link_libraries(u8glob_test Catch2::Catch2WithMain u8glob)

include(CTest)
include(Catch)
catch_discover_tests(u8glob_test)