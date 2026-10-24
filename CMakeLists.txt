cmake_minimum_required(VERSION 3.24)

project(3d_balls LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC third_party/glad/src/gl.c)
target_include_directories(glad PUBLIC third_party/glad/include)

add_executable(3d_balls src/main.cpp)
target_link_libraries(3d_balls PRIVATE glfw glad)

if(APPLE)
    target_link_libraries(3d_balls PRIVATE "-framework OpenGL")
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(3d_balls PRIVATE OpenGL::GL)
endif()
