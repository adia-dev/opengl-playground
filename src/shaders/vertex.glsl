#version 330 core

layout(location = 0) in vec2 aPosition;

out vec4 color;

void main() {
  gl_Position = vec4(aPosition, 0.0, 1.0);
}
