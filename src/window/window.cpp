#include "window/window.hpp"

Window::Window(GLFWwindow* window) noexcept : m_handle{window} {}

auto Window::window_size() const noexcept -> Size {
    Size size{};
    glfwGetWindowSize(m_handle.get(), &size.width, &size.height);
    return size;
}

auto Window::framebuffer_size() const noexcept -> Size {
    Size size{};
    glfwGetFramebufferSize(m_handle.get(), &size.width, &size.height);
    return size;
}

auto Window::get() const noexcept -> GLFWwindow* {
    return m_handle.get();
}
auto Window::swap_buffers() const noexcept -> void {
    glfwSwapBuffers(m_handle.get());
}

void Window::WindowDeleter::operator()(GLFWwindow* window) {
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
}
