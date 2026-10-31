#include "application/application.hpp"
#include <exception>
#include <print>

auto main(int argc, char* argv[]) -> int {
    try {
        application::Application app{"OpenGL", 800, 600};
        app.run();
        std::println("~Goodbye~");
        return 0;
    } catch (const std::exception& exception) {
        std::println("Application exception: {}", exception.what());
        return -1;
    }
}
