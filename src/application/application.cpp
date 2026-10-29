#include "application/application.hpp"
#include "GLFW/glfw3.h"
#include "rendering/mesh.hpp"
#include "rendering/shader.hpp"
#include "rendering/shader_program.hpp"
#include <filesystem>
#include <glad/gl.h>
#include <stdexcept>
#include <string>
#include <vector>

Application::Application(const char* title, int window_width, int window_height)
    : m_window{glfwCreateWindow(window_width, window_height, title, nullptr, nullptr)} {
    if (m_window.get() == nullptr) {
        throw std::runtime_error("Failed to create the GLFW window.");
    }
}

Application::~Application() {}

void Application::run() {
    init();
    register_callbacks();

    m_mesh.init();

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

    const std::filesystem::path resource_dir(RESOURCE_DIR);
    auto vertex =
        Shader::create(ShaderType::Vertex, std::string(resource_dir / "shaders" / "vertex.vert"));
    auto fragment = Shader::create(ShaderType::Fragment,
                                   std::string(resource_dir / "shaders" / "fragment.frag"));
    if (auto result = vertex.compile(); !result) {
        throw std::runtime_error(std::format("Failed to compile the vertex shader: {}; error: {}",
                                             result.error().path.c_str(),
                                             result.error().message));
    }
    if (auto result = fragment.compile(); !result) {
        throw std::runtime_error(std::format("Failed to compile the fragment shader: {}; error: {}",
                                             result.error().path.c_str(),
                                             result.error().message));
    }
    m_program.emplace(ShaderProgram::create());
    if (auto result = (*m_program).link(vertex, fragment); !result) {
        throw std::runtime_error(
            std::format("Failed to link the shaders to the program, {}", result.error()));
    }
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
    double current_time = glfwGetTime();
    double delta_time = current_time - m_last_time;
    m_last_time = current_time;
    const float speed{0.1F};

    if (m_input_state.is_key_down(GLFW_KEY_LEFT)) {
        m_mesh.update_vertices([&](std::vector<Vector2f>& vertices) -> void {
            for (auto& vertex : vertices) {
                vertex.x -= speed * static_cast<float>(delta_time);
            }
        });
    }
    if (m_input_state.is_key_down(GLFW_KEY_RIGHT)) {
        m_mesh.update_vertices([&](std::vector<Vector2f>& vertices) -> void {
            for (auto& vertex : vertices) {
                vertex.x += speed * static_cast<float>(delta_time);
            }
        });
    }
    if (m_input_state.is_key_down(GLFW_KEY_UP)) {
        m_mesh.update_vertices([&](std::vector<Vector2f>& vertices) -> void {
            for (auto& vertex : vertices) {
                vertex.y += speed * static_cast<float>(delta_time);
            }
        });
    }
    if (m_input_state.is_key_down(GLFW_KEY_DOWN)) {
        m_mesh.update_vertices([&](std::vector<Vector2f>& vertices) -> void {
            for (auto& vertex : vertices) {
                vertex.y -= speed * static_cast<float>(delta_time);
            }
        });
    }
}

auto Application::render() const -> void {
    if (m_program) {
        (*m_program).use();
    }
    glClearColor(m_application_state.red_intensity, 1.0, 0.2F, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    m_mesh.render();
    m_window.swap_buffers();
}
