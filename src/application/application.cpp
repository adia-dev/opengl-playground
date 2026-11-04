#include "application/application.hpp"
#include "GLFW/glfw3.h"
#include "rendering/box.hpp"
#include "rendering/image.hpp"
#include "rendering/mesh.hpp"
#include "rendering/shader.hpp"
#include "rendering/shader_program.hpp"
#include "rendering/texture.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <format>
#include <glad/gl.h>
#include <print>
#include <stb_image.h>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace application {

Application::Application(const char* title, int window_width, int window_height)
    : m_window{glfwCreateWindow(window_width, window_height, title, nullptr, nullptr)} {
    if (m_window.get() == nullptr) {
        throw std::runtime_error("Failed to create the GLFW window.");
    }
}

void Application::run() {
    init();
    register_callbacks();

    m_scene_object.mesh().init();
    m_scene_object.mesh().update_vertices([](std::vector<rendering::Vertex>& vertices) -> void {
        const auto box = rendering::box_vertices();
        vertices.insert(vertices.end(), box.begin(), box.end());
    });

    m_last_time = glfwGetTime();
    while (glfwWindowShouldClose(m_window.get()) == 0) {
        handle_events();
        update();
        render();
    }
}

void Application::load_texture(const std::filesystem::path& texture_path) {
    int width, height, channels_in_file;
    stbi_set_flip_vertically_on_load(1);
    if (auto image =
            rendering::Image{
                stbi_load(texture_path.c_str(), &width, &height, &channels_in_file, STBI_rgb_alpha),
                &stbi_image_free};
        image != nullptr) {
        m_textures.emplace_back(
            rendering::Texture2D::from_image(std::move(image), width, height, STBI_rgb_alpha));
    } else {
        const char* reason = stbi_failure_reason();
        throw std::runtime_error(std::format(
            "Failed to load image at path: {}; reason: {}", texture_path.c_str(), reason));
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const std::filesystem::path resource_dir(RESOURCE_DIR);
    auto vertex = rendering::Shader::create(rendering::ShaderType::Vertex,
                                            std::string(resource_dir / "shaders" / "vertex.vert"));
    auto fragment = rendering::Shader::create(
        rendering::ShaderType::Fragment, std::string(resource_dir / "shaders" / "fragment.frag"));
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
    auto& program = m_program.emplace(rendering::ShaderProgram::create());
    if (auto result = m_program->link(vertex, fragment); !result) {
        throw std::runtime_error(
            std::format("Failed to link the shaders to the program, {}", result.error()));
    }
    m_program->use();

    load_texture(resource_dir / "textures" / "choso.png");
    // load_texture(resource_dir / "textures" / "bartz.png");

    if (std::cmp_greater(m_textures.size(), rendering::Texture2D::max_fragment_texture_units())) {
        throw std::runtime_error("Too many textures for the fragment shader.");
    }

    program.set_int(0, "Texture1");
    // program.set_int(1, "Texture2");
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

    glfwSetCursorPosCallback(
        m_window.get(), [](GLFWwindow* window, double xpos, double ypos) -> void {
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->m_input_state.update_mouse_pos({.x = xpos, .y = ypos});

            int w, h;
            glfwGetWindowSize(app->m_window.get(), &w, &h);
            auto x = static_cast<float>((2 * xpos / w) - 1.0);
            auto y = static_cast<float>(1.0 - (2 * ypos / h));
            app->m_program->set_vec("MousePos", x, y);
            std::println("x: {}, y: {}", x, y);
        });
}

auto Application::handle_events() noexcept -> void {
    m_input_state.before_frame();
    glfwPollEvents();
}

auto Application::update() -> void {
    double current_time = glfwGetTime();
    double delta_time = current_time - m_last_time;
    m_last_time = current_time;
    const float speed{2.5F};

    if (!m_program) {
        throw std::logic_error("Cannot update before the shader program is initialized.");
    }
    m_program->set_float(static_cast<float>(current_time), "t");

    if (m_input_state.is_key_down(GLFW_KEY_A)) {
        m_scene_object.move(-speed * static_cast<float>(delta_time), 0.0F, 0.0F);
    } else if (m_input_state.is_key_down(GLFW_KEY_D)) {
        m_scene_object.move(speed * static_cast<float>(delta_time), 0.0F, 0.0F);
    }

    if (m_input_state.is_key_down(GLFW_KEY_W)) {
        m_scene_object.move(0.0F, 0.0F, speed * static_cast<float>(delta_time));
    } else if (m_input_state.is_key_down(GLFW_KEY_S)) {
        m_scene_object.move(0.0F, 0.0F, -speed * static_cast<float>(delta_time));
    }

    static int axis{0};
    axis = (axis + static_cast<int>(m_input_state.is_key_pressed(GLFW_KEY_SPACE))) % 3;
    if (m_input_state.is_key_down(GLFW_KEY_LEFT) || m_input_state.is_key_down(GLFW_KEY_RIGHT)) {
        float direction = m_input_state.is_key_down(GLFW_KEY_LEFT) ? -1.F : 1.F;
        switch (axis) {
        case 0:
            m_scene_object.rotate(direction * speed * static_cast<float>(delta_time), 0.0F, 0.0F);
            break;
        case 1:
            m_scene_object.rotate(0.0F, direction * speed * static_cast<float>(delta_time), 0.0F);
            break;
        case 2:
            m_scene_object.rotate(0.0F, 0.0F, direction * speed * static_cast<float>(delta_time));
            break;
        default:
            break;
        }
    }

    if (m_input_state.is_key_down(GLFW_KEY_UP)) {
        m_scene_object.scale(speed * static_cast<float>(delta_time));
    } else if (m_input_state.is_key_down(GLFW_KEY_DOWN)) {
        m_scene_object.scale(-speed * static_cast<float>(delta_time));
    }

    if (m_input_state.is_mouse_button_pressed(GLFW_MOUSE_BUTTON_1)) {
        m_scene_object.mesh().update_vertices(
            [&](std::vector<rendering::Vertex>& vertices) -> void {
                auto mouse_pos = m_input_state.mouse_pos();
                int w, h;
                glfwGetWindowSize(m_window.get(), &w, &h);
                auto x = static_cast<float>((2 * mouse_pos.x / w) - 1.0);
                auto y = static_cast<float>(1.0 - (2 * mouse_pos.y / h));
                vertices.push_back({
                    .position = {.x = x, .y = y},
                });
            });
    }
}

auto Application::render() -> void {
    if (m_program) {
        (*m_program).use();
    }
    glClearColor(m_application_state.red_intensity, 0.1F, 0.2F, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (std::size_t i = 0; i < m_textures.size(); ++i) {
        const auto& texture = m_textures[i];
        if (texture) {
            texture->bind(static_cast<uint8_t>(i));
        }
    }
    auto model = m_scene_object.transform().to_model();
    m_program->set_mat4(model, "Model");
    m_scene_object.mesh().render();
    m_window.swap_buffers();
}

} // namespace application
