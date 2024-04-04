#pragma once

#include <filesystem>

#include "glob.hpp"
#include "match.hpp"

namespace u8glob {

class glob_path {
public:

    using element = std::variant<star, std::string, glob>;
    using element_vector = std::vector<element>;
    using fs_path = std::filesystem::path;

    element_vector elements;

    glob_path() = default;
    glob_path(const element_vector& elements);
    glob_path(element_vector&& elements);
    glob_path(const std::initializer_list<element>& elements);

    template<std::ranges::range PathLike>
    bool matches(const PathLike& path) {
        struct match_visitor {
            match_struct<PathLike>& m;

            bool operator()(const star&) {
                ++m.cur;
                return true;
            }

            bool operator()(const std::string& s) {
                if (*m.cur == s) {
                    ++m.cur;
                    return true;
                }
                return false;
            }

            bool operator()(const glob& g) {
                if (g.matches(*m.cur)) {
                    ++m.cur;
                    return true;
                }
                return false;
            }
        };
        return glob_match<match_visitor>(elements, path);
    }

    bool matches(const fs_path& path);

    static glob_path::element parse_path_element(std::string_view v);

    // void stringify(std::string& result) const;
    // std::string stringify() const;

    // static glob_path parse(std::string_view v) const;

};

}