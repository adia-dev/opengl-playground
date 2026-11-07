#pragma once

#include "rendering/vertex.hpp"
#include <functional>
#include <glad/gl.h>
#include <initializer_list>
#include <vector>

namespace rendering {

class Mesh final {
  public:
    Mesh() = default;
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) noexcept;
    auto operator=(const Mesh&) -> Mesh& = delete;
    auto operator=(Mesh&&) -> Mesh& = delete;

    auto add_vertex(const Vertex& vertex) -> void;
    auto add_vertices(std::initializer_list<Vertex> vertices) -> void;

    auto update_vertices(const std::function<void(std::vector<Vertex>&)>& fun) -> void;

    auto render() noexcept -> void;

    void init();

  private:
    std::vector<Vertex> m_vertices;
    GLuint m_vao{0}, m_vbo{0};
    bool m_dirty{false};

    void upload_data();
};

} // namespace rendering
