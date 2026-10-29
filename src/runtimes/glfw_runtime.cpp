#include "runtimes/glfw_runtime.hpp"

#include "GLFW/glfw3.h"
#include <stdexcept>

namespace runtimes {

GlfwRuntime::GlfwRuntime() {
    if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
}

GlfwRuntime::~GlfwRuntime() {
    glfwTerminate();
}

} // namespace runtimes
