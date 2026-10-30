#pragma once

#include "rendering/vertex.hpp"
#include <cstddef>
#include <functional>
#include <glad/gl.h>
#include <vector>

namespace rendering {

class Mesh final {
  public:
    Mesh() = default;
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

    auto add_vertex(const Vertex& vertex) -> void {
        m_vertices.push_back(vertex);
        m_dirty = true;
    }

    auto update_vertices(const std::function<void(std::vector<Vertex>&)>& fun) {
        fun(m_vertices);
        m_dirty = true;
    }

    auto render() noexcept -> void {
        if (m_dirty) {
            upload_data();
            m_dirty = false;
        }
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
                     static_cast<GLsizeiptr>(m_vertices.size() * sizeof(Vertex)),
                     m_vertices.data(),
                     GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glVertexAttribPointer(1,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              reinterpret_cast<const void*>(offsetof(Vertex, color)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

  private:
    std::vector<Vertex> m_vertices;
    GLuint m_vao{0}, m_vbo{0};
    bool m_dirty{false};

    void upload_data() {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(m_vertices.size() * sizeof(Vertex)),
                     m_vertices.data(),
                     GL_DYNAMIC_DRAW);
    }
};

} // namespace rendering
