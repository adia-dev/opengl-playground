#pragma once

#include <cstdint>
#include <utility>

enum class ButtonFlag : uint8_t {
    None = 0,
    Pressed = 1 << 1,
    Down = 1 << 2,
    Released = 1 << 3,
};

constexpr auto operator|(ButtonFlag lhs, ButtonFlag rhs) noexcept
    -> ButtonFlag {
    return static_cast<ButtonFlag>(
        std::to_underlying(lhs) | std::to_underlying(rhs)
    );
}

constexpr auto operator&(ButtonFlag lhs, ButtonFlag rhs) noexcept
    -> ButtonFlag {
    return static_cast<ButtonFlag>(
        std::to_underlying(lhs) & std::to_underlying(rhs)
    );
}

constexpr auto operator~(ButtonFlag flag) noexcept
    -> ButtonFlag {
    return static_cast<ButtonFlag>(~std::to_underlying(flag));
}

constexpr auto has_flag(ButtonFlag value, ButtonFlag flag) noexcept
    -> bool {
    return (value & flag) == flag;
}
