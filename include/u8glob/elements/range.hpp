#pragma once

#include <map>
#include <optional>
#include <string_view>
#include <ostream>

namespace u8glob {

struct range {
    
    using map_type = std::map<char32_t, char32_t>;
    
    map_type map;
    bool inverse = false;

    void add(char32_t min, char32_t max);
    void add(char32_t v);
    bool contains(char32_t v) const;
    void parse(std::string_view::const_iterator& it, std::string_view::const_iterator end);

    bool empty() const;
    std::optional<char32_t> as_single_char() const;

};

std::ostream& operator<<(std::ostream& s, const range& r);

}