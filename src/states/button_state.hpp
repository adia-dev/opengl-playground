#pragma once

#include <cstdint>
#include <utility>

namespace states {

enum class ButtonFlag : uint8_t {
    None = 0,
    Pressed = 1U << 1U,
    Down = 1U << 2U,
    Released = 1U << 3U,
};

constexpr auto operator|(ButtonFlag lhs, ButtonFlag rhs) noexcept -> ButtonFlag {
    // A flag set can contain combinations that are not named enumerators.
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<ButtonFlag>(static_cast<unsigned>(std::to_underlying(lhs)) |
                                   static_cast<unsigned>(std::to_underlying(rhs)));
}

constexpr auto operator&(ButtonFlag lhs, ButtonFlag rhs) noexcept -> ButtonFlag {
    return static_cast<ButtonFlag>(static_cast<unsigned>(std::to_underlying(lhs)) &
                                   static_cast<unsigned>(std::to_underlying(rhs)));
}

constexpr auto operator~(ButtonFlag flag) noexcept -> ButtonFlag {
    // Narrow the complement back to the enum's fixed underlying type.
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange)
    return static_cast<ButtonFlag>(
        static_cast<uint8_t>(~static_cast<unsigned>(std::to_underlying(flag))));
}

constexpr auto has_flag(ButtonFlag value, ButtonFlag flag) noexcept -> bool {
    return (value & flag) == flag;
}

} // namespace states
