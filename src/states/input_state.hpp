#pragma once

#include "GLFW/glfw3.h"
#include "math/vector2.hpp"
#include "states/button_state.hpp"
#include <array>
#include <cstddef>
#include <optional>

namespace states {

class InputState final {
  public:
    auto before_frame() noexcept -> void;
    auto update_key(int key, int action) noexcept -> void;
    auto update_mouse_button(int button, int action) noexcept -> void;
    auto update_mouse_pos(const math::Vector2d& mouse_pos) noexcept -> void;

    [[nodiscard]]
    auto is_key_pressed(int key) const noexcept -> bool;
    [[nodiscard]]
    auto is_key_down(int key) const noexcept -> bool;
    [[nodiscard]]
    auto is_key_released(int key) const noexcept -> bool;

    [[nodiscard]]
    auto is_mouse_button_pressed(int mouse_button) const noexcept -> bool;
    [[nodiscard]]
    auto is_mouse_button_down(int mouse_button) const noexcept -> bool;
    [[nodiscard]]
    auto is_mouse_button_released(int mouse_button) const noexcept -> bool;

    [[nodiscard]]
    auto mouse_pos() const noexcept -> const math::Vector2d&;

  private:
    std::array<ButtonFlag, GLFW_KEY_LAST + 1> m_key_states{};
    std::array<ButtonFlag, GLFW_MOUSE_BUTTON_LAST + 1> m_mouse_button_states{};
    math::Vector2d m_mouse_pos{};

    [[nodiscard]]
    static auto as_button_index(int key, int last) noexcept -> std::optional<std::size_t>;
    static auto update_button(ButtonFlag& state, int button, int action) noexcept -> void;
};

} // namespace states
