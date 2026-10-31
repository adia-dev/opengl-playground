#pragma once

#include "rendering/shader.hpp"
#include <cstdio>
#include <expected>
#include <glad/gl.h>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace rendering {

class ShaderProgram {
  public:
    ShaderProgram(ShaderProgram&& other) noexcept;

    ShaderProgram(const ShaderProgram&) = delete;
    auto operator=(const ShaderProgram&) -> ShaderProgram& = delete;
    auto operator=(ShaderProgram&&) -> ShaderProgram& = delete;

    ~ShaderProgram();

    [[nodiscard]]
    static auto create() -> ShaderProgram;

    auto use() const -> bool;

    auto set_int(int value, const std::string& name) const -> void;
    auto set_float(float value, const std::string& name) const -> void;

    [[nodiscard]]
    auto link(const Shader& vertex_shader, const Shader& fragment_shader)
        -> std::expected<void, std::string>;

  private:
    explicit ShaderProgram(GLuint id);
    GLuint m_id{0};
    bool m_linked{false}, m_link_failed{false};
    mutable std::unordered_set<std::string> m_reported_missing_uniforms;
    mutable std::unordered_map<std::string, int> m_cached_locations;

    template <typename T, typename... K>
    auto set_value(T glFun, const std::string& name, K... args) const -> void {
        if (!use()) {
            return;
        }
        if (auto it = m_cached_locations.find(name); it != m_cached_locations.end()) {
            glFun(it->second, args...);
            return;
        }

        int location = glGetUniformLocation(m_id, name.c_str());
        if (location == -1) {
            if (m_reported_missing_uniforms.insert(name).second) {
                std::println(stderr,
                             "Shader program {}: uniform '{}' was not found or is inactive.",
                             m_id,
                             name);
            }
            return;
        }
        m_cached_locations.insert_or_assign(name, location);
        glFun(location, args...);
    }
};

} // namespace rendering
