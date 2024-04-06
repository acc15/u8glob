#include <utf8/unchecked.h>

#include "u8glob/glob.hpp"
#include "u8glob/match.hpp"

namespace u8 = utf8::unchecked;

namespace u8glob {

glob::glob(const std::initializer_list<element>& v): elements(v) {
}

glob::glob(const element_vector& v): elements(v) {
}

glob::glob(element_vector&& v): elements(v) {
}

bool glob::matches(std::string_view sv) const {
    struct match_visitor {
        match_struct<std::string_view>& m;

        bool operator()(const star&) {
            u8::advance(m.cur, 1);
            return true;
        }

        bool operator()(const any&) {
            u8::advance(m.cur, 1);
            return true;
        }

        bool operator()(const range& r) {
            return r.contains(u8::next(m.cur));
        }

        bool operator()(const std::string& str) {
            const auto p = std::mismatch(str.begin(), str.end(), m.cur, m.end);
            m.cur = p.second;
            return p.first == str.end();
        }
    };
    return glob_match<match_visitor>(elements, sv);
}

bool glob::empty() const {
    return elements.empty();
}

std::optional<std::string> glob::as_single_string() const {
    switch (elements.size()) {
        case 0: return std::string();
        case 1: 
            const auto& single = elements.front();
            if (std::holds_alternative<std::string>(single)) {
                return std::get<std::string>(single);
            }
            break;
    }
    return std::nullopt;
}

glob glob::parse(std::string_view str) {
    glob result;
    result.parse_self(str);
    return result;
}

void glob::escape(std::string_view str, std::string& result) {
    auto it = str.begin();
    const auto end = str.end();
    while (it != end) {
        const auto it_start = it;
        const auto ch = utf8::unchecked::next(it);
        const bool escape = ch == U'*' || ch == U'?' || ch == U'[' || ch == U']';
        if (escape) {
            result.append(1, '[').append(it_start, it).append(1, ']');
        } else {
            result.append(it_start, it);
        }
    }
}

std::string glob::escape(std::string_view str) {
    std::string result;
    escape(str, result);
    return result;
}

void glob::stringify(std::string& result) const {
    struct stringify_visitor {
        std::string& result;
        
        void operator()(const std::string& str) {
            glob::escape(str, result); 
        }

        void operator()(const star&) { 
            result.append(1, '*'); 
        }

        void operator()(const any&) { 
            result.append(1, '?'); 
        }

        void operator()(const range& range) {
            range.stringify(result);
        }
    } v = {result};
    for (const auto& e: elements) {
        std::visit(v, e); 
    }
}

std::string glob::stringify() const {
    std::string result;
    stringify(result);
    return result;
}

std::string& glob::last_string() {
    if (elements.empty() || !std::holds_alternative<std::string>(elements.back())) {
        elements.push_back(std::string());
    }
    return std::get<std::string>(elements.back());
}

void glob::parse_self(std::string_view str) {
    auto it = str.begin();
    const auto end = str.end();
    while (it != end) {
        const auto it_ch_begin = it;
        const auto ch = u8::next(it);
        switch (ch) {
        case U'*': 
            if (elements.empty() || !is_star(elements.back())) {
                elements.push_back(star{});
            }
            break;

        case U'?':
            elements.push_back(any{});
            break;

        case U'[':
            parse_range(it, end);
            break;

        default:
            last_string().append(it_ch_begin, it);
            break;
        }
    }
}

void glob::parse_range(std::string_view::const_iterator& it, std::string_view::const_iterator end) {
    range r;
    r.parse(it, end);

    auto single_char = r.as_single_char();
    if (single_char.has_value()) {
        utf8::unchecked::append(single_char.value(), std::back_insert_iterator(last_string()));
    } else if (!r.empty()) {
        elements.push_back(std::move(r));
    }
}

}