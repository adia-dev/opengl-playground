#include "rendering/texture.hpp"
#include <cstdint>
#include <format>
#include <glad/gl.h>
#include <print>
#include <stdexcept>
#include <utility>

namespace rendering {

Texture2D::Texture2D(unsigned int id, int width, int height, int bpp, bool loaded, Image image)
    : m_id(id), m_width(width), m_height(height), m_bpp(bpp), m_loaded(loaded),
      m_image(std::move(image)) {}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : m_id(std::exchange(other.m_id, 0)), m_width(other.m_width), m_height(other.m_height),
      m_bpp(other.m_bpp), m_loaded(std::exchange(other.m_loaded, false)),
      m_image(std::move(other.m_image)) {}

Texture2D::~Texture2D() {
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
    }
}

auto Texture2D::max_fragment_texture_units() -> int {
    static int result = -1;
    if (result != -1) {
        return result;
    }
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &result);
    return result;
}

auto Texture2D::from_image(Image image, int width, int height, int bpp) -> Texture2D {
    try {
        unsigned int id{};
        glGenTextures(1, &id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.get());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        return {id, width, height, bpp, true, std::move(image)};
    } catch (const std::exception& e) {
        std::println("Failed to load texture from image. {}", e.what());
        throw;
    }
}

auto Texture2D::bind(uint8_t unit) const -> void {
    if (auto max = max_fragment_texture_units(); std::cmp_greater_equal(unit, max)) {
        throw std::out_of_range(
            std::format("Texture unit {} is outside the valid range [0, {}).", unit, max));
    }

    if (!m_loaded) {
        throw std::runtime_error("Cannot bind a texture that hasn't been loaded.");
    }
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

auto Texture2D::unbind(uint8_t unit) const -> void {
    if (auto max = max_fragment_texture_units(); std::cmp_greater_equal(unit, max)) {
        throw std::out_of_range(
            std::format("Texture unit {} is outside the valid range [0, {}).", unit, max));
    }
    if (!m_loaded) {
        throw std::runtime_error("Cannot unbind a texture that hasn't been loaded.");
    }
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture2D::id() const noexcept -> unsigned int {
    return m_id;
}

auto Texture2D::width() const noexcept -> int {
    return m_width;
}

auto Texture2D::height() const noexcept -> int {
    return m_height;
}

auto Texture2D::pixels() const noexcept -> const Pixel* {
    return m_image.get();
}

} // namespace rendering
