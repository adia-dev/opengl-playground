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

add_executable(3d_balls src/main.cpp)
target_link_libraries(3d_balls PRIVATE glfw glad)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(3d_balls PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
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

find_program(CLANG_FORMAT_23_EXECUTABLE
    NAMES clang-format
    HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin
)
if(CLANG_FORMAT_23_EXECUTABLE)
    execute_process(
        COMMAND "${CLANG_FORMAT_23_EXECUTABLE}" --version
        OUTPUT_VARIABLE CLANG_FORMAT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(CLANG_FORMAT_VERSION MATCHES "version ([0-9]+)")
        set(CLANG_FORMAT_MAJOR_VERSION "${CMAKE_MATCH_1}")
    else()
        set(CLANG_FORMAT_MAJOR_VERSION 0)
    endif()
endif()

if(CLANG_FORMAT_MAJOR_VERSION GREATER_EQUAL 23)
    add_custom_target(format
        COMMAND "${CLANG_FORMAT_23_EXECUTABLE}" -i "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
        COMMENT "Formatting project C++ sources"
        VERBATIM
    )
    add_custom_target(format-check
        COMMAND "${CLANG_FORMAT_23_EXECUTABLE}" --dry-run --Werror "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
        COMMENT "Checking C++ formatting"
        VERBATIM
    )
elseif(CLANG_FORMAT_23_EXECUTABLE)
    message(STATUS "clang-format 23+ is required for the project's formatting targets; found ${CLANG_FORMAT_VERSION}")
endif()

if(APPLE)
    target_link_libraries(3d_balls PRIVATE "-framework OpenGL")
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(3d_balls PRIVATE OpenGL::GL)
endif()
