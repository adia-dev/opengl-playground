#pragma once

#include "math/color.hpp"
#include "math/vector2.hpp"

namespace rendering {

struct Vertex {
    math::Vector2f position;
    math::Color color;
};

} // namespace rendering
