# 3D Balls

A small C++23 + OpenGL starter. The first milestone is intentionally simple: open a window, create an OpenGL 3.3 Core context, and clear the screen each frame.

## Build and run (macOS)

Requirements: Xcode Command Line Tools, CMake 3.24+, and Ninja. GLFW is downloaded by CMake on the first configure. The generated GLAD 2.0.8 OpenGL 3.3 Core loader is included in `third_party/glad`.

```sh
cmake --preset debug
cmake --build --preset debug
./build/3d_balls
```

Close the window or press **Esc** to quit.

The first configure needs an internet connection to download GLFW 3.4. This project uses GLAD 2's `glad/gl.h` header and `gladLoadGL` initializer; LearnOpenGL examples may show GLAD 1's `glad/glad.h` and `gladLoadGLLoader` names. On macOS, OpenGL contexts are available up to 4.1 Core; this starter asks for 3.3 Core so its initial context works with a broad range of hardware. OpenGL is deprecated by Apple but remains useful for learning and for this project.
