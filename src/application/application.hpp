#pragma once

#include "rendering/scene_object.hpp"
#include "rendering/shader_program.hpp"
#include "rendering/texture.hpp"
#include "runtimes/glfw_runtime.hpp"
#include "states/application_state.hpp"
#include "states/input_state.hpp"
#include "window/window.hpp"
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <optional>
#include <vector>

namespace application {

class Application final {
  public:
    Application(const char* title, int window_width, int window_height);
    void run();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    auto operator=(const Application&) -> Application& = delete;
    auto operator=(Application&&) -> Application& = delete;

  private:
    void load_texture(const std::filesystem::path& texture_path);
    auto init() -> void;
    auto register_callbacks() -> void;
    auto update_framebuffer_size(int width, int height) -> void;
    auto handle_events() noexcept -> void;
    auto update() -> void;
    auto render() -> void;

  private:
    runtimes::GlfwRuntime m_glfw_runtime;
    window::Window m_window;
    states::InputState m_input_state;
    states::ApplicationState m_application_state;
    rendering::SceneObject m_scene_object;
    std::optional<rendering::ShaderProgram> m_program;
    std::vector<rendering::Texture2D> m_textures;

    glm::mat4 m_projection{1.0F};
    bool m_has_drawable_framebuffer{false};
    bool m_use_perspective_projection{false};

    double m_last_time{};
};

} // namespace application
