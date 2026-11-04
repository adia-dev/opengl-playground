# OpenGL Playground

A small C++23 project for learning OpenGL. Right now it renders a textured box you can move, rotate, and scale.

## Build and run

On macOS, install the Xcode Command Line Tools, CMake 3.24 or newer, and Ninja. The first build downloads GLFW, GLM, and Catch2, so it needs an internet connection.

```sh
./run.sh
```

Use A/D to move left and right, W/S to move along Z, and the arrow keys to rotate or scale. Press Space to switch the rotation axis and Escape to quit.

## Tests

```sh
cmake --preset debug
cmake --build --preset debug
ctest --test-dir build --output-on-failure
```
