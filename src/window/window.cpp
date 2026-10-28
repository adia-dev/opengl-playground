#include "window/window.hpp"

Window::Window(GLFWwindow* window) noexcept : handle_{window} {}

auto Window::window_size() const noexcept -> Size {
    Size size{};
    glfwGetWindowSize(handle_.get(), &size.width, &size.height);
    return size;
}

auto Window::framebuffer_size() const noexcept -> Size {
    Size size{};
    glfwGetFramebufferSize(handle_.get(), &size.width, &size.height);
    return size;
}

auto Window::get() const noexcept -> GLFWwindow* {
    return handle_.get();
}
auto Window::swap_buffers() const noexcept -> void {
    glfwSwapBuffers(handle_.get());
}

void Window::WindowDeleter::operator()(GLFWwindow* window) {
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
}
