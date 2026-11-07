#pragma once

#include "rendering/mesh.hpp"
#include "rendering/transform.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace rendering {

struct SceneObject {
    Transform transform;
    Mesh mesh;
};

} // namespace rendering
