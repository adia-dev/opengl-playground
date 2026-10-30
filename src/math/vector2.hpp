#pragma once

namespace math {

template <typename T>
struct Vector2 {
    T x, y;

    // TODO: Add a squared_length() const member returning x*x + y*y.
    // Compare squared distances to avoid a square root when checking proximity.
};

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

} // namespace math
