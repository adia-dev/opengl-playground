#pragma once

#include "math/vector2.hpp"

namespace rendering {

struct Vertex {
    math::Vector3f position;
    math::Vector2f uv;
};

} // namespace rendering
