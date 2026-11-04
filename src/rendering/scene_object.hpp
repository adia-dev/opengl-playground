#pragma once

#include "rendering/mesh.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <numbers>
#include <cmath>

namespace rendering {

struct Transform {
    glm::vec3 position{0.0F, 0.0F, 0.0F};
    glm::vec3 rotation{};
    glm::vec3 scale{1.0F, 1.0F, 1.0F};

    [[nodiscard]]
    auto to_model() const -> glm::mat4 {
        glm::mat4 model{1.0F};

        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.x, glm::vec3{1.0F, 0.0F, 0.0F});
        model = glm::rotate(model, rotation.y, glm::vec3{0.0F, 1.0F, 0.0F});
        model = glm::rotate(model, rotation.z, glm::vec3{0.0F, 0.0F, 1.0F});
        model = glm::scale(model, scale);

        return model;
    }
};

class SceneObject {
  public:
    void move(float x, float y) {
        m_transform.position.x += x;
        m_transform.position.y += y;
    }

    void rotate(float x, float y, float z) {
        m_transform.rotation.x = std::fmod(m_transform.rotation.x + x, 2.0F * std::numbers::pi_v<float>);
        m_transform.rotation.y = std::fmod(m_transform.rotation.y + y, 2.0F * std::numbers::pi_v<float>);
        m_transform.rotation.z = std::fmod(m_transform.rotation.z + z, 2.0F * std::numbers::pi_v<float>);
    }

    void scale(float x, float y, float z) {
        m_transform.scale.x += x;
        m_transform.scale.y += y;
        m_transform.scale.z += z;
    }

    [[nodiscard]]
    auto mesh() noexcept -> Mesh& {
        return m_mesh;
    }

    [[nodiscard]]
    auto transform() noexcept -> const Transform& {
        return m_transform;
    }

  private:
    Transform m_transform{};
    Mesh m_mesh;
};

} // namespace rendering
