#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 Model;

void main() {
  gl_Position = Model * vec4(aPosition, 0.0, 1.0);
  TexCoords = aTexCoords;
}
