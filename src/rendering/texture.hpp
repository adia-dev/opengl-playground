#pragma once

#include "rendering/image.hpp"
#include <cstdint>

namespace rendering {
class Texture2D final {
  public:
    static auto max_fragment_texture_units() -> int;

    Texture2D(const Texture2D&) = delete;
    Texture2D(Texture2D&& other) noexcept;
    auto operator=(const Texture2D&) -> Texture2D& = delete;
    auto operator=(Texture2D&&) -> Texture2D& = delete;

    static auto from_image(Image image, int width, int height, int bpp) -> Texture2D;

    auto bind(uint8_t unit) const -> void;
    auto unbind(uint8_t unit) const -> void;

    [[nodiscard]]
    auto id() const noexcept -> unsigned int;

    [[nodiscard]]
    auto width() const noexcept -> int;

    [[nodiscard]]
    auto height() const noexcept -> int;

    [[nodiscard]]
    auto pixels() const noexcept -> const Pixel*;

    ~Texture2D();

  private:
    Texture2D(unsigned int id, int width, int height, int bpp, bool loaded, Image image);
    unsigned int m_id{0};
    int m_width, m_height, m_bpp;
    bool m_loaded{false};
    Image m_image;
};
} // namespace rendering
