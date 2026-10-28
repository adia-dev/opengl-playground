#include "application/application.hpp"
#include "GLFW/glfw3.h"
#include <glad/gl.h>
#include <print>
#include <stdexcept>

Application::Application(const char* title, int window_width, int window_height)
    : window_{glfwCreateWindow(window_width, window_height, title, nullptr, nullptr)} {
    if (window_.get() == nullptr) {
        throw std::runtime_error("Failed to create the GLFW window.");
    }
}

void Application::run() {
    init();
    register_callbacks();

    while (glfwWindowShouldClose(window_.get()) == 0) {
        handle_events();
        update();
        render();
    }
}

auto Application::init() -> void {
    glfwMakeContextCurrent(window_.get());
    if (gladLoadGL(glfwGetProcAddress) == 0) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }
    glfwSetWindowUserPointer(window_.get(), this);

    int w, h;
    glfwGetFramebufferSize(window_.get(), &w, &h);
    glViewport(0, 0, w, h);
}

auto Application::register_callbacks() -> void {
    glfwSetFramebufferSizeCallback(
        window_.get(),
        [](GLFWwindow* window, int w, int h) static -> void { glViewport(0, 0, w, h); });

    glfwSetKeyCallback(
        window_.get(),
        [](GLFWwindow* window, int key, int scancode, int action, int mods) static -> void {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->input_state_.update_key(key, action);
        });

    glfwSetMouseButtonCallback(
        window_.get(), [](GLFWwindow* window, int button, int action, int mods) static -> void {
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->input_state_.update_mouse_button(button, action);
        });
}

auto Application::handle_events() noexcept -> void {
    input_state_.before_frame();
    glfwPollEvents();
}

auto Application::update() noexcept -> void {
    if (input_state_.is_key_pressed(GLFW_KEY_LEFT)) {
        application_state_.red_intensity += 0.1F;
        std::println("PRESSED AHHHHH!");
    }
}

auto Application::render() const noexcept -> void {
    glClearColor(application_state_.red_intensity, 1.0, 0.2F, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    window_.swap_buffers();
}
