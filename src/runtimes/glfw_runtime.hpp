#pragma once

struct GlfwRuntime {
    GlfwRuntime();
    ~GlfwRuntime();
    GlfwRuntime(const GlfwRuntime&) = delete;
    GlfwRuntime(GlfwRuntime&&) = delete;
    auto operator=(const GlfwRuntime&) -> GlfwRuntime& = delete;
    auto operator=(GlfwRuntime&&) -> GlfwRuntime& = delete;
};
