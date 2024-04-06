#include <catch2/catch_test_macros.hpp>

#include "u8glob/elements/range.hpp"

using namespace u8glob;

TEST_CASE("range: stringify", "[range]") {
    
    range r = { {{U'A', U'Z'}}, true };

    std::string str;
    r.stringify(str);
    REQUIRE( str == "[!A-Z]" );

}