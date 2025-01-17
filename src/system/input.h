#pragma once

#include <GLFW/glfw3.h>
#include <array>

// clang-format off
enum class Key
{
       up = GLFW_KEY_W,
     down = GLFW_KEY_S,
     left = GLFW_KEY_A,
    right = GLFW_KEY_D
};
// clang-format on

class Input
{
  public:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static bool key_pressed(Key key);

  private:
    static constexpr size_t KEYS_SIZE = 256;
    static std::array<bool, KEYS_SIZE> _keys;
};
