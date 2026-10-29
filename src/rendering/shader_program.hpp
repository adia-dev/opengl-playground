#pragma once

#include "rendering/mesh.hpp"
#include "rendering/shader.hpp"
#include <expected>
#include <format>
#include <stdexcept>
#include <string>
#include <utility>

class ShaderProgram {
  public:
    ShaderProgram(ShaderProgram&& other) noexcept
        : m_id(std::exchange(other.m_id, 0)), m_linked(std::exchange(other.m_linked, false)),
          m_link_failed(std::exchange(other.m_link_failed, false)) {}

    ShaderProgram(const ShaderProgram&) = delete;
    auto operator=(const ShaderProgram&) -> ShaderProgram& = delete;
    auto operator=(ShaderProgram&&) -> ShaderProgram& = delete;

    ~ShaderProgram() {
        glDeleteProgram(m_id);
    }

    [[nodiscard]]
    static auto create() -> ShaderProgram {
        GLuint program_id = glCreateProgram();
        if (program_id == 0) {
            throw std::runtime_error("Failed to create a shader program.");
        }

        return ShaderProgram{program_id};
    }

    auto use() const -> void {
        if (!m_linked) {
            throw std::runtime_error("Cannot use a program that hasn't been linked yet.");
        }
        glUseProgram(m_id);
    }

    [[nodiscard]]
    auto link(const Shader& vertex_shader, const Shader& fragment_shader)
        -> std::expected<void, std::string> {
        if (m_linked) {
            throw std::runtime_error("Unsupported, cannot relink a program for now.");
        }
        if (m_link_failed) {
            throw std::runtime_error("Unsupported, cannot relink a program that is dirty.");
        }
        if (vertex_shader.type() == fragment_shader.type()) {
            return std::unexpected("Unsupported operation: The shader program is only able to link "
                                   "two shaders of different type.");
        }
        glAttachShader(m_id, vertex_shader.id());
        glAttachShader(m_id, fragment_shader.id());
        glLinkProgram(m_id);

        int success{};
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (success == GL_TRUE) {
            m_linked = true;
            return {};
        }

        m_link_failed = true;

        int length{};
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);
        if (length <= 1) {
            return std::unexpected(
                std::format("Program({}) failed to link due to an error.", m_id));
        }

        std::string log(static_cast<size_t>(length), '\0');
        GLsizei written{};
        glGetProgramInfoLog(m_id, length, &written, log.data());
        log.resize(static_cast<size_t>(written));
        return std::unexpected(std::move(log));
    }

  private:
    explicit ShaderProgram(GLuint id) : m_id(id) {}
    GLuint m_id{0};
    bool m_linked{false}, m_link_failed{false};
};
