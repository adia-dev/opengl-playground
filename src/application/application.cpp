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
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
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

    m_scene_object.mesh.init();
    m_scene_object.mesh.update_vertices([](std::vector<rendering::Vertex>& vertices) -> void {
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
    update_framebuffer_size(w, h);
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

    if (std::cmp_greater(m_textures.size(), rendering::Texture2D::max_fragment_texture_units())) {
        throw std::runtime_error("Too many textures for the fragment shader.");
    }

    program.set_int(0, "Texture1");
}

auto Application::register_callbacks() -> void {
    glfwSetFramebufferSizeCallback(
        m_window.get(), [](GLFWwindow* window, int w, int h) static -> void {
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->update_framebuffer_size(w, h);
        });

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
        });
}

auto Application::update_framebuffer_size(int width, int height) -> void {
    glViewport(0, 0, width, height);
    m_has_drawable_framebuffer = width > 0 && height > 0;
    if (!m_has_drawable_framebuffer) {
        return;
    }

    constexpr float near_distance = 0.1F;
    constexpr float far_distance = 100.0F;
    const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    if (m_use_perspective_projection) {
        constexpr float fov_in_degrees = 45.0F;
        m_projection = glm::perspective(
            glm::radians(fov_in_degrees), aspect_ratio, near_distance, far_distance);
    } else {
        constexpr float half_view_height = 2.0F;
        const auto half_view_width = half_view_height * aspect_ratio;
        m_projection = glm::ortho(-half_view_width,
                                  half_view_width,
                                  -half_view_height,
                                  half_view_height,
                                  near_distance,
                                  far_distance);
    }
}

auto Application::handle_events() noexcept -> void {
    m_input_state.before_frame();
    glfwPollEvents();
}

auto Application::update() -> void {
    double current_time = glfwGetTime();
    auto delta_time = static_cast<float>(current_time - m_last_time);
    m_last_time = current_time;

    const float speed{2.5F};

    if (m_input_state.is_key_down(GLFW_KEY_A)) {
        m_scene_object.transform.translate(-speed * delta_time, 0.0F, 0.0F);
    } else if (m_input_state.is_key_down(GLFW_KEY_D)) {
        m_scene_object.transform.translate(speed * delta_time, 0.0F, 0.0F);
    }

    if (m_input_state.is_key_down(GLFW_KEY_W)) {
        m_scene_object.transform.translate(0.0F, 0.0F, speed * delta_time);
    } else if (m_input_state.is_key_down(GLFW_KEY_S)) {
        m_scene_object.transform.translate(0.0F, 0.0F, -speed * delta_time);
    }

    static int axis{0};
    axis = (axis + static_cast<int>(m_input_state.is_key_pressed(GLFW_KEY_SPACE))) % 3;
    if (m_input_state.is_key_down(GLFW_KEY_LEFT) || m_input_state.is_key_down(GLFW_KEY_RIGHT)) {
        float direction = m_input_state.is_key_down(GLFW_KEY_LEFT) ? -1.F : 1.F;
        switch (axis) {
        case 0:
            m_scene_object.transform.rotate(direction * speed * delta_time, 0.0F, 0.0F);
            break;
        case 1:
            m_scene_object.transform.rotate(0.0F, direction * speed * delta_time, 0.0F);
            break;
        case 2:
            m_scene_object.transform.rotate(0.0F, 0.0F, direction * speed * delta_time);
            break;
        default:
            break;
        }
    }

    if (m_input_state.is_key_down(GLFW_KEY_UP)) {
        m_scene_object.transform.adjust_scale(speed * delta_time);
    } else if (m_input_state.is_key_down(GLFW_KEY_DOWN)) {
        m_scene_object.transform.adjust_scale(-speed * delta_time);
    }

    if (m_input_state.is_key_pressed(GLFW_KEY_TAB)) {
        m_use_perspective_projection = !m_use_perspective_projection;
        int w, h;
        glfwGetFramebufferSize(m_window.get(), &w, &h);
        update_framebuffer_size(w, h);
    }

    // Rotate the camera in place, at one radian per second.
    // TODO: Replace the automatic rotation with camera controls of your choice.
    m_application_state.camera_transform.rotate(0.0F, delta_time, 0.0F);
}

auto Application::render() -> void {
    if (!m_has_drawable_framebuffer) {
        return;
    }
    if (m_program) {
        (*m_program).use();
    }
    glClearColor(m_application_state.red_intensity, 0.1F, 0.2F, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->set_mat4(m_projection, "Projection");

    const auto& camera = m_application_state.camera_transform;
    if (camera.scale.x == 0.0F || camera.scale.y == 0.0F || camera.scale.z == 0.0F) {
        throw std::logic_error("The camera transform cannot have a zero scale.");
    }
    auto view = glm::inverse(camera.to_model());
    m_program->set_mat4(view, "View");

    for (std::size_t i = 0; i < m_textures.size(); ++i) {
        m_textures[i].bind(static_cast<uint8_t>(i));
    }
    auto model = m_scene_object.transform.to_model();
    m_program->set_mat4(model, "Model");
    m_scene_object.mesh.render();
    m_window.swap_buffers();
}

} // namespace application
