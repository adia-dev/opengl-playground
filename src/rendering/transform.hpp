#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace rendering {
struct Transform {
    glm::vec3 position{0.0F, 0.0F, 0.0F};
    glm::vec3 rotation{};
    glm::vec3 scale{1.0F, 1.0F, 1.0F};

    [[nodiscard]]
    auto to_model() const -> glm::mat4;

    void translate(float x, float y, float z);
    void rotate(float x, float y, float z);
    void adjust_scale(float s);
    void adjust_scale(float x, float y, float z);
};
} // namespace rendering
