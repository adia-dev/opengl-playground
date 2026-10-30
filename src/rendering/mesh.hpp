#pragma once

#include "math/vector2.hpp"
#include <functional>
#include <glad/gl.h>
#include <vector>

namespace rendering {

class Mesh final {
  public:
    Mesh(const std::vector<math::Vector2f>& vertices) : m_vertices(vertices) {}
    ~Mesh() {
        if (m_vbo != 0) {
            glDeleteBuffers(1, &m_vbo);
        }
        if (m_vao != 0) {
            glDeleteVertexArrays(1, &m_vao);
        }
    }

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;
    auto operator=(const Mesh&) -> Mesh& = delete;
    auto operator=(Mesh&&) -> Mesh& = delete;

    auto update_vertices(const std::function<void(std::vector<math::Vector2f>&)>& fun) {
      fun(m_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(m_vertices.size() * sizeof(math::Vector2f)),
                     m_vertices.data(),
                     GL_DYNAMIC_DRAW);
    }

    auto render() const noexcept -> void {
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, static_cast<GLsizei>(m_vertices.size()));
    }

    void init() {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizei>(m_vertices.size() * sizeof(math::Vector2f)),
                     m_vertices.data(),
                     GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(math::Vector2f), nullptr);
        glEnableVertexAttribArray(0);
    }

  private:
    std::vector<math::Vector2f> m_vertices;
    GLuint m_vao{0}, m_vbo{0};
};

} // namespace rendering
