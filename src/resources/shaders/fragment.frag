#version 330 core

out vec4 FragColor;

in vec4 Color;
in vec2 TexCoords;

uniform sampler2D Texture1;
// uniform sampler2D Texture2;

uniform float t;

void main() {
  FragColor = texture(Texture1, TexCoords);
}
