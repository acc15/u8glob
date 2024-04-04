#include <catch2/catch_test_macros.hpp>

#include "u8glob/glob_path.hpp"

using namespace u8glob;

namespace fs = std::filesystem;

TEST_CASE("glob_path: matches", "[glob_path][match]") {
    
    glob_path gp = { "a", star{}, "c.txt" };
    REQUIRE( gp.matches(fs::path("a") / "b" / "c.txt") );
    REQUIRE_FALSE( gp.matches(fs::path("a") / "b" / "d.txt") );
    REQUIRE( gp.matches(fs::path("a") / "b" / "e" / "x" / "c.txt") );

}

TEST_CASE("glob_path: parse_path_element", "[glob_path][parse]") {

    REQUIRE( std::holds_alternative<star>(glob_path::parse_path_element("**")) );
    REQUIRE( std::holds_alternative<std::string>(glob_path::parse_path_element("abcde")) );
    REQUIRE( std::holds_alternative<std::string>(glob_path::parse_path_element("[")) );
    REQUIRE( std::holds_alternative<glob>(glob_path::parse_path_element("[a-z]")) );

}
