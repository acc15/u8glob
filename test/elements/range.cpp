#include <catch2/catch_test_macros.hpp>

#include "u8glob/elements/range.hpp"

using namespace u8glob;

TEST_CASE("range: stream print", "[range]") {
    std::stringstream s;
    range r = {{{U'А', U'Я'}}, true};
    s << r;
    REQUIRE( s.str() == "[!А-Я]" );
}