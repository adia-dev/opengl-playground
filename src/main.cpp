#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>

int main() {
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "3D Balls", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create an OpenGL window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    const int openGLVersion = gladLoadGL(glfwGetProcAddress);
    if (openGLVersion == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, [](GLFWwindow* current, int key, int, int action, int) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(current, GLFW_TRUE);
        }
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    });

    auto clear = []() {
        glClearColor(0.08F, 0.12F, 0.18F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        clear();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
