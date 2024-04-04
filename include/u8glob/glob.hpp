#pragma once

#include <vector>
#include <variant>
#include <optional>

#include <utf8.h>

#include "u8glob/elements/range.hpp"
#include "u8glob/elements/any.hpp"
#include "u8glob/elements/star.hpp"

namespace u8glob {

    class glob {
    public:
        using element = std::variant<star, any, std::string, range>;
        using element_vector = std::vector<element>;

        element_vector elements;

        glob() = default;
        glob(const element_vector& v);
        glob(element_vector&& v);
        glob(const std::initializer_list<element>& v);

        bool empty() const;
        bool matches(std::string_view sv) const;
        
        void stringify(std::string& result) const;
        std::string stringify() const;

        std::optional<std::string> as_single_string() const;

        static void escape(std::string_view str, std::string& result);
        static std::string escape(std::string_view str);

        static glob parse(std::string_view str);

    private:
        std::string& last_string();
        void parse_self(std::string_view str);
        void parse_range(std::string_view::const_iterator& it, std::string_view::const_iterator end);

    };

}