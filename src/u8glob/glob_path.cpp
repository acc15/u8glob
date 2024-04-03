#include "u8glob/glob_path.hpp"

#include <ranges>

namespace u8glob {

glob_path::glob_path(const std::initializer_list<element>& elements_init): elements(elements_init) {}

bool glob_path::matches(const fs_path& path) {
    return matches(path | std::views::transform([](const fs_path& p) { return p.string(); }));
}

glob_path::element glob_path::parse_path_element(std::string_view v) {
    if (v.empty()) {
        return static_cast<std::string>(v);
    }
    if (v == "**") {
        return star {};
    }
    auto g = glob::parse(v);
    if (g.elements.empty()) {
        return static_cast<std::string>(v);
    }
    if (g.elements.size() > 1) {
        return g;
    }
    
    auto& first = g.elements.front();
    if (std::holds_alternative<std::string>(first)) {
        return std::get<std::string>(first);
    }
    return g;
}


}

