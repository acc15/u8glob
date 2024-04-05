#include <utf8/unchecked.h>

#include "u8glob/elements/range.hpp"

namespace u8glob {

void range::add(char32_t min, char32_t max) {
    if (min > max) {
        std::swap(min, max);
    }
    
    const auto min_next = map.upper_bound(min);
    const auto min_iter = min_next != map.begin() && std::prev(min_next)->second + 1 >= min
        ? std::prev(min_next)
        : map.insert(min_next, { min, max });

    const auto max_next = map.upper_bound(max);
    if (max_next != map.end() && max + 1 == max_next->first) {
        min_iter->second = max_next->second;
        map.erase(min_next, std::next(max_next));
    } else {
        min_iter->second = std::max(max, std::prev(max_next)->second);
        map.erase(min_next, max_next);
    }
}

void range::add(char32_t v) {
    add(v, v);
}

bool range::contains(char32_t v) const {
    const auto it = map.upper_bound(v);
    const bool in_range = it != map.begin() && v <= std::prev(it)->second;
    return in_range != inverse;
}

void range::parse(std::string_view::const_iterator& it, std::string_view::const_iterator end) {
    
    std::optional<char32_t> last_char = std::nullopt;
    if (it != end) {
        const auto first_char = utf8::unchecked::next(it);
        if (first_char == U'!') {
            inverse = true;
        } else {
            last_char = first_char;
        }
    }

    bool in_char_range = false;
    while (it != end) {
        const auto ch = utf8::unchecked::next(it);
        if (ch == U']') {
            break;
        }
        
        if (in_char_range) {
            add(last_char.value(), ch);
            last_char = std::nullopt;
            in_char_range = false;
            continue;
        }

        if (ch == U'-' && last_char.has_value()) {
            in_char_range = true;
            continue;
        }

        if (last_char.has_value()) {
            add(last_char.value());
        }
        
        last_char = ch;
    }

    if (last_char.has_value()) {
        add(last_char.value());
    }

    if (in_char_range) {
        add(U'-');
    }
}

void range::stringify(std::string& result) const {
    result.append(1, '[');
    if (inverse) {
        result.append(1, '!');
    }
    for (const auto& e: map) {
        if (e.first == e.second) {
            utf8::unchecked::append(e.first, std::back_inserter(result));
        } else {
            utf8::unchecked::append(e.first, std::back_inserter(result));
            result.append(1, '-');
            utf8::unchecked::append(e.second, std::back_inserter(result));
        }
    }
    result.append(1, ']');
}

bool range::empty() const {
    return map.empty();
}

std::optional<char32_t> range::as_single_char() const {
    if (map.empty() && inverse) {
        return U'!';
    } 
    if (map.size() == 1 && !inverse) {
        const auto iter = map.begin();
        if (iter->first == iter->second) {
            return iter->first;
        }
    }
    return std::nullopt;
}

}