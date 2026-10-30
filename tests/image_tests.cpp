#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <stb_image.h>

TEST_CASE("stb_image decodes an image from memory") {
    // A binary PPM containing one red pixel.
    constexpr unsigned char image[] = {'P', '6', '\n', '1', ' ', '1', '\n', '2', '5', '5', '\n',
                                       255, 0, 0};
    int width{}, height{}, channels{};
    std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> pixels{
        stbi_load_from_memory(image, sizeof(image), &width, &height, &channels, STBI_rgb_alpha),
        &stbi_image_free};

    REQUIRE(pixels != nullptr);
    CHECK(width == 1);
    CHECK(height == 1);
    CHECK(channels == 3); // Channels in the source, not the requested output.
    CHECK(pixels.get()[0] == 255);
    CHECK(pixels.get()[1] == 0);
    CHECK(pixels.get()[2] == 0);
    CHECK(pixels.get()[3] == 255);
}

// TODO: Add a test for invalid image bytes. Hint: stbi_load_from_memory returns
// nullptr on failure; inspect stbi_failure_reason() for a diagnostic.
