#pragma once

#include <variant>

namespace u8glob {

struct star {};

template <typename T>
bool is_star(const T& element) {
    return std::holds_alternative<star>(element);
}

}