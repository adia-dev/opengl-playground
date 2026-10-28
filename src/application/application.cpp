#include "application/application.hpp"
#include <glad/gl.h>
#include <stdexcept>

Application::Application(const char* title, int window_width, int window_height)
    : window_{glfwCreateWindow(window_width, window_height, title, nullptr, nullptr)} {
    if (window_.get() == nullptr) {
        throw std::runtime_error("Failed to create the GLFW window.");
    }
}

void Application::run() {
    init();

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
}

auto Application::handle_events() noexcept -> void {
    input_state_.before_frame();
    glfwPollEvents();
}

auto Application::update() noexcept -> void {}

auto Application::render() const noexcept -> void {
    glClearColor(0.3F, 1.0, 0.2F, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    window_.swap_buffers();
}
