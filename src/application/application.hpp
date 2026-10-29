#pragma once

#include "rendering/mesh.hpp"
#include "rendering/shader_program.hpp"
#include "runtimes/glfw_runtime.hpp"
#include "states/application_state.hpp"
#include "states/input_state.hpp"
#include "window/window.hpp"

class Application final {
  public:
    Application(const char* title, int window_width, int window_height);
    ~Application();
    void run();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    auto operator=(const Application&) -> Application& = delete;
    auto operator=(Application&&) -> Application& = delete;

  private:
    auto init() -> void;
    auto register_callbacks() -> void;
    auto handle_events() noexcept -> void;
    auto update() noexcept -> void;
    auto render() const -> void;

  private:
    GlfwRuntime m_glfw_runtime;
    Window m_window;
    InputState m_input_state;
    ApplicationState m_application_state;
    Mesh m_mesh{{
        {.x = -0.5F, .y = -0.5F},
        {.x = 0.F, .y = 0.5F},
        {.x = 0.5F, .y = -0.5F},
    }};
    std::optional<ShaderProgram> m_program;

    double m_last_time{};
};
