#include "application/application.hpp"
#include <cstdio>
#include <exception>
#include <print>

auto main(int argc, char* argv[]) -> int {
    try {
        application::Application app{"OpenGL", 800, 600};
        app.run();
        std::println("~Goodbye~");
        return 0;
    } catch (const std::exception& exception) {
        try {
            std::println(stderr, "Application exception: {}", exception.what());
        } catch (...) {
            std::fputs("Failed to report the application exception\n", stderr);
        }
        return -1;
    } catch (...) {
        std::fputs("Unknown application exception\n", stderr);
        return -1;
    }
}
