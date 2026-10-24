# 3D Balls

A small C++23 + OpenGL starter. The first milestone is intentionally simple: open a window, create an OpenGL 3.3 Core context, and clear the screen each frame.

## Build and run (macOS)

Requirements: Xcode Command Line Tools, CMake 3.24+, and Ninja. GLFW is downloaded by CMake on the first configure. The generated GLAD 2.0.8 OpenGL 3.3 Core loader is included in `third_party/glad`.

```sh
./run.sh
```

`run.sh` configures with the `debug` preset, builds the app, then launches it. Close the window or press **Esc** to quit.

## C++ workflow

- `.clang-format` sets the shared formatting style. Run `cmake --build --preset debug --target format` to format `src/main.cpp`, or use `format-check` to check it.
- The formatting rules use clang-format 23+'s argument thresholds: calls and function declarations with more than three arguments/parameters break one per line. On macOS, install it with `brew install llvm`; CMake prefers the Homebrew LLVM formatter.
- `.clang-tidy` enables bug, analyzer, performance, and selected modernization checks in clangd. Neovim already starts clangd with `--clang-tidy`.
- To run clang-tidy during compilation, install `clang-tidy` and configure with `cmake --preset debug -DENABLE_CLANG_TIDY=ON`.
- To enable runtime memory and undefined-behavior checks, configure with `cmake --preset debug -DENABLE_SANITIZERS=ON`, then build and run as usual.
- `.clangd` enables parameter-name and deduced-type inlay hints. In Neovim, toggle them for the current buffer with `<leader>ci`.

The first configure needs an internet connection to download GLFW 3.4. This project uses GLAD 2's `glad/gl.h` header and `gladLoadGL` initializer; LearnOpenGL examples may show GLAD 1's `glad/glad.h` and `gladLoadGLLoader` names. On macOS, OpenGL contexts are available up to 4.1 Core; this starter asks for 3.3 Core so its initial context works with a broad range of hardware. OpenGL is deprecated by Apple but remains useful for learning and for this project.
