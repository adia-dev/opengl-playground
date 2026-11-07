#pragma once

#include "math/vector2.hpp"
#include "rendering/vertex.hpp"
#include <array>
#include <cstddef>

namespace rendering {

// Six faces, each made of two triangles with independent texture coordinates.
inline auto box_vertices(math::Vector3f offset = math::Vector3f::zero()) -> std::array<Vertex, 36> {
    constexpr float half = 0.5F;
    constexpr std::array<math::Vector3f, 8> corners{
        {
            {.x = -half, .y = -half, .z = -half},
            {.x = half, .y = -half, .z = -half},
            {.x = half, .y = half, .z = -half},
            {.x = -half, .y = half, .z = -half},
            {.x = -half, .y = -half, .z = half},
            {.x = half, .y = -half, .z = half},
            {.x = half, .y = half, .z = half},
            {.x = -half, .y = half, .z = half},
        },
    };
    // Each face lists its corners counterclockwise when viewed from outside.
    constexpr std::array<std::array<std::size_t, 4>, 6> faces{
        {
            {{0, 3, 2, 1}},
            {{4, 5, 6, 7}},
            {{0, 4, 7, 3}},
            {{1, 2, 6, 5}},
            {{0, 1, 5, 4}},
            {{3, 7, 6, 2}},
        },
    };
    constexpr std::array<math::Vector2f, 4> uvs{
        {
            {.x = 0.0F, .y = 0.0F},
            {.x = 1.0F, .y = 0.0F},
            {.x = 1.0F, .y = 1.0F},
            {.x = 0.0F, .y = 1.0F},
        },
    };
    constexpr std::array<std::size_t, 6> triangles{{0, 1, 2, 0, 2, 3}};
    std::array<Vertex, 36> vertices{};
    std::size_t output = 0;
    for (const auto& face : faces) {
        for (const auto corner : triangles) {
            vertices[output++] = {.position = corners[face[corner]] + offset, .uv = uvs[corner]};
        }
    }
    return vertices;
}

} // namespace rendering
