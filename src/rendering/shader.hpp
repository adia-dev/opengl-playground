#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <glad/gl.h>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

enum class ShaderType : uint8_t { Vertex, Fragment };
struct ShaderError {
    std::string message;
    std::filesystem::path path;
};

class Shader final {
  public:
    Shader(Shader&& other) noexcept
        : m_id(std::exchange(other.m_id, 0)), m_type(other.m_type),
          m_shader_path(std::move(other.m_shader_path)) {}

    Shader(const Shader&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    auto operator=(Shader&&) -> Shader& = delete;

    [[nodiscard]]
    auto id() const noexcept -> GLuint {
        return m_id;
    }

    [[nodiscard]]
    auto type() const noexcept -> ShaderType {
        return m_type;
    }

    [[nodiscard]]
    static auto create(ShaderType type, const std::filesystem::path& shader_path) -> Shader {
        auto path = shader_path;
        GLenum gl_type{};
        switch (type) {
        case ShaderType::Vertex:
            gl_type = GL_VERTEX_SHADER;
            break;
        case ShaderType::Fragment:
            gl_type = GL_FRAGMENT_SHADER;
            break;
        default:
            throw std::invalid_argument("Unknown shader type");
        }

        const GLuint id = glCreateShader(gl_type);
        if (id == 0) {
            throw std::runtime_error("Could not create shader object");
        }

        return Shader{
            id,
            type,
            std::move(path),
        };
    }

    [[nodiscard]]
    auto compile() -> std::expected<void, ShaderError> {
        if (m_id == 0) {
            return std::unexpected(
                ShaderError("Attempted to compile an uninitialized shader.", m_shader_path));
        }

        std::ifstream file(m_shader_path, std::ios::binary);
        if (!file) {
            return std::unexpected(ShaderError("Could not open shader file.", m_shader_path));
        }
        const std::string source{std::istreambuf_iterator<char>{file},
                                 std::istreambuf_iterator<char>{}};
        if (file.bad()) {
            return std::unexpected(ShaderError("Could not read shader file.", m_shader_path));
        }

        const char* source_ptr = source.c_str();
        glShaderSource(m_id, 1, &source_ptr, nullptr);
        glCompileShader(m_id);
        if (auto error = get_shader_error(m_id, GL_COMPILE_STATUS); error) {
            return std::unexpected(ShaderError(std::move(*error), m_shader_path));
        }

        return {};
    }

    ~Shader() {
        glDeleteShader(m_id);
    }

  private:
    explicit Shader(GLuint id, ShaderType type, std::filesystem::path shader_path)
        : m_id(id), m_type(type), m_shader_path(std::move(shader_path)) {}
    GLuint m_id;
    ShaderType m_type;
    std::filesystem::path m_shader_path;

    auto get_shader_error(GLuint id, GLenum flag) -> std::optional<std::string> {
        int success{};
        glGetShaderiv(id, flag, &success);
        if (success == GL_TRUE) {
            return std::nullopt;
        }

        GLint length{};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        if (length <= 1) {
            std::string type_str;
            switch (m_type) {
            case ShaderType::Vertex:
                type_str = "Vertex";
                break;
            case ShaderType::Fragment:
                type_str = "Fragment";
                break;
            }

            return std::format(
                "{} shader({}) compilation failed without an information log.", type_str, m_id);
        }
        std::string log(static_cast<std::size_t>(length), '\0');
        GLsizei written{};
        glGetShaderInfoLog(id, length, &written, log.data());
        log.resize(static_cast<std::size_t>(written));
        return log;
    }
};
