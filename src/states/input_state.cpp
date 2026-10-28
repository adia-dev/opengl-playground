#include "states/input_state.hpp"

auto InputState::before_frame() noexcept -> void {
    for (auto& state : key_states_) {
        state = state & ~(ButtonFlag::Pressed | ButtonFlag::Released);
    }
}

auto InputState::update_key(int button, int action) noexcept -> void {
    if (auto key_index = as_key_index(button); key_index) {
        auto& state = key_states_[*key_index];
        if (action == GLFW_PRESS) {
            if (has_flag(state, ButtonFlag::Down)) {
                state = state | ButtonFlag::Pressed;
            }
            state = state | ButtonFlag::Down;
        } else if (action == GLFW_RELEASE) {
            state = (state | ButtonFlag::Released) & ~ButtonFlag::Down;
        }
    }
}

auto InputState::as_key_index(int key) -> std::optional<size_t> {
    if (key < 0 || key >= GLFW_KEY_LAST) {
        return std::nullopt;
    }
    return static_cast<size_t>(key);
}
