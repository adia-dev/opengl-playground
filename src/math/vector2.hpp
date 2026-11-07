#pragma once

namespace math {

template <typename T> struct Vector2 {
    T x, y;
};

template <typename T> struct Vector3 {
    T x, y, z;

    [[nodiscard]]
    friend auto operator+(const Vector3& lhs, const Vector3& rhs) noexcept -> Vector3 {
        return {
            .x = lhs.x + rhs.x,
            .y = lhs.y + rhs.y,
            .z = lhs.z + rhs.z,
        };
    }

    [[nodiscard]]
    constexpr static auto zero() noexcept -> Vector3 {
        return {};
    }
};

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector2d = Vector2<double>;

} // namespace math
