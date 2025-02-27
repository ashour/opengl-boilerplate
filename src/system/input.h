#pragma once
#include "rendering/rendering.h"
#include <array>
#include <functional>

namespace eo
{
enum class Key
{
    tab = GLFW_KEY_TAB,
    escape = GLFW_KEY_ESCAPE,
    a = GLFW_KEY_A,
    d = GLFW_KEY_D,
    e = GLFW_KEY_E,
    q = GLFW_KEY_Q,
    s = GLFW_KEY_S,
    w = GLFW_KEY_W,
};

enum class MouseButton
{
    left = GLFW_MOUSE_BUTTON_LEFT,
    right = GLFW_MOUSE_BUTTON_RIGHT,
};

class Input
{
  public:
    Input() = delete;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static bool key_pressed(Key action);
    static bool key_just_pressed(Key action);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static bool mouse_button_pressed(MouseButton button);

    static void register_mouse_move_handler(std::function<void(glm::vec2, glm::vec2)>);
    static void mouse_callback(GLFWwindow* window, double x_position, double y_position);
    static glm::vec2 mouse_position() { return _current_mouse_position; }

  private:
    static constexpr size_t KEYS_SIZE = 1024;
    static std::array<bool, KEYS_SIZE> _keys_pressed;
    static std::array<bool, KEYS_SIZE> _keys_just_pressed;

    static constexpr size_t MOUSE_BUTTONS_SIZE = 8;
    static std::array<bool, MOUSE_BUTTONS_SIZE> _mouse_buttons;

    static std::function<void(glm::vec2, glm::vec2)> _on_mouse_moved;
    static bool _is_first_mouse_movement;
    static glm::vec2 _last_mouse_position;
    static glm::vec2 _current_mouse_position;
};
} // namespace eo
