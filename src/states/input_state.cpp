#include "states/input_state.hpp"
#include "GLFW/glfw3.h"
#include "states/button_state.hpp"
#include <cstddef>
#include <optional>

namespace states {

auto InputState::before_frame() noexcept -> void {
    for (auto& state : m_key_states) {
        state = state & ~(ButtonFlag::Pressed | ButtonFlag::Released);
    }
    for (auto& state : m_mouse_button_states) {
        state = state & ~(ButtonFlag::Pressed | ButtonFlag::Released);
    }
}

auto InputState::update_key(int key, int action) noexcept -> void {
    if (auto key_index = as_button_index(key, GLFW_KEY_LAST); key_index) {
        update_button(m_key_states[*key_index], key, action);
    }
}

auto InputState::update_mouse_button(int button, int action) noexcept -> void {
    if (auto key_index = as_button_index(button, GLFW_MOUSE_BUTTON_LAST); key_index) {
        update_button(m_mouse_button_states[*key_index], button, action);
    }
}

auto InputState::update_mouse_pos(const math::Vector2d& mouse_pos) noexcept -> void {
    m_mouse_pos = mouse_pos;
}

[[nodiscard]]
auto InputState::mouse_pos() const noexcept -> const math::Vector2d& {
    return m_mouse_pos;
}

[[nodiscard]]
auto InputState::is_key_pressed(int key) const noexcept -> bool {
    if (auto i = as_button_index(key, GLFW_KEY_LAST); i) {
        return has_flag(m_key_states[*i], ButtonFlag::Pressed);
    }
    return false;
}

[[nodiscard]]
auto InputState::is_key_down(int key) const noexcept -> bool {
    if (auto i = as_button_index(key, GLFW_KEY_LAST); i) {
        return has_flag(m_key_states[*i], ButtonFlag::Down);
    }
    return false;
}

[[nodiscard]]
auto InputState::is_key_released(int key) const noexcept -> bool {
    if (auto i = as_button_index(key, GLFW_KEY_LAST); i) {
        return has_flag(m_key_states[*i], ButtonFlag::Released);
    }
    return false;
}

[[nodiscard]]
auto InputState::is_mouse_button_pressed(int mouse_button) const noexcept -> bool {
    if (auto i = as_button_index(mouse_button, GLFW_MOUSE_BUTTON_LAST); i) {
        return has_flag(m_mouse_button_states[*i], ButtonFlag::Pressed);
    }
    return false;
}

[[nodiscard]]
auto InputState::is_mouse_button_down(int mouse_button) const noexcept -> bool {
    if (auto i = as_button_index(mouse_button, GLFW_MOUSE_BUTTON_LAST); i) {
        return has_flag(m_mouse_button_states[*i], ButtonFlag::Down);
    }
    return false;
}

[[nodiscard]]
auto InputState::is_mouse_button_released(int mouse_button) const noexcept -> bool {
    if (auto i = as_button_index(mouse_button, GLFW_MOUSE_BUTTON_LAST); i) {
        return has_flag(m_mouse_button_states[*i], ButtonFlag::Released);
    }
    return false;
}

auto InputState::update_button(ButtonFlag& state, int button, int action) noexcept -> void {
    if (action == GLFW_PRESS) {
        if (!has_flag(state, ButtonFlag::Down)) {
            state = state | ButtonFlag::Pressed;
        }
        state = state | ButtonFlag::Down;
    } else if (action == GLFW_RELEASE) {
        state = (state | ButtonFlag::Released) & ~ButtonFlag::Down;
    }
}

auto InputState::as_button_index(int key, int last) noexcept -> std::optional<std::size_t> {
    if (key < 0 || key > last) {
        return std::nullopt;
    }
    return static_cast<std::size_t>(key);
}

} // namespace states
