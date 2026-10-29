#include "application/application.hpp"
#include "GLFW/glfw3.h"
#include <glad/gl.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
auto compile_shader(GLenum type, const char* source) -> GLuint {
    const GLuint shader = glCreateShader(type);
    if (shader == 0) {
        throw std::runtime_error("Failed to create shader.");
    }
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(static_cast<std::size_t>(length > 0 ? length : 1));
        glGetShaderInfoLog(shader, length, nullptr, log.data());
        const std::string message = std::string("Shader compilation failed: ") + log.data();
        glDeleteShader(shader);
        throw std::runtime_error(message);
    }
    return shader;
}
} // namespace

Application::Application(const char* title, int window_width, int window_height)
    : m_window{glfwCreateWindow(window_width, window_height, title, nullptr, nullptr)} {
    if (m_window.get() == nullptr) {
        throw std::runtime_error("Failed to create the GLFW window.");
    }
}

Application::~Application() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
    }
}

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

    constexpr auto vertex_src = R"(#version 330 core
layout(location = 0) in vec2 aPosition;
void main() {
    gl_Position = vec4(aPosition, 0.0, 1.0);
})";
    constexpr auto fragment_src = R"(#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
})";

    const GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_src);
    GLuint fragment = 0;
    try {
        fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_src);
        m_program = glCreateProgram();
        if (m_program == 0) {
            throw std::runtime_error("Failed to create shader program.");
        }
        glAttachShader(m_program, vertex);
        glAttachShader(m_program, fragment);
        glLinkProgram(m_program);
        GLint success = GL_FALSE;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            GLint length = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
            std::vector<char> log(static_cast<std::size_t>(length > 0 ? length : 1));
            glGetProgramInfoLog(m_program, length, nullptr, log.data());
            throw std::runtime_error(std::string("Shader linking failed: ") + log.data());
        }
    } catch (...) {
        glDeleteShader(vertex);
        if (fragment != 0) {
            glDeleteShader(fragment);
        }
        throw;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glUseProgram(m_program);
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
    double delta_time = current_time -  m_last_time;
    m_last_time = current_time;
    const float speed{0.1F};

    if (m_input_state.is_key_down(GLFW_KEY_LEFT)) {
        m_mesh.update_vertices([&](std::vector<Vec2>& vertices) -> void {
            for (auto& vertex : vertices) {
                vertex.x -= speed * static_cast<float>(delta_time);
            }
        });
    }
    if (m_input_state.is_key_down(GLFW_KEY_RIGHT)) {
        m_mesh.update_vertices([&](std::vector<Vec2>& vertices) -> void {
            for (auto& vertex : vertices) {
                vertex.x += speed * static_cast<float>(delta_time);
            }
        });
    }
    if (m_input_state.is_key_down(GLFW_KEY_UP)) {
        m_mesh.update_vertices([&](std::vector<Vec2>& vertices) -> void {
            for (auto& vertex : vertices) {
                vertex.y += speed * static_cast<float>(delta_time);
            }
        });
    }
    if (m_input_state.is_key_down(GLFW_KEY_DOWN)) {
        m_mesh.update_vertices([&](std::vector<Vec2>& vertices) -> void {
            for (auto& vertex : vertices) {
                vertex.y -= speed * static_cast<float>(delta_time);
            }
        });
    }
}

auto Application::render() const noexcept -> void {
    glClearColor(m_application_state.red_intensity, 1.0, 0.2F, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    m_mesh.render();
    m_window.swap_buffers();
}
