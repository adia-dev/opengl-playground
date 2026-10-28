#pragma once

#include "runtimes/glfw_runtime.hpp"
#include "states/input_state.hpp"
#include "states/application_state.hpp"
#include "window/window.hpp"

class Application final {
  public:
    Application(const char* title, int window_width, int window_height);
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
    auto render() const noexcept -> void;

  private:
    GlfwRuntime glfwRuntime_;
    Window window_;
    InputState input_state_;
    ApplicationState application_state_;
};
