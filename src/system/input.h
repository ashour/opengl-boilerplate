#pragma once

#include <GLFW/glfw3.h>
#include <array>

// clang-format off
enum class Action
{
    move_forward = GLFW_KEY_W,
       move_back = GLFW_KEY_S,
       move_left = GLFW_KEY_A,
      move_right = GLFW_KEY_D
};
// clang-format on

class Input
{
  public:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static bool action_pressed(Action action);

  private:
    static constexpr size_t KEYS_SIZE = 256;
    static std::array<bool, KEYS_SIZE> _keys;
};
