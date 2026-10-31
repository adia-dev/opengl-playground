#version 330 core

out vec4 FragColor;

in vec4 Color;
in vec2 TexCoords;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

uniform float t;

void main() {
  float a = sin(t) * 0.5 + 0.5;
  FragColor = mix(texture(Texture1, TexCoords), texture(Texture2, TexCoords), a) * Color;
}
