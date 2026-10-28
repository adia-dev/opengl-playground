#pragma once

#include "GLFW/glfw3.h"
#include "states/button_state.hpp"
#include <array>
#include <optional>

class InputState final {
  public:
    auto before_frame() noexcept -> void;
    auto update_key(int button, int action) noexcept -> void;

  private:
    std::array<ButtonFlag, GLFW_KEY_LAST + 1> key_states_;

    static auto as_key_index(int key) -> std::optional<size_t>;
};
