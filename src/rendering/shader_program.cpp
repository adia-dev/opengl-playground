#include "rendering/shader_program.hpp"
#include <cstddef>
#include <cstdio>
#include <expected>
#include <format>
#include <glad/gl.h>
#include <print>
#include <stdexcept>
#include <string>
#include <utility>

namespace rendering {

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : m_id(std::exchange(other.m_id, 0)), m_linked(std::exchange(other.m_linked, false)),
      m_link_failed(std::exchange(other.m_link_failed, false)),
      m_reported_missing_uniforms(std::move(other.m_reported_missing_uniforms)),
      m_cached_locations(std::move(other.m_cached_locations)) {}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_id);
}

auto ShaderProgram::create() -> ShaderProgram {
    GLuint program_id = glCreateProgram();
    if (program_id == 0) {
        throw std::runtime_error("Failed to create a shader program.");
    }

    return ShaderProgram{program_id};
}

auto ShaderProgram::use() const -> bool {
    if (!m_linked) {
        std::println(stderr, "Shader program {} cannot be used because it is not linked.", m_id);
        return false;
    }
    glUseProgram(m_id);
    return true;
}

auto ShaderProgram::set_int(int value, const std::string& name) const -> void {
    set_value(glUniform1i, name, value);
}

auto ShaderProgram::set_float(float value, const std::string& name) const -> void {
    set_value(glUniform1f, name, value);
}

auto ShaderProgram::link(const Shader& vertex_shader, const Shader& fragment_shader)
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
        return std::unexpected(std::format("Program({}) failed to link due to an error.", m_id));
    }

    std::string log(static_cast<std::size_t>(length), '\0');
    GLsizei written{};
    glGetProgramInfoLog(m_id, length, &written, log.data());
    log.resize(static_cast<std::size_t>(written));
    return std::unexpected(std::move(log));
}

ShaderProgram::ShaderProgram(GLuint id) : m_id(id) {}

} // namespace rendering
