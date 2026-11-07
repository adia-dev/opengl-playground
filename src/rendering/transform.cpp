#include "transform.hpp"
#include <numbers>

namespace rendering {

[[nodiscard]]
auto Transform::to_model() const -> glm::mat4 {
    glm::mat4 model{1.0F};

    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3{1.0F, 0.0F, 0.0F});
    model = glm::rotate(model, rotation.y, glm::vec3{0.0F, 1.0F, 0.0F});
    model = glm::rotate(model, rotation.z, glm::vec3{0.0F, 0.0F, 1.0F});
    model = glm::scale(model, scale);

    return model;
}

void Transform::translate(float x, float y, float z) {
    position.x += x;
    position.y += y;
    position.z += z;
}

void Transform::rotate(float x, float y, float z) {
    rotation.x = std::fmod(rotation.x + x, 2.0F * std::numbers::pi_v<float>);
    rotation.y = std::fmod(rotation.y + y, 2.0F * std::numbers::pi_v<float>);
    rotation.z = std::fmod(rotation.z + z, 2.0F * std::numbers::pi_v<float>);
}

void Transform::adjust_scale(float s) {
    adjust_scale(s, s, s);
}

void Transform::adjust_scale(float x, float y, float z) {
    scale.x += x;
    scale.y += y;
    scale.z += z;
}

} // namespace rendering
