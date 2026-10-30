cmake_minimum_required(VERSION 3.24)

project(3d_balls LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_CLANG_TIDY "Run clang-tidy while compiling the application" OFF)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

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

add_library(stb_image STATIC third_party/stb/stb_image.c)
target_include_directories(stb_image PUBLIC third_party/stb)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
add_executable(3d_balls ${PROJECT_SOURCES})
target_include_directories(3d_balls PRIVATE src)
target_compile_definitions(3d_balls PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(3d_balls PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/resources")
target_link_libraries(3d_balls PRIVATE glfw glad stb_image)

include(CTest)
if(BUILD_TESTING)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    add_executable(input_state_tests tests/input_state_tests.cpp src/states/input_state.cpp)
    target_include_directories(input_state_tests PRIVATE src)
    target_compile_definitions(input_state_tests PRIVATE GLFW_INCLUDE_NONE)
    target_link_libraries(input_state_tests PRIVATE Catch2::Catch2WithMain glfw)
    include(Catch)
    catch_discover_tests(input_state_tests)
    add_executable(image_tests tests/image_tests.cpp)
    target_link_libraries(image_tests PRIVATE Catch2::Catch2WithMain stb_image)
    catch_discover_tests(image_tests)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(3d_balls PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
        -Wno-unused-parameter
    )
endif()

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy REQUIRED)
    set_property(TARGET 3d_balls PROPERTY CXX_CLANG_TIDY
        "${CLANG_TIDY_EXECUTABLE};--warnings-as-errors=*"
    )
endif()

if(ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(3d_balls PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(3d_balls PRIVATE -fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "ENABLE_SANITIZERS requires Clang or GCC")
    endif()
endif()

find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format
    HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin
)
if(CLANG_FORMAT_EXECUTABLE)
    execute_process(
        COMMAND "${CLANG_FORMAT_EXECUTABLE}" --version
        OUTPUT_VARIABLE CLANG_FORMAT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    file(GLOB_RECURSE PROJECT_FORMAT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hxx"
    )
    add_custom_target(format
        COMMAND "${CLANG_FORMAT_EXECUTABLE}" -i ${PROJECT_FORMAT_SOURCES}
        COMMENT "Formatting project C++ sources"
        VERBATIM
    )
    add_custom_target(format-check
        COMMAND "${CLANG_FORMAT_EXECUTABLE}" --dry-run --Werror ${PROJECT_FORMAT_SOURCES}
        COMMENT "Checking C++ formatting"
        VERBATIM
    )
else()
    message(STATUS "clang-format was not found; formatting targets are unavailable")
endif()

if(APPLE)
    target_link_libraries(3d_balls PRIVATE "-framework OpenGL")
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(3d_balls PRIVATE OpenGL::GL)
endif()
