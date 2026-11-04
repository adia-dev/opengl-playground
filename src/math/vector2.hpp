#pragma once

namespace math {

template <typename T>
struct Vector2 {
    T x, y;
};

template <typename T>
struct Vector3 {
    T x, y, z;
};

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector2d = Vector2<double>;

} // namespace math
