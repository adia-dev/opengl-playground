#pragma once

#include "GLFW/glfw3.h"
#include "states/button_state.hpp"
#include <array>
#include <optional>

namespace states {

class InputState final {
  public:
    auto before_frame() noexcept -> void;
    auto update_key(int key, int action) noexcept -> void;
    auto update_mouse_button(int button, int action) noexcept -> void;

    [[nodiscard]]
    auto is_key_pressed(int key) noexcept -> bool;
    [[nodiscard]]
    auto is_key_down(int key) noexcept -> bool;
    [[nodiscard]]
    auto is_key_released(int key) noexcept -> bool;

  private:
    std::array<ButtonFlag, GLFW_KEY_LAST + 1> m_key_states{};
    std::array<ButtonFlag, GLFW_MOUSE_BUTTON_LAST + 1> m_mouse_button_states{};

    [[nodiscard]]
    static auto as_button_index(int key, int last) noexcept -> std::optional<size_t>;
    static auto update_button(ButtonFlag& state, int button, int action) noexcept -> void;
};

} // namespace states
