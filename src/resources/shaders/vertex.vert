#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoords;

out vec4 Color;
out vec2 TexCoords;
uniform vec2 MousePos;
uniform float t;

void main() {
  float PI = 3.14159265359;
  float a = t;
  vec2 rotatedPosition = vec2(cos(a) * aPosition.x - sin(a) * aPosition.y, sin(a) * aPosition.x + cos(a) * aPosition.y);
  vec2 shearedPosition = vec2(aPosition.x + tan(a) * aPosition.y, aPosition.y);
  vec2 scaledPosition = vec2(aPosition.x * sin(a), aPosition.y * cos(a));

  // The object spins and scales locally, then moves away from the orbit center.
  vec2 offsetPosition = rotatedPosition * 0.3 + vec2(0.6, 0.0);
  vec2 orbitingPosition = vec2(
      cos(a) * offsetPosition.x - sin(a) * offsetPosition.y,
      sin(a) * offsetPosition.x + cos(a) * offsetPosition.y);

  // TODO: Try a different angle for the local spin, such as 2.0 * t.
  gl_Position = vec4(MousePos + orbitingPosition, 0.0, 1.0);
  Color = aColor;
  TexCoords = aTexCoords;
}
