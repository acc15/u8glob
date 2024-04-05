#include <ranges>

#include "u8glob/glob_path.hpp"

namespace u8glob {

glob_path::glob_path(const element_vector& elements): elements(elements) {}
glob_path::glob_path(element_vector&& elements): elements(elements) {}
glob_path::glob_path(const std::initializer_list<element>& elements): elements(elements) {}

bool glob_path::matches(const fs_path& path) {
    return matches(path | std::views::transform([](const fs_path& p) { return p.string(); }));
}

glob_path::element glob_path::parse_path_element(std::string_view v) {
    if (v.empty()) {
        return std::string();
    }
    if (v == "**") {
        return star {};
    }
    glob g = glob::parse(v);
    const auto str = g.as_single_string();
    if (str.has_value()) {
        return str.value();
    }
    return g;
}


}

