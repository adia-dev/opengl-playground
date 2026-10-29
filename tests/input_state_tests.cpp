#include "states/input_state.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("A key press and release are visible for one frame") {
    states::InputState input;
    constexpr int key = GLFW_KEY_A;

    input.update_key(key, GLFW_PRESS);
    CHECK(input.is_key_pressed(key));
    CHECK(input.is_key_down(key));
    CHECK_FALSE(input.is_key_released(key));

    input.before_frame();
    CHECK_FALSE(input.is_key_pressed(key));
    CHECK(input.is_key_down(key));

    input.update_key(key, GLFW_RELEASE);
    CHECK_FALSE(input.is_key_down(key));
    CHECK(input.is_key_released(key));
    input.before_frame();
    CHECK_FALSE(input.is_key_released(key));
}

TEST_CASE("Invalid key indices are ignored") {
    states::InputState input;
    for (int key : {GLFW_KEY_UNKNOWN, GLFW_KEY_LAST + 1}) {
        input.update_key(key, GLFW_PRESS);
        CHECK_FALSE(input.is_key_pressed(key));
        CHECK_FALSE(input.is_key_down(key));
        CHECK_FALSE(input.is_key_released(key));
    }
}

TEST_CASE("Repeating a held key does not create another press") {
    states::InputState input;
    input.update_key(GLFW_KEY_A, GLFW_PRESS);
    input.before_frame();
    input.update_key(GLFW_KEY_A, GLFW_REPEAT);
    CHECK(input.is_key_down(GLFW_KEY_A));
    CHECK_FALSE(input.is_key_pressed(GLFW_KEY_A));
}

TEST_CASE("Pressing and Releasing before the frame should hold both states") {
    states::InputState input;
    input.update_key(GLFW_KEY_A, GLFW_PRESS);
    CHECK(input.is_key_pressed(GLFW_KEY_A));
    CHECK(input.is_key_down(GLFW_KEY_A));

    input.update_key(GLFW_KEY_A, GLFW_RELEASE);
    CHECK(input.is_key_released(GLFW_KEY_A));
    CHECK(input.is_key_pressed(GLFW_KEY_A));
    CHECK_FALSE(input.is_key_down(GLFW_KEY_A));

    input.before_frame();
    CHECK_FALSE(input.is_key_down(GLFW_KEY_A));
    CHECK_FALSE(input.is_key_released(GLFW_KEY_A));
    CHECK_FALSE(input.is_key_pressed(GLFW_KEY_A));
}
