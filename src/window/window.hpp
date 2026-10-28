#pragma once

#include "GLFW/glfw3.h"
#include <memory>

struct Size final {
    int width;
    int height;
};

class Window {
  public:
    explicit Window(GLFWwindow* window) noexcept;

    [[nodiscard]] auto window_size() const noexcept -> Size;
    [[nodiscard]] auto framebuffer_size() const noexcept -> Size;
    [[nodiscard]] auto get() const noexcept -> GLFWwindow*;
    auto swap_buffers() const noexcept -> void;

    Window(const Window&) = delete;
    Window(Window&&) = default;
    auto operator=(const Window&) -> Window& = delete;
    auto operator=(Window&&) -> Window& = default;

  private:
    struct WindowDeleter final {
        void operator()(GLFWwindow* window);
    };
    using WindowPtr = std::unique_ptr<GLFWwindow, WindowDeleter>;
    WindowPtr handle_{nullptr};
};
