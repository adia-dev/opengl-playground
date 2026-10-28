#include "application/application.hpp"
#include "GLFW/glfw3.h"
#include <glad/gl.h>
#include <print>
#include <stdexcept>

Application::Application(const char* title, int window_width, int window_height)
    : m_window{glfwCreateWindow(window_width, window_height, title, nullptr, nullptr)} {
    if (m_window.get() == nullptr) {
        throw std::runtime_error("Failed to create the GLFW window.");
    }
}

void Application::run() {
    init();
    register_callbacks();

    while (glfwWindowShouldClose(m_window.get()) == 0) {
        handle_events();
        update();
        render();
    }
}

auto Application::init() -> void {
    glfwMakeContextCurrent(m_window.get());
    if (gladLoadGL(glfwGetProcAddress) == 0) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }
    glfwSetWindowUserPointer(m_window.get(), this);

    int w, h;
    glfwGetFramebufferSize(m_window.get(), &w, &h);
    glViewport(0, 0, w, h);
}

auto Application::register_callbacks() -> void {
    glfwSetFramebufferSizeCallback(
        m_window.get(),
        [](GLFWwindow* window, int w, int h) static -> void { glViewport(0, 0, w, h); });

    glfwSetKeyCallback(
        m_window.get(),
        [](GLFWwindow* window, int key, int scancode, int action, int mods) static -> void {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->m_input_state.update_key(key, action);
        });

    glfwSetMouseButtonCallback(
        m_window.get(), [](GLFWwindow* window, int button, int action, int mods) static -> void {
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->m_input_state.update_mouse_button(button, action);
        });
}

auto Application::handle_events() noexcept -> void {
    m_input_state.before_frame();
    glfwPollEvents();
}

auto Application::update() noexcept -> void {
    if (m_input_state.is_key_pressed(GLFW_KEY_LEFT)) {
        m_application_state.red_intensity += 0.1F;
        std::println("PRESSED AHHHHH!");
    }
}

auto Application::render() const noexcept -> void {
    glClearColor(m_application_state.red_intensity, 1.0, 0.2F, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    m_window.swap_buffers();
}
