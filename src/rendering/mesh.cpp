#include "rendering/mesh.hpp"
#include <cstddef>
#include <functional>
#include <glad/gl.h>
#include <initializer_list>
#include <vector>

namespace rendering {

Mesh::~Mesh() {
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
}

auto Mesh::add_vertex(const Vertex& vertex) -> void {
    m_vertices.push_back(vertex);
    m_dirty = true;
}

auto Mesh::add_vertices(std::initializer_list<Vertex> vertices) -> void {
    m_vertices.insert(m_vertices.end(), vertices);
    m_dirty = true;
}

auto Mesh::update_vertices(const std::function<void(std::vector<Vertex>&)>& fun) -> void {
    m_dirty = true;
    fun(m_vertices);
}

auto Mesh::render() noexcept -> void {
    if (m_dirty) {
        upload_data();
        m_dirty = false;
    }
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
}

void Mesh::init() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    upload_data();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // OpenGL interprets the pointer argument as an offset into the bound VBO.
    // NOLINTBEGIN(performance-no-int-to-ptr)
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          reinterpret_cast<const void*>(offsetof(Vertex, uv)));
    // NOLINTEND(performance-no-int-to-ptr)

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Mesh::upload_data() {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(m_vertices.size() * sizeof(Vertex)),
                 m_vertices.data(),
                 GL_DYNAMIC_DRAW);
}

} // namespace rendering
