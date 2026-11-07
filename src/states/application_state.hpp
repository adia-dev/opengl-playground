#pragma once

#include "rendering/transform.hpp"
namespace states {

struct ApplicationState final {
    float red_intensity{0.1F};
    rendering::Transform camera_transform{.position = {0.0F, 0.0F, 3.0F}};
};

} // namespace states
