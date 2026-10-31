#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <glad/gl.h>
#include <optional>
#include <string>

namespace rendering {

enum class ShaderType : uint8_t { Vertex, Fragment };
struct ShaderError {
    std::string message;
    std::filesystem::path path;
};

class Shader final {
  public:
    Shader(Shader&& other) noexcept;

    Shader(const Shader&) = delete;
    auto operator=(const Shader&) -> Shader& = delete;
    auto operator=(Shader&&) -> Shader& = delete;

    [[nodiscard]]
    auto id() const noexcept -> GLuint;

    [[nodiscard]]
    auto type() const noexcept -> ShaderType;

    [[nodiscard]]
    static auto create(ShaderType type, const std::filesystem::path& shader_path) -> Shader;

    [[nodiscard]]
    auto compile() -> std::expected<void, ShaderError>;

    ~Shader();

  private:
    explicit Shader(GLuint id, ShaderType type, std::filesystem::path shader_path);
    GLuint m_id;
    ShaderType m_type;
    std::filesystem::path m_shader_path;

    auto get_shader_error(GLuint id, GLenum flag) -> std::optional<std::string>;
};

} // namespace rendering
