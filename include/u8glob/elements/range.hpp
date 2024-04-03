#pragma once

#include <map>
#include <optional>
#include <utf8.h>

namespace u8glob {

struct range {
    
    using map_type = std::map<utf8::utfchar32_t, utf8::utfchar32_t>;
    
    map_type map;
    bool inverse = false;

    void add(utf8::utfchar32_t min, utf8::utfchar32_t max);
    void add(utf8::utfchar32_t v);
    bool contains(utf8::utfchar32_t v) const;
    void stringify(std::string& result) const;
    void parse(std::string_view::const_iterator& it, std::string_view::const_iterator end);

    bool empty() const;
    std::optional<utf8::utfchar32_t> as_single_char() const;

};

}