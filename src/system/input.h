#pragma once

#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>

// clang-format off
enum class Action
{
        quit_app = GLFW_KEY_ESCAPE,
    move_forward = GLFW_KEY_W,
       move_left = GLFW_KEY_A,
       move_back = GLFW_KEY_S,
      move_right = GLFW_KEY_D
};
// clang-format on

class Input
{
  public:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static bool action_pressed(Action action);

    static void register_mouse_move_handler(void (*on_mouse_moved)(glm::vec2, glm::vec2));
    static void mouse_callback(GLFWwindow* window, double x_position, double y_position);
    static glm::vec2 mouse_position() { return _current_mouse_position; }

  private:
    static constexpr size_t KEYS_SIZE = 1024;
    static std::array<bool, KEYS_SIZE> _keys;

    static void (*_on_mouse_moved)(glm::vec2, glm::vec2);
    static bool _is_first_mouse_movement;
    static glm::vec2 _last_mouse_position;
    static glm::vec2 _current_mouse_position;
};
