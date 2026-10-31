#pragma once

#include <stb_image.h>
#include <cstdint>
#include <memory>

namespace rendering {
  using Pixel = uint8_t;
  using Image = std::unique_ptr<Pixel, decltype(&stbi_image_free)>;
}
